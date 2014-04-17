/************************
 ***   SmilParser.c   ***
 ************************/

#include <stdlib.h>
#include <string.h>

#include	"Uigp/igp.h"

#include	"Uxml/XmlType.h"


#include	"SmilType.h"



void	smil_parser_headTag( smil_type *smil, xmlTag_type *headTag );

void	smil_parser_layoutTag( smil_type *smil, xmlTag_type *layoutTag );

static void	smil_parser_rootlayoutTag( smil_type *smil, xmlTag_type *layoutTag );

static smilRegion_type *	smil_parser_regionTag( smil_type *smil, xmlTag_type *regionTag );



static void	smil_parser_bodyTag( smil_type *smil, xmlTag_type *bodyTag );
static smilSeq_type *	smil_parser_seqTag( xmlTag_type *seqTag );

static smilPar_type *	smil_parser_parTag( xmlTag_type *parTag );
static smilAttach_type *	smil_parser_attachTag( xmlTag_type *attachTag, int type );

static smilParam_type *	smil_parser_paramTag( xmlTag_type *paramTag );



smil_type *
smil_parser_file( char *file )
{
xml_type *xmlDoc;
smil_type *smil;
	
	if( (xmlDoc = xml_parse_file(  file )) == NULL )
		return( NULL );


	smil = smil_parser( xmlDoc );

	
	xml_destroy(xmlDoc);

	return( smil );
}


smil_type *
smil_parser_buffer( char *data, int bytes )
{
xml_type *xmlDoc;
smil_type *smil;
	
	if( (xmlDoc = xml_parse_data (  data, bytes )) == NULL )
		return( NULL );


	smil = smil_parser( xmlDoc );

	
	xml_destroy(xmlDoc);

	return( smil );
}


int
smil_parser_get_srcName( char *data, int bytes, char *aFile[], int NO )
{
xml_type *xmlDoc;
smil_type *smil;
int	no;
	
	if( (xmlDoc = xml_parse_data (  data, bytes )) == NULL )
		return( -1 );


	smil = smil_parser( xmlDoc );

	no = smil_get_srcNames( smil, aFile, NO );
	xml_destroy(xmlDoc);

	smil_destroy( smil );

	return( no );
}


/*
<smil>
	<head>
	</head>

	<body>
		<seq>
			<par dur = "5200ms">
				<img src = "file5.gif" region = "image"/>
			</par>
		</seq>
	</body>
</smil>
*/

smil_type *
smil_parser( xml_type *doc )
{
smil_type	*smil;
xmlTag_type * tag;

	if( gp_stricmp( doc->root->name, "smil" ) != 0 )
		return NULL;

	smil = smil_alloc();

	for( tag = doc->root->firstChild ; tag != NULL ; tag = tag->next ){

		if ( gp_stricmp(tag->name, "head") == 0){
			smil_parser_headTag( smil, tag );
			continue;
		}


		if ( gp_stricmp(tag->name, "body") == 0){
			smil_parser_bodyTag( smil, tag );
			continue;
		}
	}

	return( smil );
}


/*
<head>
	<layout>
	</layout>
</head>

*/
void
smil_parser_headTag( smil_type *smil, xmlTag_type *headTag )
{
xmlTag_type * tag;

	for( tag = headTag->firstChild ; tag != NULL ; tag = tag->next ){

		if ( gp_stricmp(tag->name, "layout") == 0){
			smil_parser_layoutTag( smil, tag );
			continue;
		}
	}
}


/*
<layout>
	<root-layout width = "320" height = "240"  background-color="#00ffff"/>
	<region id = "image" width = "320" height = "240" top = "0" left = "0"/>
</head>
*/

void
smil_parser_layoutTag( smil_type *smil, xmlTag_type *layoutTag )
{
xmlTag_type * tag;

int	iReg;

	smil->regionNo = xmlTag_get_childNo( layoutTag, "region" );

	smil->region = ( smilRegion_type **) malloc( sizeof(smilRegion_type * ) * smil->regionNo );


	for( tag = layoutTag->firstChild, iReg = 0 ; tag != NULL ; tag = tag->next ){

		if ( gp_stricmp(tag->name, "root-layout") == 0){
			smil_parser_rootlayoutTag( smil, tag );
			continue;
		}

		if ( gp_stricmp(tag->name, "region") == 0){
			smil->region[iReg++] = smil_parser_regionTag( smil, tag );
			continue;
		}
	}
}


/*
	<root-layout width = "320" height = "240" background-color="#00ffff"/>
*/

static void
smil_parser_rootlayoutTag( smil_type *smil, xmlTag_type *layoutTag )
{
xmlAtt_type *att;


	for (att = layoutTag->firstAtt; att != NULL ; att = att->next){

		if ( (gp_stricmp(att->name, "width") == 0) ){
			smil->width = atoi(att->value);
			continue;
		}

		if ( gp_stricmp(att->name, "height") == 0 ){
			smil->height = atoi(att->value);
			continue;
		}

		if ( gp_stricmp(att->name, "background-color") == 0 ){
			strncpy(smil->background_color, att->value, MAX_PRM_VALUE );
			continue;
		}
	}

	if( smil->width ==  0 )
		smil->width =176;

	if( smil->height == 0 )
		smil->height = 144;
}


/*
	<region id = "image" width = "320" height = "240" top = "0" left = "0"/>
*/
static smilRegion_type *
smil_parser_regionTag( smil_type *smil, xmlTag_type *regionTag )
{
smilRegion_type	*reg;
xmlAtt_type *att;
int	len;


	reg = smilRegion_alloc();

	reg->width = smil->width;
	reg->height = smil->height;
	reg->top = 0;
	reg->left = 0;
		
	for (att = regionTag->firstAtt; att != NULL ; att = att->next){
		
		if ( gp_stricmp(att->name, "id") == 0){
			strncpy(reg->id, att->value, MAX_REGION_ID );
			continue;
		}

		if ( (gp_stricmp(att->name, "width") == 0) ){
			reg->width = atoi(att->value);

			len = strlen( att->value );
			if( len > 0 && att->value[len-1] == '%' )
				reg->width = smil->width * reg->width/100;

			continue;
		}

		if ( gp_stricmp(att->name, "height") == 0 ){
			reg->height = atoi(att->value);

			len = strlen( att->value );
			if( len > 0 && att->value[len-1] == '%' )
				reg->height = smil->height * reg->height/100;
			continue;
		}


		if ( gp_stricmp(att->name, "left") == 0){
			reg->left = atoi(att->value);
			len = strlen( att->value );
			if( len > 0  && att->value[len-1] == '%' )
				reg->left = smil->width * reg->left/100;
			continue;
		}


		
		if (gp_stricmp(att->name, "top") == 0){
			reg->top = atoi(att->value);
			len = strlen( att->value );
			if( len > 0 && att->value[len-1] == '%' )
				reg->top = smil->height * reg->top/100;
			continue;
		}

		if (gp_stricmp(att->name, "z-index") == 0){
			reg->z = atoi(att->value);
			continue;
		}

	}



	return( reg );
}


/*
	<body>
		<seq>
			<par dur = "5200ms">
				<img src = "file5.gif" region = "image"/>
			</par>
		</seq>
	</body>
*/
static void
smil_parser_bodyTag( smil_type *smil, xmlTag_type *bodyTag )
{
xmlTag_type * tag;
int	iSeq;

	smil->seqNo = xmlTag_get_childNo( bodyTag, "Seq" );

	if( smil->seqNo == 0 ){
		smil->seqNo = 1;
		smil->seq = ( smilSeq_type **) malloc( sizeof(smilSeq_type * ) * smil->seqNo );

		smil->seq[0] = smil_parser_seqTag( bodyTag );
		return;
	}


	smil->seq = ( smilSeq_type **) malloc( sizeof(smilSeq_type * ) * smil->seqNo );


	for( tag = bodyTag->firstChild, iSeq = 0 ; tag != NULL ; tag = tag->next ){

		if ( gp_stricmp(tag->name, "Seq") == 0){
			smil->seq[iSeq++] = smil_parser_seqTag( tag );
			continue;
		}
	}
}


static smilSeq_type *
smil_parser_seqTag( xmlTag_type *seqTag )
{
smilSeq_type	*seq;
xmlTag_type * tag;
int	iPar;

	seq = smilSeq_alloc();
	seq->parNo = xmlTag_get_childNo( seqTag, "Par" );

	seq->par = ( smilPar_type **) malloc( sizeof(smilPar_type * ) * seq->parNo );


	for( tag = seqTag->firstChild, iPar = 0 ; tag != NULL ; tag = tag->next ){


		if ( gp_stricmp(tag->name, "Par") == 0){
			seq->par[iPar++] = smil_parser_parTag( tag );
			continue;
		}
	}

	return( seq );
}

/*
<par dur = "5200ms">
	<img src = "file5.gif" region = "image"/>
</par>
*/
static smilPar_type *
smil_parser_parTag( xmlTag_type *parTag )
{
smilPar_type	*par;
xmlAtt_type *att;
xmlTag_type	*tag;
int	iSrc;
char	*p;


	par = smilPar_alloc();

		
	for (att = parTag->firstAtt; att != NULL ; att = att->next){
		
		if ( (gp_stricmp(att->name, "dur") == 0) ){
			par->duration = atoi(att->value);

			for( p = att->value ; *p != 0 ; p++ ){
				if( *p == 'm' && *(p+1) == 's' )
					break;
				if( *p == 's' ){
					par->duration *= 1000;
					break;
				}
			}

			continue;
		}
	}


	par->attachNo = xmlTag_get_childNo( parTag, NULL );

	par->attach  = ( smilAttach_type **) malloc( sizeof(smilAttach_type * ) * par->attachNo );

	for( tag = parTag->firstChild, iSrc = 0 ; tag != NULL ; tag = tag->next ){

		if ( gp_stricmp(tag->name, "img") == 0){
			par->attach [iSrc++] = smil_parser_attachTag( tag, SMIL_ATTACH_IMAGE );
			continue;
		}

		if ( gp_stricmp(tag->name, "audio") == 0){
			par->attach [iSrc++] = smil_parser_attachTag( tag, SMIL_ATTACH_AUDIO );
			continue;
		}

		if ( gp_stricmp(tag->name, "text") == 0){
			par->attach [iSrc++] = smil_parser_attachTag( tag, SMIL_ATTACH_TEXT );
			continue;
		}


		if ( gp_stricmp(tag->name, "video") == 0){
			par->attach [iSrc++] = smil_parser_attachTag( tag, SMIL_ATTACH_VIDEO );
			continue;
		}
	}

	par->attachNo = iSrc;

	return( par );
}



/*
	<img src = "file5.gif" region = "image"/>
*/
static smilAttach_type *
smil_parser_attachTag( xmlTag_type *attachTag, int type )
{
smilAttach_type	*img;
xmlAtt_type *att;
xmlTag_type	*tag;
char	*p;
int	i;



	img = smilAttach_alloc();

	img->type = type;

		
	for (att = attachTag->firstAtt; att != NULL ; att = att->next){
		
		if ( gp_stricmp(att->name, "src") == 0){

			if( gp_strnicmp(  att->value, "cid:", 4 ) == 0 ){
				 strncpy(img->src, att->value+4, MAX_SRC_NAME );
				 img->reference = SMIL_ATTACH_ID;
			}
			else strncpy(img->src, att->value, MAX_SRC_NAME );
			continue;
		}


		if ( gp_stricmp(att->name, "region") == 0){
			strncpy(img->regId, att->value, MAX_REGION_ID );
			continue;
		}


		if ( (gp_stricmp(att->name, "begin") == 0) ){
			img->begin = atoi(att->value);

			for( p = att->value ; *p != 0 ; p++ ){
				if( *p == 'm' && *(p+1) == 's' )
					break;
				if( *p == 's' ){
					img->begin *= 1000;
					break;
				}
			}
		}

		if ( (gp_stricmp(att->name, "end") == 0) ){
			img->end = atoi(att->value);

			for( p = att->value ; *p != 0 ; p++ ){
				if( *p == 'm' && *(p+1) == 's' )
					break;
				if( *p == 's' ){
					img->end *= 1000;
					break;
				}
			}
		}

	}


	img->paramNo = xmlTag_get_childNo( attachTag, NULL );

	if( img->paramNo == 0 )
		return( img );


	img->param  = ( smilParam_type **) malloc( sizeof(smilParam_type * ) * img->paramNo );
	for( tag = attachTag->firstChild, i = 0 ; tag != NULL ; tag = tag->next ){

		if ( gp_stricmp(tag->name, "param") == 0){
			img->param[i++] = smil_parser_paramTag( tag );
			continue;
		}
	}
	img->paramNo = i;

	return( img );
}


//<param name="foreground-color" value="#ff0000"/>
static smilParam_type *
smil_parser_paramTag( xmlTag_type *paramTag )
{
smilParam_type	*prm;
xmlAtt_type *att;



	prm = smilParam_alloc();

		
	for (att = paramTag->firstAtt; att != NULL ; att = att->next){
		
		if ( gp_stricmp(att->name, "name") == 0){
			strncpy(prm->name, att->value, MAX_PRM_NAME );
			continue;
		}


		if ( gp_stricmp(att->name, "value") == 0){
			strncpy(prm->value, att->value, MAX_PRM_VALUE );
			continue;
		}
	}

	return( prm );
}