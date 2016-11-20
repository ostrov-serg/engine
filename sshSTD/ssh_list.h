
#pragma once

namespace ssh
{
	template <typename T> class List
	{
	public:
		struct Node
		{
			SSH_NEW_DECL(Node, 64);
			// �����������
			Node(const T& t, Node* p, Node* n) : prev(p), next(n), value(t) {}
			// ����������
			~Node() { SSH_RELEASE_NODE(T, value); }
			// ���������
			Node* next;
			// ����������
			Node* prev;
			// ��������
			T value;
		};
		// �����������
		List() { }
		// ����������� �����
		List(const List<T>& src) { *this = src; }
		// ����������� �� ������ �������������
		List(const std::initializer_list<T>& _list) { for(auto& t : _list) add(t); }
		// ����������� ��������
		List(List<T>&& src) { nroot = src.nroot; nlast = src.nlast; src.free(); }
		// ����������
		~List() { reset(); }
		// �������������� ������������
		void free() { count = 0; nroot = nlast = nullptr; Node::reset(); }
		// ���������� �����
		int size() const { return count; }
		// ����������
		Node* operator += (const T& t) { return add(t); }
		const List& operator += (const List<T>& src) { for(auto n : src) add(n); return *this; }
		// ������������
		const List& operator = (const List<T>& src) { reset(); return operator += (src); }
		const List& operator = (List<T>&& src) { reset(); nroot = src.nroot; nlast = src.nlast; src.free(); return *this; }
		// ��������
		Node* add(const T& t)
		{
			auto n(new Node(t, nlast, nullptr));
			if(nroot) nlast->next = n; else nroot = n;
			count++;
			return (nlast = n);
		}
		// �������(����� n)
		Node* insert(Node* n, const T& t)
		{
			if(!n) n = nroot;
			auto np(n ? n->prev : nullptr);
			auto nd(new Node(t, np, n));
			if(np) np->next = nd;
			if(n) n->prev = nd;
			if(n == nroot) nroot = nd;
			if(!nlast) nlast = nd;
			count++;
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
				count--;
			}
		}
		// �������� �� ������
		Iter<Node> begin() const { return Iter<Node>(nroot); }
		Iter<Node> end() const { return Iter<Node>(nullptr); }
		auto root() const { return nroot; }
		auto last() const { return nlast; }
		// �����
		void reset() { while(nroot) { auto n(nroot->next); delete nroot; nroot = n; } free(); }
		// ������� ������� �������
		bool is_empty() const { return (nroot == nullptr); }
	protected:
		// ���������� �����
		int count = 0;
		// ������
		Node* nroot = nullptr;
		// ���������
		Node* nlast = nullptr;
	};
}
