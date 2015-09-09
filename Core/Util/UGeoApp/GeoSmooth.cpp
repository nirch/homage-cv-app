/************************
 ***	LnRegGapp.c   ***
 ************************/
#include	<stdio.h>   
#include	<math.h>

#include	"Uigp/igp.h"
#include	"GeoApp.h"





static void	gapp_smooth_end( gapp_type *gapp );

//static int	gapp_smooth_next( gapp_type *gapp, int k, int *i0, int *i1 );


static int	gapp_smooth_point( gapp_type *gapp, int i0, float *a, float *max_u );

void
gapp_smooth( gapp_type *gapp )
{
int	i;
float	max_u;


	if( gapp->no < 5 )	return;

	
	gapp->flag[2] = gapp_smooth_point( gapp, 2, &gapp->a[2], &max_u );
	if( gapp->flag[2] ==  1 &&  max_u < 0.15 )
		gapp->flag[0] = gapp->flag[1] = 1;


	for( i = 3 ; i < gapp->no -2 ; i++ ){
		gapp->flag[i] = gapp_smooth_point( gapp, i, &gapp->a[i], &max_u );
	}


	if( gapp->flag[gapp->no-3] == 1 && max_u < 0.15 )
		gapp->flag[gapp->no-2] = gapp->flag[gapp->no-1] = 1;


//	gapp_smooth_end( gapp );
}



int
gapp_smooth_next( gapp_type *gapp, int k, int *i0, int *i1 )
{
	int	i;

	for( i = k ; i < gapp->no ; i++ )
		if( gapp->flag[i] != 0 )	break;

	if( i == gapp->no )	return( -1 );
	*i0 = i;

	for( i = *i0+1 ; i < gapp->no ; i++ )
		if( gapp->flag[i] == 0 )	break;

	*i1 = i-1;

	return( 1 );
}


void
gapp_smooth_extend_end( gapp_type *gapp, int *i0, int *i1 )
{
float	a,	max_u;



	if( *i0 > 2 ){
		gapp_smooth_point( gapp, *i0, &a, &max_u );
		if( max_u < 0.1 )
			*i0 -= 2;
		else	{
			gapp_smooth_point( gapp, *i0+1, &a, &max_u );
			if( max_u < 0.1 )
				*i0 -= 1;
		}
	}

	if( *i1 < gapp->no-2 ){
		gapp_smooth_point( gapp, *i1, &a, &max_u );
		if( max_u < 0.1 )
			*i1 += 2;
		else	{

			gapp_smooth_point( gapp, *i1-1, &a, &max_u );
			if( max_u < 0.1 )
				*i1 += 1;
		}
	}
}


static void
gapp_smooth_end( gapp_type *gapp )
{
int	i,	i0,	i1;
int	j0,	j1;
float	a,	max_u;

	j0 = j1 = 0;
	for( i = 0 ; i < gapp->no ; i = i1+1 ){
		if( gapp_smooth_next( gapp, i, &i0, &i1 ) < 0 )
			break;

		if( i1 - i0 < 6 )
			continue;

		if( i0 > 2 ){
			gapp_smooth_point( gapp, i0, &a, &max_u );
			if( max_u < 0.1 )
				gapp->flag[i0-2] = gapp->flag[i0-1] = 1;
			else	{
				gapp_smooth_point( gapp, i0+1, &a, &max_u );
				if( max_u < 0.1 )
					gapp->flag[i0-1] = 1;
			}
		}

		if( i1 < gapp->no-2 ){
			gapp_smooth_point( gapp, i1, &a, &max_u );
			if( max_u < 0.1 )
				gapp->flag[i1+2] = gapp->flag[i1+1] = 1;
			else	{

				gapp_smooth_point( gapp, i1-1, &a, &max_u );
				if( max_u < 0.1 )
					gapp->flag[i1+1] = 1;
			}
		}

	}

	
}



		
static int
gapp_smooth_point( gapp_type *gapp, int i0, float *A, float *max_u )
{
vec2d	v,	uv,	dv;
float	tmin,	tmax,	st,	su;
float	t2[5],	u[5],	t,	du;
float	a;
int	i;

	tmin = tmax = 0;
	su = st = 0;

	v = gapp->v[i0];
	uv.x = gapp->tang[i0].y;
	uv.y = -gapp->tang[i0].x;

	for( i = 0 ; i < 5 ; i++ ){
		if( i == 2 ){
			t2[2] = u[2] = 0;
			continue;
		}

		dv.x = gapp->v[i0-2+i].x - v.x;
		dv.y = gapp->v[i0-2+i].y - v.y;


		t = -uv.y * dv.x + uv.x * dv.y;
		u[i] = uv.x * dv.x + uv.y * dv.y;

		if( t < tmin )	tmin = t;
		else if( t > tmax )	tmax = t;

		t2[i] = t*t;
		su += u[i] * t2[i];
		st += t2[i]*t2[i];


	}
	

	if( tmin > -1.25 || tmax < 1.25 )	return( 0 );

	if( st == 0 )	return(0);

	a = 0.5 * su / st;
	*A = a;

	if( ABS(a) > 0.5 )	return( 0 );

	*max_u = 0;
	for( i = 0 ; i < 5 ; i++ ){
		du = u[i] - a*t2[i];
		if( du < 0 )	du = -du;
		if( du > 0.5 )	return( 0 );

		if( *max_u < du )	*max_u = du;
	}


	return( 1 );
}



//static int
//gapp_smooth_next( gapp_type *gapp, int k, int *i0, int *i1 )
//{
//	int	i;
//
//	for( i = k ; i < gapp->no ; i++ )
//		if( gapp->flag[i] != 0 )	break;
//
//	if( i == gapp->no )	return( -1 );
//	*i0 = i;
//
//	for( i = *i0+1 ; i < gapp->no ; i++ )
//		if( gapp->flag[i] == 0 )	break;
//
//	*i1 = i-1;
//
//	return( 1 );
//}
