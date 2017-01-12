
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


#include <list>
#include <map>
#include <forward_list>


extern "C"
{
	ssh_u asm_strstr(void* ptr, ssh_u sz, ssh_u len);
}
void lz77(const Buffer& buf)
{
	ssh_b* b(buf);
	ssh_u sz(buf.size());
	for(ssh_u i = 0 ; i < sz; i += 8)
	{
		sz -= asm_strstr(buf, sz, i);
	}
}

#include <stdio.h>
#include <string.h>
using namespace std;
#define MEGABYTE 1024*1024

unsigned char word[256];
unsigned char _data[1024];
unsigned short sz = 0;
unsigned char d = 0;
unsigned int i = 0;
unsigned short j = 0;

unsigned short search(unsigned char l)
{
	for(j = 0; j < sz; j++)
		if(word[j] == l) return j + 1;
	return 0;
}

void MoveToFront(unsigned int rsz)
{
	unsigned short p = 0;
	for(i = 0; i < rsz; i++)
	{
		p = search(_data[i]);
		if(!p)
		{
			for(j = sz; j > 0; j--)
				word[j] = word[j - 1];
			word[0] = _data[i];
			sz++;
		}
		else
		{
			d = word[p - 1];
			for(j = p - 1; j > 0; j--)
				word[j] = word[j - 1];
			word[0] = d;
		}
		_data[i] = (p >= 256 ? 0 : p);
	}
}

void MoveToBack(unsigned int rsz)
{
	unsigned char p = 0;
	for(i = rsz - 1; i >= 0; i--)
	{
		p = word[0];
		if(!_data[i])
		{
			d = word[0];
			for(j = 0; j < sz - 1; j++)
				word[j] = word[j + 1];
			word[sz - 1] = d;
		}
		else
		{
			d = word[0];
			for(j = 0; j < _data[i] - 1; j++)
				word[j] = word[j + 1];
			word[_data[i] - 1] = d;
		}
		_data[i] = p;
		if(i == 0) break;
	}
}

/*
int main(int argc, char *argv[])
{
	if(argc <= 3)
	{
		printf("Usage: %s TYPE IN/FILE/PATH OUT/FILE/PATH\nTYPE:\n-c \t-\tCode\n-d\t-\tDecode\n\n", argv[0]);
		return 1;
	}
	FILE * in = fopen(argv[2], "r");
	FILE * out = fopen(argv[3], "w");
	bool type = false;
	if(strcmp(argv[1], "-c") == 0) type = true;
	else if(strcmp(argv[1], "-d") == 0) type = false;
	else
	{
		printf("Usage: %s TYPE IN/FILE/PATH OUT/FILE/PATH\nTYPE:\n-c \t-\tCode\n-d\t-\tDecode\n\n", argv[0]);
		return 1;
	}
	if(type)
		while(!feof(in))
		{
			unsigned int rsz = fread(data, 1, MEGABYTE, in);
			sz = 0;
			fwrite(&sz, 2, 1, out);
			fwrite(word, 1, 256, out);
			fwrite(data, 1, rsz, out);
			fflush(out);
		}
	else
		while(!feof(in))
		{
			fread(&sz, 2, 1, in);
			fread(word, 1, 256, in);
			unsigned int rsz = fread(data, 1, MEGABYTE, in);
			MoveToBack(rsz);
			fwrite(data, 1, rsz, out);
			fflush(out);
		}
	fclose(in);
	fclose(out);
	return 0;
}
*/

#include <regex>

int main() noexcept
{
	memset(word, 0, 256);
	memcpy(_data, "sergey", 7);
	MoveToFront(6);
	int T[11];
	unsigned char in[] = "рдакраааабб";// af
	int count[256];
	unsigned char out_c[11];
	unsigned char M[] = "абдкр";
	int j, i, pos = 5, N = 256, n = strlen((const char*)in), x;
	for(i = 0; i < n; i++)
	{
		int tmpl, tmp2 = 0;
		x = in[i];
		tmpl = M[0];
		M[0] = x;
		j = 0;
		while(tmpl != x)
		{
			tmp2 = tmpl;
			tmpl = M[++j];
			M[j] = tmp2;
		}
		in[i] = j;
	}
	
	int sum = 0;

	for(i = 0; i < N; i++) count[i] = 0;
	for(i = 0; i < n; i++) count[in[i]]++;
	sum = 0;
	for(i = 0; i < N; i++)
	{
		sum += count[i];
		count[i] = sum - count[i];
	}
	for(i = 0;  i < n;  i++) T[count[in[i]]++] = i;
	for(i = 0; i < n ; i++)
	{
		pos = T[pos];
		out_c[i] = in[pos];
	}
	ssh_log->init();
	std::list<int> lst;
	lst.push_back(1);
	lst.push_back(2);
	std::vector<int> ptr{1,2,3,4};
	Arith a1;
	ssh_u v = 10;
	String str(v);
	File r(L"c:\\1", File::open_read);
	Buffer out(a1.process(r.read(0), true));
	File w(L"c:\\1+", File::create_write);
	w.write(out);
	Arith a2;
	File r1(L"c:\\1+", File::open_read);
	Buffer out1(a2.process(r1.read(0), false));
	File w1(L"c:\\1++", File::create_write);
	w1.write(out1);
	return 0;
	printf("%i", 1);
	std::regex re;

	ssh_b dest[117];
	ssh_memset(dest, 0x8070605040302010, 11);
	File f(L"c:\\h", File::open_read);
	Buffer buf(f.read(0));
	f.close();
	lz77(buf);
	return 0;
	/*
	xtree x;
	x.insert(10);
	x.insert(100);
	x.insert(1);
	x.insert(5);
	x.insert(120);
	x.insert(4);
	x.insert(2);
	x.insert(9);
	x.insert(1000);
	x.insert(1200);
	x.insert(990);
	std::map<int, ssh_ccs> m;
	m[10] = "ten";
	dump_map_and_set((struct tree_struct *)(void*)&m, false);
	m[20] = "twenty";
	dump_map_and_set((struct tree_struct *)(void*)&m, false);
	m[3] = "three";
	dump_map_and_set((struct tree_struct *)(void*)&m, false);
	m[111] = "one hundred one";
	dump_map_and_set((struct tree_struct *)(void*)&m, false);
	m[100] = "one hundred";
	dump_map_and_set((struct tree_struct *)(void*)&m, false);
	m[102] = "twelve";
	dump_map_and_set((struct tree_struct *)(void*)&m, false);
	m[107] = "one hundred seven";
	dump_map_and_set((struct tree_struct *)(void*)&m, false);
	m[0] = "zero";
	dump_map_and_set((struct tree_struct *)(void*)&m, false);
	m[9] = "4";
	dump_map_and_set((struct tree_struct *)(void*)&m, false);
	m[4] = "2";
	dump_map_and_set((struct tree_struct *)(void*)&m, false);
	m[5] = "ninety-nine";
	dump_map_and_set((struct tree_struct *)(void*)&m, false);
	m[5] = "five";
	dump_map_and_set((struct tree_struct *)(void*)&m, false);
	m[11] = "eleven";
	dump_map_and_set((struct tree_struct *)(void*)&m, false);
	m[1001] = "one thousand one";
	dump_map_and_set((struct tree_struct *)(void*)&m, false);
	m[1010] = "one thousand ten";
	dump_map_and_set((struct tree_struct *)(void*)&m, false);
	m[2] = "two";
	dump_map_and_set((struct tree_struct *)(void*)&m, false);
	m[9] = "nine";
	dump_map_and_set((struct tree_struct *)(void*)&m, false);
	//printf("dumping m as map:\n");
	*/


	SSH_TRACE;
	_func();
	return 0;
}
