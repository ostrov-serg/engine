
#pragma once

namespace ssh
{
	class SSH MemMgr
	{
	public:
		struct NodeMem
		{
			// �����������
			NodeMem(int _sz, NodeMem* nn) : next(nn), prev(nullptr), sz(_sz), use(1) {}
			// ������� ��������������� �����
			int use;
			// ������ �����
			int sz;
			// ���������
			NodeMem* next;
			// ����������
			NodeMem* prev;
		};
		static MemMgr* instance() { static MemMgr mem; return &mem; }
		// ��������� �������� ����������
		bool fault(int type, ssh_cws fn, ssh_cws fl, int ln, EXCEPTION_POINTERS* except = nullptr, ssh_cws msg_ex = nullptr);
		// ������� ���������� �� ������ ������ ���������
		void output();
		// ��������� ������
		void* alloc(ssh_u sz);
		// ������������
		void free(ssh_b* p);
		// ������� ������� ����������
		bool is_started() const { return is_enabled; }
		// ���������� ������� ����������
		void start() { is_enabled = false; }
		void stop() { is_enabled = true; }
	protected:
		// ������� �� ������������� �����
		void leaks();
		// �������� ����������
		ssh_u total_alloc = 0;
		// ����� ���� �����������
		ssh_u total_free = 0;
		// �������� ������������� ���������� ������
		ssh_u use_mem = 0, use_max_mem = 0;
		// ������� ����������
		bool is_enabled = true;
		// �������� ����
		NodeMem* root = nullptr;
	};
}

inline void* operator new(ssh_u sz){return ssh::MemMgr::instance()->alloc(sz);}
inline void operator delete(void* p) { ssh::MemMgr::instance()->free((ssh_b*)p); }
inline void* operator new[](ssh_u sz) { return ssh::MemMgr::instance()->alloc(sz); }
inline void operator delete[](void* p) { ssh::MemMgr::instance()->free((ssh_b*)p); }
