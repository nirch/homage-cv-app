// Vimage.h: interface for the CVimage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FRAME_LABEL_
#define _FRAME_LABEL_

#include "Uigp/igp.h"
#include "UVec/Vec2d.h"


class CFrameLabel
{
public:
		CFrameLabel();

		virtual ~CFrameLabel();



public:

	virtual int Init( char *file );
    
    virtual int Init( int x, int y, char *file );
    
	virtual void SetGpw( class CGpw *gpw )	{ m_gpw = gpw; }

	virtual void Draw( void *dc )	{ }


    virtual void SetFocus( int x, int y );
    
    
    virtual void Set( int visable, int x, int y );
    
    
    virtual void SetBasePose( float x, float y );

	virtual void SetPose( float x, float y );
    virtual void SetPose( vec2f_type *p );
    
    virtual void SetMove( int x, int y );
    
    virtual void StartMove( int x, int y );
    
    virtual void MoveTo( int x, int y );
    
    
    


	virtual void GetPose( int *x, int *y );
    virtual void GetPose( float *x, float *y );
    
    virtual void GetPose( vec2f_type *p );
    


	virtual void SetShow( int flag )	{ m_show = flag; }

	virtual void SetDrag( int flag )	{ m_drag = flag; }
    
    virtual void SetScale( float scale )    { m_scale = scale; }

	virtual void SetRelativeScale( float x0, float y0, float rscale );
    

	virtual int IsVisable()	{ return( m_show ); }

    int m_fx;
    int m_fy;
    
	float	m_x;
	float	m_y;
    
    float m_x0;
    float m_y0;

	int	m_show;

	int m_drag;
    
    float m_scale;
    
    
    
    int m_mx;
    int m_my;
    
    

	float	m_rx,	m_ry,	m_rs;


	class CGpw	*m_gpw;

};




#endif