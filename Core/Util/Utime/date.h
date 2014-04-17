#if !defined __DATE_H__
#define __DATE_H__
#include <Winbase.h>


unsigned short CodeDate(PSYSTEMTIME p)
{
	short m[] = {31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
	short year = (short)(p->wYear - 2001);
	if (year < 0)
		return 0;
	unsigned short n = (unsigned short)(year * 365.25f);
	if( p->wMonth > 2 && (p->wYear & 3) == 0 )	//	leap-year 
		for (int i = 1; i < 11; m[i++]++);
	n += (p->wMonth > 1)? m[p->wMonth - 2]: 0;
	n += p->wDay;
	return n;
}

PSYSTEMTIME DecodeDate(unsigned short n)
{
	static SYSTEMTIME s;
	short i;
	short m[] = {31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
	memset(&s, 0, sizeof(SYSTEMTIME));
	if (n)
	{
		s.wDayOfWeek = n % 7;
		short year = (short)(n / 365.25f);
		s.wYear = year + 2001;
		n -= (short)(year * 365.25f);
		if( (s.wYear & 3) == 0)	//	leap-year 
			for (i = 1; i < 11; m[i++]++);
		for (i = 0; i < 11; i++)
			if (n < m[i]) break;
		s.wMonth = i + 1;
		s.wDay = (i)? n - m[i - 1] : n;
	}
	else
	{
		s.wYear = 2001;
		s.wMonth = 1;
		s.wDay = 1;
	}
	return &s;
}

unsigned short CodeCurrentDate()
{
	SYSTEMTIME s;
	::GetSystemTime(&s);
	return CodeDate(&s);
}
#endif