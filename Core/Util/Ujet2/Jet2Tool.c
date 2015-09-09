/**********************
 ***   Jet2tool.c   ***
 **********************/
#include	<stdio.h>
#include	<math.h>

#ifdef WIN32
#pragma warning(disable :4789)
#endif

#include	"Uigp/igp.h"
#include	"Jet2Type.h"


jet2     ZERO_JET2 = {   0,
                         0, 0,
                         0, 0, 0 };
                        /*
                         0.0, 0.0, 0.0, 0.0,
                         0.0, 0.0, 0.0, 0.0, 0.0 };
			*/

int     Coeff_no[ JET_ORDER4+1 ] = { 1, 3, 6, 10, 15 };
int     Coeff_dgree[ COEFF_NO4 ] = {    0,
                                        1, 1,
                                        2, 2, 2,
                                        3, 3, 3, 3,
                                        4, 4, 4, 4, 4 };


float
value_jet2( jet2 *f , int dgree, float x , float y )
{
float	u;
	u = J00(*f) + y*( J01(*f) + J02(*f)*y ) +
		x*(  J10(*f) + J20(*f)*x + J11(*f)*y );


	if( dgree <= 2 )	return( u );

	u += x*( x*( J30(*f)*x + J21(*f)*y ) + J12(*f)*y*y ) + J03(*f)*y*y*y ;

	if( dgree == 3 )	return( u );

	u += J40(*f) *x*x*x*x  + J31(*f) *x*x*x*y  + J22(*f) *x*x*y*y +
	     J13(*f) *x*y*y*y  + J04(*f) *y*y*y*y ;

	return( u );
}


void
value_dp_jet2( jet2 *f, float x, float y, float *val, float *dx, float *dy )
{
	*val = J00(*f) + J10(*f)*x + J01(*f)*y +
		J20(*f)*x*x + J11(*f)*x*y + J02(*f)*y*y ;

	*dx =  J10(*f) + 2*J20(*f)*x + J11(*f)*y ;

	*dy = J01(*f) + J11(*f)*x + 2*J02(*f)*y ;

}


void
divide_jet2( jet2 *g, jet2 *f )	/* compute jet2 of  1/(g(x,y)  at (0,0)  */
{
float	a;
	/*	rank	0	*/
	a = J00(*f) = 1.0 / J00(*g);

	/*	rank	1	*/
	a *= J00(*f);
	J10(*f) = - J10(*g) * a;
	J01(*f) = - J01(*g) * a;

	/*	rank	2	*/
	a *= J00(*f);
	
	J20(*f) = (   J10(*g)*J10(*g) - J20(*g)*J00(*g) ) *a;
	J11(*f) = ( 2*J10(*g)*J01(*g) - J11(*g)*J00(*g) ) *a;
	J02(*f) = (   J01(*g)*J01(*g) - J02(*g)*J00(*g) ) *a;
}


void
product_jet2( jet2 *g, jet2 *f, jet2 *F )
			/* compute the jet of g*f  at (0,0)	*/
{

	/*	rank 0		*/
	J00(*F) = J00(*g)*J00(*f);

	/*	rank 1		*/
	J10(*F) = J10(*g)*J00(*f) + J00(*g)*J10(*f);
	J01(*F) = J01(*g)*J00(*f) + J00(*g)*J01(*f);

	/*	rank 2		*/
	J20(*F) = J20(*g)*J00(*f) + J10(*g)*J10(*f) + J00(*g)*J20(*f);
	J11(*F) = J11(*g)*J00(*f) + J10(*g)*J01(*f) +
					J01(*g)*J10(*f) + J00(*g)*J11(*f);
	J02(*F) = J02(*g)*J00(*f) + J01(*g)*J01(*f) + J00(*g)*J02(*f);
}


void   
rescaling_jet2( jet2 *f, int dgree, float hx, float hy, jet2 *g )
			/* compute the jet of g(hx*x,hx*y) at (0,0)*/
{

	/*	rank 0		*/
	J00(*g) = J00(*f);

	/*	rank 1		*/
	J10(*g) = J10(*f) * hx;
	J01(*g) = J01(*f) *   hy;

	/*	rank 2		*/
	J20(*g) = J20(*f) * hx*hx;
	J11(*g) = J11(*f) *    hx*hy;
	J02(*g) = J02(*f) *       hy*hy;


	if( dgree == 2 )	return;


	/*	rank 3		*/
	J30(*g) = J30(*f) * hx*hx*hx;
	J21(*g) = J21(*f) *    hx*hx*hy;
	J12(*g) = J12(*f) *       hx*hy*hy;
	J03(*g) = J03(*f) *          hy*hy*hy;

	if( dgree == 3 )	return;

	/*	rank 4		*/
	J40(*g) = J40(*f) * hx*hx*hx*hx;
	J31(*g) = J31(*f) *    hx*hx*hx*hy;
	J22(*g) = J22(*f) *       hx*hx*hy*hy;
	J13(*g) = J13(*f) *          hx*hy*hy*hy;
	J04(*g) = J04(*f) *             hy*hy*hy*hy;
}

jet2
scalar_product_jet2( jet2 *g, float s, int dgree )
			/* compute the jet of s*g(x,y) at (0,0)	*/
{
jet2	F;

	/*	rank 0		*/
	J00(F) = J00(*g) * s;

	/*	rank 1		*/
	J10(F) = J10(*g) * s;
	J01(F) = J01(*g) * s;

	/*	rank 2		*/
	J20(F) = J20(*g) * s;
	J11(F) = J11(*g) * s;
	J02(F) = J02(*g) * s;

	if( dgree == 2 )	return( F );

	/*	rank 3		*/
	J30(F) = J30(*g) * s;
	J21(F) = J21(*g) * s;
	J12(F) = J12(*g) * s;
	J03(F) = J03(*g) * s;

	if( dgree == 3 )	return( F );

	/*	rank 4		*/
	J40(F) = J40(*g) * s;
	J31(F) = J31(*g) * s;
	J22(F) = J22(*g) * s;
	J13(F) = J13(*g) * s;
	J04(F) = J04(*g) * s;


	return( F );
}

jet2
sum_jet2( jet2 *g, jet2 *f, int dgree )
			/* compute the jet of g(x,y)+f(x,y)  at (0,0)	*/
{
jet2	F;

	/*	rank 0		*/
	J00(F) = J00(*g) + J00(*f);

	/*	rank 1		*/
	J10(F) = J10(*g) + J10(*f);
	J01(F) = J01(*g) + J01(*f);

	/*	rank 2		*/
	J20(F) = J20(*g) + J20(*f);
	J11(F) = J11(*g) + J11(*f);
	J02(F) = J02(*g) + J02(*f);

	if( dgree == 2 )	return( F );

	/*	rank 3		*/
	J30(F) = J30(*g) + J30(*f);
	J21(F) = J21(*g) + J21(*f);
	J12(F) = J12(*g) + J12(*f);
	J03(F) = J03(*g) + J03(*f);

	if( dgree == 3 )	return( F );

	/*	rank 4		*/
	J40(F) = J40(*g) + J40(*f);
	J31(F) = J31(*g) + J31(*f);
	J22(F) = J22(*g) + J22(*f);
	J13(F) = J13(*g) + J13(*f);
	J04(F) = J04(*g) + J04(*f);


	return( F );
}

jet2
sub_jet2( jet2 *g, jet2 *f, int dgree )
			/* compute the jet of g(x,y)+f(x,y) at (0,0)	*/
{
jet2	F;

	/*	rank 0		*/
	J00(F) = J00(*g) - J00(*f);

	/*	rank 1		*/
	J10(F) = J10(*g) - J10(*f);
	J01(F) = J01(*g) - J01(*f);

	/*	rank 2		*/
	J20(F) = J20(*g) - J20(*f);
	J11(F) = J11(*g) - J11(*f);
	J02(F) = J02(*g) - J02(*f);

	if( dgree == 2 )	return( F );

	/*	rank 3		*/
	J30(F) = J30(*g) - J30(*f);
	J21(F) = J21(*g) - J21(*f);
	J12(F) = J12(*g) - J12(*f);
	J03(F) = J03(*g) - J03(*f);

	if( dgree == 3 )	return( F );

	/*	rank 4		*/
	J40(F) = J40(*g) - J40(*f);
	J31(F) = J31(*g) - J31(*f);
	J22(F) = J22(*g) - J22(*f);
	J13(F) = J13(*g) - J13(*f);
	J04(F) = J04(*g) - J04(*f);

	return( F );
}


void
shift_jet2( jet2 *f , float x , float y, jet2 *g )
{
	J00( *g ) = J00(*f) + J10(*f)*x + J01(*f)*y +
		J20(*f)*x*x + J11(*f)*x*y + J02(*f)*y*y ;

	J10( *g ) =  J10(*f) +
		2*J20(*f)*x + J11(*f)*y ;

	J01( *g ) = J01(*f) +
		J11(*f)*x + 2*J02(*f)*y ;


	J20( *g ) =  J20(*f) ;

	J11( *g ) =  J11(*f) ;

	J02( *g ) = J02(*f) ;
}



/* rotate the domian coordinet such that the new Y axis is vector v */
void
rotate_jet2( jet2 *f, vec2d *v, jet2 *g )
{
float	a,	b;

	a = v->x;
	b = v->y;

	J00(*g) = J00(*f);

	J10(*g) = J10(*f) * b - J01(*f) * a;
	J01(*g) = J10(*f) * a + J01(*f) * b;


	J20(*g) = J20(*f) * b * b - J11(*f) * a * b + J02(*f) * a * a;
	J11(*g) = J20(*f) * 2 * a * b + J11(*f) * (b * b - a * a) -
			J02(*f) * 2 * a * b;
	J02(*g) = J20(*f) * a * a + J11(*f) * a * b + J02(*f) * b * b;
}




void
compos_jet2( jet2 *F, jmap22 *Q, jet2 *P )
{
jet2	*f,	f1;
	if( J00(Q->f1) != 0 || J00(Q->f2) != 0 ){
		shift_jet2( F , J00(Q->f1), J00(Q->f2), &f1 );
		f = &f1;
	}
	else	f = F;

	J00(*P) = J00(*f);


	J10(*P) = J10(*f)*J10(Q->f1) + J01(*f)*J10(Q->f2);

	J01(*P) = J10(*f)*J01(Q->f1) + J01(*f)*J01(Q->f2);


	J20(*P) = J20(*f)*J10(Q->f1)*J10(Q->f1) +
		J11(*f)*J10(Q->f1)*J10(Q->f2) +
		J02(*f)*J10(Q->f2)*J10(Q->f2) + J10(*f)*J20(Q->f1) +
		J01(*f)*J20(Q->f2) ;

	J11(*P) = (  ( 2*J20(*f)*J10(Q->f1) +
			J11(*f)*J10(Q->f2) )* J01(Q->f1) + 
		    ( J11(*f)*J10(Q->f1) + 2*J02(*f)*J10(Q->f2) )* J01(Q->f2) + 
		      J10(*f)*J11(Q->f1) + J01(*f)*J11( Q->f2 ) )/1.0;

	J02(*P) = J20(*f)*J01(Q->f1)*J01(Q->f1) +
		J11(*f)*J01(Q->f1)*J01(Q->f2) +
		J02(*f)*J01(Q->f2)*J01(Q->f2) + J10(*f)*J02(Q->f1) +
		J01(*f)*J02(Q->f2) ;
}


void
inverse_jmap22( jmap22 *F, jmap22 *G )
{
float	detf;
float	L111,	L112,	L121,	L122,	L211,	L212,	L221,	L222;
float	M111,	M112,	M121,	M122,	M211,	M212,	M221,	M222;

	J00(G->f1) = 0;
	J00(G->f2) = 0;


	detf = J10(F->f1) * J01(F->f2) - J01(F->f1)*J10(F->f2);
	J10(G->f1) = J01(F->f2) / detf;
	J01(G->f1) = -J01(F->f1) / detf;

	J10(G->f2) = -J10(F->f2) / detf;
	J01(G->f2) = J10(F->f1) / detf;



	/*	rank	2	*/
	L111 =    2*( J10(G->f1) * J20(F->f1) + J01(G->f1) * J20(F->f2) );
	L112 = L211 = J10(G->f1) * J11(F->f1) + J01(G->f1) * J11(F->f2) ;
	L212 =    2*( J10(G->f1) * J02(F->f1) + J01(G->f1) * J02(F->f2) );

	L121 =	  2*( J10(G->f2) * J20(F->f1) + J01(G->f2) * J20(F->f2) );
	L122 = L221 = J10(G->f2) * J11(F->f1) + J01(G->f2) * J11(F->f2) ;
	L222 =    2* (J10(G->f2) * J02(F->f1) + J01(G->f2) * J02(F->f2) );


	M111 = L111 * J10(G->f1) + L112 * J10(G->f2) ;
	M112 = L111 * J01(G->f1) + L112 * J01(G->f2) ;

	M121 = L121 * J10(G->f1) + L122 * J10(G->f2) ;
	M122 = L121 * J01(G->f1) + L122 * J01(G->f2) ;

	M211 = L211 * J10(G->f1) + L212 * J10(G->f2) ;
	M212 = L211 * J01(G->f1) + L212 * J01(G->f2) ;

	M221 = L221 * J10(G->f1) + L222 * J10(G->f2) ;
	M222 = L221 * J01(G->f1) + L222 * J01(G->f2) ;



	J20(G->f1) = 0.5*( - M111 * J10(G->f1) - M211 * J10(G->f2) );
	J11(G->f1) =       - M111 * J01(G->f1) - M211 * J01(G->f2) ;
	J02(G->f1) = 0.5*( - M112 * J01(G->f1) - M212 * J01(G->f2) );

	J20(G->f2) = 0.5*( - M121 * J10(G->f1) - M221 * J10(G->f2) );
	J11(G->f2) =       - M121 * J01(G->f1) - M221 * J01(G->f2) ;
	J02(G->f2) = 0.5*( - M122 * J01(G->f1) - M222 * J01(G->f2) );
}


void
normal_curve_jet2( jet2 *F, jmap22 *Q )
{
float	gradf,	a,	b;
	Q->f2 = *F;

/*	rank  0   */
	J00(Q->f1) = 0;

/*	rank  1   */
	gradf = sqrt( J10(*F)*J10(*F) + J01(*F)*J01(*F) );

	J10(Q->f1) = -J01(*F) / gradf;
	J01(Q->f1) = J10(*F) / gradf;

/*	rank  2   */
	a = -( J10(Q->f1)*2*J20(*F) + J01(Q->f1)*J11(*F) ) ;
	b = -( J10(Q->f1)*J11(*F) + J01(Q->f1)*2*J02(*F) ) ;

	J20(Q->f1) = 0.5*( J01(Q->f1)*a + J10(Q->f1)*b )/gradf;
	J11(Q->f1) = ( -J10(Q->f1)*a + J01(Q->f1)*b )/gradf;
	J02(Q->f1) = -J20(Q->f1);
}




int
env_stat_jmap22( jmap22 *F, float h, float *xs, float *ys )
{
jet2	jf;
float	grad_jf;

	J00( jf ) = J10(F->f1) * J01(F->f2)  -  J10(F->f2) * J01(F->f1);

	J10( jf ) = 2*( J20(F->f1) * J01(F->f2)  -  J20(F->f2) * J01(F->f1) ) +
		      ( J10(F->f1) * J11(F->f2)  -  J10(F->f2) * J11(F->f1) );

	J01( jf ) =   ( J11(F->f1) * J01(F->f2)  -  J11(F->f2) * J01(F->f1) ) +
		    2*( J10(F->f1) * J02(F->f2)  -  J10(F->f2) * J02(F->f1) );

	grad_jf = sqrt( J10(jf)*J10(jf) + J01(jf)*J01(jf) );

	if( ABS( J00(jf) ) >= h * grad_jf )	return( REGULAR_ENV );

	*xs = -J00(jf)* J10(jf)/grad_jf;
	*ys = -J00(jf)* J01(jf)/grad_jf;
	return( SINGULAR_ENV );
}


void
derivative_x_jet2( jet2 *f, jet2 *fx )
{
	
	J00( *fx ) = J10( *f );

	J10( *fx ) = 2*J20(*f);
	J01( *fx ) = J11(*f);

	J20( *fx ) = 0;
	J11( *fx ) = 0;
	J02( *fx ) = 0;
}

void
derivative_y_jet2( jet2 *f, jet2 *fy )
{
	J00( *fy ) = J01( *f );

	J10( *fy ) = J11(*f);
	J01( *fy ) = 2*J02(*f);

	J20( *fy ) = 0;
	J11( *fy ) = 0;
	J02( *fy ) = 0;
}


/* compute the differnce element for jet2   for dx = dy = 1	*/
void
difference_element_jet2( jet2 *f, float x, float y,
				float dx0[], float dx1[], float dx2[] )
{

	dx0[0] = value_jet2( f, JET_ORDER, x, y );
	dx0[1] = J01(*f) + J11(*f)*x + J02(*f)*( 1 + 2*y );
	dx0[2] = 2*J02(*f);

	dx1[0] = J10(*f) + J11(*f)*y + J20(*f)*( 1 + 2*x );
	dx1[1] = J11(*f);

	dx2[0] = 2*J20(*f);
}


int 
is_zero_jet2(jet2 f )
{
	if( J00(f)==0 &&
	    J10(f)==0 && J01(f)==0 &&
	    J20(f)==0 && J11(f)==0 && J02(f)==0  )
		return( 1 );
	return( 0 );
}


float
diff_jet2( jet2 *g, jet2 *f )
{
float	max,	tmp;
int	i;
	if ( COEFF_NO > 0 )
		max = g->j[0] - f->j[0];// initial value;
	for( i = 0 ; i < COEFF_NO ; i++ ){
		tmp = g->j[i] - f->j[i];

		if( tmp < 0 )	tmp = -tmp;

		if( max < tmp )	max = tmp;
	}
	
	return( max );
}


void
max_coeff_jet2( f, dgree, max )
jet2	f;
int	dgree;
float	max[JET_ORDER+1];
{
float	coeff;
int	i;

	for( i = 0 ; i <= dgree ; i++ )	max[i] = 0;

	for( i = 0 ; i < Coeff_no[dgree] ; i++ ){

		if( f.j[i] < 0 )	coeff = -f.j[i];
		else	coeff = f.j[i];

		if( coeff > max[Coeff_dgree[i]] )
			max[Coeff_dgree[i]] = coeff;
	}
}


