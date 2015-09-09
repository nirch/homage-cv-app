
#include	<string.h>
#include	"Uigp/igp.h"


#include <windows.h>
#include	<Winuser.h>

#include "VMouseEvent.h"



CVMouseEvent::CVMouseEvent()
{
	m_eTimer.Create();
	m_wait = 1;

	m_dt = 2;


	m_x0 = m_y0 = -1;
}

CVMouseEvent::~CVMouseEvent( void )
{


}



int CVMouseEvent::Move( int x, int y )
{
	WaitEventEnd();

	m_inX = x;
	m_inY = y;

	SetEventBegin();

	return( 1 );
}

	


void CVMouseEvent::Run()
{
		SetEventEnd();

	while( 1 ) {
		WaitEventBegin();
		
		MoveTo( m_inX, m_inY );


		SetEventEnd();
	}
}



	
int CVMouseEvent::MoveTo( int x, int y )
{
	int	ret;
	int	i,	n,	dx,	dy,	x1,	y1,	cx,	cy;


	if( m_x0 == -1 || m_y0 == -1 ){
		POINT p;
		GetCursorPos( &p );
		m_x0 = p.x;
		m_y0 = p.y;
	}




	dx = x - m_x0;
	dy = y - m_y0;
	if( dx < 0 )	dx = -dx;
	if( dy < 0 )	dy = -dy;


	n = MAX( dx, dy )/m_dt;


	dx = x - m_x0;
	dy = y - m_y0;
	cx = m_x0;
	cy = m_y0;
	for( i = 0 ; i < n ; i++ ){
		x1 = m_x0 + i*dx / n;
		y1 = m_y0 + i*dy / n;

		if( x1 == cx && y1 == cy )
			continue;

		cx = x1;
		cy = y1;
		ret = SetPos ( cx, cy );
	}

	if( x != cx || x != cy ){
		ret = SetPos ( x, y );
	}

	m_x0 = x;
	m_y0 = y;

	return( ret );
}


int CVMouseEvent::SetPos ( int x, int y )
{
double fScreenWidth    = ::GetSystemMetrics( SM_CXSCREEN )-1; 
double fScreenHeight  = ::GetSystemMetrics( SM_CYSCREEN )-1; 
double fx = x*(65535.0f/fScreenWidth);
double fy = y*(65535.0f/fScreenHeight);
INPUT  Input={0};

	Input.type      = INPUT_MOUSE;
	Input.mi.dwFlags  = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;
	Input.mi.dx = (LONG)fx;
	Input.mi.dy = (LONG)fy;
	::SendInput(1,&Input,sizeof(INPUT));

	m_eTimer.Wait( m_wait );

return( 1 );


}