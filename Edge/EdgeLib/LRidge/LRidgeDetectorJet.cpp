/*********************************
 ***   LRidgeDetectorJet.cpp   ***
 *********************************/
#ifdef _DEBUG
#define	_DUMP
#endif

#include	<math.h>

#include "Ucamera/Pt2dType.h"
#include "Umath/Matrix2Type.h"
#include	"ImageDump/ImageDump.h"
#include "ImageApp/ImageApp2.h"




#include "LRidgeDetectorJet.h"



CLRidgeDetectorJet::CLRidgeDetectorJet()
{

	m_im = NULL;
	m_ja = NULL;

}



CLRidgeDetectorJet::~CLRidgeDetectorJet()
{
	DeleteContents();
}


void CLRidgeDetectorJet::DeleteContents()
{



}

void CLRidgeDetectorJet::Init( char *prmFile, int height, int width )
{

	m_width = 2*width+1;
	m_height = 2*height+1;

	
	m_ja = jet2App2_init(m_width,  m_height, 0 );



}








int CLRidgeDetectorJet::Detect( image_type *sim, float d, int color, vec2f_type *P, vec2f_type *V, float *f, float *dx )
{
vec2f_type	p,	v;

	v = *V;
	p = *P;

	VEC2D_NORMALIZE( v );

	DetectStep( m_ja, sim, &p, &v );

	DetectStep( m_ja, sim, &p, &v );
	DetectStep( m_ja, sim, &p, &v );

	*f = DetectStep( m_ja, sim, &p, &v );

	fprintf( stdout, "%f  %f   %f  %f\n", p.x, p.y, v.x, v.y );
	*dx = 0;


	*P = p;
	*V = v;
	return( 1 );
}



//image_type *	image_interpolation_pv( image_type *sim, vec2f_type *p, vec2f_type *v, int w, int h, image_type *im );




float
CLRidgeDetectorJet::DetectStep( jet2App2_type *ja, image_type *sim, vec2f_type *p, vec2f_type *v )
{	
	static int iFrame = 0;
	jet2_type	f;
	djet2_type	d;

	m_im = image_interpolation_pv( sim, p, v, ja->x.n, ja->y.n, m_im );

	IMAGE_DUMP( m_im, "pv", iFrame++, NULL );

	//	image_jet2App2( sim, p->y-ja->y.dn, p->x-ja->x.dn, ja, &f );
	image_jet2App2( m_im, 0, 0, ja, &f );


	print_jet2( stdout, 2, &f );



	diagonalN_jet2( &f, 1, &d );

	float dx = -d.a1 / ( 2*d.lm1 );

	p->x += dx* d.n.x;
	p->y += dx* d.n.y;


	fprintf( stdout, "dx: %f    v: %f %f\n", dx,  d.n.x, d.n.y );


	vec2f_type	n,	dv;
	n.x = v->x * d.n.x - v->y * d.n.y;

	n.y = v->y * d.n.x + v->x * d.n.y;

	VEC2D_NORMALIZE( *v );

	dv.x = n.y;
	dv.y= -n.x;

	if( dv.x < 0 ){
		dv.x = -dv.x;
		dv.y = -dv.y;
	}

	*v = dv;

	return( d.lm1 );

}