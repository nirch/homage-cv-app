/***********************
 *** ImageDump.cpp   ***
 ***********************/
#include	<string.h>
#include	"Uigp/igp.h"

#include	"ImageType/ImageType.h"



#include	"ImageDump/ImageDump.h"


static char Dump_dir[256];
static int	Fdump = 0;

void
image_dump_set_dirR( char *baseDir, char *subDir )
{
char	dir[256];

	sprintf( dir, "%s/%s", baseDir, subDir  );

	image_dump_set_dir( dir );
}


void
image_dump_set_dir( char *dump_dir )
{
int	len;

	strcpy( Dump_dir, dump_dir );

	len = (int)strlen( Dump_dir );
	if( len == 0 )	return;
	if( Dump_dir[len-1] == '/' || Dump_dir[len-1] == '\\')
		Dump_dir[len-1] = 0;


//	if( gp_is_dir_exist(  ) != 1 )
//		gp_mkdir( Dump_dir );

	gpDir_force_exist(Dump_dir);


	Fdump = 1;

}


char *	
image_dump_get_Dir( char **dump_dir )
{
	if( dump_dir != NULL )
		*dump_dir = Dump_dir;



	return( Dump_dir );
}

int
image_dump_get_dir( char *subDir, char *dir )
{

	if( Fdump == 0 )	return( -1 );

	if( gpDir_exist( Dump_dir ) != 1 )
		return( -1 );

	if( subDir == NULL ){
		sprintf( dir, "%s", Dump_dir );
		return( 1 );
	}

	sprintf( dir, "%s/%s", Dump_dir, subDir );

//	if( gp_is_dir_exist( dir ) != 1 )
//		gp_mkdir( dir );

	gpDir_force_exist( dir );

	return( 1 );
}


void
imageF_dump( image_type *sim, int stretch, char *name, int index, char *suffix )
{
image_type	*im,	*tim;
	

	if( stretch ){
		tim = imageF_stretch( sim, NULL );
		im = imageF_to_image1( tim, NULL );
		image_destroy( tim, 1 );
	}
	else	
		im = imageF_to_image1( sim, NULL );


	image_dump( im, name, index, suffix );

	image_destroy( im, 1 );
}


void
imageF_dumpAB( image_type *sim, float a, float b, char *name, int index, char *suffix )
{
	image_type	*im,	*tim;


	if( a != 1 || b != 0 ){
		tim = imageF_linearAB( sim, a, b, NULL );
		im = imageF_to_image1( tim, NULL );
		image_destroy( tim, 1 );
	}
	else	
		im = imageF_to_image1( sim, NULL );

	image_dump( im, name, index, suffix );

	image_destroy( im, 1 );
}

void
image2_dump( image_type *sim, char *name, int index, char *suffix )
{
image_type	*im;
	
	im = image2_to_image1( sim, NULL );

	image_dump( im, name, index, suffix );

	image_destroy( im, 1 );
}

void
imageT_dump( image_type *sim, char *name, int index, char *suffix )
{
	image_type	*im;


	image_dump( sim, name, index, suffix );

	im =  imageT_copy_alpha( sim, NULL );

	image_dump( im, name, index, "alpha" );

	image_destroy( im, 1 );
}

void
image_dump_band( image_type *sim, int iBand, char *name, int index, char *suffix )
{
	image_type	*im;


	im = image_band( sim, iBand, NULL );

	image_dump( im, name, index, suffix );

	image_destroy( im, 1 );
}

void
image_dump_alpha( image_type *sim, char *name, int index, char *suffix )
{
image_type	*im;


	im =  imageT_copy_alpha( sim, NULL );

	image_dump( im, name, index, suffix );

	image_destroy( im, 1 );
}

void
	image_dump_data( int width, int height, int depth, u_char *data, char *prefix, int index, char *suffix )
{
	image_type *im;

	im = image_create( height, width, depth, 1, data );


	image_dump( im, prefix, index, suffix );

	image_destroy( im, 0 );

}

void
image_dump_scale( image_type *im, float scale, char *prefix, int index, char *suffix )
{
	if( im->depth == 1 ){
		image_type *tim = image1_contrast( im, scale, 0, NULL );
		image_dump( tim, prefix, index, suffix );
		image_destroy( tim, 1 );
		return;
	}
}




void
image_dump_dup( image_type *sim, int d, float a, char *name, int index, char *ext )
{
	image_type	*im,	*tim;

	if( sim->depth != 1 && sim->depth != 3 )
		return;

	if( sim->depth == 1 ){
		tim = image1_contrast( sim, a, 0, NULL );

		im = image1_dup( tim, d, NULL );

		image_destroy( tim, 1 );
	}
	if( sim->depth == 3 ){
		im = image3_dup( sim, d, d, NULL );
	}


	image_dump( im, name, index, ext );

	image_destroy( im, 1 );
}


void
image_dump( image_type *im, char *name, int index, char *suffix )
{
char	file[256];

#ifdef _AA_
	if( Fdump == 0 )	return;

	if( index == -1 )
		sprintf( file, "%s/%s.bmp", Dump_dir, name );
	else	if( ext == NULL )
				sprintf( file, "%s/%s-%.3d.bmp", Dump_dir, name, index );
			else
				sprintf( file, "%s/%s-%.3d-%s.bmp", Dump_dir, name, index, ext );
#endif

	if( gpDump_filename( name, index, suffix, ".bmp", file ) < 0 )
		return;


	image_write_bmp( im, file );
}




void
image_dump_delete( char *prefix, char *suffix  )
{
	if( Fdump == 0 )	return;

	gpDir_delete_files( Dump_dir, prefix, suffix  );
}
