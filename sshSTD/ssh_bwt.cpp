
#include "stdafx.h"
#include "ssh_compress.h"

extern "C"
{
	void asm_ssh_bwt_transform(void* _this, int size);
	void asm_ssh_bwt_untransform(void* _this, int size, void* vec);
}

namespace ssh
{
	static int idx;
	static int level;
	void BWT::sort() noexcept
	{
		// ���� �� ��������
		for(int i = 0; i < 256; i++)
		{
			// ���� ������������� i-� �����
			idx = LT[i];
			LT[i] = 0;
			// ��������� ����� ��� ���� �����
			if(idx)
			{
				// i-� ������ ������������ ������ �������, ������ ��������������� ����� ������� ����������� ����� ���������
				if(RT[idx] == 0) set_val(idx);
				else
				{
					// � ������ ������������� ������������� i-�� �������:
					if(level >= (keys - 1))
					{
						// ����� ���� ������ �� ���� ������:
						do
						{
							// ��������� ������� ������ � ������� ��������
							set_val(idx);
							idx = RT[idx];
						} while(idx);
					}
					else
					{
						// ���������� �������� ������� ����: ���������� �� ������� ����
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
						// ���������� ������� ���������
						level++;
						sort();
						level--;
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
		level = 0;
		idx_lit = 0;
		keys = size;
		// ������������� �������� ����
		ssh_memzero(LT, 512);
		// ���������� ����� �� ������ �����
		ssh_b is(0);
		for(int idx = 1; idx < (keys + 1); idx++)
		{
			ssh_b c(in[idx - 1]);
			// ��������� �� ���������� ��������
			is |= (c != *in);
			_result[idx - 1] = c;
			RT[idx] = LT[c];
			LT[c] = idx;
		}
		// ��������� ������� ��������� ��������� ������� � ������
		if(is) sort();
		in += size;
		_result += size;
	}

	void BWT::untransform_block(int size, ssh_w* vec) noexcept
	{
		int sum(0), i;
		int count[256];
		// ��������� �������� ������
		ssh_d idx(*(ssh_w*)_index); _index += 2;
		// �������������� ������
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
		// ��������� ������� �������������� � ������������ � ��������
		for(i = 0; i < size; i++)
		{
			idx = vec[idx];
			*_result++ = in[idx];
		}
		in += size;
	}

	Buffer BWT::transform(ssh_u size) noexcept
	{
		// ��������� ���������� ������
		int blk((int)(size / SSH_BWT_BLOCK_LENGHT));
		int count(blk + (size && (SSH_BWT_BLOCK_LENGHT - 1)));
		// �������� ������ ��� ����� + �������� �������
		Buffer result(size + count * 2 + 2); _index = result; _result = _index + count * 2 + 2;
		*(ssh_w*)_index = count; _index += 2;
		// ������������� �� ������
		BufferW l(256 * SSH_BWT_BLOCK_LENGHT); LT = l;
		BufferW r(SSH_BWT_BLOCK_LENGHT + 1); RT = r;
		if(blk)
		{
			for(int i = 0; i < blk; i++)
				transform_block(SSH_BWT_BLOCK_LENGHT);
		}
		if((blk = (size & (SSH_BWT_BLOCK_LENGHT - 1))))
			transform_block(blk);
		return result;
	}

	Buffer BWT::untransform(ssh_u size) noexcept
	{
		// ��������� ���������� ������
		int count(*(ssh_w*)in);
		// ������� �������� �����
		size -= (count * 2 + 2);
		Buffer result(size); _index = in + 2; _result = result; in = _index + count * 2;
		// �������������� �� ������
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
