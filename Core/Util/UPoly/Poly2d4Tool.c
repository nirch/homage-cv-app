/*************************
 ***   Poly2d4Type.c   ***
 *************************/
#include	<stdio.h>
#include	<string.h>
#include	"Uigp/igp.h"
#include	"Poly2d4Type.h"

double	
poly2d4_value( poly2d4_type *f, double x, double y )
{
double	PX[5],	PY[5];
double	val;
int	i;

	PX[0] = PY[0] = 1;
	for( i = 1 ; i <= 4 ; i++ ){
		PX[i] = x*PX[i-1];
		PY[i] = y*PY[i-1];
	}

	val = f->a00;

	val += f->a10 * PX[1];
	val += f->a01 * PY[1];

	val += f->a20 * PX[2];
	val += f->a11 * PX[1]*PY[1];
	val += f->a02 * PY[2];

	val += f->a30 * PX[3]*PY[0];
	val += f->a21 * PX[2]*PY[1];
	val += f->a12 * PX[1]*PY[2];
	val += f->a03 * PX[0]*PY[3];

	val += f->a40 * PX[4]*PY[0];
	val += f->a31 * PX[3]*PY[1];
	val += f->a22 * PX[2]*PY[2];
	val += f->a13 * PX[1]*PY[3];
	val += f->a04 * PX[0]*PY[4];
	
	return( val );
}

void	
poly2d4_zero( poly2d4_type *f )
{
	f->a00 = 0;

	f->a10 = 0;
	f->a01 = 0;

	f->a20 = 0;
	f->a11 = 0;
	f->a02 = 0;

	f->a30 = 0;
	f->a21 = 0;
	f->a12 = 0;
	f->a03 = 0;

	f->a40 = 0;
	f->a31 = 0;
	f->a22 = 0;
	f->a13 = 0;
	f->a04 = 0;
}



void	
poly2d4_set( poly2d4_type *f, poly2d3_type *g )
{
	f->a00 = g->a00;
			 
	f->a10 = g->a10;
	f->a01 = g->a01;
			 
	f->a20 = g->a20;
	f->a11 = g->a11;
	f->a02 = g->a02;
			 
	f->a30 = 0;
	f->a21 = 0;
	f->a12 = 0;
	f->a03 = 0;
}


void	
poly2d4_derivative_x( poly2d4_type *f, poly2d3_type *fx )
{
	fx->a00 = f->a10;

	fx->a10 = 2*f->a20;
	fx->a01 =   f->a11;

	fx->a20 = 3*f->a30;
	fx->a11 = 2*f->a21;
	fx->a02 =   f->a12;

	fx->a30 = 4*f->a40;
	fx->a21 = 3*f->a31;
	fx->a12 = 2*f->a22;
	fx->a03 =   f->a13;
}

void	
poly2d4_derivative_y( poly2d4_type *f, poly2d3_type *fy )
{
	fy->a00 = f->a01;

	fy->a10 =   f->a11;
	fy->a01 = 2*f->a02;

	fy->a20 =   f->a21;
	fy->a11 = 2*f->a12;
	fy->a02 = 3*f->a03;

	fy->a30 =   f->a31;
	fy->a21 = 2*f->a22;
	fy->a12 = 3*f->a13;
	fy->a03 = 4*f->a04;
}


void
poly2d4_shift( poly2d4_type *f, double x, double y, poly2d4_type *g )
{

	g->a00 = f->a00 + f->a10 * x + f->a01 * y +
		f->a20 * x * x + f->a11 * x * y + f->a02 * y * y +
		f->a30 * x * x * x + f->a21 * x * x * y +
		f->a12 * x * y * y + f->a03 * y * y * y +
		f->a40 * x * x * x *x + f->a31 * x * x * x * y +
		f->a22 * x* x * y * y + f->a13 * x * y * y * y + f->a04 * y * y * y * y;


	g->a10 = f->a10 +
		2*f->a20 * x + f->a11 * y + 
		3*f->a30 * x * x + 2*f->a21* x * y + f->a12 * y * y + 
		4*f->a40 * x * x *x + 3*f->a31 * x * x * y + 2*f->a22 * x * y * y + f->a13 * y * y * y;


	g->a01 = f->a01 +
		f->a11 * x + 2*f->a02 * y +
		f->a21 * x * x + 2*f->a12 * x * y + 3*f->a03 * y * y +
		f->a31 * x * x * x + 2*f->a22 * x* x * y + 3*f->a13 * x * y * y + 4*f->a04 * y * y * y;



	// 2
	g->a20 = 
		1*f->a20 + 
		3*f->a30 * x + 1*f->a21 * y +
		6*f->a40 * x *x + 3*f->a31 * x * y + 1*f->a22 * y * y;



	g->a11 =
		f->a11 + 
		2*f->a21* x + 2*f->a12 * y + 
		3*f->a31 * x * x + 4*f->a22 * x * y + 3*f->a13 * y * y;


	g->a02 = 
		1*f->a02 +
		1*f->a12 * x + 3*f->a03 * y +
		1*f->a22 * x* x + 3*f->a13 * x * y + 6*f->a04 * y * y;




	g->a30 = 
		1*f->a30 +
		4*f->a40 *x + 1*f->a31 * y;

	g->a21 = 
		1*f->a21 +
		3*f->a31 * x + 2*f->a22 * y;


	g->a12 = 
		1*f->a12 +
		2*f->a22 * x + 3*f->a13 * y;

	g->a03 = 
		1*f->a03 +
		1*f->a13 * x + 4*f->a04* y;


	g->a40 = f->a40;
	g->a31 = f->a31;
	g->a22 = f->a22;
	g->a13 = f->a13;
	g->a04 = f->a04;
}



void	
poly2d4_max_diff( poly2d4_type *f0, poly2d4_type *f1, poly2d4_type *mf  )
{
double	tmp;
int	i;

	for( i = 0 ; i < POLY2D4_COEFF ; i++ ){
		tmp = f1->a[i] - f0->a[i];
		if( tmp < 0 )	tmp = -tmp;
		if( tmp > mf->a[i] )	mf->a[i] = tmp;
	}
}


void	
poly2d4_print( poly2d4_type *f, FILE *fp )
{
int	i;

	for( i = 0 ; i < POLY2D4_COEFF ; i++ ){
		fprintf( fp, "   %f", f->a[i] );
	}

	fprintf( fp, "\n" );
}



void	
poly2d4_value_y( poly2d4_type *f, double y, poly1d4_type *fx )
{
	fx->a0 = f->a00 + y* ( f->a01 + y* (f->a02 + y* (f->a03 + y* f->a04)));

	fx->a1 = f->a10 + y* ( f->a11 + y* (f->a12 + y* f->a13));

	fx->a2 = f->a20 + y* ( f->a21 + y* f->a22 );

	fx->a3 = f->a30 + y* f->a31;

	fx->a4 = f->a40;

}


int
poly2d4_read( poly2d4_type *f, FILE *fp )
{
char	line[1024];

	while( 1 ){
		if( fgets(line, 512, fp ) == NULL )
			return( -1 );

		if( strncmp( line, "poly2d4", 7 ) == 0 )
			break;
	}

	fscanf( fp, "%lf", &f->a00 );
	fscanf( fp, "%lf  %lf", &f->a10, &f->a01 );
	fscanf( fp, "%lf  %lf  %lf", &f->a20, &f->a11, &f->a02 );
	fscanf( fp, "%lf  %lf  %lf  %lf", &f->a30, &f->a21, &f->a12, &f->a03 );
	fscanf( fp, "%lf  %lf  %lf  %lf  %lf", &f->a40, &f->a31, &f->a22, &f->a13, &f->a04 );

	return( 1 );
}



void	
poly2d4_write( poly2d4_type *f, FILE *fp )
{
	fprintf( fp, "\npoly2d4\n" );
	fprintf( fp, "%.8lf\n", f->a00 );
	fprintf( fp, "%.8lf  %.8lf\n", f->a10, f->a01 );
	fprintf( fp, "%.8lf  %.8lf  %.8lf\n", f->a20, f->a11, f->a02 );
	fprintf( fp, "%.8lf  %.8lf  %.8lf  %.8lf\n", f->a30, f->a21, f->a12, f->a03 );
	fprintf( fp, "%.8lf  %.8lf  %.8lf  %.8lf  %.8lf\n", f->a40, f->a31, f->a22, f->a13, f->a04 );
}