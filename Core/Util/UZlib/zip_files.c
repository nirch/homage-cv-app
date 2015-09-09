/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <errno.h>
#include <fcntl.h>
*/

#include <time.h>

#ifndef WIN32
# include <unistd.h>
# include <utime.h>
# include <sys/types.h>
# include <sys/stat.h>
#else
#include <sys/timeb.h>
# include <direct.h>
# include <io.h>
#endif


#include "zip_file.h"
#include "Uigp/igp.h"
#include "UTime/GpTime.h"


#define MAXFILENAME (256)
#define DELTA_CLOCK_DEFAULT 24
#define MAX_NUM_OF_FILES	1024
#define FULL_PATH			1
#define ZIP_OK				0


/*
ziping all files in folder pathname ( not in subdir )
file name.ext ziping to name.zip and deleted ( not *.zip filse )

pathname   - pathname to folder 
	0 - in curent folder
deltaClock - only files modified befor deltaClock clock
	deltaClock == -1  -  deltaClock=DELTA_CLOCK_DEFAULT
	DELTA_CLOCK_DEFAULT = 24
return 0 if OK, else no 0
*/
int zip_files_date( char *pathname, int deltaClock )
{
char *files[MAX_NUM_OF_FILES];
int  num_of_files, i;

//struct timeb timeNow;
long timeBefor, timeDelta, timeWrite;

char dir[MAXFILENAME], name[MAXFILENAME], extension[MAXFILENAME];
char filename[2*MAXFILENAME];


	if ( deltaClock <= 0 )
		deltaClock = DELTA_CLOCK_DEFAULT;

//	ftime( &timeNow );
//	timeDelta = (long)deltaClock*3600L;
//	timeBefor = timeNow.time - timeDelta;

	timeDelta = (long)deltaClock*3600L;
	timeBefor = vTime()/1000 - timeDelta;

	num_of_files = gpDir_get_files_list( pathname, 0, 0,
						 files, MAX_NUM_OF_FILES, FULL_PATH );

    for ( i = 0; i < num_of_files; i++ ) 	{

		timeWrite = gpFile_mtime( files[i] );

		if ( timeWrite >= timeBefor )
			continue;

		gp_filename_split( files[i], dir, name, extension);

		if ( gp_stricmp( ".zip", extension ) == 0 )
			continue;

		sprintf( filename, "%s/%s%s", dir, name, extension );
		filename[2*MAXFILENAME-1] = 0;

		if ( zip_file( filename, 0, -1, -1, 1 /*overwrite*/) == ZIP_OK )
			if ( unlink(filename) != 0 )
				printf( "Could not delete %s\n", filename );
	}

    for ( i = 0; i < num_of_files; i++ )
		free( files[i] );

	return ZIP_OK;
}
