
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#ifdef _DEBUG
#define _DUMP 
#endif


#include	"Uigp/igp.h"
#include	"Ulog/Log.h"



#include "Uln/PlnType.h"
#include "Umath/EigenType.h"


#include	"ImageType/ImageType.h"
#include	"ImageMark/ImageMark.h"
#include	"ImageDump/ImageDump.h"



static int		pln_thin( pln_type *pl, plnA_type *bapl, float ht[2], int flagIn, int nT, int iFrame );




pt2dA_type *	pt2dA_max( pt2dA_type *apt );

static int		pt2dA_delete( pt2dA_type *apt, pt2dA_type *mapt, int checkThin, float dT, int in );

static int		pt2dA_delete_n( pt2dA_type *apt, pt2dA_type *mapt, float dT, int nT );

static int	pt2dA_thin_in( pt2dA_type *apt, int i0, int i1 );

static int	pt2dA_check_thin_dirction( pt2dA_type *apt, int i0, int i1, float dT );




void	pt2dA_crop( pln_type *pl, pt2dA_type *apt );

int	pt2dA_update_2( pln_type *pl, pt2dA_type *apt, int *i0, int *i1 );



int	pt2dA_distanceP( pt2dA_type *apt, int i0, float dT, float *dis );

int	pt2dA_distanceT( pt2dA_type *apt, int i0, int k0, int k1, float dT, float *dis );


int	pt2dA_check_thin( pt2dA_type *apt, int i0, int i1, float dT );

void	pt2dA_set_id( pt2dA_type *apt,int k0, int k1, int id );

static int	pt2dA_nId( pt2dA_type *apt,int i0, int i1, int id );
static int	pt2dA_nId( pt2dA_type *apt,int i0, int i1 );


static int	pt2dA_in( pt2dA_type *apt,int i0, int i1 );


static int	pt2dA_in_apl( pt2dA_type *apt, float T, plnA_type *apl );

static int	pt2dA_in_head( pt2dA_type *apt, float ht[2] );


static int	pt2dA_in_head_get( pt2dA_type *apt,int i0, int i1  );



int
	plnA_adjust_thin( plnA_type *apl, plnA_type *bapl, float ht[2], int iFrame )
{



	if( apl->nA <= 0 || apl->a[0]->len < 40 )
		return( 1 );

	PLNA_DUMP( apl, "thin", iFrame, "1" );

	int ret = pln_thin( apl->a[0], bapl, ht, 0, 500, iFrame );
	PLNA_DUMP( apl, "thin", iFrame, "2" );
	if( ret > 0 ){
		pln_thin( apl->a[0], bapl, ht, 0, 0, iFrame );
		PLNA_DUMP( apl, "thin", iFrame, "3" );
	}

	pln_thin( apl->a[0], bapl, ht, 1, 0, iFrame );
	PLNA_DUMP( apl, "thin", iFrame, "4" );
	return( ret );
}




int
pln_thin( pln_type *pl, plnA_type *bapl, float ht[2], int flagIn, int nT, int iFrame )
{


	int n = pl->len;


	if( n < 40 )	return( -1 );


	pt2dA_type *apt = pt2dA_alloc( n +10 );
	pln_sampleN( pl, 1, 1, apt );
	pt2dA_curvtur( apt );

	pt2dA_set_groupId( apt, 0 );

	if( bapl != NULL )
		pt2dA_in_apl( apt, 16, bapl );

	if( ht[0] >= 0 )
		pt2dA_in_head( apt, ht );


	apt->nA -= 20;

	apt->type = PT2D_4;

	PT2DA_DUMP( apt, "thin", iFrame, "in" );

	pt2dA_type *capt = pt2dA_max( apt );

	capt->type = PT2D_4V;
	PT2DA_DUMP( capt, "thin", iFrame, "extremom" );

	int	k;
	if( flagIn == 0 ){
		k = pt2dA_delete( apt, capt, 1, 8.0, 1 );

		k += pt2dA_delete( apt, capt, 1, 8.0, 0 );


		k += pt2dA_delete_n( apt, capt, 8.0, nT );
	}
	else 
		k = pt2dA_delete( apt, capt, 0, 8.0, 1 );



	if( k > 0 ){
		pt2dA_crop(pl, apt );
		PLN_DUMP( pl, "thin", iFrame, "out" );
	}

	pt2dA_destroy( apt );
	pt2dA_destroy( capt );


	return( k );

}




int	pt2dA_update_start( pt2dA_type *apt, int *i1 );

int	pt2dA_update_end( pt2dA_type *apt, int *i0 );


void
pt2dA_crop( pln_type *pl, pt2dA_type *apt )
{
	int	i;

	for( i = apt->nA-1 ; i >= 0 ; ){
		for( ; i > 0 && apt->a[i].id >= 0 ; i-- );


		if( i <= 0  )	return;
		
		int i0 = i;

		for( ; i > 0 && apt->a[i].id < 0 ; i-- );
		int i1 = i+1;


		if( i1 <= 1 ){
			pt2dA_update_end( apt, &i0 );
			float t = apt->a[i0].f; 
			pln_trim( pl, F_BEGIN, t );
			pln_close( pl, 1.0 );
			return;
		}


		pt2dA_update_start( apt, &i1 );
		if( i0 == apt->nA - 1 ){
	
			float t = pl->len - apt->a[i1].f;
			pln_trim( pl, F_END, t );
			i = i1 - 1;
			for( ; i > 0 && apt->a[i].id < 0 ; i-- );

			continue;
		}

		pt2dA_update_end( apt, &i0 );
		

		pt2dA_update_2( pl, apt, &i0, &i1 );

		pln_type *pl1 = pln_split( pl, apt->a[i0].f, 0.5 );


		float t;
		if( i1 <= 0 )
			t = apt->a[i0].f - 1.0;
		else t = apt->a[i0].f - apt->a[i1].f;
			

		pln_trim( pl, F_END, t );

		if( pl1 != NULL )
			pln_append( pl, pl1 );
		else pln_close( pl, 1.0 );


		if( i >= i1 )
			i = i1 - 1;

	}
}

int
	pt2dA_update_start( pt2dA_type *apt, int *i1 )
{
	int i,	i0,	iMax;
	float t,	tMax;
	i0 = *i1 - 4;
	iMax = -1;
	if( i0 < 1 )	i0 = 1;
	for( i = *i1 ; i > i0 ; i-- ){
		t = VEC2D_INNER( apt->a[i].n, apt->a[i-1].n );
		if( iMax < 0 || t > tMax ){
			tMax = t;
			iMax = i;
		}
	}

	*i1 = iMax;

	return( 1 );
}

int
	pt2dA_update_end( pt2dA_type *apt, int *i0 )
{
	int i,	i1,	iMax;
	float t,	tMax;
	i1 = *i0 + 4;
	iMax = -1;
	if( i1 >= apt->nA )	i1 = apt->nA-1;
	for( i = *i0 ; i < i1 && i < apt->nA ; i++ ){
		t = VEC2D_INNER( apt->a[i].n, apt->a[i+1].n );
		if( iMax < 0 || t > tMax ){
			tMax = t;
			iMax = i;
		}
	}

	if( iMax < 0 )
		return( 1 );

	*i0 = iMax;

	return( 1 );
}


int
	pt2dA_update_2( pln_type *pl, pt2dA_type *apt, int *i0, int *i1 )
{
int k;
vec2f_type	v,	n0,	n1;

	float	t0,	t1;

	for( k = 0 ; k < 4 ; k++ ){

		
		v.x = apt->a[*i0].p.x - apt->a[*i1].p.x;
		v.y = apt->a[*i0].p.y - apt->a[*i1].p.y;


		VEC2D_NORMALIZE( v );
		VEC2D_SWAP(v );

		pln_tanget( pl, apt->a[*i0].f, &n0 );
		pln_tanget( pl, apt->a[*i1].f, &n1 );

		t0 = VEC2D_INNER( n0, v );
		t1 = VEC2D_INNER( n1, v );


		if( t0 > -0.5 && t1 > -0.5 )
			return( 1 );


		if( t0 < t1 )
			(*i0)++;
		else (*i1)--;



	}


	return( 1 );
}


pt2dA_type *
pt2dA_max( pt2dA_type *apt )
{
	pt2dA_type	*capt = pt2dA_alloc( apt->nA );
	int	i;

	for( i = 10 ; i < apt->nA - 10 ; i++ ){
		pt2d_type *pt = &apt->a[i];

		if( ABS(pt->r) < 0.1 )	continue;

		if( ABS(pt->r) > ABS( apt->a[i-1].r) && ABS(pt->r) > ABS( apt->a[i+1].r) ){
			capt->a[capt->nA] = *pt;
			capt->a[capt->nA].id = i;
			capt->nA++;
		}
	}

	return( capt );
}

int	pt2dA_distanceA( pt2dA_type *apt, int i0, float dT, float *dis );


static int 
pt2dA_delete( pt2dA_type *apt, pt2dA_type *mapt, int checkThin, float dT, int in )
{
int i,	j,	n;
float dis;

	for( i = 0, n = 0 ; i < mapt->nA ; i++ ){
		pt2d_type *pt = &mapt->a[i];
		if( pt->id < 0 )	continue;

		int k = pt2dA_distanceA( mapt, i, dT, &dis );
		
		if( k < 0 )		continue;




		if( checkThin == 1 && pt2dA_check_thin( apt, pt->id, mapt->a[k].id, dT ) < 0 )
			continue;
	

		if( in == 1 &&  pt2dA_thin_in( apt, pt->id, mapt->a[k].id ) < 0 )
			continue;

		if(  mapt->a[k].id - pt->id > 0.5 *apt->nA )
			continue;

		int i0 = pt->id;
		int i1 = mapt->a[k].id;
		if( i0 + apt->nA - i1 <  1.1*(i1 - i0) ||  i1 - i0 > 750){




			eigen2d_type e0,	e1;
			pt2dA_eigen2d(apt, i0, i1,  &e0 );
			pt2dA_eigen2d(apt, i1, i0,  &e1 );

			if( e1.r < e0.r ){
				continue;
				int tmp = i0;
				i0 = i1;
				i1 = tmp;
			}
		}


		int inHead0 = pt2dA_in_head_get( apt, i0, i1 );
		int inHead1 = pt2dA_in_head_get( apt, i1, i0 );

		if( i1-i0 > 400 && inHead0 >  inHead1 ){
			int tmp = i0;
			i0 = i1;
			i1 = tmp;
		}


		//for( j = pt->id ; j <= mapt->a[k].id ; j++ )
		//	apt->a[j].id = -1;


		pt2dA_set_id( apt, i0, i1, -1 );

		float dt = pt->f - mapt->a[k].f;
//		if( ABS( dt ) < 2*dT )	continue;
#ifdef _DEBUG
		GPLOG( ("%d %d   %f %f\n", i, k, dis, dt) );
#endif

		for( j = i+1 ; j < k ; j++ ){
			mapt->a[j].id = -1;
		}
		i = k;

		n++;
	}

	return( n );
}



static int
pt2dA_thin_in( pt2dA_type *apt, int i0, int i1 )
{
int	i,	no;

	vec2f_type	v,	u;
	v.x = apt->a[i1].p.x - apt->a[i0].p.x;
	v.y = apt->a[i1].p.y - apt->a[i0].p.y;
	VEC2D_NORM2( v );
	VEC2D_LEFT( v, u );
	VEC2D_NORMALIZE( u );

	float d = VEC2D_INNER( apt->a[i0].p, u );

	int n = no = 0;
	for( i = i0 + 1 ; i <= i1-1 ; i++ ){
		pt2d_type *pt = &apt->a[i];

		float t =  VEC2D_INNER( pt->p, u ) - d;
		if( t > 0 )
			n++;
		no++;
	}

	if( n > 0.5*no )
		return( 1 );

	return( -1 );
}




int
pt2dA_distanceA( pt2dA_type *apt, int i0, float dT, float *dis )
{
	vec2d	v;
	float	d;
	int	i,	iMin;

	pt2d_type *pt0 = &apt->p[i0];

	iMin = -1;
	*dis = dT*dT;
	for( i = i0+1 ; i < apt->nP ; i++ ){
		pt2d_type *pt = &apt->p[i];

		if( pt->id < 0 )	continue;

		float dt = pt->f - pt0->f;
		if( dt < 2*dT )	continue;  

		v.x = pt->p.x - pt0->p.x;
		v.y = pt->p.y - pt0->p.y;

		d = v.x*v.x + v.y*v.y;
		if(  d > *dis && d > dT*dT )
			continue;

		if( dt*dt < 4*d )
			continue;

		*dis = d;
		iMin = i;
	}

	
	return( iMin );
}


int
pt2dA_distanceP( pt2dA_type *apt, int i0, float dT, float *dis )
{
	vec2d	v;
	float	d;
	int	i,	iMin;

	pt2d_type *pt0 = &apt->p[i0];

	iMin = -1;
	*dis = dT*dT;
	for( i = 0 ; i < apt->nP ; i++ ){
		pt2d_type *pt = &apt->p[i];

//		if( pt->id < 0 )	continue;

		float dt = pt->f - pt0->f;
		if( ABS(dt) < 2*dT )	continue;  

		v.x = pt->p.x - pt0->p.x;
		v.y = pt->p.y - pt0->p.y;

		d = v.x*v.x + v.y*v.y;
		if(  d > *dis || d > dT*dT )
			continue;

		if( dt*dt < 4*d )
			continue;

		*dis = d;
		iMin = i;
	}

	
	return( iMin );
}


static int 
pt2dA_delete_n( pt2dA_type *apt, pt2dA_type *mapt, float dT, int nT )
{
int i,	n;
float dis;



	for( i = 0, n = 0 ; i < mapt->nA ; i++ ){
		pt2d_type *pt = &mapt->a[i];
		if( pt->id < 0 )	continue;

		if( apt->a[pt->id].id < 0 )
			continue;

		if( ABS( pt->r ) < 0.1 )
			continue;




		int k = pt2dA_distanceP( apt, pt->id, dT+2, &dis );		
		if( k < 0 )		continue;

		int i0 = pt->id;
		int i1 = k;

		if( i1 < i0 ){
			int tmp = i0;
			i0 = i1;
			i1 = tmp;
		}



		int n0 = pt2dA_nId( apt, i0,  i1 );
		int n1 = pt2dA_nId( apt, i1,  i0 );


		int in0 = pt2dA_in( apt, i0,  i1 );
		int in1 = pt2dA_in( apt, i1,  i0 );

		int inHead0 = pt2dA_in_head_get( apt, i0, i1 );
		int inHead1 = pt2dA_in_head_get( apt, i1, i0 );


		if( nT > 0 &&  n1 > nT && n0 > nT )
			continue;


		if(  n1 < 0.75*n0 ||  n0 > 750 && n1 > 750 ){

			if( in1 < in0 || in1 == in0 && n1 < 0.5 * n0 || n0 > 400 && inHead0 >  inHead1 ){
				int tmp = i0;
				i0 = i1;
				i1 = tmp;
			}
		}
		else {
			if( n0 > 400 && inHead0 >  inHead1 ){
				int tmp = i0;
				i0 = i1;
				i1 = tmp;
			}
		}



		if( pt2dA_check_thin( apt, i0, i1, dT ) < 0 &&  pt2dA_check_thin( apt, i1, i0, dT ) < 0)
			continue;





		
		pt2dA_set_id( apt, i0, i1, -1 );



		n++;

	}


	return( n );
}


#define D	8
int
pt2dA_check_thin( pt2dA_type *apt, int i0, int i1, float dT )
{
	float dis0,	dis1,	dis2,	dis3;

	vec2f_type	dp;
	dp.x = apt->a[i1].p.x - apt->a[i0].p.x;
	dp.y = apt->a[i1].p.y - apt->a[i0].p.y;
	dis0 = dp.x*dp.x + dp.y*dp.y;

	int k1 = pt2dA_distanceT( apt, i0 + D, i1 - 15, i1, 10* dT, &dis1 );
	if( k1 < 0 )	return( -1 );

	if( i1 > i0 && i1 - i0 < 40 ){
		if( dis1 > dis0 )
			return( -1 );
		return( 1 );
	}

	int k2 = pt2dA_distanceT( apt, i0 + 2*D, i1 -25, i1, 10* dT, &dis2 );
	if( k2 < 0 )	return( -1 );

	if( dis2 > 2 * dis0 && dis2 > dT*dT)
		return( -1 );


	if( i1 > i0 && i1 - i0 < 80 ){
		if( dis2 > 2 * dis0 && dis2 > dT)
			return( -1 );
		return( 1 );
	}

	int k3 = pt2dA_distanceT( apt, i0 + 3*D, i1 -35, i1, 10* dT, &dis3 );
	if( k3 < 0 )	return( -1 );

	if( dis3 > 2 * dis0 && dis3 > dT*dT)
		return( -1 );


	return( 1 );
}



static int
pt2dA_check_thin_dirction( pt2dA_type *apt, int i0, int i1, float dT )
{
	float dis0,	dis1,	dis;
	int	i,	k;


	
	for( i = 0, dis0 = 0; i < 3 ; i++ ){
		k = pt2dA_distanceT( apt, i0 + (i+1)*D, i1 - (i+1)*D, i1, 100* dT, &dis );
		dis0 += ( k < 0 )? 100:dis;
	}

	for( i = 0, dis1 = 0; i < 3 ; i++ ){
		k = pt2dA_distanceT( apt, i0 - (i+1)*D, i1 , i1+ (i+1)*D, 100* dT, &dis );
		dis1 += ( k < 0 )? 100:dis;
	}


	if( dis0 < dis1 )
		return( 1 );

	return( -1 );
}


	


int
pt2dA_distanceT( pt2dA_type *apt, int i0, int k0, int k1, float dT, float *dis )
{
	vec2d	v;
	float	d;
	int	i,	iMin;

	if( i0 >= apt->nA )	i0 = apt->nA - 1;
	if( k0 < 0 )	k0 = 0;
	if( k1 >= apt->nA )	k1 = apt->nA - 1;

	pt2d_type *pt0 = &apt->p[i0];

	

	iMin = -1;
	*dis = dT*dT;
	for( i = k0 ; i != k1 ; i++ ){
		if( i >= apt->nA )	i = 0;
		pt2d_type *pt = &apt->p[i];

		//		if( pt->id < 0 )	continue;

		float dt = pt->f - pt0->f;
		//if( ABS(dt) < 2*dT )	continue;  

		v.x = pt->p.x - pt0->p.x;
		v.y = pt->p.y - pt0->p.y;

		d = v.x*v.x + v.y*v.y;
		if(  d > *dis || d > dT*dT )
			continue;

		if( dt*dt < 4*d )
			continue;

		*dis = d;
		iMin = i;
	}


	return( iMin );
}


void
pt2dA_set_id( pt2dA_type *apt,int i0, int i1, int id )
{
	int	i;
	if( i1 == 0 )	 i1 = apt->nA;
	for( i = i0 ; i != i1 ; i++ ){
		if( i >= apt->nA )	i = 0;
		apt->a[i].id = id;
	}
}


static int
pt2dA_nId( pt2dA_type *apt,int i0, int i1, int id )
{
	int	i,	n;
	if( i1 == 0 )	 i1 = apt->nA;
	for( i = i0, n = 0 ; i != i1 ; i++ ){
		if( i >= apt->nA )	i = 0;
		if( apt->a[i].id == id )
			n++;
	}

	return( n );
}

static int
pt2dA_nId( pt2dA_type *apt,int i0, int i1 )
{
	int	i,	n;
	if( i1 == 0 )	 i1 = apt->nA;
	for( i = i0, n = 0 ; i != i1 ; i++ ){
		if( i >= apt->nA )	i = 0;
		if( apt->a[i].id >= 0 )
			n++;
	}

	return( n );
}



static int
pt2dA_in( pt2dA_type *apt,int i0, int i1 )
{
	int	i,	n;
	if( i1 == 0 )	 i1 = apt->nA;
	for( i = i0, n = 0 ; i != i1 ; i++ ){
		if( i >= apt->nA )	i = 0;

		if( apt->a[i].group > 0 )
			n++;
	}

	return( n );
}


static int
	pt2dA_in_head_get( pt2dA_type *apt,int i0, int i1  )
{
	int	i,	n;
	if( i1 == 0 )	 i1 = apt->nA;
	for( i = i0, n = 0 ; i != i1 ; i++ ){
		if( i >= apt->nA )	i = 0;

		if( apt->a[i].group == 2  )
			n++;
	}

	return( n );
}



static int
pt2dA_in_apl( pt2dA_type *apt, float T, plnA_type *apl )
{
	int	i,	n;


	for( i = 0, n = 0 ; i < apt->nA ; i++ ){
		if( i >= apt->nA )	i = 0;

		pt2d_type *pt = &apt->a[i];

		vec2f_type p;

		p.x = pt->p.y;
		p.y = pt->p.x;

		dPln_type	d;
		pln_type *spl;
		if( plnA_distance( apl, &p, 2*T, &spl, &d ) < 0 )
			continue;


		pt->group = 1;
		n++;
	}

	return( n );
}



static int
pt2dA_in_head( pt2dA_type *apt, float ht[2] )
{
	int	i,	n;

	if( ht[0] < 0 )
		return( 1 );


	for( i = 0, n = 0 ; i < apt->nA ; i++ ){
		if( i >= apt->nA )	i = 0;

		pt2d_type *pt = &apt->a[i];

		if( pt->f >= ht[0] && pt->f <= ht[1] ){
			pt->group = 2;
			n++;
		}
	}

	return( n );
}