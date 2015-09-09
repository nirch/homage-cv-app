#include	<math.h>

#include 	"Uigp/igp.h"
#include 	"ls_appr.h"
#include 	"GeoApp.h"



//
//static float	ls_approx( vec2d *v, vec2d *tang, int no,
//					gapp_prm *gprm, float *a );
//
//
//// the case of far point 
//
//int
//geo_approximate2( gapp_type *gapp, gapp_prm *gprm )
//{
//int	i0 =0,	i1,	n = 0;	
//
//
//	gapp->ino = 0;
//
//	while( i0 < gapp->no -1 ){
//
//		i1 = gapp_next2( gapp, i0, GEOM_DIVISION );
//
//		ls_approx( &gapp->v[i0], &gapp->tang[i0],
//					i1 - i0 + 1, gprm, &a )
//
//		
//		gapp->flag[i1] |= GEOM_DIVISION;
//		gapp->a[i1] = a;
//		gapp->ino ++;
//
//		i0 = i1;
//	}
//
//	return( gapp->ino );
//}						
//
//
//
//int
//gapp_next2( gapp_type *gapp, int i0, float dis )
//{
//vec2d	p0,	p,	v;
//int	i0;
//
//
//	p0 = gapp->v[i0];
//	v = gapp->tang[i0]
//
//
//	for( i = i0+1 ; i < gapp->no ; i++ ){
//		
//
//	}
//
//
//}




float
geo_approximate2( vec2d *p0, vec2d *p1, vec2d *v, vec2d *tang, int no, gapp_prm *gprm, float *a )
{
float	tg[MAX_POINT_NUMBER];
vec2d	vec[MAX_POINT_NUMBER],	ntan;
vec2d	dv;
float	asin,	acos,	length,	l2result,	maxresult;
float	t;
int	i,	j;


	if( no <= 2 ){
		*a = 0;
		return( 0.0 );
	}

	dv.x = p1->x - p0->x;
	dv.y = p1->y - p0->y;
	length = hypot( dv.x, dv.y );

//	asin = (v[no-1].x - v[0].x)/length;
//	acos = (v[no-1].y - v[0].y)/length;
	asin = dv.x/length;
	acos = dv.y/length;


	for( i = 0, j= 0 ; i < no ; i++ ){

//		vec[j].x =  (v[i].y - v[0].y) *acos  +  (v[i].x - v[0].x)*asin;
//		vec[j].y = -(v[i].y - v[0].y) *asin  +  (v[i].x - v[0].x)*acos;

		dv.x = v[i].x - p0->x;
		dv.y = v[i].y - p0->y;
		vec[j].x =  dv.y *acos  +  dv.x*asin;
		vec[j].y = -dv.y *asin  +  dv.x*acos;

		if( vec[j].x < 0 || vec[j].x > length )
			continue;

		ntan.x =  tang[j].y * acos  +  tang[i].x*asin;
		ntan.y = -tang[j].y * asin  +  tang[i].x*acos;
		tg[j] =  (ntan.x == 0.0)? 0.0 : ntan.y/ntan.x;

		if( ABS( tg[j] ) > 2 )	tg[i] = 0.0;

		j++;
	}	


	if( j <= 2 ){
		*a = 0;
		return( 0 );
	}



	*a = get_parab_coef( vec, tg, length, j, gprm->dwight );

	// begin 16.5.2002
	t = 1 /length;
	if( *a < 0 ){
		if( -*a > t )
			*a = -t;
	}
	else	{
		if( *a > t )
			*a = t;
	}
// end 16.5.2002


	if( length <  gprm->min_len && no < gprm->Min_pnumber )	return( 0.0 );


	maxresult = get_max_dist( vec, tg, *a, length, no );

	if( maxresult > gprm->Max_dev )	return (1000000.13f);
	if( maxresult < gprm->Min_dev )	return( 0.0 );


	l2result = get_l2_dist( vec, tg, *a, length, no, gprm->dwight );
	return( l2result );
}



