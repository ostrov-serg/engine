
#include "stdafx.h"
#include "ssh_compress.h"

namespace ssh
{
	void BWT::sort(int level) noexcept
	{
		// ÷икл по алфавиту
		for(int i = 0; i < 256; i++)
		{
			// »щем использование i-й буквы
			auto idx(LT[level * 256 + i]);
			LT[level * 256 + i] = 0;
			// —канируем ветвь дл€ этой буквы
			while(idx)
			{
				// i-й символ используетс€ только однажды, значит отсортированна€ часть массива пополнилась новым элементом
				if(!RT[idx])
				{
					set_val(idx);
					break;
				}
				else
				{
					// ¬ случае многократного использовани€ i-го символа:
					if(level == keys)
					{
						// ¬ывод всех данных на этом уровне:
						while(idx)
						{
							// ƒобавл€ем текущую запись в таблицу индексов
							set_val(idx);
							idx = RT[idx];
						}
					}
					else
					{
						// ѕродолжать уточн€ть пор€док слов: опускаемс€ на уровень вниз
						int newlevel(level + 1);
						while(idx)
						{
							auto nextrec(RT[idx]);
							auto c(get_val(idx, level));
							RT[idx] = LT[newlevel * 256 + c];
							LT[newlevel * 256 + c] = idx;
							idx = nextrec;
						}
						// ѕродолжаем процесс уточнени€
						sort(newlevel);
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
		*_result++ = get_val(idx, keys - 1);
		idx_lit++;
	}

	ssh_b BWT::get_val(int level, int idx) noexcept
	{
		return in[((level - 1) + idx) % keys];
	}

	void BWT::transform_block(int size) noexcept
	{
		idx_lit = 0;
		keys = size;
		// »нициализаци€ индексов букв
		ssh_memzero(LT, 512 * keys);
		// »нициализаци€ индексов слов
		//ssh_memzero(RT, keys);
		// √руппируем слова по первой букве
		for(int idx = 1; idx != keys + 1; idx++)
		{
			auto c(get_val(idx, 0));
			RT[idx] = LT[c];
			LT[c] = idx;
		}
		// «апускаем процесс уточнени€ положени€ записей в списке
		sort(0);
		in += size;
	}

	Buffer BWT::transform(ssh_u size) noexcept
	{
		// расчитать количество блоков
		int blk((int)(size / BWT_BLOCK_LENGHT));
		int count(blk + (size && (BWT_BLOCK_LENGHT - 1)));
		// выдел€ем пам€ть под блоки + исходные индексы
		Buffer result(size + count * 2 + 2); _index = result; _result = _index + count * 2 + 2;
		*(ssh_w*)_index = count; _index += 2;
		// трансформаци€ по блокам
		if(blk)
		{
			BufferW l(256 * BWT_BLOCK_LENGHT); LT = l;
			BufferW r(BWT_BLOCK_LENGHT + 1); RT = r;
			for(int i = 0; i < blk; i++)
				transform_block(BWT_BLOCK_LENGHT);
		}
		if((blk = (size & (BWT_BLOCK_LENGHT - 1))))
		{
			BufferW l(256 * blk); LT = l;
			BufferW r(blk + 1); RT = r;
			transform_block(blk);
		}
		return result;
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
			vec[count[in[i]]++] = i;
		// запускаем процесс восстановление в соответствии с вектором
		for(i = 0; i < size; i++)
		{
			idx = vec[idx];
			*_result++ = in[idx];
		}
		in += size;
	}
	Buffer BWT::untransform(ssh_u size) noexcept
	{
		// извлекаем количество блоков
		int count(*(ssh_w*)in);
		// создаем выходной буфер
		size -= (count * 2 + 2);
		Buffer result(size); _index = in + 2; _result = result; in = _index + count * 2;
		// восстановление по блокам
		int blk((int)(size / BWT_BLOCK_LENGHT));
		if(blk)
		{
			BufferW vec(BWT_BLOCK_LENGHT);
			for(int i = 0; i < blk; i++)
				untransform_block(BWT_BLOCK_LENGHT, vec);
		}
		if((blk = (size & (BWT_BLOCK_LENGHT - 1))))
		{
			BufferW vec(blk);
			untransform_block(blk, vec);
		}
		return result;
	}
}
