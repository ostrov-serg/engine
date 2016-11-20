
#include "stdafx.h"
#include "ssh_tracer.h"
#include "ssh_log.h"

namespace ssh
{
	void StackTrace::add(bool is, ssh_cws func, ssh_cws file, int line)
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

	void StackTrace::output()
	{
		if(elems.size())
		{
			ssh_log->add_msg(ssh_printf(L"\r\n\r\n-------------------------------------------------- Трассировка стека (%i вызовов) --------------------------------------------------\r\n\r\n", elems.size() / 2));
			for(auto n : elems) ssh_log->add_msg(n);
			ssh_log->add_msg(L"\r\n\r\n--------------------------------------------------------- Трассировка стека -------------------------------------------------------\r\n\r\n");
		}
		elems.reset();
	}
}