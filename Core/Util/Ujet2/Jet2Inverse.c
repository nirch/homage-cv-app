
/****************************************************************
*								*
*		Module:  Jet2Inverse.c				*
*								*
****************************************************************/
#include	<stdio.h>
#include	<math.h>

#include	"Jet2Type.h"


void
inverse_jmap22_1( jmap22 *F, jmap22 *G )
{
jmap22	G0;
float	A,	B,	C,	D,	detf;

	detf = J10(F->f1) * J01(F->f2) - J01(F->f1) * J10(F->f2);

	J00(G->f1) = 0;
	J00(G->f2) = 0;

	A = J10(G->f1) = J01(F->f2) / detf;
	B = J01(G->f1) = -J01(F->f1) / detf;

	C = J10(G->f2) = -J10(F->f2) / detf;
	D = J01(G->f2) = J10(F->f1) / detf;



	/* Compute G0 */
	J20(G0.f1) = A * J20(F->f1) + B * J20(F->f2);
	J11(G0.f1) = A * J11(F->f1) + B * J11(F->f2);
	J02(G0.f1) = A * J02(F->f1) + B * J02(F->f2);

	J20(G0.f2) = C * J20(F->f1) + D * J20(F->f2);
	J11(G0.f2) = C * J11(F->f1) + D * J11(F->f2);
	J02(G0.f2) = C * J02(F->f1) + D * J02(F->f2);




	J20(G->f1) = -( J20(G0.f1)*A*A + J11(G0.f1)*A*C + J02(G0.f1)*C*C);
	J11(G->f1) = -( J20(G0.f1)*2*A*B + J11(G0.f1)*(A*D+C*B) +
						J02(G0.f1)*2*C*D);
	J02(G->f1) = -( J20(G0.f1)*B*B + J11(G0.f1)*B*D + J02(G0.f1)*D*D);


	J20(G->f2) = -( J20(G0.f2)*A*A + J11(G0.f2)*A*C + J02(G0.f2)*C*C);
	J11(G->f2) = -( J20(G0.f2)*2*A*B + J11(G0.f2)*(A*D+C*B) +
						J02(G0.f2)*2*C*D);
	J02(G->f2) = -( J20(G0.f2)*B*B + J11(G0.f2)*B*D + J02(G0.f2)*D*D);
}


void
jacobian_jmap22( jmap22 *F, jet2 *Jac ) /* determinant of Jacobi matrix */
{

	J00( *Jac ) =   J10( F->f1 )*J01( F->f2 ) - J01( F->f1 )*J10(F->f2); 

	J10( *Jac ) =	2*J20(F->f1)*J01(F->f2) + J10(F->f1)*J11(F->f2) -
			2*J01(F->f1)*J20(F->f2) - J11(F->f1)*J10( F->f2);

	J01( *Jac ) =	2*J10(F->f1)*J02(F->f2) + J11(F->f1)*J01(F->f2) -
			2*J02(F->f1)*J10(F->f2) - J01(F->f1)*J11(F->f2);


	J20( *Jac ) =	2*J20(F->f1)*J11(F->f2) - 2*J11(F->f1)*J20(F->f2);
	J11( *Jac ) =	4*J20(F->f1)*J02(F->f2) - 4*J02(F->f1)*J20(F->f2);
	J02( *Jac ) =	2*J11(F->f1)*J02(F->f2) - 2*J02(F->f1)*J11(F->f2);
}
