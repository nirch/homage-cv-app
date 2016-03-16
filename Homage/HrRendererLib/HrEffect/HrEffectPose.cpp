//  Defines the entry point for the console application.
//
#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#ifdef _DEBUG
#define _DUMP
#endif

#define _GPLOG

#include "Ulog/Log.h"
#include "Uvl/TfType.h"

#include "Umath/Matrix3Type.h"
#include "Umath/Matrix2Type.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"






#include "HrEffectPose.h"


void	lt2_affine_setR( lt2_type *lt, float x0, float y0, float dx, float dy, float scale, float rx, float ry, float rz );




CHrEffectPose::CHrEffectPose()
{
	m_id = EFFECT_POSE;

	m_im = NULL;


	m_atf = NULL;

	m_width = -1;
	m_height = -1;

}

CHrEffectPose::~CHrEffectPose()
{
	DeleteContents();
}





void CHrEffectPose::DeleteContents()
{


	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}


	if( m_atf != NULL ){
		tfA_destroy( m_atf );
		m_atf = NULL;
	}

}

int CHrEffectPose::Init( char *inFile )
{

	if( tfA_read( &m_atf, inFile ) < 0 )
		return( -1 );

	m_iFrame = -1;
	return( 1 );
}


int CHrEffectPose::SetFrameSize( int width, int height )
{

	if( m_atf == NULL ){
		m_atf = tfA_alloc( 1 );

		tf_type *tf = tf_alloc( 6 );
		tf->iFrame = 0;
		tf->a[0] = 0.5*width;//100;
		tf->a[1] = 0.5*height;//95;
		tf->a[2] = 1;
		tf->a[3] = tf->a[4] = tf->a[5] = 0;

		m_atf->a[m_atf->nA++] = tf;
	}

	m_width = width;
	m_height = height;

	m_im = image_alloc( m_width, m_height, 4, IMAGE_TYPE_U8, 1 );

	m_iFrame = -1;
	return( 1 );
}



int CHrEffectPose::InitFromData( char *data )
{
	char	buf[10001];
	strncpy( buf, data, 1000 );
	GPLOG( ("%s\n", buf));

	if( tfA_read_from_data( &m_atf, data ) < 0 )
		return( -1 );

	m_iFrame = -1;
	return( 1 );
}


int	CHrEffectPose::Process( image_type *sim, int iFrame, long long timeStamp, image_type **im )
{
	vec2f_type	p;
	float rx,	ry,	rz,	scale;
	GetPose( iFrame, &p, &scale, &rx, &ry, &rz );
	


	lt2_type lt;
	//lt2_similarity_set( &lt, p.x, p.y, ANGLE_D2R(angle), scale );


	float y0 = 0.5*sim->height;
	float x0 = 0.5*sim->width;

//	float y0 = 0.5*m_height;
//	float x0 = 0.5*m_width;

	lt2_affine_setR( &lt, x0, y0, p.x, p.y, scale, rx, ry, -rz );

	//if( m_width >= 0 ){
//		lt.c0 -= 0.5*m_width - x0;
	//	lt.c1 -= 0.5*m_height - y0;
	//}

	//if( m_width >= 0 ){
	//	lt.c0 -= 0.5*m_width;
	//	lt.c1 -= 0.5*m_height;
	//}

	IMAGE_DUMP( sim, "aa", iFrame, "pose-src");
	m_im = image_dewarp_lt2( sim, &lt, m_im );
	IMAGE_DUMP( m_im, "aa", iFrame, "pose-out");



	m_iFrame = iFrame;

	*im = m_im;


	return( 1 );
}


int	CHrEffectPose::GetPose( int iFrame, vec2f_type *p, float *scale, float *rx, float *ry, float *rz )
{


	tf_type *tf0 = tfA_get_prev( m_atf, iFrame+1 );
	tf_type *tf1 = tfA_get_next( m_atf, iFrame );

//	if( tf0 != NULL )
//		tf_log( tf0 );
//	if( tf1 != NULL )
//		tf_log( tf1 );

	if( tf0 != NULL && (tf0->iFrame == iFrame || tf1 == NULL) ){
		p->x = tf0->a[0];
		p->y = tf0->a[1];
		*scale = tf0->a[2];
		*rx = tf0->a[3];
		*ry = tf0->a[4];
		*rz = tf0->a[5];

		GPLOG(("A  %d   %f %f %f  %f %f %f\n", iFrame, p->x, p->y, *scale, *rx, *ry, *rz ) );
		return( 1 );
	}


	if( tf0 == NULL && tf1 != NULL ){
		p->x = tf1->a[0];
		p->y = tf1->a[1];
		*scale = tf1->a[2];
		*rx = tf1->a[3];
		*ry = tf1->a[4];
		*rz = tf1->a[5];

		GPLOG(("B  %d   %f %f %f  %f %f %f\n", iFrame, p->x, p->y, *scale, *rx, *ry, *rz ) );

		return( 1 );
	}


	float t = ( iFrame - tf0->iFrame)/(float)(tf1->iFrame - tf0->iFrame);

	p->x = (1-t)*tf0->a[0] + t*tf1->a[0];
	p->y = (1-t)*tf0->a[1] + t*tf1->a[1];
	*scale = (1-t)*tf0->a[2] + t*tf1->a[2];
	*rx = (1-t)*tf0->a[3] + t*tf1->a[3];
	*ry = (1-t)*tf0->a[4] + t*tf1->a[4];
	*rz = (1-t)*tf0->a[5] + t*tf1->a[5];

	GPLOG(("C  %d   %f %f %f  %f %f %f\n", iFrame, p->x, p->y, *scale, *rx, *ry, *rz ) );

	return(  1 );

}




int	CHrEffectPose::Close()
{
		DeleteContents();

		return( 1 );
}



void
lt2_affine_setR1( lt2_type *lt, float x0, float y0, float dx, float dy, float scale, float rx, float ry, float rz )
{

	matrix3_type m3;
	matrix2_type m,	im;

	matrix3_rotationXYZ( &m3, ANGLE_D2R(rx), ANGLE_D2R(ry), ANGLE_D2R(rz) );

	im.a00 = m3.a00;
	im.a01 = m3.a01;
	im.a10 = m3.a10;
	im.a11 = m3.a11;

	matrix2_inverse( &im, &m );

	//vec3f_type	a[4];

	//VEC3D_SET( a[0], -1, -1, 0 );
	//VEC3D_SET( a[1], -1,  1, 0 );
	//VEC3D_SET( a[2],  1,  1, 0 );
	//VEC3D_SET( a[3], 1, -1, 0 );

	//vec2f_type ap[4];
	//int	i;
	//for( i = 0 ; i < 4 ; i++ ){
	//	matrix3_multV( &m, &a[i], &p );
	//	ap[i].x = p.x;
	//	ap[i].y = p.y;
	//}


	//for( i = 0 ; i < 4 ; i++ ){
	//	ap[i].x *= scale;
	//	ap[i].y *= scale;
	//}


	scale = 1.0 / scale;

	// dx -= 120;
	// dy -= 120

	lt->a0 = scale * m.a00;
	lt->b0 = scale * m.a01;
//	lt->c0 = -lt->a0 *x0 - lt->b0 *y0 + dx;
	lt->c0 = -lt->a0 *dx - lt->b0 *dy + x0;

	lt->a1 = scale * m.a10;
	lt->b1 = scale * m.a11;
//	lt->c1 = -lt->a1 *x0 - lt->b1 *y0 + dy;
	lt->c1 = -lt->a1 *dx - lt->b1 *dy + y0;

#ifdef _TEST
	for( i = 0 ; i < 4 ; i++ ){
		p.x = LT2_F1( *lt, a[i].x, a[i].y );
		p.y = LT2_F2( *lt, a[i].x, a[i].y );
		vec3f_type p1;
		matrix3_multV( &m, &a[i], &p1 );

		p1.x -= p.x;
		p1.y -= p.y;

		ap[i].x = p.x;
		ap[i].y = p.y;
	}
#endif


}




void
	lt2_affine_setR( lt2_type *lt, float x0, float y0, float dx, float dy, float scale, float rx, float ry, float rz )
{

	matrix3_type m3;
	matrix2_type m,	im;

	matrix3_rotationXYZ( &m3, ANGLE_D2R(rx), ANGLE_D2R(ry), ANGLE_D2R(rz) );

	im.a00 = m3.a00;
	im.a01 = m3.a01;
	im.a10 = m3.a10;
	im.a11 = m3.a11;

	matrix2_inverse( &im, &m );

	//scale = 1.0 / scale;

	lt2_type	lt0;


	lt0.a0 = scale * im.a00;
	lt0.b0 = scale * im.a01;
	lt0.c0 = -lt0.a0 *x0 - lt0.b0 *y0 + dx;


	lt0.a1 = scale * im.a10;
	lt0.b1 = scale * im.a11;
	lt0.c1 = -lt0.a1 *x0 - lt0.b1 *y0 + dy;


	lt2_inverse( &lt0, lt );

#ifdef _AA_
	// dx -= 120;
	// dy -= 120

	lt->a0 = scale * im.a00;
	lt->b0 = scale * im.a01;
	//	lt->c0 = -lt->a0 *x0 - lt->b0 *y0 + dx;
	lt->c0 = -lt->a0 *dx - lt->b0 *dy + x0;

	lt->a1 = scale * im.a10;
	lt->b1 = scale * im.a11;
	//	lt->c1 = -lt->a1 *x0 - lt->b1 *y0 + dy;
	lt->c1 = -lt->a1 *dx - lt->b1 *dy + y0;

#ifdef _TEST
	for( i = 0 ; i < 4 ; i++ ){
		p.x = LT2_F1( *lt, a[i].x, a[i].y );
		p.y = LT2_F2( *lt, a[i].x, a[i].y );
		vec3f_type p1;
		matrix3_multV( &im, &a[i], &p1 );

		p1.x -= p.x;
		p1.y -= p.y;

		ap[i].x = p.x;
		ap[i].y = p.y;
	}
#endif

#endif
}