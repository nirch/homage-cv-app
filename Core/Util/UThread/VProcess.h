#ifndef _VPROCESS_H
#define _VPROCESS_H

#ifdef _WIN32
#ifndef _WINDOWS_
#include <windows.h>
#endif
#else
#error _WIN32 must be defined before you include criticalsection.h
#endif

class CVPorcess
{
public:


	CVPorcess(void);

	~CVPorcess(void);


	static int Execute( char *cmd, int wait );
};

#endif