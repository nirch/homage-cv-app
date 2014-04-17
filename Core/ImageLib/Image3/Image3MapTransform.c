
#include	<stdio.h>
#include	<math.h>
#include	<string.h>
#include	<stdlib.h>


#ifdef _DEBUG
#define _DUMP
#endif


#include "Uigp/igp.h"



#include "ImageType/ImageType.h"
#include "Image3Tool.h"





image_type *
image3_map_transform( image_type *sim, image_type *map, image_type *im )
{
float	*mp;
u_char	*tp;
int	i,	j;

vec2f_type	ip;


	

	im = image_reallocL( sim, im );

	mp = map->data_f;

	tp = im->data;
	
	for( i = 0 ; i <  im->height ; i++ ){

		for( j = 0 ; j < im->width ; j++, tp += 3 ){



			ip.x = *mp++;
			ip.y = *mp++;



			image3_bilnear_pixel( sim, ip.x, ip.y,  tp );
		}
	}


	return( im );
}



