/*******************
 *** LnWrite.c   ***
 *******************/

#include	<string.h>

#include	"LnType.h"




int
lnL_write( vec2f_type *ctr, ln_type *lnL, char *file )
{
FILE	*fp;
ln_type	*l;
int	no;


	if( (fp = fopen( file, "wb")) == NULL )
		return( -1 );




	fprintf( fp, "transform\n" );
	fprintf( fp, "%.6f  %.6f  %.6f\n", 0.0,0.0, 1.0 );




	no = lnL_no( lnL );

	fprintf( fp, "polylink\n" );
	fprintf( fp, "%d   %d   %d\n", no, 0, 0 );
	fprintf( fp, "%.6f  %.6f\n", ctr->x, ctr->y );




	for( l = lnL ;  l != NULL ; l = LN_NEXT(l) )
			fprintf( fp, "%.6f  %.6f  %.6f\n", l->v.x, l->v.y, l->a );


	fclose( fp );

	return( 1 );
}



void	
lnL_dump( vec2f_type *ctr, ln_type *lnL, char *prefix, int index, char *suffix )
{
	char	file[256];

	if( gpDump_filename( prefix, index, suffix, ".ctr", file ) < 0 )
		return;


	lnL_write( ctr, lnL, file );
}