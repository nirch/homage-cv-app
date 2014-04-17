/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/************************
 ***	MediaType.h   ***
 ************************/
#ifndef         _MEDIA_DATA_TYPE_
#define         _MEDIA_DATA_TYPE_ 

#ifdef __cplusplus
extern "C" {
#endif

#include		"Uigp/igp.h"

	/* mediaData.type */
#define		MA_WAV		1
#define		MT_MIDI		2
#define		MA_WMA		3
#define		MA_MP3		4
#define		MA_AMR		5
#define		MI_BMP		6
#define		MI_JPG		7
#define		MI_GIF		8
#define		MI_PNG		9

#define		MV_VIM		10



typedef struct mediaData_type {

	char	type;

	char	mime[64];

	char	*location;
	char	*id;

	int		bytes;
	char	*data;


	// Aux
	float	sec;

} mediaData_type;




mediaData_type *	mediaData_alloc( int bytes );

void				mediaData_destroy( mediaData_type *md );

void				mediaDataA_destroy(mediaData_type * md[], int nMd );

mediaData_type *	mediaData_create( int mediaType, char *data, int bytes );

mediaData_type *	mediaData_createM( char *mime, char *data, int bytes );






mediaData_type *	mediaData_from_buffer( char *mime, char *id, char *location,
									char *data, int bytes, int Falloc );

mediaData_type *	mediaData_from_file( char *mime, char *id, char *file );


mediaData_type *	mediaData_make_copy( mediaData_type *smd );

mediaData_type *	mediaData_get( mediaData_type *md[], int nMedia, char *id, char *location );

mediaData_type *	mediaData_read_file( char *file );

void	mediaData_write_to_file( mediaData_type *md, char *file );


char *	mediaData_get_extension( mediaData_type *md, char *extension );

void	mediaData_set_location( mediaData_type *md, char *location );

void	mediaData_set_id( mediaData_type *md, char *id );





	/* MediaFormat.c */
int		mediaData_mime2format( char *mime );

char *	mediaData_format2mime( int format, char *mime );

char *	mediaData_mime( mediaData_type *md, char *mime );

int		mediaData_get_format( mediaData_type *md );


char *	mediaData_format2extension( int format, char *extension );

//char *	mediaData_get_extension( char *file, char *extension );

int		mediaData_format_from_extension( char *file );

int		mediaData_format( char *extension );



	// MediaDataMultiPart.c
int		mediaData_multipart_pack( mediaData_type *amd[], int nMd, u_char **data, int *nData );

int		mediaData_multipart_parse( u_char *data, int nData, mediaData_type *amd[], int *nMd );

#ifdef __cplusplus
}
#endif

#endif
