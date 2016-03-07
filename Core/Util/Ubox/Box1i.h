/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/*******************
 ***   Box1d.h   ***
 *******************/


#ifndef         BOX1D_TYPE
#define         BOX1D_TYPE

#ifdef __cplusplus
extern "C" {
#endif


#include	<stdio.h>

#include	"Uvec/Vec2d.h"




typedef struct box1i_type {

	int	i0;
	int	i1;

	int n;
} box1i_type;



typedef struct box1iA_type {
	int	NA;
	int	nA;

	box1i_type *a;
} box1iA_type;



typedef struct box1f_type {

	int	x0;
	int	x1;

	int n;
} box1f_type;



typedef struct box1fA_type {
	int	NA;
	int	nA;

	box1f_type *a;
} box1fA_type;



#define		BOX1D_SET( b, i0, i1 ) \
			{ \
				(b).i0 = i0;	\
			    (b).i1 = i1;	\
			}






#define         BOX1D_UPDATE( b, x, y ) \
			{ \
				if( x < (b).i0 )	(b).i0 = x; \
				else	if( x > (b).i1 )        (b).i1 = x; \
			}


#define         BOX1D_UPDATE_BOX( b, b1 ) \
{ \
				if( (b1).i0 < (b).i0 )	(b).i0 = (b1).i0; \
				if( (b1).i0 < (b).i0 )	(b).i0 = (b1).i0; \
}


#define         BOX1D_IS_IN( b, vx, vy ) \
			(	vx >= (b).i0 && vx <= (b).i1  )




box1iA_type *	box1iA_alloc( int n );

box1iA_type *box1iA_realloc( box1iA_type *avl, int n );

box1iA_type *box1iA_copy( box1iA_type *bavl, box1iA_type *avl );

void	box1iA_destroy( box1iA_type *avl );



box1fA_type *	box1fA_alloc( int n );

box1fA_type *box1fA_realloc( box1fA_type *avl, int n );

box1fA_type *box1fA_copy( box1fA_type *bavl, box1fA_type *avl );

void	box1fA_destroy( box1fA_type *avl );


void	box1i_print(  char *title, box1i_type ab[], int nP, FILE *fp );

void	box1iA_write(  char *title, box1iA_type *ab, FILE *fp );

int	box1i_overlap( box1i_type *b, box1i_type *b1, box1i_type *b2 );



#ifdef __cplusplus
}
#endif

#endif
