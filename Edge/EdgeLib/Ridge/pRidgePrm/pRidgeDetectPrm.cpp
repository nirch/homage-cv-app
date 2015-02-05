
#include	<stdio.h>
#include	<math.h>
#include	<string.h>


#include "Uigp/igp.h"

#include "Ulog/Log.h"



#include "Uxml/XmlType.h"
#include "Uxml/XmlPut.h"


#include	"pRidgePrm.h"
#include	"Uprm/prm.h"



static int	Size = 5;
static int	Weight = 3;
static int	Weight2 = 2;
static int Color = 0x03;
static float Lm_min = 0.5;
static float LmR = 1.0;
static float SaddleR = 1.4;


static GP_prm   Params[] = {
	{"Color", (char *) &Color, PRM_INTEGER, "%d", "3"},
	{"Size", (char *) &Size, PRM_INTEGER, "%d", "5"},
	{"Weight", (char *) &Weight, PRM_INTEGER, "%d", "3"},
	{"Weight2", (char *) &Weight2, PRM_INTEGER, "%d", "2"},
	{"Lm-min", (char *) &Lm_min, PRM_FLOAT, "%.2f", "1.0"},
	{"Lm-R", (char *) &LmR, PRM_FLOAT, "%.2f", "1"},
	{"SaddleR-R", (char *) &SaddleR, PRM_FLOAT, "%.2f", "1.4"},

};




void 
pRidgeDetectPrm_get_lprm( GP_prm ** prm, int *prm_no, void (**check) ())
{
	*prm = Params;
	*prm_no = PRM_NUMBER(Params);
	*check = NULL;
}



void
pRidgeDetectPrm_update( pRidgeDetectPrm_type *prm )
{
	prm->weight = Weight;
	prm->weight2 = Weight2;
	prm->size = Size;
	prm->sign = Color;
	prm->lm_min = Lm_min;
	prm->lmR = LmR;
	prm->saddleR = SaddleR;


}


void
pRidgeDetectPrm_set( pRidgeDetectPrm_type *prm )
{	
	Weight = prm->weight;
	Weight2 = prm->weight2;
	Size = prm->size;
	Color = prm->sign;
	Lm_min = prm->lm_min;

	LmR = prm->lmR;

	SaddleR = prm->saddleR;

}






pRidgeDetectPrm_type *
pRidgeDetectPrm_alloc()
{
pRidgeDetectPrm_type *prm;

	prm = (pRidgeDetectPrm_type *)malloc( sizeof(pRidgeDetectPrm_type) );

	pRidgeDetectPrm_update( prm );

	return( prm );
}



void
pRidgeDetectPrm_destroy( pRidgeDetectPrm_type *prm )
{
	free( prm );
}



int
pRidgeDetectPrm_read( pRidgeDetectPrm_type *prm, char *file )
{
	xml_type *xmlDoc;
	xmlTag_type *pTag;


	if((xmlDoc = xml_parse_file(  file )) == NULL )
		return( -1 );


	pTag = xmlTag_get_tag( xmlDoc->root, "pRidgeDetect" );
	if( pTag == NULL ){
		xml_destroy(xmlDoc);
		return( -1 );
	}


	pRidgeDetectPrm_read( pTag, prm );

	xml_destroy(xmlDoc);


	pRidgeDetectPrm_set( prm );

	return( 1 );
}

int
pRidgeDetectPrm_read( xmlTag_type *pTag, pRidgeDetectPrm_type *prm )
{
xmlTag_type *tag;


	for( tag = pTag->firstChild ; tag != NULL ; tag = tag->next ){
		if ( (gp_stricmp(tag->name, "Weight") == 0) ){
			prm->weight = atoi(tag->data);
			continue;
		}
		if ( (gp_stricmp(tag->name, "Size") == 0) ){
			prm->size = atoi(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "Sign") == 0) ){
			prm->sign = atoi(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "Lm-min") == 0) ){
			prm->lm_min = atof(tag->data);
			continue;
		}


		if ( (gp_stricmp(tag->name, "Lm-R") == 0) ){
			prm->lmR = atof(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "Saddle-R") == 0) ){
			prm->saddleR = atof(tag->data);
			continue;
		}
	}



	pRidgeDetectPrm_set( prm );

	return( 1 );
}


int
pRidgeDetectPrm_write( pRidgeDetectPrm_type *prm, char *file )
{
FILE	*fp;


	if( (fp = fopen( file, "wb" )) == NULL )
		return( -1 );


	pRidgeDetectPrm_write( prm,  0, fp );

	fclose( fp );

	return( 1 );
}



int
pRidgeDetectPrm_write( pRidgeDetectPrm_type *prm, int align, FILE *fp )
{

	xmlPut_tag_open( fp, align, "pRidgeDetect" );
	xmlPut_tag_int( fp, align+1, "Weight", prm->weight );

	xmlPut_tag_int( fp, align+1, "Size", prm->size );

	xmlPut_tag_int( fp, align+1, "Sign", prm->sign );
	xmlPut_tag_float( fp, align+1, "Lm-min", prm->lm_min );

	xmlPut_tag_float( fp, align+1, "Lm-R", prm->lmR );

	xmlPut_tag_float( fp, align+1, "Saddle-R", prm->saddleR );

	xmlPut_tag_close( fp, align, "pRidgeDetect" );

	return( 1 );
}