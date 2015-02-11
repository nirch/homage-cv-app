/********************
 ***	GifIo.h   ***
 ********************/
#ifndef	_GIF_IO_
#define _GIF_IO_

#include	"ImageType/ImageType.h"
#include	"Ugio/GioType.h"


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
	int	prev_delay_time;
	int	avrageDelay;

	int	frameTime;
	int	duration;

	int	*ftell_delayTime;

	float	dev;	//variabllty

	int	sizeLimit;	// for write
	int	Fdither;	// 24to8 conversion with dithering

	palette_type	*palette;

	image_type	*im;	// current image
	image_type	*im8;	// current index image

	unsigned char *pVimatixData;

	gio_type *gio;

	FILE	*fp;	// for write 

} gifIo_type;



#endif
