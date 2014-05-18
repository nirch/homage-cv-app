/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef         _IMAGE_TOOL_
#define         _IMAGE_TOOL_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"
#include	"Umat/Mat2fType.h"
#include	"Uvec/Vec2d.h"
#include	"Ubox/Box2d.h"


//#define	image_destory	image_destroy

#include	"Image1/Image1Tool.h"
#include	"Image3/Image3Tool.h"
#include	"Image6/Image6Tool.h"
#include	"Image2/Image2Tool.h"

#include	"ImageF/ImageFTool.h"
#include	"ImageC/ImageCTool.h"
#include	"ImageUS/ImageUSTool.h"
#include	"ImageUI/ImageUITool.h"

#include	"Bmp/ImageBmp.h"

#include	"ImageResize/ImageResize.h"



	// ImageAux.c
//void	image_box(image_type *im, box2i *box, int *x0, int *y0, int *width, int *height );
void	imageA_box(image_type *im, box2i *box, int *x0, int *y0, int *x1, int *y1 );

void	imageA_icycle( image_type *aIm[], int N );
void	imageA_cycle( imageA_type *aim );

void	imageA_ocycle( imageA_type *aim );



	/* Image4to1.c */
image_type *	image_1to4( image_type *sim );
image_type *	image_4to1( image_type *sim );


	/* Image4Tool.c */
image_type *	image4_from( image_type *sim, image_type *im );



void		image4_mark( image_type *im, int row0, int col0,
					int row, int col, int color );

void		image4_const( image_type *im, int color );

void		image4_Vflip( image_type *im );

void		image4_Hflip( image_type *im );

image_type *	image4_Vflip2( image_type *sim, image_type *tim );

image_type *    image4_Hflip2( image_type *sim, image_type *tim );

//void		image4_copy_in( image_type *sim,
//				image_type *im, int row0, int col0 );
image_type *	image4_copy_dup( image_type *sim,
								int x0, int y0, int dw, int dh, int width, int height, image_type *im );



image_type * 	image4_copy( image_type *sim );

image_type *	image4_to_red( image_type *sim );

image_type *	image4_to_green( image_type *sim );

image_type *	image4_to_blue( image_type *sim );

image_type *	image4_to_y( image_type *sim, image_type *im );

image_type *	image4_from_y( image_type *sim );
image_type *	image4_from_image1( image_type *sim, image_type *im );


image_type *	image4_from_rgb( image_type *imR, image_type *imG, image_type *imB, image_type *im );


image_type *	image4_rotate90( image_type *sim, image_type *tim );

image_type *	image4_rotate180( image_type *sim, image_type *tim);

image_type *	image4_rotate270( image_type *sim, image_type *tim );


		// Image4CopyIn.c
int				image4_copy_in( image_type *sim, int x0, int y0, image_type *im );
int				image4_copy_in_VFLIP( image_type *sim, int x0, int y0, image_type *im );


//	// ImageSample.c
//image_type *		image_sample( image_type *sim, int d, image_type *im );
//
//image_type *	image4_sample( image_type *sim, int d, image_type *im );
//
//image_type *	image4_dup( image_type *sim, int d, image_type *im );
//
//image_type *	image4_sample2( image_type *sim, image_type *im );


	/* ImageResample.c */
//image_type *	image_resample( image_type *sim, int row, int col );


//	/* ImageSampleG.c - float version grisha*/ 
//image_type *	image_sampleG( image_type *sim, float srow, float scol );
//
//image_type *	image_sampleG_transparent( image_type *sim, float srow, float scol );





//	/* ImageSamplingF.c - two pass float */
//image_type * image_samplefc( image_type *sim, int drow, int dcol );
//

//	/* ImageSampleDown.c */
//image_type * image_sample_down( image_type *sim, int drow, int dcol );

//	/* ImageSampleDownTransparent.c */
//image_type * image_sample_down_transparent( image_type *sim, int drow, int dcol );
//void image_sample_down_fix_transparent( image_type *im );


	/* ImageCut.c */
image_type *	image_cut( image_type *sim, int row0, int col0, int row, int col, image_type *im );

image_type *	image4_cut( image_type *sim, int row0, int col0, int row, int col, image_type *im );

	// ImageCrop.c
image_type *	image_crop( image_type *sim, int x0, int y0, int width, int height, image_type *im );

image_type *	image_crop_B( image_type *sim, box2i *b, image_type *im );



	// ImageSubtruct.c 
image_type *	image_subtract( image_type *sim1, image_type *sim0, image_type *im );
image_type *	image4_subtract( image_type *sim1, image_type *sim0, image_type *im );


	// ImageAdd.c
image_type *	image_add( image_type *sim0, image_type *sim1, float a0, float a1, float b );


	/* ImageRgbYCrCb.c */
//image_type *	image_Rgb_YCrCb( image_type *im, int Falloc );
image_type *	image_Rgb_YCrCb( image_type *sim, image_type *im );


	/* ImageRgbYiq.c */
image_type *image_Rgb_Yiq( image_type *im, int Falloc );


	/* ImageComparison */
void	image_comparison( image_type *im0, image_type *im1,
                float var[],    /* Variabilty */
                float dev[],
                float snr[],    /* Signal to noise ratio */
                float average[],
                int peak[] );

void	image4_comparisonG( image_type *im0, image_type *im1, float *var, float *dev, float *average );


	/* ImageEqual.c   */
int		image_equal( image_type *im0, image_type *im1 );


	/* ImageCutUarray.c */
void	image4_cut_uarray( image_type *im,
                        int x0, int y0, int row, int column,
                        u_char *aY,
                        u_char *aQ,
                        u_char *aA,
                        int acolumn);



	/* ImageFile.c */
void	image_file_attach_name( image_type *im,
			char *dir, char *name, char *extension );

void	image_file_attach_nameF( image_type *im, char *fname );

int	image_file_size( image_type *im );



        /* ImagePreview.c */
image_type *    image_preview( image_type *im, int size, int color );


        /* ImageTransparent.c */
image_type *    image_transparent(image_type *sim,
                float trans, int backcolor, int flipV, image_type *im ); 

image_type *	image_transparent_B( image_type *sim, int x0, int y0, int width, int height,
								float trans, int backcolor, image_type *im );


void	image_transparent_set(image_type *bim, box2i *b,
						image_type *im, int transpart_index );

void	image_transparent_copy_in(image_type *bim, box2i *b,
						image_type *im, int transpart_index );

void	imageT_set_by_color( image_type *sim, int color );

void	imageT_clear( image_type *im, int color );

image_type * imageT_copy_alpha( image_type *sim, image_type *im );

image_type * imageA_set_alpha( image_type *sim, int transparent, image_type *mim, image_type *im );

image_type * imageA_set_color( image_type *sim, image_type *mim, int transparent, int color, image_type *im );

image_type *imageA_set_colorN( image_type *sim, image_type *mim, int color, image_type *im );

image_type *imageA_set_backgorund( image_type *sim, image_type *mim, image_type *bim, image_type *im );




void	imageT_rectangle( image_type *im,  int *i0, int *i1, int *j0, int *j1 );

image_type *	imageT_negative_alpha( image_type *sim,	image_type *im );

image_type *	imageT_alpha64_stretching( image_type *sim, image_type *im );



void	imageA_copy_in( image_type *sim, int x0, int y0, image_type *im );



image_type * imageA_final( image_type *sim, int color, image_type *im );

image_type * imageA_finalB( image_type *sim, int color, image_type *im );




image_type *    image_border_fade(image_type * sim,
								float trans, int backcolor,
								int r0, int c0, int row, int column, image_type *im); 


void image_transparent_copy(image_type * sim,
						int r0, int c0, int row0, int col0, image_type *im);

image_type *	imageT_clear_color( image_type *sim, int color, image_type *im );

void	imageT_clearN( image_type *image );





	/* ImageEdgePixel.c */
image_type *	image_edge_pixel( image_type *sim, mat2f_type *m, float T );
image_type *	image_edge_pixel_value( image_type *sim, mat2f_type *m, float a, float b );

image_type *	image_edge_gradient( image_type *sim, mat2f_type *mx, mat2f_type *my, float T );
image_type *	image_edge_gradient_value( image_type *sim, mat2f_type *mx, mat2f_type *my, float a, float b );


	/*ImageSline.c */
image_type *	image_sline(  vec2d *n, float d0, float d1, float a,
							int c0, int c1, int c2, int row, int col );
image_type *	image_S_cosinos(  float a, float n, int row, int col );





	/* ImageFourier.c */
image_type *	image_fourier_amplitod( image_type *im ); 

image_type *	image_fourier(image_type *im);

image_type *	image_fourier_inverse(image_type *im);



    /* Image24To8.c */
image_type * image_24to8(image_type * im, palette_type * palette, box2i * pBox );

image_type * image_24to8_UNKNOWN(image_type * im, palette_type * palette, box2i * pBox);

image_type * image_24to8_smart(image_type * im, palette_type * palette, box2i * pBox);

image_type * image_24to8_222(image_type * sim, palette_type * p, box2i * pBox);

image_type * image_24to8_gray(image_type * sim, palette_type * p, box2i * pBox);

image_type * image_24to8_HALFTONE(image_type * sim, palette_type * p, box2i * pBox);

image_type * image_24to8_BW(image_type * im, palette_type * p, box2i * pBox);

image_type * image_24to8_WB(image_type * im, palette_type * p, box2i * pBox);

image_type *	image_24to8_ADAPTIVE4(image_type * im, palette_type * palette, box2i * pBox);

void    image_difference(image_type * im1, image_type * im2, box2i * b);


    /* Image24To8DitherFloyd.c */
image_type *	image_24to8_dithering_floyd(image_type * im, palette_type * palette, box2i * pBox );

image_type *	image_24to8_UNKNOWN_dithering_floyd(image_type * im, palette_type * palette, box2i * pBox );

image_type *	image_24to8_smart_dithering_floyd(image_type * im, palette_type * palette, box2i * pBox );

image_type *	image_24to8_222_dithering_floyd(image_type * im, palette_type * palette, box2i * pBox );

image_type *	image_24to8_gray_dithering_floyd(image_type * sim, palette_type * p, box2i * pBox);

image_type *	image_24to8_HALFTONE_dithering_floyd(image_type * im, palette_type * palette, box2i * pBox );

image_type *	image_color_222_dithering_floyd( image_type *im, image_type *new_im );


    /* ImageDitherFloyd.c */
image_type *	image_dithering_floyd(image_type * im, image_type * new_im);


	//	Image8To24.c
image_type *	image_8to24( image_type *sim, palette_type *palette, image_type *im );
image_type *	image_8to24_transparent( image_type *sim, int transparent_index, palette_type *p );

void	image_8to24_copy( image_type *sim, image_type *im, 
						 int row0, int col0, palette_type *p );
void	image_8to24_copy_transparent( image_type *sim, image_type *im, 
									 int row0, int col0, int transparent_index, palette_type *p);


	/* ImageSmartPalette.c */
palette_type *	image_createSmartPalette(int n, image_type * im );

palette_type *	image_createSmartPaletteTransparent( int n, int nImages, image_type *images[] );
palette_type *	image_createSmartPaletteTransparent2( int n, int nImages, image_type *images[] );


	/* ImageAdaptivePalette.c */
void	image_adaptive_palette( image_type *images[], int nImage,
					   palette_type *palette, int nColor );


	/* ImageDifference.c */
void	image_difference_rectangle( image_type *im0, image_type *im1,
							int *i0, int *i1, int *j0, int *j1 );

void	image1_difference_rectangle( image_type *im0, image_type *im1,
							int *i0, int *i1, int *j0, int *j1 );


void	image_color_rectangle( image_type *im, int color,
							  int *i0, int *i1, int *j0, int *j1 );



	/* ImageExtend.c */
image_type *	image_extend( image_type *im, int row, int column, int color, image_type *rim );
image_type *	image_border_rect(image_type * im, int backcolor, int cycle, 
				  int r0, int c0, int row0, int col0);


	/* ImageEnhancement.c */
#define ENHANCEMENT_OBJECT		0
#define ENHANCEMENT_TEXT		1
#define ENHANCEMENT_BACKGROUND	2

image_type *	image_enhancement( image_type *sim, int enhan_type );


	/* ImageGrayEnhancement.c */
image_type *	image_gray_enhancement( image_type *sim, int enhan_type );


	/* ImageNegative.c		   */
void image_negative( image_type *im );


	/* ImageResize.c		   */
//image_type *	image_resize( image_type *imd, image_type *ims, int row, int col );

	/* ImageHsiTune.c*/
image_type *	image_HSI_tune( image_type *sim, float HUE, float Saturation, float Intensity );
float	image_Saturation( image_type *sim );

	/* ImageLogo.c */
void image_add_logo( image_type *sim, image_type *logo, int x, int y, int opacity );

	/* ImageFields.c */
void image_to_fields( image_type *sim, image_type **oddField, image_type **evenField );

//image_type * image_from_fields( image_type *oddField, image_type *evenField );
image_type *image_from_fields(image_type *oddField, image_type *evenField, image_type *im );


image_type *	image_field( image_type *sim, int field, int half  );

image_type *	image_field_half( image_type *sim, int field, image_type *im );

	/* ImageStretch.c */
image_type *	image_stretch_color( image_type *sim );
image_type *	image1_stretch_color( image_type *sim );


	/* ImagePyramid.c */
image_type *	image1_pyramid( image_type *im );


	// ImageAverage.c
image_type *	image_average( image_type *images[], int nImage );



	// ImageAlpha.c
image_type *	image_alpha_copy( image_type *sim, int margin );

void	image_alpha_clear( image_type *image );



void	image_background_removal( image_type *bim, image_type *im, int rgbT, float Ymin );




	// Guasian
void	guasian( int n, float sigma, float g[] );

void	guasian_dg( int n, float sigma, float g[], float dg[] );



image_type *image_mask( image_type *sim, image_type *mim, image_type *im );
image_type *image1_mask( image_type *sim, image_type *mim, image_type *im );


	// ImageInterlive.c
image_type *	image_interlive( image_type *aim[], int nBand, image_type *im );

	// ImageBand.c
image_type *	image_band( image_type *sim, int iBand, image_type *im );

	// ImageFlipH.c
void			image_flipH( image_type *sim );

	// ImageFlipV.c
void			image_flipV( image_type *sim );

image_type *	image_flip90( image_type *sim, image_type *im );


	// ImageDewarpLt2.c
image_type *	image_dewarp_lt2( image_type *sim, struct lt2_type *lt, image_type *im );


	// ImageResizeIpp.c
//image_type *	image_resize_ipp( image_type *sim, int width, int height, int mode, image_type *im ) ;



	// ImageIo.c
int	image_write_im( image_type *im, char *file );

image_type *	image_read_im( char *file );

image_type *	image_read_raw( char *file, int width, int height, int foramt );


	// ImagePixel.c
int		image_pixel_value( image_type *sim, int i, int j, int *r, int *g, int *b );

	// ImageHistogram.c
int	image_histogram( image_type *sim, box2i_type *box, int h[], int fClear );

int	imageA_histogram( imageA_type *aim, box2i_type *box, int **h, int *nH );


	// Histogram.c
void	histogram_range( int h[], int nH, float p0, float p1, float *r0, float *r1 );


	// ImageToY.c
image_type *		image_to_y( image_type *sim, image_type *im );

	// ImageToR.c
image_type *		image_to_r( image_type *sim, image_type *im );



	// ImageLevel.c
image_type *	imageM_level( image_type *sim, image_type *mim, float hp, int nLevel, float level[], image_type *im );
image_type *	image1M_level( image_type *sim, image_type *mim, float hp, int nLevel, float level[], image_type *im );
image_type *	imageFM_level( image_type *sim, image_type *mim, float hp, int nLevel, float level[], image_type *im );


image_type *	image1_level( image_type *sim, box2i *box, float hp, int nRange, float range[], image_type *im );


image_type *	imageM_levelN( image_type *sim, image_type *mim, float aL[], int nL, image_type *im );

image_type *	image_levelN( image_type *sim, float aL[], int nL, image_type *im );


	// ImageAppend.c
image_type *	image_append_horizontal( image_type *sim0, image_type *sim1, image_type *im );

image_type *	image_append_vertical( image_type *sim0, image_type *sim1, image_type *im );


	// ImageLinearCombination.c
image_type *		image_linear_combination( image_type *sim, float a, float fR, float fG, float fB, image_type *im );

image_type *	image3_linear_combination( image_type *sim, float a, float fR, float fG, float fB, image_type *im );

image_type *	imageUS3_linear_combination( image_type *sim, float a, float fR, float fG, float fB, image_type *im );


	// imageInterpolationPv.c
image_type *	image_interpolation_pv( image_type *sim, vec2f_type *p, vec2f_type *v, int w, int h, image_type *im );


	// ImageCmyk.cpp
image_type *	image3_to_cmyk( image_type *sim, image_type *im );

image_type *	image_cmyk2rgb( image_type *sim, image_type *im );



#ifdef __cplusplus
}
#endif

#endif