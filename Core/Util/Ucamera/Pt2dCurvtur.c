/***********************
 ***  Pt2dCurvtur.c  ***
 ***********************/
#include <string.h>
#include <math.h>
#include "Uigp/igp.h"

#include "Pt2dType.h"




void
pt2dA_curvtur( pt2dA_type *apt )
{
pt2d_type	*pt;
vec2f_type	p,	v;
int	i,	i0,	i1;
poly1d4_type pl;
float	t,	d;


	for( i = 0 ; i < apt->nP ; i++ ){

		pt = &apt->p[i];

		i0 = i - 2;
		//if( i <= 1 )
		//	i0 = 0;
		//else	
		//	if( i >= apt->nP-2 )
		//		i0 = apt->nP - 5;

		if( i0 < 0 )	i0 = 0;
		if( i0+5 > apt->nP ){
			i0 = apt->nP-5;
			if( i0 < 0 )	i0 = 0;
		}


		if( ( i1 = i0 + 5 ) > apt->nP )
			i1 = apt->nP;


		pt2d_approximate_line_pv( apt, i0, i1, &p, &v, &d, NULL );



		pt2d_approximate_polynom_2( apt, &pt->p, &v, i0, i1, &pl );

//		pt->p.x += pl.a0 * -v.y;
//		pt->p.y += pl.a0 * v.x;

		pt->n.x = v.x + pl.a1 * -v.y;
		pt->n.y = v.y + pl.a1 * v.x;

		t = hypot( pt->n.x, pt->n.y );
		pt->n.x /= t;
		pt->n.y /= t;

		pt->r = poly1d4_curvtur( &pl, 0 );
	}
}




float 
pt2dA_curvtur_1( pt2dA_type *apt, vec2f_type *p0,int i0, int i1, vec2f_type *n, float *curvtur )
{
	poly1d4_type pl;
	float	t,	d;
	vec2f_type	p,	v;


	pt2d_approximate_line_pv( apt, i0, i1, &p, &v, &d, NULL );

	pt2d_approximate_polynom_2( apt, p0, &v, i0, i1, &pl );


	n->x = v.x + pl.a1 * -v.y;
	n->y = v.y + pl.a1 * v.x;

	t = hypot( n->x, n->y );
	n->x /= t;
	n->y /= t;

	*curvtur = poly1d4_curvtur( &pl, 0 );


	return( *curvtur );
}




pt2dA_type *
pt2dA_smooth( pt2dA_type *apt, float r1, pt2dA_type	*tapt )
{
pt2d_type	*pt,	*tpt;
vec2f_type	p,	v;
int	i,	i0;
poly1d4_type pl;
float	t,	b,	d;


//	if( tapt == NULL )
//		tapt = pt2dA_alloc( apt->nP );

	tapt = pt2dA_realloc( tapt, apt->nP );

	tapt->nP = 0;


	for( i = 0 ; i < apt->nP ; i++ ){

		pt = &apt->p[i];

		i0 = i - 2;
		if( i <= 1 )
			i0 = 0;
		else	
			if( i >= apt->nP-2 )
				i0 = apt->nP - 5;



		
		pt2d_approximate_line_pv( apt, i0, i0+5, &p, &v, &d, NULL );



		pt2d_approximate_polynom_2( apt, &pt->p, &v, i0, i0+5, &pl );



		tpt = &tapt->p[i];
		tpt->p.x = pt->p.x + pl.a0 * -v.y;
		tpt->p.y = pt->p.y + pl.a0 * v.x;


		tpt->n.x = v.x + pl.a1 * -v.y;
		tpt->n.y = v.y + pl.a1 * v.x;

		t = hypot( pt->n.x, pt->n.y );
		tpt->n.x /= t;
		tpt->n.y /= t;

		b = poly1d4_curvtur( &pl, 0 );

	//	fprintf( stdout, "%.4f  %.4f\n", pl.a2, b );


		//		pt2d_approximate_polynom_2( apt, i, nI, &pt->p, &pt->n, 6*r, &pl0 );
		//fprintf( stdout, "%.4f  %.4f\n", pl.a2, pl0.a2 );

		tapt->nP++;
	}

	tapt->nP = apt->nP;
	tapt->type = PT2D_4V;
//	pt2dA_dump( tapt, "1", 1, NULL );



	return( tapt );
}




pt2dA_type *
	pt2dA_smoothN( pt2dA_type *apt, int dr, int fclose, pt2dA_type	*tapt )
{
	pt2d_type	*pt,	*tpt;
	vec2f_type	p,	v;
	int	i,	i0;
	poly1d4_type pl;
	float	t,	b,	d;
	int	di,	nP;
	int	nr;


	tapt = pt2dA_realloc( tapt, apt->nP );

	tapt->nP = 0;

	di = 0;
	nP = apt->nP;
	if( fclose == 1 ){
		for( i = 0 ; i < dr ; i++ )
			apt->a[apt->nA+i] = apt->a[i];

		di = dr;
		nP = apt->nP+dr;
	}

	nr = 2*dr + 1;

	for( i = 0 ; i < apt->nP ; i++ ){

		pt = &apt->p[i];

		i0 = i - dr+di;
		if( i <= 1 )
			i0 = 0;
		else	
			if( i >= nP-dr )
				i0 = apt->nP - nr;




		pt2d_approximate_line_pv( apt, i0, i0+nr, &p, &v, &d, NULL );



		pt2d_approximate_polynom_2( apt, &pt->p, &v, i0, i0+nr, &pl );



		tpt = &tapt->p[i];
		tpt->p.x = pt->p.x + pl.a0 * -v.y;
		tpt->p.y = pt->p.y + pl.a0 * v.x;


		tpt->n.x = v.x + pl.a1 * -v.y;
		tpt->n.y = v.y + pl.a1 * v.x;

		t = hypot( pt->n.x, pt->n.y );
		tpt->n.x /= t;
		tpt->n.y /= t;

		b = poly1d4_curvtur( &pl, 0 );

		//	fprintf( stdout, "%.4f  %.4f\n", pl.a2, b );


		//		pt2d_approximate_polynom_2( apt, i, nI, &pt->p, &pt->n, 6*r, &pl0 );
		//fprintf( stdout, "%.4f  %.4f\n", pl.a2, pl0.a2 );

		tapt->nP++;
	}

	tapt->nP = apt->nP;
	tapt->type = PT2D_4V;
	//	pt2dA_dump( tapt, "1", 1, NULL );



	return( tapt );
}
