/********************
 *** GpDump.cpp   ***
 ********************/
#include	<string.h>
#include	"Uigp/igp.h"



static char Dump_dir[256];
static int	Fdump = 0;

void
gpDump_set_dirR( char *baseDir, char *subDir )
{
	char	dir[256];

	sprintf( dir, "%s/%s", baseDir, subDir  );

	gpDump_set_dir( dir );
}

void
gpDump_set_dir( char *dump_dir )
{
int	len;

	strcpy( Dump_dir, dump_dir );

	len = strlen( Dump_dir );
	if( len == 0 )	return;
	if( Dump_dir[len-1] == '/' || Dump_dir[len-1] == '\\')
		Dump_dir[len-1] = 0;


//	if( gpDir_exist( Dump_dir ) != 1 )
//		gp_mkdir( Dump_dir );

	gpDir_force_exist(Dump_dir);


	Fdump = 1;

}

int
gpDump_is_set()
{
	return( Fdump );
}

char *
gpDump_get_dir()
{
	return( Dump_dir );
}

int
gpDump_get_sub_dir( char *subDir, char *dir )
{
	if( Fdump == 0 )	return( -1 );


	if( subDir == NULL ){
		sprintf( dir, "%s", Dump_dir );
		return( 1 );
	}

	sprintf( dir, "%s/%s", Dump_dir, subDir );

	if( gpDir_exist( dir ) != 1 )
		gp_mkdir( dir );

	return( 1 );
}




int
gpDump_filename( char *prefix, int index, char *suffix, char *extension, char *file )
{

	if( Fdump == 0 )	return( -1 );




	if( index == -1 ){
		sprintf( file, "%s/%s%s", Dump_dir, prefix, extension );
		return( 1 );
	}




	if( suffix == NULL ){
		sprintf( file, "%s/%s-%.3d%s", Dump_dir, prefix, index, extension );
		return( 1 );
	}

	sprintf( file, "%s/%s-%.3d-%s%s", Dump_dir, prefix, index, suffix, extension );

	return( 1 );
}




void
gpDump_delete(  char *prefix, char *suffix )
{
#ifndef WINCE
	gpDir_delete_files( Dump_dir, prefix, suffix  );
#endif
}




void	
gpDump_double( double a[], int nA, char *prefix, int index, char *suffix )
{
char	file[256];
FILE	*fp;
int	i;


	if( gpDump_filename( prefix, index, suffix, ".txt", file ) < 0 )
		return;


	if( (fp = fopen(file, "wb")) == NULL )
		return;


	for( i = 0 ; i < nA ; i++ )
		fprintf( fp, "%lf\n", a[i] ); 

	fclose( fp );
}

void	
gpDump_float( float a[], int nA, char *prefix, int index, char *suffix )
{
	char	file[256];
	FILE	*fp;
	int	i;


	if( gpDump_filename( prefix, index, suffix, ".txt", file ) < 0 )
		return;


	if( (fp = fopen(file, "wb")) == NULL )
		return;


	for( i = 0 ; i < nA ; i++ )
		fprintf( fp, "%f\n", a[i] ); 

	fclose( fp );
}

void	
gpDump_int( int a[], int nA, char *prefix, int index, char *suffix )
{
	char	file[256];
	FILE	*fp;
	int	i;


	if( gpDump_filename( prefix, index, suffix, ".txt", file ) < 0 )
		return;


	if( (fp = fopen(file, "wb")) == NULL )
		return;


	for( i = 0 ; i < nA ; i++ )
		fprintf( fp, "%d\n", a[i] ); 

	fclose( fp );
}