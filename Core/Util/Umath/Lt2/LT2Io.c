/*********************
 ***   LT2Io.h   ***
 *********************/
#include <stdio.h>
#include <math.h>

#include "Uigp/igp.h"
#include "../LT2Type.h"



void	
lt2_dump( lt2_type *lt, char *prefix, int index, char *suffix )
{
	char	file[256];

	if( gpDump_filename( prefix, index, suffix, ".ctr", file ) < 0 )
		return;


	lt2_write_to_file( lt, file );
}

int
lt2_write_to_file( lt2_type *lt, char *file )
{
	FILE *fp;

	if( (fp = fopen( file, "wb" )) == NULL )
		return( -1 );


	lt2_write( lt, fp );

	fclose( fp );

	return( 1 );
}

void
lt2_write( lt2_type *lt, FILE *fp )
{
	fprintf( fp, "%f  %f %f\n", lt->a0, lt->b0, lt->c0 );
	fprintf( fp, "%f  %f %f\n", lt->a1, lt->b1, lt->c1 );
}



int
lt2_read_file( lt2_type *lt, char *file )
{
	FILE *fp;

	if( (fp = fopen( file, "rb" )) == NULL )
		return( -1 );


	lt2_read( lt, fp );

	fclose( fp );

	return( 1 );
}


void
lt2_read( lt2_type *lt, FILE *fp )
{
	fscanf( fp, "%f  %f %f", lt->a0, lt->b0, lt->c0 );
	fscanf( fp, "%f  %f %f", lt->a1, lt->b1, lt->c1 );
}


