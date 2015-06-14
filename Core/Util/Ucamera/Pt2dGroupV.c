/************************
 ***   Pt2dGroupV.c   ***
 ************************/
#include <math.h>

#ifdef _DEBUG
#define _DUMP
#endif


#include "Uigp/igp.h"

#include "Pt2dType.h"





pt2dGroupA_type *
pt2dA_group_v( pt2dA_type *apt, float d )
{
float	a,	t;
vec2d_type	av[360];
int	ai[360],	bi[360],	mi[360];
int	i,	j,	N;
int	i0,	i1,	i2;
float	m;

pt2dGroupA_type *ag;
pt2dGroup_type *g;


	for( i = 0, a = 0 ; a < 180 ; i++, a += d ){
		ai[i] = 0;
		av[i].x = cos( ANGLE_D2R(a) );
		av[i].y = sin( ANGLE_D2R(a) );
	}
	N = i;


	for( i = 0 ; i < apt->nP ; i++ ){
		pt2d_type	*pt = &apt->p[i];

		int	iMax = -1;
		float tMax = 0;
		for( j = 0 ; j < N ; j++ ){
			t = pt->n.x * av[j].x + pt->n.y * av[j].y;
			if( t < 0 )	t = -t;
			if( iMax < 0 || tMax < t ){
				iMax = j;
				tMax = t;
			}
		}

		ai[iMax]++;
		pt->group = iMax;
	}





	gpDump_int( ai, N, "group", 1, NULL );

	
#ifdef _AA_
	bi[0] = ai[N-1] + ai[0] + ai[1];
	bi[N-1] = ai[N-2] + ai[N-1] + ai[0];
	for( i = 1 ; i < N-1 ; i++ )
		bi[i] = ai[i-1] + ai[i] + ai[i+1];
#endif
	ai[N] = ai[0];
	ai[N+1] = ai[1];
	for( i = 1 ; i <= N ; i++ ){
		bi[i] = ai[i-1] + ai[i] + ai[i+1];
		if( bi[i] < 2 ){
			bi[i] = 0;
			mi[i] = 0;
			continue;
		}

		m = (ai[i-1]*-1 + ai[i]*0 + ai[i+1]*1)/(float)bi[i];
		if( m < -0.5 || m > 0.5 )
			mi[i] = 0;
		else	mi[i] = 1;
	}

	bi[0] = bi[N];
	mi[0] = mi[N];




	ag = pt2dGroupA_alloc( 20);


	for( i = 0 ; i < N ; i++ ){
		i0 = ( i > 0 )?i-1 : N-1;
		i1  = ( i < N-1)? i+1 : 0;
		
		if( (i1 = i+1) > N-1 )	i1 -= N;
		if( (i2 = i+2) > N-1 )	i2 -= N;


		if( mi[i] == 1 && ai[i] >= ai[i0] && ai[i] >= ai[i1] ){

			g = &ag->g[ag->nG++];
			g->no = bi[i];
			g->group = i;

			
			pt2dA_group_set_id( apt, apt->nP, i0, i );
			pt2dA_group_set_id( apt, apt->nP, i1, i );

	
//			pt2dA_group_nv3(  apt, i, i0, i1, &g->v );

		}

	}


	return( ag );
}



pt2dGroupA_type *
pt2dA_group_vG( pt2dA_type *apt, float d )
{
	float	a,	t;
	vec2d_type	av[360];
	int	ai[360],	bi[360],	mi[360];
	int	i,	j,	N;
	int	i0,	i1,	i2;
	float	m;

	pt2dGroupA_type *ag;
	pt2dGroup_type *g;


	for( i = 0, a = 0 ; a < 180 ; i++, a += d ){
		ai[i] = 0;
		av[i].x = cos( ANGLE_D2R(a) );
		av[i].y = sin( ANGLE_D2R(a) );
	}
	N = i;


	for( i = 0 ; i < apt->nP ; i++ ){
		pt2d_type	*pt = &apt->p[i];

		int	iMax = -1;
		float tMax = 0;
		for( j = 0 ; j < N ; j++ ){
			t = pt->n.x * av[j].x + pt->n.y * av[j].y;
			if( t < 0 )	t = -t;
			if( iMax < 0 || tMax < t ){
				iMax = j;
				tMax = t;
			}
		}

		ai[iMax]++;
		pt->group = iMax;
	}





	gpDump_int( ai, N, "group", 1, NULL );


	ai[N] = ai[0];
	ai[N+1] = ai[1];
	for( i = 1 ; i <= N ; i++ ){
		bi[i] = ai[i-1] + ai[i] + ai[i+1];
		if( bi[i] < 2 ){
			bi[i] = 0;
			mi[i] = 0;
			continue;
		}

		m = (ai[i-1]*-1 + ai[i]*0 + ai[i+1]*1)/(float)bi[i];
		if( m < -0.5 || m > 0.5 )
			mi[i] = 0;
		else	mi[i] = 1;
	}

	bi[0] = bi[N];
	mi[0] = mi[N];




	ag = pt2dGroupA_alloc( 20);


	for( i = 0 ; i < N ; i++ ){
		i0 = ( i > 0 )?i-1 : N-1;
		i1  = ( i < N-1)? i+1 : 0;

		if( (i1 = i+1) > N-1 )	i1 -= N;
		if( (i2 = i+2) > N-1 )	i2 -= N;


		if( mi[i] == 1 && ai[i] >= ai[i0] && ai[i] >= ai[i1] ){

			g = &ag->g[ag->nG++];
			g->no = bi[i];
			g->group = i;


			//			pt2dA_group_set_id( apt, apt->nP, i0, i );
			//			pt2dA_group_set_id( apt, apt->nP, i1, i );


			pt2dA_group_nv3(  apt, i, i0, i1, &g->v );

		}

	}



	for( i = 0 ; i < ag->nG-1 ; i++ ){
		if( ag->g[i].group +1 !=  ag->g[i+1].group )
			continue;

		if( ag->g[i].no <  ag->g[i+1].no )
			ag->g[i].no = 0;
		else ag->g[i+1].no = 0;
	}

	if( ag->g[0].group == 0 &&  ag->g[ag->nG-1].group == N-1 ){
		if( ag->g[0].no <  ag->g[ag->nG-1].no )
			ag->g[0].no = 0;
		else ag->g[ag->nG-1].no = 0;
	}

	return( ag );
}



static void	intAC_sum(  int a[], int nA, int n, int bi[] );


pt2dGroupA_type *
pt2dA_group_v1( pt2dA_type *apt, float d )
{

	float	t;
	int	a[360],	bi[360];
	int	i,	j;

	pt2dGroupA_type *ag;
	pt2dGroup_type *g;

	for( i = 0 ; i < 360 ; i++ )
		a[i] = 0;


	for( i = 0 ; i < apt->nP ; i++ ){
		pt2d_type	*pt = &apt->p[i];

		t = atan2( pt->n.y, pt->n.x ) + M_PI;

		j = ANGLE_R2D( t );
		j = PUSH_TO_RANGE( j, 0, 359 );

		a[j]++ ;
		
		pt->group = j;
	}



	intAC_sum(  a, 360, d+0.5,  bi );




	ag = pt2dGroupA_alloc( 20);

	for( i = 0 ; i < 360 ; i++ ){
		int i0 = ( i > 0 )?i-1 : 360-1;
		int i1  = ( i < 360-1)? i+1 : 0;

		if( bi[i] > bi[i0] && bi[i] > bi[i1] ){
			g = &ag->g[ag->nG++];
			g->no = bi[i];
			g->group = i;


			pt2dA_group_set_id( apt, apt->nP, i0, i );
			pt2dA_group_set_id( apt, apt->nP, i1, i );
		}

	}

	return( ag );
}


static void
intAC_sum(  int a[], int nA, int n, int bi[] )
{

	int	i,	j,	i0,	i1,	sum;
	int	N;



	sum = 0;
	for( i = nA - n ; i < nA ; i++ )
		sum += a[i];

	for( i = 0 ; i < n+1 ; i++ )
		sum += a[i];

	bi[0] = sum;


	N = 2*n+1;

	for( j = 1 ; j < nA ; j++, i++ ){
		i0 = i - N;
		if( i0 < 0 )	i0 += nA;
		if( ( i1 = i ) >= nA )	i1 -= nA;
		sum += a[i1] - a[i0];
		bi[j] = sum;
	}
}



static void
intAC_mk(  int a[], int nA, int d, int as[], float am[] )
{

	int	i,	j,	k;
	int	s,	sw,	w;
	float	m;



	for( i = 0 ; i < nA ; i++ ){
		
		w = sw = s = 0;

		for( k = -d ; k <= d ; k++ ){
			j = i + k;
			if( j < 0 )	j += nA;
			else if( j > nA )	j = j - nA;

			s += a[j];
			sw += a[j]*k;
			w += k;
		}

//		m = sw / w;
		m = -1;
		if( s != 0 )
			m = sw / (float)s;


		am[i] = ( m < -0.5 || m > 0.5 )? 0 : m;
		as[i] = s;
	}
}


pt2dGroupA_type *
pt2dA_group_vN( pt2dA_type *apt, float d )
{
	float	a,	t;
	vec2d_type	av[360];
	int	ai[360],	bi[360];//,	mi[360];
	int	i,	j,	N;
	int	i0,	i1,	i2,	d1;
	
	float am[360];


	pt2dGroupA_type *ag;
	pt2dGroup_type *g;


	for( i = 0, a = 0 ; a < 180 ; i++, a += d ){
		ai[i] = 0;
		av[i].x = cos( ANGLE_D2R(a) );
		av[i].y = sin( ANGLE_D2R(a) );
	}
	N = i;


	for( i = 0 ; i < apt->nP ; i++ ){
		pt2d_type	*pt = &apt->p[i];

		int	iMax = -1;
		float tMax = 0;
		for( j = 0 ; j < N ; j++ ){
			t = pt->n.x * av[j].x + pt->n.y * av[j].y;
			if( t < 0 )	t = -t;
			if( iMax < 0 || tMax < t ){
				iMax = j;
				tMax = t;
			}
		}

		ai[iMax]++;
		pt->group = iMax;
	}





	gpDump_int( ai, N, "group", 1, NULL );


	d1 = 6;
	intAC_mk(  ai, N,  d1, bi, am );




	ag = pt2dGroupA_alloc( 20);


	for( i = 0 ; i < N ; i++ ){
		i0 = ( i > 0 )?i-1 : N-1;
		i1  = ( i < N-1)? i+1 : 0;

		if( (i1 = i+1) > N-1 )	i1 -= N;
		if( (i2 = i+2) > N-1 )	i2 -= N;


		if( am[i] != 0  ){
			g = &ag->g[ag->nG++];
			g->no = bi[i];
			g->group = i;


			pt2dA_group_set_id( apt, apt->nP, i0, i );
			pt2dA_group_set_id( apt, apt->nP, i1, i );



		}

	}

	return( ag );
}
