/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/*********************
 ***   BltType.h   ***
 *********************/
#ifndef	_BLT_TYPE_
#define _BLT_TYPE_

#include	"Uigp/igp.h"

#include	"Ubox/Box2d.h"



typedef struct blt1_type	{
	float	a00,	a10,	a01,	a11;
} blt1_type;


#define BLT1_VALUE( b, x, y )	( (b).a00 + (b).a10* x + (b).a01*y + (b).a11*x*y )


typedef struct blt_type	{
	blt1_type	f1;
	blt1_type	f2;
} blt_type;



void	blt_set( blt_type *bl, box2f_type *b, vec2f_type *v0, vec2f_type *v1, vec2f_type *v2, vec2f_type *v3 );

void	blt1_set( blt1_type *bl, box2f_type *b, float v0, float v1, float v2, float v3 );

void	blt_test( blt_type *bl, box2f_type *b, vec2f_type *v0, vec2f_type *v1, vec2f_type *v2, vec2f_type *v3 );

void	blt_write( blt_type *bl, FILE *fp);


int	blt_inverse( blt_type *bl, vec2d_type *v, double d, vec2d_type *iv );



#endif