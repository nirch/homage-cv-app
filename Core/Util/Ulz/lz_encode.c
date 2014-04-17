/************************** lz_compress.C *************************
 *
 * This is a variable 9 - 12 bit LZW program. 
 * It uses a variable size code, and flushs the dictionary after it fills up.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "Ubst/BstType.h"
#include "Ucst/CstType.h"

#include "lz.h"

#define	LZ_ENC_BITS_NUM	 12

#define MAX_CODE					( ( 1 << LZ_ENC_BITS_NUM ) - 1 )
#define TABLE_SIZE					5021 //must be a prime larger than 2^LZ_ENC_BITS_NUM !!!

#define END_OF_STREAM				256
#define BUMP_CODE					257
#define FLUSH_DICT					258
#define FIRST_CODE					259
#define UNUSED						-1


static void initializeDictionary();



static unsigned int	find_child_node( int parent_code, int child_character );



static dictionary dict[TABLE_SIZE];
static int next_bump_code ;
static int current_code_bits ;// starts at table with 256 symbols - 9bits
static int next_code;
 

/*
 * The compressor is short and simple.  It reads in new symbols one
 * at a time from the input file.  It then  checks to see if the
 * combination of the current symbol and the current code are already
 * defined in the dictionary.  If they are not, they are added to the
 * dictionary, and we start over with a new one symbol code.  If they
 * are, the code for the combination of the code and character becomes
 * our new code.
 * if the dictionary has past current level of entries, code length increased by 1
 * if the dictionary is full - it is being flushed.
 */

bst_type *
lz_encoding( cst_type *cst )
{
	
int character;
int string_code;
unsigned int index;
unsigned int length;

bst_type *bst;





	if ( cst == NULL )	
		return NULL;

	length = cst->BYTE_NO;


//	if ( LZ_ENC_BITS_NUM < 9 )
//		LZ_ENC_BITS_NUM  = 9;
//	else if ( LZ_ENC_BITS_NUM > 12 )
//		LZ_ENC_BITS_NUM  = 12;



	bst = bst_alloc(4*length );
	//bst_put_int(bst,LZ_ENC_BITS_NUM,sizeof(int));//type of encoding    	
    
	
	// init the table
	initializeDictionary();


	string_code = (int)(CST_GET_UCHAR(cst));
	if ( string_code == EOF )
        string_code = END_OF_STREAM;

	character = CST_GET_UCHAR(cst);

	while ( --length ) 
	{		
		index = find_child_node( string_code, character );
		if ( dict[ index ].code_value != -1 )
			string_code = dict[ index ].code_value;
		else {
			//if ( next_code <= MAX_CODE ) 
			//{
			
			dict[ index ].code_value = next_code++;
			dict[ index ].parent_code = string_code;
			dict[ index ].character = (char) character;						
			bst_put_int( bst, string_code, current_code_bits );		
			string_code = character;
			//}
			if ( next_code > MAX_CODE )
			{// dictionary is full - flush it and start from init dictionary
				bst_put_int( bst, FLUSH_DICT, current_code_bits );//flush the dictionary
				initializeDictionary();

			}
			else if ( next_code > next_bump_code )
			{// passed max entries for current code bits, and has to increment it by 1
				bst_put_int( bst, BUMP_CODE, current_code_bits );
				current_code_bits++;
				next_bump_code <<= 1;
				next_bump_code |= 1;
			}
			
		}

		character = CST_GET_UCHAR(cst);		
	}	
	bst_put_int(bst, string_code, current_code_bits );		
	bst_put_int(bst, END_OF_STREAM, current_code_bits );  
	return( bst );
}



static unsigned int
find_child_node( int parent_code, int child_character )
{
int index;
int offset;
 
	index = ( child_character << ( LZ_ENC_BITS_NUM - 8 ) ) ^ parent_code;

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


static void initializeDictionary()
{
	int i;
	for ( i = 0 ; i < TABLE_SIZE ; i++ )
		dict[ i ].code_value = UNUSED;// the algorithm knows automatically that cells 0-255 are used	
	next_code = FIRST_CODE; 
	next_bump_code = 511;
	current_code_bits = 9;// starts at table with 256 symbols - 9bits
}
