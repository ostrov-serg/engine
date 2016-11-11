
#pragma once

namespace ssh
{
	template<typename T> class Range;

	class Buffer
	{
	public:
		// по умолчанию
		Buffer() { }
		// конструктор копии
		Buffer(const Buffer& buf, ssh_u offs = 0, ssh_u size = -1) : sz(size == -1 ? buf.size() : size), is_owner(true) { const_cast<Buffer*>(&buf)->is_owner = false; data = (sz ? buf.data + offs : nullptr); }
		// конструктор переноса
		Buffer(Buffer&& buf) : data(buf.data), sz(buf.sz), is_owner(buf.is_owner) { buf.data = nullptr; buf.sz = 0; }
		// создать буфер определённого размера
		Buffer(ssh_u count) : data(new ssh_b[count]), sz(count), is_owner(true) { }
//		Buffer(const Range<int>& wh, int bpp) : sz(wh.w * wh.h * bpp), data(new ssh_b[sz]), is_owner(true) { }
		// создать из существующего неопределённого буфера
		Buffer(void* p, ssh_u count, bool is_own = true) : data((ssh_b*)p), sz(count), is_owner(is_own) {}
		// деструктор
		~Buffer() { release(); }
		// оператор присваивание
		const Buffer& operator = (const Buffer& buf) { release(); is_owner = true; sz = buf.sz; const_cast<Buffer*>(&buf)->is_owner = false; data = buf.data; return *this; }
		// оператор переноса
		const Buffer& operator = (Buffer&& buf) { release(); data = buf.data; sz = buf.sz; is_owner = buf.is_owner; buf.data = nullptr; buf.sz = 0; return *this; }
		// освобождение буфера
		void reset() { release(); }
		// вернуть размер
		ssh_u size() const { return sz; }
		// привидение типа
		operator ssh_b*() const { return data; }
		// интерпретация содержимого буфера
		template<typename TYPE> TYPE to() const { return (TYPE)data; }
	protected:
		// реализация
		void release() { if(is_owner) SSH_DEL(data); }
		// указатель на данные
		ssh_b* data = nullptr;
		// размер данных
		ssh_u sz = 0;
		// признак владельца
		bool is_owner = false;
	};
}