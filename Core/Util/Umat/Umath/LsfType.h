/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/*********************
 ***   SlfType.h   ***
 *********************/
#ifndef	_SLF_TYPE_
#define _SLF_TYPE_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct lsfB_type {
	float	x;
	float	y;

}	lsfB_type;


typedef struct lsf_type {
	int	nL;
	int	NL;

	lsfB_type	*f;


}	lsf_type;




lsf_type *	lsf_alloc( int n );


void	lsf_destroy( lsf_type *m );


float	lsf_value( lsf_type *lsf, float	x );


lsf_type *	lsf_parse( char *data );


void	lsf_print( lsf_type *lsf, FILE *fp );

#ifdef __cplusplus
}
#endif

#endif