#ifndef         _IMAGE_APP2_
#define         _IMAGE_APP2_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"

#include	"Ujet2/Jet2Type.h"
#include	"Ujet1/Jet1Type.h"

#include	"ImageType/ImageType.h"

#include "ImageApp1.h"





typedef struct jet2App2_type {

	
	jet1App_type	x;

	jet1App_type	y;


	//float	f00;
	//float	f10;
	//float	f11;
	//float	f20;
	//float	f00_2;

}	jet2App2_type;



jet2App2_type *	jet2App2_init( int nx, int ny, int weight );

void	jet2App2_destroy( jet2App2_type *ja );



int		image_jet2App2( image_type *im, int	i0,	int	j0, jet2App2_type *ja, jet2 *f );

int		image1_jet2App2( image_type *im, int	i0,	int	j0, jet2App2_type *ja, jet2 *f );

int		imageF_jet2App2( image_type *im, int i0, int j0, jet2App2_type *ja, jet2 *f );


#ifdef __cplusplus
}
#endif

#endif
