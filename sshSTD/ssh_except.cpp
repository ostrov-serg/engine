
#include "stdafx.h"
#include "ssh_except.h"

namespace ssh
{
	void Exception::out_log(String msg) const
	{
		if(!msg.is_empty()) msg += L", ";
		// добавляем в лог
		ssh_log->add(Log::exception, func, file, line, msg + message);
	}
}