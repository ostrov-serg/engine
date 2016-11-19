
#pragma once

namespace ssh
{
	class SSH Exception
	{
	public:
		Exception(ssh_cws fn, ssh_cws fl, int ln, ssh_cws msg) : func(fn), file(fl), line(ln), message(msg) {}
		virtual void out_log(String msg) const;
		virtual ~Exception() {}
	protected:
		Exception() : line(0), func(nullptr), file(nullptr) {}
		int line;
		ssh_cws func, file;
		String message;
	};

}