
#ifndef _HR_OUTPUT_I_H
#define _HR_OUTPUT_I_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"





class CHrOutputI 
{
public:
	
	CHrOutputI();

	~CHrOutputI();

	void DeleteContents();

		virtual int Init( char *outFile, int width, int height ) = 0;

	virtual int	WriteFrame( image_type *im ) = 0;


	virtual int Close() = 0;
private:

	int	m_iFrame;





};


#endif


