
/**************************
 ***	FreeLineDraw.cpp	***
 **************************/

#include "FreeLineDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define REFRESH 2
#define DESTROY 0
#define NOP 1



#include	"Uigp/igp.h"
#include	"UGeoApp/GeoApp.h"

#include "../Gpw.h"





static void	edit_free_test_gapp( gapp_type *gapp, int *n0 );

static void	edit_free_rebuild_gapp( gapp_type *gapp, ln_type *link );




CFreeLineDraw::CFreeLineDraw( vec2f_type *p, CGpw *gpw)
{
	m_BeginVec = *p;

	m_gpw = gpw;
	m_LastLink = m_PolyLink = m_Link = ln_alloc();

	m_Unit = m_gpw->Unit();
	

	m_Link->ctr = *p;
	m_No = 0;
	

	m_Fgprm.dwight = 0.0;
	m_Fgprm.err = 0.00;

	m_Fgprm.min_len = 3.0;
	m_Fgprm.Min_pnumber = 10;
	m_Fgprm.max_len = 24.0;

	m_Fgprm.Max_dev = 1.0;
	m_Fgprm.Min_dev = 0.25;




	m_Fgprm.min_len = 10.0 * m_Unit;	
	if( m_Fgprm.min_len < 3.0 )	m_Fgprm.min_len = 3.0;

	m_Fgprm.Min_pnumber = 10 * m_Fgprm.min_len / 3.0;
	m_Fgprm.Min_dev = 0.25 * m_Fgprm.min_len / 3.0;




	m_Gapp = gapp_alloc( 4096 );


	m_Gapp->no = 0;
	m_Gapp->v[ m_Gapp->no] = *p;
	m_Gapp->tang[ m_Gapp->no].x = m_Gapp->tang[ m_Gapp->no].y = 0;
	m_Gapp->no++;
	ln_set_aux( m_Link );


	m_color = YELLOW;
}




CFreeLineDraw::~CFreeLineDraw()
{

	if ( m_PolyLink )
		lnL_destroy( m_PolyLink );

	if( m_Gapp != NULL )
		gapp_destroy( m_Gapp );

}





void CFreeLineDraw::setFgprmUnit()
{
	m_Unit = m_gpw->Unit();
	m_Fgprm.min_len = 10.0 * m_Unit;	

	if( m_Fgprm.min_len < 3.0 )	m_Fgprm.min_len = 3.0;


	m_Fgprm.Min_pnumber = 10 * m_Fgprm.min_len / 3.0;
	m_Fgprm.Min_dev = 0.25 * m_Fgprm.min_len / 3.0;
}


void
CFreeLineDraw::Draw()
{

	if( m_PolyLink != NULL )
		m_gpw->LnL( &m_PolyLink->ctr, m_PolyLink, NULL, m_color, 0 );
}



ln_type *CFreeLineDraw::GetLine()
{
	return m_PolyLink;
}


void CFreeLineDraw::GetEndDirection( vec2d *tanget )
{
	ln_tanget( m_Link, m_Link->len, tanget );
}



void CFreeLineDraw::Move( vec2f_type *p )
{
	if( p->x == m_Gapp->v[m_Gapp->no-1].x && p->y == m_Gapp->v[m_Gapp->no-1].y )
		return;

	vec2d	p2;
	ln_type	*link;
	float	a,	dis,	len,	a2;
	int	i2;

	setFgprmUnit();


	if( test1( p ) == true ){
		test2( p );
		return;
	}

	m_Gapp->v[ m_Gapp->no] = *p;
	m_Gapp->tang[ m_Gapp->no].x = m_Gapp->tang[ m_Gapp->no].y = 0;
	m_Gapp->no++;

	dis = geo_approximate_l2(&m_Gapp->v[m_No],&m_Gapp->v[m_Gapp->no-1],
			&m_Gapp->v[m_No], &m_Gapp->tang[m_No], m_Gapp->no - m_No,
			&m_Fgprm, &a, &len, &p2, &i2, &a2 );

	float aa = (float)(- a * len * len * 0.25);
	if( dis != 0 || (ABS(aa) > 0.25*len && len > 4.0 ) )
	{
		link = ln_alloc();
		m_No = m_Gapp->no-2;
		link->ctr = m_Gapp->v[ m_No ];

		
		link->p[0] = m_Link;
		if( m_Link != NULL )
			m_Link->p[1] = link;


		m_LastLink = m_Link;
		m_Link = link;

		dis = geo_approximate_l2(&m_Gapp->v[m_No],&m_Gapp->v[m_Gapp->no-1],
				&m_Gapp->v[m_No], &m_Gapp->tang[m_No], m_Gapp->no - m_No,
				&m_Fgprm, &a, &len, &p2, &i2, &a2 );

		edit_free_test_gapp( m_Gapp, &m_No );
	}


	m_Link->v.x = p->x - m_Link->ctr.x;
	m_Link->v.y = p->y - m_Link->ctr.y;

	m_Link->a = (float)(- a * len * len * 0.25);

	ln_set_aux( m_Link );


	return ;	

}


ln_type *CFreeLineDraw::Finish( vec2d point)
{
	ln_type *l;


	if( m_PolyLink != m_Link && m_Link->len < 0.75 ){
		l = LN_PREV( m_Link );
		l->p[1] = NULL;
		ln_destroy( m_Link );
		m_Link = l;
	}


	// clean
	l = m_PolyLink;
	m_PolyLink = NULL;
	m_Link = NULL;
	return l;

}




static void edit_free_test_gapp( gapp_type *gapp, int *n0 )
{
int	i,	n;

	if( 2*gapp->no < MAX_POINT_NUMBER )	return;

	for( i = 0, n = *n0 ; n < gapp->no; i++, n++ )
		gapp->v[i] = gapp->v[n];

	*n0 = 0;
	gapp->no = i;
}



bool CFreeLineDraw::test1( vec2d *p )
{
float	t,	u,	dis,	len;


	dis = ln_distance( &m_Link->ctr, m_Link, p, &u, &t );

	if( t >= m_Link->len )	return( false );

	if( dis > 0.75 )	return( false );

	dis = ln_distance( &m_Link->ctr, m_Link, p, &u, &t );
		
	m_Link->v.x = p->x - m_Link->ctr.x;
	m_Link->v.y = p->y - m_Link->ctr.y;

	len = m_Link->len;
	ln_set_aux( m_Link );

	m_Link->a *= m_Link->len / len;
	ln_set_aux( m_Link );


	edit_free_rebuild_gapp( m_Gapp, m_Link );
	m_No = 0;


	return( true );
}




bool CFreeLineDraw::test2( vec2d *p )
{
float	t,	u,	dis,	len;

	if( LN_PREV( m_Link ) == NULL )	return(false);

	if( m_Link->len > 1.0 )	return(false);

	ln_type *link = LN_PREV(m_Link );

	dis = ln_distance( &link->ctr, link, p, &u, &t );

	if( t >= link->len )	return( true );

	if( dis > 1.0 )	return( true );

	link->p[F_END] = NULL;
	free( m_Link );

	m_LastLink = m_Link = link;

	m_Link->v.x = p->x - m_Link->ctr.x;
	m_Link->v.y = p->y - m_Link->ctr.y;

	len = m_Link->len;
	ln_set_aux( m_Link );

	m_Link->a *= m_Link->len / len;
	ln_set_aux( m_Link );

	edit_free_rebuild_gapp( m_Gapp, m_Link );

	m_No = 0;


	return( true );
}


static void
edit_free_rebuild_gapp( gapp_type *gapp, ln_type *link )
{
float	t;

	gapp->no = 0;
	gapp->v[gapp->no] = link->ctr;
	gapp->tang[ gapp->no].x = gapp->tang[ gapp->no].y = 0;
	gapp->no++;

	for( t = 0.25 ; t < link->len ; t += 0.25 ){
		ln_t2xy( &link->ctr, link, t, &gapp->v[gapp->no] );
		gapp->tang[ gapp->no].x = gapp->tang[ gapp->no].y = 0;
		gapp->no++;
	}
	
	gapp->v[gapp->no].x = link->ctr.x + link->v.x;
	gapp->v[gapp->no].y = link->ctr.y + link->v.y;
	gapp->tang[ gapp->no].x = gapp->tang[ gapp->no].y = 0;
	gapp->no++;
}