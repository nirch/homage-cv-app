
#include	<string.h>
#include	"Uigp/igp.h"


#ifndef _GPLOG
//#define _GPLOG
#endif

#include "Ulog/Log.h"

#include "../Png/ImageWritePng.h"


#include "ImageType/ImageType.h"


#include "PngWriter.h"


#define		MAX_IM		100


CPngWriter::CPngWriter()
{

	m_aim = imageA_alloc( MAX_IM );

	m_iIm = 0;
	for (int i = 0; i < m_aim->NA ; i++ ){
		m_aim->a[i] = NULL;
	}

	GPLOGF(( "CPngWriter nA: %d", m_aim->nA ));

	m_fp = NULL;

	m_mutex = new CVMutex();
}


CPngWriter::~CPngWriter( void )
{
	if( m_aim != NULL ){
		imageA_destroy( m_aim );
		m_aim = NULL;
	}
}


int CPngWriter::Open( int quality, char *outDir )
{
	strcpy( m_dir, outDir );
	
#ifndef __ANDROID__
	gpDir_force_exist( m_dir );
#endif



	m_quality = quality;
	m_iFrame = 0;


	GPLOGF(( "CPngWriter 1"));

	char  file[256];
	sprintf( file, "%s/time.log", m_dir );
	m_fp = fopen( file, "wb" );
	m_time0 = vTime();

	GPLOGF(( "CPngWriter 2"));
	m_process = 1;

	StartThread();
	
	GPLOGF(( "CPngWriter 3"));

	return 1;
}



int CPngWriter::WriteFrame( image_type *sim )
{
int	i;

	GPLOGF(( "CPngWriter 4"));
	if( m_aim->nA - m_iIm >= m_aim->NA ){
			return( -1 );
	}



	image_type *im;
	if( sim->depth == 3 ){
		im = image4_from( sim, NULL );
		GPLOGF(( "CPngWriter 4"));
	}
	else {
		im = image_copyN( sim, NULL );
		GPLOGF(( "CPngWriter 5"));
	}




	m_mutex->Lock();


	if( m_aim->nA >= m_aim->NA ){
		m_aim->nA = 0;
		for( i = m_iIm  ; i < m_aim->NA ; i++  ){
			m_aim->a[m_aim->nA++]  = m_aim->a[i];
		}
	}

	GPLOGF(( "CPngWriter 6"));


	m_aim->a[m_aim->nA++] = im;
	m_mutex->Unlock();


	return 1;
}




int CPngWriter::Close()
{
	m_process = 0;

	while( m_process != 2 ){
		Sleep( 10 );
	}


//	m_mutex->Lock();


	int	i;
	for( i = m_iIm ; i < m_aim->nA ; i++  ){
		Write( m_aim->a[i] );
		m_aim->a[i] = NULL;
	}

	m_aim->nA = 0;
	m_iIm = 0;

	if( m_fp != NULL )
		fclose( m_fp );



	return 1;
}




	


void CPngWriter::Run()
{
image_type *im;


	while( m_process ) {


		m_mutex->Lock();
		GPLOGF(( "A"));

		if( m_iIm >= m_aim->nA ){
			m_mutex->Unlock();
			Sleep( 10 );
			GPLOGF(( "B"));
			continue;
		}



		im  = m_aim->a[m_iIm];
		m_aim->a[m_iIm] = NULL;
		m_iIm++;

		GPLOGF(( "C"));
		m_mutex->Unlock();


		GPLOGF(( "D"));
		Write( im );
	}


	m_process = 2;
}



void CPngWriter::Write( image_type *im )
{
char	file[256];


	sprintf( file, "%s/im-%.4d.png", m_dir, m_iFrame++ );
	image_write_png_TA( im, file );

	GPLOGF(( "%s", file ));
	if( m_fp != NULL ){
		vTime_type dt = im->timeStamp - m_time0;
		fprintf( m_fp, "%d    %d\n", m_iFrame, (int)dt );
	}

	GPLOGF(( "Write\n" ));
	image_destroy( im, 1 );
}


	
