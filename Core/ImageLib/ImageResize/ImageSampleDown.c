/**************************
 ***	ImageSampleDown.c  ***
 *************************/
#include	<string.h>
#include	"ImageType/ImageType.h"
 

static image_type *image1_sample_down_new( image_type *sim, int drow, int dcol );
static image_type *image4_sample_down_new( image_type *sim, int drow, int dcol);
static image_type *image4_sample_down_new_1Arr( image_type *sim, int drow, int dcol);


image_type *
image_sample_down( image_type *sim, int drow, int dcol )
{
image_type	*im;


	if ( sim->depth == 1 )
		im = image1_sample_down_new(sim, drow, dcol);
	else
		im = image4_sample_down_new(sim, drow, dcol);


	return( im );
}


// ***************
// *** image 1 ***
// ***************

// this function is with one array
static image_type *
image1_sample_down_new( image_type *sim, int drow, int dcol )
{
image_type	*im;
u_char	*tp,	*sp;
float	dt,	dt_div;
int	Cweight;		// weight of col
int	CweightLeft,	RweightLeft;
int	Csum;
int	CdtF,	CdtF_div;
int	RdtF,	RdtF_div;
int		*arSum,	*tmpArr;
int	i,	j,	k,	pixel;

	dt = sim->column / (float)dcol;
	CdtF = (int)(dt * 256);
	dt_div = 1 / dt;
	CdtF_div = (int)(dt_div * 256);

	dt = sim->row / (float)drow;
	RdtF = (int)(dt * 256);
	dt_div = 1 / dt;
	RdtF_div = (int)(dt_div * 256);
	
	arSum = (int *)malloc(dcol*2*sizeof(int) );
	memset( arSum, 0, dcol*2*sizeof(int) );

	for( i = 0 ; i < dcol*2 ; i++ )
		if( i % 2 == 0 )
			arSum[i] = RdtF;

	im = image_create( drow, dcol, 1, 1, NULL );
	
	sp = IMAGE_PIXEL( sim, 0, 0 );
	tp = IMAGE_PIXEL( im, 0, 0 );

	// sample columns
	for ( i = 0 ; i < sim->row ; i++ ) 
	{
		Cweight = CdtF;
		Csum = 0;
		k = 0;
		tmpArr = arSum;
		for ( j = 0 ; j < sim->column ; j++ ) 
		{
			if ( Cweight > 256 ) {
				Csum += *sp++ << 8;
				Cweight -= 256;
			}
			else 
			{
				Csum += Cweight * (*sp);	
				Csum = (Csum *  CdtF_div) >> 8 ;

		/////////////////////////////////////////////////////
				// A new pixel has been sampled from going over columns
				// and now it will be re-sampled from going over rows
				if ( *tmpArr > 256 ) 
				{
					*(tmpArr+1) += Csum;	
					*tmpArr -= 256;
				}
				else 
				{
					*(tmpArr+1) += ( (*tmpArr) * (Csum) ) >> 8 ;

					// A new pixel has been sampled totall
					pixel = ( *(tmpArr+1) * RdtF_div ) >> 16;
					*tp++ = PUSH_TO_RANGE( pixel, 0, 255 );
					RweightLeft = 256 - (*tmpArr);
					*(tmpArr+1) = ( RweightLeft * (Csum) ) >> 8;
					*tmpArr = RdtF - RweightLeft;
				}	
		///////////////////////////////////////////////////		
				k++;
				tmpArr += 2;

				CweightLeft = 256 - Cweight;
				Csum = CweightLeft * (*sp++);
				Cweight = CdtF - CweightLeft;
			}	
		}
	}

	free( arSum );

	return im;
}


/*
// this function is with 2 arrays
static image_type *
image1_sample_down_new( image_type *sim, int drow, int dcol )
{
image_type	*im;
u_char	*tp,	*sp;
float	dt,	dt_div;
int	Cweight;		// weight of col
int	CweightLeft,	RweightLeft;
int	Csum;
int	CdtF,	CdtF_div;
int	RdtF,	RdtF_div;
int		*arSum,	*arWeight;
int	i,	j,	k,	pixel;

	dt = sim->column / (float)dcol;
	CdtF = (int)(dt * 256);
	dt_div = 1 / dt;
	CdtF_div = (int)(dt_div * 256);

	dt = sim->row / (float)drow;
	RdtF = (int)(dt * 256);
	dt_div = 1 / dt;
	RdtF_div = (int)(dt_div * 256);
	
	arSum = (int *)malloc( sim->column*sizeof(int) );
	memset( arSum, 0, sim->column*sizeof(int) );
	arWeight = (int *)malloc( sim->column*sizeof(int) );
	for( i = 0 ; i < sim->column ; i++ )
		arWeight[i] = RdtF;

	im = image_create( drow, dcol, 1, 1, NULL );
	
	sp = IMAGE_PIXEL( sim, 0, 0 );
	tp = IMAGE_PIXEL( im, 0, 0 );

	// sample columns
	for ( i = 0 ; i < sim->row ; i++ ) 
	{
		Cweight = CdtF;
		Csum = 0;
		k = 0;
		for ( j = 0 ; j < sim->column ; j++ ) 
		{
			if ( Cweight > 256 ) {
				Csum += *sp++ << 8;
				Cweight -= 256;
			}
			else 
			{
				Csum += Cweight * (*sp);	
				Csum = (Csum *  CdtF_div) >> 8 ;

		/////////////////////////////////////////////////////
				// A new pixel has been sampled from going over columns
				// and now it will be re-sampled from going over rows
				if ( arWeight[k] > 256 ) 
				{
					arSum[j] += Csum;	
					arWeight[k] -= 256;
				}
				else 
				{
					arSum[k] += ( arWeight[k] * (Csum) ) >> 8 ;

					// A new pixel has been sampled totall
					pixel = (arSum[k] * RdtF_div) >> 16;
					*tp++ = PUSH_TO_RANGE( pixel, 0, 255 );
					RweightLeft = 256 - arWeight[k];
					arSum[k] = ( RweightLeft * (Csum) ) >> 8;
					arWeight[k] = RdtF - RweightLeft;
				}	
		///////////////////////////////////////////////////		
				k++;
				
				CweightLeft = 256 - Cweight;
				Csum = CweightLeft * (*sp++);
				Cweight = CdtF - CweightLeft;
			}	
		}
	}

	free( arSum );
	free( arWeight );

	return im;
}
*/



// ***************
// *** image 4 ***
// ***************


// this function is with one array
image_type *
image4_sample_down_new( image_type *sim, int drow, int dcol )
{
image_type	*im;
u_int	*tp,	*sp,	*tp_end;
float	dt,	dt_div;
int	CdtF,	CdtF_div;
int	RdtF,	RdtF_div;
int	Cweight;		// weight of col 
int	CweightLeft, RweightLeft;
int	i,	j,	k;
int	r_sum,	g_sum,	b_sum;
int	*arSum,	*tmpArr;
int	r,	g,	b;
int	r1,	g1,	b1,	r2,	g2,	b2;

	dt = sim->column / (float)dcol;
	CdtF = (int)(dt * 256);
	dt_div = 1 / dt;
	CdtF_div = (int)(dt_div * 256);

	dt = sim->row / (float)drow;
	RdtF = (int)(dt * 256);
	dt_div = 1 / dt;
	RdtF_div = (int)(dt_div * 256);

	arSum = (int *)malloc((dcol*4+1)*sizeof(int) );
	memset( arSum, 0, (dcol*4+1)*sizeof(int) );

	for( i = 0 ; i < dcol*4 ; i++ )
		if( i % 4 == 0 )
			arSum[i] = RdtF;



	im = image_create( drow, dcol, 4, 1, NULL );
	
	sp = IMAGE4_PIXEL( sim, 0, 0 );
	tp = IMAGE4_PIXEL( im, 0, 0 );

	tp_end = tp + im->row*im->column;

	for ( i = 0 ; i < sim->row && tp < tp_end ; i++ ) 
	{
		Cweight = CdtF;
		r_sum = g_sum = b_sum = 0;
		k = 0;
		tmpArr = arSum; 

		sp = IMAGE4_PIXEL( sim, i, 0 );
		for ( j = 0 ; k < im->column && j < sim->column ; j++ ) 
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
							
				r_sum = (r_sum * CdtF_div) >> 8;
				g_sum = (g_sum * CdtF_div) >> 8;
				b_sum = (b_sum * CdtF_div) >> 8;

		/////////////////////////////////////////////////////
				// A new pixel has been sampled from going over columns
				// and now it will be re-sampled from going over rows
				if ( *tmpArr > 256 ) 
				{
					*(tmpArr+1) += r_sum;
					
					*(tmpArr+2) += g_sum;

					*(tmpArr+3) += b_sum;

					*tmpArr -= 256;
				}
				else 
				{
					*(tmpArr+1) += ( (*tmpArr) * r_sum ) >> 8;
					
					*(tmpArr+2) += ( (*tmpArr) * g_sum ) >> 8;

					*(tmpArr+3) += ( (*tmpArr) * b_sum ) >> 8;

					r1 = ( *(tmpArr+1) * RdtF_div) >> 16;
					g1 = ( *(tmpArr+2) * RdtF_div) >> 16;
					b1 = ( *(tmpArr+3) * RdtF_div) >> 16;

					r2 = PUSH_TO_RANGE( r1, 0, 255 );
					g2 = PUSH_TO_RANGE( g1, 0, 255 );
					b2 = PUSH_TO_RANGE( b1, 0, 255 );

					// A new pixel has been sampled totally
					*tp++ = IMAGE4_RGB( r2, g2, b2 );
					
					RweightLeft = 256 - (*tmpArr);
						
					*(tmpArr+1) = ( RweightLeft * r_sum ) >> 8;
					
					*(tmpArr+2) = ( RweightLeft * g_sum) >> 8;

					*(tmpArr+3) = ( RweightLeft * b_sum) >> 8;
					*tmpArr = RdtF - RweightLeft;
				}	
		///////////////////////////////////////////////////				
				k++;
				tmpArr += 4;

				CweightLeft = 256 - Cweight;

				r_sum = CweightLeft * r;
				
				g_sum = CweightLeft * g;

				b_sum = CweightLeft * b;
				
				sp++;
				Cweight = CdtF - CweightLeft;
			}	
		}
	}

	free( arSum );

	return im;
}



/*
// this function is with 4 arrays
image_type *
image4_sample_down_new( image_type *sim, int drow, int dcol )
{
image_type	*im;
u_int	*tp,	*sp;
float	dt,	dt_div;
int	CdtF,	CdtF_div;
int	RdtF,	RdtF_div;
int	Cweight;		// weight of col 
int	CweightLeft, RweightLeft;
int	i,	j,	k;
int	r_sum,	g_sum,	b_sum;
int	*arRsum,	*arGsum,	*arBsum,	*arWeight;
int	r,	g,	b;
int	r1,	g1,	b1,	r2,	g2,	b2;

	dt = sim->column / (float)dcol;
	CdtF = (int)(dt * 256);
	dt_div = 1 / dt;
	CdtF_div = (int)(dt_div * 256);

	dt = sim->row / (float)drow;
	RdtF = (int)(dt * 256);
	dt_div = 1 / dt;
	RdtF_div = (int)(dt_div * 256);

	arRsum = (int *)malloc( dcol*sizeof(int) );
	memset( arRsum, 0, dcol*sizeof(int) );
	arGsum = (int *)malloc( dcol*sizeof(int) );
	memset( arGsum, 0, dcol*sizeof(int) );
	arBsum = (int *)malloc( dcol*sizeof(int) );
	memset( arBsum, 0, dcol*sizeof(int) );
	arWeight = (int *)malloc( dcol*sizeof(int) );
	for( i = 0 ; i < dcol ; i++ )
		arWeight[i] = RdtF;


	im = image_create( drow, dcol, 4, 1, NULL );
	
	sp = IMAGE4_PIXEL( sim, 0, 0 );
	tp = IMAGE4_PIXEL( im, 0, 0 );

	for ( i = 0 ; i < sim->row ; i++ ) 
	{
		Cweight = CdtF;
		r_sum = g_sum = b_sum = 0;
		k = 0;
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
							
				r_sum = (r_sum * CdtF_div) >> 8;
				g_sum = (g_sum * CdtF_div) >> 8;
				b_sum = (b_sum * CdtF_div) >> 8;

		/////////////////////////////////////////////////////
				// A new pixel has been sampled from going over columns
				// and now it will be re-sampled from going over rows
				if ( arWeight[k] > 256 ) 
				{
					arRsum[k] += r_sum;
					
					arGsum[k] += g_sum;

					arBsum[k] += b_sum;

					arWeight[k] -= 256;
				}
				else 
				{
					arRsum[k] += ( arWeight[k] * r_sum ) >> 8;
					
					arGsum[k] += ( arWeight[k] * g_sum ) >> 8;

					arBsum[k] += ( arWeight[k] * b_sum ) >> 8;

					r1 = (arRsum[k] * RdtF_div) >> 16;
					g1 = (arGsum[k] * RdtF_div) >> 16;
					b1 = (arBsum[k] * RdtF_div) >> 16;

					r2 = PUSH_TO_RANGE( r1, 0, 255 );
					g2 = PUSH_TO_RANGE( g1, 0, 255 );
					b2 = PUSH_TO_RANGE( b1, 0, 255 );

					// A new pixel has been sampled totally
					*tp++ = IMAGE4_RGB( r2, g2, b2 );
					
					RweightLeft = 256 - arWeight[k];
						
					arRsum[k] = ( RweightLeft * r_sum ) >> 8;
					
					arGsum[k] = ( RweightLeft * g_sum) >> 8;

					arBsum[k] = ( RweightLeft * b_sum) >> 8;

					arWeight[k] = RdtF - RweightLeft;
				}	
		///////////////////////////////////////////////////				
				k++;

				CweightLeft = 256 - Cweight;

				r_sum = CweightLeft * r;
				
				g_sum = CweightLeft * g;

				b_sum = CweightLeft * b;
				
				sp++;
				Cweight = CdtF - CweightLeft;
			}	
		}
	}

	free( arRsum );
	free( arBsum );
	free( arGsum );
	free( arWeight );


	return im;
}
*/

