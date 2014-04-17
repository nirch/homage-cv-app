/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef         _GPMEMORY_LEAK_H_
#define         _GPMEMORY_LEAK_H_

#ifdef __cplusplus
extern "C" {
#endif





#ifdef _GPMEMORY_LEAK 
#define GPMEMORY_LEAK_ALLOC( im )		gpMemory_leak_alloc( im )

#define GPMEMORY_LEAK_DESTROY( im )	gpMemory_leak_destroy( im )

#define GPMEMORY_LEAK_PRINT( fp )		gpMemory_leak_print( fp )
#else
#define GPMEMORY_LEAK_ALLOC( im )	

#define GPMEMORY_LEAK_DESTROY( im )

#define GPMEMORY_LEAK_PRINT( fp )	
#endif



void		gpMemory_leak_alloc( void *im );


void		gpMemory_leak_destroy( void *im );

void		gpMemory_leak_print( FILE *fp );


#ifdef __cplusplus
}
#endif

#endif