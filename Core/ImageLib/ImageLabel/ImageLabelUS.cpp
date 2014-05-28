/****************************
 ***   ImageLabelUS.cpp   ***
 ****************************/
#include <math.h>
#include "Uigp/igp.h"


#include "Umath/Matrix2Type.h"
#include "Umath/EigenType.h"

#include "ImageType/ImageType.h"
#include "ImageLabel.h"

#include "BwLabel.h"

static int	math_linear_equation2_symtric_eigenvalue( float xx, float xy, float yy,
							  float *e1, vec2d *v1, float *e2 );




imageLabel_type *
imageLabelUS( image_type *sim, int T, int inv, int margin, imageLabel_type *abw )
{
	int	i,	j;
	u_char	*sp;
	short	*bp;


	if( abw == NULL ){
		abw = imageLabel_alloc();
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


	//	image2_bwLabel( abw->im, &abw->a, &abw->nA );
	imageLabelUS_create( abw->im, &abw->a, &abw->nA );


	return( abw );
}



void
image2_bwLabel( image_type *im, bwLabel_type **tbw, int *tnBw )
{
	*tbw = NULL;

	imageLabelUS_create(  im, tbw, tnBw );
}


void
imageLabelUS_create( image_type *im, bwLabel_type **tbw, int *tnBw )
{
bwLabel_type	*bw;
int	nBw;

int	i,	j,	k;
short	*sp,	*sp0,	*sp1;
int	id,	id1;


	bw = *tbw;
	if( bw == NULL )
		bw = (bwLabel_type *)malloc( im->row*im->column*sizeof(bwLabel_type) );
	nBw = 0;



	bw[nBw].id = nBw;
	bw[nBw].no = 0;
	nBw++;

	sp = (short*)im->data ;
	if( *sp != 0 ){
		bw[nBw].id = nBw;
		bw[nBw].no = 0;
		bw[nBw].color = 1;
		*sp = nBw++;
		bw[*sp].no++;
	}

	sp++;
	sp0 = sp-1;
	for( j = 1 ; j < im->column ; j++, sp++, sp0++ ){
		if( *sp == 0 )	continue;

		if( *sp0 != 0 ){
			*sp = bw[*sp0].id;
			bw[*sp].no++;
			continue;
		}

		bw[nBw].id = nBw;
		bw[nBw].no = 0;
		bw[nBw].color = 1;
		*sp = nBw++;
		bw[*sp].no++;
	}

		
	for( i = 1 ; i < im->row ; i++ ){
		sp = (short*)IMAGE_PIXEL( im, i, 0 );
		sp1 = sp - im->column;
		if( *sp != 0 ){
			if( *sp1 != 0 ){
				*sp = bw[*sp1].id;
				bw[*sp].no++;
			}
			else	{
				bw[nBw].id = nBw;
				bw[nBw].no = 0;
				bw[nBw].color = 1;
				*sp = nBw++;
				bw[*sp].no++;
			}
		}


		sp++;
		sp1++;
		sp0 = sp-1;
		for( j = 1 ; j < im->column ; j++, sp++, sp1++, sp0++ ){
			if( *sp == 0 )	continue;
			if( *sp0 != 0 ){
				*sp = bw[*sp0].id;
				bw[*sp].no++;
//				if( *sp1 == 0 || *sp1 == *sp || bw[*sp].id == bw[*sp1].id )	continue;
				if( *sp1 == 0 || bw[*sp].id == bw[*sp1].id )	continue;



				if( bw[*sp].id < bw[*sp1].id ){
					id1 = bw[*sp1].id;
					id = bw[*sp].id;
				}
				else {
					id = bw[*sp1].id;
					id1 = bw[*sp].id;
				}

				for( k = id1 ; k < nBw ; k++ ){
					if( bw[k].id == id1 )
						bw[k].id = id;
				}

				bw[id].no += bw[id1].no;


				continue;
			}

			if( *sp1 != 0 ){
				*sp = bw[*sp1].id;
				bw[*sp].no++;

				continue;
			}

			bw[nBw].id = nBw;
			bw[nBw].no = 0;
			bw[nBw].color = 1;
			*sp = nBw++;
			bw[*sp].no++;
		}
	}

	*tbw = bw;
	*tnBw = nBw;

		
	imageLabel2_set_id( im, *tbw );
}


void
image2_bwLabelV( image_type *im, bwLabel_type **tbw, int *tnBw )
{
bwLabel_type	*bw;
int	nBw;

int	i,	j,	k;
short	*sp,	*sp0,	*sp1;



	bw = (bwLabel_type *)malloc( im->row*im->column*sizeof(bwLabel_type) );
	nBw = 0;

	sp = (short*)im->data ;
	if( *sp != 0 ){
		bw[nBw].id = nBw;
		bw[nBw].no = 1;
		bw[nBw].color = *sp;

		*sp = nBw++;
	}

	sp++;
	sp0 = sp-1;
	for( j = 1 ; j < im->column ; j++, sp++, sp0++ ){
		if( *sp == 0 )	continue;

		if( *sp0 != 0 && bw[*sp0].color == *sp ){
			*sp = bw[*sp0].id;
			bw[*sp].no++;
			continue;
		}

		bw[nBw].id = nBw;
		bw[nBw].no = 1;
		bw[nBw].color = *sp;

		*sp = nBw++;
	}

		
	for( i = 1 ; i < im->row ; i++ ){
		sp = (short*)IMAGE_PIXEL( im, i, 0 );
		sp1 = sp - im->column;
		if( *sp != 0 ){
			if( *sp1 != 0 && bw[*sp1].color == *sp){
				*sp = bw[*sp1].id;
				bw[*sp].no++;
			}
			else	{
				bw[nBw].id = nBw;
				bw[nBw].no = 1;
				bw[nBw].color = *sp;
				*sp = nBw++;
			}
		}


		sp++;
		sp1++;
		sp0 = sp-1;
		for( j = 1 ; j < im->column ; j++, sp++, sp1++, sp0++ ){
			if( *sp == 0 )	continue;
			if( *sp0 != 0 ){
				*sp = bw[*sp0].id;
				bw[*sp].no++;
				if( *sp1 == 0 || *sp1 == *sp )	continue;

				if( bw[*sp].id < bw[*sp1].id ){

					for( k = 0 ; k < nBw ; k++ ){
						if( bw[k].id == bw[*sp1].id )
							bw[k].id = bw[*sp].id;
					}

					bw[*sp].no += bw[*sp1].no;
					bw[*sp1].id = bw[*sp].id;

				}
				else {
					for( k = 0 ; k < nBw ; k++ ){
						if( bw[k].id == bw[*sp].id )
							bw[k].id = bw[*sp1].id;
					}
					bw[*sp1].no += bw[*sp].no;
					bw[*sp].id = bw[*sp1].id;
				}
				continue;
			}

			if( *sp1 != 0 ){
				*sp = bw[*sp1].id;
				bw[*sp].no++;

				continue;
			}

			bw[nBw].id = nBw;
			bw[nBw].no = 0;
			*sp = nBw++;
			bw[*sp].no++;
		}
	}

	*tbw = bw;
	*tnBw = nBw;


	imageLabel2_set_id( im, *tbw );
}







void
imageLabel2_set_id( image_type *im, bwLabel_type *bw )
{
short	*sp;
int	i,	j;

	sp = (short *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++ ){
				*sp = bw[*sp].id;
		}
	}
}


void
imageLabel2_set_boundary( image_type *im, bwLabel_type *abw, int nB )
{
	short	*sp;
	int	i,	j;

	for( i = 0 ; i < nB ; i++ )
		abw[i].boundary = 0;

	sp = (short *)im->data;
	for( j = 0 ; j < im->column ; j++, sp++ )
		abw[*sp].boundary = 1;


	sp = (short *)IMAGE_PIXEL( im, im->height-1, 0 );
	for( j = 0 ; j < im->column ; j++, sp++ )
			abw[*sp].boundary = 1;


	sp = (short *)IMAGE_PIXEL( im, 0, 0 );
	for( i = 0 ; i < im->row ; i++, sp += im->width )
		abw[*sp].boundary = 1;


	sp = (short *)IMAGE_PIXEL( im, 0, im->width-1 );
	for( i = 0 ; i < im->row ; i++, sp += im->width )
		abw[*sp].boundary = 1;
}




void
imageLabelUS_value( imageLabel_type *abw, image_type *sim )
{
	int	i,	j;
	short	*bp;
	u_char	*sp;

	bwLabel_type *bw;

	for( i = 0 ; i < abw->nA ; i++ ){
		abw->a[i].no = 0;
		abw->a[i].av = 0;
		abw->a[i].var = 0;
	}



	sp = sim->data;
	bp = abw->im->data_s;
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
imageLabel2_set_value( image_type *sim, image_type *im, bwLabel_type *abw, int nBw )
{
	int	i,	j;
	short	*bp;
	u_char	*sp;
	
	bwLabel_type *bw;

	for( i = 0 ; i < nBw ; i++ ){
		abw[i].no = 0;
		abw[i].av = 0;
		abw[i].var = 0;
	}



	sp = sim->data;
	bp = im->data_s;
	for( i = 0 ; i < im->height ; i++ ){
		for( j = 0 ; j < im->width ; j++, bp++, sp++ ){
			bw = &abw[*bp];
			bw->no++;
			bw->av += *sp;
			bw->var += *sp * *sp;
		}
	}



	for( i = 0 ; i < nBw ; i++ ){
		bwLabel_type *bw = &abw[i];

		if( bw->no == 0  )	continue;

		bw->av /= bw->no;
		bw->var = bw->var / bw->no - bw->av*bw->av;
	}
}

void
image2_bwLabel_set_aux( image_type *im, bwLabel_type *aBw, int nBw )
{
int	i,	j;
short	*sp;
bwLabel_type *bw;

	for( i = 0 ; i < nBw ; i++ )
		aBw[i].no = 0;


	sp = im->data_s;
	for( i = 0 ; i < im->height ; i++ ){
		for( j = 0 ; j < im->width ; j++, sp++ ){
			bw = &aBw[*sp];

			*sp = bw->id;
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
imageLabelUS_set_box( imageLabel_type *abw )
{

	int	i,	j;
	bwLabel_type *bw;

	for( i = 0 ; i < abw->nA ; i++ )
		abw->a[i].no = 0;


	u_short *sp = abw->im->data_us;
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
	}
}



void
imageLabelUS_set_mass( imageLabel_type *abw )
{
	int	i,	j;
	bwLabel_type *bw;

	for( i = 0 ; i < abw->nA ; i++ ){
		abw->a[i].no = 0;
		abw->a[i].p.x = abw->a[i].p.y = 0;
	}


	u_short *sp = abw->im->data_us;
	for( i = 0 ; i < abw->im->height ; i++ ){
		int val = *sp++;
		int n = 1;
		int sj = 0;
		for( j = 1 ; j < abw->im->width ; j++, sp++ ){

			if( *sp == val ){
				n++;
				sj += j;
				continue;
			}


			bw = &abw->a[val];

			bw->no += n;
			bw->p.x += sj;
			bw->p.y += n * i;

			val = *sp;
			n = 1;
			sj = j;
		}
	}

	for( i = 0 ; i < abw->nA ; i++ ){
		if( abw->a[i].id != i )	continue;
		abw->a[i].p.x /= abw->a[i].no;
		abw->a[i].p.y /= abw->a[i].no;
	}
}


void
image2_bwLabel_remove_boundary( image_type *im, bwLabel_type *bw )
{
short	*sp;
int	i,	j;

	sp = (short *)im->data;
	for( j = 0 ; j < im->column ; j++, sp++ ){
		if( *sp != 0 )
			bw[*sp].id = -1;
	}

	sp = (short *)IMAGE_PIXEL( im, (im->row-1), 0 );
	for( j = 0 ; j < im->column ; j++, sp++ ){
		if( *sp != 0 )
			bw[*sp].id = -1;
	}



	sp = (short *)im->data;
	for( i = 0 ; i < im->row ; i++, sp += im->column ){
		if( *sp != 0 )
			bw[*sp].id = -1;
	}

	sp = (short *)IMAGE_PIXEL( im, 0, im->column-1 );
	for( i = 0 ; i < im->row ; i++, sp += im->column ){
		if( *sp != 0 )
			bw[*sp].id = -1;
	}
}

typedef	struct bwAux_type	{
	int	n;

	vec2d	p;

	float	xx,	xy,	yy;

}	bwAux_type;

void
image2_bwLabel_set_direction( image_type *im, bwLabel_type *bw, int nBw )
{
short	*sp;
int	i,	j;
bwAux_type	*baw,	*ba;
bwLabel_type	*b;
float	x,	y;


	baw = ( bwAux_type *)malloc( nBw * sizeof( bwAux_type) );

	for( i = 0 ; i < nBw ; i++ ){
		ba = &baw[i];
		ba->p.x = ba->p.y = ba->xx = ba->xy = ba->yy = 0;
		ba->n = 0;
	}

	sp = (short *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++ ){
			if( *sp == 0 )	continue;
			ba = &baw[*sp];

			ba->n++;
			ba->p.x += j;
			ba->p.y += i;
		}
	}

	for( i = 0 ; i < nBw ; i++ ){
		ba = &baw[i];
		if( ba->n == 0 )	continue;

		ba->p.x /= ba->n;
		ba->p.y /= ba->n;
	}
			

	sp = (short *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++ ){
			if( *sp == 0 )	continue;
			ba = &baw[*sp];

			x = j - ba->p.x;
			y = i - ba->p.y;
			ba->xx += x*x;
			ba->xy += x*y;
			ba->yy += y*y;
		}
	}


	for( i = 0 ; i < nBw ; i++ ){
		ba = &baw[i];
		if( ba->n == 0 )	continue;

		ba->xx /= ba->n;
		ba->xy /= ba->n;
		ba->yy /= ba->n;

		b = &bw[i];
		math_linear_equation2_symtric_eigenvalue( ba->xx, ba->xy, ba->yy,
							  &b->e1, &b->v1, &b->e2 );

		b->p = ba->p;
	}

	free( baw );
}





// A[0][1] = A[1][0]
int
math_linear_equation2_symtric_eigenvalue( float xx, float xy, float yy,
							  float *e1, vec2d *v1, float *e2 )
{
float	a,	b,	c;
float	desc,	t;


	a = xx;
	b = xy;
	c = yy;


	desc = hypot( a-c, 2*b );
	*e1 = ((a+c) + desc ) * 0.5;
	*e2 = ((a+c) - desc ) * 0.5;

	v1->x = -b;
	v1->y = a - *e1;

	t = hypot( v1->x, v1->y);
	v1->x /= t;
	v1->y /= t;

	return( 1 );
}



int
bwLabel_gesture_id( bwLabel_type *bw, int nBw, int T )
{
int	i,	no;
bwLabel_type	*b;

	for( i = 0, no = 0 ; i < nBw ; i++ ){
		b = &bw[i];
		if( b->id != i )	continue;
		if( b->no < T )		continue;

		if( ABS(b->e2) < 60 )	continue;

		if( ABS( b->v1.x ) > ABS(b->v1.y) )
			no += 6;
		else no += 1;
	}

	return( no );
}



int
imageLabelUS_eigen2d( imageLabel_type *abw, int id,  eigen2d_type *e )
//imageLabelUI_eigen2d(  m_bw->im, i, & m_bw->a[i].b, &e );
{
	int	i,	j,	n;
	u_short	*tp;
	float	sx,	sy;

	matrix2_type	m;
	matrix2_zero( &m );

	sx = sy = 0;
	box2i_type *b = &abw->a[id].b;

	for( i = b->y0, n = 0 ; i <= b->y1 ; i++ ){
		tp = (u_short *)IMAGE_PIXEL( abw->im, i, b->x0 );
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


#ifdef _TEST_
	matrix2_type	m3;
	matrix2S_eigen_inv( &m3, e->e1, &e->v1, e->e2 );
#endif

	return( n );

}


int
imageLabelUS_eigen2d_matrix( imageLabel_type *abw, int id,  vec2f_type *p, matrix2_type *m )

{
	int	i,	j,	n;
	u_short	*tp;
	float	sx,	sy;


	matrix2_zero( m );

	sx = sy = 0;
	box2i_type *b = &abw->a[id].b;

	for( i = b->y0, n = 0 ; i <= b->y1 ; i++ ){
		tp = (u_short *)IMAGE_PIXEL( abw->im, i, b->x0 );
		for( j = b->x0 ; j <= b->x1 ; j++, tp++ ){
			if( *tp != id )	continue;

			sx += j;
			sy += i;

			m->a00 += j*j;
			m->a01 += j*i;
			m->a11 += i*i;
			n++;
		}
	}

	sx /= n;
	sy /= n;

	m->a00 = m->a00 / n - sx*sx;
	m->a01 = m->a01 / n - sx*sy;
	m->a11 = m->a11 / n - sy*sy;
	m->a10 = m->a01;


	return( 1 );

}