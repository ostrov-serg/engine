
#include "stdafx.h"
#include <regex>


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

void _func()
{
	SSH_TRACE;
	Xml xml;
	tp t;
	t.save(L"c:\\11.xml", true);
	File f(L"c:\\1.xml", File::open_read);
	t.open(f.read(0), true);
}

extern "C"
{
	ssh_ws* ssh_asm_wcsstr1(ssh_ws* _s1, ssh_cws _s2);
}


extern "C"
{
	ssh_u asm_strstr(void* ptr, ssh_u sz, ssh_u len);
	void asm_bwt(ssh_u len, void* dst, ssh_ccs src);
}
void lz77(const Buffer& buf)
{
	ssh_b* b(buf);
	ssh_u sz(buf.size());
	for(ssh_u i = 0; i < sz; i += 8)
	{
		sz -= asm_strstr(buf, sz, i);
	}
}

void get_sz(ssh_u* sz)
{

}


int main() noexcept
{
	Vec2 vec2;
	Vec3 vec3;
	Vec4 vec4;
	Ray ray;
	Sphere sphere;
	Dual_quat dq;
	Mtx _mtx;
	Color _color(0.5,1,0.3,0.25);
	Half _h;
	Bits bits;
	Pts<> pts1;
	Range<> range1;
	Bar<> bar1;
	Box<> box1;
	Bbox _bbox;
	Obox _obox(_bbox);
	Angle angle(30.0f);
	Angle angle2(0.524317f);
	Plane plane(1,2,3,4);
	Quat quat(10,20,30,40);
	Regx regx;
	Xml xml;
	File fdd(L"c:\\1", File::open_read);

	ssh_log->init(Log::TypeOutput::debug);
	ssh_unit_test();

	String str(L"Шаталов Сергей Викторович");
	Buffer buf1(10);
	BufferW buf2(L"Сергей");
	std::multiset<String> mymultiset;

	mymultiset.emplace(str.str());
	mymultiset.emplace(L"bar");
	mymultiset.emplace(L"1foo");

	for(auto item : mymultiset)
	{
		auto i = item;
	}
	ssh_i r = ssh_mod(23, 4);
	File f(L"c:\\1", File::open_read);
	ssh_compress(f.read());
	return 0;
	File f1(L"c:\\1+", File::create_write);
	File f2(L"c:\\1++", File::create_write);
	//	RLE rle;
	//Buffer r(rle.process(f.read(), true));
	BWT bwt;
	Buffer out1(bwt.process(f.read(), true));
	f1.write(out1);
	Buffer out2(bwt.process(out1, false));
	f2.write(out2);

	return 0;
	/*
	File ff(L"c:\\1.ssh", File::create_write);
	ff.write(out);
	f.close();
	ff.close();
	ff.open(L"c:\\1+", File::create_write);
	ff.write(ssh_decompress(out));
	ff.close();
	ssh_unit_test();

	return 0;
	/*
	std::list<int> lst;
	lst.push_back(1);
	lst.push_back(2);
	std::vector<int> ptr{1,2,3,4};
	Arith a1;
	ssh_u v = 10;
	String str(v);
	printf("%i", 1);
	std::regex re;

	ssh_b dest[117];
	ssh_memset(dest, 0x8070605040302010, 11);
	File f(L"c:\\h", File::open_read);
	Buffer buf(f.read(0));
	f.close();
	lz77(buf);
	SSH_TRACE;
	_func();
	return 0;
	*/
}
