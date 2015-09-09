
#include	<string.h>
#include	"Uigp/igp.h"

#include "VQueue.h"



CVQueue::CVQueue( int no )
{
	m_aq = (qData_type **)malloc( no * sizeof(qData_type) );
	m_NQ = no;

	m_nQ = 0;

	m_semIn.Create( NULL, 0, 1, NULL);

	m_semIn.Release( 1 );


	m_fp = NULL;
}


CVQueue::~CVQueue( void )
{

	if( m_fp != NULL )
		fclose( m_fp );

}

int CVQueue::Add( void *data[], int nData )
{
int	i;
	m_semIn.WaitForSingleObject();

	for( i = 0 ; i < nData && m_nQ < m_NQ ; i++ ){

	//	m_aq[m_nQ++] = CopyData( (qData_type *)data[i] );

		m_aq[m_nQ++] =(qData_type *)data[i];
	}

	m_semIn.Release( 1 );

	return 1;
}

int CVQueue::Add( void *data )
{

	m_semIn.WaitForSingleObject();

	
	m_aq[m_nQ++] = CopyData( (qData_type *)data );


	m_semIn.Release( 1 );

	return 1;
}

int CVQueue::Get( void *data[], int *nData )
{
	int	i;
	m_semIn.WaitForSingleObject();

	for( i = 0 ; i < m_nQ  ; i++ ){

		data[i] = m_aq[i];
	}

	*nData = m_nQ;

	m_nQ = 0;
	m_semIn.Release( 1 );

	Spy( data, *nData );

	return 1;
}


void CVQueue::ReleaseData( void *data[], int nData )
{
	int	i;
	
	for( i = 0 ; i < nData  ; i++ ){
		free( data[i] );
	}
}
	


int CVQueue::SetIn()
{

	m_semIn.Release( 1 );

	return 1;
}


qData_type* CVQueue::CopyData( qData_type *d )
{
	void *c = malloc( d->dataSize );
	memcpy( c, d,  d->dataSize );

	return( (qData_type *)c );
}


int CVQueue::Init( char *dir, char *prefix, int spy, int spyDtime )
{
	strcpy( m_dir, dir );

	strcpy( m_prefix, prefix );

	m_spy = spy;

	m_index = 0;

	if( spyDtime == 0 )	spyDtime = 60;

	m_spyDtime = spyDtime * 1000;

	return( 1 );

}

int CVQueue::Init( char *dir, int id, int spy, int spyDtime )
{
	strcpy( m_dir, dir );

	
	sprintf( m_prefix, "q%d", id );

	m_spy = spy;

	if( spyDtime == 0 )	spyDtime = 60;

	m_spyDtime = spyDtime * 1000;

	return( 1 );

}

int CVQueue::Spy( void *data[], int nData )
{
	

	if( m_spy == 0 )	return( -1 );


	if( m_fp == NULL ){
		char	buf[256];
		gpTime_string( buf, 0, 2 );


		sprintf( m_file, "%s/%s-%s-%.3d.log", m_dir, m_prefix, buf, m_index );

	

		if( ( m_fp = fopen( m_file, "wb" )) == NULL )
			return( -1 );

		m_time = vTime();
	}


	if( vTime() - m_time > m_spyDtime ){
		fclose( m_fp );
		NextFile( m_file );

		if( ( m_fp = fopen( m_file, "wb" )) == NULL )
			return( -1 );

		m_time = vTime();

	} 


	Write( data, nData, m_fp );

	return( 1 );
}


int CVQueue::Dump( void *data[], int nData, char *prefix, int index, char *suffix  )
{
char	file[256];


	if( gpDump_filename( prefix, index, suffix, ".pt", file ) < 0 )
		return( -1 );

	int ret = Write( data, nData, file  );

	return( ret );
}



int CVQueue::Write( void *data[], int nData, char *file )
{
FILE *fp;



	if( ( fp = fopen( file, "wb" )) == NULL )
		return( -1 );


	Write(  data, nData, fp );



	fclose( fp );
	return( 1 );
}


int CVQueue::Write( void *data[], int nData, FILE *fp )
{

	int i;

	fwrite( &nData, 1, 4, fp );
	if( nData == 0 )
		return( 1 );


	qData_type *d = (qData_type *)data[0];


	int size = d->dataSize;
	fwrite( &size, 1, 4, fp );

	for( i = 0 ; i < nData ; i++ )
		fwrite( data[i], size, 1, fp );

	return( 1 );
}


int CVQueue::Read( void *data[], int *nData, char *file )
{
FILE *fp;

	if( ( fp = fopen( file, "rb" )) == NULL )
		return( -1 );


	int ret = Read( data, nData, fp );



	fclose( fp );
	return( 1 );
}


int CVQueue::Read( void *data[], int *nData, FILE *fp )
{
int i;


	if( fread( nData, 1, 4, fp ) < 4 ){
		*nData = 0;
		return( -1 );
	}

	if( *nData == 0 )
		return( 1 );


	int size;
	fread( &size, 1, 4, fp );

	for( i = 0 ; i < *nData ; i++ ){
		 qData_type *d = (qData_type *)malloc( size );

		fread( d, size, 1, fp );

		data[i] = d;
	}

	return( 1 );
}



int CVQueue::SkipEmpty( FILE *fp )
{
	int nData;


	while( 1 ){
		int pose = ftell( fp );

		if( fread( &nData, 1, 4, fp ) < 4 ){
			return( -1 );
		}


		if( nData != 0 ){
			fseek( fp, pose, SEEK_SET );
			return( 1 );
		}
	}

	return( 1 );
}




int CVQueue::NextFile( char *file )
{
	int	i,	k;
	i = gpStr_lastIndexOf( file, '.' );

	for( k = i-3 ; k < i ; k++ ){
		if( file[i-1] < '0' || file[i-1] > '9' )
			return( -1 );
	}

	int	index;
	sscanf( &file[i-3], "%d", &index );

	sprintf( &file[i-3], "%.3d.log", index+1 );

	return( 1 );
}
