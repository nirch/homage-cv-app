#ifndef		_HUFFMAN_L_H
#define		_HUFFMAN_L_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include	"Uigp/igp.h"
//#include	"Ufbst/FbstType.h"
#include	"Huffman.h"


typedef struct NODE {
    unsigned int count;			
    unsigned int saved_count;	
    int child_0;				
    int child_1;
	
	
	unsigned int code;	
	int code_bits;
	
} NODE;







/*
typedef struct code1 {
    unsigned int code;	
    int code_bits;		
} CODE;


#define END_OF_STREAM 256



	// Huffman.c
int huff_build_tree( NODE *nodes );


	// HuffmanAux.c
void	huff_print_model( NODE *nodes, CODE *codes );


	// HuffmanEncode.c
void	huff_convert_tree_to_code( NODE *nodes,
                           CODE *codes,
                           unsigned int code_so_far,
                           int bits,
                           int node );
void	huff_output_counts( fbst_type *obst, NODE *nodes );
void	huff_scale_counts( unsigned long *long_counts, NODE *nodes );
void	huff_output_counts_cst( bst_type *obst, NODE *nodes );


	// HuffmanDecode.c
void	huff_input_counts( bst_type *ibst, NODE *nodes );
*/

#endif
