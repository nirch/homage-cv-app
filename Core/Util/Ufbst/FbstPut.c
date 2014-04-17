/**************************
 ***	BstPut.c	***
 **************************/
#include	<stdio.h>

#include	"FbstType.h"

static u_char   set_mask[8] =
	{0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

static u_char unset_mask[8] =
        {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};

#define		INCREMENT_BYTE( bst ) \
			bst->byte_i++; \
			putc( bst->byte, bst->fp );



void
fbst_put( fbst_type *bst, int bit )
{
	if( bit )	bst->byte |= set_mask[bst->MaskIndex];
	else	bst->byte &= unset_mask[bst->MaskIndex];
	bst->MaskIndex++;

	if( bst->MaskIndex == 8 ){
		INCREMENT_BYTE( bst );
		bst->MaskIndex = 0;
		bst->byte = 0;
	}
}







void
fbst_put_int( fbst_type *bst, int code, int bit_no )
{
int	shift;

	code &= ( 1L << bit_no) -1;

	if( bit_no <= 0 )	return;
	bst->byte |= (u_char)(code << bst->MaskIndex);
	shift = 8 - bst->MaskIndex;
	bst->MaskIndex += bit_no;
	if( bst->MaskIndex < 8 )	return;

	code >>= shift;
	INCREMENT_BYTE( bst );
	bst->byte = (u_char)code;
	if( (bst->MaskIndex -= 8 ) < 8 )	return;


	code >>= 8;
	INCREMENT_BYTE( bst );
	bst->byte = (u_char)code;
	if( (bst->MaskIndex -= 8 ) < 8 )	return;


	code >>= 8;
	INCREMENT_BYTE( bst );
	bst->byte = (u_char)code;
	if( (bst->MaskIndex -= 8 ) < 8 )	return;

	code >>= 8;
	INCREMENT_BYTE( bst );
	bst->byte = (u_char)code;
	bst->MaskIndex -= 8;
}



void
fbst_put_algin( fbst_type *bst )
{
	if( bst->MaskIndex == 0 )	return;

	INCREMENT_BYTE( bst );
	bst->MaskIndex = 0;
	bst->byte = 0;
}

void
fbst_put_algin_char( fbst_type *bst, int a )
{
        bst->byte = a;

	INCREMENT_BYTE( bst );
	bst->MaskIndex = 0;
}



void
fbst_put_string( fbst_type *bst, char *s, int len )
{
int	i;
	for( i = 0 ; i < len ; i++ )
		fbst_put_int( bst, (u_long)s[i], 8 );
}
