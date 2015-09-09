
#include	<string.h>
#include	"Uigp/igp.h"

#include "VQueueFile.h"



CVQueueFile::CVQueueFile( int no ) : CVQueue( no )
{

	m_fp = NULL;
}


CVQueueFile::~CVQueueFile( void )
{

	if( m_fp != NULL )
		fclose( m_fp );

}



int CVQueueFile::Init( char *file, int skip )
{
	
	if( (m_fp = fopen( file, "rb" ) ) == NULL )
		return( -1 );

	strcpy( m_file, file );


	if( skip == 1 ){
		int ret = SkipEmpty( m_fp );
		return( ret );
	}

	

	return( 1 );

}



int CVQueueFile::Get( void *data[], int *nData )
{

	if( m_fp == NULL )
		return( -1 );

	int ret = Read( data, nData, m_fp );

	if( ret < 0 ){
		fclose( m_fp );

		NextFile( m_file );
		if( (m_fp = fopen( m_file, "rb" ) ) == NULL )
			return( -1 );

		ret = Read( data, nData, m_fp );
	}

	return( ret );
}





