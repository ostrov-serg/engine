
#pragma once

#include <fcntl.h>

#include "ssh_time.h"
#include "ssh_buf.h"

namespace ssh
{
	class SSH File
	{
	public:
		enum Seek : int
		{
			begin	= SEEK_SET,
			current = SEEK_CUR,
			end		= SEEK_END
		};
		enum OpenFlags : int
		{
			append_read			= 0,
			append_read_write	= 1,
			create_write		= 2,
			create_read_write	= 3,
			open_read			= 4,
			open_write			= 5,
			open_read_write		= 6,
			access_temp_remove	= _O_TEMPORARY,
			access_temp			= _O_SHORT_LIVED,
			access_random		= _O_RANDOM,
			access_sequential	= _O_SEQUENTIAL
		};
		struct MODE_FILE_OPEN
		{
			int flag;
			int mode;
		};
		// ������������
		File() : h(0) {}
		File(ssh_cws name, int flags) : h(0) { open(name, flags); }
		// ����������
		virtual ~File() { close(); }
		// �������
		void open(ssh_cws name, int flags)
		{
			static MODE_FILE_OPEN opens[] = { {_O_APPEND | _O_RDONLY, 0}, {_O_APPEND | _O_RDWR, 0}, {_O_CREAT | _O_TRUNC | _O_WRONLY, _S_IWRITE}, {_O_CREAT | _O_TRUNC | _O_RDWR, _S_IWRITE | _S_IREAD}, {_O_RDONLY, 0}, {_O_WRONLY, 0}, {_O_RDWR, 0} };
			if((h = _wopen(name, opens[flags & 7].flag | (flags & (~7) | _O_BINARY), opens[flags & 7].mode)) == -1)
				SSH_THROW(L"�� ������� ������� ���� %s!", name);
			path = name;
		}
		// �������
		void close() { if(h) _close(h); h = 0; }
		// ������ � ��������������� ����� ������������� �������
		buffer read(const buffer& buf, ssh_u size) const
		{
			if(size > buf.size())  SSH_THROW(L"������ ������ %i ������� ��� ��� ������ ����� %s!", buf.size(), path);
			if(_read(h, buf, (ssh_i)size) != size) SSH_THROW(L"������ ������ ����� %s!", path);
			return buf;
		}
		// ������ ������������� �������
		buffer read(ssh_u size) const
		{
			if(!size) size = (length() - get_pos());
			return read(buffer(size), size);
		}
		// ������ ������������� ������� � ������������ �������
		buffer read(ssh_u size, ssh_u pos, int flags) const
		{
			set_pos(pos, flags);
			return read(size);
		}
		// ������ � ��������������� ����� ������������� ������� � ������������ �������
		buffer read(const buffer& buf, ssh_u size, ssh_u pos, int flags) const
		{
			set_pos(pos, flags);
			return read(buf, size);
		}
		// ������ ������ � ��������������� ���������
		String read(ssh_u size, ssh_cws cnv) const
		{
			return ssh_convert(cnv, read(size), 0);
		}
		// ������ ������ ������������� �������
		void write(const buffer& buf, ssh_u size = 0) const
		{
			if(!size) size = buf.size();
			if(_write(h, buf, (ssh_i)size) != size) SSH_THROW(L"������ ������ ����� %s!", path);
		}
		// ������ ������ ������������� ������� � ������������ �������
		void write(const buffer& buf, ssh_u size, ssh_u pos, int flags) const
		{
			set_pos(pos, flags);
			return write(buf, size);
		}
		// ������ ������ � ��������������� ���������
		void write(const String& str, ssh_cws cnv) const
		{
			write(ssh_convert(cnv, str));
		}
		// ������
		void get_time(Time* create, Time* access, Time* write) const
		{
			struct _stat64 stat;
			if(_wstat64(path, &stat)) SSH_THROW(L"�� ������� �������� ����� ����� %s!", path);
			if(access) *access = stat.st_atime;
			if(create) *create = stat.st_ctime;
			if(write) *write = stat.st_mtime;
		}
		ssh_u get_pos() const { return _telli64(h); }
		ssh_u length() const { return _filelengthi64(h); }
		ssh_u set_pos(ssh_u pos, int flags) const { return _lseeki64(h, pos, flags); }
		bool set_length(ssh_u size) const { return (_chsize_s(h, size) == 0); }
		bool is_close() const { return (h == 0); }
		static bool rename(ssh_cws _old, ssh_cws _new) { return (_wrename(_old, _new) == 0); }
		static bool remove(ssh_cws name) { return (_wunlink(name) == 0); }
		static bool find(ssh_cws path, _wfinddata64_t* data, ssh_l* handle, bool is_begin)
		{
			if(is_begin) return ((*handle = _wfindfirst64(path, data)) != 0);
			return (_wfindnext64(*handle, data) == 0);
		}
		static bool is_exist(ssh_cws path)
		{
			int h(_wopen(path, _O_RDONLY));
			if(h != -1) _close(h);
			return (h != -1);
		}
		String get_path() const { return path; }
	protected:
		int h;
		String path;
	};
}
