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

	float	val;

	int	n;
	float av;
	float var;
	float m0,	m1;

	float w;


} st_type;



st_type * st_alloc( char *name );

#define		ST_VAL(st)	((st).val)


void	st_destory( st_type *st );

void	st_clear( st_type *st, char *name );

void	st_init( st_type *st );

void	st_add( st_type *st, float val );

void	st_append( st_type *st, st_type *st0 );


int	st_final( st_type *st );

int		st_write( st_type *st, FILE *fp );

int		st_read( st_type *st, FILE *fp );

float	st_average( st_type *st );

void	st_get( st_type *st, float *av, float *var );


#ifdef __cplusplus
}
#endif

#endif