 /*****************************
 ***   Image3KlTransform.c  ***
 ******************************/
#include	<string.h>
#include    <math.h>

#include	 "Umath/Matrix3Type.h"
#include	"ImageType/ImageType.h"
#include	"ImageTransform.h"

#include	"ImageDump/ImageDump.h"



static void	image3_covarince_matrix( image_type *sim, matrix3_type *m );

static void	image3M_covarince_matrix( image_type *sim, image_type *mim, matrix3_type *m );

static void	imageUS3M_covarince_matrix( image_type *sim, image_type *mim, matrix3_type *m );



image_type *	
image_KL_component( image_type *sim, image_type *im )
{

	//if( sim->format == IMAGE_FORMAT_US3 ){
	//	im = image3_KL_component( sim, im )
	//	return( im  );
	//}


	if( sim->format == IMAGE_FORMAT_UC3 ){
		im = image3_KL_component( sim, 0, im );
		return( im  );
	}

	if( im != NULL )
		image_destroy( im, 1 );

	return( NULL );
}



image_type *	
image3_KL_component( image_type *sim, int i, image_type *im )
{
matrix3_type	em;
double	ev[3],	t,	a;


	image3_KL_matrix( sim, &em, ev );

//	matrix3_write( &em, stdout );


//	t = ( em.a00 + em.a10 + em.a20);


//	im = image3_linear_combination(sim, 0, em.a00/t, em.a10/t, em.a20/t, im );

	t = em.a[0][i] + em.a[1][i] + em.a[2][i];
//	t = 1.0;

	a = ( i == 0 )? 0 : 128;

	im = image3_linear_combination(sim, a, em.a[0][i]/t, em.a[1][i]/t, em.a[2][i]/t, im );

	return( im );

}


image_type *	
imageM_KL_component( image_type *sim, image_type *mim, int i, image_type *im )
{

	if( sim->format == IMAGE_FORMAT_US3 ){
		im = imageUS3M_KL_component( sim, mim, im );
		return( im  );
	}


	if( sim->format == IMAGE_FORMAT_UC3 ){
		im = image3M_KL_component( sim, mim, i, im );
			return( im  );
	}

	if( im != NULL )
		image_destroy( im, 1 );

	return( NULL );
}



image_type *	
image3M_KL_component( image_type *sim, image_type *mim, int i, image_type *im )
{
	matrix3_type	em;
	double	ev[3],	t;

	image3M_KL_matrix( sim, mim, &em, ev );


	t = em.a[0][i] + em.a[1][i] + em.a[2][i];


	im = image3_linear_combination(sim, 0, em.a[0][i]/t, em.a[1][i]/t, em.a[2][i]/t, im );

	return( im );

}

image_type *	
image3M_KL_componentF( image_type *sim, image_type *mim, int i, image_type *im )
{
	matrix3_type	em;
	double	ev[3],	t;

	image3M_KL_matrix( sim, mim, &em, ev );

	//	matrix3_write( &em, stdout );


	t = em.a[0][i]+ em.a[1][i] + em.a[2][i];


	im = image3_linear_combinationF(sim, 0, em.a[0][i]/t, em.a[1][i]/t, em.a[2][i]/t, im );

	return( im );

}

image_type *	
imageUS3M_KL_component( image_type *sim, image_type *mim, image_type *im )
{
	matrix3_type	em;
	double	ev[3],	t;

	imageUS3M_KL_matrix( sim, mim, &em, ev );

	//	matrix3_write( &em, stdout );


	t = ( em.a00 + em.a10 + em.a20);


	im = imageUS3_linear_combination(sim, 0, em.a00/t, em.a10/t, em.a20/t, im );

	return( im );

}


int
image3_KL_matrix( image_type *sim, matrix3_type *em, double ev[3] )
{
matrix3_type	m;



	image3_covarince_matrix( sim, &m );

	matrix3_write( &m, stdout );


	if( matrixS3_eigen( &m, em, ev ) < 0 )
		return( -1 );

	matrix3_write( em, stdout );

	fprintf( stdout, "%f  %f   %f\n", (float)ev[0], (float)ev[1], (float)ev[2] );

	return( 1 );
}


int	
imageM_KL_matrix( image_type *sim, image_type *mim, matrix3_type *em, double ev[3] )
{
int	ret;

	if( sim->format == IMAGE_FORMAT_US3 ){
		ret = imageUS3M_KL_matrix( sim, mim, em, ev );
		return( ret  );
	}


	if( sim->format == IMAGE_FORMAT_UC3 ){
		ret = image3M_KL_matrix( sim, mim, em, ev );
		return( ret  );
	}

	return( -1 );
}


int
image3M_KL_matrix( image_type *sim, image_type *mim, matrix3_type *em, double ev[3] )
{
	matrix3_type	m;



	image3M_covarince_matrix( sim, mim, &m );

	matrix3_write( &m, stdout );


	if( matrixS3_eigen( &m, em, ev ) < 0 )
		return( -1 );

	matrix3_write( em, stdout );

	fprintf( stdout, "%f  %f   %f\n", (float)ev[0], (float)ev[1], (float)ev[2] );

	return( 1 );
}

int
imageUS3M_KL_matrix( image_type *sim, image_type *mim, matrix3_type *em, double ev[3] )
{
	matrix3_type	m;



	imageUS3M_covarince_matrix( sim, mim, &m );

	matrix3_write( &m, stdout );


	if( matrixS3_eigen( &m, em, ev ) < 0 )
		return( -1 );

	matrix3_write( em, stdout );

//	fprintf( stdout, "%f  %f   %f\n", (float)ev[0], (float)ev[1], (float)ev[2] );

	return( 1 );
}


static void
image3_covarince_matrix( image_type *sim, matrix3_type *m )
{
u_char	*sp;

int	i,	j,	n;
vec3d_type	av;
int	r,	g,	b;



	matrix3_zero( m );
	av.x = av.y = av.z = 0;


	sp = sim->data;

	for( i = 0, n = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, n++ ){

			r = *sp++;
			g = *sp++;
			b = *sp++;

			av.x += r;
			av.y += g;
			av.z += b;

			m->a00 += r*r;
			m->a01 += r*g;
			m->a02 += r*b;
			m->a11 += g*g;
			m->a12 += g*b;
			m->a22 += b*b;
		}
	}

	av.x /= n;
	av.y /= n;
	av.z /= n;


	m->a00 /= n;
	m->a01 /= n;
	m->a02 /= n;
	m->a11 /= n;
	m->a12 /= n;
	m->a22 /= n;

	m->a00 -= av.x*av.x;
	m->a01 -= av.x*av.y;
	m->a02 -= av.x*av.z;

	m->a11 -= av.y*av.y;
	m->a12 -= av.y*av.z;

	m->a22 -= av.z*av.z;


	m->a10 = m->a01;
	m->a20 = m->a02;
	m->a21 = m->a12;
}



static void
image3M_covarince_matrix( image_type *sim, image_type *mim, matrix3_type *m )
{
	u_char	*sp,	*mp;

	int	i,	j,	n;
	vec3d_type	av;
	int	r,	g,	b;



	matrix3_zero( m );
	av.x = av.y = av.z = 0;


	sp = sim->data;
	mp = mim->data;

	for( i = 0, n = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, mp++ ){
			if( *mp == 0 ){
				sp += 3;
				continue;
			}

			n++;

			r = *sp++;
			g = *sp++;
			b = *sp++;

			av.x += r;
			av.y += g;
			av.z += b;

			m->a00 += r*r;
			m->a01 += r*g;
			m->a02 += r*b;
			m->a11 += g*g;
			m->a12 += g*b;
			m->a22 += b*b;
		}
	}

	av.x /= n;
	av.y /= n;
	av.z /= n;


	m->a00 /= n;
	m->a01 /= n;
	m->a02 /= n;
	m->a11 /= n;
	m->a12 /= n;
	m->a22 /= n;

	m->a00 -= av.x*av.x;
	m->a01 -= av.x*av.y;
	m->a02 -= av.x*av.z;

	m->a11 -= av.y*av.y;
	m->a12 -= av.y*av.z;

	m->a22 -= av.z*av.z;


	m->a10 = m->a01;
	m->a20 = m->a02;
	m->a21 = m->a12;
}

static void
imageUS3M_covarince_matrix( image_type *sim, image_type *mim, matrix3_type *m )
{
	u_short	*sp;
	u_char	*mp;

	int	i,	j,	n;
	vec3d_type	av;
	int	r,	g,	b;



	matrix3_zero( m );
	av.x = av.y = av.z = 0;


	sp = sim->data_us;
	mp = mim->data;

	for( i = 0, n = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, mp++ ){
			if( *mp == 0 ){
				sp += 3;
				continue;
			}

			n++;

			r = *sp++;
			g = *sp++;
			b = *sp++;

			av.x += r;
			av.y += g;
			av.z += b;

			m->a00 += r*r;
			m->a01 += r*g;
			m->a02 += r*b;
			m->a11 += g*g;
			m->a12 += g*b;
			m->a22 += b*b;
		}
	}

	av.x /= n;
	av.y /= n;
	av.z /= n;


	m->a00 /= n;
	m->a01 /= n;
	m->a02 /= n;
	m->a11 /= n;
	m->a12 /= n;
	m->a22 /= n;

	m->a00 -= av.x*av.x;
	m->a01 -= av.x*av.y;
	m->a02 -= av.x*av.z;

	m->a11 -= av.y*av.y;
	m->a12 -= av.y*av.z;

	m->a22 -= av.z*av.z;


	m->a10 = m->a01;
	m->a20 = m->a02;
	m->a21 = m->a12;
}


void
imageF_covarince_matrix( image_type *sim, matrix3_type *m )
{
	float	*sp;

	int	i,	j,	n;
	vec3d_type	av;
	int	r,	g,	b;



	matrix3_zero( m );
	av.x = av.y = av.z = 0;


	sp = sim->data_f;

	for( i = 0, n = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, n++ ){

			r = *sp++;
			g = *sp++;
			b = *sp++;

			av.x += r;
			av.y += g;
			av.z += b;

			m->a00 += r*r;
			m->a01 += r*g;
			m->a02 += r*b;
			m->a11 += g*g;
			m->a12 += g*b;
			m->a22 += b*b;
		}
	}

	av.x /= n;
	av.y /= n;
	av.z /= n;


	m->a00 /= n;
	m->a01 /= n;
	m->a02 /= n;
	m->a11 /= n;
	m->a12 /= n;
	m->a22 /= n;

	m->a00 -= av.x*av.x;
	m->a01 -= av.x*av.y;
	m->a02 -= av.x*av.z;

	m->a11 -= av.y*av.y;
	m->a12 -= av.y*av.z;

	m->a22 -= av.z*av.z;


	m->a10 = m->a01;
	m->a20 = m->a02;
	m->a21 = m->a12;
}


void	
image3M_KL_component_dump( image_type *sim, image_type *mim, int iFrame )
{
	matrix3_type	em;
	double	ev[3],	t,	t0,	t1;
	image_type	*im;

	image3M_KL_matrix( sim, mim, &em, ev );


	//	matrix3_write( &em, stdout );

	im = NULL;

	t = ( em.a00 + em.a10 + em.a20);
	im = image3_linear_combination(sim, 0, em.a00/t, em.a10/t, em.a20/t, im );

	image_dump( im, "PCA", iFrame, "1" );




	t0 = t1 = 0;
	if( em.a01 < 0 )	t0 -= em.a01;
	else	t1 += em.a01;

	if( em.a11 < 0 )	t0 -= em.a11;
	else	t1 += em.a11;

	if( em.a21 < 0 )	t0 -= em.a21;
	else	t1 += em.a21;

	t = MAX( t0, t1 );


	t *= 2;
	im = image3_linear_combination(sim, 128.0F, em.a01/t, em.a11/t, em.a21/t, im );

	image_dump( im, "PCA", iFrame, "2" );



	t0 = t1 = 0;
	if( em.a02 < 0 )	t0 -= em.a02;
	else	t1 += em.a02;

	if( em.a12 < 0 )	t0 -= em.a12;
	else	t1 += em.a12;

	if( em.a22 < 0 )	t0 -= em.a22;
	else	t1 += em.a22;

	t = MAX( t0, t1 );


	t *= 2;
	im = image3_linear_combination(sim, 128.0F, em.a02/t, em.a12/t, em.a22/t, im );

	image_dump( im, "PCA", iFrame, "3" );
	
	image_destroy( im, 1 );
}