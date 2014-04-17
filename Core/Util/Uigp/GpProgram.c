#include	<stdio.h>

#ifdef WINCE
#include <windows.h>
#else
#include	<sys/types.h>
#include	<sys/stat.h>
#ifdef  WIN32
#include <windows.h>
#include	<direct.h>
#else
#include	<dirent.h>
#endif
#endif

#include	<string.h>
#include	<stdlib.h>

#include <wchar.h>


#include	"igp.h"

#ifdef  WIN32
#include <windows.h>

IMAGE_DOS_HEADER __ImageBase;
#endif

	


int	
gpProgram_path( char *path )
{
#ifdef  WIN32

#ifdef _UNICODE
	wchar_t wpath[512];
	::GetModuleFileName((HINSTANCE)&__ImageBase, wpath, 512);
	int	n = wcslen(wpath);
	int a = wcstombs( path, (const wchar_t * )wpath,  wcslen(wpath) ); 
	path[n] = 0;
#else
	GetModuleFileName((HINSTANCE)&__ImageBase, path, 512);
#endif




	return( 1 );
#else
	return( -1 );
#endif
}

int	
gpProgram_dir( char *dir )
{
char	path[512];

	if( gpProgram_path( path ) < 0 ) 
		return( -1 );

	gpFilename_dir( path, dir );

	return( 1 );
}