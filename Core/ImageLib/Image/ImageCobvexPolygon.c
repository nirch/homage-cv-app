#include	<string.h>
#include	<stdio.h>
#include	<math.h>


#include	"Uigp/igp.h"

#include	"uvec/Vec2D.h"
#include	"ImageType/ImageType.h"


void	image_covex_polygon_draw( image_type *im, int color0, int color1, 
			   float x0, float y0, vec2d v0[], int nV, float T );


image_type *
image_covex_polygon( int width, int height, int color0, int color1, 
			   float x0, float y0, vec2d v0[], int nV, float T )
{
image_type	*im;

	im = image_create( height, width, 4, 1, NULL );

	image4_const( im, color0 );


	image_covex_polygon_draw( im, color0, color1, x0, y0, v0, nV, T );

	return( im );
}


void
image_covex_polygon_draw( image_type *im, int color0, int color1, 
			   float x0, float y0, vec2d v0[], int nV, float T )
{
vec2d	v[10],	u[10],	p;
float	t,	tmp;
int	i,	j,	k;
int	r,	g,	b,	r0,	g0,	b0,	r1,	g1,	b1;
u_int	*tp;




	for( i = 0 ; i < nV ; i++ ){
		v[i].x = v0[i].x + x0;
		v[i].y = v0[i].y + y0;
	}
	v[nV] = v[0];

	for( i = 0 ; i < nV ; i++ ){
		u[i].x =  -(v[i+1].y - v[i].y);
		u[i].y = (v[i+1].x - v[i].x);

		t = hypot( u[i].x, u[i].y );

		u[i].x /= t;
		u[i].y /= t;
	}


	r0 = IMAGE4_RED( color0 );
	g0 = IMAGE4_GREEN( color0 );
	b0 = IMAGE4_BLUE( color0 );

	r1 = IMAGE4_RED( color1 );
	g1 = IMAGE4_GREEN( color1 );
	b1 = IMAGE4_BLUE( color1 );

	tp = (u_int *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0; j < im->column ; j++, tp++ ){
			
			t = 10;
			for( k = 0 ; k < nV ; k++ ){
				p.x = (j+0.5) - v[k].x;
				p.y = (i+0.5) - v[k].y;

				tmp = u[k].x * p.x + u[k].y*p.y;

				if( tmp < t )	t = tmp;
			}

			if( t < -T ){
				*tp = color0;
				continue;
			}

			if( t > 0 ){
				*tp = color1;
				continue;
			}

			t /= T;

			r = r0 * ( 1 - t ) + t *r1;
			g = g0 * ( 1 - t ) + t *g1; 
			b = b0 * ( 1 - t ) + t *b1;

			*tp = IMAGE4_RGB( r, g, b );
		}
			
}
