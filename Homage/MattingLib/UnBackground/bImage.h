
#ifndef _BIMAGE_H_
#define _BIMAGE_H_

#include "ImageType/ImageType.h"


typedef	struct bImage_type {
	int n;
	int	r;
	int g;
	int b;
} bImage_type;



image_type * bImage_set( image_type *sim, image_type *mim, box2i_type *b, int N, image_type *bim );

image_type * bImage_set( image_type *sim, box2i_type *b, int N, image_type *bim );

void	bImage_mask8( image_type *bim, image_type *mim );

void	bImage_mask( image_type *bim, image_type *mim, box2i_type *b, int N );

image_type *	bImage_realloc(  box2i_type *b, int n, image_type *bim );


void	bImage_clear( image_type *bim );

void	bImage_clearN( image_type *bim );

image_type *	bImage_copy( image_type *bim, image_type *tim  );

image_type *	bImage_to_image( image_type *bim, image_type *im );

image_type *	bImage_to_image1( image_type *bim, image_type *im );
image_type *	bImage_to_image3( image_type *bim, image_type *im );


void	bImage_dump( image_type *bim, int N, char *prefix, int index, char *suffix );

image_type *	bImage_grad_x_c( image_type *bim, image_type *im );

image_type *	bImage_grad_y_c( image_type *bim, image_type *im );

image_type *	bImage_grad_c( image_type *bim, image_type *im );



image_type *	bImage_grad_x( image_type *bim, image_type *im );



void	bImage_set_sbA( image_type *bim, struct sbA_type *as );

void	bImage_set_sbA_sideL( image_type *bim, sbA_type *as, int d );

void	bImage_set_sbA_sideR( image_type *bim, sbA_type *as, int d );


void	bImage_set_sb( image_type *bim, int i0, struct sb_type *s );

int		bImage_fill_gap( image_type *bim, int a[], int side );

int		bImage_fill_gap( image_type *bim, int a[], int i0, int i1, int side );



#ifdef _DUMP
#define BIMAGE_DUMP( im, N, name, index, ext )  bImage_dump( im, N, name, index, ext )
#else
#define BIMAGE_DUMP( im, N, name, index, ext )
#endif

#endif


