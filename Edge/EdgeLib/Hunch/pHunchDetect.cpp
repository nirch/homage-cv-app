
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#include	"Uigp/igp.h"
#include	"ImageType/ImageType.h"


#include	"ImageApp/ImageApp.h"
#include	"Ujet2/Jet2Type.h"


#include "pHunchType.h"


void
pHunch_detect( image_type *im, image_type *maskIm, box2i *b, pHunchPrm_type *prm, pHunchA_type *ah )
{
djet2_type	d;
jet2	f;
int	i,	j;
u_char	*mp;
jet2App_type *ja;


	ja = jet2App_init( prm->size, prm->weight );



	if( b->x0 < ja->nh )	b->x0 = ja->nh;
	if( b->x1 + ja->nh > im->width )	b->x1 = im->width - ja->nh;

	if( b->y0 < ja->nh )	b->y0 = ja->nh;
	if( b->y1 + ja->nh > im->height )	b->y1 = im->height - ja->nh;




	for( i = b->y0 ; i < b->y1 ; i++ ){
		mp = IMAGE_PIXEL( maskIm, i, b->x0 );

		for( j = b->x0 ; j < b->x1 ; j++, mp++){

			if( *mp == 0 )	continue;

			image_jet2App( im, i - ja->nh, j - ja->nh, ja, &f );

			rescaling_jet2( &f, 2, ja->dn_inv, ja->dn_inv, &f );

			diagonalN_jet2( &f, 1.0, &d );

//			diagonalN_jet2( &f, ja->dn, &d );


			d.p.x += j + ja->dxy;
			d.p.y += i + ja->dxy; 

				
			if( ( d.lm1 > 0 ) !=  ( d.lm2 > 0 ) )
				continue;

			// black patch
			if( d.lm1 > 0  && (prm->sign & 0x02) == 0 )
				continue;
	
			// white patch
			if( d.lm1 < 0  && (prm->sign & 0x01) == 0 )
				continue;



			if( ABS(d.lm1) > prm->lmR *ABS(d.lm2) )
				continue;


			if( ABS(d.lm1) < prm->lm1Min )
				continue;

			if( ABS(d.lm2) < prm->lmMin )
				continue;


			if( ABS(d.dx) > prm->dx || ABS(d.dy) > prm->dy )
				continue;



			pHunch_type *h = pHunch_alloc();

			h->size = ja->n;
			h->p = d.p;
			h->v = d.n;
			h->lm1 = d.lm1;
			h->lm2 = d.lm2;

			ah->a[ah->nA++] = h;

		}
	}
}



static int	pHunch_detect_1( image_type *im, int i0, int j0, jet2App_type *ja, pHunchPrm_type *prm, pHunch_type *h );


void
pHunch_detectN( image_type *im, image_type *maskIm, box2i *b, pHunchPrm_type *prm, pHunchA_type *ah )
{
int	i,	j,	k;
u_char	*mp;
jet2App_type *ja[4];
pHunch_type	h[4];


	ja[0] = jet2App_init( prm->size, prm->weight );
	ja[1] = jet2App_init( prm->size+2, prm->weight );
	ja[2] = jet2App_init( prm->size+4, prm->weight );



	int	nh = ja[2]->nh;
	if( b->x0 < nh )	b->x0 = nh;
	if( b->x1 + nh > im->width )	b->x1 = im->width - nh;

	if( b->y0 < nh )	b->y0 = nh;
	if( b->y1 + nh > im->height )	b->y1 = im->height - nh;


	prm->lm1_0 = prm->lm1Min *0.5;
	prm->lm2_0 = prm->lmMin *0.5;



	for( i = b->y0 ; i < b->y1 ; i++ ){
		mp = IMAGE_PIXEL( maskIm, i, b->x0 );

		for( j = b->x0 ; j < b->x1 ; j++, mp++){

			if( *mp == 0 )	continue;



			if( pHunch_detect_1( im, i, j, ja[0], prm, &h[0] ) < 0 )
				continue;

			if( pHunch_detect_1( im, i, j, ja[1], prm, &h[1] ) < 0 )
				continue;



			
			k = 0;
			if( ABS(h[1].lm1) + ABS(h[1].lm2) > ABS(h[k].lm1) +ABS(h[k].lm2) )
				k = 1;

			if( pHunch_detect_1( im, i, j, ja[2], prm, &h[2] ) > 0 ){
				if( ABS(h[2].lm1) + ABS(h[2].lm2)  >ABS(h[k].lm1) +ABS(h[k].lm2) )
					k = 2;
			}


			if( ABS(h[k].lm1) < prm->lm1Min ||  ABS(h[k].lm2) < prm->lmMin )
				continue;



			pHunch_type *ph = pHunch_alloc();
			*ph = h[k];

			ah->a[ah->nA++] = ph;

		}
	}
}
void
pHunch_detect( image_type *im, box2i *b, pHunchPrm_type *prm, pHunchA_type *ah )
{
	jet2App_type *ja[4];


	ja[0] = jet2App_init( prm->size, prm->weight );
	ja[1] = jet2App_init( prm->size+2, prm->weight );
	ja[2] = jet2App_init( prm->size+4, prm->weight );

	pHunch_detect( im, b, ja, prm, ah );


	jet2App_destroy( ja[0] );
	jet2App_destroy( ja[1] );
	jet2App_destroy( ja[2] );



}

void
pHunch_detect( image_type *im, box2i *b, jet2App_type *ja[], pHunchPrm_type *prm, pHunchA_type *ah )
{
	int	i,	j,	k;
//	jet2App_type *ja[4];
	pHunch_type	h[4];


//	ja[0] = jet2App_init( prm->size, prm->weight );
//	ja[1] = jet2App_init( prm->size+2, prm->weight );
//	ja[2] = jet2App_init( prm->size+4, prm->weight );



	int	nh = ja[2]->nh;
	if( b->x0 < nh )	b->x0 = nh;
	if( b->x1 + nh > im->width )	b->x1 = im->width - nh;

	if( b->y0 < nh )	b->y0 = nh;
	if( b->y1 + nh > im->height )	b->y1 = im->height - nh;


	prm->lm1_0 = prm->lm1Min *0.5;
	prm->lm2_0 = prm->lmMin *0.5;



	for( i = b->y0 ; i < b->y1 ; i++ ){

		for( j = b->x0 ; j < b->x1 ; j++){




			if( pHunch_detect_1( im, i, j, ja[0], prm, &h[0] ) < 0 )
				continue;

			if( pHunch_detect_1( im, i, j, ja[1], prm, &h[1] ) < 0 )
				continue;




			k = 0;
			if( ABS(h[1].lm1) + ABS(h[1].lm2) > ABS(h[k].lm1) +ABS(h[k].lm2) )
				k = 1;

			if( pHunch_detect_1( im, i, j, ja[2], prm, &h[2] ) > 0 ){
				if( ABS(h[2].lm1) + ABS(h[2].lm2)  >ABS(h[k].lm1) +ABS(h[k].lm2) )
					k = 2;
			}


			if( ABS(h[k].lm1) < prm->lm1Min ||  ABS(h[k].lm2) < prm->lmMin )
				continue;



			pHunch_type *ph = pHunch_alloc();
			*ph = h[k];

			ah->a[ah->nA++] = ph;

		}
	}
}




static int
pHunch_detect_1( image_type *im, int i0, int j0, jet2App_type *ja, pHunchPrm_type *prm, pHunch_type *h )
{
djet2_type	d;
jet2	f;


	image_jet2App( im, i0 - ja->nh, j0 - ja->nh, ja, &f );

	rescaling_jet2( &f, 2, ja->dn_inv, ja->dn_inv, &f );

	diagonalN_jet2( &f, 1.0, &d );


	if( ( d.lm1 > 0 ) !=  ( d.lm2 > 0 ) )
		return( -1 );

	// black patch
	if( d.lm1 > 0  && (prm->sign & 0x02) == 0 )
		return( -1 );

	// white patch
	if( d.lm1 < 0  && (prm->sign & 0x01) == 0 )
		return( -1 );



	if( ABS(d.lm1) > prm->lmR *ABS(d.lm2) )
		return( -1 );


	//if( ABS(d.lm1) < prm->lm1Min )
	//	return( -1 );

	//if( ABS(d.lm2) < prm->lmMin )
	//	return( -1 );

	if( ABS(d.lm1) < prm->lm1_0 )
		return( -1 );

	if( ABS(d.lm2) < prm->lm2_0 )
		return( -1 );


	if( ABS(d.dx) > prm->dx || ABS(d.dy) > prm->dy )
		return( -1 );


	h->size = ja->n;

	h->f = VALUE_JET2( f, d.p.x, d.p.y );

	h->p.x = d.p.x + j0 + ja->dxy;
	h->p.y = d.p.y + i0 + ja->dxy; 

	h->v = d.n;
	h->lm1 = d.lm1;
	h->lm2 = d.lm2;

	return( 1 );
}





void
pHunchA_filter( pHunchA_type *ah, float T )
{
	pHunch_type	*h;
	int	i,	j,	n;
	float	x,	y;
	float	T2;

	T2 = T*T;

	for( i = 0,	n = 0 ; i < ah->nA ; i++ ){
		h = ah->a[i];

		for( j = n-1 ; j >= 0 ; j-- ){
			x = h->p.x - ah->a[j]->p.x;
			y = h->p.y - ah->a[j]->p.y;
			if( x*x + y*y < T2 )
				break;
		}

		if( j < 0 ){
			ah->a[n++] = h;
			continue;
		}

		if( ABS(h->lm1)  > ABS(ah->a[j]->lm1) ){
			free( ah->a[j] );
			ah->a[j] = h;
		}
		else	free( h );
		
	}

	ah->nA = n;
}