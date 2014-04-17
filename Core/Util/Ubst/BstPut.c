/**************************
 ***	BstPut.c	***
 **************************/
#include	<stdio.h>

#include	"BstType.h"

static const u_char   set_mask[8] =
	{0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
static const u_char   set_mask_inv[8] =
	{0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};//arie

static const u_char unset_mask[8] =
      {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};
static const u_char unset_mask_inv[8] =
        {0X7F, 0XBF, 0XDF, 0XEF, 0XF7, 0XFB, 0XFD, 0XFE};//arie

#define		INCREMENT_BYTE( bst ) \
			bst->byte_i++; \
			bst->byte_ptr++;



void
bst_put( bst_type *bst, int bit )
{
	if( bit )	*bst->byte_ptr |= set_mask[bst->MaskIndex];
	else	*bst->byte_ptr &= unset_mask[bst->MaskIndex];
	bst->MaskIndex++;

	if( bst->MaskIndex == 8 ){
		INCREMENT_BYTE( bst );
		bst->MaskIndex = 0;
		*bst->byte_ptr = 0;
	}
}

void
bst_put_inv( bst_type *bst, int bit )
{
	if( bit )	*bst->byte_ptr |= set_mask_inv[bst->MaskIndex];
	else	*bst->byte_ptr &= unset_mask_inv[bst->MaskIndex];
	bst->MaskIndex++;

	if( bst->MaskIndex == 8 ){
		INCREMENT_BYTE( bst );
		bst->MaskIndex = 0;
		*bst->byte_ptr = 0;
	}
}







void
bst_put_int( bst_type *bst, int code, int bit_no )
{
int	shift;

	code &= ( 1L << bit_no) -1;

	if( bit_no <= 0 )	return;
	*bst->byte_ptr |= (u_char)(code << bst->MaskIndex);
	shift = 8 - bst->MaskIndex;
	bst->MaskIndex += bit_no;
	if( bst->MaskIndex < 8 )	return;

	code >>= shift;
	INCREMENT_BYTE( bst );
	*bst->byte_ptr = (u_char)code;
	if( (bst->MaskIndex -= 8 ) < 8 )	return;


	code >>= 8;
	INCREMENT_BYTE( bst );
	*bst->byte_ptr = (u_char)code;
	if( (bst->MaskIndex -= 8 ) < 8 )	return;


	code >>= 8;
	INCREMENT_BYTE( bst );
	*bst->byte_ptr = (u_char)code;
	if( (bst->MaskIndex -= 8 ) < 8 )	return;

	code >>= 8;
	INCREMENT_BYTE( bst );
	*bst->byte_ptr = (u_char)code;
	bst->MaskIndex -= 8;
}



void
bst_put_align( bst_type *bst )
{
	if( bst->MaskIndex == 0 )	return;

	INCREMENT_BYTE( bst );
	bst->MaskIndex = 0;
	*bst->byte_ptr = 0;
}


void
bst_put_align_char( bst_type *bst, int a )
{
	*bst->byte_ptr = a;
	bst->byte_ptr++;
	bst->byte_i++;
}





void
bst_put_string( bst_type *bst, char *s, int len )
{
int	i;

	bst_put_align( bst );

	for( i = 0 ; i < len ; i++ ){
		bst_put_align_char( bst, s[i] );
		if( s[i] == 0 )	return;
	}
}


void
bst_put_buff( bst_type *bst, char *s, int len )
{
int	i;

	bst_put_align( bst );

	for( i = 0 ; i < len ; i++ ){
		bst_put_align_char( bst, s[i] );
	}
}

void
bst_put_align_num( bst_type *bst, int value, int byte_no )
{
	bst_put_align( bst );

	if( byte_no >= 4 )
		bst_put_align_char( bst, (value>>24)&0xff );

	if( byte_no >= 3 )
		bst_put_align_char( bst, (value>>16)&0xff );

	if( byte_no >= 2 )
		bst_put_align_char( bst, (value>>8)&0xff );

	if( byte_no >= 1 )
		bst_put_align_char( bst, value&0xff );
}  


void
bst_reput_align_num( bst_type *bst, int i, int value, int byte_no )
{

	if( byte_no >= 4 )
		bst->data[i++] = (value>>24)&0xff;

	if( byte_no >= 3 )
		bst->data[i++] = (value>>16)&0xff;

	if( byte_no >= 2 )
		bst->data[i++] = (value>>8)&0xff;

	if( byte_no >= 1 )
		bst->data[i++] = value&0xff;
}  




void
bst_put_float_code( bst_type *bst, float a, code *cd, int bit )
{
int     cno;
        if( bit == 0 )  return;
 
        cno = ZCODE( a, *cd );
        bst_put( bst, cno < 0 );
        bst_put_int( bst, ABS(cno), bit );
}



void
bst_put_float( bst_type *bst, float a, float d,  int bit )
{
int     cno;

	if( bit == 0 )  return;

	cno =  ( a < 0 )?  a/d - 0.5 : a/d + 0.5;

	bst_put( bst, cno < 0 );
	bst_put_int( bst, ABS(cno), bit );
}



void bst_print_binary( FILE *file, unsigned long code, int bits )
{
  while ( bits-- != 0 ) {
    if ( code & 1L )
      fputc( '1', file );
    else
      fputc( '0', file );
    code >>= 1;
  }
}
