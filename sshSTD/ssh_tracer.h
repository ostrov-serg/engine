
#pragma once

#include "ssh_list.h"
#include "ssh_str.h"

namespace ssh
{
	class SSH StackTrace final
	{
	public:
		// ���������������� ������������
		void init(int _depth) { depth = _depth; }
		// �������� ����� �������
		void add(bool is, ssh_cws func, ssh_cws file = nullptr, int line = 0);
		// ������� � ���
		void output();
		// ������/���������
		void start() { is_enabled = true; }
		void stop() { is_enabled = false; }
		// ������� ����������
		bool is_started() const { return is_enabled; }
	protected:
		// ������� ����������
		bool is_enabled = false;
		// ���� �������
		int depth = 256;
		// ������
		int indent = 0;
		// ������
		List<String> elems;
	};
}