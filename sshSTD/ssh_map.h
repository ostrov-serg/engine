
#pragma once

#include "ssh_mem.h"

namespace ssh
{
	template <typename T, typename K> class Map
	{
	public:
		struct Node
		{
			SSH_NEW_DECL(Node, 128);
			// ������������
			Node() : next(nullptr) {}
			Node(const K& k, const T& t, Node* n) : next(n), key(k), value(t) {}
			// ����������
			~Node() { BaseNode<T, SSH_IS_PTR(T)>::release(n->value); BaseNode<K, SSH_IS_PTR(K)>::release(n->key); }
			// �����
			K key;
			// ��������
			T value;
			// ��������� ����
			Node* next;
		};
	protected:
		class Cursor
		{
		public:
			// ���������� ������
			Cursor(Map<T, K>* arr, const K& k) : node(new Node(k, T(), arr->cells)) { arr->cells = node; }
			// �������
			Cursor(Node* n) : node(n){}
			Cursor& operator = (const T& value) { BaseNode<T, SSH_IS_PTR(T)>::release(node->value); node->value = value; return *this; }
			operator T() const { return node->value; }
			T operator->() const { return node->value; }
		protected:
			// ����
			Node* node;
		};
	public:
		// ����������� �� ���������
		Map() { }
		Map(int _ID) : ID(_ID) { }
		// ����������� �����
		Map(int _ID, const Map<T, K>& src) : ID(_ID) { *this = src; }
		// ����������� ��������
		Map(Map<T, K>&& src) { ID = src.ID; cells = src.cells; src.init(); }
		// ����������
		~Map() { reset(); }
		// ������������
		const Map& operator = (const Map<T, K>& src) { reset(); return operator += (src); }
		const Map& operator = (Map<T, K>&& src) { reset(); ID = src.ID; cells = src.cells; src.init(); return *this; }
		// ����������
		const Map& operator += (const Map<T, K>& src) { auto n(src.root()); while(n) { operator[](n->key) = n->value; n = n->next; } return *this; }
		// ���������� ���������
		ssh_u count() const { ssh_u c(0); auto n(cells); while(n) n = n->next, c++; return c; }
		// ���������/�������
		Cursor operator[](const K& key) { auto n(cells); while(n) { if(n->key == key) return Cursor(n); n = n->next; } return Cursor(this, key); }
		// ������� ��� �����
		Map<K, ssh_u> keys() const { auto n(cells); Map<K, ssh_u> keys; ssh_u i(0); while(n) { keys[i++] = n->key; n = n->next; } return keys; }
		// ������� ����� �� ��������
		K get_key(const T& value) const { auto n(cells); while(n) { if(n->value == value) return n->key; n = n->next; } return BaseNode<K, SSH_IS_PTR(K)>::dummy(); }
		// �������� - ����� ���� ����������?
		bool is_key(const K& key) const { auto n(cells); while(n) { if(n->key == key) return true; n = n->next; } return false; }
		// �������� ��������
		void remove(const K& key)
		{
			Node* n, *p;
			if((n = get_key(key, &p)))
			{
				auto nn(n->next); n->next = nullptr;
				cells == n ? cells = nn : p->next = nn;
				delete n;
			}
		}
		// ������� ������
		Node* root() const { return cells; }
		// ������� ������
		bool is_empty() const { return (cells == nullptr); }
		// �������� �����
		void reset() { auto n(cells); while(cells) { n = n->next; delete cells; cells = n; } init(); }
	protected:
		// ��������
		void init() { cells = nullptr; Node::reset(); }
		// ������� ���� �� �����
		Node* get_key(const K& key, Node** p) const { auto n(cells); while(n) {if(n->key == key) return n; if(p) *p = n; n = n->next;} return nullptr; }
		// �������������
		int ID = -1;
		// �������� �������
		Node* cells = nullre;
	};
}
