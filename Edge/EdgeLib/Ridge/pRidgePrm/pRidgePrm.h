/**********************
 ***  pRidgePrm.h  ***
 *********************/


#ifndef _PRIDGE_PRM_H_
#define _PRIDGE_PRM_H_


#include "ImageType/ImageType.h"


#include "Ubox/Box2d.h"

#include "Uprm/prm.h"


typedef struct pRidgeDetectPrm_type {
	int	weight;
	int	size;

	int	weight2;

	int	sign;

	float	lm_min;
	float	lmR;

	float	saddleR;

} pRidgeDetectPrm_type;


typedef struct pRidgeLinePrm_type {
	int		color;
	
	int		weight;

	float	lm_min;

	float	lenMin;
	float	lenMax;

	float	WeekR;

} pRidgeLinePrm_type;



typedef struct pRidgePrm_type {

	pRidgeDetectPrm_type	detect;

	pRidgeLinePrm_type		line;

} pRidgePrm_type;



	// pRidgePrm.cpp
pRidgePrm_type *	pRidgePrm_alloc();

void	pRidgePrm_destroy( pRidgePrm_type *rPrm );

void	pRidgePrm_update( pRidgePrm_type *prm );

void	pRidgePrm_set( pRidgePrm_type *prm );


int	pRidgePrm_read( pRidgePrm_type *prm, char *file );

int	pRidgePrm_read( struct xmlTag_type *pTag, pRidgePrm_type *prm );


int	pRidgePrm_write( pRidgePrm_type *prm, char *file );

int	pRidgePrm_write( pRidgePrm_type *prm, int align, FILE *fp );



	// pRidgeDetectPrm.cpp
pRidgeDetectPrm_type *	pRidgeDetectPrm_alloc();

void	pRidgeDetectPrm_destroy( pRidgeDetectPrm_type *prm );

void	pRidgeDetectPrm_get_lprm( GP_prm ** prm, int *prm_no, void (**check) ());

void	pRidgeDetectPrm_update( pRidgeDetectPrm_type *prm );

void	pRidgeDetectPrm_set( pRidgeDetectPrm_type *prm );

int		pRidgeDetectPrm_read( pRidgeDetectPrm_type *prm, char *file );

int		pRidgeDetectPrm_read( struct xmlTag_type *pTag, pRidgeDetectPrm_type *prm );



int	pRidgeDetectPrm_write( pRidgeDetectPrm_type *prm, char *file );

int	pRidgeDetectPrm_write( pRidgeDetectPrm_type *prm, int align, FILE *fp );





	// pRidgeLinePrm.cpp
pRidgeLinePrm_type *	dtRidgeLinePrm_alloc();

void	pRidgeLinePrm_get_lprm( GP_prm ** prm, int *prm_no, void (**check) ());

void	pRidgeLinePrm_update( pRidgeLinePrm_type *prm );
void	pRidgeLinePrm_set( pRidgeLinePrm_type *prm );



int	pRidgeLinePrm_read( pRidgeLinePrm_type *vsPrm, char *file );

int	pRidgeLinePrm_read( struct xmlTag_type *pTag, pRidgeLinePrm_type *vsPrm );

int	pRidgeLinePrm_write( pRidgeLinePrm_type *vsPrm, char *file );
int	pRidgeLinePrm_write( pRidgeLinePrm_type *vsPrm, int algin, FILE *fp );



#endif