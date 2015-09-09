/************************************************************************
*									*
*		mudole: s2_unity.c					*
*									*
*	u(x) = Integarl( (x+1)^2 * (x-1)^2 ) =				*
*			Integarl( x^4 - 2x^2 +1 ) =			*
*		-15/4*( 1/5*x^5 - 2/3x^3 + x ) + 0.5			*
*									*
************************************************************************/
#include	"Uigp/igp.h"
#include	"Ujet1/Jet1Type.h"
#include	"Ujet2/Jet2Type.h"

#include	"unity.h"

static float	Ph;
static float	Pe;
static float	Pa;
#define		PB	-15.0/16


void	unity2_jet1( float x, jet1_type *v );

void
unity2_set_paramter( float h, float e )
{
	Ph = h;
	Pe = e;
	Pa = 1/Pe ;
}

float
unity2_value( float x )
{
float	val;
float	x2,	x4;

	if( x < -Ph -Pe )	return( 0.0 );

	if( x < -Ph +Pe ){
		x = Pa*( x+Ph);
		x2 = x*x;
		x4 = x2*x2;
		val =  1- ( PB*x*( 1.0/5 * x4 - 2.0/3* x2 +1 ) +0.5);
		return( val );
	}

	if( x < Ph - Pe )	return( 1.0 );

	if( x < Ph +Pe ){
		x = Pa*( x-Ph);
		x2 = x*x;
		x4 = x2*x2;
		val =   PB*x*( 1.0/5 * x4 - 2.0/3* x2 +1 ) +0.5;
		return( val );
	}

	return( 0.0 );
}

void
unity2_jet1( float x, jet1_type *v )
{
float	x2,	x4;

	if( x <= -Ph -Pe || x >= Ph+Pe ){
		J0(*v) = J1(*v) = J2(*v) = J3(*v) = 0;
		return;
	}

	if( x < -Ph +Pe ){
		x = Pa*( x+Ph);
		x2 = x*x;
		x4 = x2*x2;
		J0(*v) =  1- ( PB*x*( 1.0/5 * x4 - 2.0/3* x2 +1 ) +0.5 );
		J1(*v) =  -Pa*PB*( x4 - 2.0 * x2 + 1 );
		J2(*v) =  -Pa*Pa*PB*4/2*x*( x2 - 1 );
		return;
	}

	if( x < Ph - Pe ){
		J0(*v) = 1;
		J1(*v) = J2(*v) = J3(*v) = 0;
		return;
	}

	if( x < Ph +Pe ){
		x = Pa*( x-Ph);
		x2 = x*x;
		x4 = x2*x2;
		J0(*v) =   PB*x*( 1.0/5 * x4 - 2.0/3* x2 +1 ) +0.5;
		J1(*v) =  Pa*PB*( x4 - 2.0 * x2 + 1 );
		J2(*v) =  Pa*Pa*PB*4/2*x*( x2 - 1 );
		return;
	}

}



int
unity2_jet2( jet2 *u, float x, float y )
{
jet1_type	t,	v;

	unity2_jet1( x, &v );
	if( J0(v) == 0 ){
		*u = ZERO_JET2;
		return(0);
	}

	unity2_jet1( y, &t );
	if( J0(t) == 0 ){
		
		*u = ZERO_JET2;
		return(0);
	}

	J00(*u) = J0(v)*J0(t);

	J10(*u) = J1(v)*J0(t);
	J01(*u) = J0(v)*J1(t);

	J20(*u) = J2(v)*J0(t);
	J11(*u) = J1(v)*J1(t);
	J02(*u) = J0(v)*J2(t);

	return( 1 );
}
