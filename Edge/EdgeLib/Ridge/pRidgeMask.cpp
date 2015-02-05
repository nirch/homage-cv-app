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




int
pRidge_mask( image_type *dim, int d,  image_type *mim )
{
	pRidge_type	*dp,	*mp;
	int	i,	j;


	float dT = 0.85;

//	int	d = 0;

	dp = (pRidge_type *)dim->data;
	for( i = d ; i < dim->height-d ; i++ ){
		dp = (pRidge_type *)IMAGE_PIXEL( dim, i, d );
		mp = (pRidge_type *)IMAGE_PIXEL( mim, i, d );

		for( j = d ; j < dim->width-d ; j++, dp++, mp++ ){

			if( mp->state == 0 )	continue;
			if( dp->state == 0 )	continue;




			vec2f_type	dv;
			dv.x = dp->p.x - mp->p.x;
			dv.y = dp->p.y - mp->p.y;
			if( dv.x *dv.x + dv.y*dv.y > 0.2*0.2 )
				continue;


			float t = VEC2D_INNER( dp->v, mp->v );
			if( t < 0 )	t = -t;

			if( t < dT )	continue;


			dp->state = 0;

		}
	
	}

	return( 1 );
}


