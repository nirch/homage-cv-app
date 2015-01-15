//  Defines the entry point for the console application.
//
#include <stdio.h>
#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include "Ulog/Log.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"
#include "ImageMorphology/ImageMorphology.h"
#include "Imagelabel/BwLabel.h"

#define DIFF_THRESH 16// 12


#include	"UnBackground.h"

#include	"BnImage/BnImage.h"

#include "ImageLabel/ImageLabel.h"


#include "bImage.h"


int CUnBackground::TestDrakness( image_type *sim )
{
	int	H[256];
	int	pixles;

	int	i;

	image1_histogram( sim, NULL, H, 1 );

	pixles = sim->width*sim->height;


	for( i = 1 ; i < 256 ; i++ )
		H[i] += H[i-1];


	int pixels = H[255];

	unDarkPrm_type *d = &m_prm->d;

	if( H[d->T0] > d->F1* pixels  && H[d->T1] > d->F1 *pixels )
		return( 1 );


	return( -1 );
}