
#pragma once

#include "ssh_list.h"
#include "ssh_str.h"
#include "ssh_log.h"

namespace ssh
{
	class SSH StackTrace final
	{
	public:
		// ������
		static StackTrace* instance() { static StackTrace tracer; return &tracer; }
		// ���������������� ������������
		void init(int _depth) { depth = _depth; }
		// �������� ����� �������
		void add(bool is, ssh_cws func, ssh_cws file = nullptr, int line = 0)
		{
			stop();
			if(!is) indent--;
			if(elems.size() >= depth) elems.remove(elems.last());
			if(indent < 0) indent = 0;
			String _indent(L' ', indent * 2);
			ssh_ws _is(is ? L'+' : L'-');
			elems += (file ? ssh_printf(L"%s%c%s()  -  (%s:%i)", _indent, _is, func, file, line) : ssh_printf(L"%s%c%s()", _indent, _is, func)); //-V510
			if(is) indent++;
			start();
		}
		// ������� � ���
		void output()
		{
#ifdef _DEBUG
			ssh_log->add_msg(String::fmt(L"\r\n\r\n-------------------------------------------------- ����������� ����� (%i �������) --------------------------------------------------\r\n\r\n", elems.size()));
			for(auto n : elems) ssh_log->add_msg(n);
			ssh_log->add_msg(L"\r\n\r\n--------------------------------------------------------- ����������� ����� -------------------------------------------------------\r\n\r\n");
#endif
			elems.reset();
		}
		// ������/���������
		void start() { is_enabled = true; }
		void stop() { is_enabled = false; }
		// ������� ����������
		bool is_started() const { return is_enabled; }
	protected:
		// ������� ����������
		bool is_enabled = false;
		// ���� �������
		int depth = 256;
		// ������
		int indent = 0;
		// ������
		List<String> elems;
	};

	class SSH Tracer
	{
	public:
		Tracer(ssh_cws _fn, ssh_cws _fl, int _ln) : fn(_fn)
		{
			if(ssh_trc->is_started()) ssh_trc->add(true, _fn, _fl, _ln);
		}
		~Tracer()
		{
			if(ssh_trc->is_started()) ssh_trc->add(false, fn);
		}
	protected:
		ssh_cws fn;
	};
}