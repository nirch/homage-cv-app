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



static image_type *
image_gray_enhancement_hide( image_type *sim );

static image_type *
image_gray_enhancement_histogram( image_type *simY, 
		int part, int limit );

static void
image_gray_enhancement_intensity( image_type *simY );



image_type *
image_gray_enhancement( image_type *sim, int enhan_type )
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
image_type	*imY, *simY;
u_char		FdestroySimY;


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
		return image_gray_enhancement_hide( sim );

	simY = sim;
	FdestroySimY = 0;

	if ( sim->depth == 4 )
	{
		simY = image4_to_y( sim, NULL );
		FdestroySimY = 1;
	}

	param = e_param+state;

	imY = image_gray_enhancement_histogram( simY, 
		param->lower_threshold, param->max_lower);

	if ( state == STATE_DIM )
		image_gray_enhancement_intensity( imY );

	if ( FdestroySimY )
		image_destroy( simY , 1 );

	return imY;
}


static image_type *
image_gray_enhancement_hide( image_type *sim )
{
image_type *im;
u_char		*p;
int			i, j;

	im = image_create( sim->row, sim->column, 1, 1, NULL );
	p = (u_char *)im->data;

	for( i = 0; i < im->row ; i++ )
	for( j = 0; j < im->column ; j++ )
		*p++ = 0xFF;

	return im;
};


static image_type *
image_gray_enhancement_histogram( image_type *simY, int part, int limit )
{
image_type	*imY;
int	H[256];
int	pixles, sum;
int	partR_pixles, partL_pixles;
u_char *p, *sp;
int	i, j, left, right, delta;
float leftN, rightN;
int	Y;
float rel, relL, relR;

	image1_histogram( simY, NULL, H, 1 );

	pixles = IMAGE_ROW(simY) * IMAGE_COLUMN(simY);

	partL_pixles = (int)(((float)pixles)*((float)part)/100.0F);
	left = 0;
	
	for( i = 0, sum = 0 ; i < 255; i++ ){
		sum += H[i];
		if ( sum > partL_pixles )
		{
			left = i;
			break;
		}
	}

	leftN = left - limit;
	if ( leftN < 0.0F )
		leftN = 0.0F;

	partR_pixles = (int)(((float)pixles)*((float)part)/100.0F);
	right = 255;

	for( i = 255, sum = 0 ; i >= 0; i-- ){
		sum += H[i];
		if ( sum > partR_pixles )
		{
			right = i;
			break;
		}
	}

	rightN = right + limit;
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

	imY = image_create( IMAGE_ROW(simY), IMAGE_COLUMN(simY), 1, 1, NULL );
 
	sp = (u_char *)simY->data;
	p = (u_char *)imY->data;
	for( i = 0 ; i < imY->row ; i++ )
		for( j = 0 ; j < imY->column ; j++ )
		{
			Y = *sp;
			sp++;

			if ( Y < left )
			{
				Y = Y*relL;
				Y = PUSH_TO_RANGE( Y, 0, 255 );
			}
			else if ( Y > right )
			{
				Y = rightN + (Y - right)*relR;
				Y = PUSH_TO_RANGE( Y, 0, 255 );
			}
			else
			{
				Y = leftN + (Y - left)*rel;
				Y = PUSH_TO_RANGE( Y, 0, 255 );
			}

			*p++ = (u_char)Y;
		}

	return( imY );
}


static void
image_gray_enhancement_intensity( image_type *simY )
{
int i, j;
u_char *p;

	p = (u_char *)simY->data;
	for( i = 0 ; i < simY->row ; i++ )
		for( j = 0 ; j < simY->column ; j++ )
			*p++ = (u_char)(((float)(*((u_char *)p)))/2 + 127.5F);

}



image_type *
image_gray_enhancement_state( image_type *sim, int state )
{

Enhancement_param *param, e_param[6] = 
	{ 	{ 5, 35 },
		{ 9, 50 },
		{ 13, 60 },
		{ 17, 70 },
		{ 21, 80 },
		{ 9, 50 } };

image_type	*imY, *simY;
u_char		FdestroySimY;


	if ( state == -1 )
		return NULL;

	simY = sim;
	FdestroySimY = 0;

	if ( sim->depth == 4 )
	{
		simY = image4_to_y( sim, NULL );
		FdestroySimY = 1;
	}

	param = e_param+state;

	imY = image_gray_enhancement_histogram( simY, 
				param->lower_threshold,  param->max_lower );

	if ( state == STATE_DIM )
		image_gray_enhancement_intensity( imY );

	if ( FdestroySimY )
		image_destroy( simY , 1 );

	return imY;
}

