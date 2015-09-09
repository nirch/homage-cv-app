#include	"Uigp/igp.h"
#include	"Jet2Type.h"


static void	app_jet2_one( jet2 *f,
			float fx, float fy,	/* global center of jet2 f */
			float c, float d, float a, float b,
						/* (c, d) + [ -a,a]*[-b,b] */
			jet2 *t );

static void	app_jet2_final( jet2 *t, jet2 *g );


void
app_jet2( jet2_array *ajet, float hx, float hy, float cx, float cy, jet2 *jet )
{
int     i,      j,      i0,     j0,     i1,     j1;
float   x0,     y0,     x1,     y1,     rx,     ry;
float   a,      b,      c,      d;
jet2    t;
float   dx,     dy; 
 
        t = ZERO_JET2; 
 
        dx = 2*hx; 
        dy = 2*hy;
 
        i0 = ( cx < 1 )?(cx-1)/dx -1 : (cx-1)/dx;
        j0 = ( cy < 1 )?(cy-1)/dy -1 : (cy-1)/dy;
        i1 = ( cx < -1)?(cx+1)/dx -1 : (cx+1)/dx;
        j1 = ( cy < -1)?(cy+1)/dy -1 : (cy+1)/dy;
 
 
        rx = (i0 + 0.5)*dx - cx;
        for( i = i0, x0 = -1 ; i <= i1 ; i++, rx += dx ){
 
                if( (x1= rx + hx) > 1 )        x1 = 1;
                c = (x1+x0)/2;
                a = x1 - c;             /* (x1-x0)/2 */
		if( a == 0 ){
                	x0 = x1; 
			continue;
		}

		if( i < 0 || i >= ajet->Ni ){
                	x0 = x1; 
			continue;
		}
 
                ry = (j0 + 0.5)*dy - cy;
                for( j = j0, y0 = -1 ; j <= j1 ; j++, ry += dy ){
 
                        if( (y1= ry + hy) > 1 )        y1 = 1;
 
                        d = (y1+y0)/2;
                        b = y1 - d;             /* (y1-y0)/2 */
			if( b == 0 ){
                        	y0 = y1; 
				continue;
			}

			if( j < 0 || j >= ajet->Nj ){
                        	y0 = y1; 
				continue;
			}
                 
                        app_jet2_one( &ajet->jet[i][j], rx, ry, c, d, a, b,
								&t );
  
                        y0 = y1; 
                }
                x0 = x1; 
        }

	app_jet2_final( &t, jet );
}



static void
app_jet2_one( jet2 *f,
	float fx, float fy,		/* global center of jet2 f */
	float c, float d, float a, float b,	/* (c, d) + [ -a,a]*[-b,b] */
	jet2 *t )
{
float	A,	B,	C,	D,	E,	F,	G,	H;
jet2	g,	q;


	shift_jet2( f, c-fx, d-fy, &g );

	A = 4*a*b;
	B = (a*a)*0.33333333333333;
	C = (b*b)*0.33333333333333;
	D = A*B;
	E = A*C;
	F = D*C;
	G = 1.8*D*B;
	H = 1.8 *E*C;

	
	J00(q) = J00(g)*A + J20(g)*D + J02(g)*E;

	J10(q) = J10(g)*D;
	J01(q) = J01(g)*E;

	J20(q) = J00(g)*D + J20(g)*G + J02(g)*F;
	J11(q) = J11(g)*F;
	J02(q) = J00(g)*E + J20(g)*F + J02(g)*H;


	J00(*t) += J00(q);

	J10(*t) += J10(q) + c*J00(q);
	J01(*t) += J01(q) + d*J00(q);

	J20(*t) += J20(q) + 2*c*J10(q) + c*c*J00(q);
	J11(*t) += J11(q) + d*J10(q) + c*J01(q) + c*d*J00(q);
	J02(*t) += J02(q) + 2*d*J01(q) + d*d*J00(q);
}



static void
app_jet2_final( jet2 *t, jet2 *g )
{
	/* ortogal jet
		0.5
		0.86625*x
		0.86625*y
		1.67705*x*x - 0.559016
		1.5*x*y
		1.67705*y*y - 0.559016
	*/

	J00(*g) = 0.874998 * J00(*t) - 0.937498*( J20(*t) + J02(*t) );

	J10(*g) = 0.750389 * J10(*t);
	J01(*g) = 0.750389 * J01(*t);

	J20(*g) = -0.937498*J00(*t) + 2.812497 * J20(*t);
	J11(*g) = 2.25 * J11(*t);
	J02(*g) = -0.937498*J00(*t) + 2.812497 * J02(*t);
}
