
#pragma once

#include "ssh_array.h"

namespace ssh
{
	class SSH LZW
	{
	public:
		struct dictionary
		{
			int code_value;
			int prefix_code;
			char character;
		};
		// �����������
		LZW() : rack(0), mask(0x80), in(nullptr), out(nullptr) {}
		// ��������� ������
		Buffer process(const Buffer& _in, bool is_compress) { in = _in; return (is_compress ? compress(_in.size()) : decompress()); }
	protected:
		// ��������� ������ �����
		Buffer compress(ssh_u size) noexcept;
		// ��������� ������������� ������� �����
		Buffer decompress();
		// ��������� ������������� ������. ��������� ������� � �����, ���������� �� ����������.
		ssh_i decode_string(ssh_i count, ssh_i code) noexcept;
		// ��������� ������ � ������� ��������� ���� <��� �����, ������>. ��� ��������� ������ ������������ ���, ���������� �� ����������.
		ssh_i find_dictionary_match(int prefix_code, int character) noexcept;
		// ��������� ���
		void set_bits(ssh_d code) noexcept;
		// ��������� ���
		ssh_d get_bits(int& size);
		// ������ ����� �  ������
		ssh_b* in, *out;
		// �����
		ssh_b mask;
		// ������� ������ ���������
		int rack;
	private:
		// ������ ������� � ���������
		static const int TABLE_SIZE = 5021;
		// ���������� ����� � ����
		static const int BITS = 12;
		// ������������ �������� ����
		static const int MAX_CODE = ((1 << BITS) - 1);
		// ����������� ��� ����� ������
		static const int END_OF_STREAM = 256;
		// �������� ����, ������� �������� ������ ����������� � ������� �����
		static const int FIRST_CODE = 257;
		// ������� ��������� ������ � �������
		static const int UNUSED = -1;
		// ��������� ������� ��� ��������� LZW
		dictionary dict[TABLE_SIZE];
		// ���� ��� �������������
		char decode_stack[TABLE_SIZE];
	};

	class SSH Arith
	{
	public:
		// �����������
		Arith();
		// ���������
		Buffer process(const Buffer& in, bool is_compress);
	protected:
		// ��������
		Buffer compress(ssh_u size) noexcept;
		// ����������
		Buffer decompress();
		// ����������� ���������� �������
		void encode_symbol(int symbol) noexcept;
		// ����� ���������� ���� ������ ����������
		void output(int bit) noexcept;
		// ���������� ������ ��������� ��������
		void update(int symbol) noexcept;
		// ����� ���������� ���� � ���������� �����
		void output_plus(int bit) noexcept;
		// ���� ���������� ���� ������ ����������
		int input(int& size);
		// ������������� ���������� �������
		int decode_symbol(int& size);
		// ������� �������������
		ssh_b index_to_char[257];
		int char_to_index[256];
		// ������� ������
		int cum_freq[258];
		int freq[258];
		// �������� ������ � ����
		long low, high, value;
		// ��������� ��������� ��������
		long bits_to_follow;
		int buffer, bits_to_go;
		// ������� � �������� ������
		ssh_b* in, *out;
	private:
		// ���������� ����� � ��������
		static const int BITS_IN_REGISTER = 16;
		// ����������� ��������� �������� � ��������
		static const int TOP_VALUE = ((1 << BITS_IN_REGISTER) - 1);
		// ���������
		static const int FIRST_QTR = (TOP_VALUE / 4 + 1);
		static const int HALF = (2 * FIRST_QTR);
		static const int THIRD_QTR = (3 * FIRST_QTR);
		static const int NO_OF_CHARS = 256;
		static const int EOF_SYMBOL = (NO_OF_CHARS + 1);
		static const int NO_OF_SYMBOLS = (NO_OF_CHARS + 1);
		// ����� ������� ��� ���������������
		static const int MAX_FREQUENCY = 2047;
	};
}
