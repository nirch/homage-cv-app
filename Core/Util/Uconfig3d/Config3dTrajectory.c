#include	<math.h>

#include	"Uigp/igp.h"
#include	"Config3d.h"


int
config3d_create_trajectory( config3d cm[], int cm_no, int frame, config3d tm[] )
{
config3d	pm[100],	dm[100],	*d;
float	dis[100],	t,	dt,	tmp;
int	i,	j;

	if( cm_no == 0 )	return( 0 );

	if( cm_no == 1 ){
		for( i = 0 ; i < frame ; i++ )
			tm[i] = cm[0];

		return( frame );
	}
		

	for( i = 0 ; i < cm_no ; i++ ){
		pm[i] = cm[i];
		pm[i].u1 *= M_PI/180.0;
		pm[i].u2 *= M_PI/180.0;
		pm[i].u3 *= M_PI/180.0;
	}


	dis[0] = 0;
	for( i = 1 ; i < cm_no ; i++ ){
		d = &dm[i-1];
		d->x = pm[i].x - pm[i-1].x;
		d->y = pm[i].y - pm[i-1].y;
		d->z = pm[i].z - pm[i-1].z;
		d->u1 = pm[i].u1 - pm[i-1].u1;
		d->u2 = pm[i].u2 - pm[i-1].u2;
		d->u3 = pm[i].u3 - pm[i-1].u3;

		tmp = d->x*d->x + d->y*d->y + d->z*d->z +
			d->u1*d->u1 + d->u2*d->u2 + d->u3*d->u3;

		if( tmp > 0 ){
			tmp = sqrt( tmp );

			d->x /= tmp;
			d->y /= tmp;
			d->z /= tmp;
			d->u1 /= tmp;
			d->u2 /= tmp;
			d->u3 /= tmp;
		}
		dis[i] = dis[i-1] + tmp;
	}

	dt = dis[cm_no-1]/(frame-1);


	tm[0] = pm[0];
	tm[0].u1 *= 180.0/M_PI;
	tm[0].u2 *= 180.0/M_PI;
	tm[0].u3 *= 180.0/M_PI;

	for( i = 1, j = 0, t = dt ; i < frame-1 ; i++, t += dt ){

		while( dis[j+1] < t )	j++;

		tmp = t - dis[j];

		tm[i].x = pm[j].x + tmp*dm[j].x;
		tm[i].y = pm[j].y + tmp*dm[j].y;
		tm[i].z = pm[j].z + tmp*dm[j].z;
		tm[i].u1 = pm[j].u1 + tmp*dm[j].u1;
		tm[i].u2 = pm[j].u2 + tmp*dm[j].u2;
		tm[i].u3 = pm[j].u3 + tmp*dm[j].u3;

		tm[i].u1 *= 180.0/M_PI;
		tm[i].u2 *= 180.0/M_PI;
		tm[i].u3 *= 180.0/M_PI;
	}

	i = frame-1;
	tm[i] = pm[cm_no-1];
	tm[i].u1 *= 180.0/M_PI;
	tm[i].u2 *= 180.0/M_PI;
	tm[i].u3 *= 180.0/M_PI;

	return( frame );
}
