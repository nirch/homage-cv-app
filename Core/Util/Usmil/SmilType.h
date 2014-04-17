/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef __SMIL_TYPE__
#define __SMIL_TYPE__

#include "Uxml/XmlType.h"





#define	MAX_REGION_ID	512
#define	MAX_SRC_NAME	64

#define MAX_PRM_NAME	256
#define MAX_PRM_VALUE	256

//smilSrc.type
#define	SMIL_ATTACH_IMAGE	1
#define	SMIL_ATTACH_AUDIO	2
#define	SMIL_ATTACH_TEXT	3
#define	SMIL_ATTACH_VIDEO	4



//smilAttach.reference
#define	SMIL_ATTACH_LOACTION	0
#define	SMIL_ATTACH_ID			1



typedef struct smilRegion_type 
{
	char id[MAX_REGION_ID];

	int height;
	int	width;
	int	left;
	int	top;
	int	z;

}	smilRegion_type;



typedef struct smilParam_type 
{
	char name[MAX_PRM_NAME];
	char value[MAX_PRM_VALUE];

} smilParam_type;



typedef struct smilAttach_type 
{
	int	type;
	char src[MAX_SRC_NAME];
	int	reference;

	char regId[MAX_REGION_ID];
	int width;
	int height;

	int	begin,	end;


	int	paramNo;
	smilParam_type	**param;

} smilAttach_type;



typedef struct smilPar_type 
{
	int	duration;

	int	attachNo;
	smilAttach_type	**attach;

}	smilPar_type;


typedef struct smilSeq_type 
{
	int	duration;

	int	parNo;
	smilPar_type	**par;

}	smilSeq_type;


typedef struct smil_type 
{
	int width;
	int height;

	char background_color[MAX_PRM_VALUE];


	int	regionNo;
	smilRegion_type	**region;

	int	seqNo;
	smilSeq_type	**seq;

}	smil_type;




smil_type *	smil_alloc();

smilRegion_type	*	smilRegion_alloc();

smilPar_type *	smilPar_alloc();

smilSeq_type *	smilSeq_alloc();

smilAttach_type *	smilAttach_alloc();

smilParam_type *	smilParam_alloc();



void	smil_destroy( smil_type *smil );

void	smilSeq_destroy( smilSeq_type *seq );

void	smilPar_destroy( smilPar_type *par );

void	smilAttach_destroy( smilAttach_type *att );




//smilAttach_type	*	smil_get_attach( smil_type *smil, char *src );
smilAttach_type	*	smil_get_attach( smil_type *smil, char *id, char *location );

smilRegion_type	*	smil_get_region( smil_type *smil, char *src );

int	smil_get_srcNames( smil_type *smil, char *aFile[], int NO );


	// Add
smil_type *	smil_create( int regionNo, int seqNo );

smilRegion_type *	smil_add_region( smil_type *smil,
									char *id, int width, int height, int top, int left );

smilSeq_type *	smil_add_seq( smil_type *smil, int parNo );

smilPar_type *	smilSeq_add_par( smilSeq_type *smil, int duration, int attachNo );

smilAttach_type *	smilPar_add_attach( smilPar_type *par, int type, char *id, char *location, char *regId );


int	smilAttach_set_src( smilAttach_type *attach, char *id, char *location );

int	smil_get_attachs( smil_type *smil, smilAttach_type *attach[], int NO );




	/* SmilParser.c */

smil_type *	smil_parser_file( char *file );

smil_type *	smil_parser_buffer( char *data, int bytes );

smil_type *	smil_parser( xml_type *doc );

int	smil_parser_get_srcName( char *data, int bytes, char *aFile[], int NO );



	/* SmilBuild.c */
int	smil_build_file( smil_type *smil, char *file );

int	smil_build_buffer( smil_type *smil, char **data, int *bytes );


	/* SmilUnion.c */
smil_type	*	smil_union( smil_type *s0, smil_type *s1 );




#endif
