/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef         _HISTOGRAM_
#define         _HISTOGRAM_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"


	// Histogram.c
int	histogram_range( int h[], int nH, float p0, float p1, float *r0, float *r1 );

void	histogram_write( int h[], int nH, char *file );


float	histogram_median( int h[], int nH );


	// HistogramOrder.c
int		histogram_order( int H[], int nH, int a[] );




	// HistogramClustrringIsodata.c

typedef struct isdata1_type {
	int	i0,	i1;

	int	n;

	float	mean;
	float	var;	

	float	d;
}isdata1_type;


void	histogram_clustring_isodataA( int H[], int nH, int K, int nT, float vT, float mT, int Itration, isdata1_type c[], int *nM );

void	histogram_clustring_isodata( int H[], int nH, int K, int nT, float vT, float mT, int Itration, float am[], int *nM );


#ifdef __cplusplus
}
#endif

#endif
