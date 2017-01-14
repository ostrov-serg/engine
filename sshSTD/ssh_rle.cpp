
#include "stdafx.h"
#include "ssh_compress.h"

namespace ssh
{
	Buffer RLE::compress(ssh_u size)
	{
		int l1, l2;
		Buffer out(size * 2);
		ssh_b* _out(out), *tmp, *_in(in + size);
		while(in < _in)
		{
			l1 = l2 = 1;
			tmp = _out++;
			//*_out++ = 0;
			while(in < _in)
			{
				auto c(*in++);
				if(c != *in)
				{
					*_out++ = c;
					if(l1 > 2)
					{
						*tmp = (l1 - 2) | 0x80;
						break;
					}
					else if(l2 > 127)
					{
						*tmp = l2 - 1;
						break;
					}
					l1 = 1;
					l2++;
					continue;
				}
				else
				{
					l1++;
					if(l1 > 2 && l2 > 1)
					{
						*tmp = l2 - 2;
						in -= l1 - 1;
						break;
					}
					else if(l1 > 128)
					{
						*tmp = (l1 - 2) | 0x80;
						*_out++ = c;
						break;
					}
				}
			}
		}

		return Buffer(out, 0, _out - out);
	}

	Buffer RLE::decompress(ssh_u size)
	{
		return Buffer();
	}
}
