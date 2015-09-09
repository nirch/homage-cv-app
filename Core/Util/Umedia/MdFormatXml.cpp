/*************************
 ***   Format.c   ***
 *************************/
#include	<string.h>
#include <stdlib.h>

extern "C" {
#include	"Uigp/igp.h"
#include	"Uxml/XmlType.h"
}


#include	"MdFormat.h"
#include	"MdFormatIn.h"




extern int				FormatNo;
extern mdFormatD_type		*Aformat[];
extern mdFormatD_type		*Bformat[];





static void	mdFormat_extract_formatTag( xmlTag_type *attrTag );


static void	mdFormat_extract_mediaTag( xmlTag_type *mediaTag, mdFormatD_type	*f );



int 
mdFormat_read_format_file( char *formatFile )
{
xml_type *doc;
xmlTag_type *audioSettingsTag,	*tag;

	doc = xml_parse_file(formatFile);
	if( doc == NULL )	return( -1 );


	if( doc->root == NULL ){
		xml_destroy( doc );

		return( -1 );
	}
	
	audioSettingsTag = doc->root->firstChild;
	if( gp_stricmp( audioSettingsTag->name, "Formats") != 0 ){
		xml_destroy( doc );

		return( -1 );
	}


	for ( tag = audioSettingsTag->firstChild ; tag != NULL ; tag = tag->next ){
		if (gp_stricmp(tag->name, "format") == 0 ){
			mdFormat_extract_formatTag( tag );

			continue;
		}

	}


	xml_destroy( doc );

	return( 1 );
}



static void
mdFormat_extract_formatTag( xmlTag_type *attrTag )
{
mdFormatD_type	*f;
xmlTag_type	*tag;
char	*p;
int		formatId,	i;


	f = mdFormatD_alloc();


	for ( tag = attrTag->firstChild ; tag != NULL ; tag = tag->next ){

		if (gp_stricmp(tag->name, "media") == 0 ){
			mdFormat_extract_mediaTag( tag, f );

			continue;
		}


		if (gp_stricmp(tag->name, "mime") == 0 ){
			f->mimeA = strdup( tag->data );
			f->mime = strdup( f->mimeA );

			for( p = f->mime ; *p != '\0' && *p != '|' ; p++ );
			*p = '\0';
			continue;
		}


		if (gp_stricmp(tag->name, "extension") == 0 ){
			f->extension = strdup( tag->data );

			if( f->ext != NULL )	free( f->ext );
			f->ext = strdup( f->extension );
			for( p = f->ext ; *p != '\0' && *p != '|' ; p++ );
			*p = '\0';
			
			continue;
		}
	}


	formatId = mdFormat_mimeA_id( f->mimeA );

	if( formatId > 0 ){
		for( i = 0 ; i < FormatNo ; i++ ){
			if( Aformat[i]->id == formatId ){
				mdFormatD_destroy( Aformat[i] );
				Aformat[i] = f;
				Bformat[ formatId ] = f;
				f->id =  formatId;
				return;
			}
		}
	}


	formatId = mdFormat_alloc_id(); 

	f->id = formatId;

	Aformat[FormatNo++] = f;

	Bformat[formatId] = f;

}


static void
mdFormat_extract_mediaTag( xmlTag_type *mediaTag, mdFormatD_type	*f )
{

	if( gp_stricmp(mediaTag->data, "video") == 0 ){
		f->media = MEDIA_VIDEO;
		return;
	}

	if( gp_stricmp(mediaTag->data, "audio") == 0 ){
			f->media = MEDIA_AUDIO;
			return;
	}

	if( gp_stricmp(mediaTag->data, "image") == 0 ){
			f->media = MEDIA_IMAGE;
			return;
	}

	if( gp_stricmp(mediaTag->data, "tone") == 0 ){
			f->media = MEDIA_TONE;
			return;
	}
}


