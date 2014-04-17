#include	<stdio.h>

#include	"Uigp/igp.h"
#include	"Config3d.h"



void
config3d_read( FILE *fp, config3d *cp )
{
	fscanf( fp, "%f %f %f  %f %f %f",
		&cp->x, &cp->y, &cp->z,
		&cp->u1, &cp->u2, &cp->u3 );
}


void
config3d_write( FILE *fp, config3d *cp )
{
	fprintf( fp, "%.4f %.4f %.4f  %.4f %.4f %.4f\n",
		cp->x, cp->y, cp->z,
		cp->u1, cp->u2, cp->u3 );
}



int
config3d_load( char *file, config3d cp[], int *cp_no )
{
FILE	*fp;
int	i;

	if( (fp = fopen( file, "r" ) ) == NULL ){
		warning( "config3d_load", "Can't open file" );
		return( -1 );
	}

	fscanf( fp, "%d", cp_no );

	for( i = 0 ; i < *cp_no ; i++ )
		fscanf( fp, "%f %f %f  %f %f %f",
			&cp[i].x, &cp[i].y, &cp[i].z,
			&cp[i].u1, &cp[i].u2, &cp[i].u3 );

	fclose( fp );

	return( *cp_no );
}



int
config3d_dump( char *file, config3d cp[], int cp_no )
{
FILE	*fp;
int	i;

	if( (fp = fopen( file, "w" ) ) == NULL ){
		warning( "config3d_dump", "Can't open file" );
		return( -1 );
	}

	fprintf( fp, "%d\n", cp_no );

	for( i = 0 ; i < cp_no ; i++ )
		fprintf( fp, "%.4f %.4f %.4f  %.4f %.4f %.4f\n",
			cp[i].x, cp[i].y, cp[i].z,
			cp[i].u1, cp[i].u2, cp[i].u3 );

	fclose( fp );
	return( cp_no );
}


