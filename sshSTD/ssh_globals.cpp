
#include "stdafx.h"
#include "ssh_globals.h"
#include "ssh_rtti.h"
#include "ssh_map.h"
#include "ssh_file.h"
#include "ssh_regx.h"
#include "ssh_enums.h"

namespace ssh
{
	__cnv_open ssh_cnv_open(nullptr);
	__cnv_close ssh_cnv_close(nullptr);
	__cnv_make ssh_cnv_make(nullptr);
	__cnv_calc ssh_cnv_calc(nullptr);
	__regx_compile ssh_regx_compile(nullptr);
	__regx_exec ssh_regx_exec(nullptr);
	__regx_free ssh_regx_free(nullptr);

	Base* Base::root(nullptr);
	RTTI* RTTI::rootRTTI(nullptr);

	ssh_u SSH ssh_hash(ssh_cws wcs)
	{
		ssh_u _val(14695981039346656037ULL);
		while(*wcs)
		{
			_val ^= (ssh_u)*wcs++;
			_val *= (ssh_u)1099511628211ULL;
		}
		_val ^= _val >> 32;
		return _val;
	};

	ssh_u SSH ssh_rand(ssh_u begin, ssh_u end)
	{
		static ssh_u _genRnd(0);
		ssh_u tmp;
		bool is(false);
		if(ssh_system_value(SystemInfo::CPU_CAPS, CpuCaps::RDRAND)) is = (_rdrand64_step(&tmp) == 1);
		if(!is)
		{
			tmp = _genRnd;
			if(!tmp) tmp = _time64((__time64_t*)tmp);
			tmp *= 1103515245;
			_genRnd = tmp;
			tmp = ((tmp >> 16) & 0x7fff);
		}
		return begin + (tmp % ((end - begin) + 1));
	}

	Buffer SSH ssh_base64(const String& str)
	{
		ssh_u len_buf(0);
		return Buffer(asm_ssh_from_base64(str.buffer(), str.length(), &len_buf), len_buf, false);
	}

	String SSH ssh_base64(const Buffer& buf)
	{
		return String((ssh_cws)asm_ssh_to_base64(buf, buf.size()));
	}

	String SSH ssh_base64(ssh_cws charset, const String& str)
	{
		return ssh_base64(ssh_convert(charset, str));
	}

	String SSH ssh_md5(const String& str)
	{
		ssh_b md[16];
		String ret(L'\0', 32);
		ssh_ws* _ret(ret.buffer());
		MD5(ssh_convert(cp_ansi, str), str.length(), md);
		for(ssh_u i = 0; i < 16; i++) wsprintf(_ret + i * 2, L"%02x", md[i]);
		return ret;
	}

	Buffer SSH ssh_convert(ssh_cws charset, ssh_cws str)
	{
		ssh_cnv h;
		ssh_u in_c(wcslen(str) * 2);
		h = ssh_cnv_open(charset, cp_utf);
		Buffer out(ssh_cnv_calc(h, (ssh_b*)str, in_c));
		ssh_cnv_make(h, (ssh_b*)str, in_c, out);
		ssh_cnv_close(h);
		return out;
	}

	String SSH ssh_convert(ssh_cws charset, const Buffer& in, ssh_u offs)
	{
		ssh_cnv h;
		ssh_u in_c(in.size() - offs), out_c(0);
		h = ssh_cnv_open(cp_utf, charset);
		Buffer out(ssh_cnv_calc(h, (ssh_b*)in + offs, in_c));
		ssh_cnv_make(h, (ssh_b*)in + offs, in_c, out);
		ssh_cnv_close(h);
		return String(out.to<ssh_cws>(), out.size() / 2);
	}

	String SSH ssh_implode2(ssh_u val, const EnumReflector* stk, ssh_cws def, bool is_enum)
	{
		ssh_u idx;
		String _tmp;
		if(is_enum) _tmp = ((idx = stk->find(val)) == -1 ? def : stk->at(idx).name);
		else
		{
			for(idx = 0; idx < stk->count(); idx++)
			{
				if((val & stk->at(idx).value))
				{
					if(!_tmp.is_empty()) _tmp += L'|';
					_tmp += stk->at(idx).name;
				}
			}
			if(_tmp.is_empty()) _tmp = def;
		}
		return _tmp;
	}

	// добавить слеш на конец пути
	String SSH ssh_slash_path(const String& path)
	{
		return ((path.get(path.length() - 1) != L'\\') ? path + L'\\' : path);
	}
	// извлечь только имя файла
	String SSH ssh_file_title(const String& path)
	{
		ssh_l sl(wcsrchr((ssh_cws)path, L'\\') - (ssh_cws)path + 1);
		ssh_l pt(wcsrchr((ssh_cws)path, L'.') - (ssh_cws)path);
		return path.substr(sl, pt - sl);
	}
	// извлечь расширение файла
	String SSH ssh_file_ext(const String& path, bool is_pt)
	{
		ssh_l pt((wcsrchr((ssh_cws)path, L'.') - (ssh_cws)path) + !is_pt);
		return path.substr(pt);
	}
	// извлечь имя файла с расширением
	String SSH ssh_file_name(const String& path)
	{
		ssh_l sl(wcsrchr((ssh_cws)path, L'\\') - (ssh_cws)path + 1);
		return path.substr(sl);
	}
	// извлечь путь
	String SSH ssh_file_path(const String& path)
	{
		ssh_l sl(wcsrchr((ssh_cws)path, L'\\') - (ssh_cws)path + 1);
		return path.left(sl);
	}
	// извлечь путь c файлом
	String SSH ssh_file_path_title(const String& path)
	{
		ssh_l sl(wcsrchr((ssh_cws)path, L'.') - (ssh_cws)path);
		return path.left(sl);
	}
	// сгенерировать случайное имя
	String SSH ssh_gen_name(ssh_cws nm, bool is_long)
	{
		static ssh_u gen_count(0);
		gen_count++;
		return (is_long ? String::fmt(L"%s%I64X%016I64X", nm, gen_count, __rdtsc()) : String::fmt(L"%s%I64X", nm, __rdtsc()));
	}

	ssh_u SSH ssh_offset_line(const String& text, ssh_l ln)
	{
		ssh_cws _text(text);
		ssh_cws txt(_text);
		while(ln-- > 0)
		{
			if(!(_text = wcschr(_text, L'\n'))) return text.length();
			_text++;
		}
		return (_text - txt);
	}

	ssh_u SSH ssh_count_lines(const String & text)
	{
		ssh_u count(0);
		ssh_cws _text(text);
		while(!ssh_is_null(_text))
		{
			count++;
			if(!(_text = wcschr(_text, L'\n'))) break;
			_text++;
		}
		return count;
	}

	ssh_u SSH ssh_split(ssh_ws split, ssh_cws src, ssh_u* vec, ssh_u count)
	{
		ssh_u i(0);
		if(split && src && vec && count)
		{
			ssh_ws* _src((ssh_ws*)src), *esrc(_src + wcslen(src)), *_wcs;
			while(i < count && _src < esrc)
			{
				vec[i * 2 + 0] = (int)(_src - src);
				if(!(_wcs = wcschr(_src, split))) _wcs = esrc;
				vec[i * 2 + 1] = (int)(_wcs - _src);
				_src = _wcs + 1;
				i++;
			}
		}
		return i;
	}

	GUID SSH ssh_make_guid(ssh_cws src)
	{
		GUID _guid;
		if(!src) CoCreateGuid(&_guid);
		else
		{
			src += (src[0] == L'{' ? 1 : 0);
			ssh_ws* dst((ssh_ws*)src);
			_guid.Data1 = (ssh_d)_wcstoi64(dst, &dst, 16); dst++;
			_guid.Data2 = (ssh_w)_wcstoi64(dst, &dst, 16); dst++;
			_guid.Data3 = (ssh_w)_wcstoi64(dst, &dst, 16); dst++;
			ssh_w _w((ssh_w)_wcstoi64(dst, &dst, 16)); dst++; ssh_b* _wp((ssh_b*)&_w);
			ssh_u _u(_wcstoi64(dst, nullptr, 16)); ssh_b* _up((ssh_b*)&_u);
			_guid.Data4[0] = _wp[1]; _guid.Data4[1] = _wp[0];
			_guid.Data4[2] = _up[5]; _guid.Data4[3] = _up[4]; _guid.Data4[4] = _up[3];
			_guid.Data4[5] = _up[2]; _guid.Data4[6] = _up[1]; _guid.Data4[7] = _up[0];
		}
		return _guid;
	}

	void SSH ssh_make_path(ssh_cws path)
	{
		ssh_ws* _path((ssh_ws*)path);
		String dir(ssh_system_paths(SystemInfo::WORK_FOLDER));
		while((_path = wcschr(_path, L'\\')))
		{
			*_path = 0;
			if(!SetCurrentDirectory(path)) CreateDirectory(path, nullptr);
			*_path++ = L'\\';
		}
		SetCurrentDirectory(dir);
	}

	void SSH ssh_remove_comments(String * lst, ssh_u count, bool is_simple)
	{
		if(lst)
		{
			regx rx;
			rx.set_pattern(0, LR"((?mUs)/\*.*\*/)");
			rx.set_pattern(1, LR"((?m)\s*//.*[\r\n]*)");

			for(ssh_u i = 0; i < count; i++)
			{
				String path(lst[i]), npath(ssh_file_path_title(path) + L'+' + ssh_file_ext(path, true));
				File of(path, File::open_read);
				File nf(npath, File::create_write);
				String text(of.read(0, cp_ansi));
				if(is_simple) rx.replace(text, L"", 0, 1);
				rx.replace(text, L"", 0, 0);
				nf.write(text, cp_ansi);
			}
		}
	}

	bool SSH ssh_is_wrong_lex(const String& str, ssh_cws errLexs)
	{
		static ssh_cws wrong_lexem = L",#\"?&^%$:;\'|~<>/!{}[]";

		ssh_u l(str.length());
		ssh_cws err(errLexs ? errLexs : wrong_lexem);
		for(ssh_u i = 0; i < l; i++)
		{
			if(wcschr(err, str[i])) return true;
		}
		return false;
	}

	String SSH ssh_num_volume(ssh_u num)
	{
		ssh_cws suffix;
		double fNum((double)num);
		if(num > (1024 * 1024 * 1024))
		{
			suffix = L"ГБ";
			fNum /= 1073741824.0;
		}
		else if(num > (1024 * 1024))
		{
			suffix = L"МБ";
			fNum /= 1048576;
		}
		else if(num > 1024)
		{
			suffix = L"КБ";
			fNum /= 1024.0;
		}
		else
		{
			suffix = L"Б";
		}
		return String::fmt(L"%.4f %s", fNum, suffix);
	}

	String SSH ssh_path_in_range(const String& path, ssh_u range)
	{
		ssh_cws result(path);
		if((ssh_u)path.length() > range)
		{
			ssh_u strs[64];
			ssh_u pf(0), pl(ssh_split(L'\\', result, strs, 32) - 1), pll(pl);
			if(pl)
			{
				String resultF, resultL;
				if((strs[1] + strs[pl * 2 + 1]) <= range)
				{
					while(pf < pl)
					{
						String tmpF(resultF + String(result + strs[pf * 2], strs[pf * 2 + 1]) + L"\\"), tmpL(resultL);
						if(!tmpL.is_empty()) tmpL = L'\\' + tmpL;
						tmpL = String(result + strs[pl * 2], strs[pl * 2 + 1]) + tmpL;
						if((ssh_u)(tmpL.length() + tmpF.length() + 4) > range)
						{
							ssh_u ll(resultF.length() + tmpL.length());
							ssh_u lf(resultL.length() + tmpF.length());
							if(ll > range || lf > range) break;
							if(lf > ll) resultL = tmpL; else resultF = tmpF;
							break;
						}
						resultF = tmpF;
						resultL = tmpL;
						pl--;
						pf++;
					}
				}
				if(resultL.is_empty()) resultL = String(result + strs[pll * 2], strs[pll * 2 + 1]);
				return resultF + L"...\\" + resultL;
			}
		}
		return path;
	}

	String SSH ssh_make_guid(const GUID& guid)
	{
		ssh_ws buf[64];
		_swprintf_p(buf, 64, L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}", guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
		return buf;
	}

	String SSH ssh_translate(ssh_cws text, bool to_eng)
	{
		static ssh_cws rus1[] = {L"Ч", L"Ш", L"Э", L"А", L"Б", L"В", L"Г", L"Д", L"Е", L"Ё", L"Ж", L"З", L"И", L"Й", L"К", L"Л", L"М", L"Н", L"О", L"П", L"Р", L"С", L"Т", L"У", L"Ф", L"Х", L"Ц", L"Щ", L"Ю", L"Я", L"Ь", L"Ъ", L"Ы",
			L"ч", L"ш", L"э", L"а", L"б", L"в", L"г", L"д", L"е", L"ё", L"ж", L"з", L"и", L"й", L"к", L"л", L"м", L"н", L"о", L"п", L"р", L"с", L"т", L"у", L"ф", L"х", L"ц", L"щ", L"ю", L"я", L"ь", L"ъ", L"ы", nullptr};
		static ssh_cws eng1 = L"Ch\0Sh\0Je\0A\0B\0V\0G\0D\0E\0Jo\0Zh\0Z\0I\0J\0K\0L\0M\0N\0O\0P\0R\0S\0T\0U\0F\0H\0C\0W\0Yu\0Ya\0Q\0X\0Y\0ch\0sh\0je\0a\0b\0v\0g\0d\0e\0jo\0zh\0z\0i\0j\0k\0l\0m\0n\0o\0p\0r\0s\0t\0u\0f\0h\0c\0w\0yu\0ya\0q\0x\0y\0\0";

		static ssh_cws eng2[] = {L"Ch", L"Sh", L"Je", L"Yu", L"Ya", L"A", L"B", L"V", L"G", L"D", L"E", L"Jo", L"Zh", L"Z", L"I", L"J", L"K", L"L", L"M", L"N", L"O", L"P", L"R", L"S", L"T", L"U", L"F", L"H", L"C", L"W", L"Q", L"X", L"Y",
			L"ch", L"sh", L"je", L"yu", L"ya", L"a", L"b", L"v", L"g", L"d", L"e", L"jo", L"zh", L"z", L"i", L"j", L"k", L"l", L"m", L"n", L"o", L"p", L"r", L"s", L"t", L"u", L"f", L"h", L"c", L"w", L"q", L"x", L"y", nullptr};
		static ssh_cws rus2 = L"Ч\0Ш\0Э\0Ю\0Я\0А\0Б\0В\0Г\0Д\0Е\0Ё\0Ж\0З\0И\0Й\0К\0Л\0М\0Н\0О\0П\0Р\0С\0Т\0У\0Ф\0Х\0Ц\0Щ\0Ь\0Ъ\0Ы\0ч\0ш\0э\0ю\0я\0а\0б\0в\0г\0д\0е\0ё\0ж\0з\0и\0й\0к\0л\0м\0н\0о\0п\0р\0с\0т\0у\0ф\0х\0ц\0щ\0ь\0ъ\0ы\0\0";

		String txt(text);
		if(to_eng) return txt.replace(rus1, eng1);
		return txt.replace(eng2, rus2);
	}

	ssh_u SSH ssh_system_value(SystemInfo type, CpuCaps value)
	{
		// платформа
		static WindowsTypes platform(WindowsTypes::_UNK);
		// версия платформы
		static OSVERSIONINFOW osVersion;
		// статус памяти
		static MEMORYSTATUS memStatus;
		// скорость процессора
		static ssh_u cpuSpeed(0);
		// флаги процессора
		static ssh_u cpuCaps;

		if(!cpuSpeed)
		{
			memStatus.dwLength = sizeof(MEMORYSTATUS);
			GlobalMemoryStatus(&memStatus);

			osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
			if(GetVersionEx(&osVersion))
			{
				DWORD ver1 = osVersion.dwMajorVersion;
				DWORD ver2 = osVersion.dwMinorVersion;

				if(osVersion.dwPlatformId == VER_PLATFORM_WIN32_NT)
				{
					if(ver1 < 4) platform = WindowsTypes::_NT;
					else if(ver1 == 4) platform = WindowsTypes::_2K;
					else if(ver1 == 5) platform = WindowsTypes::_XP;
					else if(ver1 == 6)
					{
						switch(ver2)
						{
							case 0: platform = WindowsTypes::_VISTA; break;
							case 1: platform = WindowsTypes::_7; break;
							case 2: platform = WindowsTypes::_8; break;
							case 3: platform = WindowsTypes::_8_1; break;
						}
					}
					else if(ver1 == 10 && ver2 == 0) platform = WindowsTypes::_10;
				}
			}

			ssh_u startTime(__rdtsc());
			Sleep(100);
			ssh_u endTime(__rdtsc());
			cpuSpeed = (endTime - startTime) / 100;
			cpuCaps = asm_ssh_capability();
		}

		switch(type)
		{
			case SystemInfo::CPU_CAPS: return _bittest64((const ssh_l*)&cpuCaps, (ssh_u)value);
			case SystemInfo::PLATFORM: return (ssh_u)platform;
			case SystemInfo::TOTAL_MEMORY: return memStatus.dwAvailPhys + memStatus.dwAvailPageFile;
			case SystemInfo::PHYSICAL_MEMORY: return memStatus.dwTotalPhys;
			case SystemInfo::CPU_SPEED: return cpuSpeed;
		}
		return 0;
	}

	String SSH ssh_system_paths(SystemInfo type, int csidl)
	{
		ssh_ws tmp[MAX_PATH];
		ssh_d sz(MAX_PATH);
		String ret;
		switch(type)
		{
			case SystemInfo::PROG_FOLDER:
			case SystemInfo::PROG_NAME:
				::GetModuleFileName(::GetModuleHandle(nullptr), tmp, MAX_PATH);
				ret = (type == SystemInfo::PROG_FOLDER ? ssh_file_path(tmp) : ssh_file_title(tmp));
				break;
			case SystemInfo::WORK_FOLDER:
				::GetCurrentDirectory(MAX_PATH, tmp);
				ret = ssh_slash_path(tmp);
				break;
			case SystemInfo::TEMP_FOLDER:
				::GetTempPath(MAX_PATH, tmp);
				ret = ssh_slash_path(tmp);
				break;
			case SystemInfo::USER_FOLDER:
				if(SUCCEEDED(SHGetFolderPath(nullptr, CSIDL_APPDATA, nullptr, 0, tmp))) ret = ssh_slash_path(tmp);
				break;
			case SystemInfo::USER_NAME:
				if(GetUserName(tmp, &sz)) ret = tmp;
				break;
			case SystemInfo::COMP_NAME:
				if(GetComputerName(tmp, &sz)) ret = tmp;
				break;
			case SystemInfo::CUSTOM:
				if(SUCCEEDED(SHGetFolderPath(nullptr, csidl, nullptr, 0, tmp))) ret = ssh_slash_path(tmp);
		}
		return ret;
	}

	ssh_u SSH ssh_dll_proc(ssh_cws dll, ssh_ccs proc, ssh_cws suffix)
	{
		// хэндлы загруженных dll
		HMODULE hdll;
		static Map<HMODULE, String> dlls;

		String module(ssh_file_path_title(dll));
#ifdef _DEBUG
		if(suffix) module += suffix;
#endif
		module += (ssh_file_ext(dll, true));
		if(!(hdll = dlls[module]))
		{
			if(!(hdll = LoadLibrary(module))) return 0;
			dlls[module] = hdll;
		}
		return (ssh_u)GetProcAddress(hdll, proc);
	}

	static void ssh_init_libs()
	{
		// инициализировать функции стандартных библиотек - sshREGX, sshCNV
		ssh_cnv_open = (__cnv_open)ssh_dll_proc(L"sshCNV.dll", "cnv_open");
		ssh_cnv_close = (__cnv_close)ssh_dll_proc(L"sshCNV.dll", "cnv_close");
		ssh_cnv_make = (__cnv_make)ssh_dll_proc(L"sshCNV.dll", "cnv_make");
		ssh_cnv_calc = (__cnv_calc)ssh_dll_proc(L"sshCNV.dll", "cnv_calc");
		ssh_regx_compile = (__regx_compile)ssh_dll_proc(L"sshREGX.dll", "regx_compile");
		ssh_regx_exec = (__regx_exec)ssh_dll_proc(L"sshREGX.dll", "regx_exec");
		ssh_regx_free = (__regx_free)ssh_dll_proc(L"sshREGX.dll", "regx_free");
		// получить возможности процессора
		ssh_cws avx(ssh_system_value(SystemInfo::CPU_CAPS, CpuCaps::AVX) ? L"sshAVX" : L"sshSSE");
		// инициализировать процессорно-зависимые функции

	}

#define IDC_FOLDERTREE		0x3741
#define IDC_STATUSTEXT		0x3743
#define IDC_NEW_EDIT_PATH	0x3744

	static int CALLBACK SelectFolderCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM pData)
	{
		String* folder((String*)pData);

		switch(uMsg)
		{
			case BFFM_INITIALIZED:
				RECT rc;
				HWND hEdit;
				HFONT hFont;
				::ShowWindow(::GetDlgItem(hwnd, IDC_STATUSTEXT), SW_HIDE);
				::GetWindowRect(::GetDlgItem(hwnd, IDC_FOLDERTREE), &rc);
				rc.bottom = rc.top - 8;
				rc.top = rc.bottom - 23;
				::ScreenToClient(hwnd, (LPPOINT)&rc);
				::ScreenToClient(hwnd, ((LPPOINT)&rc) + 1);
				hEdit = ::CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hwnd, nullptr, nullptr, nullptr);
				::SetWindowLong(hEdit, GWL_ID, IDC_NEW_EDIT_PATH);
				::ShowWindow(hEdit, SW_SHOW);

				hFont = (HFONT)::SendMessage(hwnd, WM_GETFONT, 0, 0);
				::SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
				if(folder->is_empty()) *folder = ssh_system_paths(SystemInfo::WORK_FOLDER);
				::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)folder->buffer());
				break;
			case BFFM_SELCHANGED:
				HWND hDlg(::GetDlgItem(hwnd, IDC_NEW_EDIT_PATH));
				if(hDlg)
				{
					::SendMessage(hwnd, BFFM_ENABLEOK, 0, SHGetPathFromIDList((LPITEMIDLIST)lParam, folder->buffer()));
					::SetWindowText(hDlg, folder->buffer());
				}
				break;
		}

		return 0;
	}

	bool SSH ssh_dlg_sel_folder(ssh_cws title, String& folder, HWND hWnd)
	{
		bool result(false);
		BROWSEINFO m_bi;
		LPITEMIDLIST pidl;
		LPMALLOC pMalloc;
		Buffer tmp((ssh_b*)folder.buffer(), MAX_PATH, false);

		SSH_MEMZERO(&m_bi, sizeof(BROWSEINFO));

		m_bi.hwndOwner = hWnd;
		m_bi.pszDisplayName = nullptr;
		m_bi.pidlRoot = 0;
		m_bi.ulFlags = BIF_VALIDATE | BIF_STATUSTEXT | BIF_RETURNFSANCESTORS;
		m_bi.lpfn = SelectFolderCallbackProc;
		m_bi.lParam = (LPARAM)&tmp;
		m_bi.lpszTitle = title;

		if(SUCCEEDED(SHGetMalloc(&pMalloc)))
		{
			if((pidl = SHBrowseForFolder(&m_bi)))
			{
				pMalloc->Free(pidl);
				folder = ssh_slash_path(tmp.to<ssh_cws>());
				result = true;
			}
			pMalloc->Release();
		}

		return result;
	}

	bool SSH ssh_make_wnd(const DESC_WND& desc, bool is_show_wnd)
	{
		WNDCLASSEX wc;
		if(desc.className.is_empty()) desc.className = ssh_gen_name(L"");

		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = desc.stylesClass;
		wc.lpfnWndProc = desc.processingWnd;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = desc.hInst;
		wc.hIcon = (desc.icon ? desc.icon : LoadIcon(0, IDI_APPLICATION));
		wc.hCursor = (desc.cursor ? desc.cursor : LoadCursor(0, IDC_ARROW));
		wc.hbrBackground = desc.bkg ? desc.bkg : (HBRUSH)COLOR_WINDOW;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = desc.className;
		wc.hIconSm = desc.iconSm ? desc.iconSm : LoadIcon(0, IDI_APPLICATION);
		if(RegisterClassEx(&wc))
		{
			//if((desc.hWnd = CreateWindow(desc.className, desc.caption, desc.stylesWnd, desc.bar.x, desc.bar.y, desc.bar.w, desc.bar.h, desc.hWndParent, nullptr, desc.hInst, nullptr)))
			{
				if(is_show_wnd)
				{
					ShowWindow(desc.hWnd, SW_NORMAL);
					UpdateWindow(desc.hWnd);
				}
				return true;
			}
		}
		return false;
	}
	
	ssh_u ssh_dlg_save_or_open(bool bOpen, ssh_cws title, ssh_cws filter, ssh_cws ext, String & folder, HWND hWnd, String * arr, ssh_u count)
	{
		// filter = например, C++ source|*.cpp;
		ssh_u result(0);
		ssh_ws flt[256];
		ssh_u nBytesFiles(1), i(0);

		int flags(OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);

		OPENFILENAME ofn;
		SSH_MEMZERO(&ofn, sizeof(OPENFILENAME));

		while((flt[i++] = *filter))
		{
			if(*filter++ == L'|') flt[i - 1] = 0;
		}
		flt[i] = 0;
		if(arr)
		{
			flags |= OFN_ALLOWMULTISELECT;
			nBytesFiles = count;
		}
		if(bOpen) flags |= OFN_FILEMUSTEXIST;
		nBytesFiles *= MAX_PATH;

		Buffer files(nBytesFiles); ssh_ws* _files(files.to<ssh_ws*>());
		SSH_MEMZERO(files, nBytesFiles);

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrDefExt = ext;
		ofn.lpstrFilter = flt;
		ofn.lpstrInitialDir = folder;
		ofn.lpstrTitle = title;
		ofn.lpstrFile = _files;
		ofn.lpstrFileTitle = nullptr;
		ofn.nFilterIndex = 1;
		ofn.nMaxFile = (DWORD)nBytesFiles;
		ofn.Flags = flags;

		if((bOpen ? ::GetOpenFileName(&ofn) : ::GetSaveFileNameW(&ofn)))
		{
			folder = (ssh_cws)_files;
			result = 1;
			if(arr)
			{
				if(!_files[folder.length() + 1]) arr[0] = folder;
				else
				{
					folder = ssh_slash_path(folder);
					result--;
					while(result < count && *(_files += (wcslen(_files) + 1))) arr[result++] = folder + (ssh_cws)_files;
				}
			}
		}

		return result;
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch(ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			ssh::ssh_init_libs();
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}
