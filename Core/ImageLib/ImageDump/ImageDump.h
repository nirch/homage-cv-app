#ifndef         _IMAGE2_DUMP_
#define         _IMAGE2_DUMP_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"

#include	"ImageType/ImageType.h"

void	image_dump_set_dirR( char *baseDir, char *dump_dir );
void	image_dump_set_dir( char *dump_dir );

char *	image_dump_get_Dir( char **dump_dir );

void	image_dump( image_type *im, char *name, int index, char *ext );

void	imageF_dump( image_type *sim, int stretch, char *name, int index, char *suffix );

void	imageF_dumpAB( image_type *sim, float a, float b, char *name, int index, char *suffix );


void	image2_dump( image_type *sim, char *name, int index, char *suffix );


void	image_dump_delete( char *prefix, char *suffix  );


void	image_dump_scale( image_type *im, float scale, char *prefix, int index, char *suffix );

void	image_dump_dup( image_type *sim, int dup, float a, char *name, int index, char *ext );



void	image_dump_band( image_type *sim, int iBand, char *name, int index, char *suffix );


void	image_dump_alpha( image_type *sim, char *name, int index, char *suffix );

void	imageT_dump( image_type *sim, char *name, int index, char *suffix );



#ifdef _DUMP
#define IMAGE_DUMP( im, name, index, ext )  image_dump( im, name, index, ext )
#define IMAGEF_DUMP( im, stretch, name, index, ext )  imageF_dump( im, stretch, name, index, ext )
#define IMAGE_DUMP_SCALE( im, scale, name, index, ext )  image_dump_scale( im, scale, name, index, ext )
#define IMAGEF_DUMPAB( im, a, b, name, index, ext )  imageF_dumpAB( im, a, b, name, index, ext )
#define IMAGE_DUMP_ALPHA( im, name, index, ext )  image_dump_alpha( im, name, index, ext )
#define IMAGET_DUMP( im, name, index, ext )  imageT_dump( im, name, index, ext )
#define IMAGE_DUMP_DUP( im, dup, a, name, index, ext )  image_dump_dup( im, dup, a, name, index, ext )

#else
#define IMAGE_DUMP( im, name, index, ext )
#define IMAGEF_DUMP( im, stretch, name, index, ext )
#define IMAGE_DUMP_SCALE( im, scale, name, index, ext )
#define IMAGEF_DUMPAB( im, a, b, name, index, ext )
#define IMAGE_DUMP_ALPHA( im, name, index, ext )
#define IMAGET_DUMP( im, name, index, ext )
#define IMAGE_DUMP_DUP( im, dup, a, name, index, ext )

#endif


#ifdef __cplusplus
}
#endif

#endif
