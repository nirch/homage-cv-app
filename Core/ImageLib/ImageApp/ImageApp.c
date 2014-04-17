
#include	<math.h>

#include	"ImageType/ImageType.h"

#include	"ImageApp.h"

#include	"Umath/umath.h"
#include	"Ujet2/Jet2Type.h"
#include	"Ujet1/Jet1Type.h"




jet2App_type *
jet2App_init( int n, int weight )
{
jet2App_type	*ja;
int	i;
float	x,	t;

	if( weight != 0 ){
		ja = jet2AppW_init( n, weight );
		return( ja );
	}


	ja = (jet2App_type *)malloc( sizeof(jet2App_type) );

	ja->type = JAPP_NORMAL;

	ja->n = n;
	ja->nh = 0.5*n;
	ja->dn = 0.5*n;
	ja->dn_inv = 1.0 / ja->dn;

	ja->dxy = ( 2*ja->nh == ja->n )? 0 : 0.5;

	for( i = 0 ; i < n ; i++ ){
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


int	
image_jet2App( image_type *im, int	i0,	int	j0, jet2App_type *ja, jet2 *f )
{
	if( im->format == IMAGE_FORMAT_UC )
		return( image1_jet2App( im,	i0,	j0, ja, f ) );

	if( im->format == IMAGE_FORMAT_F )
		return( imageF_jet2App( im,	i0,	j0, ja, f ) );


	if( im->format == IMAGE_FORMAT_US )
		return( imageUS_jet2App( im,	i0,	j0, ja, f ) );


	return( -1 );
}




int	
image1_jet2App( image_type *im, int	i0,	int	j0, jet2App_type *ja, jet2 *f )
{
u_char	*sp;
jet2	g;
float	s0,	s1,	s2,	t;
int	align,	i,	j;

int	ret;

	if( ja->type == JAPP_WIGHT ){
		ret = image1_jet2AppW( im, i0,	j0, ja, f );
		return( ret );
	}

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

	//f->a00 -= f->a20 * ja->t02 *ja->t0* ja->t0 + f->a02 * ja->t02 *ja->t0* ja->t0;

	f->a00 -= ( f->a20 + f->a02 )* ja->t02 *ja->t0* ja->t0;


	return( 1 );
}


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



void
jet2App_destroy( jet2App_type *ja )
{
	free( ja );
}





int	
image1_jet1App( image_type *im, int	i0,	int	j0, jet2App_type *ja, jet1_type *f )
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
		g.a0 += ja->a0[j] * t;
		g.a1 += ja->a1[j] * t;
		g.a2 += ja->a2[j] * t;
	}



	f->a0 = g.a0 * ja->t0;

	f->a1 = g.a1 * ja->t1;


	f->a2 = g.a2 * ja->t2;


	f->a0 -= f->a2* ja->t02 *ja->t0;


	return( 1 );
}



int	
image1_jet1AppY( image_type *im, int	i0,	int	j0, jet2App_type *ja, jet1_type *f )
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
		g.a0 += ja->a0[j] * t;
		g.a1 += ja->a1[j] * t;
		g.a2 += ja->a2[j] * t;
	}



	f->a0 = g.a0 * ja->t0;

	f->a1 = g.a1 * ja->t1;


	f->a2 = g.a2 * ja->t2;


	f->a0 -= f->a2* ja->t02 *ja->t0;


	return( 1 );
}



int	
image1_jet1AppY_A( image_type *im, int i0, int j0, int n, jet2App_type *ja, jet1_type *f )
{
	u_char	*sp;
	jet1_type	g;
	float	s0,	s1,	s2,	t;
	int	i,	j,	align,	n2;

	sp = IMAGE_PIXEL(im, i0, j0-n );


	JET1_ZERO( g );
	JET1_ZERO( *f );

	
	n2 = 2*n+1;
	align = im->width - n2;
	s0 = s1 = s2 = 0;
	for( i = 0 ; i < ja->n ; i++, sp += align ){

		for( j = 0, t = 0 ; j < n2 ; j++ )
			t += *sp++;

		t /= n2;

		g.a0 += ja->a0[i] * t;
		g.a1 += ja->a1[i] * t;
		g.a2 += ja->a2[i] * t;
	}



	f->a0 = g.a0 * ja->t0;

	f->a1 = g.a1 * ja->t1;


	f->a2 = g.a2 * ja->t2;


	f->a0 -= f->a2* ja->t02 *ja->t0;


	return( 1 );
}


