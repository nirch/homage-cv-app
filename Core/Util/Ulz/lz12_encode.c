/************************** lz_compress.C *************************
 *
 * This is 12 bit LZW program. 
 * It uses a fixed size code, and does not attempt to flush the dictionary after it fills up.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "Ubst/BstType.h"
#include "Ucst/CstType.h"

#include "lz.h"

#define BITS                        12
#define MAX_CODE                   ( ( 1 << BITS ) - 1 )
#define TABLE_SIZE                 5021 
#define END_OF_STREAM              256
#define FIRST_CODE                 257
#define UNUSED                     -1


static unsigned int	find_child_node( int parent_code, int child_character );

dictionary dict[TABLE_SIZE];
/*
bst_type *lz_compress( cst_type *cst );


cst_type *LZCompress(cst_type *cst)
{
	return bst2cst(lz_compress(cst));
}
*/
	
/*
 * The compressor is short and simple.  It reads in new symbols one
 * at a time from the input file.  It then  checks to see if the
 * combination of the current symbol and the current code are already
 * defined in the dictionary.  If they are not, they are added to the
 * dictionary, and we start over with a new one symbol code.  If they
 * are, the code for the combination of the code and character becomes
 * our new code.
 */

bst_type *
lz12_encoding( cst_type *cst )
{
	
int next_code;
int character;
int string_code;
unsigned int index;
unsigned int length;


bst_type *bst;
/*
bst_type *ret;
*/
int	i;

	if ( cst == NULL )	return NULL;
	
	
	length = cst->BYTE_NO;
	bst = bst_alloc(4*length);
	

        /* init the dictionary */
        for ( i = 0 ; i < TABLE_SIZE ; i++ )
                dict[ i ].code_value = UNUSED;
        next_code = FIRST_CODE; 


	string_code = (int)(CST_GET_UCHAR(cst));
	if ( string_code == EOF )
        string_code = END_OF_STREAM;

	character = CST_GET_UCHAR(cst);

	while ( --length ) {

/*
		if ( bst->byte_i + 10 >= bst->BYTE_NO )
			bst_extend(bst,bst->BYTE_NO);
*/
		
		index = find_child_node( string_code, character );

		if ( dict[ index ].code_value != -1 )
			string_code = dict[ index ].code_value;
		else {
			if ( next_code <= MAX_CODE ) {
				dict[ index ].code_value = next_code++;
				dict[ index ].parent_code = string_code;
				dict[ index ].character = (char) character;
			}

	
			bst_put_int( bst, string_code, BITS);		
			string_code = character;
		}

		character = CST_GET_UCHAR(cst);		
	}

	bst_put_int(bst,string_code,BITS);		

	bst_put_int(bst,END_OF_STREAM,BITS);  

	return( bst );

/*
	ret = bst_alloc(bst->byte_i);
	memcpy(ret->data,bst->data,bst->byte_i);
	bst_free(bst);

	return ret;
*/
}



static unsigned int
find_child_node( int parent_code, int child_character )
{
int index;
int offset;
 
	index = ( child_character << ( BITS - 8 ) ) ^ parent_code;
	if ( index == 0 )
		offset = 1;
	else
		offset = TABLE_SIZE - index;

	for( ; ; ) {
		if ( index < 0 )
			index += TABLE_SIZE;

		if ( dict[ index ].code_value == UNUSED )
			return( index );

		if ( dict[ index ].parent_code == parent_code &&
			dict[ index ].character == (char) child_character )
			return( index );

		index -= offset;
	}
} 
