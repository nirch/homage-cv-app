
#include	<math.h>

#include	"ImageType/ImageType.h"
#include	"Image1Tool.h"

#include	"Umath/umath.h"
#include	"Ujet2/Jet2Type.h"

/*
#define	 APP_ZIZE	10

typedef struct jet2App_type {
	int	n;

	float	a0[APP_ZIZE];
	float	a1[APP_ZIZE];
	float	a2[APP_ZIZE];


	float	t0;
	float	t1;
	float	t02;
	float	t2;
}	jet2App_type;
*/


et2App_type *
jet2App_init( int n )
{
jet2App_type	*ja;
int	i;
float	x,	t;

	ja = (jet2App_type *)malloc( sizeof(jet2App_type) );

	ja->n = n;

	for( i = 0 ; i < n ; i++ ){
//		x = ( -(2*n+1) +1 + 2*i)/( 2.0 * n+1 );
		x = ( -(n) +1 + 2*i)/(float)( n );

		ja->a0[i] = 1;
		ja->a1[i] = x;
		ja->a2[i] = x*x;
	}


	t = vector_inner( ja->a0, ja->a0, ja->n );
	ja->t0 = 1.0 / sqrt( t );
	vector_multiply( ja->a0, ja->t0, ja->n );

	t = vector_inner( ja->a1, ja->a1, ja->n );
	ja->t1 = 1.0 / sqrt( t );
	vector_multiply( ja->a1, ja->t1, ja->n );

	ja->t02 = vector_inner( ja->a0, ja->a2, ja->n );
	vector_acumalute( ja->a2, ja->a0, -ja->t02, ja->n );

	t = vector_inner( ja->a2, ja->a2, ja->n );
	ja->t2 = 1.0 / sqrt( t );
	vector_multiply( ja->a2, ja->t2, ja->n );



	ja->f00 = ja->t0 * ja->t0;
	ja->f10 = ja->t1 * ja->t0;
	ja->f11 = ja->t1 * ja->t1;
	ja->f20 = ja->t2 * ja->t0;
	ja->f00_2 = ja->t0 * ja->t02;




	return( ja );


}

int	image1_jet2App( image_type *im, int	i0,	int	j0, jet2App_type *ja, jet2 *f )
{
u_char	*sp;
jet2	g;
float	s0,	s1,	s2,	t;
int	align,	i,	j;

	sp = IMAGE_PIXEL(im, i0, j0 );
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

		g._00 += ja->a0[i] * s0;

		g._10 += ja->a0[i] * s1;
		g._01 += ja->a1[i] * s0;

		g._20 += ja->a0[i] * s2;
		g._11 += ja->a1[i] * s1;
		g._02 += ja->a2[i] * s0;
	}



	f->_00 = g._00 * ( ja->t0*ja->t0 );

	f->_10 = g._10 * ( ja->t0*ja->t1 );
	f->_01 = g._01 * ( ja->t0*ja->t1 );


	f->_20 = g._20 * ( ja->t0*ja->t2 );
	f->_11 = g._11 * ( ja->t1*ja->t1 );
	f->_02 = g._02 * ( ja->t0*ja->t2 );

	f->_00 -= f->_20 * ja->t02 *ja->t0* ja->t0 + f->_02 * ja->t02 *ja->t0* ja->t0;



	return( 1 );
}


int	imageF_jet2App( image_type *im, int	i0,	int	j0, jet2App_type *ja, jet2 *f )
{
float	*sp;
jet2	g;
float	s0,	s1,	s2,	t;
int	align,	i,	j;

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

		g._00 += ja->a0[i] * s0;

		g._10 += ja->a0[i] * s1;
		g._01 += ja->a1[i] * s0;

		g._20 += ja->a0[i] * s2;
		g._11 += ja->a1[i] * s1;
		g._02 += ja->a2[i] * s0;
	}



	f->_00 = g._00 * ja->f00;//( ja->t0*ja->t0 );

	f->_10 = g._10 * ja->f10;//( ja->t0*ja->t1 );
	f->_01 = g._01 * ja->f10;//( ja->t0*ja->t1 );


	f->_20 = g._20 * ja->f20;//( ja->t0*ja->t2 );
	f->_11 = g._11 * ja->f11;//( ja->t1*ja->t1 );
	f->_02 = g._02 * ja->f20;//( ja->t0*ja->t2 );

	f->_00 -= (f->_20  + f->_02)*  ja->f00_2;//ja->t0 * ja->t02;



	return( 1 );
}
