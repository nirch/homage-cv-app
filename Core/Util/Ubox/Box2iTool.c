#include	<stdio.h>


#include	"Uigp/igp.h"

#include	"Box2d.h"


int
box2i_select( box2i *b, int select_t, int x, int y )
{
	int	tmp,	max,	op;

	op = 0;
	max = select_t;

	tmp = ( b->x0 - x )*( b->x0 - x ) + ( b->y0 - y )*( b->y0 - y );
	if( tmp < max ){
		max = tmp;
		op = 1;
	}

	tmp = ( b->x0 - x )*( b->x0 - x ) + ( b->y1 - y )*( b->y1 - y );
	if( tmp < max ){
		max = tmp;
		op = 2;
	}


	tmp = ( b->x1 - x )*( b->x1 - x ) + ( b->y0 - y )*( b->y0 - y );
	if( tmp < max ){
		max = tmp;
		op = 3;
	}

	tmp = ( b->x1 - x )*( b->x1 - x ) + ( b->y1 - y )*( b->y1 - y );
	if( tmp < max ){
		max = tmp;
		op = 4;
	}

	if( op != 0 )	return( op );

	return( 0 );
}


void
box2i_set( box2i *b, box2d *box, float margin )
{
	b->x0 = box->x0 - margin;
	b->x1 = box->x1 + margin;

	b->y0 = box->y0 - margin;
	b->y1 = box->y1 + margin;
}

void
box2i_crop( box2i *b, int x0, int y0, int x1, int y1 )
{
	if( b->x0 < x0 )	b->x0 = x0;
	if( b->x1 > x1 )	b->x1 = x1;

	if( b->y0 < y0 )	b->y0 = y0;
	if( b->y1 > y1 )	b->y1 = y1;
}


void
box2i_extend( box2i *b, int margin )
{
	b->x0 -= margin;
	b->x1 += margin;

	b->y0 -= margin;
	b->y1 += margin;
}

void
box2i_extendI( box2i *b, int margin, int width, int height )
{
	b->x0 -= margin;
	b->x1 += margin;

	b->y0 -= margin;
	b->y1 += margin;

	if( b->x0 < 0 )	b->x0 = 0;
	if( b->y0 < 0 )	b->y0 = 0;

	if( b->x1 > width )	b->x1 = width;
	if( b->y1 > height )	b->y1 = height;

}


void
box2i_translate( box2i *b, int x, int y )
{
	b->x0 += x;
	b->x1 += x;

	b->y0 += y;
	b->y1 += y;
}





int
box2i_read( FILE *fp, box2i *b )
{
char	str[256];

	fscanf( fp, "%s %d %d %d %d", str, &b->x0, &b->y0, &b->x1, &b->y1 );

	if( str[0] == 'R' ){
		b->x1 += b->x0;
		b->y1 += b->y0;
	}


	return( 1 );
}


int
box2i_write( FILE *fp, box2i *b )
{
	fprintf( fp, "N %d %d %d %d\n", b->x0, b->y0, b->x1, b->y1 );

	return( 1 );
}

int
box2i_read_from_file( char *file, box2i *lb, box2i *gb )
{
	FILE	*fp;

	if( (fp = fopen( file, "rb" )) == NULL )
		return( -1 );


	box2i_read( fp, lb );

	if( gb != NULL )
		box2i_read( fp, gb );

	fclose( fp );

	return( 1 );
}

int
box2i_write_to_file( char *file, box2i *lb, box2i *gb )
{
	FILE	*fp;

	if( (fp = fopen( file, "wb" )) == NULL )
		return( -1 );



	box2i_write( fp, lb );

	if( gb != NULL )
		box2i_write( fp, gb );

	fclose( fp );

	return( 1 );
}





static int	box2i_remove_in_1( box2i ab[], int nB, int i0 );

void
box2iA_remove_in( box2i ab[], int *nB )
{
	int	i;

	for( i = 0 ; i < *nB ; ){

		if( box2i_remove_in_1( ab, *nB, i) < 0 ){
			i++;
			continue;
		}


		(*nB)--;
		ab[i] = ab[*nB];
	}
}

static int
box2i_remove_in_1( box2i ab[], int nB, int i0 )
{
	int	i;
	box2i	*b,	*cb;

	b = &ab[i0];

	for( i = 0 ; i < nB ; i++ ){
		if( i == i0 )	continue;
		cb = &ab[i];
		if( BOX2D_IS_BOX_IN( *cb, *b ) )
			return( 1 );
	}

	return( -1 );
}

