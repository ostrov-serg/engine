
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
			// конструкторы
			Node() : prev(nullptr), next(nullptr) {}
			Node(const T& t, Node* p, Node* n) : prev(p), next(n), value(t) {}
			// деструктор
			~Node() { BaseNode<T, SSH_IS_PTR(T)>::release(value); }
			// следующий
			Node* next;
			// предыдущий
			Node* prev;
			// значение
			T value;
		};
		// конструктор
		List(int _ID) : ID(_ID) { }
		// конструктор копии
		List(int _ID, const List<T>& src) : ID(_ID) { *this = src; }
		// конструктор из списка инициализации
		List(int _ID, const std::initializer_list<T>& _list) : ID(_ID) { for(auto& t : _list) insert(nlast, t); }
		// конструктор переноса
		List(List<T>&& src) { ID = src.ID; nroot = src.nroot; nlast = src.nlast; src.init(); }
		// деструктор
		~List() { reset(); }
		// приращение
		Node* operator += (const T& t) { return insert(nlast, t); }
		const List& operator += (const List<T>& src) { auto n(src.root()); while(n) insert(nlast, n->value), n = n->next; return *this; }
		// присваивание
		const List& operator = (const List<T>& src) { reset(); return operator += (src); }
		const List& operator = (List<T>&& src) { reset(); ID = src.ID; nroot = src.nroot; nlast = src.nlast; src.init(); return *this; }
		// вставка
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
		// удалить
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
		// вернуть первый
		Node* root() const { return nroot; }
		// вернуть последний
		Node* last() const { return nlast; }
		// найти
		Node* find(const T& value) const { auto n(root()); while(n && n->value != value) n = n->next; return n; }
		// сброс
		void reset() { auto n(nroot); while(nroot) { n = nroot->next; delete nroot; nroot = n; } init(); }
		// вернуть признак пустого
		bool is_empty() const { return nroot == nullptr; }
	protected:
		// очистить
		void init() { nroot = nlast = nullptr; Node::reset(); }
		// идентификатор
		int ID = -1;
		// корень
		Node* nroot = nullptr;
		// последний
		Node* nlast = nullptr;
	};
}
