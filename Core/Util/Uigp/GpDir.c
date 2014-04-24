/******************
 ***	GpDir.c	***
 ******************/
#include	<stdio.h>

#ifdef  WIN32
#include <windows.h>
#include	<direct.h>
#else
#include	<dirent.h>
#endif


#include	<string.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/stat.h>

#include	"igp.h"


/*********  gpDir_get_files_list  **********
* @Description	:  Retrieves the names of files in that directory
* 
* @Param  const char *dir :  directory to search in
* @Param  const char *pre :  prefix of the files to find
* @Param  const char *ext :  extensions of the files to find //NOT YET SUPPORTED : (separated by ';')
* @Param  char *[]files :  array to put the filenames in.
* @Param  int max_num_of_files :  length of that array
* @Param  int bFullPath :  flag :	(0) name and extension only. 
									(1) full path - dir/name.extension
* @Return  int  :  num of files retrieved
*****************************************/

int gpDir_get_files_list_W( char *dir,  char *prefix, char *ext,
						 char *files[], int max_num_of_files, int bFullPath )
{
int	n;

	if( gpDir_exist( dir ) != 1 ){
		files[0] = strdup( dir );
		return( 1 );
	}

	n = gpDir_get_files_list( dir,  prefix, ext, files, max_num_of_files,  bFullPath );

	return( n );
}



int gpDir_get_files_list( char *dir,  char *prefix, char *ext,
						 char *files[], int max_num_of_files, int bFullPath )
{
	int numOfFilesFound = 0;
char	cext[256],	extension[512];
int	n;



#if defined  WINDOWS || defined WIN32 






	char m_sFilter[2048];
	WIN32_FIND_DATA m_w32fdFindFileData;
	HANDLE m_hFind = NULL;	
	//char m_sSearch[2048] ;

	char * fullPathName ;
	int dirStrLength = strlen(dir);



	if( gpDir_exist( dir ) != 1 ){
		files[0] = strdup( dir );
		return( 1 );
	}



	strcpy(m_sFilter,dir);
	if ( dir[dirStrLength -1] != '/' && 
		dir[dirStrLength -1] != '\\')
		strcat(m_sFilter, "\\");
	
	if ( prefix != NULL )
		strcat(m_sFilter,prefix);
//	strcat(m_sFilter , "*.");
		

	//if ( ext != NULL ){
	//	strcat(m_sFilter , "*" );
	//	strcat(m_sFilter,ext);
	//}
	//else
		strcat(m_sFilter , "*.*");

	
	if( ext != NULL ){
		strcpy( extension, ext );
		n = strlen( extension );

		extension[n] = ';';
		extension[n+1] = 0;
	}


	m_hFind = FindFirstFile(m_sFilter, &m_w32fdFindFileData);
	if (m_hFind == INVALID_HANDLE_VALUE) 
	{
		FindClose(m_hFind);
		m_hFind = NULL;
		return 0;
	}	
	do 
	{	// search for files:
		if  (m_w32fdFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;

		if( ext != NULL ){
			gp_filename_extract_extension( m_w32fdFindFileData.cFileName, cext, 256 );
			n = strlen( cext );
			if( n == 0 )	continue;

			cext[n] = ';';
			cext[n+1] = 0;
//			if( strstr( extension, cext ) == NULL )
			if( gp_strcasestr( extension, cext ) == NULL )


				continue;
		}

		
		if (!bFullPath)
		{
			files[numOfFilesFound] = strdup(m_w32fdFindFileData.cFileName);		
		}
		else
		{
			fullPathName = (char *)malloc(dirStrLength + strlen(m_w32fdFindFileData.cFileName) + 2 );
			sprintf(fullPathName, "%s/%s", dir, m_w32fdFindFileData.cFileName);
			files[numOfFilesFound] = fullPathName;

		}
		numOfFilesFound++;
		
	}while (numOfFilesFound < max_num_of_files && 
		FindNextFile(m_hFind, &m_w32fdFindFileData) != FALSE );
	

	
	
	// clean : 
	FindClose(m_hFind);
	return numOfFilesFound;



#else // unix
	DIR *m_dDir = NULL;	
	struct dirent* entry;
//	struct stat     statbuf;
	char *fullPathName;
	int dirStrLength; 


	m_dDir = opendir(dir);
	
	if ( m_dDir == NULL)
		return 0;

	dirStrLength = strlen(dir);


	while (numOfFilesFound < max_num_of_files && 
			(entry = readdir(m_dDir )) != NULL ) 
	{

		if( strcmp( entry->d_name, "." ) == 0 )
			continue;

		if( strcmp( entry->d_name, ".." ) == 0 )
			continue;
	
		if ( prefix != NULL )
		{
			if ( strncmp(entry->d_name, prefix, strlen(prefix)) != 0)
				continue;
		}

		if( ext != NULL && gp_stricmp((entry->d_name)+ (strlen(entry->d_name) - strlen(ext)),ext) != 0 )
			continue;


		
		if (!bFullPath)
		{
			files[numOfFilesFound] = strdup(entry->d_name);		
		}
		else
		{
		
			fullPathName = (char *)malloc(dirStrLength + strlen(entry->d_name) + 2);
			sprintf(fullPathName, "%s/%s", dir, entry->d_name);
			files[numOfFilesFound] = fullPathName;

		}
		numOfFilesFound++;		
	}
	closedir(m_dDir);
	return numOfFilesFound;
#endif// defined  WINDOWS || defined WIN32 
	
}


#ifdef WIN32
int gpDir_get_files( char *dir,  char *prefix, char *suffix,
						 char *files[], int NFiles, int fullPath )
{

int nFiles = 0;


	char m_sFilter[2048];
	WIN32_FIND_DATA m_w32fdFindFileData;
	HANDLE m_hFind = NULL;	

	char * fullPathName ;
	int dirStrLength = strlen(dir);

	strcpy(m_sFilter,dir);
	if ( dir[dirStrLength -1] != '/' && 
		dir[dirStrLength -1] != '\\')
		strcat(m_sFilter, "\\");

	if ( prefix != NULL )
		strcat(m_sFilter,prefix);



	if ( suffix != NULL ){
		strcat(m_sFilter , "*" );
		strcat(m_sFilter,suffix);
	}


	m_hFind = FindFirstFile(m_sFilter, &m_w32fdFindFileData);
	if (m_hFind == INVALID_HANDLE_VALUE) 
	{
		FindClose(m_hFind);
		m_hFind = NULL;
		return 0;
	}	


	nFiles = 0;
	do 
	{	// search for files:
		if  (m_w32fdFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;


		if( fullPath == 1){
			fullPathName = (char *)malloc(dirStrLength + strlen(m_w32fdFindFileData.cFileName) + 2 );
			sprintf(fullPathName, "%s\\%s", dir, m_w32fdFindFileData.cFileName);
			files[nFiles] = fullPathName;

		}
		else
			files[nFiles] = strdup(m_w32fdFindFileData.cFileName);		


		nFiles++;

	}while (nFiles < NFiles && 
		FindNextFile(m_hFind, &m_w32fdFindFileData) != FALSE );



 
	FindClose(m_hFind);

	return nFiles;

//	return( 1 );
}
#endif

#ifndef WIN32
int gpDir_get_files( char *dir,  char *prefix, char *suffix,
					char *files[], int NFiles, int fullPath )
{
	return( 1 );
}
#endif


int
gpDir_get_directory( char *dir, char *precede, char *extension,
					char *name[], int max, int bFullPath  )
{
#ifdef	WIN32
	char m_sFilter[2048];
	WIN32_FIND_DATA m_w32fdFindFileData;
	HANDLE m_hFind = NULL;	
	//char m_sSearch[2048] ;
//	int	bFullPath = 1;

	char * fullPathName ;
	int dirStrLength = strlen(dir);

	int numOfFilesFound = 0;


//	if( gpFile_exist( dir ) == 1 ){
	if( gpDir_exist( dir ) != 1 ){
		char	dir1[256];
		if( gpFile_exist( dir ) != 1 )
			return( -1 );

	
		gpFilename_dir( dir, dir1 );
		name[0] = strdup( dir1 );
		return( 1 );
	}

	strcpy(m_sFilter,dir);
	if ( dir[dirStrLength -1] != '/' && 
		dir[dirStrLength -1] != '\\')
		strcat(m_sFilter, "\\");

	if ( precede != NULL )
		strcat(m_sFilter,precede);
	//	strcat(m_sFilter , "*.");


	//if ( extension != NULL ){
	//	strcat(m_sFilter , "*" );
	//	strcat(m_sFilter,extension);
	//}
	//else
		strcat(m_sFilter , "*.*");


	m_hFind = FindFirstFile(m_sFilter, &m_w32fdFindFileData);
	if (m_hFind == INVALID_HANDLE_VALUE) 
	{
		FindClose(m_hFind);
		m_hFind = NULL;
		return 0;
	}	
	do 
	{	// search for files:
		if(  (m_w32fdFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 )
			continue;


		if( strcmp( m_w32fdFindFileData.cFileName, "." ) == 0 )
			continue;

		if( strcmp( m_w32fdFindFileData.cFileName, ".." ) == 0 )
			continue;

		if (!bFullPath)
		{
			name[numOfFilesFound] = strdup(m_w32fdFindFileData.cFileName);		
		}
		else
		{
			fullPathName = (char *)malloc(dirStrLength + strlen(m_w32fdFindFileData.cFileName) + 2 );
			sprintf(fullPathName, "%s/%s", dir, m_w32fdFindFileData.cFileName);
			name[numOfFilesFound] = fullPathName;

		}
		numOfFilesFound++;

	}while (numOfFilesFound < max && 
		FindNextFile(m_hFind, &m_w32fdFindFileData) != FALSE );




	// clean : 
	FindClose(m_hFind);
	return numOfFilesFound;


#else
struct dirent	*dp;
DIR	*dfd;
char	ext[10][64],	*e;
int	ext_no;

int	no,	plen,	elen[10],	len,	k,	i;

	no = 0;

        if ( (dfd = opendir(dir)) == NULL)
                return( no );


	ext_no = 0;
	e = ext[ext_no++];
	for( i = 0 ; extension[i] != 0 ; i++ ){
		if( extension[i] == '\n' ){
			*e = 0;
			e = ext[ext_no++];
		}
		else	*e++ = extension[i];
	}
	*e = 0;


	plen = strlen( precede );
	for( i = 0 ; i < ext_no ; i++ )
		elen[i] = strlen( ext[i] );


        while ( (dp = readdir(dfd)) != NULL ) {

		len = strlen( dp->d_name );


		if( plen > 0  &&  strncmp( dp->d_name, precede, plen) != 0 )
			continue;


		for( i = 0 ; i < ext_no ; i++ ){
			if( len < elen[i] )	continue;
			
			k = len - elen[i];
			if( strncmp( &dp->d_name[k], ext[i], elen[i]) )
				continue;

			name[no] = (char *)malloc( len+1 );
			strcpy( name[no], dp->d_name );

			no++;
			name[no] = NULL;

			if( no == max )	return( no );

			break;
		}
	}

	closedir( dfd );

	gpStr_sort( name, no  );
	
	return( no );
#endif
}



int
gp_directory_dir( char *dir, char *name[], int max  )
{
#ifdef	WIN32
	return( 0 );
#else
struct dirent	*dp;
struct stat	stbuf;
DIR	*dfd;
char	file[256];

int	no,	len;

	no = 0;

        if ( (dfd = opendir(dir)) == NULL)
                return( no );


        while ( (dp = readdir(dfd)) != NULL ) {

		if( strcmp( dp->d_name, "." ) == 0 )	continue;

		sprintf( file, "%s/%s", dir, dp->d_name );

		if( stat( file, &stbuf ) != 0 )	continue;

		if(( stbuf.st_mode & S_IFMT ) != S_IFDIR ) continue;

		len = strlen( dp->d_name );


		name[no] = (char *)malloc( len + 1 );
		strcpy( name[no], dp->d_name );

		no++;
		name[no] = NULL;

		if( no == max )	return( no );
	}

	closedir( dfd );

	gpStr_sort( name, no  );
	
	return( no );
#endif
}


int
gp_directory_childs( char *dir, char *precede, char *extension,
						char *name[], int max  )
{
#ifdef	WIN32
	return( 0 );
#else
struct dirent	*dp;
//struct stat	stbuf;
DIR	*dfd;

int	no,	plen,	elen,	len,	k;

	no = 0;

        if ( (dfd = opendir(dir)) == NULL)
                return( no );

	plen = strlen( precede );
	elen = strlen( extension );

        while ( (dp = readdir(dfd)) != NULL ) {

		len = strlen( dp->d_name );


		if( plen > 0  &&  strncmp( dp->d_name, precede, plen) != 0 )
			continue;

		k = len - elen;
		if( k <= 0 )	continue;
		if( strncmp( &dp->d_name[k], extension, elen) )
			continue;

		name[no] = (char *)malloc( k + 1 );
		strncpy( name[no], dp->d_name, k );
		name[no][k] = '\0';

		no++;
		name[no] = NULL;

		if( no == max )	return( no );
	}

	closedir( dfd );

	gpStr_sort( name, no  );
	
	return( no );
#endif
}



void
gpDir_delete_files(  char *dir, char *prefix, char *ext )
{
	char	*file[100];
	int	i,	no;

	while( (no = gpDir_get_files_list( dir, prefix, ext, file, 100, 1 )) > 0 ){
		for( i = 0 ; i < no ; i++ ){
			gpFile_delete( file[i] );
			free( file[i] );
		}
	}
}





int gpDir_get_file( char *dir, char *prefix, char *sufix, char *file )
{
char	*fname[1];


//	if(  gpDir_get_files_list( dir, prefix, ext, fname, 1, 1 ) <= 0 )

	if( gpDir_get_files( dir, prefix, sufix, fname, 1, 1 ) <= 0 )
		return( -1 );

	strcpy( file, fname[0] );
	free( fname[0] );

	return( 1 );
}