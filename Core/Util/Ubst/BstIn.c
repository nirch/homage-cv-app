/**************************
 ***	BstIn.c		***
 **************************/
#include	<stdio.h>

#include	"BstType.h"


#define		INCREMENT_BYTE( bst ) \
			bst->byte_i++; \
			bst->byte_ptr++;



int
bst_InputBit( bst_type *bst )
{
int	value;
 

	value = (*bst->byte_ptr) & bst->mask;
	bst->mask >>= 1;

	if ( bst->mask == 0 ){
		bst->mask = 0x80;
		bst->byte_i++;
		bst->byte_ptr++;
	}

	return( value ? 1 : 0 );
}



int
bst_InputBits( bst_type *bst, int bit_no )
{
unsigned int	mask;
unsigned int	return_value;
 
	 mask = 1L << ( bit_no - 1 );
	 return_value = 0;

	 while( mask != 0 ){

		if( (*bst->byte_ptr) & bst->mask )
			return_value |= mask;
		mask >>= 1;
		bst->mask >>= 1;

		if( bst->mask == 0 ){
			bst->mask = 0x80;
			bst->byte_i++;
			bst->byte_ptr++;
		}
	}

	 return( (int)return_value );
}




void
bst_OutputBit( bst_type *bst, int bit )
{
	if( bit != 0 )
		*bst->byte_ptr |= bst->mask;

	bst->mask >>= 1;

	if ( bst->mask == 0 ){
		bst->mask = 0x80;
		bst->byte_i++;
		bst->byte_ptr++;

		*bst->byte_ptr = 0;
	}
}

void
bst_OutputBits( bst_type *bst, int value, int nBits )
{
int	m,	i;

	m = 1L << ( nBits - 1 );
//	for( i = nBits-1 ; i >= 0 ; i++, m >>= 1 )
	for( i = 0 ; i < nBits ; i++, m >>= 1 )
		bst_OutputBit( bst, value & m );
}



void
bst_in_align( bst_type *bst )
{

	if( bst->mask == 0x80 )	return;

	INCREMENT_BYTE( bst );
	bst->mask = 0x80;
}


void
bst_in_align_seek( bst_type *bst, int bytes )
{

	bst->byte_i += bytes;
	bst->byte_ptr += bytes;

	bst->mask = 0x80;
}



void
bst_OutputAlignByte( bst_type *bst, int value )
{
	bst_in_align( bst );


	*bst->byte_ptr = 0xff & value;
	bst->byte_i++;
	bst->byte_ptr++;

}

void
bst_OutputAlignInt( bst_type *bst, int value, int bytes )
{
int	i,	s;

	bst_in_align( bst );


	for( i = 0, s = 0 ; i < bytes ; i++, s += 8 ){
		*bst->byte_ptr = 0xff&(value>>s);
		bst->byte_i++;
		bst->byte_ptr++;
	}

}

void
bst_OutputAlignBytes( bst_type *bst, char *data, int bytes )
{
int	i;

	bst_in_align( bst );

	for( i = 0 ; i < bytes ; i++ ){
		*bst->byte_ptr = data[i];
		bst->byte_i++;
		bst->byte_ptr++;
	}

}
