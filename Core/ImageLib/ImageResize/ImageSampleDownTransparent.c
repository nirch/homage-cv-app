/**************************
 ***	ImageSampleDown.c  ***
 *************************/
#include	<string.h>

#include	"ImageType/ImageType.h"
 

static image_type *image4_sample_down_new_transparent( image_type *sim, int drow, int dcol);


image_type *
image_sample_down_transparent( image_type *sim, int drow, int dcol )
{
image_type	*im;


	if ( sim->depth == 4 ) {
		im = image4_sample_down_new_transparent(sim, drow, dcol);

		return( im );
	}
	else
		return NULL;
}




// ***************
// *** image 4 ***
// ***************


// this function is with one array
image_type *
image4_sample_down_new_transparent( image_type *sim, int drow, int dcol )
{
image_type	*im;
u_int	*tp,	*sp,	*tp_end;
float	dt,	dt_div;
int	CdtF,	CdtF_div;
int	RdtF,	RdtF_div;
int	Cweight;		// weight of col 
int	CweightLeft, RweightLeft;
int	i,	j,	k;
int	r_sum,	g_sum,	b_sum, t_sum;	// t_sum is for transparency
int	*arSum,	*tmpArr;
int	r,	g,	b, t;					// t is for transparency
int	r1,	g1,	b1,	t1,	r2,	g2,	b2,	t2;

	dt = sim->column / (float)dcol;
	CdtF = (int)(dt * 256);
	dt_div = 1 / dt;
	CdtF_div = (int)(dt_div * 256);

	dt = sim->row / (float)drow;
	RdtF = (int)(dt * 256);
	dt_div = 1 / dt;
	RdtF_div = (int)(dt_div * 256);

	arSum = (int *)malloc((dcol*5+1)*sizeof(int) );
	memset( arSum, 0, (dcol*5+1)*sizeof(int) );

	for( i = 0 ; i < dcol*5 ; i++ )
		if( i % 5 == 0 )
			arSum[i] = RdtF;



	im = image_create( drow, dcol, 4, 1, NULL );
	
	sp = IMAGE4_PIXEL( sim, 0, 0 );
	tp = IMAGE4_PIXEL( im, 0, 0 );

	tp_end = tp + im->row*im->column;

	for ( i = 0 ; i < sim->row && tp < tp_end ; i++ ) 
	{
		Cweight = CdtF;
		r_sum = g_sum = b_sum = t_sum = 0;
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

				t = ( ( *sp >> 24) & 0xff );
				t_sum += t << 8;

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

				t = ( ( *sp >> 24) & 0xff );
				t_sum += Cweight * t;
							
				r_sum = (r_sum * CdtF_div) >> 8;
				g_sum = (g_sum * CdtF_div) >> 8;
				b_sum = (b_sum * CdtF_div) >> 8;
				t_sum = (t_sum * CdtF_div) >> 8;

		/////////////////////////////////////////////////////
				// A new pixel has been sampled from going over columns
				// and now it will be re-sampled from going over rows
				if ( *tmpArr > 256 ) 
				{
					*(tmpArr+1) += r_sum;
					
					*(tmpArr+2) += g_sum;

					*(tmpArr+3) += b_sum;

					*(tmpArr+4) += t_sum;

					*tmpArr -= 256;
				}
				else 
				{
					*(tmpArr+1) += ( (*tmpArr) * r_sum ) >> 8;
					
					*(tmpArr+2) += ( (*tmpArr) * g_sum ) >> 8;

					*(tmpArr+3) += ( (*tmpArr) * b_sum ) >> 8;

					*(tmpArr+4) += ( (*tmpArr) * t_sum ) >> 8;

					r1 = ( *(tmpArr+1) * RdtF_div) >> 16;
					g1 = ( *(tmpArr+2) * RdtF_div) >> 16;
					b1 = ( *(tmpArr+3) * RdtF_div) >> 16;
					t1 = ( *(tmpArr+4) * RdtF_div) >> 16;

					r2 = PUSH_TO_RANGE( r1, 0, 255 );
					g2 = PUSH_TO_RANGE( g1, 0, 255 );
					b2 = PUSH_TO_RANGE( b1, 0, 255 );
					t2 = PUSH_TO_RANGE( t1, 0, 255 );

					// A new pixel has been sampled totally
					*tp = IMAGE4_RGB( r2, g2, b2 );
					t2 = t2 << 24;
					*tp = *tp | t2;
					tp++;
					
					RweightLeft = 256 - (*tmpArr);
						
					*(tmpArr+1) = ( RweightLeft * r_sum ) >> 8;
					
					*(tmpArr+2) = ( RweightLeft * g_sum) >> 8;

					*(tmpArr+3) = ( RweightLeft * b_sum) >> 8;

					*(tmpArr+4) = ( RweightLeft * t_sum) >> 8;

					*tmpArr = RdtF - RweightLeft;
				}	
		///////////////////////////////////////////////////				
				k++;
				tmpArr += 5;

				CweightLeft = 256 - Cweight;

				r_sum = CweightLeft * r;
				
				g_sum = CweightLeft * g;

				b_sum = CweightLeft * b;

				t_sum = CweightLeft * t;
				
				sp++;
				Cweight = CdtF - CweightLeft;
			}	
		}
	}

	free( arSum );

	return im;
}


void image_sample_down_fix_transparent( image_type *im )
{
int	i,	j,	t;
u_int	*tp;

	tp = (u_int *)im->data;
	for(i = 0; i < im->row; i++ ) {
		for(j = 0; j < im->column; j++, tp++ ) {
			t = ( *tp >> 24 ) & 0xff;
			if( t > 32 )
				*tp = *tp | 0xff000000;
			else *tp &= 0x00ffffff;
		}
	}
}

