/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef __USYNCH_H__
#define __USYNCH_H__


/** 
 * return values on sinch operations
 */
#define SYNCH_OK     0
#define SYNCH_ERR   -1

#if defined WIN32 
/**
  * System includes
  */
#include <Windows.h>

#if defined __cplusplus 
extern "C" {
#endif // __cplusplus 

/**
  * Since Windows rise exceptions we use our wrappers to get 
  * right return value
  */
int synch_init( LPCRITICAL_SECTION );
int synch_enter( LPCRITICAL_SECTION );

#if defined __cplusplus 
}
#endif //__cplusplus 

/**
  * Macros that normalize return value
  */
#define SYNCH_RETURN(EXP)       (EXP,SYNCH_OK) 

/**
  * The subject
  */
#define SYNCH_OBJECT              CRITICAL_SECTION
#define SYNCH_INIT( objPtr )      synch_init( objPtr )
#define SYNCH_FINI( objPtr )      SYNCH_RETURN(DeleteCriticalSection( objPtr ))
#define SYNCH_ACQUIRE( objPtr )   synch_enter( objPtr )
#define SYNCH_RELEASE( objPtr )   SYNCH_RETURN(LeaveCriticalSection( objPtr ))

#else  // SOLARIS

/**
  * Macro that normalize return value
  */
#define SYNCH_RETURN( EXP )     ((EXP) == 0 ? SYNCH_OK : SYNCH_ERR )

/**
  * System includes
  */
#include <thread.h>
#include <synch.h>

/**
  * The subject
  */
#define SYNCH_OBJECT             mutex_t   
#define SYNCH_INIT( objPtr )     SYNCH_RETURN(mutex_init( objPtr, USYNC_THREAD, 0L )) 
#define SYNCH_FINI( objPtr )     SYNCH_RETURN(mutex_destroy( objPtr ))
#define SYNCH_ACQUIRE( objPtr )  SYNCH_RETURN(mutex_unlock( objPtr ))
#define SYNCH_RELEASE( objPtr )  SYNCH_RETURN(mutex_lock( objPtr ))

#endif // WIN32
#endif // __USYNCH_H__