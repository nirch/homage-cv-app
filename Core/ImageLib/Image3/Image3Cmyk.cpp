

#include	"Uigp/igp.h"
#include	"Utime/GpTime.h"
#include	"BMP/ImageBmp.h"
#include	"ImageType/ImageType.h"




image_type *
image3_to_cmyk( image_type *sim, image_type *im )
{
	int	i,	j;
	int	r,	g,	b;

	im = image_realloc( im, sim->width, sim->height, 4, IMAGE_TYPE_U8, 1 );

	u_char *sp = sim->data;
	u_char *tp = im->data;

	for( i = 0; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, tp += 4  ){
			r = *sp++;
			g = *sp++;
			b = *sp++;

			tp[3] = 0;//255;	// K
			tp[0] = 255 - r;
			tp[1] = 255 - g;
			tp[2] = 255 - b;
			
			//k = 255 - sp[3];
			//r = (k*(255-pp[0]))/255;
			//g = (k*(255-pp[1]))/255;
			//b = (k*(255-pp[2]))/255;
			//*cp++ = PACK(r, g, b);
			//pp += samplesperpixel);
		}
	}

	return( im );
}





static char *Usage = 
"Usage: TiffCA      imageFile  outDir";



static void	
read_command_line(int argc, char *argv[],
				  char *imageFile, char *outFile )
{
int     k;

	k = 1;






	if (k < argc) {
		sprintf( imageFile, "%s", argv[k]);
		k++;
	}
	else	error("read_command_line", Usage);
	
	if (k < argc) {
		sprintf( outFile, "%s", argv[k]);
		k++;
	}
	else	error("read_command_line", Usage);

}
