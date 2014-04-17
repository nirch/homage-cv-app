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





typedef struct crPln_type {
	int	iPl;
	pln_type	*pl;


	float	u;
	float	du;
	dPln_type	d[2];

	int		type;

} crPln_type;




int	crPln_create( pln_type *bpl, pln_type *pl, int iPl, float r1, float r2, crPln_type *cr );

int	crPln_createN( pln_type *bpl, pln_type *pl, int iPl, float r1, float r2, crPln_type cr[] );


void	crPln_inverse( crPln_type *c, int fPl );


#ifdef __cplusplus
}
#endif


#endif