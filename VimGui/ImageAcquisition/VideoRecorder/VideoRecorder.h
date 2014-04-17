
#ifndef _VIDEO_RECORD_
#define _VIDEO_RECORD_


#include "VideoRecorderI.h"
#include "ImageType/ImageType.h"



class CVideoRecorder : public CVideoRecorderI
{
public:
	CVideoRecorder();
	virtual ~CVideoRecorder();





	void SetDir( char *dir );

	char *GetDir( ) { return m_recoedDir; };


	void SetCompression( int compression );
	void SetCompression( char *compression );
	char * GetComporession( );

	int IsRecordOn()		{ return( m_aviWriter != NULL ); }
	int Start( char *fname = NULL );
	int Stop();

	int RecordFrame( image_type *im );
	


	int RecordImage( image_type *im );



public:

	int		m_record;
	char	m_recoedDir[256];

	int		m_recordId;

	int	m_compression;
	char m_compressionName[64];


	class CAviWriter	*m_aviWriter;
};


#endif 



