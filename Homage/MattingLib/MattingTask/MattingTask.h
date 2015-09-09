#ifndef _MATTING_TASK_
#define _MATTING_TASK_

#include "Uigp/igp.h"
#include "ImageType/ImageType.h"

#include "UThread/VThread.h"


typedef struct hiFrame_type {
	int iFrame;

	vTime_type	timeStamp;

	image_type *im;

	image_type *aim;	// source with alpha


	int		nFl;

	int		afl[16];

	int status;

}	hiFrame_type;





class CImageAcquisition;

class  CMattingTask : public CVThread
{
public:

	CMattingTask();

	~CMattingTask();

	int Init( char *prmFile,  char *ctrFile, int width, int height );
	void SetFrameRate( int frameRate )	{ m_frameRate = frameRate; }

	void SetImageAcquisition( CImageAcquisition *imageAcquisition );

	void SetOperation( int operation );

	void SetBackground( int color );

	void SetBackground( image_type *bim );

	void SetDumpDir( char *dir );

	void DumpImage( image_type *sim, char *fname );


	int StartTask();
	int StopTask();

	void Run();


	int Process();


	int	SetOutput( int operation, hiFrame_type *vf, image_type *im );

	int	GetOutput( u_char *data, int *nData );
	//int	GetOutputM( u_char *data, int *nData, int afl[] );

	int	GetOutput( u_char *data, int *nData, u_char *data1, int *nData1 );



	int	Record( image_type *im );

	int	StartRecord( char *dir );

	int	StopRecord();


public:
	

	int m_width;
	int	m_height;

	int m_color;

	image_type *m_bim;

	image_type *m_im;

	image_type *m_sim;

	class CImageAcquisition *m_imageAcquisition;

	class CUniformBackground *m_matting;

	int	m_iFrame;



	hiFrame_type	m_vf;


	int	m_process;

	int m_operation;

	class CVConditionMutex *m_mutex;

	char	m_processLog[256];


	char	m_dumpDir[256];


	int	m_record;
	class CPngWriter *m_recorder;

	int m_frameRate;
	vTimer_type	m_timer;


	gpTime_type	m_tCapture;
	gpTime_type	m_tProcess;
	gpTime_type m_tGet;


};

#endif