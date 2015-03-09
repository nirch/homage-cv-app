/**************************
 ***   pRidgeLine.cpp   ***
 **************************/
#ifdef _DEBUG
#define	_DUMP
#endif

#include	<math.h>



#include	"ImageDump/ImageDump.h"


#include	"UGeoApp/GeoApp.h"


#include "pRidgePrm/pRidgePrm.h" 


#include "pRidgeType.h"
#include "plRidgeType.h"




static int	vim_detect_ridgeL( image_type *dim, int d, pRidgeLinePrm_type *prm, plRidge_type *aRidge[] );

static plRidge_type *	plRidge_set(  pRidge_type *arp[], int nRp, pRidgeLinePrm_type *prm );




int
vim_detect_ridge( image_type *sim, box2i *box, pRidgePrm_type *prm, plRidgeA_type *ar )
{

	image_type *im = image_cut( sim, box->y0, box->x0, box->y1-box->y0+1, box->x1-box->x0+1, NULL );

	image_type *imy = image1_from( im, NULL );


//	IMAGE_DUMP( im, "ridge", -1, NULL );


	image_type *rim = pRidge_detector( imy, NULL, NULL, 0, &prm->detect, NULL );


	pRidge_lineN(  rim, 2, &prm->line, ar );



	image_destroy( im, 1 );
	image_destroy( imy, 1 );
	image_destroy( rim, 1 );



	plRidgeA_translate( ar, box->y0, box->x0 );



	return( ar->nR );
}






int
pRidge_lineN( image_type *dim, int d, pRidgeLinePrm_type *prm, plRidgeA_type *aRidge )
{
pRidge_type	*dp;
int	i,	j;

pRidge_type	*ad[4096];
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


			if( (aRidge->r[aRidge->nR] = plRidge_set( ad, nD, prm ) ) != NULL )
				aRidge->nR++;
		}
	}

	//	plnA_dump( aPl, "aa", 1, "2" );


	return( aRidge->nR );
}


static int	plRidge_reduce(  pRidge_type *arp[], int nRp, pRidgeLinePrm_type *prm );


static plRidge_type *
plRidge_set(  pRidge_type *arp[], int nRp, pRidgeLinePrm_type *prm )
{
	int	i,	n,	w;

	float	lm,	val;
	plRidge_type	*r;


	nRp = plRidge_reduce(  arp, nRp,  prm );
	if( nRp < 0 )	return( NULL );


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




	if( lm < 0 && ( prm->color & 0x01) == 0 )
		return( NULL );

	if( lm > 0 && ( prm->color & 0x02) == 0 )
		return( NULL );


	if( n < 0.25 * nRp )
		return( NULL );

	if( ABS(lm) < prm->lm_min )
		return( NULL );;


	if( w > prm->WeekR * n )
		return( NULL );



	r = plRidge_alloc();

	r->lm = lm;

	r->f = val;


	pRidge_approximate_pl( arp, nRp, &r->pl );

	if( r->pl->len < prm->lenMin ||  r->pl->len >  prm->lenMax ){
		plRidge_destroy( r );
		return( NULL );
	}
	

	return( r );
}




static int
plRidge_reduce(  pRidge_type *arp[], int nRp, pRidgeLinePrm_type *prm )
{
	int	i,	i0,	i1,	j;
	pRidge_type	*r;

	for( i = 0 ; i < nRp ; i++ ){
		r = arp[i];
		if( ABS(r->lm) > prm->lm_min )	break;
	}

	if( i >= nRp )	return( -1 );

	i0 = i;

	for( i = nRp-1 ; i >= i0 ; i-- ){
		r = arp[i];
		if( ABS(r->lm) > prm->lm_min )	break;
	}

	i1 = i;

	i0 -= 2;
	if( i0 < 0 )	i0 = 0;
	i1 += 3;
	if( i1 > nRp )	i1 = nRp;

	if( i0 == 0 )	return( i1 - i0 );

	for( i = i0, j = 0 ; i < i1 ; i++ )
		arp[j++] = arp[i];


	return( i1 - i0 );
}





