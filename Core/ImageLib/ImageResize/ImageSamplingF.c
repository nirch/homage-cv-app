/**************************
 ***	ImageSamplingF.c  ***
 *************************/

#include	"ImageType/ImageType.h"
 

static image_type *image1_sample_up_col( image_type *sim, int dcol);
static image_type *image1_sample_down_col( image_type *sim, int dcol);
static image_type *image1_sample_up_row( image_type *sim, int drow);
static image_type *image1_sample_down_row( image_type *sim, int drow);

static image_type *image4_sample_up_col( image_type *sim, int dcol);
static image_type *image4_sample_down_col( image_type *sim, int dcol);
static image_type *image4_sample_up_row( image_type *sim, int drow);
static image_type *image4_sample_down_row( image_type *sim, int drow);


static image_type *image4_sample_up_col_BiLinear( image_type *sim, int dcol );
static image_type *image4_sample_up_row_BiLinear( image_type *sim, int drow );



image_type *
image_samplefc( image_type *sim, int drow, int dcol )
{
image_type	*im1, *im2;
	

	if ( sim->depth == 4 && sim->row == drow && sim->column == dcol)
	{
		return image4_copy(sim);
	}


	if( sim->depth == 1 )
	{
		if ( drow > sim->row )
			im1 = image1_sample_up_row(sim,drow);
		else
			im1 = image1_sample_down_row(sim,drow);
		if ( dcol > sim->column )
			im2 = image1_sample_up_col(im1,dcol);
		else
			im2 = image1_sample_down_col(im1,dcol);
		image_destroy(im1,1);
	}
	else
	{		
		if ( drow > sim->row )
			//im1 = image4_sample_up_row(sim,drow);
			im1 = image4_sample_up_row_BiLinear(sim,drow);

		else
			im1 = image4_sample_down_row(sim,drow);
		if ( dcol > sim->column )
			//im2 = image4_sample_up_col(im1,dcol);
			im2 = image4_sample_up_col_BiLinear(im1,dcol);
		else
			im2 = image4_sample_down_col(im1,dcol);


		image_destroy(im1,1);

	}

	return( im2 );
}


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







static image_type *
image1_sample_down_col( image_type *sim, int dcol )
{
image_type	*im1;
u_char	*tp1,	*sp;
float	dt,	ratioc_div;
float	w_col;		// weights of row and col
float	pixel,	sample_point = 0;
int	i,	j,	k;


	dt = sim->column / (float)dcol;
	ratioc_div = 1 / dt;

	im1 = image_create( sim->row, dcol, 1, 1, NULL );
	
	sp = IMAGE_PIXEL( sim, 0, 0 );
	tp1 = IMAGE_PIXEL( im1, 0, 0 );

	// sample columns
	for ( i = 0 ; i < sim->row ; i++ ) 
	{
		w_col = dt;
		pixel = 0;
		k = 0;
		for ( j = 0 ; j < sim->column ; j++ ) 
		{
			if ( w_col > 1 ) {
				pixel += *sp++;
				w_col--;
			}
			else 
			{
				pixel += w_col * (*sp);	
				pixel = pixel * ratioc_div + 0.5;
				*tp1++ = PUSH_TO_RANGE( (int)pixel, 0, 255 );
				k++;
				sample_point = 1 - w_col;
				w_col = dt;
				pixel = 0;
				pixel += sample_point * (*sp++);
				w_col -= sample_point;
			}	
		}
		if ( ( w_col < 0.5 ) && ( k < im1->column ) )
		{
			pixel = pixel * ratioc_div + 0.5;
			*tp1++ = PUSH_TO_RANGE( (int)pixel, 0, 255 );
		}

	}
	return im1;
}




static image_type *
image1_sample_down_row( image_type *sim, int drow)
{
image_type		*im2;
u_char		*tp2,	*sp;
float	dt,	ratior_div;
float	w_row;		// weights of row and col
float	pixel,	sample_point = 0;
int	i,	j,	k;


	dt = sim->row / (float)drow;
	ratior_div = 1 / dt;
	

	im2 = image_create( drow, sim->column, 1, 1, NULL );


	//sample rows
	for ( i = 0 ; i < sim->column ; i++ ) 
	{
		sp = IMAGE_PIXEL( sim, 0, i );
		tp2 = IMAGE_PIXEL( im2, 0, i );
		w_row = dt;
		pixel = 0;
		k = 0;
		for ( j = 0 ; j < sim->row ; j++ ) 
		{
			if ( w_row > 1 ) 
			{
				pixel += *sp;	
				sp += sim->column;
				w_row--;
			}
			else 
			{
				pixel += w_row * (*sp);
				pixel = pixel * ratior_div + 0.5;
				*tp2 = PUSH_TO_RANGE( (int)pixel, 0, 255 );
				tp2 += im2->column;
				k++;
				sample_point = 1 - w_row;
				w_row = dt;
				pixel = 0;
				pixel += sample_point * (*sp);
				sp += sim->column;
				w_row -= sample_point;
			}	
		}
		if ( ( w_row < 0.5 ) && ( k < im2->row ) ) 
		{
			pixel = pixel * ratior_div + 0.5;
			*tp2 = PUSH_TO_RANGE( (int)pixel, 0, 255 );
			tp2 += im2->column;
		}
	}	
	return ( im2 );
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



static image_type *
image4_sample_down_col( image_type *sim, int dcol )
{
image_type	*im1;
u_int	*tp1,	*sp;
float	dt,	ratioc_div;
float	w_col;		// weights or row and col 
float	sample_point = 0;
int	pixel,	i,	j,	k;
float	r1,	g1,	b1;
int	r,	g,	b,	tmp;


	dt = sim->column / (float)dcol;
	ratioc_div = 1 / dt;

	im1 = image_create( sim->row, dcol, 4, 1, NULL );
	
	sp = IMAGE4_PIXEL( sim, 0, 0 );
	tp1 = IMAGE4_PIXEL( im1, 0, 0 );

	for ( i = 0 ; i < sim->row ; i++ ) 
	{
		w_col = dt;
		r1 = g1 = b1 = 0;
		k = 0;
		for ( j = 0 ; j < sim->column ; j++ ) 
		{
			if ( w_col > 1 )
			{

				tmp = IMAGE4_RED(*sp);
				r1 += tmp;
				
				tmp = IMAGE4_GREEN(*sp);
				g1 += tmp;

				tmp = IMAGE4_BLUE(*sp);
				b1 += tmp;

				sp++;
				w_col--;
			}
			else 
			{

				tmp = IMAGE4_RED(*sp);
				r1 += w_col * tmp;
				
				tmp = IMAGE4_GREEN(*sp);
				g1 += w_col * tmp;

				tmp = IMAGE4_BLUE(*sp);
				b1 += w_col * tmp;
							
				r1 = r1 * ratioc_div + 0.5;
				g1 = g1 * ratioc_div + 0.5;
				b1 = b1 * ratioc_div + 0.5;
				r = PUSH_TO_RANGE( (int)r1, 0, 255 );
				g = PUSH_TO_RANGE( (int)g1, 0, 255 );
				b = PUSH_TO_RANGE( (int)b1, 0, 255 );
				pixel = IMAGE4_RGB( r, g, b );
				*tp1++ = pixel;
				k++;
				
				sample_point = 1 - w_col;
				w_col = dt;
				r1 = g1 = b1 = 0;

				tmp = IMAGE4_RED(*sp);
				r1 += sample_point * tmp;
				
				tmp = IMAGE4_GREEN(*sp);
				g1 += sample_point * tmp;

				tmp = IMAGE4_BLUE(*sp);
				b1 += sample_point * tmp;
				
				sp++;
				w_col -= sample_point;
			}	
		}
		if ( ( w_col < 0.5 ) && ( k < im1->column ) )
		{
						
			r1 = r1 * ratioc_div + 0.5;
			g1 = g1 * ratioc_div + 0.5;
			b1 = b1 * ratioc_div + 0.5;
			r = PUSH_TO_RANGE( (int)r1, 0, 255 );
			g = PUSH_TO_RANGE( (int)g1, 0, 255 );
			b = PUSH_TO_RANGE( (int)b1, 0, 255 );
			pixel = IMAGE4_RGB( r, g, b );
			*tp1++ = pixel;
		}
	}
	return im1;
}






static image_type *
image4_sample_down_row( image_type *sim, int drow)
{
image_type	*im2;
u_int	*tp1,	*tp2;
float	dt,	ratior_div;
float	w_row;		// weights or row and col 
float	sample_point = 0;
int	pixel,	i,	j,	k;
float	r1,	g1,	b1;
int	r,	g,	b,	tmp;


	dt = sim->row / (float)drow;
	ratior_div = 1 / dt;
	
	im2 = image_create( drow, sim->column, 4, 1, NULL );
	
	for ( i = 0 ; i < sim->column ; i++ ) 
	{
		tp1 = IMAGE4_PIXEL( sim, 0, i );
		tp2 = IMAGE4_PIXEL( im2, 0, i );
		w_row = dt;
		r1 = g1 = b1 = 0;
		k = 0;
		for ( j = 0 ; j < sim->row ; j++ ) 
		{
			if ( w_row > 1 ) 
			{
				tmp = IMAGE4_RED(*tp1);
				r1 += tmp;
				
				tmp = IMAGE4_GREEN(*tp1);
				g1 += tmp;

				tmp = IMAGE4_BLUE(*tp1);
				b1 += tmp;

				tp1 += sim->column;
				w_row--;
			}
			else 
			{
				tmp = IMAGE4_RED(*tp1);
				r1 += w_row * tmp;
				
				tmp = IMAGE4_GREEN(*tp1);
				g1 += w_row * tmp;

				tmp = IMAGE4_BLUE(*tp1);
				b1 += w_row * tmp;

				r1 = r1 * ratior_div + 0.5;
				g1 = g1 * ratior_div + 0.5;
				b1 = b1 * ratior_div + 0.5;
				r = PUSH_TO_RANGE( (int)r1, 0, 255 );
				g = PUSH_TO_RANGE( (int)g1, 0, 255 );
				b = PUSH_TO_RANGE( (int)b1, 0, 255 );
				pixel = IMAGE4_RGB( r, g, b );
				*tp2 = pixel;
				tp2 += im2->column;
				k++;
				
				sample_point = 1 - w_row;
				w_row = dt;
				r1 = g1 = b1 = 0;

				tmp = IMAGE4_RED(*tp1);
				r1 += sample_point * tmp;
				
				tmp = IMAGE4_GREEN(*tp1);
				g1 += sample_point * tmp;

				tmp = IMAGE4_BLUE(*tp1);
				b1 += sample_point * tmp;

				tp1 += sim->column;
				w_row -= sample_point;
			}
		}
		if ( ( w_row < 0.5 ) && ( k < im2->row ) ) 
		{
		
			r1 = r1 * ratior_div + 0.5;
			g1 = g1 * ratior_div + 0.5;
			b1 = b1 * ratior_div + 0.5;
			r = PUSH_TO_RANGE( (int)r1, 0, 255 );
			g = PUSH_TO_RANGE( (int)g1, 0, 255 );
			b = PUSH_TO_RANGE( (int)b1, 0, 255 );
			pixel = IMAGE4_RGB( r, g, b );
			*tp2 = pixel;
			tp2 += im2->column;
		}
	}
	return ( im2 );
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





