
#pragma once

namespace ssh
{
	template <typename T> class List
	{
	public:
		struct Node
		{
			SSH_NEW_DECL(Node, 128);
			// конструктор
			Node(const T& t, Node* p, Node* n) : prev(p), next(n), value(t) {}
			// деструктор
			~Node() { release_node<T, SSH_IS_PTR(T)>::release(value); }
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
		List(const std::initializer_list<T>& _list) { for(auto& t : _list) insert(last, t); }
		// конструктор переноса
		List(List<T>&& src) { root = src.root; last = src.last; src.free(); }
		// деструктор
		~List() { reset(); }
		// принудительное освобождение
		void free() { root = last = nullptr; Node::reset(); }
		// приращение
		Node* operator += (const T& t) { return insert(last, t); }
		const List& operator += (const List<T>& src) { for(auto n : src) insert(last, n); return *this; }
		// присваивание
		const List& operator = (const List<T>& src) { reset(); return operator += (src); }
		const List& operator = (List<T>&& src) { reset(); root = src.root; last = src.last; src.free(); return *this; }
		// вставка
		Node* insert(Node* n, const T& t)
		{
			auto nn(n ? n->next : root);
			auto nd(new Node(t, n, nn));
			if(n) n->next = nd;
			if(nn) nn->prev = nd;
			if(nn == root) root = nd;
			if(n == last) last = nd;
			return nd;
		}
		// удалить
		void remove(Node* nd)
		{
			if(nd)
			{
				auto n(nd->next);
				auto p(nd->prev);
				if(nd == root) root = n;
				if(nd == last) last = p;
				if(n) n->prev = p;
				if(p) p->next = n;
				delete nd;
			}
		}
		// итерация по списку
		Iter<Node> begin() const { return Iter<Node>(root); }
		Iter<Node> end() const { return Iter<Node>(nullptr); }
		// сброс
		void reset() { auto n(root); while(root) { n = root->next; delete root; root = n; } free(); }
		// вернуть признак пустого
		bool is_empty() const { return (root == nullptr); }
	protected:
		// корень
		Node* root = nullptr;
		// последний
		Node* last = nullptr;
	};
}
