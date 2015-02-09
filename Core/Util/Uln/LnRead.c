/******************
 *** GelmIo.c   ***
 ******************/
#include <stdio.h>
#include "Uigp/igp.h"


#include 	"LnType.h"

////static void	ln_write( vec2d *ctr, ln_type *gelm, FILE *fp );
static void	ln_read( FILE *fp, vec2d *ctr, ln_type **link );
//
// 
//int
//ln_write_to_file( vec2d *ctr, ln_type *gelm, char *file )
//{
//FILE	*fp;
//
//
//	if( (fp = fopen( file, "wb")) == NULL )
//		return( -1 );
//
//	fprintf( fp, "transform\n" );
//	fprintf( fp, "%.6f  %.6f  %.6f\n", 0.0, 0.0, 1.0 );
//
//
//
//	
//	ln_write( ctr, gelm, fp );
//
//	fclose( fp );
//
//	return(1 );
//}
//
//
//
//static void
//ln_write( vec2d *ctr, ln_type *gelm, FILE *fp )
//{
//ln_type	*l;
//int	no,	state,	type;
//
//	for( l = gelm, no = 0 ; l != NULL ; l = l->p[F_END], no++ );
//
//	state = 1;// open  2: close
//	type = 0;
//
//	fprintf( fp, "polylink\n" );
//	fprintf( fp, "%d   %d   %d\n", no, state, type );
//	fprintf( fp, "%.6f  %.6f\n", ctr->x, ctr->y );
//
//
//	for( l = gelm ; l != NULL ; l = l->p[F_END] ){
//		fprintf( fp, "%.6f  %.6f  %.6f\n",
//			l->v.x, l->v.y, l->a );
//
//	}
//}


int
lnL_read_file( char *file, vec2d *ctr, ln_type **link )
{
FILE	*fp;
float	x,	y,	s;
char	buf[256];

	if( (fp = fopen( file, "rb")) == NULL )
		return( -1 );

	fscanf( fp, "%s", buf );
	fscanf( fp, "%f  %f  %f", &x, &y, &s );




	ln_read( fp, ctr, link );

	fclose( fp );

	return(1 );
}




static void
ln_read( FILE *fp, vec2d *ctr, ln_type **link )
{
ln_type	*l;
int	no,	state,	type;
char	s[256];


	fscanf( fp, "%s", s );
	fscanf( fp, "%d   %d   %d\n", &no, &state, &type );
	fscanf( fp, "%f  %f\n", &ctr->x, &ctr->y );


	*link = lnL_alloc( no );


	for( l = *link ; l != NULL   ; l = l->p[F_END] ){
		fscanf( fp, "%f  %f  %f\n",
			&l->v.x, &l->v.y, &l->a );

		ln_set_aux( l );
	}
}
