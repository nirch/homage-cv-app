/************************
 *** jet2_array.c ***
 ************************/
#include <stdio.h>

#include	"Uigp/igp.h"
#include	"Jet2Type.h"
#include	"Uunity/unity.h"

float
jet2map_value( jet2_array *ajet, float x, float y )
{
jet2	f;

	jet2map_jet2( ajet, x, y, &f );

	return( J00(f) );
}


int
jet2map_jet2( jet2_array *ajet, float x, float y, jet2 *f )
{
int	i,	j;
float	x0,	y0;
jet2	u,	t,	t1;

	if( x < ajet->Xmin + ajet->dh || x > ajet->Xmax - ajet->dh ||
			y < ajet->Ymin + ajet->dh || y > ajet->Ymax - ajet->dh )
		return( 0 );

	unity2_set_paramter( ajet->dh, ajet->dh );

	i = (x - ajet->Xmin ) *ajet->inv_dxy;
	j = (y - ajet->Ymin ) *ajet->inv_dxy;

        x0 = x - ( ajet->Xmin + i*ajet->dxy +ajet->dh );
        y0 = y - ( ajet->Ymin + j*ajet->dxy +ajet->dh);

	unity2_jet2( &u, x0, y0 );
	shift_jet2( &ajet->jet[i][j], x0, y0, &t );
	product_jet2( &u, &t, f );


	if( i+1 < ajet->Ni ){
		unity2_jet2( &u, x0 - ajet->dxy, y0 );
		if( J00(u) != 0 ){
			shift_jet2( &ajet->jet[i+1][j], x0 - ajet->dxy, y0, &t);
			product_jet2( &u, &t, &t1 );
			*f = sum_jet2( f, &t1, 2 );
		}
	}


	if( i+1 < ajet->Ni && j+1 < ajet->Nj  ){
		unity2_jet2( &u, x0 - ajet->dxy, y0 - ajet->dxy );
		if( J00(u) != 0 ){
			shift_jet2( &ajet->jet[i+1][j+1],
					x0 -ajet->dxy, y0 -ajet->dxy, &t );
			product_jet2( &u, &t, &t1 );
			*f = sum_jet2( f, &t1, 2 );
		}
	}


	if( j+1 < ajet->Nj  ){
		unity2_jet2( &u, x0 , y0 - ajet->dxy );
		if( J00(u) != 0 ){
			shift_jet2( &ajet->jet[i][j+1], x0, y0 - ajet->dxy, &t);
			product_jet2( &u, &t, &t1 );
			*f = sum_jet2( f, &t1, 2 );
		}
	}


	if( i > 0 && j+1 < ajet->Nj  ){
		unity2_jet2( &u, x0+ajet->dxy , y0 - ajet->dxy );
		if( J00(u) != 0 ){
			shift_jet2( &ajet->jet[i-1][j+1],
					x0 +ajet->dxy, y0-ajet->dxy, &t ) ;
			product_jet2( &u, &t, &t1 );
			*f = sum_jet2( f, &t1, 2 );
		}
	}


	if( i > 0 ){
		unity2_jet2( &u, x0+ajet->dxy , y0 );
		if( J00(u) != 0 ){
			shift_jet2( &ajet->jet[i-1][j], x0 + ajet->dxy, y0, &t);
			product_jet2( &u, &t, &t1 );
			*f = sum_jet2( f, &t1, 2 );
		}
	}


	if( i > 0 && j > 0 ){
		unity2_jet2( &u, x0+ajet->dxy , y0+ajet->dxy );
		if( J00(u) != 0 ){
			shift_jet2( &ajet->jet[i-1][j-1],
					x0 + ajet->dxy, y0+ajet->dxy, &t );
			product_jet2( &u, &t, &t1 );
			*f = sum_jet2( f, &t1, 2 );
		}
	}


	if( j > 0 ){
		unity2_jet2( &u, x0 , y0+ajet->dxy );
		if( J00(u) != 0 ){
			shift_jet2( &ajet->jet[i][j-1], x0,  y0+ajet->dxy, &t ) ;
			product_jet2( &u, &t, &t1 );
			*f = sum_jet2( f, &t1, 2 );
		}
	}


	if( i+1 < ajet->Ni && j > 0 ){
		unity2_jet2( &u, x0 - ajet->dxy , y0+ajet->dxy );
		if( J00(u) != 0 ){
			shift_jet2( &ajet->jet[i+1][j-1],
					x0 -ajet->dxy,  y0+ajet->dxy, &t );
			product_jet2( &u, &t, &t1 );
			*f = sum_jet2( f, &t1, 2 );
		}
	}
	return( 1 );

}
