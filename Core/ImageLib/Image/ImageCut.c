/**************************
 ***	ImageCut.c	***
 *************************/

#include "ImageType/ImageType.h"


static image_type *	image1_cut( image_type *sim, int row0, int col0, int row, int col, image_type *im );

static image_type *	image3_cut( image_type *sim, int row0, int col0, int row, int col, image_type *im );

static image_type *	imageS3_cut( image_type *sim, int row0, int col0, int row, int col, image_type *im );
static image_type *	imageUS1_cut( image_type *sim, int row0, int col0, int row, int col, image_type *im );


image_type *
image_cut( image_type *sim, int row0, int col0, int row, int col, image_type *im )
{ 
	if( IMAGE_TYPE(sim) == IMAGE_TYPE_U16 ){
		if( sim->channel == 3){
			im = imageS3_cut( sim, row0, col0, row, col, im );
			return( im );
		}

		if( sim->channel == 1){
			im = imageUS1_cut( sim, row0, col0, row, col, im );
			return( im );
		}

		if( im != NULL )
			image_destroy( im, 1 );

		return( NULL );
	}

    switch( sim->depth ){
    case 1:
            im = image1_cut( sim, row0, col0, row, col, im );
        break;

	case 3:
		im = image3_cut( sim, row0, col0, row, col, im );
		break;

    case 4:
            im = image4_cut( sim, row0, col0, row, col, im );
        break;

	case 6:
		im = imageS3_cut( sim, row0, col0, row, col, im );
		break;
    }


	return( im );
} 




image_type *
image4_cut( image_type *sim, int row0, int col0, int row, int col, image_type *im )
{
u_int	*sp,	*p;

int	i,	j;
int	row1,	col1;
int	r,	c,	r0,	c0,	rr0,	cc0;

	im = image_recreate( im, row, col, 4, 1 );

	if( row0 >= IMAGE_ROW(sim ) || col0 >= IMAGE_COLUMN(sim) ){
		p = (u_int *)im->data;
		for( i = 0 ; i < row ; i++ )
			for( j = 0 ; j < col ; j++ )
				*p++ = 0;

		return( im );
	}


	r0 = ( row0 < 0 )? 0 : row0;
	rr0 = r0 - row0;
	
	if( (row1 = row0 + row) > IMAGE_ROW(sim) )
		row1 = IMAGE_ROW(sim);
	r = row1 - r0;

	c0 = ( col0 < 0 )? 0 : col0;
	cc0 = c0 - col0;
	if( ( col1 = col0 + col) > IMAGE_COLUMN(sim) )
		col1 = IMAGE_COLUMN(sim);
	c = col1 - c0;




	for( i = 0 ; i < r ; i++ ){
		sp = IMAGE4_PIXEL( sim, r0+i, c0 );
		p = IMAGE4_PIXEL( im, rr0+i, cc0 );

		for( j = 0 ; j < c ; j++ )
			*p++ = *sp++;
	}

	if( row0 < 0 ){
		p = (u_int *)im->data;
		for( i = 0 ; i < rr0 ; i++ )
			for( j = 0 ; j < col ; j++ )
				*p++ = 0;
	}

	if( r < row ){
		p = IMAGE4_PIXEL( im, rr0+r, 0 );
		for( i = rr0 +r ; i < row ; i++ )
			for( j = 0 ; j < col ; j++ )
				*p++ = 0;
	}

	if( col0 < 0 )
		for( i = 0 ; i < r ; i++ ){
			p = IMAGE4_PIXEL( im, rr0+i, 0 );

			for( j = 0 ; j < cc0 ; j++ )
				*p++ = 0;
		}

	if( c < col )
		for( i = 0 ; i < r ; i++ ){
			p = IMAGE4_PIXEL( im, rr0+i, cc0+c );

			for( j = cc0 + c ; j < col ; j++ )
				*p++ = 0;
		}


	return( im );
}




static image_type *
image1_cut( image_type *sim, int row0, int col0, int row, int col, image_type *im )
{
u_char	*sp,	*p;
int	i,	j;
int	row1,	col1;
int	r,	c,	r0,	c0,	rr0,	cc0;

	im = image_realloc( im, col, row, 1, IMAGE_TYPE_U8, 1 );

	if( row0 >= IMAGE_ROW(sim ) || col0 >= IMAGE_COLUMN(sim) ){
		p = (u_char *)im->data;
		for( i = 0 ; i < row ; i++ )
			for( j = 0 ; j < col ; j++ )
				*p++ = 0;

		return( im );
	}


	r0 = ( row0 < 0 )? 0 : row0;
	rr0 = r0 - row0;
	
	if( (row1 = row0 + row) > IMAGE_ROW(sim) )
		row1 = IMAGE_ROW(sim);
	r = row1 - r0;

	c0 = ( col0 < 0 )? 0 : col0;
	cc0 = c0 - col0;
	if( ( col1 = col0 + col) > IMAGE_COLUMN(sim) )
		col1 = IMAGE_COLUMN(sim);
	c = col1 - c0;




	for( i = 0 ; i < r ; i++ ){
		sp = IMAGE_PIXEL( sim, r0+i, c0 );
		p = IMAGE_PIXEL( im, rr0+i, cc0 );

		for( j = 0 ; j < c ; j++ )
			*p++ = *sp++;
	}

	if( row0 < 0 ){
		p = (u_char *)im->data;
		for( i = 0 ; i < rr0 ; i++ )
			for( j = 0 ; j < col ; j++ )
				*p++ = 0;
	}

	if( r < row ){
		p = IMAGE_PIXEL( im, rr0+r, 0 );
		for( i = rr0 +r ; i < row ; i++ )
			for( j = 0 ; j < col ; j++ )
				*p++ = 0;
	}

	if( col0 < 0 )
		for( i = 0 ; i < r ; i++ ){
			p = IMAGE_PIXEL( im, rr0+i, 0 );

			for( j = 0 ; j < cc0 ; j++ )
				*p++ = 0;
		}

	if( c < col )
		for( i = 0 ; i < r ; i++ ){
			p = IMAGE_PIXEL( im, rr0+i, cc0+c );

			for( j = cc0 + c ; j < col ; j++ )
				*p++ = 0;
		}

	if( sim->palette != NULL )
		im->palette = palette_copy( sim->palette, im->palette );

	return( im );
}





static image_type *
image3_cut( image_type *sim, int row0, int col0, int row, int col, image_type *im )
{
u_char	*sp,	*p;
int	i,	j;
int	row1,	col1;
int	r,	c,	r0,	c0,	rr0,	cc0;

	im = image_recreate( im, row, col, 3, 1 );

	if( row0 >= IMAGE_ROW(sim ) || col0 >= IMAGE_COLUMN(sim) ){
		p = (u_char *)im->data;
		for( i = 0 ; i < row ; i++ )
			for( j = 0 ; j < col ; j++ ){
				*p++ = 0;
				*p++ = 0;
				*p++ = 0;
			}

		return( im );
	}


	r0 = ( row0 < 0 )? 0 : row0;
	rr0 = r0 - row0;

	if( (row1 = row0 + row) > IMAGE_ROW(sim) )
		row1 = IMAGE_ROW(sim);
	r = row1 - r0;

	c0 = ( col0 < 0 )? 0 : col0;
	cc0 = c0 - col0;
	if( ( col1 = col0 + col) > IMAGE_COLUMN(sim) )
		col1 = IMAGE_COLUMN(sim);
	c = col1 - c0;




	for( i = 0 ; i < r ; i++ ){
		sp = IMAGE_PIXEL( sim, r0+i, c0 );
		p = IMAGE_PIXEL( im, rr0+i, cc0 );

		for( j = 0 ; j < c ; j++ ){
			*p++ = *sp++;
			*p++ = *sp++;
			*p++ = *sp++;
		}
	}

	if( row0 < 0 ){
		p = (u_char *)im->data;
		for( i = 0 ; i < rr0 ; i++ )
			for( j = 0 ; j < col ; j++ ){
				*p++ = 0;
				*p++ = 0;
				*p++ = 0;
			}
	}

	if( r < row ){
		p = IMAGE_PIXEL( im, rr0+r, 0 );
		for( i = rr0 +r ; i < row ; i++ )
			for( j = 0 ; j < col ; j++ ){
				*p++ = 0;
				*p++ = 0;
				*p++ = 0;
			}
	}

	if( col0 < 0 )
		for( i = 0 ; i < r ; i++ ){
			p = IMAGE_PIXEL( im, rr0+i, 0 );

			for( j = 0 ; j < cc0 ; j++ ){
				*p++ = 0;
				*p++ = 0;
				*p++ = 0;
			}
		}

		if( c < col )
			for( i = 0 ; i < r ; i++ ){
				p = IMAGE_PIXEL( im, rr0+i, cc0+c );

				for( j = cc0 + c ; j < col ; j++ ){
					*p++ = 0;
					*p++ = 0;
					*p++ = 0;
				}
			}


			return( im );
}



static image_type *
imageS3_cut( image_type *sim, int row0, int col0, int row, int col, image_type *im )
{
short	*sp,	*p;
int	i,	j;
int	row1,	col1;
int	r,	c,	r0,	c0,	rr0,	cc0;

	im = image_recreate( im, row, col, IMAGE_FORMAT_US3, 1 );

	if( row0 >= IMAGE_ROW(sim ) || col0 >= IMAGE_COLUMN(sim) ){
		p = im->data_s;
		for( i = 0 ; i < row ; i++ )
			for( j = 0 ; j < col ; j++ ){
				*p++ = 0;
				*p++ = 0;
				*p++ = 0;
			}

			return( im );
	}


	r0 = ( row0 < 0 )? 0 : row0;
	rr0 = r0 - row0;

	if( (row1 = row0 + row) > IMAGE_ROW(sim) )
		row1 = IMAGE_ROW(sim);
	r = row1 - r0;

	c0 = ( col0 < 0 )? 0 : col0;
	cc0 = c0 - col0;
	if( ( col1 = col0 + col) > IMAGE_COLUMN(sim) )
		col1 = IMAGE_COLUMN(sim);
	c = col1 - c0;




	for( i = 0 ; i < r ; i++ ){
//		sp = IMAGES_PIXEL( sim, r0+i, c0 );
//		p = IMAGES_PIXEL( im, rr0+i, cc0 );
		sp = (short *)IMAGE_PIXEL( sim, r0+i, c0 );
		p = (short *)IMAGE_PIXEL( im, rr0+i, cc0 );

		for( j = 0 ; j < c ; j++ ){
			*p++ = *sp++;
			*p++ = *sp++;
			*p++ = *sp++;
		}
	}

	if( row0 < 0 ){
		p = im->data_s;
		for( i = 0 ; i < rr0 ; i++ )
			for( j = 0 ; j < col ; j++ ){
				*p++ = 0;
				*p++ = 0;
				*p++ = 0;
			}
	}

	if( r < row ){
		p = IMAGES_PIXEL( im, rr0+r, 0 );
		for( i = rr0 +r ; i < row ; i++ )
			for( j = 0 ; j < col ; j++ ){
				*p++ = 0;
				*p++ = 0;
				*p++ = 0;
			}
	}

	if( col0 < 0 )
		for( i = 0 ; i < r ; i++ ){
			p = IMAGES_PIXEL( im, rr0+i, 0 );

			for( j = 0 ; j < cc0 ; j++ ){
				*p++ = 0;
				*p++ = 0;
				*p++ = 0;
			}
		}

		if( c < col )
			for( i = 0 ; i < r ; i++ ){
				p = IMAGES_PIXEL( im, rr0+i, cc0+c );

				for( j = cc0 + c ; j < col ; j++ ){
					*p++ = 0;
					*p++ = 0;
					*p++ = 0;
				}
			}


			return( im );
}



static image_type *
	imageUS1_cut( image_type *sim, int row0, int col0, int row, int col, image_type *im )
{
	short	*sp,	*p;
	int	i,	j;
	int	row1,	col1;
	int	r,	c,	r0,	c0,	rr0,	cc0;

	im = image_recreate( im, row, col, IMAGE_FORMAT_US, 1 );

	if( row0 >= IMAGE_ROW(sim ) || col0 >= IMAGE_COLUMN(sim) ){
		p = im->data_s;
		for( i = 0 ; i < row ; i++ )
			for( j = 0 ; j < col ; j++ ){
				*p++ = 0;
			}

			return( im );
	}


	r0 = ( row0 < 0 )? 0 : row0;
	rr0 = r0 - row0;

	if( (row1 = row0 + row) > IMAGE_ROW(sim) )
		row1 = IMAGE_ROW(sim);
	r = row1 - r0;

	c0 = ( col0 < 0 )? 0 : col0;
	cc0 = c0 - col0;
	if( ( col1 = col0 + col) > IMAGE_COLUMN(sim) )
		col1 = IMAGE_COLUMN(sim);
	c = col1 - c0;




	for( i = 0 ; i < r ; i++ ){
		sp = (short *)IMAGE_PIXEL( sim, r0+i, c0 );
		p = (short *)IMAGE_PIXEL( im, rr0+i, cc0 );

		for( j = 0 ; j < c ; j++ ){
			*p++ = *sp++;
		}
	}

	if( row0 < 0 ){
		p = im->data_s;
		for( i = 0 ; i < rr0 ; i++ )
			for( j = 0 ; j < col ; j++ ){
				*p++ = 0;
			}
	}

	if( r < row ){
		p = IMAGES_PIXEL( im, rr0+r, 0 );
		for( i = rr0 +r ; i < row ; i++ )
			for( j = 0 ; j < col ; j++ ){
				*p++ = 0;
			}
	}

	if( col0 < 0 )
		for( i = 0 ; i < r ; i++ ){
			p = IMAGES_PIXEL( im, rr0+i, 0 );

			for( j = 0 ; j < cc0 ; j++ ){
				*p++ = 0;
			}
		}

		if( c < col )
			for( i = 0 ; i < r ; i++ ){
				p = IMAGES_PIXEL( im, rr0+i, cc0+c );

				for( j = cc0 + c ; j < col ; j++ ){
					*p++ = 0;
				}
			}


			return( im );
}