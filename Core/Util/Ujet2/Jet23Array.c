/************************
 *** AppJet2.cArray.c ***
 ************************/
#include <stdio.h>
#include <stdlib.h>	

#include	"Uigp/igp.h"
#include	"Jet23Type.h"

static void	jet23_array_alloc( jet23_array *ajet, int ni, int nj );


void
jet23_array_load( char *jetmap_name, jet23_array *ajet )
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

	jet23_array_alloc( ajet, ni, nj );

	ajet->dxy = dxy;
	ajet->inv_dxy = 1.0 / dxy;
	ajet->dh = 0.5*dxy;


	ajet->Xmin = ajet->Ymin = 0;

	ajet->Xmax = ajet->Ni * ajet->dxy;
	ajet->Ymax = ajet->Nj * ajet->dxy;

	for( i = 0; i < ajet->Ni ; i++ )
		for( j = 0; j < ajet->Nj ; j++ )
			load_jet2( fp, 3, (jet2 * )&ajet->jet[i][j] );

}



void
jet23_array_realloc( jet23_array *ajet, int ni, int nj )
{
	if( ajet->Ni == ni && ajet->Nj == nj )	return;

	jet23_array_free( ajet );

	jet23_array_alloc( ajet, ni, nj );
}


static void
jet23_array_alloc( jet23_array *ajet, int ni, int nj )
{
int          i;
int          byte;

	ajet->Ni  = ni ;
	ajet->Nj  = nj ;

	ajet->jet = (jet23 **)malloc( ajet->Ni * sizeof( jet23 * ) );

	byte = ajet->Nj * sizeof(jet23);

	for( i = 0 ; i < ajet->Ni ; i++ )
		ajet->jet[i] = (jet23 *)malloc( byte );
}



void
jet23_array_free( jet23_array *ajet )
{
int          i;

	for( i = 0 ; i < ajet->Ni ; i++ ){
		free( ajet->jet[i] );
		ajet->jet[i] = NULL;
	}

	free( ajet->jet );
	ajet->Ni = ajet->Nj = 0;
}
