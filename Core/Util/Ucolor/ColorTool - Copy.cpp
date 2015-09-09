
#include	<stdio.h>
#include	<math.h>
#include	<string.h>
#include	<stdlib.h>


#ifdef _DEBUG
#define _DUMP
#endif

#include "ColorType.h"




int
color_test( int a, int r, int g, int b )
{
color_type	c;

//	c.color = ( a<<24) | COLOR_RGB( r, g, b);
	c.color = COLOR_RGBA( a, r, g, b);
	

	fprintf( stdout, "%d %d %d %d\n", (int)c.alpha, (int)c.Red, (int)c.Green, (int)c.Blue  );


	c.color = ( c.alpha<<24) | COLOR_RGB( c.Red, c.Green, c.Blue);

	return( 1 );
}