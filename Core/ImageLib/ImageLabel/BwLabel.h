#ifndef         _BW_LABEL_
#define         _BW_LABEL_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"
#include "Ubox/Box2d.h"

#include "ImageType/ImageType.h"

typedef struct lbm_type {
	short i;
	short j0;
	short j1;
	u_short id;
} lbm_type;



	// Image2BwLabel.c
typedef struct bwLabel_type {
	short	color;
	int	id;
	int	no;

	vec2d	p;

	float	e1,	e2;
	vec2d	v1;


	short	i,	j;	//first pixel
	box2i	b;

	int	boundary;
	float	av;
	float	var;
	union {
		struct {
			int		R,	G,	B;
		};
		int rgb[3];
	};
	

	int existence;

	int	nB,	nE;

}	bwLabel_type;



typedef struct imageLabel_type {
	image_type	*mim;   // Original image from which blobs were created
	int	margin;


	image_type *im;     // Per pixel holds the id of the relevant blob

	int	NA;
	int	nA;
	bwLabel_type *a;    // Array of blobs

} imageLabel_type;





void	image2_bwLabel( image_type *im, bwLabel_type **tbw, int *tnBw );


void	image2_bwLabel_remove_boundary( image_type *im, bwLabel_type *bw );



void	imageLabel2_set_boundary( image_type *im, bwLabel_type *abw, int nB );

void	imageLabelUS_set_boundary( imageLabel_type *abw );


void	imageLabel2_set_value( image_type *sim, image_type *im, bwLabel_type *abw, int nBw );

void	image2_bwLabel_set_aux( image_type *im, bwLabel_type *aBw, int nBw );

void	imageLabelUS_set_box( imageLabel_type *abw );

void	imageLabelUS_set_mass( imageLabel_type *abw );



void	image2_bwLabel_set_direction( image_type *im, bwLabel_type *bw, int nBw );


int		bwLabel_gesture_id( bwLabel_type *bw, int nBw, int T );












image_type *	image1_connect_component( image_type *sim, int T, bwLabel_type **abw, int *nBw, image_type *im );



#ifdef __cplusplus
}
#endif

#endif
