/**********************
 ***   VimJvm.cpp   ***
 **********************/
#include	<string.h>

extern "C" {
#include	"Uigp/igp.h"
}



#include "VimJvm.h"

#pragma comment( lib, "jvm.lib" )





char * CVimJvm::m_classPath = NULL;
JNIEnv * CVimJvm::m_env = NULL;
JavaVM * CVimJvm::m_vm = NULL;


CVimJvm::CVimJvm() 
{
	m_env = NULL;
	m_vm = NULL;
	m_classPath = NULL;

} 






CVimJvm::~CVimJvm()
{
	if( m_classPath != NULL ){
		free( m_classPath );
		m_classPath = NULL;
	}
}


int	CVimJvm::AddClassPath( char *dir, char *className )
{
char	classPath[2048];

	sprintf( classPath, "%s/%s", dir, className );

	return AddClassPath( classPath );
}



int	CVimJvm::AddClassPath( char *classPath )
{
	if( m_classPath == NULL ){
		m_classPath = strdup( classPath );
		return( 1 );
	}

	int len = strlen( m_classPath ) + strlen( classPath );

	char *s = (char *)malloc( len + 2 );
#ifdef WIN32
	sprintf( s, "%s;%s", m_classPath, classPath );
#else
	sprintf( s, "%s:%s", m_classPath, classPath );
#endif

	free( m_classPath );
	m_classPath = s;

	return( 1 );
}

		

int CVimJvm::LoadJvm()
{
char	*libPath;
int	ret;

	if( m_env != NULL )	return( 0 );

	JavaVMInitArgs vm_args;
	JavaVMOption options[3];
	
	libPath = ( char * )malloc( strlen( m_classPath ) + 25 );

	sprintf( libPath, "-Djava.class.path=%s", m_classPath );
	
	options[0].optionString = "-Djava.compiler=NONE";	/* disable JIT */	
	options[1].optionString = libPath;					/* user classes */
	options[2].optionString = "-verbose:gc,class,jni";	/* do not print JNI-related messages */

	vm_args.version = JNI_VERSION_1_2;
	vm_args.options = options;
#ifdef WIN32
	vm_args.nOptions = 3;
#else
	vm_args.nOptions = 2;
#endif
	vm_args.ignoreUnrecognized = true;

/* Note that in the Java 2 SDK, there is no longer any need to call 
 * JNI_GetDefaultJavaVMInitArgs. 
 */
	ret = JNI_CreateJavaVM( &m_vm, (void **)&m_env, &vm_args);

	free( libPath );
	return( ret );
}




