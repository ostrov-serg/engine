
#pragma once

namespace ssh
{
	class buffer
	{
	public:
		buffer() : ptr(nullptr), sz(0) {}
		buffer(ssh_u size) : ptr(nullptr), sz(size) {}
		buffer(ssh_b* p, ssh_u size) : ptr(p), sz(size) {}
		~buffer() {}
		//template<typename T> T to() const { return (T*)ptr; }
		template<typename T> operator T() const { return (T)ptr; }
//		template<typename T> operator const T() const { return (const T*)ptr; }
		ssh_u size() const { return sz; }
	protected:
		ssh_b* ptr;
		ssh_u sz;
	};
}