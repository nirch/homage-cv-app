/***********************
 ***   GpwTool.cpp   ***
 ***********************/
//#include    <malloc.h>
#include	<math.h>


#include	"Uigp/igp.h"



#include "GpwMac.h"


#import <QuartzCore/QuartzCore.h>

CGpwMac::CGpwMac( void *w )
{
    drawOn = (UIView*)w;
}


CGpwMac::~CGpwMac()
{
    //[drawing release];
}




void CGpwMac::PointW( int wx, int wy, int color )
{
    this->RectangleW(wx, wy, wx+1, wy+1, color);
}



void CGpwMac::LineW( int wx0, int wy0, int wx1, int wy1, int color, int width )
{
    width = (width == 0) ? 1 : width;
    UIGraphicsBeginImageContext(drawOn.frame.size);
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetStrokeColorWithColor(context, [UIColor redColor].CGColor);
    CGContextSetLineWidth(context, width);
    CGContextMoveToPoint(context, wx0, wy0);
    CGContextAddLineToPoint(context, wx1, wy1);
    CGContextStrokePath(context);
    
    UIImage *img = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    [drawOn addSubview:[[[UIImageView alloc] initWithImage:img] autorelease]];
}





void CGpwMac::String( float x, float y,
					 char *s, int len, int color )
{
    UIGraphicsBeginImageContext(drawOn.frame.size);
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSelectFont (context, "Arial", 20, kCGEncodingMacRoman);
    CGContextSetFillColorWithColor(context, [UIColor blackColor].CGColor);
    CGContextSetTextMatrix(context, CGAffineTransformMake(1.0,0.0, 0.0, -1.0, 0.0, 0.0));
    CGContextShowTextAtPoint (context, x, y+20, s, len);
    
    UIImage *img = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    [drawOn addSubview:[[[UIImageView alloc] initWithImage:img] autorelease]];
}





void CGpwMac::RectangleW( int wx0, int wy0, int wx1, int wy1, int color )
{
    UIGraphicsBeginImageContext(drawOn.frame.size);

    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetFillColorWithColor(context, [UIColor blackColor].CGColor);
    CGContextSetStrokeColorWithColor(context, [UIColor redColor].CGColor);
    CGContextFillRect(context, CGRectMake(wx0, wy0, wx1 - wx0, wy1 - wy0));

    UIImage *img = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    [drawOn addSubview:[[[UIImageView alloc] initWithImage:img] autorelease]];
}


void CGpwMac::Clean(){
    for (UIView *subview in drawOn.subviews){
        [subview removeFromSuperview];
    }
}





