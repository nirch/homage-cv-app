
#include	<stdio.h>
#include	<math.h>
#include	<string.h>


#include "Uigp/igp.h"

#include "Ulog/Log.h"



#include "Uxml/XmlType.h"
#include "Uxml/XmlPut.h"


#include	"Uprm/prm.h"


#include "PlnFeature.h"




plnFeaturePrm_type *
plnFeaturePrm_alloc()
{
plnFeaturePrm_type *prm;

	prm = (plnFeaturePrm_type *)malloc( sizeof(plnFeaturePrm_type) );

	prm->on = 1;

	prm->cornerDis = 5;

	prm->featureLen = 25;

	prm->angle = 10;


	return( prm );
}



void
	plnFeaturePrm_set( plnFeaturePrm_type *prm, float dT, float len, float angle )
{
	prm->on = 1;

	prm->featureLen = len;

	prm->cornerDis = dT;
	prm->cornerLen = dT;

	prm->angle = angle;





	plnFeaturePrm_aux( prm );
}

void
plnFeaturePrm_aux( plnFeaturePrm_type *prm )
{
	prm->aT = cos( ANGLE_D2R(prm->angle) );
}


int
plnFeaturePrm_read(  plnFeaturePrm_type *prm, char *file )
{
xml_type *xmlDoc;
xmlTag_type *pTag;


	if((xmlDoc = xml_parse_file(  file )) == NULL )
		return( -1 );
	

	pTag = xmlTag_get_tag( xmlDoc->root, "PlnCFeature" );
	if( pTag == NULL )	return( -1 );



	plnFeaturePrm_read(  prm, pTag );


	xml_destroy(xmlDoc);


	plnFeaturePrm_aux( prm );

	return( 1 );
}




int
plnFeaturePrm_read(  plnFeaturePrm_type *prm, xmlTag_type *pTag )
{
xmlTag_type *tag;



	for( tag = pTag->firstChild ; tag != NULL ; tag = tag->next ){

		if ( (gp_stricmp(tag->name, "on") == 0) ){
			prm->on = atof(tag->data);
			continue;
		}


		if ( (gp_stricmp(tag->name, "Len") == 0) ){
			prm->featureLen = atof(tag->data);
			continue;
		}


		if ( (gp_stricmp(tag->name, "Corner-D") == 0) ){
			prm->cornerDis = atof(tag->data);
			continue;
		}
		if ( (gp_stricmp(tag->name, "Corner-Len") == 0) ){
			prm->cornerLen = atof(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "angle") == 0) ){
			prm->angle = atof(tag->data);
			continue;
		}





	}


	plnFeaturePrm_aux( prm );

	return( 1 );
}