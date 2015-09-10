/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef __PNG_WRITER__H_
#define __PNG_WRITER__H_



#include "UThread/VThread.h"
#include "UThread/VMutex.h"



#ifdef _DEBUG 
#pragma comment( lib, "PngCodecD.lib" )
#else
#pragma comment( lib, "PngCodec.lib" )
#endif


//#define		MAX_IM		5000

#define		AW_SYNCHRONY	0
#define		AW_ASYNCHRONY   1
#define		AW_ASYNCHRONY_THREAD   2




class CPngWriter :public CVThread
{
private:




public:
	CPngWriter();
	~CPngWriter(void);

	int Open( int quality, char *outFile );

	int WriteFrame( image_type *im );

//	int WriteFrame( vTime_type timeStamp, u_char *data, int nData );

	int Close();


private:
	void		Run();

	void Write( image_type *im);


private:
	char	m_dir[256];
	int		m_quality;
	int		m_iFrame;


	int		m_iIm;

	imageA_type *m_aim;

	int m_index;

	FILE *m_fp;
	vTime_type	m_time0;


	CVMutex  *m_mutex;

	int m_process;
};

#endif //__JPEG_WRITER__H_
