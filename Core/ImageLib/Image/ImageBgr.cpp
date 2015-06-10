 /************************
 ***   Image3Tool.c   ***
 ************************/
#include	<string.h>
#include    <math.h>

#include	"ImageType/ImageType.h"
#include	"ImageType/ImageTool.h"




image_type *
image_bgr2rgb( image_type *sim, image_type *im )
{

	if( sim->depth == 3 ){
		im = image3_bgr2rgb( sim, im );
		return( im );
	}

	if( sim->depth == 4 ){
		im = image4_bgr2rgb( sim, im );
		return( im );
	}

	if( im != NULL )
		image_destroy( im, 1 );

	return( NULL );
}


image_type *
image4_bgr2rgb( image_type *sim, image_type *im )
{
	int	i,	j;

	im = image_realloc( im, sim->width, sim->height, 4, IMAGE_TYPE_U8, 1  );


	u_char *tp = im->data;
	u_char *sp = sim->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp += 4, tp += 4 ){
			//int R = IMAGE4_RED( *sp );
			//int G = IMAGE4_GREEN( *sp );
			//int B = IMAGE4_BLUE( *sp );

			//*tp++ = IMAGE4_RGB( B, G, R );


			u_char sp0 = sp[0];
			u_char sp2 = sp[2];

			tp[0] = sp2;
			tp[1] = sp[1];
			tp[2] = sp0;
			tp[3] = sp[3];


		}

		return( im );
}


image_type *
image3_bgr2rgb( image_type *sim, image_type *im )
{
u_char	*tp;
u_char	*sp;
int	i,	j;

	im = image_realloc( im, sim->width, sim->height, 3, IMAGE_TYPE_U8, 1  );


	tp = (u_char *)im->data;
	sp = sim->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp += 3 ){
			int tmp = *sp;
			*tp++ = *(sp+2);
			*tp++ = *(sp+1);
			*tp++ = tmp;
		}
	}
	return( im );
}


#ifdef _AA_
void
image3_bgr2rgb( image_type *im )
{

u_char	*sp,	tmp;
int	i,	j;

	sp = im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp += 3 ){
			tmp = *sp;
			*sp = *(sp+2);
			*(sp+2) = tmp;
		}
	}

	return( im );
}

#endif



