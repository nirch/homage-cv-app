/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#include <stdlib.h>


#include "GpProgress.h"


gpProgress_type *
gpProgress_create( ProgressCallback progressCallback, void *data )
{
gpProgress_type *p;

	p = ( gpProgress_type *)malloc( sizeof(gpProgress_type) );

	p->callback = progressCallback;
	p->data = data;

	return( p );
}


void 
gpProgress_destroy( gpProgress_type *gpProgress )
{
	free( gpProgress );
}

