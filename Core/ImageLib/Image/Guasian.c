
#include	<math.h>

#include	"ImageType/ImageType.h"


void
guasian( int n, float sigma, float g[] )
{
float	x,	t,	sum;
int	i;

	t = exp(1);

	for( x = -n, i = 0, sum = 0 ; x <= n ; x++, i++ ){
		t = -0.5*x*x / ( sigma*sigma);
		g[i] = exp( t );

		sum += g[i];
	}


	for( i = 0 ; i < 2*n+1 ; i++ )
		g[i] /= sum;
}





void
guasian_dg( int n, float sigma, float g[], float dg[] )
{
float	x,	t,	sum;
int	i;

	t = exp(1);

	for( x = -n, i = 0, sum = 0 ; x <= n ; x++, i++ ){
		t = -0.5*x*x / ( sigma*sigma);
		g[i] = exp( t );

		sum += g[i];
	}


	for( i = 0 ; i < 2*n+1 ; i++ )
		g[i] /= sum;

	for( x = -n, i = 0 ; x <= n ; x++, i++ )
		dg[i] = -x * g[i]/ ( sigma*sigma );
}


