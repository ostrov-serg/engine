
#pragma once

namespace ssh
{
	class SSH Time
	{
		__time64_t time;
	public:
		Time() : time(0) {}
		Time(const time_t& t) { *this = t; }
		Time(const Time& t) { *this = t; }
		Time(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nDST = -1);
		Time(const SYSTEMTIME& sysTime, int nDST = -1);
		Time(const FILETIME& fileTime, int nDST = -1);
		// Операции
		bool operator == (const Time& t) const { return (time == t.time); }
		bool operator != (const Time& t) const { return (time != t.time); }
		bool operator < (const Time& t) const { return (time < t.time); }
		bool operator > (const Time& t) const { return (time > t.time); }
		bool operator <= (const Time& t) const { return (time <= t.time); }
		bool operator >= (const Time& t) const { return (time >= t.time); }
		const Time& operator = (const Time& t) { time = t.time; return *this; }
		const Time& operator = (const time_t& t) { time = t; return *this; }
		// Атрибуты
		struct tm* local() const;
		struct tm* gmt() const;
		SYSTEMTIME getAsSystemTime() const;
		time_t	getTime() const { return time; }
		int	year() const { return local()->tm_year + 1900; }
		int	month() const { return local()->tm_mon + 1; }
		int	day() const { return local()->tm_mday; }
		int	hour() const { return local()->tm_hour; }
		int	minute() const { return local()->tm_min; }
		int	second() const { return local()->tm_sec; }
		int	dayOfWeek() const;
		int	dayOfYear() const { return local()->tm_yday + 1; }
		int	weekOfYear() const;
		String fmt(ssh_cws str) const;
		static Time current() { return Time(::time(nullptr)); }
	};
}
