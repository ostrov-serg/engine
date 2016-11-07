
#pragma once

namespace ssh
{
	class SSH Exception
	{
	public:
		Exception() : line(0), func(nullptr), file(nullptr) {}
		Exception(ssh_cws fn, ssh_cws fl, int ln, ssh_cws msg, ...);
		virtual ~Exception() {}
		virtual void add(ssh_cws msg, ...) const;
	protected:
		int line;
		ssh_cws func, file;
		String message;
	};
}