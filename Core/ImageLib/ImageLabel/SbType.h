
#ifndef _SB_TYPE_H_
#define _SB_TYPE_H_



#include "ImageType/ImageType.h"



typedef struct sb_type {
	int	j0;
	int j1;

	int	r, g, b,	y;
	int	ys;

	int	n;
	int	id;
} sb_type;


typedef struct sbA_type {
	int NA;
	int	nA;


	sb_type a[256];
} sbA_type;



int	sbA_write( sbA_type *as, FILE *fp );

int	sbA_write3( sbA_type *as, sbA_type *as0, sbA_type *as1, FILE *fp );

int	sbA_in( sbA_type *as, int j0 );

int	sbA_overlap( sbA_type *as, sb_type *s, int a[], int *nA );

int	seg_overlap( sb_type *s0, sb_type *s );

int	sbA_range( sbA_type *as, int *i0, int *i1 );

int	sbA_value( sbA_type *as, int *i0, int *i1, int *y, int *ys );


#endif


