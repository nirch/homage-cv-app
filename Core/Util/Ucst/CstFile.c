/**********************
 ***	CstFile.c   ***
 **********************/
#include	<stdio.h>

#include	"CstType.h"



void
cst_read( FILE *fp, cst_type *cst, int byte_no )
{
int	no;

	cst_realloc( cst, byte_no );

//	cst->byte_no = fread( cst->data, 1, byte_no, fp );

	no = fread( cst->data, 1, byte_no, fp );

	cst->rp =  cst->data;
	cst->wp = cst->data + no;
}



void
cst_write( FILE *fp, cst_type *cst )
{
//	fwrite( cst->data, 1, cst->byte_no, fp );
	fwrite( cst->data, 1, CST_BYTES(cst), fp );
}



cst_type *
cst_load_from_file( char *file )
{
cst_type	*cst;
FILE	*fp;
int	byte_no;

	byte_no = gpFile_size( file );

	if ( (fp = fopen ( file, "rb" )) == NULL )
		return( NULL );

	cst = cst_alloc( byte_no );
	cst_read( fp, cst, byte_no );

	fclose( fp );

	return( cst );
}


int
cst_write_to_file( cst_type *cst, char *file )
{
FILE	*fp;

    if( (fp = fopen( file, "wb" )) == NULL )	return( -1 );

    cst_write( fp, cst );
    fclose( fp );

	return( 1 );
}

