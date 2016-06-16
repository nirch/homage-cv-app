
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#ifdef _DEBUG
 
#endif
#define _DUMP


#include	"Uigp/igp.h"
#include	"Utime\GpTime.h"
#include	"Ulog/Log.h"

#include "Uln/PlnType.h"



pt2dA_type *	pln_smooth( pln_type *pl, int d, pt2dA_type *apt );

static pt2dA_type *	pt2dA_smooth_open( pt2dA_type *apt0, int d, pt2dA_type *apt );


void	pt2dA_smooth_filter_degenerate( pt2dA_type *apt, pt2dA_type *apt1 );

int	pt2d_smooth_degenerate( pt2d_type *pt0, pt2d_type *pt1 );



int
plnA_smoothA( plnA_type *apl, int d )
{
int	i;

	for( i = 0 ; i < apl->nA ; i++ ){
		pln_type *pl = pln_smoothA( apl->a[i], d );

		pln_destroy( apl->a[i] );
		apl->a[i] = pl;
	}

	return( 1 );
}




static pt2dA_type *	pt2dA_smooth_close( pt2dA_type *apt0, int d, pt2dA_type *apt );


pln_type *
pln_smoothA( pln_type *pl, int d )
{

	PLN_DUMP( pl, "AA", 1, NULL );
	pt2dA_type *apt = pt2dA_alloc( (int)pl->len + 3*d );

	
	pln_sampleN( pl, 1.0, 1.0, apt );

	apt->axis = PT2D_AXIS_XY;

	pln_type *spl;
	pt2dA_type *apt1 = NULL;
	if( pl->state == PLN_CLOSE ){
		apt1 = pt2dA_smooth_close( apt, d, apt1 );
		//pln_approximateA( apt1, 1, &spl );
	}
	else {
		apt1 = pt2dA_smooth_open( apt, d, apt1 );
		//pln_approximateA( apt1, 0, &spl );
	}


	//pt2d_smooth_degenerate( &apt->a[872], &apt->a[873] );


	PT2DA_DUMP( apt, "smooth", 1, "src" );
	PT2DA_DUMP( apt1, "amooth", 1, "out" );


	pt2dA_smooth_filter_degenerate( apt, apt1 );
	PT2DA_DUMP( apt1, "amooth", 1, "out-d" );

	
	pln_approximateA( apt1,  pl->state == PLN_CLOSE, &spl );

	pt2dA_destroy( apt );
	pt2dA_destroy( apt1 );

	spl->size = pl->size;

	return( spl );

}



static pt2dA_type *
pt2dA_smooth_close( pt2dA_type *apt0, int d, pt2dA_type *apt )
{
	int	i,	k;



	int n2 = 2*d+1;


	
	
	apt = pt2dA_realloc( apt, apt0->NA );
	apt->nA = apt0->nA;
	apt->axis = apt0->axis;

	for( i = 0 ; i < n2 ; i++ )
		apt0->a[apt0->nA+i] = apt0->a[i];

	apt0->nA += n2;

	vl2f_type vl;
	for( i = 0, k = d ; i < apt->nA ; i++, k++ ){

		pt2d_approximate_line_vl( apt0, i, i+n2, &vl );


		if( k >= apt->nA )
			k -= apt->nA;

		pt2d_type *pt0 = &apt0->a[k];
		float t = vl.v.x*( pt0->p.x - vl.p.x) +  vl.v.y*( pt0->p.y - vl.p.y);


		pt2d_type *pt = &apt->a[k];
		pt->p.x = vl.p.x + t *vl.v.x;
		pt->p.y = vl.p.y + t *vl.v.y;

		pt->n.x = -vl.v.y;
		pt->n.y =  vl.v.x;

		pt0->n.x = pt->p.x - pt0->p.x;
		pt0->n.y = pt->p.y - pt0->p.y;

	}

	apt0->nA -= n2;

	


	//apt->axis = PT2D_AXIS_YX;
	//PT2DA_DUMP( apt, "AA", 1, NULL );

	return( apt );

}



static pt2dA_type *
	pt2dA_smooth_open( pt2dA_type *apt0, int d, pt2dA_type *apt )
{
	int	i,	k;



	int n2 = 2*d+1;




	apt = pt2dA_realloc( apt, apt0->NA );
	apt->nA = apt0->nA;
	apt->axis = apt0->axis;

	//for( i = 0 ; i < n2 ; i++ )
	//	apt0->a[apt0->nA+i] = apt0->a[i];

	//apt0->nA += n2;

	vl2f_type vl;
	for( i = 0, k = d ; i < apt->nA ; i++, k++ ){
		int	i0 = ( i - d < 0 )? 0 : i - d;
		if( i0 + n2 > apt->nA )	i0 = apt->nA - n2;

	//	pt2d_approximate_line_vl( apt0, i, i+n2, &vl );
		pt2d_approximate_line_vl( apt0, i0, i0+n2, &vl );



		pt2d_type *pt0 = &apt0->a[i];
		float t = vl.v.x*( pt0->p.x - vl.p.x) +  vl.v.y*( pt0->p.y - vl.p.y);


		pt2d_type *pt = &apt->a[i];
		pt->p.x = vl.p.x + t *vl.v.x;
		pt->p.y = vl.p.y + t *vl.v.y;

		pt->n.x = -vl.v.y;
		pt->n.y =  vl.v.x;

	}



	return( apt );

}


void
pt2dA_smooth_filter_degenerate( pt2dA_type *apt, pt2dA_type *apt1 )
{
	int i,	j;


	for( i = 0, j = 0 ; i < apt->nA-1 ; i++ ){
		pt2d_type *pt0 = &apt->a[i];
		pt2d_type *pt1 = &apt->a[i+1];

		if( pt2d_smooth_degenerate( &apt->a[i], &apt->a[i+1] ) < 0 ){
			apt1->a[j++] = apt1->a[i];
			continue;
		}

		i++;
		
	}

	if( i < apt->nA )
		apt1->a[j++] = apt1->a[i];

	apt1->nA = j;
}


int
pt2d_smooth_degenerate( pt2d_type *pt0, pt2d_type *pt1 )
{

	vec2f_type dp;
	dp.x = pt1->p.x - pt0->p.x;
	dp.y = pt1->p.y - pt0->p.y;

	vec2f_type bu;
	VEC2D_RIGHT( pt0->n, bu );

	float t0 = VEC2D_INNER( dp, bu );
	float t1 = t0 + VEC2D_INNER( pt1->n, bu );
	if( (t0<0) == (t1< 0) )
		return( -1 );


	vec2f_type bu1;
	VEC2D_RIGHT( pt1->n, bu1 );
	t0 = -VEC2D_INNER( dp, bu1 );
	t1 = t0 + VEC2D_INNER( pt0->n, bu1 );
	if( (t0<0) == (t1< 0) )
		return( -1 );

	return( 1 );
}

#ifdef _AA_
pt2dA_type *
	pln_smooth( pln_type *pl, int d, pt2dA_type *apt )
{
	int	i;

	vl2f_type	tv;

	apt = pt2dA_realloc( apt, (int)pl->len + 10 );

	for( i = 0 ; i < pl->len ; i++ ){

		pln_pv_d( pl, i-d,  i+d, 1.0, &tv );

		pt2d_type *pt = &apt->a[apt->nA++];

		vec2f_type p;
		pln_gt2p( pl, i, &p );
		float t = tv.v.x*( p.x - tv.p.x) +  tv.v.y*( p.y - tv.p.y);

		pt->p.x = tv.p.x + t *tv.v.x;
		pt->p.y = tv.p.y + t *tv.v.y;

		//	pt->p = p;//tv.p;
	}


	PLN_DUMP( pl, "AA", 1, NULL );

	apt->axis = PT2D_AXIS_YX;
	PT2DA_DUMP( apt, "AA", 1, NULL );

	return( apt );

}
#endif