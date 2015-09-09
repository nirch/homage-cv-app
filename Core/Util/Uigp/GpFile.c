#include	<stdio.h>

#ifdef WINCE
#include <windows.h>
#else
#include	<sys/types.h>
#include	<sys/stat.h>
#ifdef  WIN32
#include	<direct.h>
#include <windows.h>
#else
#include	<dirent.h>
#endif
#endif

#include	<string.h>



//#include "Utime/GpTime.h"


#include	"igp.h"


int
gpFile_exist( char *file )
{
#ifdef WINCE
	return gpDir_exist(file);
#else
	struct stat     statbuf;

	if( stat( file, &statbuf ) == 0 )       return( 1 );
	return( -1 );
#endif
}



int
gpFile_size( char *file )
{
#ifdef WINCE
	//HANDLE h;
	//DWORD r;
	//int ir;
	//TCHAR * buf = (TCHAR *)malloc((strlen(file) + 1) * sizeof(TCHAR));
	//wsprintf(buf, _T("%S"), file);
	//h = CreateFile(buf, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	//free(buf);
	//if (h == INVALID_HANDLE_VALUE)
	//	return -1;
	//r = GetFileSize(h, NULL);
	//ir = (r == 0xFFFFFFFF)? -1: (int)r;
	//CloseHandle(h);
	//return ir;

	return(-1);
#else
struct stat     statbuf;

        if( stat( file, &statbuf ) != 0 )       return( -1 );

	return( (int)statbuf.st_size );
#endif
}


vTime_type
gpFile_mtime( char *file )
{
#ifdef WINCE
	return 0;
#else
struct stat     statbuf;

    if( stat( file, &statbuf ) != 0 )       return( 0 );

	return( (long)statbuf.st_mtime );
#endif
}


int
gpDir_exist( char *dir )
{

#ifdef WINCE
	//HANDLE h;
	//TCHAR * buf = (TCHAR*)malloc((strlen(dir) + 1) * sizeof(TCHAR));
	//wsprintf(buf, _T("%S"), dir);
	//h = CreateFile(buf, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	//free(buf);
	//if (h == INVALID_HANDLE_VALUE)
	//	return 0;
	//else
	//	CloseHandle(h);
	//return 1;
	return( -1 );
#else
struct stat     statbuf;
int	len;
char	*p;

	len = strlen( dir );
	if( len <= 0 )	return( 0 );

	if( len == 1 && dir[0] == '.' )	return( 1);
	
	for( p = &dir[len-1] ; p != dir ; p-- ){
		if( *p != '/' && *p != '\\' )	break;
		*p = 0;
	}

	if( p == dir && len > 1)	return( -1 );

	if( stat( dir, &statbuf ) != 0 )       return( -1 );

	if(( statbuf.st_mode & S_IFMT ) != S_IFDIR )
		return( -1 );

       return( 1 );
#endif
}

int
gp_getcwd( char *dir, int size )
{
#ifndef WINCE
#ifdef	WIN32
	_getcwd( dir, size );
#else
	getcwd( dir, size );
#endif//WIN32
#endif//CE
	return( 1 );
}


int gp_setcwd(char *dir)
{
#ifndef WINCE
#ifdef WIN32
	_chdir(dir);
#else
	chdir(dir);
#endif// WIN32
#endif// CE
	return 1;
}


int
gpDir_force_exist( char *dir )
{
#ifdef __ANDROID__
	return( 1 );
#endif

	if( gpDir_exist( dir ) == 1 )
		return( 1 );

	if( gp_mkdir( dir ) == 1 )
		return( 1 );

	return( -1 );
}

int
gp_mkdir( char *dir )
{
#ifndef WINCE
#ifdef	WIN32
	if( mkdir( dir ) == -1 ) return( 0 );
#else
	if( mkdir( dir, 0755 ) == -1 ) return( 0 );
#endif
#endif
	return( 1 );
}



int
gp_rmdir( char *dir )
{

#ifndef WINCE
#ifdef	WIN32
	if( _rmdir( dir ) == -1 )	return( -1 );
	return( 1 );
#else
struct dirent	*dp;
DIR	*dfd;
char	name[256];

        if ( (dfd = opendir(dir)) == NULL)
                return( -1 );

        while ( (dp = readdir(dfd)) != NULL ) {

		if( strcmp( dp->d_name, "." ) == 0 )	continue;
		if( strcmp( dp->d_name, ".." ) == 0 )	continue;

		strcpy( name, dir );
		strcat( name, "/" );
		strcat( name, dp->d_name );
		unlink( name );
	}

	closedir( dfd );


        if ( (dfd = opendir(dir)) == NULL)
                return( -1 );
	closedir( dfd );



	if( rmdir( dir ) == -1 )
		perror( "rmdir" );

	return( 1 );
#endif
#else
	return 1;
#endif
}



int
gpFile_delete( char *file )
{
#ifndef WINCE
	int	ret;

	ret = unlink( file );

	if( ret != 0 )	return(-1);

	return( 1 );
#else
	return( -1 );
#endif
}


int
gpFile_rename( char *oldName, char *newName )
{
#ifndef WINCE
int	ret;

	ret = rename( oldName, newName );

	if( ret != 0 )	return(-1);

	return( 1 );
#else
return( -1 );
#endif
}


int
gpFile_moveto( char *inFile, char *dir )
{
	char	fname[256],	file[256];
	int		ret;

	gpFilename_fullname( inFile, fname );

	sprintf( file, "%s\\%s", dir, fname );

	ret = gpFile_rename( inFile, file );

	return( ret );
}

int gpDir_unuse_id( char *base, int i0, char *dirName )
{
	char	dir[256];
	int	i;

	for( i = i0 ; ; i++ ){
		sprintf( dir, base, i );

		if( gpDir_exist( dir ) < 0 ){
			if( dirName != NULL )
				strcpy( dirName, dir );
			return( i );
		}
	}

	return( -1 );
}

int gpFile_unuse_id( char *base, int i0, char *fileName )
{
	char	file[256];
	int	i;

	for( i = i0 ; ; i++ ){
		sprintf( file, base, i );

		if( gpFile_exist( file ) < 0 ){
			if( fileName != NULL )
				strcpy( fileName, file );
			return( i );
		}
	}

	return( -1 );
}



char *
gpFile_fullpath( char *file, char *fullpath )
{
#if defined (_WIN32)
  {
//    char buf[MAX_PATH];
    char* basename;
    DWORD len = GetFullPathName ( file, MAX_PATH, fullpath,&basename);



 //   if (len == 0 || len > MAX_PATH - 1)
 //     return strdup (filename);
 //   else
 //     {
	///* The file system is case-preserving but case-insensitive,
	//   Canonicalize to lowercase, using the codepage associated
	//   with the process locale.  */
 //       CharLowerBuff (buf, len);
 //       return strdup (buf);
 //     }
	 return( fullpath );
  }
#endif

  return( NULL );

 

}
