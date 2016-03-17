//  Defines the entry point for the console application.
//
#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#ifdef _DEBUG
//#define _DUMP
#endif

#include "Ulog/Log.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"

#include "HrSourceI.h"

#include "../HrEffect/HrEffectPose.h"


void	imageA_merge_alpha( image_type *sim, image_type *im );




CHrSourceI::CHrSourceI()
{
	m_alphaIm = NULL;
	m_nE = 0;
    
    this->SetSourceDuration(2.0);
    this->first_iFrame = 0;
    this->startTimeOffset = 0;
    this->freezeTime = -1;
    this->endTime = -1;
    this->lastUsedTimeStamp = -1;
    this->shouldUseTiming = false;
}

CHrSourceI::~CHrSourceI()
{
	DeleteContents();
}

void CHrSourceI::DeleteContents()
{
int	i;

	if( m_alphaIm != NULL ){
		image_destroy( m_alphaIm, 1 );
		m_alphaIm = NULL;
	}

	for( i = 0 ; i < m_nE ; i++ ){
		delete m_ae[i];
	}
	m_nE = 0;
}


int CHrSourceI::AddEffect( CHrEffectI *e )
{
	m_ae[m_nE++] = e;

	return( m_nE-1 );
}


CHrEffectI * CHrSourceI::GetEffect( int id )
{
int	i;
	for( i = 0 ; i < m_nE ; i++ ){
		CHrEffectI *e = m_ae[i];
		if( e->m_id == id )
			return( e );
	}

	return( NULL );
}


void
	CHrSourceI::SetFrameSize( int width, int height )
{

	CHrEffectPose *e = (CHrEffectPose*)GetEffect( EFFECT_POSE );

	if( e == NULL ){
		e = new CHrEffectPose();

		AddEffect( e );
	}

	e->SetFrameSize( width, height );
}

void CHrSourceI::SetShouldUseTiming(bool shouldUseTiming)
{
    this->shouldUseTiming = shouldUseTiming;
}

void CHrSourceI::SetSourceDuration(double duration)
{
    this->duration = duration;
}

double CHrSourceI::GetDuration()
{
    return this->duration;
}

void CHrSourceI::SetTimingOffset(long long startTimeOffset)
{
    this->startTimeOffset = startTimeOffset;
}

void CHrSourceI::SetTimingEnd(long long endTime)
{
    this->endTime = endTime;
}

void CHrSourceI::SetFreezeTime(long long freezeTime)
{
    this->freezeTime = freezeTime;
}

int CHrSourceI::PickFrameAtTS(long long timeStamp, int maxFramesCount)
{
    // Get the calculated time stamp, taking into account timing effects.
    long long calculatedTimeStamp = this->CalculatedTS(timeStamp);
    if (calculatedTimeStamp == -1) return -1;
    
    // Calculate the frame needed to be used (and keep it in available bounds).
    long double timeInSeconds = timeStamp/1000000000.0;
    int pickedFrame = (int)((timeInSeconds / (long double)this->duration) * maxFramesCount);
    pickedFrame = pickedFrame % maxFramesCount;
    
    // Return the picked frame.
    return pickedFrame;
}

long long CHrSourceI::CalculatedTS(long long timeStamp)
{
    long long ts = -1;
    
    if (startTimeOffset >=0 && timeStamp < startTimeOffset) {
        // If timeStamp is smaller than startTime, an empty frame should be used.
        ts = -1;
    } else if (endTime >= 0 && timeStamp > endTime) {
        // If timeStamp is bigger then endTime, an empty frame should be used.
        ts = -1;
    } else if (freezeTime >=0 && timeStamp >= freezeTime) {
        // If timeStamp is bigger then freezeTime, continue returning the last picked frame.
        ts = lastUsedTimeStamp;
    } else {
        ts = timeStamp - startTimeOffset;
    }
    this->lastUsedTimeStamp = ts;
    return ts;
}

int CHrSourceI::ProcessEffect( image_type *sim, int iFrame, long long timeStamp, image_type **im )
{
int	i;

	*im = sim;

	for( i = 0 ; i < m_nE ; i++ ){
		m_ae[i]->Process( sim, iFrame, timeStamp, im );
		sim = *im;
	}

	return( 1 );
}

void CHrSourceI::SetAlpha( image_type *im)
{
	if( im->depth != 1 ){
		m_alphaIm = image1_from( im, NULL );
		image_destroy( im, 1 );
		return;
	}

	m_alphaIm = im;
}

void CHrSourceI::MergeAlpha( image_type *sim )
{
	if( m_alphaIm != NULL )
		imageA_merge_alpha( sim, m_alphaIm );
}



image_type *
image3_green_alpha( image_type *sim, image_type *im )
{
	int	i,	j;

	im = image_realloc( im, sim->width, sim->height, 4, IMAGE_TYPE_U8, 1 );

	u_char *sp = sim->data;
	u_int *tp = im->data_ui;

	for(i = 0 ; i < im->height ; i++ ){
		for( j = 0 ; j < im->width ; j++, tp++, sp += 3 ){
			int t0 = sp[0] - 1;
			int t1 = sp[1] - 254;
			int t2 = sp[2] - 6;
			if( ABS(t0) < 16 && ABS(t1) < 16 && ABS(t2) <16 ){
		//	if( sp[0] == 1 && sp[1] == 254 && sp[2] == 6  ){
				*tp = 0;
				continue;
			}

			*tp = 0xFF000000 | IMAGE4_RGB( sp[0], sp[1], sp[2] );
		}
	}

	return( im );
}



void
imageA_merge_alpha( image_type *sim, image_type *im )
{
	int	i,	j;



	u_int *sp = sim->data_ui;
	u_char *ap = im->data;

	for(i = 0 ; i < im->height ; i++ ){
		for( j = 0 ; j < im->width ; j++, ap++, sp ++ ){
			int a = (*sp)>>24;

			if( a > *ap ){
				a = *ap;
				*sp = (*sp&0X00FFFFFF) | ( (a << 24 ));
			}
		}
	}
}