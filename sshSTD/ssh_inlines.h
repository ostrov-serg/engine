#pragma once

extern "C"
{
	ssh_u	asm_ssh_capability();
	ssh_cws asm_ssh_to_base64(ssh_b* ptr, ssh_u count);
	ssh_b*	asm_ssh_from_base64(ssh_ws* str, ssh_u count, ssh_u* len_buf);
	void*	asm_ssh_wton(ssh_cws str, ssh_u radix, ssh_ws* end = nullptr);
	ssh_cws asm_ssh_ntow(const void* ptr, ssh_u radix, ssh_ws* end = nullptr);
	ssh_ws* asm_ssh_parse_spec(void* arg, ssh_cws* str);
	ssh_l	asm_ssh_parse_xml(ssh_ws* src, ssh_w* vec);
}

namespace ssh
{
	using	__regx_compile		= regex16* (CALLBACK*)(ssh_cws pattern, ssh_l options);
	using	__regx_exec			= ssh_l(CALLBACK*)(const void* re, ssh_cws subj, ssh_l len_subj, ssh_l idx, ssh_l options, ssh_l* vec, ssh_l count_vec);
	using	__regx_free			= ssh_l(CALLBACK*)(void* p);

	using	__xin_xenable		= void (CALLBACK*)(int is);
	using	__xin_xgstate		= ssh_d(CALLBACK*)(ssh_d idx, XINPUT_STATE* state);
	using	__xin_xsstate		= ssh_d(CALLBACK*)(ssh_d idx, XINPUT_VIBRATION* state);
	using	__xin_xcaps			= ssh_d(CALLBACK*)(ssh_d idx, ssh_d flags, XINPUT_CAPABILITIES* caps);

	// процедуры из библиотеки sshREGX
	extern	__regx_compile		ssh_regx_compile;
	extern	__regx_exec			ssh_regx_exec;
	extern	__regx_free			ssh_regx_free;

	// процедуры из библиотеки x_input1_3
	extern __xin_xenable		ssh_xin_enable;
	extern __xin_xgstate		ssh_xin_gstate;
	extern __xin_xsstate		ssh_xin_sstate;
	extern __xin_xcaps			ssh_xin_caps;

	// определения для процессорно - зависимых функций
	using	__asm_ssh_wcsstr	= ssh_ws*(*)(ssh_cws _1, ssh_cws _2);
	using	__asm_ssh_wcscmp	= int(*)(ssh_cws _1, ssh_cws _2);
	using	__ssh_rand			= ssh_u(*)(ssh_u begin, ssh_u end);
	using	__ssh_hash			= int(*)(ssh_cws _cws);

	// указатели на процессорно - зависимые функции
	extern __ssh_rand			SSH ssh_rand;
	extern __asm_ssh_wcsstr		SSH asm_ssh_wcsstr;
	extern __asm_ssh_wcscmp		SSH asm_ssh_wcscmp;
	extern	__ssh_hash			SSH ssh_hash;

	enum class Radix { _dec, _bin, _oct, _hex, _dbl, _flt, _bool };

	// возможности процессора
	enum class CpuCaps : ssh_u
	{
		NONE, MMX, SSE, SSE2, SSE3, SSSE3, SSE4_1, SSE4_2, PCLMULQDQ, FMA, CMPXCHG16B, MOVBE, POPCNT, AES, AVX, RDRAND, CMOV, BMI1, AVX2, BMI2, AVX512F, RDSEED, AVX512PF, AVX512ER, AVX512CD, HALF
	};

	static CpuCaps cpuCaps((CpuCaps)asm_ssh_capability());

	// глобальные встроенные функции
	inline bool ssh_is_null(ssh_cws str) { return (!str || !str[0]); }
	inline bool ssh_is_digit(ssh_cws str) { return (*str >= L'0' && *str <= L'9'); }
	inline ssh_b ssh_cpu_caps(CpuCaps caps) { return _bittest64((const ssh_l*)&cpuCaps, (ssh_u)caps); }

	inline ssh_u ssh_sse_wcslen(ssh_cws _wcs)
	{
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

	inline ssh_u ssh_wcslen(ssh_cws _wcs)
	{
		if(!ssh_cpu_caps(CpuCaps::SSE4_2)) return wcslen(_wcs);
		return ssh_sse_wcslen(_wcs);
	}
	
	inline ssh_ws* ssh_wcschr(ssh_cws _wcs, ssh_ws _ws)
	{
		if(!ssh_cpu_caps(CpuCaps::SSE4_2)) return (ssh_ws*)wcschr(_wcs, _ws);
		__m128i _1(_mm_cvtsi32_si128((int)_ws)), _2;
		int res, is;
		do
		{
			_2 = _mm_lddqu_si128((__m128i*)_wcs);
			res = _mm_cmpistri(_1, _2, 0b00000001);
			is = _mm_cmpistrz(_1, _2, 0b00000001);
			_wcs += res;
		} while(res == 8 && !is);
		return (ssh_ws*)(is ? nullptr : _wcs);
	}

	inline int ssh_wcscmp(ssh_cws _s1, ssh_cws _s2)
	{
		return asm_ssh_wcscmp(_s1, _s2);
	}

	inline ssh_ws* ssh_wcsstr(ssh_ws* _s1, ssh_cws _s2)
	{
		return asm_ssh_wcsstr(_s1, _s2);
	}

	inline void* ssh_memset(void* ptr, ssh_u set, ssh_u count)
	{
		ssh_b* _ptr((ssh_b*)ptr);
		if(count >= 8) { __stosq((ssh_u*)_ptr, set, count / 8); _ptr += (count / 8) * 8; count &= 7; }
		if(count >= 4) { *(ssh_d*)_ptr = (ssh_d)set; _ptr += 4; count &= 3; set >>= 32; }
		if(count >= 2) { *(ssh_w*)_ptr = (ssh_w)set; _ptr += 2; count &= 1; set >>= 16; }
		if(count >= 1) { *(ssh_b*)_ptr = (ssh_b)set; _ptr++; }
		return _ptr;
	}

	inline void* ssh_memzero(void* ptr, ssh_u count)
	{
		return ssh_memset(ptr, 0, count);
	}

	inline void* ssh_memcpy(void* dst, const void* src, ssh_u count)
	{
		ssh_b* _dst((ssh_b*)dst);
		ssh_b* _src((ssh_b*)src);
		if(count >= 8) { __movsq((ssh_u*)_dst, (ssh_u*)_src, count / 8); _dst += (count / 8) * 8; _src += (count / 8) * 8; count &= 7; }
		if(count >= 4) { __movsd((ssh_d*)_dst, (ssh_d*)_src, 1); _dst += 4; _src += 4; count &= 3; }
		if(count >= 2) { __movsw((ssh_w*)_dst, (ssh_w*)_src, 1); _dst += 2; _src += 2; count &= 1; }
		if(count >= 1) { __movsb((ssh_b*)_dst, (ssh_b*)_src, 1); _dst++; }
		return _dst;
	}
	// реверс бит
	inline int ssh_rev(int x)
	{
		x = ((x & 0x55555555) << 1) | ((x >> 1) & 0x55555555);
		x = ((x & 0x33333333) << 2) | ((x >> 2) & 0x33333333);
		x = ((x & 0x0f0f0f0f) << 4) | ((x >> 4) & 0x0f0f0f0f);
		return (_rotr((x & 0x00ff00ff), 8) | (_rotl(x, 8) & 0x00ff00ff));
	}
	// остаток от деления для вещественных чисел
	inline float ssh_modf(float x, float y)
	{
		return (x - y *(int)(x / y));
	}
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
	// итератор
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
	// менеджер памяти для однотипных элементов
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
				ssh_memzero(tmp->arr, sizeof(Block) * N);
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