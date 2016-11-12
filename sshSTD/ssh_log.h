
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
			// ��������
			String value;
			// ���������
			NodeTrace* next;
		};
		StackTrace() : cdepth(0), depth(512), indent(0), root(nullptr), last(nullptr) { }
		~StackTrace() { clear(); }
		// ���������������� ������������
		void init(ssh_u _depth) { depth = _depth; cdepth = 0; }
		// �������� ����� �������
		void add(bool is, ssh_wcs func, ssh_wcs file, int line);
		// ������� � ���
		void output();
		// ��������� �������� ����������
		void start() { StackTrace::is_disabled = false; }
		void stop() { StackTrace::is_disabled = true; }
		// ������� ����������
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
			// ������� ���� �� ������
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
		// ���� ������� � ������� �������
		ssh_u depth, cdepth;
		// ������
		ssh_l indent;
		// ������ ������
		NodeTrace* root;
		// ��������� ���� ������
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
	// ������ ��������:
	// $fn - ����
	// $ln - ������
	// $fl - ����
	// $ms - ���������
	// $tm - �����
	// $dt - ���� ������� ���� ����� ��� ���� ������
	// $us - ��� ������������
	// $cm - ��� ����������
	// $nm - ��� ���������
	// $tp - ��� ���������
	// $DT - ���� �������� ����.�����.���
	// $

	class SSH Log
	{
//		friend void socket_receive(Socket* sock, Socket::SOCK* s, const Buffer& buf);
	public:
		enum TypeMessage
		{
			none,
			info,		// ����������
			assert,		// ��������
			exception,	// ����������
			tracer		// �����������
		};
		enum class TypeOutput
		{
			null,		// �� ��������
			screen,		// �� �����
			file,		// � ����
			debug,		// � ����� ���������
			email,		// �� �����
			net			// �� ����
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
			// ���������
			void message(const String& msg)
			{
				if(!is_blocked)
				{
					if(!file.is_close()) file.write(msg, cp_utf);
					count_msgs++;
					if(count_msgs >= max_msgs) send(Log::apply_template(cont));
				}
			}
			// ��������
			void send(const String& ln);
			// �����
			String address = L"ostrov_skal@mail.ru";
			// ���� ����� � ����
			String host = L"smtp.mail.ru:465";
			// ������
			String templ = L"[$tp] $DT-$tm\t$fn\t - \t($fl: $ln) - <$ms>\r\n";
			// ����
			String subj = L"����������� ������������";
			// �����
			String login = L"ostrov_skal";
			// ������
			String pass = MAIL_PASS;
			// �����������
			String cont = L"\r\n------------------------------ ����������� ������ - $nm ($dt - $tm) ------------------------------\r\n\r\n";
			// ���� �� ���������� �����
			String path = ssh_system_paths(SystemInfo::TEMP_FOLDER) + ssh_gen_name(L"__MAIL__LOG__");
			// ���������
			String charset = cp_utf;
			// ������� ����������
			bool is_blocked = false;
			// ����� ��������
			int flags = 0;// = Mail::stSSL;
			int flags_file = File::create_read_write | File::access_temp_remove;
			// ������������ ���������� ���������
			int max_msgs = 200;
			// ������� ���������� ���������
			int count_msgs = 0;
		protected:
			// ��������� ����
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
			// ���������
			void message(const String& msg)
			{
//				if(sock.is_closed())
				{
//					sock.init(name, 0, flags, cert, pwd_cert);
					send(Log::header);
				}
				send(msg);
			}
			// �������� ��������� �������
			void send(const String& msg);
			// host:port ��� �������� �� ����
			String name = L"localhost:11111";
			// ����������
			String cert = L"e:\\SSL\\cer\\client.cer";
			// ������ � �����������
			String pwd_cert;
			// ������ ��� �����
			String templ = L"[$tp] $fn - ($fl: $ln) - <$ms>";
			// ����� ��� �������� ������
			int flags = 0;
		protected:
			// �����
			// Socket sock;
			// ����� �������
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
			// ���������
			void message(const String& msg)
			{
				if(!file.is_close()) file.write(msg, L"utf-8");
			}
			// ����
			String path = ssh_system_paths(SystemInfo::PROG_FOLDER) + ssh_system_paths(SystemInfo::PROG_NAME) + L".log";
			// ������
			String templ = L"[$tp] $DT-$tm\t$fn  -  ($fl: $ln) - <$ms>\r\n";;
			// �����
			int flags = File::create_write;
		protected:
			// ����
			File file;
		};
		struct stk_common
		{
			// �������������
			void init(TypeOutput _out);
			// ����������
			void shutdown();
			// ���������
			void message(const String& msg, TypeMessage type = none, ssh_cws func = nullptr, ssh_cws file = nullptr, int line = 0);
			// ������ ��� ������ �� �����
			String screen = L"\r\n-------------------- [$tp] --------------------\r\n�������: $fn\r\n\r\n����: $fl\r\n\r\n������: $ln\r\n\r\n�������: $ms\r\n\r\n\r\n���������� ���������� ���������?\r\n\r\n\r\n-------------------- [$tp] --------------------\r\n";
			// ������ ��� ������ � ����� ���������
			String debug = L"[$tp] $DT-$tm\t$fn\t - \t($fl: $ln) - [$ms]\r\n";
			// ������ ��� �������������
			String trace = L"$ms\r\n";
		protected:
			// ��������� ���� ������
			stk_file* file = nullptr;
			stk_host* host = nullptr;
			stk_email* email = nullptr;
			// ��� ������
			TypeOutput out = TypeOutput::file;
		};
		static Log* instance() { static Log log; return &log; }
		// �������� ��������� ���������
		void add(TypeMessage type, ssh_cws func, ssh_cws file, int line, ssh_cws msg, ...);
		// �������� ������� ���������
		void add_msg(ssh_cws msg) { common.message(apply_template(common.trace, msg)); }
		// �������������
		void init(TypeOutput _out = TypeOutput::file);
		// ������� ������� ���������
		const stk_common* get_common() const { return &common; }
		// ���������
		static String header;
		// ����������
		static String footer;
	protected:
		//Log() {}
		// ����������
		virtual ~Log() { close(); }
		// ���������� ������� � ���������
		static String apply_template(String templ, ssh_cws msg = nullptr, ssh_cws fn = nullptr, ssh_cws fl = nullptr, int ln = 0, int tp = 0);
		// �������
		void close();
		// �����
		void shutdown();
		// �������� ���������
		stk_common common;
	};
}