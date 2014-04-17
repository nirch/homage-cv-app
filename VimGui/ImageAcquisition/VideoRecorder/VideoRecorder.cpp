// SklData.cpp: implementation of the CVideoRecorder class.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include "VideoRecorder.h"

#ifdef _DEBUG
#define _DUMP
#endif

#include "Uigp/igp.h"
#include "ImageDump/ImageDump.h"




#include "Avi\Avi\/AviWriter/AviWriter.h"



CVideoRecorder::CVideoRecorder()
{
	m_aviWriter = NULL;

	m_width = 640;
	m_height = 490;

	strcpy( m_compressionName, "INDEO50");
	m_compression = AVI_CODEC_INDEO50;


}



CVideoRecorder::~CVideoRecorder()
{


//	DeleteContents();

}

void CVideoRecorder::SetCompression( int compression )
{
	m_compression = compression;
	
	strcpy( m_compressionName, "UNKNOWN");

	if( m_compression == AVI_CODEC_RAW )
		strcpy( m_compressionName, "RAW");

	if( m_compression == AVI_CODEC_INDEO50 )
		strcpy( m_compressionName, "INDEO50");

}


void CVideoRecorder::SetCompression( char *compression )
{
	if( gp_stricmp( compression, "INDEO50") == 0 ){
		strcpy( m_compressionName, "INDEO50");
		m_compression = AVI_CODEC_INDEO50;
		return;
	}

	if( gp_stricmp( compression, "RAW") == 0 ){
		strcpy( m_compressionName, "RAW");
		m_compression = AVI_CODEC_RAW;
		return;
	}

	strcpy( m_compressionName, "INDEO50");
	m_compression = AVI_CODEC_INDEO50;

}


char * CVideoRecorder::GetComporession( )
{
	return( m_compressionName );
}


void CVideoRecorder::SetDir( char *dir )
{
	sprintf( m_recoedDir, "%s", dir );

	gpDir_force_exist( m_recoedDir );

	m_recordId = 0;
}


//
//void CVideoRecorder::SetFrameSize( int width, int height )
//{
//	m_width = width;
//	m_height = height;
//}


int CVideoRecorder:: Start( char *fname )
{
	
//	char file[256];
	char bfile[256];

	if( fname == NULL ){
		sprintf( bfile, "%s/rec-%%.2d.avi", m_recoedDir );
		m_recordId = gpFile_unuse_id( bfile, m_recordId, m_file );
	}
	else	gpFilename_force_extension( fname, ".avi", m_file );


//	m_aviWriter = new CAviWriter( AW_ASYNCHRONY );
	m_aviWriter = new CAviWriter( AW_ASYNCHRONY_THREAD );




	if( m_aviWriter->Open( m_width, m_height, 10, 
				/*AVI_CODEC_RAW, //*/ //AVI_CODEC_INDEO50,//
				m_compression,
				m_file ) < 0 )
				return( -1 );


	return( 1 );

}

int CVideoRecorder:: Stop()
{

	if( m_aviWriter == NULL )	return( -1 );

	m_aviWriter->Close();

	delete m_aviWriter;

	m_aviWriter = NULL;

	return( 1 );
}



int CVideoRecorder:: RecordFrame( image_type *im )
{

	if( m_aviWriter == NULL )	return( -1 );

	m_aviWriter->WriteFrame( im );

	return( 1 );
}





int CVideoRecorder:: RecordImage( image_type *im )
{
char file[256];
char bfile[256];

	sprintf( bfile, "%s/im-%%.2d.bmp", m_recoedDir );
	m_recordId = gpFile_unuse_id( bfile, m_recordId, file );
	
	image_write_bmp( im, file );

	return( 1 );


	return( 1 );
}
