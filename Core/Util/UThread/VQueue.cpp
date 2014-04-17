
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
}


CVQueue::~CVQueue( void )
{

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

	return 1;
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




