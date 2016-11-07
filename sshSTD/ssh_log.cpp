
#include "stdafx.h"
#include "ssh_log.h"

namespace ssh
{
	Exception::Exception(ssh_cws fn, ssh_cws fl, int ln, ssh_cws msg, ...)
	{
		func = fn;
		file = fl;
		line = ln;

		va_list argList;
		va_start(argList, msg);
		message = String::fmt(msg, argList);
		va_end(argList);
	}

	void Exception::add(ssh_cws msg, ...) const
	{
		// формируем сообщение
		va_list	arglist;
		va_start(arglist, msg);
		String msgArgs(String::fmt(msg, arglist));
		va_end(arglist);
		if(!msgArgs.is_empty()) msgArgs += L", ";
		// добавляем в лог
//		log->add(Log::mException, func, file, line, msgArgs + message);
	}

}