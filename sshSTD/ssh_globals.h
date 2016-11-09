
#pragma once

extern "C"
{
	ssh_u	asm_ssh_capability();
	ssh_b* asm_ssh_to_base64(ssh_b* ptr, ssh_u count);
	ssh_b* asm_ssh_from_base64(ssh_ws* str, ssh_u count, ssh_u* len_buf);
	ssh_l	asm_ssh_parse_xml(ssh_ws* src, ssh_w* vec);
};

#include "ssh_str.h"
#include "ssh_buf.h"

namespace ssh
{
	//typedef ssh_cnv (CALLBACK* __cnv_open)(ssh_cws to, ssh_cws from);
	using __cnv_open = ssh_cnv (CALLBACK*)(ssh_cws to, ssh_cws from);
	typedef int (CALLBACK* __cnv_close)(void* h);
	typedef ssh_u (CALLBACK* __cnv_make)(void* cd, ssh_ccs* inbuf, size_t* inbytesleft, ssh_cs** outbuf, ssh_u* outbytesleft);
	typedef regex16* (CALLBACK* __regx_compile)(ssh_cws pattern, ssh_l options);
	typedef ssh_l (CALLBACK* __regx_exec)(const void* re, ssh_cws subj, ssh_l len_subj, ssh_l idx, ssh_l options, ssh_l* vec, ssh_l count_vec);
	typedef ssh_l (CALLBACK* __regx_free)(void* p);

	int SSH ssh_ext_count_array();
	ssh_u SSH ssh_ext_exec_hash(ssh_ccs str);
	String SSH ssh_ext_exec(ssh_ccs str);

	extern __cnv_open ssh_cnv_open;
	extern __cnv_close ssh_cnv_close;
	extern __cnv_make ssh_cnv_make;
	extern __regx_compile ssh_regx_compile;
	extern __regx_exec ssh_regx_exec;
	extern __regx_free ssh_regx_free;

	extern ssh_u SSH singltons[32];

	struct DESC_WND
	{
		//		DESC_WND() : bkg(0), icon(0), iconSm(0), cursor(0), processingWnd(nullptr), stylesClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS), stylesWnd(WS_OVERLAPPEDWINDOW), hWndParent(nullptr), hInst(::GetModuleHandle(nullptr)), bar(0, 0, 256, 256) {}
		// заголовок
		String caption;
		// курсор
		HCURSOR cursor;
		// иконка
		HICON icon;
		// малая иконка
		HICON iconSm;
		// фон
		HBRUSH bkg;
		// имя класса
		mutable String className;
		// стили класса
		UINT stylesClass;
		// стили окна
		UINT stylesWnd;
		// хэндл родительского окна
		HWND hWndParent;
		// хэндл окна
		mutable HWND hWnd;
		// хэндл модуля
		HINSTANCE hInst;
		// габариты окна
		//Bar<int> bar;
		// процедура обработки окна
		WNDPROC processingWnd;
	};

	// типы операционных систем 
	enum class WindowsTypes : ssh_w
	{
		_UNK, _NT, _2K, _XP, _VISTA, _7, _8, _8_1, _10
	};
	// возможности процессора
	enum class CpuCaps : ssh_u
	{
		NONE, MMX, SSE, SSE2, SSE3, SSSE3, SSE4_1, SSE4_2, PCLMULQDQ, FMA, CMPXCHG16B, MOVBE, POPCNT, AES, AVX, RDRAND, CMOV, BMI1, AVX2, BMI2, AVX512F, RDSEED, AVX512PF, AVX512ER, AVX512CD, HALF
	};

	enum class SystemInfo : ssh_w
	{
		PROG_FOLDER, WORK_FOLDER, TEMP_FOLDER, USER_FOLDER, PROG_NAME, USER_NAME, COMP_NAME, CUSTOM, PLATFORM, TOTAL_MEMORY, PHYSICAL_MEMORY, CPU_SPEED, CPU_CAPS
	};

	ssh_u SSH ssh_system_value(SystemInfo type, CpuCaps value);
	ssh_u SSH ssh_dll_proc(ssh_cws dll, ssh_ccs proc, ssh_cws suffix = L"d");
	ssh_u SSH ssh_hash(ssh_cws wcs);
	ssh_u SSH ssh_rand(ssh_u begin, ssh_u end);
	ssh_u SSH ssh_offset_line(const String& text, ssh_l ln);
	ssh_u SSH ssh_split(ssh_ws split, ssh_cws src, ssh_u* vec, ssh_u count_vec);
	ssh_u SSH ssh_rand(ssh_u begin, ssh_u end);
	String SSH ssh_system_paths(SystemInfo type, int csidl = CSIDL_LOCAL_APPDATA);
	String SSH ssh_slash_path(const String& path);
	String SSH ssh_file_ext(const String& path, bool is_pt = false);
	String SSH ssh_file_name(const String& path);
	String SSH ssh_file_title(const String& path);
	String SSH ssh_file_path(const String& path);
	String SSH ssh_file_path_title(const String& path);
	String SSH ssh_gen_name(ssh_cws nm, bool is_long = true);
	String SSH ssh_md5(const String& str);
	String SSH ssh_base64(ssh_cws to, const String& str);
	String SSH ssh_base64(const buffer& buf);
	String SSH ssh_make_params(ssh_cws fmt, ...);
	String SSH ssh_translate(ssh_cws text, bool to_eng);
	String SSH ssh_num_volume(ssh_u num);
	String SSH ssh_path_in_range(const String& path, ssh_u range);
	String SSH ssh_make_guid(const GUID& guid);
	String SSH ssh_convert(ssh_cws from, const buffer& in, ssh_u offs);
	GUID   SSH ssh_make_guid(ssh_cws src);
	buffer SSH ssh_base64(const String& str);
	buffer SSH ssh_convert(ssh_cws to, ssh_cws str);
	void SSH ssh_make_path(ssh_cws path, bool is_file);
	bool SSH ssh_is_null(ssh_cws str);
	bool SSH ssh_is_wrong_lex(const String& str, ssh_cws errLexs);
	bool SSH ssh_dlg_sel_folder(ssh_cws title, String& folder, HWND hWnd);
	bool SSH ssh_make_wnd(const DESC_WND& desc, bool is_show_wnd);
	ssh_u SSH ssh_dlg_save_or_open(bool bOpen, ssh_cws title, ssh_cws filter, ssh_cws ext, String& folder, HWND hWnd, String* arr, ssh_u count);
	// преобразовать значение в ближайшую степень двойки
	inline ssh_u ssh_pow2(ssh_u val, bool nearest)
	{
		ssh_d idx;
		_BitScanReverse64(&idx, val);
		ssh_u _val((1ULL << idx));
		return ((_val == val || nearest) ? _val : _val << 1ULL);
	}
	// проверяет на кратность значения степени двойки
	template <typename T> bool ssh_is_pow2(const T& value)
	{
		return (value == ssh_pow2<T>(value, true));
	}
	// обменять значения
	template <class T> void ssh_swap(T& val1, T& val2)
	{
		T tmp(val1); val1 = val2; val2 = tmp;
	}
	// минимум
	template <class T> T ssh_min(const T& val1, const T& val2)
	{
		return (val1 < val2 ? val1 : val2);
	}
	// максимум
	template <class T> T ssh_max(const T& val1, const T& val2)
	{
		return (val1 > val2 ? val1 : val2);
	}
	// разбить строку на элементы
	template <typename T> T* ssh_explode(ssh_cws split, const String& src, T* dst, ssh_u count_dst, const T& def, ENUM_DATA* stk = nullptr, bool is_hex = false)
	{
		ssh_ws* _wcs(src.buffer()), *t;
		ssh_u i(0), j(wcslen(split));
		T tmp;
		while(i < count_dst && *_wcs)
		{
			if((t = wcsstr(_wcs, split))) *t = 0;
			if(stk) tmp = (T)ssh_cnv_value(_wcs, stk, (ssh_u)def);
			else if(is_hex) tmp = String(_wcs).toNum<T>(0, String::_hex);
			else tmp = String(_wcs);
			dst[i] = tmp;
			if(t) { *t = *split; _wcs = t + j; }
			i++;
		}
		// заполняем значениями по умолчанию
		for(; i < count_dst; i++) dst[i] = def;
		return dst;
	}
	// соеденить элементы в строку
	template <typename T> String ssh_implode(ssh_wcs split, T* src, ssh_u count_src, ssh_wcs def, ENUM_DATA* stk = nullptr, bool is_enum = true, bool is_hex = false)
	{
		String ret, _tmp;

		for(ssh_u i = 0; i < count_src; i++)
		{
			T tmp(src[i]);
			if(stk) _tmp = ssh_cnv_string((ssh_u)tmp, stk, def, is_enum);
			else _tmp = (tmp, is_hex ? String::_hex : String::_dec);
			if(i) ret += split;
			ret += _tmp;
		}
		return ret;
	}






	/*
	ssh_u SSH ssh_cnv_value(ssh_wcs str, ENUM_DATA* stk, ssh_u def);
	String SSH ssh_cnv_string(ssh_u flags, ENUM_DATA* stk, ssh_wcs def, bool enumerate = true);
	void SSH ssh_remove_comments(String* lst, ssh_u count, bool is_simple);

	template <typename T > String ssh_make_hex_string(T* p, ssh_u count, String& txt, bool is_cont)
	{
	String bytes(L'\0', count * 3);
	String gran;
	ssh_ws* _ws(bytes.buffer());
	gran.fmt(L"%%0%ix ", sizeof(T) * 2);
	for(ssh_u i = 0; i < count / sizeof(T); i++)
	{
	swprintf(_ws, gran, (T*)p[i]);
	_ws += (sizeof(T) * 2) + 1;
	}
	_ws = (ssh_ws*)p;
	txt.empty();
	for(ssh_u i = 0; i < count / 2; i++)
	{
	ssh_ws val(*_ws++);
	if(val < 33) val = L'.';
	txt += val;
	}
	if(is_cont) bytes += L"...";
	return bytes;
	}
	*/

}
