/**********************************
 ***   ImageInterpolationPv.c   ***
 **********************************/
#include	<math.h>

#include "Umath/LT2Type.h"

#include	"ImageType/ImageType.h"
#include	"ImageType/ImageTool.h"





image_type *
image_interpolation_pv( image_type *sim, vec2f_type *p, vec2f_type *v, int w, int h, image_type *im )
{


	lt2_type	lt;
	float	t;

	vec2f_type	dv;
	t = hypot( v->x, v->y );

	dv.x = t * v->x;
	dv.y = t * v->y;

	lt.a0 = dv.x;
	lt.b0 = -dv.y;
	lt.c0 = 0;

	lt.a1 = dv.y;
	lt.b1 = dv.x;
	lt.c1 = 0;



	lt.c0 = p->x - 0.5*w*lt.a0 - 0.5*h*lt.b0;
	lt.c1 = p->y - 0.5*w*lt.a1 - 0.5*h*lt.b1;




	//	im = image_realloc( im, w, h, 1, IMAGE_TYPE_U8, 1 );


	if( IMAGE_TYPE(sim) == IMAGE_TYPE_U8 ){
		im = image1_interpolation_lt2( sim, &lt, 0, 0, w, h, im );
		return( im );
	}

	if( IMAGE_TYPE(sim) == IMAGE_TYPE_U16 ){
		im = imageUS_interpolation_lt2( sim, &lt, 0, 0, w, h, im );
		return( im );
	}




	return( im );


}
