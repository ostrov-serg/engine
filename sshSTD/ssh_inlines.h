
#pragma once

extern "C"
{
	ssh_u asm_ssh_capability();
	ssh_cws asm_ssh_to_base64(ssh_b* ptr, ssh_u count);
	ssh_b* asm_ssh_from_base64(ssh_ws* str, ssh_u count, ssh_u* len_buf);
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
	using	__asm_ssh_parse_xml = ssh_l(*)(ssh_ws* src, ssh_w* vec);
	using	__asm_ssh_parse_spec = ssh_ws*(*)(void* val, ssh_cws* ptr);
	using	__asm_ssh_wcslen	= ssh_u(*)(ssh_cws _1);
	using	__asm_ssh_wcsstr	= ssh_ws*(*)(ssh_cws _1, ssh_cws _2);
	using	__asm_ssh_wcschr	= ssh_ws*(*)(ssh_cws _cws, ssh_ws _ws);
	using	__asm_ssh_wcscmp	= ssh_u(*)(ssh_cws _1, ssh_cws _2);
	using	__ssh_rand			= ssh_u(*)(ssh_u begin, ssh_u end);
	using	__ssh_hash			= int(*)(ssh_cws _cws);
	using	__asm_ssh_wton		= void*(*)(ssh_cws str, ssh_u radix, ssh_ws* end);
	using	__asm_ssh_ntow		= ssh_cws(*)(const void* num, ssh_u radix, ssh_ws* end);

	// указатели на процессорно - зависимые функции
	extern __ssh_rand			SSH ssh_rand;
	extern __asm_ssh_parse_xml	SSH asm_ssh_parse_xml;
	extern __asm_ssh_parse_spec	SSH asm_ssh_parse_spec;
	extern __asm_ssh_wcslen		SSH asm_ssh_wcslen;
	extern __asm_ssh_wcsstr		SSH asm_ssh_wcsstr;
	extern __asm_ssh_wcschr		SSH asm_ssh_wcschr;
	extern __asm_ssh_wcscmp		SSH asm_ssh_wcscmp;
	extern	__ssh_hash			SSH ssh_hash;
	extern	__asm_ssh_wton		SSH asm_ssh_wton;
	extern	__asm_ssh_ntow		SSH asm_ssh_ntow;

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

	inline ssh_u ssh_wcslen(ssh_cws _wcs)
	{
		if(!ssh_cpu_caps(CpuCaps::SSE4_2)) return wcslen(_wcs);
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
		if(!ssh_cpu_caps(CpuCaps::SSE4_2)) return (ssh_ws*)wcschr(_wcs, _ws);
		return nullptr;

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

	inline int ssh_wcscmp(ssh_cws _s1, ssh_cws _s2)
	{
		if(!ssh_cpu_caps(CpuCaps::SSE4_2)) return wcscmp(_s1, _s2);
		return 0;
	}

	inline ssh_ws* ssh_wcsstr(ssh_ws* _s1, ssh_cws _s2)
	{
		if(!ssh_cpu_caps(CpuCaps::SSE4_2)) return wcsstr(_s1, _s2);
		return 0;
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
}