
#include "stdafx.h"
#include "ssh_compress.h"

namespace ssh
{
	Buffer RLE::compress(ssh_u size) noexcept
	{
		Buffer _out(size * 2); out = _out; out++;
		ssh_b* _in(in + size);

		while(in < _in)
		{
			auto c1(*in);
			auto c2(*++in);
			int is1(c1 == c2);
			auto l(0);
			auto tmp(out++);
			*out++ = c1;
			while(in < _in)
			{
				c1 = c2;
				c2 = *++in;
				int is2(c1 == c2);
				l++;
				if(is1 != is2 || l > 127)
				{
					l--;
					if(is1) l |= 0x80, out = tmp + 2; else in--;
					*tmp = l;
					break;
				}
				*out++ = c1;
			}
		}
		ssh_u l(out - _out);
		*_out = (ssh_b)((size / l) + 1);
		return Buffer(_out, 0, l);
	}

	Buffer RLE::decompress(ssh_u size) noexcept
	{
		Buffer _out(size * *in++); out = _out; size--;
		while(size--)
		{
			auto l(*in++);
			if(l & 0x80)
			{
				l = (l & 0x7f) + 2;
				memset(out, *in++, l);
				out += l;
				size--;
			}
			else
			{
				l++;
				out = (ssh_b*)ssh_memcpy(out, in, l);
				in += l;
				size -= l;
			}
		}
		return Buffer(_out, 0, out - _out);
	}
}
