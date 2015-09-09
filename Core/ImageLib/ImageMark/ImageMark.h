#ifndef         _IMAGE_MARK_
#define         _IMAGE_MARK_


#ifdef __cplusplus
extern "C" {
#endif





#include	"Uigp/igp.h"
#include "ImageType/ImageType.h"



#ifndef BLACK
#define		BLACK	0x000000
#endif

#ifndef WHITE
#define		WHITE	0xffffff
#endif

#ifndef RED
#define		RED		0xff0000
#endif

#ifndef GREEN
#define		GREEN	0x00ff00
#endif

#ifndef BLUE
#define		BLUE	0x0000ff
#endif

#ifndef CYAN
#define		CYAN	0x00ffff
#endif

#ifndef MAGENTA
#define		MAGENTA	0xff00ff
#endif

#ifndef YELLOW
#define		YELLOW	0xffff00
#endif



	// ImagePrint.c
void	image1_print( image_type *sim, int i0, int j0, int n );

void	imageF_print( image_type *sim, int i0, int j0, int n );

void	image2_print( image_type *sim, int i0, int j0, int n );


	// ImageDraw.c
void	image4_draw_point( image_type *im, int x, int y, int color );

void	image4_draw_pointA( image_type *im, int x, int y, int color );


void	image4_draw_circle( image_type *sim, float x, float y, float r, int color );

void	image_mask_circle( image_type *im, float x, float y, float r, int color );

void	image4_draw_rectangle( image_type *im, int x, int y,
							  int col, int row, int color );

void	image_draw_rectangle( image_type *im, int x0, int y0, int dx, int dy, int color );


void	image_draw_line( image_type *im, float x0, float y0, float x1, float y1, int color );

void	image4_draw_line( image_type *im, float x0, float y0, float x1, float y1, int color );



void	image_draw_strip( image_type *im, vec2d *p0, vec2d *p1, float tm, float um, int color );


void	image4_draw_skeleton( image_type *im, vec2d p[], int color, int color1 );


void	image_draw_ellipse( image_type *im, float x, float y,
				   float r1, float r2, float angle, int color );


void	image1_draw_ellipse_fill( image_type *im, float x0, float y0, float Rx, float Ry, float angle );



#ifdef __cplusplus
}
#endif

#endif
