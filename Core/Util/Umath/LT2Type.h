/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/*******************
 ***   umath.h   ***
 *******************/
#ifndef	_LT2_TYPE_
#define _LT2_TYPE_

#include <stdio.h>
#include "Uvec/Vec2d.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct lt2_type {
	union {
		struct  {
			float	f1[3];
			float	f2[3];
		};

		struct  {
			float	a00,	a01,	a02;		// a00*x + a01*Y + a02
			float	a10,	a11,	a12;		// a10*x + a11*Y + a12
		};

		struct  {
			float	a0,	b0,	c0;		// a0*x + b0*Y + c0
			float	a1,	b1,	c1;		// a1*x + b1*Y + c1
		};

		float	a[2][2];
	};

	//float	f1[3];
	//float	f2[3];

}	lt2_type;




typedef struct lt2A_type {
	int			Ni,	Nj;
	int	Nij;

	float	x0,	y0;
	float	dx,	dy;

	lt2_type	*lt;

} lt2A_type;





#define	LT2_F1( lt, x, y )	( (lt).f1[0] * (x) + (lt).f1[1] * (y) + (lt).f1[2] )
#define	LT2_F2( lt, x, y )	( (lt).f2[0] * (x) + (lt).f2[1] * (y) + (lt).f2[2] )

#define	LT2_VALUE( lt, x, y, tx, ty ) \
	} \
		tx = LT2_F1( lt, x, y ); \
		ty = LT2_F2( lt, x, y ); \
	}


#define	LT2_SET_F1( lt, a, b, c )	{ (lt).f1[0] = a ; (lt).f1[1] = b; (lt).f1[2] = c; }
#define	LT2_SET_F2( lt, a, b, c )	{ (lt).f2[0] = a ; (lt).f2[1] = b; (lt).f2[2] = c; }

#define	LT2_SET_F1_A( lt )	( (lt).f1[0] )
#define	LT2_SET_F1_B( lt )	( (lt).f1[1] )
#define	LT2_SET_F1_C( lt )	( (lt).f1[2] )

#define	LT2_SET_F2_A( lt )	( (lt).f2[0] )
#define	LT2_SET_F2_B( lt )	( (lt).f2[1] )
#define	LT2_SET_F2_C( lt )	( (lt).f2[2] )


#define	LT2_F1_A( lt )	( (lt).f1[0] )
#define	LT2_F1_B( lt )	( (lt).f1[1] )
#define	LT2_F1_C( lt )	( (lt).f1[2] )

#define	LT2_F2_A( lt )	( (lt).f2[0] )
#define	LT2_F2_B( lt )	( (lt).f2[1] )
#define	LT2_F2_C( lt )	( (lt).f2[2] )

	// LT2Tool.c
void	lt2_clear( lt2_type *lt );

void	lt2_unity( lt2_type *lt );

int		lt2_inverse( lt2_type *t, lt2_type *it );

void	lt2_shift( lt2_type *t, float x0, float y0, lt2_type *it );

void	lt2_scale( lt2_type *t, float scale );


void	lt2_compose( lt2_type *t, lt2_type *s, lt2_type *ct );

void	lt2_subtruct( lt2_type *lt0, lt2_type *lt1, lt2_type *lt );

void	lt2_max_diff( lt2_type *lt1, lt2_type *lt2, lt2_type *LT );

void	lt2_print( FILE *fp, lt2_type *lt );



lt2A_type *	lt2A_alloc( int ni, int nj );

void	lt2A_destroy( lt2A_type *ltA );

void	lt2A_unity( lt2A_type *ltA );


void	lt2A_set( lt2A_type *ltA, lt2_type *glt );

void	lt2A_scale( lt2A_type *ltA, float f );

void	lt2A_value_lt( lt2A_type *alt, float x, float y, lt2_type *tlt );

void	lt2A_value( lt2A_type *alt, float x, float y, float *u, float *v );

void	lt2A_valueW( lt2A_type *alt, float x, float y, float *u, float *v );

void	lt2_affine_setV( lt2_type *lt, float a, float b, float s, float t );

void	lt2_affine_set( lt2_type *lt, float a, float b, float angle, float scale );

void	lt2_affine_get( lt2_type *lt, float *a, float *b, float *angle, float *scale );


int		lt2_affine( lt2_type *lt, vec2f_type s[3], vec2f_type t[3] );

void	lt2_swap( lt2_type *lt, lt2_type *sw );



	// Lt2Similarity.c
int	lt2_similarity( lt2_type *lt, vec2f_type *sp0, vec2f_type *sp1, vec2f_type *tp0, vec2f_type *tp1 );



void	lt2_dump( lt2_type *lt, char *prefix, int index, char *suffix );

int		lt2_write_to_file( lt2_type *lt, char *file );

void	lt2_write( lt2_type *lt, FILE *fp );

int		lt2_read_file( lt2_type *lt, char *file );

void	lt2_read( lt2_type *lt, FILE *fp );




#ifdef _DUMP
#define LT2_DUMP( lt, name, index, ext )  lt2_dump( lt, name, index, ext )
#else
#define LT2_DUMP( lt, name, index, ext )
#endif

#ifdef __cplusplus
}
#endif

#endif