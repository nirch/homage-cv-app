
/****************************************************************
*								*
*		Module:  Jet2Io.c				*
*								*
****************************************************************/
#include	<stdio.h>
#include	<math.h>

#include	"Uigp/igp.h"

#include	"Jet2Type.h"

void
print_jet2( FILE *fp, int dgree, jet2 *f )
{
	fprintf(fp, "{ %.4f }\n", J00(*f) );
	fprintf(fp, "{ %.4f    %.4f }\n", J10(*f), J01(*f) );
	fprintf(fp, "{ %.4f    %.4f    %.4f }\n", J20(*f), J11(*f), J02(*f) );

	if( dgree == 2 )	return;

	fprintf(fp, "{ %.4f    %.4f    %.4f    %.4f }\n",
			 J30(*f), J21(*f), J12(*f), J03(*f) );
}


void
load_jet2( FILE *fp, int dgree, jet2 *f )
{
	fscanf(fp, "%f", &J00(*f) );
	fscanf(fp, "%f %f", &J10(*f), &J01(*f) );
	fscanf(fp, "%f %f %f", &J20(*f), &J11(*f), &J02(*f) );

	if( dgree == 2 )	return;

	fscanf(fp, "%f %f %f %f",
		&J30(*f), &J21(*f), &J12(*f), &J03(*f) );
}

