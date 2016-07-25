
#ifndef _HR_SOURCE_I_H
#define _HR_SOURCE_I_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "../HrEffect/HrEffectI.h"

#define HR_GIF	1
#define HR_PNG	2
#define HR_JPG	3
#define HR_PNG_SEQ	4
#define HR_COLOR 5
#define HR_JAVA	6


class CHrSourceI 
{
public:	
	CHrSourceI();
	virtual ~CHrSourceI();
    virtual int Close() = 0;

	virtual int Init( char *file ) { return( -1 ); };

	virtual int Init( char *inFile, int aFrame[], int nF )	{ return( -1 ); };

	virtual int	ReadFrame( int iFrame, long long timeStamp, image_type **im ) = 0;

	void SetAlpha( image_type *im );

	void MergeAlpha( image_type *sim );

	int ProcessEffect( image_type *sim, int iFrame, long long timeStamp, image_type **im );

	int AddEffect( CHrEffectI *e );

	CHrEffectI * GetEffect( int id );
	void	SetFrameSize( int width, int height );
    
    // Timing methods
    void SetShouldUseTiming(bool shouldUseTiming);
    void SetSourceDuration(double duration);
    double GetDuration();
    void SetTimingOffset(long long startTimeOffset);
    void SetTimingEnd(long long endTime);
    void SetFreezeTime(long long freezeTime);
    int PickFrameAtTS(long long timeStamp, int maxFramesCount);
    long long CalculatedTS(long long timeStamp);

private:
    void DeleteContents();
protected:
    int first_iFrame;
    bool shouldUseTiming;
    double duration;
    long long startTimeOffset;
    long long endTime;
    long long freezeTime;
    long long lastUsedTimeStamp;
    long long renderStartTimeStamp;
public:
	int	m_iFrame;

	int m_width;
	int m_height;

	image_type *m_alphaIm;
    image_type *empty_image;

    int	m_nE;
	CHrEffectI	*m_ae[128];

};


image_type *	image3_green_alpha( image_type *sim, image_type *im );


#endif


