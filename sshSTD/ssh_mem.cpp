
#include "stdafx.h"
#include "ssh_mem.h"
#include "ssh_list.h"

namespace ssh
{
	List<MemMgr::Block> MemMgr::blocks(1);
	typedef List<MemMgr::Block>::Node NodeMem;

	MemMgr::~MemMgr() { blocks.reset(); }

	void MemMgr::leaks()
	{
		Section cs;

		if(!blocks.is_empty())
		{
			String txt;

//			log->add_msg(String::fmt(L"Обнаружено %I64i потерянных блоков памяти...\r\n", count));

			auto fix(blocks.root());
			while(fix)
			{
				Block* blk(&fix->value);
				String bytes;// (ssh_make_hex_string<ssh_b>(nd->ptr, nd->sz > 48 ? 48 : n->sz, txt, n->sz > 48));
//				log->add_msg(String::fmt(L"node <0x%X, %i, <%s>, <%s>", nd->ptr, nd->sz, bytes, txt));
				fix = fix->next;
			}
		}
	}

	void MemMgr::output()
	{
		leaks();
//		log->add_msg(String::fmt(L"\r\nЗа данный сеанс было выделено %i(~%s) байт памяти ..., освобождено %i(~%s) ...:%c, максимум - %i блоков\r\n", use_max_mem, ssh_num_volume(use_max_mem), total_free, ssh_num_volume(total_free), (use_mem != total_free ? L'(' : L')'), total_alloc));
	}

	void* MemMgr::alloc(ssh_u sz)
	{
		Section cs;

		char* p((char*)::malloc(sz + sizeof(NodeMem*)));
		NodeMem* nd(nullptr);
		if(is_enabled)
		{
			nd = (blocks += Block((ssh_u)(p + sizeof(NodeMem*)), sz));
			total_alloc++;
			use_mem += sz;
			if(use_max_mem < use_mem) use_max_mem = use_mem;
		}
		*(NodeMem**)p = nd;
		return (void*)(p + sizeof(NodeMem*));
	}

	void MemMgr::free(ssh_b* p)
	{
		Section cs;

		auto nd(*(NodeMem**)(p - sizeof(NodeMem*)));
		if(nd)
		{
			ssh_u sz(nd->value.sz);
#ifdef _DEBUG
			memset(p, 0xaa, sz);
#endif
			// корректируем статистику
			total_free += sz;
			use_mem -= sz;
			blocks.remove(nd);
		}
	}
}