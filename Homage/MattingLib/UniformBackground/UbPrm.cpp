#include	<string.h>
#include "Uigp/igp.h"

#include "Uxml/XmlType.h"

#include "UniformBackground.h"




ubPrm_type *
ubPrm_alloc()
{
	ubPrm_type *prm;

	prm = (ubPrm_type *)malloc( sizeof(ubPrm_type) );

	prm->thin = 0;

	prm->fillBlob = 120*120;

	return( prm );
}

void
ubPrm_destroy( ubPrm_type *prm )
{
	free( prm );
}



int
CUniformBackground::ReadPrm( char *inFile )
{
	xml_type *xmlDoc;
	xmlTag_type *pTag,	*tag;



	if((xmlDoc = xml_parse_file(  inFile )) == NULL ){
		fprintf( stdout, "Reading %s  failed\n", inFile );
		return( -1 );
	}


	pTag = xmlTag_get_tag( xmlDoc->root, "UniformBackgroundPrm" );
	if( pTag == NULL ){
		xml_destroy(xmlDoc);
		return( -1 );
	}



	if( m_prm == NULL )
		m_prm = ubPrm_alloc();



	for( tag = pTag->firstChild ; tag != NULL ; tag = tag->next ){


		if ( (gp_stricmp(tag->name, "thin") == 0) ){
			m_prm->thin = atoi( tag->data );

			continue;
		}

		if ( (gp_stricmp(tag->name, "FillBlob") == 0) ){
			m_prm->fillBlob = atoi( tag->data );

			continue;
		}
	}



	xml_destroy(xmlDoc);


	fprintf( stdout, "Reading %s\n", inFile );

	return( 1 );
}
