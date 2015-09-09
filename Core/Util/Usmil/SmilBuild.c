/************************
 ***   SmilParser.c   ***
 ************************/

#include <stdlib.h>
#include <string.h>

#include	"Uigp/igp.h"

#include	"Uxml/XmlType.h"


#include	"SmilType.h"



void	smil_build( smil_type *smil, cst_type *cst );

void	smil_build_headTag( smil_type *smil, int align, cst_type *cst );

void	smil_build_layoutTag( smil_type *smil, int align, cst_type *cst );
void	smil_build_rootlayoutTag( smil_type *smil, int align, cst_type *cst );
void	smil_build_regionTag( smilRegion_type *region, int align, cst_type *cst );
void	smil_build_regionTagP( smil_type *smil, smilRegion_type *region, int align, cst_type *cst );





static void	smil_build_bodyTag( smil_type *smil, int align, cst_type *cst );

static void	smil_build_seqTag( smilSeq_type *smil, int align, cst_type *cst );

void	smil_build_parTag( smilPar_type *par, int align, cst_type *cst );

void	smil_build_attachTag( smilAttach_type *attach, int align, cst_type *cst );

static void	smil_build_paramTag( smilParam_type *prm, int align, cst_type *cst );





static smilPar_type *	smil_parser_parTag( xmlTag_type *parTag );
static smilAttach_type *	smil_parser_attachTag( xmlTag_type *attachTag, int type );



int
smil_build_file( smil_type *smil, char *file )
{
char	*data;
int	bytes;

	if( smil_build_buffer( smil, &data, &bytes ) < 0 )
		return( -1 );

	gpFile_write_buffer( file, data, bytes );

	free( data );
	return( 1 );
}


int
smil_build_buffer( smil_type *smil, char **data, int *bytes )
{
cst_type	*cst;

	cst = cst_alloc( 20000 );



	smil_build( smil, cst );

	
	*data = (char *)cst->data;
	*bytes = CST_BYTES( cst );
	(*data)[*bytes] = 0;

	cst->data = NULL;
	cst_destroy( cst, 0 );

	return( 1 );
}


/*
<smil>
	<head>
	</head>

	<body>
	</body>
</smil>
*/

void
smil_build( smil_type *smil, cst_type *cst )
{

	cst_put_xml_tag_openA( cst, 0, "smil" );


	smil_build_headTag( smil, 1, cst );

	smil_build_bodyTag( smil, 1, cst );


	cst_put_xml_tag_closeA( cst, 0, "smil" );

//	CST_PUT_CHAR( cst, 0 );

}


/*
<head>
	<layout>
	</layout>
</head>

*/
void
smil_build_headTag( smil_type *smil, int align, cst_type *cst )
{
	cst_put_xml_tag_openA( cst, align, "head" );

	smil_build_layoutTag( smil, align+1, cst );


	cst_put_xml_tag_closeA( cst, align, "head" );

}


/*
<layout>
	<root-layout width = "320" height = "240"  background-color="#00ffff" />
	<region id = "image" width = "320" height = "240" top = "0" left = "0"/>
</head>
*/


void
smil_build_layoutTag( smil_type *smil, int align, cst_type *cst )
{
int	i;
	cst_put_xml_tag_openA( cst, align, "layout" );

	smil_build_rootlayoutTag( smil, align+1, cst );

	for( i = 0 ; i < smil->regionNo ; i++ )
//		smil_build_regionTag( smil->region[i], align+1, cst );
		smil_build_regionTagP( smil, smil->region[i], align+1, cst );

	cst_put_xml_tag_closeA( cst, align, "layout" );

}


/*
	<root-layout width = "320" height = "240"/>
*/
void
smil_build_rootlayoutTag( smil_type *smil, int align, cst_type *cst )
{
	cst_put_stringA( cst, align, "<root-layout" );

	cst_put_xml_att_int( cst, "width", smil->width, NULL );
	cst_put_xml_att_int( cst, "height", smil->height, NULL );

	if( smil->background_color[0] != 0 )
		cst_put_xml_att_string( cst, "background-color", smil->background_color );


	cst_put_string( cst, "/>\n" );
}





/*
	<region id = "image" width = "320" height = "240" top = "0" left = "0"/>
*/
void
smil_build_regionTag( smilRegion_type *region, int align, cst_type *cst )
{

	cst_put_stringA( cst, align, "<region" );
	
	cst_put_xml_att_string( cst, "id", region->id );

	cst_put_xml_att_int( cst, "width", region->width, NULL );
	cst_put_xml_att_int( cst, "height", region->height, NULL );

	cst_put_xml_att_int( cst, "top", region->top, NULL );
	cst_put_xml_att_int( cst, "left", region->left, NULL );

	if( region->z > 0 )
		cst_put_xml_att_int( cst, "z-index", region->z, NULL );

	cst_put_string( cst, "/>\n" );
}


void
smil_build_regionTagP( smil_type *smil, smilRegion_type *region, int align, cst_type *cst )
{
int	percent;

	cst_put_stringA( cst, align, "<region" );
	
	cst_put_xml_att_string( cst, "id", region->id );

	if( (percent = region->width*100/smil->width) > 100 )	percent = 100;
	cst_put_xml_att_int( cst, "width", percent, "%" );

	if( (percent = region->height*100/smil->height) > 100 )	percent = 100;
	cst_put_xml_att_int( cst, "height", percent, "%" );


	if( region->top == 0 )
		cst_put_xml_att_int( cst, "top", region->top, NULL );
	else {
		if( (percent = region->top*100/smil->height) > 100 )	percent = 100;
		cst_put_xml_att_int( cst, "top", percent, "%" );
	}


	if( region->left == 0 )
		cst_put_xml_att_int( cst, "left", region->left, NULL );
	else {
		if( (percent = region->left*100/smil->width) > 100 )	percent = 100;
		cst_put_xml_att_int( cst, "left", percent, "%" );
	}

	if( region->z > 0 )
		cst_put_xml_att_int( cst, "z-index", region->z, NULL );

	cst_put_string( cst, "/>\n" );
}


/*
	<body>
		<seq>
		</seq>
		<seq>
		</seq>
	</body>
*/

static void
smil_build_bodyTag( smil_type *smil, int align, cst_type *cst )
{
int	i;
	cst_put_xml_tag_openA( cst, align, "body" );

	for( i = 0 ; i < smil->seqNo ; i++ )
		smil_build_seqTag( smil->seq[i], align+1, cst );

	cst_put_xml_tag_closeA( cst, align, "body" );

}


static void
smil_build_seqTag( smilSeq_type *seq, int align, cst_type *cst )
{
int	i;
	cst_put_xml_tag_openA( cst, align, "seq" );

	for( i = 0 ; i < seq->parNo ; i++ )
		smil_build_parTag( seq->par[i], align+1, cst );

	cst_put_xml_tag_closeA( cst, align, "seq" );

}



/*
<par dur = "5200ms">
	<img src = "file5.gif" region = "image"/>
</par>
*/

void
smil_build_parTag( smilPar_type *par, int align, cst_type *cst )
{
int	i;
	cst_put_stringA( cst, align, "<par" );
	cst_put_xml_att_int( cst, "dur", par->duration, "ms" );
	cst_put_string( cst, ">\n" );


	for( i = 0 ; i < par->attachNo ; i++ )
		smil_build_attachTag( par->attach[i], align+1, cst );


	cst_put_xml_tag_closeA( cst, align, "par" );
}


/*
	<img src = "file5.gif" region = "image"/>
*/
void
smil_build_attachTag( smilAttach_type *attach, int align, cst_type *cst )
{
char	src[256];
int	i;

	if( attach->reference == SMIL_ATTACH_ID )
		sprintf( src, "cid:%s", attach->src );
	else	strcpy( src, attach->src );


	switch( attach->type ){
	case SMIL_ATTACH_IMAGE:
		cst_put_stringA( cst, align, "<img" );
		cst_put_xml_att_string( cst, "src", src );
		cst_put_xml_att_string( cst, "region", attach->regId );
		break;

	case SMIL_ATTACH_AUDIO:
		cst_put_stringA( cst, align, "<audio" );
		cst_put_xml_att_string( cst, "src", src );
		break;

	case SMIL_ATTACH_TEXT:
		cst_put_stringA( cst, align, "<text" );
		cst_put_xml_att_string( cst, "src", src );
		cst_put_xml_att_string( cst, "region", attach->regId );
		break;

	case SMIL_ATTACH_VIDEO:
		cst_put_stringA( cst, align, "<video" );
		cst_put_xml_att_string( cst, "src", src );
		cst_put_xml_att_string( cst, "region", attach->regId );
		break;
	}

	if( attach->begin != 0 )
		cst_put_xml_att_int( cst, "begin", attach->begin, "ms" );

	if( attach->end != 0 )
		cst_put_xml_att_int( cst, "end", attach->end, "ms" );


	if( attach->paramNo == 0 ){
		cst_put_string( cst, "/>\n" );
		return;
	}

	cst_put_string( cst, ">\n" );
	for( i = 0 ; i < attach->paramNo ; i++ )
		smil_build_paramTag( attach->param[i], align+1, cst );


	switch( attach->type ){
	case SMIL_ATTACH_IMAGE:
		cst_put_xml_tag_closeA( cst, align, "img" );
		break;

	case SMIL_ATTACH_AUDIO:
		cst_put_xml_tag_closeA( cst, align, "audio" );
		break;

	case SMIL_ATTACH_TEXT:
		cst_put_xml_tag_closeA( cst, align, "text" );
		break;
	}
}


//<param name="foreground-color" value="#ff0000"/>
static void
smil_build_paramTag( smilParam_type *prm, int align, cst_type *cst )
{
	cst_put_stringA( cst, align, "<param" );
	cst_put_xml_att_string( cst, "name", prm->name );
	cst_put_xml_att_string( cst, "value", prm->value );

	cst_put_string( cst, "/>\n" );
}



