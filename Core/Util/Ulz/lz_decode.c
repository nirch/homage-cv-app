/************************** lz_expand.c *************************
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

#define	LZ_ENC_BITS_NUM	 12

#define MAX_CODE					( ( 1 << LZ_ENC_BITS_NUM ) - 1 )
#define TABLE_SIZE					5021 //must be a prime larger than 2^LZ_ENC_BITS_NUM !!!

#define END_OF_STREAM				256
#define BUMP_CODE					257
#define FLUSH_DICT					258
#define FIRST_CODE					259
#define UNUSED						-1



static void initializeDictionary( dictionary dict[],
								 int *current_code_bits,int *next_code );

static u_int	decode_string( dictionary dict[],
							  char decode_stack[], u_int count, u_int code );








/*
 * The file expander operates much like the encoder.  It has to
 * read in codes, the convert the codes to a string of characters.
 * The only catch in the whole operation occurs when the encoder
 * encounters a CHAR+STRING+CHAR+STRING+CHAR sequence.  When this
 * occurs, the encoder outputs a code that is not presently defined
 * in the table.  This is handled as an exception.
 */
cst_type *
lz_decoding( bst_type *bst)
{
dictionary dict[TABLE_SIZE];
char	decode_stack[1024];
unsigned int new_code;
unsigned int old_code;
	
int character;
unsigned int count;


int current_code_bits ;// starts at table with 256 symbols - 9bits
int next_code;



#ifdef _DEBUG
	u_char *data;
	
	int point;
#endif // _DEBUG



	cst_type *cst = cst_alloc( 20*bst->BYTE_NO );	



	bst_rewind(bst);
	


/*
** This code checks for the CHARACTER+STRING+CHARACTER+STRING+CHARACTER
** case which generates an undefined code.  It handles it by decoding
** the last code, and adding a single character to the end of the decode string.
*/

	while (TRUE)
	{

		initializeDictionary( dict, &current_code_bits, &next_code );

		old_code = (unsigned int) bst_get_int(bst, current_code_bits);    
		if ( old_code == END_OF_STREAM )	
			return cst;

		character = old_code;
		CST_PUT_CHAR( cst, old_code );
		while ( TRUE )
		{
			new_code = (u_int) bst_get_int(bst, current_code_bits);
			if ( new_code == END_OF_STREAM )
				return cst;
			if ( new_code == FLUSH_DICT )
				break;
			if ( new_code == BUMP_CODE ) 
			{
				current_code_bits++;            
				continue;
			}
			if ( new_code >= (u_int)next_code ) {
				decode_stack[ 0 ] = (char) character;
				count = decode_string( dict, decode_stack, 1, old_code );
			}
			else
				count = decode_string( dict, decode_stack, 0, new_code );

			character = decode_stack[ count - 1 ];
			
	#ifdef _DEBUG		
			//extend
			
//			if ( cst->BYTE_NO - cst->byte_no < (int)count )
			if ( CST_BYTE_AVAILABLE(cst) < (int)count )
			{
				warning("eyal","decoding: cst extend");
				point = cst->rp - cst->data;
				data = cst->data;
				cst_realloc(cst,cst->BYTE_NO *2);
				memcpy(cst->data,data,point);
//				cst->byte_no = point;
				cst->rp = cst->data;
				cst->wp = cst->data + point;
			}
	#endif // _DEBUG		
			
			while ( count > 0 )
				CST_PUT_CHAR( cst, decode_stack[ --count ] );            

			dict[ next_code ].parent_code = old_code;
			dict[ next_code ].character = (char) character;
			next_code++;
			
			old_code = new_code;
		}// inner while true
	}//while true
	return cst;
}



/*
 * This routine decodes a string from the dictionary, and stores it
 * in the decode_stack data structure.  It returns a count to the
 * calling program of how many characters were placed in the stack.
 */
 
static u_int
decode_string( dictionary dict[], char decode_stack[], u_int count, u_int code )
{
	while ( code > 255 ) {
		decode_stack[ count++ ] = dict[ code ].character;
		code = dict[ code ].parent_code;
	}

	decode_stack[ count++ ] = (char) code;

	return( count );
} 



static void initializeDictionary( dictionary dict[],
								 int *current_code_bits,int *next_code )
{
	int i;
	for ( i = 0 ; i < TABLE_SIZE ; i++ )
		dict[ i ].code_value = UNUSED;// the algorithm knows automatically that cells 0-255 are used	
	*next_code = FIRST_CODE; 
	*current_code_bits = 9;// starts at table with 256 symbols - 9bits
}

