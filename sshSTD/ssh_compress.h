
#pragma once

#include "ssh_array.h"

namespace ssh
{
	class SSH RLE
	{
	public:
		// конструктор
		RLE() : in(nullptr), out(nullptr) {}
		// обработка
		Buffer process(const Buffer& _in, bool is_compress) noexcept { in = _in; return (is_compress ? compress(_in.size()) : decompress(_in.size())); }
	protected:
		// упаковщик
		Buffer compress(ssh_u size) noexcept;
		// распаковщик
		Buffer decompress(ssh_u size) noexcept;
		// буферы ввода и вывода
		ssh_b* in, *out;
	};

	#define SSH_BWT_BLOCK_LENGHT		2048
	
	class SSH BWT
	{
	public:
		// конструктор
		BWT() : in(nullptr), RT(nullptr), LT(nullptr) {}
		// обработка
		Buffer process(const Buffer& _in, bool is_transform) noexcept { in = _in; return (is_transform ? transform(_in.size()) : untransform(_in.size())); }
	protected:
		// установка текущего значения после трансформации
		void set_val(int idx) noexcept;
		// получение текущего значения на этапе трансформации
		ssh_b get_val(int idx) noexcept;
		// рекурсивная процедура сортировки
		void sort() noexcept;
		// блочное восстановление
		void untransform_block(int size, ssh_w* vec) noexcept;
		// блочная трансформация
		void transform_block(int size, bool is_txt = true) noexcept;
		// Процедура трансформации данных
		Buffer transform(ssh_u size) noexcept;
		// Процедура восстановление данных
		Buffer untransform(ssh_u size) noexcept;
		// буфер индексов слов
		ssh_w* RT;
		// буфер индексов букв
		ssh_w* LT;
		// указатель на буфер индексов
		ssh_b* _index;
		// указатель на текущий блок трансформации
		ssh_b* _result;
		// входной буфер
		ssh_b* in;
		// количество разрядов
		ssh_w keys;
		// текущий индекс буквы
		ssh_w idx_lit;
	};
	
	class SSH MTF
	{
	public:
		// конструктор
		MTF() : in(nullptr), out(nullptr), sz_alphabit(0) { }
		// обработка
		Buffer process(const Buffer& _in, bool is_transform) { in = _in; return (is_transform ? transform(_in.size()) : untransform(_in.size())); }
	protected:
		// Процедура трансформации данных
		Buffer transform(ssh_u size) noexcept;
		// Процедура восстановление данных
		Buffer untransform(ssh_u size) noexcept;
		// буферы ввода и вывода
		ssh_b* in, *out;
		// размер алфавита
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
		// конструктор
		LZW() : rack(0), mask(0x80), in(nullptr), out(nullptr) {}
		// обработка буфера
		Buffer process(const Buffer& _in, bool is_compress) { in = _in; return (is_compress ? compress(_in.size()) : decompress()); }
	protected:
		// Процедура сжатия данных
		Buffer compress(ssh_u size) noexcept;
		// Процедура декодирования сжатых данных
		Buffer decompress();
		// Процедура декодирования строки. Размещает символы в стеке, возвращает их количество.
		ssh_i decode_string(ssh_i count, ssh_i code) noexcept;
		// Процедура поиска в словаре указанной пары <код фразы, символ>. Для ускорения поиска используется хеш, получаемый из параметров.
		ssh_i find_dictionary_match(int prefix_code, int character) noexcept;
		// установка бит
		void set_bits(ssh_d code) noexcept;
		// получение бит
		ssh_d get_bits(int& size);
		// буферы ввода и вывода
		ssh_b* in, *out;
		// маска
		ssh_b mask;
		// текущий символ обработки
		int rack;
	private:
		// Размер словаря в элементах
		static const int TABLE_SIZE = 5021;
		// Количество битов в коде
		static const int BITS = 12;
		// Максимальное значение кода
		static const int MAX_CODE = ((1 << BITS) - 1);
		// Специальный код конца потока
		static const int END_OF_STREAM = 256;
		// Значение кода, которое получает первая добавленная в словарь фраза
		static const int FIRST_CODE = 257;
		// Признак свободной ячейки в словаре
		static const int UNUSED = -1;
		// Структура словаря
		dictionary dict[TABLE_SIZE];
		// Стек для декодирования
		char decode_stack[TABLE_SIZE];
	};

	class SSH Arith
	{
	public:
		// конструктор
		Arith() { }
		// обработка
		Buffer process(const Buffer& _in, bool is_compress) noexcept { in = _in; init(); return (is_compress ? compress(_in.size()) : decompress(_in.size())); }
	protected:
		// упаковка
		Buffer compress(ssh_u size) noexcept;
		// распаковка
		Buffer decompress(ssh_u size) noexcept;
		// инициализация среды
		void init() noexcept;
		// Кодирование очередного символа
		void encode_symbol(int symbol) noexcept;
		// Вывод очередного бита сжатой информации
		void output(int bit) noexcept;
		// Обновление модели очередным символом
		void update(int symbol) noexcept;
		// Процедура переноса найденных битов
		void output_plus(int bit) noexcept;
		// Ввод очередного бита сжатой информации
		int input() noexcept;
		// Декодирование очередного символа
		int decode_symbol() noexcept;
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
	private:
		// Количество битов в регистре
		static const int BITS_IN_REGISTER	= 16;
		// Максимально возможное значение в регистре (65535)
		static const int TOP_VALUE			= ((1 << BITS_IN_REGISTER) - 1);
		// Диапазоны
		static const int FIRST_QTR			= (TOP_VALUE / 4 + 1);	// 16384
		static const int HALF				= (2 * FIRST_QTR);		// 32768
		static const int THIRD_QTR			= (3 * FIRST_QTR);		// 49152
		static const int NO_OF_CHARS		= 256;
		static const int EOF_SYMBOL			= (NO_OF_CHARS + 1);	// 257
		static const int NO_OF_SYMBOLS		= (NO_OF_CHARS + 1);	// 257
		// Порог частоты для масштабирования
		static const int MAX_FREQUENCY		= 2047;
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
		Haffman() {}
		// деструктор
		~Haffman() { head->reset(); }
		// обработка
		Buffer process(const Buffer& in, bool is_compress) noexcept;
	protected:
		void init() noexcept;
		// упаковка
		Buffer compress(ssh_u size) noexcept;
		// распаковка
		Buffer decompress() noexcept;
		// сформировать дерево
		void make_tree(int size) noexcept;
		// отображение диагностики
		void print_haff(Haffman::node* n, int tabs = 1, bool is_tree = true);
		// сохранение дерева
		void store_tree() noexcept;
		// восстановление дерева
		void restore_tree() noexcept;
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

	#define SSH_COMPRESS_RLE		0x01
	#define SSH_COMPRESS_LZW		0x02
	#define SSH_COMPRESS_ARI		0x04
	
	Buffer SSH ssh_compress(const Buffer& in);
	Buffer SSH ssh_decompress(const Buffer& in);
}
