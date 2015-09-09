
#ifndef WIN32
#include	<sys/times.h>
#else
#include	<time.h>
#include	<sys/timeb.h>
#endif




#include   "Uigp/igp.h"
#include   "GpTime.h"


#include   <stdio.h>


gpTime_type *
gpTime_alloc()
{
gpTime_type *gt;

	gt = ( gpTime_type *)malloc( sizeof(gpTime_type) );

	gt->sec = 0;

	gt->no = 0;

	gt->t0 = vTime();

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
gpTime_initM( gpTime_type *gt, int modulo )
{
	if( (gt->no%modulo) != 0 )
			return;

	gt->sec = 0;

	gt->no = 0;
}



void
gpTime_start( gpTime_type *gt )
{ 
#ifdef _AA_
struct timeb    tp;
	ftime( &tp );
	gt->sec0 = tp.time;
	gt->mil_sec0 = tp.millitm;
#endif

	vTime_type vt = vTime();
	gt->sec0 = vt/1000;
	gt->mil_sec0 = vt%1000; 


	if( gt->no == 0 )
		gt->t0 = vTime();
}


void
gpTime_set( gpTime_type *gt )
{ 
#ifdef _AA_
	struct timeb    tp;
	ftime( &tp );
	t->sec0 = tp.time;
	t->mil_sec0 = tp.millitm;
#endif

	vTime_type vt = vTime();
	gt->sec0 = vt/1000;
	gt->mil_sec0 = vt%1000; 
}




int
gpTime_stop( gpTime_type *t )
{
int	sec,	mil_sec;

#ifdef _AA_
struct timeb    tp;

	ftime( &tp );
	sec = tp.time;
	mil_sec = tp.millitm;
#endif

	vTime_type vt = vTime();
	sec = vt/1000;
	mil_sec = vt%1000; 



	t->ms = sec*1000 + mil_sec - (t->sec0 * 1000 + t->mil_sec0);


	t->sec += sec - t->sec0 + ((int)mil_sec - t->mil_sec0)/1000.0;

	t->no ++;

	return( t->ms );
}



float 
gpTime_fps( gpTime_type *t )
{
	if( t->sec == 0 )
		return( 0 );

	return( t->no / t->sec );
}

int 
gpTime_fpm( gpTime_type *t )
{
	if( t->sec == 0 )
		return( 0 );

	return( 1000*t->no / t->sec );
}

int 
gpTime_mpf( gpTime_type *t )
{
	if( t->sec == 0 )
		return( 0 );

	return( 1000*t->sec / t->no );
}

int 
gpTime_afps( gpTime_type *t )
{
float afps;

//	if( t->sec == 0 )
//		return( 0 );

	afps = t->no*1000.0 / ( vTime() - t->t0);

	return( afps );
}

void 
gpTime_print( FILE *fp, char *string, gpTime_type *t )
{
float afps;

	if( t->no == 0 )	return;

	fprintf( fp, "%-10s time:  %.2fs", string, t->sec );

//	fprintf( fp, "   %.2fs", t->sec );

	afps = t->no*1000.0 / ( vTime() - t->t0);
	fprintf( fp, "   spf: %.4fs   fps: %.4f   afps: %.4f  (%d)", t->sec / t->no, t->no/t->sec, afps, t->no );

	fprintf( fp, "\n");
}


int 
gpTime_sprint( char *msg, char *string, gpTime_type *t )
{
int	n;
float	afps;

	if( t->no == 0 )	return( -1 );

	n = sprintf( msg, "%-10s time:  %.2fs", string, t->sec );


	afps = t->no*1000.0 / ( vTime() - t->t0);
	n += sprintf( &msg[n], "   spf: %.4fs   fps: %.2f   afps: %.2f  (%d)", t->sec / t->no, t->no/t->sec, afps, t->no );


//	n += sprintf( &msg[n], "   spf: %.4fs(%d)   fps: %.4f(%d)", t->sec / t->no, t->no, t->no/t->sec, t->no );

	n += sprintf( &msg[n], "\n" );

	return( 1 );
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
gpTime_base()
{
#ifdef _AA_
struct	timeb	tp;

	ftime( &tp );
	return( tp.time );
#endif

	
	vTime_type vt = vTime();
	int sec = vt/1000;
	
	return( sec );

}


int
gpTime( int base_time )
{
#ifdef _AA_
struct	timeb	tp;
int	sec;

	ftime( &tp );
	sec = tp.time - base_time;


	return( sec *1000 + tp.millitm );
#endif

	vTime_type vt = vTime();
	vt -= base_time *1000;
	return( vt );
}

vTime_type
vTime()
{
vTime_type	vt;

#ifdef WIN32
	struct	timeb	tp;
	ftime( &tp );
	vt = tp.time * 1000 +  tp.millitm;
#else
#ifndef __pnacl__
    #ifndef __APPLE__
        struct timeval tv;
        struct timezone tz;
        gettimeofday( &tv, &tz);
        vt = tv.tv_sec * 1000 +  (tv.tv_usec/1000);
    #endif
#endif
#endif


	return( vt );
}


#ifdef WIN32
int 
vTime_scheduler( int type )
{
	struct tm *gmt;
	int	vs;

	
	time_t timeStamp = time(0);

	gmt = localtime((time_t*)&timeStamp);

	vs = 0;
	if( type >= 2 )
		vs = 100*vs + gmt->tm_hour;

	if( type >= 1 )
		vs = 100*vs + gmt->tm_min;

	if( type >= 0 )
		vs = 100*vs + gmt->tm_sec;

	return( vs );
}

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
	gpTime_date_iso681( char *buf, time_t timeStamp )
{
	time_t ltime;
	struct tm *gmt;//,	*timeInfo;

	//	time(&ltime);
	ltime = timeStamp;
	if( ltime == 0 )
		ltime = time(0);
	gmt = localtime(&ltime);
	//	gmt = gmtime( &ltime );

	sprintf( buf, "%.4d-%.2d-%.2d",
		1900+gmt->tm_year, gmt->tm_mon+1, gmt->tm_mday );

	return( buf );
}

char * 
gp_time_iso681( char *buf, time_t timeStamp )
{
struct tm *gmt;//,	*timeInfo;

	if( timeStamp == 0 )
		timeStamp = time(0);

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
gpTime_string( char *buf, vTime_type ltime, int flag )
{
//	time_t ltime;
	struct tm *m_timeInfo;


//	time(&ltime);

	if( ltime == 0 )
		ltime = time(0);

//	time( &ltime );

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
		return( buf );
	}

	if( flag == 3 ){
		sprintf( buf, "%.2d-%.2d-%.2d-%.2d",
			m_timeInfo->tm_mon+1,
			m_timeInfo->tm_mday,
			m_timeInfo->tm_hour,
			m_timeInfo->tm_min );
		return( buf );
	}



	if( flag == 4 ){
		sprintf( buf, "%.2d-%.2d-%.2d",
			m_timeInfo->tm_mday,
			m_timeInfo->tm_hour,
			m_timeInfo->tm_min );
		return( buf );
	}

	sprintf( buf, "%.2d-%.2d-%.2d-%.2d-%.2d",
		m_timeInfo->tm_mon+1,
		m_timeInfo->tm_mday,
		m_timeInfo->tm_hour,
		m_timeInfo->tm_min,
		m_timeInfo->tm_sec );

	return( buf );
}



char * 
	gpTime_iso681( char *buf, time_t timeStamp )
{
	struct tm *gmt;//,	*timeInfo;


	if( timeStamp == 0 )
		timeStamp = time(0);

	gmt = localtime((time_t*)&timeStamp);
	//	gmt = gmtime( (time_t*)&timeStamp );

	sprintf( buf, "%.2d:%.2d:%.2d",
		gmt->tm_hour, gmt->tm_min, gmt->tm_sec );
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


char *
gpTime_asctime( char *str, vTime_type vt )
{
	struct tm *gmt;



	time_t ltime;
	int	ms;


	ltime = vt / 1000;
	ms = vt % 1000;
	
	gmt = localtime ( &ltime );

//	sprintf( str, "%s m: %d\n", asctime (timeinfo), ms );

	sprintf( str, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d [%3d]",
		1900+gmt->tm_year, gmt->tm_mon+1, gmt->tm_mday,
		gmt->tm_hour, gmt->tm_min, gmt->tm_sec, ms );


	return( str );
}



#endif






void
vTimer_init( vTimer_type *vt, float mpt )
{
	vt->t0 = vTime();

	vt->mpt = mpt;
	
	vt->i = 0;
}


int
vTimer_test( vTimer_type *vt, vTime_type t )
{
int	i;

	if( t == 0 )
		t = vTime();

	i = (t - vt->t0 ) /  vt->mpt;

	if( i <= vt->i )
		return( -1 );

	vt->i = i;
	return( vt->i );
}


