
#pragma once

namespace ssh
{
	template<typename T> class Range;

	template <typename T> class _Buffer
	{
	public:
		// по умолчанию
		_Buffer() : data(nullptr), sz(0), is_owner(false) { }
		// конструктор копии
		_Buffer(const _Buffer& buf, ssh_u offs = 0, ssh_u size = -1) : sz(size == -1 ? buf.size() : size), is_owner(true)
		{
			const_cast<_Buffer*>(&buf)->is_owner = false;
			data = (sz ? buf.data + offs : nullptr);
		}
		// конструктор переноса
		_Buffer(_Buffer&& buf) : data(buf.data), sz(buf.sz), is_owner(buf.is_owner)
		{
			buf.data = nullptr;
			buf.sz = 0;
		}
		// создать буфер определённого размера
		_Buffer(ssh_u count) : data(new T[count]), sz(count * sizeof(T)), is_owner(true) { }
		// создать из диапазона
		_Buffer(const Range<int>& wh, int bpp) : sz(wh.w * wh.h * bpp), data(new T[sz]), is_owner(true) { }
		// создать из существующего неопределённого буфера
		_Buffer(T* p, ssh_u count, bool is_own = true) : data(p), sz(count * sizeof(T)), is_owner(is_own) {}
		// деструктор
		~_Buffer() { release(); }
		// оператор присваивание
		const _Buffer& operator = (const _Buffer& buf) noexcept
		{
			release();
			is_owner = true;
			sz = buf.sz;
			const_cast<_Buffer*>(&buf)->is_owner = false;
			data = buf.data;
			return *this;
		}
		// оператор переноса
		const _Buffer& operator = (_Buffer&& buf) noexcept
		{
			release();
			data = buf.data;
			sz = buf.sz;
			is_owner = buf.is_owner;
			buf.data = nullptr;
			buf.sz = 0;
			return *this;
		}
		// освобождение буфера
		void reset() { release(); }
		// вернуть размер
		ssh_u size() const { return sz; }
		// вернуть размер
		ssh_u count() const { return sz / sizeof(T); }
		// привидение типа
		operator T*() const { return data; }
		// интерпретация содержимого буфера
		template<typename TYPE> TYPE to() const { return (TYPE)data; }
#ifdef _DEBUG
		// тест
		static void unit_test();
#endif
	protected:
		// реализация
		void release() noexcept { if(is_owner) SSH_DEL(data); }
		// указатель на данные
		T* data;
		// размер данных
		ssh_u sz;
		// признак владельца
		bool is_owner;
	};

	typedef _Buffer<ssh_b> Buffer;
	typedef _Buffer<ssh_w> BufferW;
	typedef _Buffer<ssh_d> BufferD;
}