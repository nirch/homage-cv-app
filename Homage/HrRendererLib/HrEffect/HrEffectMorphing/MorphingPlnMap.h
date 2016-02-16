
#ifndef _MORPHING_PLN_MAP_H
#define _MORPHING_PLN_MAP_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "../HrEffectI.h"



pt2dA_type *	morohing_pln_map( plnA_type *apl, plnA_type *aplt, int width, int height, int dxy, float t, pt2dA_type *apt );

pt2dA_type *	morohing_pln_map_step( pt2dA_type *apt, vec2f_type *p0, float T, pt2dA_type *capt );


#endif


