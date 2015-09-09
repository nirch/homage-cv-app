// Configurator.h: interface for the CConfigurator class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __VIM_JVM_H__
#define __VIM_JVM_H__





#include <jni.h>


class CVimJvm
{
public:
	CVimJvm();
	virtual ~CVimJvm();

	int	LoadJvm();

	int	AddClassPath( char *dir, char *className );

	int	AddClassPath( char *classPath );


public:
	static char *m_classPath;
	static JNIEnv *m_env;
	static JavaVM *m_vm;
};



#endif



