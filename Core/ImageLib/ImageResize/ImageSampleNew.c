/**************************
 ***	ImageSampleDown.c  ***
 *************************/
#include <string.h>
#include	"ImageType/ImageType.h"
 

static image_type *image1_sample_down_new( image_type *sim, int drow, int dcol );
static image_type *image4_sample_down_new( image_type *sim, int drow, int dcol);


image_type *
image_sampleDown( image_type *sim, int drow, int dcol )
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
int	i,	j;

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
		for ( j = 0 ; j < sim->column ; j++ ) 
		{
			if ( Cweight > 256 ) {
				Csum += *sp++ << 8;
				Cweight -= 256;
			}
			else 
			{
				Csum += Cweight * (*sp);	
				Csum = (Csum *  CdtF_div) >> 16 ;

		/////////////////////////////////////////////////////
				// A new pixel has been sampled from going over columns
				// and now it will be re-sampled from going over rows
				if ( arWeight[j] > 256 ) 
				{
					arSum[j] += Csum << 8;	
					arWeight[j] -= 256;
				}
				else 
				{
					arSum[j] += arWeight[j] * (Csum) ;
					// A new pixel has been sampled totally
					*tp = (arSum[j] * RdtF_div) >> 16;
					tp++;
					RweightLeft = 256 - arWeight[j];
					arWeight[j] = RdtF;
					arSum[j] = 0;
					arSum[j] += RweightLeft * (Csum);
					arWeight[j] -= RweightLeft;
				}	
		///////////////////////////////////////////////////		
				CweightLeft = 256 - Cweight;
				Cweight = CdtF;
				Csum = 0;
				Csum += CweightLeft * (*sp++);
				Cweight -= CweightLeft;
			}	
		}
	}

	free( arSum );
	free( arWeight );

	return im;
}




// ***************
// *** image 4 ***
// ***************


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
int	Csum,	Rsum,	i,	j;
int	r_sum,	g_sum,	b_sum;
int	*arRsum,	*arGsum,	*arBsum,	*arWeight;
int	tmp;

	dt = sim->column / (float)dcol;
	CdtF = (int)(dt * 256);
	dt_div = 1 / dt;
	CdtF_div = (int)(dt_div * 256);

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


	im = image_create( drow, dcol, 4, 1, NULL );
	
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
				tmp = IMAGE4_RED(*sp);
				r_sum += tmp << 8;
				
				tmp = IMAGE4_GREEN(*sp);
				g_sum += tmp << 8;

				tmp = IMAGE4_BLUE(*sp);
				b_sum += tmp << 8;

				sp++;
				Cweight -= 256;
			}
			else 
			{

				tmp = IMAGE4_RED(*sp);
				r_sum += Cweight * tmp;
				
				tmp = IMAGE4_GREEN(*sp);
				g_sum += Cweight * tmp;

				tmp = IMAGE4_BLUE(*sp);
				b_sum += Cweight * tmp;
							
				r_sum = (r_sum * CdtF_div) >> 16;
				g_sum = (g_sum * CdtF_div) >> 16;
				b_sum = (b_sum * CdtF_div) >> 16;
				Csum = IMAGE4_RGB( r_sum, g_sum, b_sum );

		/////////////////////////////////////////////////////
				// A new pixel has been sampled from going over columns
				// and now it will be re-sampled from going over rows
				if ( arWeight[j] > 256 ) 
				{
					tmp = IMAGE4_RED(Csum);
					arRsum[j] += tmp << 8;
					
					tmp = IMAGE4_GREEN(Csum);
					arGsum[j] += tmp << 8;

					tmp = IMAGE4_BLUE(Csum);
					arBsum[j] += tmp << 8;

					arWeight[j] -= 256;
				}
				else 
				{
					tmp = IMAGE4_RED(Csum);
					arRsum[j] += arWeight[j] * tmp;
					
					tmp = IMAGE4_GREEN(Csum);
					arGsum[j] += arWeight[j] * tmp;

					tmp = IMAGE4_BLUE(Csum);
					arBsum[j] += arWeight[j] * tmp;

					arRsum[j] = (arRsum[j] * RdtF_div) >> 16;
					arGsum[j] = (arGsum[j] * RdtF_div) >> 16;
					arBsum[j] = (arBsum[j] * RdtF_div) >> 16;
					Rsum = IMAGE4_RGB( arRsum[j], arGsum[j], arBsum[j] );

					// A new pixel has been sampled totally
					*tp = Rsum;
					tp++;
					
					RweightLeft = 256 - arWeight[j];
					arWeight[j] = RdtF;
					arRsum[j] = arGsum[j] = arBsum[j] = 0;

					tmp = IMAGE4_RED(Csum);
					arRsum[j] += RweightLeft * tmp;
					
					tmp = IMAGE4_GREEN(Csum);
					arGsum[j] += RweightLeft * tmp;

					tmp = IMAGE4_BLUE(Csum);
					arBsum[j] += RweightLeft * tmp;

					arWeight[j] -= RweightLeft;
				}	
		///////////////////////////////////////////////////				
				CweightLeft = 256 - Cweight;
				Cweight = CdtF;
				r_sum = g_sum = b_sum = 0;

				tmp = IMAGE4_RED(*sp);
				r_sum += CweightLeft * tmp;
				
				tmp = IMAGE4_GREEN(*sp);
				g_sum += CweightLeft * tmp;

				tmp = IMAGE4_BLUE(*sp);
				b_sum += CweightLeft * tmp;
				
				sp++;
				Cweight -= CweightLeft;
			}	
		}
	}

	free( arRsum );
	free( arBsum );
	free( arGsum );
	free( arWeight );


	return im;
}



