/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/****************************************************************
*								*
*		Module:  Jet2_3Type.h				*
*		jets of order 3 of 2 variables.			*
****************************************************************/
#ifndef         _JET2_3_TYPE_
#define         _JET2_3_TYPE_


#include	"Uvec/Vec2d.h"
#include	"Jet2Type.h"


#define		JET_ORDER3	3
#define         COEFF_NO3	10
		/*	(( JET_ORDER3 + 1)*( JET_ORDER3 +2)/2) */

//typedef	struct	jet23	{
//	float	j[ ( JET_ORDER3 + 1)*( JET_ORDER3 +2)/2 ];
//} jet23;

typedef struct jet3_type {
    union {
        struct {
            float	a00;

			float	a10;
			float	a01;
			
			float	a20;
			float	a11;
			float	a02;

			float	a30;
			float	a21;
			float	a12;
			float	a03;
        };

        float j[ ( JET_ORDER3 + 1)*( JET_ORDER3 +2)/2 ];
    };
} jet3_type;

#define	jet23	jet3_type
typedef jet23  jet3;


typedef struct jmap23 {
	jet23	f1;
	jet23	f2;
} jmap23;




typedef  struct  jet23_array {
		int     Ni,    Nj;
		float   dxy,    inv_dxy;
		float   dh;                     /* 0.5 * dxy */
		float   Xmin,   Xmax;
		float   Ymin,   Ymax;
 
		jet23    **jet;
} jet23_array;


/*
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
*/


#define	VALUE_JET23( f, x, y ) \
				  (f->a00 + x*(f->a10 + x*(f->a20 + x*f->a30) )) +\
				y*(f->a01 + x*(f->a11 + x*f->a21) +\
				y*(f->a02 + x* f->a12 +\
				y*(f->a03) ))


#define	VALUE_DIVY_JET23( f, x, y ) \
				(f->a01 + x*(f->a11 + x*f->a21) +\
				2*y*(f->a02 + x* f->a12 +\
				3*y*(f->a03) ))

#define	VALUE_DIVX_JET23( f, x, y ) \
				  ( (f->a10 + x*(2*f->a20 + x*3*f->a30) )) +\
				y*( (f->a11 + x* 2*f->a21) +\
				y*(  f->a12 ))

	/* Jet2_3Tool.c */
void	zero_jet23( jet23 *f );

void	zero_jmap23( jmap23 *f );

void	identity_jmap23( jmap23 *F );
void	value_jmap23( jmap23 *F, vec2d *p, vec2d *val );

void	scalar_product_jet23( jet23 *g, float s, jet23 *F );

void	sum_jet23( jet23 *g, jet23 *f, jet23 *F );

void	sub_jet23( jet23 *g,  jet23 *f, jet23 *F );

void	shift_jet23( jet23 *f, float x, float y, jet23 *g );

void	product_jet23( jet23 *g, jet23 *f, jet23 *F );	/* F = g*f */

void	productAsum_jet23( jet23 *g, jet23 *f, jet23 *F );	/* F += g*f */

void	productACsum_jet23( jet23 *g, jet23 *f, float a, jet23 *F ); // F += a* g*f


void	divide_jet23( jet23 *g, jet23 *F ); 		/* F = 1/(g(x,y) */

void	rescaling_jet23( jet23 *f, float hx, float hy, jet23 *g );

void	rotate_jet23( jet23 *f, vec2d *v, jet23 *g );

void	compos_jet23( jet23 *F, jmap23 *Q, jet23 *P );

void	jacobian_jmap23( jmap23 *F, jet2 *Jac );

void	substit_jmap23( jmap23 *F, jmap23 *Q, jmap23 *FQ );

void	derivative_x_jet23( jet23 *f, jet23 *fx );

void	derivative_y_jet23( jet23 *f, jet23 *fy );

float	diff_jet23( jet23 *g, jet23 *f );


	/* InverseJet22_3.c */
void	inverse_jmap23( jmap23 *F, jmap23 *g );

	/* FoldJet22_3.c */
void	fold_jmap23( jmap23 *F, jmap23 *R, jmap23 *L, float *a, float *b );
void	fold_jmap23_test( jmap23 *F, jmap23 *R, jmap23 *L, float a, float b );


	/* CuspJet22_3.c */
void	cusp_jmap23( jmap23 *F, jmap23 *R, jmap23 *L,
					float *a, float *b, float *c );
void	cusp_jmap23_test( jmap23 *F, jmap23 *R, jmap23 *L,
					float a, float b, float c );




	/* Jet2_3Array.c */

void	jet23_array_load( char *jetmap_name, jet23_array *ajet );

void	jet23_array_realloc( jet23_array *ajet, int Ni, int Nj );

void	jet23_array_free( jet23_array *ajet );



	/* Jet2_3Map.c */

float	jet23map_value( jet23_array *ajet, float x, float y );

int	jet23map_jet23( jet23_array *ajet, float x, float y, jet23 *f );

void	jet23mapB_jet23( jet23_array *ajet, float x, float y, jet23 *f );



#endif
