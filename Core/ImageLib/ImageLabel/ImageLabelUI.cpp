/*************************
 ***   Image1Label.c   ***
 *************************/
#include <math.h>
#include "Uigp/igp.h"
#include "ImageType/ImageType.h"

#ifdef _DEBUG
//#define _DUMP
#endif

#include "Umath/Matrix2Type.h"
#include "Umath/EigenType.h"

#include "ImageLabel/ImageLabel.h"

#include "ImageDump/ImageDump.h"


static void	imageLabelUI_set_idP( image_type *im, bwLabel_type *bw, int nBw );

static image_type *	imageLabelUI_create( image_type *sim, bwLabel_type **tbw, int *tnBw, image_type *im );







imageLabel_type *
imageLabelUI( image_type *sim, imageLabel_type *abw )
{
	if( abw == NULL )
		abw = imageLabel_alloc();


	abw->im = imageLabelUI_create( sim, &abw->a, &abw->nA, abw->im );

	return( abw );
}



imageLabel_type *
imageLabelUI_T( image_type *sim, int T, int margin, imageLabel_type *abw )
{
u_char	*sp,	*tp;
int	i,	j;

	if( abw == NULL ){
		abw = imageLabel_alloc();
		abw->mim = image_alloc( sim->width+2*margin, sim->height+2*margin, 1, IMAGE_TYPE_U8, 1 );

		image1_const( abw->mim, 0 );
	}

	abw->margin = margin;

//	abw->mim = image1_binary( sim, T, abw->mim );

	sp = sim->data;

	tp = abw->mim->data + margin*(abw->mim->width + 1);
	for( i = 0 ; i < sim->height ; i++, tp += 2*margin ){
		for( j = 0 ; j < sim->width ; j++, sp++, tp++ )
			*tp = ( *sp < T )? 0 :1;
	}



//	IMAGE_DUMP_SCALE( abw->mim, 255, "aa", 1, NULL );
	//image1_boundary_set( abw->mim, 0 );

	abw->im = imageLabelUI_create( abw->mim, &abw->a, &abw->nA, abw->im );

	return( abw );
}


image_type *
image1_label( image_type *sim, bwLabel_type **tbw, int *tnBw, image_type *im )
{
	*tbw = NULL;

	im = imageLabelUI_create(  sim, tbw, tnBw, im );
	
	return( im );
}



static image_type *
imageLabelUI_create( image_type *sim, bwLabel_type **tbw, int *tnBw, image_type *im )
{
bwLabel_type	*bw;
int	nBw;

int	i,	j,	k;
u_char	*sp,	*sp0,	*sp1;
u_int	*tp,	*tp0,	*tp1;
int	id1,	id,	NBW;


//	im  = image_recreate( im, sim->height, sim->width, 4, 1 );
	im = image_realloc( im, sim->width, sim->height, 4, IMAGE_TYPE_U8, 1 );

	NBW = sim->row*sim->column/9;
	bw = *tbw;
	if( bw == NULL )
		bw = (bwLabel_type *)malloc( NBW * sizeof(bwLabel_type) ); //sim->row*sim->column*sizeof(bwLabel_type)/16 );

	nBw = 0;

	sp = sim->data ;
	tp = im->data_ui;
//	if( *sp != 0 ){
		bw[nBw].id = nBw;
		bw[nBw].no = 1;
		bw[nBw].color = *sp;
		bw[nBw].i = 0;
		bw[nBw].j = 0;

		*tp = nBw++;
//	}

	sp++;
	sp0 = sp-1;
	tp++;
	tp0 = tp-1;


	for( j = 1 ; j < sim->column ; j++, sp++, sp0++, tp++, tp0++ ){
//		if( *sp == 0 )	continue;

		if( *sp0 == *sp ){
			*tp = *tp0;
			bw[*tp].no++;
			continue;
		}

		bw[nBw].id = nBw;
		bw[nBw].no = 1;
		bw[nBw].color = *sp;
		bw[nBw].i = 0;
		bw[nBw].j = j;

		*tp = nBw++;
	}

		
	for( i = 1 ; i < sim->row ; i++ ){
		sp = IMAGE_PIXEL( sim, i, 0 );
		sp1 = sp - sim->column;

		tp = (u_int*)IMAGE_PIXEL( im, i, 0 );
		tp1 = tp - im->column;


		if( *sp1 == *sp){
			*tp = *tp1;
			bw[*tp].no++;
		}
		else	{
			bw[nBw].id = nBw;
			bw[nBw].no = 1;
			bw[nBw].color = *sp;
			bw[nBw].i = i;
			bw[nBw].j = j;
			*tp = nBw++;
		}



		sp++;
		sp1++;
		sp0 = sp-1;

		tp++;
		tp1++;
		tp0 = tp-1;
		for( j = 1 ; j < sim->column ; j++, sp++, sp1++, sp0++, tp++, tp0++, *tp1++ ){
//			if( *sp == 0 )	continue;
			if( *sp0 == *sp ){

				*tp = bw[*tp0].id;
				bw[*tp].no++;
				if( *sp1 != *sp || bw[*tp].id == bw[*tp1].id )	continue;



				if( bw[*tp].id < bw[*tp1].id ){
					id1 = bw[*tp1].id;
					id = bw[*tp].id;
				}
				else {
					id = bw[*tp1].id;
					id1 = bw[*tp].id;
				}

				for( k = id1 ; k < nBw ; k++ ){
					if( bw[k].id == id1 )
						bw[k].id = id;
				}

				bw[id].no += bw[id1].no;


				continue;
			}

			if( *sp1 == *sp ){
				*tp = bw[*tp1].id;
				bw[*tp].no++;

				continue;
			}

	

			bw[nBw].id = nBw;
			bw[nBw].color = *sp;
			bw[nBw].no = 0;
			bw[nBw].i = i;
			bw[nBw].j = j;

			*tp = nBw++;
			bw[*tp].no++;
		}

		if( nBw+im->width >= NBW ){
			gpWarning( "imageLabelUI_create", "Out of Range" );
			break;
		}


	}

	*tbw = bw;
	*tnBw = nBw;


	imageLabelUI_set_idP( im, bw, nBw );

	return( im );
}


void
imageLabelUI_set_id( imageLabel_type *abw )
{
	imageLabelUI_set_idP( abw->im, abw->a, abw->nA );
}


void
imageLabelUI_set_idP( image_type *im, bwLabel_type *bw, int nBw )
{
int	i,	j;
u_int	*tp;

	tp = im->data_ui;
	for( i = 0 ; i < im->height ; i++ )
		for( j = 0 ; j < im->width ; j++, tp++ )
			*tp = bw[*tp].id;
}




int
imageLabelUI_eigen2d( image_type *im, int id, box2i_type *b, eigen2d_type *e )
{
int	i,	j,	n;
u_int	*tp;
float	sx,	sy;

	matrix2_type	m;
	matrix2_zero( &m );
	
	sx = sy = 0;

	tp = im->data_ui;
	for( i = b->y0, n = 0 ; i <= b->y1 ; i++ ){
		tp = IMAGE4_PIXEL( im, i, b->x0 );
		for( j = b->x0 ; j <= b->x1 ; j++, tp++ ){
			if( *tp != id )	continue;

			sx += j;
			sy += i;

			m.a00 += j*j;
			m.a01 += j*i;
			m.a11 += i*i;
			n++;
		}
	}

	sx /= n;
	sy /= n;

	m.a00 = m.a00 / n - sx*sx;
	m.a01 = m.a01 / n - sx*sy;
	m.a11 = m.a11 / n - sy*sy;
	m.a10 = m.a01;


	e->p.x = sx;
	e->p.y = sy;




	matrix2S_eigen( &m, &e->e1, &e->v1, &e->e2 );


	// test 
	matrix2_type m1,	m2;
	m1.a00 = e->v1.x;
	m1.a10 = e->v1.y;
	m1.a00 = -e->v1.y;
	m1.a10 = e->v1.x;

	matrix2_mult( &m1, &m, &m2 );


	return( 1 );

}






static int	imageUI_label_nieg( image_type *im, bwLabel_type *bw, int g[], int *nG );

static void	imageUI_label_replace( image_type *sim, image_type *im, bwLabel_type *bw, bwLabel_type *nbw );


void
imageLabelUI_filter( image_type *sim, image_type *lim, bwLabel_type *aBw, int nBw, int nT )
{
int	i,	k,	ik;
bwLabel_type	*bw;
int	g[100],	nG;


	imageLabelUI_set_aux( lim, aBw, nBw );

	for( i = 0 ; i < nBw ; i++ ){
		bw = &aBw[i];

		if( bw->id != i )	continue;
		if( bw->no >= nT )	continue;

		imageUI_label_nieg( lim, bw, g, &nG );

		for( k = 1, ik = 0 ; k < nG ; k++ )
			if( g[k] > g[ik] )	ik = k;

	
		imageUI_label_replace( sim, lim, bw, &aBw[g[ik]] );
	}
}


void
imageLabelUI_set_aux( image_type *im, bwLabel_type *aBw, int nBw )
{
int	i,	j;
u_int	*tp;
bwLabel_type *bw;

	for( i = 0 ; i < nBw ; i++ )
		aBw[i].no = 0;


	tp = im->data_ui;
	for( i = 0 ; i < im->height ; i++ ){
		for( j = 0 ; j < im->width ; j++, tp++ ){
			bw = &aBw[*tp];

			*tp = bw->id;
			bw->no++;

			if( bw->no <= 1 ){
				BOX2D_INIT( bw->b, j, i );
			}
			else	{
				BOX2D_UPDATE( bw->b, j, i );
			}
		}
	}
}



void
imageLabelUI_set_box( imageLabel_type *abw )
{
	//m_abwC->im, m_abwC->a, m_abwC->nA

	int	i,	j;
	bwLabel_type *bw;

	for( i = 0 ; i < abw->nA ; i++ )
		abw->a[i].no = 0;


	u_int *sp = abw->im->data_ui;
	for( i = 0 ; i < abw->im->height ; i++ ){
		int val = *sp++;
		int n = 1;
		int	j0 = 0;
		for( j = 1 ; j < abw->im->width ; j++, sp++ ){
		
			if( *sp == val ){
				n++;
				continue;
			}


			bw = &abw->a[val];
			



			if( bw->no <= 0 ){
				bw->b.x0 = j0;
				bw->b.x1 = j-1;
				bw->b.y0 = bw->b.y1 = i;
			}
			else	{
				bw->b.y1 = i;
				if( j0 < bw->b.x0 )	bw->b.x0 = j0;
				if( j-1 > bw->b.x1 )	bw->b.x1 = j-1;
			}

			bw->no += n;

			val = *sp;
			n = 1;
			j0 = j;
		}

		if( n > 0 ){
			bw = &abw->a[val];

			if( bw->no <= 0 ){
				bw->b.x0 = j0;
				bw->b.x1 = j-1;
				bw->b.y0 = bw->b.y1 = i;
			}
			else	{
				bw->b.y1 = i;
				if( j0 < bw->b.x0 )	bw->b.x0 = j0;
				if( j-1 > bw->b.x1 )	bw->b.x1 = j-1;
			}

			bw->no += n;
		}
	}
}



int
imageUI_label_nieg( image_type *im, bwLabel_type *bw, int g[], int *nG )
{
int	i,	j,	k,	id;
u_int	*tp;



	*nG = 0;
	for( i = bw->b.y0 ; i <= bw->b.y1 ; i++ ){
		tp = (u_int *)IMAGE_PIXEL( im, i, bw->b.x0 );

		for( j = bw->b.x0 ; j <= bw->b.x1 ; j++, tp++ ){

			if( *tp != bw->id )	continue;

			if( i > 0 && (id = *(tp-im->width)) != *tp ){
				for( k = 0 ; k < *nG ; k++ ){
					if( g[k] == id )	break;
				}
				if( k == *nG )	g[(*nG)++] = id;
			}

			if( i < im->height-1 && (id = *(tp+im->width)) != *tp ){
				for( k = 0 ; k < *nG ; k++ ){
					if( g[k] == id )	break;
				}
				if( k == *nG )	g[(*nG)++] = id;
			}

			if( j > 0 && (id = *(tp-1)) != *tp ){
				for( k = 0 ; k < *nG ; k++ ){
					if( g[k] == id )	break;
				}
				if( k == *nG )	g[(*nG)++] = id;
			}

			if( j < im->width-1 && (id = *(tp+1)) != *tp ){
				for( k = 0 ; k < *nG ; k++ ){
					if( g[k] == id )	break;
				}
				if( k == *nG )	g[(*nG)++] = id;
			}
		}
	}

	return( *nG );
}




static void
imageUI_label_replace( image_type *sim, image_type *im, bwLabel_type *bw, bwLabel_type *nbw )
{
int	i,	j;
u_int	*tp;
u_char	*sp;


	for( i = bw->b.y0 ; i <= bw->b.y1 ; i++ ){
		sp = (u_char *)IMAGE_PIXEL( sim, i, bw->b.x0 );
		tp = (u_int *)IMAGE_PIXEL( im, i, bw->b.x0 );

		for( j = bw->b.x0 ; j <= bw->b.x1 ; j++, tp++, sp++ ){

			if( *tp != bw->id )	continue;

			*tp = nbw->id;
			*sp = nbw->color;
			nbw->no++;

			BOX2D_UPDATE( nbw->b, j, i );

		}
	}

	bw->id = -1;
}




image_type *
imageLabelUI_color_image( imageLabel_type *abw, int color, image_type *im )
{
	int	i,	j;



	im = image_alloc( abw->im->width, abw->im->height, 1, IMAGE_TYPE_U8, 1 );


	u_char *tp = im->data;
	u_int *bp = abw->im->data_ui;
	for( i = 0 ; i < abw->im->height ; i++ ){
		for( j = 0 ; j < abw->im->width ; j++, bp++, tp++ ){
			bwLabel_type *bw = &abw->a[*bp];

			if( bw->color == color )
				*tp = 255;
			else *tp = 0;
		}
	}

	return( im );
}


image_type *
	imageLabelUI_color_imageAXB( imageLabel_type *abw, float a, float b, image_type *im )
{
	int	i,	j;



	im = image_alloc( abw->im->width, abw->im->height, 1, IMAGE_TYPE_U8, 1 );


	u_char *tp = im->data;
	u_int *bp = abw->im->data_ui;
	for( i = 0 ; i < abw->im->height ; i++ ){
		for( j = 0 ; j < abw->im->width ; j++, bp++, tp++ ){
			bwLabel_type *bw = &abw->a[*bp];

			int tmp = a*bw->color + b;
			*tp = PUSH_TO_RANGE( tmp, 0, 255 );

			//if( bw->color == color )
			//	*tp = 255;
			//else *tp = 0;
		}
	}

	return( im );
}


image_type *
	imageLabelUI_color_imageT( imageLabel_type *abw, u_char T[], image_type *im )
{
	int	i,	j;



	im = image_alloc( abw->im->width, abw->im->height, 1, IMAGE_TYPE_U8, 1 );


	u_char *tp = im->data;
	u_int *bp = abw->im->data_ui;
	for( i = 0 ; i < abw->im->height ; i++ ){
		for( j = 0 ; j < abw->im->width ; j++, bp++, tp++ ){
			bwLabel_type *bw = &abw->a[*bp];

		
			*tp = T[bw->color];
		}
	}

	return( im );
}


image_type *
imageLabelUI_imageID( imageLabel_type *abw, image_type *im )
{
	int	i,	j;




	im = image_alloc( abw->im->width, abw->im->height, 1, IMAGE_TYPE_U8, 1 );


	u_char *tp = im->data;
	u_int *bp = abw->im->data_ui;
	for( i = 0 ; i < abw->im->height ; i++ ){
		for( j = 0 ; j < abw->im->width ; j++, bp++, tp++ ){
			bwLabel_type *bw = &abw->a[*bp];
			*tp = bw->id;
		}
	}

	return( im );
}



void
	imageLabelUI_value( imageLabel_type *abw, image_type *sim )
{
	int	i,	j;


	bwLabel_type *bw;

	for( i = 0 ; i < abw->nA ; i++ ){
		abw->a[i].no = 0;
		abw->a[i].av = 0;
		abw->a[i].var = 0;
	}



	u_char *sp = sim->data;
	u_int *bp = abw->im->data_ui;
	for( i = 0 ; i < abw->im->height ; i++ ){
		for( j = 0 ; j < abw->im->width ; j++, bp++, sp++ ){
			bw = &abw->a[*bp];
			bw->no++;
			bw->av += *sp;
			bw->var += *sp * *sp;
		}
	}



	for( i = 0 ; i < abw->nA ; i++ ){
		bwLabel_type *bw = &abw->a[i];

		if( bw->no == 0  )	continue;

		bw->av /= bw->no;
		bw->var = bw->var / bw->no - bw->av*bw->av;
	}
}



void
	imageLabelUI_svalue( imageLabel_type *abw, image_type *sim )
{
	int	i,	j;



	bwLabel_type *bw;

	for( i = 0 ; i < abw->nA ; i++ ){
		abw->a[i].no = 0;
		abw->a[i].av = 0;
		abw->a[i].var = 0;
	}



	s_char *sp = (s_char *)sim->data;
	u_int *bp = abw->im->data_ui;
	for( i = 0 ; i < abw->im->height ; i++ ){
		for( j = 0 ; j < abw->im->width ; j++, bp++, sp++ ){
			bw = &abw->a[*bp];
			bw->no++;
			bw->av += *sp;
			bw->var += *sp * *sp;
		}
	}



	for( i = 0 ; i < abw->nA ; i++ ){
		bwLabel_type *bw = &abw->a[i];

		if( bw->no == 0  )	continue;

		bw->av /= bw->no;
		bw->var = bw->var / bw->no - bw->av*bw->av;
	}
}



void
imageLabelUI_merge( imageLabel_type *abw, int i0, int i1 )
{
	int	i,	j;
	u_int	*tp;



	image_type *im = abw->im;
	bwLabel_type *bw = &abw->a[i0];
	bwLabel_type *nbw = &abw->a[i1];

	for( i = bw->b.y0 ; i <= bw->b.y1 ; i++ ){
		tp = (u_int *)IMAGE_PIXEL( im, i, bw->b.x0 );

		for( j = bw->b.x0 ; j <= bw->b.x1 ; j++, tp++){

			if( *tp != bw->id )	continue;

			*tp = nbw->id;
		}
	}

	nbw->no += bw->no;

	BOX2D_UPDATE_BOX( nbw->b, bw->b );

	bw->id = -1;
}


int
imageLabelUI_nieg( imageLabel_type *abw, int i0, int g[], int *nG )
{
	int	i,	j,	k,	id;
	u_int	*tp;



	bwLabel_type *bw = &abw->a[i0];
	image_type *im = abw->im;

	*nG = 0;
	for( i = bw->b.y0 ; i <= bw->b.y1 ; i++ ){
		tp = (u_int *)IMAGE_PIXEL( im, i, bw->b.x0 );

		for( j = bw->b.x0 ; j <= bw->b.x1 ; j++, tp++ ){

			if( *tp != bw->id )	continue;

			if( i > 0 && (id = *(tp- im->width)) != *tp ){
				for( k = 0 ; k < *nG ; k++ ){
					if( g[k] == id )	break;
				}
				if( k == *nG )	g[(*nG)++] = id;
			}

			if( i < im->height-1 && (id = *(tp+im->width)) != *tp ){
				for( k = 0 ; k < *nG ; k++ ){
					if( g[k] == id )	break;
				}
				if( k == *nG )	g[(*nG)++] = id;
			}

			if( j > 0 && (id = *(tp-1)) != *tp ){
				for( k = 0 ; k < *nG ; k++ ){
					if( g[k] == id )	break;
				}
				if( k == *nG )	g[(*nG)++] = id;
			}

			if( j < im->width-1 && (id = *(tp+1)) != *tp ){
				for( k = 0 ; k < *nG ; k++ ){
					if( g[k] == id )	break;
				}
				if( k == *nG )	g[(*nG)++] = id;
			}
		}
	}

	return( *nG );
}



int 
imageLabelUI_nieg1( imageLabel_type *abw, int i0 )
{
	int	i,	j,	id;
	u_int	*tp;



	bwLabel_type *bw = &abw->a[i0];
	image_type *im = abw->im;


	for( i = bw->b.y0 ; i <= bw->b.y1 ; i++ ){
		tp = (u_int *)IMAGE_PIXEL( im, i, bw->b.x0 );
		for( j = bw->b.x0 ; j <= bw->b.x1 ; j++, tp++ ){

			if( *tp != bw->id )	continue;

			if( i > 0 && (id = *(tp- im->width)) != *tp ){
				return( id );
			}

			if( i < im->height-1 && (id = *(tp+im->width)) != *tp ){
				return( id );
			}

			if( j > 0 && (id = *(tp-1)) != *tp ){
				return( id );
			}

			if( j < im->width-1 && (id = *(tp+1)) != *tp ){
				return( id );
			}
		}
	}

	return( -1 );
}