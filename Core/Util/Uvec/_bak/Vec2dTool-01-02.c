/**********************
 ***	VecTool.c   ***
 **********************/
#include	"Uigp/igp.h"
#include	"Vec2d.h"
#include	<math.h>


vec2fA_type *
vec2fA_alloc( int n )
{
vec2fA_type	*av;

	av = (vec2fA_type *)malloc( sizeof( vec2fA_type) );

	av->v = ( vec2d *)malloc( n*sizeof(vec2d) );

	av->nV = 0;

	return( av );
}


void
vec2fA_destroy( vec2fA_type *av )
{
	free( av->v );

	free( av );
}

void
vec2d_reorder( vec2d V[], int no )
{
vec2d	v,	*v0,	*v1,	*v2;
int	i,	j,	I;
int	flag,	tflag;


	for( i = 1 ; i < no-1 ; i++ ){
		v0 = &V[i-1];
		v1 = &V[i];

		flag = -( v1->x * v0->y + v1->y * -v0->x ) > 0;

		I = i;

		for( j = i+1 ; j < no ; j++ ){
			v2 = &V[j];
			tflag = -( v2->x * v0->y + v2->y * -v0->x ) > 0;
			if( -( v2->x * v1->y + v2->y * -v1->x ) > 0 )
					tflag |= 0x02;

			if( (flag == 1 && tflag == 0x01 ) ||
				(flag == 0 && tflag != 0x02 ) ){

				I = j;
				v1 = v2;
				flag = -( v1->x * v0->y + v1->y * -v0->x ) > 0; 
			}
		}

		if( i == I )    continue;

		v = V[i];
		V[i] = V[I];
		V[I] = v;
	}
}

int
vec2d_select( vec2d V[], int no, vec2d *p )
{
vec2d	*v;
int	i,	flag,	tflag;

	v = &V[0];
	flag = ( -p->x * v->y + p->y * v->x ) > 0;


	for( i = 1 ; i < no ; i++ ){
		v = &V[i];
		tflag = ( -p->x * v->y + p->y * v->x ) > 0;

		if( flag == 1 && tflag == 0 )	return( i -1 );

		flag = tflag;
	}

	return( no-1 );
}



float
vec2d_distance( vec2d *v0 ,vec2d *v1 )
{
vec2d	dv;
float	dis;

	dv.x = v0->x - v1->x;
	dv.y = v0->y - v1->y;
	dis = hypot( dv.x, dv.y );

	return( dis );
}


float
vec2d_distance2( vec2d *v0 ,vec2d *v1 )
{
vec2d	dv;
float	dis;

	dv.x = v0->x - v1->x;
	dv.y = v0->y - v1->y;
	dis = dv.x*dv.x + dv.y*dv.y;

	return( dis );
}


float
vec2d_angle( vec2d *v0 ,vec2d *v1 )
{
float	l0,	l1,	t;
float	angle;

	l0 = hypot( v0->x, v0->y );
	l1 = hypot( v1->x, v1->y );

	t = (v0->x*v1->x + v0->y*v1->y )/( l0 * l1 );
	angle = acos( t );

	return( angle );
}




/*
int
vec2d_select( vec2d V[], int no, vec2d *p )
{
vec2d	*v;
int	i,	flag,	tflag;

	v = &V[0];
	flag = ( p->x * v->y + p->y * -v->x ) > 0;


	for( i = 1 ; i < no ; i++ ){
		v = &V[i];
		tflag = ( p->x * v->y + p->y * -v->x ) > 0;

		if( flag != tflag )	return( i-1 );

		flag = tflag;
	}

	return( no );
}
*/




// return vector that crospond to the two parall motion vector
int
vec2d_joint( vec2d *mv0, vec2d *mv1, vec2d *mv )
{
vec2d	v0,	v1;
float	det;
float	l0,	l1;


	l0 = hypot( mv0->x, mv0->y );
	if( l0 < 0.25 )	return( -1 );

	v0.x = mv0->x / l0;
	v0.y = mv0->y / l0;


	l1 = hypot( mv1->x, mv1->y );
	if( l1 < 0.25 )	return( -1 );

	v1.x = mv1->x / l1;
	v1.y = mv1->y / l1;




	det = v0.x * v1.y -v0.y *v1.x;

	if( ABS(det) < 0.382 ){//0.923 ){	// 22.5 dgree

		return( -1 );
	}


	mv->x = (l0 * v1.y - l0 *v0.y)/ det;
	mv->y = (v0.x * l1 - v1.x *l0)/ det;

	return( 1 );
}