
#pragma once

#include "ssh_list.h"
#include "ssh_str.h"

namespace ssh
{
	class SSH StackTrace final
	{
	public:
		// инициализировать трассировщик
		void init(int _depth) { depth = _depth; }
		// добавить новый элемент
		void add(bool is, ssh_cws func, ssh_cws file = nullptr, int line = 0);
		// вывести в лог
		void output();
		// запуск/остановка
		void start() { is_enabled = true; }
		void stop() { is_enabled = false; }
		// признак отключения
		bool is_started() const { return is_enabled; }
	protected:
		// признак активности
		bool is_enabled = false;
		// макс глубина
		int depth = 256;
		// отступ
		int indent = 0;
		// список
		List<String> elems;
	};
}