/**********************
 ***	GpColor.h   ***
 **********************/

#ifndef _GpColor_h
#define _GpColor_h

#ifndef BLACK
#define		BLACK	0x000000
#endif

#ifndef WHITE
#define		WHITE	0xffffff
#endif

#ifndef RED
#define		RED		0x0000ff
#endif

#ifndef GREEN
#define		GREEN	0x00ff00
#endif

#ifndef BLUE
#define		BLUE	0xff0000
#endif

#ifndef CYAN
#define		CYAN	0xffff00
#endif

#ifndef MAGENTA
#define		MAGENTA	0xff00ff
#endif

#ifndef YELLOW
#define		YELLOW	0x00ffff
#endif

#ifndef GRAY
#define		GRAY	0x00a6a6a6
#endif

#define		RED1		0x0000c8
#define		GREEN1		0x00c800
#define		BLUE1		0xc80000

#define		CYAN1		0xc8c800
#define		MAGENTA1	0xc800c8
#define		YELLOW1	0x00c8c8



#define		WBG		0xb5998b
#define		WFG		0xdbc0b2
#define		WBORDER		0xc8ac9e



#define		COLOR_SWAP_RB( color )	( (color & 0xff00) | ( (color>>16) &0xff ) | ( (color<<16) &0xff0000 ) )

#endif
