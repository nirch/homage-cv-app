

#ifndef _TEXT_LABEL_
#define _TEXT_LABEL_

#include "Uigp/igp.h"
#include "Utime/GpTime.h"

#include "../FrameLabel/FrameLabel.h"


#define TEXT_LEN 512


class CTextLabel //: public CFrameLabel
{
public:
    CTextLabel( void *view );
     CTextLabel();

    virtual ~CTextLabel();

    virtual void SetFrameSize( int width, int height );
    virtual void SetBackgroundColor( int color, float transparent );
    
    virtual void   SetColor( int color );

    virtual int HandleLButtonUp(u_int nFlags, int x, int y );
    
    virtual void SetVisible( int flag );



public:

	virtual int Init( char *file );

	virtual int Draw()	{ return(0); }

	int		Set( char *text );

	int		Open( char *str );

	int		Open( char *text, int color, int bcolor, int transparency, int timeout, int motion = 0 );

	void	Close();

	void	Close( char *text );


public:





	int		IsActive();

private:
	void	SplitText( int n );


public:
	int	m_width;
	int	m_height;
    
    int m_show;

	int m_bcolor;
	float m_transparent;




	char	m_text[TEXT_LEN];
	int		m_color;

	int		m_timeout;
	int		m_motion;

	char	m_aText[4][256];
	int		m_nText;

//	float	m_transparency;

	int		m_textLen;

	vTime_type	m_time;



	int	m_iText; 
};




#endif