/*****************************
 ***	ImageTransparent   ***
 *****************************/

#include	"ImageType/ImageType.h"


static image_type *		image4_transparent_from_image1(image_type * sim,
							   float trans, int backcolor, int flipV, image_type *im);

static image_type *		image4_transparent_from_image3(image_type * sim,
							   float trans, int backcolor, int flipV, image_type *im );



image_type * 
image_transparent(image_type * sim,
				float trans, int backcolor, int flipV, image_type *im)
{
unsigned int * sp,*tp;
int	R,G,B,Rb,Gb,Bb;
int	i,j,	ti;

	if( sim->depth == 1 ){
		im = image4_transparent_from_image1( sim, trans, backcolor, flipV, im );
		return( im );
	}

	if( sim->depth == 3 ){
		im = image4_transparent_from_image3( sim, trans, backcolor, flipV, im );
		return( im );
	}
	Rb = IMAGE4_RED(backcolor); 
	Gb = IMAGE4_GREEN(backcolor);
	Bb = IMAGE4_BLUE(backcolor);


	im = image_recreate( im, sim->height, sim->width, 4 , 1 );
	
	sp = (u_int *)sim->data;
//	tp = (u_int *)im->data;


	for( i = 0 ; i< sim->height  ; i++ ){
		ti = ( flipV == 0 )? i : sim->height-1 - i;
		tp = IMAGE4_PIXEL( im, ti, 0 );

		for( j = 0 ; j< sim->width ; j++ ){			
			
			R = IMAGE4_RED(*sp); 
			G = IMAGE4_GREEN(*sp);
			B = IMAGE4_BLUE(*sp);
			sp++;



			R = R - ((R-Rb)*trans);
			G = G - ((G-Gb)*trans);
			B = B - ((B-Bb)*trans);

			*tp = IMAGE4_RGB(R,G,B);
			tp++;
			
		}
	}

	return( im );
}


static image_type * 
image4_transparent_from_image3(image_type * sim,
				  float trans, int backcolor, int flipV, image_type *im )
{
u_char *sp;
u_int	*tp;
int	R,G,B,Rb,Gb,Bb;
int	i,j,	ti;



	Rb = IMAGE4_RED(backcolor); 
	Gb = IMAGE4_GREEN(backcolor);
	Bb = IMAGE4_BLUE(backcolor);


	im = image_recreate( im, sim->height, sim->width, 4 , 1 );

	sp = sim->data;
	


	for( i = 0 ; i< sim->height  ; i++ ){
		ti = ( flipV == 0 )? i : sim->height-1 - i;
		tp = IMAGE4_PIXEL( im, ti, 0 );

		for( j = 0 ; j< sim->width ; j++ ){			

			R = *sp++;
			G = *sp++;
			B = *sp++;




			R = R - ((R-Rb)*trans);
			G = G - ((G-Gb)*trans);
			B = B - ((B-Bb)*trans);

			*tp = IMAGE4_RGB(R,G,B);
			tp++;

		}
	}

	return( im );
}


static image_type * 
image4_transparent_from_image1(image_type * sim,
				  float trans, int backcolor, int flipV, image_type *im)
{
u_char *sp;
u_int	*tp;
int	R	,Rb;
int	i,	j,	ti;


	im = image_recreate( im, sim->height, sim->width,4 , 1 );

	sp = sim->data;
//	tp = (u_int *)im->data;

	Rb = backcolor & 0xFF; 

	for( i = 0 ; i< sim->height  ; i++ ){
		ti = ( flipV == 0 )? i : sim->height-1 - i;
		tp = IMAGE4_PIXEL( im, ti, 0 );

		for( j = 0 ; j< sim->width ; j++ ){			

			R = *sp++;
	

			R = R - ((R-Rb)*trans);

			*tp++ = IMAGE4_RGB( R, R, R);
		}
	}

	return( im );
}





image_type * 
image_transparent_B( image_type *sim, int x0, int y0, int width, int height,
					float trans, int backcolor, image_type *im)
{
	u_int	*sp,	*tp;
	int	R,	G,	B,	Rb,	Gb,	Bb;
	int	i,	j;



	if( x0 < 0 )	x0 = 0;
	if( y0 < 0 )	y0 = 0;
	if( width < 0 )	width = sim->column - x0;
	if( height < 0 )	height = sim->row - y0;


	if(im == NULL)
		im = image_create( height, width, 4, 1, NULL);

	if( width > im->column )	width = im->column;
	if( height > im->row )		height = im->row;



	Rb = IMAGE4_RED(backcolor); 
	Gb = IMAGE4_GREEN(backcolor);
	Bb = IMAGE4_BLUE(backcolor);



	for( i = 0 ; i < height ; i++ ){

		sp = (u_int *)IMAGE_PIXEL( sim, i+y0, x0 );
		tp = (u_int *)IMAGE_PIXEL( im, i, 0 );

		for( j = 0 ; j < width ; j++ ){			
			
			R = IMAGE4_RED(*sp); 
			G = IMAGE4_GREEN(*sp);
			B = IMAGE4_BLUE(*sp);
			sp++;



			R = R - ((R-Rb)*trans);
			G = G - ((G-Gb)*trans);
			B = B - ((B-Bb)*trans);

			*tp = IMAGE4_RGB(R,G,B);
			tp++;
			
		}
	}

	return( im );
}



image_type * 
image4_transparent( image_type *sim, box2i *b, float trans, int backcolor, image_type *im )
{
	u_int	*sp,	*tp;
	int	R,	G,	B,	Rb,	Gb,	Bb;
	int	i,	j;

	
	im = image_recreate( im, sim->height, sim->width, 4, 1 );


	Rb = IMAGE4_RED(backcolor); 
	Gb = IMAGE4_GREEN(backcolor);
	Bb = IMAGE4_BLUE(backcolor);


	for( i = b->y0 ; i < b->y1 ; i++ ){

		sp = (u_int *)IMAGE_PIXEL( sim, i, b->x0 );
		tp = (u_int *)IMAGE_PIXEL( im, i, b->x0 );

		for( j = b->x0 ; j < b->x1 ; j++ ){			

			R = IMAGE4_RED(*sp); 
			G = IMAGE4_GREEN(*sp);
			B = IMAGE4_BLUE(*sp);
			sp++;



			R = R + (Rb - R)*trans;
			G = G + (Gb - G)*trans;
			B = B + (Bb - B)*trans;

			*tp = IMAGE4_RGB(R,G,B);
			tp++;

		}
	}

	return( im );
}



void
image_transparent_set(image_type *bim, box2i *b, image_type *im, int transpart_index )
{
int	x0,	y0,	x1, y1;
u_char	*sp,	*tp;
int	i,	j;

//	image_box( bim, b, &i0, &j0, &row, &column );
	imageA_box( bim, b, &x0, &y0, &x1, &y1 );


	for( i = y0 ; i < y1 ; i++ ){
		sp = IMAGE_PIXEL( bim, i, x0 );
		tp = IMAGE_PIXEL( im, i-y0, 0 );

		for( j = x0 ; j < x1 ; j++, tp++, sp++ ){
			if( *tp == *sp )
				*tp = transpart_index;
		}
	}
}



void
image_transparent_copy_in(image_type *bim, box2i *b, image_type *im, int transpart_index )
{
int	x0,	y0,	x1, y1;
u_char	*sp,	*tp;
int	i,	j;

//	image_box( bim, b, &j0, &i0, &column, &row );
	imageA_box( bim, b, &x0, &y0, &x1, &y1 );


	for( i = y0 ; i < y1 ; i++ ){
		sp = IMAGE_PIXEL( bim, i, x0 );
		tp = IMAGE_PIXEL( im, i-y0, 0 );

		for( j = x0 ; j < x1 ; j++, tp++, sp++ ){
			if( *tp != transpart_index )
				*sp = *tp;
		}
	}
}




/*
void
imageT_clear( image_type *im, int color )
{
u_int	*tp;
int	i,	j;

	tp = (u_int *)im->data;

	for( i=0 ; i < im->row ; i++ )
		for( j=0 ; j < im->column ; j++, tp++ )
			if( (*tp) & 0xF0000000 )
				*tp = color;
}
*/			

image_type *    
image_border_fade(image_type * sim,
						float trans, int backcolor,
						int r0, int c0, int row0, int col0, image_type *im)
{
int	row = IMAGE_ROW(sim);
int	col = IMAGE_COLUMN(sim);
unsigned int * sp,*tp;
int	R,	G,	B,	Rb,	Gb,	Bb;
int	i,j;
//int	backcolor;

//	backcolor = IMAGE4_RGB(128,128,128);

	if(im == NULL)
		im = image_create(row,col,4,1,NULL);
	
	sp = (u_int *)sim->data;
	tp = (u_int *)im->data;

	Rb = IMAGE4_RED(backcolor); 
	Gb = IMAGE4_GREEN(backcolor);
	Bb = IMAGE4_BLUE(backcolor);

	for( i=0 ; i<row ; i++ ){
		for( j=0 ; j<col ; j++ ){			
			if(( i < r0 || i > r0 + row0 ) ||
				(( i >= r0 && i <= r0 + row0 ) && (j < c0 || j > c0 + col0 ))){
			
				R = IMAGE4_RED(*sp); 
				G = IMAGE4_GREEN(*sp);
				B = IMAGE4_BLUE(*sp);
				sp++;

				R = R - ((R-Rb)*trans);
				G = G - ((G-Gb)*trans);
				B = B - ((B-Bb)*trans);

				*tp = IMAGE4_RGB(R,G,B);
				tp++;
			}
			else {
				*tp = *sp;
				sp++;
				tp++;

			}
			
		}
	}

	return( im );
}


// opacity: 0 - opaque  255: transparent
void   
image_transparent_copy(image_type * sim,
						int r0, int c0, int row0, int col0, image_type *im )
{
u_int	*sp,	*tp;
int		R,	G,	B,	Rb,	Gb,	Bb;
int		i,j;
float	trans;
int		r1;//row = IMAGE_ROW(sim), r1;
int		c1;//col = IMAGE_COLUMN(sim), c1;
int	sc0,	sr0;
int	t;

	if( row0 <= 0 )	row0 = sim->height;
	if( col0 <= 0 ) col0 = sim->width;

	sc0 = 0;
	if( c0 < 0 ){
		sc0 = -c0;
		col0 += c0;
		c0 = 0;
	}
	sr0 = 0;
	if( r0 < 0 ){
		sr0 = -r0;
		row0 += r0;
		r0 = 0;
	}

	
	sp = (u_int *)sim->data;
	
	r1 = MIN( r0+row0, im->row );
	c1 = MIN( c0 + col0, im->column );

	for( i = r0; i < r1; i++ ){
		tp = (u_int *)im->data + i*im->column + c0;
		sp = (u_int *)sim->data + (i-r0+sr0)* sim->column + sc0;

		for( j = c0 ; j < c1; j++, sp++, tp++ ){

			t = (*sp) >> 24;
			if( t == 255 )
				continue;
			
			if( t == 0 ){
				*tp = *sp;
				continue;
			}


			trans = t / 255.0;


			Rb = IMAGE4_RED(*tp); 
			Gb = IMAGE4_GREEN(*tp);
			Bb = IMAGE4_BLUE(*tp);

			R = IMAGE4_RED(*sp); 
			G = IMAGE4_GREEN(*sp);
			B = IMAGE4_BLUE(*sp);

			R = R - ((R-Rb)*trans);
			G = G - ((G-Gb)*trans);
			B = B - ((B-Bb)*trans);

			*tp = IMAGE4_RGB(R,G,B);
		}
	}
}



// opacity: 0 - opaque  255: transparent
image_type *
imageT_clear_color( image_type *sim, int color, image_type *im )
{
u_int	*sp,	*tp;
int		R,	G,	B,	Rb,	Gb,	Bb;
int		i,j;
int		tr;


	if(im == NULL)
		im = image_create( sim->row, sim->column, 4, 1, NULL);


	Rb = IMAGE4_RED(color); 
	Gb = IMAGE4_GREEN(color);
	Bb = IMAGE4_BLUE(color);

	sp = ( u_int*)sim->data;
	tp = ( u_int*)im->data;
	for( i = 0; i < im->row; i++ ){
		for( j = 0 ; j < im->column; j++, sp++, tp++ ){


			tr = ( *sp ) >> 24;
			if( tr == 0 ){
				*tp = *sp & 0x00FFFFFF;
				continue;
			}


			R = IMAGE4_RED(*sp); 
			G = IMAGE4_GREEN(*sp);
			B = IMAGE4_BLUE(*sp);

			R = R + (((Rb - R)*tr)>>8);
			G = G + (((Gb - G)*tr)>>8);
			B = B + (((Bb - B)*tr)>>8);

			*tp = IMAGE4_RGB(R,G,B);
		}
	}

	return( im );
}


// opacity: 0 - opaque  255: transparent
void
imageT_set_by_color( image_type *im, int color )
{
u_int	*sp;
int	i,	j;

	sp = ( u_int*)im->data;
	for( i = 0; i < im->row; i++ ){
		for( j = 0 ; j < im->column; j++, sp++ ){

			if( *sp == (u_int)color )
				*sp |= 0xFF000000;
		}
	}
}

image_type *
imageT_copy_alpha( image_type *sim,	image_type *im )
{
u_int	*sp;
u_char	*tp;

int	i,	j;

	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	sp = sim->data_ui;
	tp = im->data;
	for( i = 0; i < im->row; i++ ){
		for( j = 0 ; j < im->column; j++, sp++, tp++ ){
			*tp = ( *sp >> 24)&0xFF;
		}
	}

	return( im );
}

int
imageT_set_FF( image_type *sim )
{

	int	i,	j;

	

	u_int *sp = sim->data_ui;
	
	for( i = 0; i < sim->height; i++ ){
		for( j = 0 ; j < sim->width; j++, sp++){
			*sp |= 0xFF000000;
		}
	}

	return( 1 );
}

int
	imageT_set_00( image_type *sim )
{

	int	i,	j;



	u_int *sp = sim->data_ui;

	for( i = 0; i < sim->height; i++ ){
		for( j = 0 ; j < sim->width; j++, sp++){
			*sp &= 0x00FFFFFF;
		}
	}

	return( 1 );
}


int
imageT_nPixel( image_type *sim )
{
	int	i,	j,	n;

	u_int *sp = sim->data_ui;

	for( i = 0, n = 0 ; i < sim->height; i++ ){
		for( j = 0 ; j < sim->width; j++, sp++){
			if( *sp & 0xFF000000 )
				n++;
		}
	}

	return( n );
}


void
imageT_clearN( image_type *image )
{
int i, j;
u_int *p;

	p = (u_int *)image->data;

    for( i = 0 ; i < image->row; i++ )
		for( j = 0; j < image->column; j++, p++ )
			*p &= 0x00FFFFFF;
}




void
imageT_rectangle( image_type *im,  int *i0, int *i1, int *j0, int *j1 )
{
u_int	*sp;
int *row_arr,	*col_arr;
int	i,	j;



	row_arr = (int *)malloc((im->row)*sizeof(int));
	col_arr = (int *)malloc((im->column)*sizeof(int));

	for ( i = 0 ; i < im->row ; i++ )
		row_arr[i] = 0;

	for ( i = 0 ; i < im->column ; i++ )
		col_arr[i] = 0;


	sp = (u_int *)im->data;
	for ( i = 0 ; i < im->row ; i++ ) {
		for ( j = 0 ; j < im->column ; j++, sp++ ) {
			row_arr[i] |= *sp;
			col_arr[j] |= *sp;
		}
	}



	*i0 = 0;
	for ( i = 0 ; i < im->row ; i++ )
		if( row_arr[i] & 0xFF000000 ) {
			*i0 = i-1;
			if( *i0 < 0 )	*i0 = 0;
			break;
		}

	*i1 = im->row;
	for ( i = im->row - 1 ; i >= 0  ; i-- )
		if( row_arr[i] & 0xFF000000 ){
			*i1 = i;
			break;
		}

	*j0 = 0;
	for( j = 0 ; j < im->column ; j++ )
		if( col_arr[j] & 0xFF000000 ){
			*j0 = j-1;
			if( *j0 < 0 )	*j0 = 0;
			break;
		}

	*j1 = im->column-1;
	for ( j = im->column - 1 ; j >= 0 ; j-- )
		if( col_arr[j] & 0xFF000000 ){
			*j1 = j;
			break;
		}

	free( row_arr );
	free( col_arr );
}




image_type *
imageT_negative_alpha( image_type *sim,	image_type *im )
{
u_int	*sp,	*tp;
int	alpha;

	int	i,	j;

	im = image_recreate( im, sim->height, sim->width, 4, 1 );

	sp = sim->data_ui;
	tp = im->data_ui;
	for( i = 0; i < im->row; i++ ){
		for( j = 0 ; j < im->column; j++, sp++, tp++ ){
			alpha = (*sp >> 24) & 0xFF;
			alpha = 255 - alpha;
			*tp = ( alpha << 24) | ( *sp & 0x00FFFFFF);
		}
	}

	return( im );
}


image_type *
imageT_alpha64_stretching( image_type *sim, image_type *im )
{
u_int	*sp,	*tp,	w;
int		i,	j;
u_int	H[256];

	for( i = 0 ; i < 64 ; i++ )
		H[i] = i << 26;

	for( i = 64 ; i < 256 ; i++ )
		H[i] = 0XFF000000;


	im = image_recreate( im, sim->height, sim->width, 4, 1 );

	sp = (u_int *)sim->data;
	tp = (u_int *)im->data;

	for( i = 0 ; i < sim->row ; i++ )
		for( j = 0 ; j < sim->column ; j++, sp++, tp++ ){

//			w = (*sp >> 24 )&0xFF;
			w = (*sp & 0XFF000000) >> 24;

			*tp = H[w] | (*sp & 0x00FFFFFF);
		}

		return( im );
}





void
imageA_copy_in( image_type *sim, int x0, int y0, image_type *im )
{
	int	x1, y1;
	u_int	*sp,	*tp;
	int	i,	j,	k;
	int	sw,	tw,	w;
	int	R,	G,	B;
	float	fw;

	int	sy0,	sx0;


	x1 = x0+sim->width;
	if( x1 > im->width )	x1 = im->width;
	y1 = y0+sim->height;
	if( y1 > im->height )	y1 = im->height;

	if( y0 < 0 ){
		sy0 = -y0;
		y0 = 0;
	}
	else	sy0 = 0;

	if( x0 < 0 ){
		sx0 = -x0;
		x0 = 0;
	}
	else	sx0 = 0;


	for( i = y0, k = sy0 ; i < y1 ; i++, k++ ){
		sp = IMAGE4_PIXEL( sim, k, sx0 );
		tp = IMAGE4_PIXEL( im, i, x0 );

		for( j = x0 ; j < x1 ; j++, tp++, sp++ ){
			sw = (*sp)>>24;
			if( sw == 255 )	continue;
			if( sw == 0 ){
				*tp = *sp;
				continue;
			}

			tw = (*tp)>>24;

			w = MIN( tw, sw );

			sw = (255 - sw );
			tw = ( 255 - tw );
			fw = 1.0/(sw+tw);



			R = fw * ( sw * IMAGE4_RED(*sp) + tw *  IMAGE4_RED(*tp) );
			G = fw * ( sw * IMAGE4_GREEN(*sp) + tw *  IMAGE4_GREEN(*tp) );
			B = fw * ( sw * IMAGE4_BLUE(*sp) + tw *  IMAGE4_BLUE(*tp) );


			*tp = (w<<24) | IMAGE4_RGB( R, G, B );

		}
	}
}


image_type *
imageA_final( image_type *sim, int color, image_type *im )
{
u_int	*sp,	*tp;
int	i,	j;
int	w;
int	R,	G,	B;
float	fw;


	im = image_realloc( im, sim->width, sim->height, 4, IMAGE_TYPE_U8, 1 );



	fw = 1.0/255.0;

	sp = sim->data_ui;
	tp = im->data_ui;
	for( i = 0 ; i < sim->height ; i++){

		for( j = 0 ; j < sim->width ; j++, tp++, sp++ ){
			w = (*sp)>>24;
			if( w == 0 ){
				*tp = *sp;
				continue;
			}


			if( w == 255 ){
				*tp = color;
				continue;
			}

			R = fw * ( (255-w) * IMAGE4_RED(*sp) + w *  IMAGE4_RED(color) );
			G = fw * ( (255-w) * IMAGE4_GREEN(*sp) + w *  IMAGE4_GREEN(color) );
			B = fw * ( (255-w) * IMAGE4_BLUE(*sp) +  w *  IMAGE4_BLUE(color) );


			*tp = IMAGE4_RGB( R, G, B );

		}
	}

	return( im );
}


image_type *
imageA_finalB( image_type *sim, int color, image_type *im )
{
	u_int	*sp,	*tp;
	int	i,	j,	w;





	im = image_realloc( im, sim->width, sim->height, 4, IMAGE_TYPE_U8, 1 );



	sp = sim->data_ui;
	tp = im->data_ui;
	for( i = 0 ; i < sim->height ; i++){

		for( j = 0 ; j < sim->width ; j++, tp++, sp++ ){
			w = (*sp)>>24;
//			if( w == 0 ){
			if( w < 128 ){
				*tp = *sp;
				continue;
			}



			*tp = color;
		}
	}

	return( im );
}




static image_type *imageA_set_alpha3( image_type *sim, int transparent, image_type *mim, image_type *im );

image_type *
imageA_set_alpha( image_type *sim, int transparent, image_type *mim, image_type *im )
{
u_int	*sp,	*tp;
u_char	*mp;
int	i,	j;
float f;


	if( sim->depth == 3 ){
		im = imageA_set_alpha3( sim, transparent, mim, im );
		return( im );
	}


	f = transparent / 255.0;

	im = image_realloc( im, sim->width, sim->height, 4, IMAGE_TYPE_U8, 1 );

	sp = sim->data_ui;
	mp = mim->data;
	tp = im->data_ui;
	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, sp++, mp++, tp++ ){

			int t = 255 - f* *mp;

			*tp = *sp | ( t << 24 );
		}
	}

	return( im );
}


static image_type *
imageA_set_alpha3( image_type *sim, int transparent, image_type *mim, image_type *im )
{
	u_int	*tp;
	u_char	*sp,	*mp;
	int	i,	j;
	int	R,	G,	B;

	float	f = transparent / 255.0;

	im = image_realloc( im, sim->width, sim->height, 4, IMAGE_TYPE_U8, 1 );

	sp = sim->data;
	mp = mim->data;
	tp = im->data_ui;
	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, mp++, tp++ ){

			int t = 255 - f* *mp;
			R = *sp++;
			G = *sp++;
			B = *sp++;
			*tp = IMAGE4_RGB( R, G, B ) | ( t << 24 );
		}
	}

	return( im );
}


void
imageA_set_alpha_band( image_type *sim, image_type *aim )
{

	int	i,	j;




	u_int *sp = sim->data_ui;
	u_char *ap = aim->data;

	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, sp++, ap++ ){

			u_int t = *ap;

			*sp = (*sp  & 0xFFFFFF )| ( t << 24 );

			///*sp = (0xFF0000 ) | ( t << 24 );
		}
	}

}


static image_type *	imageA_set_alpha3_inversed_mask( image_type *sim, int transparent, image_type *mim, image_type *im );


image_type *
imageA_set_alpha_inversed_mask( image_type *sim, int transparent, image_type *mim, image_type *im )
{
	u_int	*sp,	*tp;
	u_char	*mp;
	int	i,	j;
	float f;


	if( sim->depth == 3 ){
		im = imageA_set_alpha3_inversed_mask( sim, transparent, mim, im );
		return( im );
	}


	f = transparent / 255.0;

	im = image_realloc( im, sim->width, sim->height, 4, IMAGE_TYPE_U8, 1 );

	sp = sim->data_ui;
	mp = mim->data;
	tp = im->data_ui;
	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, sp++, mp++, tp++ ){

			int t = 255 - f* (255-*mp);

			*tp = *sp | ( t << 24 );
		}
	}

	return( im );
}

static image_type *
imageA_set_alpha3_inversed_mask( image_type *sim, int transparent, image_type *mim, image_type *im )
{
	u_int	*tp;
	u_char	*sp,	*mp;
	int	i,	j;
	int	R,	G,	B;

	float	f = transparent / 255.0;

	im = image_realloc( im, sim->width, sim->height, 4, IMAGE_TYPE_U8, 1 );

	sp = sim->data;
	mp = mim->data;
	tp = im->data_ui;
	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, mp++, tp++ ){

			int t = 255 - f* (255-*mp);
			R = *sp++;
			G = *sp++;
			B = *sp++;
			*tp = IMAGE4_RGB( R, G, B ) | ( t << 24 );
		}
	}

	return( im );
}


image_type *
imageA_set_color( image_type *sim, image_type *mim, int transparent, int color, image_type *im )
{
	u_char	*tp;
	u_char	*sp,	*mp;
	int	i,	j;
	int	R,	G,	B,	w;
	float	fw;

	float	f = transparent / 255.0;

	im = image_realloc( im, sim->width, sim->height, 3, IMAGE_TYPE_U8, 1 );


	R = IMAGE4_RED(color);
	G = IMAGE4_GREEN(color);
	B = IMAGE4_BLUE(color);

	fw = 1.0/255.0;

	sp = sim->data;
	mp = mim->data;
	tp = im->data;
	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, mp++,sp += 3 ){

			if( *mp == 0 ){
				*tp++ = R;
				*tp++ = G;
				*tp++ = B;
				continue;
			}

			w = 255 - f* *mp;

			if( w == 0 ){
				*tp++ = sp[0];
				*tp++ = sp[1];
				*tp++ = sp[2];
				continue;
			}




			*tp++ = fw * ( (255-w) * sp[0] + w *  R);
			*tp++ = fw * ( (255-w) * sp[1] + w *  G );
			*tp++ = fw * ( (255-w) * sp[2] +  w *  B );

		}
	}

	return( im );
}


image_type *
imageA_set_colorN( image_type *sim, image_type *mim, int color, image_type *im )
{
	u_char	*tp;
	u_char	*sp,	*mp;
	int	i,	j;
	int	R,	G,	B,	w;




	im = image_realloc( im, sim->width, sim->height, 3, IMAGE_TYPE_U8, 1 );


	R = IMAGE4_RED(color);
	G = IMAGE4_GREEN(color);
	B = IMAGE4_BLUE(color);


	sp = sim->data;
	mp = mim->data;
	tp = im->data;
	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, mp++,sp += 3 ){

			if( *mp == 0 ){
				*tp++ = R;
				*tp++ = G;
				*tp++ = B;
				continue;
			}


			if( *mp == 255 ){
				*tp++ = sp[0];
				*tp++ = sp[1];
				*tp++ = sp[2];
				continue;
			}


			w = *mp +1 ;


			*tp++ =  (( w * ( sp[0] - R)) >> 8)  + R;
			*tp++ =  (( w * ( sp[1] - G)) >> 8)  + G;
			*tp++ =  (( w * ( sp[2] - B)) >> 8)  + B;

		}
	}

	return( im );
}


image_type *
	imageA_set_colorM( image_type *sim, int color, image_type *im )
{
	u_char	*tp;
	u_int	*sp;
	int	i,	j;
	int	R,	G,	B;




	im = image_realloc( im, sim->width, sim->height, 3, IMAGE_TYPE_U8, 1 );


	R = IMAGE4_RED(color);
	G = IMAGE4_GREEN(color);
	B = IMAGE4_BLUE(color);


	sp = sim->data_ui;
	
	tp = im->data;
	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, sp ++ ){

			int r = IMAGE4_RED(*sp);
			int g = IMAGE4_GREEN(*sp);
			int b = IMAGE4_BLUE(*sp);
			int w = (*sp)>>24 & 0xFF;


			if( w == 0 ){

				*tp++ = R;
				*tp++ = G;
				*tp++ = B;
				continue;
			}




			if( w == 255 ){
				*tp++ = r;
				*tp++ = g;
				*tp++ = b;
				continue;
			}


			w = w +1 ;


			*tp++ =  (( w * ( r - R)) >> 8)  + R;
			*tp++ =  (( w * ( g - G)) >> 8)  + G;
			*tp++ =  (( w * ( b - B)) >> 8)  + B;

		}
	}

	return( im );
}


image_type *
	imageA_set_colorM4( image_type *sim, int color, image_type *im )
{
	u_int	*tp;
	u_int	*sp;
	int	i,	j;
	int	R,	G,	B;




	im = image_realloc( im, sim->width, sim->height, 4, IMAGE_TYPE_U8, 1 );


	R = IMAGE4_RED(color);
	G = IMAGE4_GREEN(color);
	B = IMAGE4_BLUE(color);


	sp = sim->data_ui;

	tp = im->data_ui;
	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, sp ++ ){

			int r = IMAGE4_RED(*sp);
			int g = IMAGE4_GREEN(*sp);
			int b = IMAGE4_BLUE(*sp);
			int w = (*sp)>>24 & 0xFF;
			w = 255 - w;


			if( w == 0 ){
				*tp++ = color;

				//*tp++ = R;
				//*tp++ = G;
				//*tp++ = B;
				//*tp++ = 0;
				continue;
			}




			if( w == 255 ){
				//*tp++ = 0;
				//*tp++ = r;
				//*tp++ = g;
				//*tp++ = b;

				*tp++ = IMAGE4_RGB( r, g, b );
				continue;
			}


			w = w +1 ;

			//*tp++ = 0;
			//*tp++ =  (( w * ( r - R)) >> 8)  + R;
			//*tp++ =  (( w * ( g - G)) >> 8)  + G;
			//*tp++ =  (( w * ( b - B)) >> 8)  + B;
			r =  (( w * ( r - R)) >> 8)  + R;
			g =  (( w * ( g - G)) >> 8)  + G;
			b =  (( w * ( b - B)) >> 8)  + B;
			*tp++ = IMAGE4_RGB( r, g, b );

		}
	}

	return( im );
}



void
imageA_combine( image_type *sim, image_type *im )
{

	int	i,	j;
	



	u_int *sp = sim->data_ui;
	u_int *tp = im->data_ui;


	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, tp++, sp++ ){

			int r = IMAGE4_RED(*sp);
			int g = IMAGE4_GREEN(*sp);
			int b = IMAGE4_BLUE(*sp);
			int w = (*sp)>>24 & 0xFF;

			int tr = IMAGE4_RED(*tp);
			int tg = IMAGE4_GREEN(*tp);
			int tb = IMAGE4_BLUE(*tp);
			int tw = (*tp)>>24 & 0xFF;

			if( w == 0 )
				continue;




			if( w == 255 ){
				*tp = *sp;
				continue;
			}



			float f = 1.0 / 255.0;
			int R = ((255-w)*tr + w*r )* f;
			int G = ((255-w)*tg + w*g )* f;
			int B = ((255-w)*tb + w*b )* f;
			int A = tw + w;
			if( A > 255 )	A = 255;

			*tp = ( A << 24 ) | IMAGE4_RGB( R, G, B ); 

		}
	}


}


image_type *
imageA_set_backgorund( image_type *sim, image_type *mim, image_type *bim, image_type *im )
{
	u_char	*tp;
	u_char	*sp,	*mp,	*bp;
	int	i,	j;
	int	w;




	im = image_realloc( im, sim->width, sim->height, 3, IMAGE_TYPE_U8, 1 );





	sp = sim->data;
	mp = mim->data;
	bp = bim->data;
	tp = im->data;
	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, mp++,sp += 3, bp += 3 ){

			if( *mp == 0 ){
				*tp++ = bp[0];
				*tp++ = bp[1];
				*tp++ = bp[2];
				continue;
			}


			if( *mp == 255 ){
				*tp++ = sp[0];
				*tp++ = sp[1];
				*tp++ = sp[2];
				continue;
			}


			w = *mp +1 ;


			*tp++ =  (( w * ( sp[0] - bp[0])) >> 8)  + bp[0];
			*tp++ =  (( w * ( sp[1] - bp[1])) >> 8)  + bp[1];
			*tp++ =  (( w * ( sp[2] - bp[2])) >> 8)  + bp[2];

		}
	}

	return( im );
}




