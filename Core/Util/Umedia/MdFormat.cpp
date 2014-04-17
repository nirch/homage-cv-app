/*************************
 ***   MdFormat.c   ***
 *************************/
#include	<string.h>
#include	<stdlib.h>

extern "C" {
#include	"Uigp/igp.h"
#include	"Uxml/XmlType.h"
}


#include	"MdFormat.h"
#include	"MdFormatIn.h"





#define	MAX_FORMAT	100
#define MAX_FORAMT_ID		1000

int				FormatNo = 0;
mdFormatD_type		*Aformat[MAX_FORMAT];
mdFormatD_type		*Bformat[MAX_FORAMT_ID];





int
mdFormat_initD( char *configDir )
{
char	file[256];

	sprintf( file, "%s/mdFormats.xml", configDir );
	if( mdFormat_init( file ) < 0 ){
		fprintf( stderr, "Can't open mdFormats.xml file\n" );
		return( -1 );
	}

	return( 1 );
}


int mdFormat_init( char *foramtFile )
{
int	ret;

	mdFormat_set( MI_WBMAP, MEDIA_IMAGE, ".wbmp", "image/vnd.wap.wbmp" );
	mdFormat_set( MI_NPM,   MEDIA_IMAGE, ".npm", "image/npm" );
	mdFormat_set( MI_EMS,   MEDIA_IMAGE, ".ems", "image/ems" );
	mdFormat_set( MI_JPEG,  MEDIA_IMAGE, ".jpg|.jpeg", "image/jpeg" );
	mdFormat_set( MI_PNG,   MEDIA_IMAGE, ".png", "image/png" );
	mdFormat_set( MI_BMAP,  MEDIA_IMAGE, ".bmp", "image/bmp" );
	mdFormat_set( MI_TIFF,  MEDIA_IMAGE, ".tif|.tiff", "image/tiff" );

	mdFormat_set( MV_VIM,   MEDIA_VIDEO, ".vim|.vima", "image/vim" );
	mdFormat_set( MV_VIML,   MEDIA_VIDEO, ".viml", "image/viml" );
	mdFormat_set( MI_GIF,   MEDIA_VIDEO, ".gif", "image/gif" );
	mdFormat_set( MV_AVI, MEDIA_VIDEO, ".avi", "video/x-msvideo" );
	mdFormat_set( MV_MP4, MEDIA_VIDEO, ".mp4", "video/mp4");
	mdFormat_set( MV_MPEG, MEDIA_VIDEO, ".mpeg|.mpg", "video/mpeg");	
	mdFormat_set(MV_MNG, MEDIA_VIDEO, 	".mng", "video/x-mng");
	mdFormat_set(MV_JNG, MEDIA_VIDEO, 	".jng", "video/x-jng");
	mdFormat_set(MV_WMV, MEDIA_VIDEO, 	".wmv", "video/wmv");
	mdFormat_set(MV_H263, MEDIA_VIDEO, ".h263", "video/h263");
	mdFormat_set(MV_H264, MEDIA_VIDEO, ".h264", "video/h264");
	mdFormat_set(MV_3GPP, MEDIA_VIDEO, ".3gp", "video/3gpp");
	mdFormat_set(MV_RM, MEDIA_VIDEO, ".rm", "video/rm");
	mdFormat_set(MV_MOV, MEDIA_VIDEO, ".mov", "video/mov");
	
	mdFormat_set( MF_SMIL, MEDIA_LAYOUT, ".smil", "application/smil" );

	mdFormat_set( MT_TEXT, MEDIA_TEXT, ".txt", "text/plain" );

	mdFormat_set( MA_WAV, MEDIA_AUDIO, ".wav", "audio/wav" ); 
	mdFormat_set( MA_MP3, MEDIA_AUDIO, ".mp3", "audio/mpeg" );
	mdFormat_set( MA_AMR, MEDIA_AUDIO, ".amr", "audio/amr" );
	mdFormat_set( MA_SUNAU, MEDIA_AUDIO, ".au", "audio/au" );
	mdFormat_set( MA_AAC, MEDIA_AUDIO, ".aac", "audio/aac" );	
	mdFormat_set( MA_WMA, MEDIA_AUDIO, ".wma", "audio/wma" );
	mdFormat_set( MA_EVRC, MEDIA_AUDIO, ".evrc", "audio/evrc" );
	mdFormat_set( MA_3GPP, MEDIA_AUDIO, ".3gp", "audio/3gpp" );
	mdFormat_set( MA_QCP, MEDIA_AUDIO, ".qcp", "audio/qcp" );
	mdFormat_set(MA_RM, MEDIA_AUDIO, ".rma|.ra", "audio/rm");
	mdFormat_set(MA_MP4, MEDIA_AUDIO, ".mp4|.m4a", "audio/mp4");
	
	mdFormat_set( MT_MIDI, MEDIA_TONE, ".mid|.midi", "audio/x-midi" );
	mdFormat_set( MT_IMELODY, MEDIA_TONE, ".imy", "text/x-imelody" );
	mdFormat_set( MT_EMELODY, MEDIA_TONE, ".emy", "text/x-emelody" );
	mdFormat_set( MT_RINGING_TONE, MEDIA_TONE, ".rtx|.rng", "text/x-rtttl" );

	mdFormat_set( MF_MMS, MEDIA_MESSAGE, ".mms", "application/vnd.wap.mms-message" );
	

	ret = mdFormat_read_format_file( foramtFile );

	return( ret );
}


void mdFormat_close()
{
	int i;
	for(  i = 0 ; i < FormatNo ; i++ )
	{
		mdFormatD_destroy(Aformat[i]);
		Aformat[i] = NULL;
	}
}

int
mdFormat_id( const char *file )
{
char	extension[256],	*p;
mdFormatD_type	*f;
int	len,	i;

	gp_filename_extract_extension( (char *)file, extension, 256 );
	len = strlen( extension );

	if ( len <= 1 )	return -1;

	
	for( i = 0 ; i < FormatNo ; i++ ){
		f = Aformat[i];
		if( gp_strnicmp( extension, f->extension, len ) == 0 )
			return( f->id );

		for( p = f->extension; *p != 0 ; p++ )
			if( *p == '|' && gp_strnicmp( extension, p+1, len ) == 0 )	return( f->id );
	}

	return( -1 );
}


int
mdFormat_mime_id( char *mime )
{
mdFormatD_type	*f;
char	*p;
int	len,	i;

	len = strlen( mime );
	if( len <= 0 )	return( -1 );
	for( i = 0 ; i < FormatNo ; i++ ){
		f = Aformat[i];

		if( gp_stricmp( mime, f->mime ) == 0 )
			return( f->id );

		for( p = f->mimeA; *p != 0 ; p++ ){
			if( *p != '|' )	continue;
			if( gp_strnicmp( mime, p+1, len ) == 0 ){
				if( p[len+1] == 0 || p[len+1] == '|' )
					return( f->id );
			}
		}
	}

	return( -1 );
}


int
mdFormat_mimeA_id( char *mimeA )
{
char	mime[64],	*p;
int		formatId,	i;


	for( p = mimeA, i = 0 ; *p != 0 ; p++ ){
		if( *p == '|' ){
			mime[i] = 0;
			formatId = mdFormat_mime_id( mime );
			if( formatId > 0 )	return( formatId );
			i = 0;
			continue;
		}

		mime[i++] = *p;
	}


	if( i > 0 ){
		mime[i] = 0;
		formatId = mdFormat_mime_id( mime );
		return( formatId );
	}

	return( -1 );
}

int
mdFormat_alloc_id()
{
mdFormatD_type	*f;
int	i,	max_id;

	max_id = 0;
	for( i = 0 ; i < FormatNo ; i++ ){
		f = Aformat[i];
		if( f->id > max_id )	max_id = f->id;
	}

	return( max_id+1 );
}

int
mdFormat_is_valid( int id )
{

	if( Bformat[id] == NULL )	return( -1 );

	return( 1 );
}


char *
mdFormat_extension( int id )
{

	if( Bformat[id] == NULL )	return( NULL );

	return( Bformat[id]->ext );
}


char *
mdFormat_mime( int id )
{
static char *unknown = "Unknown";

	if( id < 0 )	return( unknown);

	if( Bformat[id] == NULL )	return( unknown );


	return( Bformat[id]->mime );
}


int
mdFormat_index2id( int index )
{
	if( index < 0 || index >= FormatNo || Aformat[index] == NULL )
		return( -1 );


	return( Aformat[index]->id );
}


int
mdFormat_media( int id )
{

	if( Bformat[id] == NULL )	return( -1 );

	return( Bformat[id]->media );
}


int
mdFormat_set( int id, int media, char *extension, char *mimeA )
{
mdFormatD_type	*f;

	if( id > MAX_FORAMT_ID )	return( -1 );

	if( Bformat[id] != NULL )	return( -1 );

	if( FormatNo >= MAX_FORMAT )	return( -1 );



	f = mdFormatD_create( id, media, extension, mimeA );

	Aformat[FormatNo++] = f;

	Bformat[id] = f;


	return( 1 );
}



mdFormatD_type *
mdFormatD_alloc()
{
mdFormatD_type	*f;

	f = ( mdFormatD_type *)malloc( sizeof(mdFormatD_type) );
	f->id = -1;

	f->mimeA = NULL;
	f->ext = NULL;
	f->extension = NULL;

	return( f );
}


void mdFormatD_destroy(mdFormatD_type *	f)
{

	if ( f->extension != NULL)
		free(f->extension);

	if ( f->ext != NULL)
		free(f->ext);

	if ( f->mimeA != NULL)
		free(f->mimeA);

	if ( f->mime != NULL)
		free(f->mime);

	free(f);
}


mdFormatD_type *	
mdFormatD_create( int id, int media, char *extension, char *mimeA )
{
mdFormatD_type	*f;
char	*p;


	f = mdFormatD_alloc();

	f->id = id;
	f->media = media;

	f->extension = strdup( extension );


	f->ext = strdup( extension );
	for( p = f->ext ; *p != '\0' && *p != '|' ; p++ );
	*p = '\0';



	f->mimeA = strdup( mimeA );

	f->mime = strdup( mimeA );
	for( p = f->ext ; *p != '\0' && *p != '|' ; p++ );
	*p = '\0';


	return( f );
}


int
mdFormat_media_id( char *media )
{

	if( gp_stricmp(media, "video") == 0 )
		return(  MEDIA_VIDEO );


	if( gp_stricmp(media, "audio") == 0 )
			return( MEDIA_AUDIO );

	if( gp_stricmp(media, "image") == 0 )
			return( MEDIA_IMAGE );

	if( gp_stricmp(media, "tone") == 0 )
			return( MEDIA_TONE );

	if( gp_stricmp(media, "LAYOUT") == 0 )
			return( MEDIA_LAYOUT );

	if( gp_stricmp(media, "TEXT") == 0 )
			return( MEDIA_TEXT );

	return( -1 );
}

int
mdForamt_mediaString2mask( char *mediaStr )
{
int	mediaMask,	mediaId,	i;
char	media[512],	*p;

	mediaMask = 0;
	
	for( p = mediaStr, i = 0 ; *p != 0 ; p++ ){
		if( *p == '|' ){
			media[i] = 0;
			mediaId = mdFormat_media_id( media );
			if( mediaId > 0 )
				mediaMask |= ( 1<<mediaId );
			i= 0;
			continue;
		}

		media[i++] = *p;
	}

	if( i > 0 ){
		media[i] = 0;
		mediaId = mdFormat_media_id( media );
		if( mediaId > 0 )
			mediaMask |= ( 1<<mediaId );
	}

	return( mediaMask );
}