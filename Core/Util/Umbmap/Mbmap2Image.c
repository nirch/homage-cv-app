
#include	"Umbmap/mbmap.h"
#include	"Uimage/ImageType.h"



int
mbmap_from_image( image_type *im, mbmap bmap[], int flag )
{
u_char	*r,	*g,	*b;
u_int	*p;
int	i,	j;

	if( flag ){
		MBMAP_CREATE(bmap[0], im->row, im->column );
		MBMAP_CREATE(bmap[1], im->row, im->column );
		MBMAP_CREATE(bmap[2], im->row, im->column );
	}
	else	{
		MBMAP_RECREATE(bmap[0], im->row, im->column );
		MBMAP_RECREATE(bmap[1], im->row, im->column );
		MBMAP_RECREATE(bmap[2], im->row, im->column );
	}

	r = MBMAP_DATA_ADRESS( bmap[0] );
	g = MBMAP_DATA_ADRESS( bmap[1] );
	b = MBMAP_DATA_ADRESS( bmap[2] );

	p = (u_int *)im->data;
	for( i = 0 ; i < im->row ; i++, p += im->pad )
		for( j = 0 ; j < im->column ; j++ ){
			*b++ = IMAGE4_BLUE( *p );
			*g++ = IMAGE4_GREEN( *p );
			*r++ = IMAGE4_RED( *p );
			p++;
		}

	return( 4 );
}



image_type *
mbmap_to_image( mbmap bmap[] )
{
u_char	*r,	*g,	*b;
u_int	*p;
int	i,	j;
int	row,	column;
image_type	*im;

	r = MBMAP_DATA_ADRESS( bmap[0] );
	g = MBMAP_DATA_ADRESS( bmap[1] );
	b = MBMAP_DATA_ADRESS( bmap[2] );

	row = MBMAP_ROW(bmap[0]);
	column = MBMAP_COLUMN(bmap[0]);

	im = image_create( row, column, 4, 1, NULL );

	p = (u_int *)im->data;
	for( i = 0 ; i < im->row ; i++, p += im->pad )
		for( j = 0 ; j < im->column ; j++ ){

			*p++ = IMAGE4_RGB( *r, *g, *b );
			r++;
			g++;
			b++;
		}

	return( im );
}
