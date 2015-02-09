/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/

#ifndef	_COLOR_TYPE_
#define	_COLOR_TYPE_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"



#ifdef WIN32
#define		COLOR_RED( pixel )		( ((pixel) >> 16) &0xff )
#define		COLOR_GREEN( pixel )	( ((pixel) >> 8 ) &0xff )
#define		COLOR_BLUE( pixel )	(  (pixel)&0xff )

#define		COLOR_RGB( r, g, b ) 	(((r)<<16) | ((g)<<8) | (b) )
#define		COLOR_RGBA( a, r, g, b ) 	( ((a)<<24) | ((r)<<16) | ((g)<<8) | (b) )
#endif
    
    
#ifdef __APPLE__
#define		COLOR_RED( pixel )		( ((pixel) >> 16) &0xff )
#define		COLOR_GREEN( pixel )	( ((pixel) >> 8 ) &0xff )
#define		COLOR_BLUE( pixel )	(  (pixel)&0xff )
    
#define		COLOR_RGB( r, g, b ) 	(((r)<<16) | ((g)<<8) | (b) )
#define		COLOR_RGBA( a, r, g, b ) 	( ((a)<<24) | ((r)<<16) | ((g)<<8) | (b) )
#endif


#ifdef __linux
#define		COLOR_ALPHA( pixel )	( ((pixel) >> 24) &0xff )
#define		COLOR_RED( pixel )		( ((pixel) >> 16) &0xff )
#define		COLOR_GREEN( pixel )	( ((pixel) >> 8 ) &0xff )
#define		COLOR_BLUE( pixel )	(  (pixel)&0xff )

#define		COLOR_RGB( r, g, b ) 	(((r)<<16) | ((g)<<8) | (b) )
#define		COLOR_RGBA( a, r, g, b ) 	( ((a)<<24) | ((r)<<16) | ((g)<<8) | (b) )
#endif



#define COLOR_RGB2Y_F( R, G, B )	(R*0.29900 + G *0.58700 + B *0.11400)
#define COLOR_RGB2Y( R, G, B )		((R*19595 + G *38469 + B *7471)>>16)

#define COLOR_Y( c )		(((c).r*19595 + (c).g *38469 + (c).b *7471)>>16)




typedef struct color_type
{
	union {

		struct {
			u_char	b;
			u_char	g;
			u_char	r;
			u_char	a;
		};


		struct {
			u_char	Blue;
			u_char	Green;
			u_char	Red;
			u_char	alpha;
		};

		int	color;			// ( c.alpha<<24) | COLOR_RGB( c.Red, c.Green, c.Blue);
		
		int	val;
	};


} color_type;





int		color_test( int a, int r, int g, int b );




#ifdef __cplusplus
}
#endif
#endif


