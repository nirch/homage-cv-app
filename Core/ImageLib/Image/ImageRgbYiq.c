

#include	"Uigp/igp.h"
#include	"ImageType/ImageType.h"








image_type *
image_Rgb_Yiq( image_type *im, int Falloc )
{
image_type	*tim;
u_int	*sp,	*tp;
int	row,	col;
int	r,	g,	b;
int	y,	q,	a;
int	i,	j;



	row = IMAGE_ROW(im);
	col = IMAGE_COLUMN(im);


	if( Falloc == 1 )
		tim = image_create( row, col, 4, 1, NULL );
	else	tim = im;


	sp = IMAGE4_DATA( im );
	tp = IMAGE4_DATA( tim );



	for( i = 0 ; i < row ; i++ ){
		for( j = 0 ; j < col ; j++ ){

			r = IMAGE4_RED( *sp );
			g = IMAGE4_GREEN( *sp );
			b = IMAGE4_BLUE( *sp );

			sp++;


			y = 0.3333*( r + g+ b );

			a = 0.25*( 2*r - g- b ) + 128;

			q = 0.5*( g- b ) + 128;

			*tp = IMAGE4_RGB( y, a, q );

			tp++;
		}
	}

	return( tim );
}
