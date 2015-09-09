/*********************
 ***	hyputA.c   ***
 *********************/
#include	<math.h>

#include	"Uigp/igp.h"


#ifdef _DEBUG
//static float hypotA_error = 0;
#endif

#define VIM_CONST_EXAPND_TABLE

#ifdef VIM_CONST_EXAPND_TABLE

#include	"HypotA.tab"

#else
static int HypotA[257];


void
hypotA_table()
{

float	t;
int	i,	tmp;


	for( i =0, t = 0 ; i < 257 ; i++, t += 1.0/256 ){
		tmp = (sqrt( 1+t)-1) *256 + 0.5;
		HypotA[i] = tmp;
	}

//	gp_write_INTtable_to_file( "D:/Player/hypotA.tab", "HypotA", HypotA, 256 );
}
#endif

float
hypotA( float x, float y )
{
float	t,	t0;
int	i;


#ifndef VIM_CONST_EXAPND_TABLE
static int Ftab =0;
	if( Ftab == 0 ){

		hypotA_table();

		Ftab = 1;
	}
#endif


	if( x < 0 )	x = -x;
	if( y < 0 ) y = -y;

	if( x < y ){
		t =  x / y;
		t0 = y;
	}
	else	{
		t0 = x;
		t = y /x;
	}

	i = t*t*256 + 0.5;

	t  = 1.0 + HypotA[i] * (1.0/256);
	t *= t0;


#ifdef _DEBUG
//	t0 = hypot( x, y );
//	if( (t0 = t0 - t) < 0 ) t0 = -t0;
//	if( t0 > hypotA_error ) hypotA_error = t0;
#endif


	return( t );
}
