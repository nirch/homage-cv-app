/**************************
 ***   pRidgeLine.cpp   ***
 **************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>

#include	"Uigp/igp.h"
#include "Ulog/Log.h"
#include "Uvec/Vec3d.h"
#include "Uln/PlnType.h"

#include "Ucamera/Pt2dType.h"
#include "ImageType/ImageType.h"
#include "ImageMark/ImageMark.h"
#include "ImageDump/ImageDump.h"


#include "pRidgeType.h"
#include "plRidgeType.h"



#ifdef _DEBUG
#define _DUMP 
#endif


static pt2dA_type *	apt_from_pRidge( pRidge_type *ad[200], int nD, pt2dA_type *apt );

static plRidge_type *	plRidge_set(  pRidge_type *arp[], int nRp, pRidgeLinePrm_type *prm );



int
pRidge_line( image_type *dim, int d,  pRidgeLinePrm_type *prm, plRidgeA_type *ar )
{
	pRidge_type	*dp;
	int	i,	j;

	pRidge_type	*ad[1024];
	int	nD;





	dp = (pRidge_type *)dim->data;
	for( i = d ; i < dim->height-d ; i++ ){
		dp = (pRidge_type *)IMAGE_PIXEL( dim, i, d );
		for( j = d ; j < dim->width-d ; j++, dp++ ){

			if( dp->state == 0 || dp->state & 0x8 )	continue;

			if( dp->ng[0] >= 0 && dp->ng[1] >= 0 )
				continue;

			pRidge_line_1( dim, dp, ad, &nD );
			

			if( nD < 3 )	
				continue;


			//if( (n = pRidge_approximate_pl( ad, nD, &aPl->a[aPl->nA] )) > 0 )
			//	aPl->nA++;

			plRidge_type *r;

			if( ( r = plRidge_set( ad, nD, prm ) ) != NULL )
				plRidgeA_add( ar, r );


		}
	}

//	plnA_dump( aPl, "aa", 1, "2" );


	return( 1 );
}



static plRidge_type *
plRidge_set(  pRidge_type *arp[], int nRp, pRidgeLinePrm_type *prm )
{
int	i,	n,	w;

float	lm,	val;
plRidge_type	*r;


	lm = val = 0;
	n = w = 0;
	for( i = 0 ; i < nRp ; i++ ){
		pRidge_type *rp = arp[i];

		val += rp->f;
		lm += rp->lm;

		if( ABS(rp->lm) > prm->lm_min  )	n++;

		if( rp->state & 0x02 )	w++;
	}

	val /= nRp;
	lm /= nRp;




	//if( lm < 0 && ( prm->color & 0x01) == 0 )
	//	return( NULL );

	//if( lm > 0 && ( prm->color & 0x02) == 0 )
	//	return( NULL );


	//if( n < 0.25 * nRp )
	//	return( NULL );

	//if( ABS(lm) < prm->lm_min )
	//	return( NULL );


	//if( w > prm->WeekR * n )
	//	return( NULL );



	r = plRidge_alloc();

	r->lm = lm;

	r->f = val;


	pRidge_approximate_pl( arp, nRp, &r->pl );


	return( r );
}





int
pRidge_line_1( image_type *rim, pRidge_type *dp00, pRidge_type *ad[], int *nD )
{
pRidge_type	*rp,	*nrp;
int	arp[9];



	arp[0] = -rim->width - 1;
	arp[1] = -rim->width;
	arp[2] = -rim->width + 1;

	arp[3] = - 1;
	arp[4] = 0;
	arp[5] = + 1;

	arp[6] = rim->width - 1;
	arp[7] = rim->width;
	arp[8] = rim->width + 1;



	int	direct = ( dp00->ng[0] < 0 )? 1 : 0;
	int	k;

	*nD = 0;
	ad[(*nD)++] = dp00;

	for( rp = dp00, k = direct ; rp != NULL ;  ){

		if( rp->ng[k] < 0 )	break;

		nrp = rp + arp[ rp->ng[k] ];

		if( nrp->state & 0x08 )	break;

		if( rp->v.x * nrp->v.x + rp->v.y * nrp->v.y < 0 )
			k = 1-k;

		rp = nrp;

		rp->state |= 0x08;

		ad[(*nD)++] = rp;
	}




	return( 1 );

}





int
pRidge_approximate_pl( pRidge_type *ad[], int nD, pln_type *pl[] )
{
pt2dA_type *apt;

	apt = apt_from_pRidge( ad, nD, NULL );

	pl[0] = pln_alloc( 0 );
	pt2d_approximate_linkO( apt, &pl[0]->ctr, &pl[0]->link );

	pl[0]->len = lnL_length( pl[0]->link );

	pt2dA_destroy( apt );

	return( 1 );

}









static pt2dA_type *
apt_from_pRidge( pRidge_type *ad[], int nD, pt2dA_type *apt )
{
pRidge_type	*dp;
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




