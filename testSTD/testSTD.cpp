
#include "stdafx.h"


//typedef void* (CALLBACK* __cnv_open)(ssh_cws to, ssh_cws from);
typedef ssh_cs* (CALLBACK* __ext_undname)(char* out, const char* name, int len_out, unsigned long flags);



ssh_u ssh_dll_proc(ssh_cws dll, ssh_ccs nm)
{
	HMODULE hdll;
	if(!(hdll = LoadLibrary(dll))) return 0;
	return (ssh_u)GetProcAddress(hdll, nm);
}

class tp : public Serialize
{
public:
//	tp() : x(0), y(0) {}
	virtual ~tp() { x = y = nullptr; }
	struct stk2 { String str[3]; ssh_cws _cws; };
	struct stk { double xx, *yy[4]; stk2 _stk2; };
	virtual SCHEME* get_scheme() const override
	{
		SCHEME_BEGIN(tp)
			SCHEME_NODE_BEGIN(tp, _stk)
				SCHEME_VAR(stk, xx, L"null", 0, nullptr)
					SCHEME_NODE_BEGIN(stk, _stk2)
						SCHEME_VAR(stk2, str, L"null", 0, nullptr)
						SCHEME_VAR(stk2, _cws, L"null", 0, nullptr)
					SCHEME_NODE_END()
				SCHEME_VAR(stk, yy, L"null", 0, nullptr)
			SCHEME_NODE_END()
			SCHEME_VAR(tp, x, L"null", 0, nullptr)
			SCHEME_VAR(tp, y, L"null", 0, nullptr)
		SCHEME_END(tp);
	}
	float* x, *y;
	stk _stk;
};

int main() noexcept
{
	tp t;
	Serialize::SCHEME* _sc = t.get_scheme();
	return 0;
}

