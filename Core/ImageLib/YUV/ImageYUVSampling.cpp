
#include "Image2YUV.h"


void	imageYUV_b_sampling2( u_char *sim, int sw, int sh, u_char *im, int w, int h );

void	imageY_b_sampling2( u_char *sim, int sw, int sh,  u_char *im, int w, int h );



image_type *
imageYUV_sampling2( image_type *sim, image_type *im )
{


	if( im != NULL )
		image_destroy( im, 1 );

	im = image_createYUV420( sim->height/2, sim->width/2, NULL );


	imageYUV_b_sampling2( sim->data, sim->width, sim->height, im->data, im->width, im->height );


	return( im );
}


void
imageYUV_b_sampling2( u_char *sim, int sw, int sh, u_char *im, int w, int h )
{
int	sd,	d;
	
	sd = sw*sh;	
	d = w*h;

	imageY_b_sampling2( sim, sw, sh,  im, w, h );

	imageY_b_sampling2( sim+sd, sw/2, sh/2, im+d, w/2, h/2 );

	imageY_b_sampling2( sim+ sd + sd/4, sw/2, sh/2, im + d + d/4, w/2, h/2 );


}

void
imageY_b_sampling2( u_char *sim, int sw, int sh,  u_char *im, int w, int h )
{
u_char	*sp,	*sp1,	*tp;
int	i,	j;

	tp = im;
	for( i = 0 ; i < h ; i++, sim += 2*sw ){
		sp = sim;
		sp1 = sp + sw;

		for( j = 0 ; j < w ; j++, sp += 2, sp1 += 2 ){
			*tp++ = ( *sp + *(sp+1) + *sp1 + *(sp1+1) ) >> 2;
		}
	}
}