/**************************
 ***	ImageSampleG.c  ***
 **************************/

#include	<math.h>
//#include	<stdio.h>
#include	<string.h>

#include "Uigp/igp.h"

#include	"ImageType/ImageType.h"


image_type *
image_sampleG( image_type *sim, float srow, float scol )
{
image_type	*im;
int		i,	j,	i1,	j1, ii, jj, ind_i, ind_ij;
int		r,	g,	b;
int		i_fraction, j_fraction;

int		nrow, ncol, ndepth=4, npad=1, ncolumn_byte, nbyte;
float   *arj, ri, rj;
int		*aj1;
float	*a_r, *a_g, *a_b, *a_d;
float	*p_r, *p_g, *p_b, *p_d;
u_int	*p, *sp;
float	pi, pj, di, dj;
double  ci, cj;
float	si, sj;

	if ( srow < 1.0F || scol < 1.0F )
		return NULL;

	arj = (float *)malloc( sim->column*sizeof(float) );
	memset( arj, 0, sim->column*sizeof(float) );
	aj1 = (int *)malloc( sim->column*sizeof(int) );
	memset( aj1, 0, sim->column*sizeof(int) );

	nrow = (int)(sim->row/srow);
	ncol = (int)(sim->column/scol);
	im = image_create( nrow, ncol, 4, 1, NULL );

	ncolumn_byte = (( ncol*ndepth + npad -1 )/npad )*npad;
	npad = ncolumn_byte - ncol*ndepth;
	nbyte = nrow*ncolumn_byte;
	a_r = (float *)malloc( nbyte );
	a_g = (float *)malloc( nbyte );
	a_b = (float *)malloc( nbyte );
	a_d = (float *)malloc( nbyte );
	memset( a_r, 0, nbyte );
	memset( a_g, 0, nbyte );
	memset( a_b, 0, nbyte );
	memset( a_d, 0, nbyte );

	for( j = 0; j < sim->column ; j++ )
	{
		arj[j] = 1.0F;
		sj = ((float)(j))+1.0F;
		pj = (float)(fmod( sj, scol )); //sj = INT*scol + pj
		dj = (float)(modf( pj, &cj ));  //pj = cj + dj
		if ( cj < 1E-3 && dj > 1E-3 )
			arj[j] = dj;
		aj1[j] = (int)(((float)(j))/scol + 1E-3);
		if ( aj1[j] >= ncol ) aj1[j] = ncol - 1;
	}

	i_fraction = -1;
	for( i = 0; i < sim->row ; i = ii )
	{
		i1 = (int)(((float)(i))/srow + 1E-3);
		if ( i1 >= nrow ) i1 = nrow - 1;
		ri = 1.0F;
		si = ((float)(i))+1.0F;
		pi = (float)(fmod( si, srow )); //si = INT*srow + pi
		di = (float)(modf( pi, &ci ));  //pi = ci + di
		if ( ci < 1E-3 && di > 1E-3 )
			ri = di;

		if ( i && ri < 1.0F-1E-3 && i != i_fraction )
		{
			ri = 1.0F - ri;
			i_fraction = i;
		}
		else
		{
			ii = i + 1;
			i_fraction = -1;
		}

		ind_i = i1*ncol;

		j_fraction = -1;
		for( j = 0; j < sim->column ; j = jj )
		{
			j1 = aj1[j];
			rj = arj[j];

			if ( j && arj[j-1] < 1.0F-1E-3 && j != j_fraction )
			{
				rj = 1.0F - arj[j-1];
				j_fraction = j;
			}
			else
			{
				jj = j + 1;
				j_fraction = -1;
			}

			sp = IMAGE4_PIXEL( sim, i, j );
			ind_ij = ind_i+j1;

			a_r[ind_ij] += ri*rj*(((*sp)&0xFF0000)>>16);
			a_g[ind_ij] += ri*rj*(((*sp)&0x00FF00)>>8);
			a_b[ind_ij] += ri*rj*((*sp)&0x0000FF);
			a_d[ind_ij] += ri*rj;
		}
	}

	p_r = a_r;
	p_g = a_g;
	p_b = a_b;
	p_d = a_d;
	p = IMAGE4_PIXEL( im, 0, 0 );
	for( i1 = 0 ; i1 < nrow ; i1++ )
	for( j1 = 0 ; j1 < ncol ; j1++ )
	{
		if( *p_d > 0 )
		{
			r = (int)((*p_r++)/(*p_d));
			g = (int)((*p_g++)/(*p_d));
			b = (int)((*p_b++)/(*p_d++));
			*p++ = (r<<16) | (g<<8) | (b&0xFF);
		}
	}
	
	free( a_d );
	free( a_b );
	free( a_g );
	free( a_r );
	free( aj1 );
	free( arj );
	return( im );
}

image_type *
image_sampleG_transparent( image_type *sim, float srow, float scol )
{
image_type	*im;
int		i,	j,	i1,	j1, ii, jj, ind_i, ind_ij;
int		r,	g,	b;
int		i_fraction, j_fraction;

int		nrow, ncol, ndepth=4, npad=1, ncolumn_byte, nbyte;
float   *arj, ri, rj;
int		*aj1;
float	*a_r, *a_g, *a_b, *a_d, *a_tr;
float	*p_r, *p_g, *p_b, *p_d, *p_tr;
u_int	*p, *sp;
float	pi, pj, di, dj, tr;
double  ci, cj;
float	si, sj;

	if ( srow < 1.0F || scol < 1.0F )
		return NULL;

	arj = (float *)malloc( sim->column*sizeof(float) );
	memset( arj, 0, sim->column*sizeof(float) );
	aj1 = (int *)malloc( sim->column*sizeof(int) );
	memset( aj1, 0, sim->column*sizeof(int) );

	nrow = (int)(sim->row/srow);
	ncol = (int)(sim->column/scol);
	im = image_create( nrow, ncol, 4, 1, NULL );

	ncolumn_byte = (( ncol*ndepth + npad -1 )/npad )*npad;
	npad = ncolumn_byte - ncol*ndepth;
	nbyte = nrow*ncolumn_byte;
	a_r = (float *)malloc( nbyte );
	a_g = (float *)malloc( nbyte );
	a_b = (float *)malloc( nbyte );
	a_d = (float *)malloc( nbyte );
	a_tr= (float *)malloc( nbyte );

	memset( a_r, 0, nbyte );
	memset( a_g, 0, nbyte );
	memset( a_b, 0, nbyte );
	memset( a_d, 0, nbyte );
	memset( a_tr, 0, nbyte );

	for( j = 0; j < sim->column ; j++ )
	{
		arj[j] = 1.0F;
		sj = ((float)(j))+1.0F;
		pj = (float)(fmod( sj, scol )); //sj = INT*scol + pj
		dj = (float)(modf( pj, &cj ));  //pj = cj + dj
		if ( cj < 1E-3 && dj > 1E-3 )
			arj[j] = dj;
		aj1[j] = (int)(((float)(j))/scol + 1E-3);
		if ( aj1[j] >= ncol ) aj1[j] = ncol - 1;
	}

	i_fraction = -1;
	for( i = 0; i < sim->row ; i = ii )
	{
		i1 = (int)(((float)(i))/srow + 1E-3);
		if ( i1 >= nrow ) i1 = nrow - 1;
		ri = 1.0F;
		si = ((float)(i))+1.0F;
		pi = (float)(fmod( si, srow )); //si = INT*srow + pi
		di = (float)(modf( pi, &ci ));  //pi = ci + di
		if ( ci < 1E-3 && di > 1E-3 )
			ri = di;

		if ( i && ri < 1.0F-1E-3 && i != i_fraction )
		{
			ri = 1.0F - ri;
			i_fraction = i;
		}
		else
		{
			ii = i + 1;
			i_fraction = -1;
		}

		ind_i = i1*ncol;

		j_fraction = -1;
		for( j = 0; j < sim->column ; j = jj )
		{
			j1 = aj1[j];
			rj = arj[j];

			if ( j && arj[j-1] < 1.0F-1E-3 && j != j_fraction )
			{
				rj = 1.0F - arj[j-1];
				j_fraction = j;
			}
			else
			{
				jj = j + 1;
				j_fraction = -1;
			}

			sp = IMAGE4_PIXEL( sim, i, j );
			ind_ij = ind_i+j1;

			a_r[ind_ij] += ri*rj*(((*sp)&0xFF0000)>>16);
			a_g[ind_ij] += ri*rj*(((*sp)&0x00FF00)>>8);
			a_b[ind_ij] += ri*rj*((*sp)&0x0000FF);
			a_d[ind_ij] += ri*rj;
			a_tr[ind_ij] += ri*rj*(((*sp)&0x80000000)!=0 ? 1 : 0 );
		}
	}

	p_r = a_r;
	p_g = a_g;
	p_b = a_b;
	p_d = a_d;
	p_tr = a_tr;
	p = IMAGE4_PIXEL( im, 0, 0 );
	for( i1 = 0 ; i1 < nrow ; i1++ )
	for( j1 = 0 ; j1 < ncol ; j1++ )
	{
		if( *p_d > 0 )
		{
			r = (int)((*p_r++)/(*p_d));
			g = (int)((*p_g++)/(*p_d));
			b = (int)((*p_b++)/(*p_d));
			tr = (float)((*p_tr++)/(*p_d++));
			if ( tr >= 0.5F )
				*p++ = 0x80000000;
			else
				*p++ = (r<<16) | (g<<8) | (b&0xFF);
		}
	}
	
	free( a_tr );
	free( a_d );
	free( a_b );
	free( a_g );
	free( a_r );
	free( aj1 );
	free( arj );
	return( im );
}


