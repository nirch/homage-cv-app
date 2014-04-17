/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/*****************************
***   Module:  jettype.h   ***
******************************/

#ifndef         JET1D_TYPE
#define         JET1D_TYPE

#include	<stdio.h>

#define		JET1_ORDER	3
#define		JET1_COEFF_NO	( JET1_ORDER + 1)


typedef	struct	jet1_type	{
	union {
		struct {
			float	a0;
			float	a1;
			float	a2;
			float	a3;
		};

		float j[ JET1_COEFF_NO ];
	};

} jet1_type;



static jet1_type	ZERO_JET1 = { 0.0, 0.0, 0.0, 0.0 };



#define		J0( f )		(f).j[0]
#define		J1( f )		(f).j[1]
#define		J2( f )		(f).j[2]
#define		J3( f )		(f).j[3]
#define		J4( f )		(f).j[4]



	/* Jet1dTool.c */

#define	JET1_2_VALUE( f, x )	( J0(f) + x* ( J1(f) + x* J2(f) )  )
#define	JET1_3_VALUE( f, x )	( J0(f) +x*( J1(f) +x*( J2(f) +x*J3(f) ) ) )

#define		JET1_ZERO( f )		{ \
				(f).a0 = 0; \
				(f).a1 = 0; \
				(f).a2 = 0; \
				(f).a3 = 0; \
}


float		jet1_value( jet1_type *f, int dgree, float x );

void	print_jet1( FILE *fp, int dgree, jet1_type *f );

void	derivative_jet1( jet1_type *f, jet1_type *d );


#endif
