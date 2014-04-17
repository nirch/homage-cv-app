#ifndef		_LS_APPR
#define		_LS_APPR

#include 	"Uvec/Vec2d.h"

float	get_parab_coef( vec2d v[],float tg[], float len, int no, float dw );

float	get_l2_dist( vec2d v[], float tg[], float a, float len,
						int no, float dw );

float	get_max_dist( vec2d *v, float *tg, float a, float len, int no );

#endif

