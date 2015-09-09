#ifndef _IM_HDR_

#define _IM_HDR_

#include <string.h>

struct IM_HEADER	{
	char	h[4];	/* "GP1 "	*/
	long	column;
	long	row;
	char	image_name[20];
	char	comments[32];
};


#define		IM_HEADER_SIZE	sizeof( struct IM_HEADER )

#define	FILL_IM_HEADER( im_h, c, r, name, comm )	\
	strncpy( im_h.h, "GP1 ", 4 ); \
	im_h.column = c; \
	im_h.row = r; \
	if( name != NULL )	strncpy( im_h.image_name, name, 20 ); \
	if( comm != NULL )	strncpy( im_h.comments, comm, 32 );

#define		IM_IS_GP_BMAP_FILE( im_h )	\
			( strncmp( im_h.h, "GP1 ", 4) == 0 )


#endif  /* _IM_HDR_ */
