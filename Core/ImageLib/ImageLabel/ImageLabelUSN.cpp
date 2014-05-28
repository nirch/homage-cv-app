/***************************
 ***   Image2BwLabel.c   ***
 ***************************/
#include <math.h>
#include "Uigp/igp.h"
#include "ImageType/ImageType.h"
#include "BwLabel.h"

#include "ImageLabel.h"




void	imageLabelUS_createN( imageLabel_type *abw );




imageLabel_type *
imageLabelUS_N( image_type *sim, int T, int inv, int margin, imageLabel_type *abw )
{
	int	i,	j;
	u_char	*sp;
	short	*bp;


	if( abw == NULL ){
		abw = imageLabel_alloc();
		abw->a = (bwLabel_type *)malloc( sim->row*sim->column*sizeof(bwLabel_type) );

		abw->im = image_realloc( abw->im, sim->width+2*margin, sim->height+2*margin, 1, IMAGE_TYPE_U16, 1 );

		image2_const( abw->im, 0 );
	}

	abw->margin = margin;


	abw->im = image_realloc( abw->im, sim->width+2*margin, sim->height+2*margin, 1, IMAGE_TYPE_U16, 1 );
	image2_set_boundary( abw->im, 0 );


	sp = sim->data;
//	bp = abw->mim->data_s + margin*(abw->mim->width + 1);
	bp = (short*)IMAGE_PIXEL( abw->im, margin, margin );


	if( inv == 0 ){
		for( i = 0 ; i < sim->height ; i++, bp += 2*margin )
			for( j = 0 ; j < sim->width ; j++, sp++, bp++ )
				*bp = ( *sp < T )? 0 :1;
	}
	else {
		for( i = 0 ; i < sim->height ; i++, bp += 2*margin )
			for( j = 0 ; j < sim->width ; j++, sp++, bp++ )
				*bp = ( *sp < T )? 1 : 0;
	}



	imageLabelUS_createN( abw );


	return( abw );
}







void
imageLabelUS_createN(imageLabel_type *abw )
{
int	i,	j,	k;

int	id;


	abw->nA = 0;
	lbm_type *b0 = (lbm_type *)malloc( abw->im->width * sizeof(lbm_type) );
	lbm_type *b1 = (lbm_type *)malloc( abw->im->width * sizeof(lbm_type) );


	abw->a[0].id = 0;
	abw->a[0].no = 0;
	abw->nA = 1;

	b0[0].j0 = abw->im->width;
	lbm_type *b = b1;

	short *sp = abw->im->data_s;
	for( i = 0 ; i < abw->im->height ; i++ ){
		k = 0;
		lbm_type *b = b1;

		for( j = 0 ; j < abw->im->width ;  ){
			if( *sp == 0 ){
				j++;
				sp++;
				continue;
			}

			short *tp = sp;

			b->j0 = b->j1 = j;
			for( ; j < abw->im->width && *sp != 0 ; j++, sp++ );
			b->j1 = j -1;


			int	k0,	k1;
			k0 = k1 = id = -1;
			for( ; b->j1 >= b0[k].j0  ; k++ ){
				if( b0[k].j1 < b->j0 )	continue;

				if( k0 < 0 )	k0 = k;
				k1 = k;

				if( id < 0 || b0[k].id < id )
					id = b0[k].id;

			}


			if( id < 0  ){
				id = abw->nA++;
				abw->a[id].id = id;
				abw->a[id].no = 0;
				abw->a[id].color = 1;
			}
			else {
				for( k = k0 ; k <= k1 ; k++ ){
					if( b0[k].id > id ){
						bwLabel_merge( abw->a, abw->nA,  b0[k].id , id );
					}
				}
				k = k1;
			}




			
			for( j = b->j0 ; j <= b->j1 ; j++ ){
				*tp++ = id;
			}
			abw->a[id].no += b->j1 - b->j0 + 1;


			b->id = id;
			b++;




		}

		b->j0 = abw->im->width;
		b = b0;
		b0 = b1;
		b1 = b;
	}

	free( b0 );
	free( b1 );


		
	imageLabel2_set_id( abw->im, abw->a );
}


