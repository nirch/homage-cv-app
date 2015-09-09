/************************
 ***   MdFormatIn.h   ***
 ************************/

#ifndef		_MD_FORMAT_IN_
#define		_MD_FORMAT_IN_

#include	"Uigp/igp.h"
//#include	"Uigp/M64Type.h"


#define	MAX_EXTENSION	4

#define	MIME_LEN	64



typedef struct mdFormatD_type {
	int	id;

	int	media;

	char *extension;
	char *ext;	// output file extension

	char *mimeA;
	char *mime;

}
mdFormatD_type;



mdFormatD_type *	mdFormatD_alloc();

void				mdFormatD_destroy(mdFormatD_type *	f);

mdFormatD_type *	mdFormatD_create( int id, int media, char *extension, char *mimeA );

int					mdFormat_alloc_id();

int					mdFormat_set( int id, int media, char *extension, char *mimeA );



#endif
