
#ifndef _VIDEO_RECORDER_
#define _VIDEO_RECORDER_



#include "ImageType/ImageType.h"



class CVideoRecorderI
{
public:
	CVideoRecorderI() { m_file[0] = 0; }
	virtual ~CVideoRecorderI() {};





	virtual void SetDir( char *dir ) {};
	virtual char * GetDir( ) { return NULL; };

	virtual void	SetCompression( int compression ) {};
	virtual void   SetCompression( char *compression ) {};
	virtual char * GetComporession( ) { return NULL; };

	virtual void SetFrameSize( int width, int heght ) { m_width = width; m_height = heght; };



	virtual int IsRecordOn()		{ return( 0 ); }
	virtual int Start( char *fname = NULL )	{ return( -1 ); };
	virtual int Stop()	{ return( -1 ); };

	virtual int RecordFrame( image_type *im )	{ return 1;}
	


	virtual int RecordImage( image_type *im )	{ return 1;}

	char * GetFilename()	{ return( m_file ); }

public:

	int	m_width;
	int	m_height;

	char m_file[256];
};


#endif 



