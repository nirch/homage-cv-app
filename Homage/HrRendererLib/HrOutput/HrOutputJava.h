
#ifndef _HR_OUTPUT_JAVA_H
#define _HR_OUTPUT_JAVA_H



#include "Ujni/VimJni.h"

#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "HrOutputI.h"





class CHrOutputJava : public CHrOutputI
{
public:
	
	CHrOutputJava();

	virtual ~CHrOutputJava();

	void DeleteContents();

	int Init( char *outFile, int width, int height, int frameRate );

	int Init( char *className, char *outFile, int width, int height, int frameRate );

	int SetJavaEncoder( JNIEnv* env, jobject mainJavaClass );


	virtual int	WriteFrame( image_type *im, int iFrame );

	int WriteFrameA( char *data, int nData );


	int	Close();

private:

//	int	m_iFrame;


	JNIEnv	*m_env;
	jobject m_obj;
	
	char		m_className[256];

	jmethodID  m_methodOpen;
	jmethodID  m_methodWrite;
	jmethodID  m_methodClose;
	jmethodID  m_methodTest;


	//int	m_width;
	//int	m_height;


	char	m_file[256];

//	image_type *m_im;
};


#endif


