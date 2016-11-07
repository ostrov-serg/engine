
#pragma once

#include "ssh_mem.h"

namespace ssh
{
	template <typename T> class List
	{
	public:
		struct Node
		{
			SSH_NEW_DECL(Node, 128);
			// ������������
			Node() : prev(nullptr), next(nullptr) {}
			Node(const T& t, Node* p, Node* n) : prev(p), next(n), value(t) {}
			// ����������
			~Node() { BaseNode<T, SSH_IS_PTR(T)>::release(value); }
			// ���������
			Node* next;
			// ����������
			Node* prev;
			// ��������
			T value;
		};
		// �����������
		List(int _ID) : ID(_ID) { }
		// ����������� �����
		List(int _ID, const List<T>& src) : ID(_ID) { *this = src; }
		// ����������� �� ������ �������������
		List(int _ID, const std::initializer_list<T>& _list) : ID(_ID) { for(auto& t : _list) insert(nlast, t); }
		// ����������� ��������
		List(List<T>&& src) { ID = src.ID; nroot = src.nroot; nlast = src.nlast; src.init(); }
		// ����������
		~List() { reset(); }
		// ����������
		Node* operator += (const T& t) { return insert(nlast, t); }
		const List& operator += (const List<T>& src) { auto n(src.root()); while(n) insert(nlast, n->value), n = n->next; return *this; }
		// ������������
		const List& operator = (const List<T>& src) { reset(); return operator += (src); }
		const List& operator = (List<T>&& src) { reset(); ID = src.ID; nroot = src.nroot; nlast = src.nlast; src.init(); return *this; }
		// �������
		Node* insert(Node* n, const T& t)
		{
			auto nn(n ? n->next : nroot);
			auto nd(new Node(t, n, nn));
			if(n) n->next = nd;
			if(nn) nn->prev = nd;
			if(nn == nroot) nroot = nd;
			if(n == nlast) nlast = nd;
			return nd;
		}
		// �������
		void remove(Node* nd)
		{
			if(nd)
			{
				auto n(nd->next);
				auto p(nd->prev);
				if(nd == nroot) nroot = n;
				if(nd == nlast) nlast = p;
				if(n) n->prev = p;
				if(p) p->next = n;
				delete nd;
			}
		}
		// ������� ������
		Node* root() const { return nroot; }
		// ������� ���������
		Node* last() const { return nlast; }
		// �����
		Node* find(const T& value) const { auto n(root()); while(n && n->value != value) n = n->next; return n; }
		// �����
		void reset() { auto n(nroot); while(nroot) { n = nroot->next; delete nroot; nroot = n; } init(); }
		// ������� ������� �������
		bool is_empty() const { return nroot == nullptr; }
	protected:
		// ��������
		void init() { nroot = nlast = nullptr; Node::reset(); }
		// �������������
		int ID = -1;
		// ������
		Node* nroot = nullptr;
		// ���������
		Node* nlast = nullptr;
	};
}
