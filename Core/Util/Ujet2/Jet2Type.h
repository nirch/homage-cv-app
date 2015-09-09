/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/**********************
 ***   Jet2Tool.h   ***
 **********************/
#ifndef         _JET2_TYPE_
#define         _JET2_TYPE_

#ifdef __cplusplus
extern "C" {
#endif


#include	<stdio.h>

#include        "Uvec/Vec2d.h"


#define		JET_ORDER	2
#define		COEFF_NO	( JET_ORDER + 1)*( JET_ORDER +2)/2

#define		JET_ORDER4	4
#define		COEFF_NO4	( JET_ORDER4 + 1)*( JET_ORDER4 +2)/2


//typedef	struct	jet2	{
//	float	j[ COEFF_NO ];
//} jet2;


typedef struct jet2_type {
    union {
        struct {
            float	a00;
			float	a10;
			float	a01;
			
			float	a20;
			float	a11;
			float	a02;
        };

        float j[ COEFF_NO ];
    };
} jet2_type;

#define	jet2	jet2_type


extern jet2	ZERO_JET2;
/*
static jet2	ZERO_JET2 = {	0,
				0, 0,
				0, 0, 0 };
*/
				/*
				0.0, 0.0, 0.0, 0.0,
				0.0, 0.0, 0.0, 0.0, 0.0 };
				*/


typedef struct jmap22 {
	jet2	f1;
	jet2	f2;
} jmap22;

typedef jmap22  jmap2;


typedef  struct  jet2_array {
	int	Ni,	Nj;

	int	NI,	NJ;

	float	dxy,	inv_dxy;
	float	dh;			/* 0.5 * dxy */
	float	Xmin,	Xmax;
	float	Ymin,	Ymax;

	jet2    **jet;
} jet2_array;




typedef struct djet2_type {	// a + a1*x + a2*y + lm1*x*x + lm2*y*y   
	vec2d	p;

	vec2d	n;


	float	a;
	float	a1,	a2;
	float	lm1,	lm2;

	float	dx,	dy;


}	djet2_type;



extern int	Coeff_no[JET_ORDER4+1],	Coeff_dgree[COEFF_NO4];
/*
int	Coeff_no[ JET_ORDER4+1 ] = { 1, 3, 6, 10, 15 };
int     Coeff_dgree[ COEFF_NO4 ] = {    0,
                                        1, 1,
                                        2, 2, 2,
                                        3, 3, 3, 3,
                                        4, 4, 4, 4, 4 };
*/


#define		J00( f )	(f).j[0]

#define		J10( f )	(f).j[1]
#define		J01( f )	(f).j[2]

#define		J20( f )	(f).j[3]
#define		J11( f )	(f).j[4]
#define		J02( f )	(f).j[5]

#define		J30( f )	(f).j[6]
#define		J21( f )	(f).j[7]
#define		J12( f )	(f).j[8]
#define		J03( f )	(f).j[9]

#define		J40( f )	(f).j[10]
#define		J31( f )	(f).j[11]
#define		J22( f )	(f).j[12]
#define		J13( f )	(f).j[13]
#define		J04( f )	(f).j[14]



#define         VALUE_JET2( jet, x, y ) \
				( x*(x* J20(jet) +  \
				y* J11(jet) + J10(jet) ) +  \
				y*(y* J02(jet) + J01(jet)) + J00(jet) )



#define		INNER2_JET2( f, g )	\
( J00(f)*J00(g) + J10(f)*J10(g) + J01(f)*J01(g) +J20(f)*J20(g) +\
  J11(f)*J11(g) + J02(f)*J02(g) )

#define		NORM2_JET2( f )	INNER2_JET2( f, f )


#define		JET2_ZERO( f )		{ \
								(f).a00 = 0; \
								(f).a10 = 0; \
								(f).a01 = 0; \
								(f).a20 = 0; \
								(f).a11 = 0; \
								(f).a02 = 0; \
								}




	/* jet2tool.c */
float	value_jet2( jet2 *f, int dgree, float x , float y );

void	value_dp_jet2( jet2 *f, float x, float y,
					float *val, float *dx, float *dy );

void	rescaling_jet2( jet2 *f, int dgree, float hx, float hy, jet2 *g );

void	shift_jet2( jet2 *f , float x , float y, jet2 *g );

jet2	sub_jet2( jet2 *g, jet2 *f, int dgree );

jet2	sum_jet2( jet2 *g, jet2 *f, int dgree );

jet2	scalar_product_jet2( jet2 *g, float s, int dgree );


void	difference_element_jet2( jet2 *f, float x, float y,
                                float dx0[], float dx1[], float dx2[] );


void	rotate_jet2( jet2 *f, vec2d *v, jet2 *g );
/* rotate the domian coordinet such that the new Y axis is vector v */ 


void	compos_jet2( jet2 *F, jmap22 *Q, jet2 *P );

void	inverse_jmap22( jmap22 *F, jmap22 *G );

void	normal_curve_jet2( jet2 *F, jmap22 *Q );

void 	derivative_x_jet2( jet2 *f, jet2 *fx );

void	derivative_y_jet2( jet2 *f, jet2 *fy );

void	divide_jet2( jet2 *g, jet2 *f );
	/* compute jet2 of  1/(g(x,y)  at (0,0)  */

void	product_jet2( jet2 *g, jet2 *f, jet2 *F );	
	/* compute the jet of g(x,y)*f(x,y)  at (0,0)	*/


#define		UNSET_ENV		0
#define		REGULAR_ENV		1
#define		SINGULAR_ENV		2


void 	jacobian_jmap22( jmap22 *F, jet2 *Jac );
	/* determinant of Jacobi matrix */

int	env_stat_jmap22( jmap22 *F, float h, float *xs, float *ys );


float	diff_jet2( jet2 *g, jet2 *f );


	/* Jet2Io.c */
void	print_jet2( FILE *fp, int dgree, jet2 *f );

void	load_jet2( FILE *fp, int dgree, jet2 *f );



	// Jet2Array.c

jet2_array *	ajet2_alloc();

void	ajet2_destroy( jet2_array *ajet );

void	jet2_array_load( char *jetmap_name, jet2_array *ajet );

void	jet2_array_realloc( jet2_array *ajet, int Ni, int Nj );

void	jet2_array_free( jet2_array *ajet );



	/* Jet2Map.c */

float	jet2map_value( jet2_array *ajet, float x, float y );

int	jet2map_jet2( jet2_array *ajet, float x, float y, jet2 *f );



	/* app_jet2.c */
void	app_jet2( jet2_array *ajet, float hx, float hy,
					float cx, float cy, jet2 *jet );


	// DiagonalJet2.c
int	diagonalN_jet2( jet2_type *f, float fxy, djet2_type *d );


void	diagonal_jet2(jet2 *fp, float a[3],
						float *lamda1, float *lamda2,
						vec2d *n );

void	diagonal_set_jet2( jet2_type *f, float a, float lm1, float lm2,
					float dx, float dy, vec2d *n );



#ifdef __cplusplus
}
#endif


#endif
