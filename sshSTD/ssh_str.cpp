
#include "stdafx.h"
#include "ssh_str.h"
//#include "ssh_buf.h"

namespace ssh
{
	ssh_cws String::_empty = L"\0\0\0\0\0\0\0\0\0";

	String::String(ssh_cws cws, ssh_l len)
	{
		init();
		if(cws)
		{
			ssh_l t(wcslen(cws));
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
			if(alloc(len, false))
			{
				MultiByteToWideChar(CP_ACP, 0, ccs, (int)len, buf, (int)len);
				data()->update();
			}
		}
	}

	String::String(ssh_ws ws, ssh_l rep)
	{
		init();
		if(alloc(rep, false))
		{
			_wcsset(buf, ws);
			data()->update();
		}
	}

	const String& String::add(ssh_cws wcs, ssh_l len)
	{
		ssh_l l(length());
		if(alloc(l + len, true))
		{
			memcpy(buf + l, wcs, len * 2);
			data()->update();
		}
		return *this;
	}

	const String& String::make(ssh_cws wcs, ssh_l len)
	{
		if(alloc(len, false))
		{
			memcpy(buf, wcs, len * 2);
			data()->update();
		}
		return *this;
	}

	bool String::alloc(ssh_l sz, bool is_copy)
	{
		if(sz)
		{
			STRING_BUFFER* buffer(data());
			ssh_l nsz(sz + 1);
			if(nsz > buffer->len_buf)
			{
				if(nsz < 8192) { nsz = ssh_pow2(nsz, true) * 2; if(nsz < 64) nsz = 64; }
				// выделим память под новый буфер
				buffer = (STRING_BUFFER*)new ssh_b[sizeof(STRING_BUFFER) + nsz * sizeof(ssh_ws)];
				// скопировать старый, если необходимо
				if(is_copy && !is_empty()) memcpy(buffer->data(), buf, length() * 2);
				// очистить старый
				empty();
				// инициализируем новый
				buffer->len_buf = (int)nsz;
				buf = buffer->data();
			}
			buffer->len = (int)sz;
			buf[sz] = 0;
			return true;
		}
		empty();
		return false;
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
		return String(buf + idx, len);
	}

	String String::add(ssh_cws wcs1, ssh_l len1, ssh_cws wcs2, ssh_l len2)
	{
		String ret(L'\0', len1 + len2);
		if(wcs1) memcpy(ret.buffer(), wcs1, len1 * 2);
		if(wcs2) memcpy(ret.buffer() + len1, wcs2, len2 * 2);
		return ret;
	}

	const String& String::replace(ssh_cws _old, ssh_cws _new)
	{
		ssh_l nOld(wcslen(_old)), nNew(wcslen(_new)), nLen(length()), nCount(0);
		ssh_l nDstOffs(0), nSrcOffs(0), nDiff, l;
		ssh_ws* f(buf);
		// расчитать новый размер
		while((f = wcsstr(f, _old))) nCount++, f += nOld;
		nDiff = nNew - nOld;
		if(nNew > nOld) nDstOffs = nDiff; else nSrcOffs = -nDiff;
		l = nDiff * nCount;
		// проверка на вместительность буфера
		if(alloc(nLen + l, true))
		{
			ssh_ws* _buf(buf);
			// непосредственно замена
			while((f = wcsstr(_buf, _old)))
			{
				l = (nLen - ((f + nSrcOffs) - buf));
				memmove(f + nDstOffs, f + nSrcOffs, l * 2);
				memcpy(f, _new, nNew * 2);
				_buf = f + nNew;
				nLen += nDiff;
			}
			//buf[nLen] = 0;
			data()->update();
		}
		return *this;
	}

	const String& String::replace(ssh_ws _old, ssh_ws _new)
	{
		ssh_ws* ptr(buf);
		while(*ptr) { if(*ptr == _old) *ptr = _new; ptr++; }
		data()->update();
		return *this;
	}

	const String& String::remove(ssh_cws wcs)
	{
		ssh_l nWcs(wcslen(wcs)), nLen(length());
		ssh_ws* f(buf);
		while((f = wcsstr(f, wcs))) { nLen -= nWcs; memcpy(f, f + nWcs, ((nLen - (f - buf)) + 1) * 2); }
		data()->len = (int)nLen;
		data()->update();
		return *this;
	}

	const String& String::remove(ssh_ws ws)
	{
		ssh_ws* ptr(buf), *rem(buf);
		while(*ptr) { if(*ptr != ws) *rem++ = *ptr; ptr++; }
		*rem = 0;
		data()->len -= (int)(ptr - rem);
		data()->update();
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
			memcpy(buf + idx, buf + ll, ((l - ll) + 1) * 2);
			data()->len -= (int)len;
			data()->update();
		}
		return *this;
	}

	const String& String::insert(ssh_l idx, ssh_cws wcs)
	{
		ssh_l len(length()), nWcs(wcslen(wcs));
		if(idx >= 0 && idx < len && alloc(len + nWcs, true))
		{
			memmove(buf + idx + nWcs, buf + idx, ((len - idx) + 1) * 2);
			memcpy(buf + idx, wcs, nWcs * 2);
			data()->update();
		}
		return *this;
	}

	const String& String::insert(ssh_l idx, ssh_ws ws)
	{
		ssh_l len(length());
		if(idx >= 0 && idx < len && alloc(len + 1, true))
		{
			memmove(buf + idx + 1, buf + idx, ((len - idx) + 1) * 2);
			buf[idx] = ws;
			data()->update();
		}
		return *this;
	}

	String String::fmt(ssh_cws pattern, ...)
	{
		va_list argList;
		va_start(argList, pattern);
		String result(fmt(pattern, argList));
		va_end(argList);
		return result;
	}

	String String::fmt(ssh_cws pattern, va_list argList)
	{
		String result(L'\0', _vscwprintf(pattern, argList));
		vswprintf(result.buf, result.length() + 1, pattern, argList);
		result.data()->update();
		return result;
	}

	const String& String::replace(ssh_cws* _old, ssh_cws _new)
	{
		ssh_l idx(0);
		while(_old[idx]) replace(_old[idx++], _new), _new += (wcslen(_new) + 1);
		return *this;
	}

	const String& String::load(ssh_u id)
	{
		HINSTANCE hInst(::GetModuleHandle(nullptr));
		HRSRC h(::FindResourceW(hInst, MAKEINTRESOURCE((((id >> 4) + 1) & static_cast<ssh_w>(~0))), RT_STRING));
		int len(::SizeofResource(hInst, h) / sizeof(ssh_ws));
		if(alloc(len, false))
		{
			::LoadString(hInst, (UINT)id, buf, len);
			data()->update();
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
		ssh_l len(length()), ln(wcslen(wcs));
		ssh_ws* _ws(buf);
		while(is_chars(_ws, wcs, ln)) { len--; _ws++; }
		memcpy(buf, _ws, (len + 1) * 2);
		data()->len = (int)len;
		data()->update();
		return *this;
	}

	const String& String::trim_right(ssh_cws wcs)
	{
		ssh_l len(length()), ln(wcslen(wcs));
		ssh_ws* _ws(buf + len - 1);
		while(is_chars(_ws, wcs, ln)) { len--; _ws--; }
		buf[len] = 0;
		data()->len = (int)len;
		data()->update();
		return *this;
	}
}
