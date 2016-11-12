
#include "stdafx.h"
#include "ssh_mem.h"
#include <signal.h>

#define SIGNAL_INTERRUPT		SIGINT
#define SIGNAL_INSTRUCTION		SIGILL
#define SIGNAL_FLOATING			SIGFPE
#define SIGNAL_FAULT			SIGSEGV
#define SIGNAL_TERMINATE		SIGTERM
#define SIGNAL_ABORT			SIGABRT
#define	UNHANDLED_EXCEPTION		0x8000
#define	TERMINATE_CALL			0x4000
#define	UNEXPECTED_CALL			0x2000
#define	PURE_CALL				0x1000
#define	SECURITY_ERROR			0x0800
#define	NEW_OPERATOR_ERROR		0x0400
#define	INVALID_PARAMETER_ERROR 0x0200

namespace ssh
{
	static void __cdecl ssh_signal_handler(int numSignal)
	{
		SSH_FAULT(numSignal, (EXCEPTION_POINTERS*)_pxcptinfoptrs);
		exit(1);
	}

	static void __cdecl ssh_terminate_handler()
	{
		SSH_FAULT(TERMINATE_CALL, nullptr);
		exit(2);
	}

	static void __cdecl ssh_unexp_handler()
	{
		SSH_FAULT(UNEXPECTED_CALL, nullptr);
		exit(3);
	}

	static void __cdecl ssh_purecall_handler()
	{
		SSH_FAULT(PURE_CALL, nullptr);
		exit(4);
	}

	static void __cdecl ssh_security_handler(int code, void *x)
	{
		SSH_FAULT(SECURITY_ERROR, nullptr);
		exit(5);
	}

	static void __cdecl ssh_invalid_parameter_handler(ssh_cws expression, ssh_cws function, ssh_cws file, ssh_i line, uintptr_t pReserved)
	{
		MemMgr::instance()->fault(INVALID_PARAMETER_ERROR, function, file, line, nullptr, expression);
		exit(6);
	}

	static int __cdecl ssh_new_handler(ssh_u size)
	{
		SSH_FAULT(NEW_OPERATOR_ERROR, nullptr);
		exit(7);
	}

	static LONG WINAPI Win32UnhandledExceptionFilter(EXCEPTION_POINTERS* except)
	{
		SSH_FAULT(UNHANDLED_EXCEPTION, except);
		exit(8);
	}

	void MemMgr::set_exceptionHandlers()
	{
		// установить фильтр исключений
		SetUnhandledExceptionFilter(Win32UnhandledExceptionFilter);
		// установить режимы отчета библиотеки времени выполнения
		//_CrtSetReportMode(_CRT_ERROR, 0);
		_CrtSetReportMode(_CRT_ASSERT, 0);

		set_terminate(ssh_terminate_handler);
		set_unexpected(ssh_unexp_handler);
		_set_purecall_handler(ssh_purecall_handler);
		_set_invalid_parameter_handler(ssh_invalid_parameter_handler);
		_set_new_handler(ssh_new_handler);
//		_set_security_error_handler(ssh_security_handler);
		signal(SIGABRT, ssh_signal_handler);
		signal(SIGINT, ssh_signal_handler);
		signal(SIGTERM, ssh_signal_handler);
		signal(SIGFPE, ssh_signal_handler);
		signal(SIGILL, ssh_signal_handler);
		signal(SIGSEGV, ssh_signal_handler);
	}

	bool MemMgr::fault(int type, ssh_cws fn, ssh_cws fl, int ln, EXCEPTION_POINTERS* except, ssh_cws msg_ex)
	{
		//if(!except)
		{
			CONTEXT ContextRecord;
			EXCEPTION_RECORD ExceptionRecord;
			memset(&ContextRecord, 0, sizeof(CONTEXT));
			memset(&ExceptionRecord, 0, sizeof(EXCEPTION_RECORD));

			RtlCaptureContext(&ContextRecord);

			ExceptionRecord.ExceptionCode = 0;
			ExceptionRecord.ExceptionAddress = _ReturnAddress();

			except = new EXCEPTION_POINTERS;
			except->ContextRecord = new CONTEXT;
			except->ExceptionRecord = new EXCEPTION_RECORD;

			memcpy(except->ContextRecord, &ContextRecord, sizeof(CONTEXT));
			memcpy(except->ExceptionRecord, &ExceptionRecord, sizeof(EXCEPTION_RECORD));
		}
		String msg(L"\r\nКонтекст на момент возбуждения исключения: \r'n");
		String caption;
		switch(type)
		{
			case SIGNAL_INTERRUPT: caption = L"Прерывание. "; break;
			case SIGNAL_INSTRUCTION: caption = L"Недопустимая инструкция. "; break;
			case SIGNAL_FLOATING: caption = L"Недопустимая операция с вещественными числами. "; break;
			case SIGNAL_FAULT: caption = L"Недопустимый указатель. "; break;
			case SIGNAL_TERMINATE: caption = L"SIGNAL TERMINATE. "; break;
			case SIGNAL_ABORT: caption = L"SIGNAL ABORT. "; break;
			case UNHANDLED_EXCEPTION: caption = L"Необработанное исключение. "; break;
			case TERMINATE_CALL: caption = L"Вызов функции terminate(). "; break;
			case UNEXPECTED_CALL: caption = L"Неожиданное исключение. "; break;
			case PURE_CALL: caption = L"Вызов чистой виртуальной функции. "; break;
			case SECURITY_ERROR: caption = L"Переполнение буфера. "; break;
			case NEW_OPERATOR_ERROR: caption = L"Не удалось выделить память оператором new. "; break;
			case INVALID_PARAMETER_ERROR: caption = L"Недопустимый параметр CRT функции, при выполнении операции "; caption += msg_ex; caption += L". "; break;
		}
		if(except)
		{
			msg.fmt(L"\r\nадрес: %016I64X flags: %08X\r\n"
					L"rax: %016I64X rcx: %016I64X rdx: %016I64X rbx: %016I64X rbp: %016I64X rsp: %016I64X rsi: %016I64X rdi: %016I64X\r\n"
					L"r8: %016I64X r9: %016I64X r10: %016I64X r11: %016I64X r12: %016I64X r13: %016I64X r14: %016I64X r15: %016I64X\r\n"
					L"xmm0: %016I64X%016I64X xmm1: %016I64X%016I64X xmm2: %016I64X%016I64X xmm3: %016I64X%016I64X xmm4: %016I64X%016I64X xmm5: %016I64X%016I64X xmm6: %016I64X%016I64X xmm7: %016I64X%016I64X\r\n"
					L"xmm8: %016I64X%016I64X xmm9: %016I64X%016I64X xmm10: %016I64X%016I64X xmm11: %016I64X%016I64X xmm12: %016I64X%016I64X xmm13: %016I64X%016I64X xmm14: %016I64X%016I64X xmm15: %016I64X%016I64X",
					except->ExceptionRecord->ExceptionAddress, except->ContextRecord->EFlags,
					except->ContextRecord->Rax, except->ContextRecord->Rcx, except->ContextRecord->Rdx, except->ContextRecord->Rbx,
					except->ContextRecord->Rbp, except->ContextRecord->Rsp, except->ContextRecord->Rsi, except->ContextRecord->Rdi,
					except->ContextRecord->R8, except->ContextRecord->R9, except->ContextRecord->R10, except->ContextRecord->R11,
					except->ContextRecord->R12, except->ContextRecord->R13, except->ContextRecord->R14, except->ContextRecord->R15,
					except->ContextRecord->Xmm0.Low, except->ContextRecord->Xmm0.High,
					except->ContextRecord->Xmm1.Low, except->ContextRecord->Xmm1.High,
					except->ContextRecord->Xmm2.Low, except->ContextRecord->Xmm2.High,
					except->ContextRecord->Xmm3.Low, except->ContextRecord->Xmm3.High,
					except->ContextRecord->Xmm4.Low, except->ContextRecord->Xmm4.High,
					except->ContextRecord->Xmm5.Low, except->ContextRecord->Xmm5.High,
					except->ContextRecord->Xmm6.Low, except->ContextRecord->Xmm6.High,
					except->ContextRecord->Xmm7.Low, except->ContextRecord->Xmm7.High,
					except->ContextRecord->Xmm8.Low, except->ContextRecord->Xmm8.High,
					except->ContextRecord->Xmm9.Low, except->ContextRecord->Xmm9.High,
					except->ContextRecord->Xmm10.Low, except->ContextRecord->Xmm10.High,
					except->ContextRecord->Xmm11.Low, except->ContextRecord->Xmm11.High,
					except->ContextRecord->Xmm12.Low, except->ContextRecord->Xmm12.High,
					except->ContextRecord->Xmm13.Low, except->ContextRecord->Xmm13.High,
					except->ContextRecord->Xmm14.Low, except->ContextRecord->Xmm14.High,
					except->ContextRecord->Xmm15.Low, except->ContextRecord->Xmm15.High);
		}
		ssh_log->add(Log::exception, fn, fl, ln, caption + msg);
		return true;
	}

	void MemMgr::leaks()
	{
		if(total_alloc)
		{
			Section cs;
			String txt;
			ssh_log->add_msg(String::fmt(L"Обнаружено %I64i потерянных блоков памяти...\r\n", total_alloc));
			auto n(root);
			while(n)
			{
				ssh_b* ptr((ssh_b*)(n + sizeof(NodeMem)));
				String bytes(ssh_make_hex_string(ptr, n->sz > 48 ? 48 : n->sz, txt, true, n->sz > 48));
				ssh_log->add_msg(String::fmt(L"node <0x%I64X, %i, %s\t%s>", ptr, n->sz, bytes, txt));
				n = n->next;
			}
		}
	}

	void MemMgr::output()
	{
		leaks();
		ssh_log->add_msg(String::fmt(L"\r\nЗа данный сеанс было выделено %i(~%s) байт памяти ..., освобождено %i(~%s) ...:%c, максимум - %i блоков\r\n", use_max_mem, ssh_num_volume(use_max_mem), total_free, ssh_num_volume(total_free), (use_mem != total_free ? L'(' : L')'), total_alloc));
	}

	void* MemMgr::alloc(ssh_u sz)
	{
		Section cs;

		ssh_b* p((ssh_b*)::malloc(sz + sizeof(NodeMem) + 4));
		// создать узел
		NodeMem* nd(::new((NodeMem*)(p)) NodeMem((int)sz, root));
		if(is_enabled)
		{
#ifdef _DEBUG
			memset(p + sizeof(NodeMem), 0xBB, sz);
#endif
			// добавить статистику
			total_alloc++;
			use_mem += sz;
			if(use_max_mem < use_mem) use_max_mem = use_mem;
			if(root) root->prev = nd;
			root = nd;
		}
		else nd->use = 0;
		return (void*)(p + sizeof(NodeMem));
	}

	// освобождение
	void MemMgr::free(ssh_b* p)
	{
		Section cs;

		if(p)
		{
			p -= sizeof(NodeMem);
			NodeMem* nd((NodeMem*)p);

			if(nd->use)
			{
				ssh_u sz(nd->sz);
				// корректируем статистику
				total_free += sz;
				use_mem -= sz;
				total_alloc--;
				// удаляем узел из списка
				auto nn(nd->next);
				auto np(nd->prev);
				if(nn) nn->prev = np;
				if(np) np->next = nn;
				if(nd == root) root = nn;
#ifdef _DEBUG
				memset(p + sizeof(NodeMem), 0xaa, sz);
#endif
			}
			// освобождаем память
			::free(p);
		}
	}
}
