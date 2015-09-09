/***********************
 ***	Pt3fTool.c   ***
 ***********************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Umath/Matrix2Type.h"
#include	"Ubox/Box2d.h"
#include	"Pt3fType.h"

#define _GPMEMORY_LEAK 
#include "Uigp/GpMemoryLeak.h"


pt3fA_type *
pt3fA_alloc( int n )
{
pt3fA_type	*apt;
int	i;

	apt = (pt3fA_type *)malloc( sizeof( pt3fA_type) );

	apt->a = ( pt3f_type *)malloc( n*sizeof(pt3f_type) );

	apt->state = PT3_OPEN;


	apt->NA = n;

	apt->nA = 0;


	for( i = 0 ; i < apt->NA ; i++ ){
		apt->a[i].id = 0;
		apt->a[i].group = 0;
	}


	return( apt );
}


pt3fA_type *
pt3fA_realloc( pt3fA_type*apt, int n )
{
	if( apt == NULL ){
		apt = pt3fA_alloc(n );
		return( apt );
	}


	if( apt->NA < n ){
		apt->a = ( pt3f_type *)realloc( apt->a, n*sizeof(pt3f_type) );
		apt->NA = n;
	}



	return( apt );
}



void
pt3fA_destroy( pt3fA_type *apt )
{

	free( apt->a );

	free( apt );
}






pt3fA_type *
pt3fA_set( pt3fA_type *apt, vec3fA_type *av )
{
pt3f_type	*pt;
int	i;

	if( apt == NULL )
		apt = pt3fA_alloc( av->nA );

	for( i = 0 ; i < av->nA ; i++ ){
		pt = &apt->a[i];
		pt->p = av->a[i];
		pt->group = 0;
	}

	apt->nA = i;

	return( apt );
}



pt3f_type *
pt3fA_add( pt3fA_type *av, vec3f_type *p )
{
pt3f_type *pt;

	if( av->nA >= av->NA )
		av = pt3fA_realloc( av, av->NA + 100 );


	pt = &av->a[av->nA++];

	pt->p = *p;

	return( pt );
}



pt3fA_type *
pt3fA_copy( pt3fA_type *apt, pt3fA_type *capt )
{
int	i;

	capt = pt3fA_realloc( capt, apt->nA );
	capt->type = apt->type;

	for( i = 0 ; i < apt->nA ; i++ ){
		capt->a[ i ] = apt->a[i];
	}

	capt->nA = i;

	return( capt );
}

pt3fA_type *
	pt3fA_copy_group( pt3fA_type *apt, int group, int no )
{
	pt3fA_type *gpt;
	int	i;

	if( no <= 0 )
		no = apt->nA;

	if( no <= 0 )	no = apt->nA;

	gpt = pt3fA_alloc( no );

	gpt->type = apt->type;
	gpt->state = apt->state;

	for( i = 0 ; i < apt->nA ; i++ ){
		if( apt->a[i].group != group )	continue;

		gpt->a[ gpt->nA++ ] = apt->a[i];
	}

	if( gpt->state == PT3_CLOSE )
		gpt->nA--;

	return( gpt);
}


#ifdef _AA_
pt3fA_type *
pt3fA_copy_box( pt3fA_type *apt, box2i *box, pt3fA_type *capt )
{
	int	i,	j;

	capt = pt3fA_realloc( capt, apt->nA );
	capt->type = apt->type;

	for( i = 0, j = 0 ; i < apt->nA ; i++ ){
		pt3f_type	*pt = &apt->a[i];

		if( BOX2D_IS_IN( *box, pt->p.x, pt->p.y ) )
			capt->a[ j++ ] = apt->a[i];
	}

	capt->nA = j;

	return( capt );
}




void
pt3fA_nGroup( pt3fA_type *apt )
{
	int	i;
	int64_t	mask;
	int	nGroup;

	mask = 0;
	nGroup = 0;

	for( i = 0; i < apt->nA; i++ ){
		pt3f_type	*pt = &apt->a[i];

		if( mask & ( ((int64_t)1)<< pt->group)  )
			continue;

		mask |=  ((int64_t)1)<< pt->group;
		nGroup++;
	}

	apt->nGroup = nGroup;
}



void
pt3fA_set_default_group( pt3fA_type *apt )
{
int	i;

	for(  i = 0 ; i < apt->nA ; i++ )
		apt->a[i].group = i;
}
#endif

int
pt3fA_select( pt3fA_type *apt, vec3f_type *p, float dis )
{
pt3f_type	*pt;
vec3f_type	v;
float	dMin,	d;
int	i,	iMin;

	dMin = dis*dis;
	iMin = -1;
	for( i = 0 ; i < apt->nA ; i++ ){
		pt = & apt->a[i];
		v.x = pt->p.x - p->x;
		v.y = pt->p.y - p->y;
		v.z = pt->p.z - p->z;

		d = v.x*v.x + v.y*v.y + v.z*v.z;
		if( d < dMin ){
			dMin = d;
			iMin = i;
		}
	}

	return( iMin );
}



pt3fA_type * 
pt3fA_copy_neighbor( pt3fA_type *apt, vec3f_type *p, float D, pt3fA_type *capt )
{
	vec3f_type	v;
	float	D2,	d;
	int	i;




	if( capt == NULL )
		capt = pt3fA_alloc( apt->nA );

	capt->type = apt->type;

	D2 = D*D;
	capt->nA = 0;
	for( i = 0 ; i < apt->nA ; i++ ){
		pt3f_type *pt = &apt->a[i];

		v.x = pt->p.x - p->x;
		v.y = pt->p.y - p->y;
		v.z = pt->p.z - p->z;

		d = v.x*v.x + v.y *v.y + v.z*v.z;

		if( d == 0 )	continue;
		if( d > D2 )	continue;

//		pt->id = i;
		capt->a[capt->nA] =  *pt;
		capt->a[capt->nA].id = i;
		capt->nA++;

	}

	return( capt );
}




int 
pt3fA_neighbor( pt3fA_type *apt, vec3f_type *p, float D )
{
vec3f_type	v;
float	D2,	d;
int	i,	no;

	D2 = D*D;

	for( i = 0, no = 0 ; i < apt->nA ; i++ ){
		pt3f_type *pt = &apt->a[i];

		v.x = pt->p.x - p->x;
		v.y = pt->p.y - p->y;
		v.z = pt->p.z - p->z;

		d = v.x*v.x + v.y *v.y + v.z*v.z;

		if( d < D2 )
			no++;
	}

	return( no );
}

int 
pt3fA_neighbor_remove( pt3fA_type *apt, vec3f_type *p, float D )
{
vec3f_type	v;
float	D2,	d;
int	i,	j,	no;

	D2 = D*D;

	for( i = 0, j = 0 ; i < apt->nA ; i++ ){
		pt3f_type *pt = &apt->a[i];

		v.x = pt->p.x - p->x;
		v.y = pt->p.y - p->y;
		v.z = pt->p.z - p->z;

		d = v.x*v.x + v.y *v.y + v.z*v.z;

		if( d >= D2 )
			apt->a[j++] = apt->a[i];
		
	}

	no = apt->nA - j;
	apt->nA = j;

	return( no );
}







void
pt3fA_box( pt3fA_type *apt, box3f_type *box )
{
int	i;

	BOX3D_INIT( *box, apt->a[0].p.x, apt->a[0].p.y, apt->a[0].p.z );

	for( i = 0 ; i < apt->nA ; i++ ){
		pt3f_type *pt = &apt->a[i];
		BOX3D_UPDATE( *box, pt->p.x, pt->p.y, pt->p.z );
	}
}


#ifdef _AA_
int
pt3fA_no( pt3fA_type *apt, box2f_type *b )
{
int	i,	n;


	for( i = 0, n = 0 ; i < apt->nA ; i++ ){
		pt3f_type *pt = &apt->a[i];

		if( pt->p.x < b->x0 || pt->p.x > b->x1 || pt->p.y < b->y0 || pt->p.y > b->y1 )
			continue;

		n++;

	}


	return( n );
}



#endif

void
pt3fA_translate( pt3fA_type *apt, float x, float y, float z )
{
pt3f_type	*pt;
int	i;


	for( i = 0 ; i < apt->nA ; i++ ){
		pt = & apt->a[i];
		pt->p.x += x;
		pt->p.y += y;
		pt->p.z += z;
	}
}


void
pt3fA_scale( pt3fA_type *apt, float scale )
{
	pt3f_type	*pt;
	int	i;


	for( i = 0 ; i < apt->nA ; i++ ){
		pt = & apt->a[i];
		pt->p.x *= scale;
		pt->p.y *= scale;
		pt->p.z *= scale;
	}


}




float 
pt3fA_length( pt3fA_type *apt )
{
	int	i;
	float len = 0;

	for( i = 1 ; i < apt->nA ; i++ ){
		vec3f_type	p;
		p.x = apt->a[i].p.x - apt->a[i-1].p.x;
		p.y = apt->a[i].p.y - apt->a[i-1].p.y;
		p.z = apt->a[i].p.z - apt->a[i-1].p.z;

		len += sqrt( p.x*p.x + p.y*p.y + p.z*p.z );
	} 

	return( len );
}


int
pt3fA_t2p( pt3fA_type *apt, float t, vec3f_type *p )
{
	int	i;
	float len;
	vec3f_type	dp;

	for( i = 1 ; i < apt->nA ; i++ ){
		dp.x = apt->a[i].p.x - apt->a[i-1].p.x;
		dp.y = apt->a[i].p.y - apt->a[i-1].p.y;
		dp.z = apt->a[i].p.z - apt->a[i-1].p.z;

		len = VEC3D_NORM( dp );
		if( len > t )
			break;

		t -= len;
	} 

	t = t /len;
	p->x = apt->a[i-1].p.x + t *dp.x;
	p->y = apt->a[i-1].p.y + t *dp.y;
	p->z = apt->a[i-1].p.z + t *dp.z;

	return( 1 );
}


double
pt3fA_distance2( pt3fA_type *apt, vec3f_type *p )
{
	int	i;

	double tMin,	t;


	if( apt->nA == 1 ){
		vec3f_type	dp;
		dp.x = p->x - apt->a[0].p.x;
		dp.y = p->y - apt->a[0].p.y;
		dp.z = p->z - apt->a[0].p.z;

		t = VEC3D_INNER( dp, dp );

		return( t );
	}


	for( i = 0 ; i < apt->nA-1 ; i++ ){

		t = vec3f_lineS_distane( &apt->a[i].p, &apt->a[i+1].p, p );
		if( i == 0 || t < tMin ){
			tMin = t;
		}
	}

	if( apt->state == PT3_CLOSE ){
		t = vec3f_lineS_distane( &apt->a[apt->nA-1].p, &apt->a[0].p, p );
		if(  t < tMin )
			tMin = t;
	}

	return( tMin );

}




pt3fA_type	*
pt3fA_append( pt3fA_type *apt, pt3fA_type *sapt )
{
int	i;

	if( apt == NULL )
		return( sapt );


	//if( apt->NA < apt->nA + sapt->nA ){
	//	apt->NA = apt->nA + sapt->nA;
	//	apt->p = (pt3f_type *)realloc( apt->p, apt->NA*sizeof(pt3f_type) );
	//}

	apt = pt3fA_realloc( apt,  apt->nA + sapt->nA );

	for( i = 0 ; i < sapt->nA ; i++ )
		apt->a[apt->nA++] = sapt->a[i];


	pt3fA_destroy( sapt );

	return( apt );
}







pt3fG_type *
pt3fG_alloc( int n )
{
	pt3fG_type	*ptA;


	ptA = (pt3fG_type *)malloc( sizeof( pt3fG_type) );

	ptA->a = ( pt3fA_type **)malloc( n*sizeof(pt3fA_type*) );


	ptA->NA = n;

	ptA->nA = 0;



	return( ptA );
}

void
pt3fG_destroy( pt3fG_type *apt )
{

	free( apt->a );

	free( apt );
}
