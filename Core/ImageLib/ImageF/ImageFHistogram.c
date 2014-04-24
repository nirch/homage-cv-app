/******************************
 ***   ImageFHistograme.c   ***
 ******************************/

#include "ImageType/ImageType.h"
#include "ImageType/ImageTool.h"




void
imageF_histogram( image_type *im, float min, float max, int h[] )
{
float	*sp;
int	i,	j,	k;


	for( i = 0 ; i < 4096 ; i++ )	h[i] = 0;

	sp = im->data_f;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++ ){
			k = 4095*( *sp - min)/( max - min);
			k = PUSH_TO_RANGE( k, 0, 4095 );
			h[k] ++;
		}
}

void
imageFM_histogram( image_type *im, image_type *mim, float min, float max, int h[] )
{
float	*sp;
u_char	*mp;
int	i,	j,	k;


	for( i = 0 ; i < 4096 ; i++ )	h[i] = 0;

	sp = im->data_f;
	mp = mim->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++, mp++ ){
			if( *mp != 0 ){
				k = 4095*( *sp - min)/( max - min);
				k = PUSH_TO_RANGE( k, 0, 4095 );
				h[k] ++;
			}
		}
}

