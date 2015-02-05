
#include	<stdio.h>
#include	<math.h>
#include	<string.h>


#include "Uigp/igp.h"

#include "Ulog/Log.h"



#include "Uxml/XmlType.h"
#include "Uxml/XmlPut.h"


#include	"pEdgeType.h"
#include	"Uprm/prm.h"

int	pEdgePrm_read( xmlTag_type *pTag, pEdgePrm_type *prm );

static int Scale = 1;
static int D = 3;
static float GradT = 10;
static float Len_min = 0;
static float Linking_a = 25;

static int StraightLine = 0;



static GP_prm   Params[] = {
	{"Scale", (char *) &Scale, PRM_INTEGER, "%d", "1"},
	{"D", (char *) &D, PRM_INTEGER, "%d", "3"},
	{"grad-T", (char *) &GradT, PRM_FLOAT, "%.2f", "10.0"},

	{"Len-min", (char *) &Len_min, PRM_FLOAT, "%.2f", "20.0"},

	{"Linking-A", (char *) &Linking_a, PRM_FLOAT, "%.2f", "25.0"},

	{"Line-straight", (char *) &StraightLine, PRM_INTEGER, "%d", "0"},
};




void 
pEdgePrm_get_lprm( GP_prm ** prm, int *prm_no, void (**check) ())
{
	*prm = Params;
	*prm_no = PRM_NUMBER(Params);
	*check = NULL;
}



void
pEdgePrm_update( pEdgePrm_type *prm )
{
	prm->d = D;
	prm->gradT = GradT;

	prm->scale = Scale;

	prm->lenMin = Len_min;

	prm->linking_A = Linking_a;

	prm->straightLine = StraightLine;
}


void
pEdgePrm_set( pEdgePrm_type *prm )
{	
	
	D = prm->d;

	GradT = prm->gradT;

	Scale = prm->scale;

	Len_min = prm->lenMin;

	Linking_a = prm->linking_A;
	StraightLine = prm->straightLine;
}





pEdgePrm_type *
pEdgePrm_alloc()
{
pEdgePrm_type *prm;

	prm = (pEdgePrm_type *)malloc( sizeof(pEdgePrm_type) );

	pEdgePrm_update( prm );

	return( prm );
}

void
pEdgePrm_destroy( pEdgePrm_type *prm )
{
	free( prm );
}



int
pEdgePrm_read( pEdgePrm_type *prm, char *file )
{
	xml_type *xmlDoc;
	xmlTag_type *pTag;


	if((xmlDoc = xml_parse_file(  file )) == NULL )
		return( -1 );


	pTag = xmlTag_get_tag( xmlDoc->root, "pEdge" );
	if( pTag == NULL ){
		xml_destroy(xmlDoc);
		return( -1 );
	}


	pEdgePrm_read( pTag, prm );

	xml_destroy(xmlDoc);


	pEdgePrm_set( prm );

	return( 1 );
}

int
pEdgePrm_read( xmlTag_type *pTag, pEdgePrm_type *prm )
{
xmlTag_type *tag;


	for( tag = pTag->firstChild ; tag != NULL ; tag = tag->next ){

		if ( (gp_stricmp(tag->name, "Scale") == 0) ){
			prm->scale = atof(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "D") == 0) ){
			prm->d = atoi(tag->data);
			continue;
		}


		if ( (gp_stricmp(tag->name, "Grad-T") == 0) ){
			prm->gradT = atof(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "Len-min") == 0) ){
			prm->lenMin = atof(tag->data);
			continue;
		}


		if ( (gp_stricmp(tag->name, "Linking-Angle") == 0) ){
			prm->linking_A = atof(tag->data);
			continue;
		}

		if ( (gp_stricmp(tag->name, "Line-straightLine") == 0) ){
			prm->straightLine = atoi(tag->data);
			continue;
		}
	}



	pEdgePrm_set( prm );

	return( 1 );
}


int
pEdgePrm_write( pEdgePrm_type *prm, char *file )
{
FILE	*fp;


	if( (fp = fopen( file, "wb" )) == NULL )
		return( -1 );


	pEdgePrm_write( prm,  0, fp );

	fclose( fp );

	return( 1 );
}



int
pEdgePrm_write( pEdgePrm_type *prm, int align, FILE *fp )
{

	xmlPut_tag_open( fp, align, "pEdge" );

	xmlPut_tag_int( fp, align+1, "Scale", prm->scale );

	xmlPut_tag_float( fp, align+1, "D", prm->d );

	xmlPut_tag_float( fp, align+1, "Grad-T", prm->gradT );

	xmlPut_tag_float( fp, align+1, "Len-Min", prm->lenMin );

	xmlPut_tag_float( fp, align+1, "Linking-Angle", prm->linking_A );

	xmlPut_tag_int( fp, align+1, "Line-straightLine", prm->straightLine );

	xmlPut_tag_close( fp, align, "pEdge" );

	return( 1 );
}