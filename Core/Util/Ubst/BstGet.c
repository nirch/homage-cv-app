/**************************
 ***	BstGet.c	***
 **************************/
#include	<string.h>
#include	<stdlib.h>
#include	"BstType.h"

static const unsigned char   set_mask[8] =
		{0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
static const unsigned char   set_mask_inv[8] =
		{0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};//arie

#define		INCREMENT_BYTE( bst ) \
			bst->byte_i++; \
			bst->byte_ptr++;



int
bst_get( bst_type *bst )
{
int	bit;

	bit = *bst->byte_ptr & set_mask[bst->MaskIndex];
	bst->MaskIndex++;

	if( bst->MaskIndex == 8 ){
		INCREMENT_BYTE( bst );
		bst->MaskIndex = 0;
	}

	return( bit ? 1 : 0 );
}

int
bst_get_inv( bst_type *bst )
{
int	bit;

	bit = *bst->byte_ptr & set_mask_inv[bst->MaskIndex];
	bst->MaskIndex++;

	if( bst->MaskIndex == 8 ){
		INCREMENT_BYTE( bst );
		bst->MaskIndex = 0;
	}

	return( bit ? 1 : 0 );
}




int
bst_get_int( bst_type *bst, int bit_no )
{
u_long	code,	mask;
int	shift;

	if( bit_no <= 0 )	return( 0 );

	mask = (1L<<bit_no) -1;

	code = ( *bst->byte_ptr >> bst->MaskIndex );
	shift = 8 - bst->MaskIndex;
	bst->MaskIndex += bit_no;
	if( bst->MaskIndex < 8 )	return( code&mask );


	INCREMENT_BYTE( bst );
	if( (bst->MaskIndex -= 8 ) == 0 )
		return( code&mask );

	code |= ( (long)*bst->byte_ptr << shift );
	if( bst->MaskIndex < 8 )	return( code&mask );
	shift += 8;



	INCREMENT_BYTE( bst );
	if( (bst->MaskIndex -= 8 ) == 0 )	
		return( code&mask );

	code |= ( (long)*bst->byte_ptr << shift );
	if( bst->MaskIndex < 8 )	return( code&mask );
	shift += 8;



	INCREMENT_BYTE( bst );
	if( (bst->MaskIndex -= 8 ) == 0 )
		return( code&mask );

	code |= ( (long)*bst->byte_ptr << shift );
	if( bst->MaskIndex < 8 )	return( code&mask );
	shift += 8;



	INCREMENT_BYTE( bst );
	if( (bst->MaskIndex -= 8 ) == 0 )
		return( code&mask );

	code |= ( (long)*bst->byte_ptr << shift );
	if( bst->MaskIndex < 8 )	return( code&mask );
	shift += 8;


	return( code&mask );

}



void
bst_get_string( bst_type *bst, char *s, int len )
{
int     i;

        bst_get_align(bst);

        for( i = 0 ; i < len ; i++ ){
		s[i] =  bst_get_align_char(bst);
		if( s[i] == 0 )	return;
	}
}


char *
bst_get_Astring(  bst_type *bst, int len )
{
char    b[1000],        *str;
int     i;
 
        bst_get_align(bst);
 
        for( i = 0 ; i < len ; i++ ){
                b[i] = bst_get_align_char(bst);
                if( b[i] == 0 ) break;
        }
        b[i] = 0;
 
        len = i;
        str = (char *)malloc( len+1 );
        for( i = 0 ; i <= len ; i++ )
                str[i] = b[i];
 
        return( str );
}


void
bst_get_buff( bst_type *bst, char *s, int len )
{
int     i;

        bst_get_align(bst);

        for( i = 0 ; i < len ; i++ ){
		s[i] =  bst_get_align_char(bst);	}
}


void
bst_get_align( bst_type *bst )
{

	if( bst->MaskIndex == 0 )	return;

	INCREMENT_BYTE( bst );
	bst->MaskIndex = 0;
}


int
bst_get_align_char( bst_type *bst )
{
int	a;

	a = *bst->byte_ptr;

	bst->byte_i++;
	bst->byte_ptr++;

	return( a );
}


int
bst_get_align_num( bst_type *bst, int byte_no )
{//bst_get_int assumes that numbers are stored low order byte first
        unsigned long result=0;

	if( bst->MaskIndex != 0 )
		bst_get_align( bst );

        for(;byte_no>0;byte_no--)
        {
                result <<= 8;
                result += bst_get_int(bst,8);
        }
        return result;
}


int
bst_get_sint( bst_type *bst, int bits )
{
int	val, shift;

	val = bst_get_int( bst, bits );

	shift = 32 - bits;
	val = (val << shift)>>shift;

	return( val );
}



float
bst_get_float_code( bst_type *bst, code *cd, int bit )
{
int     cno,    sign;
float	a;
 
	if( bit == 0 )	return( 0.0 );
 

	sign = bst_get( bst );
	cno = bst_get_int( bst, bit );
	if( sign == 1 ) cno = -cno;
 
	a = UNZCODE( cno, *cd );

	return( a );
}


float
bst_get_float( bst_type *bst, float d, int bit )
{
int     cno,    sign;
float	a;
 
	if( bit == 0 )	return( 0.0 );
 

	sign = bst_get( bst );
	cno = bst_get_int( bst, bit );
	if( sign == 1 ) cno = -cno;
 
	a = cno * d;

	return( a );
}


bst_type *
bst_get_align_bst( bst_type *bst_in, int byte_no )
{
bst_type	*bst;

	bst = bst_alloc( byte_no + 1 );

	memcpy( bst->data, bst_in->byte_ptr, byte_no);
	bst->byte_no = byte_no;

	bst_in->byte_i += byte_no;
	bst_in->byte_ptr += byte_no;

	return( bst );
}
