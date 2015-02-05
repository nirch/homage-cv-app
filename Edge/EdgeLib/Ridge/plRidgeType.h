/***********************
 ***  PlRidgeType.h  ***
 ***********************/


#ifndef _PL_RIDGE_H_
#define _PL_RIDGE_H_

#include "Uln/PlnType.h"





typedef struct plRidge_type {
	int	state;
	int	id;

	vec2d	p;
	vec2d	v;
	float	d;

	pln_type	*pl;



	float	f;
	float	lm;


	int	nR,	nL;
	float fR,	fL;


	
} plRidge_type;





typedef struct plRidgeA_type {
	int	NR;
	int	nR;

	plRidge_type	**r;

} plRidgeA_type;




	// plRidge.cpp
void plRidge_draw( struct gpw_type *gpw, plRidge_type *c );

plRidge_type * plRidge_alloc();

void plRidge_destroy( plRidge_type *c );

void	plRidgeA_decrease( plRidgeA_type *ar );

void plRidge_translate( plRidge_type *c, vec2f_type *ctr );

void plRidgeA_translate( plRidgeA_type *ar, float x, float y );



plRidgeA_type * plRidgeA_alloc( int n );

plRidgeA_type * plRidgeA_realloc( plRidgeA_type *ar, int n );

void	plRidgeA_add( plRidgeA_type *ar, plRidge_type *pl );


void plRidgeA_destroy( plRidgeA_type *ar );


int	plRidgeA_select( plRidgeA_type *ar, vec2f_type *p, float D );

plnA_type *	plRidgeA_to_plnA( plRidgeA_type *ar, float Len, int fCopy );


	// PlRidgeUnion.cpp
int	plRidgeA_union( plRidgeA_type *ar, float dLen, float dT, float aT );


#endif