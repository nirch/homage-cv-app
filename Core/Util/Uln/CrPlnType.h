/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/******************
 ***  PtLine.h  ***
 ******************/
#ifndef _CR_PT_LINE_
#define _CR_PT_LINE_


#ifdef __cplusplus
extern "C" {
#endif


#include	"Uln/PlnType.h"



typedef struct crPln_type {
	int	iPl;
	pln_type	*pl;


	float	u;
	float	du;
	dPln_type	d[2];

	vec2f_type	mv;

	float	gt0,	gt1;
	int		direction;

	int		type;
	int		end;

} crPln_type;


typedef struct crPlnA_type {
	int	NA;
	int	nA;

	crPln_type	*a;

} crPlnA_type;


crPlnA_type *	crPlnA_alloc ( int n );


void	crPlnA_destroy ( crPlnA_type *ac );

void	crPlnA_destroy_cr( crPlnA_type *ac, int i0 );



int	crPln_createA( pln_type *bpl, plnA_type *apl, int i0,  float r1, float r2, int affine, crPlnA_type *ac );


int	crPln_create( pln_type *bpl, pln_type *pl, int iPl, float r1, float r2, int affine, crPln_type *cr );

int	crPln_createN( pln_type *bpl, pln_type *pl, int iPl, float r0, float r1, crPln_type cr[] );



int	crPlnA_parallel( pln_type *bpl, plnA_type *apl, int i0,  float r1, float r2, float aT, crPlnA_type *ac );
int	crPln_create_parallel( pln_type *bpl, pln_type *pl, int iPl, float r1, float r2, float aT, crPln_type cr[] );



void	crPln_inverse( crPln_type *c, int fPl );


int	crPln_duplicate( plnA_type *apl, int i0, float r, crPln_type acr[] );


	// get order side
int	crLineA_oreder_side( crPln_type acr[], int nAcr, int side, crPln_type *ad[], int *nD );

int	crPlnA_union( crPln_type acr[], int *nAcr );


void	crPlnA_direction( crPlnA_type *ac );

int	crPlnA_order( crPlnA_type *ac );

void	crPlnA_print( crPlnA_type *ac, FILE *fp );



#ifdef __cplusplus
}
#endif


#endif