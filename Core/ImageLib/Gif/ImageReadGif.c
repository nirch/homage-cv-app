/****************************
 ***	ImageReadGif.c   ***
 ****************************/
#include	<string.h>


#include	"ImageReadGif.h"
#include	"Bmp/ImageBmp.h"


#define MAX_LENGHT_VIMATIX_DATA 1024

#define HEADER_VIMATIX_DATA "@vim"



static int			image_read_gif_header( gifIo_type *gifIo );
static image_type *	image_read_gif_next_imageData( gifIo_type *gifIo, int *top, int *left, palette_type **pal);
static int			image_read_gif_next_imageData_skip( gifIo_type *gifIo );
static int			image_read_gif_extension( gifIo_type *gifIo );
static int			image_read_gif_GraphicControl( gifIo_type *gifIo );
static int			image_read_gif_extension_skip( gifIo_type *gifIo );
static int			image_read_gif_extension_VimatixData( gifIo_type *gifIo );
static void			image_read_gif_rewind( gifIo_type *gifIo );



image_type *
image_read_gif_file( char *file )
{
gifIo_type	*gifIo;
image_type	*im;
int	r,	g,	b,	color;

	gifIo = image_read_gif_open_file( file );
	if( gifIo == NULL )	return( NULL);

	gifIo->im = image_create( gifIo->row, gifIo->column, 4, 4, NULL );

	r = gifIo->palette->data[gifIo->BgColor].Red;
	g = gifIo->palette->data[gifIo->BgColor].Green;
	b = gifIo->palette->data[gifIo->BgColor].Blue;
	color = IMAGE4_RGB( r, g, b );

	// 0xff000000  is a transparent flag
	image4_const( gifIo->im, color | 0xff000000 ); 

	image_read_gif_i( gifIo, 0 );

	im = gifIo->im;
	gifIo->im = NULL;


	image_read_gif_close( gifIo );

	return( im );
}


int
image_read_gif_is_valid_SINGATURE( char *data, int bytes )
{
	if( strncmp( data, "GIF89a", 6 ) == 0 )
		return( 1 );

	if( strncmp( data, "GIF87a", 6 ) == 0 )
		return( 1 );

	return( -1 );
}


image_type *
image_read_gif_buffer( char *data, int bytes, int Fdata )
{
gifIo_type	*gifIo;
image_type	*im;
int	r,	g,	b,	color;


	gifIo = image_read_gif_open_buffer( data, bytes,  Fdata );
	if( gifIo == NULL )	return( NULL);

	gifIo->im = image_create( gifIo->row, gifIo->column, 4, 4, NULL );

	r = gifIo->palette->data[gifIo->BgColor].Red;
	g = gifIo->palette->data[gifIo->BgColor].Green;
	b = gifIo->palette->data[gifIo->BgColor].Blue;
	color = IMAGE4_RGB( r, g, b );

	// 0xff000000  is a transparent flag
	image4_const( gifIo->im, color | 0xff000000 ); 

	image_read_gif_i( gifIo, 0 );


	im = gifIo->im;
	gifIo->im = NULL;


	image_read_gif_close( gifIo );

	return( im );
}


image_type *
image_read_gif_file_VimatixData( char *file, unsigned char *pVimatixData )
{
gifIo_type	*gifIo;
image_type	*im;
int	r,	g,	b,	color;

	gifIo = image_read_gif_open_file( file );
	if( gifIo == NULL )	return( NULL);

	gifIo->im = image_create( gifIo->row, gifIo->column, 4, 4, NULL );

	r = gifIo->palette->data[gifIo->BgColor].Red;
	g = gifIo->palette->data[gifIo->BgColor].Green;
	b = gifIo->palette->data[gifIo->BgColor].Blue;
	color = IMAGE4_RGB( r, g, b );

	// 0xff000000  is a transparent flag
	image4_const( gifIo->im, color | 0xff000000 ); 

	image_read_gif_i( gifIo, 0 );

	im = gifIo->im;
	gifIo->im = NULL;

	if ( gifIo->pVimatixData && gifIo->pVimatixData[0] )
		strncpy( pVimatixData, gifIo->pVimatixData, 256 );

	image_read_gif_close( gifIo );

	return( im );
}


image_type *
image_read_gif_buffer_VimatixData( char *data, int bytes, int Fdata,
									unsigned char *pVimatixData )
{
gifIo_type	*gifIo;
image_type	*im;
int	r,	g,	b,	color;


	gifIo = image_read_gif_open_buffer( data, bytes,  Fdata );
	if( gifIo == NULL )	return( NULL);

	gifIo->im = image_create( gifIo->row, gifIo->column, 4, 4, NULL );

	r = gifIo->palette->data[gifIo->BgColor].Red;
	g = gifIo->palette->data[gifIo->BgColor].Green;
	b = gifIo->palette->data[gifIo->BgColor].Blue;
	color = IMAGE4_RGB( r, g, b );

	// 0xff000000  is a transparent flag
	image4_const( gifIo->im, color | 0xff000000 ); 

	image_read_gif_i( gifIo, 0 );


	im = gifIo->im;
	gifIo->im = NULL;

	if ( gifIo->pVimatixData && gifIo->pVimatixData[0] )
		strncpy( pVimatixData, gifIo->pVimatixData, 256 );

	image_read_gif_close( gifIo );

	return( im );
}


int			
image_read_gif_info( char *file, int *row, int *column, int *frame_no, palette_type **pal )
{
gifIo_type	*gifIo;

	gifIo = image_read_gif_open_file( file );

	if( gifIo == NULL )	return( 0 );

	*row = gifIo->row;
	*column = gifIo->column;


	image_read_gif_get_frame_no( gifIo );
	*frame_no = gifIo->frame_no;


	if ( pal != NULL ) {
		*pal = gifIo->palette;
		gifIo->palette = NULL;
	}

	image_read_gif_close( gifIo );
	
	return ( 1 );
}







gifIo_type *
image_read_gif_open_file( char *file )
{
gio_type	*gio;
gifIo_type	*gifIo;

	gio = gio_open_file_to_read( file );
	if( gio == NULL )
		return( NULL );

	gifIo = image_read_gif_open( gio );

	return( gifIo );
}



gifIo_type *
image_read_gif_open_buffer( char *data, int bytes, int Fdata )
{
gio_type	*gio;
gifIo_type	*gifIo;

	gio = gio_open_buffer_to_read( data, bytes, Fdata );

	gifIo = image_read_gif_open( gio );

	return( gifIo );
}




gifIo_type *
image_read_gif_open( gio_type *gio )
{
gifIo_type	*gifIo;


	gifIo = (gifIo_type *)malloc( sizeof(gifIo_type) );
	
	gifIo->gio = gio;


	
	gifIo->frame_i = -1;
	gifIo->frame_no = -1;

	gifIo->im = NULL;
	gifIo->bim = NULL;

	gifIo->transparent_flag = 0;
	gifIo->disposal_method = 0;

	gifIo->delay_time = 30;
	gifIo->prev_delay_time = 0;

	gifIo->frameTime = 0;

	gifIo->pVimatixData = NULL;


	if( image_read_gif_header( gifIo ) < 0 ){
		image_read_gif_close( gifIo );
		return NULL;
	}

	gifIo->end_header_location = gio_ftell( gifIo->gio );

	return gifIo;
}


void
image_read_gif_get_frame_no( gifIo_type *gifIo )
{
long	location;
int	counter;
int	ret;

	if( gifIo->frame_no <= 0 ) {

		location = gio_ftell( gifIo->gio );

		if ( gifIo->frame_i == -1 )
			counter = 0 ;
		else
			counter = gifIo->frame_i;

		for( ; ; counter++ ){


			ret = image_read_gif_next_imageData_skip( gifIo );
			(gifIo->frame_i)++;
			if (ret == -1 )
				break;
		}

		gio_seek( gifIo->gio, location);

		gifIo->frame_no = counter;

		gifIo->duration = gifIo->frameTime;
//		gifIo->avrageDelay = gifIo->frameTime / (gifIo->frame_no -1 );
		gifIo->avrageDelay = (gifIo->duration / gifIo->frame_no) / 10;

		image_read_gif_rewind( gifIo );
	}
}


int	
image_read_gif_header( gifIo_type *gifIo )
{
u_char buf[13];
int	mask,	no,	i;
int AsepctRetio;
int	gif89a;

	gifIo->palette = NULL;
	gifIo->im = NULL;

	for ( i = 0 ; i < 13 ; i++ )
		buf[i] = GIO_GETC( gifIo->gio);
	
	if( strncmp(buf, "GIF", 3 ) != 0 )	return( -1 );


	if( strncmp(buf+3, "89a", 3 ) == 0 ){
		gif89a = 1;
	}
	else	{	
		if( strncmp(buf+3, "87a", 3 ) == 0 ){
			gif89a = 0;
		}
		else	return( -1 );
	}
	gifIo->column = buf[6] + 0x100 * buf[7];
	gifIo->row = buf[8] + 0x100 * buf[9];


	mask = buf[10];


	gifIo->BgColor = buf[11];

//	if( buf[12] != 0 )	return( -1 );
	AsepctRetio = buf[12];

	gifIo->palette = NULL;
	if (mask & 0x80) {
		no = 1<<((mask&7)+1);
		if( gifIo->palette != NULL )	palette_destroy( gifIo->palette );
		gifIo->palette = palette_alloc( no );
		for( i = 0 ; i < no ; i++ ){
			(gifIo->palette)->data[i].Red = GIO_GETC( gifIo->gio);
			(gifIo->palette)->data[i].Green = GIO_GETC( gifIo->gio);
			(gifIo->palette)->data[i].Blue = GIO_GETC( gifIo->gio);
		}
	}
	else
		gifIo->palette = palette_create( PALETTE_GRAY );

	gifIo->palette->type = PALETTE_UNKNOWN;


		
	return( 1 );
}


int
image_read_gif_i (gifIo_type *gifIo, int frame_i )
{
	if ( gifIo->frame_i > frame_i )
		image_read_gif_rewind( gifIo );

	while( gifIo->frame_i < frame_i ) {
		if( image_read_gif_next_frame( gifIo ) < 0 )	return( -1 );
		(gifIo->frame_i)++;
	}

	return( 1 );
}



int
image_read_gif_next_frame( gifIo_type *gifIo ) 
{
image_type	*im;
palette_type *palette;
int	top,	left;
int	r,	g,	b,	color;

	if ( gifIo->im == NULL ){
		gifIo->im = image_create( gifIo->row, gifIo->column, 4, 4, NULL );

		r = gifIo->palette->data[gifIo->BgColor].Red;
		g = gifIo->palette->data[gifIo->BgColor].Green;
		b = gifIo->palette->data[gifIo->BgColor].Blue;
		color = IMAGE4_RGB( r, g, b );

		if( gifIo->transparent_flag == 1 )
			color |= 0xFF000000;

		image4_const( gifIo->im, color ); 
//		image4_const( gifIo->im, 0xffffff ); 
	}

	im = image_read_gif_next_imageData( gifIo, &top, &left, &palette );




	if( im == NULL )	return( -1 );


#ifdef _GIF_DUMP
	{
		static int n = 0;
		image_type	*im1;
		im1 = image4_from_y( im );

		image_writeFN_bmp( im1, "d:/_out/giff/aa", n++ );

		image_destroy( im1, 1 );
	}
#endif



	if( palette == NULL )	palette = gifIo->palette;
	

	if ( gifIo->disposal_method == 2 ) {
		r = gifIo->palette->data[gifIo->BgColor].Red;
		g = gifIo->palette->data[gifIo->BgColor].Green;
		b = gifIo->palette->data[gifIo->BgColor].Blue;
		color = IMAGE4_RGB( r, g, b );
		if( gifIo->transparent_flag == 1)
			color |= 0xFF000000;
		image4_const( gifIo->im, color );

		if( gifIo->bim != NULL )
			gifIo->im = image_make_copy( gifIo->bim, gifIo->im );
	}



	if ( gifIo->transparent_flag ){
			image_8to24_copy_transparent( im, gifIo->im, top, left, palette, gifIo->transparent_index,  (gifIo->frame_i < 0 )? 1: 0 );
	}
	else	image_8to24_copy( im, gifIo->im, top, left, palette );


//	if( gifIo->transparent_flag == 1 )
//		imageT_negative_alpha( gifIo->im, gifIo->im );


//	gifIo->transparent_flag = 0;
//	gifIo->disposal_method = 0;

		
	image_destroy( im, 1 );

	if ( palette != gifIo->palette )
		palette_destroy( palette );

	if( gifIo->bim == NULL || gifIo->disposal_method == 1 )
		gifIo->bim = image_make_copy( gifIo->im, gifIo->bim );

	return( 1 );
}



void
image_read_gif_close( gifIo_type *gifIo )
{
	if( gifIo->palette != NULL )	
		palette_destroy( gifIo->palette );

	if( gifIo->im != NULL )
		image_destroy( gifIo->im, 1 );

	if( gifIo->pVimatixData != NULL )
		free( gifIo->pVimatixData );

	gio_close( gifIo->gio );

	free ( gifIo );

}



static image_type *
image_read_gif_next_imageData( gifIo_type *gifIo, int *top, int *left, palette_type **pal)
{
image_type	*im;
int	type;
int	ft;

//	if( gifIo->frame_i > -1 )
//		gifIo->frameTime += gifIo->delay_time * 10;

	while( 1 ){
		ft = gio_ftell( gifIo->gio );
		type = GIO_GETC( gifIo->gio);

		switch( type ){
		case  0x21:
			image_read_gif_extension( gifIo ); 
			break;

		case 0x2c:
			if (gifIo->frame_i > -1 )
				gifIo->frameTime += gifIo->prev_delay_time * 10;
			gifIo->prev_delay_time = gifIo->delay_time;
			im = image_read_gif_imageData( gifIo, top, left, pal );
			return( im );
			break;

		case 0x3b:
			return( NULL );
			break;
		}
	}

	return( NULL );
}


static int
image_read_gif_extension( gifIo_type *gifIo )
{
int	type;
int	ret;

	
	type = GIO_GETC( gifIo->gio);

	switch( type ){
	case  0x01:
		ret = image_read_gif_extension_skip( gifIo );
		break;

	case 0xf9:
		ret = image_read_gif_GraphicControl ( gifIo );
		break;

	case 0xfe:
		ret = image_read_gif_extension_VimatixData( gifIo );
//		ret = image_read_gif_extension_skip( gifIo );
		break;
	
	case 0xff:
		ret = image_read_gif_extension_skip( gifIo );
		break;
	}
	
	return( ret );
}

static int 
image_read_gif_GraphicControl( gifIo_type *gifIo )
{
int	bytes;
int	a,	b,	mask;

	bytes = GIO_GETC( gifIo->gio);
	mask = GIO_GETC( gifIo->gio);

	a = GIO_GETC( gifIo->gio);
	b = GIO_GETC( gifIo->gio);
	
	gifIo->delay_time = a  + 0x100 * b;



	gifIo->transparent_flag = mask&0x01;

	gifIo->disposal_method = mask&0x1c;
	gifIo->disposal_method = gifIo->disposal_method >> 2;

	fprintf( stdout, "%d", gifIo->disposal_method );
	
	gifIo->transparent_index = GIO_GETC( gifIo->gio);


	if( GIO_GETC( gifIo->gio) != 0 )	return( -1 );

	return( 1 );
}

static int
image_read_gif_extension_skip( gifIo_type *gifIo )
{
int	bytes,	i;
int	a;


	while( (bytes = GIO_GETC( gifIo->gio) ) ){
		for( i = 0 ; i < bytes  ; i++ )
			a = GIO_GETC( gifIo->gio);
	}


//	if( GIO_GETC( gifIo->gio) != 0 ) return ( -1 );

	return( 1 );
}


static int
image_read_gif_extension_VimatixData( gifIo_type *gifIo )
{
int	bytes,	i;
u_char buf[300];

	bytes = GIO_GETC( gifIo->gio);
	
	if( bytes == 0 )	return( -1 );

	for( i = 0 ; i < bytes  ; i++)
		buf[i] = GIO_GETC( gifIo->gio);

	if( GIO_GETC( gifIo->gio) != 0 ) return ( -1 );

	if ( strncmp( buf, HEADER_VIMATIX_DATA, 4 ) == 0 )
	{
		if( gifIo->pVimatixData != NULL )
			free( gifIo->pVimatixData );

		gifIo->pVimatixData = malloc( bytes-3 );
		memcpy( gifIo->pVimatixData, buf+4, bytes-4 );
		gifIo->pVimatixData[bytes-4] = 0;
	}

	return( 1 );
}


/*
static  int 
image_write_gif_application( FILE *fp )
{
u_char buf[19];

	buf[0] = 0x21;
	buf[1] = 0xff;
	buf[2] = 0x0b;
	memcpy(buf + 3, "NETSCAPE", 8);
	memcpy(buf + 11, "2.0", 3);
	buf[14] = 3;
	buf[15] = 1;
	buf[16] = 0;
	buf[17] = 0;
	buf[18] = 0;
	fwrite(buf, 1, 19, fp);
}
*/





static int
image_read_gif_next_imageData_skip( gifIo_type *gifIo )
{
int	type;

//	if( gifIo->frame_i > -1 )
//		gifIo->frameTime += gifIo->delay_time * 10;

	while( 1 ){

//		if( gifIo->gio->cst != NULL && 
//				gifIo->gio->cst->rp - gifIo->gio->cst->data >= gifIo->gio->cst->BYTE_NO )
//			return( -1 );

		type = GIO_GETC( gifIo->gio);

		switch( type ){
		case  0x21:
			image_read_gif_extension( gifIo ); 
			break;

		case 0x2c:
		if( gifIo->frame_i > -1 )
				gifIo->frameTime += gifIo->prev_delay_time * 10;
			gifIo->prev_delay_time = gifIo->delay_time;
			image_read_gif_imageData_skip( gifIo );
			return ( 1 );
			break;

		case 0x3b:
			return( -1 );
			break;
		}
	}

	return( -1 );
}


static void			
image_read_gif_rewind( gifIo_type *gifIo )
{
	gio_seek(gifIo->gio, gifIo->end_header_location);

	gifIo->frame_i = -1;
	gifIo->frameTime = 0;
	gifIo->delay_time = 30;
	gifIo->prev_delay_time = 0;
}


int
image_gif_get_frame_no( char *data, int bytes, int Fdata )
{
gifIo_type	*gifIo;

	gifIo = image_read_gif_open_buffer( data, bytes, Fdata );
	image_read_gif_get_frame_no( gifIo );

	return( gifIo->frame_no );
}


