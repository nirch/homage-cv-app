/******************************
 ***   FrameNavigator.cpp   ***
 ******************************/


#include "TouchEventWrappar.h"



CTouchEventWrappar::CTouchEventWrappar()
{
	m_nT = 0;
}



CTouchEventWrappar::~CTouchEventWrappar()
{
}



int CTouchEventWrappar::Add( void *touch, int x, int y )
{
	int	k;

	k = Get( touch );
	if( k >= 0 )
		return( -1 );

	m_at[m_nT].touch = touch;
	m_at[m_nT].p.x = x;
	m_at[m_nT].p.y = y;
	m_at[m_nT].p0 = m_at[m_nT].p;

	m_nT++;

	return( 1 );
}



int CTouchEventWrappar::Remove( void *touch )
{
int	i,	k;

	k = Get( touch );
	if( k < 0 )
		return( -1 );


	m_nT--;
	for( i = k ; i < m_nT ; i++ )
		m_at[i] = m_at[i+1];

	return( 1 );
}

int CTouchEventWrappar::Update( void *touch, int x, int y )
{
	int	k;

	k = Get( touch );
	if( k < 0 )
		return( -1 );

	m_at[k].p0 = m_at[k].p;

	m_at[k].p.x = x;
	m_at[k].p.y = y;

	return( 1 );
}


int	 CTouchEventWrappar::Get( void *touch )
{
	int	i;

	for( i = 0 ; i < m_nT ; i++ )
		if( m_at[i].touch == touch )
			return( i );

	return( -1 );
}


int CTouchEventWrappar::GetPose( vec2i_type p0[], vec2i_type p[] )
{
	int	i;

	for( i = 0 ; i < m_nT ; i++ ){
		p0[i] = m_at[i].p0;
        p[i] = m_at[i].p;
	}

	return( m_nT );
}


