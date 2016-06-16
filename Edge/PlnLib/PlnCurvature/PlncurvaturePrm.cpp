
#include	<stdio.h>
#include	<math.h>
#include	<string.h>


#include "Uigp/igp.h"

#include "Ulog/Log.h"



#include "Uxml/XmlType.h"
#include "Uxml/XmlPut.h"


#include	"Uprm/prm.h"


#include "PlnCurvature.h"




plnCurvaturePrm_type *
plnCurvaturePrm_alloc()
{
plnCurvaturePrm_type *prm;

	prm = (plnCurvaturePrm_type *)malloc( sizeof(plnCurvaturePrm_type) );

	prm->on = 1;

	prm->dt = 5;

	//prm->angle = 25;
	//prm->r = 50;

	//prm->r0 = 0.5*prm->dt;

	//prm->gapC = 15;


	prm->slT = 2.0;
	prm->slT1 = 16.0;
	prm->slLen = 10;
	


	return( prm );
}



void
	plnCurvaturePrm_set( plnCurvaturePrm_type *prm, float dT, float angle, float radius )
{
	prm->on = 1;

	prm->dt = dT;


	prm->slT = 2.0;
	prm->slT1 = 16.0;
	prm->slLen = 10;

	plnCurvaturePrm_aux( prm );
}

void
plnCurvaturePrm_aux( plnCurvaturePrm_type *prm )
{
//	prm->aT = cos( ANGLE_D2R(prm->angle) );
}


int
plnCurvaturePrm_read(  plnCurvaturePrm_type *prm, char *file )
{
xml_type *xmlDoc;
xmlTag_type *pTag;


	if((xmlDoc = xml_parse_file(  file )) == NULL )
		return( -1 );
	

	pTag = xmlTag_get_tag( xmlDoc->root, "PlnCurvature" );
	if( pTag == NULL )	return( -1 );



	plnCurvaturePrm_read(  prm, pTag );


	xml_destroy(xmlDoc);


	plnCurvaturePrm_aux( prm );

	return( 1 );
}




int
plnCurvaturePrm_read(  plnCurvaturePrm_type *prm, xmlTag_type *pTag )
{
xmlTag_type *tag;



	for( tag = pTag->firstChild ; tag != NULL ; tag = tag->next ){

		if ( (gp_stricmp(tag->name, "on") == 0) ){
			prm->on = atof(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "dt") == 0) ){
			prm->dt = atof(tag->data);
			continue;
		}


		//if ( (gp_stricmp(tag->name, "angle") == 0) ){
		//	prm->angle = atof(tag->data);
		//	continue;
		//}


		//if ( (gp_stricmp(tag->name, "Radius") == 0) ){
		//	prm->r = atof(tag->data);
		//	continue;
		//}

		if ( (gp_stricmp(tag->name, "SL-T") == 0) ){
			prm->slT = atof(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "SL-T1") == 0) ){
			prm->slT1 = atof(tag->data);
			continue;
		}
	}


	plnCurvaturePrm_aux( prm );

	return( 1 );
}