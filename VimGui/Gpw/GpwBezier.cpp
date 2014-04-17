
#include        <math.h>

#include	"GpwType.h"






void gpw_bezier( gpw_type *gpw, vec2d* p0, vec2d* p1, vec2d* p2, vec2d* p3, int color )
{
	float t1,t=0.0;
	float res = 1.0/100.0;
	vec2d	v;
	
	


	for(t=0;t<=1.0;t+=res)
	{
		t1=1.0-t;
		if(t1<0.01)
		{
			t1=0;
			t=1.0;
		}
		v.x = t1*t1*t1*p0->x + 3.0*t1*t1 * t * p1->x + 3.0*t1*t*t*p2->x + t*t*t*p3->x;
		v.y = t1*t1*t1*p0->y + 3.0*t1*t1 * t * p1->y + 3.0*t1*t*t*p2->y + t*t*t*p3->y;		
		gpw_point( gpw, v.x, v.y, color );
	}
}