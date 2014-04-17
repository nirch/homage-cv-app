#include	<string.h>
#include	<stdio.h>
#include	<math.h>


#include	"Uigp/igp.h"
#include	"Utime/GpTime.h"
#include	"ImageType/ImageType.h"

#include	"Bmp/ImageBmp.h"

#include	"./SSEUtils.h"

static void	read_command_line(int argc, char *argv[],
							  char *inFile, char *outFile );

image_type *	conv3( image_type *sim, int rC, float sigma );

image_type *	conv6( image_type *sim, int rC, float sigma );

image_type *	yCrCb6( image_type *sim );
					
image_type *	conv2( image_type *sim, int rC, float sigma );


int
main( int argc, char **argv )
{
char inFile[256],	outFile[256];
image_type	*sim,	*im,	*im2,	*oim;
gp_time_type ret_time;



	read_command_line( argc, argv, inFile, outFile );

	sim = image_read_bmp_file( inFile );
	if( sim == NULL )
		error( "CreateImages", "Read image failed" );



	im = image4_to_y( sim );

	im2 = image2_from_image1( im );

	image_destroy( im, 1 );


	gp_time_init( &ret_time );
	gp_time_start( &ret_time );

	oim = conv2( im2, 2, 0.75 );

	gp_time_stop( &ret_time );



//	oim = image2_to_image1( im );

	gp_filename_force_extension( outFile, ".bmp" );
	image_writeF_bmp( oim, outFile );
	image_destroy( oim, 1 );

	image_destroy( im2, 1 );

	gp_time_print( stderr, "convolution", &ret_time );
}

int
mainaa( int argc, char **argv )
{
char inFile[256],	outFile[256];
//char	dir[256],	name[256],	extension[256],	file[256];
image_type	*sim,	*im,	*cim;
gp_time_type ret_time;
//float	g[5];
//int	i;


	read_command_line( argc, argv, inFile, outFile );

	sim = image_read_bmp_file( inFile );
	if( sim == NULL )
		error( "CreateImages", "Read image failed" );



	im = image3_form_image4( sim );
//	im = image4_to_y( sim );


	gp_time_init( &ret_time );
	gp_time_start( &ret_time );

/*
	cim = conv3( im, 2, 0.5 );
	gp_time_stop( &ret_time );


//	im = image3_to_image4( cim );


	gp_filename_force_extension( outFile, ".bmp" );
	image_writeF_bmp( cim, outFile );
	image_destroy( cim, 1 );





	cim = conv6( im, 2, 0.5 );

	gp_filename_split( outFile, dir, name, extension );
	sprintf( file, "%s/%s_6.bmp", dir, name );
	image_writeF_bmp( cim, file );
	image_destroy( cim, 1 );
*/


	cim = yCrCb6( im );


	gp_filename_force_extension( outFile, ".bmp" );
	image_writeF_bmp( cim, outFile );
	image_destroy( cim, 1 );




	image_destroy( im, 1 );
	image_destroy( sim, 1 );

//	image_destroy( im, 1 );


	gp_time_print( stderr, "convolution", &ret_time );

	return( 1 );
}


image_type *
conv3( image_type *sim, int rC, float sigma )
{
image_type *cim;
float	g[10];
int	i;

	cim = NULL;
	guasian( 2, 0.5, g );
	for( i = 0 ; i < 10 ; i++ )
		cim = image3_convolution_separably( sim, g, g, 2, cim );
//		cim = image1_convolution_separably( im, g, g, 2, cim );

	return( cim );

}

image_type *
conv6( image_type *sim, int rC, float sigma )
{
image_type	*im,	*im1,	*im2;
float	g[10];
	
	guasian( rC, sigma, g );

	im1 = image6_from_image3( sim );
//	im1 = sim;

	im2 = image6_convolution_separably( im1, g, g, rC, NULL );


	im = image6_to_image3( im2);


	return( im );
}


image_type *
yCrCb6( image_type *sim )
{
image_type	*im,	*im1,	*im2;
image_type	*imY,	*imCr,	*imCb;

	
	im1 = image6_from_image3( sim );
//	im1 = sim;

	image6_to_YCrCb( im1, &imY, &imCr, &imCb );

	im2 = image6_YCrCb_to_RGB( imY, imCr, imCb, NULL );


	im = image6_to_image3( im2);


	return( im );
}



image_type *
conv2( image_type *sim, int rC, float sigma )
{
image_type	*im,	*im1,	*im2;
float	g[10];
int	i;
	
	//guasian( rC, sigma, g );

	im1 = sim;
	im2 = image_create( sim->row, sim->column, 2 , 1, NULL );
	for( i = 0 ; i < 10 ; i++ )
	{
		image2_convolution((unsigned short*)im1->data , im1->column * im1->row,(unsigned short*) im2->data);
	
		//im2 = image2_convolution_separably( im1, g, g, rC, im2 );
	}

	im1 = image2_to_image1( im2 );


	return( im1 );
}


static char *Usage = "convolution  inFile   outFile\n";




static void	read_command_line(int argc, char *argv[],
							  char *inFile, char *outFile )
{
int     k;

	k = 1;



	if (k < argc) {
		sprintf( inFile, "%s", argv[k]);
		k++;
	}
	else	error("read_command_line", Usage);


	if (k < argc) {
		sprintf( outFile, "%s", argv[k]);
		k++;
	}
	else	strcpy( outFile, inFile );
	//else	error("read_command_line", Usage);
}


