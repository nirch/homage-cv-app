/*******************
 ***	GpIo.c	***
 *******************/


#include <stdio.h>



#ifdef	WIN32
#include  <windows.h>
#endif

#include	"igp.h"


void
gp_relinquish_cpu()
{

#ifdef WIN32
	Sleep(0);
#endif

}

void
gp_sleep( int milisec )
{

#ifdef WIN32
	Sleep(milisec );
#endif

}