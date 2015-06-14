

#include "TrackPoint.h"

#include	"Lk/LkRegistration.h"
#include	"Wutil/UWgpw/GpwType.h"



CTrackPoint::CTrackPoint()
{
	m_nTp = 0;
}



CTrackPoint::~CTrackPoint()
{
}



void CTrackPoint::DeleteContents()
{
	m_nTp = 0;
}



//int CTrackPoint::AddPoint( vec2d *p )
//{
//trackPoint_type	*tp;
//
//	tp = &m_tp[m_nTp++];
//
//	tp->p = *p;
//	tp->dw = 16;//12;
//	tp->dh = 16;//12;
//
//	tp->v[0].x = tp->p.x - tp->dw;
//	tp->v[0].y = tp->p.y - tp->dh;
//
//	tp->v[1].x = tp->p.x + tp->dw;
//	tp->v[1].y = tp->p.y - tp->dh;
//
//	tp->v[2].x = tp->p.x + tp->dw;
//	tp->v[2].y = tp->p.y + tp->dh;
//
//	tp->v[3].x = tp->p.x - tp->dw;
//	tp->v[3].y = tp->p.y + tp->dh;
//
//
//	return( m_nTp -1 );
//}


int CTrackPoint::AddPoint( vec2d *p, float dw, float dh )
{
	m_tp[m_nTp].p = *p;
	m_tp[m_nTp].p0 = m_tp[m_nTp].p;

	m_tp[m_nTp].dw = dw;
	m_tp[m_nTp].dh = dh;

	m_nTp++;

	return( m_nTp -1 );
}


int CTrackPoint::AddPoint( float x0, float y0, float x1, float y1 )
{

	m_tp[m_nTp].p.x = 0.5*( x1 + x0 );
	m_tp[m_nTp].p.x = 0.5*( y1 + y0 );
	m_tp[m_nTp].dw = 0.5*( x1 - x0 );
	m_tp[m_nTp].dh = 0.5*( y1 - y0 );

	m_tp[m_nTp].p0 = m_tp[m_nTp].p;

	m_nTp++;

	return( m_nTp -1 );
}


int CTrackPoint::RemovePoint( vec2d *p )
{
int	i;

	i = SelectPoint( p );
	if( i < 0 )	return( -1 );

	m_nTp--;
	m_tp[i] = m_tp[m_nTp];

	return( 1 );
}


int CTrackPoint::SelectPoint( vec2d *p )
{
float	dmin,	d,	x,	y;
int	imin,	i;


	imin = -1;
	dmin = 1000;

	for( i = 0 ; i < m_nTp ; i++ ){
		x = p->x - m_tp[i].p.x;
		y = p->y - m_tp[i].p.y;
		d = x*x + y*y;

		if( imin == -1 || d < dmin ){
			dmin = d;
			imin = i;
		}
	}

	return( imin );
}


void CTrackPoint::Track( image_type *cim, image_type *im)
{
trackPoint_type	*tp;
float	dd = 12;
box2i	b;
lkState_type	stat;
int	i,	k;
lt2_type lt;

	CLkRegistration *lk = new CLkRegistration();

	lk->SetRegistrationType(LK_SCALE_ROTATE);



	for( i = 0 ; i < m_nTp ; i++ ){
		tp = &m_tp[i];

		b.y0 = tp->p.y - dd;
		b.y1 = tp->p.y + dd;
		b.x0 = tp->p.x - dd;
		b.x1 = tp->p.x + dd;


		lk->SetRefernceImage( cim, &b );


		lk->InitilizeState( tp->p0.x, tp->p0.y, 0, 1.0 );

		lk->Registration( im );

		lk->GetStat( &stat );



		lk->GetGlobalLt( &lt );

		vec2f_type	p;
		p.x = LT2_F1( lt, tp->p.x, tp->p.y );
		p.y = LT2_F2( lt, tp->p.x, tp->p.y );
		tp->p = p;
		tp->p0 = p;

		for( k = 0 ; k < 4 ; k++ ){
			tp->v[k].x = LT2_F1( lt, tp->v[k].x, tp->v[k].y );
			tp->v[k].y = LT2_F2( lt, tp->v[k].x, tp->v[k].y );
		}
	}



	m_im = im;

	delete lk;
}




void CTrackPoint::Draw( gpw_type *gpw )
{
trackPoint_type	*tp;
int	i;


	for( i = 0 ; i < m_nTp ; i++ ){
		tp = &m_tp[i];


		gpw_marker_0( gpw, tp->p.y, tp->p.x, 2, YELLOW );

		gpw_empty_rectangle( gpw,
			tp->p.y-tp->dw, tp->p.x-tp->dh, tp->p.y+tp->dw, tp->p.x+tp->dh, RED );

		gpw_line( gpw, tp->v[0].y, tp->v[0].x, tp->v[1].y, tp->v[1].x, BLUE );
		gpw_line( gpw, tp->v[1].y, tp->v[1].x, tp->v[2].y, tp->v[2].x, BLUE );
		gpw_line( gpw, tp->v[2].y, tp->v[2].x, tp->v[3].y, tp->v[3].x, BLUE );
		gpw_line( gpw, tp->v[3].y, tp->v[3].x, tp->v[0].y, tp->v[0].x, BLUE );
	}
}