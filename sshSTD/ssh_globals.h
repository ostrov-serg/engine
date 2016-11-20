
#pragma once

#include "ssh_str.h"
#include "ssh_buf.h"

namespace ssh
{
	class EnumReflector;
	using	__regx_compile			= regex16* (CALLBACK* )(ssh_cws pattern, ssh_l options);
	using	__regx_exec				= ssh_l (CALLBACK* )(const void* re, ssh_cws subj, ssh_l len_subj, ssh_l idx, ssh_l options, ssh_l* vec, ssh_l count_vec);
	using	__regx_free				= ssh_l (CALLBACK* )(void* p);

	using	__xin_xenable			= void (CALLBACK* )(int is);
	using	__xin_xgstate			= ssh_d (CALLBACK* )(ssh_d idx, XINPUT_STATE* state);
	using	__xin_xsstate			= ssh_d (CALLBACK* )(ssh_d idx, XINPUT_VIBRATION* state);
	using	__xin_xcaps				= ssh_d (CALLBACK* )(ssh_d idx, ssh_d flags, XINPUT_CAPABILITIES* caps);

	// определения для процессорно - зависимых функций
	using	__asm_ssh_capability	= ssh_u(*)();
	using	__asm_ssh_to_base64		= ssh_cws(*)(ssh_b* ptr, ssh_u count);
	using	__asm_ssh_from_base64	= ssh_b*(*)(ssh_ws* str, ssh_u count, ssh_u* len_buf);
	using	__asm_ssh_parse_xml		= ssh_l(*)(ssh_ws* src, ssh_w* vec);
	using	__asm_ssh_parse_spec	= ssh_ws*(*)(void* val, ssh_cws* ptr);
	using	__asm_ssh_wcslen		= ssh_u(*)(ssh_cws _1);
	using	__asm_ssh_wcsstr		= ssh_ws*(*)(ssh_cws _1, ssh_cws _2);
	using	__asm_ssh_wcschr		= ssh_ws*(*)(ssh_cws _cws, ssh_ws _ws);
	using	__asm_ssh_wcscmp		= ssh_u(*)(ssh_cws _1, ssh_cws _2);
	using	__ssh_rand				= ssh_u (*)(ssh_u begin, ssh_u end);

	// указатели на процессорно - зависимые функции
	extern __ssh_rand				SSH ssh_rand;
	extern __asm_ssh_capability		SSH asm_ssh_capability;
	extern __asm_ssh_to_base64		SSH asm_ssh_to_base64;
	extern __asm_ssh_from_base64	SSH asm_ssh_from_base64;
	extern __asm_ssh_parse_xml		SSH asm_ssh_parse_xml;
	extern __asm_ssh_parse_spec		SSH asm_ssh_parse_spec;
	extern __asm_ssh_wcslen			SSH asm_ssh_wcslen;
	extern __asm_ssh_wcsstr			SSH asm_ssh_wcsstr;
	extern __asm_ssh_wcschr			SSH asm_ssh_wcschr;
	extern __asm_ssh_wcscmp			SSH asm_ssh_wcscmp;

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

	SSH float* ssh_vec3_mtx(const float* v, const float* m);
	SSH float* ssh_vec4_mtx(const float* v, const float* m);
	SSH float* ssh_mtx_vec3(const float* m, const float* v);
	SSH float* ssh_mtx_vec4(const float* m, const float* v);
	SSH float* ssh_mtx_mtx(const float* m1, const float* m2);

	int SSH ssh_ext_count_array();
	ssh_u SSH ssh_ext_exec_hash(ssh_ccs str);
	String SSH ssh_ext_exec(ssh_ccs str);

	ssh_u SSH ssh_system_values(SystemInfo type, CpuCaps value);
	ssh_u SSH ssh_dll_proc(ssh_cws dll, ssh_ccs proc, ssh_cws suffix = L"d");
	ssh_u SSH ssh_offset_line(const String& text, ssh_l ln);
	ssh_u SSH ssh_count_lines(const String& text);
	ssh_u SSH ssh_split(ssh_ws split, ssh_cws src, ssh_u* vec, ssh_u count_vec);
	String SSH ssh_printf(ssh_cws s, ...);
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
	void SSH _ssh_printf(String& ret, ssh_cws s);
	void SSH ssh_make_path(ssh_cws path);
	void SSH ssh_remove_comments(String* lst, ssh_u count, bool is_simple);//
	inline bool ssh_is_null(ssh_cws str) { return (!str || !str[0]); }
	inline bool ssh_is_digit(ssh_cws str) { return (*str >= L'0' && *str <= L'9'); }
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
		T tmp(std::move(val1));
		val1 = std::move(val2);
		val2 = std::move(tmp);
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
				ssh_ws* tt;
				while(true)
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
		String gran(ssh_printf(L"%%0%iI64x ", sizeof(T) * 2));
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

	inline ssh_u ssh_wcslen(ssh_cws _wcs)
	{
		if(!ssh_system_values(SystemInfo::CPU_CAPS, CpuCaps::SSE4_2)) return wcslen(_wcs);
		int res(0), ret(0);
		do
		{
			__m128i _1(_mm_lddqu_si128((__m128i*)(_wcs)));
			res = _mm_cmpistri(_1, _1, 0b00010001);
			ret += res;
			_wcs += res;
		} while(res == 8);
		return ret;
	}

	inline ssh_ws* ssh_wcschr(ssh_cws _wcs, ssh_ws _ws)
	{
		return nullptr;
//		if(!ssh_system_values(SystemInfo::CPU_CAPS, CpuCaps::SSE4_2)) return wcschr(_wcs, _ws);

		/*
		test rdx, rdx
		jz _zero
		movd xmm0, rdx
		mov rax, rcx
		@@:		pcmpistri xmm0, xmmword ptr [rax], 00000001b
		lea rax, [rax + rcx * 2]
		jnc @b
		ret
		_zero:	mov r8, rcx
		call asm_ssh_wcslen
		lea rax, [r8 + rax * 2]
		ret
		*/
	}
}
