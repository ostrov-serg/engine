
#pragma once

#include "ssh_array.h"

namespace ssh
{
	class SSH RLE
	{
	public:
		// �����������
		RLE() : in(nullptr), out(nullptr) {}
		// ���������
		Buffer process(const Buffer& _in, bool is_compress) { in = _in; return (is_compress ? compress(_in.size()) : decompress(_in.size())); }
	protected:
		// ���������
		Buffer compress(ssh_u size) noexcept;
		// �����������
		Buffer decompress(ssh_u size) noexcept;
		// ������ ���������, ����� � ������
		ssh_b* in, *out;
	};

	#define SSH_BWT_BLOCK_LENGHT		2048
	
	class SSH BWT
	{
	public:
		// �����������
		BWT() : in(nullptr), RT(nullptr), LT(nullptr) {}
		// ���������
		Buffer process(const Buffer& _in, bool is_transform) noexcept { in = _in; return (is_transform ? transform(_in.size()) : untransform(_in.size())); }
	protected:
		// ��������� �������� �������� ����� �������������
		void set_val(int idx) noexcept;
		// ��������� �������� �������� �� ����� �������������
		ssh_b get_val(int idx) noexcept;
		// ����������� ��������� ����������
		void sort(int level) noexcept;
		// ������� ��������������
		void untransform_block(int size, ssh_w* vec) noexcept;
		// ������� �������������
		void transform_block(int size, bool is_txt = true) noexcept;
		// ��������� ������������� ������
		Buffer transform(ssh_u size) noexcept;
		// ��������� �������������� ������
		Buffer untransform(ssh_u size) noexcept;
		// ����� �������� ����
		ssh_w* RT;
		// ����� �������� ����
		ssh_w* LT;
		// ��������� �� ����� ��������
		ssh_b* _index;
		// ��������� �� ������� ���� �������������
		ssh_b* _result;
		// ������� �����
		ssh_b* in;
		// ���������� ��������
		ssh_w keys;
		// ������� ������ �����
		ssh_w idx_lit;
	};
	
	class SSH MTF
	{
	public:
		// �����������
		MTF() : in(nullptr), out(nullptr), sz_alphabit(0) { }
		// ���������
		Buffer process(const Buffer& _in, bool is_transform) { in = _in; return (is_transform ? transform(_in.size()) : untransform(_in.size())); }
	protected:
		// ��������� ������������� ������
		Buffer transform(ssh_u size) noexcept;
		// ��������� �������������� ������
		Buffer untransform(ssh_u size) noexcept;
		// ������ ����� � ������
		ssh_b* in, *out;
		// ������ ��������
		ssh_w sz_alphabit;
	};
	
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
		// ��������� ������ ������
		Buffer compress(ssh_u size) noexcept;
		// ��������� ������������� ������ ������
		Buffer decompress();
		// ��������� ������������� ������. ��������� ������� � �����, ���������� �� ����������.
		ssh_i decode_string(ssh_i count, ssh_i code) noexcept;
		// ��������� ������ � ������� ��������� ���� <��� �����, ������>. ��� ��������� ������ ������������ ���, ���������� �� ����������.
		ssh_i find_dictionary_match(int prefix_code, int character) noexcept;
		// ��������� ���
		void set_bits(ssh_d code) noexcept;
		// ��������� ���
		ssh_d get_bits(int& size);
		// ������ ����� � ������
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
		// ��������� �������
		dictionary dict[TABLE_SIZE];
		// ���� ��� �������������
		char decode_stack[TABLE_SIZE];
	};

	class SSH Arith
	{
	public:
		// �����������
		Arith() { }
		// ���������
		Buffer process(const Buffer& _in, bool is_compress) { in = _in; return (is_compress ? compress(_in.size()) : decompress()); }
	protected:
		// ��������
		Buffer compress(ssh_u size) noexcept;
		// ����������
		Buffer decompress();
		// ������������� �����
		void init();
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
		static const int BITS_IN_REGISTER	= 16;
		// ����������� ��������� �������� � ��������
		static const int TOP_VALUE			= ((1 << BITS_IN_REGISTER) - 1);
		// ���������
		static const int FIRST_QTR			= (TOP_VALUE / 4 + 1);
		static const int HALF				= (2 * FIRST_QTR);
		static const int THIRD_QTR			= (3 * FIRST_QTR);
		static const int NO_OF_CHARS		= 256;
		static const int EOF_SYMBOL			= (NO_OF_CHARS + 1);
		static const int NO_OF_SYMBOLS		= (NO_OF_CHARS + 1);
		// ����� ������� ��� ���������������
		static const int MAX_FREQUENCY = 2047;
	};

	class SSH Haffman
	{
	public:
#pragma pack(push, 1)
		struct node;
		struct value
		{
			value() : len(0) {}
			union
			{
				node* ref = nullptr;
				ssh_u val;
			};
			ssh_w len;
			// ������ ���
			void write(ssh_b** p, int& shift) noexcept;
		};
		struct node
		{
			SSH_NEW_DECL(node, 128);
			node() : l(nullptr), r(nullptr), val(nullptr), freq(0) {}
			node(node* _l, node* _r, int f) : l(_l), r(_r), freq(f), val(nullptr) {}
			int freq;
			value* val;
			node* l;
			node* r;
		};
#pragma pack(pop)
		// �����������
		Haffman() : head(nullptr), in(nullptr), out(nullptr) {}
		// ����������
		~Haffman() { head->reset(); }
		// ���������
		Buffer process(const Buffer& in, bool is_compress) noexcept;
	protected:
		// ��������
		Buffer compress(ssh_u size) noexcept;
		// ����������
		Buffer decompress() noexcept;
		// ������������ ������
		void make_tree(int size) noexcept;
		// ����������� �����������
		void print_haff(Haffman::node* n, int tabs = 1, bool is_tree = true);
		// ������ ������
		void store_tree() noexcept;
		// ������ ������
		void restore_tree() noexcept;
		// ������ ���
		value* read(long val) noexcept;
		// ������ ������
		node* head;
		// ��������
		value vals[256];
		// ������ �����
		Array<node*> nodes;
		// ������ ����� � ������
		ssh_b* in, *out;
	};

	#define SSH_COMPRESS_MTF		0x01
	#define SSH_COMPRESS_RLE		0x02
	#define SSH_COMPRESS_LZW		0x04
	#define SSH_COMPRESS_ARI		0x08
	#define SSH_COMPRESS_HFM		0x10

	Buffer ssh_compress(const Buffer& in, int opt);
	Buffer ssh_decompress(const Buffer& in);
}
