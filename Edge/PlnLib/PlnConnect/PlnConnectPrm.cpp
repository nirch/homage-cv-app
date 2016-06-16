
#include	<stdio.h>
#include	<math.h>
#include	<string.h>


#include "Uigp/igp.h"

#include "Ulog/Log.h"



#include "Uxml/XmlType.h"
#include "Uxml/XmlPut.h"


#include	"Uprm/prm.h"


#include "PlnConnect.h"




plnConnectPrm_type *
plnConnectPrm_alloc()
{
plnConnectPrm_type *prm;

	prm = (plnConnectPrm_type *)malloc( sizeof(plnConnectPrm_type) );

	prm->on = 1;

	prm->dt = 20;

	prm->angle = 25;
	prm->r = 50;

	prm->r0 = 0.5*prm->dt;

	prm->gapC = 15;

	


	return( prm );
}



void
	plnConnectPrm_set( plnConnectPrm_type *prm, float dT, float angle, float radius )
{
	prm->on = 1;

	prm->dt = dT;

	prm->angle = angle;

	prm->r = 50;

	prm->r0 = 0.5*prm->dt;

	prm->gapC = 15;

	plnConnectPrm_aux( prm );
}

void
plnConnectPrm_aux( plnConnectPrm_type *prm )
{
	prm->aT = cos( ANGLE_D2R(prm->angle) );
}


int
plnConnectPrm_read(  plnConnectPrm_type *prm, char *file )
{
xml_type *xmlDoc;
xmlTag_type *pTag;


	if((xmlDoc = xml_parse_file(  file )) == NULL )
		return( -1 );
	

	pTag = xmlTag_get_tag( xmlDoc->root, "PlnConnect" );
	if( pTag == NULL )	return( -1 );



	plnConnectPrm_read(  prm, pTag );


	xml_destroy(xmlDoc);


	plnConnectPrm_aux( prm );

	return( 1 );
}




int
	plnConnectPrm_read(  plnConnectPrm_type *prm, xmlTag_type *pTag )
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


		if ( (gp_stricmp(tag->name, "angle") == 0) ){
			prm->angle = atof(tag->data);
			continue;
		}


		if ( (gp_stricmp(tag->name, "Radius") == 0) ){
			prm->r = atof(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "Radius-0") == 0) ){
			prm->r0 = atof(tag->data);
			continue;
		}


		if ( (gp_stricmp(tag->name, "Gap") == 0) ){
			prm->gapC = atof(tag->data);
			continue;
		}
	}


	plnConnectPrm_aux( prm );

	return( 1 );
}