/**********************
 ***	Vec3dTool.c   ***
 **********************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Vec3d.h"

#include "Umath/Matrix3Type.h"



vec3fA_type *
vec3fA_alloc( int n )
{
	vec3fA_type	*av;

	av = (vec3fA_type *)malloc( sizeof( vec3fA_type) );

	av->a = ( vec3f_type *)malloc( n*sizeof(vec3f_type) );

	av->NA = n;

	av->nA = 0;

	return( av );
}

vec3fA_type *
vec3fA_realloc( vec3fA_type *av, int n )
{
	if( av == NULL ){
		av = vec3fA_alloc( n );
		return( av );
	}


	if( av->NA > n )
		return( av );


	av->a = ( vec3f_type *)realloc( av->a, n*sizeof(vec3f_type) );

	av->NA = n;



	return( av );
}

void
vec3fA_destroy( vec3fA_type *av )
{
	free( av->a );

	free( av );
}



vec3dA_type *
vec3dA_alloc( int n )
{
	vec3dA_type	*av;

	av = (vec3dA_type *)malloc( sizeof( vec3dA_type) );

	av->a = ( vec3d_type *)malloc( n*sizeof(vec3d_type) );

	av->NA = n;

	av->nA = 0;

	return( av );
}



void
vec3dA_destroy( vec3dA_type *av )
{
	free( av->a );

	free( av );
}

// make R2 to be orthonormal to R1;  assume that R1 is unit vector
void	
vec3d_orthonormal( vec3d_type *R1, vec3d_type *R2 )
{
	double t;

	t = VEC3D_INNER( *R1, *R2 );

	R2->x = R2->x - t * R1->x;

	R2->y = R2->y - t * R1->y;
	R2->z = R2->z - t * R1->z;

	VEC3D_NORMALIZE( *R2 );

//	t = VEC3D_INNER( *R1, *R2 );
}


float
vec3f_distance( vec3f_type *p0, vec3f_type *p1 )
{
	vec3d	dp;
	float	d;

	dp.x = p1->x - p0->x;
	dp.y = p1->y - p0->y;
	dp.z = p1->z - p0->z;

	d = sqrt( dp.x*dp.x + dp.y*dp.y + dp.z*dp.z );

	return( d );
}

float
vec3f_distane2( vec3f_type *p0, vec3f_type *p1 )
{
vec3f_type	dp;

	dp.x = p1->x - p0->x;
	dp.y = p1->y - p0->y;
	dp.z = p1->z - p0->z;

	return( VEC3D_NORM2(dp) );
}


float
vec3f_line_distane( vec3f_type *p0, vec3f_type *v, vec3f_type *p )
{
	double	a,	b;
	vec3f_type	dp;
	double	t;


	a = VEC3D_INNER( *v, *v );

	dp.x = p0->x - p->x;
	dp.y = p0->y - p->y;
	dp.z = p0->z - p->z;

	b = 2*VEC3D_INNER( *v, dp );

	t = -b / (2 *a );

	dp.x = p0->x + t*v->x - p->x;
	dp.y = p0->y + t*v->y - p->y;
	dp.z = p0->z + t*v->z - p->z;

	t = VEC3D_NORM( dp );

	return( t );
}


float
vec3f_lineS_distane( vec3f_type *p0, vec3f_type *p1, vec3f_type *p )
{
double	a,	b;
vec3f_type	v,	dp;
double	t;

	v.x = p1->x - p0->x;
	v.y = p1->y - p0->y;
	v.z = p1->z - p0->z;

	a = VEC3D_INNER( v, v );

	dp.x = p0->x - p->x;
	dp.y = p0->y - p->y;
	dp.z = p0->z - p->z;

	b = VEC3D_INNER( v, dp );

	t = -b / a;

	t = PUSH_TO_RANGE( t, 0, 1.0 );

	dp.x = p0->x + t*v.x - p->x;
	dp.y = p0->y + t*v.y - p->y;
	dp.z = p0->z + t*v.z - p->z;

	t = VEC3D_NORM2( dp );

	return( t );
}


float
vec3d_lineS_distane( vec3d_type *p0, vec3d_type *p1, vec3d_type *p )
{
	double	a,	b;
	vec3d_type	v,	dp;
	double	t;

	v.x = p1->x - p0->x;
	v.y = p1->y - p0->y;
	v.z = p1->z - p0->z;

	a = VEC3D_INNER( v, v );

	dp.x = p0->x - p->x;
	dp.y = p0->y - p->y;
	dp.z = p0->z - p->z;

	b = VEC3D_INNER( v, dp );

	t = -b / a;

	t = PUSH_TO_RANGE( t, 0, 1.0 );

	dp.x = p0->x + t*v.x - p->x;
	dp.y = p0->y + t*v.y - p->y;
	dp.z = p0->z + t*v.z - p->z;

	t = VEC3D_NORM2( dp );

	return( t );
}


void	
vec3fA_dump(  vec3fA_type *av, char *prefix, int index, char *suffix )
{
	char	file[256];

	if( gpDump_filename( prefix, index, suffix, ".pt", file ) < 0 )
		return;


	vec3fA_write_to_file( av, file );
}

int
vec3fA_write_to_file( vec3fA_type *av, char *file )
{
	FILE	*fp;
	int	i;

	if( (fp = fopen( file, "wb" ) ) == NULL )
		return( -1 );


	fprintf( fp, "V3   %d\n", av->nA );
	for( i = 0 ; i < av->nA ; i++ ){
		fprintf( fp, "%f %f %f\n", av->a[i].x, av->a[i].y, av->a[i].z );
	}


	fclose( fp );

	return( 1 );
}


int
vec3fA_read_to_file( vec3fA_type **av, char *file )
{
	FILE	*fp;
	int	i,	nA;
	char	str[256];
	vec3f_type	v;

	if( (fp = fopen( file, "rb" ) ) == NULL )
		return( -1 );


	fscanf( fp, "%s %d", str, &nA );

	*av = vec3fA_alloc( nA );
	for( i = 0 ; i < nA ; i++ ){
		fscanf( fp, "%f %f %f", &v.x, &v.y, &v.z );
		(*av)->a[(*av)->nA++ ] = v;
	}


	fclose( fp );

	vec3fA_remove_dup( *av );

	return( 1 );
}


int
vec3fA_remove_dup( vec3fA_type *av )
{
int	i,	j;

vec3f_type	dp;

	for( i = 1, j = 0 ; i < av->nA ; i++ ){
		dp.x = av->a[j].x - av->a[i].x;
		dp.y = av->a[j].y - av->a[i].y;
		dp.z = av->a[j].z - av->a[i].z;

		if( VEC3D_NORM2( dp ) < 0.001*0.001 )
			continue;
		j++;
		av->a[j] = av->a[i];
	}

	av->nA = j+1;

	return( 1 );
}


void
vec3fA_add( vec3fA_type *av, vec3f_type *p )
{
	if( av->nA >= av->NA )
		av = vec3fA_realloc( av, av->NA + 100 );


	av->a[av->nA++] = *p;
}




vec3fA_type	*
vec3fA_circle_yz( float x, float r, int nr, vec3fA_type *av )
{
	vec3f_type	*v;
	double	a;
	int	i;

	if( r == 0 )
		return( NULL );

	if( av == NULL )
		av = vec3fA_alloc( nr );

	av->nA = 0;

	for( i = 0; i < nr ; i++ ){

		a = 2*M_PI * i /nr;

		v = &av->a[av->nA++];
		v->x = x;
		v->y = r*cos(a);
		v->z = r*sin(a);
	}


	return( av );
}


vec3fA_type	*
vec3fA_circle_xy( float z, float r, int nr, vec3fA_type *av )
{
	vec3f_type	*v;
	double	a;
	int	i;

	if( r == 0 )
		return( NULL );

	if( av == NULL )
		av = vec3fA_alloc( nr );

	av->nA = 0;

	for( i = 0; i < nr ; i++ ){

		a = 2*M_PI * i /nr;

		v = &av->a[av->nA++];
		v->z = z;
		v->x = r*cos(a);
		v->y = r*sin(a);
	}


	return( av );
}
