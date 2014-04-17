/*****************************
 ***   FrameBufferMac.cpp   ***
 *****************************/

#include "Utime/GpTime.h"
#include "FrameBufferMac.h"



CFrameBufferMac::CFrameBufferMac()
{
    m_view = nil;
    
	m_bcolor = 0x4e145c;
    
	m_transparency = 0.5;
    
	m_textBoxActive = 0;
    
	int i;
	for( i = 0 ; i < 4 ; i++ )
		m_aText[i][0] = 0;
    
	m_nText = 0;
    
	m_time = 0;
    
	m_timeout = 0;
    
	m_motion = 0;
    
}


CFrameBufferMac::~CFrameBufferMac()
{
	if( m_view != nil ) { [m_view release]; }
    if (m_textboxview != nil) { [m_textboxview release]; }
    if (m_textview != nil) { [m_textview release]; }
}


void
CFrameBufferMac::Draw ( UIView *view, int x, int y )
{
	if( m_view == nil ){
        return;
	}
    
    
	//::BitBlt( hdc, x, y, col, row, m_hdc, 0, 0, SRCCOPY );
    
    if (m_view.superview != view){
        [view addSubview:m_view];
    }
    
    //[m_view setFrame:CGRectMake(x, y, m_view.frame.size.width, m_view.frame.size.height)];
    [m_view setFrame:CGRectMake(x, y, view.frame.size.width, view.frame.size.height)];
    
    this->CreateImageFromData();
    
    [m_textboxview setHidden:YES];
    [m_textview setHidden:YES];
    //[m_hidetextbutton setHidden:YES];
    DrawTextBox();
}


image_type *	CFrameBufferMac::Create( int width, int height )
{
    int depth = 4;
    //int bpp = 32;
    
	if( m_view == nil ){
       m_view = [[UIImageView alloc] initWithFrame:CGRectMake(0.0, 0.0, width, height)];
        
		m_image = image_create( height, width, depth, 1, NULL );
        
        // create text box
        UIImage *headerDecImg = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"header_decoration@2x" ofType:@"png"]];
        UIImage *logoImg = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"logo@2x" ofType:@"png"]];
        
        m_textboxview = [[UIView alloc] initWithFrame:CGRectMake(0.0, 0.0, width, 55.0 + headerDecImg.size.height)];
        [m_textboxview setHidden:YES];
        
        UIImageView *headerDecImgView = [[[UIImageView alloc] initWithImage:headerDecImg] autorelease];
        [headerDecImgView setFrame:CGRectMake(0.0, m_textboxview.frame.size.height - headerDecImg.size.height, headerDecImg.size.width, headerDecImg.size.height)];
        [m_textboxview addSubview:headerDecImgView];
        
        UIImageView *logoImgView = [[[UIImageView alloc] initWithImage:logoImg] autorelease];
        [logoImgView setFrame:CGRectMake(5.0, 12.0, logoImg.size.width, logoImg.size.height)];
        [m_textboxview addSubview:logoImgView];
        
        m_textview = [[UITextView alloc] initWithFrame:CGRectMake(10.0 + logoImg.size.width, 10.0, width - logoImgView.frame.size.width - 50.0, 45.0)];
        [m_textview setBackgroundColor:[UIColor clearColor]];
        [m_textview setHidden:YES];
        
        
        [m_view addSubview:m_textboxview];
        [m_view addSubview:m_textview];
 
        
		return( m_image );
	}
    
    
	if ((m_view.frame.size.width == width) && (m_view.frame.size.height == height)){
        return m_image;
    }
    

    m_image = image_recreate( m_image, height, width, depth, 1);
    
	return( m_image );
}

UIImage* CFrameBufferMac::createImageFromImageType(image_type *imageData){
    unsigned char *pixels        = imageData->data;
    int size = imageData->width * imageData->height * 4;
    
    size_t width                    = imageData->width;
    size_t height                   = imageData->height;
    size_t bitsPerComponent         = 8;
    size_t bitsPerPixel             = 32;
    size_t bytesPerRow              = imageData->width * 4;
    
    CGColorSpaceRef colorspace      = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo         = kCGBitmapByteOrderDefault;
    
    NSData* newPixelData = [NSData dataWithBytes:pixels length:size];
    CFDataRef imgData = (CFDataRef)newPixelData;
    CGDataProviderRef imgDataProvider = CGDataProviderCreateWithCFData(imgData);
    
    CGImageRef newImageRef = CGImageCreate (
                                            width,
                                            height,
                                            bitsPerComponent,
                                            bitsPerPixel,
                                            bytesPerRow,
                                            colorspace,
                                            bitmapInfo,
                                            imgDataProvider,
                                            NULL,
                                            NO,
                                            kCGRenderingIntentDefault
                                            );
    
    UIImage *newImage   = [[UIImage alloc] initWithCGImage:newImageRef];//[UIImage imageWithCGImage:newImageRef];
    
    CGColorSpaceRelease(colorspace);
    CGDataProviderRelease(imgDataProvider);
    CGImageRelease(newImageRef);
    
    return [newImage autorelease];
}


void CFrameBufferMac::CreateImageFromData()
{
    UIImage* newImage = CFrameBufferMac::createImageFromImageType(m_image);
    
    //NSLog(@"%.1f, %.1f", newImage.size.width, newImage.size.height);
    if (newImage){
        [m_view setContentMode:UIViewContentModeScaleToFill];
        [m_view setImage:newImage];
    }
    
    
}

void CFrameBufferMac::Fill()
{
    CreateImageFromData();
}






int CFrameBufferMac::OpenTextBox( char *text, int color, int bcolor, int transparency, int timeout, int motion )
{
	if( m_image == NULL )	return( -1 );
    
    
	strncpy( m_text, text, TEXT_LEN );
    
	m_textLen = (int)strlen( m_text );
    
	m_color = color;
    
	m_bcolor = bcolor;
    
	m_transparency = transparency/100.0;
    
	m_timeout = timeout;
    
	m_motion = motion;
    
	m_iText = 0;
    
    
	SplitText( 64 );
    
	return( 1 );
    
}

int CFrameBufferMac::OpenTextBox( char *str )
{
    int	transparency;
    
	if( m_image == NULL )	return(-1);
    
	if( gpStr_strtok_string( str, "|", m_text ) < 0 )
		return( -1 );
    
	if( gpStr_strtok_hexa( NULL, "|", &m_color ) < 0 )
		return( -1 );
    
	if( gpStr_strtok_hexa( NULL, "|", &m_bcolor ) < 0 )
		return( -1 );
    
    
	if( gpStr_strtok_int( NULL, "|", &transparency ) < 0 )
		return( -1 );
    
	if( gpStr_strtok_int( NULL, "|", &m_timeout ) < 0 )
		return( -1 );
    
	m_motion = 0;
	gpStr_strtok_int( NULL, "|", &m_motion );
    
	m_time = gpTime_base();
    
	m_textLen = (int)strlen( m_text );
    
	m_transparency = transparency/100.0;
    
	m_iText = 0;
    
    
	//CreateBoxDib( m_image->width, 50 );
    
	SplitText( 64 );
    
	return( 1 );
}


void CFrameBufferMac::SplitText( int n )
{
    int	i,	i0,	k,	size;
    
	for( i = 0 ; i < 4 ; i++ )
		m_aText[i][0] = 0;
    
	m_nText = 0;
	size = strlen( m_text );
    
	for( i = 0, i0 = 0 ; i < 2 ; i++ ){
		if( i0 + n > size ){
			strncpy( &m_aText[m_nText][0], &m_text[i0], size - i0 );
			m_aText[m_nText][size - i0] = 0;
			m_nText++;
			return;
		}
        
		for( k = i0+n ; k > i0 ; k-- )
			if( m_text[k] == ' ' )	break;
		if( k == i0 )	return;
        
		strncpy( &m_aText[m_nText][0], &m_text[i0], k - i0 );
		m_aText[m_nText][k - i0] = 0;
		m_nText++;
        
		i0 = k + 1;
	}
}




void CFrameBufferMac::CloseTextBox( )
{
	m_time = -1;
    
}


void CFrameBufferMac::CloseTextBox( char *text )
{
	if( m_time < 0 )	return;
    
	if( strncmp( m_text, text, strlen(text)) == 0 )
		m_time = -1;
    
}

void
CFrameBufferMac::DrawTextBox ()
{
	m_textBoxActive = 0;
	if( IsTextBoxActive( ) < 0 )
		return;
    
    
	//SeteBoxDib();
    
    [m_textboxview setHidden:NO];
    [m_textview setHidden:NO];
    [m_textview setText:@""];
    //[m_hidetextbutton setHidden:NO];
    
    CGFloat red = (m_color >> 16) & 0xFF;
    CGFloat green = (m_color >> 8) & 0xFF;
    CGFloat blue = m_color & 0xFF;
    [m_textview setTextColor:[UIColor colorWithRed:red/255.0 green:green/255.0 blue:blue/255.0 alpha:1.0]];
    
    CGFloat redbg = (m_bcolor >> 16) & 0xFF;
    CGFloat greenbg = (m_bcolor >> 8) & 0xFF;
    CGFloat bluebg = m_bcolor & 0xFF;
    [m_textboxview setBackgroundColor:[UIColor colorWithRed:redbg/255.0 green:greenbg/255.0 blue:bluebg/255.0 alpha:0.8]];
    
	//WCHAR    wtext[TEXT_LEN];
	//MultiByteToWideChar( 0,0, m_text, m_textLen, wtext, m_textLen);
    
    //	TextOut( m_hdc, 10, 20, wtext, m_textLen );
    //	TextOut( m_hdc, 10, 5, m_text, m_textLen );
    
	if( m_iText * 10 > m_textBox.x1 )	m_iText = 0;
	int	x;
	x = 10 + m_iText*m_motion*10;
	m_iText++;
    
    [m_textview setFrame:CGRectMake(m_textview.frame.origin.x, m_textview.frame.origin.y, m_textview.frame.size.width, m_textview.frame.size.height)];
    [m_textview setText:@""];

    NSString *str = @"";
    
    const char *line;
	int	i;
	for( i = 0 ; i < m_nText ; i++ ){
		//TextOut( m_hdc, x, 5+i*20, &m_aText[i][0], strlen(m_aText[i]) );
        line = m_aText[i];
        str = [str stringByAppendingString:[NSString stringWithFormat:@"%@\n", [NSString stringWithCString:line encoding:NSUTF8StringEncoding]]];
        //NSLog(@"%@", str);
    }
    [m_textview setText:str];
	m_textBoxActive = 1;
}



int CFrameBufferMac::IsTextBoxActive( )
{
    
	if( m_time <= 0  )	return( -1 );
    
	if( m_timeout > 0 && gp_time( m_time ) > m_timeout*1000 )	return( -1 );
    
    
	return( 1 );
}




#include "TextBoxX.h"




int CFrameBufferMac::HandleLButtonUp(u_int nFlags, int x, int y )
{
    //int	x0,	y0;
    
    
    
	if( m_textBoxActive == 1 ){
        
		//x0 = m_boxIm->width - XICON_SIZE-5;
		//y0 = m_boxIm->height -XICON_SIZE-5;
        
		//if( x > x0+1 && x < x0 + XICON_SIZE -2   && y > y0+1 && y < y0 + XICON_SIZE -2 ){
        /*if ((x > m_hidetextbutton.frame.origin.x) && (x < m_hidetextbutton.frame.size.width) && (y > m_hidetextbutton.frame.origin.y) && (y < m_hidetextbutton.frame.size.height)){
			CloseTextBox();
			return( 1 );
		}*/
	}
    
    
	return( -1 );
}


