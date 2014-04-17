#include	<stdio.h>

#include	"Uigp/igp.h"
#include	"Config3d.h"



void
config3d_init( config3d *cp )
{
	cp->x = cp->y = cp->z = 0;
	cp->u1 = cp->u2 = cp->u3 = 0;
}
