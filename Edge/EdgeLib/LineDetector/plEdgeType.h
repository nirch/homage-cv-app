/***********************
 ***  PlEdgeType.h  ***
 ***********************/


#ifndef _PL_EDGE_H_
#define _PL_EDGE_H_

#include "Uln/PlnType.h"



typedef struct plEdge_type {
	int	state;
	int	id;


	pln_type	*pl;



	float	grad;	// averege gradinat
	float	f[2];	// averege value 
	

	struct pEdge_type	*rp;

} plEdge_type;





typedef struct plEdgeA_type {
	int	NA;
	int	nA;

	plEdge_type	**a;

} plEdgeA_type;




	// plRidge.cpp
void plEdge_draw( struct gpw_type *gpw, plEdge_type *c );

plEdge_type * plEdge_alloc();

void plEdge_destroy( plEdge_type *c );

void plEdge_translate( plEdge_type *c, vec2f_type *ctr );



plEdgeA_type *plEdgeA_alloc( int n );

plEdgeA_type *plEdgeA_realloc(plEdgeA_type *ar, int n );

void plEdgeA_destroy(plEdgeA_type *ar );

void plEdgeA_clear( plEdgeA_type *ar );

void	plEdgeA_translate( plEdgeA_type *ae, vec2f_type *ctr );\

void	plEdgeA_scale( plEdgeA_type *ae, float scale );

plnA_type *	plEdgeA_to_plnA( plEdgeA_type *ae, float minLen, int fCopy );

void	plEdgeA_write( plEdgeA_type *ae, char *outFile );

void	plEdgeA_dump( plEdgeA_type *ae, char *prefix, int index, char *suffix );



int	plEdgeA_select(plEdgeA_type *ar, vec2f_type *p, float D );



#ifdef _DUMP
#define PLEDGE_DUMP( ae, prefix, index,  suffix )  plEdgeA_dump( ae, prefix, index,  suffix )
#else
#define PLEDGE_DUMP( ae, prefix, index,  suffix )
#endif

#endif