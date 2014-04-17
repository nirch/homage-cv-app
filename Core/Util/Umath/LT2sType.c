/**********************
 ***   Lt2sTool.c   ***
 **********************/
#include	<stdlib.h>
#include	<string.h>
#include	<math.h>

#include	"Uigp/igp.h"
#include	"Umath/LT2sType.h"

void
lt2s_clear( lt2s_type *lt )
{
	lt->x = 0;
	lt->y = 0;
	lt->r = 0;
	lt->s = 0;
}

void
lt2s_from_lt2( lt2_type *lt, lt2s_type *qr )
{
float	a,	b;

	qr->x = LT2_F1_C(*lt);
	qr->y = LT2_F2_C(*lt);

	a = LT2_F1_A(*lt);
	b = LT2_F1_B(*lt);
	qr->s = hypot( a, b );

	qr->r = atan2( b, a );
}



void
lt2s_to_lt2( lt2s_type *qr, lt2_type *lt )
{
	LT2_F1_A(*lt) = qr->s * cos( qr->r );
	LT2_F1_B(*lt) = qr->s * sin( qr->r );
	LT2_F2_A(*lt) = -LT2_F1_B(*lt);
	LT2_F2_B(*lt) = LT2_F1_A(*lt);

	LT2_F1_C(*lt) = qr->x;
	LT2_F2_C(*lt) = qr->y;
}




void 
lt2s_max_diff( lt2s_type *lt1, lt2s_type *lt2, lt2s_type *m )
{
int	i;
float	a;

	for( i = 0 ; i < 4 ; i++ ){
		if( (a = lt1->a[i] - lt2->a[i]) < 0 )	a = -a;
		if( a > m->a[i] )	m->a[i] = a;
	}
}



void lt2s_print( lt2s_type *qr, FILE *fp )
{	
	fprintf( fp, "%6.4f   %6.4f    %6.4f    %6.4f\n",
			qr->x, qr->y, qr->r, qr->s );
}


void lt2sA_print( lt2s_type qr[], int nQr, char *title, FILE *fp )
{
int	i;

	fprintf( stdout, "%s\n", title );

	for( i = 0 ; i < nQr ; i++ ){
		fprintf( fp, "%3d   %6.4f   %6.4f    %6.4f    %6.4f\n",
							i,
							qr[i].x,
							qr[i].y, 
							qr[i].s,
							qr[i].r );


	}
}




/*
void CMepStabilization::FilterQr( qr_type qr[], qr_type sqr[], int nQr, int k0, float sigma )
{
float	g[MAX_RC];
int	i,	j,	k,	rC;
float	sum,	w;
float	 a;

	rC = (int)(3*sigma);
	guasian( rC, sigma, g );


	fprintf( stdout, "rC: %d  sigma: %f\n", rC, sigma );


	a = 0;
	sqr[0].a[k0] = (k0 != 2 )? 0 : 1.0;
	for( i = 1 ; i < nQr ; i++ ){
		sum = w = 0;
		for( k = 0, j = i-rC ; k < 2*rC+1 && j < nQr ; k++, j++ ){
			if( j < 1 )	continue;
			sum += g[k]*qr[j].a[k0];
			w += g[k];
		}

		sqr[i].a[k0] = sum / w;
	}
}
*/


