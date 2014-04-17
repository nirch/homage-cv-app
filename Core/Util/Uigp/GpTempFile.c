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




#include	"igp.h"

static char	Tmp_dir[512] = "";

static void VimGetUniqueId(long * inId);


int
gp_unique_id()
{
int	id;

	VimGetUniqueId(&id);

	return( id );
}

int
gp_filename_set_temporary_dirName( char *base, char *name )
{
char	dir[512],	baseDir[512];

	if( base == NULL || base[0] == 0 ){
#if defined (WIN32) 
#ifndef WINCE
		GetTempPath(512, baseDir);
#endif
#else
		strcpy( baseDir, "/tmp" );
#endif
	}
	else	{
		strcpy( baseDir, base );
		if( gpDir_exist( baseDir ) != 1 )
			if( gp_mkdir( baseDir ) == 0 )	return( -1 );
	}



	if( name != NULL )
		sprintf( dir, "%s/%s", baseDir, name );
	else	strncpy( dir, baseDir, 512 );

	return( gp_filename_set_temporary_dir( dir ) );
}


int
gp_filename_set_temporary_dir( char *dir )
{
	strncpy( Tmp_dir, dir, 512 );

	if( gpDir_exist( Tmp_dir ) == 1 )	return( 1 );
	gp_mkdir( Tmp_dir );

	return( 1 );
}



void
gp_filename_get_temporary( char *prefix, int unique, char *extension, char *filename )
{



#if defined (WIN32) 
#ifndef WINCE
	char	dir[2048],	name[2048],	extension1[2048];
	long id;

	if( Tmp_dir[0] == '\0' )
		gp_filename_set_temporary_dirName( NULL, NULL );

//	GetTempPath(2048, dir);

//	GetTempFileName(dir, prefix, unique, filename);
	GetTempFileName( Tmp_dir, prefix, unique, filename );


	gp_filename_split( filename, dir, name, extension1 );
	gpFile_delete(filename);
/*
	sprintf( filename, "%s\\%s%s", dir, name, extension );
*/

	VimGetUniqueId(&id);

	if( unique == 0 )	unique = id;
	sprintf( name, "%s%d",  prefix, unique );

	sprintf( filename, "%s/%s%s", dir, name, extension );
#endif
#else // UNIX
	char	name[2048];
	long id;

	if( Tmp_dir[0] == '\0' )
		gp_filename_set_temporary_dirName( NULL, NULL );

	VimGetUniqueId(&id);
	//if( unique == 0 )	unique = (int)time(NULL );
	if( unique == 0 )	unique = id;
	sprintf( name, "%s%d",  prefix, unique );


//	sprintf( filename, "/tmp/%s%s", name, extension );
	sprintf( filename, "%s/%s%s", Tmp_dir, name, extension );

#endif
}


void
gp_filename_get_temporaryD( char *prefix, int unique, char *dir, char *name )
{


#if defined (WIN32) // eyal 29.9
#ifndef WINCE
	char	filename[256];
	char	extension[256];

	if( Tmp_dir[0] == '\0' )
		gp_filename_set_temporary_dirName( NULL, NULL );

//	GetTempPath(255, dir);
	GetTempFileName(Tmp_dir, prefix, unique, filename);
	unlink(filename);



	gp_filename_split( filename, dir, name, extension );
#endif
#else // UNIX
	long id;
//	strcpy(dir, "/tmp");

	if( Tmp_dir[0] == '\0' )
		gp_filename_set_temporary_dirName( NULL, NULL );

	strcpy(dir, Tmp_dir);

	VimGetUniqueId(&id);
	//if( unique == 0 )	unique = (int)time(NULL );
	if( unique == 0 )	unique = id;
	sprintf( name, "%s%d",  prefix, unique );
#endif
}





///////////////////////////////////////////////////////////////////
/*!
********
** @brief            Utility for directory actions 
**						- get the currect directory
**						- remove directory
**						- change current directory
** @todo             Create log on errors.
** @author           oren zamir
** @date             8/8/2002 11:56:24 AM
********/
#ifdef NEED_2_CONVERT_2_C
#ifdef __GNUC__  // For gcc under Linux/Unix
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
  static std::string getCurrentDir() 
  {
    char path[PATH_MAX];
    getcwd(path, PATH_MAX);
    return std::string(path);
  }
  static void makeDir(std::string dir) 
  {
    mkdir(dir.c_str(), 0777);
  }
  static void changeDir(std::string dir) 
  {
    chdir(dir.c_str());
  }
#else // For Dos/Windows:
#ifdef WIN32
#include <direct.h>

	static std::string getCurrentDir() 
	{
    char path[_MAX_PATH];
    getcwd(path, _MAX_PATH);
    return std::string(path);
  }
  static void makeDir(std::string dir) 
  {
    mkdir(dir.c_str());
  }
  static void changeDir(std::string dir) 
  {
    chdir(dir.c_str());
  }
#else
	#error no compilation defination for directory utility file!
#endif
#endif ///// End of OS-specific code
#endif


void VimGetUniqueId(long * inId);


/*
 *	function	:	VimGetUniqueId
 *	in			:	void
 *	out			:	a unique id 
 *	notes		:	This is a thread safe function.
 */
#ifdef WIN32
#include "windows.h"
#else
#include <pthread.h>
#endif

void VimGetUniqueId(long * inId)
{
	static  long Id=0;
#ifdef WIN32
	// Global variable
	static CRITICAL_SECTION CriticalSection; 
    // Initialize the critical section one time only.
    InitializeCriticalSection(&CriticalSection); 
#else // solaris
	static pthread_mutex_t Global_mutex;
	pthread_mutex_lock(&Global_mutex);
#endif
	/*
	 *	the actual unique id is processed here.
	 */
	*inId = Id++;
#ifdef WIN32
    // Release resources used by the critical section object.
    DeleteCriticalSection(&CriticalSection);
#else // solaris
	pthread_mutex_lock(&Global_mutex);
#endif
}
