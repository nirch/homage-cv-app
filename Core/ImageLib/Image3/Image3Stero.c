/*************************
 ***   Image3Stero.c   ***
 *************************/

#include "ImageType/ImageType.h"
#include "Image3Tool.h"




image_type * 
image3_stereo( image_type *sim, image_type *mim, float _shiftSilh, float _shiftBgr, image_type *im )
{	
int	i,	j,	r,	s;
u_char *sp,	*spS,	*spB,	*mp,	*tp;

int	db,	ds;
float	dx;

	im = image_recreate( im, sim->height, sim->width, 3, 1 );


	db = _shiftBgr + 0.5;
	ds = _shiftSilh;

	dx = _shiftSilh - (int)_shiftSilh;

	sp = sim->data;
	spS = sim->data - ds*3;
	spB =  sim->data  - db*3;

	mp = mim->data;// - ds;



	tp = im->data;

	for(  i = 0 ; i < sim->row ; i++ ){
		for(  j = 0 ; j < sim->column ; j++, spS += 3, spB += 3, mp++ ){

			r = *sp++;

			if (j >= db )
				r = *spB;

			if( *mp > 0 && j >= ds ){
				s =  *spS * (1-dx) +  *(spS+1) * dx;

				if( *mp < 250 )

					r = (r*( 255 - *mp ) + *spS * *mp )/255.0;

				else
					r = *spS;

			}


			*tp++ = r;
			*tp++ = *sp++;
			*tp++ = *sp++;
		}
	}



	return( im );
}