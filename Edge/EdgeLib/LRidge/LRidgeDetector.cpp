/*************************
 ***   VaHolding.cpp   ***
 *************************/
#ifdef _DEBUG
#define	_DUMP
#endif

#include	<math.h>

#include "Ucamera/Pt2dType.h"
#include "Uln/PlnType.h"
#include "LRidgeDetector.h"
#include "Umath/Matrix2Type.h"

#include "Lk/LkType.h"


#include	"ImageDump/ImageDump.h"








lkImage_type *	lkImage_ridge_create( lkImage_type *lkM, int width, int height );




CLRidgeDetector::CLRidgeDetector()
{

	m_im = NULL;
	m_lkM = NULL;

}



CLRidgeDetector::~CLRidgeDetector()
{
	DeleteContents();
}


void CLRidgeDetector::DeleteContents()
{



}

void CLRidgeDetector::Init( char *prmFile, int height, int width )
{

	m_width = 2*width+1;
	m_height = 2*height+1;

	m_lkM =	lkImage_ridge_create( m_lkM, m_width, m_height );



}








int CLRidgeDetector::Detect( image_type *sim, float d, int color, vec2f_type *P, vec2f_type *V, float *f, float *dx )
{
	vec2f_type	v,	p,	p0;
	float	d0;
	int	i;
	float	tmp,	tMax;
	v.x = V->y;
	v.y = -V->x;

	tMax = 0;


	for( i = -d ; i <= d ; i++ ){
		p.x = P->x + i*v.x;
		p.y = P->y+ i*v.y;

		tmp = Detect_1( sim, &p, V );

		if( color == 2 && tmp > tMax || color == 1 &&  -tmp > -tMax ){
			tMax = tmp;
			p0 = p;
			d0 = i;
		}
	}

	if( tMax == 0 )
		return( -1 );

	if( d0 == -d || d0 == d )
		return( -1 );

	*P = p0;
	*f = tMax;
	*dx = d0;

	return( 1 );
}



//image_type *	image_interpolation_pv( image_type *sim, vec2f_type *p, vec2f_type *v, int w, int h, image_type *im );



float CLRidgeDetector::Detect_1( image_type *sim, vec2f_type *p, vec2f_type *v )
{
float	val;
int	a = 0;
	
	m_im = image_interpolation_pv( sim, p, v, m_width, m_height, m_im );

	if( a == 1 ){
		IMAGE_DUMP( m_im, "ridge", 1, NULL );
	}


	val =  imageF_accumulate( m_lkM->im, 0, 0,
			m_im, 0, 0,
			m_width, m_height );

	return( val );
}




lkImage_type * 
lkImage_ridge_create( lkImage_type *lkM, int width, int height )
{
int	i,	j;
float	y,	f,	fy;



	if( lkM == NULL )
		lkM = lkImage_alloc();

	lkM->width = width;
	lkM->height = height;
	lkM->x0 = 0;
	lkM->y0 = 0;
	lkM->fIm = 1;

	lkM->im = image_realloc( lkM->im, width, height, 1, IMAGE_TYPE_F, 1 );

	lkM->dyIm = image_realloc( lkM->dyIm, width, height, 1, IMAGE_TYPE_F, 1 );
	lkM->dxIm = image_realloc( lkM->dxIm, width, height, 1, IMAGE_TYPE_F, 1 );



	double s,	s2;
	s = s2 = 0;
	for( i = 0 ; i < lkM->im->height ; i++ ){
		y = -1 + 2*(i+0.5)/height;
		f = y*y;

		for( j = 0 ; j < lkM->im->width ; j++ ){
			s += f;
			s2 += f*f;
		}
	}
	int	n = width*height;

	double a,	b;
	b = s / n;
	a = s2/n - b*b;




	
	float	*tp = lkM->im->data_f;
	float	*yp = lkM->dyIm->data_f;
	float	*xp = lkM->dxIm->data_f;

	for( i = 0 ; i < lkM->im->height ; i++ ){
		y = -1 + 2*(i+0.5)/height;

		f = a*y*y - a*b;
		fy = 2*a*y;

		for( j = 0 ; j < lkM->im->width ; j++ ){
			*tp++ = f;
			*yp++ = fy;
			*xp++ = 0;
		}
	}



	lkM->xIm = image_reallocL( lkM->im, lkM->xIm );
	lkM->yIm = image_reallocL( lkM->im, lkM->yIm );


	yp = lkM->yIm->data_f;
	xp = lkM->xIm->data_f;


	for( i = 0 ; i < lkM->im->height ; i++ ){
		float y = -0.5 * (lkM->im->height-1) + i ;

		for( j = 0 ; j < lkM->im->width ; j++ ){

			float x = -0.5 * (lkM->im->width-1) + j ;
			*yp++ = y;
			*xp++ = x;
		}
	}


	return( lkM );
}




#ifdef _AA_
image_type *
image_interpolation_pv( image_type *sim, vec2f_type *p, vec2f_type *v, int w, int h, image_type *im )
{


	lt2_type	lt;


	lt.a0 = v->x;
	lt.b0 = -v->y;
	lt.c0 = 0;

	lt.a1 = v->y;
	lt.b1 = v->x;
	lt.c1 = 0;



	lt.c0 = p->x - 0.5*w*lt.a0 - 0.5*h*lt.b0;
	lt.c1 = p->y - 0.5*w*lt.a1 - 0.5*h*lt.b1;




//	im = image_realloc( im, w, h, 1, IMAGE_TYPE_U8, 1 );


	if( IMAGE_TYPE(sim) == IMAGE_TYPE_U8 ){
		im = image1_interpolation_lt2( sim, &lt, 0, 0, w, h, im );
		return( im );
	}

	if( IMAGE_TYPE(sim) == IMAGE_TYPE_U16 ){
		im = imageUS_interpolation_lt2( sim, &lt, 0, 0, w, h, im );
		return( im );
	}




	return( im );


}

#endif



int CLRidgeDetector::DetectT( image_type *sim, float d, int color, vec2f_type *P, vec2f_type *V )
{
	Detect_2( sim, P,  V );

	return( 1 );

}


float CLRidgeDetector::Detect_2( image_type *sim, vec2f_type *p, vec2f_type *v )
{
	int	G = 0;

	m_im = image_interpolation_pv( sim, p, v, m_width, m_height, m_im );

	if( G == 1 ){
		IMAGE_DUMP( m_im, "ridge", 1, NULL );
	}



	float f = imageF_accumulate( m_lkM->im, 0, 0,
		m_im, 0, 0,
		m_width, m_height );
		
	float fxX = imageFW_accumulate( m_lkM->dxIm, 0, 0,
		m_im, 0, 0,
		m_lkM->xIm,
		m_width, m_height );

	float fyY = imageFW_accumulate( m_lkM->dyIm, 0, 0,
		m_im, 0, 0,
		m_lkM->yIm,
		m_width, m_height );

	float fyX = imageFW_accumulate( m_lkM->dyIm, 0, 0,
		m_im, 0, 0,
		m_lkM->xIm,
		m_width, m_height );

	float fy = imageF_accumulate( m_lkM->dyIm, 0, 0,
		m_im, 0, 0,
		m_width, m_height );


	float A = f + fxX + fyY;
	float B = fyY;
	float C = fyX;
	float E = fy;




	if( ABS(C) < 0.001 ){
		matrix2_type m2;
		vec2d_type	D2,	X2;
		m2.a00 = B*B;
		m2.a01 = E*B;
		D2.x = (-A)*B;

		m2.a10 = B*E;
		m2.a11 = E*E;
		D2.y = (-A)*E;


		matrix2S_solve( &m2, &D2, &X2 );

		float s = X2.x;
		float a = 0;
		float b = X2.y;

		return( A );

	}

	matrix3_type m;
	vec3d_type	D,	X;

	m.a00 = B*B;
	m.a01 = C*B;
	m.a02 = E*B;
	D.x = (1-A)*B;

	m.a10 = B*C;
	m.a11 = C*C;
	m.a12 = E*C;
	D.y = (1-A)*C;

	m.a20 = B*E;
	m.a21 = C*E;
	m.a22 = E*E;
	D.z = (1-A)*E;


	matrix3S_solve( &m, &D, &X );

	float s = X.x;
	float a = X.y;
	float b = X.z;



	return( A );
}