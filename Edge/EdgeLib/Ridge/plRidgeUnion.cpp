 //  Defines the entry point for the console application.
//
#include	<string.h>
#include	<math.h>

//#ifdef _DEBUG
#define _DUMP
//#endif

#include "Uln/PlnType.h"
#include "plRidgeType.h"


static int	plnA_union_1( plRidgeA_type *ar, int i0, float dT, float aT, int direct );


int
plRidgeA_union( plRidgeA_type *ar, float dLen, float dT, float aT )
{
int	i;

	for( i = 0 ; i < ar->nR ; i++ ){
		plRidge_type *r = ar->r[i];
		if( r->pl == NULL || r->pl->link == NULL )
			continue;

		if( r->pl->len < dLen )
			continue;

		while( plnA_union_1( ar, i, dT, aT, 1  ) > 0 );

		if( plnA_union_1( ar, i, dT, aT, 0  ) > 0 )
			while( plnA_union_1( ar, i, dT, aT, 1  ) > 0 );




	}


	plRidgeA_decrease( ar );

	return( 1 );
}


static int
plnA_union_1( plRidgeA_type *ar, int i0, float dT, float aT, int direct )
{
	int	i;
	vec2f_type	p0,	v0,	p1,	v1,	p;
	float	t,	d;

	pln_type *bpl;

	bpl = ar->r[i0]->pl;



	if( direct == 1 )
		pln_tangetP( bpl, bpl->len, &p0, &v0 );
	else	pln_tangetP( bpl, 0, &p0, &v0 );

	for( i = 0 ; i < ar->nR ; i++ ){
		if( i == i0 )	continue;


		pln_type *pl = ar->r[i]->pl;
		if( pl == NULL || pl->link == NULL )
			continue;

		pln_tangetP( pl, 0, &p1, &v1 );

		p.x = p1.x - p0.x;
		p.y = p1.y - p0.y;

		d = p.x *p.x + p.y *p.y;
		if( d < dT*dT ){
			t = v0.x * v1.x + v0.y*v1.y;
			if( ABS(t) > aT ||  d < 0.75*dT*dT && ABS(t) > 0.75*aT  ){
				if( direct == 0 )
					pln_inverse( bpl );

				pln_append( bpl, pl );
				pln_destroy( pl );
				ar->r[i]->pl = NULL ;

				return( 1 );
			}

			continue;
		}





		pln_tangetP( pl, pl->len, &p1, &v1 );

		p.x = p1.x - p0.x;
		p.y = p1.y - p0.y;

		d = p.x *p.x + p.y *p.y;
		if( d < dT*dT ){
			t = v0.x * v1.x + v0.y*v1.y;
			if( ABS(t) > aT ||  d < 0.75*dT*dT && ABS(t) > 0.75*aT ){
				if( direct == 0 )
					pln_inverse( bpl );

				pln_inverse( pl );
				pln_append( bpl, pl );
				pln_destroy( pl );
				ar->r[i]->pl = NULL ;


				return( 1 );
			}

			continue;
		}

	}

	return( -1 );

}


