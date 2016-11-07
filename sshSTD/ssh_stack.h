
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
			// ����������� ����
			Node() : next(nullptr) {}
			// ����������� ����
			Node(const T& t, Node* n) : next(n), value(t) {}
			// ����������
			~Node() { BaseNode<T, SSH_IS_PTR(T)>::release(root->value); }
			// ��������� ����
			Node* next;
			// ��������
			T value;
		};
		// ������������
		Stack() { }
		Stack(int _ID) : ID(_ID) { }
		// ���������������� �����������
		Stack(int _ID, const std::initializer_list<T>& _list) : ID(_ID) { for(auto& t : _list) operator += (t); }
		// ����������� ��������
		Stack(Stack&& src) { ID = src.ID; root = src.root; src.init(); }
		// ����������
		~Stack() { reset(); }
		// �������� ��������
		const Stack& operator = (Stack&& src) { reset(); ID = src.ID; root = src.root; src.init(); return *this; }
		// ���������� ��������
		Node* operator += (const T& t) { return (root = new Node(t, root)); }
		// ����������
		T pop()
		{
			if(!root) return BaseNode<T, SSH_IS_PTR(T)>::dummy();
			T t(root->value);
			auto n(root->next);
			delete root;
			root = n;
			return t;
		}
		// ������������ �����
		void reset() { auto n(root); while(root) { n = n->next; delete root; root = n; } init(); }
		// �������� �� ������� ���������
		bool is_empty() const {return (root == nullptr);}
	protected:
		void init() { root = nullptr; Node::reset(); }
		// �������������
		int ID = -1;
		// �������
		Node* root = nullptr;
	};
}
