
#pragma once

#include "ssh_file.h"
//#include "ssh_email.h"
//#include "ssh_sock.h"

namespace ssh
{
	/*
	class SSH StackTrace
	{
		friend class Tracer;
	public:
		struct NodeTrace
		{
			SSH_NEW_DECL(NodeTrace, 256);
			NodeTrace() {}
			NodeTrace(const String& str, NodeTrace* n) : next(n), value(str) {}
			~NodeTrace() { SSH_DEL(next); }
			// значение
			String value;
			// следующий
			NodeTrace* next;
		};
		StackTrace() : cdepth(0), depth(512), indent(0), root(nullptr), last(nullptr) { }
		~StackTrace() { clear(); }
		// инициализировать трассировщик
		void init(ssh_u _depth) { depth = _depth; cdepth = 0; }
		// добавить новый элемент
		void add(bool is, ssh_wcs func, ssh_wcs file, int line);
		// вывести в лог
		void output();
		// установка признака отключения
		void start() { StackTrace::is_disabled = false; }
		void stop() { StackTrace::is_disabled = true; }
		// признак отключение
		static bool is_disabled;
	protected:
		void clear()
		{
			auto m(NodeTrace::get_MemArrayNodeTrace());
			if(m->Valid())
			{
				SSH_DEL(root);
				m->Reset();
			}
		}
		void remove_node()
		{
			// удаляем узел из списка
			auto n(root->next);
			root->next = nullptr;
			delete root;
			root->next = n;
		}
		void add_node(const String& str)
		{
			auto n(new NodeTrace(str, nullptr));
			if(root) last->next = n; else root = n;
			last = n;
		}
		// макс глубина и текущая глубина
		ssh_u depth, cdepth;
		// отступ
		ssh_l indent;
		// корень списка
		NodeTrace* root;
		// последний узел списка
		NodeTrace* last;
	};

	class SSH Tracer
	{
	public:
		Tracer(ssh_wcs _fn, ssh_wcs _fl, int _ln) : fn(_fn)// ln(_ln), fl(_fl), 
		{
			if(!StackTrace::is_disabled) log->get_tracer()->add(true, _fn, _fl, _ln);
		}
		~Tracer()
		{
			if(!StackTrace::is_disabled) log->get_tracer()->add(false, fn, nullptr, 0);
		}
	protected:
		//Section sect;
		//int ln;
		//ssh_wcs fl;
		ssh_wcs fn;
	};
	*/
	// формат шаблонов:
	// $fn - файл
	// $ln - строка
	// $fl - файл
	// $ms - сообщение
	// $tm - время
	// $dt - дата длинная день месяц год день недели
	// $us - имя пользователя
	// $cm - имя компьютера
	// $nm - имя программы
	// $tp - тип сообщения
	// $DT - дата короткая день.месяц.год
	// $

	class SSH Log
	{
//		friend void socket_receive(Socket* sock, Socket::SOCK* s, const Buffer& buf);
	public:
		enum TypeMessage
		{
			none,
			info,		// информация
			assert,		// проверка
			exception,	// исключение
			tracer		// трасировщмк
		};
		enum class TypeOutput
		{
			null,		// не выводить
			screen,		// на экран
			file,		// в файл
			debug,		// в поток отладчика
			email,		// на почту
			net			// на хост
		};
		struct stk_email
		{
			stk_email()
			{
				file.open(path, flags_file);
				file.write(Log::header, charset);
			}
			void shutdown()
			{
				if(!file.is_close()) file.write(Log::footer, charset);
				send(L"");
			}
			// сообщение
			void message(const String& msg)
			{
				if(!is_blocked)
				{
					if(!file.is_close()) file.write(msg, cp_utf);
					count_msgs++;
					if(count_msgs >= max_msgs) send(Log::apply_template(cont));
				}
			}
			// отправка
			void send(const String& ln);
			// адрес
			String address = L"ostrov_skal@mail.ru";
			// хост почты и порт
			String host = L"smtp.mail.ru:465";
			// шаблон
			String templ = L"[$tp] $DT-$tm\t$fn\t - \t($fl: $ln) - <$ms>\r\n";
			// тема
			String subj = L"Уведомление логгирования";
			// логин
			String login = L"ostrov_skal";
			// пароль
			String pass = MAIL_PASS;
			// продолжение
			String cont = L"\r\n------------------------------ Продолжение сеанса - $nm ($dt - $tm) ------------------------------\r\n\r\n";
			// путь до временного файла
			String path = ssh_system_paths(SystemInfo::TEMP_FOLDER) + ssh_gen_name(L"__MAIL__LOG__");
			// кодировка
			String charset = cp_utf;
			// признак блокировки
			bool is_blocked = false;
			// флаги создания
			int flags = 0;// = Mail::stSSL;
			int flags_file = File::create_read_write | File::access_temp_remove;
			// максимальное количество сообщений
			int max_msgs = 200;
			// текущее количество сообщений
			int count_msgs = 0;
		protected:
			// временный файл
			File file;
		};
		struct stk_host
		{
			stk_host()
			{
				if((hEventSocket = CreateEvent(nullptr, true, false, nullptr))) {}
//					sock.setCallbacks(socket_receive, nullptr, nullptr, nullptr, nullptr);
			}
			void shutdown()
			{
				send(Log::footer);
				if(hEventSocket)
				{
					CloseHandle(hEventSocket);
					hEventSocket = nullptr;
				}
			}
			// сообщение
			void message(const String& msg)
			{
//				if(sock.is_closed())
				{
//					sock.init(name, 0, flags, cert, pwd_cert);
					send(Log::header);
				}
				send(msg);
			}
			// отправка сообщения серверу
			void send(const String& msg);
			// host:port для отправки по сети
			String name = L"localhost:11111";
			// сертификат
			String cert = L"e:\\SSL\\cer\\client.cer";
			// пароль к сертификату
			String pwd_cert;
			// шаблон для хоста
			String templ = L"[$tp] $fn - ($fl: $ln) - <$ms>";
			// флаги для создания сокета
			int flags = 0;
		protected:
			// сокет
			// Socket sock;
			// хэндл события
			HANDLE hEventSocket = 0;
		};
		struct stk_file
		{
			stk_file()
			{
				file.open(path, flags);
				file.write(Log::header, L"utf-8");
			}
			void shutdown()
			{
				if(!file.is_close()) file.write(Log::footer, L"utf-8");
			}
			// сообщение
			void message(const String& msg)
			{
				if(!file.is_close()) file.write(msg, L"utf-8");
			}
			// путь
			String path = ssh_system_paths(SystemInfo::PROG_FOLDER) + ssh_system_paths(SystemInfo::PROG_NAME) + L".log";
			// шаблон
			String templ = L"[$tp] $DT-$tm\t$fn  -  ($fl: $ln) - <$ms>\r\n";;
			// флаги
			int flags = File::create_write;
		protected:
			// файл
			File file;
		};
		struct stk_common
		{
			// инициализация
			void init(TypeOutput _out);
			// завершение
			void shutdown();
			// сообщение
			void message(const String& msg, TypeMessage type = none, ssh_cws func = nullptr, ssh_cws file = nullptr, int line = 0);
			// шаблон для вывода на экран
			String screen = L"\r\n-------------------- [$tp] --------------------\r\nФункция: $fn\r\n\r\nФайл: $fl\r\n\r\nСтрока: $ln\r\n\r\nПричина: $ms\r\n\r\n\r\nПродолжить выполнение программы?\r\n\r\n\r\n-------------------- [$tp] --------------------\r\n";
			// шаблон для вывода в поток отладчика
			String debug = L"[$tp] $DT-$tm\t$fn\t - \t($fl: $ln) - [$ms]\r\n";
			// шаблон для трассировщика
			String trace = L"$ms\r\n";
		protected:
			// структуры типа вывода
			stk_file* file = nullptr;
			stk_host* host = nullptr;
			stk_email* email = nullptr;
			// тип вывода
			TypeOutput out = TypeOutput::file;
		};
		static Log* instance() { static Log log; return &log; }
		// добавить шаблонное сообщение
		void add(TypeMessage type, ssh_cws func, ssh_cws file, int line, ssh_cws msg, ...);
		// добавить простое сообщение
		void add_msg(ssh_cws msg) { common.message(apply_template(common.trace, msg)); }
		// инициализация
		void init(TypeOutput _out = TypeOutput::file);
		// вернуть базовую структуру
		const stk_common* get_common() const { return &common; }
		// заголовок
		static String header;
		// завершение
		static String footer;
	protected:
		//Log() {}
		// деструктор
		virtual ~Log() { close(); }
		// приминение шаблона к сообщению
		static String apply_template(String templ, ssh_cws msg = nullptr, ssh_cws fn = nullptr, ssh_cws fl = nullptr, int ln = 0, int tp = 0);
		// закрыть
		void close();
		// сброс
		void shutdown();
		// основная структура
		stk_common common;
	};
}