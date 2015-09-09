
#include	<stdio.h>
#include	<math.h>
#include	<string.h>


#include "Uigp/igp.h"

#include "Ulog/Log.h"



#include "Uxml/XmlType.h"
#include "Uxml/XmlPut.h"





#include "PlnHeadDetect.h"




plnHeadDetectPrm_type *
plnHeadDetectPrm_alloc()
{
plnHeadDetectPrm_type *prm;

	prm = (plnHeadDetectPrm_type *)malloc( sizeof(plnHeadDetectPrm_type) );




	prm->dt = 20;

	prm->angle = 10;

	prm->du = 4;

	prm->scale = 0.5;
	prm->qulity = 0.3;

	plnHeadDetectPrm_aux( prm );

	


	return( prm );
}



void
plnHeadDetectPrm_set( plnHeadDetectPrm_type *prm, float dT, float angle )
{
	prm->dt = dT;


	plnHeadDetectPrm_aux( prm );
}




void
plnHeadDetectPrm_aux( plnHeadDetectPrm_type *prm )
{
	prm->aT = cos( ANGLE_D2R(prm->angle) );
}


int
plnHeadDetectPrm_read(  plnHeadDetectPrm_type *prm, char *file )
{
xml_type *xmlDoc;
xmlTag_type *pTag;


	if((xmlDoc = xml_parse_file(  file )) == NULL )
		return( -1 );
	

	pTag = xmlTag_get_tag( xmlDoc->root, "PlnHeadDetect" );
	if( pTag == NULL )	return( -1 );



	plnHeadDetectPrm_read(  prm, pTag );


	xml_destroy(xmlDoc);


	plnHeadDetectPrm_aux( prm );

	return( 1 );
}




int
	plnHeadDetectPrm_read(  plnHeadDetectPrm_type *prm, xmlTag_type *pTag )
{
xmlTag_type *tag;



	for( tag = pTag->firstChild ; tag != NULL ; tag = tag->next ){


		if ( (gp_stricmp(tag->name, "dt") == 0) ){
			prm->dt = atof(tag->data);
			continue;
		}


		if ( (gp_stricmp(tag->name, "angle") == 0) ){
			prm->angle = atof(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "du") == 0) ){
			prm->du = atof(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "Scale-Min") == 0) ){
			prm->scale = atof(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "Qulity-Min") == 0) ){
			prm->qulity = atof(tag->data);
			continue;
		}

	}


	plnHeadDetectPrm_aux( prm );

	return( 1 );
}