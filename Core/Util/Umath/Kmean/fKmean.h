/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/**********************
***   fKmean.cpp   ***
**********************/
#ifndef	_FKMEAN_TYPE_
#define _FKMEAN_TYPE_

#include	<math.h>
#include	<string.h>


#include	"Uigp/igp.h"




typedef struct fKmean_type {
	float m;

	float	dm;

	int	n;
	float	s;

}fKmean_type ;



int		fKmean( float a[], int nA, fKmean_type ak[], int nK );

int		fKmean( float a[], int nA, fKmean_type ak[], int nK, float d );

int		fKmean_histogram( int a[], int nA, fKmean_type ak[], int nK, float d );


int		fKmean_cyclic( fKmean_type ak[], int nK );


void	floatA_order( float a[], int nA );



//#ifdef __cplusplus
//}
//#endif

#endif