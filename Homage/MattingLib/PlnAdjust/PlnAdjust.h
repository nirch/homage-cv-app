
#ifndef _PLN_ADJUST_H_
#define _PLN_ADJUST_H_


#include "Uvec/Vec3d.h"
#include "Utime/GpTime.h"

#include "Uln/Cln/ClnType.h"

#include "ImageType/ImageType.h"


	// PlnAdjustStart.cpp
int	plnA_adjust_start( plnA_type *apl );

	// PlnAdjustThin.cpp
int	plnA_adjust_thin( plnA_type *apl, int iFrame );


	// PlnAdjustIn.cpp
int	plnA_adjust_in( plnA_type *apl, plnA_type *bapl, int iFrame );


#endif


