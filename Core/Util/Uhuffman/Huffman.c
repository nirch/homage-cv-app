
#include "Huffman.h"
#include "HuffmanIn.h"


static void		huffman_build_codeR( huffman_type *h,
									unsigned int code_so_far,
									int bits,
									int node);



huffman_type *
huffman_alloc( int msg_bit, int cont_bit )
{
huffman_type *h;

	h = ( huffman_type *)malloc( sizeof( huffman_type) );

	h->msg_bit = cont_bit;

	h->cont_bit = cont_bit;
	h->count_max = ( 1 << h->cont_bit );

	h->nMsg = ( 1 << h->msg_bit);

	h->eos = ( 1 << h->msg_bit);	// 256;


	h->nNode = h->nMsg * 2 + 2;


	h->node = ( NODE *)malloc( h->nNode * sizeof( NODE) );

	return( h );
}

void
huffman_destroy( huffman_type *h )
{
	free( h->node );

	free( h );
}


// use only by encoder
void huffman_set_counts( huffman_type *h, int counts[] )
{
int	max_count;
int i;

    max_count = 0;
    for ( i = 0 ; i < h->nMsg ; i++ )
       if ( counts[ i ] > max_count )
			max_count = counts[ i ];

    if ( max_count == 0 ) {
		counts[ 0 ] = 1;
		max_count = 1;
    }


    max_count = max_count / (h->count_max-1) + 1;
 //   max_count = max_count + 1;


	// ensure that the max value of count will be h->cont_max==256
    for ( i = 0 ; i < h->nMsg ; i++ ){
		h->node[ i ].count = (unsigned int) ( counts[ i ] / max_count );
		h->node[ i ].saved_count = 0;
		if ( h->node[ i ].count == 0 && counts[ i ] != 0 )
			h->node[ i ].count = 1;
    }

    h->node[ h->eos ].count = 1;
}

int 
huffman_build_tree( huffman_type *h )
{
int next_free;
int i;
int min_1;
int min_2;
int	lasNode;

	lasNode = h->nNode-1;

    h->node[ lasNode ].count = 0xffff;//h->count_max;
    for ( next_free = h->eos + 1 ; ; next_free++ ){
		min_1 = lasNode;
		min_2 = lasNode;

		for ( i = 0 ; i < next_free ; i++ ){

			if ( h->node[ i ].count == 0 )	continue;

			if ( h->node[ i ].count < h->node[ min_1 ].count ) {
				min_2 = min_1;
				min_1 = i;
			} else if ( h->node[ i ].count < h->node[ min_2 ].count )
				min_2 = i;

		}

		if ( min_2 == lasNode )
			break;

		h->node[ next_free ].count = h->node[ min_1 ].count
	                           + h->node[ min_2 ].count;
        h->node[ min_1 ].saved_count = h->node[ min_1 ].count;
        h->node[ min_1 ].count = 0;
        h->node[ min_2 ].saved_count =  h->node[ min_2 ].count;
        h->node[ min_2 ].count = 0;
		h->node[ next_free ].child_0 = min_1;
		h->node[ next_free ].child_1 = min_2;
    }

    next_free--;
    h->node[ next_free ].saved_count = h->node[ next_free ].count;

	h->root_node = next_free;

    return( next_free );
}



// use onlyby encoder 
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
