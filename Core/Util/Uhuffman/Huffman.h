/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef		_HUFFMAN_H
#define		_HUFFMAN_H

#ifdef __cplusplus
extern "C" {
#endif


#include	"Ubst/BstType.h"



typedef struct huffman_type {
	int	msg_bit;
	int	nMsg;

	int	cont_bit;
	int count_max;


	int	eos;	// code for end of stream;

	int	root_node;	// set by build tree

	int	nNode;
	struct NODE	*node;

}	huffman_type;



huffman_type *	huffman_alloc( int msg_bit, int cont_bit );

void	huffman_destroy( huffman_type *h );


void	huffman_set_counts( huffman_type *h, int counts[] );

int		huffman_build_tree( huffman_type *h );




// HuffmanGet.c
int		huffman_get( huffman_type *h, bst_type *bst );

void	huffman_get_counts( huffman_type *h, bst_type *bst );



// HuffmanPut.c
void	huffman_put( huffman_type *h, int msg, bst_type *bst );

void	huffman_put_counts( huffman_type *h, bst_type *bst );

void	huffman_build_code( huffman_type *h );


#ifdef __cplusplus
}
#endif

#endif

