
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include	"Uigp/igp.h"
#include	"Uln/PlnType.h"
#include	"Uvl/Vl2fType.h"



int	pln_split_straightline( pln_type *pl, float minLen, pln_type *apl[] );

static int	pt2dA_line_approximateA( pt2dA_type *apt, int i0, int i1, vl2f_type av[] );



plnA_type *
plnA_straightline_split( plnA_type *apl, float minLen, plnA_type *apl1 )
{
 vl2f_type vl;
int	i;

	apl1 = plnA_alloc( 4*apl->nA );


	for( i = 0 ; i <  apl->nA ; i++ ){

		if( pln_straightline( apl->a[i], 0, -1, 1, &vl ) > 0 ){
			if( vl.d < minLen )	continue;
			pln_type *spl = pln_from_vl( &vl );
			spl->color[0] = apl->a[i]->color[0];
			spl->color[1] = apl->a[i]->color[1];
	
			apl1->a[apl1->nA++] = spl;
			continue;
		}


		apl1->nA += pln_split_straightline( apl->a[i], minLen, &apl1->a[apl1->nA] );



	//	fprintf( stdout, ". " );
	}

	return( apl1 );
}





plnA_type *
	plnA_straightline_remove( plnA_type *apl, float minLen, plnA_type *apl1 )
{
	vl2f_type vl;
	int	i;

	apl1 = plnA_alloc( 4*apl->nA );


	for( i = 0 ; i <  apl->nA ; i++ ){

		if( pln_straightline( apl->a[i], 0, -1, 1, &vl ) > 0 ){
			continue;
		}


		plnA_type *tapl = plnA_alloc( 100 );
		tapl->nA += pln_split_straightline( apl->a[i], minLen, &tapl->a[tapl->nA] );
		if( tapl->nA > 0 ){
			plnA_destroy( tapl );
			continue;
		}

		plnA_destroy( tapl );

		apl1->a[apl1->nA++] = pln_copy( apl->a[i] );


		//	fprintf( stdout, ". " );
	}

	return( apl1 );
}







static int	pt2dA_line_approximateA_union( pt2dA_type *apt,  vl2f_type av[], int *nV );


int
pln_split_straightline( pln_type *pl, float minLen, pln_type *apl[] )
{
	static int iLine = 0;
	pt2dA_type *apt;

	apt = pln_sampleN( pl, 1.0, 1, NULL );


	int	i,	n,	nA;
	vl2f_type	av[100];

	PT2DA_DUMP( apt, "line-pt", iLine, NULL );

	nA = pt2dA_line_approximateA( apt, 0, apt->nP-1, av );

	pt2dA_line_approximateA_union( apt,  av,  &nA );


	int j;
	for( i = 0, j = 0 ; i < nA ; i++ ){
		if( av[i].d < minLen )	continue;
		av[j++] = av[i];
	}
	nA = j;


	//float T = cos( ANGLE_D2R(75) );
	//float t;
	//for( i = 0 ; i < nA-1 ; i++ ){
	//	t = VEC2D_INNER( av[i].v, av[i+1].v );

	//	if( ABS(t) > T )
	//		return( 0 );
	//}





	for( i = 0, n = 0 ; i < nA ; i++ ){
		if( av[i].e > 0.25 )	continue;
		if( av[i].d  < 2.5 )	continue;

		pln_type *spl = pln_from_vl_N( &av[i] );
		spl->color[0] = pl->color[0];
		spl->color[1] = pl->color[1];

		apl[n++] = spl;


	}





	return( n );
}







static int
pt2dA_line_approximateA( pt2dA_type *apt, int i0, int i1, vl2f_type av[] )
{
	float	dMax;
	int	no,	i,	n;



	vl2f_type vl;
	pt2d_approximate_line_vl( apt, i0, i1, &vl );

	if( vl.e < 0.25 ){//|| no < 3 ){
		vl.id = i0;
		av[0] = vl;

		return( 1 );
	}


	dMax = pt2d_approximate_line_pv_test( apt, i0, i1, &vl.p, &vl.v, 2.5, &no );


	//if( ABS(dMax) < 2.5 ){//|| no < 3 ){
	//	av[0] = vl;

	//	return( 1 );
	//}


	float	m,	sm;
	i = pt2d_approximate_line_pv_split( apt, i0, i1, &vl.p, &vl.v, &m, &sm );

	if(  i < i0+3 || i1 -i < 3 ){
		vl.id = i0;
		av[0] = vl;
		return( 1 );
	}


	n = pt2dA_line_approximateA( apt, i0, i-1, av );

	n += pt2dA_line_approximateA( apt, i+1, i1, &av[n] );

	return( n );
}




static int
	pt2dA_line_approximateA_union( pt2dA_type *apt,  vl2f_type av[], int *nV )
{
	int	i,	j;


	for( i = 1, j = 1 ; i < *nV ; i++ ){
		int i0 = av[j-1].id;
		int i1 = ( i < *nV-1 )? av[i+1].id : apt->nA-1;
	
		vl2f_type vl;
		pt2d_approximate_line_vl( apt, i0, i1, &vl );

		if( vl.e < 0.25 ){//|| no < 3 ){
			vl.id = i0;
			av[j-1] = vl;

			continue;
		}
		else {
			av[j++] = av[i];
		}
	}

	*nV = j;

	return( 1 );
}