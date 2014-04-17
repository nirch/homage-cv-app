/**********************
 ***	CstTool.c   ***
 **********************/
#include	<string.h>
#include	<stdlib.h>
#include	"Uigp/igp.h"

#include	"CstType.h"
#include	"Ubst/BstType.h"

cst_type *
cst_alloc( int byte_no )
{
cst_type *cst;

	cst = (cst_type *)malloc( sizeof(cst_type) );

	cst->BYTE_NO = byte_no;
	if( cst->BYTE_NO > 0 )
		cst->data = (u_char *) calloc( cst->BYTE_NO, 1 );
	else	cst->data = NULL;

//	cst->byte_no = 0;

	cst->rp = cst->data;
	cst->wp = cst->data;

	return( cst );
}


void
cst_realloc( cst_type *cst, int byte_no )
{
	if( byte_no > cst->BYTE_NO ){
		free( cst->data );
		cst->BYTE_NO = byte_no;
		cst->data = (u_char *) calloc( cst->BYTE_NO, 1 );
	}

//	cst->byte_no = 0;

	cst->rp = cst->data;
	cst->wp = cst->data;
}


void
cst_extend(cst_type *cst, int byte_extention)
{
	u_char* buf = cst->data;
	int location = (int)(cst->rp - cst->data);
	int last_byte = (int)(cst->wp - cst->data);

	if ( byte_extention > 0 ){

		cst->BYTE_NO += byte_extention;
		cst->data = (u_char *) calloc( cst->BYTE_NO, 1 );
		memcpy(cst->data,buf, last_byte );
		free(buf);
		cst->rp =  &cst->data[location];
		cst->wp =  &cst->data[last_byte];
	}

/*
	u_char* buf = cst->data;
	int location = (int)(cst->rp - cst->data);
	int old_byte_no = cst->BYTE_NO;
	if ( byte_extention > 0 )
	{
		cst->BYTE_NO += byte_extention;
		cst->data = (u_char *) calloc( cst->BYTE_NO, 1 );
		memcpy(cst->data,buf,old_byte_no);
		free(buf);
		cst->rp =  &cst->data[location];
	}
*/
}



void
cst_clear( cst_type *cst )
{
//	cst->byte_no = 0;

	cst->rp =  cst->data;
	cst->wp =  cst->data;
}



void
cst_free( cst_type *cst )
{
	if( cst->data != NULL )
		free( cst->data );

	free( cst );
}


void
cst_rewind( cst_type *cst )
{
	cst->rp =  cst->data;
}


int
cst_get_buffer( cst_type *cst, u_char buf[], int byte_no )
{
int	i;

//	if( CST_BYTE_INDEX(cst) + byte_no > cst->byte_no )
//		byte_no = cst->byte_no - CST_BYTE_INDEX( cst );
	if( cst->rp + byte_no > cst->wp )
		byte_no = cst->wp - cst->rp;

	for( i = 0 ; i < byte_no ; i++ )
		buf[i] = CST_GET_UCHAR( cst );

	return( byte_no );

}



int
cst_put_buffer_int( cst_type *cst, u_char buf[], int byte_no )
{
	int	i;
	for( i = 0 ; i < byte_no ; i++ )
		CST_PUT_CHAR( cst , buf[i]);

//	cst->byte_no = cst->rp - cst->data;
	return byte_no;
}





int
cst_put_buffer( cst_type *cst, char buf[], int byte_no )
{
char *p;
int	i;

	for( p= buf, i = 0 ; i < byte_no ; i++, p++ )
		CST_PUT_CHAR( cst , *p);

//	cst->byte_no = cst->rp - cst->data;

	return byte_no;
}

int
cst_put_string( cst_type *cst, char buf[] )
{
char *p;
	if (!buf)
		return (1);
	for( p = buf ; *p != 0 ; p++ )
		CST_PUT_CHAR( cst , *p);

	return( 1 );
}


void	
cst_put_stringA( cst_type *cst, int align, char *name )
{
char *p;
int	i;

	for( i = 0 ; i < align ; i++ )
		CST_PUT_CHAR( cst , '\t' );

	for( p = name ; *p != 0 ; p++ )
		CST_PUT_CHAR( cst , *p);
}



cst_type *
cst_make_copy( cst_type *cst )
{
cst_type	*ccst;

	ccst = cst_create( (char *)cst->data, CST_BYTES(cst), 1 ); //cst->byte_no, 1 );

	return( ccst );
}

cst_type *
cst_create( char data[], int byte_no, int FCopydata )
{
cst_type	*cst;

	
	
	if( FCopydata  == 1 ){
		cst = cst_alloc(byte_no);
		memcpy( cst->data, data, byte_no );
	}
	else	
	{
		cst = (cst_type *)malloc( sizeof(cst_type) );
		cst->data = (unsigned char *)data;
	}
	cst->BYTE_NO = byte_no;


//	cst->byte_no = cst->BYTE_NO;

	cst->rp =  cst->data;
	cst->wp = cst->data + byte_no;

	return cst;
}



void
cst_destroy( cst_type *cst, int FDeletedata )
{
	if( FDeletedata )
		free( cst->data );

	free( cst );
}


struct bst_type *cst2bst(cst_type* cst, int byte_no)
{
	bst_type *bst = bst_alloc( byte_no );

	memcpy( bst->data, cst->data, byte_no );
	bst->byte_no = bst->BYTE_NO;
	return bst;
}

void
cst_insert( cst_type *cst, char *data, int bytes )
{
char	*buf;

	if( cst->BYTE_NO - (cst->wp - cst->data ) > bytes + 1 ){
		memcpy( cst->wp, data, bytes );
		cst->wp += bytes;

		return;
	}


	if(  cst->BYTE_NO - (cst->wp - cst->rp) > bytes + 1 ){
		memcpy( cst->data, cst->rp, cst->wp - cst->rp );
		cst->wp -= (cst->rp - cst->data);
		cst->rp = cst->data;


		memcpy( cst->wp, data, bytes );
		cst->wp += bytes;
	

		return;
	}



	cst->BYTE_NO = (cst->wp - cst->rp) + bytes +1;
	buf = (char *)malloc( cst->BYTE_NO );

	memcpy (buf, cst->rp, cst->wp - cst->rp );
	free( cst->data );
	cst->data = buf;
	cst->wp = cst->data + (cst->wp - cst->rp);
	cst->rp = cst->data;


	memcpy( cst->wp, data, bytes );
	cst->wp += bytes;
}


void
cst_rewind_data( cst_type *cst )
{

	if( cst->wp == cst->rp ){
		cst->wp = cst->rp = cst->data;
		return;
	}

	if( cst->rp != cst->data ){

		memcpy( cst->data, cst->rp, cst->wp - cst->rp );
		cst->wp -= (cst->rp - cst->data);
		cst->rp = cst->data;
	}
}