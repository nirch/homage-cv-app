#ifndef         _IMAGE_READ_PSD_
#define         _IMAGE_READ_PSD_



#include	"PsdType.h"


	/* ImageReadPSD.c */
struct image_type *image_read_PSD_file( char *file );

struct image_type *image_read_PSD_buffer( char *data, int bytes, int Fdata );

int image_read_PSDlayers_number( char *file );


int		image_read_PSDLayer_file( char *file, psdLayer_type **imL, int *row, int *column );




#endif
