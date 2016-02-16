#include	<string.h>
#include	<stdio.h>
#include	<math.h>
#include	"Uigp/igp.h"

#include	"Umath/Matrix2Type.h"
#include 	"UGeoApp/GeoApp.h"

#include	"Uln/PlnType.h"
#include	"Ucamera/Pt2dType.h"



static int		pt2dA_get_next_break( pt2dA_type *apt, vec2d	*v, int i0, float dis );


void
pt2d_approximate_linkO( pt2dA_type *apt, vec2d *ctr, ln_type **link )
{
int	i,	j;
pt2d_type	*pt;
gapp_type	*gapp;
gapp_prm	Qgprm;


	gapp = gapp_alloc(  apt->nP + 100 );

	for( i = 0, j = 0 ; i < apt->nP ;  i++ ){
		pt = &apt->p[i];
		if( pt->r < 0 )	continue;
 
		gapp->v[j].x = pt->p.y;
		gapp->v[j].y = pt->p.x;

		gapp->tang[j] = pt->n;

		gapp->flag[j] = 0;
		j++;
	}

	gapp->no = j;


	Qgprm.dwight = 0.10;
	Qgprm.err = 0.01;

	Qgprm.min_len = 8;//3.0;
	Qgprm.Min_pnumber = 4;
	Qgprm.max_len = 30.0;

	Qgprm.Max_dev = 5.0;
	Qgprm.Min_dev = 0.5;

	geo_approximate( gapp, &Qgprm );




	lnL_from_gapp( gapp, ctr, link );

	gapp_destroy( gapp );
}



int	pt2dA_approximate_pln( pt2dA_type *apt, int i0, int i1, pln_type **pl )
{
	*pl = pln_alloc(0);

	pt2d_approximate_linkO( apt, &(*pl)->ctr, &(*pl)->link );

	pln_set_length( *pl );

	

	(*pl)->state = PLN_OPEN;

	return( 1 );
}
		

void
pt2d_approximate_link( pt2dA_type *apt, int i0, int i1, vec2d *ctr, ln_type **link )
{
pt2d_type	*pt;
int	i,	j,	j0	,j1;
ln_type	*l,	*pl;
vec2d	v,	p0,	p1,	dv;


gapp_type	*gapp;
gapp_prm	Qgprm;
float	a;



	gapp = gapp_alloc( 4096 );

	for( i = i0, j = 0 ; i < i1 ;  i++ ){
		pt = &apt->p[i];

		gapp->v[j].x = pt->p.y;
		gapp->v[j].y = pt->p.x;

		gapp->tang[j] = pt->n;

		gapp->flag[j] = 0;
		j++;
	}
	gapp->no = j;

	Qgprm.dwight = 0;//0.10;
	Qgprm.err = 0.01;

	Qgprm.min_len = 30;//3.0;
	Qgprm.Min_pnumber = 4;
	Qgprm.max_len = 60.0;

	Qgprm.Max_dev = 5.0;
	Qgprm.Min_dev = 1.0;


	*link = NULL;

	*ctr = gapp->v[0];

	 v = apt->p[i0].n;

	for( i = 0, pl = NULL ; i < gapp->no-1 ; ){
		i1 = pt2dA_get_next_break( apt, &v, i0+i, 60 );
		if( i1 > gapp->no-1 )	i1 = gapp->no-1;
		


	
		p0 = gapp->v[i];
		p1 = gapp->v[i1];

		if( i1 == apt->nGroup-1 ){
			dv.x = p1.x - p0.x;
			dv.y = p1.y - p0.y;
			if( dv.x * dv.x + dv.y*dv.y < 5 )
				break;
		}

		if( (j0 = i-10) < 0 )	j0 =0;
		if( (j1 = i1+10) >= gapp->no )	j1 = gapp->no -1;

		geo_approximate2( &p0, &p1, &gapp->v[j0], &gapp->tang[j0], j1 - j0 + 1, &Qgprm, &a );

		l = ln_create( &gapp->v[i], &gapp->v[i1], a );
		if( l == NULL )
			continue;

		l->p[0] = pl;
		if( pl != NULL )
			pl->p[1] = l;
		else	*link = l;
		pl = l;

		i = i1;

		v.y = -l->u.y;
		v.x = l->u.x;
	}

}




void
pt2d_approximate_linkN( pt2dA_type *apt, int ai[], int nI, vec2d *ctr, ln_type **link )
{
	pt2d_type	*pt;
	int	i,	j,	j0	,j1,	i1,	k;
	ln_type	*l,	*pl;
	vec2d	v,	p0,	p1;


	gapp_type	*gapp;
	gapp_prm	Qgprm;
	float	a;



	gapp = gapp_alloc( 4096 );

	for( i = 0, j = 0 ; i < apt->nP ;  i++ ){
		pt = &apt->p[i];

		gapp->v[j].x = pt->p.y;
		gapp->v[j].y = pt->p.x;

		gapp->tang[j] = pt->n;

		gapp->flag[j] = 0;
		j++;
	}
	gapp->no = j;

	Qgprm.dwight = 0;//0.10;
	Qgprm.err = 0.01;

	Qgprm.min_len = 30;//3.0;
	Qgprm.Min_pnumber = 4;
	Qgprm.max_len = 60.0;

	Qgprm.Max_dev = 5.0;
	Qgprm.Min_dev = 1.0;


	*link = NULL;

	*ctr = gapp->v[0];

	v = apt->p[0].n;

	for( i = 0, k = 1, pl = NULL ; i < gapp->no-1 && k < nI ; k++ ){
		i1 = pt2dA_get_next_break( apt, &v, i, 60 );
		if( i1 > ai[k] )	i1 = ai[k];




		p0 = gapp->v[i];
		p1 = gapp->v[i1];

		//if( i1 == apt->nGroup-1 ){
		//	dv.x = p1.x - p0.x;
		//	dv.y = p1.y - p0.y;
		//	if( dv.x * dv.x + dv.y*dv.y < 5 )
		//		break;
		//}

		if( (j0 = i-10) < 0 )	j0 =0;
		if( (j1 = i1+10) >= gapp->no )	j1 = gapp->no -1;

		geo_approximate2( &p0, &p1, &gapp->v[j0], &gapp->tang[j0], j1 - j0 + 1, &Qgprm, &a );

		l = ln_create( &gapp->v[i], &gapp->v[i1], a );
		if( l == NULL )
			continue;

		l->p[0] = pl;
		if( pl != NULL )
			pl->p[1] = l;
		else	*link = l;
		pl = l;


		i = i1;

		v.y = -l->u.y;
		v.x = l->u.x;
	}

	//	(*link)->p[0] = NULL;
}


static int	
pt2dA_get_next_break( pt2dA_type *apt, vec2d	*v, int i0, float dis )
{
pt2d_type	*pt0,	*pt;
vec2d	p;
float	t,	d,	tMin,	dMin;
int	i,	iMin;

	pt0 = &apt->p[i0];


	tMin = 100;
	iMin = -1;
	for( i = i0 ; i < apt->nP ; i++ ){
		pt = &apt->p[i];
		

		p.x = pt->p.x - pt0->p.x;
		p.y = pt->p.y - pt0->p.y;

		t = p.x * v->x + p.y * v->y; 

		if( t < 0 )	continue;

		d = hypot( p.x, p.y );
		
		if( d < 0.25*dis )	continue;

		if( iMin != -1 && dis > 1.25*dis )
			continue;

		t = d - dis;
		if( t < 0 )	t = -t;
		if( iMin == -1 || tMin > t ){
			iMin = i;
			tMin = t;
			dMin = d;

		}
	}


	if( iMin == -1 )
		return( apt->nP-1 );

	return(iMin);
}



pt2dA_type *
pt2d_from_polylink(vec2d *ctr, ln_type *link, float d )
{
	pt2dA_type	*tapt;
	ln_type *l;
	vec2d	p,	ctr0;
	float	t,	t0,	len;
	int	nP;



	for( l = link, len = 0 ; l != NULL ; l = l->p[F_END] )
		len += l->len;

	nP = (len + 0.35) / d;
	if( nP < 2 )	nP = 2;
	d = len / nP;

	tapt = pt2dA_alloc( nP+4 );

	tapt->nP = 0;
	tapt->p[tapt->nP].p.x = ctr->y;
	tapt->p[tapt->nP].p.y = ctr->x;
	tapt->nP++;

	for( t0 = 0, t = d, l = link, ctr0 = *ctr ; t < len ; t += d ){
		while( t > t0 + l->len ){
			t0 += l->len;
			ctr0.x += l->v.x;
			ctr0.y += l->v.y;
			l = l->p[F_END];
		}

		ln_t2xy( &ctr0, l, t-t0, &p );
		tapt->p[tapt->nP].p.x = p.y;
		tapt->p[tapt->nP].p.y = p.x;
		tapt->nP++;
	}

	ctr0.x += l->v.x;
	ctr0.y += l->v.y;

	if( t - t0 < 0.5*d )
		tapt->nP--;

	tapt->p[tapt->nP].p.x = ctr0.y;
	tapt->p[tapt->nP].p.y = ctr0.x;
	tapt->nP++;

	return( tapt );
}


void
pt2d_to_polylink( pt2dA_type *apt, vec2d *ctr, ln_type **link )
{
	ln_type	*l;
	vec2f_type	p,	p1;
	int	i;

	*link = lnL_alloc( apt->nP-1 );

	ctr->x = apt->p[0].p.y;
	ctr->y = apt->p[0].p.x;




	for( l = *link, p = *ctr, i = 1 ; l != NULL   ; l = l->p[F_END], i++ ){
		p1.x = apt->p[0].p.y;
		p1.y = apt->p[0].p.x;

		l->v.x = p1.x - p.x;
		l->v.y = p1.y - p.y;
		l->a = 0;

		ln_set_aux( l );

		p.x += l->v.x;
		p.y += l->v.y;
	}
}


void
pt2d_distance_from_polylink( pt2dA_type *apt, vec2f_type *ctr, ln_type *link )
{
pt2d_type	*pt;
ln_type *l;
vec2d	p;
float	t,	u,	gt;
int	i;


	for( i = 0 ; i < apt->nP ; i++ ){

		pt = &apt->p[i];

		p.x = pt->p.y;
		p.y = pt->p.x;
		lnL_distance( ctr, link, NULL, &p, &l, &u, &t, &gt );

		pt->r = gt;
	}
}



