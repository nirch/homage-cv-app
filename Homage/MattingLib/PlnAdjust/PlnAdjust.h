
#ifndef _PLN_ADJUST_H_
#define _PLN_ADJUST_H_


#include "Uvec/Vec3d.h"


#include "Uln/Cln/ClnType.h"

#include "ImageType/ImageType.h"


	// PlnAdjustStart.cpp
int	plnA_adjust_start( plnA_type *apl, float height );

	// PlnAdjustThin.cpp
int	plnA_adjust_thin( plnA_type *apl, plnA_type *bapl, float ht[2], int iFrame );





	// PlnAdjustIn.cpp
int	plnA_adjust_in( plnA_type *apl, plnA_type *bapl, plnA_type *eapl, float dT, int iFrame );

int	plnA_adjust_in_head( plnA_type *apl, vec2f_type *hp, float r, int iFrame );

	// PlnAdjustCoherent.cpp
int	pln_coherent( plnA_type *apl, plnA_type *bpl, int height, int iFrame);


	// PlnAdjustEdge.cpp
int	plnA_adjust_edge( plnA_type *apl, plnA_type *eapl, float height, float ht[2], int iFrame );

	// PlnAdjustBottom.cpp
int	plnA_adjust_bottom( plnA_type *apl, plnA_type *eapl, float height, int iFrame );


	// PlnIntersect.cpp
int	pln_adjust_intersect( pln_type *pl );

int	plnA_adjust_intersect( plnA_type *apl );



#endif


