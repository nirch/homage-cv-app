#include	<string.h>
#include "Uigp/igp.h"

#include "Uxml/XmlType.h"

#include "UnBackground.h"

static int ReadPrmDarkTag( unDarkPrm_type *d, xmlTag_type *pTag );


unPrm_type *
unPrm_alloc()
{
	unPrm_type *prm;

	prm = (unPrm_type *)malloc( sizeof(unPrm_type) );

	prm->thin = 0;

	prm->fillBlob = 120*120;



	prm->d.T0 = 64;
	prm->d.F0 = 0.75;

	prm->d.T1 = 144;
	prm->d.F1 = 0.9;

	return( prm );
}

void
unPrm_destroy( unPrm_type *prm )
{
	free( prm );
}



int
CUnBackground::ReadPrm( char *inFile )
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
		m_prm = unPrm_alloc();



	for( tag = pTag->firstChild ; tag != NULL ; tag = tag->next ){


		if ( (gp_stricmp(tag->name, "thin") == 0) ){
			m_prm->thin = atoi( tag->data );

			continue;
		}

		if ( (gp_stricmp(tag->name, "FillBlob") == 0) ){
			m_prm->fillBlob = atoi( tag->data );

			continue;
		}


		if ( (gp_stricmp(tag->name, "Dark") == 0) ){
			ReadPrmDarkTag( &m_prm->d, tag );
			continue;
		}
	}



	xml_destroy(xmlDoc);


	fprintf( stdout, "Reading %s\n", inFile );

	return( 1 );
}



static int ReadPrmDarkTag( unDarkPrm_type *d, xmlTag_type *pTag )
{
xmlTag_type	*tag;


	for( tag = pTag->firstChild ; tag != NULL ; tag = tag->next ){


		if ( (gp_stricmp(tag->name, "T0") == 0) ){
			d->T0 = atoi( tag->data );
			continue;
		}

		if ( (gp_stricmp(tag->name, "F0") == 0) ){
			d->F0 = atof( tag->data );
			continue;
		}

		if ( (gp_stricmp(tag->name, "T1") == 0) ){
			d->T1 = atoi( tag->data );
			continue;
		}

		if ( (gp_stricmp(tag->name, "F1") == 0) ){
			d->F1 = atof( tag->data );
			continue;
		}
	}

	return( 1 );
}
