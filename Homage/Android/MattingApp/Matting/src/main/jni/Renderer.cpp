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




static int	m_nR = 0;
static CHomageRenderer *m_ar[16];

CHrSourceI  *	HrSource_create( int type );

CHrEffectI  *	HrEffect_create( int type );




JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer_create
	(JNIEnv *, jclass )
{
	GPLOGF(("<Renderer Create"));

	int	i;
	for( i = 0 ; i < m_nR ; i++ ){
		if( m_ar[i] == NULL )
			break;
	}

	if( i >= 8 )	return( -1 );

	if( i >= m_nR )	m_nR = i+1;


	m_ar[i] = new CHomageRenderer();
	


	GPLOGF((" %d>\n", i ));

	return( i );
}



JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer_addSource
	(JNIEnv * env, jclass c, jint iR, jint type, jstring jfile )
{

	GPLOGF(("<addSource"));


	CHomageRenderer *hr  = m_ar[iR];

	if( hr == NULL || hr->IsProcess() ){
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

	GPLOGF((" nS: %d ", hr->m_nS ));

	int id = hr->AddSource( source );
	
	GPLOGF((" Id: %d.%d ", iR, id ));


	ujni_releaseJString( env, jfile, file );

	GPLOGF((" >\n"));


	return( id );
}



JNIEXPORT jint JNICALL Java_com_homage_renderer_Renderer_addSourceFrame
	(JNIEnv * env, jclass c, jint iR, jint type, jstring jfile, jintArray jframe )
{

	GPLOGF(("<start Png Frames"));

	CHomageRenderer *hr  = m_ar[iR];
	if( hr == NULL || hr->IsProcess() ){
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

	GPLOGF((" nS: %d ", hr->m_nS ));

	int id = hr->AddSource( source );

	GPLOGF((" Id: %d ", id ));


	(env->functions)->ReleaseIntArrayElements(env, jframe, (jint *)frame, 0);

	ujni_releaseJString( env, jfile, file );

	GPLOGF((" >\n"));


	return( id );
}



JNIEXPORT jint JNICALL  Java_com_homage_renderer_Renderer_addEffect
	(JNIEnv * env, jclass c, jint iR, jint sourceId, jint type, jstring jfile )
{
	GPLOGF(("<addEffect"));

	CHomageRenderer *hr  = m_ar[iR];
	if( hr == NULL || hr->IsProcess() ){
		GPLOGF(("In process  >\n"));
		return( -1 );
	}

	CHrSourceI *src = hr->GetSource( sourceId );
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
	(JNIEnv * env, jclass c, jint iR, jint sourceId, jint type, jstring jdata )
{

	GPLOGF(("<addEffectData"));
	CHomageRenderer *hr  = m_ar[iR];
	if( hr == NULL || hr->IsProcess() ){
		GPLOGF(("In process  >\n"));
		return( -1 );
	}


	CHrSourceI *src = hr->GetSource( sourceId );
	if( src == NULL )
		return( -1 );


	CHrEffectI  *ef = HrEffect_create( type );

	if( ef == NULL )
		return( -1 );


	const char *data = ujni_getJString( env, jdata );

	ef->InitFromData( (char *)data );

	int id = src->AddEffect( ef );


	ujni_releaseJString( env, jdata, data );


	GPLOGF((" >\n"));
	return( id );
}





JNIEXPORT int JNICALL Java_com_homage_renderer_Renderer_addOutput
	(JNIEnv * env, jclass c, jint iR, jint type, jstring jfile, jint width, jint height, int fjrameSpeed )
{
	GPLOGF(("<addOutput"));

	CHomageRenderer *hr  = m_ar[iR];
	if( hr == NULL || hr->IsProcess() ){
		GPLOGF(("In process  >\n"));
		return( -1 );
	}


	const char *file = ujni_getJString( env, jfile );

	if( type == HR_GIF ){
		CHrOutputGif *out = new CHrOutputGif();
		GPLOGF((" GIF  %d %d", width, height ));
		if( out->Init( (char *)file, width, height, fjrameSpeed ) < 0 ){
			ujni_releaseJString( env, jfile, file );
			return( -1 );
		}

		GPLOGF((" Init"));

		int id = hr->AddOutput( out );

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

		int id = hr->AddOutput( out );

		GPLOGF((" id = %d >\n", id));

		ujni_releaseJString( env, jfile, file );
		return( id );
	}


	ujni_releaseJString( env, jfile, file );
	GPLOGF((" >\n"));
	return( -1 );
}



/*JNIEXPORT int JNICALL Java_com_homage_renderer_Renderer_addJavaOutput
	(JNIEnv * env, jclass c, jint iR, jobject obj,
	jstring jfile, jint width, jint height, jint frameRate )
{
	GPLOGF(("<addJavaOutput"));

	CHomageRenderer *hr  = m_ar[iR];
	if( hr == NULL || hr->IsProcess() ){
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

	int id = hr->AddOutput( out );

	

	ujni_releaseJString( env, jfile, file );

	GPLOGF((" id = %d >\n", id));

	return( id );
}*/



JNIEXPORT int JNICALL Java_com_homage_renderer_Renderer_setOutputPallete
	(JNIEnv * env, jclass c, jint iR, int id, jbyteArray jdata )
{

	GPLOGF(("<setOutputPallete"));
	CHomageRenderer *hr  = m_ar[iR];
	if( hr == NULL || hr->IsProcess() ){
		GPLOGF(("In process  >\n"));
		return( -1 );
	}


	CHrOutputI *out = hr->GetOuput( id );
	if( out == NULL )
		return( -1 );


	u_char *data = (u_char *)(env->functions)->GetByteArrayElements(env, jdata, NULL);



	out->SetPalette( (char *)data );

	(env->functions)->ReleaseByteArrayElements(env, jdata, (jbyte *)data, 0);


	GPLOGF((" >\n"));

	return( 1 );
}



JNIEXPORT int JNICALL Java_com_homage_renderer_Renderer_setFrameSize
	(JNIEnv * env, jclass c, jint iR, jint width, jint height )
{
	GPLOGF(("<setFrameSize"));

	CHomageRenderer *hr  = m_ar[iR];
	if( hr == NULL || hr->IsProcess() ){
		GPLOGF(("In process  >\n"));
		return( -1 );
	}


	hr->SetFrameSize( width, height );

	GPLOGF((">/n"));

	return( 1 );
}



JNIEXPORT int JNICALL Java_com_homage_renderer_Renderer_process
	(JNIEnv * env, jclass c, jint iR )
{
	GPLOGF(("<Render Process: %d", iR ));

	CHomageRenderer *hr  = m_ar[iR];
	if( hr == NULL || hr->IsProcess() )
		return( -1 );

	hr->Process();

	delete m_ar[iR];
	m_ar[iR] = NULL;


	GPLOGF((" %d >\n", iR ));

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
