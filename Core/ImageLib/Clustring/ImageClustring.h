/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef         _IMAGE_CLUSTRING_
#define         _IMAGE_CLUSTRING_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"


	// ImageClustringIsoData.c
void	imageM_clustring_isodata( image_type *sim,  image_type *mim, int NM, int nT, float vT, float mT, int Itration, float *aM, int *nM );



#ifdef __cplusplus
}
#endif

#endif
