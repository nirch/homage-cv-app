// Vimage.h: interface for the CVimage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _FRAMEBUFFER_MAC_
#define _FRAMEBUFFER_MAC_


//#if defined _DEBUG || defined DEBUG
//#pragma comment( lib, "VimRendererD.lib" )
//#else
//#pragma comment( lib, "VimRenderer.lib" )
//#endif

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "../FrameBuffer.h"
#include "Ubox/Box2d.h"
#include "ImageType/ImageType.h"



#define TEXT_LEN 512



class CFrameBufferMac  : public CFrameBuffer
{
public:
    CFrameBufferMac();
    virtual ~CFrameBufferMac();
    
    
public:
    
    UIImageView *m_view;
    UIView *m_textboxview;
    UITextView *m_textview;
    UIButton *m_hidetextbutton;
    
    void	Draw ( UIView *view, int x, int y );
    
    //virtual void Draw( struct gpw_type *gpw ) {};
    
    
    // frame buffer
    struct image_type *	Create( int width, int height );
    
    void	Fill();
    
    void SetBitPerPixel();
    
    
    
    // text box
    int	OpenTextBox( char *text, int color, int bcolor, int transparency, int timeout, int motion = 0 );
    
    int	OpenTextBox( char *str );
    
    void	 CloseTextBox();
    
    void	CloseTextBox( char *text );
    
    void	SplitText( int n );
    
    int	 IsTextBoxActive();
    
    void	CreateBoxDib( int width, int height );
    
    void	SeteBoxDib();
    
    void	DrawTextBox ();
    
    int HandleLButtonUp(u_int nFlags, int x, int y );
    

private:
    void CreateImageFromData();

public:
    static UIImage* createImageFromImageType(image_type *imageData);
    
public:
	char	m_text[TEXT_LEN];
	int		m_color;
	int		m_bcolor;
	int		m_timeout;
	int		m_motion;
    
	char	m_aText[4][256];
	int		m_nText;
    
	float		m_transparency;
    
	int		m_textLen;
    
	int		m_time;
	box2i	m_textBox;
    
	int m_textBoxActive;
    
    
	int	m_iText;
    
};




#endif