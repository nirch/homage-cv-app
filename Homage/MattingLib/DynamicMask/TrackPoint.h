
#ifndef __TRACK_POINT_H__
#define __TRACK_POINT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000




#include	"ImageType/ImageType.h"




typedef struct trackPoint_type {

	vec2d	p;

	float	dw;
	float	dh;

	vec2d	v[4];

	vec2f_type	p0;

}	trackPoint_type;




class CTrackPoint
{
public:
	CTrackPoint();
	virtual ~CTrackPoint();

	void	DeleteContents();

//	int	AddPoint( vec2d *p );
	int	AddPoint( vec2d *p, float dw = 16, float dh = 16 );
	int	AddPoint( float x0, float y0, float x1, float y1 );
		
	int		RemovePoint( vec2d *p );

	int		SelectPoint( vec2d *p );

	void	Track( image_type *cim, image_type *nim);
	void	Trackm( image_type *cim, image_type *nim);	


	void	Draw( struct gpw_type *gpw );

public:
	image_type *m_im;	

	int	m_nTp;
	trackPoint_type	m_tp[100];

	int	m_frame;
};
#endif 


