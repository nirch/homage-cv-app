/**************************
 ***	Pt2dDensity.c   ***
 **************************/
#include	<math.h>
#include	"Uigp/igp.h"


#include	"Pt2dType.h"




void
pt2dA_density_filter( pt2dA_type *apt, float T )
{
pt2d_type	*pt;
int	i,	j,	n;
float	x,	y;
float	T2;

	T2 = T*T;

	for( i = 0,	n = 0 ; i < apt->nP ; i++ ){
		pt = &apt->p[i];

		for( j = i-1 ; j >= 0 ; j-- ){
			x = pt->p.x - apt->p[j].p.x;
			y = pt->p.y - apt->p[j].p.y;
			if( x*x + y*y < T2 )
				break;
		}

		if( j >= 0 )
			continue;

		apt->p[n++] = *pt;
	}

	apt->nP = n;
}




void
pt2dA_density_filter_r( pt2dA_type *apt, float T )
{
pt2d_type	*pt;
int	i,	j,	n;
float	x,	y;
float	T2;

	T2 = T*T;

	for( i = 0,	n = 0 ; i < apt->nP ; i++ ){
		pt = &apt->p[i];

		for( j = i-1 ; j >= 0 ; j-- ){
			x = pt->p.x - apt->p[j].p.x;
			y = pt->p.y - apt->p[j].p.y;
			if( x*x + y*y < T2 )
				break;



		}

		if( j >= 0 ){
			if(  apt->p[j].r < pt->r )
				apt->p[j] = *pt;
			continue;
		}

		//		apt->p[n++] = ( pt->r > apt->p[j].r )? *pt : apt->p[j];

		apt->p[n++] = *pt;


	}

	apt->nP = n;
}


void
pt2dA_density_filter_vitality( pt2dA_type *apt, float T )
{
	pt2d_type	*pt;
	int	i,	j,	n;
	float	x,	y;
	float	T2;

	T2 = T*T;

	for( i = 0,	n = 0 ; i < apt->nP ; i++ ){
		pt = &apt->p[i];

		for( j = i-1 ; j >= 0 ; j-- ){
			x = pt->p.x - apt->p[j].p.x;
			y = pt->p.y - apt->p[j].p.y;
			if( x*x + y*y < T2 )
				break;



		}

		if( j >= 0 ){
			if(  apt->p[j].vitality < pt->vitality )
				apt->p[j] = *pt;
			continue;
		}

		//		apt->p[n++] = ( pt->r > apt->p[j].r )? *pt : apt->p[j];

		apt->p[n++] = *pt;


	}

	apt->nP = n;
}


void
pt2dA_density_merge( pt2dA_type *apt, pt2dA_type *tapt, float T )
{
pt2d_type	*pt;
int	i,	j,	n;
float	x,	y;
float	T2;

//	if( tapt->nP + apt->nP < tapt->NP )
	tapt = pt2dA_realloc( tapt, tapt->nP + apt->nP );

	T2 = T*T;

	for( i = 0,	n = 0 ; i < apt->nP ; i++ ){
		pt = &apt->p[i];

		for( j = 0 ; j < tapt->nP ; j++ ){
			x = pt->p.x - tapt->p[j].p.x;
			y = pt->p.y - tapt->p[j].p.y;

			if( x*x + y*y < T2 )
				break;
		}

		if( j < tapt->nP )	continue;



		tapt->p[tapt->nP] = *pt;
		//		tapt->p[tapt->nP].group = tapt->nGroup;
		tapt->nP++;
		n++;
	}


	//	if( n > 0 )
	//		tapt->nGroup++;
}