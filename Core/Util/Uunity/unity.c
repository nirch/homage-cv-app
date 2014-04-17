
#include	"unity.h"

void
unity_init( unity_type u[], int no )
{
int	i;
	for( i = 0 ; i < no ; i++ ){
		u[i].b = (u[i+1].h - u[i].h)/(u[i+1].t - u[i].t );
		u[i].a = u[i].h -  u[i].t*u[i].b;
	}
}




float
unity_weight( float t, unity_type u[], int no )
{
int	i;
	if( t < u[0].t )	return( u[0].h );

	for( i = 0 ; i < no ; i++ )
		if( t <= u[i+1].t )
			return( u[i].a+ t*u[i].b );

	return( u[no].h );
}
