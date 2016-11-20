
#pragma once

namespace ssh
{
	template <typename T> class List
	{
	public:
		struct Node
		{
			SSH_NEW_DECL(Node, 64);
			// конструктор
			Node(const T& t, Node* p, Node* n) : prev(p), next(n), value(t) {}
			// деструктор
			~Node() { SSH_RELEASE_NODE(T, value); }
			// следующий
			Node* next;
			// предыдущий
			Node* prev;
			// значение
			T value;
		};
		// конструктор
		List() { }
		// конструктор копии
		List(const List<T>& src) { *this = src; }
		// конструктор из списка инициализации
		List(const std::initializer_list<T>& _list) { for(auto& t : _list) add(t); }
		// конструктор переноса
		List(List<T>&& src) { nroot = src.nroot; nlast = src.nlast; src.free(); }
		// деструктор
		~List() { reset(); }
		// принудительное освобождение
		void free() { count = 0; nroot = nlast = nullptr; Node::reset(); }
		// количество узлов
		int size() const { return count; }
		// приращение
		Node* operator += (const T& t) { return add(t); }
		const List& operator += (const List<T>& src) { for(auto n : src) add(n); return *this; }
		// присваивание
		const List& operator = (const List<T>& src) { reset(); return operator += (src); }
		const List& operator = (List<T>&& src) { reset(); nroot = src.nroot; nlast = src.nlast; src.free(); return *this; }
		// добавить
		Node* add(const T& t)
		{
			auto n(new Node(t, nlast, nullptr));
			if(nroot) nlast->next = n; else nroot = n;
			count++;
			return (nlast = n);
		}
		// вставка(перед n)
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
				count--;
			}
		}
		// итерация по списку
		Iter<Node> begin() const { return Iter<Node>(nroot); }
		Iter<Node> end() const { return Iter<Node>(nullptr); }
		auto root() const { return nroot; }
		auto last() const { return nlast; }
		// сброс
		void reset() { while(nroot) { auto n(nroot->next); delete nroot; nroot = n; } free(); }
		// вернуть признак пустого
		bool is_empty() const { return (nroot == nullptr); }
	protected:
		// количество узлов
		int count = 0;
		// корень
		Node* nroot = nullptr;
		// последний
		Node* nlast = nullptr;
	};
}
