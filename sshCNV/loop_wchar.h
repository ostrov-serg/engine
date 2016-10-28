
struct wchar_conv_struct
{
	struct conv_struct parent;
	int state;
};

static ssh_u wchar_id_loop_convert(ssh_cnv icd, const char* * inbuf, ssh_u *inbytesleft, char* * outbuf, ssh_u *outbytesleft)
{
	const wchar_t* inptr = (const wchar_t*)*inbuf;
	ssh_u inleft = *inbytesleft / sizeof(wchar_t);
	wchar_t* outptr = (wchar_t*)*outbuf;
	ssh_u outleft = *outbytesleft / sizeof(wchar_t);
	ssh_u count = (inleft <= outleft ? inleft : outleft);
	if(count > 0)
	{
		*inbytesleft -= count * sizeof(wchar_t);
		*outbytesleft -= count * sizeof(wchar_t);
		do
			*outptr++ = *inptr++;
		while(--count > 0);
		*inbuf = (const char*)inptr;
		*outbuf = (char*)outptr;
	}
	return 0;
}

static ssh_u wchar_id_loop_reset(ssh_cnv icd, char* * outbuf, ssh_u *outbytesleft)
{
	return 0;
}
