
#include	<stdio.h>
#include	<math.h>
#include	<string.h>


#include "Uigp/igp.h"

#include "Ulog/Log.h"



#include "Uxml/XmlType.h"
#include "Uxml/XmlPut.h"


#include	"pHunchType.h"
#include	"Uprm/prm.h"



static int	size = 5;
static int	weight = 2;
static int sign = 0x2;
static float lm1Min = 2.4;
static float lmMin = 2;
static float lmR = 8;



static GP_prm   Params[] = {
	{"size", (char *) &size, PRM_INTEGER, "%d", "5"},
	{"weight", (char *) &weight, PRM_INTEGER, "%d", "3"},
	{"color", (char *) &sign, PRM_INTEGER, "%d", "2"},
	{"Lm-min", (char *) &lmMin, PRM_FLOAT, "%.2f", "2.0"},
	{"Lm1-min", (char *) &lm1Min, PRM_FLOAT, "%.2f", "2.4"},
	{"lmR", (char *) &lmR, PRM_FLOAT, "%.2f", "8"},
};




void 
pHunchPrm_get_lprm( GP_prm ** prm, int *prm_no, void (**check) ())
{
	*prm = Params;
	*prm_no = PRM_NUMBER(Params);
	*check = NULL;
}



void
pHunchPrm_update( pHunchPrm_type *prm )
{
	prm->size = size;
	prm->weight = weight;
	prm->sign = sign;
	prm->lmMin = lmMin;
	prm->lm1Min = lm1Min;
	prm->lmR = lmR;
}


void
pHunchPrm_set( pHunchPrm_type *prm )
{	
	size = prm->size;
	weight = prm->weight;
	sign = prm->sign;
	lmMin = prm->lmMin;
	lm1Min = prm->lm1Min;
	lmR = prm->lmR;
}





pHunchPrm_type *
pHunchPrm_alloc()
{
pHunchPrm_type *prm;

	prm = (pHunchPrm_type *)malloc( sizeof(pHunchPrm_type) );

	prm->size = size;
	prm->weight = weight;
	prm->sign = sign;
	prm->lmMin = lmMin;
	prm->lm1Min = lm1Min;
	prm->lmR = lmR;

	prm->dx = 0.55;
	prm->dy = 0.65;

	return( prm );
}


int
pHunchPrm_read( pHunchPrm_type *prm, char *file )
{
xml_type *xmlDoc;
xmlTag_type *pTag;


	if((xmlDoc = xml_parse_file(  file )) == NULL )
		return( -1 );
	

	pTag = xmlTag_get_tag( xmlDoc->root, "dtHunch" );
	if( pTag == NULL ){
		xml_destroy(xmlDoc);

		return( -1 );
	}


	pHunchPrm_read( pTag, prm );

	xml_destroy(xmlDoc);


	pHunchPrm_set( prm );

	return( 1 );
}

int
pHunchPrm_read( xmlTag_type *pTag, pHunchPrm_type *prm )
{
xmlTag_type *tag;

	for( tag = pTag->firstChild ; tag != NULL ; tag = tag->next ){
		if ( (gp_stricmp(tag->name, "Size") == 0) ){
			prm->size = atoi(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "Weight") == 0) ){
			prm->weight = atoi(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "Lm-Min") == 0) ){
			prm->lmMin = atof(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "Lm1-Min") == 0) ){
			prm->lm1Min = atof(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "Lm-R") == 0) ){
			prm->lmR = atof(tag->data);
			continue;
		}
	}


	pHunchPrm_set( prm );
	return( 1 );
}




int
pHunchPrm_write( pHunchPrm_type *prm, char *file )
{
FILE	*fp;

	if( (fp = fopen( file, "wb" )) == NULL )
		return( -1 );


	pHunchPrm_write( prm, 0, fp );

	fclose( fp );

	return( 1 );
}


int
pHunchPrm_write( pHunchPrm_type *prm, int	align, FILE *fp )
{

	xmlPut_tag_open( fp, align, "DtHunch" );

	xmlPut_tag_int( fp, align+1, "Size", prm->size );
	xmlPut_tag_int( fp, align+1, "Weight", prm->weight );

	xmlPut_tag_float( fp, align+1, "Lm-min", prm->lmMin );
	xmlPut_tag_float( fp, align+1, "Lm1-min", prm->lm1Min );

	xmlPut_tag_float( fp, align+1, "Lm-R", prm->lmR );


	xmlPut_tag_close( fp, align, "DtHunch" );


	return( 1 );
}
