//  Defines the entry point for the console application.
//
#include <stdio.h>
#include	<string.h>
#include	<math.h>


#ifdef _DEBUG
#define _DUMP
#endif

#include "Ulog/Log.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"
#include "ImageLabel/ImageLabel.h"


#include "SbType.h"


sbA_type *
sbA_alloc()
{
	sbA_type *as = ( sbA_type *)malloc( sizeof(sbA_type) );
	return( as );
}


int
sbA_write( sbA_type *as, FILE *fp )
{
int	i;
	for( i = 0 ; i < as->nA ; i++ ){
		if( as->a[i].j0 < 0 )	continue;

		fprintf( fp, "%d   %d %d\n", i, as->a[i].j0, as->a[i].j1 );
	}


	return( 1 );
}


int
sbA_write3( sbA_type *as, sbA_type *as0, sbA_type *as1, FILE *fp )
{
	int	i;
	for( i = 0 ; i < as->nA ; i++ ){
		//if( as->a[i].j0 < 0 &&  as1->a[i].j0 )	continue;

		fprintf( fp, "%d   %3d %3d (%d)    %3d %3d    %3d %3d ( %d)\n", i,
				as->a[i].j0, as->a[i].j1, as->a[i].y,
				as0->a[i].j0, as0->a[i].j1, 
				as1->a[i].j0, as1->a[i].j1, as1->a[i].y );
	}


	return( 1 );
}



int
sbA_range( sbA_type *as, int *i0, int *i1 )
{
	int	i,	n;

	*i0 = *i1 = -1;
	n = 0;
	for( i = 0 ; i < as->nA ; i++ ){
		if( as->a[i].j0 < 0 )	continue;

		if( n == 0 ){
			n = 1;
			*i0 = *i1 = i;
			continue;
		}
		n++;
		*i1 = i;
	}

	return( n );
}

int
sbA_value( sbA_type *as, int *i0, int *i1, int *val, int *ys )
{
	int	i,	n;

	*i0 = *i1 = -1;
	*val = *ys = 0;
	n = 0;
	for( i = 0 ; i < as->nA ; i++ ){
		if( as->a[i].j0 < 0 )	continue;

		if( n == 0 ){
			n = 1;
			*i0 = *i1 = i;
			continue;
		}

		*val += as->a[i].y;
		*ys += as->a[i].ys;
		n++;
		*i1 = i;
	}

	if( n > 0 ){
		*val /= n;
		*ys /= n;
	}

	return( n );
}

int
sbA_in( sbA_type *as, int j0 )
{
int i;

	for( i = 0 ; i < as->nA ; i++ ){
		if( as->a[i].j0 <= j0  && as->a[i].j1 >= j0 )
			return( i );
	}

	return( -1 );
}


int
sbA_overlap( sbA_type *as, sb_type *s, int a[], int *nA )
{
	int i;

	*nA = 0;
	for( i = 0 ; i < as->nA ; i++ ){
		if( s->j0 > as->a[i].j1 )	continue;
		if( s->j1 < as->a[i].j0 )	continue;
		
		a[(*nA)++] = i;
	}

	return( -1 );
}


int
seg_overlap( sb_type *s0, sb_type *s )
{

	
	if( s->j0 > s0->j1 )	return( -1 );
	if( s->j1 < s0->j0 )	return( -1 );


	return( 1 );
}
