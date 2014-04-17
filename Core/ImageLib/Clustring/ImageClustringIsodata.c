#include	<string.h>
#include	<stdio.h>
#include	<math.h>



#include	"Uigp/igp.h"


#include "ImageType/ImageType.h"
#include "ImageUS/ImageUSTool.h"
#include	"ImageDump/ImageDump.h"

#include "Histogram/Histogram.h"

#include "Clustring/ImageClustring.h"


static void	image1M_clustring_isodata( image_type *sim, image_type *mim, int NM, int nT, float vT, float mT, int Itration, float *aM, int *nM );
static void	imageFM_clustring_isodata( image_type *sim, image_type *mim, int NM, int nT, float vT, float mT, int Itration, float *aM, int *nM );



void
imageM_clustring_isodata( image_type *sim,  image_type *mim, int NM, int nT, float vT, float mT, int Itration, float *aM, int *nM )
{

	if( sim->format == IMAGE_FORMAT_UC ){
		image1M_clustring_isodata( sim, mim, NM, nT, vT, mT, Itration, aM, nM );
		return;
	}

	if( sim->format == IMAGE_FORMAT_F ){
		imageFM_clustring_isodata( sim, mim, NM, nT, vT, mT, Itration, aM, nM );
		return;
	}
}


static void
image1M_clustring_isodata( image_type *sim, image_type *mim, int NM, int nT, float vT, float mT, int Itration, float *aM, int *nM )
{
int	H[256];

	image1M_histogram( sim, mim, H );

	histogram_clustring_isodata( H, 256, NM, nT, vT, mT, Itration, aM, nM );
}



static void
imageFM_clustring_isodata( image_type *sim, image_type *mim, int NM, int nT, float vT, float mT, int Itration, float *aM, int *nM )
{
float	m0,	m1;
int	H[4096];
int	i;

	imageFM_minmax( sim, mim, &m0, &m1 );

	imageFM_histogram( sim, mim, m0, m1, H );



	histogram_clustring_isodata( H, 4096, NM, nT, vT, mT, Itration, aM, nM );


	for( i = 0 ; i < *nM ; i++ ){
		aM[i] = m0 + aM[i]*( m1 - m0 )/ 4096;
	}

}

