/***********************
 ***   MediaFile.c   ***
 ***********************/
#include	<string.h>
#include	"Uigp/igp.h"

#include	"MediaDataType.h"



mediaData_type *
mediaData_from_file( char *mime, char *id, char *file )
{
mediaData_type	*md;
char	*data;
int		dataSize;
	

	if( gpFile_read_to_buffer( file, (char**) &data, &dataSize ) < 0 )
		return( NULL );

	md = mediaData_from_buffer( mime, id, file, data, dataSize, 0 );

	return( md );
}
