// Vimage.h: interface for the CVimage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FRAME_LABELM_
#define _FRAME_LABELM_

#include "Uigp/igp.h"
#include "UVec/Vec2d.h"

#include "ImageLabel.h"

#include "FrameLabel.h"

#define MAX_LABEL 8



class CFrameLabelM : public CFrameLabel
{
public:
		CFrameLabelM( class CGpw *gpw, int dragable = 1 );

		virtual ~CFrameLabelM();



public:

//	virtual void SetGpw( class CGpw *gpw )	{ m_gpw = gpw; }

	virtual int Add( CImageLabel *label );

	virtual void  SetLabel( int iL );//	{ m_iL = iL; }
    

	virtual void Draw( void *dc );
 
    
    virtual void Set( int visable, int x, int y );
    
    
    virtual void SetBasePose( float x, float y );


	virtual void SetPose( vec2f_type *p );
	virtual void SetPose( float x, float y );
         

    virtual void StartMove( int x, int y );
	virtual void MoveTo( int x, int y );
    
    
    


	virtual void GetPose( int *x, int *y );
    virtual void GetPose( float *x, float *y );
    
    virtual void GetPose( vec2f_type *p );
    


	virtual void SetShow( int flag )	{ SetVisible( flag ); }

	virtual void SetVisible( int flag );

	virtual void SetDrag( int flag );//	{ m_drag = flag; }
    

	virtual int IsVisable()	{ return( m_visible ); }


	int TouchBegin( void *view, int x, int y );
	int TouchMove( void *view, int x, int y );
	int TouchEnd( void *view, int x, int y );


	virtual void SetRelativeScale( float x0, float y0, float rscale ) {};


public:
	int	m_nL;
	CImageLabel	*m_aL[MAX_LABEL];

	int	m_iL;

    
	float	m_x;
	float	m_y;
    

	vec2f_type	m_bp;


	int	m_visible;

	int m_drag;
       
    
	vec2f_type	m_mp;

	
	class CGpw	*m_gpw;
};




#endif