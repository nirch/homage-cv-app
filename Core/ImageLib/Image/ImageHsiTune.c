/**************************
 ***	ImageHsiTune.c	***
 **************************/

#include <math.h>

#include	"ImageType/ImageType.h"


// -----------------------RGB to HSI Conversion-----------------------
//  H = atan(y/x) = atan( (sqrt(3) * (G-B)) / (2R-G+b)
//  I = ( R + G + B) / 3
//  S = 1 - ( min(R,G,B) / I )


void	RGBtoHSI( float r, float g, float b, float *H, float *S, float *I );

static void	HSItoRGB( float H, float S,float I, float *r, float *g, float *b );


image_type *
image_HSI_tune( image_type *sim, float HUE, float Saturation, float Intensity )
{
image_type	*im;
u_int	*sp,	*p;
int	row,	col,	i,	j;
float	H,	S,	I;
float	r,	g,	b;
int	ri,	gi,	bi;

	row = IMAGE_ROW(sim);
	col = IMAGE_COLUMN(sim);

	im = image_create( row, col, 4, 1, NULL );

	sp = (u_int *)sim->data;
	p  = (u_int *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			r = IMAGE4_RED( *sp )/ 255.0;
			g = IMAGE4_GREEN( *sp )/ 255.0;
			b = IMAGE4_BLUE( *sp )/ 255.0;

			RGBtoHSI( r, g, b, &H, &S, &I );

			/*if ( S >= 1.0F )
			{
				*p++ = *sp++;
				continue;
			}*/

			H += HUE;
			if ( H > 360 )	H -= 360 ;
			if ( H <= 0)		H += 360;

			S *= 1.0F+ Saturation;
			//S = Saturation;
			S = PUSH_TO_RANGE( S, 0, 1.0 );

			I += Intensity;
			I = PUSH_TO_RANGE( I, 0, 1.0 );

			HSItoRGB( H, S, I, &r,&g, &b );

			ri = r*255;
			gi = g*255;
			bi = b*255;

			ri = PUSH_TO_RANGE( ri, 0, 255 );
			gi = PUSH_TO_RANGE( gi, 0, 255 );
			bi = PUSH_TO_RANGE( bi, 0, 255 );

			*p++ = IMAGE4_RGB( ri, gi, bi );
			sp++;

		}
	}

	return( im );
}


void
RGBtoHSI( float r, float g, float b, float *H,float *S, float *I )
{
double min, x,radius,temp;


	// Finding the value of I = Intensity
	*I =(double)( (double)(r + g + b) / (double)3.0 );
	
	// Finding the value of S = Saturation

	min = MIN( r, g );
	if( b < min )	min = b;
 

	*S = ( *I == 0 )? 0 : (*I - min) / *I;


	//*S = 1 - 3.0 * min / ((double)(rgb.r + rgb.g + rgb.b));

	// Finding the value of H = Hue
	radius = (0.5 * ((double)( r -  g) + (double)( r -  b))) ;
	x      = sqrt( (double) (( r- g)*( r -  g)) + (double) (( r- b)*( g -  b)) ) ;
    temp   = radius / x ;
	*H =(double) acos(temp); 
	*H = *H * 180.00 / M_PI ;

	if ( b >  g)
		*H = 360.0 - *H ;


}

static void
HSItoRGB( float H, float S,float I, float *r, float *g, float *b)
{
float	Hradian;


	if ( H>0 && H <= 120) {
		Hradian = 2.0 * M_PI * ( H / 360.0);
		
		*b = ( (1 - S) * I); 
		*r = ((1 + ( (S * cos(Hradian)) / cos((M_PI / 3.0) - Hradian) ) ) * I );
		*g = 3.0 * I - (*r + *b) ; 
		return;
	}

	if (H > 120 && H <= 240) {
		H      = H - 120 ;
		Hradian = 2.0 * M_PI * ( H / 360.0);

		*r = ( (1 - S) * I);
		*g = ((1 + ( (S * cos(Hradian)) / cos((M_PI / 3.0) - Hradian) ) ) * I);
		*b = 3.0 * I - (*g + *r);

		return;
	}


	if (H > 240 && H <= 360) {
		H      = H - 240 ;
		Hradian = 2.0 * M_PI * ( H / 360.0);
		*g = ( (1 - S) * I);
		*b = ((1 + ( (S * cos(Hradian)) / cos((M_PI / 3.0) - Hradian) ) ) * I);
		*r = 3.0 * I - (*g + *b) ;	

		return;
	}
}



///////////////////////////////////////////////////////////
/*            Old variant								 */
///////////////////////////////////////////////////////////

void ConvertRGB2HSI(u_int red, u_int green, u_int blue, 
					double *H, double *S, double *I );

void ConvertHSI2RGB(double INhue,double INsaturation,double INintensity,
					u_int *R, u_int *G, u_int *B);


#ifndef MAX
#define		MAX( a, b )        (( (a) < (b) )? (b) : (a) )
#endif

#ifndef MIN
#define		MIN( a, b )        (( (a) < (b) )? (a) : (b) )
#endif

image_type *
image_HSI_tune_( image_type *sim, float HUE, float Saturation, float Intensity )
{
image_type	*im;
u_int	*sp,	*p;
int	row,	col,	i,	j;
double	H,	S,	I;
u_int	r,	g,	b;

	row = IMAGE_ROW(sim);
	col = IMAGE_COLUMN(sim);

	im = image_create( row, col, 4, 1, NULL );

	sp = (u_int *)sim->data;
	p  = (u_int *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			r = IMAGE4_RED( *sp );
			g = IMAGE4_GREEN( *sp );
			b = IMAGE4_BLUE( *sp );

			ConvertRGB2HSI( r, g, b, &H, &S, &I );

			H += HUE;
			H = PUSH_TO_RANGE( H, 0, 1.0 );

			S *= 1.0F+ Saturation;
			//S = Saturation;
			S = PUSH_TO_RANGE( S, 0, 1.0 );

			I += Intensity;
			I = PUSH_TO_RANGE( I, 0, 1.0 );

			ConvertHSI2RGB( H, S, I, &r, &g, &b );

			r = PUSH_TO_RANGE( r, 0, 255 );
			g = PUSH_TO_RANGE( g, 0, 255 );
			b = PUSH_TO_RANGE( b, 0, 255 );

			*p++ = IMAGE4_RGB( r, g, b );
			sp++;

		}
	}

	return( im );
}


void ConvertRGB2HSI(u_int red, u_int green, u_int blue, 
					double *H, double *S, double *I )
{

	double hueRange = 1;
	double max = MAX(red,MAX(green,blue));
	double min = MIN(red,MIN(green,blue));
	double mid;

	double brightness;
	double saturation;
	double hue;

	double bri;
	double desaturator;

	double domainBase;
	double oneSixth;
	double domainOffset;

	if (max != red && min != red)
		mid = red;
	else 
	if (max != blue && min != blue)
		mid = blue;
	else
		mid = green;

	brightness = 0.0;  //defaults
	saturation = 0.0;
	hue = 0.0;

	bri = max;

	if (bri == 0.0)    // black
	{
		*H = hue;
		*I = brightness;
		*S = saturation;
		return;
	}

	brightness = bri / 255.0;


	desaturator = min;
	if (bri == desaturator) // grey
	{ 
		*H = hue;
		*I = brightness;
		*S = saturation;
		return; 
	}

	saturation = (brightness - (desaturator/255.0))/brightness;


	// 'domains' are 60 degrees of red, yellow, green, cyan, blue, or magenta  

	domainBase = 0.0;
	oneSixth = 1.0 / 6.0;
	domainOffset = ((mid - desaturator) / (bri - desaturator)) / 6.0;

	if (red == bri) {
	if (mid == green) {  // green is ascending
	  domainBase = 0.0;             // red domain
	}
	else {  // blue is descending
	   domainBase = 5.0/6.0;         // magenta domain
	   domainOffset = oneSixth - domainOffset;
	}

	} else {
	 if (green == bri) {
		if (mid == blue) {  // blue is ascending
			domainBase = 2.0/6.0;   // green domain
		}
		else { // red is descending
		   domainBase = 1.0/6.0;    // yellow domain
		   domainOffset = oneSixth - domainOffset;
		}
	 }
	 else {
		if (mid == red) {  // red is ascending
		   domainBase = 4.0/6.0;      // blue domain
		}
		else {  // green is descending
		   domainBase = 3.0/6.0;    // cyan domain
		   domainOffset = oneSixth - domainOffset;
		}
	 }
	}
 
	hue = domainBase + domainOffset;

	*H = hue;
	*I = brightness;
	*S = saturation;
}

void ConvertHSI2RGB(double INhue,double INsaturation,double INintensity,
					u_int *R, u_int *G, u_int *B)
{
	double hueRange = 1;

	double hue =        INhue / hueRange;
	double saturation = INsaturation;
	double brightness = INintensity; // INTENSITY IS SAME AS BRIGHTNESS

	double domainOffset;      
	double red,blue,green;

	if (hue < 0 || hue > 1 || saturation < 0 || saturation > 1 ||
		brightness < 0 || brightness > 1) 
	{
		//One or more of the HSI values is out of range
		return;
	}

	if (brightness == 0.0) // black
	{ 
		*R = 0;
		*G = 0;
		*B = 0;
		return;
	}

	if (saturation == 0.0)  // grey
	{
		*R = (int)(255*brightness); 
		*G = (int)(255*brightness); 
		*B = (int)(255*brightness); 
		return;
	}

	domainOffset = 0;      

	if (hue < 1.0 / 6) 
	{		   // red domain; green ascends
		domainOffset = hue;
		red = brightness;
		blue = brightness * (1.0 - saturation);
		green = blue + (brightness - blue) * domainOffset * 6;
	}
	else if (hue < 2.0/6)
	{         // yellow domain; red descends
		domainOffset = hue - 1.0/6;
		green = brightness;
		blue = brightness * (1.0 - saturation);
		red = green - (brightness - blue) * domainOffset * 6;
	}   
	else if (hue < 3.0/6)     // green domain; blue ascends
		{
			domainOffset = hue - 2.0/6;
			green = brightness;
			red = brightness * (1.0 - saturation);
			blue = red + (brightness - red) * domainOffset * 6;
	}
	else if (hue < 4.0 / 6)   // cyan domain; green descends
	{
		domainOffset = hue - 3.0/6;
		blue = brightness;
		red = brightness * (1.0 - saturation);
		green = blue - (brightness - red) * domainOffset * 6;
	}
	else if (hue < 5.0 / 6)        // blue domain; red ascends
	{
		domainOffset = hue - 4.0/6;
		blue = brightness;
		green = brightness * (1.0 - saturation);
		red = green + (brightness - green) * domainOffset * 6;
	}
	else {                     // magenta domain; blue descends
		domainOffset = hue - 5.0/6;
		red = brightness;
		green = brightness * (1.0 - saturation);
		blue = red - (brightness - green) * domainOffset * 6;
	}

	*R = (int)(255*red);
	*G = (int)(255*green);
	*B = (int)(255*blue);
}



float
image_Saturation( image_type *sim )
{
u_int	*sp;
int		i,	j;
float	H,	S,	I;
float	sumS = 0;
float	avgS;
float	r,	g,	b;

	sp = (u_int *)sim->data;
	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++ ){

			r = IMAGE4_RED( *sp )/ 255.0;
			g = IMAGE4_GREEN( *sp )/ 255.0;
			b = IMAGE4_BLUE( *sp )/ 255.0;

			RGBtoHSI( r, g, b, &H, &S, &I );
		
			S = PUSH_TO_RANGE( S, 0, 1.0 );
			sumS += S;

			sp++;
		}
	}
	
	avgS = sumS / ( sim->row * sim->column );

	return( avgS );
}