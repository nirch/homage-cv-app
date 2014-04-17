/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef		_GP_PROGRESS_
#define		_GP_PROGRESS_

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*ProgressCallback)( void *data, int iStep, int nStep );


typedef struct gpProgress_type {
	ProgressCallback callback;

	void *data;

} gpProgress_type;


gpProgress_type *	gpProgress_create( ProgressCallback progressCallback, void *data );

void gpProgress_destroy( gpProgress_type *p );



#define GPPROGRESS( p, iStep,  nStep )	{ if( p != NULL )	(*p->callback)( p->data, iStep, nStep ); }

#ifdef __cplusplus
}
#endif
#endif