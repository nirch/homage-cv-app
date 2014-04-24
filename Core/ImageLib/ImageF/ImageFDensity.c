
#include	<math.h>
#include "Ucamera/Pt2dType.h"

#include	"ImageType/ImageType.h"
#include	"ImageF/ImageFTool.h"




static float	pt2dA_density(pt2dA_type *apt, float h, vec2f_type *p );






static void	imageF_density_block( image_type *im, int x0, int y0, int D, pt2dA_type *apt, float h );


static void	imageFM_density_block( image_type *im, image_type *mim, int x0, int y0, int D, pt2dA_type *apt, float h );



image_type *
imageF_density( int width, int height, pt2dA_type *apt, float h, image_type *im )
{
int	i,	j,	D;

	D = 16;


	im = image_alloc( width, height, 1, IMAGE_TYPE_F, 1 );


	for( i = 0 ; i < im->row ; i += D ){
		for( j = 0 ; j < im->column ; j += D ){


			imageF_density_block( im, j, i, D, apt, h );
		}
	}


	return( im );
}




static void
imageF_density_block( image_type *im, int x0, int y0, int D, pt2dA_type *apt, float h )
{
float	*tp;
float	tmp;
int	i,	j,	x1,	y1;
vec2f_type	p;
pt2dA_type *capt;
box2i box;


	box.x0 = x0 - h - 1;
	box.x1 = x0 + D + h + 1;
	box.y0 = y0 - h - 1;
	box.y1 = y0 + D + h + 1;


	capt = pt2dA_copy_box( apt, &box, NULL );

	if( (y1 = y0 + D) > im->height )	y1 = im->height;
	if( (x1 = x0 + D) > im->width )	x1 = im->width;

	for( i = y0 ; i < y1 ; i++ ){
		tp = IMAGEF_PIXEL( im, i, x0 );
		p.y = i + 0.5;
		for( j = x0 ; j < x1 ; j++, tp++ ){
			p.x = j + 0.5;


			tmp = pt2dA_density( capt, h, &p );

			*tp = tmp;
		}
	}


	pt2dA_destroy( capt );
}



image_type *
imageFM_density( image_type *mim, pt2dA_type *apt, float h, image_type *im )
{
	int	i,	j,	D;

	D = 16;


	im = image_alloc( mim->width, mim->height, 1, IMAGE_TYPE_F, 1 );


	for( i = 0 ; i < im->row ; i += D ){
		for( j = 0 ; j < im->column ; j += D ){


			imageFM_density_block( im, mim, j, i, D, apt, h );
		}
	}


	return( im );
}




static void
imageFM_density_block( image_type *im, image_type *mim, int x0, int y0, int D, pt2dA_type *apt, float h )
{
u_char	*mp;
float	*tp;
float	tmp;
int	i,	j,	x1,	y1;
vec2f_type	p;
pt2dA_type *capt;
box2i box;
float	af[256];


	box.x0 = x0 - h - 1;
	box.x1 = x0 + D + h + 1;
	box.y0 = y0 - h - 1;
	box.y1 = y0 + D + h + 1;
	capt = pt2dA_copy_box( apt, &box, NULL );



	for( i = 0 ; i < 256 ; i++ )
		af[i] = 255.0/i;

	if( (y1 = y0 + D) > im->height )	y1 = im->height;
	if( (x1 = x0 + D) > im->width )	x1 = im->width;

	for( i = y0 ; i < y1 ; i++ ){
		mp = IMAGE_PIXEL( mim, i, x0 );

		tp = IMAGEF_PIXEL( im, i, x0 );

		p.y = i + 0.5;
		for( j = x0 ; j < x1 ; j++, tp++, mp++ ){
			if( *mp == 0 ){
				*tp = 0;
				continue;
			}

			p.x = j + 0.5;


			tmp = pt2dA_density( capt, h, &p );

			tmp *= af[*mp];

			*tp = tmp;
		}
	}


	pt2dA_destroy( capt );
}


static float 
pt2dA_density(pt2dA_type *apt, float h, vec2f_type *p )
{
pt2d_type	*pt;
vec2f_type	dv;
float	d,	val;
int	i;


	for( i = 0, val = 0 ; i < apt->nP ; i++ ){
		pt = &apt->p[i];

		dv.x = p->x - pt->p.x;
		dv.y = p->y - pt->p.y;


		d = dv.x *dv.x + dv.y *dv.y ;
		if( d > h*h )	
			continue;

		val += 1 - d / (h*h);
	}


	return( val );

}