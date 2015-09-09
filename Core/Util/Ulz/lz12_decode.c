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

#define BITS                        12
#define MAX_CODE                   ( ( 1 << BITS ) - 1 )
#define TABLE_SIZE                 5021 
#define END_OF_STREAM              256
#define FIRST_CODE                 257
#define UNUSED                     -1



static u_int	decode_string( dictionary dict[], char decode_stack[], u_int count, u_int code );

//dictionary dict[TABLE_SIZE];






/*
 * The file expander operates much like the encoder.  It has to
 * read in codes, the convert the codes to a string of characters.
 * The only catch in the whole operation occurs when the encoder
 * encounters a CHAR+STRING+CHAR+STRING+CHAR sequence.  When this
 * occurs, the encoder outputs a code that is not presently defined
 * in the table.  This is handled as an exception.
 */
cst_type *
lz12_decoding( bst_type *bst, int size )
{
dictionary *dict;
char	decode_stack[1024];
cst_type *cst;
int	i;

unsigned int next_code;
unsigned int new_code;
unsigned int old_code;
	
int character;
unsigned int count;

	dict = ( dictionary *)malloc( sizeof(dictionary) * TABLE_SIZE );

	if( size <= 0 )	size = 10*bst->BYTE_NO;
	
	cst = cst_alloc( size );
// start at buflength, and increment by this size each time....
	
	bst_rewind(bst);

	/* init the dictionary */
	for ( i = 0 ; i < TABLE_SIZE ; i++ )
		dict[ i ].code_value = UNUSED;
	next_code = FIRST_CODE;


	old_code = (unsigned int) bst_get_int( bst, BITS );    
	if ( old_code == END_OF_STREAM )	return cst;

	character = old_code;
	CST_PUT_CHAR( cst, old_code );


/*
** This code checks for the CHARACTER+STRING+CHARACTER+STRING+CHARACTER
** case which generates an undefined code.  It handles it by decoding
** the last code, and adding a single character to the end of the decode string.
*/


    while ( ( new_code = (u_int) bst_get_int( bst, BITS ) ) != END_OF_STREAM ){

		if ( new_code >= next_code ) {
			decode_stack[ 0 ] = (char) character;
			count = decode_string( dict, decode_stack, 1, old_code );
		}
		else
			count = decode_string( dict, decode_stack, 0, new_code );

		character = decode_stack[ count - 1 ];




		while ( count > 0 )
			CST_PUT_CHAR( cst, decode_stack[ --count ] );            

		if ( next_code <= MAX_CODE ){
			dict[ next_code ].parent_code = old_code;
			dict[ next_code ].character = (char) character;
			next_code++;
		}

		old_code = new_code;
	}

	free( dict );
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

