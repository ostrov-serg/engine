
#include "stdafx.h"
#include "ssh_compress.h"

extern "C"
{
	void asm_ssh_bwt_transform(void* _this, int size);
	void asm_ssh_bwt_untransform(void* _this, int size, void* vec);
}

namespace ssh
{
	void BWT::sort(int level) noexcept
	{
		// ÷икл по алфавиту
		for(int i = 0; i < 256; i++)
		{
			// »щем использование i-й буквы
			auto idx(LT[i]);
			LT[i] = 0;
			// —канируем ветвь дл€ этой буквы
			if(idx)
			{
				// i-й символ используетс€ только однажды, значит отсортированна€ часть массива пополнилась новым элементом
				if(RT[idx] == 0)
				{
					set_val(idx);
					//	break;
				}
				else
				{
					// ¬ случае многократного использовани€ i-го символа:
					if(level >= (keys - 1))
					{
						// ¬ывод всех данных на этом уровне:
						do
						{
							// ƒобавл€ем текущую запись в таблицу индексов
							set_val(idx);
							idx = RT[idx];
						} while(idx);
					}
					else
					{
						// ѕродолжать уточн€ть пор€док слов: опускаемс€ на уровень вниз
						LT += 256;
						ssh_memzero(LT, 512);
						do
						{
							auto nextrec(RT[idx]);
							auto c(get_val(idx + level));
							RT[idx] = LT[c];
							LT[c] = idx;
							idx = nextrec;
						} while(idx);
						// ѕродолжаем процесс уточнени€
						sort(level + 1);
						LT -= 256;
					}
				}
			}
		}
	}

	void BWT::set_val(int idx) noexcept
	{
		if(idx == 1)
		{
			*(ssh_w*)_index = idx_lit;
			_index += 2;
		}
		_result[idx_lit++] = get_val(idx + keys - 1);
	}

	ssh_b BWT::get_val(int idx) noexcept
	{
		return in[(idx - 1) % keys];
	}

	void BWT::transform_block(int size, bool is_txt) noexcept
	{
		idx_lit = 0;
		keys = size;
		// »нициализаци€ индексов букв
		ssh_memzero(LT, 512);
		// √руппируем слова по первой букве
		ssh_b is(0);
		for(int idx = 1; idx < (keys + 1); idx++)
		{
			ssh_b c(in[idx - 1]);
			// проверить на однотипные элементы
			is |= (c != *in);
			_result[idx - 1] = c;
			RT[idx] = LT[c];
			LT[c] = idx;
		}
		// «апускаем процесс уточнени€ положени€ записей в списке
		if(is) sort(0);
		in += size;
		_result += size;
	}

	void BWT::untransform_block(int size, ssh_w* vec) noexcept
	{
		int sum(0), i;
		int count[256];
		// извлекаем исходный индекс
		ssh_d idx(*(ssh_w*)_index); _index += 2;
		// инициализируем вектор
		ssh_memzero(count, 256 * 4);
		for(i = 0; i < size; i++)
			count[in[i]]++;
		for(i = 0; i < 256; i++)
		{
			sum += count[i];
			count[i] = sum - count[i];
		}
		for(i = 0; i < size; i++)
		{
			int idx(count[in[i]]++);
			vec[idx] = i;
		}
		// запускаем процесс восстановление в соответствии с вектором
		for(i = 0; i < size; i++)
		{
			idx = vec[idx];
			*_result++ = in[idx];
		}
		in += size;
	}

	Buffer BWT::transform(ssh_u size) noexcept
	{
		// расчитать количество блоков
		int blk((int)(size / SSH_BWT_BLOCK_LENGHT));
		int count(blk + (size && (SSH_BWT_BLOCK_LENGHT - 1)));
		// выдел€ем пам€ть под блоки + исходные индексы
		Buffer result(size + count * 2 + 2); _index = result; _result = _index + count * 2 + 2;
		*(ssh_w*)_index = count; _index += 2;
		// трансформаци€ по блокам
		BufferW l(256 * SSH_BWT_BLOCK_LENGHT); LT = l;
		BufferW r(SSH_BWT_BLOCK_LENGHT + 1); RT = r;
		if(blk)
		{
			for(int i = 0; i < blk; i++)
				transform_block(SSH_BWT_BLOCK_LENGHT);
			//asm_ssh_bwt_transform(this, SSH_BWT_BLOCK_LENGHT);
		}
		if((blk = (size & (SSH_BWT_BLOCK_LENGHT - 1))))
			transform_block(blk);
		//asm_ssh_bwt_transform(this, blk);
		return result;
	}

	Buffer BWT::untransform(ssh_u size) noexcept
	{
		// извлекаем количество блоков
		int count(*(ssh_w*)in);
		// создаем выходной буфер
		size -= (count * 2 + 2);
		Buffer result(size); _index = in + 2; _result = result; in = _index + count * 2;
		// восстановление по блокам
		int blk((int)(size / SSH_BWT_BLOCK_LENGHT));
		BufferW vec(SSH_BWT_BLOCK_LENGHT);
		if(blk)
		{
			for(int i = 0; i < blk; i++)
				untransform_block(SSH_BWT_BLOCK_LENGHT, vec);
			//asm_ssh_bwt_untransform(this, SSH_BWT_BLOCK_LENGHT, vec);
		}
		if((blk = (size & (SSH_BWT_BLOCK_LENGHT - 1))))
			untransform_block(blk, vec);
		//asm_ssh_bwt_untransform(this, blk, vec);
		return result;
	}
}

/*
*/

