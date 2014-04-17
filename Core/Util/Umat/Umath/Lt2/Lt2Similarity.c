#include	<string.h>
#include	<stdio.h>
#include	<math.h>

#include	"Uigp/igp.h"

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