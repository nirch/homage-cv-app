#include	<string.h>
#include "Uigp/igp.h"

#include "Uxml/XmlType.h"

#include "Unedge.h"



unEdgePrm_type *
unEdgePrm_alloc()
{
	unEdgePrm_type *prm;

	prm = (unEdgePrm_type *)malloc( sizeof(unEdgePrm_type) );



	return( prm );
}

void
unEdgePrm_destroy( unEdgePrm_type *prm )
{
	free( prm );
}



int
CUnEdge::ReadPrm( char *inFile )
{
	xml_type *xmlDoc;
	xmlTag_type *pTag,	*tag;



	if((xmlDoc = xml_parse_file(  inFile )) == NULL ){
		return( -1 );
	}


	pTag = xmlTag_get_tag( xmlDoc->root, "UniformBackgroundPrm" );
	if( pTag == NULL ){
		xml_destroy(xmlDoc);
		return( -1 );
	}



	if( m_prm == NULL )
		m_prm = unEdgePrm_alloc();



	for( tag = pTag->firstChild ; tag != NULL ; tag = tag->next ){


		//if ( (gp_stricmp(tag->name, "thin") == 0) ){
		//	m_prm->thin = atoi( tag->data );

		//	continue;
		//}



	}



	xml_destroy(xmlDoc);


	

	return( 1 );
}

