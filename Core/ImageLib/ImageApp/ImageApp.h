#ifndef         _IMAGE_APP_
#define         _IMAGE_APP_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"

#include	"Ujet2/Jet2Type.h"
#include	"Ujet1/Jet1Type.h"

#include	"ImageType/ImageType.h"


#define	 APP_ZIZE	30

#define  JAPP_NORMAL	0
#define  JAPP_WIGHT	    1


typedef struct jet2App_type {
	int	type;		// JAPP_NORMAL | JAPP_WIGHT
	int		n;
	int		nh;
	float	dn;

	float	dn_inv;

	float	dxy;
	
	float	w[APP_ZIZE];

	float	a0[APP_ZIZE];
	float	a1[APP_ZIZE];
	float	a2[APP_ZIZE];


	float	t0;
	float	t1;
	float	t02;
	float	t2;


	float	f00;
	float	f10;
	float	f11;
	float	f20;
	float	f00_2;

}	jet2App_type;



jet2App_type *	jet2App_init( int n, int weight );

void	jet2App_destroy( jet2App_type *ja );


int	image_jet2App( image_type *im, int	i0,	int	j0, jet2App_type *ja, jet2 *f );

int	image1_jet2App( image_type *im, int	i0,	int	j0, jet2App_type *ja, jet2_type *f );

int	imageF_jet2App( image_type *im, int	i0,	int	j0, jet2App_type *ja, jet2_type *f );

int	imageUS_jet2App( image_type *im, int	i0,	int	j0, jet2App_type *ja, jet2_type *f );

int	image1_jet1App( image_type *im, int i0,	int	j0, jet2App_type *ja, jet1_type *f );

int	image1_jet1AppY( image_type *im, int	i0,	int	j0, jet2App_type *ja, jet1_type *f );


int	image1_jet1AppY_A( image_type *im, int i0, int j0, int n, jet2App_type *ja, jet1_type *f );



jet2App_type *	jet2AppW_init( int n, int weight );

jet2App_type *	jet2AppW_init_w( int n, float w[] );

int	image1_jet2AppW( image_type *im, int	i0,	int	j0, jet2App_type *ja, jet2_type *f );

int	imageF_jet2AppW( image_type *im, int	i0,	int	j0, jet2App_type *ja, jet2_type *f );

int	imageUS_jet2AppW( image_type *im, int	i0,	int	j0, jet2App_type *ja, jet2_type *f );

int	image1_jet1AppW( image_type *im, int i0,	int	j0, jet2App_type *ja, jet1_type *f );

int	image1_jet1AppWY( image_type *im, int	i0,	int	j0, jet2App_type *ja, jet1_type *f );



#ifdef __cplusplus
}
#endif

#endif
