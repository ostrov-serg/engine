
#pragma once

namespace ssh
{
	template<typename T> class Range;

	class Buffer
	{
	public:
		// �� ���������
		Buffer() { }
		// ����������� �����
		Buffer(const Buffer& buf, ssh_u offs = 0, ssh_u size = -1) : sz(size == -1 ? buf.size() : size), is_owner(true) { const_cast<Buffer*>(&buf)->is_owner = false; data = (sz ? buf.data + offs : nullptr); }
		// ����������� ��������
		Buffer(Buffer&& buf) : data(buf.data), sz(buf.sz), is_owner(buf.is_owner) { buf.data = nullptr; buf.sz = 0; }
		// ������� ����� ������������ �������
		Buffer(ssh_u count) : data(new ssh_b[count]), sz(count), is_owner(true) { }
//		Buffer(const Range<int>& wh, int bpp) : sz(wh.w * wh.h * bpp), data(new ssh_b[sz]), is_owner(true) { }
		// ������� �� ������������� �������������� ������
		Buffer(void* p, ssh_u count, bool is_own = true) : data((ssh_b*)p), sz(count), is_owner(is_own) {}
		// ����������
		~Buffer() { release(); }
		// �������� ������������
		const Buffer& operator = (const Buffer& buf) { release(); is_owner = true; sz = buf.sz; const_cast<Buffer*>(&buf)->is_owner = false; data = buf.data; return *this; }
		// �������� ��������
		const Buffer& operator = (Buffer&& buf) { release(); data = buf.data; sz = buf.sz; is_owner = buf.is_owner; buf.data = nullptr; buf.sz = 0; return *this; }
		// ������������ ������
		void reset() { release(); }
		// ������� ������
		ssh_u size() const { return sz; }
		// ���������� ����
		operator ssh_b*() const { return data; }
		// ������������� ����������� ������
		template<typename TYPE> TYPE to() const { return (TYPE)data; }
	protected:
		// ����������
		void release() { if(is_owner) SSH_DEL(data); }
		// ��������� �� ������
		ssh_b* data = nullptr;
		// ������ ������
		ssh_u sz = 0;
		// ������� ���������
		bool is_owner = false;
	};
}