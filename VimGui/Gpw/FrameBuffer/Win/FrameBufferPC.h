// Vimage.h: interface for the CVimage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FRAME_BUFFER_PC_
#define _FRAME_BUFFER_PC_




#include	"../FrameBuffer.h"



class CFrameBufferPC  : public CFrameBuffer
{
public:
		CFrameBufferPC();
		virtual ~CFrameBufferPC();


		void Draw ( HDC hdc, int x, int y );


public:

		class CDib		*m_pDib;

	
		image_type *	Create( int width, int height );



		void SetBitPerPixel();
};




#endif