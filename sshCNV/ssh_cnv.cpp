
#include "stdafx.h"

#define USE_DOS

struct loop_funcs
{
	ssh_u(*loop_convert) (ssh_cnv icd, ssh_ccs* inbuf, ssh_u* inbytesleft, char** outbuf, ssh_u* outbytesleft);
	ssh_u(*loop_reset) (ssh_cnv icd, char** outbuf, ssh_u* outbytesleft);
};

#include "converters.h"
#include "cjk_variants.h"
#include "translit.h"

struct encoding
{
	struct mbtowc_funcs ifuncs;
	struct wctomb_funcs ofuncs;
	int oflags;
};

enum
{
	#define DEFENCODING(xxx_names,xxx,xxx_ifuncs1,xxx_ifuncs2,xxx_ofuncs1,xxx_ofuncs2) ei_##xxx ,
	#include "encodings.def"
	#include "encodings_dos.def"
	#include "encodings_local.def"
	#undef DEFENCODING
	ei_for_broken_compilers_that_dont_like_trailing_commas
};

#include "flags.h"

static struct encoding const all_encodings[] =
{
	#define DEFENCODING(xxx_names,xxx,xxx_ifuncs1,xxx_ifuncs2,xxx_ofuncs1,xxx_ofuncs2) { xxx_ifuncs1,xxx_ifuncs2, xxx_ofuncs1,xxx_ofuncs2, ei_##xxx##_oflags },
	#include "encodings.def"
	#include "encodings_dos.def"
	#undef DEFENCODING
	#define DEFENCODING(xxx_names,xxx,xxx_ifuncs1,xxx_ifuncs2,xxx_ofuncs1,xxx_ofuncs2) { xxx_ifuncs1,xxx_ifuncs2, xxx_ofuncs1,xxx_ofuncs2, 0 },
	#include "encodings_local.def"
	#undef DEFENCODING
};

#include "loops.h"
#include "aliases.h"

struct stringpool2_t
{
	#define S(tag, name, encoding_index) wchar_t stringpool_##tag[sizeof(name)];
	#include "aliases_dos.h"
	#undef S
};

static const struct stringpool2_t stringpool2_contents =
{
	#define S(tag, name, encoding_index) name,
	#include "aliases_dos.h"
	#undef S
};

#define stringpool2 ((ssh_cws)&stringpool2_contents)

static const struct alias sysdep_aliases[] =
{
	#define S(tag, name, encoding_index) { (int)(ssh_l)&((struct stringpool2_t*)0)->stringpool_##tag, encoding_index },
	#include "aliases_dos.h"
	#undef S
};

static ssh_cws volatile charset_aliases;

static ssh_cws get_charset_aliases()
{
	ssh_cws cp(charset_aliases);
	if(!cp)
	{
		cp = L"CP936\0GBK\0CP1361\0JOHAB\0CP20127\0ASCII\0CP20866\0KOI8-R\0CP21866\0KOI8-RU\0CP28591\0ISO-8859-1\0CP28592\0ISO-8859-2\0CP28593\0ISO-8859-3\0CP28594\0ISO-8859-4\0"
			 L"CP28595\0ISO-8859-5\0CP28596\0ISO-8859-6\0CP28597\0ISO-8859-7\0CP28598\0ISO-8859-8\0CP28599\0ISO-8859-9\0CP28605\0ISO-8859-15\0";
		charset_aliases = cp;
	}
	return cp;
}

ssh_cws locale_charset()
{
	ssh_cws codeset;
	ssh_cws aliases;
	static ssh_ws buf[2 + 10 + 1];
	swprintf(buf, L"CP%u", GetACP());
	codeset = buf;
	if(codeset == NULL) codeset = L"";
	for(aliases = get_charset_aliases(); *aliases != L'\0'; aliases += wcslen(aliases) + 1, aliases += wcslen(aliases) + 1)
	{
		if(wcscmp(codeset, aliases) == 0 || (aliases[0] == L'*' && aliases[1] == L'\0'))
		{
			codeset = aliases + wcslen(aliases) + 1;
			break;
		}
	}
	if(codeset[0] == L'\0') codeset = L"ASCII";
	return codeset;
}

inline static unsigned int aliases_hash(ssh_cws str, ssh_i len)
{
	static const ssh_w asso_values[] =
	{
		850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850,
		850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 
		// 45
		12, 99, 850, 41, 2, 7, 6, 56, 4, 3, 74, 8, 17, 181, 850, 850, 850, 850, 850, 850, 
		// 65
		18, 172, 5, 18, 59, 123, 45, 100, 2, 227, 205, 135, 173, 4, 2, 20, 850, 5, 58, 20, 143, 324, 131, 164, 13,5,
		// 91
		850, 850, 850, 850, 49, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850,
		850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850, 850
	};
	register int hval = len;

	switch(hval)
	{
		default:
			hval += asso_values[str[10]];
			/*FALLTHROUGH*/
		case 10:
			hval += asso_values[str[9]];
			/*FALLTHROUGH*/
		case 9:
			hval += asso_values[str[8]];
			/*FALLTHROUGH*/
		case 8:
			hval += asso_values[str[7]];
			/*FALLTHROUGH*/
		case 7:
			hval += asso_values[str[6]];
			/*FALLTHROUGH*/
		case 6:
			hval += asso_values[str[5]];
			/*FALLTHROUGH*/
		case 5:
			hval += asso_values[str[4]];
			/*FALLTHROUGH*/
		case 4:
			hval += asso_values[str[3]];
			/*FALLTHROUGH*/
		case 3:
			hval += asso_values[str[2]];
			/*FALLTHROUGH*/
		case 2:
		case 1:
			hval += asso_values[str[0]];
			break;
	}
	hval += asso_values[str[len - 1]];
	return hval;//384
}

const struct alias* aliases_lookup(ssh_cws str, ssh_i len)
{
	if(len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
	{
		int key(aliases_hash(str, len));
		if(key <= MAX_HASH_VALUE && key >= 0)
		{
			int o(aliases[key].name / 2);
			if(o >= 0)
			{
				ssh_cws s(o + stringpool);
				if(*str == *s && !wcscmp(str + 1, s + 1))
					return &aliases[key];
			}
		}
	}
	return 0;
}

const struct alias* aliases2_lookup(ssh_cws str)
{
	const struct alias* ptr;
	ssh_i count;
	for(ptr = sysdep_aliases, count = sizeof(sysdep_aliases) / sizeof(sysdep_aliases[0]); count > 0; ptr++, count--)
	{
		if(!wcscmp(str, stringpool2 + (ptr->name / 2))) return ptr;
	}
	return NULL;
}

unsigned int parse_charset(ssh_cws str, int* idx)
{
	static ssh_ws buf[MAX_WORD_LENGTH + 10 + 1];
	ssh_cws cp;
	ssh_i count;
	ssh_pws bp;
	const struct alias* ap;
	int i;
	for(i = 0;;)
	{
		for(cp = str, bp = buf, count = MAX_WORD_LENGTH + 10 + 1;; cp++, bp++)
		{
			unsigned short c(*(unsigned short*)cp);
			if(c >= 0x80) return -1;
			if(c >= L'a' && c <= L'z') c -= L'a' - L'A';
			*bp = c;
			if(c == L'\0') break;
			if(--count == 0) return -1;
		}
		if(buf[0] == L'\0')
		{
			str = locale_charset();
			if(str[0] == L'\0') return -1;
			continue;
		}
		if(!(ap = aliases_lookup(buf, bp - buf)))
		{
			if(!(ap = aliases2_lookup(buf))) return -1;
		}
		if(ap->encoding_index == ei_local_char)
		{
			str = locale_charset();
			if(str[0] == L'\0') return -1;
			continue;
		}
		if(ap->encoding_index == ei_local_wchar_t) return -1;
		break;
	}
	*idx = i;
	return ap->encoding_index;
}

ssh_cnv cnv_open(ssh_cws tocode, ssh_cws fromcode)
{
	struct conv_struct* cd;
	ssh_i to_index, from_index;
	int to_wchar, from_wchar;

	if((to_index = parse_charset(tocode, &to_wchar)) == -1) return (ssh_cnv)-1;
	if((from_index = parse_charset(fromcode, &from_wchar)) == -1) return (ssh_cnv)-1;
	if(!(cd = (struct conv_struct*)malloc(from_wchar != to_wchar ? sizeof(struct wchar_conv_struct) : sizeof(struct conv_struct)))) return (ssh_cnv)(-1);
	cd->iindex = from_index;
	cd->ifuncs = all_encodings[from_index].ifuncs;
	cd->oindex = to_index;
	cd->ofuncs = all_encodings[to_index].ofuncs;
	cd->oflags = all_encodings[to_index].oflags;
	cd->lfuncs.loop_convert = unicode_loop_convert;
	cd->lfuncs.loop_reset = unicode_loop_reset;
	memset(&cd->istate, '\0', sizeof(state_t));
	memset(&cd->ostate, '\0', sizeof(state_t));
	cd->transliterate = 0;
	cd->discard_ilseq = 1;
	if(from_wchar != to_wchar)
	{
		struct wchar_conv_struct* wcd((struct wchar_conv_struct*)cd);
		memset(&wcd->state, '\0', sizeof(int));
	}
	return (ssh_cnv)cd;
}

ssh_u cnv_exec(ssh_cnv icd, ssh_ccs* inbuf, ssh_u *inbytesleft, char** outbuf, ssh_u* outbytesleft)
{
	conv_t cd((conv_t)icd);
	if(!inbuf || !*inbuf) return cd->lfuncs.loop_reset(icd, outbuf, outbytesleft); else return cd->lfuncs.loop_convert(icd, (ssh_ccs*)inbuf, inbytesleft, outbuf, outbytesleft);
}

int cnv_close(ssh_cnv icd)
{
	free((conv_t)icd);
	return 0;
}

int cnv_ctl(ssh_cnv icd, int request, void* argument)
{
	conv_t cd((conv_t)icd);
	switch(request)
	{
		case ICONV_TRIVIALP:
			*(int *)argument = ((cd->lfuncs.loop_convert == unicode_loop_convert && cd->iindex == cd->oindex) || cd->lfuncs.loop_convert == wchar_id_loop_convert ? 1 : 0);
			return 0;
		case ICONV_GET_TRANSLITERATE:
			*(int *)argument = cd->transliterate;
			return 0;
		case ICONV_SET_TRANSLITERATE:
			cd->transliterate = (*(const int *)argument ? 1 : 0);
			return 0;
		case ICONV_GET_DISCARD_ILSEQ:
			*(int *)argument = cd->discard_ilseq;
			return 0;
		case ICONV_SET_DISCARD_ILSEQ:
			cd->discard_ilseq = (*(const int *)argument ? 1 : 0);
			return 0;
		default:
			errno = EINVAL;
			return -1;
	}
}
