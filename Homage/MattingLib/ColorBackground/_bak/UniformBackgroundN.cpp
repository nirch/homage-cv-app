
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

#include "UniformBackgroundN.h"

#include "BnImage/BnImage.h"




#define RGB2CBI( R, G, B )	((-0.16874 * R - 0.33126 * G + 0.50000 * B) + 128.5)
#define RGB2CRI( R, G, B )	(( 0.50000 * R - 0.41869 * G - 0.08131 * B) + 128.5)

#define RGB2CB( R, G, B )	((-11058 * R - 21709 * G + 32768 * B + 8421376)>>16)
#define RGB2CR( R, G, B )	(( 32768 * R - 27439 * G - 5328  * B + 8421376)>>16)


static void	image3_histogram_RGB( image_type *sim, image_type *mim, int *r, int *g, int *b );

static image_type *	bnImage_dd( image_type *sim, int T, image_type *im );




CUniformBackgroundN::CUniformBackgroundN()
{


	m_cim = NULL;


	m_T = 12;

	m_bnIm  = NULL;
	

	gpTime_init( &m_gTime );


}


CUniformBackgroundN::~CUniformBackgroundN()
{
	DeleteContents();

}



void CUniformBackgroundN::DeleteContents()
{

	//if( m_him != NULL ){
	//	image_destroy( m_him, 1 );
	//	m_him = NULL;
	//}

}




int	CUniformBackgroundN::ProcessHistogram( image_type *sim, image_type *mim )
{
	
	image3_histogram_RGB( sim, mim, &m_r, &m_g, &m_b );


	return( 1 );


}






int	CUniformBackgroundN::Process( image_type *sim, image_type *mim, image_type **cim )
{

	gpTime_start( &m_gTime );

	image_type *yim = image1_from( sim, NULL );
	m_bnIm = bnImage_average4( yim, m_bnIm );
	image_destroy( yim, 1 );

	
	image_type *im = bnImage_dd( m_bnIm, m_T, NULL );

	IMAGE_DUMP( im, "aa", 1, NULL );
	m_cim = image_realloc( m_cim, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );




	m_cim = image1_dup( im, 8, m_cim );
	IMAGE_DUMP( m_cim, "bb", 1, NULL );

#ifdef _AA_
	mp = mim->data;
	sp = sim->data;
	tp = m_cim->data;
	


	for( i = 0 ; i < im->height ; i++ ){
		for( j = 0 ; j < im->width ; j++,  mp++, tp++ ){
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
		

			if( d < 20 )
				*tp = 0;
			else *tp = 255;

		}
	}


	image1_remove_blob( m_cim, 255, 400, 0 );
#endif

	image_destroy( im, 1 );

	*cim = m_cim;


	gpTime_stop( &m_gTime );

	return( 1 );
	//	image_dump( im, "mask", 1, "mm"  );
}





static image_type *
bnImage_dd( image_type *sim, int T, image_type *im )
{

	short	*sp;
	int	i,	j;
	int	no;



	im = image_realloc( im, sim->width/2 -1, sim->height/2-1,1, IMAGE_TYPE_U8, 1 );




	u_char *tp = im->data;

	for( i = 0,	no = 0 ; i < im->height ; i++ ){
		sp = (short*)IMAGE_PIXEL( sim, 2*i+1, 1 );
		short *sp0 = sp - sim->width;
		short *sp1 = sp + sim->width;
		for( j = 0 ; j < im->width ; j++, sp += 2, sp1 += 2, sp0 += 2, tp++ ){
			int	d,	dMax = 0;

			if( (d = *(sp0-1) - *(sp) ) < 0)	 d = -d;
			if( d > dMax )	dMax = d;

			if( (d = *(sp0+0) - *(sp) ) < 0)	 d = -d;
			if( d > dMax )	dMax = d;

			if( (d = *(sp0+1) - *(sp) ) < 0)	 d = -d;
			if( d > dMax )	dMax = d;


			if( (d = *(sp-1) - *(sp) ) < 0)	 d = -d;
			if( d > dMax )	dMax = d;


			if( (d = *(sp+1) - *(sp) ) < 0)	 d = -d;
			if( d > dMax )	dMax = d;



			if( (d = *(sp1-1) - *(sp) ) < 0)	 d = -d;
			if( d > dMax )	dMax = d;

			if( (d = *(sp1+0) - *(sp) ) < 0)	 d = -d;
			if( d > dMax )	dMax = d;

			if( (d = *(sp1+1) - *(sp) ) < 0)	 d = -d;
			if( d > dMax )	dMax = d;


			if( dMax < T*16 )
				*tp = 0;
			else	
				*tp = 255;
		}
	}

	return( im );
}


static void
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




void CUniformBackgroundN::Trace( FILE *fp )
{
	gpTime_print( fp, "UniformBackground", &m_gTime );
}