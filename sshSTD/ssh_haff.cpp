
#include "stdafx.h"
#include "ssh_compress.h"

namespace ssh
{
	void Haffman::print_haff(Haffman::node* n, int tabs, bool is_tree)
	{
#ifdef _DEBUG
		if(is_tree)
		{
			if(n->val) ssh_log->add_msg(ssh_printf(L"%3i[%i] - %i{%*I64b}", (n->val - vals), n->freq, n->val->len, n->val->len, n->val->val), false);
			ssh_log->add_msg(L"\r\n", false);
			String t(L'\t', tabs * 2);
			if(n->r)
			{
				ssh_log->add_msg(ssh_printf(L"%zR-------", t), false);
				print_haff(n->r, tabs + 1, is_tree);
			}
			if(n->l)
			{
				ssh_log->add_msg(ssh_printf(L"%zL-------", t), false);
				print_haff(n->l, tabs + 1, is_tree);
			}
		}
		else
		{
			if(n->val) ssh_log->add_msg(ssh_printf(L"%3i[%i] - %i{%z}\r\n", (n->val - vals), n->freq, n->val->len, n->val->val), false);
			if(n->r) print_haff(n->r, tabs + 1, is_tree);
			if(n->l) print_haff(n->l, tabs + 1, is_tree);
		}
#endif
	}

	static void make_code(Haffman::node* n, ssh_u val, ssh_w len) noexcept
	{
		if(n->val)
		{
			n->val->val = (val >> (64 - len));
			n->val->len = len;
		}
		if(n->l) make_code(n->l, val >> 1, len + 1);
		if(n->r) make_code(n->r, (val >> 1) | 0x8000000000000000, len + 1);
	}

	void Haffman::init() noexcept
	{
		nodes.reset();
		if(head)
		{
			head->reset();
			head = nullptr;
		}
		in = out = nullptr;
		ssh_memzero(vals, sizeof(vals));
	}
		
	Buffer Haffman::process(const Buffer& _in, bool is_compress) noexcept
	{
		init();
		in = _in;
		if(is_compress)
			return compress(_in.size());
		return decompress();
	}

	void Haffman::make_tree(int size) noexcept
	{
		node* n;
		// 1. считаем частоты вхождения символов и их количество
		for(ssh_u i = 0; i < size; i++)
		{
			int v(in[i]);
			if(!vals[v].ref)
			{
				n = nodes += new node();
				vals[v].ref = n;
				n->val = &vals[v];
			}
			vals[v].ref->freq++;
		}
		ssh_u count(nodes.size());
		// 2. сортируем алфавит по возрастанию
		qsort(nodes.get_data(), count, sizeof(node*), [](const void* _1, const void* _2) -> int
		{
			node* __1(*(node**)_1), *__2(*(node**)_2);
			if(__1->freq < __2->freq) return -1;
			return 1;
		});
		// 3. строим двоичное дерево
		for(int i = 0; i < count / 2; i++)
		{
			auto n1(nodes[i * 2]);
			auto n2(nodes[i * 2 + 1]);
			if(n1->freq > n2->freq) std::swap(n1, n2);
			head = new node(n1, n2, n1->freq + n2->freq);
			int j(i);
			// вставить новый узел с учетом сортировки
			for(; j < count; j++)
			{
				if(nodes[j]->freq >= head->freq) break;
			}
			nodes.insert(j, head);
			count++;
		}
		// 4. составить код Хаффмана
		make_code(head, 0, 0);
		// 5. проверить как создалось дерево
		if(head)
		{
			//			ssh_log->add_msg(ssh_printf(L"root-------\r\n"), false);
			//			print_haff(head, 1, true);
		}
	}

	Buffer Haffman::compress(ssh_u size) noexcept
	{
		int sh(0);
		// построить дерево
		make_tree((int)size);
		// новый буфер
		Buffer _out(size * 2 + sizeof(int)); out = _out; *(int*)out = (int)size;
		ssh_b* _out1(out); out += sizeof(int);
		// запомнить дерево
		store_tree();
		// закодировать
		for(ssh_u i = 0; i < size; i++) vals[in[i]].write(&out, sh);
		// вернуть буфер
		return Buffer(_out, 0, out - _out1);
	}

	Buffer Haffman::decompress() noexcept
	{
		value* v;
		int sh(0), size(*(int*)in);
		// новый буфер
		Buffer _out(size); in += sizeof(int); out = _out;
		// прочитать дерево из буфера
		restore_tree();
		// раскодировать
		for(int i = 0; i < size; i++)
		{
			ssh_w val((*(int*)out) >> sh);
			v = read(val);
			*out = (ssh_b)(v - vals);
			out += ((v->len + sh) / 8);
			sh = (v->len + sh) & 7;
		}
		return _out;
	}

	void Haffman::store_tree() noexcept
	{
		// ?0 - left
		// ?1 - right
		// 1? - value exist
		// xxxxxxxx - val
	}

	void Haffman::restore_tree() noexcept
	{
	}

	void Haffman::value::write(ssh_b ** p, int & shift) noexcept
	{
		*(ssh_w*)(*p) |= (val & ((1 << len) - 1)) << shift;
		*p += ((shift + len) / 8);
		shift = (shift + len) & 7;
	}

	Haffman::value * Haffman::read(long val) noexcept
	{
		node* nn, *n(head);
		int bit(0);
		do { nn = n; if(_bittest(&val, bit++)) n = n->r; else n = n->l; } while(n);
		return nn->val;
	}
}