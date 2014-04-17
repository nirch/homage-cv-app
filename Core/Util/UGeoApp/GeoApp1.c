#include	<math.h>

#include 	"Uigp/igp.h"
#include 	"ls_appr.h"
#include 	"GeoApp.h"


 
/*
void
geo_approximate_1( gapp_type *gapp, int i0, int i1,
			vec2d *p0, vec2d *p1, gapp_prm *gprm )
{	
float	ans,	a,	a2,	len;
vec2d	p2;
int	i2;


	ans = geo_approximate_l2( p0, p1,
			&gapp->v[i0], &gapp->tang[i0], i1 - i0 + 1, gprm,
			&a, &len, &p2, &i2, &a2 );

	if( ans < gprm->err ){
		gapp->flag[i1] |= GEOM_DIVISION;

		gapp->p[i0] = *p0;
		gapp->p[i1] = *p1;

		gapp->a[i1] = a;
		gapp->ino ++;
		return;
	}



	i2 += i0;
	geo_approximate_1( gapp, i0, i2-1, p0, &p2, gprm );
	geo_approximate_1( gapp, i2, i1, &p2, p1, gprm );
}
*/





float
geo_approximate_l2( vec2d *p0, vec2d *p1,
		vec2d *v, vec2d *tang, int no,
		gapp_prm *gprm,
		float *a, float *len,
		vec2d *p2, int *i2, float *a2 )
{
float	tg[MAX_POINT_NUMBER];
vec2d	vec[MAX_POINT_NUMBER],	ntan;

float	asin,	acos,	length;
float	l2result,	dev,	t,	dt;
vec2d	dp;

int	i,	tmp;


	if( no == 2 ){
		dp.x = p1->x - p0->x;
		dp.y = p1->y - p0->y;
		*len = hypot( dp.x, dp.y );
		*a = 0;
		return( 0.0 );
	}


	dp.x = p1->x - p0->x;
	dp.y = p1->y - p0->y;
	*len = length = hypot( dp.x, dp.y );

//	if( length > gprm->max_len ){
//		need to set i2 and p2
//		return (1000000.13F);
//	}


//	if( length == 0 && no > gprm->Min_pnumber ){ //Grisha 10.07.01
	if( length < 1E-3 && no > gprm->Min_pnumber ){
		i = no >> 1;
		*p2 =  v[i];
		*i2 = i;

		return( 10000 );
	}

		

	asin = dp.x / length;
	acos = dp.y / length;


	for( i = 0 ; i < no ; i++ ){

		vec[i].x =  (v[i].y - p0->y) *acos  +  (v[i].x - p0->x)*asin;
		vec[i].y = -(v[i].y - p0->y) *asin  +  (v[i].x - p0->x)*acos;

		ntan.x =  tang[i].y * acos  +  tang[i].x*asin;
		ntan.y = -tang[i].y * asin  +  tang[i].x*acos;
		tg[i] =  (ntan.x == 0.0)? 0.0 : ntan.y/ntan.x;

		if( ABS( tg[i] ) > 2 )	tg[i] = 0.0;
	}	




	*a = get_parab_coef( vec, tg, length, no, gprm->dwight );
	*a2 = 0;

// begin 16.5.2002
//	t = 0.25 *length;

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


	dev = get_max_dist( vec, tg, *a, length, no );

	if( dev < gprm->Min_dev && length < gprm->max_len )	return( 0.0 );


	t = 0.5*length;
	for( i = 0 ; i < no ; i++ )
		if( vec[i].x > t )	break;


	if( (tmp = 0.5*no - i ) < 0 )	tmp = -tmp;

	if( tmp > 2*gprm->Min_pnumber || i < 3 || no-i < 3 ){
		i = 0.5*no;
		t =  vec[i].x;
	}

		
	*i2 = i;

	dt = (t - vec[i-1].x) / (vec[i].x - vec[i-1].x);
	p2->x = dt * v[i-1].x + (1-dt)*v[i].x;
	p2->y = dt * v[i-1].y + (1-dt)*v[i].y;
	*a2 = -(p2->y - p0->y) *asin  +  (p2->x - p0->x)*acos;



	if( dev > gprm->Max_dev )	return (1000000.13F);
	if( length > gprm->max_len )	return (1000000.13F);


	l2result = get_l2_dist( vec, tg, *a, length, no, gprm->dwight );
	return( l2result );
}
