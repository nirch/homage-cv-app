
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

//#ifdef _DEBUG
#define _DUMP 
//#endif

#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"
#include	"ImageMark/ImageMark.h"
#include	"ImageLabel/ImageLabel.h"

#include "UniformBackground.h"






#define RGB2CBI( R, G, B )	((-0.16874 * R - 0.33126 * G + 0.50000 * B) + 128.5)
#define RGB2CRI( R, G, B )	(( 0.50000 * R - 0.41869 * G - 0.08131 * B) + 128.5)

#define RGB2CB( R, G, B )	((-11058 * R - 21709 * G + 32768 * B + 8421376)>>16)
#define RGB2CR( R, G, B )	(( 32768 * R - 27439 * G - 5328  * B + 8421376)>>16)


void	image3_histogram_RGB( image_type *sim, image_type *mim, int *r, int *g, int *b );





CUniformBackground::CUniformBackground()
{


	m_cim = NULL;


	colorShiftThresh = 25;
	

	gpTime_init( &m_gTime );


}


CUniformBackground::~CUniformBackground()
{
	DeleteContents();

}



void CUniformBackground::DeleteContents()
{

	//if( m_him != NULL ){
	//	image_destroy( m_him, 1 );
	//	m_him = NULL;
	//}

}




int	CUniformBackground::ProcessHistogram( image_type *sim, image_type *mim )
{
	
	image3_histogram_RGB( sim, mim, &m_r, &m_g, &m_b );


	return( 1 );


}






int	CUniformBackground::Process( image_type *sim, image_type *mim, image_type **cim )
{
	int	i,	j;
	u_char	*sp,	*mp,	*tp;
	int	R,	G,	B;
	

	gpTime_start( &m_gTime );

	m_cim = image_realloc( m_cim, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	mp = mim->data;
	sp = sim->data;
	tp = m_cim->data;
	

	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++,  mp++, tp++ ){
			R = *sp++;
			G = *sp++;
			B = *sp++;

			if( *mp == 0 )	continue;



			int dr = R - m_r;
		
			int dg = G - m_g;
			int db = B - m_b;
			
			int d = ABS(dr);
			if( dg < 0 )	dg = -dg;
			if( d < dg )	d = dg;

			if( db < 0 )	db = -db;
			if( d < db )	d = db;
		

			if( d < colorShiftThresh )
				*tp = 0;
			else *tp = 255;

		}
	}


	image1_remove_blob( m_cim, 255, 400, 0 );


	*cim = m_cim;


	gpTime_stop( &m_gTime );

	return( 1 );
	//	image_dump( im, "mask", 1, "mm"  );
}






void
image3_histogram_RGB( image_type *sim, image_type *mim, int *r, int *g, int *b )
{
int	i,	j;
u_char	*sp,	*mp;
int	R,	G,	B;


	mp = mim->data;
	sp = sim->data;


	*r = *g = *b = 0;

	int	n = 0;
	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++,  mp++ ){
			R = *sp++;
			G = *sp++;
			B = *sp++;

			if( *mp == 0 )	continue;


			*r += R;
			*g += G;
			*b += B;
			n++;
		}
	}

	*r /= n;
	*g /= n;
	*b /= n;
}




void CUniformBackground::Trace( FILE *fp )
{
	gpTime_print( fp, "UniformBackground", &m_gTime );
}