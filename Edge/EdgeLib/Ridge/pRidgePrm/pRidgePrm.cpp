
#include	<stdio.h>
#include	<math.h>
#include	<string.h>


#include "Uigp/igp.h"





#include "Uxml/XmlType.h"
#include "Uxml/XmlPut.h"

#include	"Uprm/prm.h"


#include "pRidgePrm.h"




pRidgePrm_type *
pRidgePrm_alloc()
{
pRidgePrm_type *prm;

	prm = (pRidgePrm_type *)malloc( sizeof(pRidgePrm_type) );

	pRidgePrm_update( prm );

	return( prm );
}


void
pRidgePrm_update( pRidgePrm_type *prm )
{
	pRidgeDetectPrm_update( &prm->detect );

	pRidgeLinePrm_update( &prm->line );
}


void
pRidgePrm_set( pRidgePrm_type *prm )
{
	pRidgeDetectPrm_set( &prm->detect );

	pRidgeLinePrm_set( &prm->line );
}


void
pRidgePrm_destroy( pRidgePrm_type *prm )
{
	free( prm );
}



int
pRidgePrm_read( pRidgePrm_type *prm, char *file )
{
	xml_type *xmlDoc;
	xmlTag_type *pTag;

	if( file == NULL )
		return( -1 );

	if((xmlDoc = xml_parse_file(  file )) == NULL )
		return( -1 );


	pTag = xmlTag_get_tag( xmlDoc->root, "pRidgePrm" );
	if( pTag == NULL ){
		xml_destroy(xmlDoc);
		return( -1 );
	}


	pRidgePrm_read( pTag, prm );

	xml_destroy(xmlDoc);


	return( 1 );
}

int
pRidgePrm_read( xmlTag_type *pTag, pRidgePrm_type *prm )
{
xmlTag_type *tag;


	for( tag = pTag->firstChild ; tag != NULL ; tag = tag->next ){

		if ( (gp_stricmp(tag->name, "pRidgeDetect") == 0) ){
			pRidgeDetectPrm_read( tag, &prm->detect );
			continue;
		}

		if ( (gp_stricmp(tag->name, "pRidgeLine") == 0) ){
			pRidgeLinePrm_read( tag, &prm->line );
			continue;
		}
	}



	pRidgePrm_set( prm );

	return( 1 );
}


int
pRidgePrm_write( pRidgePrm_type *prm, char *file )
{
FILE	*fp;


	if( (fp = fopen( file, "wb" )) == NULL )
		return( -1 );


	pRidgePrm_write( prm,  0, fp );

	fclose( fp );

	return( 1 );
}



int
pRidgePrm_write( pRidgePrm_type *prm, int align, FILE *fp )
{

	xmlPut_tag_open( fp, align, "pRidgePrm" );


	pRidgeDetectPrm_write( &prm->detect, align+1, fp );

	pRidgeLinePrm_write( &prm->line, align+1, fp );


	xmlPut_tag_close( fp, align, "pRidgePrm" );

	return( 1 );
}