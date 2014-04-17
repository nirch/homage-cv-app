#ifndef _BN_STATE_H_
#define _BN_STATE_H_



typedef struct bgStage_type {

	image_type	*rgbBim;

	image_type	*bim;

	image_type	*bim4;

	int	iFrame;

}	bgStage_type;



bgStage_type	*bgStage_alloc();

void	bgStage_destroy( bgStage_type *bg );


bgStage_type *	bgStage_set( bgStage_type *bg, image_type *rgbBim, image_type *bim, image_type *bim4, int iFRame );

void	bgStage_restore( bgStage_type *bg, image_type **rgbBim, image_type **bim, image_type **bim4, int *iFrame );

#endif 