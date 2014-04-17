/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/******************
 ***  PtLine.h  ***
 ******************/
#ifndef _PT_LINE_
#define _PT_LINE_

#ifdef __cplusplus
extern "C" {
#endif

#include "Ucamera/Pt2dType.h"

#define PLN_OPEN	1
#define PLN_CLOSE	2



typedef struct pln_type {

	int	i[2];
	int	nP;		// point number;

	vec2f_type	ctr;
	ln_type *link;

	float	len;

	int	state;

} pln_type;





typedef struct plnA_type {
	int	NA;

	int	nA;
	pln_type	**a;

} plnA_type;




pln_type *pln_alloc( int no );

void	pln_destroy( pln_type *pl );

pln_type *	pln_copy( pln_type *spl );


int		pln_set( pln_type *pl, vec2f_type *ctr, ln_type *l );


void	pln_inverse( pln_type *pl );

int		pln_create( pln_type *pl, pt2dA_type *apt );

void	pln_trim( pln_type *pl, int direct, float gt );

void	pln_appendL( pln_type *pl, vec2f_type *ctr, ln_type *link );

void	pln_append( pln_type *pl, pln_type *pl1 );


void	pln_gt2p( pln_type *pl, float gt, vec2f_type *p );

void	pln_end_point( pln_type *pl, vec2d *p );


pln_type *	pln_from_point( vec2d p[], int no, int Fclose );

pln_type *	pln_from_lnA( ln_type al[], int nAl );

void	pln_scale( pln_type *pl, float scale );

void	plnA_clear( plnA_type *apl );




int	pln_distance( pln_type *pl, vec2f_type *p, dPln_type *d );

void	pln_box( pln_type *pl, box2f_type *box );


plnA_type *		plnA_alloc( int n );

void	plnA_destroy( plnA_type *apl );

int		plnA_decrease( plnA_type *apl );

int		plnA_select( plnA_type *apl, vec2f_type *v, float dis );

int		plnA_length( plnA_type *apl );

int		plnA_distance( plnA_type *apl, vec2f_type *p, float D, pln_type **spl, dPln_type *sd );

void	pln_sample( pln_type *pl, float t0, float r, int n, int direct, pt2dA_type *apt );



	// PlnWrite.c
//int		pln_write( pln_type *aPl[], int nPl, char *file );
//void	pln_dump( pln_type *aPl[], int nPl, char *prefix, int index, char *suffix );


int		plnA_write( plnA_type *apl, char *file );

void	plnA_dump( plnA_type *apl, char *prefix, int index, char *suffix );

int	plnA_read( char *file, plnA_type **apl );



#ifdef _DUMP
#define PLNA_DUMP( apl, name, index, ext )  plnA_dump( apl, name, index, ext )

#else
#define PLNA_DUMP( apl, name, index, ext )

#endif



#ifdef __cplusplus
}
#endif

#endif