/******************
 ***   Tf.cpp   ***
 ******************/
#include	<math.h>
#include	<string.h>

#define _GPLOG

#include	"Uigp/igp.h"
#include "Ulog/Log.h"
#include	"Umath/Matrix.h"
#include	"TfType.h"


#define TF_VERSION 1     


static tf_type *	tf_read( FILE *fp );


int
	tfB_write( tfB_type *btf, char *prefix )
{
	char	file[256];

	int	i;
	for( i = 0 ; i < btf->nA ; i++ ){
		if( btf->a[i] == NULL )	continue;
		
		sprintf( file, "%s-%.2d.tf", prefix, i );

		tfA_write( btf->a[i], 1, file );
	}

	return( 1 );
}


int
tfA_write( tfA_type *aS, int prefix, char *file )
{
FILE	*fp;
int	i;

	gp_filename_force_extension( file, ".tf" );

	if(  (fp = fopen( file, "wb" )) == NULL )
		return( -1 );

	if( prefix == 1 )
		fprintf( fp, "TF  %d  %d\n", TF_VERSION, aS->nA );


	for( i = 0 ; i < aS->nA ; i++ ){
		if(  aS->a[i] == NULL )	continue;

		tf_write( aS->a[i], prefix, fp );
	}


	fclose( fp );

	return ( 1 );
}


int
tf_write( tf_type *s, int prefix, FILE *fp )
{
int	i;

	if( prefix  == 1 )
		fprintf( fp, "%s\t%d\t%d", s->name, s->iFrame, s->nA );


	for( i = 0 ; i < s->nA ; i++ )
		fprintf( fp, "\t%f", s->a[i] );

	fprintf( fp, "\n" );

	return ( 1 );
}






int
	tfB_read( tfB_type **btf, int nB, char *prefix )
{
	char	file[256];
	*btf = tfB_alloc( nB );


	int	i;
	for( i = 0 ; i < nB ; i++ ){
		(*btf)->a[i] = NULL;

		sprintf( file, "%s-%.2d.tf", prefix, i );

		tfA_type *atf;


		if( tfA_read( &atf, file ) > 0 ){
			(*btf)->a[i] = atf;
			(*btf)->nA = i+1;
		}
	}

	return( 1 );
}



tfA_type *
tfA_read( char *file )
{
tfA_type *atf;
tf_type *tf;
FILE	*fp;
int	nA;


	if(  (fp = fopen( file, "rb" ) ) == NULL )
		return( NULL );


	fscanf( fp, "%d", &nA );

	atf = tfA_alloc( nA );


	int n = 0;
	while( (tf = tf_read( fp )) != NULL ){
		tfA_add( atf, tf->iFrame, tf );

		n++;

	}


	fclose( fp );

	return( atf );
}


int
tfA_read( tfA_type **atf, char *file )
{
	tf_type *s;
	FILE	*fp;
	int	nS,	version;
	char	signature[256];

	//	gp_filename_force_extension( file, ".tf" );

	*atf = NULL;
	if(  (fp = fopen( file, "rb" ) ) == NULL )
		return( NULL );


	fscanf( fp, "%s", signature );
	if( strcmp( signature, "TF" ) != 0 ){
		version = 0;
		nS = atoi( signature );
	}
	else
		fscanf( fp, "%d %d", &version, &nS );


	*atf = tfA_alloc( nS );


	int	n = 0;
	while( (s = tf_read( fp )) != NULL && n < nS ){
		tfA_add( *atf, s->iFrame, s );
		n++;
	}

	fclose( fp );

	return( 1 );
}




static tf_type *
tf_read( FILE *fp )
{
tf_type *s;
int	i;
int	iFrame,	nP;
char	name[256];


	if( fscanf( fp, "%s  %d   %d", name, &iFrame, &nP ) == EOF )
		return( NULL );

	s = tf_alloc( nP );
	s->iFrame = iFrame;
	strcpy( s->name, name );
	s->nA = nP;


	for( i = 0 ; i < s->nA ; i++ )
		fscanf( fp, "%f", &s->a[i] );

	return ( s );
}


static tf_type *	tf_read_strtok( );

int
	tfA_read_from_data( tfA_type **atf, char *data )
{
	tf_type *s;

	int	nS,	version;
	char	signature[256];


	if( gp_strtok_string( data, " \t\r\n", signature ) < 0 )
		return( -1 );

	if( strcmp( signature, "TF" ) != 0 ){
		version = 0;
		nS = atoi( signature );
	}
	else {
		if( gp_strtok_int( NULL, " \t\r\n", &version ) < 0 )
			return(-1 );
		if( gp_strtok_int( NULL, " \t\r\n", &nS ) < 0 )
			return( -1 );
	}
	


	*atf = tfA_alloc( 100 );//nS );

	int n = 0;
	while( (s = tf_read_strtok()) != NULL && n < nS ){
		tfA_add( *atf, s->iFrame, s );
		n++;
	}
	

	return( 1 );
}




static tf_type *
tf_read_strtok( )
{
	tf_type *s;
	int	i;
	int	iFrame,	nP;
	char	name[256];


	if( gp_strtok_string( NULL, " \t\r\n", name ) < 0 )
		return( NULL );


	if( gp_strtok_int( NULL, " \t\r\n", &iFrame ) < 0 )
		return( NULL );
	if( gp_strtok_int( NULL, " \t\r\n", &nP ) < 0 )
		return( NULL );


	s = tf_alloc( nP );
	s->iFrame = iFrame;
	strcpy( s->name, name );
	s->nA = nP;


	for( i = 0 ; i < s->nA ; i++ )
		gp_strtok_float( NULL, " \t\r\n", &s->a[i] );
	

	tf_log( s );

	return ( s );
}


void
tf_log( tf_type *tf )
{
	GPLOG(("%s  %d %d  ", tf->name, tf->iFrame, tf->nA ) );

	int	i;

	for( i = 0 ; i < tf->nA ; i++ ){
		GPLOG(( "  %.4f", tf->a[i] ) );
	}

	GPLOG(( "\n" ) );
}