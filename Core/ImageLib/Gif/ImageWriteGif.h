/****************************
 ***	ImageWriteGif.h   ***
 ****************************/
#ifndef	_IMAGE_WRITE_GIF_
#define _IMAGE_WRITE_GIF_

#include "ImageType/ImageType.h"
//#include "ImageType/PaletteTypeHash.h"

#include	"GifIo.h"

//
//gifIo_type *	image_write_gifIo_open_file( char *file, int height, int width,
//							int frameDelay,
//							palette_type *palette, int transparent_index, int Fdither,
//							int sizeLimit );
//

void	image_write_gif_VimatixData( FILE *fp, const u_char *pVimatixData );

int		image_write_gifIo_add_frame( gifIo_type *gifIo, image_type *im, int FDestroyIm);

int		image_write_gifIo_close( gifIo_type *gifIo );


#include	"GifIo.h"


gifIo_type *	image_write_gifIo_open_file( char *file, int height, int width,
							int frameDelay,
							palette_type *palette, int transparent_index, int Fdither,
							int sizeLimit );

int		image_write_gifIo_add_frame( gifIo_type *gifIo, image_type *rim, int Fimage );

int		image_write_gifIo_close( gifIo_type *gifIo );



void	image_write_gif_warning_dev( gifIo_type *gifIo );


void	image_write_gif_set_frameDelay( gifIo_type *gifIo, int frameDelay );

int	image_write_gif_set_frameDelayA( gifIo_type *gifIo, int delayTime[] );



int		image_write_gif( image_type *im, palette_type *palette, char *dir, char *name );

FILE *	image_write_gif_open( char *file,int height, int width, palette_type *p );

int		image_write_gif_add_frame(FILE * fp, image_type *im,
							image_type * rim, palette_type *palette,
							int sizeLimit, int delay, int Fdither );

int		image_write_gif_GraphicControl( gifIo_type *gifIo, int DelayTime, int transparentIndex );

int		image_write_gif_close( FILE *fp );


#endif


