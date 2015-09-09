
#include	<string.h>
#include	"Uigp/igp.h"

#include "VPipeTask.h"



CVPipeTask::CVPipeTask()
{


}

CVPipeTask::~CVPipeTask( void )
{

}

int CVPipeTask::Start()
{

	m_semIn.Create( NULL, 0, 1, NULL);

	m_semOut.Create( NULL, 0, 1, NULL);
	
	StartThread();

	return 1;
}


int CVPipeTask::SetIn()
{

	m_semIn.Release( 1 );

	return 1;
}


//int CVPipeTask::WaitForIn()
//{
//
//	m_semIn.Release( 1 );
//
//	return 1;
//}


int CVPipeTask::WaitForOut()
{

	m_semOut.WaitForSingleObject();

	return 1;
}


void CVPipeTask::Run()
{

	while( 1 ) {
		m_semIn.WaitForSingleObject();

//		(process)*
		m_semOut.Release( 1 );
	}
}



