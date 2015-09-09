/*******************************************
 *** ImageEnhancement.c                    *
 *******************************************/

#include	"Uigp/igp.h"

#include	"ImageType/ImageType.h"
#include "ImageDithering.h"


#define STATE_BASIC				0
#define STATE_LOW				1
#define STATE_MEDIUM			2
#define STATE_HIGH				3
#define STATE_ICON				4
#define STATE_DIM				5
#define STATE_HIDE				6

typedef struct Enhancement_param{
	int lower_threshold;
	int max_lower;
} Enhancement_param;



image_type *
image_enhancement_histogram( image_type *sim4, image_type *simY, 
		int partL, int partR, int limL, int limR );

static void
image_enhancement_intensity( image_type	*im );



image_type *
image_enhancement( image_type *sim, int enhan_type )
{

Enhancement_param *param, e_param[6] = 
	{ 	{ 5, 35 },
		{ 9, 50 },
		{ 13, 60 },
		{ 17, 70 },
		{ 21, 80 },
		{ 9, 50 } };

int states[5][3] =
	{	//OBJECT		//TEXT			//BACKGROUND
		{ STATE_ICON,	STATE_ICON,		STATE_HIDE },
		{ STATE_HIGH,	STATE_ICON,		STATE_HIDE },
		{ STATE_MEDIUM,	STATE_ICON,		STATE_DIM },
		{ STATE_LOW,	STATE_HIGH,		STATE_DIM },
		{ STATE_BASIC,	STATE_MEDIUM,	STATE_LOW } };
	

int			state = -1;
int			size_mid, size_ind;
image_type	*im, *simY;
u_int		*p;
int			i, j;

	if ( enhan_type == -1 )
		enhan_type = ENHANCEMENT_OBJECT;

	size_mid = ( sim->column+ sim->row )/2.0F;

	if ( size_mid <= 20 )		size_ind = 0;
	else if ( size_mid <= 40 )	size_ind = 1;
	else if ( size_mid <= 70 )	size_ind = 2;
	else if ( size_mid <= 119 )	size_ind = 3;
	else						size_ind = 4;

	state = states[size_ind][enhan_type];

	if ( state == STATE_HIDE )
	{
		im = image4_copy( sim );
		p = (u_int *)im->data;
		for( i = 0; i < im->row ; i++ )
	    for( j = 0; j < im->column ; j++ )
			*p++ = 0x00FFFFFF;

		return im;
	};

	simY = image4_to_y( sim, NULL );

	param = e_param+state;

	im = image_enhancement_histogram( sim, simY, 
		param->lower_threshold, 100 - param->lower_threshold,
		param->max_lower, param->max_lower);

	if ( state == STATE_DIM )
		image_enhancement_intensity( im );

	image_destroy( simY , 1 );

	return im;
}


image_type *
image_enhancement_histogram( image_type *sim4, image_type *simY, 
		int partL, int partR, int limL, int limR )
{
image_type	*im;
int	H[256];
int	pixles, sum;
int	partR_pixles, partL_pixles;
int	*p,	*sp;
int	i, j, left, right, delta;
float leftN, rightN;
int	R,	G,	B;
float rel, relL, relR;

	image1_histogram( simY, NULL, H, 1 );

	pixles = IMAGE_ROW(sim4) * IMAGE_COLUMN(sim4);

	partL_pixles = (int)(((float)pixles)*((float)partL)/100.0F);
	if ( limL < 0 || limL > 255 ) 
		limL = 255;
	left = 0;
	
	for( i = 0, sum = 0 ; i < 255; i++ ){
		sum += H[i];
		if ( sum > partL_pixles )
		{
			left = i;
			break;
		}
	}

	leftN = left - limL;
	if ( leftN < 0.0F )
		leftN = 0.0F;

	partR_pixles = (int)(((float)pixles)*((float)(100-partR))/100.0F);
	limR = limL;
	right = 255;

	for( i = 255, sum = 0 ; i >= 0; i-- ){
		sum += H[i];
		if ( sum > partR_pixles )
		{
			right = i;
			break;
		}
	}

	rightN = right + limR;
	if ( rightN > 255.0F )
		rightN = 255.0F;

	delta = right - left;
	delta = PUSH_TO_RANGE( delta, 1, 255 );

	rel = (rightN - leftN)/((float)delta);
	relL = relR = 0;

	if ( left )
		relL = leftN/(float)left;

	if( right != 255 )
		relR = (255.0F - rightN)/(255.0F - (float)(right));

	im = image_create( IMAGE_ROW(sim4), IMAGE_COLUMN(sim4), 4, 1, NULL );
 
	sp = (int *)sim4->data;
	p = (int *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ )
		{
			R = IMAGE4_RED(*sp);
			G = IMAGE4_GREEN(*sp);
			B = IMAGE4_BLUE(*sp);
			sp++;

			if ( R < left )
			{
				R = R*relL;
				R = PUSH_TO_RANGE( R, 0, 255 );
			}
			else if ( R > right )
			{
				R = rightN + (R - right)*relR;
				R = PUSH_TO_RANGE( R, 0, 255 );
			}
			else
			{
				R = leftN + (R - left)*rel;
				R = PUSH_TO_RANGE( R, 0, 255 );
			}

			if ( G < left )
			{
				G = G*relL;
				G = PUSH_TO_RANGE( G, 0, 255 );
			}
			else if ( R > right )
			{
				G = rightN + (G - right)*relR;
				G = PUSH_TO_RANGE( G, 0, 255 );
			}
			else
			{
				G = leftN + (G - left)*rel;
				G = PUSH_TO_RANGE( G, 0, 255 );
			}

			if ( B < left )
			{
				B = B*relL;
				B = PUSH_TO_RANGE( B, 0, 255 );
			}
			else if ( R > right )
			{
				B = rightN + (B - right)*relR;
				B = PUSH_TO_RANGE( B, 0, 255 );
			}
			else
			{
				B = leftN + (B - left)*rel;
				B = PUSH_TO_RANGE( B, 0, 255 );
			}

			*p++ = IMAGE4_RGB( R, G, B );
		}

	return( im );
}


static void
image_enhancement_intensity( image_type	*im )
{
int i, j;
u_int *p;
int R, G, B;

	p = (int *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ )
		{
			R = IMAGE4_RED(*p);
			G = IMAGE4_GREEN(*p);
			B = IMAGE4_BLUE(*p);

			R = (int)((float)R/2 + 127.5F);
			R = PUSH_TO_RANGE( R, 0, 255 );
			G = (int)((float)G/2 + 127.5F);
			G = PUSH_TO_RANGE( G, 0, 255 );
			B = (int)((float)B/2 + 127.5F);
			B = PUSH_TO_RANGE( B, 0, 255 );

			*p++ = IMAGE4_RGB( R, G, B );
		}

}



image_type *
image_enhancement_state( image_type *sim, int state )
{

Enhancement_param *param, e_param[6] = 
	{ 	{ 5, 35 },
		{ 9, 50 },
		{ 13, 60 },
		{ 17, 70 },
		{ 21, 80 },
		{ 9, 50 } };

image_type	*im, *simY;

	if ( state == -1 )
	{
		im = image4_copy( sim );
		return im;
	};

	simY = image4_to_y( sim, NULL );

	param = e_param+state;

	im = image_enhancement_histogram( sim, simY, 
		param->lower_threshold, 100 - param->lower_threshold,
		param->max_lower, param->max_lower);

	if ( state == STATE_DIM )
		image_enhancement_intensity( im );

	return im;
}

/*
static image_type *
image_enhancement_histogramY( image_type *sim4, image_type *simY, 
		int partL, int partR, int limL, int limR );

image_type *
image_enhancement_stateY( image_type *sim, int state )
{

Enhancement_param *param, e_param[5] = 
	{ 	{ 5, 35 },
		{ 9, 50 },
		{ 13, 60 },
		{ 17, 70 },
		{ 21, 80 },
		{ 9, 50 } };


image_type	*im, *simY;

	if ( state == -1 )
	{
		im = image4_copy( sim );
		return im;
	};

	simY = image4_to_y( sim );

	param = e_param+state;

	im = image_enhancement_histogramY( sim, simY, 
		param->lower_threshold, 100 - param->lower_threshold,
		param->max_lower, param->max_lower);

	if ( param->Fintensity )
		image_enhancement_intensity( im );

	return im;
}


#define RGB_TO_Y(y) (PUSH_TO_RANGE((u_int)(IMAGE4_RED(y)*0.29900 + \
								0.58700 * IMAGE4_GREEN(y) + \
								0.11400 * IMAGE4_BLUE(y) + 0.5), 0, 255 ) )

static image_type *
image_enhancement_histogramY( image_type *sim4, image_type *simY, 
		int partL, int partR, int limL, int limR )
{
image_type	*im;
int	H[256];
int	pixles, sum;
int	partR_pixles, partL_pixles;
int	*p,	*sp;
int	i, j, left, right, delta;
int	R,	G,	B,  Y;
float rel;

	image_histogram( simY, H );

	pixles = IMAGE_ROW(sim4) * IMAGE_COLUMN(sim4);

	partL_pixles = (int)(((float)pixles)*((float)partL)/100.0F);
	if ( limL < 0 || limL > 255 ) 
		limL = 255;
	left = limL;
	
	for( i = 0, sum = 0 ; i <= limL; i++ ){
		sum += H[i];
		if ( sum > partL_pixles )
		{
			left = i;
			break;
		}
	}

	partR_pixles = (int)(((float)pixles)*((float)(100-partR))/100.0F);
	if ( limR < 0 || limR > 255 ) 
		limR = 0;
	right = limR;

	for( i = 255, sum = 0 ; i >= limR ; i-- ){
		sum += H[i];
		if ( sum > partR_pixles )
		{
			right = i;
			break;
		}
	}

	delta = right - left;
	delta = PUSH_TO_RANGE( delta, 1, 255 );

	rel = 255.0F/((float)delta);

	im = image_create( IMAGE_ROW(sim4), IMAGE_COLUMN(sim4), 4, 1, NULL );
 
	sp = (int *)sim4->data;
	p = (int *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ )
		{
			R = IMAGE4_RED(*sp);
			G = IMAGE4_GREEN(*sp);
			B = IMAGE4_BLUE(*sp);
			sp++;

			Y = RGB_TO_Y( *sp );
			Y = Y <= left ? 0 : 
				( Y >= right ? 255 : (Y - left)*rel );

			*p++ = IMAGE4_RGB( Y, Y, Y );
		}

	return( im );
}
*/
