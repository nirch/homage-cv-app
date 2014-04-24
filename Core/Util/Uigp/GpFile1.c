/**********************
 ***	GpFile1.c	***
 **********************/


#include <stdio.h>
#include <string.h>


#ifndef CE
#include <fcntl.h>                        /* for O_BINARY */
#endif

#include	"igp.h"



#ifndef __SYMBIAN32__
void
gp_fwrite_bits( FILE *fp, int code, int no )
{
	while ( no-- != 0 ) {
 
		if ( code & 1L )
			fprintf( fp, "1 " );
		else
			fprintf( fp, "0 " );
 
		code >>= 1;
	}
 
	fprintf( fp, "\n");
}





int
gp_file_create( char *data, int bytes, char *file )
{
FILE	*fp;


	if ( (fp = fopen( file, "wb" )) == NULL )
		return( 0 );

	fwrite( data, 1, bytes, fp );

	fclose( fp );

	return( 1 );
}

int
gpFile_copy_to( char *sfile, char *dir )
{
char	file[256];
int	ret;

	gpFilename_force_dir( sfile, dir, file );

	ret = gpFile_copy( sfile, file );


	return( ret );
}


int
gpFile_copy( char *sfile, char *tfile )
{
FILE	*ifp,	*fp;
int	c;

	if ( (ifp = fopen( sfile, "rb" )) == NULL )
		return( 0 );

	if ( (fp = fopen( tfile, "wb" )) == NULL )
		return( 0 );

	 while ((c = getc( ifp )) != EOF) 
		putc( c, fp );


	fclose( ifp );
	fclose( fp );

	return( 1 );
}


int
gp_file_append( char *file, int i0, FILE *fp )
{
FILE	*ifp;
int	c;

	if ( (ifp = fopen( file, "rb" )) == NULL )
		return( 0 );


	if( i0 > 0 )	fseek( ifp, i0, 0L );


	 while ((c = getc( ifp )) != EOF) 
		putc( c, fp );


	fclose( ifp );

	return( 1 );
}


int
gp_file_compare( char *file1, char *file2 )
{
FILE	*fp1, *fp2;
int	c1,	c2;

	if ( (fp1 = fopen( file1, "rb" )) == NULL )
		return( -2 );

	if ( (fp2 = fopen( file2, "rb" )) == NULL ){
		fclose( fp1 );
		return( -2 );
	}


	while ((c1 = getc( fp1 )) != EOF){
		if( (c2 = getc( fp2 )) == EOF || c1 != c2 ){
			fclose( fp1 );
			fclose( fp2 );

			if( c2 == EOF )	return( -2 );
			return( -1 );
		}
	}
	

	if( getc( fp2 ) != EOF )	return( -3 );

	fclose( fp1 );
	fclose( fp2 );

	return( 1 );
}


int gp_write_INTtable_to_file(char *file, char *name, int table[], int no )
{
FILE *fp;
int	i;

	if( (fp = fopen( file, "wb")) == NULL )	return( -1 );

	fprintf( fp, "const int %s[] = {", name );

	for( i = 0 ; i < no-1 ; i++ )
		fprintf( fp, "%16d,\n", table[i] );

	fprintf( fp, "%16d };\n", table[i] );

	fclose( fp);

	return( 1 );
}


/*
int gp_read_buffer_from_file(char *file, u_char **buffer)
{
	FILE *fp;
	int size, asize;
	fp = fopen(file,"rb");
	if ( file == NULL ) 
	{
		*buffer = NULL;
		return -1;
	}

	size = gp_file_size( file );

	*buffer = (u_char *) malloc(size * sizeof(u_char) + 2);
	asize = fread( (*buffer), sizeof(u_char), size, fp );
	fclose(fp);
	return asize;
	
}
*/


int
gpFile_read_to_buffer(char *file, char **data, int *bytes )
{
FILE *fp;


	*bytes = 0;
	if( (fp = fopen(file,"rb")) == NULL )
		return( -1 );


	*bytes = gpFile_size( file );

	*data = (char *) malloc(*bytes * sizeof(u_char) + 2);
	*bytes = fread( (*data), sizeof(u_char), *bytes, fp );
	fclose(fp);

	(*data)[*bytes] = 0;

	return *bytes;
}
#endif

int
gpFile_write_buffer(char *file, char *data, int bytes )
{
	FILE *fp;

	if( (fp = fopen(file, "wb")) == NULL )
		return( -1 );

	fwrite( data, bytes, 1, fp);


	fclose(fp);

	return( 1 );
}


//int
//gp_write_buffer_to_file(char *file, char *data, int bytes )
//{
//	FILE *fp;
//
//	if( (fp = fopen(file, "wb")) == NULL )
//		return( -1 );
//
//	fwrite( data, bytes, 1, fp);
//
//
//	fclose(fp);
//
//	return( 1 );
//}



int
gpFile_lines( char *file )
{
FILE	*fp;
char	line[1024];
int	n = 0;

	if( (fp = fopen(file, "rb")) == NULL )
		return( -1 );


	n = 0;
	while( fgets( line, 1024, fp ) != NULL )
		n++;


	fclose( fp );

	return( n );
}


int
gpFile_find_token( FILE *fp, char *token, char line[1024] )
{
	char	*p;
	int	len;
	int	n = 0;

	len = (int)strlen( token );

	while( fgets( line, 1024, fp ) != NULL ){

		if( (p = strchr( line, token[0] )) == NULL )
			continue;

		if( strncmp( p, token, len ) == 0 )
//		if( strcmp( p, token ) == 0 )
			return( 1 );
		n++;
	}

	return( -1 );
}

int
gpFile_find_itoken( FILE *fp, char *token, char line[1024] )
{
	char	*p;
	int	len;

	len = (int)strlen( token );

	while( fgets( line, 1024, fp ) != NULL ){
		if( (p = gp_strichr( line, token[0] )) == NULL )
			continue;

		if( gp_strnicmp( p, token, len ) == 0 )
			return( 1 );

	}

	return( -1 );
}



int gpFile_read_path( FILE *fp, char *dir, char *path )
{
	char	fname[512];

	if( fscanf( fp, "%s", fname ) == EOF )
		return( -1 );

	if( fname[0] == '\\' || fname[0] == '/'  || fname[1] == ':' ){
		strcpy( path, fname );
		return( 1 );
	}

	sprintf( path, "%s\\%s", dir, fname );

	return( 1 );
}


int gpFile_sread_path( char *str, char *dir, char *path )
{
	char	fname[512];

	path[0] = 0;

	if( sscanf( str, "%s", fname ) == EOF )
		return( -1 );

	if( fname[0] == '\\' || fname[0] == '/'  || fname[1] == ':' ){
		strcpy( path, fname );
		return( 1 );
	}

	sprintf( path, "%s\\%s", dir, fname );

	return( 1 );
}


int gpFile_abs_path( char *fname, char *dir, char *path )
{
	if( fname[0] == '\\' || fname[0] == '/'  || fname[1] == ':' ){
		strcpy( path, fname );
		return( 1 );
	}

	sprintf( path, "%s\\%s", dir, fname );

	return( 1 );
}




int gpFile_find( char *dir, char *extension, char *path )
{
char	*aFile[2];
int	nFile;
int	i;

	nFile = gpDir_get_files_list( dir, NULL, extension, aFile, 1, 1 );

	if( nFile <= 0 )
		return( -1 );
	

	strcpy( path, aFile[0] );

	for( i = 0 ; i < nFile ; i++ )
		free( aFile[i] );


	return( nFile );
}

int 
gpFile_read_path_line( FILE *fp, char *token, char *relativeDir, char *path )
{
	char	line[1024];

	int	k;

	if( gpFile_find_token( fp, token, line ) < 0 )
		return( -1 );

	k = strlen(token);
	gpFile_sread_path( &line[k], relativeDir, path );


	return( 1 );
}



int
gpFile_fseek( FILE *fp, char *token )
{
	char	line[1024];
	char	*p;
	int	len,	pose;

	len = (int)strlen( token );
	fseek( fp, 0L, SEEK_SET );
	while( 1 ){
		pose = ftell( fp );

		if( fgets( line, 1024, fp ) == NULL )
			return( -1 );

		if( (p = gp_strichr( line, token[0] )) == NULL )
			continue;

		if( gp_strnicmp( p, token, len ) == 0 ){
			fseek( fp, pose, SEEK_SET );
			return( 1 );
		}

	}

	return( -1 );
}
