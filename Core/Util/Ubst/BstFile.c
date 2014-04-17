/*********************
 ***   BstFile.c   ***
 *********************/
#include	<stdio.h>
#include	"Uigp/igp.h"


#include	"BstType.h"

 

int
bst_write_to_file( bst_type *bst, char *file )
{
FILE	*fp;

        if( (fp = fopen( file, "wb" )) == NULL )	return( -1 );

        bst_write( fp, bst );
        fclose( fp );

	return( 1 );
}



bst_type *
bst_read_from_file( char *file )
{
bst_type	*bst;
FILE	*fp;
int	byte_no;


	if( (byte_no = gpFile_size(file)) == -1 )	return( NULL );

	if( (fp = fopen( file, "rb" )) == NULL )	return( NULL );


	bst = bst_read( fp, byte_no );

	fclose( fp );

	return( bst );
}

