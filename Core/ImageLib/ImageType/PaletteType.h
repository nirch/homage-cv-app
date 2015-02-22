/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/

#ifndef	_COLOR_PALETTE_
#define	_COLOR_PALETTE_

#include	"Uigp/igp.h"
#include	<stdio.h>


//typedef	unsigned char   u_char;


#define PL_NBITS 5



#define	PALETTE_RGB				0
#define PALETTE_BW				1
#define PALETTE_WB				2
#define PALETTE_GRAY			3
#define PALETTE_ADAPTIVE		4
#define PALETTE_WINDOW_SYSTEM	5
#define PALETTE_HALFTONE		6
#define PALETTE_222				7
#define PALETTE_UNKNOWN			8

#define PALETTE_ADAPTIVE4		10


#define PALETTE_SMART			PALETTE_ADAPTIVE


#define	PALETTE_IS_VALID( colorSpace )	( colorSpace >= 0 && colorSpace <= 7 )



#include "Ucolor/ColorType.h"


#ifdef _AA_
typedef struct color_type
{
	union {
		struct {
			u_char	Blue;
			u_char	Green;
			u_char	Red;
			u_char	alpha;
		};

		int	color;
	};


} color_type;
#endif



typedef struct palette_type
{
	int		type;

	int		bitPerPixel;

	int		ALLOCATED;
	int		nColor;
	color_type *data;

	char	*table;

} palette_type;



palette_type *	palette_alloc( int no );

palette_type *	palette_copy( palette_type *palette, palette_type *tp );


palette_type *	palette_copy_sample( palette_type *spl, int nColor );


void	palette_destroy( palette_type *p );


palette_type *	palette_create( int type );
palette_type *	palette_create_WINDOW_SYSTEM();
palette_type *  palette_create_222( );
palette_type *  palette_create_gray( int bit );
palette_type *  palette_create_HALFTONE();


int	palette_index( palette_type *palette, int red, int green, int blue );

int palette_color_space(char * colorStr);



void	palette_write( palette_type *p, int no, FILE *fp );

int		palette_write_to_file( palette_type *p, char *file );

int		palette_write_to_open_file( palette_type *p, FILE *fp );


palette_type *	palette_read_file( char *file );

palette_type *	palette_read_data( char *data );

#endif


