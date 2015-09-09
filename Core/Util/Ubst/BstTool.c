/*********************
 ***   BstTool.c   ***
 *********************/
#include	<string.h>
#include	<stdlib.h>
#include	"Uigp/igp.h"

#include	"BstType.h"


bst_type *
bst_alloc( int byte_no )
{
bst_type *bst;

	bst = (bst_type *)malloc( sizeof(bst_type) );

	bst->BYTE_NO = byte_no;
	if( bst->BYTE_NO > 0 )
		bst->data = (u_char *) calloc( bst->BYTE_NO, 1 );
	else	bst->data = NULL;

	bst->byte_no = 0;

	bst->byte_i = 0;
	bst->byte_ptr =  &bst->data[bst->byte_i];
	bst->MaskIndex = 0;
	bst->mask = 0x80;

	return( bst );
}


void
bst_realloc( bst_type *bst, int byte_no )
{
	if( byte_no > bst->BYTE_NO ){
		free( bst->data );
		bst->BYTE_NO = byte_no;
		bst->data = (u_char *) calloc( bst->BYTE_NO, 1 );
	}

	bst->byte_no = 0;

	bst->byte_i = 0;
	bst->byte_ptr =  &bst->data[bst->byte_i];
	bst->MaskIndex = 0;
	bst->mask = 0x80;
}


void
bst_extend(bst_type *bst, int byte_extention)
{
	u_char* buf = bst->data;
	int old_byte_no = bst->BYTE_NO;
	if ( byte_extention > 0 )
	{
		bst->BYTE_NO += byte_extention;
		bst->data = (u_char *) calloc( bst->BYTE_NO, 1 );
		memcpy(bst->data,buf,old_byte_no);
		free(buf);
		bst->byte_ptr =  &bst->data[bst->byte_i];
	}
}


void
bst_rewind( bst_type *bst )
{
	bst->byte_i = 0;

        bst->byte_ptr =  &bst->data[0];
        bst->MaskIndex = 0;
        bst->mask = 0x80; 
}



void
bst_clear( bst_type *bst )
{
	bst->byte_no = 0;
	bst->byte_i = 0;
	bst->byte_ptr =  &bst->data[bst->byte_i];
	bst->MaskIndex = 0;
}



void
bst_destroy( bst_type *bst )
{
	free( bst->data );

	free( bst );
}





bst_type *
bst_create( char data[], int byte_no, int FCopydata )
{
bst_type	*bst;

	
	
	if( FCopydata  == 1 ){
		bst = bst_alloc(byte_no);
		memcpy( bst->data, data, byte_no );
	}
	else	
	{
		bst = (bst_type *)malloc( sizeof(cst_type) );
		bst->data = (unsigned char *)data;
		bst->BYTE_NO = byte_no;
	}

	bst->byte_no = byte_no;



	bst->byte_i = 0;
	bst->byte_ptr =  &bst->data[bst->byte_i];
	bst->MaskIndex = 0;
	bst->mask = 0x80;


	return bst;
}



bst_type *
bst_make_copy( bst_type *bst_in )
{
bst_type	*bst;
int	bytes;

	bytes = MAX( BST_BYTES(bst_in), bst_in->byte_no );

	bst = bst_alloc( bytes + 1 );

	memcpy( bst->data, bst_in->data, bytes);
	bst->byte_no = bytes;


	bst->byte_i = 0;
	bst->byte_ptr =  &bst->data[bst->byte_i];
	bst->MaskIndex = 0;
	bst->mask = 0x80;


	return bst;

}



bst_type *
bst_read( FILE *fp, int byte_no )
{
bst_type	*bst;

	bst = bst_alloc( byte_no );

	bst->byte_no = fread( bst->data, 1, byte_no, fp );

	bst_rewind( bst );


	return( bst );
}



void
bst_write( FILE *fp, bst_type *bst )
{
int	byte;

	byte = ( bst->MaskIndex == 0 )? bst->byte_i : bst->byte_i +1;

	fwrite( bst->data, 1, byte, fp );

}

