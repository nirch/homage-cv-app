/***********************
 ***   ImageApp2.c   ***
 ***********************/
#include	<math.h>

#include	"ImageType/ImageType.h"

#include	"ImageApp1.h"

#include	"Umath/umath.h"

#include	"Ujet1/Jet1Type.h"



void
jet1App_destroy( jet1App_type *ja )
{
	free( ja );
}



jet1App_type *
jet1App_init( int n, int weight )
{
	jet1App_type	*ja;


	ja = (jet1App_type *)malloc( sizeof(jet1App_type) );

	jet1App_initA( ja, n, weight );

	return( ja );
}





int
jet1App_initA( jet1App_type *ja, int n, int weight )
{
	int	i;
	float	x,	t;


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

	return( 1 );
}








#ifdef _AA_

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


#endif