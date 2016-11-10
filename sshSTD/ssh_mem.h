
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
		// выделение памяти
		void* alloc(ssh_u sz);
		// освобождение
		void free(ssh_b* p);
		// вернуть информацию об сеансе работы менеджера
		void output();
		// вернуть признак блокировки
		bool is_started() const { return is_enabled; }
		// запуск/останов
		void start() { is_enabled = true; }
		void stop() { is_enabled = false; }
	protected:
		// конструктор
//		MemMgr() {}
		// деструктор
		~MemMgr();
		// вернуть массив не освобожденных узлов
		void leaks();
		// максимум выделенных
		ssh_u total_alloc = 0;
		// всего было освобождено
		ssh_u total_free = 0;
		// максимум единовременно выделенной памяти
		ssh_u use_mem = 0, use_max_mem = 0;
		// признак активности
		bool is_enabled = false;
		// список блоков памяти
		static List<Block> blocks;
	};
}

inline void* operator new(ssh_u sz){return ssh::MemMgr::instance()->alloc(sz);}
inline void operator delete(void* p) { ssh::MemMgr::instance()->free((ssh_b*)p); }
inline void* operator new[](ssh_u sz) { return ssh::MemMgr::instance()->alloc(sz); }
inline void operator delete[](void* p) { ssh::MemMgr::instance()->free((ssh_b*)p); }
