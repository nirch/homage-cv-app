/************************
 *** DiagonalJet2.c   ***
 ************************/
#include	<math.h>

#include	"Uigp/igp.h"
#include	"Ujet2/Jet2Type.h"

/*
 * f  is a polynom of the form :  
 * f = a00 + a10*X + a01*Y + a20*X*X + a11*X*Y + a02*Y*Y
 *
 *
 *
 * final normal form:
 *		f = a[0] + a[1]*X + a[2] * Y + lamda1*XX + lamda2 *Y*Y
 *		n is unit vector in X direction
 *		|lm1| > |lm2|
 */


int
diagonalN_jet2( jet2_type *f, float fxy, djet2_type *d )
{
	float	a[3];

	diagonal_jet2( f, a, &d->lm1, &d->lm2, &d->n );


	d->a = a[0];

	d->a1 = a[1];
	d->a2 = a[2];

	d->dx = -d->a1 / (2*d->lm1);
	d->dy = -d->a2 / (2*d->lm2);

	d->dx *= fxy;
	d->dy *= fxy;


	d->p.x = d->n.x * d->dx + d->n.y *d->dy;
	d->p.y = d->n.y * d->dx - d->n.x *d->dy;

	return( 1 );
}



void 
diagonal_jet2(jet2 *fp, float a[3],
		      float *lamda1, float *lamda2,
		      vec2d *n )
{
float	desc , b ;
double	t;



	if( J11(*fp) > -0.001 && J11(*fp) < 0.001 ){

		if ( ABS(J20(*fp)) < ABS(J02(*fp))) {
			*lamda1 = J02(*fp);
			*lamda2 = J20(*fp);
			n->x = 0.;
			n->y = 1.;

			a[0] = J00(*fp);
			a[1] = J01(*fp) ;
			a[2] = J10(*fp) ;
		}
		else {
			*lamda1 = J20(*fp);
			*lamda2 = J02(*fp);
			n->x = 1.;
			n->y = 0.;
			a[0] = J00(*fp);
			a[1] = J10(*fp) ;
			a[2] = J01(*fp) ;
		}
		return;
	}


    b = J20(*fp) + J02(*fp) ;
    
    desc = hypot(J20(*fp) -J02(*fp), J11(*fp)) ;
  
    if( b > 0.0 )
		*lamda1 = 0.5 * ( b + desc ) ;
    else
		*lamda1 = 0.5 * ( b - desc ) ; 

    *lamda2 = b - *lamda1 ; 
 
  
    n->x = -J11(*fp);
    n->y = J20(*fp) - *lamda1 ;
    n->y += n->y;


	t = hypot( n->x, n->y ) ; 
	n->x /= t;
	n->y /= t;


    a[1] = J10(*fp)*n->x + J01(*fp)*n->y;
    a[2] = J10(*fp)*n->y - J01(*fp)*n->x;
  
  
    a[0] = J00(*fp);
}



void
diagonal_set_jet2( jet2_type *f, float a, float lm1, float lm2,
					float dx, float dy, vec2d *n )
{
jet2_type	g,	sf;
vec2d	n1;

	g.a00 = a;
	g.a10 = 0;
	g.a01 = 0;

	g.a20 = lm1;
	g.a11 = 0;
	g.a02 = lm2;

	shift_jet2( &g, -dx, -dy, &sf );

	n1.x = -n->y;
	n1.y = n->x;
	rotate_jet2( &sf, &n1, f ); 
}