
#pragma once

namespace ssh
{
	template <typename T> class Array
	{
	public:
		// конструктор по умолчанию
		Array() { }
		// инициализирующий конструктор
		Array(ssh_u _count, ssh_u _grow) : grow(_grow) { alloc(_count); }
		// конструктор из списка инициализации
		Array(const std::initializer_list<T>& _list) { insert(0, _list.begin(), _list.size()); }
		// конструктор копии
		Array(const Array<T>& src) { *this = src; }
		// конструктор переноса
		Array(Array<T>&& src) { data = src.data; count = src.count; max_count = src.max_count; grow = src.grow; src.init(); }
		// деструктор
		~Array() { reset(); }
		// сброс
		void reset() { remove(0, count); SSH_DEL(data); init(); }
		// установить размер
		void resize(ssh_u _count) { reset(); alloc(_count); }
		// добавить элемент
		const Array& operator += (const T& elem) { return insert(count, elem, 1); }
		// добавить массив
		const Array& operator += (const Array<T>& src) { return insert(count, src.data(), src.size()); }
		// заместить массив
		const Array& operator = (const Array<T>& src) { reset(); grow = src.grow; return insert(0, src); }
		// оператор индекса
		T& operator [] (ssh_u idx) const { return data[idx]; }
		// оператор переноса
		const Array& operator = (Array<T>&& src) { reset(); data = src.data; count = src.count; max_count = src.max_count; grow = src.grow; src.init(); return *this; }
		// установка элемента по индексу
		const Array& set(ssh_u idx, const T& elem)
		{
			if(idx < count)
			{
				release_node<T, SSH_IS_PTR(T)>::release(data[idx]);
				data[idx] = elem;
			}
			return *this;
		}
		// вставка элемента по индексу
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
		// удаление элемента
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
		// найти элемент
		ssh_l find(const T& t) const
		{
			for(ssh_u i = 0; i < count; i++)
			{
				if(data[i] == t) return i;
			}
			return -1;
		}
		// вернуть размер
		ssh_u size() const { return count; }
		// вернуть по индексу
		T& at(ssh_u idx) const { return data[idx]; }
		// вернуть указатель на данные
		const T* getData() const { return (const T*)data; }
		T* getData() { return (T*)data; }
	protected:
		// очистить
		void init() { count = max_count = 0; data = nullptr; }
		// выделение памяти
		void alloc(ssh_u _count)
		{
			if((count + _count) >= max_count)
			{
				max_count += (_count + grow);
				// выделяем блок
				T* ptr((T*)new ssh_b[max_count * sizeof(T)]);
				// копируем старые данные, если есть
				if(data)
				{
					memcpy(ptr, data, count * sizeof(T));
					delete data;
				}
				// инициализируем новые
				for(ssh_u i = count; i < max_count; i++) ::new((void*)(ptr + i)) T();
				data = ptr;
			}
			count += _count;
		}
		// количество элементов
		ssh_u count = 0;
		// приращение
		ssh_u grow = 32;
		// выделено элементов
		ssh_u max_count = 0;
		// данные
		T* data = nullptr;
	};
}
