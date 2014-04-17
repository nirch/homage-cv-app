#ifndef         _IMAGE_LABEL_
#define         _IMAGE_LABEL_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"
#include	"Uln/Cln/ClnType.h"

#include "BwLabel.h"


		// ImageLabel.c
imageLabel_type *imageLabel_alloc();

void imageLabel_destroy( imageLabel_type *abw );

int imageLabel_no( imageLabel_type *abw );

int	imageLabel_bigest( imageLabel_type *abw, int color );


int	imageLabel_print( imageLabel_type *abw, FILE *fp );

int	imageLabel_printA( bwLabel_type *bw, int nBw, FILE *fp );



	// Image1Label.c
imageLabel_type *imageLabelUI( image_type *sim, imageLabel_type *abw );

imageLabel_type *imageLabelUI_T( image_type *sim, int T, int margin, imageLabel_type *abw );

image_type *	image1_label( image_type *sim, bwLabel_type **tbw, int *tnBw, image_type *im );

void	imageLabelUI_filter( image_type *sim, image_type *lim, bwLabel_type *aBw, int nBw, int nT );

//int		imageLabel_print( bwLabel_type *bw, int nBw, FILE *fp );

void	imageLabelUI_set_id( imageLabel_type *abw );

void	imageLabelUI_set_aux( image_type *im, bwLabel_type *aBw, int nBw );

int		imageLabelUI_eigen2d( image_type *im, int id, box2i_type *b, struct eigen2d_type *e );



	// Image1Blob.c
imageLabel_type *imageLabelUS( image_type *sim, int T, int inv, imageLabel_type *abw );

int		imageLabelUS_set_pixel( image_type *sim, imageLabel_type *abw, int value );

void	imageLabelUS_create( image_type *im, bwLabel_type **tbw, int *tnBw );




	// Image1InletFilter.c
int		imageLabel_inlet_filter( image_type *sim, int n );





	// Image1Blob.c
void	image1_remove_blob( image_type *sim, int nP, int nT, int value );

void	image1_remove_blobM( image_type *sim, int nP, int pT, int bT, int value );


//void	image1_fill_blob( image_type *sim, int nP, int nT, int value );
void	image1_fill_blob( image_type *sim, int nP, int pT, int bT, int value );

void	image1_fill_blobV( image_type *vim, image_type *sim, int nP, int pT, int bT, int vT, int value );


	// ImageLabelContour.cpp
cln_type *	imageLabelUS_contour( image_type *im, int id );


#ifdef __cplusplus
}
#endif

#endif
