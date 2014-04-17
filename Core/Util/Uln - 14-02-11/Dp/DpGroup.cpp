#include	<string.h>
#include	<stdio.h>
#include	<math.h>



#include	"Uigp/igp.h"
#include	"DpType.h"



static void	dpA_group_replace( dpA_type *ad, int i0, int j0, int iGroup, int nGroup );





int
dpA_group( dpA_type *ad )
{
	dp_type	*dr,	*d;
	int	i,	j;
	float	t;

	int	nGroup = 1;




	for( i = 0 ; i < ad->Ni ; i++ ){
		dr = DPA( ad, i, 0 );
		for( j = 0 ; j < ad->Nj ; j++, dr++ ){
			if( dr->no > 0 )
				dr->no = 0;
		}
	}



	dp_type	*dr0 = DPA( ad, 28, 71 );

	for( i = 0 ; i < ad->Ni ; i++ ){
		dr = DPA( ad, i, 0 );

		for( j = 0 ; j < ad->Nj ; j++, dr++ ){
			if( dr->f < 0 )	continue;

			if( i > 0 ){
				d = dr - ad->Nj;

				t = VEC2D_INNER( d->v, dr->v );
				if( ABS(t) > 0.975 ){
					dr->no = d->no;
				} 
			}

			if( j > 0 ){
				d = dr - 1;

				t = VEC2D_INNER( d->v, dr->v );
				if( ABS(t) > 0.975 ){
					if( dr->no <= 0 ){
						dr->no = d->no;
					}
					else	{
						if( dr->no != d->no ){
							if( d->no < dr->no )
								dpA_group_replace( ad, i, j, dr->no, d->no );
							else dpA_group_replace( ad, i, j, d->no, dr->no );
						}
					}
				} 
			}

			if( dr->no <= 0 )
				dr->no = nGroup++;
		}
	}



	// renumbering
	int	*a = (int *)malloc( (nGroup + 10)*sizeof(int) );
	for( i = 0; i < nGroup ; i++ )
		a[i] = 0;


	for( i = 0 ; i < ad->Ni ; i++ ){
		dr = DPA( ad, i, 0 );
		for( j = 0 ; j < ad->Nj ; j++, dr++ ){
			if( dr->no <= 0 )	continue;
			a[dr->no]++;
		}
	}

	int	iGroup = 1;

	a[0] = 0;
	for( i = 1; i < nGroup ; i++ ){
		if( a[i] < 10 ){
			a[i] = 0;
			continue;
		}
		a[i] = iGroup++;
	}





	for( i = 0 ; i < ad->Ni ; i++ ){
		dr = DPA( ad, i, 0 );
		for( j = 0 ; j < ad->Nj ; j++, dr++ ){
			if( dr->no <= 0 )	continue;
			dr->no = a[dr->no];
		}
	}

	return( 1 );
}




static void
dpA_group_replace( dpA_type *ad, int i0, int j0, int iGroup, int nGroup )
{
	dp_type	*dr;
	int	i,	j;



	for( i = 0 ; i < ad->Ni ; i++ ){
		dr = DPA( ad, i, 0 );
		for( j = 0 ; j < ad->Nj ; j++, dr++ ){
			if( dr->no == iGroup )
				dr->no = nGroup;

			if( i == i0 && j == j0 )
				return;
		}
	}
}



