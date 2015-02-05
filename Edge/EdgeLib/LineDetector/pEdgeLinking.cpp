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


#include "pEdgeType.h"



#ifdef _DEBUG
#define _DUMP 
#endif


void	pEdge_linking_1( image_type *rim );


void
pEdge_linking( image_type *rim )
{
pEdge_type	*rp,	*nrp;
int	arp[9];

int	i,	j,	k,	g[10],	nG;
vec2f_type	dp;
float	t,	at[2];

	float	dA,	dT;
	dA = 0.9397;	// 20
	dA = 0.9063;	// 25

	dT = 1.25;

	arp[0] = -rim->width - 1;
	arp[1] = -rim->width;
	arp[2] = -rim->width + 1;

	arp[3] = - 1;
	arp[4] = 0;
	arp[5] = + 1;

	arp[6] = rim->width - 1;
	arp[7] = rim->width;
	arp[8] = rim->width + 1;


	at[0] = at[1] = 0;

	for( i = 1 ; i < rim->height - 1 ; i++ ){

		rp = ( pEdge_type *)IMAGE_PIXEL( rim, i, 1 );

		for( j = 1 ; j < rim->width -1 ; j++, rp++ ){
			rp->ng[0] = rp->ng[1] = -1;
			if( rp->state == 0 )	continue;


			nG = 0;
			g[0] = g[1] = -1;

			for( k = 0 ; k < 9 ; k++ ){
				if( k == 4 )	continue;
				nrp = rp + arp[k];

				if( nrp->state == 0 )	continue;


				// angle
				t = rp->v.x * nrp->v.x + rp->v.y * nrp->v.y;
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


			rp->ng[0] = g[0];
			rp->ng[1] = g[1];

		}
	}

	pEdge_linking_1( rim );
}






void
pEdge_linking_1( image_type *rim )
{
	pEdge_type	*rp,	*nrp;
	int	arp[9];
	int	i,	j;

	arp[0] = -rim->width - 1;
	arp[1] = -rim->width;
	arp[2] = -rim->width + 1;

	arp[3] = - 1;
	arp[4] = 0;
	arp[5] = + 1;

	arp[6] = rim->width - 1;
	arp[7] = rim->width;
	arp[8] = rim->width + 1;


	for( i = 1 ; i < rim->height - 1 ; i++ ){

		rp = ( pEdge_type *)IMAGE_PIXEL( rim, i, 1 );

		for( j = 1 ; j < rim->width -1 ; j++, rp++ ){

			if( rp->ng[0] >= 0 ){

				nrp = rp + arp[rp->ng[0] ];

				if( nrp->ng[1] != 8 - rp->ng[0] )
					rp->ng[0] = -1;
			}

			if( rp->ng[1] >= 0 ){

				nrp = rp + arp[rp->ng[1] ];

				if( nrp->ng[0] != 8 - rp->ng[1] )
					rp->ng[1] = -1;
			}

		}
	}
}





int
pEdge_linking_get( image_type *rim, pEdge_type *dp00, pEdge_type *ad[512] )
{
int	nD;

pEdge_type	*rp;
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




	for( rp = dp00, nD = 0 ; rp != NULL ;  ){
		ad[nD++] = rp;

		if( rp->ng[1] < 0 )	break;
		rp = rp + arp[ rp->ng[1] ];
	}



	return( nD );
}