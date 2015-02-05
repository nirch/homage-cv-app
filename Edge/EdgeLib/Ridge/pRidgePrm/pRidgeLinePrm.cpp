
#include	<stdio.h>
#include	<math.h>
#include	<string.h>


#include "Uigp/igp.h"

#include "Ulog/Log.h"



#include "Uxml/XmlType.h"
#include "Uxml/XmlPut.h"


#include	"pRidgePrm.h"
#include	"Uprm/prm.h"

static int Color = 0x02;
static int	Weight = 3;
static float Lm_min = 2.0;
static float Len_min = 2.5;
static float Len_max = 0;
static float WeekR = 0.75;



static GP_prm   Params[] = {
	{"Color", (char *) &Color, PRM_INTEGER, "%d", "2"},
	{"Weight", (char *) &Weight, PRM_INTEGER, "%d", "3"},
	{"Lm-min", (char *) &Lm_min, PRM_FLOAT, "%.2f", "2"},
	{"Len-min", (char *) &Len_min, PRM_FLOAT, "%.2f", "2.5"},
	{"Len-max", (char *) &Len_max, PRM_FLOAT, "%.2f", "0"},
	{"Week-R", (char *) &WeekR, PRM_FLOAT, "%.2f", "0.75"},
};




void 
pRidgeLinePrm_get_lprm( GP_prm ** prm, int *prm_no, void (**check) ())
{
	*prm = Params;
	*prm_no = PRM_NUMBER(Params);
	*check = NULL;
}



void
pRidgeLinePrm_update( pRidgeLinePrm_type *prm )
{
	prm->color = Color;
	prm->weight = Weight;
	prm->lm_min = Lm_min;
	prm->lenMin = Len_min;
	prm->lenMax = Len_max;
	prm->WeekR = WeekR;

}


void
pRidgeLinePrm_set( pRidgeLinePrm_type *prm )
{	
	Color = prm->color;
	Weight = prm->weight;
	Lm_min = prm->lm_min;

	Len_min = prm->lenMin;
	Len_max = prm->lenMax;

	WeekR = prm->WeekR;
}





pRidgeLinePrm_type *
dtRidgePrm_alloc()
{
pRidgeLinePrm_type *prm;

	prm = (pRidgeLinePrm_type *)malloc( sizeof(pRidgeLinePrm_type) );

	prm->color = 0x02;	//balck
	prm->weight = Weight;
	prm->lm_min = Lm_min;
	prm->lenMin = Len_min;
	prm->lenMax = Len_max;
	prm->WeekR = WeekR;


	return( prm );
}



int
pRidgeLinePrm_read( pRidgeLinePrm_type *prm, char *file )
{
	xml_type *xmlDoc;
	xmlTag_type *pTag;


	if((xmlDoc = xml_parse_file(  file )) == NULL )
		return( -1 );


	pTag = xmlTag_get_tag( xmlDoc->root, "pRidgeLine" );
	if( pTag == NULL ){
		xml_destroy(xmlDoc);
		return( -1 );
	}


	pRidgeLinePrm_read( pTag, prm );

	xml_destroy(xmlDoc);


	pRidgeLinePrm_set( prm );

	return( 1 );
}

int
pRidgeLinePrm_read( xmlTag_type *pTag, pRidgeLinePrm_type *prm )
{
xmlTag_type *tag;


	for( tag = pTag->firstChild ; tag != NULL ; tag = tag->next ){
		if ( (gp_stricmp(tag->name, "Color") == 0) ){
			prm->color = atoi(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "Lm-Min") == 0) ){
			prm->lm_min = atof(tag->data);
			continue;
		}


		if ( (gp_stricmp(tag->name, "Len-Min") == 0) ){
			prm->lenMin = atof(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "Len-Max") == 0) ){
			prm->lenMax = atof(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "Week-R") == 0) ){
			prm->WeekR = atof(tag->data);
			continue;
		}

	}



	pRidgeLinePrm_set( prm );

	return( 1 );
}


int
pRidgeLinePrm_write( pRidgeLinePrm_type *prm, char *file )
{
FILE	*fp;


	if( (fp = fopen( file, "wb" )) == NULL )
		return( -1 );


	pRidgeLinePrm_write( prm,  0, fp );

	fclose( fp );

	return( 1 );
}



int
pRidgeLinePrm_write( pRidgeLinePrm_type *prm, int align, FILE *fp )
{

	xmlPut_tag_open( fp, align, "pRidgeLine" );

	xmlPut_tag_int( fp, align+1, "Color", prm->color );
	xmlPut_tag_float( fp, align+1, "Lm-min", prm->lm_min );

	xmlPut_tag_float( fp, align+1, "Len-min", prm->lenMin );
	xmlPut_tag_float( fp, align+1, "Len-max", prm->lenMax );

	xmlPut_tag_float( fp, align+1, "Week-R", prm->WeekR );


	xmlPut_tag_close( fp, align, "pRidgeLine" );

	return( 1 );
}