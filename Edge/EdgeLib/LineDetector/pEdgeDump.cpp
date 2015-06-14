/***************************
***   ArDetectGrad.cpp   ***
****************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>

#ifdef _DEBUG
//#define _DUMP
#endif

#include	"Uigp/igp.h"
#include	"Ulog/Log.h"

#include	"Ucamera/Pt2dType.h"



#include	"pEdgeType.h"



void
pEdge_dump( image_type *dim, char *prefix, int index, char *suffix )
{
	pt2dA_type *apt;

	apt = pEdge_to_pt2dA( dim );

	pt2dA_dump( apt, prefix, index, suffix );

	pt2dA_destroy( apt );
}

void
pEdge_dump_T( image_type *dim, float x, float y, char *prefix, int index, char *suffix )
{
	pt2dA_type *apt;

	apt = pEdge_to_pt2dA( dim );

	pt2dA_translate( apt, x, y );

	pt2dA_dump( apt, prefix, index, suffix );

	pt2dA_destroy( apt );
}


void
pEdge_write_to_file( image_type *dim, char *fname )
{
pt2dA_type *apt;
char	file[256];

	apt = pEdge_to_pt2dA( dim );

	gpFilename_force_extension( fname, ".pt", file );
	pt2dA_write_to_file( apt, file );

	pt2dA_destroy( apt );
}



pt2dA_type *
pEdge_to_pt2dA( image_type *dim )
{
	pEdge_type	*dp;
	int	i,	j;
	pt2dA_type	*apt;


	apt = pt2dA_alloc( 100000);
	apt->type = PT2D_4V;
	apt->nP = 0;



	dp = (pEdge_type *)dim->data;
	for( i = 0 ; i < dim->height ; i++ ){
		for( j = 0 ; j < dim->width ; j++, dp++ ){
			if( dp->state == 0 )	continue;

			pt2d_type	*pt = &apt->p[apt->nP++];

			pt->p = dp->p;
			pt->n = dp->v;

			pt->group = ( dp->nStatic > 10 )? 2 : 0;
		}
	}


	return( apt );
}



//void
//pEdge_dump( image_type *dim, char *prefix, int index, char *suffix )
//{
//pEdge_type	*dp;
//int	i,	j;
//pt2dA_type	*apt;
//
//
//	apt = pt2dA_alloc( 100000);
//	apt->type = PT2D_4V;
//	apt->nP = 0;
//
//
//
//	dp = (pEdge_type *)dim->data;
//	for( i = 0 ; i < dim->height ; i++ ){
//		for( j = 0 ; j < dim->width ; j++, dp++ ){
//			if( dp->state == 0 )	continue;
//
//			pt2d_type	*pt = &apt->p[apt->nP++];
//	
//			pt->p = dp->p;
//			pt->n = dp->v;
//		}
//	}
//
//	pt2dA_dump( apt, prefix, index, suffix );
//
//	pt2dA_destroy( apt );
//}
