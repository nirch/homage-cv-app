	
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#include	"Uigp/igp.h"
#include	"ImageType/ImageType.h"


static image_type *	image3_sepia_efect( image_type *sim,  float fr, float fg, float fb, int switchBR, image_type *im );
static image_type *	image4_sepia_efect( image_type *sim,  float fr, float fg, float fb, int switchBR, image_type *im );


  
image_type *
	image_sepia_efect( image_type *sim,  float fr, float fg, float fb, int switchBR, image_type *im )
{


	if( sim->depth == 3){
		im = image3_sepia_efect( sim, fr, fg, fb, switchBR, im );
		return( im );
	}

	if( sim->depth == 4){
		im = image4_sepia_efect( sim, fr, fg, fb, switchBR, im );
		return( im );
	}


	if( im != NULL )
		image_destroy( im, 1 );

	return( NULL );
} 


static image_type *
image3_sepia_efect( image_type *sim, float fr, float fg, float fb, int switchBR, image_type *im )
{
	int	i,	j;


	im = image_reallocL( sim, im );

	u_char *sp = sim->data;
	u_char *tp = im->data;

    int blueIndex;
    int redIndex;
    
    if (switchBR) {
        blueIndex = 2;
        redIndex = 0;
    } else {
        blueIndex = 0;
        redIndex = 2;
    }
    
	for( i = 0; i < sim->height ; i++ ){
		for( j = 0; j < sim->width ; j++, sp += 3, tp += 3 ){


			int red = sp[2];
			int green = sp[1];
			int blue = sp[0];



			int tmp;
			tmp = fb*( ( red * .272) + (green *.534) + (blue * .131));
			tp[0] = PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = fg*(( red * .349) + (green *.686) + (blue * .168));
			tp[1] = PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = fr*((red * .393) + (green *.769) + (blue * .189));
			tp[2] = PUSH_TO_RANGE( tmp, 0, 255 );
		}
	}

	return( im );

}



static image_type *
image4_sepia_efect( image_type *sim, float fr, float fg, float fb, int switchBR, image_type *im )
{
	int	i,	j;


	im = image_reallocL( sim, im );

	u_char *sp = sim->data;
	u_char *tp = im->data;

    int blueIndex;
    int redIndex;
    
    if (switchBR) {
        blueIndex = 2;
        redIndex = 0;
    } else {
        blueIndex = 0;
        redIndex = 2;
    }
    
	for( i = 0; i < sim->height ; i++ ){
		for( j = 0; j < sim->width ; j++, sp += 4, tp += 4 ){


			//int tmp;
			//tmp = (sp[0] * .393) + (sp[1] *.769) + (sp[2] * .189);
			//tp[2] = PUSH_TO_RANGE( tmp, 0, 255 );
			//tmp = (sp[0] * .349) + (sp[1] *.686) + (sp[2] * .168);
			//tp[1] = PUSH_TO_RANGE( tmp, 0, 255 );
			//tmp = (sp[0] * .272) + (sp[1] *.534) + (sp[2] * .131);
			//tp[0] = PUSH_TO_RANGE( tmp, 0, 255 );

			int blue = sp[0];
			int green = sp[1];
			int red = sp[2];


			int tmp;
			tmp = fb*( ( red * .272) + (green *.534) + (blue * .131));
			tp[blueIndex] = PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = fg*(( red * .349) + (green *.686) + (blue * .168));
			tp[1] = PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = fr*((red * .393) + (green *.769) + (blue * .189));
			tp[redIndex] = PUSH_TO_RANGE( tmp, 0, 255 );

			tp[3] = sp[3];
		}
	}

	return( im );

}

