
#ifndef _HR_OUTPUT_GIF_H
#define _HR_OUTPUT_GIF_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "HrOutputI.h"


class CHrOutputGif : public CHrOutputI
{
public:
	
	CHrOutputGif();

	~CHrOutputGif();

	void DeleteContents();

	void SetColor( int color )	{ m_color = color; }


	int Init( char *outFile, int width, int height );

	virtual int	WriteFrame( image_type *im );

	int	Close();

private:

	int	m_iFrame;

	int	m_color;

	int	m_width;
	int	m_height;


	char	m_file[256];

	struct gifIo_type	*m_gifIo;

	palette_type *m_palette;
	image_type *m_im;
};


#endif


