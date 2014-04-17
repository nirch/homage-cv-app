
#ifndef WIN32
#include	<sys/times.h>
#else
#include	<time.h>
#endif

#include	<sys/timeb.h>


#include   "Uigp/igp.h"
#include   "GpTime.h"


#include   <stdio.h>

#ifdef  __APPLE__
#import <Foundation/Foundation.h>
#endif


gpTime_type *
gpTime_alloc()
{
gpTime_type *gt;

	gt = ( gpTime_type *)malloc( sizeof(gpTime_type) );

	gt->sec = 0;

	gt->no = 0;

	return( gt );
}


void 
gpTime_destroy( gpTime_type *gt )
{
	free( gt );
}



void
gpTime_init( gpTime_type *t )
{

	t->sec = 0;

	t->no = 0;
}


void
gpTime_start( gpTime_type *t )
{ 
#ifdef WIN32
struct timeb    tp;

	ftime( &tp );
	t->sec0 = tp.time;
	t->mil_sec0 = tp.millitm;
#endif

#ifdef  __APPLE__
    NSDate *time = [NSDate date];
    t->sec0 = ((float)time.timeIntervalSince1970) / 1000.0;
    t->mil_sec0 = time.timeIntervalSince1970;
#endif
}


void
gpTime_stop( gpTime_type *t )
{

int	sec,	mil_sec;

#ifdef WIN32
struct timeb    tp;
	ftime( &tp );
	sec = tp.time;
	mil_sec = tp.millitm;
#endif

#ifdef  __APPLE__
    NSDate *time = [NSDate date];
	sec = ((float)time.timeIntervalSince1970) / 1000.0;
	mil_sec = time.timeIntervalSince1970;
#endif

	t->sec += sec - t->sec0 + ((int)mil_sec - t->mil_sec0)/1000.0;

	t->no ++;
}





void 
gpTime_print( FILE *fp, char *string, gpTime_type *t )
{
	if( t->no == 0 )	return;

	fprintf( fp, "%-10s  time:  ", string);

	fprintf( fp, "   %.2fs", t->sec );
	if( t->no > 0 )
//		fprintf( fp, "   spf: %.4fs(%d)", t->sec / t->no, t->no );
		fprintf( fp, "   spf: %.4fs(%d)   fps: %.4f(%d)", t->sec / t->no, t->no, t->no/t->sec, t->no );
	fprintf( fp, "\n");
}



void 
gpTime_log( LogCallback logCallback, char *string, gpTime_type *t )
{
	if( t->no == 0 )	return;

	(*logCallback)( "%-10s  time:  ", string);

	(*logCallback)( "   %.2fs", t->sec );
	if( t->no > 0 )
		(*logCallback)( "   spf: %.4fs(%d)   fps: %.4f(%d)", t->sec / t->no, t->no, t->no/t->sec, t->no );
	(*logCallback)("\n");
}






int
gp_base_time()
{
#ifdef WIN32
struct	timeb	tp;

	ftime( &tp );
	return( tp.time );
#endif

#ifdef  __APPLE__
    NSDate *time = [NSDate date];
    return time.timeIntervalSince1970;
	//return( 0 );
#endif

}


int
gp_time( int base_time )
{
#ifdef WIN32
struct	timeb	tp;
int	sec;

	ftime( &tp );
	sec = tp.time - base_time;


	return( sec *1000 + tp.millitm );
#endif

#ifdef  __APPLE__
    NSDate *time = [NSDate date];
    int sec = time.timeIntervalSince1970 - base_time;
    return sec;
	//return( 0 );
#endif
}

#ifdef WIN32
char * 
gp_dateTime_iso681( char *buf, time_t timeStamp )
{
time_t ltime;
struct tm *gmt;//,	*timeInfo;

//	time(&ltime);
	ltime = timeStamp;
	if( ltime == 0 )
		ltime = time(0);
    gmt = localtime(&ltime);
//	gmt = gmtime( &ltime );

	sprintf( buf, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
				1900+gmt->tm_year, gmt->tm_mon+1, gmt->tm_mday,
				gmt->tm_hour, gmt->tm_min, gmt->tm_sec );

	return( buf );
}


char * 
gp_time_iso681( char *buf, time_t timeStamp )
{
struct tm *gmt;//,	*timeInfo;

	gmt = localtime((time_t*)&timeStamp);
//	gmt = gmtime( (time_t*)&timeStamp );

	sprintf( buf, "%.2d:%.2d:%.2d",
				gmt->tm_hour, gmt->tm_min, gmt->tm_sec );
	return( buf );
}



char * 
gp_time_string( char *buf, int Fhour )
{
time_t ltime;
struct tm *m_timeInfo;

	time(&ltime);
    m_timeInfo = localtime(&ltime);


	if( Fhour == 0 ){
		sprintf( buf, "%.2d-%.2d",
					m_timeInfo->tm_mon+1,
					m_timeInfo->tm_mday );
		return( buf );
	}


	sprintf( buf, "%.2d-%.2d-%.2d",
				m_timeInfo->tm_mon+1,
				m_timeInfo->tm_mday,
				m_timeInfo->tm_hour );

	return( buf );
}


char * 
	gpTime_string( char *buf, int ltime, int flag )
{
	//	time_t ltime;
	struct tm *m_timeInfo;


	//	time(&ltime);

	if( ltime == 0 )
		ltime = time(0);

	m_timeInfo = localtime((time_t *)&ltime);


	if( flag == 1 ){
		sprintf( buf, "%.2d-%.2d",
			m_timeInfo->tm_mon+1,
			m_timeInfo->tm_mday );
		return( buf );
	}

	if( flag == 2 ){
		sprintf( buf, "%.2d-%.2d-%.2d",
			m_timeInfo->tm_mon+1,
			m_timeInfo->tm_mday,
			m_timeInfo->tm_hour );
	}

	if( flag == 3 ){
		sprintf( buf, "%.2d-%.2d-%.2d-%.2d",
			m_timeInfo->tm_mon+1,
			m_timeInfo->tm_mday,
			m_timeInfo->tm_hour,
			m_timeInfo->tm_min );
	}

	sprintf( buf, "%.2d-%.2d-%.2d-%.2d-%.2d",
		m_timeInfo->tm_mon+1,
		m_timeInfo->tm_mday,
		m_timeInfo->tm_hour,
		m_timeInfo->tm_min,
		m_timeInfo->tm_sec );

	return( buf );
}

int
gpTime_date( time_t ltime )
{
struct tm *t;
int	date;

	if( ltime == 0 )
		time(&ltime);

	t = localtime(&ltime);

	date = (t->tm_year + 1900 )*10000 + (t->tm_mon+1) * 100 + t->tm_mday;

	return( date );
}
#endif