
#include "stdafx.h"
#include <regex>


class ttt : public Serialize
{
public:
	ttt() : a(999), b(111), c(222) {}
	virtual SCHEME* get_scheme() const override
	{
		SCHEME_BEGIN(ttt)
			SCHEME_VAR(ttt, a, L"1", SC_BIN, nullptr)
			SCHEME_VAR(ttt, b, L"2", SC_OCT, nullptr)
			SCHEME_VAR(ttt, c, L"3", SC_HEX, nullptr)
			SCHEME_END(ttt);
	}
	ssh_w a, b, c;
};
class tp : public Serialize
{
public:
	SSH_ENUM(_enum, _one = 1, _two = 2);
	tp() : x(_one), y(_two) { }
	virtual ~tp() { }
	struct stk2
	{
		//stk2() { str[0] = L"Сергей"; str[1] = L"Влад"; str[2] = L"Макс"; _cws = L'!'; }
		String str[3];
	};
	struct stk
	{
		stk() { xx = 10.0; yy[0] = 1.0; yy[1] = 2.0; yy[2] = 3.0; yy[3] = 4.0; }
		double xx, yy[4];
		stk2 _stk2[3]{{L"1",L"2",L"3"},{L"11",L"22",L"33"},{L"111",L"222",L"333"}};
	};
	virtual SCHEME* get_scheme() const override
	{
		auto& reflector = ssh::EnumReflector::get<_enum>();
		SCHEME_BEGIN(tp)
			SCHEME_VAR(tp, _cws, L"null", 0, nullptr)
			SCHEME_VAR(tp, x, L"null", 0, &reflector)
			SCHEME_VAR(tp, _ccs, L"null", 0, nullptr)
			SCHEME_NODE_BEGIN(tp, _stk)
			SCHEME_VAR(stk, xx, L"null", 0, nullptr)
			SCHEME_NODE_BEGIN(stk, _stk2)
			SCHEME_VAR(stk2, str, L"null", SC_BASE64, nullptr)
			SCHEME_NODE_END()
			SCHEME_VAR(stk, yy, L"null", 0, nullptr)
			SCHEME_NODE_END()
			SCHEME_VAR(tp, y, L"null", 0, &reflector)
			SCHEME_VAR(tp, is, L"false", 0, nullptr)
			SCHEME_NODE_BEGIN(tp, _t)
			SCHEME_NODE_END()
			SCHEME_END(tp);
	}
	int x, y;
	const bool is = true;
	ssh_ws _cws[10] = L"Влад";
	ssh_cs _ccs = '?';
	stk _stk[2];
	ttt _t[3];
};

class bs1 : public Base
{
	SSH_DYNCREATE(bs1);
public:
	bs1() { x = 0; }
	bs1(const bs1& b)
	{
		x = b.x;
	}
	bs1(int _x, const String& _s) : x(_x), str(_s) { }
	void set_str(String s)
	{
		str = std::move(s);
		str1 = s;
		ssh_cws _1 = __FUNCTIONW__;
		ssh_ccs _2 = __FUNCDNAME__;
		ssh_ccs _3 = __FUNCSIG__;
		ssh_ccs _4 = __func__;
	}

	String str, str1;
	int x;

};

ssh_u operator "" _suffix(ssh_u minutes)
{
	return minutes * 60;
}

String operator ""_r(ssh_ccs _ccs)
{
	return String(_ccs);
}

using namespace std;

void _func()
{
	SSH_TRACE;
	Xml xml;
	tp t;
	t.save(L"c:\\11.xml", true);
	File f(L"c:\\1.xml", File::open_read);
	t.open(f.read(0), true);
}

extern "C"
{
	ssh_ws* ssh_asm_wcsstr1(ssh_ws* _s1, ssh_cws _s2);
}


#include <list>
#include <map>
#include <forward_list>


extern "C"
{
	ssh_u asm_strstr(void* ptr, ssh_u sz, ssh_u len);
	void asm_bwt(ssh_u len, void* dst, ssh_ccs src);
}
void lz77(const Buffer& buf)
{
	ssh_b* b(buf);
	ssh_u sz(buf.size());
	for(ssh_u i = 0; i < sz; i += 8)
	{
		sz -= asm_strstr(buf, sz, i);
	}
}

ssh_ccs _str[9];
long RT[20];
long LT[11][256];
String result[256];
int _result[256];

void PutCurrRecord(int recno)
{
	static int i = 0;
	_result[i++] = recno;// _str[recno - 1];
}

// Функция обработки данных после 1-го этапа: Перегруппировываем слова, переходя от одной буквы к следующей
void process(int level, int keys)
{
	// Цикл по алфавиту
	for(int i = 0; i < 256; i++)
	{
		// Ищем использование i-й буквы
		auto recno(LT[level][i]);
		LT[level][i] = 0;
		// Сканируем ветвь для этой буквы
		while(recno)
		{
			// i-й символ используется только однажды, значит отсортированная часть массива пополнилась новым элементом
			if(!RT[recno])
			{
				PutCurrRecord(recno);
				break;
			}
			else
			{
				// В случае многократного использования i-го символа:
				if(level == keys)
				{
					// Вывод всех данных на этом уровне:
					while(recno)
					{
						// Добавляем текущую запись в таблицу индексов
						PutCurrRecord(recno);
						recno = RT[recno];
					}
				}
				else
				{
					// Продолжать уточнять порядок слов: опускаемся на уровень вниз
					int newlevel(level + 1);
					while(recno)
					{
						auto nextrec(RT[recno]);
						auto c(_str[recno - 1][newlevel]);
						RT[recno] = LT[newlevel][c];
						LT[newlevel][c] = recno;
						recno = nextrec;
					}
					// Продолжаем процесс уточнения
					process(newlevel, keys);
				}
			}
		}
	}
}

// Количество используемых ключевых полей
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

	int N = 9;
	// Инициализация таблицы символов
	memset(LT, 0, 256 * keys);
	// Этап 1 - Группируем слова по первой букве
	for(int recno = 1; recno != N + 1; recno++)
	{
		auto c(_str[recno - 1][0]);
		RT[recno] = LT[0][c];
		LT[0][c] = recno;
	}
	// Запускаем процесс уточнения положения записей в списке.
	process(0, keys);
}


//ssh_ccs _ccs = "RNDSRRFFN,RSRENLYYL,RSENN,,R,,,,,EYYFETT,,HENYSYTSDSESYDD,UYFS,TRDEGADEEFSRRTLTSS,FRHFYFNFED,FTYS,NGGDOONDSSSSLELNTDETNSYRESCCE TII MMLIIITHDDM  HRR       RRLWW,WWPPHUMTTHCS. AAIAMNN NUREN NNII RIIEEEIEENEERENNN  IIOUUN AHSRBHCSHHLSUSSPRRNSSIIDNBE EVDUVHSVHVMPTHGGINRSRHORWO OOOOOOOOOOO O  ROONNNAA  NCTSC.WTTTTTTTWPPTTTTTT LLTRCSLLHHTVCDLTSSBBBLL A .    ,KK SDDVTTTSHDD- HHWVRFMMWWWWFLLL  AALAACUB  IIP  IIIA   ECCAAR EFIIIIEOIOOIOOOOOIIAIIIAAIIINOTIIIIO ROEAEEAAEAAAAAAATTBNS     Y    R SS CC IIIIYCCF F      F   EPLLLPPNRYYHH HRH  SRAAMR  X   EOOOOEOOEAOOERRGGEOIAIIPA EEAOOPOPOUAAAAEOIASESMSGIIIESSCCEA,LUIUOUINS S  IST   EOOOEOOE UUUCNOUICNNIIAI E   EO   IIINRRCAAI  AFFNNIIIOOSLLNQCP  AOOOOEEEEOADTT  O  R    ENNNNTRTTNNTTN  ";

int main() noexcept
{
	ssh_log->init(Log::TypeOutput::debug);

	ABCsort(10);
	ssh_b _str[] = "carmel adela  beatrixabbey  abigalebarbaracamaliabelindabeckie \0";

	ssh_b src[] = "абракадабра\0";
	BWT _bwt;
	size_t n = strlen((ssh_ccs)_str);
	File f(L"c:\\1", File::open_read);

	Buffer _out(_bwt.process(f.read(), true));
	MTF _mtf;
	Buffer _mout(_mtf.process(_out, true));
	File f1(L"c:\\1+", File::open_write);
	f1.write(_mout);
	Buffer _out1(_bwt.process(_out, false));
	File f2(L"c:\\1++", File::open_write);
	f2.write(_out1);
	int x = 0;
	/*
	ssh_b WT[11];			// word tracker
	ssh_b LT[11][256];		// little tracker
	SSH_MSG(L"\t1\t\t2\t\t3\t\t4\t\t5\t\t6\t\t7\t\t8\t\t9\t\r\n");
	for(int i = 0; i < 9; i++)
		SSH_MSG(String(_str[i]) + L"\t");
	SSH_MSG(L"\r\n");

	memset(LT, 0, 256 * 11);
	for(int i = 0; i < 7; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			//auto l(src[(i + j) % 11]);
			auto l(_str[j][i]);
			WT[j] = LT[i][l];
			SSH_MSG(L"\t" + String(WT[j]) + L"\t");
			LT[i][l] = j + 1;
		}
		SSH_MSG(L"\r\n");
	}

	SSH_MSG(L"\r\na\tb\tc\td\te\tf\tg\th\ti\tj\tk\tl\tm\tn\to\tp\tq\tr\ts\tt\tu\tv\tw\tx\ty\tz\r\n\r\n");
	for(int i = 0; i < 11; i++)
	{
		String str;
		for(int j = 'a'; j <= 'z'; j++)
		{
			str += String(LT[i][j]) + L"\t";
		}
		str += L"\r\n";
		SSH_MSG(str);
	}

	ssh_unit_test();

	//	asm_bwt(11, arr, src);

	Arith a1;
	File r(L"c:\\1", File::open_read);
	Buffer out(a1.process(r.read(0), true));
	File w(L"c:\\1+", File::create_write);
	w.write(out);
	Arith a2;
	File r1(L"c:\\1+", File::open_read);
	Buffer out1(a2.process(r1.read(0), false));
	File w1(L"c:\\1++", File::create_write);
	w1.write(out1);
	return 0;
	*/
	/*
	std::list<int> lst;
	lst.push_back(1);
	lst.push_back(2);
	std::vector<int> ptr{1,2,3,4};
	Arith a1;
	ssh_u v = 10;
	String str(v);
	printf("%i", 1);
	std::regex re;

	ssh_b dest[117];
	ssh_memset(dest, 0x8070605040302010, 11);
	File f(L"c:\\h", File::open_read);
	Buffer buf(f.read(0));
	f.close();
	lz77(buf);
	SSH_TRACE;
	_func();
	return 0;
	*/
}
