/**************************
 ***	MediaFormat.c   ***
 **************************/
#include <stdio.h>
#include <string.h>

#include "Uigp/igp.h"

#include "MediaDataType.h"





int
mediaData_mime2format( char *mime )
{
	if( mime == NULL )	return( -1 );

	if( gp_stricmp( mime, "audio/midi" ) == 0 || 
				gp_stricmp( mime, "audio/mid" ) == 0 ||
				gp_stricmp( mime, "audio/x-midi" ) == 0 || 
				gp_stricmp( mime, ".mid") == 0 )
		return( MT_MIDI );

	if( gp_stricmp( mime, "audio/wav" ) == 0 || gp_stricmp( mime, ".wav") == 0 )
		return( MA_WAV );

	if( gp_stricmp( mime, "audio/wma" ) == 0 || gp_stricmp( mime, ".wma") == 0 )
		return( MA_WMA );

	if( gp_stricmp( mime, "audio/mp3" ) == 0 || gp_stricmp( mime, ".mp3") == 0 )
		return( MA_MP3 );

	// Eli
	if( gp_stricmp( mime, "audio/amr" ) == 0 || gp_stricmp( mime, ".amr") == 0 )
		return( MA_AMR );

	if( gp_stricmp( mime, "image/bmp" ) == 0 || gp_stricmp( mime, ".bmp") == 0 )
		return( MI_BMP );

	if( gp_stricmp( mime, "image/gif" ) == 0 || gp_stricmp( mime, ".gif") == 0 )
		return( MI_BMP );


	if( gp_stricmp( mime, "image/jpeg" ) == 0 || gp_stricmp( mime, ".jpeg") == 0 || gp_stricmp( mime, ".jpg") == 0)
		return( MI_BMP );


	if( gp_stricmp( mime, "video/vim" ) == 0 || gp_stricmp( mime, "image/vim" ) == 0 || gp_stricmp( mime, ".vim") == 0 )
		return( MV_VIM );


	return( -1 );
}


char *
mediaData_mime( mediaData_type *md, char *mime )
{
	if( md->mime[0] != 0 ){
		strcpy( mime, md->mime );
		return( mime );
	}

	if( md->type <= 0 )
		md->type = mediaData_format_from_extension( md->location );

	return( mediaData_format2mime(md->type, mime ) );
}


int
mediaData_get_format( mediaData_type *md )
{
	if( md->type >= 0 )	return( md->type );

	if( md->location != NULL ){
		md->type = mediaData_format_from_extension( md->location );
		return( md->type );
	}

	return( md->type );
}


char *
mediaData_get_extension( mediaData_type *md, char *extension )
{
	extension[0] = 0;

	if( md->type >= 0 ){
		mediaData_format2extension( md->type, extension );
		return( extension );
	}

	if( md->location != NULL ){
		gp_filename_extract_extension( md->location, extension, 64 );
		return( extension );
	}

	return( extension );
}


char *
mediaData_format2mime( int format, char *mime )
{
	mime[0] = 0;

	switch( format ){
	case MT_MIDI:
			strcpy( mime, "audio/midi" );
		break;

	case MA_WAV:
			strcpy( mime, "audio/wav" );
		break;

	case MA_WMA:
			strcpy( mime, "audio/wma" );
		break;

	case MA_MP3:
			strcpy( mime, "audio/mpeg" );
		break;

	case MA_AMR:
			strcpy( mime, "audio/amr" );
		break;

	case MI_BMP:
			strcpy( mime, "image/bmp" );
		break;

	case MI_JPG:
		strcpy( mime, "image/jpeg" );
		break;

	case MI_GIF:
		strcpy( mime, "image/gif" );
		break;

	case MI_PNG:
		strcpy( mime, "image/png" );
		break;


	case MV_VIM:
		strcpy( mime, "video/vim" );
	break;
	}

	if( mime[0] == 0 )	return( NULL );

	return( mime );
}

char *
mediaData_format2extension( int format, char *extension )
{
	extension[0] = 0;

	switch( format ){
	case MT_MIDI:
			strcpy( extension, ".mid" );
		break;

	case MA_WAV:
			strcpy( extension, ".wav" );
		break;

	case MA_WMA:
			strcpy( extension, ".wma" );
		break;

	case MA_MP3:
			strcpy( extension, ".mp3" );
		break;

	case MA_AMR:
			strcpy( extension, ".amr" );
		break;

	case MI_BMP:
			strcpy( extension, ".bmp" );
		break;

	case MV_VIM:
		strcpy( extension, ".vim" );
		break;
	}

	return( extension );
}



int
mediaData_format_from_extension( char *file )
{
char	extension[256];

	gp_filename_extract_extension( file, extension, 256 );

	gp_strlwr(extension);

	return( mediaData_format( extension ) );
}



int
mediaData_format( char *extension )
{
	if( extension == NULL )	return( -1 );

	if( gp_stricmp( extension, ".mid") == 0 )
		return( MT_MIDI );

	if( gp_stricmp( extension, ".wav") == 0 )
		return( MA_WAV );

	if( gp_stricmp( extension, ".wma") == 0 )
		return( MA_WMA );

	if( gp_stricmp( extension, ".mp3") == 0 )
		return( MA_MP3 );

	if( gp_stricmp( extension, ".amr") == 0 )
		return( MA_AMR );



	if( gp_stricmp( extension, ".bmp") == 0 )
		return( MI_BMP );

	if( gp_stricmp( extension, ".jpg") == 0 || gp_stricmp( extension, ".jpeg") == 0)
		return( MI_JPG );

	if( gp_stricmp( extension, ".gif") == 0 )
		return( MI_GIF );

	if( gp_stricmp( extension, ".png") == 0 )
		return( MI_PNG );





	if( gp_strnicmp( extension, ".vim", 4) == 0 )
		return( MV_VIM );


	return( -1 );
}

