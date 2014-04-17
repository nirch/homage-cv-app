/**********************
 ***	GappDump.c	***
 **********************/
#include	<stdio.h>
#include	<stdlib.h>


#include	"Uigp/igp.h"

#include	"GeoApp.h"

#include "Ucamera/Pt2dType.h"


void
gapp_dump( gapp_type *gapp,  char *prefix, int index, char *suffix )
{
	pt2dA_type	*apt;
	int	i;

	apt = pt2dA_alloc( gapp->no );

	apt->type = PT2D_4V;
	apt->axis = PT2D_AXIS_YX;

	for( i = 0; i < gapp->no ; i++ ){
		apt->p[i].p = gapp->v[i];
		apt->p[i].p.x = gapp->v[i].x;
		apt->p[i].p.y = gapp->v[i].y;

		apt->p[i].n = gapp->tang[i];

		apt->p[i].r = 0.5;
	}

	apt->nP = gapp->no;

	pt2dA_dump( apt, prefix, index, suffix );

	pt2dA_destroy( apt );
}
