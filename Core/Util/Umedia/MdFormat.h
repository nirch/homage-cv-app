/**********************
 ***   MdFormat.h   ***
 **********************/

#ifndef         _MD_FORMAT_
#define         _MD_FORMAT_

#ifdef __cplusplus
extern "C" {
#endif


#include	"Uigp/igp.h"
//#include	"Uigp/M64Type.h"



#define	MAX_EXTENSION	4

#define	MIME_LEN	64


#define		MEDIA_IMAGE		1
#define		MEDIA_AUDIO		2
#define		MEDIA_VIDEO		3
#define		MEDIA_LAYOUT	4
#define		MEDIA_TEXT		5
#define		MEDIA_TONE		6

#define		MEDIA_MESSAGE	7

#define		MEDIA_MAX		8




/* MEDIA_IMAGE id */
#define	MI_WBMAP	1
#define	MI_NPM		2
#define	MT_TEXT		3
#define	MI_EMS		4
#define	MI_GIF		5
#define	MI_JPEG		6
#define	MI_PNG		7
#define	MI_BMAP		8
#define	MI_TGA		9
#define	MI_TIFF		10
#define MI_J2K		11


/* MEDIA_VIDEO id */
#define	MV_VIM		15
#define MV_AVI		16
#define MV_MP4		17
#define MV_MPEG		18
#define MV_MNG		19
#define MV_JNG		20
#define MV_WMV		21
#define MV_H263		22
#define MV_3GPP		23
#define MV_RM		24
#define MV_MOV		25
#define MV_H264		26

/* MEDIA_AUDIO id */
#define	MA_AUDIO	30
#define	MA_WAV		30
#define	MA_SUNAU	31
#define	MA_AMR		32
#define	MA_MP3		33
#define	MA_AAC		34
#define	MA_AMR_WB	35
#define	MA_WMA		36
#define	MA_EVRC		37
#define	MA_3GPP		38
#define	MA_QCP		39
#define	MA_RM   	40
#define	MA_MP4   	41


/* MEDIA_TONE id */
#define	MT_MIDI			45
#define MT_IMELODY		46
#define	MT_EMELODY		47
#define	MT_RINGING_TONE	48

#define	MF_SMIL		50

#define	MF_MMS		55

#define	MV_VIML		60





	/* MdFormat.cpp */
int		mdFormat_initD( char *configDir );

int		mdFormat_init( char *file );

void	mdFormat_close();



int	mdFormat_id( const char *file );

int	mdFormat_mime_id( char *mime );

int	mdFormat_mimeA_id( char *mimeA );

char *	mdFormat_extension( int id );

char *	mdFormat_mime( int id );

int	mdFormat_is_valid( int id );

int	mdFormat_index2id( int index );

int	mdFormat_media( int id );

int	mdFormat_media_id( char *media );

int		mdForamt_mediaString2mask( char *mediaStr );


	/* MdFormatXml.cpp */
int		mdFormat_read_format_file( char *formatFile );



#ifdef __cplusplus
}
#endif

#endif
