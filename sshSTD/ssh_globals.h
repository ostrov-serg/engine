
#pragma once

extern "C"
{
	ssh_u	asm_ssh_capability();
	ssh_b*	asm_ssh_to_base64(ssh_b* ptr, ssh_u count);
	ssh_b*	asm_ssh_from_base64(ssh_ws* str, ssh_u count, ssh_u* len_buf);
	ssh_l	asm_ssh_parse_xml(ssh_ws* src, ssh_w* vec);
};

#include "ssh_str.h"
#include "ssh_buf.h"

namespace ssh
{
	class EnumReflector;
	using __cnv_open = ssh_cnv (CALLBACK*)(ssh_cws to, ssh_cws from);
	typedef void (CALLBACK* __cnv_close)(void* h);
	typedef void (CALLBACK* __cnv_make)(void* cd, const ssh_b* inbuf, ssh_u inbytesleft, ssh_b* out);
	typedef ssh_u (CALLBACK* __cnv_calc)(void* cd, const ssh_b* inbuf, ssh_u inbytesleft);
	typedef regex16* (CALLBACK* __regx_compile)(ssh_cws pattern, ssh_l options);
	typedef ssh_l (CALLBACK* __regx_exec)(const void* re, ssh_cws subj, ssh_l len_subj, ssh_l idx, ssh_l options, ssh_l* vec, ssh_l count_vec);
	typedef ssh_l (CALLBACK* __regx_free)(void* p);

	int SSH ssh_ext_count_array();
	ssh_u SSH ssh_ext_exec_hash(ssh_ccs str);
	String SSH ssh_ext_exec(ssh_ccs str);

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
		PROG_FOLDER, WORK_FOLDER, TEMP_FOLDER, USER_FOLDER, PROG_NAME, USER_NAME, COMP_NAME, CUSTOM, CPU_CAPS, PLATFORM, TOTAL_MEMORY, PHYSICAL_MEMORY, CPU_SPEED
	};

	ssh_u SSH ssh_system_value(SystemInfo type, CpuCaps value);
	ssh_u SSH ssh_dll_proc(ssh_cws dll, ssh_ccs proc, ssh_cws suffix = L"d");
	ssh_u SSH ssh_hash(ssh_cws wcs);//
	ssh_u SSH ssh_rand(ssh_u begin, ssh_u end);
	ssh_u SSH ssh_offset_line(const String& text, ssh_l ln);
	ssh_u SSH ssh_count_lines(const String& text);
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
	String SSH ssh_base64(const Buffer& buf);
	String SSH ssh_translate(ssh_cws text, bool to_eng);
	String SSH ssh_num_volume(ssh_u num);
	String SSH ssh_path_in_range(const String& path, ssh_u range);
	String SSH ssh_make_guid(const GUID& guid);
	String SSH ssh_convert(ssh_cws charset, const Buffer& in, ssh_u offs);
	String SSH ssh_implode2(ssh_u val, const EnumReflector* stk, ssh_cws def, bool is_enum);
	GUID   SSH ssh_make_guid(ssh_cws src);
	Buffer SSH ssh_base64(const String& str);
	Buffer SSH ssh_convert(ssh_cws charset, ssh_cws str);
	void SSH ssh_make_path(ssh_cws path);
	void SSH ssh_remove_comments(String* lst, ssh_u count, bool is_simple);//
	inline bool ssh_is_null(ssh_cws str) { return (!str || !str[0]); }
	bool SSH ssh_is_wrong_lex(const String& str, ssh_cws errLexs = nullptr);
	bool SSH ssh_dlg_sel_folder(ssh_cws title, String& folder, HWND hWnd);
	bool SSH ssh_make_wnd(const DESC_WND& desc, bool is_show_wnd);//
	ssh_u SSH ssh_dlg_save_or_open(bool bOpen, ssh_cws title, ssh_cws filter, ssh_cws ext, String& folder, HWND hWnd, String* arr = nullptr, ssh_u count = 0);
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
	template <typename T> void ssh_swap(T& val1, T& val2)
	{
		T tmp(val1); val1 = val2; val2 = tmp;
	}
	// минимум
	template <typename T> T ssh_min(const T& val1, const T& val2)
	{
		return (val1 < val2 ? val1 : val2);
	}
	// максимум
	template <typename T> T ssh_max(const T& val1, const T& val2)
	{
		return (val1 > val2 ? val1 : val2);
	}
	// разбить строку на элементы
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
		// заполняем значениями по умолчанию
		while(count--) *dst++ = def;
	}
	// соединить элементы в строку
	template <typename T> String ssh_implode(ssh_cws split, T* src, ssh_u count, ssh_cws def, const EnumReflector* stk = nullptr, bool is_enum = false, Radix R = Radix::_dec)
	{
		String ret, _tmp;

		while(count--)
		{
			T tmp(*src++);
			if(stk)
			{
				_tmp.empty();
				_tmp = ssh_implode2((int)tmp, def, is_enum);
			}
			else _tmp = String(tmp, R);
			if(!ret.is_empty()) ret += split;
			ret += _tmp;
		}
		return ret;
	}
	// сформировать строку байт в hex счислении
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

	template <typename T> class Iter
	{
	public:
		// конструктор по значению
		Iter(T* n) : val(n) {}
		// оператор сравнения
		bool operator != (const Iter& it) const { return (val != it.val); }
		// оператор приращения
		Iter operator++() const { val = val->next; return *this; }
		// оператор извлечения
		auto operator*() const { return val->value; }
	protected:
		// значение
		mutable T* val;
	};

	template <typename T, ssh_u N = 128> class MemArray
	{
	public:
		struct Block
		{
			union
			{
				Block* next;
				ssh_b t[sizeof(T)];
			};
		};

		struct BlockFix
		{
			BlockFix() : next(nullptr) {}
			~BlockFix() { SSH_DEL(next); }
			BlockFix* next;
			Block arr[N];
		};
		void Reset() { if(!count) { SSH_DEL(arrs); free = nullptr; } }
		T* Alloc()
		{
			if(!free)
			{
				BlockFix* tmp(new BlockFix);
				memset(tmp->arr, 0, sizeof(Block) * N);
				tmp->next = arrs; arrs = tmp;
				for(ssh_u i = 0; i < N; i++)
				{
					arrs->arr[i].next = free;
					free = &(arrs->arr[i]);
				}
			}
			Block* b(free);
			free = free->next;
			count++;
			return (T*)(b->t);
		}
		void Free(T* t)
		{
			Block* b((Block*)t);
			b->next = free;
			free = b;
			count--;
		}
		Block* free = nullptr;
		BlockFix* arrs = nullptr;
		int count = 0;
	};
}
