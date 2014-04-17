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





void	
pt3fA_dump( pt3fA_type *apt, char *prefix, int index, char *suffix )
{
char	file[256];

	if( gpDump_filename( prefix, index, suffix, ".pt", file ) < 0 )
		return;


	pt3fA_write_to_file( apt, file );
}


#define PT3_VERSION		2

int
pt3fG_write_to_file( pt3fG_type *apt, char *file )
{
	FILE	*fp;
	int	i;

	if( (fp = fopen( file, "wb" ) ) == NULL )
		return( -1 );

	fprintf( fp, "PT3  %d  %d\n", PT3_VERSION , apt->nA );


	for( i = 0 ; i < apt->nA ; i++ )
		pt3fA_write( apt->a[i], fp );

	fclose( fp );

	return( 1 );
}


int
pt3fA_write_to_file( pt3fA_type *apt, char *file )
{
	FILE	*fp;


	if( (fp = fopen( file, "wb" ) ) == NULL )
		return( -1 );


	pt3fA_write( apt, fp );

	fclose( fp );

	return( 1 );
}


int
pt3fA_write( pt3fA_type *apt, FILE *fp )
{
int	i;

	fprintf( fp, "PT3  %d  %d   %d  %d\n", PT3_VERSION , apt->nA, apt->state, apt->type );

	for( i = 0 ; i < apt->nA ; i++ ){
		pt3f_type *pt = &apt->a[i];
		fprintf( fp, "%f %f %f  %d\n", pt->p.x, pt->p.y, pt->p.z, pt->group );
	}


	return( 1 );
}

int
pt3fA_read_from_file( pt3fA_type **apt, char *file )
{
	FILE	*fp;
	//int	i,	nA;
	//char	str[256];
	//int	version,	state,	type;
	int	ret;

	if( (fp = fopen( file, "rb" ) ) == NULL )
		return( -1 );

	ret = pt3fA_read( apt, fp );


#ifdef _AA_
	fscanf( fp, "%s %d %d %d", str, &version, &nA, &state );

	type = 0;
	if( version >= 2 )
		fscanf( fp, "%d",  &type );


	*apt = pt3fA_alloc( nA );
	(*apt)->state = state;
	(*apt)->type = type;


	
	for( i = 0 ; i < nA ; i++ ){
		pt3f_type *pt = &(*apt)->a[i];
		fscanf( fp, "%f %f %f  %d", &pt->p.x, &pt->p.y, &pt->p.z, &pt->group );
	}

	(*apt)->nA = nA;
#endif
	fclose( fp );

	
	return( ret );
}



int
pt3fA_read( pt3fA_type **apt, FILE *fp )
{

	int	i,	nA;
	char	str[256];
	int	version,	state,	type;





	fscanf( fp, "%s %d %d %d", str, &version, &nA, &state );

	type = 0;
	if( version >= 2 )
		fscanf( fp, "%d",  &type );


	*apt = pt3fA_alloc( nA );
	(*apt)->state = state;
	(*apt)->type = type;



	for( i = 0 ; i < nA ; i++ ){
		pt3f_type *pt = &(*apt)->a[i];
		fscanf( fp, "%f %f %f  %d", &pt->p.x, &pt->p.y, &pt->p.z, &pt->group );
	}

	(*apt)->nA = nA;


	return( 1 );
}