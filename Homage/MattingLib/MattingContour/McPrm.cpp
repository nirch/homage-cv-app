#include	<string.h>
#include "Uigp/igp.h"

#include "Uxml/XmlType.h"

#include "MattingContour.h"




mcPrm_type *
mcPrm_alloc()
{
	mcPrm_type *prm;

	prm = (mcPrm_type *)malloc( sizeof(mcPrm_type) );

	prm->thin = 0;

	prm->fillBlob = 120*120;

	prm->fillBlobAll = 0;


	prm->enableEdge = 1;

	prm->in_d = 50;

	prm->smooth = 1;






	return( prm );
}

void
mcPrm_destroy( mcPrm_type *prm )
{
	free( prm );
}



int
CMattingContour::ReadPrm( char *inFile )
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
		m_prm = mcPrm_alloc();



	for( tag = pTag->firstChild ; tag != NULL ; tag = tag->next ){


		if ( (gp_stricmp(tag->name, "thin") == 0) ){
			m_prm->thin = atoi( tag->data );

			continue;
		}

		if ( (gp_stricmp(tag->name, "FillBlob") == 0) ){
			m_prm->fillBlob = atoi( tag->data );

			continue;
		}

		if ( (gp_stricmp(tag->name, "FillBlobAll") == 0) ){
			m_prm->fillBlobAll = atoi( tag->data );

			continue;
		}




		if ( (gp_stricmp(tag->name, "Enable-Edge") == 0) ){
			m_prm->enableEdge = atoi( tag->data );
			continue;
		}

		if ( (gp_stricmp(tag->name, "Contour-In-D") == 0) ){
			m_prm->in_d = atof( tag->data );
			continue;
		}

		if ( (gp_stricmp(tag->name, "Smooth") == 0) ){
			m_prm->smooth = atoi( tag->data );
			continue;
		}


	}



	xml_destroy(xmlDoc);


	fprintf( stdout, "Reading %s\n", inFile );

	return( 1 );
}


