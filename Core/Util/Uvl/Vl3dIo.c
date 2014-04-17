/******************
 *** Vl3dIo.c   ***
 ******************/
#include	<stdio.h>
#include	<string.h>

#include "Uigp/igp.h"

#include	"Vl3dType.h"







void	
vl3dA_dump( vl3dA_type *avl, char *prefix, int index, char *suffix )
{
	char	file[256];

	if( gpDump_filename( prefix, index, suffix, ".pl", file ) < 0 )
		return;


	vl3dA_write( avl, file );
}


void	
vl3d_dump( vl3d_type *pl, char *prefix, int index, char *suffix )
{
vl3dA_type *avl;

	avl = vl3dA_alloc( 1 );
	avl->a[0] = *pl;
	avl->nA = 1;

	vl3dA_dump( avl, prefix, index, suffix );

	avl->nA = 0;

	vl3dA_destroy( avl );
}



#define	VL3D_VERSION	1

int
vl3dA_write( vl3dA_type *avl, char *file )
{
	FILE	*fp;
	int	i;


	if( (fp = fopen( file, "wb")) == NULL )
		return( -1 );


	fprintf(fp, "%s  %d  %d\n", "VL3D", VL3D_VERSION, avl->nA );


	for( i = 0 ; i < avl->nA ; i++ ){
		vl3d_write( &avl->a[i], fp );
	}

	fclose( fp );

	return( 1 );

}





int
vl3d_write_to_file( vl3d_type *pl, char *file )
{
vl3dA_type *avl;
int	ret;

	avl = vl3dA_alloc( 1 );
	avl->a[0] = *pl;
	avl->nA = 1;


	ret = vl3dA_write( avl, file );


	avl->nA = 0;
	vl3dA_destroy( avl );

	return( ret );
}





int
vl3dA_read( char *file, vl3dA_type **avl )
{
	FILE	*fp;
	int	i,	version,	nPl;
	char	signature[64];


	if( (fp = fopen( file, "rb")) == NULL )
		return( -1 );


	fscanf(fp, "%s  %d", signature, &version );

	fscanf(fp, "%d", &nPl );


	*avl = vl3dA_alloc( nPl+10 );

	for( i = 0 ; i < nPl ; i++ ){
		
		vl3d_read( &(*avl)->a[(*avl)->nA++ ], fp );
	}

	fclose( fp );

	return( 1 );

}

void
vl3d_write(vl3d_type *vl, FILE *fp )
{

	fprintf( fp, "%lf %lf %lf  %lf %lf %lf   %lf %lf   %d\n",
		vl->p.x, vl->p.y, vl->p.z,
		vl->v.x, vl->v.y, vl->v.z,
		vl->t0, vl->t1, vl->group );

}




void
vl3d_read( vl3d_type *vl, FILE *fp )
{

	fscanf( fp, "%lf %lf %lf  %lf %lf %lf   %lf %lf   %d",
		&vl->p.x, &vl->p.y, &vl->p.z,
		&vl->v.x, &vl->v.y, &vl->v.z,
		&vl->t0, &vl->t1, &vl->group );

}


void
vl3d_writeb(vl3d_type *vl, FILE *fp )
{
vec3d_type	p0,	p1;

	vl3d_points( vl, &p0, &p1 );
	fwrite( &p0, 8, 3, fp );
	fwrite( &p1, 8, 3, fp );

	fwrite( &vl->group, 4, 1, fp );
}


void
vl3d_readb(vl3d_type *vl, FILE *fp )
{
	vec3d_type	p0,	p1;	
	fread( &p0, 8, 3, fp );
	fread( &p1, 8, 3, fp );

	vl3d_set( vl, &p0, &p1 );

	fread( &vl->group, 4, 1, fp );
}