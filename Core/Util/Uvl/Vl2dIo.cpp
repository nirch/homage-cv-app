/******************
 *** VlfdIo.c   ***
 ******************/
#include	<stdio.h>
#include	<string.h>

#include "Uigp/igp.h"

#include	"Vl2fType.h"


#define VL2F_VERSION	1


static void	vl2f_write(vl2f_type *vl, FILE *fp );

static void	vl2f_read( vl2f_type *vl, FILE *fp );




//int	
//vl2fA_dump( vl2fA_type *avl, char *prefix, int index, char *suffix )
//{
//	char	file[256];
//
//	if( gpDump_filename( prefix, index, suffix, ".vl2", file ) < 0 )
//		return( -1 );
//
//
//	vl2fA_write( avl, file );
//
//	return( 1 );
//}


void	
vl2f_dump( vl2f_type *vl, char *prefix, int index, char *suffix )
{
vl2fA_type *avl;

	avl = vl2fA_alloc( 1 );
	avl->a[0] = *vl;
	avl->nA = 1;

	vl2fA_dump( avl, prefix, index, suffix );

	avl->nA = 0;

	vl2fA_destroy( avl );
}




int
vl2fA_write( vl2fA_type *avl, char *file )
{
	FILE	*fp;
	int	i;


	if( (fp = fopen( file, "wb")) == NULL )
		return( -1 );


	fprintf(fp, "%s  %d  %d\n", "VL2D", VL2F_VERSION, avl->nA );


	for( i = 0 ; i < avl->nA ; i++ ){
		vl2f_write( &avl->a[i], fp );
	}

	fclose( fp );

	return( 1 );

}





int
vl2f_write_to_file( vl2f_type *pl, char *file )
{
vl2fA_type *avl;
int	ret;

	avl = vl2fA_alloc( 1 );
	avl->a[0] = *pl;
	avl->nA = 1;


	ret = vl2fA_write( avl, file );


	avl->nA = 0;
	vl2fA_destroy( avl );

	return( ret );
}





int
vl2fA_read( char *file, vl2fA_type **avl )
{
	FILE	*fp;
	int	i,	version,	nPl;
	char	signature[64];


	if( (fp = fopen( file, "rb")) == NULL )
		return( -1 );


	fscanf(fp, "%s  %d", signature, &version );

	fscanf(fp, "%d", &nPl );


	*avl = vl2fA_alloc( nPl+10 );

	for( i = 0 ; i < nPl ; i++ ){
		
		vl2f_read( &(*avl)->a[(*avl)->nA++ ], fp );
	}

	fclose( fp );

	return( 1 );

}

static void
vl2f_write(vl2f_type *vl, FILE *fp )
{

	fprintf( fp, "%f %f %f  %f %f   %f   %d\n",
		vl->p.x, vl->p.y,
		vl->v.x, vl->v.y,
		vl->d, vl->group );

}




static void
vl2f_read( vl2f_type *vl, FILE *fp )
{

	fscanf( fp, "%f %f  %f %   %f   %d",
		&vl->p.x, &vl->p.y,
		&vl->v.x, &vl->v.y,
		&vl->d, &vl->group );

}


void
vl2f_writeb(vl2f_type *vl, FILE *fp )
{
vec2f_type	p0,	p1;

	vl2f_points( vl, &p0, &p1 );
	fwrite( &p0, 4, 2, fp );
	fwrite( &p1, 4, 2, fp );

	fwrite( &vl->group, 4, 1, fp );
}


void
vl2f_readb(vl2f_type *vl, FILE *fp )
{
	vec2f_type	p0,	p1;	
	fread( &p0, 8, 2, fp );
	fread( &p1, 8, 2, fp );

	vl2f_set( vl, &p0, &p1 );

	fread( &vl->group, 4, 1, fp );
}