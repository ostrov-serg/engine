
#include "stdafx.h"
#include "ssh_log.h"
#include "ssh_mem.h"

namespace ssh
{
	String Log::header(L"\r\n-------------------- Запуск сеанса [$us - $cm] - $nm ($dt - $tm) --------------------\r\n\r\n");
	String Log::footer(L"\r\n------------------------------ Завершение сеанса - $nm ($dt - $tm) ------------------------------\r\n\r\n");

	void Log::stk_common::init(TypeOutput _out)
	{
		out = _out;
		header = Log::apply_template(header);
		switch(out)
		{
			case TypeOutput::file:
				file = new stk_file();
				break;
			case TypeOutput::debug:
				OutputDebugString(header);
				break;
			case TypeOutput::email:
				email = new stk_email();
				break;
			case TypeOutput::net:
				host = new stk_host();
				break;
		}
	}

	void Log::stk_common::shutdown()
	{
		Log::footer = Log::apply_template(Log::footer);
		switch(out)
		{
			case TypeOutput::net:
				host->shutdown();
				SSH_DEL(host);
				break;
			case TypeOutput::screen:
				break;
			case TypeOutput::file:
				file->shutdown();
				SSH_DEL(file);
				break;
			case TypeOutput::email:
				email->shutdown();
				SSH_DEL(email);
				break;
			case TypeOutput::debug:
				OutputDebugStringW(footer);
				break;
		}
		out = TypeOutput::null;
	}

	void Log::stk_common::message(const String & msg, TypeMessage type, ssh_cws fn, ssh_cws fl, int ln)
	{
		switch(out)
		{
			case TypeOutput::net:
				host->message(type != none ? Log::apply_template(host->templ, msg, fn, fl, ln, type) : msg);
				break;
			case TypeOutput::file:
				file->message(type != none ? Log::apply_template(file->templ, msg, fn, fl, ln, type) : msg);
				break;
			case TypeOutput::email:
				email->message(type != none ? Log::apply_template(email->templ, msg, fn, fl, ln, type) : msg);
				break;
			case TypeOutput::debug:
				OutputDebugStringW(type != none ? Log::apply_template(debug, msg, fn, fl, ln, type) : msg);
				break;
			case TypeOutput::screen:
				if(MessageBox(nullptr, msg, ssh_system_paths(SystemInfo::PROG_NAME), MB_ICONERROR | MB_YESNO) == IDNO) exit(4);
				break;
		}
	}

	void Log::init(TypeOutput _out)
	{
		shutdown();
		try
		{
			common.init(_out);
			// установить обработчики исключений
			ssh_mem->set_exceptionHandlers();
			// старт трассировщика
//			tracer.start();
			// старт менеджера памяти
			ssh_mem->start();
		}
		catch(const Exception&)
		{
			shutdown();
		}
	}

	void Log::add(TypeMessage type, ssh_cws fn, ssh_cws fl, int ln, ssh_cws msg, ...)
	{
//		if(!tracer.is_disabled)
		{
//			tracer.stop();
			String _msg;
			// формируем сообщение
			va_list	arglist;
			va_start(arglist, msg);
			String msgArgs(String::fmt(msg, arglist));
			va_end(arglist);
			msgArgs.replace(L'\r', L'.');
			msgArgs.replace(L'\n', L'.');
			// формируем сообщение на основании шаблона
			common.message(msgArgs, type, fn, fl, ln);
//			tracer.start();
		}
	}

	String Log::apply_template(String tpl, ssh_cws msg, ssh_cws fn, ssh_cws fl, int ln, int tp)
	{
		static ssh_cws m_types[] = {L"INFO", L"ASSERT", L"EXCEPTION", L"TRACE"};
		static ssh_cws rpl[] = {L"$DT", L"$fn", L"$ln", L"$fl", L"$ms", L"$tm", L"$dt", L"$us", L"$cm", L"$nm", L"$tp", nullptr};
		String tmp(String::fmt(L"%s\1%s\1%i\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1\1",
							   Time::current().fmt(L"$d.$m.$y"), fn, ln, fl, msg, Time::current().fmt(L"$h:$nm:$s"),
							   Time::current().fmt(L"$d $MN)+ $Y ($dW)"), ssh_system_paths(SystemInfo::USER_NAME),
							   ssh_system_paths(SystemInfo::COMP_NAME), ssh_system_paths(SystemInfo::PROG_NAME), m_types[tp]));
		tmp.replace(L'\1', L'\0');
		return tpl.replace(rpl, tmp);
	}

	void Log::close()
	{
//		tracer.stop();
		ssh_mem->stop();
//		tracer.output();
		ssh_mem->output();
		common.shutdown();
	}

	void Log::shutdown()
	{
//		tracer.stop();
		ssh_mem->stop();
		common.shutdown();
	}

	void Log::stk_email::send(const String & ln)
	{
		if(!file.is_close())
		{
			is_blocked = true;
			// читаем из файла
			file.set_pos(0, File::begin);
			String str(file.read(0, charset));
			file.close();
			// обнуляем файл
			file.open(path, flags_file);
			file.write(ln, charset);
			count_msgs = 0;
			// отправка
			/*
			Mail m(host, login, pass, flags), 1);
			m.set_charset(L"utf-8");
			m.add_recipient(L"", address);
			m.set_sender(ssh_system_paths(SystemInfo::PROG_NAME), address);
			m.set_message(L"Система логгирования для отладки программы.");
			m.smtp(subj, str);
			*/
			is_blocked = false;
		}
	}

//	static void socket_receive(Socket* sock, Socket::SOCK* s, const Buffer& buf)
//	{
//		SetEvent(hEventSocket);
//	}

	void Log::stk_host::send(const String & msg)
	{
//		if(!sock.is_closed())
		{
//			sock.send(0, msg);
			if(WaitForSingleObject(hEventSocket, 30000) != WAIT_OBJECT_0)
//				sock.close();
			ResetEvent(hEventSocket);
		}
	}
}
