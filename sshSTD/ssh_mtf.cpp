
#include "stdafx.h"
#include "ssh_compress.h"

namespace ssh
{
	Buffer MTF::transform(ssh_u size) noexcept
	{
		int i;
		ssh_b alphabit[512];
		ssh_memzero(alphabit, 512);
		// создать алфавит
		for(i = 0; i < size; i++)
		{
			auto l(in[i]);
			if(!alphabit[l + 256]) alphabit[sz_alphabit++] = l, alphabit[in[i] + 256] = 1;
		}
		// создать буфер
		Buffer _out(size + sz_alphabit + 2); out = _out;
		*(ssh_w*)out = sz_alphabit;
		out = (ssh_b*)ssh_memcpy(out + 2, alphabit, sz_alphabit);
		// закодировать
		for(i = 0; i < size; i++)
		{
			auto l(*in++);
			ssh_b idx(0);
			while(alphabit[idx] != l) { idx++; }
			memmove(&alphabit[1], &alphabit[0], idx);
			alphabit[0] = l;
			*out++ = idx;
		}
		return _out;
	}

	Buffer MTF::untransform(ssh_u size) noexcept
	{
		// прочитать алфавит и его размер
		sz_alphabit = *(ssh_w*)in;
		ssh_b* alphabit(in + 2);
		in += sz_alphabit + 2;
		// создать буфер
		size -= (sz_alphabit + 2);
		Buffer _out(size); out = _out;
		// раскодировать
		while(size--)
		{
			auto idx(*in++);
			auto s(alphabit[idx]);
			memmove(alphabit + 1, alphabit, idx);
			*out++ = s;
			*alphabit = s;
		}
		return _out;
	}
}
