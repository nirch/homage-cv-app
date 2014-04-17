/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef _ST_TYPE_
#define _ST_TYPE_


#ifdef __cplusplus
extern "C" {
#endif


typedef struct st_type {
	char	name[64];

	int	n;
	float av;
	float var;
	float m0,	m1;

	float w;
} st_type;



st_type * st_alloc( char *name, int no );


void	st_destory( st_type *st );

void	st_init( st_type *st );

void	st_add( st_type *st, float val );

void	st_append( st_type *st, st_type *st0 );


void	st_final( st_type *st );

int		st_write( st_type *st, FILE *fp );

int		st_read( st_type *st, FILE *fp );




#ifdef __cplusplus
}
#endif

#endif