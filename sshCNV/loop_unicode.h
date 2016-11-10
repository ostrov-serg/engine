
static int unicode_transliterate(conv_t cd, ucs4_t wc, unsigned char* outptr, ssh_u outleft)
{
	if(cd->oflags & HAVE_HANGUL_JAMO)
	{
		/* Decompose Hangul into Jamo. Use double-width Jamo (contained
		   in all Korean encodings and ISO-2022-JP-2), not half-width Jamo
		   (contained in Unicode only). */
		ucs4_t buf[3];
		int ret = johab_hangul_decompose(cd, buf, wc);
		if(ret != RET_ILUNI)
		{
			/* we know 1 <= ret <= 3 */
			state_t backup_state = cd->ostate;
			unsigned char* backup_outptr = outptr;
			ssh_u backup_outleft = outleft;
			int i, sub_outcount;
			for(i = 0; i < ret; i++)
			{
				if(outleft == 0)
				{
					sub_outcount = RET_TOOSMALL;
					goto johab_hangul_failed;
				}
				sub_outcount = cd->ofuncs.xxx_wctomb(cd, outptr, buf[i], outleft);
				if(sub_outcount <= RET_ILUNI)
					goto johab_hangul_failed;
				if(!(sub_outcount <= outleft)) abort();
				outptr += sub_outcount; outleft -= sub_outcount;
			}
			return outptr - backup_outptr;
johab_hangul_failed:
			cd->ostate = backup_state;
			outptr = backup_outptr;
			outleft = backup_outleft;
			if(sub_outcount != RET_ILUNI)
				return RET_TOOSMALL;
		}
	}
  {
	  /* Try to use a variant, but postfix it with
		 U+303E IDEOGRAPHIC VARIATION INDICATOR
		 (cf. Ken Lunde's "CJKV information processing", p. 188). */
	  int indx = -1;
	  if(wc == 0x3006)
		  indx = 0;
	  else if(wc == 0x30f6)
		  indx = 1;
	  else if(wc >= 0x4e00 && wc < 0xa000)
		  indx = cjk_variants_indx[wc - 0x4e00];
	  if(indx >= 0)
	  {
		  for(;; indx++)
		  {
			  ucs4_t buf[2];
			  unsigned short variant = cjk_variants[indx];
			  unsigned short last = variant & 0x8000;
			  variant &= 0x7fff;
			  variant += 0x3000;
			  buf[0] = variant; buf[1] = 0x303e;
			  {
				  state_t backup_state = cd->ostate;
				  unsigned char* backup_outptr = outptr;
				  ssh_u backup_outleft = outleft;
				  int i, sub_outcount;
				  for(i = 0; i < 2; i++)
				  {
					  if(outleft == 0)
					  {
						  sub_outcount = RET_TOOSMALL;
						  goto variant_failed;
					  }
					  sub_outcount = cd->ofuncs.xxx_wctomb(cd, outptr, buf[i], outleft);
					  if(sub_outcount <= RET_ILUNI)
						  goto variant_failed;
					  if(!(sub_outcount <= outleft)) abort();
					  outptr += sub_outcount; outleft -= sub_outcount;
				  }
				  return outptr - backup_outptr;
variant_failed:
				  cd->ostate = backup_state;
				  outptr = backup_outptr;
				  outleft = backup_outleft;
				  if(sub_outcount != RET_ILUNI)
					  return RET_TOOSMALL;
			  }
			  if(last)
				  break;
		  }
	  }
  }
  if(wc >= 0x2018 && wc <= 0x201a)
  {
	  /* Special case for quotation marks 0x2018, 0x2019, 0x201a */
	  ucs4_t substitute =
		  (cd->oflags & HAVE_QUOTATION_MARKS
		  ? (wc == 0x201a ? 0x2018 : wc)
		  : (cd->oflags & HAVE_ACCENTS
		  ? (wc == 0x2019 ? 0x00b4 : 0x0060) /* use accents */
		  : 0x0027 /* use apostrophe */
		  ));
	  int outcount = cd->ofuncs.xxx_wctomb(cd, outptr, substitute, outleft);
	  if(outcount != RET_ILUNI)
		  return outcount;
  }
  {
	  /* Use the transliteration table. */
	  int indx = translit_index(wc);
	  if(indx >= 0)
	  {
		  const unsigned int * cp = &translit_data[indx];
		  unsigned int num = *cp++;
		  state_t backup_state = cd->ostate;
		  unsigned char* backup_outptr = outptr;
		  ssh_u backup_outleft = outleft;
		  unsigned int i;
		  int sub_outcount;
		  for(i = 0; i < num; i++)
		  {
			  if(outleft == 0)
			  {
				  sub_outcount = RET_TOOSMALL;
				  goto translit_failed;
			  }
			  sub_outcount = cd->ofuncs.xxx_wctomb(cd, outptr, cp[i], outleft);
			  if(sub_outcount == RET_ILUNI)
				  /* Recursive transliteration. */
				  sub_outcount = unicode_transliterate(cd, cp[i], outptr, outleft);
			  if(sub_outcount <= RET_ILUNI)
				  goto translit_failed;
			  if(!(sub_outcount <= outleft)) abort();
			  outptr += sub_outcount; outleft -= sub_outcount;
		  }
		  return outptr - backup_outptr;
translit_failed:
		  cd->ostate = backup_state;
		  outptr = backup_outptr;
		  outleft = backup_outleft;
		  if(sub_outcount != RET_ILUNI)
			  return RET_TOOSMALL;
	  }
  }
  return RET_ILUNI;
}

static void unicode_loop_convert(ssh_cnv icd, const ssh_b* inptr, ssh_u inleft, ssh_b* outptr)
{
	conv_t cd((conv_t)icd);
	while(inleft > 0)
	{
		ucs4_t wc;
		int incount(cd->ifuncs.xxx_mbtowc(cd, &wc, inptr, inleft));
		if(incount < 0)
		{
			if(incount == RET_ILSEQ)
			{
				// Case 1: invalid input
				if(cd->discard_ilseq)
				{
					switch(cd->iindex)
					{
						case ei_ucs4: case ei_ucs4be: case ei_ucs4le:
						case ei_utf32: case ei_utf32be: case ei_utf32le:
						case ei_ucs4internal: case ei_ucs4swapped: incount = 4; break;
						case ei_ucs2: case ei_ucs2be: case ei_ucs2le:
						case ei_utf16: case ei_utf16be: case ei_utf16le:
						case ei_ucs2internal: case ei_ucs2swapped: incount = 2; break;
						default: incount = 1; break;
					}
					goto outcount_zero;
				}
				return;
			}
			if(incount == RET_TOOFEW(0)) return;
			incount = -2 - incount;
		}
		else
		{
			int outcount(cd->ofuncs.xxx_wctomb(cd, outptr, wc, 8));
			if(outcount != RET_ILUNI) goto outcount_ok;
			if((wc >> 7) == (0xe0000 >> 7)) goto outcount_zero;
			if(cd->transliterate)
			{
				if((outcount = unicode_transliterate(cd, wc, outptr, 8)) != RET_ILUNI) goto outcount_ok;
			}
			if(cd->discard_ilseq) goto outcount_zero;
			if((outcount = cd->ofuncs.xxx_wctomb(cd, outptr, 0xFFFD, 8)) != RET_ILUNI) goto outcount_ok;
			return;
outcount_ok:
			if(outcount < 0) return;
			outptr += outcount;
		}
outcount_zero:
		if(incount > inleft) return;
		inptr += incount; inleft -= incount;
	}
}

static ssh_u unicode_loop_calc(ssh_cnv icd, const ssh_b* inptr, ssh_u inleft)
{
	conv_t cd((conv_t)icd);
	ssh_b outptr[8];
	int result(0);
	while(inleft > 0)
	{
		state_t last_istate(cd->istate);
		ucs4_t wc;
		int incount(cd->ifuncs.xxx_mbtowc(cd, &wc, inptr, inleft));
		if(incount < 0)
		{
			if(incount == RET_ILSEQ)
			{
				if(cd->discard_ilseq)
				{
					switch(cd->iindex)
					{
						case ei_ucs4: case ei_ucs4be: case ei_ucs4le:
						case ei_utf32: case ei_utf32be: case ei_utf32le:
						case ei_ucs4internal: case ei_ucs4swapped: incount = 4; break;
						case ei_ucs2: case ei_ucs2be: case ei_ucs2le:
						case ei_utf16: case ei_utf16be: case ei_utf16le:
						case ei_ucs2internal: case ei_ucs2swapped: incount = 2; break;
						default: incount = 1; break;
					}
					goto outcount_zero;
				}
				return 0;
			}
			if(incount == RET_TOOFEW(0)) return 0;
			incount = -2 - incount;
		}
		else
		{
			int outcount(cd->ofuncs.xxx_wctomb(cd, outptr, wc, 8));
			if(outcount != RET_ILUNI) goto outcount_ok;
			if((wc >> 7) == (0xe0000 >> 7)) goto outcount_zero;
			if(cd->transliterate)
			{
				if((outcount = unicode_transliterate(cd, wc, outptr, 8)) != RET_ILUNI) goto outcount_ok;
			}
			if(cd->discard_ilseq) goto outcount_zero;
			if((outcount = cd->ofuncs.xxx_wctomb(cd, outptr, 0xFFFD, 8)) != RET_ILUNI) goto outcount_ok;
			return 0;
outcount_ok:
			if(outcount < 0) return 0;
			result += outcount;
		}
outcount_zero:
		if(incount > inleft) return 0;
		inptr += incount; inleft -= incount;
	}
	return result;
}
