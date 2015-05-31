/*****************
	Vl3dTool.c
******************/

#include	<math.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include	"Uigp/igp.h"
#include "Ulog/Log.h"




#include "Vl3dType.h"


vl3dA_type *
vl3dA_alloc( int n )
{
	vl3dA_type	*avl;


	avl = (vl3dA_type *)malloc( sizeof( vl3dA_type) );

	avl->NA = n;
	avl->a = ( vl3d_type *)malloc( avl->NA*sizeof(vl3d_type) );

	avl->nA = 0;



	return( avl );
}


vl3dA_type *
vl3dA_realloc( vl3dA_type *avl, int n )
{
	if( avl == NULL ){
		avl = vl3dA_alloc(n );
		return( avl );
	}


	if( avl->NA < n ){
		avl->a = ( vl3d_type *)realloc( avl->a, n*sizeof(vl3d_type) );
		avl->NA = n;
	}

	return( avl );
}



void
vl3dA_destroy( vl3dA_type *avl )
{
	free( avl->a );

	free( avl );
}



void
vl3dA_add( vl3dA_type *avl, vl3d_type *vl )
{

	if( avl->nA >= avl->NA)
		avl = vl3dA_realloc( avl, avl->nA + 100 );


	avl->a[avl->nA++] = *vl;
}


void
vl3dA_remove( vl3dA_type *avl, int i0 )
{
int	i;

	avl->nA--;

	for( i = i0 ; i < avl->nA ; i++ )
		avl->a[i] = avl->a[i+1];
}



void 
vl3d_points( vl3d_type *vl, vec3d_type *p0, vec3d_type *p1 )
{

	p0->x = vl->p.x + vl->t0*vl->v.x;
	p0->y = vl->p.y + vl->t0*vl->v.y;
	p0->z = vl->p.z + vl->t0*vl->v.z;


	p1->x = vl->p.x + vl->t1*vl->v.x;
	p1->y = vl->p.y + vl->t1*vl->v.y;
	p1->z = vl->p.z + vl->t1*vl->v.z;
}




vl3dA_type *
vl3dA_create( vec3fA_type *a )
{

	vl3dA_type	*av;
	vl3d_type	*vl;
	int	i,	i1;


	av = vl3dA_alloc( a->nA );

	for( i = 0 ; i < a->nA ; i++ ){
		i1 = ( i == a->nA-1 )? 0 : i+1;
		vl = &av->a[av->nA++];
		
		vl->p.x = a->a[i].x;
		vl->p.y = a->a[i].y;
		vl->p.z = a->a[i].z;

		vl->v.x = a->a[i1].x - vl->p.x;
		vl->v.y = a->a[i1].y - vl->p.y;
		vl->v.z = a->a[i1].z - vl->p.z;

		vl->t0 = 0;
		vl->t1 = VEC3D_NORM( vl->v );

		vl->v.x /= vl->t1;
		vl->v.y /= vl->t1;
		vl->v.z /= vl->t1;
	}

	return( av );
}


void
vl3d_set( vl3d_type *vl, vec3d_type *w0, vec3d_type *w1 )
{
	vl->p.x = w0->x;
	vl->p.y = w0->y;
	vl->p.z = w0->z;

	vl->v.x = w1->x - vl->p.x;
	vl->v.y = w1->y - vl->p.y;
	vl->v.z = w1->z - vl->p.z;

	vl->t0 = 0;
	vl->t1 = VEC3D_NORM( vl->v );

	vl->v.x /= vl->t1;
	vl->v.y /= vl->t1;
	vl->v.z /= vl->t1;

}





double
vl3d_distane2( vl3d_type *vl, vec3f_type *p )
{

	vec3f_type	dp;
	double	t;




	dp.x = p->x - vl->p.x;
	dp.y = p->y - vl->p.y;
	dp.z = p->z - vl->p.z;

	t  = VEC3D_INNER( vl->v, dp );





	t = PUSH_TO_RANGE( t, vl->t0, vl->t1 );

	dp.x = p->x + t*vl->v.x - vl->p.x;
	dp.y = p->y + t*vl->v.y - vl->p.y;
	dp.z = p->z + t*vl->v.z - vl->p.z;

	t = VEC3D_NORM2( dp );

	return( t );
}


double
vl3dA_distance2( vl3dA_type *av, vec3f_type *p )
{
int	i,	iMin;

double tMin,	t;


	iMin = -1;

	for( i = 0 ; i < av->nA ; i++ ){
		t = vl3d_distane2( &av->a[i], p  );
		if( iMin < 0 || t < tMin ){
			iMin = i;
			tMin = t;
		}
	}

	return( tMin );

}


void
vl3dA_set_group( vl3dA_type *av, int group )
{
	int	i;

	for( i = 0 ; i < av->nA ; i++ )
		av->a[i].group = group;
}

int
vl3dA_get_group_size( vl3dA_type *av, int group )
{
	int	i,	n;

	for( i = 0, n =0 ; i < av->nA ; i++ ){
		if( av->a[i].group == group )
			n++;
	}

	return( n );
}