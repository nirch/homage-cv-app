
#include	<math.h>

#include	"ImageType/ImageType.h"

#include	"ImageApp.h"

#include	"Umath/umath.h"
#include	"Ujet2/Jet2Type.h"
#include	"Ujet1/Jet1Type.h"


jet2App_type *
jet2AppW_init( int n, int weight )
{
jet2App_type *jaw;
float	w[100];


	if( weight == 2 )
		vector_weight_guasian( w, n, 1.5 );
	else
		if( weight == 3 )
			vector_weight_121( w, n );
		else
			vector_weight_linear( w, n, 1.0, 0.5 );


	jaw = jet2AppW_init_w( n, w );

	return( jaw );
}


jet2App_type *
jet2AppW_init_w( int n, float w[] )
{
jet2App_type	*ja;
int	i;
float	x,	t;


	ja = (jet2App_type *)malloc( sizeof(jet2App_type) );


	ja->type = JAPP_WIGHT;

	ja->n = n;
	ja->nh = 0.5*n;
	ja->dn = 0.5*n;
	ja->dn_inv = 1.0 / ja->dn;

	ja->dxy = ( 2*ja->nh == ja->n )? 0 : 0.5;



	t = vector_sum( w, n );
	t = 1.0 / t;
	for( i = 0 ; i < n ; i++ )
		ja->w[i] = w[i] *t;


	for( i = 0 ; i < n ; i++ ){
		x = ( -(n) +1 + 2*i)/(float)( n );


		ja->a0[i] = 1;
		ja->a1[i] = x;
		ja->a2[i] = x*x;
	}




	t = vector_inner_w( ja->a0, ja->a0, ja->w, ja->n );
	ja->t0 = 1.0 / sqrt( t );
	vector_multiply( ja->a0, ja->t0, ja->n );

	t = vector_inner_w( ja->a1, ja->a1, ja->w, ja->n );
	ja->t1 = 1.0 / sqrt( t );
	vector_multiply( ja->a1, ja->t1, ja->n );

	ja->t02 = vector_inner_w( ja->a0, ja->a2, ja->w, ja->n );
	vector_acumalute( ja->a2, ja->a0, -ja->t02, ja->n );

	t = vector_inner_w( ja->a2, ja->a2, ja->w, ja->n );
	ja->t2 = 1.0 / sqrt( t );
	vector_multiply( ja->a2, ja->t2, ja->n );



	ja->f00 = ja->t0 * ja->t0;
	ja->f10 = ja->t1 * ja->t0;
	ja->f11 = ja->t1 * ja->t1;
	ja->f20 = ja->t2 * ja->t0;
	ja->f00_2 = ja->t0 * ja->t02;




	return( ja );


}

int	
image1_jet2AppW( image_type *im, int	i0,	int	j0, jet2App_type *ja, jet2 *f )
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
			s0 += ja->a0[j] * ja->w[j] * t;
			s1 += ja->a1[j] * ja->w[j] * t;
			s2 += ja->a2[j] * ja->w[j] * t;
		}

		g.a00 += ja->a0[i] * ja->w[i] * s0;

		g.a10 += ja->a0[i] * ja->w[i] * s1;
		g.a01 += ja->a1[i] * ja->w[i] * s0;

		g.a20 += ja->a0[i] * ja->w[i] * s2;
		g.a11 += ja->a1[i] * ja->w[i] * s1;
		g.a02 += ja->a2[i] * ja->w[i] * s0;
	}



	f->a00 = g.a00 * ( ja->t0*ja->t0 );

	f->a10 = g.a10 * ( ja->t0*ja->t1 );
	f->a01 = g.a01 * ( ja->t0*ja->t1 );


	f->a20 = g.a20 * ( ja->t0*ja->t2 );
	f->a11 = g.a11 * ( ja->t1*ja->t1 );
	f->a02 = g.a02 * ( ja->t0*ja->t2 );

	//f->a00 -= f->a20 * ja->t02 *ja->t0* ja->t0 + f->a02 * ja->t02 *ja->t0* ja->t0;

//	f->a00 -= ( f->a20 + f->a02 )* ja->t02 *ja->t0* ja->t0;
	f->a00 -= (f->a20  + f->a02)*  ja->f00_2;


	return( 1 );
}


int	imageF_jet2AppW( image_type *im, int i0, int j0, jet2App_type *ja, jet2 *f )
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
			s0 += ja->a0[j] * ja->w[j] * t;
			s1 += ja->a1[j] * ja->w[j] * t;
			s2 += ja->a2[j] * ja->w[j] * t;
		}

		g.a00 += ja->a0[i] * ja->w[i] * s0;

		g.a10 += ja->a0[i] * ja->w[i] * s1;
		g.a01 += ja->a1[i] * ja->w[i] * s0;

		g.a20 += ja->a0[i] * ja->w[i] * s2;
		g.a11 += ja->a1[i] * ja->w[i] * s1;
		g.a02 += ja->a2[i] * ja->w[i] * s0;
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
imageUS_jet2AppW( image_type *im, int	i0,	int	j0, jet2App_type *ja, jet2 *f )
{
u_short	*sp;
jet2	g;
float	s0,	s1,	s2,	t;
int	align,	i,	j;

	sp = IMAGES_PIXEL(im, i0, j0 );
	align = im->column - ja->n;


	JET2_ZERO( g );
	JET2_ZERO( *f );



	for( i = 0 ; i < ja->n ; i++, sp += align ){
		s0 = s1 = s2 = 0;
		for( j = 0 ; j < ja->n ; j++ ){
			t = *sp++;
			s0 += ja->a0[j] * ja->w[j] * t;
			s1 += ja->a1[j] * ja->w[j] * t;
			s2 += ja->a2[j] * ja->w[j] * t;
		}

		g.a00 += ja->a0[i] * ja->w[i] * s0;

		g.a10 += ja->a0[i] * ja->w[i] * s1;
		g.a01 += ja->a1[i] * ja->w[i] * s0;

		g.a20 += ja->a0[i] * ja->w[i] * s2;
		g.a11 += ja->a1[i] * ja->w[i] * s1;
		g.a02 += ja->a2[i] * ja->w[i] * s0;
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





int	
image1_jet1AppW( image_type *im, int i0, int j0, jet2App_type *ja, jet1_type *f )
{
u_char	*sp;
jet1_type	g;
float	s0,	s1,	s2,	t;
int	j;

	sp = IMAGE_PIXEL(im, i0, j0 );


	JET1_ZERO( g );
	JET1_ZERO( *f );



	s0 = s1 = s2 = 0;
	for( j = 0 ; j < ja->n ; j++ ){
		t = *sp++;
		g.a0 += ja->a0[j] * ja->w[j] * t;
		g.a1 += ja->a1[j] * ja->w[j] * t;
		g.a2 += ja->a2[j] * ja->w[j] * t;
	}



	f->a0 = g.a0 * ja->t0;

	f->a1 = g.a1 * ja->t1;


	f->a2 = g.a2 * ja->t2;


	f->a0 -= f->a2* ja->t02 *ja->t0;


	return( 1 );
}



int	
image1_jet1AppWY( image_type *im, int i0, int j0, jet2App_type *ja, jet1_type *f )
{
	u_char	*sp;
	jet1_type	g;
	float	s0,	s1,	s2,	t;
	int	j;

	sp = IMAGE_PIXEL(im, i0, j0 );


	JET1_ZERO( g );
	JET1_ZERO( *f );



	s0 = s1 = s2 = 0;
	for( j = 0 ; j < ja->n ; j++, sp += im->width ){
		t = *sp;
		g.a0 += ja->a0[j] * ja->w[j] * t;
		g.a1 += ja->a1[j] * ja->w[j] * t;
		g.a2 += ja->a2[j] * ja->w[j] * t;
	}



	f->a0 = g.a0 * ja->t0;

	f->a1 = g.a1 * ja->t1;


	f->a2 = g.a2 * ja->t2;


	f->a0 -= f->a2* ja->t02 *ja->t0;


	return( 1 );
}