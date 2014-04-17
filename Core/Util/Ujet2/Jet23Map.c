/************************
 *** AppJet2.cMap.c ***
 ************************/
#include <stdio.h>

#include	"Uigp/igp.h"
#include	"Jet23Type.h"
#include	"Uunity/unity.h"

float
jet23map_value( jet23_array *ajet, float x, float y )
{
jet23	f;

	jet23map_jet23( ajet, x, y, &f );

	return( J00(f) );
}


void
jet23mapB_jet23( jet23_array *ajet, float x, float y, jet23 *f )
{
vec2d	p;
jet23	pf;

	p.x = x;
	p.y = y;
	if( p.x < ajet->Xmin + ajet->dh )	p.x = ajet->Xmin + ajet->dh;
	if( p.x > ajet->Xmax - ajet->dh )	p.x = ajet->Xmax - ajet->dh;

	if( p.y < ajet->Ymin + ajet->dh )	p.y = ajet->Ymin + ajet->dh;
	if( p.y > ajet->Ymax - ajet->dh )	p.y = ajet->Ymax - ajet->dh;

	if( x == p.x && y == p.y ){
		jet23map_jet23( ajet, x, y, f );
		return;
	}

	jet23map_jet23( ajet, p.x, p.y, &pf );
	shift_jet23( &pf, x - p.x, y - p.y, f );
}



int
jet23map_jet23( jet23_array *ajet, float x, float y, jet23 *f )
{
int	i,	j;
float	x0,	y0;
jet23	u,	t;

	if( x < ajet->Xmin + ajet->dh || x > ajet->Xmax - ajet->dh ||
			y < ajet->Ymin + ajet->dh || y > ajet->Ymax - ajet->dh )
		return( 0 );

	unity3_set_paramter( ajet->dh, ajet->dh );

	i = x *ajet->inv_dxy;
	j = y *ajet->inv_dxy;

        x0 = x - ( i*ajet->dxy +ajet->dh );
        y0 = y - ( j*ajet->dxy +ajet->dh);

	unity3_jet23( &u, x0, y0 );
	shift_jet23( &ajet->jet[i][j], x0, y0, &t );
	product_jet23( &u, &t, f );


	if( i > 0 && j > 0 ){
		if( unity3_jet23( &u, x0+ajet->dxy , y0+ajet->dxy ) == 1 ){
			shift_jet23( &ajet->jet[i-1][j-1],
					x0 + ajet->dxy, y0+ajet->dxy, &t );
			productAsum_jet23( &u, &t, f );
		}
	}


	if( i > 0 ){
		if( unity3_jet23( &u, x0+ajet->dxy , y0 ) == 1 ){
			shift_jet23( &ajet->jet[i-1][j], x0 + ajet->dxy, y0, &t);
			productAsum_jet23( &u, &t, f );
		}
	}


	if( i > 0 && j+1 < ajet->Nj  ){
		if( unity3_jet23( &u, x0+ajet->dxy , y0 - ajet->dxy ) == 1 ){
			shift_jet23( &ajet->jet[i-1][j+1],
					x0 +ajet->dxy, y0-ajet->dxy, &t ) ;
			productAsum_jet23( &u, &t, f );
		}
	}




	if( j > 0 ){
		if( unity3_jet23( &u, x0 , y0+ajet->dxy ) == 1 ){
			shift_jet23( &ajet->jet[i][j-1], x0,  y0+ajet->dxy, &t ) ;
			productAsum_jet23( &u, &t, f );
		}
	}



	if( j+1 < ajet->Nj  ){
		if( unity3_jet23( &u, x0 , y0 - ajet->dxy ) == 1 ){
			shift_jet23( &ajet->jet[i][j+1], x0, y0 - ajet->dxy, &t);
			productAsum_jet23( &u, &t, f );
		}
	}





	if( i+1 < ajet->Ni && j > 0 ){
		if( unity3_jet23( &u, x0 - ajet->dxy , y0+ajet->dxy ) == 1 ){
			shift_jet23( &ajet->jet[i+1][j-1],
					x0 -ajet->dxy,  y0+ajet->dxy, &t );
			productAsum_jet23( &u, &t, f );
		}
	}

	if( i+1 < ajet->Ni ){
		if( unity3_jet23( &u, x0 - ajet->dxy, y0 ) == 1 ){
			shift_jet23( &ajet->jet[i+1][j], x0 - ajet->dxy, y0, &t);
			productAsum_jet23( &u, &t, f );
		}
	}


	if( i+1 < ajet->Ni && j+1 < ajet->Nj  ){
		if( unity3_jet23( &u, x0 - ajet->dxy, y0 - ajet->dxy ) == 1 ){
			shift_jet23( &ajet->jet[i+1][j+1],
					x0 -ajet->dxy, y0 -ajet->dxy, &t );
			productAsum_jet23( &u, &t, f );
		}
	}



	return( 1 );

}
