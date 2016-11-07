
#include "stdafx.h"
#include "ssh_time.h"

namespace ssh
{
	static ssh_cws m_day_of_week_big[] = {L"Понедельник", L"Вторник", L"Среда", L"Четверг", L"Пятница", L"Суббота", L"Воскресенье"};
	static ssh_cws m_day_of_week_small[] = {L"Пн", L"Вт", L"Ср", L"Чт", L"Пт", L"Сб", L"Вс"};
	static ssh_cws m_month_big[] = {L"Январь", L"Февраль", L"Март", L"Апрель", L"Май", L"Июнь", L"Июль", L"Август", L"Сентябрь", L"Октябрь", L"Ноябрь", L"Декабрь"};
	static ssh_cws m_month_big_[] = {L"Января", L"Февраля", L"Марта", L"Апреля", L"Мая", L"Июня", L"Июля", L"Августа", L"Сентября", L"Октября", L"Ноября", L"Декабря"};
	static ssh_cws m_month_big_smail[] = {L"Пьянварь", L"Фигвраль", L"Кошмарт", L"Сопрель", L"Сымай", L"Теплюнь", L"Жарюль", L"Авгрусть", L"Свистябрь", L"Моктябрь", L"Гноябрь", L"Дубабрь"};
	static ssh_cws m_month_big_smail_[] = {L"Пьянваря", L"Фигвраля", L"Кошмарта", L"Сопреля", L"Сымая", L"Теплюня", L"Жарюля", L"Авгрустя", L"Свистября", L"Моктября", L"Гноября", L"Дубабря"};
	static ssh_cws m_month_small[] = {L"Янв", L"Фев", L"Мар", L"Апр", L"Май", L"Июн", L"Июл", L"Авг", L"Сен", L"Окт", L"Ноя", L"Дек"};

	String Time::fmt(ssh_cws str) const
	{
		static ssh_cws to[] = {L"$MN)+", L"$MN)", L"$MN+", L"$MN", L"$Mn", L"$m", L"$Y", L"$y", L"$dw", L"$dW", L"$dy", L"$d", L"$h", L"$H", L"$s", L"$nw", L"$nm", nullptr};
		int _month(month() - 1), _year(year()), _dw(dayOfWeek()), _hour(hour());

		String result(str);
		String tmp(String::fmt(L"%s\1%s\1%s\1%s\1%s\1%02i\1%02i\1%02i\1%s\1%s\1%02i\1%02i\1%02i\1%02i%s\1%02i\1%02i\1%02i\1\1",
							   m_month_big_smail_[_month], m_month_big_smail[_month], m_month_big_[_month], m_month_big[_month], m_month_small[_month],
							   _month + 1, _year, _year % 100, m_day_of_week_small[_dw], m_day_of_week_big[_dw], dayOfYear(), day(), _hour,
							   (_hour < 12 ? _hour : _hour - 12), (_hour < 12 ? L"pm" : L"am"), second(), weekOfYear(), minute()));
		tmp.replace(L'\1', L'\0');
		return result.replace(to, tmp);
	}

	int	Time::dayOfWeek() const
	{
		static int arr_week_days[] = {6, 0, 1, 2, 3, 4, 5};
		return arr_week_days[local()->tm_wday];
	}

	Time::Time(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nDST)
	{
		struct tm atm;

		atm.tm_sec = nSec;
		atm.tm_min = nMin;
		atm.tm_hour = nHour;
		atm.tm_mday = nDay;
		atm.tm_mon = nMonth - 1;
		atm.tm_year = nYear - 1900;
		atm.tm_isdst = nDST;
		time = _mktime64(&atm);
	}

	Time::Time(const SYSTEMTIME& sysTime, int nDST)
	{
		Time timeT(sysTime.wYear < 1900 ? 0L : ((int)sysTime.wYear, (int)sysTime.wMonth, (int)sysTime.wDay, (int)sysTime.wHour, (int)sysTime.wMinute, (int)sysTime.wSecond, nDST));
		*this = timeT;
	}

	Time::Time(const FILETIME& fileTime, int nDST)
	{
		FILETIME localTime;
		SYSTEMTIME sysTime;
		if(FileTimeToLocalFileTime(&fileTime, &localTime) && FileTimeToSystemTime(&localTime, &sysTime))
			*this = Time(sysTime, nDST);
	}

	struct tm* Time::local() const
	{
		static struct tm ptm;
		localtime_s(&ptm, &time);
		return &ptm;
	}

	struct tm* Time::gmt() const
	{
		static struct tm ptm;
		gmtime_s(&ptm, &time);
		return &ptm;
	}

	SYSTEMTIME Time::getAsSystemTime() const
	{
		struct tm* ptm(local());
		SYSTEMTIME timeDest;
		timeDest.wYear = (WORD)(1900 + ptm->tm_year);
		timeDest.wMonth = (WORD)(1 + ptm->tm_mon);
		timeDest.wDayOfWeek = (WORD)ptm->tm_wday;
		timeDest.wDay = (WORD)ptm->tm_mday;
		timeDest.wHour = (WORD)ptm->tm_hour;
		timeDest.wMinute = (WORD)ptm->tm_min;
		timeDest.wSecond = (WORD)ptm->tm_sec;
		timeDest.wMilliseconds = 0;
		return timeDest;
	}

	int Time::weekOfYear() const
	{
		// получаем 1 января текущего года
		Time bt(year(), 1, 1, 0, 0, 0);
		// вычисляем разницу между текущей датой и началом года
		time_t wd(bt.getTime() - (bt.dayOfWeek() * 24 * 60 * 60));
		// вычисляем номер недели
		return (int)(((time - wd) / (7 * 24 * 60 * 60)) + 1);
	}
}
