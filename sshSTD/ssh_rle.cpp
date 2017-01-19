
#include "stdafx.h"
#include "ssh_compress.h"

namespace ssh
{
	Buffer RLE::compress(ssh_u size) noexcept
	{
		Buffer _out(size * 2); out = _out; out++;
		ssh_b* _in(in + size); *_in = 0xff;

		while(in < _in)
		{
			auto c1(*in++);
			auto c2(*in);
			auto c3(*(in + 1));
			int is1((c1 == c2) &(c2 == c3));
			auto ll(0);
			auto tmp(out++);
			*out++ = c1;
			while(in < _in)
			{
				c1 = *in++;
				c2 = *in;
				c3 = *(in + 1);
				int is2((c1 == c2) & (c2 == c3));
				ll++;
				if(is1 != is2 || ll > 126)
				{
					if(is1) in++, ll |= 0x80, out = tmp + 2; else ll--, in--;
					*tmp = ll;
					break;
				}
				*out++ = c1;
			}
			if(ll) { *tmp = ll; if(is1) *tmp |= 0x80; }
		}
		auto l(out - _out);
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
