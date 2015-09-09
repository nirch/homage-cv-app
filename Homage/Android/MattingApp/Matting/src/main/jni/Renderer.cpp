#include <jni.h>




#include	"Renderer.h"

#define _GPLOG

#include "Uigp/igp.h"
#include "Ulog/Log.h"

#include	"UjniTool.h"

#include "ImageAcquisition/Android/AndroidCameraAcquisition.h"
#include "MattingLib/MattingTask/MattingTask.h"



#include "HrRendererLib/HomageRenderer.h"
#include "HrRendererLib/HrSource/HrSourceGif.h"
#include "HrRendererLib/HrSource/HrSourcePng.h"


#include "HrRendererLib/HrOutput/HrOutputGif.h"
#include "HrRendererLib/HrOutput/HrOutputJava.h"
#include "HrRendererLib/HrOutput/HrOutputPng.h"

#include "HrRendererLib/HrEffect/HrEffectMask.h"
#include "HrRendererLib/HrEffect/HrEffectPose.h"
#include "HrRendererLib/HrEffect/HrEffectAlignment.h"
#include "HrRendererLib/HrEffect/HrEffectMaskGif.h"


static CHomageRenderer *m_hr = NULL;



CHrSourceI  *	HrSource_create( int type );

CHrEffectI  *	HrEffect_create( int type );




JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer_create
	(JNIEnv *, jclass )
{
	GPLOGF(("<Renderer Create"));

	if( m_hr != NULL )
		return( -1 );

	if( m_hr == NULL ){
		m_hr = new CHomageRenderer();
	}


	GPLOGF((" >\n"));

	return( 1 );
}



JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer_addSource
	(JNIEnv * env, jclass c, jint type, jstring jfile )
{

	GPLOGF(("<addSource"));


	if( m_hr == NULL || m_hr->IsProcess() ){
		GPLOGF(("In process  >\n"));
		return( -1 );
	}


	const char *file = ujni_getJString( env, jfile );



	CHrSourceI  *source = HrSource_create( type );

	if( source == NULL ){
		ujni_releaseJString( env, jfile, file );
		GPLOGF((" >\n"));
		return( -1 );
	}

	GPLOGF((" %s", file ));

	if( source->Init( (char *)file ) < 0 ){
		delete source;
		ujni_releaseJString( env, jfile, file );
		GPLOGF(("%s failed >\n", file ));
		return( -1 );
	}

	GPLOGF((" Init %s", file ));

	GPLOGF((" nS: %d ", m_hr->m_nS ));

	int id = m_hr->AddSource( source );
	
	GPLOGF((" Id: %d ", id ));


	ujni_releaseJString( env, jfile, file );

	GPLOGF((" >\n"));


	return( id );
}



JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer_addSourceFrame
	(JNIEnv * env, jclass c, jint type, jstring jfile, jintArray jframe )
{

	GPLOGF(("<start Png Frames"));

	if( m_hr == NULL || m_hr->IsProcess() ){
		GPLOGF(("In process  >\n"));
		return( -1 );
	}




	const char *file = ujni_getJString( env, jfile );



	CHrSourceI  *source = HrSource_create( type );

	if( source == NULL ){
		ujni_releaseJString( env, jfile, file );
		GPLOGF((" >\n"));
		return( -1 );
	}

	GPLOGF((" %s", file ));


	int *frame = (int *)(env->functions)->GetIntArrayElements(env, jframe, NULL);
	int nFrame = (env->functions)->GetArrayLength(env, jframe);


	GPLOGF((" F: %d", nFrame ));


	if( source->Init( (char *)file, frame, nFrame ) < 0 ){
		delete source;
		ujni_releaseJString( env, jfile, file );
		GPLOGF(("%s failed >\n", file ));
		return( -1 );
	}

	GPLOGF((" Init %s", file ));

	GPLOGF((" nS: %d ", m_hr->m_nS ));

	int id = m_hr->AddSource( source );

	GPLOGF((" Id: %d ", id ));


	(env->functions)->ReleaseIntArrayElements(env, jframe, (jint *)frame, 0);

	ujni_releaseJString( env, jfile, file );

	GPLOGF((" >\n"));


	return( id );
}



JNIEXPORT jint JNICALL  Java_com_homage_renderer_Renderer_addEffect
	(JNIEnv * env, jclass c, jint sourceId, jint type, jstring jfile )
{
	GPLOGF(("<addEffect"));

	if( m_hr == NULL || m_hr->IsProcess() ){
		GPLOGF(("In process  >\n"));
		return( -1 );
	}

	CHrSourceI *src = m_hr->GetSource( sourceId );
	if( src == NULL )
		return( -1 );


	CHrEffectI  *ef = HrEffect_create( type );

	if( ef == NULL )
		return( -1 );

	const char *file = ujni_getJString( env, jfile );

	GPLOGF((" %s", file ));

	if( ef->Init( (char *)file ) < 0 ){
		GPLOGF(("Init failed >\n"));
		return( -1 );
	}

	GPLOGF((" Init %s", file ));

	int id = src->AddEffect( ef );

	GPLOGF((" Id: %d ", id ));


	ujni_releaseJString( env, jfile, file );

	GPLOGF((" >\n"));
	return( id );
}


JNIEXPORT jint JNICALL  Java_com_homage_renderer_Renderer_addEffectData
	(JNIEnv * env, jclass c, jint sourceId, jint type, jbyteArray jdata )
{

	GPLOGF(("<addEffectData"));
	if( m_hr == NULL || m_hr->IsProcess() ){
		GPLOGF(("In process  >\n"));
		return( -1 );
	}


	CHrSourceI *src = m_hr->GetSource( sourceId );
	if( src == NULL )
		return( -1 );


	CHrEffectI  *ef = HrEffect_create( type );

	if( ef == NULL )
		return( -1 );

	u_char *data = (u_char *)(env->functions)->GetByteArrayElements(env, jdata, NULL);

	ef->InitFromData( (char *)data );

	int id = src->AddEffect( ef );


	(env->functions)->ReleaseByteArrayElements(env, jdata, (jbyte *)data, 0);


	GPLOGF((" >\n"));
	return( id );
}



JNIEXPORT int JNICALL Java_com_homage_renderer_Renderer_addOutput
	(JNIEnv * env, jclass c, jint type, jstring jfile, jint width, jint height, int fjrameSpeed )
{
	GPLOGF(("<addOutpur"));

	if( m_hr == NULL || m_hr->IsProcess() ){
		GPLOGF(("In process  >\n"));
		return( -1 );
	}


	const char *file = ujni_getJString( env, jfile );

	if( type == HR_GIF ){
		CHrOutputGif *out = new CHrOutputGif();
		GPLOGF((" GIF"));
		if( out->Init( (char *)file, width, height, fjrameSpeed ) < 0 ){
			ujni_releaseJString( env, jfile, file );
			return( -1 );
		}

		GPLOGF((" Init"));

		int id = m_hr->AddOutput( out );

		GPLOGF((" id = %d >\n", id));

		ujni_releaseJString( env, jfile, file );
		return( id );
	}

	if( type == HR_PNG ){
		CHrOutputPng *out = new CHrOutputPng();
		GPLOGF((" PNG"));
		if( out->Init( (char *)file, width, height, fjrameSpeed ) < 0 ){
			ujni_releaseJString( env, jfile, file );
			return( -1 );
		}

		GPLOGF((" Init"));

		int id = m_hr->AddOutput( out );

		GPLOGF((" id = %d >\n", id));

		ujni_releaseJString( env, jfile, file );
		return( id );
	}


	ujni_releaseJString( env, jfile, file );
	GPLOGF((" >\n"));
	return( -1 );
}



JNIEXPORT int JNICALL Java_com_homage_renderer_Renderer_addJavaOutput
	(JNIEnv * env, jclass c, jobject obj,
	jstring jfile, jint width, jint height, jint frameRate )
{
	GPLOGF(("<addJavaOutput"));

	if( m_hr == NULL || m_hr->IsProcess() ){
		GPLOGF(("In process  >\n"));
		return( -1 );
	}


	const char *file = ujni_getJString( env, jfile );
	GPLOGF((" File: %s", file));


	CHrOutputJava *out = new CHrOutputJava();
	GPLOGF((" Java"));


	if( out->SetJavaEncoder( env, obj ) < 0 ){
		ujni_releaseJString( env, jfile, file );
		GPLOGF((" Init Failed"));
		return( -1 );
	}

	GPLOGF((" Init"));

	if( out->Init( (char *)file, width, height, frameRate ) < 0 ){
		ujni_releaseJString( env, jfile, file );
		return( -1 );
	}

	GPLOGF((" Init"));

	int id = m_hr->AddOutput( out );

	

	ujni_releaseJString( env, jfile, file );

	GPLOGF((" id = %d >\n", id));

	return( id );
}

#ifdef _AA_
JNIEXPORT int JNICALL Java_com_homage_renderer_Renderer_addJavaOutputA
	(JNIEnv * env, jclass c, jstring jclassName,
	jstring jfile, jint width, jint height, jint frameRate )
{
	GPLOGF(("<addJavaOutputA"));

	const char *className = ujni_getJString( env, jclassName );
	GPLOGF((" class: %s", className));

	const char *file = ujni_getJString( env, jfile );
	GPLOGF((" File: %s", file));


	CHrOutputJava *out = new CHrOutputJava();
	GPLOGF((" Java"));


	//if( out->SetJavaEncoder( env, obj ) < 0 ){
	//	ujni_releaseJString( env, jclassName, className );
	//	ujni_releaseJString( env, jfile, file );
	//	GPLOGF((" Init Failed"));
	//	return( -1 );
	//}

	GPLOGF((" Init"));

	if( out->Init( (char *)className, (char *)file, width, height, frameRate ) < 0 ){
		ujni_releaseJString( env, jclassName, className );
		ujni_releaseJString( env, jfile, file );
		return( -1 );
	}

	GPLOGF((" Init"));

	int id = m_hr->AddOutput( out );

	GPLOGF((" id = %d >\n", id));

	ujni_releaseJString( env, jclassName, className );
	ujni_releaseJString( env, jfile, file );


	return( id );
}
#endif

JNIEXPORT int JNICALL Java_com_homage_renderer_Renderer_setOutputPallete
	(JNIEnv * env, jclass c, int id, jbyteArray jdata )
{

	GPLOGF(("<setOutputPallete"));
	if( m_hr == NULL || m_hr->IsProcess() ){
		GPLOGF(("In process  >\n"));
		return( -1 );
	}


	CHrOutputI *out = m_hr->GetOuput( id );
	if( out == NULL )
		return( -1 );


	u_char *data = (u_char *)(env->functions)->GetByteArrayElements(env, jdata, NULL);



	out->SetPalette( (char *)data );

	(env->functions)->ReleaseByteArrayElements(env, jdata, (jbyte *)data, 0);


	GPLOGF((" >\n"));

	return( 1 );
}


JNIEXPORT int JNICALL Java_com_homage_renderer_Renderer_process
	(JNIEnv * env, jclass c )
{
	GPLOGF(("<Render Process"));


	m_hr->Process();

	delete m_hr;

	m_hr = NULL;


	GPLOGF((" >\n"));

	return( 1 );
}





CHrSourceI  *
HrSource_create( int type )
{


	if( type == HR_GIF ){
		CHrSourceGif  *b = new CHrSourceGif();

		GPLOGF((" source gif"));

		return( b );
	}


	if( type == HR_PNG ){
		CHrSourcePng  *b = new CHrSourcePng();
		GPLOGF((" source png"));


		return( b );
	}

	GPLOGF((" source none"));

	return( NULL );
}



CHrEffectI  *
	HrEffect_create( int type )
{


	if( type == EFFECT_MASK ){
		CHrEffectMask  *b = new CHrEffectMask();

		GPLOGF((" effect mask"));
		return( b );
	}

	if( type == EFFECT_POSE ){
		CHrEffectPose  *b = new CHrEffectPose();

		GPLOGF((" effect pose"));
		return( b );
	}

	if( type == EFFECT_ALIGNMENT ){
		CHrEffectAlignment  *b = new CHrEffectAlignment();

		GPLOGF((" effect ALIGNMENT"));
		return( b );
	}


	if( type == EFFECT_MASKGIF ){
		CHrEffectMaskGif  *b = new CHrEffectMaskGif();

		GPLOGF((" effect MASK GIF"));
		return( b );
	}

	return( NULL );
}
