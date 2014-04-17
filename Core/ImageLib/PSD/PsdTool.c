/*********************
 ***   PsdType.h   ***
 *********************/

#include	<string.h>

#include	"PsdType.h"


#include	"ImageType/ImageType.h"




psd_type *
psd_alloc( int nLayer )
{
psd_type	*psd;

	psd = (psd_type *)calloc( sizeof(psd_type), 1 );

	if( nLayer > 0 ){
		psd->Layer = (psdLayer_type **)calloc( sizeof( psdLayer_type * ), nLayer );
		psd->LAYER_NO = nLayer;
	}

	psd->nLayer = 0;


	psd->image = NULL;

	return( psd );
}


void
psd_realloc( psd_type *psd, int nLayer )
{
	if( nLayer < psd->LAYER_NO )
		return;

	psd->Layer = (psdLayer_type **)realloc( psd->Layer, sizeof( psdLayer_type * ) * nLayer );
	psd->LAYER_NO = nLayer;

}




void
psd_add_layer( psd_type *psd, psdLayer_type *psdL )
{
	if( psd->LAYER_NO == psd->nLayer )
		psd_realloc( psd, psd->nLayer + 1 );

	psd->Layer[psd->nLayer++] = psdL;
}


void 
psd_destroy( psd_type *psd )
{
int	i;

	for ( i = 0; i < psd->nLayer; i++ )
		psdLayer_destroy( psd->Layer[i] );

	free( psd->Layer );


	if( psd->image != NULL )
		image_destroy( psd->image, 1 );


	free( psd );
}



psdLayer_type *
psdLayer_alloc()
{
psdLayer_type	*psdL;

	psdL = (psdLayer_type *)malloc( sizeof( psdLayer_type ) );

	psdL->image = NULL;
	
	return( psdL );
}


void 
psdLayer_destroy( psdLayer_type *psdL )
{
	if( psdL->image != NULL )
		image_destroy( psdL->image, 1 );


	free( psdL );

}


psdLayer_type * 
psdLayer_make_copy( psdLayer_type *psdL0 )
{
psdLayer_type *psdL;


	psdL = psdLayer_alloc();

	*psdL = *psdL0;

	psdL->image = image_make_copy( psdL0->image, NULL );



	return( psdL );

}




void
psd_build_image( psd_type *psd )
{
psdLayer_type *psdLayer;
int	i;


	if( psd->image != NULL )
		image_destroy( psd->image, 1 );


	psd->image = image_create( psd->height, psd->width, 4, 4, NULL );
	image4_const( psd->image, 0x00FFFFFF );


	for( i = 0; i < psd->nLayer ; i++ ){
		 psdLayer = psd->Layer[i];



		image_transparent_copy( psdLayer->image, psdLayer->y, psdLayer->x, 
				psdLayer->height, psdLayer->width, psd->image );

	}
}