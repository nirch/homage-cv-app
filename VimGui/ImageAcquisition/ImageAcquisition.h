/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef _IMAGE_ACQUISITION_
#define _IMAGE_ACQUISITION_

#if defined _DEBUG && defined DEBUG
//#pragma comment( lib, "VideoCodecD.lib" )
#else
//#pragma comment( lib, "VideoCodec.lib" )
#endif


#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "ImageAcquisitionD.lib" )
#else
#pragma comment( lib, "ImageAcquisition.lib" )
#endif

#include	"Uigp/igp.h"
#include "Utime/GpTime.h"

#define CAMERA_FOCUS_NONE	0
#define CAMERA_FOCUS_NEAR	1
#define CAMERA_FOCUS_FAR	2


struct image_type;


class CImageAcquisition
{
public:
	CImageAcquisition();
	virtual ~CImageAcquisition();	

	virtual int Open( char *name );// = 0;
	virtual int Open( int index = -1  ) { return( -1 ); } // = 0;



	virtual void Close();// = 0;

	virtual void SetFrameSize( int width, int height );

	virtual void SetActiveOn()	{ m_active = 1; }

	virtual void SetActiveOff()	{ m_active = 0; }

	virtual void SetCameraId( int id )	{ m_cameraId = id; }

	virtual int	IsActive()		{ return m_active; }

	virtual void	Restart()	{}


	virtual float   GetCcdWidth()   { return(0); }
	virtual float   GetFocalLength()  { return( 0 ); }


	virtual int Get( image_type **im, int iFrame = -1 );

	virtual int Get( u_char **data, int *nData, vTime_type *timeStamp ) { return -1 ;}

	virtual int GetImage( image_type **im, u_char **data, int *nData ) { return -1 ;}

	virtual image_type *GetImage() { return( m_image); }

	virtual int GetMetaData( u_char * _frameData ) { return 0; };

	virtual int LogStatus()	{ return 1; }
	virtual int LogStatus( char *file )	{ return 1; }


	void SetFrame( int iFrame )	{ m_iFrame = iFrame; }
	void SetFrameD( int iFrame )	{ m_iFrame += iFrame; }



	int			GetWidth()	{ return( m_width ); }
	int			GetHeight()	{ return( m_height ); }
	int			GetFrame()	{ return( m_iFrame ); }

	float    GetTimeStamp(){return m_timeStamp;};


//	char    *getAcqName(){return m_acqName;}

	int		GetAcq()	{ return( m_cameraId ); }


	virtual void SetFoucusPixel( int x, int y )   { }

	virtual int SetAutoFocus( float x, float y )    { return 0; }
	virtual int SetContinuousAutoFocus( float x, float y ) { return 0  ; }
	virtual int SetFocusRestriction( int flag ) {return 0; }
	virtual int GetFocusState() {return 0; }




protected:
//	char		m_acqName[256]; // name of the acquisition
	int			m_cameraId;
	int			m_active;				// start to save data		


	int         m_fx;
	int         m_fy;

	int         m_x0;
	int         m_y0;
	int			m_width;
	int			m_height;

	int			m_rWidth;
	int			m_rHeight;


	image_type	*m_image;

	int			m_iFrame;
	int			m_timeStamp;
};



CImageAcquisition *CreateCameraAcquisition();

CImageAcquisition *CreateVideoAcquisition();

CImageAcquisition *CreateCropVideoAcquisition();

#endif
