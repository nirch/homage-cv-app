
#ifndef _HR_OUTPUT_I_H
#define _HR_OUTPUT_I_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"





class CHrOutputI 
{
public:
	
	CHrOutputI();

	virtual ~CHrOutputI();

	void DeleteContents();

	virtual int Init( char *outFile, int width, int height, int frameSpeed )	{ return( -1 ); }

	virtual int SetPalette( char *data )	{ return( -1 ); }

	virtual int	WriteFrame( image_type *im, int iFrame ) = 0;


	virtual int Close() = 0;
private:

	int	m_iFrame;





};


#endif


