/**************************
 ***	BstTool.c	***
 **************************/
#include	<stdio.h>

#include	"Uigp/igp.h"

#include	"FbstType.h"


fbst_type *
fbst_alloc()
{
fbst_type *bst;

	bst = (fbst_type *)malloc( sizeof(fbst_type) );

	bst->byte_i = 0;

	bst->byte =  0;
	bst->MaskIndex = 0;
	bst->mask = 0x80;

	return( bst );
}


void
fbst_free( fbst_type *bst )
{
	free( bst );
}



fbst_type *
fbst_open_to_write( char *file_name )
{
fbst_type *bst;
FILE	*fp;

	if( (fp = fopen( file_name, "wb" )) == NULL )
		return( NULL );

	bst = fbst_alloc();
	bst->fp = fp;

	bst->MaskIndex = 0;


	return( bst );
}


fbst_type *
fbst_open_to_read( char *file_name )
{
fbst_type *bst;
FILE	*fp;


	if( (fp = fopen( file_name, "rb" )) == NULL )
		return( NULL );

	bst = fbst_alloc();
	bst->fp = fp;

	bst->MaskIndex = 8;

	return( bst );
}


fbst_type *
fbst_set_to_read( FILE *fp )
{
fbst_type *bst;

	bst = fbst_alloc();
	bst->fp = fp;

	bst->MaskIndex = 8;

	return( bst );
}



void
fbst_close( fbst_type *bst, int Fwrite )
{
	if( Fwrite && bst->MaskIndex != 0 )
		putc( bst->byte, bst->fp );

	fclose( bst->fp );

	free( bst );
}
