
#ifndef _CSEG_TYPE_
#define _CSEG_TYPE_


#include	<stdio.h>
#include	<math.h>


#ifdef _DEBUG
//#define	_DUMP
#endif
	
#include	"Uigp/igp.h"
#include	"Utime/GpTime.h"
#include	"Ulog/Log.h"





typedef struct cseg_type {

	int	i0;
	int	i1;

	float u;
	int	iu;

	float	m0,	m1;
	float	su;

	int	state;

} cseg_type;



int	pt2dA_seg_r( pt2dA_type *apt, int cyclic, cseg_type ap[], int *nP );

void	cseg_print( cseg_type *as, int nA );



#endif