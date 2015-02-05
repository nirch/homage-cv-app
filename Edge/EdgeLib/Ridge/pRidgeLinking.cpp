/*****************************
***   pRidgeDetector.cpp   ***
******************************/
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
#include "Umath/Matrix2Type.h"

#include "Ucamera/Pt2dType.h"
#include "ImageType/ImageType.h"
#include "ImageMark/ImageMark.h"
#include "ImageDump/ImageDump.h"


#include	"ImageApp/ImageApp.h"
#include	"Ujet2/Jet2Type.h"


#include "PRidgeType.h"



#ifdef _DEBUG
#define _DUMP 
#endif


void	pRidge_linking_filter( image_type *rim );



void
pRidge_linking( image_type *rim )
{
pRidge_type	*rp,	*nrp;
int	arp[9];

int	i,	j,	k,	g[10],	nG;
vec2f_type	dp;
float	t,	at[2];

	float	dA,	dT;
	dA = 0.9397;	// 20
	dA = 0.9063;	// 25

	dT = 0.4;

	arp[0] = -rim->width - 1;
	arp[1] = -rim->width;
	arp[2] = -rim->width + 1;

	arp[3] = - 1;
	arp[4] = 0;
	arp[5] = + 1;

	arp[6] = rim->width - 1;
	arp[7] = rim->width;
	arp[8] = rim->width + 1;


	for( i = 2 ; i < rim->height - 2 ; i++ ){

		rp = ( pRidge_type *)IMAGE_PIXEL( rim, i, 2 );

		for( j = 2 ; j < rim->width -2 ; j++, rp++ ){
			rp->ng[0] = rp->ng[1] = -1;
			if( rp->state == 0 )	continue;


			nG = 0;
			g[0] = g[1] = -1;

			for( k = 0 ; k < 9 ; k++ ){
//				if( k == 4 )	continue;
				if( arp[k] == 0 )	continue;

				nrp = rp + arp[k];

				if( nrp->state == 0 )	continue;


				// angle
				t = rp->v.x * nrp->v.x + rp->v.y * nrp->v.y;
				if( t < 0 )	t= -t;
				if( t < dA )	continue;


				dp.x = nrp->p.x - rp->p.x;
				dp.y = nrp->p.y - rp->p.y;

				t = rp->v.x * dp.x + rp->v.y * dp.y;
				if( t < 0 )	t = -t;

				if( t > dT )	continue; 

				t = -rp->v.y * dp.x + rp->v.x * dp.y;



				if( t < 0 ){
					if( g[0] < 0 || t > at[0] ){
						g[0] = k;
						at[0] = t;
					}
				}
				else {
					if( g[1] < 0 || t < at[1] ){
						g[1] = k;
						at[1] = t;
					}
				}
			}

			//nG = 0;
			//if( g[0] >=0 )
			//	rp->ng[nG++] = g[0];
			//if( g[1] >= 0 )
			//	rp->ng[nG++] = g[1];

			rp->ng[0] = g[0];
			rp->ng[1] = g[1];

		}
	}



	pRidge_linking_filter( rim );
}



void
pRidge_linking_filter( image_type *rim )
{
	pRidge_type	*rp,	*nrp;
	int	arp[9];

	int	i,	j,	ng;



	arp[0] = -rim->width - 1;
	arp[1] = -rim->width;
	arp[2] = -rim->width + 1;

	arp[3] = - 1;
	arp[4] = 0;
	arp[5] = + 1;

	arp[6] = rim->width - 1;
	arp[7] = rim->width;
	arp[8] = rim->width + 1;


	for( i = 2 ; i < rim->height - 2 ; i++ ){

		rp = ( pRidge_type *)IMAGE_PIXEL( rim, i, 2 );

		for( j = 2 ; j < rim->width -2 ; j++, rp++ ){
			if( rp->state == 0 )	continue;


			if( rp->ng[0] >= 0 ){
				nrp = rp + arp[ rp->ng[0] ];
				ng = 8 - rp->ng[0];
				if( nrp->ng[0] != ng && nrp->ng[1] != ng )
					rp->ng[0] = -1;
			}

			if( rp->ng[1] >= 0 ){
				nrp = rp + arp[ rp->ng[1] ];
				ng = 8 - rp->ng[1];
				if( nrp->ng[0] != ng && nrp->ng[1] != ng )
					rp->ng[1] = -1;
			}
		}
	}
}





void
pRidge_linking_union( image_type *rim )
{
	pRidge_type	*rp,	*nrp;
	int	arp[9];

	int	i,	j,	k,	g[10],	nG;
	vec2f_type	dp;
	float	t;
	int	k0,	k1;

	float	dA,	dT;
	dA = 0.9397;	// 20
	dA = 0.85;//0.9063;	// 25

	dT = 0.4;

	arp[0] = -rim->width - 1;
	arp[1] = -rim->width;
	arp[2] = -rim->width + 1;

	arp[3] = - 1;
	arp[4] = 0;
	arp[5] = + 1;

	arp[6] = rim->width - 1;
	arp[7] = rim->width;
	arp[8] = rim->width + 1;


	for( i = 2 ; i < rim->height - 2 ; i++ ){

		rp = ( pRidge_type *)IMAGE_PIXEL( rim, i, 2 );

		for( j = 2 ; j < rim->width -2 ; j++, rp++ ){

			if( rp->state == 0 )	continue;

			if( rp->ng[0] >= 0 && rp->ng[1] >= 0 )
				continue;

			if( rp->ng[0] < 0 && rp->ng[1] < 0 )
				continue;




			nG = 0;
			g[0] = g[1] = -1;

			for( k = 0 ; k < 9 ; k++ ){
				//				if( k == 4 )	continue;
				if( arp[k] == 0 )	continue;

				nrp = rp + arp[k];

				if( nrp->state == 0 )	continue;

				if( nrp->ng[0] >= 0 && nrp->ng[1] >= 0 )
					continue;

				if( nrp->ng[0] < 0 && nrp->ng[1] < 0 )
					continue;


				// angle
				t = rp->v.x * nrp->v.x + rp->v.y * nrp->v.y;
				if( t < 0 )	t= -t;
				if( t < dA )	continue;


				dp.x = nrp->p.x - rp->p.x;
				dp.y = nrp->p.y - rp->p.y;

				t = rp->v.x * dp.x + rp->v.y * dp.y;
				if( t < 0 )	t = -t;

				if( t > dT )	continue; 

				t = -rp->v.y * dp.x + rp->v.x * dp.y;

				k0 = ( t < 0 )? 0 : 1;
				if( rp->ng[k0] >= 0 )
					continue;

				t = -(-nrp->v.y * dp.x + nrp->v.x * dp.y);

				k1 = ( t < 0 )? 0 : 1;
				if( nrp->ng[k1] >= 0 )
					continue;

				rp->ng[k0] = k;
				nrp->ng[k1] = 8 - k;

			}

		}
	}



	//pRidge_linking_filter( rim );
}
