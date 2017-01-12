
#include "stdafx.h"
#include "ssh_time.h"

namespace ssh
{
	static ssh_cws ����������[] = {L"�����������", L"�������", L"�����", L"�������", L"�������", L"�������", L"�����������"};
	static ssh_cws ����������[] = {L"��", L"��", L"��", L"��", L"��", L"��", L"��"};
	static ssh_cws �������[] = {L"������", L"�������", L"�����", L"������", L"���", L"����", L"����", L"�������", L"��������", L"�������", L"������", L"�������"};
	static ssh_cws �������[] = {L"���", L"���", L"���", L"���", L"���", L"���", L"���", L"���", L"���", L"���", L"���", L"���"};
	static ssh_cws �������������[] = {L"��������", L"��������", L"��������", L"�������", L"�����", L"�������", L"������", L"��������", L"���������", L"��������", L"�������", L"�������"};

	String Time::fmt(String str) const
	{
		static ssh_cws to[] = {L"%����_", L"%����", L"%�����", L"%����_", L"%����", L"%�_)", L"%�_", L"%�_", L"%��", L"%�", L"%�", L"%�", L"%��_", L"%��_", L"%��", L"%�", L"%�", L"%�_", L"%�", L"%�", L"%�", nullptr};
		int _month(month() - 1), _year(year()), _dw(dayOfWeek()), _hour(hour()), _day(day()), _min(minute()), _sec(second());

		String tmp(ssh_printf(L"%02i %s %02i;%02i.%02i.%02i;%02i:%02i:%02i;%02i %s %02i;%02i.%02i.%02i;%s;%s;%s;%02i;%02i;%02i;%02i;%s;%s;%02i;%02i;%02i;%s;%02i;%02i;%02i;;",
							   _day, �������[_month], _year,
							   _day, _month + 1, _year,
							   _hour, _min, _sec,
							   _day, �������[_month], _year,
							   _day, _month + 1, _year % 100,

								�������������[_month],
								�������[_month],
								�������[_month],
								_min,
								_month + 1,
								_year,
								_year % 100,
								����������[_dw],
								����������[_dw],
								dayOfYear(),
								_day,
								_hour,
								(_hour < 12 ? L"pm" : L"am"),
								(_hour < 12 ? _hour : _hour - 12),
								_sec,
								weekOfYear()));
		tmp.replace(L';', L'\0');
		return str.replace(to, tmp);
	}

	int	Time::dayOfWeek() const
	{
		static int arr_week_days[] = {6, 0, 1, 2, 3, 4, 5};
		return arr_week_days[local()->tm_wday];
	}

	Time::Time(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nDST)
	{
		struct tm atm { nSec, nMin, nHour, nDay, nMonth - 1, nYear - 1900, 0, 0, nDST };
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
		// �������� 1 ������ �������� ����
		Time bt(year(), 1, 1, 0, 0, 0);
		// ��������� ������� ����� ������� ����� � ������� ����
		time_t wd(bt.getTime() - (bt.dayOfWeek() * 24 * 60 * 60));
		// ��������� ����� ������
		return (int)((time - wd) / (7 * 24 * 60 * 60));
	}
}
