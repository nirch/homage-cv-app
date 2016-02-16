/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/***********************
 ***  PlnIterator.h  ***
 ***********************/
#ifndef _PL_ITERATOR_
#define _PL_ITERATOR_


#ifdef __cplusplus
extern "C" {
#endif


#include "Uigp/igp.h"



typedef struct  iPln_type {
	struct pln_type *pl;


	float	gt;

	vec2f_type	ctr;
	ln_type *l;
	float	t;

} iPln_type;



int	iPln_set( iPln_type *ip, struct pln_type *pl );

int		iPln_next( iPln_type *pi, float dt );




#ifdef __cplusplus
}
#endif

#endif