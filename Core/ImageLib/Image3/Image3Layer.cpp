/*****************************
 ***	ImageTransparent   ***
 *****************************/

#include	"ImageType/ImageType.h"

#include "Image3Tool.h"



void
image3_add_layer( image_type *im, image_type *sim, image_type *mim, int x0, int y0 )
{
	u_char	*sp,	*mp,	*tp;
	int	i,	j;
	int	w;

	tp = IMAGE_PIXEL( im, x0, y0 );
	int align = 3*(im->width - sim->width);

	sp = sim->data;
	mp = mim->data;


	for( i = 0 ; i < sim->height ; i++, tp += align ){
		for( j = 0 ; j < sim->width ; j++, mp++,sp += 3, tp += 3 ){

			if( *mp == 0 )	continue;


			if( *mp == 255 ){
				tp[0] = sp[0];
				tp[1] = sp[1];
				tp[2] = sp[2];
				continue;
			}


			w = *mp +1 ;


			tp[0] =  (( w * ( sp[0] - tp[0])) >> 8)  + tp[0];
			tp[1] =  (( w * ( sp[1] - tp[1])) >> 8)  + tp[1];
			tp[2] =  (( w * ( sp[2] - tp[2])) >> 8)  + tp[2];

		}
	}
}



void
image3_add_layerA( image_type *im, image_type *sim, int x0, int y0 )
{

	int	i,	j;
	int	w;

	u_char *tp = IMAGE_PIXEL( im, x0, y0 );
	int align = 3*(im->width - sim->width);

	u_int *sp = sim->data_ui;



	for( i = 0 ; i < sim->height ; i++, tp += align ){
		for( j = 0 ; j < sim->width ; j++, sp += 1, tp += 3 ){

			w = (*sp) >> 24;

			if( w == 0 )	continue;


			int R = IMAGE4_RED( *sp );
			int G = IMAGE4_GREEN( *sp );
			int B = IMAGE4_BLUE( *sp );

			if( w == 255 ){
				tp[0] = R;
				tp[1] = G;
				tp[2] = B;
				continue;
			}


			w += 1 ;


			tp[0] =  (( w * ( R - tp[0])) >> 8)  + tp[0];
			tp[1] =  (( w * ( G - tp[1])) >> 8)  + tp[1];
			tp[2] =  (( w * ( B - tp[2])) >> 8)  + tp[2];

		}
	}
}