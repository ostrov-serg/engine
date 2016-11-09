
#pragma once

namespace ssh
{
	template <typename T> class Array
	{
	public:
		// ����������� �� ���������
		Array() { }
		// ���������������� �����������
		Array(ssh_u _count, ssh_u _grow) : grow(_grow) { alloc(_count); }
		// ����������� �� ������ �������������
		Array(const std::initializer_list<T>& _list) { insert(0, _list.begin(), _list.size()); }
		// ����������� �����
		Array(const Array<T>& src) { *this = src; }
		// ����������� ��������
		Array(Array<T>&& src) { data = src.data; count = src.count; max_count = src.max_count; grow = src.grow; src.init(); }
		// ����������
		~Array() { reset(); }
		// �����
		void reset() { remove(0, count); SSH_DEL(data); init(); }
		// ���������� ������
		void resize(ssh_u _count) { reset(); alloc(_count); }
		// �������� �������
		const Array& operator += (const T& elem) { return insert(count, elem, 1); }
		// �������� ������
		const Array& operator += (const Array<T>& src) { return insert(count, src.data(), src.size()); }
		// ��������� ������
		const Array& operator = (const Array<T>& src) { reset(); grow = src.grow; return insert(0, src); }
		// �������� �������
		T& operator [] (ssh_u idx) const { return data[idx]; }
		// �������� ��������
		const Array& operator = (Array<T>&& src) { reset(); data = src.data; count = src.count; max_count = src.max_count; grow = src.grow; src.init(); return *this; }
		// ��������� �������� �� �������
		const Array& set(ssh_u idx, const T& elem)
		{
			if(idx < count)
			{
				release_node<T, SSH_IS_PTR(T)>::release(data[idx]);
				data[idx] = elem;
			}
			return *this;
		}
		// ������� �������� �� �������
		const Array& insert(ssh_u idx, const T* elem, ssh_u _count)
		{
			if(idx <= count)
			{
				alloc(_count);
				memmove(data + idx + _count, data + idx, (count - idx) * sizeof(T));
				for(ssh_u i = 0; i < _count; i++) data[i + idx] = elem[i];
			}
			return *this;
		}
		// �������� ��������
		const Array& remove(ssh_u idx, ssh_u _count)
		{
			if(idx < count)
			{
				if((idx + _count) > count) _count = (count - idx);
				for(ssh_u i = 0; i < _count; i++) release_node<T, SSH_IS_PTR(T)>::release(data[i + idx]);
				ssh_u ll(idx + _count);
				memcpy(data + idx, data + ll, (count - ll) * sizeof(T));
				count -= _count;
			}
			return *this;
		}
		// ����� �������
		ssh_l find(const T& t) const
		{
			for(ssh_u i = 0; i < count; i++)
			{
				if(data[i] == t) return i;
			}
			return -1;
		}
		// ������� ������
		ssh_u size() const { return count; }
		// ������� �� �������
		T& at(ssh_u idx) const { return data[idx]; }
		// ������� ��������� �� ������
		const T* getData() const { return (const T*)data; }
		T* getData() { return (T*)data; }
	protected:
		// ��������
		void init() { count = max_count = 0; data = nullptr; }
		// ��������� ������
		void alloc(ssh_u _count)
		{
			if((count + _count) >= max_count)
			{
				max_count += (_count + grow);
				// �������� ����
				T* ptr((T*)new ssh_b[max_count * sizeof(T)]);
				// �������� ������ ������, ���� ����
				if(data)
				{
					memcpy(ptr, data, count * sizeof(T));
					delete data;
				}
				// �������������� �����
				for(ssh_u i = count; i < max_count; i++) ::new((void*)(ptr + i)) T();
				data = ptr;
			}
			count += _count;
		}
		// ���������� ���������
		ssh_u count = 0;
		// ����������
		ssh_u grow = 32;
		// �������� ���������
		ssh_u max_count = 0;
		// ������
		T* data = nullptr;
	};
}
