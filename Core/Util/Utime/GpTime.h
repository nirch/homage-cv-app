/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef         GP_TIME
#define         GP_TIME

#include <stdio.h>




#ifdef __cplusplus
extern "C" {
#endif


#include "Uigp/igp.h"

#ifdef WIN32
	typedef _int64	vTime_type;
#else
	typedef long long vTime_type;
#endif



typedef struct gpTime_type {
	int	sec0,	mil_sec0;
	vTime_type t0;

	float	sec;

	int	no;

	int	ms;	// last step

}	gpTime_type;


typedef struct vTimer_type {
	vTime_type t0;

	float  mpt; 

	int		i;
} vTimer_type;




typedef gpTime_type gp_time_type;


gpTime_type *	gpTime_alloc();

void	gpTime_destroy( gpTime_type *gt );



void	gpTime_init( gpTime_type *t );
#define gp_time_init	gpTime_init

void	gpTime_initM( gpTime_type *gt, int modulo );



void	gpTime_start( gpTime_type *t );
#define gp_time_start	gpTime_start


void	gpTime_set( gpTime_type *t );


int	gpTime_stop( gpTime_type *t );
#define gp_time_stop	gpTime_stop
//#define gp_time_add gp_time_stop

void	gpTime_print( FILE *fp, char *string, gpTime_type *t );
int		gpTime_sprint( char *msg, char *string, gpTime_type *t );

void	gpTime_log( LogCallback logCallback, char *string, gpTime_type *t );

float	gpTime_fps( gpTime_type *t );
int		gpTime_fpm( gpTime_type *t );
int		gpTime_mpf( gpTime_type *t );
int		gpTime_afps( gpTime_type *t );





//#define gp_time_print	gpTime_print


int	gpTime_base();
#define	gp_base_time	gpTime_base


int	gpTime(  int base_time  );
#define	gp_time	gpTime

vTime_type 	vTime();


char *	gp_dateTime_iso681( char *buf, time_t timeStamp );
char *	gp_time_iso681( char *buf, time_t timeStamp );
char *	gp_time_iso681( char *buf, time_t timeStamp );


char *	gp_time_string( char *buf, int Fhour );

char * gpTime_string( char *buf, vTime_type ltime, int flag );


int	gpTime_date( time_t ltime );

char *	gpTime_asctime( char *str, vTime_type vt );




void	vTimer_init( vTimer_type *vt, float mpt );

int		vTimer_test( vTimer_type *vt, vTime_type t );



#ifdef _GPTIME
#define GPTIME_INIT( a )		gpTime_init( a )
#define GPTIME_START( a )		gpTime_start( a )
#define GPTIME_STOP( a )		gpTime_stop( a )
#else
#define GPTIME_INIT( a )
#define GPTIME_START( a )
#define GPTIME_STOP( a )
#endif

#ifdef __cplusplus
}
#endif

#endif
