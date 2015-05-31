//  Defines the entry point for the console application.
//
#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include "Ulog/Log.h"
#include "UThread/VConditionMutex.h"


#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"





#include "HrSourceLive.h"





CHrSourceLive::CHrSourceLive()
{
	m_mutex = new CVConditionMutex();


	m_im = NULL;


}

CHrSourceLive::~CHrSourceLive()
{
	DeleteContents();
}





void CHrSourceLive::DeleteContents()
{
	if( m_mutex != NULL ){
		delete m_mutex;
		m_mutex = NULL;
	}

	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}
}

int CHrSourceLive::Init( int nFrame, char *inFile )
{



	m_aim = imageA_alloc( nFrame );

	m_iImage = 0;

	m_iFrame = 0;

	return( 1 );


}

int	CHrSourceLive::AddFrame( image_type *sim, int iFrame )
{
int	i,		j;

	m_mutex->Lock();



	if( m_aim->nA >= m_aim->NA ){
		for( i = m_iImage, j = 0 ; i < m_aim->nA ; i++, j++ )
			m_aim->a[j] = m_aim->a[i];

		m_aim->nA = j;
	}

	if( m_aim->nA >= m_aim->NA ){
		GPLOG( ("CHrSourceLive: out of range") );
		m_mutex->Unlock();
		return( -1 );
	}
	
	m_aim->a[m_aim->nA++] = sim;

	m_mutex->Signal();
	m_mutex->Unlock();

	return( 1 );
}


int	CHrSourceLive::ReadFrame( int iFrame, image_type **im )
{

	m_mutex->Lock();


	if( m_iImage >= m_aim->nA ){
		if( m_mutex->Wait() < 0 ){
			m_mutex->Unlock();
			return( -1 );
		}
	}


	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}

	m_im = m_aim->a[m_iImage];
	m_aim->a[m_iImage] = NULL;
	m_iImage++;


	m_mutex->Unlock();

	m_iFrame = iFrame;

	*im = m_im;


	return( 1 );
}


int	CHrSourceLive::Close()
{
		DeleteContents();

		return( 1 );
}

