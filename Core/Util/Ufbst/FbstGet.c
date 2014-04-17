/**************************
 ***	FbstGet.c	***
 **************************/
#include	<stdio.h>

#include	"FbstType.h"

static unsigned char   set_mask[8] =
		{0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

#define		INCREMENT_BYTE( bst ) \
			bst->byte_i++; \
			bst->byte = getc( bst->fp );



int
fbst_get( fbst_type *bst )
{
int	bit;


	if( bst->MaskIndex == 8 ){
		INCREMENT_BYTE( bst );
		bst->MaskIndex = 0;
	}


	bit = bst->byte & set_mask[bst->MaskIndex];
	bst->MaskIndex++;

	return( bit ? 1 : 0 );
}




void
fbst_get_int1( fbst_type *bst, int *cno, int bit_no )
{
	*cno = fbst_get_int( bst, bit_no );
}


int
fbst_get_int( fbst_type *bst, int bit_no )
{
u_long	code,	mask;
int	shift;

	if( bit_no <= 0 )	return( 0 );

	if( bst->MaskIndex == 8 ){
		INCREMENT_BYTE( bst );
		bst->MaskIndex = 0;
	}

	mask = (1L<<bit_no) -1;

	code = ( bst->byte >> bst->MaskIndex );
	shift = 8 - bst->MaskIndex;
	bst->MaskIndex += bit_no;
	if( bst->MaskIndex <= 8 )	return( code&mask );


	INCREMENT_BYTE( bst );
	code |= ( (long)bst->byte << shift );
	if( (bst->MaskIndex -= 8 ) <= 8 )	return( code&mask );
	shift += 8;

	INCREMENT_BYTE( bst );
	code |= ( (long)bst->byte << shift );
	if( (bst->MaskIndex -= 8 ) <= 8 )	return( code&mask );
	shift += 8;

	INCREMENT_BYTE( bst );
	code |= ( (long)bst->byte << shift );
	if( (bst->MaskIndex -= 8 ) <= 8 )	return( code&mask );
	shift += 8;

	INCREMENT_BYTE( bst );
	code |= ( (long)bst->byte << shift );
	if( (bst->MaskIndex -= 8 ) <= 8 )	return( code&mask );
	shift += 8;

	return( code&mask );

}




void
fbst_get_algin( fbst_type *bst )
{

	if( bst->MaskIndex == 0 )	return;

	INCREMENT_BYTE( bst );
	bst->MaskIndex = 0;
}


int
fbst_get_algin_char( fbst_type *bst )
{
int	a;

	a = bst->byte;

	INCREMENT_BYTE( bst );
	bst->MaskIndex = 0;

	return( a );
}
