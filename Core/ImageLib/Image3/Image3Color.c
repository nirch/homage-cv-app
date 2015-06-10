 /************************
 ***   Image3Tool.c   ***
 ************************/
#include	<string.h>

#include	"ImageType/ImageType.h"
#include	"Image3Tool.h"
#include    <math.h>




#ifdef _AA_
image_type *
image3_to_BGR( image_type *sim, image_type *im )
{
u_char	*tp;
u_char	*sp;
int	i,	j;

/*
	if( im != NULL && ( im->row != sim->row || im->column != sim->column ) ){
		image_destroy( im, 1 );
		im = NULL;
	}
	
	if( im == NULL )
		im = image_create( sim->row, sim->column, 3, 1, NULL );
*/

	im = image_realloc( im, sim->width, sim->height, 3, IMAGE_TYPE_U8, 1  );


	tp = (u_char *)im->data;
	sp = sim->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp += 3 ){
			*tp++ = *(sp+2);
			*tp++ = *(sp+1);
			*tp++ = *sp;
		}

	return( im );
}



void
image3_bgr2rgb( image_type *im )
{

u_char	*sp,	tmp;
int	i,	j;

	sp = im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp += 3 ){
			tmp = *sp;
			*sp = *(sp+2);
			*(sp+2) = tmp;
		}
}
#endif

void
image3_VH_RGB_flip( image_type *im )
{
	u_char	*p0,	*p1,	tmp;
	int	i,	j, k,	hrow;

	hrow = im->row / 2;
	for( i = 0 ; i < hrow  ; i++ ){

		p0 = IMAGE_PIXEL( im, i, 0 );
		p1 = IMAGE_PIXEL( im, im->row-1-i, im->column-1 );
		for( j = 0 ; j < im->column ; j++, p0+=3, p1-=3 )
		{
			for (k = 0; k < 3; k++ )
			{
				tmp = *(p0 +k);
				*(p0 + k) = *(p1 + (2-k));
				*(p1 + (2-k)) = tmp;
				//*(p0 + k) = *(p1 + k);
				//*(p1 + k) = tmp;
			}
		}
	}
}



image_type* 
image3_GetBlueColorGray(image_type *im_YCbCr)
{
	u_char *sp_YCbCr, *sp_blue; 
	int i,j;
	image_type * im_blue = image_create( im_YCbCr->row, im_YCbCr->column, 1, 1, NULL );
	for( i = 0 ; i < im_YCbCr->row ; i++ ){
		for( j = 0 ; j < im_YCbCr->column ; j++ ){
			sp_YCbCr = IMAGE_PIXEL( im_YCbCr, i, j );
			sp_blue = IMAGE_PIXEL( im_blue, i, j );
			*sp_blue = MAX(0,sp_YCbCr[1] - sp_YCbCr[2]);
		}
	}
	return im_blue;
}

image_type * 
image3_bgr2ycbcr(image_type *im_BGR)
{
	u_char	*sp_BGR, *sp_YCbCr;
	int i,j;
	image_type * im_YCbCr = image_create( im_BGR->row, im_BGR->column, 3, 1, NULL );
	for( i = 0 ; i < im_YCbCr->row ; i++ ){
		for( j = 0 ; j < im_YCbCr->column ; j++ ){
			sp_BGR = IMAGE_PIXEL( im_BGR, i, j );
			sp_YCbCr = IMAGE_PIXEL( im_YCbCr, i, j );
			//asumming that sp_BGR[2] is R, and sp_BGR[1] is G, sp_BGR[0] is B.
			sp_YCbCr[0] = 16  + 1.0/256 * (   65.738  * sp_BGR[2] +  129.057  * sp_BGR[1] +  25.064  * sp_BGR[0]);//Y
			sp_YCbCr[1] = 128 + 1.0/256 * ( - 37.945  * sp_BGR[2] -   74.494  * sp_BGR[1] + 112.439  * sp_BGR[0]);//Cb
			sp_YCbCr[2] = 128 + 1.0/256 * (  112.439  * sp_BGR[2] -   94.154  * sp_BGR[1] -  18.285  * sp_BGR[0]);//Cr
			//scale Cb and Cr from range [16 240] to range [0 255]
			sp_YCbCr[1] = (sp_YCbCr[1] - 16)*(255.0/240.0);
			sp_YCbCr[2] = (sp_YCbCr[2] - 16)*(255.0/240.0);
		}
	}
	return im_YCbCr;
}


image_type * 
image3_rgb2ycbcr(image_type *im_RGB)
{
	u_char	*sp_RGB, *sp_YCbCr;
	int i,j;
	image_type * im_YCbCr = image_create( im_RGB->row, im_RGB->column, 3, 1, NULL );
	for( i = 0 ; i < im_YCbCr->row ; i++ ){
		for( j = 0 ; j < im_YCbCr->column ; j++ ){
			sp_RGB = IMAGE_PIXEL( im_RGB, i, j );
			sp_YCbCr = IMAGE_PIXEL( im_YCbCr, i, j );
			
			sp_YCbCr[0] = 16  + 1.0/256 * (   65.738  * sp_RGB[0] +  129.057  * sp_RGB[1] +  25.064  * sp_RGB[2]);//Y
			sp_YCbCr[1] = 128 + 1.0/256 * ( - 37.945  * sp_RGB[0] -   74.494  * sp_RGB[1] + 112.439  * sp_RGB[2]);//Cb
			sp_YCbCr[2] = 128 + 1.0/256 * (  112.439  * sp_RGB[0] -   94.154  * sp_RGB[1] -  18.285  * sp_RGB[2]);//Cr
			//scale Cb and Cr from range [16 240] to range [0 255]
			sp_YCbCr[1] = (sp_YCbCr[1] - 16)*(255.0/240.0);
			sp_YCbCr[2] = (sp_YCbCr[2] - 16)*(255.0/240.0);
		}
	}
	return im_YCbCr;
}

image_type * 
image3_findLiklihoodCbCr(image_type *im_YCbCr,int cb,int cr)
{
	u_char	*sp,*sp2;
	int i,j;
	image_type * Liklihood = image_create( im_YCbCr->row, im_YCbCr->column, 1, 1, NULL );
	for( i = 0 ; i < im_YCbCr->row ; i++ ){
		for( j = 0 ; j < im_YCbCr->column ; j++ ){
			sp = IMAGE_PIXEL( im_YCbCr, i, j );
			sp2 = IMAGE_PIXEL( Liklihood, i, j );
			*sp2 = 255 - (abs(sp[1] - cb) + abs(sp[2] - cr))/2;
		}
	}
	return Liklihood;
}

image_type * 
image3_findLiklihoodYCbCr(image_type *im_YCbCr,int cb,int cr,int Y)
{
	int dist;
	int nByte0, nByte1, nByte2;
	int nVal;
	u_char	*sp,*sp2;
	int i,j;

	image_type * Liklihood = image_create( im_YCbCr->row, im_YCbCr->column, 1, 1, NULL );
	for( i = 0 ; i < im_YCbCr->row ; i++ ){
		for( j = 0 ; j < im_YCbCr->column ; j++ ){
			sp = IMAGE_PIXEL( im_YCbCr, i, j );
			sp2 = IMAGE_PIXEL( Liklihood, i, j );
			//temp = MAX(abs(sp[1] - cb),abs(sp[2] - cr)); 
			//*sp2 = 255 - MAX(temp,abs(sp[0] - Y));
			nByte0 = (int)sp[0] - Y;
			nByte1 = (int)sp[1] - cb;
			nByte2 = (int)sp[2] - cr;
			nVal = nByte0 * nByte0 + nByte1 * nByte1 + nByte2 * nByte2;

			dist = (int)sqrt((double) nVal);
			*sp2 = 255 - MIN(dist,255);
		}
	}
	return Liklihood;
}

image_type * 
image3_findLiklihoodYCbCr2(image_type *im_YCbCr,int cb,int cr,int Y)
{
	int nByte0, nByte1, nByte2;
	u_char	*sp,*sp2;
	int i,j;

	image_type * Liklihood = image_create( im_YCbCr->row, im_YCbCr->column, 1, 1, NULL );
	for( i = 0 ; i < im_YCbCr->row ; i++ ){
		for( j = 0 ; j < im_YCbCr->column ; j++ ){
			sp = IMAGE_PIXEL( im_YCbCr, i, j );
			sp2 = IMAGE_PIXEL( Liklihood, i, j );
			//temp = MAX(abs(sp[1] - cb),abs(sp[2] - cr)); 
			//*sp2 = 255 - MAX(temp,abs(sp[0] - Y));
			nByte0 = (int)sp[0] - Y;
			nByte1 = (int)sp[1] - cb;
			nByte2 = (int)sp[2] - cr;
			if(abs(nByte0) > 20){
				*sp2 = 0;
			}else{
				//*sp2 = 255 - MIN(MAX(abs(nByte1),abs(nByte2)),255);
				*sp2 = 255 - MIN(sqrt(nByte1*nByte1 +  nByte2*nByte2),255);
				//*sp2 = 255 - MIN((abs(nByte1) + abs(nByte2))/2,255);
			}
		}
	}
	return Liklihood;
}

