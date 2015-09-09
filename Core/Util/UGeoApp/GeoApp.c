#include	<math.h>

#include 	"Uigp/igp.h"
#include 	"ls_appr.h"
#include 	"GeoApp.h"


 
static int	g_div_corner_appr( gapp_type *gapp, int from, int to,
					gapp_prm *gprm );


static float	ls_approx( vec2d *v, vec2d *tang, int no,
					gapp_prm *gprm, float *a );




int
geo_approximate( gapp_type *gapp, gapp_prm *gprm )
{
int	i0 =0,	i1,	n = 0;	


	gapp->ino = 0;

	while( i0 < gapp->no -1 ){

		i1 = gapp_next( gapp, i0, GEOM_DIVISION );

		n = g_div_corner_appr( gapp, i0, i1, gprm );

		i0 = i1;
	}

	return( gapp->ino );
}						






static int
g_div_corner_appr( gapp_type *gapp, int from, int to, gapp_prm *gprm )
{	
int	i,	n0,	n1;
float	ans,	len2,	len,	a;
vec2d	dv;
int	dk,	n,	k,	no;

	dv.x = gapp->v[to].x - gapp->v[from].x;
	dv.y = gapp->v[to].y - gapp->v[from].y;


	len2 = dv.x * dv.x  + dv.y * dv.y;


	if( len2 > gprm->max_len * gprm->max_len && to - from > 1 ) {
		len = sqrt( len2 );
		no = len/gprm->max_len+1;
		dk = (to - from)/no;

		if ( dk == 0 ) {
			k = (to - from)/2;

			n0 = g_div_corner_appr( gapp, from, from + k, gprm );

			n1 = g_div_corner_appr( gapp, from + k, to, gprm );

			return( n0 + n1 ); 
		}

		n = 0;
		for( k = 0, i = from ; k < no-1 ; k++, i += dk )
			n += g_div_corner_appr( gapp, i, i + dk, gprm );

		n += g_div_corner_appr( gapp, i, to, gprm );

		return( n );
	}


	if ((len2 == 0) && (to == gapp->no - 1))
		ans = gprm->err+ 1.0;
	else	{
		ans = ls_approx( &gapp->v[from], &gapp->tang[from],
			to - from + 1, gprm, &a );

//		if( IS_NAN( a) )
//			fprintf( stdout, "NaN" );
	}




/*
	if ((len2 < gprm->max_len * gprm->max_len) && ans < gprm->err ) {
*/
	if( ans < gprm->err ) {
		gapp->flag[to] |= GEOM_DIVISION;
		gapp->a[to] = a;
		gapp->ino ++;
		return( 1 );
	}


	k = (to - from)/2;

	n0 = g_div_corner_appr( gapp, from, from + k, gprm );

	n1 = g_div_corner_appr( gapp, from + k, to, gprm );

	return( n0 + n1 );
}


void
geo_approximate_modify( gapp_type *gapp, int dlen, float Dang, gapp_prm *gprm )
{	
int	i0,	i1,	i2;
float	ans,	a,	dang;
vec2d	dv0,	dv1;

	if( gapp->no == 1 )	return;
	i0 = 0;
	i1 = gapp_next( gapp, i0, GEOM_DIVISION );

	while( i1 < gapp->no -1 ){

		i2 = gapp_next( gapp, i1, GEOM_DIVISION );

		if( i2 - i0 > dlen ){
			i0 = i1;
			i1 = i2;
			continue;
		}

		if( i2 - i0 > 3 ){
			dv0.x = gapp->v[i1].x - gapp->v[i0].x;
			dv0.y = gapp->v[i1].y - gapp->v[i0].y;

			dv1.x = gapp->v[i2].x - gapp->v[i1].x;
			dv1.y = gapp->v[i2].y - gapp->v[i1].y;
			dang = (dv0.x *dv1.x + dv0.y *dv1.y)/
				(hypot( dv0.x, dv0.y )*hypot( dv1.x, dv1.y ));

			if( dang < Dang ){
				i0 = i1;
				i1 = i2;
				continue;
			}
		}


		ans = ls_approx( &gapp->v[i0], &gapp->tang[i0],
			i2 - i0 + 1, gprm, &a );

		if( ans < gprm->err ) {
			gapp->flag[i1] &= ~GEOM_DIVISION;
			gapp->a[i2] = a;
			gapp->ino--;
			if( gapp->ino == 1 )	return;
			i1 = i2;
			continue;
		}

		i0 = i1;
		i1 = i2;
	}
}







static float
ls_approx( vec2d *v, vec2d *tang, int no, gapp_prm *gprm, float *a )
{
float	tg[MAX_POINT_NUMBER];
vec2d	vec[MAX_POINT_NUMBER],	ntan;

float	asin,	acos,	length,	l2result,	maxresult;
float	t;
int	i;


	if( no == 2 ){
		*a = 0;
		return( 0.0 );
	}


	length = hypot( v[no-1].x - v[0].x, v[no-1].y - v[0].y );

	if( length < 0.5 && no > 32 )
		return (1000000.13f);

	asin = (v[no-1].x - v[0].x)/length;
	acos = (v[no-1].y - v[0].y)/length;


	for( i = 0 ; i < no ; i++ ){

		vec[i].x =  (v[i].y - v[0].y) *acos  +  (v[i].x - v[0].x)*asin;
		vec[i].y = -(v[i].y - v[0].y) *asin  +  (v[i].x - v[0].x)*acos;

		ntan.x =  tang[i].y * acos  +  tang[i].x*asin;
		ntan.y = -tang[i].y * asin  +  tang[i].x*acos;
		tg[i] =  (ntan.x == 0.0)? 0.0 : ntan.y/ntan.x;

		if( ABS( tg[i] ) > 2 )	tg[i] = 0.0;
	}	




	*a = get_parab_coef( vec, tg, length, no, gprm->dwight );

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




int
gapp_next( gapp_type *gapp, int from, int div_type )
{
int next;

	for(next = from + 1; next < gapp->no ; next++ )
		if( gapp->flag[next] & div_type )
			return( next );


	return( gapp->no-1 );
}


void
gapp_inverse( gapp_type *gapp )
{
	int	i,	j;
	vec2d	tmp;
	int	flag;

	for( i = 0, j = gapp->no-1 ; i < gapp->no /2 ; i++, j-- ){
		tmp = gapp->v[i];
		gapp->v[i] = gapp->v[j];
		gapp->v[j] = tmp;

		tmp = gapp->tang[i];
		gapp->tang[i] = gapp->tang[j];
		gapp->tang[j] = tmp;

		flag = gapp->flag[i];
		gapp->flag[i] = gapp->flag[j];
		gapp->flag[j] = flag;
	}
}