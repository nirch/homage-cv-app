
#ifndef _HR_Source_JAVA_H
#define _HR_Source_JAVA_H



#include "Ujni/VimJni.h"

#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "HrSourceI.h"





class CHrSourceJava : public CHrSourceI
{
public:
	
	CHrSourceJava();

	virtual ~CHrSourceJava();

	void DeleteContents();

	int SetJavaSource( JNIEnv *env, jobject javaSource );

	int LoadInfo();
	int	Open();

	// Overrides
	int ReadFrame( int iFrame, long long timeStamp, image_type **im );
	int	Close();

private:

	JNIEnv	*m_env;
	jobject m_jSrc;

	jmethodID  m_methodOpen;
	jmethodID  m_methodGetInfo;
	jmethodID  m_methodRead;
	jmethodID  m_methodClose;


	image_type* m_im;

};


#endif


