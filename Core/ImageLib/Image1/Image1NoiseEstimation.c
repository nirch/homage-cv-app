
#include	<math.h>

#include	"ImageType/ImageType.h"

#define	RC	24




float
image1_noise_estimation( image_type *sim, float sigma, float minG )
{
image_type	*im;
float	g[RC*2+1];
float	sum,	sum2;
int	i,	j,	no,	rC;
u_char	*sp,	*cp,	*cp1;
float	dx,	dy,	d,	dv,	var,	st;


	rC = (int)(3*sigma);
	guasian( rC, sigma, g );

	im = image1_convolution_separably( sim, g, g, rC, NULL );


	sum = sum2 = 0;
	no = 0;

	sp = sim->data;
	cp = im->data;

	for( i = 0 ; i < im->row-1 ; i++, sp++, cp++ ){
		cp1 = cp + im->column;
		for( j = 0 ; j < im->column-1 ; j++, sp++, cp++, cp1++ ){

			if( *cp < 10 || *cp > 50 )	continue;


			dx = (int)*(cp+1) - (int)*cp;
			dy = (int)*(cp1) - (int)*cp;

			d = dx*dx + dy*dy;

			if( d > minG )	continue;

			dv = (int)*sp - (int)*cp;
			sum += dv;
			sum2 += dv *dv;
			no++;
		}
	}

	image_destroy( im, 1 );


	if( sim->row *sim->column > no * 100 )
		return( 0 );

	sum /= no;
	sum2 /= no;
	var = sum2 - sum*sum;

	st = sqrt( var );

	return( st );
}


float
image1_information_estimation( image_type *sim )
{

int	i,	j,	no;
u_char	*cp,	*cp1;
float	dx,	dy;
float	sdx,	sdy,	sdxy;



	sdx = sdy = sdxy = 0;
	no = 0;


	cp = sim->data;

	for( i = 0 ; i < sim->row-1 ; i++, cp++ ){
		cp1 = cp + sim->column;
		for( j = 0 ; j < sim->column-1 ; j++, cp++, cp1++ ){


			dx = (int)*(cp+1) - (int)*cp;
			dy = (int)*(cp1) - (int)*cp;


			sdx += dx*dx;
			sdy += dy*dy;

			sdxy += dx*dy;
			no++;
		}
	}

	sdx  /= no;
	sdy /= no;
	sdxy /= no;

	return( sdx*sdy - sdxy*sdxy );
}


