/****************************
 ***    Image3Histogram.c  ***
 ***************************/
 
#include	"ImageType/ImageType.h"



#define RGB2CBI( R, G, B )	((-0.16874 * R - 0.33126 * G + 0.50000 * B) + 128.5)
#define RGB2CRI( R, G, B )	(( 0.50000 * R - 0.41869 * G - 0.08131 * B) + 128.5)

#define RGB2CB( R, G, B )	((-11058 * R - 21709 * G + 32768 * B + 8421376)>>16)
#define RGB2CR( R, G, B )	(( 32768 * R - 27439 * G - 5328  * B + 8421376)>>16)



void	image3_histogram_CrCb_update( image_type *sim,  image_type *mim, image_type *him );




image_type *
image3_histogram_CrCb( image_type *sim,  image_type *mim, image_type *him )
{
	him = image_realloc( him, 256, 256, 1, IMAGE_TYPE_U32, 1 );
	imageUI_const( him, 0 );

	image3_histogram_CrCb_update( sim, mim, him );


//	imageUI_bluring( him, 5 );

	return( him );
}



void
image3_histogram_CrCb_update( image_type *sim,  image_type *mim, image_type *him )
{
	int	i,	j;
	u_char	*sp,	*mp;
	u_int	*hp;
	int	R,	G,	B;
	int	cR,	cB;



	mp = mim->data;
	sp = sim->data;


	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++,  mp++ ){
			R = *sp++;
			G = *sp++;
			B = *sp++;

			if( *mp == 0 )	continue;


			cB = RGB2CB( R, G, B );
			cR = RGB2CR( R, G, B );


			hp = (u_int *)IMAGE_PIXEL( him, cR, cB );
			(*hp)++;
		}
	}

	//	image_dump( im, "mask", 1, "mm"  );
}



