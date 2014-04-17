/********************
 ***   LkType1.h   ***
 ********************/

#include	<stdio.h>   
#include	<math.h>
#include	<stdlib.h>


#include	"Uigp/igp.h"
#include	"Umath/Lt2Type.h"

#include	"LnType.h"

ln_type *
lnL_copy( ln_type *plink, ln_type *elink ) 
{ 
	ln_type	*link,	*l,	*pl,	*el;

	pl = l = NULL;

	for( el = plink ; el != NULL  && el != elink;  el = LN_NEXT(el) ){

		l = LN_ALLOC();
		*l = *el;

		if( pl != NULL ){
			l->p[F_BEGIN] = pl;
			pl->p[F_END] = l;
		}
		else	link = l;


		pl = l;
	}


	link->p[0] = NULL;

	pl->p[1] = NULL;

	return( link ); 
}

ln_type *
lnL_last( ln_type *link )
{
ln_type	*l,	*pl;

	for( l = link; l != NULL ; l = LN_NEXT(l) ){
		pl = l;
	}

	return( pl );
}


void
lnL_connect( ln_type *link, ln_type *nl )
{
	ln_type	*l;

	l = lnL_last( link );

	l->p[1] = nl;
	nl->p[0] = l;
}



void
lnL_end_point( vec2d *ctr, ln_type *link, vec2d *p )
{
	ln_type	*l;

	*p = *ctr;

	for( l = link; l != NULL ; l = LN_NEXT(l) ){
		p->x += l->v.x;
		p->y += l->v.y;
	}
}

void
lnL_lastP( vec2d *ctr, ln_type *link, vec2d *p, ln_type **ll )
{
	ln_type	*l;

	*p = *ctr;

	for( l = link; l != NULL ; l = LN_NEXT(l) ){
		p->x += l->v.x;
		p->y += l->v.y;
		*ll = l;
	}
}

void
lnL_gt2lt( ln_type *link, float gt, ln_type **l0, float *t0 )
{
	ln_type	*l;

	if( gt <= 0 ){
		*l0 = link;
		*t0 = 0;
		return;
	}

	for( l = link ; gt > l->len && LN_NEXT(l) != NULL ; l = LN_NEXT(l) )
		gt -= l->len;

	if( gt > l->len )	gt = l->len;

	*l0 = l;
	*t0 = gt;
}


void
lnL_gt2lt_p( vec2f_type *ctr, ln_type *link, float gt, vec2f_type *ctr0, ln_type **l0, float *t0 )
{
	ln_type	*l;


	*ctr0 = *ctr;
	if( gt <= 0 ){
		*l0 = link;
		*t0 = 0;
		return;
	}

	for( l = link ; gt > l->len && LN_NEXT(l) != NULL ; l = LN_NEXT(l) ){
		gt -= l->len;
		ctr0->x += l->v.x;
		ctr0->y += l->v.y;
	}

	if( gt > l->len )	gt = l->len;

	*l0 = l;
	*t0 = gt;
}



int
lnL_copy_sub( vec2d *pctr, ln_type *plink, float gt0, float gt1, vec2d *ctr, ln_type **link  )
{
	ln_type	*l0, *l1;
	float		t0, t1;
	ln_type	*l,	*l2;


	lnL_gt2lt( plink, gt0, &l0, &t0 );
	lnL_gt2lt( plink, gt1, &l1, &t1 );



	for( l = plink, *ctr = *pctr ; l != l0 ; l = LN_NEXT(l) ){
		ctr->x += l->v.x;
		ctr->y += l->v.y;
	}

	*link = lnL_copy( l0, LN_NEXT(l1) );



	if( t1 < l1->len - 0.25 ){
		for( l = *link ; LN_NEXT(l) != NULL ; l = LN_NEXT(l) );

		l2 = ln_split( l, t1, 0.25 );

		if( l2 == l )
			l = LN_PREV(l);

		free( l2 );
		l->p[1] = NULL;

	}


	if( t0 > 0.25 ){
		l = ln_split( *link, t0, 0.25 );
		if( l != *link && l != NULL ){
			ctr->x += (*link)->v.x;
			ctr->y += (*link)->v.y;
			free( *link );
			*link = l;
			l->p[0] = NULL;
		}
	}

	return( 1 );
}

void
lnL_connect_s( vec2f_type *ctr, ln_type *link, vec2f_type *ctr1, ln_type *link1, float T )
{
	vec2f_type	p1,	p,	v;
	ln_type	*l,	*el;

	for( l = link, p = *ctr ; LN_NEXT(l) != NULL ; l = LN_NEXT(l) ){
		p.x += l->v.x;
		p.y += l->v.y;
	}

	el = l;

	p1.x = p.x + el->v.x;
	p1.y = p.y + el->v.y;

	v.x = ctr1->x - p1.x;
	v.y = ctr1->y - p1.y;

	if( v.x*v.x + v.y*v.y < T ){
		el->v.x = ctr1->x - p.x;
		el->v.y = ctr1->y - p.y;
		ln_set_aux( el );
		lnL_connect( el, link1 );
		return;
	}

	l = LN_ALLOC();
	l->v = v;
	l->a = 0;
	ln_set_aux( l );

	ln_connect( el, l );
	ln_connect( l, link1 );
}


ln_type *
lnL_inverse( ln_type *link )
{
	ln_type	*l,	*l1;

	while( (l = LN_PREV(link)) != NULL )	link = l;

	l = link;

	while(  l != NULL ){
		ln_inverse( l );

		l1 = l;

		l = LN_PREV(l);
	}

	return( l1 );
}


void
ln_inverse( ln_type *l )
{
ln_type	*p;	

	l->v.x = - l->v.x;
	l->v.y = - l->v.y;

	l->u.x = - l->u.x;
	l->u.y = - l->u.y;

	l->a = -l->a;
	l->c_prb = -l->c_prb;


	p = l->p[0];
	l->p[0] = l->p[1];
	l->p[1] = p;
}






void
lnL_lt2( vec2d *ctr, ln_type *link, lt2_type *lt )
{
ln_type	*l;
vec2f_type	v,	p0,	p1;
float	len;


	v = *ctr;
	p0.x = LT2_F1( *lt, v.x, v.y );
	p0.y = LT2_F2( *lt, v.x, v.y );

	*ctr = p0;

	for( l = link; l != NULL ; l = LN_NEXT(l) ){
		v.x += l->v.x;
		v.y += l->v.y;

		p1.x = LT2_F1( *lt, v.x, v.y );
		p1.y = LT2_F2( *lt, v.x, v.y );

		l->v.x = p1.x - p0.x;
		l->v.y = p1.y - p0.y;

		len = l->len;
		l->len = hypot( l->v.x, l->v.y );
		l->u.x = l->v.y/l->len;
		l->u.y = -l->v.x/l->len;

		l->a *= l->len/len;

		l->c_prb = -4.0 *l->a/( l->len*l->len);


		p0 = p1;
	}
}


void
lnL_at_b( vec2d *ctr, ln_type *link, float a, float b )
{
vec2f_type	v,	u,	end,	p0,	p1;
float	len,	t;
lt2_type	lt;

	lnL_end_point( ctr, link, &end );
	v.x = end.x - ctr->x;
	v.y = end.y - ctr->y;
	t = hypot( v.x, v.y );
	u.x = v.y/t;
	u.y = -v.x/t;

	len = lnL_length( link );

	t = b;
	p0.x = ctr->x + t* u.x;
	p0.y = ctr->y + t* u.y;

	t = a*len + b;
	p1.x = end.x + t* u.x;
	p1.y = end.y + t* u.y;




	lt2_similarity( &lt, ctr, &end, &p0, &p1 );


	lnL_lt2( ctr, link, &lt );
}




int
lnL_from_points( vec2d p[], int no, int Fclose, vec2f_type *ctr, ln_type **lnL )
{
	ln_type	*link,	*l;
	vec2d	v;
	int	i,	j,	nL;

	nL = (Fclose == 1)? no : no-1;

	link = lnL_alloc( nL );


	v.x = p[0].x;
	v.y = p[0].y;


	for( i = 0, j = 1, l = link ; i < nL ; i++, j++, l = LN_NEXT(l) ){
		if( j >= no )	j = 0;
		l->v.x = p[j].x - v.x;
		l->v.y = p[j].y - v.y;

		l->a = 0;

		ln_set_aux( l );

		v.x += l->v.x;
		v.y += l->v.y;
	}


	*ctr = p[0];
	*lnL = link;
	return( 1 );
}