/************************
 *** ImageReadYuv.c   ***
 ************************/
#include	"ImageType/ImageType.h"

#include "image2yuv.h"


image_type *	image_read_row( int row, int column, FILE *fp );
static image_type *image_from_YUV420_F(int row, int column, image_type *im_y, image_type *im_u,
									image_type *im_v, int bufCol);

static image_type *	image_from_YUV420_I(int row, int column,
									image_type *im_y, image_type *im_u,
									image_type *im_v, int bufCol, image_type *im );




image_type *
image_read_yuv( int row, int column, int frame, char *file )
{
FILE	*fp;
image_type	*im,	*im_y,	*im_u,	*im_v;
int	y,	u,	v;
int	r,	g,	b;
int	i,	j;
u_int	*p;
u_char	*py,	*pu,	*pv;
int	flag;
	
	if( (fp = fopen( file, "rb" )) == NULL )
		return( NULL );

	fseek( fp, (long)(row*column*1.5*frame), 0L );


	im_y = image_read_row( row, column, fp );

	im_v = image_read_row( row>>1, column>>1, fp );
	im_u = image_read_row( row>>1, column>>1, fp );


	im = image_create( row, column, 4, 1, 0 );



	for( i = 0 ; i < row ; i++ ){
		p = IMAGE4_PIXEL( im, i, 0 );
		py = IMAGE_PIXEL( im_y, i, 0 );
		pu = IMAGE_PIXEL( im_u, i>>1, 0 );
		pv = IMAGE_PIXEL( im_v, i>>1, 0 );

		flag = 0;
		for (j = 0; j < column ; j++) {
			y = *py;
			u = *pu;
			v = *pv;

			py++;

			if( flag == 1){
				pu++;
				pv++;
				flag = 0;
			}
			else	flag = 1;


			r = (int)((v-128) / 0.877 + y);
			b = (int)((u-128) /0.492 + y);

			g = (int)((y - 0.299*r - 0.144* b)/0.587);

			r = PUSH_TO_RANGE( r, 0, 255 );
			g = PUSH_TO_RANGE( g, 0, 255 );
			b = PUSH_TO_RANGE( b, 0, 255 );

			*p++ = IMAGE4_RGB( r, g, b );
		}

	}

	image_destroy( im_y, 1 );
	image_destroy( im_u, 1 );
	image_destroy( im_v, 1 );

	return( im );
}

image_type *
image_read_row( int row, int column, FILE *fp )
{
image_type *im;

	im = image_create( row, column, 1, 1, 0 );

	fread( im->data, 1, im->row*im->column, fp );

	return( im );
}




image_type *
image_create_from_yuv( int row, int col,
								  char *yb, char *ub, char*vb,
								  int bufCol)
{
image_type *im, *imY, *imU, *imV;
	
	

	imY = image_create(row,bufCol,  1, 1, (u_char*)yb);
	imU = image_create( row/2,bufCol/2,1, 1, (u_char*)ub);	
	imV = image_create(row/2, bufCol/2, 1, 1, (u_char*)vb);
//	im = image_from_YUV420_F(row, col, imY, imU, imV, bufCol);
	im = image_from_YUV420_I(row, col, imY, imU, imV, bufCol, NULL );

	image_destroy(imY,0);
	image_destroy(imU,0);
	image_destroy(imV,0);

	return im;
}




static image_type *
image_from_YUV420_F(int row, int column, image_type *im_y, image_type *im_u,
									image_type *im_v, int bufCol)
{
	u_int	*p;
	u_char	*py,	*pu,	*pv;
	int	y,	u,	v;
	int	r,	g,	b;
	int i,j;
	int flag;
	image_type *im;




	im = image_create( row, column, 4, 1, 0 );

	
	for( i = 0 ; i < row ; i++ )
	{
		p = IMAGE4_PIXEL( im, i, 0 );
		py = IMAGE_PIXEL( im_y, i, 0 );
		pu = IMAGE_PIXEL( im_u, i>>1, 0 );
		pv = IMAGE_PIXEL( im_v, i>>1, 0 );

		flag = 0;
		for (j = 0; j < column ; j++) 
		{
			y = *py;
			u = *pu;
			v = *pv;

			py++;

			if( flag == 1){
				pu++;
				pv++;
				flag = 0;
			}
			else	flag = 1;


			r = (int)((v-128) / 0.877 + y);
			b = (int)((u-128) /0.492 + y);

			g = (int)((y - 0.299*r - 0.144* b)/0.587);

			r = PUSH_TO_RANGE( r, 0, 255 );
			g = PUSH_TO_RANGE( g, 0, 255 );
			b = PUSH_TO_RANGE( b, 0, 255 );

			*p++ = IMAGE4_RGB( r, g, b );
		}

	}
	return im;
}





/*
image_type *image_create_from_yuv_fxPnt(int row, int col, char *yb, char *ub, char*vb, int bufCol)
{

	

	image_type *im, *imY, *imU, *imV;
	
	

	imY = image_create(row,bufCol,  1, 1, (u_char*)yb);
	imU = image_create( row/2,bufCol/2,1, 1, (u_char*)ub);	
	imV = image_create(row/2, bufCol/2, 1, 1, (u_char*)vb);
	im = image_from_YUV420_I(row, col, imY, imU, imV, bufCol, NULL );

	image_destroy(imY,0);
	image_destroy(imU,0);
	image_destroy(imV,0);
	return im;
 
}
*/




static image_type *
image_from_YUV420_I(int row, int column, image_type *im_y, image_type *im_u,
									image_type *im_v, int bufCol, image_type *im )
{
u_int	*p;
u_char	*py,	*pu,	*pv;
int	y,	u,	v;
int	r,	g,	b;
int i,j;
int flag;
//	image_type *im;


	if( im != NULL && (im->row != row || im->column != column || im->depth != 4) ){
		image_destroy( im, 1 );
		im = NULL;
	}

	if( im == NULL )
		im = image_create( row, column, 4, 1, 0 );


	for( i = 0 ; i < row ; i++ )
	{
		p = IMAGE4_PIXEL( im, i, 0 );
		py = IMAGE_PIXEL( im_y, i, 0 );
		pu = IMAGE_PIXEL( im_u, i>>1, 0 );
		pv = IMAGE_PIXEL( im_v, i>>1, 0 );

		flag = 0;
		for (j = 0; j < column ; j++)
		{
			y = *py;
			u = *pu;
			v = *pv;

			py++;

			if( flag == 1){
				pu++;
				pv++;
				flag = 0;
			}
			else	flag = 1;


			//r = (int)((v-128) / 0.877 + y); = (int)((v-128) * 1.14025 + y);
			// 256 / 0.877 = 291.9 = ~292
			r = (int)((((v-128) * 292 ) >> 8 ) + y);
			//b = (int)((u-128) /0.492 + y); = (int)((u-128)* 2.0325 + y);
			// 256 / 0.492 = 520.325 = ~520
			b = (int)((((u-128) * 520 ) >> 8 ) + y);
			//g = (int)((y - 0.299*r - 0.144* b)/0.587);
			// 256 / 0.587 = 430.11 = ~430
			// 256 * 0.299 = 76.544 = ~76
			// 256 * 0.144 = 36.864 = ~37
			g = (int)(((y - ((76*r)>>8) - ((37* b)>>8)) * 430) >> 8 );

			r = PUSH_TO_RANGE( r, 0, 255 );
			g = PUSH_TO_RANGE( g, 0, 255 );
			b = PUSH_TO_RANGE( b, 0, 255 );

			*p++ = IMAGE4_RGB( r, g, b );
		}

	}
	return im;
}



image_type *
image_from_YUV420(int row, int column, u_char *pY, u_char *pU,
									u_char *pV, int bufCol, image_type *im )
{
u_int	*p;
u_char	*py,	*pu,	*pv,	*pu0,	*pv0;
int	y,	u,	v;
int	r,	g,	b;
int i,j;
int	algin;//,	algin2;


	if( im != NULL && (im->row != row || im->column != column || im->depth != 4) ){
		image_destroy( im, 1 );
		im = NULL;
	}

	if( im == NULL )
		im = image_create( row, column, 4, 1, 0 );

	algin = bufCol - column;
//	algin2 = algin >> 1;

	py = pY;
	pv0 = pV;
	pu0 = pU;

	
	p = (u_int *)im->data;
	for( i = 0 ; i < row ; i++ ){
		pu = pu0;
		pv = pv0;
		for (j = 0; j < column ; j++)
		{
			y = *py;
			u = *pu;
			v = *pv;

			py++;

			if( j & 0x01 ){
				pu++;
				pv++;
			}



			//r = (int)((v-128) / 0.877 + y); = (int)((v-128) * 1.14025 + y);
			// 256 / 0.877 = 291.9 = ~292
			r = (int)((((v-128) * 292 ) >> 8 ) + y);
			//b = (int)((u-128) /0.492 + y); = (int)((u-128)* 2.0325 + y);
			// 256 / 0.492 = 520.325 = ~520
			b = (int)((((u-128) * 520 ) >> 8 ) + y);
			//g = (int)((y - 0.299*r - 0.144* b)/0.587);
			// 256 / 0.587 = 430.11 = ~430
			// 256 * 0.299 = 76.544 = ~76
			// 256 * 0.144 = 36.864 = ~37
			g = (int)(((y - ((76*r)>>8) - ((37* b)>>8)) * 430) >> 8 );

			r = PUSH_TO_RANGE( r, 0, 255 );
			g = PUSH_TO_RANGE( g, 0, 255 );
			b = PUSH_TO_RANGE( b, 0, 255 );

			*p++ = IMAGE4_RGB( r, g, b );
		}

		py += algin;
		if( i& 0x01){
			pu0 += (bufCol>>1);
			pv0 += (bufCol>>1);
		}
		

	}
	return im;
}