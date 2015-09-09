/********************************************************
*							*
*		PerDump.c				*
*							*
********************************************************/
#include	<stdio.h>

#include	"ViewType.h"


void
view_dumps( char *dir, char *fname, view_type *vw[], int vw_no )
{
char	file[256];
int	i;

	for( i = 0 ; i < vw_no ; i++ ){

		sprintf( file, "%s-%d", fname, i );

		view_dump( dir, file, vw[i] );
	}
}


void
view_dump( char *dir, char *fname, view_type *vw )
{
char	file[256];
FILE	*fp;

	sprintf( file, "%s/%s.view", dir, fname );

	fp = fopen( file, "w" );

	fprintf( fp, "!  \"%s\"		\"%d\"\n", "V-type",  vw->type );
	fprintf( fp, "!  \"%s\"		\"%.2f\"\n", "V-Ang",  vw->angle );
	fprintf( fp, "!  \"%s\"		\"%.2f\"\n", "V-Dis",  vw->dis );

	fprintf( fp, "!  \"%s\"		\"%.4f\"\n", "V-Cp-x",  vw->cp.x );
	fprintf( fp, "!  \"%s\"		\"%.4f\"\n", "V-Cp-y",  vw->cp.y );
	fprintf( fp, "!  \"%s\"		\"%.4f\"\n", "V-Cp-z",  vw->cp.z );

	fprintf( fp, "!  \"%s\"		\"%d\"\n", "V-N-flag",  1);
	fprintf( fp, "!  \"%s\"		\"%.4f\"\n", "V-N-x",  vw->N.x );
	fprintf( fp, "!  \"%s\"		\"%.4f\"\n", "V-N-y",  vw->N.y );
	fprintf( fp, "!  \"%s\"		\"%.4f\"\n", "V-N-z",  vw->N.z );


	fprintf( fp, "!  \"%s\"		\"%d\"\n", "V-v-flag",  1);
	fprintf( fp, "!  \"%s\"		\"%.4f\"\n", "V-v-x",  vw->V.x );
	fprintf( fp, "!  \"%s\"		\"%.4f\"\n", "V-v-y",  vw->V.y );
	fprintf( fp, "!  \"%s\"		\"%.4f\"\n", "V-v-z",  vw->V.z );


	fprintf( fp, "!  \"%s\"		\"%d\"\n", "V-w-flag",  0);
	fprintf( fp, "!  \"%s\"		\"%.4f\"\n", "V-U0",  vw->uv.x0 );
	fprintf( fp, "!  \"%s\"		\"%.4f\"\n", "V-U1",  vw->uv.x1 );
	fprintf( fp, "!  \"%s\"		\"%.4f\"\n", "V-V0",  vw->uv.y0 );
	fprintf( fp, "!  \"%s\"		\"%.4f\"\n", "V-V1",  vw->uv.y1 );


	fprintf( fp, "!  \"%s\"		\"%d\"\n", "V-pixel",  vw->Vpixel );
	fprintf( fp, "!  \"%s\"		\"%d\"\n", "U-pixel",  vw->Upixel );

	fprintf( fp, "!  \"%s\"		\"%d\"\n", "V-p0",  0 );
	fprintf( fp, "!  \"%s\"		\"%d\"\n", "U-p0",  0 );

	fclose( fp );
}
