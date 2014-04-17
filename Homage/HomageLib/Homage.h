// Homage.h: interface for the VimArRenderer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _HOMAGE_LIB_
#define _HOMAGE_LIB_

#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "HomageLibD.lib" )
#else
#pragma comment( lib, "HomageLib.lib" )
#endif





#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Uvec/Vec3d.h"
#include "Utime/GpTime.h"
#include "Umedia/MediaDataType.h"

#include "ImageType/ImageType.h"
#include "Gpw/FrameBuffer/FrameBuffer.h"
#include "Gpw/Gpw.h"





class CHomage
{
public:
	CHomage(  CFrameBuffer *fb);
	virtual ~CHomage();

	void	DeleteContents();
    
 

    int Init( char *ctrFile, int width, int height );


	int	IsEmpty()	{ return ( m_im == NULL); }
	int	IsReady()	{ return (  m_im != NULL); }

    void SetProcess( int process )  { m_process = process; }

	
	int	Process( int iFrame );

	int Process( image_type *im, int iFrame );



	int GetProcessTime();

	void Trace( FILE *fp );

	void Write( char *file );
    
    char * GetProcessLog();




private:


public:

    CFrameBuffer *m_fb;

	int m_iFrame;

	image_type *m_im;
    
    image_type *m_aim;
    
    
    class CUniformBackground *m_cb;
    
    int m_process;
    
    
    char m_processLog[1024];



	gpTime_type	m_gTime;
	gpTime_type	m_tProcess;
};
#endif 


