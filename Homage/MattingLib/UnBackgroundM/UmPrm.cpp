#include	<string.h>
#include "Uigp/igp.h"

#include "Uxml/XmlType.h"

#include "UnBackgroundM.h"




umPrm_type *
umPrm_alloc()
{
	umPrm_type *prm;

	prm = (umPrm_type *)malloc( sizeof(umPrm_type) );


	prm->dx = 8;
	prm->av = 4;


	return( prm );
}

void
umPrm_destroy( umPrm_type *prm )
{
	free( prm );
}



int
CUnBackgroundM::ReadPrm( char *inFile )
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
		m_prm = umPrm_alloc();



	for( tag = pTag->firstChild ; tag != NULL ; tag = tag->next ){



		if ( (gp_stricmp(tag->name, "Background-Dx") == 0) ){
			m_prm->dx = atoi( tag->data );

			continue;
		}

		if ( (gp_stricmp(tag->name, "Background-Av") == 0) ){
			m_prm->av = atoi( tag->data );

			continue;
		}

	}



	xml_destroy(xmlDoc);


	fprintf( stdout, "Reading %s\n", inFile );

	return( 1 );
}



