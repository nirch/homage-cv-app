/****************************************************************
*								*
*		Module:  InverseJet22_3.c              		*
*		jets of order 3 of 2 variables.  		*
****************************************************************/
#include	<math.h>

#include        "Jet23Type.h"

/*
 * inversion jet at  ( J00(F.f1), J00(F.f2) ); under assumptions:
 * determinant of the linear part is far from zero;
 */

void
inverse_jmap23( jmap23 *F, jmap23 *g )
{
jmap23	t,	r;
	float   a11, a12, a21, a22, detf;


	zero_jmap23( g );

	detf = J10(F->f1) * J01(F->f2) - J01(F->f1) * J10(F->f2);

	a11 = J10(g->f1) = J01(F->f2) / detf;
	a12 = J01(g->f1) = -J01(F->f1) / detf;

	a21 = J10(g->f2) = -J10(F->f2) / detf;
	a22 = J01(g->f2) = J10(F->f1) / detf;

	/* rank    2       */
	zero_jmap23( &t );
	J20(t.f1) = J20(F->f1);
	J11(t.f1) = J11(F->f1);
	J02(t.f1) = J02(F->f1);
	J20(t.f2) = J20(F->f2);
	J11(t.f2) = J11(F->f2);
	J02(t.f2) = J02(F->f2);

	substit_jmap23( &t, g, &r );

	J20(g->f1) = -a11 * J20(r.f1) - a12 * J20(r.f2);
	J20(g->f2) = -a21 * J20(r.f1) - a22 * J20(r.f2);

	J11(g->f1) = -a11 * J11(r.f1) - a12 * J11(r.f2);
	J11(g->f2) = -a21 * J11(r.f1) - a22 * J11(r.f2);

	J02(g->f1) = -a11 * J02(r.f1) - a12 * J02(r.f2);
	J02(g->f2) = -a21 * J02(r.f1) - a22 * J02(r.f2);

	/* rank    3       */
	J30(t.f1) = J30(F->f1);
	J21(t.f1) = J21(F->f1);
	J12(t.f1) = J12(F->f1);
	J03(t.f1) = J03(F->f1);
	J30(t.f2) = J30(F->f2);
	J21(t.f2) = J21(F->f2);
	J12(t.f2) = J12(F->f2);
	J03(t.f2) = J03(F->f2);

	substit_jmap23( &t, g, &r );

	J30(g->f1) = -a11 * J30(r.f1) - a12 * J30(r.f2);
	J30(g->f2) = -a21 * J30(r.f1) - a22 * J30(r.f2);

	J21(g->f1) = -a11 * J21(r.f1) - a12 * J21(r.f2);
	J21(g->f2) = -a21 * J21(r.f1) - a22 * J21(r.f2);

	J12(g->f1) = -a11 * J12(r.f1) - a12 * J12(r.f2);
	J12(g->f2) = -a21 * J12(r.f1) - a22 * J12(r.f2);

	J03(g->f1) = -a11 * J03(r.f1) - a12 * J03(r.f2);
	J03(g->f2) = -a21 * J03(r.f1) - a22 * J03(r.f2);
}
