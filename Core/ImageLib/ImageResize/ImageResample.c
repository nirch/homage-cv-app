/**************************
 ***	ImageResample.c  ***
 *************************/
#include	<string.h>

#include	"ImageType/ImageType.h"


image_type *	image4_resample( image_type *sim, int drow, int dcol );

 

static image_type *image1_sample_up_col( image_type *sim, int dcol);
static image_type *image1_sample_up_row( image_type *sim, int drow);

static image_type *image4_sample_up_col( image_type *sim, int dcol);
static image_type *image4_sample_up_row( image_type *sim, int drow);

static image_type *image4_sample_up_col_BiLinear( image_type *sim, int dcol );
static image_type *image4_sample_up_row_BiLinear( image_type *sim, int drow );

///////////////////////////////// NEW ////////////////////////////////////
static image_type *image1_sample_down_col_new( image_type *sim, int dcol);
static image_type *image1_sample_down_row_new( image_type *sim, int drow);
static image_type *image1_sample_down_row_newA( image_type *sim, int drow);

static image_type *image4_sample_down_col_new( image_type *sim, int dcol);
static image_type *image4_sample_down_row_new( image_type *sim, int drow);
static image_type *image4_sample_down_row_newA( image_type *sim, int drow);


image_type *
image_resample( image_type *sim, int drow, int dcol )
{
image_type *im1,	*im2,	*im;
	


	if( sim->depth == 1 )
	{
		if ( dcol > sim->column )
			im1 = image1_sample_up_col(sim, dcol);
		else
			im1 = image1_sample_down_col_new(sim, dcol);
		if ( drow > sim->row )
			im2 = image1_sample_up_row(im1, drow);
		else
			im2 = image1_sample_down_row_new(im1, drow);

		image_destroy(im1,1);

		return( im2 );
	}


	if( sim->depth == 3 ){
		im1 = image4_from( sim, NULL );

		im2 = image4_resample( im1, drow, dcol );
		image_destroy( im1, 1 );


		im = image3_from( im2, NULL );
		image_destroy( im2, 1 );

		return( im );
	}


	if( sim->depth == 4 ){
		im = image4_resample( sim, drow, dcol );
		return( im );
	}

	return( NULL );
}


image_type *
image4_resample( image_type *sim, int drow, int dcol )
{
image_type	*im2,	*im1,	*tim;


	if ( sim->depth == 4 ){

		if( sim->row == drow && sim->column == dcol )
			return image4_copy(sim);


		if( drow <= sim->row && dcol <= sim->column){
			im2 = image_sample_down_transparent(sim, drow, dcol);
			return( im2 );
		}
	}

/*
	if ( sim->depth == 4 && sim->row == drow && sim->column == dcol)
	{
		return image4_copy(sim);
	}


	if( sim->depth == 4 && drow <= sim->row && dcol <= sim->column){
//		im2 = image_sample_down(sim, drow, dcol);
		im2 = image_sample_down_transparent(sim, drow, dcol);
		return( im2 );
	}
*/

	if( sim->depth == 1 )
	{
		if ( dcol > sim->column )
			im1 = image1_sample_up_col(sim, dcol);
		else
			im1 = image1_sample_down_col_new(sim, dcol);
		if ( drow > sim->row )
			im2 = image1_sample_up_row(im1, drow);
		else
			im2 = image1_sample_down_row_new(im1, drow);
				
		image_destroy(im1,1);

		return( im2 );
	}


	



	tim = image4_from( sim, NULL );

	if ( dcol > tim->column )
		im1 = image4_sample_up_col_BiLinear( tim, dcol);
	else
		im1 = image4_sample_down_col_new( tim, dcol);

	if ( drow > tim->row )
		im2 = image4_sample_up_row_BiLinear( im1,drow);
	else
		im2 = image4_sample_down_row_new( im1, drow);
	
	image_destroy(im1,1);

	if( tim != sim )
		image_destroy( tim, 1 );

	return( im2 );
}

/* 
image_type *
image_sampleic2( image_type *sim, int drow, int dcol )
{
image_type	*im2,	*im1=NULL;


	if ( sim->depth == 4 && sim->row == drow && sim->column == dcol)
	{
		return image4_copy(sim);
	}

	if( sim->depth == 1 )
	{
		if ( dcol > sim->column )
			im1 = image1_sample_up_col(sim, dcol);
		else
			im1 = image1_sample_down_col_new(sim, dcol);
		if ( drow > sim->row )
			im2 = image1_sample_up_row(im1, drow);
		else
			im2 = image1_sample_down_row_new(im1, drow);

		image_destroy(im1,1);
	}
	else
	{
		if ( dcol > sim->column )
		//	im1 = image4_sample_up_col(sim, dcol);
			im1 = image4_sample_up_col_BiLinear(sim,dcol);
		else
			im1 = image4_sample_down_col_new(sim, dcol);
		if ( drow > sim->row )
		//	im2 = image4_sample_up_row(im1, drow);
			im2 = image4_sample_up_row_BiLinear(im1,drow);
		else
			im2 = image4_sample_down_row_new(im1, drow);		

		image_destroy(im1,1);
	}

	return( im2 );
}


image_type *
image_sampleic2A( image_type *sim, int drow, int dcol )
{
image_type	*im2,	*im1=NULL;


	if ( sim->depth == 4 && sim->row == drow && sim->column == dcol)
	{
		return image4_copy(sim);
	}

	if( sim->depth == 1 )
	{
		if ( dcol > sim->column )
			im1 = image1_sample_up_col(sim, dcol);
		else
			im1 = image1_sample_down_col_new(sim, dcol);
		if ( drow > sim->row )
			im2 = image1_sample_up_row(im1, drow);
		else
			im2 = image1_sample_down_row_newA(im1, drow);

		image_destroy(im1,1);
	}
	else
	{
		if ( dcol > sim->column )
		//	im1 = image4_sample_up_col(sim, dcol);
			im1 = image4_sample_up_col_BiLinear(sim,dcol);
		else
			im1 = image4_sample_down_col_new(sim, dcol);
		if ( drow > sim->row )
		//	im2 = image4_sample_up_row(im1, drow);
			im2 = image4_sample_up_row_BiLinear(im1,drow);
		else
			im2 = image4_sample_down_row_newA(im1, drow);		

		image_destroy(im1,1);
	}

	return( im2 );
}
*/

// ***************
// *** image 1 ***
// ***************


static image_type *
image1_sample_up_col( image_type *sim, int dcol )
{
image_type	*im1;
u_char	*tp1,	*sp;
float	dt,	ratioc_div, sample_point = 0, start;
float	rgb;
int		i,	j, i_ratio, k;


	dt = sim->column / (float)dcol;
	i_ratio = (int)(dt * 10000);
	dt = (float)i_ratio / 10000.0;
	ratioc_div = 1 / dt;

	im1 = image_create( sim->row, dcol, 1, 1, NULL );
	
	sp = IMAGE_PIXEL( sim, 0, 0 );
	tp1 = IMAGE_PIXEL( im1, 0, 0 );

	for ( i = 0 ; i < im1->row ; i++ ) 
	{
		rgb = 0;
		sample_point = 1;
		sp = IMAGE_PIXEL( sim, i, 0 );
		tp1 = IMAGE_PIXEL( im1, i ,0 );
		k = 0;
		for ( j = 0 ; j < im1->column -1; j++ ) 
		{
			if ( sample_point < dt )
			{
 				start = dt - sample_point;									
				rgb = sample_point * (*sp);
				k ++;
				if ( k < sim->column )
					sp++;
				rgb += start * (*sp); 				
				rgb *= ratioc_div;
				*tp1++ = PUSH_TO_RANGE( (int)rgb, 0, 255 );
				sample_point = 1 - start;				
			}
			else 
			{

				*tp1++ = PUSH_TO_RANGE( (int)(*sp), 0, 255 );
				sample_point = sample_point - dt;
			}	
		}
		// last pixel - get it explicitly.
		sp = IMAGE_PIXEL( sim, i, sim->column - 1 );
		*tp1++ = PUSH_TO_RANGE( (int)(*sp), 0, 255 );
	}
	return im1;
}


static image_type *
image1_sample_up_row( image_type *sim, int drow )
{
image_type	*im1;
u_char	*tp1,	*sp;
float	dt,	ratior_div, sample_point = 0, start;
float	rgb;
int	i,	j, i_ratio, k ;


	dt = sim->row / (float)drow;
	i_ratio = (int)(dt * 10000);
	dt = (float)i_ratio / 10000.0;
	ratior_div = 1 / dt;

	im1 = image_create( drow, sim->column, 1, 1, NULL );

	for ( i = 0 ; i < im1->column ; i++ ) 
	{
		rgb = 0;
		sample_point = 1;
		sp = IMAGE_PIXEL( sim, 0, i );
		tp1 = IMAGE_PIXEL( im1, 0 ,i );
		k = 0;
		for ( j = 0 ; j < im1->row ; j++ ) 
		{
			if ( sample_point < dt )
			{
 				start = dt - sample_point;									
				rgb = sample_point * (*sp);
				k ++;
				if ( k < sim->row )
					sp += sim->column;
				rgb += start * (*sp); 				
				rgb *= ratior_div;
				*tp1 = PUSH_TO_RANGE( (int)rgb, 0, 255 );
				tp1 += im1->column;
				sample_point = 1 - start;				
			}
			else 
			{

				*tp1 = PUSH_TO_RANGE( (int)(*sp), 0, 255 );
				tp1 += im1->column;
				sample_point = sample_point - dt;
			}	
		}
		// last pixel - get it explicitly.
		sp = IMAGE_PIXEL( sim,  sim->row - 1 ,i);
		*tp1++ = PUSH_TO_RANGE( (int)(*sp), 0, 255 );
	}
	return im1;
}





/**** image1_sample_up_col_BiLinear ****/
/*!
 * The method scales up an image using BiLinear algorithm - each pixel in the new
 * image would be constructed from two original pixel using weighted average.
 * @param *sim : source image
 * @param dcol : destination column
 *
 * @return  image_type: new image with extended columns number
 */
static image_type *
image1_sample_up_col_BiLinear( image_type *sim, int dcol )
{
image_type	*im1;
u_char	*tp1,	*sp0, *sp1;
float	dt, half_dt;
float	remainder = 0;
float	sample_point,sample_ratio1, sample_ratio2; 
int rgb1, rgb2;
int	i,	j;
int i_ratio,k;


	dt = sim->column / (float)dcol;
	i_ratio = (int)(dt * 10000);
	dt = (float)i_ratio / 10000.0;
	half_dt = dt / 2.0;
	

	im1 = image_create( sim->row, dcol, 1, 1, NULL );
	
	for ( i = 0 ; i < im1->row ; i++ ) {
		sample_point = 0.5 + half_dt;// starts at half old pixel's position
		sp1 = sp0 = IMAGE_PIXEL( sim, i, 0 );
		//sp1 ++; 
		k = 0;
		tp1 = IMAGE_PIXEL( im1, i ,0 );
		for ( j = 0 ; j < im1->column -1 ; j++ ) 
		{
			
			sample_ratio1 = 1 - sample_point;
			sample_ratio2 = sample_point;
			// get proportional part from first pixel
			rgb1 = sample_ratio1 * (*sp0);
			// get proportional part from second pixel
			rgb2 = sample_ratio2 * (*sp1);
			// combine into RGB
			*tp1++ = PUSH_TO_RANGE( (int)(rgb1 + rgb2), 0, 255 );

			sample_point += dt;
			if ( sample_point > 1)			
			{
				k++;
				sample_point -= 1;				
				sp0 = sp1; 
				if ( k < sim->column)// so won't try to read pixel after last one
					sp1 ++;	
			}
		}
		sp0 = IMAGE_PIXEL(sim, i, sim->column - 1);
		rgb1 = (*sp0);		
		*tp1 = PUSH_TO_RANGE( (int)(rgb1), 0, 255 );		
	}
	return im1;
}




/**** image1_sample_up_row_BiLinear ****/
/*!
 * The method scales up an image using BiLinear algorithm - each pixel in the new
 * image would be constructed from two original pixel using weighted average.
 * @param *sim : source image
 * @param dcol : destination row
 *
 * @return  image_type: new image with extended rows number
 */
static image_type *
image1_sample_up_row_BiLinear( image_type *sim, int drow )
{
image_type	*im1;
u_char	*tp1,	*sp0, *sp1;
float	dt, half_dt;
float	sample_point = 0, sample_ratio1, sample_ratio2;
int		i,	j;
int	rgb1,	rgb2;
int i_ratio,k=0;


	dt = sim->row / (float)drow;
	i_ratio = (int)(dt * 10000);
	dt = (float)i_ratio / 10000.0;
	half_dt = dt / 2.0;
	
	im1 = image_create( drow, sim->column, 1, 1, NULL );
	

	for ( i = 0 ; i < im1->column ; i++ ) 
	{		
		sample_point = 0.5 + half_dt;// starts at half old pixel's position
		sp1 = sp0 = IMAGE_PIXEL( sim, 0, i );
		
		tp1 = IMAGE_PIXEL( im1, 0 ,i );
		k = 0;
		for ( j = 0 ; j < im1->row  - 1; j++ ) 
		{			
			sample_ratio1 = 1 - sample_point;
			sample_ratio2 = sample_point;
			// get proportional part from first pixel
			rgb1 = sample_ratio1 * (*sp0);
			// get proportional part from second pixel
			rgb2 = sample_ratio2 * (*sp1);			
			// combine into RGB
			*tp1  = PUSH_TO_RANGE( (int)(rgb1 + rgb2), 0, 255 );			
			tp1 += im1->column;		
			sample_point += dt;			
			if ( sample_point > 1)			
			{
				k++;
				sample_point -= 1;				
				sp0 = sp1; 
				if ( k < sim->row)// so won't try to read pixel after last one
					sp1 += sim->column;	
			}
		}
		sp0 = IMAGE_PIXEL(sim, sim->row - 1, i);
		rgb1 =(*sp0);
		*tp1 = PUSH_TO_RANGE( (int)(rgb1), 0, 255 );
	}
	return im1;
}



// ***************
// *** image 4 ***
// ***************




static image_type *
image4_sample_up_col( image_type *sim, int dcol )
{
image_type	*im1;
u_int	*tp1,	*sp;
float	dt,	ratioc_div;
float	sample_point = 0, start;
int	pixel,	i,	j;
float	r1,	g1,	b1,	tmp;
int	r,	g,	b;
int i_ratio, k = 0;
	dt = sim->column / (float)dcol;
	i_ratio = (int)(dt * 10000);
	dt = (float)i_ratio / 10000.0;
	ratioc_div = 1 / dt;

	im1 = image_create( sim->row, dcol, 4, 1, NULL );
	
	sp = IMAGE4_PIXEL( sim, 0, 0 );
	tp1 = IMAGE4_PIXEL( im1, 0, 0 );


	for ( i = 0 ; i < im1->row ; i++ ) 
	{
		r1 = g1 = b1 = 0;
		sample_point = 1;
		sp = IMAGE4_PIXEL( sim, i, 0 );
		tp1 = IMAGE4_PIXEL( im1, i ,0 );
		k = 0;
		for ( j = 0 ; j < im1->column -1; j++ ) 
		{
			if ( sample_point < dt )
			{
 
				start = dt - sample_point;
								
				r1 = sample_point * (IMAGE4_RED(*sp));
				g1 = sample_point * (IMAGE4_GREEN(*sp));
				b1 = sample_point * (IMAGE4_BLUE(*sp));
				k ++;
				if ( k < sim->column )
					sp++;

				tmp = start * (IMAGE4_RED(*sp));
				r1 += tmp;
				tmp = start * (IMAGE4_GREEN(*sp));
				g1 += tmp;
				tmp = start * (IMAGE4_BLUE(*sp));
				b1 += tmp;	

				r1 *= ratioc_div;//+ 0.5;
				g1 *= ratioc_div;//+ 0.5;
				b1 *= ratioc_div;//+ 0.5;
				r = PUSH_TO_RANGE( (int)r1, 0, 255 );
				g = PUSH_TO_RANGE( (int)g1, 0, 255 );
				b = PUSH_TO_RANGE( (int)b1, 0, 255 );
				pixel = IMAGE4_RGB( r, g, b );
				*tp1++ = pixel;
				sample_point = 1 - start;				
			}
			else 
			{
/*
				r1 = (IMAGE4_RED(*sp));
				g1 = (IMAGE4_GREEN(*sp));
				b1 = (IMAGE4_BLUE(*sp));
				r = PUSH_TO_RANGE( (int)r1, 0, 255 );
				g = PUSH_TO_RANGE( (int)g1, 0, 255 );
				b = PUSH_TO_RANGE( (int)b1, 0, 255 );
				pixel = IMAGE4_RGB( r, g, b );
				*tp1++ = pixel;
*/
				*tp1++ = *sp;
				sample_point = sample_point - dt;
			}	
		}
		// last pixel - get it explicitly.
/*
		sp = IMAGE4_PIXEL( sim, i, sim->column - 1 );
		r1 = (IMAGE4_RED(*sp));
		g1 = (IMAGE4_GREEN(*sp));
		b1 = (IMAGE4_BLUE(*sp));
		r = PUSH_TO_RANGE( (int)r1, 0, 255 );
		g = PUSH_TO_RANGE( (int)g1, 0, 255 );
		b = PUSH_TO_RANGE( (int)b1, 0, 255 );
		pixel = IMAGE4_RGB( r, g, b );
		*tp1++ = pixel;
*/
		*tp1++ = *sp;

	}
	return im1;
}


static image_type *
image4_sample_up_row( image_type *sim, int drow )
{
image_type	*im1;
u_int	*tp1,	*sp;
float	dt,	ratior_div;
float	sample_point = 0, start;
int	pixel,	i,	j;
float	r1,	g1,	b1,	tmp;
int	r,	g,	b;
int i_ratio, k=0;

	dt = sim->row / (float)drow;
	i_ratio = (int)(dt * 10000);
	dt = (float)i_ratio / 10000.0;
	ratior_div = 1 / dt;

	im1 = image_create( drow, sim->column, 4, 1, NULL );
	


	for ( i = 0 ; i < im1->column ; i++ ) 
	{
		r1 = g1 = b1 = 0;
		sample_point = 1;
		sp = IMAGE4_PIXEL( sim, 0, i );
		tp1 = IMAGE4_PIXEL( im1, 0, i );
		k = 0;
		for ( j = 0 ; j < im1->row -1 ; j++ )
		{
			if ( sample_point < dt )
			{
				start = dt - sample_point;
				
				r1 = sample_point * (IMAGE4_RED(*sp));
				g1 = sample_point * (IMAGE4_GREEN(*sp));
				b1 = sample_point * (IMAGE4_BLUE(*sp));
				k++;
				if ( k < sim->row )
					sp += sim->column;

				
				tmp = start * (IMAGE4_RED(*sp));
				r1 += tmp;
				tmp = start * (IMAGE4_GREEN(*sp));
				g1 += tmp;
				tmp = start * (IMAGE4_BLUE(*sp));
				b1 += tmp;	

				r1 *= ratior_div; //+ 0.5;
				g1 *= ratior_div; //+ 0.5;
				b1 *= ratior_div; //+ 0.5;
				r = PUSH_TO_RANGE( (int)r1, 0, 255 );
				g = PUSH_TO_RANGE( (int)g1, 0, 255 );
				b = PUSH_TO_RANGE( (int)b1, 0, 255 );
				pixel = IMAGE4_RGB( r, g, b );
				*tp1 = pixel;
				tp1 += im1->column;
				
				sample_point = 1 - start;			
			}
			else 
			{
/*
				r1 = (IMAGE4_RED(*sp));
				g1 = (IMAGE4_GREEN(*sp));
				b1 = (IMAGE4_BLUE(*sp));
				r = PUSH_TO_RANGE( (int)r1, 0, 255 );
				g = PUSH_TO_RANGE( (int)g1, 0, 255 );
				b = PUSH_TO_RANGE( (int)b1, 0, 255 );
				pixel = IMAGE4_RGB( r, g, b );
				*tp1 = pixel;
*/
				*tp1++ = *sp;
				tp1 += im1->column;
				sample_point = sample_point - dt;
			}	
		}	
		// last pixel - get it explicitly.
/*
		sp = IMAGE4_PIXEL( sim, sim->row - 1, i );
		r1 = (IMAGE4_RED(*sp));
		g1 = (IMAGE4_GREEN(*sp));
		b1 = (IMAGE4_BLUE(*sp));
		r = PUSH_TO_RANGE( (int)r1, 0, 255 );
		g = PUSH_TO_RANGE( (int)g1, 0, 255 );
		b = PUSH_TO_RANGE( (int)b1, 0, 255 );
		pixel = IMAGE4_RGB( r, g, b );
*/
		*tp1++ = *sp;
		*tp1 = pixel;		
	}
	return im1;
}




/**** image4_sample_up_col_BiLinear ****/
/*!
 * The method scales up an image using BiLinear algorithm - each pixel in the new
 * image would be constructed from two original pixel using weighted average.
 * @param *sim : source image
 * @param dcol : destination column
 *
 * @return  image_type: new image with extended columns number
 */
static image_type *
image4_sample_up_col_BiLinear( image_type *sim, int dcol )
{
image_type	*im1;
u_int	*tp1,	*sp0, *sp1;
float	dt, half_dt;
float	remainder = 0;
float	sample_point,sample_ratio1, sample_ratio2; 
float	r1,	g1,	b1, r2,	g2,	b2;
int	pixel,	i,	j;
int	r,	g,	b;
int i_ratio,k;


	dt = sim->column / (float)dcol;
	i_ratio = (int)(dt * 10000);
	dt = (float)i_ratio / 10000.0;
	half_dt = dt / 2.0;
	

	im1 = image_create( sim->row, dcol, 4, 1, NULL );
	
	for ( i = 0 ; i < im1->row ; i++ ) {
		sample_point = 0.5 + half_dt;// starts at half old pixel's position
		sp1 = sp0 = IMAGE4_PIXEL( sim, i, 0 );
		//sp1 ++; 
		k = 0;
		tp1 = IMAGE4_PIXEL( im1, i ,0 );
		for ( j = 0 ; j < im1->column -1 ; j++ ) 
		{
			
			sample_ratio1 = 1 - sample_point;
			sample_ratio2 = sample_point;
			// get proportional part from first pixel
			r1 = sample_ratio1 * (IMAGE4_RED(*sp0));
			g1 = sample_ratio1 * (IMAGE4_GREEN(*sp0));
			b1 = sample_ratio1 * (IMAGE4_BLUE(*sp0));
			// get proportional part from second pixel
			r2 = sample_ratio2 * (IMAGE4_RED(*sp1));
			g2 = sample_ratio2 * (IMAGE4_GREEN(*sp1));
			b2 = sample_ratio2 * (IMAGE4_BLUE(*sp1));			

			// combine into RGB
			r = PUSH_TO_RANGE( (int)(r1 + r2), 0, 255 );
			g = PUSH_TO_RANGE( (int)(g1 + g2) , 0, 255 );
			b = PUSH_TO_RANGE( (int)(b1 + b2), 0, 255 );				
			pixel = IMAGE4_RGB( r, g, b );

			*tp1++ = pixel;					
				
			sample_point += dt;
			if ( sample_point > 1)			
			{
				k++;
				sample_point -= 1;				
				sp0 = sp1; 
				if ( k < sim->column)// so won't try to read pixel after last one
					sp1 ++;	
			}
		}
		sp0 = IMAGE4_PIXEL(sim, i, sim->column - 1);
		r1 = (IMAGE4_RED(*sp0));
		g1 = (IMAGE4_GREEN(*sp0));
		b1 = (IMAGE4_BLUE(*sp0));
		r = PUSH_TO_RANGE( (int)(r1), 0, 255 );
		g = PUSH_TO_RANGE( (int)(g1) , 0, 255 );
		b = PUSH_TO_RANGE( (int)(b1), 0, 255 );				
		pixel = IMAGE4_RGB( r, g, b );
		*tp1 = pixel;
	}
	return im1;
}




/**** image4_sample_up_row_BiLinear ****/
/*!
 * The method scales up an image using BiLinear algorithm - each pixel in the new
 * image would be constructed from two original pixel using weighted average.
 * @param *sim : source image
 * @param dcol : destination row
 *
 * @return  image_type: new image with extended rows number
 */
static image_type *
image4_sample_up_row_BiLinear( image_type *sim, int drow )
{
image_type	*im1;
u_int	*tp1,	*sp0, *sp1;
float	dt, half_dt;
float	sample_point = 0, sample_ratio1, sample_ratio2;
int	pixel,	i,	j;
float	r1,	g1,	b1,	r2,	g2,	b2;
int	r,	g,	b;
int i_ratio,k=0;


	dt = sim->row / (float)drow;
	i_ratio = (int)(dt * 10000);
	dt = (float)i_ratio / 10000.0;
	half_dt = dt / 2.0;
	
	im1 = image_create( drow, sim->column, 4, 1, NULL );
	

	for ( i = 0 ; i < im1->column ; i++ ) 
	{		
		sample_point = 0.5 + half_dt;// starts at half old pixel's position
		sp1 = sp0 = IMAGE4_PIXEL( sim, 0, i );
		//sp1 = IMAGE4_PIXEL( sim, 1, i );// next row pixel in this column
		tp1 = IMAGE4_PIXEL( im1, 0 ,i );
		k = 0;
		for ( j = 0 ; j < im1->row  - 1; j++ ) 
		{			
			sample_ratio1 = 1 - sample_point;
			sample_ratio2 = sample_point;
			// get proportional part from first pixel
			r1 = sample_ratio1 * (IMAGE4_RED(*sp0));
			g1 = sample_ratio1 * (IMAGE4_GREEN(*sp0));
			b1 = sample_ratio1 * (IMAGE4_BLUE(*sp0));
			// get proportional part from second pixel
			r2 = sample_ratio2 * (IMAGE4_RED(*sp1));
			g2 = sample_ratio2 * (IMAGE4_GREEN(*sp1));
			b2 = sample_ratio2 * (IMAGE4_BLUE(*sp1));

			// combine into RGB
			r = PUSH_TO_RANGE( (int)(r1 + r2), 0, 255 );
			g = PUSH_TO_RANGE( (int)(g1 + g2) , 0, 255 );
			b = PUSH_TO_RANGE( (int)(b1 + b2), 0, 255 );				
			pixel = IMAGE4_RGB( r, g, b );

			*tp1 = pixel;
			tp1 += im1->column;
		
			sample_point += dt;			
			if ( sample_point > 1)			
			{
				k++;
				sample_point -= 1;				
				sp0 = sp1; 
				if ( k < sim->row)// so won't try to read pixel after last one
					sp1 += sim->column;	
			}
		}		
		sp0 = IMAGE4_PIXEL(sim, sim->row - 1, i);
		r1 = (IMAGE4_RED(*sp0));
		g1 = (IMAGE4_GREEN(*sp0));
		b1 = (IMAGE4_BLUE(*sp0));
		r = PUSH_TO_RANGE( (int)(r1), 0, 255 );
		g = PUSH_TO_RANGE( (int)(g1) , 0, 255 );
		b = PUSH_TO_RANGE( (int)(b1), 0, 255 );				
		pixel = IMAGE4_RGB( r, g, b );
		*tp1 = pixel;
	}
	return im1;
}


//////////////////////////////// NEW CODE ///////////////////////////////////
////////////////////////////// SAMPLE DOWN //////////////////////////////////

// ***************
// *** image 1 ***
// ***************


static image_type *
image1_sample_down_col_new( image_type *sim, int dcol )
{
image_type	*im;
u_char	*tp,	*sp;
float	dt,	dt_div;
int	Cweight;		// weight of col
int	CweightLeft;
int	CdtF,	CdtF_div,	Csum;
int	i,	j,	k,	pixel;

	dt = sim->column / (float)dcol;
	CdtF = (int)(dt * 256);
	dt_div = 1 / dt;
	CdtF_div = (int)(dt_div * 256);

	im = image_create( sim->row, dcol, 1, 1, NULL );
	
//	sp = IMAGE_PIXEL( sim, 0, 0 );
//	tp = IMAGE_PIXEL( im, 0, 0 );

	// sample columns
	for ( i = 0 ; i < sim->row ; i++ ){
		Cweight = CdtF;
		Csum = 0;

		sp = IMAGE_PIXEL( sim, i, 0 );
		tp = IMAGE_PIXEL( im, i, 0 );

		for ( j = 0, k = 0 ; j < sim->column && k < im->column ; j++ ) {
			if ( Cweight > 256 ) {
				Csum += *sp++ << 8;
				Cweight -= 256;
				continue;
			}

			Csum += Cweight * (*sp);	
			pixel = (Csum *  CdtF_div) >> 16 ;
			*tp++ = PUSH_TO_RANGE( pixel, 0, 255 );
			k++;

			CweightLeft = 256 - Cweight;
			Csum = CweightLeft * (*sp++);
			Cweight = CdtF - CweightLeft;
	
		}
	}
	return im;
}

static image_type *
image1_sample_down_row_new( image_type *sim, int drow)
{
image_type		*im;
u_char		*tp,	*sp;
float	dt,	dt_div;
int	Rweight,	RweightLeft;
int	RdtF,	RdtF_div;
int	i,	j,	pixel;


	dt = sim->row / (float)drow;
	RdtF = (int)(dt * 256);
	dt_div = 1 / dt;
	RdtF_div = (int)(dt_div * 256);
	

	im = image_create( drow, sim->column, 1, 1, NULL );


	//sample rows
	for ( i = 0 ; i < sim->column ; i++ ) 
	{
		sp = IMAGE_PIXEL( sim, 0, i );
		tp = IMAGE_PIXEL( im, 0, i );
		Rweight = RdtF;
		pixel = 0;
		for ( j = 0 ; j < sim->row ; j++ ) 
		{
			if ( Rweight > 256 ) 
			{
				pixel += *sp << 8;	
				sp += sim->column;
				Rweight -= 256;
			}
			else 
			{
				pixel += Rweight * (*sp);
				pixel = ( pixel * RdtF_div ) >> 16;
				*tp = PUSH_TO_RANGE( pixel, 0, 255 );
				tp += im->column;
				RweightLeft = 256 - Rweight;
				pixel = RweightLeft * (*sp);
				sp += sim->column;
				Rweight = RdtF - RweightLeft;
			}	
		}
	}	
	return ( im );
}


// a new version going over the matrix in an opposite way
static image_type *
image1_sample_down_row_newA( image_type *sim, int drow)
{
image_type		*im;
u_char		*tp,	*sp;
float	dt,	dt_div;
int	RweightLeft;
int	RdtF,	RdtF_div;
int		*arSum,	*arWeight;
int	i,	j,	pixel;


	dt = sim->row / (float)drow;
	RdtF = (int)(dt * 256);
	dt_div = 1 / dt;
	RdtF_div = (int)(dt_div * 256);
	
	arSum = (int *)malloc( sim->column*sizeof(int) );
	memset( arSum, 0, sim->column*sizeof(int) );
	arWeight = (int *)malloc( sim->column*sizeof(int) );
	for( i = 0 ; i < sim->column ; i++ )
		arWeight[i] = RdtF;

	im = image_create( drow, sim->column, 1, 1, NULL );

	sp = IMAGE_PIXEL( sim, 0, 0 );
	tp = IMAGE_PIXEL( im, 0, 0 );

	for ( i = 0 ; i < sim->row; i++ )
	{
		for ( j = 0 ; j < sim->column; j++ )
		{
			if ( arWeight[j] > 256 ) 
			{
				arSum[j] += *sp << 8;	
				sp++;
				arWeight[j] -= 256;
			}
			else 
			{
				arSum[j] += arWeight[j] * (*sp);
				pixel = (arSum[j] * RdtF_div) >> 16;
				*tp++ = PUSH_TO_RANGE( pixel, 0, 255 );
				RweightLeft = 256 - arWeight[j];
				arSum[j] = RweightLeft * (*sp);
				sp++;
				arWeight[j] = RdtF - RweightLeft;
			}	
		}
	}

	free( arSum );
	free( arWeight );

	return ( im );
}



// ***************
// *** image 4 ***
// ***************


image_type *
image4_sample_down_col_new( image_type *sim, int dcol )
{
image_type	*im;
u_int	*tp,	*sp;
float	dt,	dt_div;
int	CdtF,	CdtF_div;
int	Cweight;		// weight of col 
int	CweightLeft;
int	i,	j;
int	r_sum,	g_sum,	b_sum;
int	r,	g,	b,	r1,	g1,	b1;

	dt = sim->column / (float)dcol;
	CdtF = (int)(dt * 256);
	dt_div = 1 / dt;
	CdtF_div = (int)(dt_div * 256);


	im = image_create( sim->row, dcol, 4, 1, NULL );
	
	sp = IMAGE4_PIXEL( sim, 0, 0 );
	tp = IMAGE4_PIXEL( im, 0, 0 );

	for ( i = 0 ; i < sim->row ; i++ ) 
	{
		Cweight = CdtF;
		r_sum = g_sum = b_sum = 0;
		for ( j = 0 ; j < sim->column ; j++ ) 
		{
			if ( Cweight > 256 )
			{

				r = IMAGE4_RED(*sp);
				r_sum += r << 8;
				
				g = IMAGE4_GREEN(*sp);
				g_sum += g << 8;

				b = IMAGE4_BLUE(*sp);
				b_sum += b << 8;

				sp++;
				Cweight -= 256;
			}
			else 
			{

				r = IMAGE4_RED(*sp);
				r_sum += Cweight * r;
				
				g = IMAGE4_GREEN(*sp);
				g_sum += Cweight * g;

				b = IMAGE4_BLUE(*sp);
				b_sum += Cweight * b;
							
				r_sum = (r_sum * CdtF_div) >> 16;
				g_sum = (g_sum * CdtF_div) >> 16;
				b_sum = (b_sum * CdtF_div) >> 16;

				r1 = PUSH_TO_RANGE( r_sum, 0, 255 );
				g1 = PUSH_TO_RANGE( g_sum, 0, 255 );
				b1 = PUSH_TO_RANGE( b_sum, 0, 255 );

				*tp++ = IMAGE4_RGB( r_sum, g_sum, b_sum );;
				
				CweightLeft = 256 - Cweight;

				r_sum = CweightLeft * r;
				
				g_sum = CweightLeft * g;

				b_sum = CweightLeft * b;
				
				sp++;
				Cweight = CdtF - CweightLeft;
			}	
		}
	}
	return im;
}

static image_type *
image4_sample_down_row_new( image_type *sim, int drow)
{
image_type	*im;
u_int	*tp,	*sp;
float	dt,	dt_div;
int	RdtF,	RdtF_div;
int	Rweight,	RweightLeft;
int	i,	j;
int	r,	g,	b;
int	r1,	g1,	b1,	r2,	g2,	b2;

	dt = sim->row / (float)drow;
	RdtF = (int)(dt * 256);
	dt_div = 1 / dt;
	RdtF_div = (int)(dt_div * 256);
	
	im = image_create( drow, sim->column, 4, 1, NULL );
	
	for ( i = 0 ; i < sim->column ; i++ ) 
	{
		sp = IMAGE4_PIXEL( sim, 0, i );
		tp = IMAGE4_PIXEL( im, 0, i );
		Rweight = RdtF;
		r1 = g1 = b1 = 0;
		for ( j = 0 ; j < sim->row ; j++ ) 
		{
			if ( Rweight > 256 ) 
			{
				r = IMAGE4_RED(*sp);
				r1 += r << 8;
				
				g = IMAGE4_GREEN(*sp);
				g1 += g << 8;

				b = IMAGE4_BLUE(*sp);
				b1 += b << 8;

				sp += sim->column;
				Rweight -= 256;
			}
			else 
			{
				r = IMAGE4_RED(*sp);
				r1 += Rweight * r;
				
				g = IMAGE4_GREEN(*sp);
				g1 += Rweight * g;

				b = IMAGE4_BLUE(*sp);
				b1 += Rweight * b;

				r1 = ( r1 * RdtF_div ) >> 16;
				g1 = ( g1 * RdtF_div ) >> 16;
				b1 = ( b1 * RdtF_div ) >> 16;
				r2 = PUSH_TO_RANGE( r1, 0, 255 );
				g2 = PUSH_TO_RANGE( g1, 0, 255 );
				b2 = PUSH_TO_RANGE( b1, 0, 255 );
				*tp = IMAGE4_RGB( r2, g2, b2 );
				tp += im->column;
				
				RweightLeft = 256 - Rweight;

				r1 = RweightLeft * r;
				
				g1 = RweightLeft * g;

				b1 = RweightLeft * b;

				sp += sim->column;
				Rweight = RdtF - RweightLeft;
			}
		}
	}
	return ( im );
}




// a new version going over the matrix in an opposite way
static image_type *
image4_sample_down_row_newA( image_type *sim, int drow)
{
image_type	*im;
u_int	*tp,	*sp;
float	dt,	dt_div;
int	RdtF,	RdtF_div;
int	RweightLeft;
int	i,	j;
int	*arRsum,	*arGsum,	*arBsum,	*arWeight;
int	r,	g,	b;
int	r1,	g1,	b1,	r2,	g2,	b2;

	dt = sim->row / (float)drow;
	RdtF = (int)(dt * 256);
	dt_div = 1 / dt;
	RdtF_div = (int)(dt_div * 256);

	arRsum = (int *)malloc( sim->column*sizeof(int) );
	memset( arRsum, 0, sim->column*sizeof(int) );
	arGsum = (int *)malloc( sim->column*sizeof(int) );
	memset( arGsum, 0, sim->column*sizeof(int) );
	arBsum = (int *)malloc( sim->column*sizeof(int) );
	memset( arBsum, 0, sim->column*sizeof(int) );
	arWeight = (int *)malloc( sim->column*sizeof(int) );
	for( i = 0 ; i < sim->column ; i++ )
		arWeight[i] = RdtF;
	
	im = image_create( drow, sim->column, 4, 1, NULL );

	sp = IMAGE4_PIXEL( sim, 0, 0 );
	tp = IMAGE4_PIXEL( im, 0, 0 );

	for ( i = 0 ; i < sim->row; i++ )
	{
		for ( j = 0 ; j < sim->column; j++ )
		{
			if ( arWeight[j] > 256 ) 
			{
				r = IMAGE4_RED(*sp);
				arRsum[j] += r << 8;
				
				g = IMAGE4_GREEN(*sp);
				arGsum[j] += g << 8;

				b = IMAGE4_BLUE(*sp);
				arBsum[j] += b << 8;

				sp++;
				arWeight[j] -= 256;
			}
			else 
			{
				r = IMAGE4_RED(*sp);
				arRsum[j] += arWeight[j] * r;
				
				g = IMAGE4_GREEN(*sp);
				arGsum[j] += arWeight[j] * g;

				b = IMAGE4_BLUE(*sp);
				arBsum[j] += arWeight[j] * b;

				r1 = (arRsum[j] * RdtF_div) >> 16;
				g1 = (arGsum[j] * RdtF_div) >> 16;
				b1 = (arBsum[j] * RdtF_div) >> 16;

				r2 = PUSH_TO_RANGE( r1, 0, 255 );
				g2 = PUSH_TO_RANGE( g1, 0, 255 );
				b2 = PUSH_TO_RANGE( b1, 0, 255 );

				*tp++ = IMAGE4_RGB( r1, g1, b1 );
				
				RweightLeft = 256 - arWeight[j];

				arRsum[j] = RweightLeft * r;
				
				arGsum[j] = RweightLeft * g;

				arBsum[j] = RweightLeft * b;

				sp++;
				arWeight[j] = RdtF - RweightLeft;
			}	
		}
	}

	free( arRsum );
	free( arBsum );
	free( arGsum );
	free( arWeight );

	return ( im );
}

