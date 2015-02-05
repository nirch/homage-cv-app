/****************************
 ***   ArDetectLine.cpp   ***
 ****************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>

#ifdef _DEBUG
#define _DUMP 
#endif



#include	"Uigp/igp.h"
#include "Ulog/Log.h"
#include "Uvec/Vec3d.h"
#include "Uln/PlnType.h"
#include "Uvl/Vl2fType.h"

#include "Ucamera/Pt2dType.h"
#include "ImageType/ImageType.h"
#include "ImageMark/ImageMark.h"
#include "ImageDump/ImageDump.h"


#include "pEdgeType.h"
#include "plEdgeType.h"




static int	image_pEdge_line_1( image_type *rim, pEdge_type *dp00, pEdgePrm_type *prm, plEdge_type **ple );


static int	pEdge_line_approximate( pEdge_type *ad[200], int nD, pEdgePrm_type *prm, plEdge_type **ple );


static pt2dA_type *	apt_from_pEdge( pEdge_type *ad[200], int nD, pt2dA_type *apt );



static int iLine;



void	ln_update_end( ln_type *l0, ln_type *l1 );




int
pEdge_straight_line( image_type *dim, int d, pEdgePrm_type *prm, plEdgeA_type *aPl )
{
	pEdge_type	*dp;
	int	i,	j,	n;

	iLine = 0;




	dp = (pEdge_type *)dim->data;
	for( i = d ; i < dim->height-d ; i++ ){
		dp = (pEdge_type *)IMAGE_PIXEL( dim, i, d );
		for( j = d ; j < dim->width-d ; j++, dp++ ){
			if( dp->state != 1 )	continue;
			if( dp->ng[0] >= 0 )	continue;

			if( (n = image_pEdge_line_1( dim, dp, prm, &aPl->a[aPl->nA] )) > 0 ){
				aPl->nA++;
				
				plEdgeA_realloc( aPl, aPl->nA + 100 );
			}

		}
	}

//	plnA_dump( aPl, "aa", 1, "2" );


	return( 1 );
}




static int
image_pEdge_line_1( image_type *rim, pEdge_type *dp00, pEdgePrm_type *prm, plEdge_type **ple )
{
	pEdge_type	*ad[512];
	int	nD;

	pEdge_type	*rp;
	int	arp[9];
	int	ret;


	arp[0] = -rim->width - 1;
	arp[1] = -rim->width;
	arp[2] = -rim->width + 1;

	arp[3] = - 1;
	arp[4] = 0;
	arp[5] = + 1;

	arp[6] = rim->width - 1;
	arp[7] = rim->width;
	arp[8] = rim->width + 1;




	for( rp = dp00, nD = 0 ; rp != NULL ;  ){
		ad[nD++] = rp;

		if( rp->ng[1] < 0 )	break;
		rp = rp + arp[ rp->ng[1] ];
	}


	if( nD <= 3 )	return( -1 );


	ret = pEdge_line_approximate( ad, nD, prm, ple );

	if( ret > 0 )
		(*ple)->rp = dp00;


	return( ret );

}





static int
pEdge_line_approximate( pEdge_type *ad[200], int nD, pEdgePrm_type *prm, plEdge_type **ple )
{
pt2dA_type *apt;
pln_type	*pl;
int	i;


	apt = apt_from_pEdge( ad, nD, NULL );

	vl2f_type vl;

	pt2d_approximate_line_vl( apt, 0, apt->nP-1, &vl );

	if( vl.d  < prm->lenMin ){
		pt2dA_destroy( apt );
		return( -1 );
	}

	if( vl.e > 0.15 ){
		pt2dA_destroy( apt );
		return( -1 );
	}


	float	av,	dMax;
	pt2d_approximate_line_vl_test( apt, 0, apt->nP-1, &vl, &av, &dMax );

	pt2dA_destroy( apt );

	if( av > 0.5 )
		return( -1 );



	pl = pln_from_vl( &vl );




	float	sum = 0;
	int n = 0;
	for( i = 0 ; i < nD ; i++ ){
		sum += ad[i]->g;

		if( ad[i]->nStatic > 10 )
			n++;
	}


	if( n > 0.75* nD )
		pl->state |= PLN_WEEK;


	*ple = plEdge_alloc();
	(*ple)->grad = sum / nD;
	(*ple)->pl = pl;
	pl->qulity = (*ple)->grad;

//	fprintf( stdout, "%.2f\n", pl->qulity );




	return( 1 );

}







static pt2dA_type *
apt_from_pEdge( pEdge_type *ad[200], int nD, pt2dA_type *apt )
{
pEdge_type	*dp;
pt2d_type	*pt;
int	i;


	apt = pt2dA_realloc( apt, nD );

	for( i = 0 ; i < nD ; i++ ){
		dp = ad[i];

		pt = &apt->p[i];

		pt->p = dp->p;
		pt->n = dp->v;
	}

	apt->nP = nD;
	return( apt );
}





int
pEdge_straight_line( image_type *rim, pEdge_type *rp0,  pEdge_type *rp1, pEdgePrm_type *prm, vl2f_type *vl )
{
pEdge_type *ad[1024];
int	nD;

	nD = pEdge_linking_get( rim, rp0, ad );

	nD += pEdge_linking_get( rim, rp1, &ad[nD] );


	pt2dA_type *apt;

	apt = apt_from_pEdge( ad, nD, NULL );


	pt2d_approximate_line_vl( apt, 0, apt->nP-1, vl );


	if( vl->e > 0.15 ){
		pt2dA_destroy( apt );
		return( -1 );
	}


	float	av,	dMax;
	pt2d_approximate_line_vl_test( apt, 0, apt->nP-1, vl, &av, &dMax );

	PT2DA_DUMP( apt, "reap", 1, "AA" );

	pt2dA_destroy( apt );

	if( av > 0.5 )
		return( -1 );

	return( 1 );
}
