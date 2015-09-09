#include	"ls_appr.h"
#include	"Uigp/igp.h"

float
get_parab_coef( vec2d v[],float tg[], float len, int no, float dw )
{
float	a,	ftemp,	dtemp;
float	fsum,	dsum;
int i;

	fsum = dsum = 0;

	for (i = 0 ; i < no; i++ ){
		ftemp = v[i].x * (v[i].x - len);

		dtemp = 2.0 * v[i].x - len;

		fsum += ftemp * ftemp + dw *dtemp * dtemp; 
		dsum += v[i].y * ftemp + dw * tg[i] * dtemp;

		//if( IS_NAN( dsum) ){
		//	fprintf( stdout, "NaN  %f %f %f %f  %f", dsum, fsum, dw,  v[i].y, tg[i] );
		//	exit(0);
		//}
	}
//	if( fsum == 0 )
//		return( 0 );

	a = dsum/fsum;

	if( IS_NAN( a) )
		fprintf( stdout, "NaN  %f %f %f", dsum, fsum, dw );
	return( a );
}


float
get_l2_dist( vec2d v[], float tg[], float a, float len, int no, float dw )
{
float	tmp,	sum = 0.0;
int i;

	for( i = 0 ; i < no ; i++ ) {
		sum += (a * v[i].x * (v[i].x - len ) - v[i].y) * (a * v[i].x
			* (v[i].x - len ) - v[i].y);

		sum += dw * (a * (2.0 * v[i].x - len) - tg[i]) * (a * (2.0 *
			v[i].x - len) - tg[i]);
	}

	tmp = sum / no;
	return( tmp );
}


float
get_max_dist( vec2d *v, float *tg, float a, float len, int no )
{
int	i;
float	tmp,	dis = 0;

	for (i = 0 ; i < no ; i++ ){

		tmp = a * v[i].x * (v[i].x - len ) - v[i].y;
		if( tmp < 0 )	tmp = -tmp;

		if( tmp > dis )	dis = tmp;
	}

	return( dis );
}
