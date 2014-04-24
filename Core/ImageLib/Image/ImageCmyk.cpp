
#include	<stdio.h>


#include	"Uigp/igp.h"

#include	"ImageType/ImageType.h"



image_type *
image3_to_cmyk( image_type *sim, image_type *im )
{
	int	i,	j;
	int	r,	g,	b;

	im = image_realloc( im, sim->width, sim->height, 4, IMAGE_TYPE_U8, 1 );

	u_char *sp = sim->data;
	u_char *tp = im->data;

	for( i = 0; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, tp += 4  ){
			r = *sp++;
			g = *sp++;
			b = *sp++;


			tp[0] = 255 - r;
			tp[1] = 255 - g;
			tp[2] = 255 - b;
			tp[3] = 0;//255;	// K

			
			//k = 255 - sp[3];
			//r = (k*(255-pp[0]))/255;
			//g = (k*(255-pp[1]))/255;
			//b = (k*(255-pp[2]))/255;
			//*cp++ = PACK(r, g, b);
			//pp += samplesperpixel);
		}
	}

	return( im );
}




image_type *
image_cmyk2rgb( image_type *sim, image_type *im )
{
	int	i,	j;
	int	r,	g,	b;
	int	c,	m,	y,	k;

	im = image_realloc( im, sim->width, sim->height, 4, IMAGE_TYPE_U8, 1 );

	u_char *sp = sim->data;
	u_int *tp = im->data_ui;

	for( i = 0; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++  ){
			c = *sp++;
			m = *sp++;
			y = *sp++;
			k = *sp++;


			r = 255 - c;
			g = 255 - m;
			b = 255 - y;
			if( k == 255 )	
				r = g = b = 0;

			
			*tp++ = IMAGE4_RGB( r, g, b );
		}
	}

	return( im );
}