/************************
 ***   HuffmanGet.c   ***
 ************************/

#include "Huffman.h"

#include "HuffmanIn.h"




int
huffman_get( huffman_type *h, bst_type *bst )
{
int	node;

	node = h->root_node;

	do {
		if( bst_get( bst ) )
			node = h->node[ node ].child_1;
		else
			node = h->node[ node ].child_0;

	} while ( node > h->eos );


	return( node );
}


void
huffman_get_counts( huffman_type *h, bst_type *bst )
{
int	first;
int	last;
int	i;

	for ( i = 0 ; i < h->nMsg ; i++ )
		h->node[ i ].count = 0;



	first = bst_get_int(bst,h->msg_bit);
	last = bst_get_int(bst,h->msg_bit);

	for ( ; ; ) {

		for ( i = first ; i <= last ; i++ )
			h->node[ i ].count = (unsigned int) bst_get_int( bst, h->cont_bit );

		if ( ( first = bst_get_int( bst, h->msg_bit ) ) == 0 )
			break;


		last = bst_get_int( bst, h->msg_bit );
	}

	h->node[ h->eos ].count = 1;
}
