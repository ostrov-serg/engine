
#include "stdafx.h"


//typedef void* (CALLBACK* __cnv_open)(ssh_cws to, ssh_cws from);
typedef ssh_cs* (CALLBACK* __ext_undname)(char* out, const char* name, int len_out, unsigned long flags);



ssh_u ssh_dll_proc(ssh_cws dll, ssh_ccs nm)
{
	HMODULE hdll;
	if(!(hdll = LoadLibrary(dll))) return 0;
	return (ssh_u)GetProcAddress(hdll, nm);
}

class ttt : public Serialize
{
public:
	ttt() : a(999), b(111), c(222) {}
	virtual SCHEME* get_scheme() const override
	{
		SCHEME_BEGIN(ttt)
			SCHEME_VAR(ttt, a, L"1", SC_BIN, nullptr)
			SCHEME_VAR(ttt, b, L"2", SC_OCT, nullptr)
			SCHEME_VAR(ttt, c, L"3", SC_HEX, nullptr)
		SCHEME_END(ttt);
	}
		ssh_w a, b, c;
};
class tp : public Serialize
{
public:
	SSH_ENUM(_enum, _one = 1, _two = 2);
	tp() : x(_one), y(_two) { }
	virtual ~tp() { }
	struct stk2
	{
		//stk2() { str[0] = L"Сергей"; str[1] = L"Влад"; str[2] = L"Макс"; _cws = L'!'; }
		String str[3];
	};
	struct stk
	{
		stk() { xx = 10.0; yy[0] = 1.0; yy[1] = 2.0; yy[2] = 3.0; yy[3] = 4.0; }
		double xx, yy[4];
		stk2 _stk2[3]{{L"1",L"2",L"3"},{L"11",L"22",L"33"},{L"111",L"222",L"333"}};
	};
	virtual SCHEME* get_scheme() const override
	{
		auto& reflector = ssh::EnumReflector::get<_enum>();
		SCHEME_BEGIN(tp)
			SCHEME_VAR(tp, _cws, L"null", 0, nullptr)
			SCHEME_VAR(tp, x, L"null", 0, &reflector)
			SCHEME_VAR(tp, _ccs, L"null", 0, nullptr)
			SCHEME_NODE_BEGIN(tp, _stk)
				SCHEME_VAR(stk, xx, L"null", 0, nullptr)
					SCHEME_NODE_BEGIN(stk, _stk2)
						SCHEME_VAR(stk2, str, L"null", SC_BASE64, nullptr)
					SCHEME_NODE_END()
				SCHEME_VAR(stk, yy, L"null", 0, nullptr)
			SCHEME_NODE_END()
			SCHEME_VAR(tp, y, L"null", 0, &reflector)
			SCHEME_VAR(tp, is, L"false", 0, nullptr)
			SCHEME_NODE_BEGIN(tp, _t)
			SCHEME_NODE_END()
		SCHEME_END(tp);
	}
	int x, y;
	const bool is = true;
	ssh_ws _cws[10] = L"Влад";
	ssh_cs _ccs = '?';
	stk _stk[2];
	ttt _t[3];
	void make()
	{
		Serialize::save(L"c:\\1.xml", true);
	}
};

class bs1 : public Base
{
	SSH_DYNCREATE(bs1);
public:
	bs1() { x = 0; }
	bs1(const bs1& b)
	{
		x = b.x;
	}
	bs1(int _x, const String& _s) : x(_x), str(_s) { }
	void set_str(String s)
	{
		str = std::move(s);
		str1 = s;
		ssh_cws _1 = __FUNCTIONW__;
		ssh_ccs _2 = __FUNCDNAME__;
		ssh_ccs _3 = __FUNCSIG__;
		ssh_ccs _4 = __func__;
	}

	String str, str1;
	int x;

};

ssh_u operator "" _suffix(ssh_u minutes)
{
	return minutes * 60;
}

String operator ""_r(ssh_ccs _ccs)
{
	return String(_ccs);
}

using namespace std;

extern "C"
{
	ssh_u	asm_ssh_wcslen1(ssh_cws _1);
}

inline int ssh_wcslen1(ssh_cws _wcs)
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

int main() noexcept
{
	String _1(L"123");
	_1 += L" 456";
	String _2(_1 + L"2345677");
	ssh_ws* _ws1 = _1.str();
	ssh_ws* _ws2 = _2.str();
	ssh_u ret(123);
	_1 = ssh_printf(L"%% %i % I64i, %.9f, %s, %S, %c, %C", 1L, ret, 3.0, _1.str(), "qwert", L'!', '@');
	Log::instance()->init();
	Log::stk_common _c;
	Xml xml;
	tp t;
	t.make();
	return 0;
}
