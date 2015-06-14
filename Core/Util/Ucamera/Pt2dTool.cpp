/***********************
 ***	Pt2dTool.c   ***
 ***********************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Umath/Matrix2Type.h"
#include "Umath/EigenType.h"
#include	"Ubox/Box2d.h"
#include	"Pt2dType.h"
#ifdef _WIN32
#define _GPMEMORY_LEAK 
#endif
#include "Uigp/GpMemoryLeak.h"


pt2dA_type *
pt2dA_alloc( int n )
{
pt2dA_type	*apt;
int	i;

	apt = (pt2dA_type *)malloc( sizeof( pt2dA_type) );

	apt->p = ( pt2d_type *)malloc( n*sizeof(pt2d_type) );

	apt->type = PT2D_2;

	apt->axis = PT2D_AXIS_XY;

	apt->NP = n;

	apt->nP = 0;

	apt->gpt = NULL;

	for( i = 0 ; i < apt->NP ; i++ ){
		apt->p[i].r = 0;
		apt->p[i].group = 0;
	}

	apt->box.x0 = apt->box.x1 = 0;
	apt->box.y0 = apt->box.y1 = 0;


	GPMEMORY_LEAK_ALLOC( apt );

	return( apt );
}


pt2dA_type *
pt2dA_realloc( pt2dA_type*apt, int n )
{
	if( apt == NULL ){
		apt = pt2dA_alloc(n );
		return( apt );
	}


	if( apt->NP < n ){
		apt->p = ( pt2d_type *)realloc( apt->p, n*sizeof(pt2d_type) );
		apt->NP = n;
	}

//	apt->type = PT2D_2;

//	apt->nP = 0;

	return( apt );
}



void
pt2dA_destroy( pt2dA_type *apt )
{
	if( apt->nP > apt->NP )
		fprintf( stderr, "pt2dA_destroy:  apt->nP >= apt->NP");


	GPMEMORY_LEAK_DESTROY( apt );

	free( apt->p );

	free( apt );
}




pt2dA_type *
pt2dA_set( pt2dA_type *apt, vec2fA_type *av )
{
pt2d_type	*pt;
int	i;

	if( apt == NULL )
		apt = pt2dA_alloc( av->nA );

	for( i = 0 ; i < av->nA ; i++ ){
		pt = &apt->p[i];
		pt->p = av->a[i];
		pt->r = 0;
		pt->group = 0;
	}

	apt->nP = i;

	return( apt );
}


#ifdef _AA_
void	
pt2dA_dump( pt2dA_type *apt, char *prefix, int index, char *suffix )
{
char	file[256];

	if( gpDump_filename( prefix, index, suffix, ".pt", file ) < 0 )
		return;


	pt2dA_write_to_file( apt, file );
}
#endif

pt2dA_type *
pt2dA_copy( pt2dA_type *apt, pt2dA_type *capt )
{
int	i;

	capt = pt2dA_realloc( capt, apt->nP );
	capt->type = apt->type;

	for( i = 0 ; i < apt->nP ; i++ ){
		capt->p[ i ] = apt->p[i];
	}

	capt->nP = i;

	return( capt );
}

pt2dA_type *
pt2dA_copy_box( pt2dA_type *apt, box2i *box, pt2dA_type *capt )
{
	int	i,	j;

	capt = pt2dA_realloc( capt, apt->nP );
	capt->type = apt->type;

	for( i = 0, j = 0 ; i < apt->nP ; i++ ){
		pt2d_type	*pt = &apt->p[i];

		if( BOX2D_IS_IN( *box, pt->p.x, pt->p.y ) )
			capt->p[ j++ ] = apt->p[i];
	}

	capt->nP = j;

	return( capt );
}

pt2dA_type *
pt2dA_from_point( vec2f_type p[], int nP )
{
int	i;

	pt2dA_type	*apt = pt2dA_alloc( nP );

	for( i = 0 ; i < nP ; i++ )
		apt->p[i].p = p[i];


	apt->nP = nP;
	apt->axis = PT2D_AXIS_XY;

	return( apt );
}



pt2dA_type *
pt2dA_copy_group( pt2dA_type *apt, int group, int no )
{
	pt2dA_type *gpt;
	int	i;

	if( no <= 0 )
		no = apt->nP;

	gpt = pt2dA_alloc( no );

	for( i = 0 ; i < apt->nP ; i++ ){
		if( apt->p[i].group != group )	continue;

		gpt->p[ gpt->nP++ ] = apt->p[i];
	}

	return( gpt);
}


void
pt2dA_nGroup( pt2dA_type *apt )
{
	int	i;
	int64_t	mask;
	int	nGroup;

	mask = 0;
	nGroup = 0;

	for( i = 0; i < apt->nP; i++ ){
		pt2d_type	*pt = &apt->p[i];

		if( mask & ( ((int64_t)1)<< pt->group)  )
			continue;

		mask |=  ((int64_t)1)<< pt->group;
		nGroup++;
	}

	apt->nGroup = nGroup;
}



void
pt2dA_set_default_group( pt2dA_type *apt )
{
int	i;

	for(  i = 0 ; i < apt->nP ; i++ )
		apt->p[i].group = i;
}


int
pt2dA_select( pt2dA_type *apt, vec2d *p, float dis )
{
pt2d_type	*pt;
vec2d	v;
float	dMin,	d;
int	i,	iMin;

	dMin = dis*dis;
	iMin = -1;
	for( i = 0 ; i < apt->nP ; i++ ){
		pt = & apt->p[i];
		v.x = pt->p.x - p->x;
		v.y = pt->p.y - p->y;

		d = v.x*v.x + v.y*v.y;
		if( d < dMin ){
			dMin = d;
			iMin = i;
		}
	}

	return( iMin );
}


int
pt2dA_distance( pt2dA_type *apt, int ignore, vec2d *p, float *dis )
{
	pt2d_type	*pt;
	vec2d	v;
	float	dMin,	d;
	int	i,	iMin;

	
	iMin = -1;
	for( i = 0 ; i < apt->nP ; i++ ){
		if( i == ignore )	continue;

		pt = & apt->p[i];
		v.x = pt->p.x - p->x;
		v.y = pt->p.y - p->y;

		d = v.x*v.x + v.y*v.y;
		if( iMin < 0 || d < dMin ){
			dMin = d;
			iMin = i;
		}
	}

	*dis = dMin;
	return( iMin );
}



pt2dA_type * 
pt2dA_copy_neighbor( pt2dA_type *apt, vec2d *p, float D, pt2dA_type *capt )
{
	vec2d	v;
	float	D2,	d;
	int	i;

	if( apt->gpt != NULL ){
		capt = pt2dG_copy_neighbor( apt, p, D, capt );
		return( capt );
	}



	if( capt == NULL )
		capt = pt2dA_alloc( apt->nP );

	capt->type = apt->type;

	D2 = D*D;
	capt->nP = 0;
	for( i = 0 ; i < apt->nP ; i++ ){
		pt2d_type *pt = &apt->p[i];

		v.x = pt->p.x - p->x;
		v.y = pt->p.y - p->y;

		d = v.x*v.x + v.y *v.y;

		if( d == 0 )	continue;
		if( d > D2 )	continue;

//		pt->id = i;
		capt->p[capt->nP] =  *pt;
		capt->p[capt->nP].id = i;
		capt->nP++;

	}

	return( capt );
}


pt2dA_type * 
pt2dA_copy_neighbor_id( pt2dA_type *apt, vec2d *p, float D, int mId, pt2dA_type *capt )
{
	vec2d	v;
	float	D2,	d;
	int	i;
	if( capt == NULL )
		capt = pt2dA_alloc( apt->nP );

	capt->type = apt->type;

	D2 = D*D;
	capt->nP = 0;
	for( i = 0 ; i < apt->nP ; i++ ){
		pt2d_type *pt = &apt->p[i];

		if( (pt->id & mId) == 0 )
			continue;

		v.x = pt->p.x - p->x;
		v.y = pt->p.y - p->y;

		d = v.x*v.x + v.y *v.y;

		if( d == 0 )	continue;
		if( d > D2 )	continue;

		//		pt->id = i;
		capt->p[capt->nP] =  *pt;
		capt->p[capt->nP].id = i;
		capt->nP++;

	}

	return( capt );
}


int 
pt2dA_neighbor( pt2dA_type *apt, vec2d *p, float D )
{
vec2d	v;
float	D2,	d;
int	i,	no;

	D2 = D*D;

	for( i = 0, no = 0 ; i < apt->nP ; i++ ){
		pt2d_type *pt = &apt->p[i];

		v.x = pt->p.x - p->x;
		v.y = pt->p.y - p->y;

		d = v.x*v.x + v.y *v.y;

		if( d < D2 )
			no++;
	}

	return( no );
}

int 
pt2dA_neighbor_remove( pt2dA_type *apt, vec2d *p, float D )
{
vec2d	v;
float	D2,	d;
int	i,	j,	no;

	D2 = D*D;

	for( i = 0, j = 0 ; i < apt->nP ; i++ ){
		pt2d_type *pt = &apt->p[i];

		v.x = pt->p.x - p->x;
		v.y = pt->p.y - p->y;

		d = v.x*v.x + v.y *v.y;

		if( d >= D2 )
			apt->p[j++] = apt->p[i];
		
	}

	no = apt->nP - j;
	apt->nP = j;

	return( no );
}


void pt2dA_remove( pt2dA_type *apt, int k )
{
	int	i;

	apt->nA--;

	for( i = k ; i < apt->nA ; i++ ){
		apt->a[i] = apt->a[i+1];
	}
}

void pt2dA_remove_box( pt2dA_type *apt, box2i *box )
{
	int	i,	j;


	for( i = 0, j = 0 ; i < apt->nP ; i++ ){
		pt2d_type *pt = &apt->p[i];

		if( BOX2D_IS_IN( *box, pt->p.x, pt->p.y ) )
			continue;

		apt->p[j++] = apt->p[i];
	}

	apt->nP = j;
}


void pt2dA_remove_outbox( pt2dA_type *apt, box2i *box )
{
	int	i,	j;


	for( i = 0, j = 0 ; i < apt->nP ; i++ ){
		pt2d_type *pt = &apt->p[i];

		if( BOX2D_IS_IN( *box, pt->p.x, pt->p.y ) )
			apt->p[j++] = apt->p[i];
	}

	apt->nP = j;
}


void pt2dA_remove_group( pt2dA_type *apt, int group )
{
	int	i,	j;


	for( i = 0, j = 0 ; i < apt->nP ; i++ ){
		pt2d_type *pt = &apt->p[i];

		if( pt->group == group )
			continue;

		apt->p[j++] = apt->p[i];
	}

	apt->nP = j;
}


void
pt2dA_box( pt2dA_type *apt )
{
int	i;

	BOX2D_INIT( apt->box, apt->p[0].p.x, apt->p[0].p.y );

	for( i = 0 ; i < apt->nP ; i++ ){
		pt2d_type *pt = &apt->p[i];
		BOX2D_UPDATE( apt->box, pt->p.x, pt->p.y );
	}
}







int
pt2dA_no( pt2dA_type *apt, box2f_type *b )
{
int	i,	n;


	for( i = 0, n = 0 ; i < apt->nP ; i++ ){
		pt2d_type *pt = &apt->p[i];

		if( pt->p.x < b->x0 || pt->p.x > b->x1 || pt->p.y < b->y0 || pt->p.y > b->y1 )
			continue;

		n++;

	}


	return( n );
}



int
pt2dA_nCircle( pt2dA_type *apt, vec2f_type *p, float r )
{
int	i,	n;
vec2f_type	v;
float	r2;

	r2 = r*r;

	for( i = 0, n = 0 ; i < apt->nP ; i++ ){
		pt2d_type *pt = &apt->p[i];


		v.x = pt->p.x - p->x;
		v.y = pt->p.y - p->y;

		if( v.x*v.x + v.y*v.y > r2 )
			continue;

		n++;

	}


	return( n );
}


void
pt2dA_translate( pt2dA_type *apt, float x, float y )
{
pt2d_type	*pt;
int	i;


	for( i = 0 ; i < apt->nP ; i++ ){
		pt = & apt->p[i];
		pt->p.x += x;
		pt->p.y += y;
	}
}


void
pt2dA_scale( pt2dA_type *apt, float scale )
{
	pt2d_type	*pt;
	int	i;


	for( i = 0 ; i < apt->nP ; i++ ){
		pt = & apt->p[i];
		pt->p.x *= scale;
		pt->p.y *= scale;
	}


}

void
pt2dA_set_r( pt2dA_type *apt, float r )
{
	pt2d_type	*pt;
	int	i;


	for( i = 0 ; i < apt->nP ; i++ ){
		pt = & apt->p[i];
		pt->group = r;
	}
}


void
pt2dA_set_groupId( pt2dA_type *apt, int iGroup )
{
	pt2d_type	*pt;
	int	i;


	for( i = 0 ; i < apt->nP ; i++ ){
		pt = & apt->p[i];
		pt->group = iGroup;
	}
}


void
pt2d_tanget_line( pt2d_type *pt0, pt2d_type *pt1, vec2d *p0, vec2d *p1, vec2d *v )
{
	float	L,	L1,	L2;
	vec2f_type	V,	u;
	float	cosa,	sina;

	V.x = pt1->p.x - pt0->p.x;
	V.y = pt1->p.y - pt0->p.y;

	L = hypot( V.x, V.y );
	V.x /= L;
	V.y /= L;


	L1 = pt1->r - pt0->r;
	//	if( L1 < 0 )	L1 = -L1;

	L2 = sqrt( L*L - L1*L1 );

	sina = L1/L;
	cosa = L2/L;

	v->x =  cosa * V.x + sina * V.y;
	v->y = -sina * V.x + cosa * V.y;


	u.x = -v->y;
	u.y =  v->x;

	p0->x = pt0->p.x - pt0->r * u.x;
	p0->y = pt0->p.y - pt0->r * u.y;

	p1->x = pt1->p.x - pt1->r * u.x;
	p1->y = pt1->p.y - pt1->r * u.y;

}



static void	pt2dA_moment_test( pt2dA_type *apt, vec2f_type *p, vec2f_type *v );


void
pt2dA_moment( pt2dA_type *apt, vec2f_type *p, vec2f_type *v, float *e1, float *e2 )
{
pt2d_type	*pt;
matrix2_type	m;
int	i,	n;
float	t;
vec2d_type	sp;
float	x0,	y0;
	
	x0 = apt->p[0].p.x;
	y0 = apt->p[0].p.y;

	for( i = 0, n = 0 ; i < apt->nP ; i++ ){
		pt = &apt->p[i];

		pt->p.x -= x0;
		pt->p.y -= y0;
	}


	sp.x = sp.y = 0;
	m.a00 = m.a01 = m.a10 = m.a11 = 0;
	for( i = 0, n = 0 ; i < apt->nP ; i++ ){
		pt = &apt->p[i];

		sp.x += pt->p.x;
		sp.y += pt->p.y;

		m.a00 += pt->p.x * pt->p.x;
		m.a01 += pt->p.x * pt->p.y;
		m.a11 += pt->p.y * pt->p.y;
		n++;
	}


	p->x = sp.x /n;
	p->y = sp.y /n;


	//m.a00 /= n;
	//m.a01 /= n;
	//m.a11 /= n;
	//m.a10 = m.a01;

	m.a00 = m.a00 / n - p->x*p->x;
	m.a01 = m.a01 / n - p->x*p->y;
	m.a11 = m.a11 / n - p->y*p->y;
	m.a10 = m.a01;


	matrix2S_eigen( &m, e1, v, e2 );

	t = p->x * v->x + p->y *v->y;
	*e1 -= t*t;

	t = p->x * v->y + p->y *-v->x;
	*e2 -= t*t;

	pt2dA_moment_test( apt, p, v );
}


static void
pt2dA_moment_test( pt2dA_type *apt, vec2f_type *p, vec2f_type *v )
{
pt2d_type	*pt;
int	i,	n;
float	sx,	sx2;
float	sy,	sy2;
float	x,	y,	t;
float	mx,	my;
	sx = sx2 = 0;
	sy = sy2 = 0;


	x = apt->p[20].p.x - apt->p[5].p.x;
	y = apt->p[20].p.y - apt->p[5].p.y;
	t = hypot( x, y );
	x /= t;
	y /= t;


	mx = my = 0;
	for( i = 0, n = 0 ; i < apt->nP ; i++ ){
		pt = &apt->p[i];

		x = pt->p.x - p->x;
		y = pt->p.y - p->y;

		t = x * v->x + y *v->y;
		sx += t;
		sx2 += t*t;

		if( t > mx )	mx = t;


		t = x * -v->y + y *v->x;
		sx += t;
		sy2 += t*t;
		if( t > my )	my = t;
		n++;
	}

	sx /= n;
	sx2 /= n;

//	t = p->x * v->x + p->y *v->y;
//	sx2 -= t*t;

	sy /= n;
	sy2 /= n;

	sy2 -= p->y*p->y;
}


int
pt2dA_eigen2d( pt2dA_type *apt, int i0, int i1,  eigen2d_type *e )
{
	int	i,	n;
	vec2d_type	s;

	matrix2_type	m;
	matrix2_zero( &m );

	s.x = s.y = 0;
	

	for( i = i0, n = 0 ; i != i1 ; i++ ){
		if( i >= apt->nA )	i = 0;

		pt2d_type *pt = &apt->a[i];
		s.x +=  pt->p.x;
		s.y +=  pt->p.y;

		m.a00 += pt->p.x* pt->p.x;
		m.a01 += pt->p.x* pt->p.y;
		m.a11 += pt->p.y* pt->p.y;
		n++;

		if( i == i1 )	break;
	}

	if( n == 0 )
		return( -1 );

	s.x /= n;
	s.y /= n;

	m.a00 = m.a00 / n - s.x*s.x;
	m.a01 = m.a01 / n - s.x*s.y;
	m.a11 = m.a11 / n - s.y*s.y;
	m.a10 = m.a01;


	e->p.x = s.x;
	e->p.y = s.y;




	matrix2S_eigen( &m, &e->e1, &e->v1, &e->e2 );


	e->r = M_PI*4*sqrt( e->e1*e->e2);


#ifdef _TEST_
	matrix2_type	m3;
	matrix2S_eigen_inv( &m3, e->e1, &e->v1, e->e2 );
#endif

	return( n );

}






pt2dA_type	*
pt2dA_append( pt2dA_type *apt, pt2dA_type *sapt )
{
int	i;

	if( apt == NULL )
		return( sapt );


	if( apt->NP < apt->nP + sapt->nP ){
		apt->NP = apt->nP + sapt->nP;
		apt->p = (pt2d_type *)realloc( apt->p, apt->NP*sizeof(pt2d_type) );
	}

	for( i = 0 ; i < sapt->nP ; i++ )
		apt->p[apt->nP++] = sapt->p[i];


	pt2dA_destroy( sapt );

	return( apt );
}


void
pt2dA_axis_swap( pt2dA_type *apt )
{
	int	i;
	pt2d_type	*pt;
	float	tmp;

	for( i = 0 ; i < apt->nP ; i++ ){
		pt = &apt->p[i];

		tmp = pt->p.x;
		pt->p.x = pt->p.y;
		pt->p.y = tmp;
	}

	apt->axis = ( apt->axis == PT2D_AXIS_XY )? PT2D_AXIS_YX : PT2D_AXIS_XY;
}


