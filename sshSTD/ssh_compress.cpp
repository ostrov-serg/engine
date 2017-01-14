
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
		// �������� �� ������������ �������� �������
		if(cum_freq[0] == MAX_FREQUENCY)
		{
			int cum(0);
			// ��������������� ������ ��� ������������
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
		// ���������� �������� � �������� ������
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
			if(!--size) SSH_THROW(L"������������ ������� ������ ��� ���������� ��������������� �����������!");
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
		// �������� �������� ������
		range = (long)(high - low) + 1;
		high = low + (range * cum_freq[symbol - 1]) / cum_freq[0] - 1;
		low = low + (range * cum_freq[symbol]) / cum_freq[0];
		// ���������� ��������� �����
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
			// ����� ����� � ����������� ���������� ����
			low = 2 * low;
			high = 2 * high + 1;
		}
	}

	int Arith::decode_symbol(int& size)
	{
		long range;
		int cum, symbol;
		// ����������� �������� �������� ������
		range = (long)(high - low) + 1;
		// ��������������� �������� � �������� ����
		cum = (int)((((long)(value - low) + 1) * cum_freq[0] - 1) / range);
		// ����� ���������������� ������� � ������� ������
		for(symbol = 1; cum_freq[symbol] > cum; symbol++);
		// �������� ������
		high = low + (range * cum_freq[symbol - 1]) / cum_freq[0] - 1;
		low = low + (range * cum_freq[symbol]) / cum_freq[0];
		// �������� ���������� ������� �� �������� ������
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
			// ����� ����� � ����������� ���������� ����
			low = 2 * low;
			high = 2 * high + 1;
			value = 2 * value + input(size);
		}
		return symbol;
	}

	Buffer Arith::process(const Buffer& _in, bool is_compress)
	{
		LZW lzw;
		MTF mtf;
		if(is_compress)
		{
			Buffer _mtf(mtf.process(_in, true));
			Buffer _lzw(lzw.process(_mtf, true));
			in = _lzw;
			return compress(_lzw.size());
		}
		in = _in;
		return mtf.process(lzw.process(decompress(), false), false);
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
		// ������� ���������� ������
		bits_to_follow++;
		output_plus(low >= FIRST_QTR);
		*out++ = (buffer >> bits_to_go);
		// ������� ������
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
				if(!--size) SSH_THROW(L"������������ ������� ������ ��� ���������� LZW!");
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

		// �������������
		next_code = FIRST_CODE;
		for(i = 0; i < TABLE_SIZE; i++) dict[i].code_value = UNUSED;
		// ������� ������ ������
		string_code = *in++;
		// ���� �� ����� ���������
		for(ssh_u i = 0 ; i < (size - 1); i++)
		{
			character = *in++;
			// ������� ����� � ������� ���� <�����, ������>
			index = find_dictionary_match(string_code, character);
			// ������������ �������
			if(dict[index].code_value != -1) string_code = dict[index].code_value;
			// ����� ���� � ������� ���
			else
			{
				// ���������� � �������
				if(next_code <= MAX_CODE)
				{
					dict[index].code_value = next_code++;
					dict[index].prefix_code = string_code;
					dict[index].character = (char)character;
				}
				// ������ ����
				set_bits(string_code);
				string_code = character;
			}
		}
		// ���������� �����������
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
				// ��������� ��������� �������������� ��������
				if(new_code >= next_code)
				{
					decode_stack[0] = (char)character;
					count = decode_string(1, old_code);
				}
				else count = decode_string(0, new_code);
				character = decode_stack[count - 1];
				// ������ ��������������� ������
				while(count > 0) *out++ = decode_stack[--count];
				// ���������� �������
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
		// ���������� ��������� �������� ���-�������
		index = (character << (BITS - 8)) ^ prefix_code;
		// ���������� ��������� ��������
		if(index == 0) offset = 1; else offset = TABLE_SIZE - index;
		while(true)
		{
			// ��� ������ ������� �� ������������
			if(dict[index].code_value == UNUSED) return index;
			// ������� ������������
			if(dict[index].prefix_code == prefix_code && dict[index].character == (char)character) return index;
			// ��������. ���������� � ��������� ������� �� ����������
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

	Buffer MTF::transform(ssh_u size) noexcept
	{
		int i;
		ssh_b alphabit[512];
		// ������� �������
		for(i = 0; i < size; i++)
		{
			auto l(in[i]);
			if(!alphabit[l + 256]) alphabit[sz_alphabit++] = l, alphabit[in[i] + 256] = 1;
		}
		// ������� �����
		Buffer _out(size + sz_alphabit + 2); out = _out;
		*(ssh_w*)out = sz_alphabit;
		out = (ssh_b*)ssh_memcpy(out + 2, alphabit, sz_alphabit);
		// ������������
		for(i = 0; i < size; i++)
		{
			auto l(*in++);
			ssh_b idx(0);
			while(alphabit[idx] != l) { idx++; }
			memmove(&alphabit[1], &alphabit[0], idx);
			alphabit[0] = l;
			*out++ = idx;
		}
		return _out;
	}

	Buffer MTF::untransform(ssh_u size) noexcept
	{
		// ��������� ������� � ��� ������
		sz_alphabit = *(ssh_w*)in;
		ssh_b* alphabit(in + 2);
		in += sz_alphabit + 2;
		// ������� �����
		size -= (sz_alphabit + 2);
		Buffer _out(size); out = _out;
		// �������������
		while(size--)
		{
			auto idx(*in++);
			auto s(alphabit[idx]);
			memmove(alphabit + 1, alphabit, idx);
			*out++ = s;
			*alphabit = s;
		}
		return _out;
	}

	void BWT::sort(int level) noexcept
	{
		// ���� �� ��������
		for(int i = 0; i < 256; i++)
		{
			// ���� ������������� i-� �����
			auto idx(LT[level * 256 + i]);
			LT[level * 256 + i] = 0;
			// ��������� ����� ��� ���� �����
			while(idx)
			{
				// i-� ������ ������������ ������ �������, ������ ��������������� ����� ������� ����������� ����� ���������
				if(!RT[idx])
				{
					set_val(idx);
					break;
				}
				else
				{
					// � ������ ������������� ������������� i-�� �������:
					if(level == keys)
					{
						// ����� ���� ������ �� ���� ������:
						while(idx)
						{
							// ��������� ������� ������ � ������� ��������
							set_val(idx);
							idx = RT[idx];
						}
					}
					else
					{
						// ���������� �������� ������� ����: ���������� �� ������� ����
						int newlevel(level + 1);
						while(idx)
						{
							auto nextrec(RT[idx]);
							auto c(get_val(idx, level));
							RT[idx] = LT[newlevel * 256 + c];
							LT[newlevel * 256 + c] = idx;
							idx = nextrec;
						}
						// ���������� ������� ���������
						sort(newlevel);
					}
				}
			}
		}
	}

	void BWT::set_val(int idx) noexcept
	{
		if(idx == 1) idx_src = idx_lit;
		result[idx_lit++] = get_val(idx, keys - 1);
	}

	ssh_b BWT::get_val(int level, int idx) noexcept
	{
		return in[((level - 1) + idx) % keys];
	}

	int BWT::transform(ssh_u size) noexcept
	{
		keys = (int)size;
		result = Buffer(keys);
		// ������������� �������� ����
		LT = BufferW(256 * keys);
		ssh_memzero(LT, LT.size());
		// ������������� �������� ����
		RT = BufferW(keys);
		ssh_memzero(RT, RT.size());
		// ���� 1 - ���������� ����� �� ������ �����
		for(int idx = 1; idx != keys + 1; idx++)
		{
			auto c(get_val(idx, 0));
			RT[idx] = LT[c];
			LT[c] = idx;
		}
		// ��������� ������� ��������� ��������� ������� � ������.
		sort(0);
		ssh_memcpy(in, result, keys);
		return idx_src;
	}

	int BWT::untransform(ssh_u size) noexcept
	{
		return 0;
	}
}

/*
void PutCurrRecord(int recno)
{
static int i = 0;
result[i++] = _str[recno - 1];
}

// ������� ��������� ������ ����� 1-�� �����: ������������������ �����, �������� �� ����� ����� � ���������
void process(int level, int keys)
{
}

// ���������� ������������ �������� �����
void ABCsort(int keys)
{
_str[0] = "carmel ";
_str[1] = "adela  ";
_str[2] = "beatrix";
_str[3] = "abbey  ";
_str[4] = "abigale";
_str[5] = "barbara";
_str[6] = "camalia";
_str[7] = "belinda";
_str[8] = "beckie ";

}
*/