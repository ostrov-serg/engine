
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
		// ���������
		String caption;
		// ������
		HCURSOR cursor;
		// ������
		HICON icon;
		// ����� ������
		HICON iconSm;
		// ���
		HBRUSH bkg;
		// ��� ������
		mutable String className;
		// ����� ������
		UINT stylesClass;
		// ����� ����
		UINT stylesWnd;
		// ����� ������������� ����
		HWND hWndParent;
		// ����� ����
		mutable HWND hWnd;
		// ����� ������
		HINSTANCE hInst;
		// �������� ����
		//Bar<int> bar;
		// ��������� ��������� ����
		WNDPROC processingWnd;
	};

	// ���� ������������ ������ 
	enum class WindowsTypes : ssh_w
	{
		_UNK, _NT, _2K, _XP, _VISTA, _7, _8, _8_1, _10
	};
	// ����������� ����������
	enum class CpuCaps : ssh_u
	{
		NONE, MMX, SSE, SSE2, SSE3, SSSE3, SSE4_1, SSE4_2, PCLMULQDQ, FMA, CMPXCHG16B, MOVBE, POPCNT, AES, AVX, RDRAND, CMOV, BMI1, AVX2, BMI2, AVX512F, RDSEED, AVX512PF, AVX512ER, AVX512CD, HALF
	};

	enum class SystemInfo : ssh_w
	{
		PROG_FOLDER, WORK_FOLDER, TEMP_FOLDER, USER_FOLDER, PROG_NAME, USER_NAME, COMP_NAME, CUSTOM, CPU_CAPS, PLATFORM, TOTAL_MEMORY, PHYSICAL_MEMORY, CPU_SPEED
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
	String SSH ssh_translate(ssh_cws text, bool to_eng);
	String SSH ssh_num_volume(ssh_u num);
	String SSH ssh_path_in_range(const String& path, ssh_u range);
	String SSH ssh_make_guid(const GUID& guid);
	String SSH ssh_convert(ssh_cws from, const buffer& in, ssh_u offs);
	GUID   SSH ssh_make_guid(ssh_cws src);
	buffer SSH ssh_base64(const String& str);
	buffer SSH ssh_convert(ssh_cws to, ssh_cws str);
	void SSH ssh_make_path(ssh_cws path, bool is_file);
	void SSH ssh_remove_comments(String* lst, ssh_u count, bool is_simple);
	bool SSH ssh_is_null(ssh_cws str);
	bool SSH ssh_is_wrong_lex(const String& str, ssh_cws errLexs = nullptr);
	bool SSH ssh_dlg_sel_folder(ssh_cws title, String& folder, HWND hWnd);
	bool SSH ssh_make_wnd(const DESC_WND& desc, bool is_show_wnd);
	ssh_u SSH ssh_dlg_save_or_open(bool bOpen, ssh_cws title, ssh_cws filter, ssh_cws ext, String& folder, HWND hWnd, String* arr, ssh_u count);
	// ������������� �������� � ��������� ������� ������
	inline ssh_u ssh_pow2(ssh_u val, bool nearest)
	{
		ssh_d idx;
		_BitScanReverse64(&idx, val);
		ssh_u _val((1ULL << idx));
		return ((_val == val || nearest) ? _val : _val << 1ULL);
	}
	// ��������� �� ��������� �������� ������� ������
	template <typename T> bool ssh_is_pow2(const T& value)
	{
		return (value == ssh_pow2<T>(value, true));
	}
	// �������� ��������
	template <typename T> void ssh_swap(T& val1, T& val2)
	{
		T tmp(val1); val1 = val2; val2 = tmp;
	}
	// �������
	template <typename T> T ssh_min(const T& val1, const T& val2)
	{
		return (val1 < val2 ? val1 : val2);
	}
	// ��������
	template <typename T> T ssh_max(const T& val1, const T& val2)
	{
		return (val1 > val2 ? val1 : val2);
	}
	// ������� ������ �� ��������
	class EnumReflector;
	template <typename T> void ssh_explode(ssh_cws split, const String& src, T* dst, ssh_u count, const T& def, const EnumReflector* stk = nullptr, Radix R = Radix::_dec)
	{
		ssh_ws* _wcs(src.buffer()), *t(_wcs);
		ssh_u j(wcslen(split));
		T tmp;
		while(count && t)
		{
			if((t = wcsstr(_wcs, split))) *t = 0;
			if(stk)
			{
				ssh_u val(0);
				ssh_ws* tt(_wcs);
				while(tt)
				{
					if((tt = (ssh_ws*)wcschr(_wcs, L'|'))) *tt = 0;
					ssh_u idx(stk->find(_wcs));
					if(idx == -1) { val = (ssh_u)def; break; }
					val |= stk->at(idx).value;
					if(!tt) break;
					*tt = L'|';
					_wcs = ++tt;
				}
				tmp = (T)val;
			}
			else tmp = String(_wcs).to_num<T>(0, R);
			*dst++ = (T)tmp;
			if(t) { *t = *split; _wcs = t + j; }
			count--;
		}
		// ��������� ���������� �� ���������
		while(count--) *dst++ = def;
	}
	// ��������� �������� � ������
	template <typename T> String ssh_implode(ssh_cws split, T* src, ssh_u count, ssh_cws def, const EnumReflector* stk = nullptr, bool is_enum = false, Radix R = Radix::_dec)
	{
		String ret, _tmp;

		while(count--)
		{
			T tmp(*src++);
			if(stk)
			{
				_tmp.empty();
				ssh_u idx;
				int t((int)tmp);
				if(is_enum) _tmp = ((idx = stk->find(t)) == -1 ? def : stk->at(idx).name);
				else
				{
					for(idx = 0; idx < stk->count(); idx++)
					{
						if((t & stk->at(idx).value))
						{
							if(!_tmp.is_empty()) _tmp += L'|';
							_tmp += stk->at(idx).name;
						}
					}
					if(_tmp.is_empty()) _tmp = def;
				}
			}
			else _tmp = String(tmp, R);
			if(!ret.is_empty()) ret += split;
			ret += _tmp;
		}
		return ret;
	}
	// ������������ ������ ���� � hex ���������
	template <typename T > String ssh_make_hex_string(T* p, ssh_u count, String& txt, bool is_w, bool is_cont)
	{
		String bytes(L'\0', count * (sizeof(T) * 2 + 1)), _spc(L" ");
		ssh_ws* _ws(bytes.buffer());
		String gran(String::fmt(L"%%0%iI64x ", sizeof(T) * 2));
		for(ssh_u i = 0; i < count; i++)
		{
			swprintf(_ws, gran, p[i]);
			_ws += (sizeof(T) * 2) + 1;
		}
		ssh_cs* _cs((ssh_cs*)p);
		txt.empty();
		count *= sizeof(T);
		while(count--)
		{
			ssh_ws val(is_w ? *(ssh_ws*)_cs : *_cs);
			_cs += 1 + is_w;
			count -= is_w;
			if(val < 33) val = L'.';
			txt += val + _spc;
		}
		if(is_cont) bytes += L". . .";
		return bytes;
	}
}
