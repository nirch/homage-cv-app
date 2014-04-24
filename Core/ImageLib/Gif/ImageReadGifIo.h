/****************************
 ***	GifIo_type.h   ***
 ****************************/
#ifndef	_GIFIO_
#define _GIFIO_

#include "ImageType/ImageType.h"

typedef struct gifIo_type {
	
	int	width;		// column

	int	height;		// row

	int	top;

	int	left;

	int	frame_no;

	int	number_of_frames;

	int BackgroundColor;

	int	*transparent_flag;

	int	*transparent_index;

	palette_type	*palette;

	image_type	*current_image;

	FILE	*fp;

} gifIo_type;


image_type *	image_read_gif( char *file );

gifIo_type *	image_read_gif_open( char *file );

image_type *	image_read_gif_next( gifIo_type *gifIo );

void			image_read_gif_close( FILE *fp );

image_type *	image_read_gif_frame( gifIo_type *gifIo );

void			image_read_gif_frame_skip( FILE *fp );

// get number of frames
int				image_read_gif_get_frame_no( char *file);

// get number of frames
int				image_read_gif_get_frame_no2( char *file);

int				image_read_gif_next_skip( gifIo_type *gifIo );

image_type *	image_read_gif_frame_by_number( FILE *fp, int frame );

void			image_read_gif_next_frame( gifIo_type *gifIo );

gifIo_type *	image_read_gif_info( char *file );

int				image_read_extension( gifIo_type *gifIo );

int				image_read_transparent( gifIo_type *gifIo );

#endif
