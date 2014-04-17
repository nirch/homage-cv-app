/************************
 ***   jet2_array.c   ***
 ************************/
#include <stdio.h>
#include <stdlib.h>

#include	"Uigp/igp.h"
#include	"Jet2Type.h"

static void	jet2_array_alloc( jet2_array *ajet, int ni, int nj );


void
jet2_array_load( char *jetmap_name, jet2_array *ajet )
{
char	jetmap_file[50],	garbage[50];
FILE	*fp;
int	ni,	nj,	i,	j;
float	dxy;

/*
	sprintf( jetmap_file, "%s.jet",jetmap_name );
*/
	sprintf( jetmap_file, "%s",jetmap_name );

	if( (fp = fopen( jetmap_file, "r" ) ) == NULL )
		error("load_jetmap","can't open jetmap file" );


	fscanf( fp,"%[^G]" ,garbage);
	fscanf( fp,"%s %d %d %f" ,garbage,  &ni, &nj, &dxy );

	jet2_array_alloc( ajet, ni, nj );

	ajet->dxy = dxy;
	ajet->inv_dxy = 1.0 / dxy;
	ajet->dh = 0.5*dxy;


	ajet->Xmin = ajet->Ymin = 0;

	ajet->Xmax = ajet->Ni * ajet->dxy;
	ajet->Ymax = ajet->Nj * ajet->dxy;

	for( i = 0; i < ajet->Ni ; i++ )
		for( j = 0; j < ajet->Nj ; j++ )
			load_jet2( fp, 2, &ajet->jet[i][j] );

}



jet2_array *
ajet2_alloc()
{
jet2_array	*ajet;

	ajet = (jet2_array *)malloc( sizeof( jet2_array) );

	ajet->NI = ajet->NJ = 0;
	ajet->Ni = ajet->Nj = 0;
	ajet->jet = NULL;

	return( ajet );
}

void
ajet2_destroy( jet2_array *ajet )
{
	jet2_array_free( ajet );

	free( ajet );
}


void
jet2_array_realloc( jet2_array *ajet, int ni, int nj )
{
//	if( ajet->Ni == ni && ajet->Nj == nj )	return;
	if( ni <= ajet->NI  && nj <= ajet->NJ ){
		ajet->Ni = ni;
		ajet->Nj = nj;
		return;
	}

	jet2_array_free( ajet );

	jet2_array_alloc( ajet, ni, nj );
}


static void
jet2_array_alloc( jet2_array *ajet, int ni, int nj )
{
int          i;
int          byte;

	ajet->NI = ajet->Ni = ni;
	ajet->NJ = ajet->Nj = nj;

	ajet->jet = (jet2 **)malloc( ajet->NI * sizeof( jet2 * ) );

	byte = ajet->NJ * sizeof(jet2);

	for( i = 0 ; i < ajet->NI ; i++ )
		ajet->jet[i] = (jet2 *)malloc( byte );
}



void
jet2_array_free( jet2_array *ajet )
{
int          i;

	for( i = 0 ; i < ajet->NI ; i++ ){
		free( ajet->jet[i] );
		ajet->jet[i] = NULL;
	}

	if( ajet->jet != NULL )
		free( ajet->jet );

	ajet->NI = ajet->NJ = 0;
	ajet->jet = NULL;
}
