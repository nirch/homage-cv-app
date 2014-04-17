/************************** LZW12.C *************************
 *
 * This is 12 bit LZW program. 
 * It uses a fixed size code, and does not attempt to flush the dictionary after it fills up.
 */

#ifndef	__LZ15_H__
#define __LZ15_H__
#include "Ubst/BstType.h"
#include "Ucst/CstType.h"


 /*
 * BITS defines how many bits will be in a code.  
 * TABLE_SIZE defines the size of the dictionary table.
 */
 



/*
 * This data structure defines the dictionary.  Each entry in the dictionary
 * has a code value.  This is the code emitted by the compressor.  Each
 * code is actually made up of two pieces:  a parent_code, and a
 * character.  Code values of less than 256 are actually plain
 * text codes.
 */

typedef struct dictionary {
    int code_value;
    int parent_code;
    char character;
} dictionary;




/*	LzEncoding.c	*/
bst_type *	lz_encoding( cst_type *cst );


/*	LzDecoding.c	*/
cst_type *	lz_decoding( bst_type *bst );


/*	Lz12Encoding.c	*/
bst_type *	lz12_encoding( cst_type *cst );


/*	Lz12Decoding.c	*/
cst_type *	lz12_decoding( bst_type *bst, int size );



#endif//	__LZ15_H__

