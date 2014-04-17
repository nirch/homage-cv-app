/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef		_GP_PROGRESS_
#define		_GP_PROGRESS_

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*ProgressCallback)( void *data, int progress, int iStep, int nStep );


typedef struct gpProgress_type {
	ProgressCallback callback;

	void *data;

	int progress;
	int iPhase;
	int nPhase;

} gpProgress_type;


gpProgress_type *	gpProgress_create( ProgressCallback progressCallback, void *data );

void gpProgress_destroy( gpProgress_type *p );

void	gpProgress( gpProgress_type *p, int progress, int iPhase, int nPhase );


//#define GPPROGRESS( p, progress, iStep,  nStep )	{ if( p != NULL )	(*p->callback)( p->data, progress, iStep, nStep ); }

#define GPPROGRESS( p, progress, iStep,  nStep )	gpProgress( p, progress, iStep, nStep )


#ifdef __cplusplus
}
#endif
#endif