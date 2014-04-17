/***********************
 ***   GpwTool.cpp   ***
 ***********************/
//#include    <malloc.h>
#include	<math.h>


#include	"Uigp/igp.h"



#include "GpwIos.h"


#import <QuartzCore/QuartzCore.h>

CGColor * vCGColor( int color );


CGpwIos::CGpwIos()
{
    m_view = NULL;
}


CGpwIos::~CGpwIos()
{
    //[drawing release];
}


void CGpwIos::AddSubview(  UIView *view )
{
    
    
	if( m_view == nil ){
        m_view = [[UIImageView alloc] initWithFrame:CGRectMake(0.0, 0.0, view.frame.size.width, view.frame.size.height)];
        
        int width = view.frame.size.width;
        int height = view.frame.size.height;
        
        [view addSubview:m_view];
        
    }
}


void CGpwIos::Start()
{
    UIGraphicsBeginImageContext(m_view.frame.size);
    
    m_context = UIGraphicsGetCurrentContext();
}


void  CGpwIos::End()
{
    UIImage *img = UIGraphicsGetImageFromCurrentImageContext();
    
    UIGraphicsEndImageContext();
    
    m_view.image = img;
    
//    [drawOn addSubview:[[[UIImageView alloc] initWithImage:img] autorelease]];
}



void CGpwIos::PointW( int wx, int wy, int color )
{
    this->RectangleW(wx, wy, wx+1, wy+1, color);
}




void CGpwIos::LineW( int wx0, int wy0, int wx1, int wy1, int color, int width )
{
    width = (width == 0) ? 1 : width;

    
    
    CGContextSetStrokeColorWithColor(m_context, vCGColor(color) );
    CGContextSetLineWidth(m_context, width);
                                                 
    CGContextMoveToPoint(m_context, wx0, wy0);
    CGContextAddLineToPoint(m_context, wx1, wy1);
    CGContextStrokePath(m_context);
}





void CGpwIos::String( float x, float y,
					 char *s, int len, int color )
{
   
    CGContextSelectFont (m_context, "Arial", 20, kCGEncodingMacRoman);
    CGContextSetFillColorWithColor(m_context, vCGColor(color) );
    CGContextSetTextMatrix(m_context, CGAffineTransformMake(1.0,0.0, 0.0, -1.0, 0.0, 0.0));
    CGContextShowTextAtPoint (m_context, x, y+20, s, len);
}





void CGpwIos::RectangleW( int wx0, int wy0, int wx1, int wy1, int color )
{

    CGContextSetFillColorWithColor(m_context, vCGColor(color) );
    
    CGContextSetStrokeColorWithColor(m_context, [UIColor redColor].CGColor);
    
    CGContextFillRect(m_context, CGRectMake(wx0, wy0, wx1 - wx0, wy1 - wy0));
}


 UIColor *CGpwIos::Color( int color )
{
    int red = color >>16;
    int green = (color>>8)&0xFF;
    int blue = ( color&0xFF);
    
      
//    return( [[UIColor alloc] colorWithRed: red/255.0 green: green/255.0 blue: blue/255.0 alpha: 1.0] );
    return( [[UIColor alloc] initWithRed:red/255.0 green:green/255.0 blue:blue/255.0 alpha:1.0] );
}



CGColor * vCGColor( int color )
{
    int red = color >>16;
    int green = (color>>8)&0xFF;
    int blue = ( color&0xFF);
    
    
//    UIColor *c = [[UIColor alloc] colorWithRed: red/255.0 green: green/255.0 blue: blue/255.0 alpha: 1.0] ;
    UIColor *c = [[UIColor alloc] initWithRed:red/255.0 green:green/255.0 blue:blue/255.0 alpha:1.0] ;
                  
    return( [c CGColor] );
}



