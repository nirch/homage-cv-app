/****************************
 ***	ImageWriteMng.c   ***
 ****************************/
#include <malloc.h>

#include "MngIo.h" 
#include "ImageWriteMng.h"


#define PNG_INTERNAL
#include "PngLib/png.h"


static void image_write_mng_signature( FILE *fp );

static void image_write_mng_header( FILE *fp, mngIo_type *mngIo );

static void image_write_mng_MHDR( FILE *fp, mngIo_type *mngIo );

static void image_write_mng_PLTE( FILE *fp, mngIo_type *mngIo );

static void image_write_mng_TERM( FILE *fp, mngIo_type *mngIo );

static void image_write_mng_BACK( FILE *fp,  mngIo_type *mngIo );

static void image_write_mng_FRAM( FILE *fp, mngIo_type *mngIo );

static void image_write_mng_add_frame_DEFI( FILE *fp, int frame_i );

static void image_write_mng_add_frame_DHDR( FILE *fp, int frame_i_pr,
						int width, int height, int x_loc, int y_loc );

static void image_write_mng_chunk( FILE *fp, char *name_chunk, 
								  char *data, int lenght );

// return code :number of bytes if ok 
//				-1 - could not open file for writing
//				-2 - error while writing				
int image_write_mng( image_type *im, palette_type *palette, char *file )
{
FILE *fp;

mngIo_type mngIo;
	
	memset( &mngIo, 0, sizeof( mngIo_type ) );

	mngIo.row = IMAGE_ROW(im);
	mngIo.column = IMAGE_COLUMN(im);
	mngIo.palette = NULL;

	fp = image_write_mng_open(file, &mngIo );

	if( fp == NULL )	return( -1 );



	if (image_write_mng_add_frame(fp, 1, im, palette, 0 ) >= 0 )
		return image_write_mng_close( fp, NULL );
	
	return -2;
}


FILE * 
image_write_mng_open( char *file, mngIo_type *mngIo	)
{
FILE	*fp;


	if( (fp = fopen( file, "wb")) == NULL )
		return NULL;

	image_write_mng_header( fp, mngIo );

	return fp;
}


void
image_write_mng_rewrite_header( FILE *fp, mngIo_type *mngIo )
{

	fseek( fp, 0, SEEK_SET );

	image_write_mng_header( fp, mngIo );

	fseek( fp, 0, SEEK_END );
}


static void
image_write_mng_header( FILE *fp, mngIo_type *mngIo )
{
	image_write_mng_signature( fp );

	image_write_mng_MHDR( fp, mngIo );

	if ( mngIo && mngIo->frame_no > 1 ) {
		image_write_mng_TERM( fp, mngIo );
		image_write_mng_BACK( fp, mngIo );
		image_write_mng_FRAM( fp, mngIo );
	}

	if ( mngIo && mngIo->palette != NULL && mngIo->palette->data != NULL )
		image_write_mng_PLTE( fp, mngIo );
}


static void
image_write_mng_signature( FILE *fp )
{
u_char buf[8] = "\212MNG\r\n\032\n";

	fwrite( buf, 1, 8, fp);
}


static void
image_write_mng_MHDR( FILE *fp, mngIo_type *mngIo )
{
MHDR_chunk_type mhdr;

	png_save_uint_32( mhdr.Frame_height, mngIo->row);
	png_save_uint_32( mhdr.Frame_width, mngIo->column);
	png_save_uint_32( mhdr.Ticks_per_second, 1000 );
	png_save_uint_32( mhdr.Nominal_layer_count, mngIo->frame_no+1);
	png_save_uint_32( mhdr.Nominal_frame_count, mngIo->frame_no);
	png_save_uint_32( mhdr.Nominal_play_time, 0);
	//png_save_uint_32( mhdr.Simplicity_profile, 583); // No Delta-PNG
	png_save_uint_32( mhdr.Simplicity_profile, 1007);

	image_write_mng_chunk( fp, "MHDR", (char *)&mhdr, sizeof(MHDR_chunk_type) );
}


static void 
image_write_mng_PLTE( FILE *fp,  mngIo_type *mngIo )
{
int size_plte;

	size_plte = mngIo->palette->nColor*3;

	image_write_mng_chunk( fp, "PLTE", 
			(char *)mngIo->palette->data, size_plte );
}

static void 
image_write_mng_TERM( FILE *fp,  mngIo_type *mngIo )
{
TERM_chunk_type term;
unsigned int delay;

	term.Termination_action			= 3;
	term.Action_after_iterations	= 0;
	delay = mngIo->delay_time;
	if ( mngIo->duration )
		delay = mngIo->duration/(mngIo->frame_no-1);
	png_save_uint_32( term.Delay, delay );
	png_save_uint_32( term.Iteration_max, 0x7fffffff);
						
	image_write_mng_chunk( fp, "TERM", (char *)&term, sizeof(TERM_chunk_type) );
}


static void 
image_write_mng_BACK( FILE *fp,  mngIo_type *mngIo )
{
BACK_chunk_type back;
color_type bg;

	png_save_uint_16( back.Red, 0 );
	png_save_uint_16( back.Green, 0 );
	png_save_uint_16( back.Blue, 0 );

	if ( mngIo->palette && mngIo->palette->data &&
		 mngIo->BgColor < mngIo->palette->nColor )
	{
		bg = mngIo->palette->data[mngIo->BgColor];
		png_save_uint_16( back.Red, bg.Red<<8 );
		png_save_uint_16( back.Green, bg.Green<<8 );
		png_save_uint_16( back.Blue, bg.Blue<<8 );
	}

	//back.Mandatory_background = 0;
	
	image_write_mng_chunk( fp, "BACK", (char *)&back, sizeof(BACK_chunk_type) );
}


static void
image_write_mng_FRAM( FILE *fp, mngIo_type *mngIo )
{
FRAM_chunk_type fram;
unsigned int delay;

	fram.Framing_mode = 3;
	fram.Separator = 0; //0
	fram.Change_interframe_delay = 2;
	fram.Change_timeout_and_termination = 0;
	fram.Change_layer_clipping_boundaries = 0;
	fram.Change_sync_id_list = 0;
	delay = mngIo->delay_time;
	if ( mngIo->duration )
		delay = mngIo->duration/(mngIo->frame_no-1);
	png_save_uint_32( fram.Interframe_delay, delay );//mngIo->avrageDelay );

	image_write_mng_chunk( fp, "FRAM", (char *)&fram, sizeof(FRAM_chunk_type) );
}


int 
image_write_mng_add_frame( FILE *fp, int frame_i,
		image_type *im, palette_type * palette, int fGlobalPalette )
{
int bytes;

	image_write_mng_add_frame_DEFI( fp, frame_i );

	if ( palette || fGlobalPalette )
		image_write_mng_add_frame__png_T( im, palette, 
											fGlobalPalette, -1, fp );
	else
		image_write_mng_add_frame__png( im, fp );

	bytes = ftell( fp );

	return bytes;
}


static void 
image_write_mng_add_frame_DEFI( FILE *fp, int frame_i )
{
DEFI_chunk_type defi;

	png_save_uint_16( defi.Object_id, frame_i);
	defi.Do_not_show			= 0;
	defi.Concrete_flag			= 1;//1 - can be used as source for delta-PNG
						
	image_write_mng_chunk( fp, "DEFI", (char *)&defi, sizeof(DEFI_chunk_type) );
}


int 
image_write_mng_add_frame_delta( FILE *fp, int frame_i,
		image_type *im_delta, int x_loc, int y_loc,
		palette_type * palette, int fGlobalPalette )
{
int bytes;

	image_write_mng_add_frame_DHDR( fp, frame_i-1,
		im_delta->column, im_delta->row, x_loc, y_loc );

	if ( palette || fGlobalPalette )
		image_write_mng_add_frame__png_T( im_delta, palette, 
											fGlobalPalette, -1, fp );
	else
		image_write_mng_add_frame__png( im_delta, fp );

	bytes = ftell( fp );

	return bytes;
}


static void 
image_write_mng_add_frame_DHDR( FILE *fp, int frame_i_pr,
						int width, int height, int x_loc, int y_loc )
{
DHDR_chunk_type dhdr;

	png_save_uint_16( dhdr.Object_id, frame_i_pr);
	dhdr.Image_type			= 1; //Image type is PNG
	dhdr.Delta_type			= 4; //Block pixel replacement
	png_save_uint_32( dhdr.Block_width, width);
	png_save_uint_32( dhdr.Block_height, height);
	png_save_uint_32( dhdr.Block_X_location, x_loc);
	png_save_uint_32( dhdr.Block_Y_location, y_loc);
						
	image_write_mng_chunk( fp, "DHDR", (char *)&dhdr, sizeof(DHDR_chunk_type) );

}


int
image_write_mng_close(FILE * fp, mngIo_type	*mngIo )
{
int bytes;

	//image_write_mng_chunk( fp, "SAVE", 0, 0 );
	//image_write_mng_chunk( fp, "SEEK", 0, 0 );
	//image_write_mng_chunk( fp, "SHOW", 0, 0 );
	
	image_write_mng_chunk( fp, "MEND", 0, 0 );

	bytes = ftell( fp );

	fclose(fp);

	return bytes;
}


static void 
image_write_mng_chunk( FILE *fp, char *name_chunk, 
								  char *data, int lenght )
{
unsigned char buf[4];
unsigned int crc = 0;

	png_save_uint_32( buf, lenght );
	fwrite( buf, 1, 4, fp);

	crc = crc32( crc, name_chunk, 4 );
	fwrite( name_chunk, 1, 4, fp);
	
	if ( data && lenght ) {
		crc = crc32( crc, data, lenght );
		fwrite( data, lenght, 1, fp);
	}

	png_save_uint_32( buf, crc );
	fwrite( buf, 1, 4, fp);
}


