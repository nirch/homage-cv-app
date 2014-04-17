/********************
 ***   LkType.h   ***
 ********************/

#include	<stdio.h>   
#include	<math.h>
#include	<stdlib.h>


#include	"Uigp/igp.h"


#include	"LnType.h"


ln_type *	
ln_alloc()
{
	ln_type	*link;

	link = LN_ALLOC();
	link->p[0] = NULL;
	link->p[1] = NULL;

	return( link );
}



ln_type *	
lnL_alloc( int ino )
{
ln_type	*link,	*l,	*pl;
int	i;


	link = LN_ALLOC();
	link->p[0] = NULL;

	for( i = 1, pl = link ; i < ino ; i++ ){
		l = LN_ALLOC();
		l->p[0] = pl;
		pl->p[1] = l;
		pl = l;
	}

	pl->p[1] = NULL;


	return( link );
}

//void
//ln_from_gapp( gapp_type *gapp, vec2f_type *ctr, ln_type **link )
//{
//ln_type	*l,	*pl;
//vec2f_type	p,	v;
//int	i,	iseg,	cseg;
//  
//	*ctr = gapp->v[0];
//
//
//	p = *ctr;
//	
////	pl = NULL;
////	for( i = 0, g = link, cseg = iseg = 0 ; i < gapp->ino; i++ ){
//	for( i = 0, pl = NULL, cseg = iseg = 0 ; i < gapp->ino; i++ ){
//		iseg = gapp_next( gapp, iseg, GEOM_DIVISION);
//
//		v = gapp->v[iseg];
//
//
////		l = *g = LN_ALLOC();
////		g = &l->p[F_END];
//
//		l = LN_ALLOC();
//		l->p[0] = pl;
//
//		if( pl != NULL )
//			pl->p[1] = l;
//		else	*link = l;
//
//		pl = l;
//
//		l->v.x = v.x - p.x;
//		l->v.y = v.y - p.y;
//
//		p.x += l->v.x;
//		p.y += l->v.y;
//
//
//		/* set AUX */
//		if( l->v.x == 0.0 && l->v.y == 0 ){
//			l->len = l->u.x = l->u.y = l->c_prb = l->a = 0.0;
//			continue;
//		}
//
//
//		l->len = hypot( l->v.x, l->v.y );
// 
//		l->u.x = l->v.y/l->len;
//		l->u.y = -l->v.x/l->len;
//
//		l->c_prb = gapp->a[iseg];
//		l->a = - l->c_prb * l->len * l->len * 0.25;
//
//
//	}
//
////	*g = NULL;
//}
//


void
ln_destroy( ln_type *ln )
{
	free( ln );
}


void
lnL_destroy( ln_type *ln )
{
ln_type	*l,	*l1;

	for( l = ln ; l != NULL ; ){
		l1 = l;
		l = l->p[F_END];
		free( l1 );
	}
}




ln_type *
ln_create( vec2f_type *p0, vec2f_type *p1, float crbA )
{
ln_type	*l;

	l = LN_ALLOC();
	l->p[0] = l->p[1] = NULL;

	l->v.x = p1->x - p0->x;
	l->v.y = p1->y - p0->y;


	/* set AUX */
	if( l->v.x == 0.0 && l->v.y == 0 ){
		l->len = l->u.x = l->u.y = l->c_prb = l->a = 0.0;
		free( l );
		return( NULL );
	}



	l->len = hypot( l->v.x, l->v.y );

	l->u.x = l->v.y/l->len;
	l->u.y = -l->v.x/l->len;


	l->c_prb = crbA;
	l->a = - l->c_prb * l->len * l->len * 0.25;

	return( l );
}


void 
ln_set_aux( ln_type *l ) 
{ 
	double	len;

	if( l->v.x == 0.0 && l->v.y == 0 ){ 
		l->len = l->u.x = l->u.y = l->c_prb = l->a = 0.0; 
		return;  
	}

	len = hypot( l->v.x, l->v.y );
	l->len = len;
	l->u.x = l->v.y/len;
	l->u.y = -l->v.x/len;

	l->c_prb = -4.0 *l->a/( l->len*l->len); 
} 


void
ln_t2xy( vec2f_type *vm, ln_type *link, float t, vec2f_type *p )
{
	float	h;

	h = link->c_prb * t * (t - link->len);

	p->x = vm->x  +  t * -link->u.y  +  h * link->u.x;
	p->y = vm->y  +  t *  link->u.x  +  h * link->u.y;
}


void
ln_tanget( ln_type *l, float t, vec2f_type *tanget )
{
	float	tmp;

	tmp = l->c_prb * ( 2*t - l->len );

	tanget->x = -l->u.y + l->u.x* tmp;
	tanget->y =  l->u.x + l->u.y* tmp;

	tmp = 1.0/hypot( tanget->x, tanget->y );
	tanget->x *= tmp;
	tanget->y *= tmp;
}

void
ln_tanget_beg( ln_type *l, vec2f_type *t )
{
	float	tmp = l->c_prb* l->len;
	vec2f_type	v;

	v.x = -l->u.y - l->u.x* tmp;
	v.y =  l->u.x - l->u.y* tmp;

	tmp = 1.0/hypot( v.x, v.y );

	t->x = v.x*tmp;
	t->y = v.y*tmp;
}

void
ln_tanget_end( ln_type *l, vec2f_type *t )
{
	float	tmp = l->c_prb* l->len;
	vec2f_type	v;

	v.x = -l->u.y + l->u.x* tmp;
	v.y =  l->u.x + l->u.y* tmp;

	tmp = 1.0/hypot( v.x, v.y );

	t->x = v.x*tmp;
	t->y = v.y*tmp;
}

void
ln_Ntanget( ln_type *l, float t, vec2f_type *nt )
{
	vec2f_type	tanget;
	float	tmp;

	tmp = l->c_prb * ( 2*t - l->len );

	tanget.x = -l->u.y + l->u.x* tmp;
	tanget.y =  l->u.x + l->u.y* tmp;

	tmp = 1.0/hypot( tanget.x, tanget.y );

	nt->x =  tanget.y * tmp;
	nt->y = -tanget.x * tmp;
}

void
ln_Ntanget_beg( ln_type *l, vec2f_type *t )
{
	float	tmp = l->c_prb* l->len;
	vec2f_type	v;

	v.x = -l->u.y - l->u.x* tmp;
	v.y =  l->u.x - l->u.y* tmp;

	tmp = 1.0/hypot( v.x, v.y );

	t->x = v.y*tmp;
	t->y = -v.x*tmp;
}

void
ln_Ntanget_end( ln_type *l, vec2f_type *t )
{
	float	tmp = l->c_prb* l->len;
	vec2f_type	v;

	v.x = -l->u.y + l->u.x* tmp;
	v.y =  l->u.x + l->u.y* tmp;

	tmp = 1.0/hypot( v.x, v.y );

	t->x = v.y*tmp;
	t->y = -v.x*tmp;
}

// point with tanget vector ( 1, 0 )
int
ln_tanget_point_10( ln_type *l, vec2d *vm, float *t, vec2d *p )
{
	float	tmp;

	if( l->c_prb == 0.0 )	return(0);

	tmp = l->c_prb * l->u.y;

	if( ABS( l->u.x ) > ABS( tmp )*l->len )	return( 0 );

	*t = 0.5*( -l->u.x / tmp + l->len);
	ln_t2xy( vm, l, *t, p );
	return( 1 );
}



// point with tanget vector ( 0, 1 )
int
ln_tanget_point_01( ln_type *l, vec2d *vm, float *t, vec2d *p )
{
	float	tmp;

	if( l->c_prb == 0.0 )	return(0);

	tmp = l->c_prb * l->u.x;

	if( ABS( l->u.y ) > ABS( tmp )*l->len )	return( 0 );

	*t = 0.5*( +l->u.y / tmp + l->len);
	ln_t2xy( vm, l, *t, p );
	return( 1 );
}


float
ln_distance( vec2d *ctr, ln_type *l, vec2d *v, float *u, float *t )
{
	vec2d	dv;
	float	dt,	cu;


	dv.x = v->x - ctr->x;
	dv.y = v->y - ctr->y;

	*t = -dv.x *l->u.y + dv.y *l->u.x;

	*u = l->u.x * dv.x + l->u.y * dv.y;

	if( *t < 0 ){
		cu = hypot( *u, -1.2*(*t) );

		if( *u < 0 )	*u = -cu;
		else	*u = cu;

		return( cu );
	}

	if( (dt = *t - l->len) > 0 ){
		cu = hypot( *u, 1.2*dt );

		if( *u < 0 )	*u = -cu;
		else	*u = cu;

		return( cu );
	}

	*u -= l->c_prb*(*t) *((*t) - l->len );

	return( *u );
}

int
ln_distanceG( vec2f_type *ctr, ln_type  *link, ln_type *elink, vec2f_type  *v, float *gt )
{
ln_type	*sl;
float	t,	u;
int	ret;

	ret = lnL_distance( ctr, link,  elink, v, &sl, &u, &t, gt );

	return( ret );
}


int
lnL_distance( vec2f_type *ctr, ln_type  *link, ln_type *elink, vec2f_type  *v,
			   ln_type **sl, float *u, float *t, float *gt )
{
ln_type	*l;
int	i,	ret;
vec2f_type	cv;
float	cu,	ct,	dt;
float	dis,	tmp,	len;


	*sl = NULL;
	*u = 100;
	*t = 100;
	ret = -1;

	dis = 100000;

	cv.x = v->x - ctr->x;
	cv.y = v->y - ctr->y;

	*gt = 0;
	for( l = link, i = 0, len = 0 ; l != elink ; len += l->len, l = l->p[F_END], i++ ){

		cu = l->u.x * cv.x + l->u.y * cv.y;

		ct = -l->u.y * cv.x + l->u.x * cv.y;

		cv.x -= l->v.x;
		cv.y -= l->v.y;



		if( ct < 0 ){
			tmp = cu*cu + 1.44*ct*ct;

			if( cu > 0 )	cu -= ct;
			else	cu += ct;

//			ct = 0;
		}
		else	if( (dt = ct - l->len) > 0 ){

			tmp = cu*cu + 1.44*dt*dt;

			if( cu > 0 )	cu += dt;
			else	cu -= dt;
//			ct = l->len;
		}
		else	{
			cu -= l->c_prb*ct *(ct - l->len );
			tmp = cu*cu;
		}


		if( tmp >= dis )	continue;

		dis = tmp;


		*u = cu;
		*t = ct;
		*sl = l;
		*gt = len;// + ct;
		ret = i;
	}

	if( *sl == NULL )	return( -1 );

	if( *t < 0 ){
		if( *sl == link )	*gt = *t;
	}
	else	if( *t > (*sl)->len )
		*gt += ( LN_NEXT((*sl)) == elink )? *t : (*sl)->len;
	else	*gt += *t;


	return( ret );
}

float
lnL_length( ln_type *link )
{
ln_type	*l;
float len;

	if( link == NULL )	return( 0 );

	for( l = link, len = 0 ; l != NULL ; l = l->p[F_END] )
		len += l->len;

	return( len );
}


int
lnL_no( ln_type *link )
{
ln_type	*l;
int	n;


	if( link == NULL )	return( 0 );

	for( l = link, n = 0 ; l != NULL ; l = l->p[F_END] )
		n++;

	return( n );
}




void
ln_connect( ln_type *l, ln_type *nl )
{
	l->p[1] = nl;
	nl->p[0] = l;
}
