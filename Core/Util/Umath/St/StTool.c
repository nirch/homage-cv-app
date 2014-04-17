
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>

#include	"Uigp/igp.h"
#include	"StType.h"



st_type *
st_alloc( char *name, int no )
{
	st_type	*st;

	st = ( st_type *)malloc( sizeof(st_type ) );

	strcpy( st->name, name );

	st_init( st );

	return( st );
}

void
st_destory( st_type *st )
{
	free( st );
}


void
st_init( st_type *st )
{
	st->n = 0;
	st->m0 = st->m1 = 0;
	st->av = st->var  = 0;
	st->w = 1;
}


void
st_add( st_type *st, float val )
{
	if( st->n == 0 ){
		st->av = val;
		st->var = val*val;
		st->m0 = st->m1 = val;
		st->n = 1;
		return;
	}


	st->av += val;
	st->var += val*val;

	if( val < st->m0 )
		st->m0 = val;
	else
		if( val > st->m1 )
			st->m1 = val;

	st->n++;
}


void
st_append( st_type *st, st_type *st0 )
{
	if( st->n == 0 ){
		st->av = st0->av;
		st->var = st0->var;
		st->m0 = st0->m0;
		st->m1 = st0->m1;
		st->n = st0->n;
		return;
	}


	st->av += st0->av;
	st->var += st0->var;

	if( st0->m0 < st->m0 )
		st->m0 = st0->m0;
	else
		if( st0->m1 > st->m1 )
			st->m1 = st0->m1;

	st->n += st0->n;
}


void
st_final( st_type *st )
{
	st->av /= st->n;

	st->var = st->var/ st->n - st->av*st->av;

	st->var = sqrt( st->var );
}






int
st_write( st_type *st, FILE *fp )
{
	fprintf( fp, "%s\t%f\t%f\t%f\t%f\t%d\t%.4f\n",
		st->name,
		st->av,
		st->var,
		st->m0,
		st->m1,
		st->n,
		st->w );

	return( 1 );
}



int
st_read( st_type *st, FILE *fp )
{
	fscanf( fp, "%s %f %f %f %f %d %f",
		st->name,
		&st->av,
		&st->var,
		&st->m0,
		&st->m1,
		&st->n,
		&st->w );

	return( 1 );
}