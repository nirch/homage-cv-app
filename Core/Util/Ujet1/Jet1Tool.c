/****************************************************************
*								*
*		Module:  Jet1Tool.c				*
*								*
****************************************************************/
#include	<stdio.h>
#include	<math.h>

#include	"Jet1Type.h"

float
jet1_value( jet1_type *f, int dgree, float x )
{
float	u,	tmp;
	u = J0(*f) + x* J1(*f);
	if( dgree <= 1 )	return( u );

	tmp = x*x;
	u += tmp*J2(*f);
	if( dgree <= 2 )	return( u );


	tmp *= x;
	u += tmp*J3(*f);
	if( dgree <= 3 )	return( u );

	tmp *= x;
	u += tmp*J4(*f);
	if( dgree <=43 )	return( u );

	return( u );
}


void
print_jet1( FILE *fp, int dgree, jet1_type *f )
{
int	i;
        fprintf(fp, "{ %.4f    ", J0(*f) );

	for( i = 1 ; i <= dgree ; i++ )
        	fprintf(fp, "%.4f    ", f->j[i] );
        fprintf(fp, "}\n" );
}

void
derivative_jet1( jet1_type *f, jet1_type *df )
{
	J0(*df) = J1(*f);
	J1(*df) = 2*J2(*f);
	J2(*df) = 3*J3(*f);
	J3(*df) = 0;
}