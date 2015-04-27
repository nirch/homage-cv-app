#include	<string.h>
#include	<stdio.h>
#include	<math.h>


#include	"Uigp/igp.h"


#include	"Ubox/Box2d.h"
#include	"Umath/Matrix2Type.h"

#include	"BltType.h"







void
blt_set( blt_type *bl, box2f_type *b, vec2f_type *v0, vec2f_type *v1, vec2f_type *v2, vec2f_type *v3 )
{

	blt1_set( &bl->f1, b, v0->x, v1->x, v2->x, v3->x );

	blt1_set( &bl->f2, b, v0->y, v1->y, v2->y, v3->y );

#define _TEST_
#ifdef _TEST_
	vec2f_type	u0,	u1,	u2,	u3;
	float	x,	y;
	x = b->x0;
	y = b->y0;
	u0.x = BLT1_VALUE( bl->f1, x, y );
	u0.y = BLT1_VALUE( bl->f2, x, y );

	x = b->x1;
	y = b->y0;
	u1.x = BLT1_VALUE( bl->f1, x, y );
	u1.y = BLT1_VALUE( bl->f2, x, y );

	x = b->x0;
	y = b->y1;
	u2.x = BLT1_VALUE( bl->f1, x, y );
	u2.y = BLT1_VALUE( bl->f2, x, y );

	x = b->x1;
	y = b->y1;
	u3.x = BLT1_VALUE( bl->f1, x, y );
	u3.y = BLT1_VALUE( bl->f2, x, y );

#endif
}




void
blt1_set( blt1_type *bl, box2f_type *b, float v0, float v1, float v2, float v3 )
{
float a0,	b0;
float a1,	b1;


//	v0 + (v1 - v0 )*(x - b->x0 )/(b->x1 - b->x0);
	b0 = (v1 - v0 )/(b->x1 - b->x0);
	a0 = v0 - b0*b->x0;



//	v0 + (v1 - v0 )*(x - b->x0 )/(b->x1 - b->x0);
	b1 = (v3 - v2 )/(b->x1 - b->x0);
	a1 = v2 - b1*b->x0;





//	A0 + B0*X   + ( A1 + b1X - a0 - b0x)* (y - b->y0 )/(b->y1 - b->y0)
	float t = 1.0 / (b->y1 - b->y0);


	//float	a00,	a01,	a10,	a11;
	bl->a11 =  (b1 - b0 )*t;
	bl->a10 = b0 -(b1 - b0 )*b->y0 *t;
	bl->a01 = (a1 - a0 )* t;
	bl->a00 = a0 - (a1 - a0)*b->y0 * t;


#ifdef _TEST_
	float	x,	y;
	x = b->x0;
	y = b->y0;
//	float t0 = a00 + a10* x + a01*y + a11*x*y;
	float t0 = BLT1_VALUE( *bl, x, y );

	x = b->x1;
	y = b->y0;
	//float t1 = a00 + a10* x + a01*y + a11*x*y;
	float t1 = BLT1_VALUE( *bl, x, y );

	x = b->x0;
	y = b->y1;
	//float t2 = a00 + a10* x + a01*y + a11*x*y;
	float t2 = BLT1_VALUE( *bl, x, y );

	x = b->x1;
	y = b->y1;
	//float t3 = a00 + a10* x + a01*y + a11*x*y;
	float t3 = BLT1_VALUE( *bl, x, y );
#endif
}



void
blt_test( blt_type *bl, box2f_type *b, vec2f_type *v0, vec2f_type *v1, vec2f_type *v2, vec2f_type *v3 )
{

	vec2f_type	u0,	u1,	u2,	u3;
	float	x,	y;
	x = b->x0;
	y = b->y0;
	u0.x = BLT1_VALUE( bl->f1, x, y );
	u0.y = BLT1_VALUE( bl->f2, x, y );
	fprintf( stdout, "%f %f\n", u0.x - v0->x, u0.y - v0->y );

	x = b->x1;
	y = b->y0;
	u1.x = BLT1_VALUE( bl->f1, x, y );
	u1.y = BLT1_VALUE( bl->f2, x, y );
	fprintf( stdout, "%f %f\n", u1.x - v1->x, u1.y - v1->y );

	x = b->x0;
	y = b->y1;
	u2.x = BLT1_VALUE( bl->f1, x, y );
	u2.y = BLT1_VALUE( bl->f2, x, y );
	fprintf( stdout, "%f %f\n", u2.x - v2->x, u2.y - v2->y );

	x = b->x1;
	y = b->y1;
	u3.x = BLT1_VALUE( bl->f1, x, y );
	u3.y = BLT1_VALUE( bl->f2, x, y );
	fprintf( stdout, "%f %f\n", u3.x - v3->x, u3.y - v3->y );

}


void
blt_write( blt_type *bl, FILE *fp)
{

	fprintf( fp, "%f   %f  %f   %f\n", bl->f1.a00, bl->f1.a10, bl->f1.a01, bl->f1.a11 );

	fprintf( fp, "%f   %f  %f   %f\n", bl->f2.a00, bl->f2.a10, bl->f2.a01, bl->f2.a11 );
}




int
blt_inverse( blt_type *bl, vec2d_type *v, double d, vec2d_type *iv )
{
	vec2d_type	p,	u,	X;

	p.x = p.y = 0;

	int	i;
	for( i = 0 ; i < 10 ; i++ ){
		u.x = BLT1_VALUE( bl->f1, p.x, p.y ) - v->x;
		u.y = BLT1_VALUE( bl->f2, p.x, p.y ) - v->y;


		if( ABS(u.x) < d && ABS(u.y) < d )
			break;


		matrix2_type	mm;
		mm.a00 = bl->f1.a10 + bl->f1.a11*p.y;
		mm.a01 = bl->f1.a01 + bl->f1.a11*p.x;

		mm.a10 = bl->f2.a10 + bl->f2.a11*p.y;
		mm.a11 = bl->f2.a01 + bl->f2.a11*p.x;

		matrix2_solve( &mm, &u, &X );

		p.x -= X.x;
		p.y -= X.y;
	}

	fprintf( stdout, "%d   %lf   %lf\n", i, u.x, u.y );

	*iv = p;

	return( 1 );
}