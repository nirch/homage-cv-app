#ifndef VIM_CRITICAL_SECTION_H
#define VIM_CRITICAL_SECTION_H



#ifdef _WIN32
#ifndef _WINDOWS_
#include <windows.h>
#endif
#else
#error _WIN32 must be defined before you include criticalsection.h
#endif







class CVCriticalSection
{
	typedef CRITICAL_SECTION cs;
	cs m_cs;
public:
	CVCriticalSection(void);

	~CVCriticalSection(void);

	void enter();
	
	void leave();
};

#endif