
#include	"Uigp/igp.h"
#include	"Uvec/Vec2d.h"
#include	"Box2d.h"

#include	"Ucamera/Pt2dType.h"


box2d *box2d_alloc()
{
	box2d *box= (box2d*)malloc(sizeof(box2d));
	box->x0 = box->y0 = box->x1 = box->y1 = 0;
	return box;
}


int
box2_segment( box2d *b, vec2d *p, vec2d *p1, int Fp1, vec2d *v0, vec2d *v1 )
{
vec2d	n;
float	t0,	t1,	y0,	y1;

	if( Fp1 == 1 ){
		n.x = p1->x - p->x;
		n.y = p1->y - p->y;
	}
	else	n = *p1;

	if( n.x == 0 && n.y == 0 )	return( 0 );

	if( n.y == 0 ){

/*
		if( v0->y < b->y0 || v0->y > b->y1 )	return( 0 );
*/
		if( p->y < b->y0 || p->y > b->y1 )	return( 0 );

		t0 = (b->x0 - p->x )/n.x;
		t1 = (b->x1 - p->x )/n.x;

		v0->x = p->x + t0 *n.x;
		v0->y = p->y;
/*
		if( p->x < b->x0 || p->x > b->x1 )	return( 0 );
*/
		if( v0->x < b->x0 || v0->x > b->x1 )	return( 0 );


		v1->x = p->x + t1 *n.x;
		v1->y = p->y;

		return( 1 );
	}


	if( n.x == 0 ){

/*
		if( v0->x < b->x0 || v0->x > b->x1 )	return( 0 );
*/
		if( p->x < b->x0 || p->x > b->x1 )	return( 0 );
		t0 = (b->y0 - p->y )/n.y;
		t1 = (b->y1 - p->y )/n.y;

		v0->x = p->x;
		v0->y = p->y + t0 *n.y;
		if( v0->y < b->y0 || v0->y > b->y1 )	return( 0 );

		v1->x = p->x;
		v1->y = p->y + t1 *n.y;

		return( 1 );
	}




	if( n.x > 0 ){
		t0 = (b->x0 - p->x )/n.x;
		t1 = (b->x1 - p->x )/n.x;
	}
	else	{
		t0 = (b->x1 - p->x )/n.x;
		t1 = (b->x0 - p->x )/n.x;
	}

	if( n.y > 0 ){
		y0 = (b->y0 - p->y )/n.y;
		y1 = (b->y1 - p->y )/n.y;
	}
	else	{
		y0 = (b->y1 - p->y )/n.y;
		y1 = (b->y0 - p->y )/n.y;
	}

	if( t0 < y0 )	t0 = y0;
	if( t1 > y1 )	t1 = y1;


	v0->x = p->x + t0 *n.x;
	v0->y = p->y + t0 *n.y;
	if( v0->x < b->x0-0.001 || v0->x > b->x1+0.001 )	return( 0 );
	if( v0->y < b->y0-0.001 || v0->y > b->y1+0.001 )	return( 0 );

	v1->x = p->x + t1 *n.x;
	v1->y = p->y + t1 *n.y;

	return( 1 );
}



void
box_shift( box2d *b, float x, float y )
{
	b->x0 += x;
	b->x1 += x;

	b->y0 += y;
	b->y1 += y;
}


void
box2f_translate( box2d *b, float x, float y )
{
	b->x0 += x;
	b->x1 += x;

	b->y0 += y;
	b->y1 += y;
}

void
box2f_update( box2f_type *b, box2f_type *b1 )
{
	if( b->x0 > b1->x0 )	b->x0 = b1->x0;
	if( b->x1 < b1->x1 )	b->x1 = b1->x1;

	if( b->y0 > b1->y0 )	b->y0 = b1->y0;
	if( b->y1 < b1->y1 )	b->y1 = b1->y1;
}

void
box2f_extend( box2f_type *b, int margin )
{
	b->x0 -= margin;
	b->x1 += margin;

	b->y0 -= margin;
	b->y1 += margin;
}


void
box2f_union( box2f_type ab[], int no, box2f_type *b )
{
int	i;
	*b = ab[0];

	for( i = 1 ; i < no ; i++ ){
		if( b->x0 > ab[i].x0 )	b->x0 = ab[i].x0;
		if( b->x1 < ab[i].x1 )	b->x1 = ab[i].x1;

		if( b->y0 > ab[i].y0 )	b->y0 = ab[i].y0;
		if( b->y1 < ab[i].y1 )	b->y1 = ab[i].y1;
	}
}


void
box2d_round( box2d *b )
{
	if( b->x0 < 0 )	b->x0 = ((int)b->x0) -1;
	else	b->x0 = ((int)b->x0);

	if( b->y0 < 0 )	b->y0 = ((int)b->y0) -1;
	else	b->y0 = ((int)b->y0);


	if( b->x1 < 0 )	b->x1 = ((int)b->x1);
	else	b->x1 = ((int)b->x1+1);

	if( b->y1 < 0 )	b->y1 = ((int)b->y1);
	else	b->y1 = ((int)b->y1+1);
}


void
box2dA_union( box2f_type ab[], int *nB, float d )
{
	int	i,	j,	n;

	for( i = 1, n = 1 ; i < *nB ; i++ ){

		for( j = 0 ; j < n ; j++ ){
			if( box2d_distance( &ab[i], &ab[j] ) < d )
				break;
		}

		if( j < n  ){
			box2f_update( &ab[j], &ab[i] );
			continue;
		}

		ab[n++] = ab[i];
	}

	*nB = n ;
}



//return positive value if the box are separate and negative value for intersect 
float
box2f_distance( box2f_type *b, box2f_type *b1 )
{
float	x,	y,	dis;


	if( b->x0 < b1->x0 )
		x = b1->x0 - b->x1;
	else x = b->x0 - b1->x1;


	if( b->y0 < b1->y0 )
		y = b1->y0 - b->y1;
	else y = b->y0 - b1->y1;


	dis = MAX( x, y );

	return( dis );
}



int
box2f_read( FILE *fp, box2f_type *b )
{
	char	str[256];

	fscanf( fp, "%s %f %f %f %f", str, &b->x0, &b->y0, &b->x1, &b->y1 );

	if( str[0] == 'R' ){
		b->x1 += b->x0;
		b->y1 += b->y0;
	}


	return( 1 );
}

int
box2f_read_from_file( box2f_type *b, char *file )
{
FILE	*fp;

	if( (fp = fopen( file, "rb" )) == NULL )
		return( -1 );


	box2f_read( fp, b );


	fclose( fp );

	return( 1 );
}

int
box2f_read_data( box2f_type *b, char *data )
{
char str[16];


	if( sscanf( data, "%s %f %f %f %f", str, &b->x0, &b->y0, &b->x1, &b->y1 ) != 5 )
		return( -1 );

	if( str[0] == 'R' ){
		b->x1 += b->x0;
		b->y1 += b->y0;
	}


	return( 1 );
}
int
	box2f_write_from_file( box2f_type *b, char *file )
{
	FILE	*fp;


	if( (fp = fopen( file, "wb" )) == NULL )
		return( -1 );


	fprintf( fp, "%s %f %f %f %f", "N", b->x0, b->y0, b->x1, b->y1 );



	fclose( fp );

	return( 1 );
}



int
box2d_read_from_file( box2d_type *b, char *file )
{
	FILE	*fp;
	char str[256];

	if( (fp = fopen( file, "rb" )) == NULL )
		return( -1 );


	fscanf( fp, "%s %lf %lf %lf %lf", str, &b->x0, &b->y0, &b->x1, &b->y1 );

	if( str[0] == 'R' ){
		b->x1 += b->x0;
		b->y1 += b->y0;
	}


	fclose( fp );

	return( 1 );
}


int
box2d_write_from_file( box2d_type *b, char *file )
{
	FILE	*fp;


	if( (fp = fopen( file, "wb" )) == NULL )
		return( -1 );


	fprintf( fp, "%s %lf %lf %lf %lf", "N", b->x0, b->y0, b->x1, b->y1 );



	fclose( fp );

	return( 1 );
}

//
//int
//	box2f_read_from_file( box2f_type *b, char *file )
//{
//	FILE	*fp;
//	char str[256];
//
//	if( (fp = fopen( file, "rb" )) == NULL )
//		return( -1 );
//
//
//	fscanf( fp, "%s %f %f %f %f", str, &b->x0, &b->y0, &b->x1, &b->y1 );
//
//	if( str[0] == 'R' ){
//		b->x1 += b->x0;
//		b->y1 += b->y0;
//	}
//
//
//	fclose( fp );
//
//	return( 1 );
//}







int
box2f_push_in( box2f_type *b, vec2f_type *p )
{
	
	int n = 0;

	if( p->x < b->x0 ){
		p->x = b->x0;
		n += 1;
	}
	else
		if( p->x > b->x1 ){
			p->x = b->x1;
			n += 1;
		}


	if( p->y < b->y0 ){
		p->y = b->y0;
		n += 1;
	}
	else
		if( p->y > b->y1 ){
			p->y = b->y1;
			n += 1;
		}

	return( n );
}


