/****************************
 ***	ImageReadGifIo.h   ***
 ****************************/
#ifndef	_IMAGE_READ_GIF_IO_
#define _IMAGE_READ_GIF_IO_

#ifdef __cplusplus
extern "C" {
#endif


#include	"ImageType/ImageType.h"
#include	"Ugio/GioType.h"

#include	"GifIo.h"

/*
typedef struct gifIo_type {
	
	int	column;

	int	row;

	int	frame_i;	// index of current frame

	int	frame_no;

	int BgColor;

	long	end_header_location;
	
	// graphic control
	int	transparent_flag;
	int	transparent_index;
	int	disposal_method;
	int	delay_time;

	palette_type	*palette;

	image_type	*im;	// current image

	gio_type *gio;

} gifIo_type;
*/


image_type *	image_read_gif_file( char *file );

image_type *	image_read_gif_buffer( char *data, int bytes, int Fdata );

int				image_read_gif_is_valid_SINGATURE( char *data, int bytes );



image_type *	image_read_gif_file_VimatixData( char *file, unsigned char *pVimatixData );

image_type *	image_read_gif_buffer_VimatixData( char *data, int bytes, int Fdata,
													unsigned char *pVimatixData );


gifIo_type *	image_read_gif_open_file( char *file );

gifIo_type *	image_read_gif_open_buffer( char *data, int bytes, int Fdata );

gifIo_type *	image_read_gif_open( gio_type *gio );

int			image_read_gif_next_frame( gifIo_type *gifIo );

// reads the i'th frame
int			image_read_gif_i( gifIo_type *gifIo, int frame_i );

void			image_read_gif_close( gifIo_type *gifIo );

image_type *	image_read_gif( gio_type *gio );

image_type *	image_read_gif_imageData( gifIo_type *gifIo,
									int *top, int *left, palette_type **pal );

void			image_read_gif_imageData_skip( gifIo_type *gifIo );

void			image_read_gif_get_frame_no( gifIo_type *gifIo );

int				image_read_gif_info( char *file, int *row, int *column, int *frame_no, palette_type **pal );


int				image_gif_get_frame_no( char *data, int bytes, int Fdata );


#ifdef __cplusplus
}
#endif

#endif
