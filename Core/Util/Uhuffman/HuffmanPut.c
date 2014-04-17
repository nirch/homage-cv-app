/************************
 ***   HuffmanPut.c   ***
 ************************/

#include "Huffman.h"

#include "HuffmanIn.h"




void
huffman_put( huffman_type *h, int msg, bst_type *bst )
{
	bst_put_int( bst, h->node[msg].code, h->node[msg].code_bits );
}



void 
huffman_put_counts( huffman_type *h, bst_type *bst )
{
int	first;
int	last;
int	next;
int	i;

	first = 0;
	while ( first < h->nMsg-1 && h->node[ first ].saved_count == 0 )
		first++;
/*
 * Each time I hit the start of the loop, I assume that first is the
 * number for a run of non-zero values.  The rest of the loop is
 * concerned with finding the value for last, which is the end of the
 * run, and the value of next, which is the start of the next run.
 * At the end of the loop, I assign next to first, so it starts in on
 * the next run.
 */
	for ( ; first < h->nMsg ; first = next ) {
		last = first + 1;
		for ( ; ; ) {
			for ( ; last < h->nMsg ; last++ )
				if ( h->node[ last ].saved_count == 0 )	break;

			last--;
			for ( next = last + 1; next < h->nMsg ; next++ )
				if ( h->node[ next ].saved_count != 0 )	break;

			if ( next > h->nMsg-1 )	break;
			if ( ( next - last ) > 3 )	break;
			last = next;
		};
/*
 * Here is where I output first, last, and all the counts in between.
 */
		bst_put_int( bst, first, h->msg_bit );
		bst_put_int( bst, last, h->msg_bit );

		for ( i = first ; i <= last ; i++ ) 
			bst_put_int( bst, h->node[ i ].saved_count, h->cont_bit );
	}

	bst_put_int( bst, 0, h->msg_bit );
}



/*
static void		huffman_build_codeR( huffman_type *h,
									unsigned int code_so_far,
									int bits,
									int node);

void 
huffman_build_code( huffman_type *h )
{
	huffman_build_codeR( h, 0, 0, h->root_node );
}



static void 
huffman_build_codeR( huffman_type *h,
					unsigned int code_so_far,
					int bits,
					int node)
{

	if ( node <= h->eos ){
		h->node[ node ].code = code_so_far;
		h->node[ node ].code_bits = bits;
		return;
	}



	huffman_build_codeR( h, code_so_far, bits+1,
                          h->node[ node ].child_0 );

	huffman_build_codeR( h, code_so_far | (1L<<bits),
                          bits+1, h->node[ node ].child_1 );
}
*/