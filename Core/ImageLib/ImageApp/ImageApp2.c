/***********************
 ***   ImageApp2.c   ***
 ***********************/
#include	<math.h>

#include	"ImageType/ImageType.h"

#include	"ImageApp2.h"

#include	"Umath/umath.h"
#include	"Ujet2/Jet2Type.h"
#include	"Ujet1/Jet1Type.h"






jet2App2_type *
jet2App2_init( int nx, int ny, int weight )
{
jet2App2_type	*ja;



	ja = (jet2App2_type *)malloc( sizeof(jet2App2_type) );

	jet1App_initA( &ja->x, nx, weight );
	jet1App_initA( &ja->y, ny, weight );


	return( ja );
}



void
jet2App2_destroy( jet2App2_type *ja )
{
	free( ja );
}




int	
image_jet2App2( image_type *im, int	i0,	int	j0, jet2App2_type *ja, jet2 *f )
{
	if( im->format == IMAGE_FORMAT_UC )
		return( image1_jet2App2( im, i0,	j0, ja, f ) );

	if( im->format == IMAGE_FORMAT_F )
		return( imageF_jet2App2( im,	i0,	j0, ja, f ) );

#ifdef _AA_
	if( im->format == IMAGE_FORMAT_US )
		return( imageUS_jet2App( im,	i0,	j0, ja, f ) );
#endif

	return( -1 );
}




int	
image1_jet2App2( image_type *im, int	i0,	int	j0, jet2App2_type *ja, jet2 *f )
{
u_char	*sp;
jet2	g;
float	s0,	s1,	s2,	t;
int	align,	i,	j;


	//if( ja->type == JAPP_WIGHT ){
	//	ret = image1_jet2AppW( im, i0,	j0, ja, f );
	//	return( ret );
	//}

	sp = IMAGE_PIXEL(im, i0, j0 );
	align = im->column - ja->x.n;


	JET2_ZERO( g );
	JET2_ZERO( *f );



	for( i = 0 ; i < ja->y.n ; i++, sp += align ){
		s0 = s1 = s2 = 0;
		for( j = 0 ; j < ja->x.n ; j++ ){
			t = *sp++;
			s0 += ja->x.a0[j] * t;
			s1 += ja->x.a1[j] * t;
			s2 += ja->x.a2[j] * t;
		}

		g.a00 += ja->y.a0[i] * s0;

		g.a10 += ja->y.a0[i] * s1;
		g.a01 += ja->y.a1[i] * s0;

		g.a20 += ja->y.a0[i] * s2;
		g.a11 += ja->y.a1[i] * s1;
		g.a02 += ja->y.a2[i] * s0;
	}



	f->a00 = g.a00 * ( ja->x.t0*ja->x.t0 );

	f->a10 = g.a10 * ( ja->x.t1*ja->y.t0 );
	f->a01 = g.a01 * ( ja->x.t0*ja->y.t1 );


	f->a20 = g.a20 * ( ja->x.t2*ja->y.t0 );
	f->a11 = g.a11 * ( ja->x.t1*ja->y.t1 );
	f->a02 = g.a02 * ( ja->x.t0*ja->y.t2 );

	//f->a00 -= f->a20 * ja->t02 *ja->t0* ja->t0 + f->a02 * ja->t02 *ja->t0* ja->t0;

	t = ( f->a20 * ja->x.t02 *ja->x.t0* ja->x.t0  + f->a02* ja->y.t02 *ja->y.t0* ja->y.t0 );
	f->a00 -= t;

	return( 1 );
}



int	
imageF_jet2App2( image_type *im, int i0, int j0, jet2App2_type *ja, jet2 *f )
{
	float	*sp;
	jet2	g;
	float	s0,	s1,	s2,	t;
	int	align,	i,	j;


	//if( ja->type == JAPP_WIGHT ){
	//	ret = image1_jet2AppW( im, i0,	j0, ja, f );
	//	return( ret );
	//}

	sp = (float *)IMAGE_PIXEL(im, i0, j0 );
	align = im->column - ja->x.n;


	JET2_ZERO( g );
	JET2_ZERO( *f );



	for( i = 0 ; i < ja->y.n ; i++, sp += align ){
		s0 = s1 = s2 = 0;
		for( j = 0 ; j < ja->x.n ; j++ ){
			t = *sp++;
			s0 += ja->x.a0[j] * t;
			s1 += ja->x.a1[j] * t;
			s2 += ja->x.a2[j] * t;
		}

		g.a00 += ja->y.a0[i] * s0;

		g.a10 += ja->y.a0[i] * s1;
		g.a01 += ja->y.a1[i] * s0;

		g.a20 += ja->y.a0[i] * s2;
		g.a11 += ja->y.a1[i] * s1;
		g.a02 += ja->y.a2[i] * s0;
	}



	f->a00 = g.a00 * ( ja->x.t0*ja->y.t0 );

	f->a10 = g.a10 * ( ja->x.t1*ja->y.t0 );
	f->a01 = g.a01 * ( ja->x.t0*ja->y.t1 );


	f->a20 = g.a20 * ( ja->x.t2*ja->y.t0 );
	f->a11 = g.a11 * ( ja->x.t1*ja->y.t1 );
	f->a02 = g.a02 * ( ja->x.t0*ja->y.t2 );

	//f->a00 -= f->a20 * ja->t02 *ja->t0* ja->t0 + f->a02 * ja->t02 *ja->t0* ja->t0;

	t = ( f->a20 * ja->x.t02 *ja->x.t0  + f->a02* ja->y.t02 *ja->y.t0 );
	f->a00 -= t;

	return( 1 );
}


#ifdef _AA_
int	imageF_jet2App( image_type *im, int	i0,	int	j0, jet2App_type *ja, jet2 *f )
{
float	*sp;
jet2	g;
float	s0,	s1,	s2,	t;
int	align,	i,	j;

int	ret;

	if( ja->type == JAPP_WIGHT ){
		ret = imageF_jet2AppW( im, i0,	j0, ja, f );
		return( ret );
	}

	sp = (float *)IMAGE_PIXEL(im, i0, j0 );
	align = im->column - ja->n;


	JET2_ZERO( g );
	JET2_ZERO( *f );



	for( i = 0 ; i < ja->n ; i++, sp += align ){
		s0 = s1 = s2 = 0;
		for( j = 0 ; j < ja->n ; j++ ){
			t = *sp++;
			s0 += ja->a0[j] * t;
			s1 += ja->a1[j] * t;
			s2 += ja->a2[j] * t;
		}

		g.a00 += ja->a0[i] * s0;

		g.a10 += ja->a0[i] * s1;
		g.a01 += ja->a1[i] * s0;

		g.a20 += ja->a0[i] * s2;
		g.a11 += ja->a1[i] * s1;
		g.a02 += ja->a2[i] * s0;
	}



	f->a00 = g.a00 * ja->f00;//( ja->t0*ja->t0 );

	f->a10 = g.a10 * ja->f10;//( ja->t0*ja->t1 );
	f->a01 = g.a01 * ja->f10;//( ja->t0*ja->t1 );


	f->a20 = g.a20 * ja->f20;//( ja->t0*ja->t2 );
	f->a11 = g.a11 * ja->f11;//( ja->t1*ja->t1 );
	f->a02 = g.a02 * ja->f20;//( ja->t0*ja->t2 );

	f->a00 -= (f->a20  + f->a02)*  ja->f00_2;//ja->t0 * ja->t02;



	return( 1 );
}




int	
imageUS_jet2App( image_type *im, int	i0,	int	j0, jet2App_type *ja, jet2 *f )
{
u_short	*sp;
jet2	g;
float	s0,	s1,	s2,	t;
int	align,	i,	j;

int	ret;

	if( ja->type == JAPP_WIGHT ){
		ret = imageUS_jet2AppW( im, i0,	j0, ja, f );
		return( ret );
	}

	sp = IMAGES_PIXEL(im, i0, j0 );
	align = im->column - ja->n;


	JET2_ZERO( g );
	JET2_ZERO( *f );



	for( i = 0 ; i < ja->n ; i++, sp += align ){
		s0 = s1 = s2 = 0;
		for( j = 0 ; j < ja->n ; j++ ){
			t = *sp++;
			s0 += ja->a0[j] * t;
			s1 += ja->a1[j] * t;
			s2 += ja->a2[j] * t;
		}

		g.a00 += ja->a0[i] * s0;

		g.a10 += ja->a0[i] * s1;
		g.a01 += ja->a1[i] * s0;

		g.a20 += ja->a0[i] * s2;
		g.a11 += ja->a1[i] * s1;
		g.a02 += ja->a2[i] * s0;
	}



	f->a00 = g.a00 * ( ja->t0*ja->t0 );

	f->a10 = g.a10 * ( ja->t0*ja->t1 );
	f->a01 = g.a01 * ( ja->t0*ja->t1 );


	f->a20 = g.a20 * ( ja->t0*ja->t2 );
	f->a11 = g.a11 * ( ja->t1*ja->t1 );
	f->a02 = g.a02 * ( ja->t0*ja->t2 );

	f->a00 -= f->a20 * ja->t02 *ja->t0* ja->t0 + f->a02 * ja->t02 *ja->t0* ja->t0;



	return( 1 );
}
#endif






