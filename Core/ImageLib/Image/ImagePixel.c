/**************************
 ***	ImagePixel.c	***
 *************************/

#include "ImageType/ImageType.h"



int 
image_pixel_value( image_type *sim, int i, int j, int *r, int *g, int *b )
{
 
    switch( sim->depth ){
    case 1:
		{
			u_char *sp = (u_char *)IMAGE_PIXEL( sim, i, j );

			*r = *sp;
			*g = *sp;
			*b = *sp;
		}
        break;

	case 3:
		{
			u_char *sp = (u_char *)IMAGE_PIXEL( sim, i, j );

			*r = *sp++;
			*g = *sp++;
			*b = *sp++;
		}
		break;


    case 4:
		{
			u_int *sp = (u_int *)IMAGE4_PIXEL( sim, i, j );
	
			*r = IMAGE4_RED(*sp);
			*g = IMAGE4_GREEN(*sp);
			*b = IMAGE4_BLUE(*sp);
		}
        break;

	case 6:
		{

	
		short *sp = (short *)IMAGE_PIXEL( sim, i, j );

		*r = *sp>>8;
		*g = *(sp+1)>>8;
		*b = *(sp+2)>>8;
		}
		break;


    }

	return( 1 );
} 


