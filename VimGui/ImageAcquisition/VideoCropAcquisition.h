/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/

#ifndef _VIDEO_CROP_ACQUISITION_
#define _VIDEO_CROP_ACQUISITION_

#if defined _DEBUG && defined DEBUG
//#pragma comment( lib, "VideoCodecD.lib" )
#else
//#pragma comment( lib, "VideoCodec.lib" )
#endif


#include	"Uigp/igp.h"

#include "ImageAcquisition.h"


struct image_type;


class CVideoCropAcquisition: public CImageAcquisition
{
public:
	CVideoCropAcquisition();
	virtual ~CVideoCropAcquisition();	


	int Open( char *name = NULL );

	void Close();

	int Get( image_type **im, int iFrame = -1 );

	void	Restart()		{ m_iFrame = 0; }


protected:

	class CVideoDecoder	*m_decoder;

//	image_type	*m_image;

};



#endif
