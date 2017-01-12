
#include "stdafx.h"
#include "ssh_str.h"

namespace ssh
{
	String::String(ssh_cws cws, ssh_l len)
	{
		init();
		if(cws)
		{
			ssh_l t(ssh_wcslen(cws));
			make(cws, (len < 0 || len >= t) ? t : len);
		}
	}

	String::String(ssh_ccs ccs, ssh_l len)
	{
		init();
		if(ccs)
		{
			ssh_l t(strlen(ccs));
			if(len < 0 || len >= t) len = t;
			*this = ssh_convert(cp_ansi, Buffer((ssh_b*)ccs, len, false), 0);
		}
	}

	String::String(ssh_ws ws, ssh_l rep)
	{
		init();
		auto buf(alloc(rep, false));
		while(rep-- > 0) { *buf++ = ws; }
		update();
	}

	const String& String::add(ssh_cws wcs, ssh_l len)
	{
		ssh_l l(length());
		ssh_memcpy(alloc(l + len, true) + l, wcs, len * 2);
		update();
		return *this;
	}

	const String& String::make(ssh_cws wcs, ssh_l len)
	{
		ssh_memcpy(alloc(len, false), wcs, len * 2);
		update();
		return *this;
	}

	ssh_ws* String::alloc(ssh_l sz, bool is_copy)
	{
		if(sz)
		{
			ssh_ws* _new(buffer());
			ssh_l nsz(sz + 1);
			if(nsz > _str.len_buf)
			{
				if(nsz < 8192) nsz = ssh_pow2(nsz * sizeof(ssh_ws), false);
				// выделим память под новый буфер
				_new = new ssh_ws[nsz];
				// скопировать старый, если необходимо
				if(is_copy && !is_empty()) memcpy(_new, str(), length() * 2);
				// стереть старый
				empty();
				// инициализируем новый
				_str.len_buf = (int)nsz;
				_str.ptr = _new;
			}
			_str.len = (int)sz;
			_new[sz] = 0;
			return _new;
		}
		empty();
		return _str.str;
	}

	String String::substr(ssh_l idx, ssh_l len) const
	{
		ssh_l l(length());
		if(idx <= l && idx >= 0)
		{
			if(len < 0) len = l;
			if((idx + len) > l) len = (l - idx);
		}
		else idx = len = 0;
		return String(str() + idx, len);
	}

	String String::add(ssh_cws wcs1, ssh_l len1, ssh_cws wcs2, ssh_l len2)
	{
		String ret(L'\0', len1 + len2);
		if(wcs1) ssh_memcpy(ret.buffer(), wcs1, len1 * 2);
		if(wcs2) ssh_memcpy(ret.buffer() + len1, wcs2, len2 * 2);
		return ret;
	}

	void String::_replace(ssh_cws _old, ssh_cws _new)
	{
		ssh_l nOld(ssh_wcslen(_old)), nNew(ssh_wcslen(_new)), nLen(length()), nCount(0);
		ssh_l nDstOffs(0), nSrcOffs(0), nDiff, l;
		ssh_ws* f(buffer()), *_buf(f), *buf;
		// расчитать новый размер
		while((f = ssh_wcsstr(f, _old))) nCount++, f += nOld;
		nDiff = nNew - nOld;
		if(nNew > nOld) nDstOffs = nDiff; else nSrcOffs = -nDiff;
		l = nDiff * nCount;
		// проверка на вместительность буфера
		if(l)
		{
			// запомнить значение buf[sz]
			ssh_ws _ws(_buf[nLen + l]);
			_buf = alloc(nLen + l, true);
			_buf[nLen + l] = _ws;
		}
		buf = _buf;
		// непосредственно замена
		while((f = ssh_wcsstr(_buf, _old)))
		{
			l = (nLen - ((f + nSrcOffs) - buf)) + 1;
			memmove(f + nDstOffs, f + nSrcOffs, l * 2);
			ssh_memcpy(f, _new, nNew * 2);
			_buf = f + nNew;
			nLen += nDiff;
		}
	}

	const String& String::replace(ssh_cws _old, ssh_cws _new)
	{
		_replace(_old, _new);
		update();
		return *this;
	}

	const String& String::replace(ssh_ws _old, ssh_ws _new)
	{
		ssh_ws* ptr(buffer());
		while(*ptr) { if(*ptr == _old) *ptr = _new; ptr++; }
		update();
		return *this;
	}

	const String& String::remove(ssh_cws wcs)
	{
		ssh_l nWcs(ssh_wcslen(wcs)), nLen(length());
		ssh_ws* f(buffer()), *_buf(f);
		while((f = ssh_wcsstr(f, wcs))) { nLen -= nWcs; memcpy(f, f + nWcs, ((nLen - (f - _buf)) + 1) * 2); }
		_str.len = (int)nLen;
		update();
		return *this;
	}

	const String& String::remove(ssh_ws ws)
	{
		ssh_ws* ptr(buffer()), *rem(ptr);
		while(*ptr) { if(*ptr != ws) *rem++ = *ptr; ptr++; }
		*rem = 0;
		_str.len -= (int)(ptr - rem);
		update();
		return *this;
	}

	const String& String::remove(ssh_l idx, ssh_l len)
	{
		ssh_l l(length());
		if(idx < l && idx >= 0)
		{
			if(len < 0) len = l;
			if((idx + len) > l) len = (l - idx);
			ssh_l ll(idx + len);
			ssh_ws* buf(buffer());
			ssh_memcpy(buf + idx, buf + ll, ((l - ll) + 1) * 2);
			_str.len -= (int)len;
			update();
		}
		return *this;
	}

	const String& String::insert(ssh_l idx, ssh_cws wcs)
	{
		ssh_l len(length()), nWcs(ssh_wcslen(wcs));
		if(idx >= 0 && idx < len && alloc(len + nWcs, true))
		{
			ssh_ws* _buf(buffer());
			memmove(_buf + idx + nWcs, _buf + idx, ((len - idx) + 1) * 2);
			ssh_memcpy(_buf + idx, wcs, nWcs * 2);
			update();
		}
		return *this;
	}

	const String& String::insert(ssh_l idx, ssh_ws ws)
	{
		ssh_l len(length());
		if(idx >= 0 && idx < len && alloc(len + 1, true))
		{
			ssh_ws* _buf(buffer());
			memmove(_buf + idx + 1, _buf + idx, ((len - idx) + 1) * 2);
			_buf[idx] = ws;
			update();
		}
		return *this;
	}

	const String& String::replace(ssh_cws* _old, ssh_cws _new)
	{
		ssh_l idx(0);
		while(_old[idx]) { _replace(_old[idx++], _new); _new += (ssh_wcslen(_new) + 1); }
		update();
		return *this;
	}

	const String& String::load(ssh_u id)
	{
		HINSTANCE hInst(::GetModuleHandle(nullptr));
		HRSRC h(::FindResourceW(hInst, MAKEINTRESOURCE((((id >> 4) + 1) & static_cast<ssh_w>(~0))), RT_STRING));
		int len(::SizeofResource(hInst, h) / sizeof(ssh_ws));
		if(alloc(len, false))
		{
			::LoadString(hInst, (UINT)id, buffer(), len);
			update();
		}
		return *this;
	}

	const bool is_chars(ssh_ws* ws, ssh_cws wcs, ssh_l ln)
	{
		for(ssh_l i = 0; i < ln; i++)
		{
			if(*ws == wcs[i]) return true;
		}
		return (*ws == L' ');
	}

	const String& String::trim_left(ssh_cws wcs)
	{
		ssh_l len(length()), ln(ssh_wcslen(wcs));
		ssh_ws* _ws(buffer()), *_buf(_ws);
		while(is_chars(_ws, wcs, ln)) { len--; _ws++; }
		ssh_memcpy(_buf, _ws, (len + 1) * 2);
		_str.len = (int)len;
		update();
		return *this;
	}

	const String& String::trim_right(ssh_cws wcs)
	{
		ssh_l len(length()), ln(ssh_wcslen(wcs));
		ssh_ws* _ws(buffer() + len - 1);
		while(is_chars(_ws, wcs, ln)) { len--; _ws--; }
		buffer()[len] = 0;
		_str.len = (int)len;
		update();
		return *this;
	}
}
