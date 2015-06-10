/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef         _IMAGE_LEAK_
#define         _IMAGE_LEAK_

#ifdef __cplusplus
extern "C" {
#endif

#include	"ImageType/ImageType.h"




#ifdef _IMAGE_LEAK 
#define IMAGE_LEAK_ALLOC( im )		image_leak_alloc( im )

#define IMAGE_LEAK_DESTROY( im )	image_leak_destroy( im )

#define IMAGE_LEAK_PRINT( fp )		image_leak_print( fp )
#else
#define IMAGE_LEAK_ALLOC( im )	

#define IMAGE_LEAK_DESTROY( im )

#define IMAGE_LEAK_PRINT( fp )	
#endif



void		image_leak_alloc( image_type *im );


void		image_leak_destroy( image_type *im );

void		image_leak_print( FILE *fp );

int			image_leak_no();



#ifdef __cplusplus
}
#endif

#endif