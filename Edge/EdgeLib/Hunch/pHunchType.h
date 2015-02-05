/**********************
 ***  pHunchType.h  ***
 **********************/


#ifndef _P_HUNCH_H_
#define _P_HUNCH_H_
#include "Uigp/igp.h"
#include "Uvec/Vec2d.h"
#include "Uprm/prm.h"


#include "ImageType/ImageType.h"


typedef struct pHunchPrm_type {
	int	weight;

	int	size;

	int sign;

	float lm1Min;

	float lmMin;

	float lmR;

	//	float	r;

	float	dx;
	float	dy;

	// Aux
	float	lm1_0;
	float	lm2_0;

} pHunchPrm_type;



typedef struct pHunch_type {

	int	state;

	int	size;

	vec2d	p;
	vec2d	v;


	float	f;

	float	lm1,	lm2;

} pHunch_type;



typedef struct pHunchA_type {

	int	NA;
	int	nA;
	
	pHunch_type	**a;

} pHunchA_type;



pHunch_type *	pHunch_alloc();

void	pHunch_destroy( pHunch_type *h );

pHunchA_type *	pHunchA_alloc( int nH );

void	pHunchA_destroy( pHunchA_type *ah );

void	pHunchA_clear( pHunchA_type *ah );


int	pHunchA_translate( pHunchA_type *ah, float x, float y );

int	pHunchA_swap( pHunchA_type *ah );



void	pHunch_detect( image_type *im, image_type *maskIm, box2i *b, struct pHunchPrm_type *prm, pHunchA_type *ah );

void	pHunch_detectN( image_type *im, image_type *maskIm, box2i *b, pHunchPrm_type *prm, pHunchA_type *ah );

void	pHunch_detect( image_type *im, box2i *b, pHunchPrm_type *prm, pHunchA_type *ah );

void	pHunch_detect( image_type *im, box2i *b, struct jet2App_type *ja[], pHunchPrm_type *prm, pHunchA_type *ah );


void	pHunchA_filter( pHunchA_type *ah, float T );


int	pHunchA_select( pHunchA_type *ah, vec2f_type *p, float D );













// DtHunchPrm.cpp
void	pHunchPrm_get_lprm( GP_prm ** prm, int *prm_no, void (**check) ());

void	pHunchPrm_update( pHunchPrm_type *vPrm );

void	pHunchPrm_set( pHunchPrm_type *vPrm );

pHunchPrm_type *	pHunchPrm_alloc();


int	pHunchPrm_read( pHunchPrm_type *vsPrm, char *file );

int	pHunchPrm_read( struct xmlTag_type *pTag, pHunchPrm_type *prm );


int	pHunchPrm_write( pHunchPrm_type *vsPrm, char *file );

int	pHunchPrm_write( pHunchPrm_type *prm, int	align, FILE *fp );


#endif