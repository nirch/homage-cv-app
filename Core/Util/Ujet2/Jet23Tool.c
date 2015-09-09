/****************************************************************
*								*
*		Module:  jet2tool.c              		*
*		jets of order 3 of 2 variables.  		*
****************************************************************/
#include	<math.h>
#include	"Uigp/igp.h"

#include        "Jet23Type.h"

static jet23   ZERO_JET3_2 = { 0.0,    0.0, 0.0,    0.0, 0.0, 0.0,
                                             0.0, 0.0, 0.0, 0.0 };

void
zero_jet23( jet23 *f )
{
	*f = ZERO_JET3_2;
}

void
value_jmap23( jmap23 *F, vec2d *p, vec2d *val )
{
        val->x = value_jet2( (jet2 *)&F->f1, 3, p->x, p->y );
        val->y = value_jet2( (jet2 *)&F->f2, 3, p->x, p->y );
}


void
zero_jmap23( jmap23 *F )
{
	F->f1 = ZERO_JET3_2;
	F->f2 = ZERO_JET3_2;
}

void
identity_jmap23( jmap23 *F )
{
	F->f1 = ZERO_JET3_2;
	F->f2 = ZERO_JET3_2;
	J10( F->f1 ) = 1.0;
	J01( F->f2 ) = 1.0;
}




void
scalar_product_jet23( jet23 *g, float s, jet23 *F )
			/* compute the jet of s*g(x,y) at  (0,0)  */
{

	/* rank 0		 */
	J00(*F) = J00(*g) * s;

	/* rank 1		 */
	J10(*F) = J10(*g) * s;
	J01(*F) = J01(*g) * s;

	/* rank 2		 */
	J20(*F) = J20(*g) * s;
	J11(*F) = J11(*g) * s;
	J02(*F) = J02(*g) * s;

	/* rank 3		 */
	J30(*F) = J30(*g) * s;
	J21(*F) = J21(*g) * s;
	J12(*F) = J12(*g) * s;
	J03(*F) = J03(*g) * s;
}

void
sum_jet23( jet23 *g, jet23 *f, jet23 *F )	
			/* compute the jet of g(x,y)+f(x,y)  at (0,0)	 */
{

	/* rank 0		 */
	J00(*F) = J00(*g) + J00(*f);

	/* rank 1		 */
	J10(*F) = J10(*g) + J10(*f);
	J01(*F) = J01(*g) + J01(*f);

	/* rank 2		 */
	J20(*F) = J20(*g) + J20(*f);
	J11(*F) = J11(*g) + J11(*f);
	J02(*F) = J02(*g) + J02(*f);

	/* rank 3		 */
	J30(*F) = J30(*g) + J30(*f);
	J21(*F) = J21(*g) + J21(*f);
	J12(*F) = J12(*g) + J12(*f);
	J03(*F) = J03(*g) + J03(*f);
}

void
sub_jet23( jet23 *g,  jet23 *f, jet23 *F )
	/* compute the jet of g(x,y)-f(x,y)  at (0,0)	 */
{

	/* rank 0		 */
	J00(*F) = J00(*g) - J00(*f);

	/* rank 1		 */
	J10(*F) = J10(*g) - J10(*f);
	J01(*F) = J01(*g) - J01(*f);

	/* rank 2		 */
	J20(*F) = J20(*g) - J20(*f);
	J11(*F) = J11(*g) - J11(*f);
	J02(*F) = J02(*g) - J02(*f);

	/* rank 3		 */
	J30(*F) = J30(*g) - J30(*f);
	J21(*F) = J21(*g) - J21(*f);
	J12(*F) = J12(*g) - J12(*f);
	J03(*F) = J03(*g) - J03(*f);
}


void
shift_jet23( jet23 *f, float x, float y, jet23 *g )
{

	J00(*g) = J00(*f) + J10(*f) * x + J01(*f) * y +
		J20(*f) * x * x + J11(*f) * x * y + J02(*f) * y * y +
		J30(*f) * x * x * x + J21(*f) * x * x * y +
		J12(*f) * x * y * y + J03(*f) * y * y * y;

	J10(*g) = J10(*f) +
		2 * J20(*f) * x + J11(*f) * y +
		3 * J30(*f) * x * x + 2 * J21(*f) * x * y + J12(*f) * y * y;

	J01(*g) = J01(*f) +
		J11(*f) * x + 2 * J02(*f) * y +
		J21(*f) * x * x + 2 * J12(*f) * x * y + 3 * J03(*f) * y * y;


	J20(*g) = J20(*f) + 3 * J30(*f) * x + J21(*f) * y;

	J11(*g) = J11(*f) + 2 * J21(*f) * x + 2 * J12(*f) * y;

	J02(*g) = J02(*f) + J12(*f) * x + 3 * J03(*f) * y;

	J30(*g) = J30(*f);
	J21(*g) = J21(*f);
	J12(*g) = J12(*f);
	J03(*g) = J03(*f);
}


void
product_jet23( jet23 *g, jet23 *f, jet23 *F )
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

	/*	rank 3		*/
	J30(*F) = J30(*g)*J00(*f) + J20(*g)*J10(*f) +
				J10(*g)*J20(*f) + J00(*g)*J30(*f);

	J21(*F) = J21(*g)*J00(*f) + J20(*g)*J01(*f) + J11(*g)*J10(*f) +
		  J00(*g)*J21(*f) + J01(*g)*J20(*f) + J10(*g)*J11(*f);

	J12(*F) = J12(*g)*J00(*f) + J02(*g)*J10(*f) + J11(*g)*J01(*f) +
		  J00(*g)*J12(*f) + J10(*g)*J02(*f) + J01(*g)*J11(*f);

	J03(*F) = J03(*g)*J00(*f) + J02(*g)*J01(*f) +
				J01(*g)*J02(*f) + J00(*g)*J03(*f);
}


void
productAsum_jet23( jet23 *g, jet23 *f, jet23 *F )
			/* compute the jet of g*f  at (0,0)	*/
{

	/*	rank 0		*/
	J00(*F) += J00(*g)*J00(*f);

	/*	rank 1		*/
	J10(*F) += J10(*g)*J00(*f) + J00(*g)*J10(*f);
	J01(*F) += J01(*g)*J00(*f) + J00(*g)*J01(*f);

	/*	rank 2		*/
	J20(*F) += J20(*g)*J00(*f) + J10(*g)*J10(*f) + J00(*g)*J20(*f);
	J11(*F) += J11(*g)*J00(*f) + J10(*g)*J01(*f) +
					J01(*g)*J10(*f) + J00(*g)*J11(*f);
	J02(*F) += J02(*g)*J00(*f) + J01(*g)*J01(*f) + J00(*g)*J02(*f);

	/*	rank 3		*/
	J30(*F) += J30(*g)*J00(*f) + J20(*g)*J10(*f) +
				J10(*g)*J20(*f) + J00(*g)*J30(*f);

	J21(*F) += J21(*g)*J00(*f) + J20(*g)*J01(*f) + J11(*g)*J10(*f) +
		  J00(*g)*J21(*f) + J01(*g)*J20(*f) + J10(*g)*J11(*f);

	J12(*F) += J12(*g)*J00(*f) + J02(*g)*J10(*f) + J11(*g)*J01(*f) +
		  J00(*g)*J12(*f) + J10(*g)*J02(*f) + J01(*g)*J11(*f);

	J03(*F) += J03(*g)*J00(*f) + J02(*g)*J01(*f) +
				J01(*g)*J02(*f) + J00(*g)*J03(*f);
}

void
productACsum_jet23( jet23 *g, jet23 *f, float a, jet23 *F )
			/* compute the jet of g*f  at (0,0)	*/
{

	/*	rank 0		*/
	J00(*F) += a*(J00(*g)*J00(*f));

	/*	rank 1		*/
	J10(*F) += a*(J10(*g)*J00(*f) + J00(*g)*J10(*f) );
	J01(*F) += a*(J01(*g)*J00(*f) + J00(*g)*J01(*f) );
 
	/*	rank 2		*/
	J20(*F) += a*(J20(*g)*J00(*f) + J10(*g)*J10(*f) + J00(*g)*J20(*f));
	J11(*F) += a*(J11(*g)*J00(*f) + J10(*g)*J01(*f) +
					J01(*g)*J10(*f) + J00(*g)*J11(*f));
	J02(*F) += a*(J02(*g)*J00(*f) + J01(*g)*J01(*f) + J00(*g)*J02(*f));

	/*	rank 3		*/
	J30(*F) += a*(J30(*g)*J00(*f) + J20(*g)*J10(*f) +
				J10(*g)*J20(*f) + J00(*g)*J30(*f));

	J21(*F) += a*(J21(*g)*J00(*f) + J20(*g)*J01(*f) + J11(*g)*J10(*f) +
		  J00(*g)*J21(*f) + J01(*g)*J20(*f) + J10(*g)*J11(*f));

	J12(*F) += a*(J12(*g)*J00(*f) + J02(*g)*J10(*f) + J11(*g)*J01(*f) +
		  J00(*g)*J12(*f) + J10(*g)*J02(*f) + J01(*g)*J11(*f));

	J03(*F) += a*(J03(*g)*J00(*f) + J02(*g)*J01(*f) +
				J01(*g)*J02(*f) + J00(*g)*J03(*f));
}


void
divide_jet23( jet23 *g, jet23 *F )	/* compute jet2 of  1/(g(x,y)  at (0,0)  */
{
float	a,	a2,	a3;
	/*	rank	0	*/
	a = J00(*F) = 1.0 / J00(*g);

	/*	rank	1	*/
	a2 = a*J00(*F);
	J10(*F) = - J10(*g) * a2;
	J01(*F) = - J01(*g) * a2;

	/*	rank	2	*/
	a3 = a2*J00(*F);
	
	J20(*F) = (   J10(*g)*J10(*g) - J20(*g)*J00(*g) ) *a3;
	J11(*F) = ( 2*J10(*g)*J01(*g) - J11(*g)*J00(*g) ) *a3;
	J02(*F) = (   J01(*g)*J01(*g) - J02(*g)*J00(*g) ) *a3;



	J30(*F) = (-J10(*g)*J10(*g)*J10(*g)* a2 +
			2*J10(*g)*J20(*g)*a -
			J30(*g) )*a2;

	J21(*F) = ( -3*J01(*g)*J10(*g)*J10(*g)* a2 +
			2*( J10(*g)*J11(*g) + J01(*g)*J20(*g))*a -
			J21(*g) ) * a2;

	J12(*F) = ( -3*J10(*g)*J01(*g)*J01(*g)* a2 +
			2*( J01(*g)*J11(*g) + J10(*g)*J02(*g))*a -
			J12(*g) ) * a2;

	J03(*F) = (-J01(*g)*J01(*g)*J01(*g)* a2+
			2*J01(*g)*J02(*g)*a -
			J03(*g))*a2;
}



void   
rescaling_jet23( jet23 *f, float hx, float hy, jet23 *g )
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


	/*	rank 3		*/
	J30(*g) = J30(*f) * hx*hx*hx;
	J21(*g) = J21(*f) *    hx*hx*hy;
	J12(*g) = J12(*f) *       hx*hy*hy;
	J03(*g) = J03(*f) *          hy*hy*hy;
}






/* rotate the domian coordinet such that the new Y axis is vector v */
void
rotate_jet23( jet23 *f, vec2d *v, jet23 *g )
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


	J30(*g) = J30(*f) * b * b * b - J21(*f) * a * b * b +
		J12(*f) * a * a * b - J03(*f) * a * a * a;

	J21(*g) = J30(*f) * 3 * a * b * b + J21(*f) * (-2 * a * a + b * b) * b +
		J12(*f) * (-2 * b * b + a * a) * a + J03(*f) * 3 * a * a * b;

	J12(*g) = J30(*f) * 3 * a * a * b + J21(*f) * (-a * a + 2 * b * b) * a +
		J12(*f) * (-2 * a * a + b * b) * b - J03(*f) * 3 * a * b * b;

	J03(*g) = J30(*f) * a * a * a + J21(*f) * a * a * b +
		J12(*f) * a * b * b + J03(*f) * b * b * b;
}


void
derivative_x_jet23( jet23 *f, jet23 *fx )
{
	
	J00( *fx ) = J10( *f );

	J10( *fx ) = 2*J20(*f);
	J01( *fx ) = J11(*f);

	J20( *fx ) = 3*J30(*f);
	J11( *fx ) = 2*J21(*f);
	J02( *fx ) = J12(*f);

	J30( *fx ) = 0.0;
	J21( *fx ) = 0.0;
	J12( *fx ) = 0.0;
	J03( *fx ) = 0.0;
}

void
derivative_y_jet23( jet23 *f, jet23 *fy )
{
	J00( *fy ) = J01( *f );

	J10( *fy ) = J11(*f);
	J01( *fy ) = 2*J02(*f);

	J20( *fy ) = J21(*f);
	J11( *fy ) = 2*J12(*f);
	J02( *fy ) = 3*J03(*f);

	J30( *fy ) = 0.0;
	J21( *fy ) = 0.0;
	J12( *fy ) = 0.0;
	J03( *fy ) = 0.0;
}



void
compos_jet23( jet23 *F, jmap23 *Q, jet23 *P )
{
jet23	af,	*f;

	if (J00(Q->f1) != 0 || J00(Q->f2) != 0){
		shift_jet23( F, J00(Q->f1), J00(Q->f2), &af );
		f = &af;
	}
	else	f = F;

	J00(*P) = J00(*f);


	J10(*P) = J10(*f) * J10(Q->f1) + J01(*f) * J10(Q->f2);


	J01(*P) = J10(*f) * J01(Q->f1) + J01(*f) * J01(Q->f2);


	J20(*P) = J20(*f) * J10(Q->f1) * J10(Q->f1) +
		J11(*f) * J10(Q->f1) * J10(Q->f2) +
		J02(*f) * J10(Q->f2) * J10(Q->f2) + J10(*f) * J20(Q->f1) +
		J01(*f) * J20(Q->f2);

	J11(*P) = ((2 * J20(*f) * J10(Q->f1) + J11(*f) * J10(Q->f2))*J01(Q->f1) +
		  (J11(*f) * J10(Q->f1) + 2 * J02(*f) * J10(Q->f2)) * J01(Q->f2) +
		  J10(*f) * J11(Q->f1) + J01(*f) * J11(Q->f2));

	J02(*P) = J20(*f) * J01(Q->f1) * J01(Q->f1) +
		J11(*f) * J01(Q->f1) * J01(Q->f2) +
		J02(*f) * J01(Q->f2) * J01(Q->f2) + J10(*f) * J02(Q->f1) +
		J01(*f) * J02(Q->f2);

	
	J30(*P) = J10(*f)*J30(Q->f1) + J01(*f)*J30(Q->f2) +
		2*J20(*f)*J10(Q->f1)*J20(Q->f1) +
		J11(*f)*( J10(Q->f1)*J20(Q->f2) +
		J20(Q->f1)*J10(Q->f2) ) + 2*J02(*f)*J10(Q->f2)*J20(Q->f2) +
		J30(*f)*J10(Q->f1)*J10(Q->f1)*J10(Q->f1) +
		J21(*f)*J10(Q->f1)*J10(Q->f1)*J10(Q->f2) +
		J12(*f)*J10(Q->f1)*J10(Q->f2)*J10(Q->f2) +
		J03(*f)*J10(Q->f2)*J10(Q->f2)*J10(Q->f2) ;
	


	J21(*P) = J10(*f) * J21(Q->f1) + J01(*f) * J21(Q->f2) +
		J20(*f) * 2 * (J10(Q->f1) * J11(Q->f1) +J01(Q->f1)*J20(Q->f1)) +
		J11(*f) * (J10(Q->f1) * J11(Q->f2) + J01(Q->f1) * J20(Q->f2)
			  + J20(Q->f1) * J01(Q->f2) + J11(Q->f1) * J10(Q->f2)) +
		J02(*f) * 2 * (J10(Q->f2) * J11(Q->f2) +J01(Q->f2) *J20(Q->f2))+
		J30(*f) * 3 * J10(Q->f1) * J10(Q->f1) * J01(Q->f1) +
		J21(*f) * (2 * J10(Q->f1) * J01(Q->f1) * J10(Q->f2) +
			  J10(Q->f1) * J10(Q->f1) * J01(Q->f2)) +
		J12(*f) * (2 * J10(Q->f1) * J10(Q->f2) * J01(Q->f2) +
			  J01(Q->f1) * J10(Q->f2) * J10(Q->f2)) +
		J03(*f) * 3 * J10(Q->f2) * J10(Q->f2) * J01(Q->f2);



	J12(*P) = J10(*f) * J12(Q->f1) + J01(*f) * J12(Q->f2) +
		J20(*f) * 2 * (J10(Q->f1) * J02(Q->f1) +J01(Q->f1)*J11(Q->f1)) +
		J11(*f) * (J10(Q->f1) * J02(Q->f2) + J01(Q->f1) * J11(Q->f2)
			  + J11(Q->f1) * J01(Q->f2) + J02(Q->f1) * J10(Q->f2)) +
		J02(*f) * 2 * (J10(Q->f2) * J02(Q->f2) +J01(Q->f2) *J11(Q->f2))+
		J30(*f) * 3 * J10(Q->f1) * J01(Q->f1) * J01(Q->f1) +
		J21(*f) * (2 * J10(Q->f1) * J01(Q->f1) * J01(Q->f2) +
			  J01(Q->f1) * J01(Q->f1) * J10(Q->f2)) +
		J12(*f) * (J10(Q->f1) * J01(Q->f2) * J01(Q->f2) +
			  2 * J01(Q->f1) * J10(Q->f2) * J01(Q->f2)) +
		J03(*f) * 3 * J10(Q->f2) * J01(Q->f2) * J01(Q->f2);



	J03(*P) = J10(*f) * J03(Q->f1) + J01(*f) * J03(Q->f2) +
		2 * J20(*f) * J01(Q->f1) * J02(Q->f1) +
		J11(*f) * (J01(Q->f1) * J02(Q->f2) + J02(Q->f1) * J01(Q->f2)) +
		2 * J02(*f) * J01(Q->f2) * J02(Q->f2) +
		J30(*f) * J01(Q->f1) * J01(Q->f1) * J01(Q->f1) +
		J21(*f) * J01(Q->f1) * J01(Q->f1) * J01(Q->f2) +
		J12(*f) * J01(Q->f1) * J01(Q->f2) * J01(Q->f2) +
		J03(*f) * J01(Q->f2) * J01(Q->f2) * J01(Q->f2);

}



void
jacobian_jmap23( jmap23 *F, jet2 *Jac ) /* determinant of Jacobi matrix */
{

	J00( *Jac ) =   J10( F->f1 )*J01( F->f2 ) - J01( F->f1 )*J10(F->f2); 

	J10( *Jac ) =	2*J20(F->f1)*J01(F->f2) + J10(F->f1)*J11(F->f2) -
			2*J01(F->f1)*J20(F->f2) - J11(F->f1)*J10( F->f2);

	J01( *Jac ) =	2*J10(F->f1)*J02(F->f2) + J11(F->f1)*J01(F->f2) -
			2*J02(F->f1)*J10(F->f2) - J01(F->f1)*J11(F->f2);


	J20( *Jac ) = 3*J30(F->f1)*J01(F->f2) + 2*J20(F->f1)*J11(F->f2) +
			J10(F->f1)*J21(F->f2) -
			J21(F->f1)*J10(F->f2) - 2*J11(F->f1)*J20(F->f2) -
			3*J01(F->f1)*J30(F->f2);

	J11( *Jac ) = 2*J21(F->f1)*J01(F->f2) + 4*J20(F->f1)*J02(F->f2) +
		2*J10(F->f1)*J12(F->f2) -
		2*J12(F->f1)*J10(F->f2)  - 4*J02(F->f1)*J20(F->f2) -
		2*J01(F->f1)*J21(F->f2);


	J02( *Jac ) = J12(F->f1)*J01(F->f2) + 2*J11(F->f1)*J02(F->f2) +
			3*J10(F->f1)*J03(F->f2) -
			3*J03(F->f1)*J10(F->f2) - 2*J02(F->f1)*J11(F->f2) -
			J01(F->f1)*J12(F->f2);
			
/*
	J20( *Jac ) = 2*J20(F->f1)*J11(F->f2) - 2*J11(F->f1)*J20(F->f2) +
                   J10(F->f1)*J21(F->f2) + 3*J30(F->f1)*J01(F->f2) -
                   J21(F->f1)*J10(F->f2) - 3*J01(F->f1)*J30(F->f2);
*/

/*
	J11( *Jac ) = 4*J20(F->f1)*J02(F->f2) - 4*J02(F->f1)*J20(F->f2) +
                 2*J10(F->f1)*J12(F->f2) + 2*J21(F->f1)*J01(F->f2) -
                 2*J01(F->f1)*J21(F->f2) - 2*J12(F->f1)*J10(F->f2);
*/

/*
	J02( *Jac ) = 2*J11(F->f1)*J02(F->f2) - 2*J02(F->f1)*J11(F->f2) +
                 3*J10(F->f1)*J03(F->f2) + J12(F->f1)*J01(F->f2) -
                 3*J03(F->f1)*J10(F->f2) - J01(F->f1)*J12(F->f2);
*/
}


void                    /* coordinate transformation */
substit_jmap23( jmap23 *F, jmap23 *Q, jmap23 *FQ )
{
	compos_jet23( &F->f1, Q, &FQ->f1 );
	compos_jet23( &F->f2, Q, &FQ->f2 );
}

float
diff_jet23( jet23 *g, jet23 *f )
{
float	max,	tmp;
int	i;

	max = 0;

	for( i = 0 ; i < COEFF_NO3 ; i++ ){
		tmp = g->j[i] - f->j[i];

		if( tmp < 0 )	tmp = -tmp;

		if( max < tmp )	max = tmp;
	}
	
	return( max );
}
