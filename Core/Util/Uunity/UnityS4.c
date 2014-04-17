/************************************************************************
*									*
*		mudole: s3_unity.c					*
*									*
*	u(x) = Integarl( (x+1)^3 * (x-1)^3 ) =				*
*			Integarl( x^6 - 3x^4 + 3x^2 -1 ) =		*
*		-35/4*( 1/7*x^7 - 3/5x^5 + x^3 -x ) + 0.5		*
*									*
************************************************************************/
#include	"Ujet1/Jet1Type.h"
#include	"Ujet2/Jet23Type.h"
#include	"Uigp/igp.h"

#include	"unity.h"

static float	Ph;
static float	Pe;
static float	Pa;
#define		PB	315.0/256.0

void
unity4_set_paramter( float h, float e )
{
	Ph = h;
	Pe = e;
	Pa = 1/Pe ;
}

float
unity4_value( float x )
{
float	val;
float	x2,	x4,	x6,	x8;

	if( x < -Ph -Pe )	return( 0.0 );

	if( x < -Ph +Pe ){
		x = Pa*( x+Ph);
		x2 = x*x;
		x4 = x2*x2;
		x6 = x4*x2;
		x8 = x4*x4;
		val =  1- ( PB*x*( 1.0/9*x8 -4.0/7*x6 +
					6.0/5 * x4 - 4.0/3*x2 + 1 ) +0.5);
		return( val );
	}

	if( x < Ph - Pe )	return( 1.0 );

	if( x < Ph +Pe ){
		x = Pa*( x-Ph);
		x2 = x*x;
		x4 = x2*x2;
		x6 = x4*x2;
		x8 = x4*x4;
		val =  PB*x*( 1.0/9*x8 -4.0/7*x6 +
					6.0/5 * x4 - 4.0/3*x2 + 1 ) +0.5;
		return( val );
	}

	return( 0.0 );
}

void
unity4_jet1( float x, jet1_type *v )
{
float	x2,	x4,	x6,	x8;

	if( x <= -Ph -Pe || x >= Ph+Pe ){
		J0(*v) = J1(*v) = J2(*v) = J3(*v) = 0;
		return;
	}

	if( x < -Ph +Pe ){
		x = Pa*( x+Ph);
		x2 = x*x;
		x4 = x2*x2;
		x6 = x4*x2;
		x8 = x4*x4;
		J0(*v) =  1- (PB*x*( 1.0/9*x8 -4.0/7*x6 +
				6.0/5 * x4 - 4.0/3*x2 + 1 ) +0.5);
		J1(*v) =  -Pa*PB*( x8 - 4*x6 + 6.0 * x4 - 4*x2 +1 );

		J2(*v) =  -Pa*Pa*PB*8.0/2.0*x*( x6 - 3*x4 + 3*x2 - 1 );
		J3(*v) =  -Pa*Pa*Pa*PB*8.0/6.0*( 7*x6 - 15*x4 + 9*x2 -1 );
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
		x6 = x4*x2;
		x8 = x4*x4;
		J0(*v) =  PB*x*( 1.0/9*x8 -4.0/7*x6 +
					6.0/5 * x4 - 4.0/3*x2 +1 ) +0.5;

		J1(*v) =  Pa*PB*( x8 - 4*x6 + 6.0 * x4 - 4*x2 +1 );

		J2(*v) =  Pa*Pa*PB*8.0/2*x*( x6 - 3*x4 + 3*x2 - 1 );

		J3(*v) =  Pa*Pa*Pa*PB*8.0/6.0*( 7*x6 - 15*x4 + 9*x2 -1 );
		return;
	}

}



int
unity4_jet23( jet23 *u, float x, float y )
{
jet1_type	t,	v;

	unity4_jet1( x, &v );
	if( J0(v) == 0 ){
		zero_jet23(u );
		return(0);
	}

	unity4_jet1( y, &t );
	if( J0(t) == 0 ){
		
		zero_jet23(u );
		return(0);
	}

	J00(*u) = J0(v)*J0(t);

	J10(*u) = J1(v)*J0(t);
	J01(*u) = J0(v)*J1(t);

	J20(*u) = J2(v)*J0(t);
	J11(*u) = J1(v)*J1(t);
	J02(*u) = J0(v)*J2(t);

	J30(*u) = J3(v)*J0(t);
	J21(*u) = J2(v)*J1(t);
	J12(*u) = J1(v)*J2(t);
	J03(*u) = J0(v)*J3(t);

	return( 1 );
}
