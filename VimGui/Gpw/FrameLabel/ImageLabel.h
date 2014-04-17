// Vimage.h: interface for the CVimage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _IMAGE_LABEL_
#define _IMAGE_LABEL_

#include "ImageLabel.h"



class CImageLabel 
{
public:
    CImageLabel( void *view );
	CImageLabel();

    virtual ~CImageLabel();
    
    virtual int  Init( char *file, int x, int y );
   
	virtual int  Init( char *file );

	virtual int Draw( void *dc, int wx, int wy );
    
    virtual void SetPose( int x, int y );

  
	virtual void SetVisible( int flag );

	virtual void SetDrag( int flag );


	virtual int IsTouchBegin( void *view, int x, int y ) { return( -1 ); }
    virtual int IsTouchMove( void *view, int x, int y ) { return( -1 ); }
	virtual int IsTouchEnd( void *view, int x, int y ) { return( -1 ); }

    
protected:

	int	m_fx;
	int	m_fy;
  
	int	m_x;
	int m_y;

	int	m_visible;

	int m_drag;
};




#endif