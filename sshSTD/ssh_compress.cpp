
#include "stdafx.h"
#include "ssh_compress.h"

namespace ssh
{
	Arith::Arith() : low(0), buffer(0), value(0), bits_to_follow(0), in(nullptr), out(nullptr)
	{
		int i;
		for(i = 0; i < 256; i++)
		{
			char_to_index[i] = i + 1;
			index_to_char[i + 1] = i;
		}
		for(i = 0; i <= 257; i++)
		{
			freq[i] = 1;
			cum_freq[i] = 257 - i;
		}
		freq[0] = 0;
		high = TOP_VALUE;
	}

	void Arith::update(int symbol) noexcept
	{
		int i;
		// проверка на переполнение счетчика частоты
		if(cum_freq[0] == MAX_FREQUENCY)
		{
			int cum(0);
			// масштабирование частот при переполнении
			for(i = 257; i >= 0; i--)
			{
				freq[i] = (freq[i] + 1) / 2;
				cum_freq[i] = cum;
				cum += freq[i];
			}
		}
		for(i = symbol; freq[i] == freq[i - 1]; i--);
		if(i < symbol)
		{
			int ch_i(index_to_char[i]);
			int ch_symbol(index_to_char[symbol]);

			index_to_char[i] = ch_symbol;
			index_to_char[symbol] = ch_i;

			char_to_index[ch_i] = symbol;
			char_to_index[ch_symbol] = i;
		}
		// обновление значений в таблицах частот
		freq[i]++;
		while(i-- > 0) cum_freq[i]++;
	}

	void Arith::output(int bit) noexcept
	{
		buffer >>= 1;
		if(bit) buffer |= 0x80;
		if(!--bits_to_go)
		{
			*out++ = buffer;
			bits_to_go = 8;
		}
	}

	int Arith::input(int& size)
	{
		int t;

		if(!bits_to_go)
		{
			buffer = *in++;
			if(!--size) SSH_THROW(L"Некорректные входные данные при распаковке Арифметического кодирования!");
			bits_to_go = 8;
		}
		t = buffer & 1;
		buffer >>= 1;
		bits_to_go--;
		return t;
	}

	void Arith::output_plus(int bit) noexcept
	{
		output(bit);
		while(bits_to_follow > 0)
		{
			output(!bit);
			bits_to_follow--;
		}
	}

	void Arith::encode_symbol(int symbol) noexcept
	{
		long range;
		// пересчет значений границ
		range = (long)(high - low) + 1;
		high = low + (range * cum_freq[symbol - 1]) / cum_freq[0] - 1;
		low = low + (range * cum_freq[symbol]) / cum_freq[0];
		// выдвигание очередных битов
		while(true)
		{
			if(high < HALF) output_plus(0);
			else if(low >= HALF)
			{
				output_plus(1);
				low -= HALF;
				high -= HALF;
			}
			else if(low >= FIRST_QTR && high < THIRD_QTR)
			{
				bits_to_follow++;
				low -= FIRST_QTR;
				high -= FIRST_QTR;
			}
			else break;
			// сдвиг влево с втягиванием очередного бита
			low = 2 * low;
			high = 2 * high + 1;
		}
	}

	int Arith::decode_symbol(int& size)
	{
		long range;
		int cum, symbol;
		// определение текущего масштаба частот
		range = (long)(high - low) + 1;
		// масштабирование значения в регистре кода
		cum = (int)((((long)(value - low) + 1) * cum_freq[0] - 1) / range);
		// поиск соответствующего символа в таблице частот
		for(symbol = 1; cum_freq[symbol] > cum; symbol++);
		// пересчет границ
		high = low + (range * cum_freq[symbol - 1]) / cum_freq[0] - 1;
		low = low + (range * cum_freq[symbol]) / cum_freq[0];
		// удаление очередного символа из входного потока
		while(true)
		{
			if(high < HALF) {}
			else if(low >= HALF)
			{
				value -= HALF;
				low -= HALF;
				high -= HALF;
			}
			else if(low >= FIRST_QTR && high < THIRD_QTR)
			{
				value -= FIRST_QTR;
				low -= FIRST_QTR;
				high -= FIRST_QTR;
			}
			else break;
			// сдвиг влево с втягиванием очередного бита
			low = 2 * low;
			high = 2 * high + 1;
			value = 2 * value + input(size);
		}
		return symbol;
	}

	Buffer Arith::process(const Buffer& _in, bool is_compress)
	{
		LZW lzw;
		if(is_compress)
		{
			Buffer _lzw(lzw.process(_in, true));
			in = _lzw;
			return compress(_lzw.size());
		}
		in = _in;
		return lzw.process(decompress(), false);
	}

	Buffer Arith::compress(ssh_u size) noexcept
	{
		int i;
		
		Buffer _out(size * 2 + sizeof(int)); out = _out; *(int*)out = (int)size;
		ssh_b* _out1(out); out += sizeof(int);

		bits_to_go = 8;
		for(i = 0 ; i < size; i++)
		{
			int symbol(char_to_index[*in++]);
			encode_symbol(symbol);
			update(symbol);
		}
		encode_symbol(EOF_SYMBOL);
		// Очистка побитового вывода
		bits_to_follow++;
		output_plus(low >= FIRST_QTR);
		*out++ = (buffer >> bits_to_go);
		// вернуть буффер
		return Buffer(_out, 0, out - _out1);
	}

	Buffer Arith::decompress()
	{
		int symbol;
		
		int size(*(int*)in);
		Buffer _out(size); in += sizeof(int); out = _out;

		bits_to_go = 0;
		for(int i = 1; i <= BITS_IN_REGISTER; i++) value = 2 * value + input(size);
		while((symbol = decode_symbol(size)) != EOF_SYMBOL)
		{
			*out++ = index_to_char[symbol];
			update(symbol);
		}
		return _out;
	}

	void LZW::set_bits(ssh_d code) noexcept
	{
		ssh_d _mask(1L << (BITS - 1));
		while(_mask)
		{
			if(_mask & code) rack |= mask;
			mask >>= 1;
			if(!mask)
			{
				*out++ = rack;
				rack = 0;
				mask = 0x80;
			}
			_mask >>= 1;
		}
	}

	ssh_d LZW::get_bits(int& size)
	{
		ssh_d _mask(1L << (BITS - 1)), return_value;
		return_value = 0;
		while(_mask)
		{
			if(mask == 0x80)
			{
				rack = *in++;
				if(!--size) SSH_THROW(L"Некорректные входные данные при распаковке LZW!");
			}
			if(rack & mask) return_value |= _mask;
			_mask >>= 1;
			mask >>= 1;
			if(!mask) mask = 0x80;
		}
		return return_value;
	}

	Buffer LZW::compress(ssh_u size) noexcept
	{
		int next_code, character, string_code;
		ssh_i index, i;

		Buffer _out(size * 2 + sizeof(int)); out = _out; *(int*)out = (int)size;
		ssh_b* _out1(out); out += sizeof(int);

		// Инициализация
		next_code = FIRST_CODE;
		for(i = 0; i < TABLE_SIZE; i++) dict[i].code_value = UNUSED;
		// Считать первый символ
		string_code = *in++;
		// Пока не конец сообщения
		for(ssh_u i = 0 ; i < (size - 1); i++)
		{
			character = *in++;
			// Попытка найти в словаре пару <фраза, символ>
			index = find_dictionary_match(string_code, character);
			// Соответствие найдено
			if(dict[index].code_value != -1) string_code = dict[index].code_value;
			// Такой пары в словаре нет
			else
			{
				// Добавление в словарь
				if(next_code <= MAX_CODE)
				{
					dict[index].code_value = next_code++;
					dict[index].prefix_code = string_code;
					dict[index].character = (char)character;
				}
				// Выдача кода
				set_bits(string_code);
				string_code = character;
			}
		}
		// Завершение кодирования
		set_bits(string_code);
		set_bits(END_OF_STREAM);
		if(mask != 0x80) *out++ = rack;
		return Buffer(_out, 0, out - _out1);
	}

	Buffer LZW::decompress()
	{
		ssh_i count, next_code, new_code, old_code;
		int character;

		int size(*(int*)in);
		Buffer _out(size); in += sizeof(int); out = _out;
		
		next_code = FIRST_CODE;
		old_code = get_bits(size);
		if(old_code != END_OF_STREAM)
		{
			character = old_code;
			*out++ = old_code;
			while((new_code = get_bits(size)) != END_OF_STREAM)
			{
				// Обработка возможной исключительной ситуации
				if(new_code >= next_code)
				{
					decode_stack[0] = (char)character;
					count = decode_string(1, old_code);
				}
				else count = decode_string(0, new_code);
				character = decode_stack[count - 1];
				// Выдача раскодированной строки
				while(count > 0) *out++ = decode_stack[--count];
				// Обновление словаря
				if(next_code <= MAX_CODE)
				{
					dict[next_code].prefix_code = old_code;
					dict[next_code].character = (char)character;
					next_code++;
				}
				old_code = new_code;
			}
		}
		return _out;
	}

	ssh_i LZW::find_dictionary_match(int prefix_code, int character) noexcept
	{
		int index, offset;
		// Собственно получение значения хеш-функции
		index = (character << (BITS - 8)) ^ prefix_code;
		// Разрешение возможных коллизий
		if(index == 0) offset = 1; else offset = TABLE_SIZE - index;
		while(true)
		{
			// Эта ячейка словаря не использована
			if(dict[index].code_value == UNUSED) return index;
			// Найдено соответствие
			if(dict[index].prefix_code == prefix_code && dict[index].character == (char)character) return index;
			// Коллизия. Подготовка к следующей попытке ее разрешения
			index -= offset;
			if(index < 0) index += TABLE_SIZE;
		}
	}

	ssh_i LZW::decode_string(ssh_i count, ssh_i code) noexcept
	{
		while(code > 255)
		{
			decode_stack[count++] = dict[code].character;
			code = dict[code].prefix_code;
		}
		decode_stack[count++] = (char)code;
		return count;
	}

	void Haffman::print_haff(Haffman::node* n, int tabs, bool is_tree)
	{
#ifdef _DEBUG
		if(is_tree)
		{
			if(n->val) ssh_log->add_msg(ssh_printf(L"%3i[%i] - %i{%*I64b}", (n->val - vals), n->freq, n->val->len, n->val->len, n->val->val), false);
			ssh_log->add_msg(L"\r\n", false);
			String t(L'\t', tabs * 2);
			if(n->r)
			{
				ssh_log->add_msg(ssh_printf(L"%zR-------", t), false);
				print_haff(n->r, tabs + 1, is_tree);
			}
			if(n->l)
			{
				ssh_log->add_msg(ssh_printf(L"%zL-------", t), false);
				print_haff(n->l, tabs + 1, is_tree);
			}
		}
		else
		{
			if(n->val) ssh_log->add_msg(ssh_printf(L"%3i[%i] - %i{%z}\r\n", (n->val - vals), n->freq, n->val->len, n->val->val), false);
			if(n->r) print_haff(n->r, tabs + 1, is_tree);
			if(n->l) print_haff(n->l, tabs + 1, is_tree);
		}
#endif
	}

	static void make_code(Haffman::node* n, ssh_u val, ssh_w len) noexcept
	{
		if(n->val)
		{
			n->val->val = (val >> (64 - len));
			n->val->len = len;
		}
		if(n->l) make_code(n->l, val >> 1, len + 1);
		if(n->r) make_code(n->r, (val >> 1) | 0x8000000000000000, len + 1);
	}

	Buffer Haffman::process(const Buffer& _in, bool is_compress) noexcept
	{
		LZW lzw;
		if(is_compress)
		{
			Buffer _lzw(lzw.process(_in, true));
			in = _lzw;
			return compress(_lzw.size());
		}
		in = _in;
		return lzw.process(decompress(), false);
	}

	void Haffman::make_tree(int size) noexcept
	{
		node* n;
		// 1. считаем частоты вхождения символов и их количество
		for(ssh_u i = 0; i < size; i++)
		{
			int v(in[i]);
			if(!vals[v].ref)
			{
				n = nodes += new node();
				vals[v].ref = n;
				n->val = &vals[v];
			}
			vals[v].ref->freq++;
		}
		ssh_u count(nodes.size());
		// 2. сортируем алфавит по возрастанию
		qsort(nodes.get_data(), count, sizeof(node*), [](const void* _1, const void* _2) -> int
		{
			node* __1(*(node**)_1), *__2(*(node**)_2);
			if(__1->freq < __2->freq) return -1;
			return 1;
		});
		// 3. строим двоичное дерево
		for(int i = 0; i < count / 2; i++)
		{
			auto n1(nodes[i * 2]);
			auto n2(nodes[i * 2 + 1]);
			if(n1->freq > n2->freq) std::swap(n1, n2);
			head = new node(n1, n2, n1->freq + n2->freq);
			int j(i);
			// вставить новый узел с учетом сортировки
			for(; j < count; j++)
			{
				if(nodes[j]->freq >= head->freq) break;
			}
			nodes.insert(j, head);
			count++;
		}
		// 4. составить код Хаффмана
		make_code(head, 0, 0);
		// 5. проверить как создалось дерево
		if(head)
		{
//			ssh_log->add_msg(ssh_printf(L"root-------\r\n"), false);
//			print_haff(head, 1, true);
		}
	}

	Buffer Haffman::compress(ssh_u size) noexcept
	{
		int sh(0);
		// построить дерево
		make_tree((int)size);
		// новый буфер
		Buffer _out(size * 2 + sizeof(int)); out = _out; *(int*)out = (int)size;
		ssh_b* _out1(out); out += sizeof(int);
		// запомнить дерево
		write_tree();
		// закодировать
		for(ssh_u i = 0; i < size; i++) vals[in[i]].write(&out, sh);
		// вернуть буфер
		return Buffer(_out, 0, out - _out1);
	}

	Buffer Haffman::decompress() noexcept
	{
		value* v;
		int sh(0), size(*(int*)in);
		// новый буфер
		Buffer _out(size); in += sizeof(int); out = _out;
		// прочитать дерево из буфера
		read_tree();
		// раскодировать
		for(int i = 0; i < size; i++)
		{
			ssh_w val((*(int*)out) >> sh);
			v = read(val);
			*out = (ssh_b)(v - vals);
			out += ((v->len + sh) / 8);
			sh = (v->len + sh) & 7;
		}
		return _out;
	}

	void Haffman::write_tree() noexcept
	{
		// ?0 - left
		// ?1 - right
		// 1? - value exist
		// xxxxxxxx - val
	}

	void Haffman::read_tree() noexcept
	{
	}

	void Haffman::value::write(ssh_b ** p, int & shift) noexcept
	{
		*(ssh_w*)(*p) |= (val & ((1 << len) - 1)) << shift;
		*p += ((shift + len) / 8);
		shift = (shift + len) & 7;
	}

	Haffman::value * Haffman::read(long val) noexcept
	{
		node* nn, *n(head);
		int bit(0);
		do { nn = n; if(_bittest(&val, bit++)) n = n->r; else n = n->l; } while(n);
		return nn->val;
	}
}
