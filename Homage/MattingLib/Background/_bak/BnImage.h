#ifndef _BN_IMAGE_H_
#define _BN_IMAGE_H_








typedef struct bn_type {
	u_char	flag;

	short	no;
} bn_type;



	// BnImage.cpp
void	bnImage_init( image_type *sim );

int		bnImage_no( image_type *sim );


image_type *	bnImage_average4( image_type *sim, image_type *im );

image_type *	bnImage_abs_diff( image_type *sim, image_type *sim1, image_type *im );


image_type *	image3_ncc8( image_type *sim, image_type *bim, int tVal, image_type *nim );



void			bnImage_dump(  image_type *sim, image_type *gvar, int ds, char *prefix, int iFrame, char *suffix, int flag );

image_type *	bnImage_dup(  image_type *sim, image_type *gvar, int ds, int flag, image_type *im );



#ifdef _DUMP 
#define BNIMAGE_DUMP( sim, gvar, ds, prefix, iFrame, suffix, flag ) \
					bnImage_dump( sim, gvar, ds, prefix, iFrame, suffix, flag ) 
#else
#define BNIMAGE_DUMP( sim, gvar, ds, prefix, iFrame, suffix, flag ) 
#endif

#endif 