#include	<string.h>
#include	<stdio.h>
#include	<math.h>

#include	"Uigp/igp.h"
#include	"Umath/Matrix4Type.h"

#include "../Lt2Type.h"


static int	lt2_similarity_test( lt2_type *lt, vec2f_type *sp0, vec2f_type *sp1, vec2f_type *tp0, vec2f_type *tp1 );

int
lt2_similarity( lt2_type *lt, vec2f_type *sp0, vec2f_type *sp1, vec2f_type *tp0, vec2f_type *tp1 )
{
vec2f_type	sv,	tv;
vec2f_type	v,	u,	l;
float	t;

	sv.x = sp1->x - sp0->x;
	sv.y = sp1->y - sp0->y;

	tv.x = tp1->x - tp0->x;
	tv.y = tp1->y - tp0->y;


	t = hypot( sv.x, sv.y );
	v.x = sv.x/t;
	v.y = sv.y/t;

	u.x = -v.y;
	u.y = v.x;

	l.x = v.x * tv.x + v.y * tv.y;
	l.y = u.x * tv.x + u.y * tv.y;


	l.x /= t;
	l.y /= t;



	lt->a0 = l.x;
	lt->b0 = -l.y;
	lt->a1 = l.y;
	lt->b1 = l.x;



	lt->c0 = tp0->x - ( lt->a0*sp0->x + lt->b0*sp0->y );
	lt->c1 = tp0->y - ( lt->a1*sp0->x + lt->b1*sp0->y );

//#define TEST
#ifdef TEST

		lt2_similarity_test( lt, sp0, sp1, tp0, tp1 );

#endif

	return( 1 );
}


static int
lt2_similarity_test( lt2_type *lt, vec2f_type *sp0, vec2f_type *sp1, vec2f_type *tp0, vec2f_type *tp1 )
{
vec2f_type	pt0,	pt1;

	pt0.x = LT2_F1( *lt, sp0->x, sp0->y );
	pt0.y = LT2_F2( *lt, sp0->x, sp0->y );

	pt0.x -= tp0->x;
	pt0.y -= tp0->y;


	pt1.x = LT2_F1( *lt, sp1->x, sp1->y );
	pt1.y = LT2_F2( *lt, sp1->x, sp1->y );

	pt1.x -= tp1->x;
	pt1.y -= tp1->y;

	if( ABS(pt0.x) > 0.001  || ABS(pt0.y) > 0.001 || ABS(pt1.x) > 0.001  || ABS(pt1.y) > 0.001 )
		return( -1 );



	return( 1 );
}






int	
lt2_similarity_approximate( lt2_type *lt, vec2fA_type *as, vec2fA_type *at )
{

	matrix4_type m;
	double	D[4],	X[4];
	int	i;

	matrix4_zero( &m );
	D[0] = D[1] = D[2] = D[3] = 0;

	for( i = 0 ; i < as->nA ; i++ ){

		vec2f_type	*s = &as->a[i];
		vec2f_type  *t = &at->a[i];

		m.a00 += s->x*s->x + s->y*s->y;
		m.a01 += s->y*s->x - s->x*s->y;
		m.a02 += s->x;
		m.a03 += s->y;

		D[0] += t->x*s->x + t->y*s->y;


		m.a10 += s->x*s->y - s->y*s->x;
		m.a11 += s->y*s->y + s->x*s->x;
		m.a12 += s->y;
		m.a13 += -s->x;

		D[1] += t->x*s->y - t->y*s->x;




		m.a20 += s->x;
		m.a21 += s->y ;
		m.a22 += 1;
		m.a23 += 0;

		D[2] += t->x;


		m.a30 += s->y;
		m.a31 += -s->x ;
		m.a32 += 0;
		m.a33 += 1;

		D[3] += t->y;

	}


	if( matrix4_solve( &m, D, X ) < 0 )
		return( -1 );


	lt->a0 = X[0];
	lt->b0 = X[1];
	lt->c0 = X[2];
	lt->a1 = -X[1];
	lt->b1 = X[0];
	lt->c1 = X[3];

	return( 1 );


}



void
lt2_similarity_approximate_test()
{
	lt2_type	lt;


	lt.a0 = 1;
	lt.b0 = 1.5;
	lt.c0 = 2;

	lt.a1 = -1.5;
	lt.b1 = 1;
	lt.c1 = -1;

	vec2fA_type *as = vec2fA_alloc( 16 );
	vec2fA_type *at = vec2fA_alloc( 16 );

	int	i,	j,	n;
	for( i = 0,	n = 0  ; i < 4 ; i++ ){
		for( j = 0 ; j < 4 ; j++ ){
			as->a[n].x = i;
			as->a[n].y = j;

			at->a[n].x  = LT2_F1( lt, as->a[n].x, as->a[n].y );
			at->a[n].y  = LT2_F2( lt, as->a[n].x, as->a[n].y );

			n++;
		}
	}

	as->nA = at->nA = 16;

	lt2_type	lt1;
	lt2_similarity_approximate( &lt1, as, at );

	lt2_print( stdout, &lt1 );

}
