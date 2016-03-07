
#ifndef _HR_Output_JAVA_H
#define _HR_Output_JAVA_H



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

	int SetJavaOutput( JNIEnv *env, jobject javaOutput );

	int	Open();

	// Overrides
	int WriteFrame( image_type *im, int iFrame );
	int	Close();

private:

	JNIEnv	*m_env;
	jobject m_jOutput;

	jmethodID  m_methodOpen;
	jmethodID  m_methodWrite;
	jmethodID  m_methodClose;

	jbyteArray m_frame;
	jint m_frameLen;
};


#endif


