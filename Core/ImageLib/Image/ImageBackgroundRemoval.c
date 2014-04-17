/************************************
 ***   ImageBackgroundRemoval.c   ***
 ************************************/

#include	"ImageType/ImageType.h"


void
image_background_removal( image_type *bim, image_type *im, int rgbT, float Ymin )
{
u_int	*bp,	*tp;
int	i,	j;
int	R,	G,	B,	Y; 
int	bR,	bG,	bB,	bY;
int	dR,	dG,	dB;

	rgbT = 30;



	bp = (u_int *)bim->data;
	tp = (u_int *)im->data;

	for( i = 0 ; i < im->row ; i++ ){

		for( j = 0 ; j < im->column ; j++, bp++, tp++ ){
			
			R = IMAGE4_RED(*tp);
			G = IMAGE4_GREEN(*tp);
			B = IMAGE4_BLUE(*tp);

			bR = IMAGE4_RED(*bp);
			bG = IMAGE4_GREEN(*bp);
			bB = IMAGE4_BLUE(*bp);


			if( ( dR = R - bR) < 0 )	dR = -dR;
			if( ( dG = G - bG) < 0 )	dG = -dG;
			if( ( dB = B - bB) < 0 )	dB = -dB;

			if( dR < rgbT && dG < rgbT && dB < rgbT ){
				*tp = 0;
				continue;
			}

	

			if( dR > 2*rgbT || dG > 2*rgbT || dB > 2*rgbT ){
				continue;
			}

			bY = ( bR + bG + bB )/3;
			if( bY < Ymin )	continue; 


			Y = ( R + G + B )/3;
			if( Y < Ymin )	continue;

			if( Y + 0.5*rgbT > bY )	continue;

			dR = (R / (float)Y -  bR / (float)bY)*256;
			dG = (G / (float)Y -  bG / (float)bY)*256;
			dB = (B / (float)Y -  bB / (float)bY)*256;

			if( dR < 0 ) dR = -dR;
			if( dG < 0 ) dG = -dG;
			if( dB < 0 ) dB = -dB;


			if( dR < rgbT && dG < rgbT && dB < rgbT ){
				*tp = 0;
				continue;
			}
		}
	}
}


void
image_background_removalN( image_type *bim, image_type *im, int rgbT, float Ymin )
{
u_int	*bp,	*tp;
int	i,	j;
int	R,	G,	B,	Y;




	bp = (u_int *)bim->data;
	tp = (u_int *)im->data;

	for( i = 0 ; i < im->row ; i++ ){

		for( j = 0 ; j < im->column ; j++, bp++, tp++ ){
			
			R = IMAGE4_RED(*tp);
			G = IMAGE4_GREEN(*tp);
			B = IMAGE4_BLUE(*tp);

			Y = ( R + G + B )/3;

			R = ( R-Y)+128;
			G = ( G-Y)+128;
			B = ( B-Y)+128;

			*tp = IMAGE4_RGB( R, G, B );
		}
	}
}