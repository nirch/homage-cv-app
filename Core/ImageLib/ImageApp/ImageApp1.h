#ifndef         _IMAGE_APP1_
#define         _IMAGE_APP1_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"

#include	"Ujet2/Jet2Type.h"
#include	"Ujet1/Jet1Type.h"

#include	"ImageType/ImageType.h"


#define	 APP_ZIZE_1	128



#define  JAPP_NORMAL	0
#define  JAPP_WIGHT	    1



typedef struct jet1App_type {
	int	type;		// JAPP_NORMAL | JAPP_WIGHT
	int		n;

	int		nh;
	float	dn;

	float	dn_inv;

	float	dxy;

	float	w[APP_ZIZE_1];


	float	a0[APP_ZIZE_1];
	float	a1[APP_ZIZE_1];
	float	a2[APP_ZIZE_1];

	float	t0;
	float	t1;
	float	t2;
	float	t02;

} jet1App_type;







jet1App_type *	jet1App_init( int n, int weight );

void	jet1App_destroy( jet1App_type *ja );


int		jet1App_initA( jet1App_type *ja, int n, int weight );


#ifdef __cplusplus
}
#endif

#endif
