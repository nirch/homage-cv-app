
#include	<stdio.h>
#include	<math.h>
#include	<string.h>


#include "Uigp/igp.h"

#include "Ulog/Log.h"



#include "Uxml/XmlType.h"
#include "Uxml/XmlPut.h"


#include	"Uprm/prm.h"


#include "plnMerging.h"




plnMergingPrm_type *
plnMergingPrm_alloc()
{
plnMergingPrm_type *prm;

	prm = (plnMergingPrm_type *)malloc( sizeof(plnMergingPrm_type) );

	prm->on = 1;

	prm->dU = 5;

	prm->dT = 6;


	return( prm );
}



void
plnMergingPrm_set( plnMergingPrm_type *prm, float dU, float dT )
{
	prm->on = 1;

	prm->dU = dU;

	prm->dT = dT;


	plnMergingPrm_aux( prm );
}

void
plnMergingPrm_aux( plnMergingPrm_type *prm )
{
//	prm->aT = cos( ANGLE_D2R(prm->angle) );
}


int
plnMergingPrm_read(  plnMergingPrm_type *prm, char *file )
{
xml_type *xmlDoc;
xmlTag_type *pTag;


	if((xmlDoc = xml_parse_file(  file )) == NULL )
		return( -1 );
	

	pTag = xmlTag_get_tag( xmlDoc->root, "plnMerging" );
	if( pTag == NULL )	return( -1 );



	plnMergingPrm_read(  prm, pTag );


	xml_destroy(xmlDoc);


	plnMergingPrm_aux( prm );

	return( 1 );
}




int
plnMergingPrm_read(  plnMergingPrm_type *prm, xmlTag_type *pTag )
{
xmlTag_type *tag;



	for( tag = pTag->firstChild ; tag != NULL ; tag = tag->next ){

		if ( (gp_stricmp(tag->name, "on") == 0) ){
			prm->on = atof(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "dU") == 0) ){
			prm->dU = atof(tag->data);
			continue;
		}


		if ( (gp_stricmp(tag->name, "dT") == 0) ){
			prm->dT = atof(tag->data);
			continue;
		}
	}


	plnMergingPrm_aux( prm );

	return( 1 );
}