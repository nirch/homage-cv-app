/**************************
 ***    ContourTool.c   ***
 **************************/
#include	<math.h>
#include	<string.h>
#include	"Umath/LT2sType.h"

#include	"../PlnType.h"
#include	"ClnType.h"



cln_type *
cln_alloc()
{
	cln_type *c = (cln_type *)malloc(sizeof(cln_type));

	memset(c, 0, sizeof(cln_type) );

	c->type = 0;

	c->scale = 1.0;
	c->angle = 0.0;

	c->ctr.x = c->ctr.y = 0;

	return( c );
}



void
cln_destroy( cln_type *c )
{
int	i;

	for( i = 0 ; i < c->nA ; i++ )
		pln_destroy( c->a[i] );

	free( c );
}



clnA_type *
	clnA_alloc( int n )
{
	clnA_type *ac = (clnA_type *)malloc(sizeof(clnA_type));

	ac->NA = n;

	ac->a =  (cln_type **)malloc( ac->NA *sizeof(cln_type *));

	ac->nA = 0;

	return( ac );
}



void
clnA_destroy( clnA_type *ac )
{
	int	i;

	for( i = 0 ; i < ac->nA ; i++ )
		cln_destroy( ac->a[i] );


	free( ac->a );

	free( ac );
}



cln_type *
cln_create( pln_type *pl, int Fdata )
{
	cln_type *c;

	c = cln_alloc();

	if( Fdata == 1 )
		pl = pln_copy( pl );

	c->a[c->nA++] = pl;

	return( c );
}





int
cln_add( cln_type *c, pln_type *pl, int Fdata )
{
	if( Fdata == 1 )
		c->a[c->nA++] = pln_copy( pl );
	else	c->a[c->nA++] = pl;

	return( 1 );
}


cln_type *
cln_make_copy( cln_type *sc )
{
cln_type *c;
int	i;

	c = cln_alloc();

	*c = *sc;

	for( i = 0 ; i < c->nA ; i++ )
		c->a[i] = pln_copy( sc->a[i] );

	return( c );
}

cln_type *
cln_make_copyS( cln_type *sc, int state )
{
	cln_type *c;
	int	i;


	c = cln_alloc();

	*c = *sc;

	c->nA = 0;
	for( i = 0 ; i < sc->nA ; i++ ){
		if( state != 0 && (sc->a[i]->state & state ) == 0 )
			continue;

		c->a[c->nA++] = pln_copy( sc->a[i] );
	}

	return( c );
}


cln_type *
cln_detach_plink( cln_type *sc, int iPlink )
{
cln_type *c;
int	i;

	c = cln_alloc();


	c->angle = sc->angle;
	c->ctr = sc->ctr;
	c->scale = sc->scale;
	c->nA = 1;
	c->a[0] = sc->a[iPlink];


	sc->nA--;

	for( i = iPlink ; i < sc->nA ; i++ )
		sc->a[i] = sc->a[i+1];


	return( c );
}



cln_type * 
cln_lt2( cln_type *sc, lt2_type *lt, cln_type *c )
{
	int	i;

	if( c == NULL )
		c = cln_make_copy( sc );


	c->ctr.x = LT2_F1( *lt, sc->ctr.x, sc->ctr.y );
	c->ctr.y = LT2_F2( *lt, sc->ctr.x, sc->ctr.y );


	c->v.x = LT2_F1( *lt, sc->v.x, sc->v.y ) - lt->c0;
	c->v.y = LT2_F2( *lt, sc->v.x, sc->v.y ) - lt->c1;


	for( i = 0 ; i < c->nA ; i++ )
		c->a[i] = pln_affine_lt( sc->a[i], lt, c->a[i]);


	return( c );
}



#ifdef _AA_
void 
cln_affine( cln_type *c, float a, float b, float s, float t )
{
vec2d	ctr;
int	i;

	ctr.x = c->ctr.x *  s + c->ctr.y * t + a;
	ctr.y = c->ctr.x * -t + c->ctr.y * s + b;

	c->ctr.x = ctr.x;
	c->ctr.y = ctr.y;

	c->scale *= hypot( s, t );

	c->angle += atan( t / s ) * 180 / M_PI;


	for( i = 0 ; i < c->nA ; i++ )
		pln_affine( c->a[i], a, b, s, t );

}



void 
cln_affine_lt2( cln_type *c, lt2_type *lt )
{
vec2d	ctr;
int	i;

	ctr.x = LT2_F1( *lt, c->ctr.x, c->ctr.y );
	ctr.y = LT2_F2( *lt, c->ctr.x, c->ctr.y );

	c->ctr.x = ctr.x;
	c->ctr.y = ctr.y;


	for( i = 0 ; i < c->nA ; i++ )
		pln_affine_lt2( c->a[i], lt );

}



void 
cln_affine_estimate( cln_type *c, float a, float b, float s, float t,
									float *dx, float *dy, float *ds, float *da )
{
float	x,	y;
	x = c->ctr.x *  s + c->ctr.y * t + a;
	y = c->ctr.x * -t + c->ctr.y * s + b;

	*dx = x - c->ctr.x;
	*dy = y -  c->ctr.y ;


	*ds = 1.0 - hypot( s, t );
	*da = s * 0 + t * 1;

	*da = atan( t / s ) * 180 / M_PI;
}


void
cln_set_ctr( cln_type *c )
{
int	i;

	for( i = 0 ; i < c->nA ; i++ )
		polylink_set_ctr( &c->a[i]->ctr, c->a[i]->link );
}

void
cln_normelize( cln_type *c )
{
vec2d	p0;
float	scale;


	c->a[0]->ctr.x -= c->ctr.x;
	c->a[0]->ctr.y -= c->ctr.y;


	p0.x = p0.y = 0;
	scale = 1.0/c->scale;
	polylink_rescale( c->a[0]->link, &c->a[0]->ctr, &p0, scale, scale);
}


void
cln_unnormelize( cln_type *c )
{
vec2d	p0;

	p0.x = p0.y = 0;
	polylink_rescale( c->a[0]->link, &c->a[0]->ctr, &p0, c->scale, c->scale );


	c->a[0]->ctr.x += c->ctr.x;
	c->a[0]->ctr.y += c->ctr.y;

	polylink_set_ctr( &c->a[0]->ctr, c->a[0]->link );
}
#endif

void
cln_box( cln_type *c, box2f_type *box )
{
box2f_type	b;
int	i;

	pln_box( c->a[0], box );


	for( i = 1 ; i < c->nA; i++ ){
		pln_box( c->a[i], &b );
	
		box2f_update( box, &b );
	}
}


void
cln_center( cln_type *c, vec2d *p )
{
	box2f_type	b;

	cln_box( c, &b );

	p->x = 0.5* ( b.x1 + b.x0 );
	p->y = 0.5* ( b.y1 + b.y0 );
}


void
clnA_translate( clnA_type *ac, float x, float y )
{
	int	i;

	for( i = 0 ; i < ac->nA ; i++ )
		cln_translate( ac->a[i], x, y );
}


void
cln_translate( cln_type *c, float x, float y )
{
	int	i;

	c->ctr.x += x;
	c->ctr.y += y;

	for( i = 0 ; i < c->nA ; i++ ){
		pln_translate( c->a[i], x, y );
	}

}




#ifdef _AA_
void
cln_link_len( cln_type *c, float len )
{
int	i;


	for( i = 0 ; i < c->nA ; i++ ){
		c->a[0]->link = polylink_max_len( c->a[0]->link, len );
	}
}


void
clnA_link_len( clnA_type *ac, float len )
{
	cln_type	*c;
	int	i;


	for( i = 0 ; i < ac->nC ; i++ ){
		c = ac->c[i];

		cln_link_len( c, len );
	}
}

		




void
clnA_clear_anchor( clnA_type *ac )
{
	int	i;

	for( i = 0 ; i < ac->nC ; i++ )
		cln_clear_anchor( ac->c[i] );
}


void
cln_clear_anchor( cln_type *c )
{
int	i;

	for( i = 0 ; i < c->nA ; i++ )
		pln_clear_anchor( c->a[i] );
}


void
cln_simplify( cln_type *c )
{
int	i;

	for( i = 0 ; i < c->nA ; i++ )
		pln_simplify( c->a[i] );
}

void
cln_simlifay_len( cln_type *c, float minLen, float maxLen )
{
	int	i;

	for( i = 0 ; i < c->nA ; i++ )
		pln_simplify_len( c->a[i], minLen, maxLen );
}




void	
cln_set_dirrection( cln_type *c )
{
int	i;

	cln_set_range( c );

	for( i = 0 ; i < c->nA ; i++ ) 
		if ( pln_check_dirrection( c->a[i] ) == 0 )
			pln_change_dirrection( c->a[i] );
}


void	
cln_set_range( cln_type *c )
{
int	i, j;

	for( i = 0 ; i < c->nA ; i++ ) {
		c->a[i]->range = 0;

		for( j = 0 ; j < c->nA ; j++ ) {
			if ( i == j )	continue;

			if ( pln_inside_plink( c->a[i], c->a[j] ) )
				c->a[i]->range++;
		}
	}
}


int		
cln_check_cross( cln_type *c, float d_min, vec2f_type *mp )
{
int	i, j;
int rez;

	for( i = 0; i < c->nA ; i++ ) {

		for( j = i; j < c->nA ; j++ ) {
			if ( i == j )	continue;

			if ( ( rez = pln_check_cross( c->a[i], c->a[j], d_min, mp ) ) != 0 )
				return rez;
		}
	}

	return 0;
}
#endif


void	
cln_pln_destroy( cln_type *c, pln_type *pl )
{
int	i, j;


	for( i = 0; i < c->nA ; i++ )  {
		if ( c->a[i] != pl ) continue;

		for( j = i+1; j < c->nA; j++ ) 
			c->a[i] = c->a[j];
		c->a[c->nA] = NULL;
		c->nA--;

		pln_destroy( pl );

		return;
	}
}





cln_type *
cln_union( cln_type *ac, cln_type *c )
{
	int	i;

	if( ac == NULL )
		return( c );

	for( i = 0 ; i < c->nA ; i++ )
		ac->a[ac->nA++] = c->a[i];

	free( c );

	return( ac );
}


void
cln_inverse( cln_type *c )
{
	int	i;

	for( i = 0 ; i < c->nA ; i++ ){

		pln_inverse( c->a[i] );

	}
}




float
cln_distance( cln_type *c, vec2d  *p, dPln_type *d )
{
int	i;
dPln_type cd;
float	dis,	mDis;


	mDis = pln_distance(  c->a[0], p, d );

	for( i = 1 ; i < c->nA ; i++ ){
	

		dis = pln_distance(  c->a[i], p, &cd );
		if( dis < mDis ){
			*d = cd;
			mDis = dis;
		}

	}
	return( mDis );
}


cln_type *
cln_detach_transparent( cln_type *c )
{
	cln_type *ct;
	int	i,	j;

	ct = cln_alloc();

	ct->angle= c->angle;
	ct->ctr = c->ctr;
	ct->scale = c->scale;
	ct->nA = 0;

	for( i = 0, j = 0 ; i < c->nA ; i++ ){
		if( c->a[i]->state & PL_TRANSPARENT )
			ct->a[ct->nA++] = c->a[i];
		else
			c->a[j++] = c->a[i];
	}

	c->nA = j;

	if( ct->nA > 0  )
		return( ct );

	cln_destroy( ct );
	return( NULL );
}


void
cln_axis( cln_type *c )
{
box2f_type	b;


	cln_box( c, &b );

	c->ctr.x = 0.5*(b.x0 + b.x1);
	c->ctr.y = 0.5*(b.y0 + b.y1);
	c->angle = 0;

	c->v.x = 20*cos( c->angle );
	c->v.y = 20*sin( c->angle );
}



void
cln_set_dirrection( cln_type *c )
{
	int	i;

	cln_set_range( c );

	for( i = 0 ; i < c->nA ; i++ ) {
		pln_type *pl = c->a[i];
		int dir = pln_interior_side( pl );

		if(  pl->step & 0x01 )
			dir = 1 - dir;

		if( dir == 0 )
			pln_inverse( pl );
	}
}


void	
cln_set_range( cln_type *c )
{
	int	i, j;

	for( i = 0 ; i < c->nA ; i++ ) {
		c->a[i]->step = 0;

		for( j = 0 ; j < c->nA ; j++ ) {
			if ( i == j )	continue;

			if ( pln_inside( c->a[i], c->a[j] ) )
				c->a[i]->step++;
		}
	}
}


int	
cln_point_inside( cln_type *c, vec2f_type *p )
{
	int	i;

	for( i = 0 ; i < c->nA ; i++ ) 
		if( pln_point_side( c->a[i], p ) == 0 )
			return( -1 );

	return( 1 );
}