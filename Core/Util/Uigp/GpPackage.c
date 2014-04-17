#include	<string.h>
#include	<stdio.h>
#include	<math.h>


#include	"Uigp/igp.h"
#include	"Ulog/Log.h"

#include "GpPackage.h"

#define VERSION	0x01

static int	vimLib_create_dir( char *path, FILE *fp );


typedef struct dd_type {
	char	path[256];
	char	name[256];
} dd_type;




int gpPackage_pack( char *path, char *file )
{
FILE *fp;
dd_type	dd[6];
int	i,	k;




	strcpy( dd[0].path, path );
	for( i = 0 ; i < 4 ; i++ ){
		gpFilename_name(  dd[i].path, dd[i].name );
		if( gp_stricmp(  dd[i].name, "CastLib" ) == 0  || gp_stricmp(  dd[i].name, "SoundLib" ) == 0  || gp_stricmp(  dd[i].name, "BckLib" ) == 0 )
			break;

		gpFilename_dir(  dd[i].path, dd[i+1].path );
	}

	if( i == 4 )	return( -1 );


//	sprintf( file, "%s/%s.vimp", outDir, dd[0].name );


	gpFilename_force_extension( path, ".vimp", file );
	if( (fp = fopen( file, "wb")) == NULL )
		return( -1 );


	fputc( 'V', fp );
	gp_fwrite_num( fp, VERSION, 3 );


	for( k = i ; k > 0 ; k-- ){
		fputc( 'D', fp );
		fwrite( dd[k].name, 1, 32, fp );

		gp_fwrite_num( fp, 1, 3 );
	}


	vimLib_create_dir( path, fp );

	fclose( fp );

	return( 1 );
	
}



static int 
vimLib_create_dir( char *path, FILE *fp )
{
char	*aDir[256],	*aFile[256],	name[256];
int	nDir,	nFile;

char *data;
int bytes;
int	i;

	nDir = gp_directory_file( path, "", "", 1, aDir, 256 );
	nFile = gpDir_get_files( path, "", "", aFile, 256, 1 );


	gpFilename_name( path, name );


	fputc( 'D', fp );
	fwrite( name, 1, 32, fp );

	gp_fwrite_num( fp, nDir+nFile, 3 );
	GPLOG( ("D   %s   %d %d ", name, nDir, nFile ));

	for( i = 0 ; i < nDir ; i++ ){
		vimLib_create_dir( aDir[i], fp );

		free( aDir[i] );
	}



	for( i = 0 ; i < nFile ; i++ ){
		GPLOG( ("F   %s   %d ", aFile[i], bytes ));

		gpFile_read_to_buffer( aFile[i], &data, &bytes );
		fputc( 'F', fp );
		gpFilename_fullname( aFile[i], name );
		fwrite( name, 1, 32, fp );

		gp_fwrite_num( fp, bytes, 3 );



		fwrite( data, 1, bytes, fp );

		GPLOG( ("F   %s   %d ", name, bytes ));

		free( data );
		free( aFile[i] );
	}



	return( 1 );

}



static int	vimLib_extract_dir( FILE *fp, char *path );


int gpPackage_extract( char *file, char *dir )
{
char	file[256];
FILE *fp;
int	version,	ret;
char	c;



	if( (fp = fopen( file, "rb")) == NULL )
		return( -1 );


	c = fgetc(fp);
	version = gp_fread_num( fp, 3 );


	c = fgetc(fp );

	if( c != 'D' ){
		fclose( fp );
		return( -1 );
	}
	
	
	ret = vimLib_extract_dir( fp, dir );
	

	fclose( fp );



	return( ret );

}

static int 
vimLib_extract_dir( FILE *fp, char *path )
{
char	dir[512],	name[256],	file[256];

char *data;
int bytes;
int	i,	ret;





//	fputc( 'D', fp );
	fread( name, 1, 32, fp );

	sprintf( dir, "%s/%s", path, name );

	gpDir_force_exist( dir );


	int nChild = gp_fread_num( fp, 3 );


	for( i = 0 ; i < nChild ; i++ ){
		
		char c = fgetc(fp );

		if( c == 'D' ){
			ret = vimLib_extract_dir( fp, dir );

			if( ret < 0 )	return( ret );

			continue;
		}


		if( c == 'F' ){

			fread( name, 1, 32, fp );

			bytes = gp_fread_num( fp, 3 );

			data = (char *)malloc( bytes+1 );

			fread( data, 1, bytes, fp );

			sprintf( file, "%s/%s", dir, name );

			if( gpFile_exist( file ) < 0 )
				gpFile_write_buffer( file, data, bytes );

			free( data );
			continue;
		}


		return( -1 );
	}


	return( 1 );

}