 /*********************************
 ***	Image1FisherMatrix.c	***
 *********************************/
#include	"Ubox/Box2d.h"
#include	"Umath/Matrix2Type.h"
#include	"ImageType/ImageType.h"
#include	"Image1Tool.h"




float
image_fisher_info( image_type *sim, box2i *b )
{
image_type	*im;
float	info;

	im = image1_from( sim, NULL );


	info = image1_fisher_info( im, b );

	if( im != sim )
		image_destroy( im, 1 );

	return( info );
}



float	
image1_fisher_info( image_type *sim, box2i *b )
{
matrix2_type m;

	image1_fisher_matrix( sim,  b, &m );

	return( m.a00 *m.a11 - m.a01*m.a10 );
}


void
image1_fisher_matrix( image_type *sim, box2i *b, matrix2_type *m )
{
u_char	*sp,	*sp0,	*sp1;
int	i,	j,	 n;
int	cx,	cy;
float	Ixx,	Ixy,	Iyy,	t;

int	i0,	i1,	j0,	j1;
	
	if( b == NULL ){
		i0 = 1;
		i1 = sim->height-1;
		j0 = 1;
		j1 = sim->width-1;
	}
	else	{
		i0 = (b->x0 < 0 )? b->x0 : 1;
		i1 = (b->x1 < sim->height-1)? b->x1 : sim->height;
		j0 = (b->y0 < 0 )? b->y0 : 1;
		j1 = (b->y1 < sim->width-1)? b->y1 : sim->width;
	}



	Ixx = Ixy = Iyy = 0;

	for( i = i0, n = 0 ; i <i1 ; i++ ){

		sp = IMAGE_PIXEL( sim, i, j0 );

		sp0 = sp - sim->column;
		sp1 = sp + sim->column;
		for( j = j0 ; j < j1 ; j++, sp++, sp0++, sp1++ ){

			cx = ((*(sp0+1) - *(sp0-1)) + 2*(*(sp +1) - *(sp -1)) + (*(sp1+1) - *(sp1-1)));
			cy = ((*(sp1-1) - *(sp0-1)) + 2*(*(sp1+0) - *(sp0+0)) + (*(sp1+1) - *(sp0+1)));


			Ixx  += cx * cx;
			Ixy += cx * cy;
			Iyy  += cy * cy;
			n++;
		}
	}


	t = 1.0/ (64*n);
	m->a00 = Ixx * t;
	m->a01 = m->a10 = Ixy * t;
	m->a11 = Iyy * t;
}



