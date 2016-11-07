
#pragma once

#include "ssh_mem.h"

namespace ssh
{
	template <typename T> class Stack
	{
		Stack(const Stack& src) {}
		const Stack& operator = (const Stack& src) { return *this; }
	public:
		struct Node
		{
			SSH_NEW_DECL(Node, 128);
			// конструктор узла
			Node() : next(nullptr) {}
			// конструктор узла
			Node(const T& t, Node* n) : next(n), value(t) {}
			// деструктор
			~Node() { BaseNode<T, SSH_IS_PTR(T)>::release(root->value); }
			// следующий узел
			Node* next;
			// значение
			T value;
		};
		// конструкторы
		Stack() { }
		Stack(int _ID) : ID(_ID) { }
		// инициализирующий конструктор
		Stack(int _ID, const std::initializer_list<T>& _list) : ID(_ID) { for(auto& t : _list) operator += (t); }
		// конструктор переноса
		Stack(Stack&& src) { ID = src.ID; root = src.root; src.init(); }
		// деструктор
		~Stack() { reset(); }
		// оператор переноса
		const Stack& operator = (Stack&& src) { reset(); ID = src.ID; root = src.root; src.init(); return *this; }
		// добавление элемента
		Node* operator += (const T& t) { return (root = new Node(t, root)); }
		// извлечение
		T pop()
		{
			if(!root) return BaseNode<T, SSH_IS_PTR(T)>::dummy();
			T t(root->value);
			auto n(root->next);
			delete root;
			root = n;
			return t;
		}
		// освобождение стэка
		void reset() { auto n(root); while(root) { n = n->next; delete root; root = n; } init(); }
		// проверка на наличие элементов
		bool is_empty() const {return (root == nullptr);}
	protected:
		void init() { root = nullptr; Node::reset(); }
		// идентификатор
		int ID = -1;
		// вершина
		Node* root = nullptr;
	};
}
