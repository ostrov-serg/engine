
#pragma once

namespace ssh
{
	template <typename T, ssh_u N = 128> class MemArray
	{
	public:
		struct Block
		{
			union
			{
				Block* next;
				ssh_b t[sizeof(T)];
			};
		};

		struct BlockFix
		{
			BlockFix() : next(nullptr) {}
			~BlockFix() { SSH_DEL(next); }
			BlockFix* next;
			Block arr[N];
		};
		MemArray() : free(nullptr), arrs(nullptr), count(0) {}
		~MemArray() { Reset(); }
		void Reset() { SSH_DEL(arrs); free = nullptr; }
		T* Alloc()
		{
			if(!free)
			{
				BlockFix* tmp(new BlockFix);
				memset(tmp->arr, 0, sizeof(Block) * N);
				tmp->next = arrs; arrs = tmp;
				for(ssh_u i = 0; i < N; i++)
				{
					arrs->arr[i].next = free;
					free = &(arrs->arr[i]);
				}
			}
			count++;
			Block* b(free);
			free = free->next;
			::new((T*)(b->t)) T();
			return (T*)(b->t);
		}
		void Free(T* t)
		{
			Block* b((Block*)t);
			t->~T();
			b->next = free;
			free = b;
			count--;
		}
		int count;
		Block* free;
		BlockFix* arrs;
	};

	template <typename T> class List;

	class SSH MemMgr
	{
	public:
		struct Block
		{
			Block() : ptr(0), sz(0) {}
			Block(ssh_u _ptr, ssh_u _sz) : ptr(_ptr), sz(_sz) {}
			ssh_u ptr;
			ssh_u sz;
		};
		static MemMgr* instance() { static MemMgr mem; return &mem; }
		// ��������� ������
		void* alloc(ssh_u sz);
		// ������������
		void free(ssh_b* p);
		// ������� ���������� �� ������ ������ ���������
		void output();
		// ������� ������� ����������
		bool is_started() const { return is_enabled; }
		// ������/�������
		void start() { is_enabled = true; }
		void stop() { is_enabled = false; }
	protected:
		// �����������
//		MemMgr() {}
		// ����������
		~MemMgr();
		// ������� ������ �� ������������� �����
		void leaks();
		// �������� ����������
		ssh_u total_alloc = 0;
		// ����� ���� �����������
		ssh_u total_free = 0;
		// �������� ������������� ���������� ������
		ssh_u use_mem = 0, use_max_mem = 0;
		// ������� ����������
		bool is_enabled = false;
		// ������ ������ ������
		static List<Block> blocks;
	};
}

inline void* operator new(ssh_u sz){return ssh::MemMgr::instance()->alloc(sz);}
inline void operator delete(void* p) { ssh::MemMgr::instance()->free((ssh_b*)p); }
inline void* operator new[](ssh_u sz) { return ssh::MemMgr::instance()->alloc(sz); }
inline void operator delete[](void* p) { ssh::MemMgr::instance()->free((ssh_b*)p); }
