
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

#pragma pack(push, 8)
struct tree_node
{
	struct tree_node *Left;
	struct tree_node *Parent;
	struct tree_node *Right;
	char Color; // 0 - Red, 1 - Black
	char Isnil;
	// ssh_w _ssh_w;
	int _int;
	//std::pair Myval;
	int first; // called Myval in std::set
	ssh_ccs second; // not present in std::set
};
struct tree_struct
{
	ssh_u null;
	struct tree_node *Myhead;
	size_t Mysize;
}; 

void dump_as_tree(int tabs, struct tree_node *n)
{
	ssh_log->add_msg(ssh_printf(L"%i %C\n", n->first, n->Color + 48), false);
	String t(L'\t', tabs * 2);
	if(n->Left->Isnil == 0)
	{
		ssh_log->add_msg(ssh_printf(L"%zL-------", t), false);
		dump_as_tree(tabs + 1, n->Left);
	};
	if(n->Right->Isnil == 0)
	{
		ssh_log->add_msg(ssh_printf(L"%zR-------", t), false);
		dump_as_tree(tabs + 1, n->Right);
	};
};

#include <list>
#include <map>
#include <forward_list>

void dump_map_and_set(struct tree_struct *m, bool is_set)
{
	ssh_log->add_msg(ssh_printf(L"root----"), false);
	dump_as_tree(1, m->Myhead->Parent);
	ssh_log->add_msg(ssh_printf(L"\r\n\r\n"), false);
};

class xtree
{
public:
	enum caps { red, black };
	struct node
	{
		node() {}
		node(node* p, node* l, node* r, caps _c, int v) : parent(p), left(l), right(r), c(_c), val(v) { }
		~node() {}
		int val = 0;
		caps c;
		node* parent = nullptr;
		node* left = nullptr;
		node* right = nullptr;
		int count = 0;
	};
	void insert(int v)
	{
		if(!head)
		{
			head = new node(nullptr, nullptr, nullptr, red, v);
		}
		else
		{
			_insert(head, 0, v);
		}
	}
protected:
	void swap(node* n, int v)
	{
		auto ll(n->left);
		auto rr(n->right);
		auto pp(n->parent);
		node* nn;
		if(n->c == red)
		{
			if(v > n->val) n->right = nullptr, ll = n; else n->left = nullptr, rr = n;
			nn = new node(pp, ll, rr, n->c, v);
			if(pp) pp->left = nn;
		}
		else
		{
			if(v < n->val) n->right = nullptr, ll = n; else n->left = nullptr, rr = n;
			nn = new node(pp, ll, rr, n->c, v);
			if(pp) pp->right = nn;
		}
		if(rr) rr->parent = nn;
		if(ll) ll->parent = nn;
		n->parent = nn;
	}
	void _insert(node* n, int c, int v)
	{
		auto pp(n->parent);
		if(n->val == v) return;
		else if(v > n->val)
		{
			if(n->right)
			{
				_insert(n->right, c + 1, v);
			}
			else
			{
				// проверить на родителя
				if(c > 2 && pp->val > v)
					swap(pp, v);
				else
					n->right = new node(n, nullptr, nullptr, black, v);
			}
		}
		else
		{
			if(n->left)
			{
				_insert(n->left, c + 1, v);
			}
			else
			{
				if(c > 2 && pp->val < v)
					swap(pp, v);
				// проверить на родителя
				n->left = new node(n, nullptr, nullptr, red, v);
			}
		}
	}
	node* head = nullptr;
};


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

class xTree
{
public:
	struct node
	{
		node(node* p, int v) : parent(p), val(v) {}
		int val;
		node* parent;
		List<node*> child;
	};
	xTree() {}
	void set(node* n, int v) noexcept
	{
//		SSH_RELEASE_NODE(T, n->value);
		n->val = v;
	}
	auto get_root() const noexcept { return root; }
	auto insert(node* p, int v) noexcept { return insert(p, new node(p, v)); }
	auto insert(node* p, const xTree& tree) noexcept { return insert(p, tree.get_root()); }
	auto add(node* p, int v) noexcept { return add(p, new node(p, v)); }
	auto add(node* p, const xTree& tree) noexcept { return add(p, tree.get_root()); }
	void remove(node* n) noexcept
	{
		if(n == root)
		{
			delete root;
			root = nullptr;
		}
		else if(n)
		{
			auto p(n->parent);
			p->child.remove(p->child.get_node_value(n));
		}
	}
	void move(node* n1, node* n2) noexcept
	{
		if(is_move(n1, n2))
		{

		}
	}
protected:
	node* add(node* p, node* n) noexcept
	{
		if(p) p->child.add(n);
		else if(!root) root = n;
		return n;
	}
	node* insert(node* p, node* n) noexcept
	{
		if(p) p->child.insert(n);
		else if(!root) root = n;
		return n;
	}
	// возможно ли перемещение узла? n1 перенести в n2
	bool is_move(node* n1, node* n2) const noexcept
	{
		// проверить находится ли n2 в n1
		return false;
	}

	node* root = nullptr;
};
#include <regex>

static int ssh_AVX_hash(ssh_cws _cws)
{
	int hash(0);
	while(*_cws) hash = _mm_crc32_u16(hash, *_cws++);
	return hash;
}

int main() noexcept
{
	int hash1 = ssh_AVX_hash(L"Сергей Шаталов");
	int hash2 = ssh_hash(L"Сергей Шаталов");
	String _f_f(hash1, Radix::_bin);
	String _f_f1(hash2, Radix::_bin);
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
