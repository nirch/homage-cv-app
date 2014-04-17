/******************
 ***	code.c	***
 ******************/
#include	<string.h>
#include	"code.h"


void
code_set( code *c, float min, float max, float d, float avarege, float nf )
{
	c->min = min * nf;
	c->max = max * nf;
	c->d = d * nf;

	c->a = avarege;



	c->inv_d = 1.0/c->d;

	c->imax = (c->max - c->min) * c->inv_d;

	if( c->imax*c->d == c->max - c->min )
		c->imax -= 1;

	c->rbit = coding_ilog2( c->imax );

	c->hr = (c->imax+1)/2;



	c->bits = 0;
}


void
coding_set_lprm( code_in *c_in, code *c, float	nf )
{
	c->min = c_in->min * nf;
	c->max = c_in->max * nf;
	c->d = c_in->d * nf;

	c->inv_d = 1.0/c->d;

	c->imax = (c->max - c->min) * c->inv_d;

	if( c->imax*c->d == c->max - c->min )
		c->imax -= 1;

	c->rbit = coding_ilog2( c->imax );

	c->hr = (c->imax+1)/2;


	c->a = c_in->a;



	c->bits = 0;

}

/*
void
coding_set_data_lprm( code_in *c_in, code *c, float nf, char *file_name )
{
	c->min = c_in->min * nf;
	c->max = c_in->max * nf;
	c->d = c_in->d * nf;

	c->inv_d = 1.0/c->d;

	c->imax = (c->max - c->min) * c->inv_d;

	if( c->imax*c->d == c->max - c->min )
		c->imax -= 1;

	c->rbit = coding_ilog2( c->imax );

	c->hr = (c->imax+1)/2;

	c->a = c_in->a;
	c->f = c_in->f;

	strcpy( c->fname, file_name );

	c->bits = 0;
}
*/

void
code_file_set( cfile_type *c, char *file_name )
{
	strcpy( c->fname, file_name );

	c->fd = -1;
	c->fp = NULL;

	c->bits = 0;
}



int
coding_ilog2( int a )
{
int     i;
        if( a < 0 )     a = -a;
 
        if( a < 1 )     return( 0 ); 
 
        for( i = 1 ; ; i++ )
                if( a < (1L<<i) )  return( i );
}
