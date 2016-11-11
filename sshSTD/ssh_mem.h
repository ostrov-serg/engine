
#pragma once

namespace ssh
{
	class SSH MemMgr
	{
	public:
		struct NodeMem
		{
			// конструктор
			NodeMem(int _sz, NodeMem* nn) : next(nn), prev(nullptr), sz(_sz), use(1) {}
			// признак использованного блока
			int use;
			// размер блока
			int sz;
			// следующий
			NodeMem* next;
			// предыдущий
			NodeMem* prev;
		};
		static MemMgr* instance() { static MemMgr mem; return &mem; }
		// обработка внешнего исключения
		bool fault(int type, ssh_cws fn, ssh_cws fl, int ln, EXCEPTION_POINTERS* except = nullptr, ssh_cws msg_ex = nullptr);
		// вернуть информацию об сеансе работы менеджера
		void output();
		// выделение памяти
		void* alloc(ssh_u sz);
		// освобождение
		void free(ssh_b* p);
		// вернуть признак блокировки
		bool is_started() const { return is_enabled; }
		// установить признак блокировки
		void start() { is_enabled = false; }
		void stop() { is_enabled = true; }
	protected:
		// вернуть не освобожденные блоки
		void leaks();
		// максимум выделенных
		ssh_u total_alloc = 0;
		// всего было освобождено
		ssh_u total_free = 0;
		// максимум единовременно выделенной памяти
		ssh_u use_mem = 0, use_max_mem = 0;
		// признак блокировки
		bool is_enabled = true;
		// корневой блок
		NodeMem* root = nullptr;
	};
}

inline void* operator new(ssh_u sz){return ssh::MemMgr::instance()->alloc(sz);}
inline void operator delete(void* p) { ssh::MemMgr::instance()->free((ssh_b*)p); }
inline void* operator new[](ssh_u sz) { return ssh::MemMgr::instance()->alloc(sz); }
inline void operator delete[](void* p) { ssh::MemMgr::instance()->free((ssh_b*)p); }
