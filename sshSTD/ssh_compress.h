
#pragma once

#include "ssh_array.h"

namespace ssh
{
	class SSH LZW
	{
	public:
		enum Consts
		{
			// Размер словаря в элементах
			TABLE_SIZE = 5021,
			// Количество битов в коде
			BITS = 12,
			// Максимальное значение кода
			MAX_CODE = ((1 << BITS) - 1),
			// Специальный код конца потока
			END_OF_STREAM = 256,
			// Значение кода, которое получает первая добавленная в словарь фраза
			FIRST_CODE = 257,
			// Признак свободной ячейки в словаре
			UNUSED = -1
		};
		struct dictionary
		{
			int code_value;
			int prefix_code;
			char character;
		};
		// конструктор
		LZW() : rack(0), mask(0x80), in(nullptr), out(nullptr) {}
		// обработка буфера
		Buffer process(const Buffer& _in, bool is_compress) { in = _in; return (is_compress ? compress(_in.size()) : decompress()); }
	protected:
		// Процедура сжатия файла
		Buffer compress(ssh_u size) noexcept;
		// Процедура декодирования сжатого файла
		Buffer decompress();
		// Процедура декодирования строки. Размещает символы в стеке, возвращает их количество.
		ssh_i decode_string(ssh_i count, ssh_i code) noexcept;
		// Процедура поиска в словаре указанной пары <код фразы, символ>. Для ускорения поиска используется хеш, получаемый из параметров.
		ssh_i find_dictionary_match(int prefix_code, int character) noexcept;
		// установка бит
		void set_bits(ssh_d code) noexcept;
		// получение бит
		ssh_d get_bits(int& size);
		// буферы ввода и  вывода
		ssh_b* in, *out;
		// маска
		ssh_b mask;
		// текущий символ обработки
		int rack;
		// Структура словаря для алгоритма LZW
		dictionary dict[TABLE_SIZE];
		// Стек для декодирования
		char decode_stack[TABLE_SIZE];
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
			// запись бит
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
		// конструктор
		Haffman() : head(nullptr), in(nullptr), out(nullptr) {}
		// деструктор
		~Haffman() { head->reset(); }
		// обработка
		Buffer process(const Buffer& in, bool is_compress) noexcept;
	protected:
		// упаковка
		Buffer compress(ssh_u size) noexcept;
		// распаковка
		Buffer decompress() noexcept;
		// сформировать дерево
		void make_tree(int size) noexcept;
		// отображение диагностики
		void print_haff(Haffman::node* n, int tabs = 1, bool is_tree = true);
		// запись дерева
		void write_tree() noexcept;
		// чтение дерева
		void read_tree() noexcept;
		// чтение бит
		value* read(long val) noexcept;
		// корень дерева
		node* head;
		// значения
		value vals[256];
		// дерево узлов
		Array<node*> nodes;
		// буферы ввода и вывода
		ssh_b* in, *out;
	};

	class SSH Arith
	{
	public:
		enum Consts
		{
			// Количество битов в регистре
			BITS_IN_REGISTER		= 16,
			// Максимально возможное значение в регистре
			TOP_VALUE				= ((1 << BITS_IN_REGISTER) - 1),
			// Диапазоны
			FIRST_QTR				= (TOP_VALUE / 4 + 1),
			HALF					= (2 * FIRST_QTR),
			THIRD_QTR				= (3 * FIRST_QTR),
			NO_OF_CHARS				= 256,
			EOF_SYMBOL				= (NO_OF_CHARS + 1),
			NO_OF_SYMBOLS			= (NO_OF_CHARS + 1),
			// Порог частоты для масштабирования
			MAX_FREQUENCY			= 2047
		};
		// конструктор
		Arith();
		// обработка
		Buffer process(const Buffer& in, bool is_compress);
	protected:
		// упаковка
		Buffer compress(ssh_u size) noexcept;
		// распаковка
		Buffer decompress();
		// Кодирование очередного символа
		void encode_symbol(int symbol) noexcept;
		// Вывод очередного бита сжатой информации
		void output(int bit) noexcept;
		// Обновление модели очередным символом
		void update(int symbol) noexcept;
		// Вывод указанного бита и отложенных ранее
		void output_plus(int bit) noexcept;
		// Ввод очередного бита сжатой информации
		int input(int& size);
		// Декодирование очередного символа
		int decode_symbol(int& size);
		// Таблицы перекодировки
		ssh_b index_to_char[257];
		int char_to_index[256];
		// Таблицы частот
		int cum_freq[258];
		int freq[258];
		// Регистры границ и кода
		long low, high, value;
		// Поддержка побитовых операций
		long bits_to_follow;
		int buffer, bits_to_go;
		// входной и выходной буферы
		ssh_b* in, *out;
	};
}
