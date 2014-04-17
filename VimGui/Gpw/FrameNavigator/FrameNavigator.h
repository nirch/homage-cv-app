// Vimage.h: interface for the CVimage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FRAME_NAVIGATOR_
#define _FRAME_NAVIGATOR_

#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "GpwD.lib" )
#else
#pragma comment( lib, "Gpw.lib" )
#endif

#include "Uvec/Vec2d.h"
#include "Ubox/Box2d.h"

#include "../Gpw.h"


class CFrameNavigator
{
public:
		CFrameNavigator(  int wWidth, int wHeight, int width, int height, CGpw *gpw );

		virtual ~CFrameNavigator();
		

		void SetWinSize( int width, int height );

		void SetSize( int width, int height );


		int Move( vec2i_type p[], vec2i_type p0[], int n );

		int Move( vec2i_type *p0, vec2i_type *p1 );

		int Move2( vec2i_type *p0, vec2i_type *p1, vec2i_type *q0, vec2i_type *q1 );



private:

	void PushTo();

	int	m_sWidth;
	int	m_sHeight;

	int	m_width;
	int m_height;

	vec2f_type	m_p;
	float		m_scale;

	CGpw	*m_gpw;
    
public:
    box2i_type  m_b;

};




#endif