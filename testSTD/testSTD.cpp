
#include "stdafx.h"


//typedef void* (CALLBACK* __cnv_open)(ssh_cws to, ssh_cws from);
typedef ssh_cs* (CALLBACK* __ext_undname)(char* out, const char* name, int len_out, unsigned long flags);

ssh_u ssh_dll_proc(ssh_cws dll, ssh_ccs nm)
{
	HMODULE hdll;
	if(!(hdll = LoadLibrary(dll))) return 0;
	return (ssh_u)GetProcAddress(hdll, nm);
}

class tp
{
	SSH_RTTI_DECL(tp);
public:
//	tp() : x(0), y(0) {}
	~tp()
	{
		x = y = -1;
	}
	ssh_u x, y;
};

SSH_RTTI_IMPL(tp);

int main() noexcept
{
	ssh_ccs c1 = typeid(tp).raw_name();
	ssh_ccs c2 = typeid(tp*).raw_name();
	Array<tp> a1(1, {{1, 2}, {3, 4}});
	a1.reset();
	tp* t1 = new tp{1,1};
	tp* t2 = new tp{2, 2};
	tp* t3 = new tp{3, 3};
	List<tp*> l1(1, {t1, t2, t3});
	Array<tp*> a2(1, {t1, t2, t3});
	a2.reset();
	try
	{
		tp* t = (tp*)RTTI::createClass(L"tp");
		int a = sizeof(decltype(t));
//		t->exec(100);
	}
	catch(const Exception& e)
	{
		e.add(L"111");
	}
	return 0;
}

