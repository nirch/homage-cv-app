// Vimage.h: interface for the CVimage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FRAME_BUFFER_
#define _FRAME_BUFFER_

#include "ImageType/ImageType.h"


class CFrameBuffer
{
public:
		CFrameBuffer();

		virtual ~CFrameBuffer();



public:

		virtual image_type * Create( int width, int height );

		virtual void	Fill();

		virtual void	Set( image_type *sim );

		virtual int HandleLButtonUp(u_int nFlags, int x, int y )	{ return(0); }

		virtual int	OpenTextBox( char *str )	{ return(0); }
		virtual	int OpenTextBox( char *text, int color, int bcolor, int transparency, int timeout, int motion = 0 )	{ return(0); };

		virtual void	 CloseTextBox()	{ }
		virtual void	 CloseTextBox( char *str )	{ }


		struct image_type *m_image;

};




#endif