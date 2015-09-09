/***********************
 ***	Pt3dTool.c   ***
 ***********************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Umath/Matrix2Type.h"
#include	"Ubox/Box2d.h"
#include	"Pt3dType.h"

#define _GPMEMORY_LEAK 
#include "Uigp/GpMemoryLeak.h"


pt3dA_type *
pt3dA_alloc( int n )
{
pt3dA_type	*apt;
int	i;

	apt = (pt3dA_type *)malloc( sizeof( pt3dA_type) );

	apt->a = ( pt3d_type *)malloc( n*sizeof(pt3d_type) );

	apt->state = PT3_OPEN;


	apt->NA = n;

	apt->nA = 0;


	for( i = 0 ; i < apt->NA ; i++ ){
		apt->a[i].id = 0;
		apt->a[i].group = 0;
	}


	return( apt );
}


pt3dA_type *
pt3dA_realloc( pt3dA_type*apt, int n )
{
	if( apt == NULL ){
		apt = pt3dA_alloc(n );
		return( apt );
	}


	if( apt->NA < n ){
		apt->a = ( pt3d_type *)realloc( apt->a, n*sizeof(pt3d_type) );
		apt->NA = n;
	}



	return( apt );
}



void
pt3dA_destroy( pt3dA_type *apt )
{

	free( apt->a );

	free( apt );
}






pt3dA_type *
pt3dA_set( pt3dA_type *apt, vec3dA_type *av )
{
pt3d_type	*pt;
int	i;

	if( apt == NULL )
		apt = pt3dA_alloc( av->nA );

	for( i = 0 ; i < av->nA ; i++ ){
		pt = &apt->a[i];
		pt->p = av->a[i];
		pt->group = 0;
	}

	apt->nA = i;

	return( apt );
}



pt3d_type *
pt3dA_add( pt3dA_type *av, vec3d_type *p )
{
pt3d_type *pt;

	if( av->nA >= av->NA )
		av = pt3dA_realloc( av, av->NA + 100 );


	pt = &av->a[av->nA++];

	pt->p = *p;

	return( pt );
}



pt3dA_type *
pt3dA_copy( pt3dA_type *apt, pt3dA_type *capt )
{
int	i;

	capt = pt3dA_realloc( capt, apt->nA );
	capt->type = apt->type;

	for( i = 0 ; i < apt->nA ; i++ ){
		capt->a[ i ] = apt->a[i];
	}

	capt->nA = i;

	return( capt );
}

pt3dA_type *
	pt3dA_copy_group( pt3dA_type *apt, int group, int no )
{
	pt3dA_type *gpt;
	int	i;

	if( no <= 0 )
		no = apt->nA;

	if( no <= 0 )	no = apt->nA;

	gpt = pt3dA_alloc( no );

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
pt3dA_type *
pt3dA_copy_box( pt3dA_type *apt, box2i *box, pt3dA_type *capt )
{
	int	i,	j;

	capt = pt3dA_realloc( capt, apt->nA );
	capt->type = apt->type;

	for( i = 0, j = 0 ; i < apt->nA ; i++ ){
		pt3d_type	*pt = &apt->a[i];

		if( BOX2D_IS_IN( *box, pt->p.x, pt->p.y ) )
			capt->a[ j++ ] = apt->a[i];
	}

	capt->nA = j;

	return( capt );
}




void
pt3dA_nGroup( pt3dA_type *apt )
{
	int	i;
	int64_t	mask;
	int	nGroup;

	mask = 0;
	nGroup = 0;

	for( i = 0; i < apt->nA; i++ ){
		pt3d_type	*pt = &apt->a[i];

		if( mask & ( ((int64_t)1)<< pt->group)  )
			continue;

		mask |=  ((int64_t)1)<< pt->group;
		nGroup++;
	}

	apt->nGroup = nGroup;
}



void
pt3dA_set_default_group( pt3dA_type *apt )
{
int	i;

	for(  i = 0 ; i < apt->nA ; i++ )
		apt->a[i].group = i;
}
#endif

int
pt3dA_select( pt3dA_type *apt, vec3f_type *p, float dis )
{
pt3d_type	*pt;
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



pt3dA_type * 
pt3dA_copy_neighbor( pt3dA_type *apt, vec3f_type *p, float D, pt3dA_type *capt )
{
	vec3f_type	v;
	float	D2,	d;
	int	i;




	if( capt == NULL )
		capt = pt3dA_alloc( apt->nA );

	capt->type = apt->type;

	D2 = D*D;
	capt->nA = 0;
	for( i = 0 ; i < apt->nA ; i++ ){
		pt3d_type *pt = &apt->a[i];

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
pt3dA_neighbor( pt3dA_type *apt, vec3f_type *p, float D )
{
vec3f_type	v;
float	D2,	d;
int	i,	no;

	D2 = D*D;

	for( i = 0, no = 0 ; i < apt->nA ; i++ ){
		pt3d_type *pt = &apt->a[i];

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
pt3dA_neighbor_remove( pt3dA_type *apt, vec3f_type *p, float D )
{
vec3f_type	v;
float	D2,	d;
int	i,	j,	no;

	D2 = D*D;

	for( i = 0, j = 0 ; i < apt->nA ; i++ ){
		pt3d_type *pt = &apt->a[i];

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
pt3dA_box( pt3dA_type *apt, box3f_type *box )
{
int	i;

	BOX3D_INIT( *box, apt->a[0].p.x, apt->a[0].p.y, apt->a[0].p.z );

	for( i = 0 ; i < apt->nA ; i++ ){
		pt3d_type *pt = &apt->a[i];
		BOX3D_UPDATE( *box, pt->p.x, pt->p.y, pt->p.z );
	}
}


#ifdef _AA_
int
pt3dA_no( pt3dA_type *apt, box2f_type *b )
{
int	i,	n;


	for( i = 0, n = 0 ; i < apt->nA ; i++ ){
		pt3d_type *pt = &apt->a[i];

		if( pt->p.x < b->x0 || pt->p.x > b->x1 || pt->p.y < b->y0 || pt->p.y > b->y1 )
			continue;

		n++;

	}


	return( n );
}



#endif

void
pt3dA_translate( pt3dA_type *apt, float x, float y, float z )
{
pt3d_type	*pt;
int	i;


	for( i = 0 ; i < apt->nA ; i++ ){
		pt = & apt->a[i];
		pt->p.x += x;
		pt->p.y += y;
		pt->p.z += z;
	}
}


void
pt3dA_scale( pt3dA_type *apt, float scale )
{
	pt3d_type	*pt;
	int	i;


	for( i = 0 ; i < apt->nA ; i++ ){
		pt = & apt->a[i];
		pt->p.x *= scale;
		pt->p.y *= scale;
		pt->p.z *= scale;
	}


}




float 
pt3dA_length( pt3dA_type *apt )
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
pt3dA_t2p( pt3dA_type *apt, float t, vec3f_type *p )
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
pt3dA_distance2( pt3dA_type *apt, vec3d_type *p )
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

		t = vec3d_lineS_distane( &apt->a[i].p, &apt->a[i+1].p, p );
		if( i == 0 || t < tMin ){
			tMin = t;
		}
	}

	if( apt->state == PT3_CLOSE ){
		t = vec3d_lineS_distane( &apt->a[apt->nA-1].p, &apt->a[0].p, p );
		if(  t < tMin )
			tMin = t;
	}

	return( tMin );

}




pt3dA_type	*
pt3dA_append( pt3dA_type *apt, pt3dA_type *sapt )
{
int	i;

	if( apt == NULL )
		return( sapt );


	//if( apt->NA < apt->nA + sapt->nA ){
	//	apt->NA = apt->nA + sapt->nA;
	//	apt->p = (pt3d_type *)realloc( apt->p, apt->NA*sizeof(pt3d_type) );
	//}

	apt = pt3dA_realloc( apt,  apt->nA + sapt->nA );

	for( i = 0 ; i < sapt->nA ; i++ )
		apt->a[apt->nA++] = sapt->a[i];


	pt3dA_destroy( sapt );

	return( apt );
}







pt3dG_type *
pt3dG_alloc( int n )
{
	pt3dG_type	*ptA;


	ptA = (pt3dG_type *)malloc( sizeof( pt3dG_type) );

	ptA->a = ( pt3dA_type **)malloc( n*sizeof(pt3dA_type*) );


	ptA->NA = n;

	ptA->nA = 0;



	return( ptA );
}

void
pt3dG_destroy( pt3dG_type *apt )
{

	free( apt->a );

	free( apt );
}
