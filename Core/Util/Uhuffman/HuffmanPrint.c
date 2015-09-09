/***************************
 ***   HuffmanDecode.c   ***
 ***************************/

#include	"HuffmanIn.h" 

static void	print_char( int c );

void
huffman_print_count( huffman_type *h, FILE *fp )
{
int	i;
	fprintf( fp, "%d\n\n", h->nMsg );
	
	for( i = 0 ; i < h->nMsg ; i++ )
		fprintf( fp, "%d\n", h->node[i].saved_count );
}


void huffman_print_model( huffman_type *h )
{
    int i;

    for ( i = 0 ; i < 513 ; i++ ){
		if ( h->node[ i ].saved_count != 0 ) {
			printf( "node=" );
			print_char( i );
			printf( "  count=%3d", h->node[ i ].saved_count );
			printf( "  child_0=" );
			print_char( h->node[ i ].child_0 );
			printf( "  child_1=" );
			print_char( h->node[ i ].child_1 );

			if( i <= h->nMsg ) {
				printf( "  Huffman code=" );
					gp_fwrite_bits( stdout, h->node[ i ].code, h->node[ i ].code_bits );
			}

			printf( "\n" );
		}
    }
}

static void
print_char( int c )
{
    if ( c >= 0x20 && c < 127 )
        printf( "'%c'", c );
    else
        printf( "%3d", c );
}

