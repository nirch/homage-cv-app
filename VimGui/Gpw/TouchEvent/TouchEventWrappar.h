// Vimage.h: interface for the CVimage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TOUCH_EVENT_WRAPPAR_
#define _TOUCH_EVENT_WRAPPAR_

#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "GpwD.lib" )
#else
#pragma comment( lib, "Gpw.lib" )
#endif

#include "Uvec/Vec2d.h"
#include "Ubox/Box2d.h"



typedef struct touchEvent_type {
	void *touch;

	vec2i_type	p;
	vec2i_type	p0;

} touchEvent_type;


class CTouchEventWrappar
{
public:
		CTouchEventWrappar();

		virtual ~CTouchEventWrappar();
		

		int Add( void *touch, int x, int y );

		int Remove( void *touch );

		int	GetNo()	{ return( m_nT ); }

		int GetPose( vec2i_type p0[], vec2i_type p[] );

		int Update( void *touch, int x, int y );

private:

	int	Get( void *touch );

	int m_nT;
	touchEvent_type	m_at[4];

};




#endif