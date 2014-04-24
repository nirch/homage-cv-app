
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

#ifdef _DEBUG
#define _DUMP 
#endif

#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"
#include	"ImageMark/ImageMark.h"
#include	"ImageLabel/ImageLabel.h"
#include	"BnImage/BnImage.h"

#include "UniformBackground.h"

#include "ImageMorphology/ImageMorphology.h"


#define EXCEPTION


void	image3_histogram_RGB( image_type *sim, image_type *mim, int *r, int *g, int *b );



static image_type *	image1_nn( image_type *sim, int N, image_type *im );

static void	image1_nn1( image_type *im, int N );

static void	image1_nn2( image_type *im );

static void	image1_nn_clear( image_type *sim, int N, image_type *im );



int	CUniformBackground::ProcessThin( int N )
{

	gpTime_start( &m_tThin );
	m_cim8 = image1_nn( m_cim, N, m_cim8 );

	if( m_iFrame == m_dFrame ){
		IMAGE_DUMP_DUP( m_cim8, N, 1, "m", m_iFrame, "d2-1" );
	}


	image1_nn1( m_cim8, N );

	if( m_iFrame == m_dFrame ){
		IMAGE_DUMP_DUP( m_cim8, N, 1, "m", m_iFrame, "d2-2" );
	}

	if( N == 4 ){
//		image1_nn2( m_cim8 );
		image1_remove_blobM( m_cim8, 0x40, 10, 1, 0x40 );
	}
	else
		image1_remove_blobM( m_cim8, 0x80, 10, 1, 0x40 );

	if( m_iFrame == m_dFrame ){
		IMAGE_DUMP_DUP( m_cim8, N, 1, "m", m_iFrame, "d2-3" );
	}
	
	image1_nn_clear( m_cim, N, m_cim8 );

	if( m_iFrame == m_dFrame ){
		IMAGE_DUMP( m_cim, "m", m_iFrame, "d2" );
	}

	gpTime_stop( &m_tThin );


	return( 1 );
}



static image_type *
image1_nn( image_type *sim, int N, image_type *im )
{
	u_char	*sp;
	int	i,	j,	k,	n,	s;


	int width = (sim->width+N-1)/N ;
	int	height = ( sim->height+N-1)/N;


	im = image_realloc( im, width, height, 1, IMAGE_TYPE_U8, 1 );
	image1_const( im, 0 );


	u_char *tp = im->data;
	for( i = 0 ; i < sim->row ; i += N ){
		

		u_char *sp0 = IMAGE_PIXEL( sim, i, 0 );
		int k1 = ( i + N < sim->height )? N : sim->height - i;
		for( j = 0 ; j < sim->column ; j += N, sp0 += N, tp++ ){

			int n1 = ( j + N < sim->width )? N : sim->width - j;
			int align = sim->width - n1;
			for( k = 0, sp = sp0, s = 0 ; k < k1 ; k++, sp += align )
				for( n = 0 ; n < n1 ; n++, sp++ ){
					if( *sp != 0 )	s++;
				}

			*tp = s;
		}
	}


	return( im );
}



static void
image1_nn1( image_type *im, int N )
{

	int	i,	j;


	int	N1 = 0.75*N*N;
	int	N2 = 0.75 *N *N;
	u_char *sp = IMAGE_PIXEL( im, 1, 1 );
	for( i = 1 ; i < im->row-1 ; i++, sp += 2 ){
		u_char *sp0 = sp - im->width;
		u_char *sp1 = sp + im->width;

		for( j = 1 ; j < im->column-1 ; j ++, sp++, sp0++, sp1++ ){
			if( *sp == 0 )	continue;

			if( (*sp&0x7F) < N2 )	continue;
				
			int n = ((*(sp0-1)&0x7F) > N1)  +   ((*(sp0)&0x7F) > N1) + ((*(sp0+1)&0x7F) > N1 ) + 
				 ((*(sp-1 )&0x7F) > N1)  +   ((*(sp )&0x7F) > N1) + ((*(sp+1 )&0x7F) > N1 ) + 
				 ((*(sp1-1)&0x7F) > N1)  +   ((*(sp1)&0x7F) > N1) + ((*(sp1+1)&0x7F) > N1 ) ;

			if( n < 5 )
				continue;

			if( *(sp0-1) != 0 )
				*(sp0-1) |= 0x80;
			if( *(sp0) != 0 )
				*(sp0  ) |= 0x80;
			if( *(sp0+1) != 0 )
				*(sp0+1) |= 0x80;
			
			if( *(sp-1) != 0 )
				*(sp-1) |= 0x80;
			if( *(sp) != 0 )
				*(sp  ) |= 0x80;
			if( *(sp+1) != 0 )
				*(sp+1) |= 0x80;

			if( *(sp1-1) != 0 )
				*(sp1-1) |= 0x80;
			if( *(sp1) != 0 )
				*(sp1  ) |= 0x80;
			if( *(sp1+1) != 0 )
				*(sp1+1) |= 0x80;
		}
	}
}

static void
image1_nn2( image_type *im )
{
	int	i,	j;

	u_char *sp = IMAGE_PIXEL( im, 1, 1 );
	for( i = 1 ; i < im->row-1 ; i++, sp += 2 ){
		u_char *sp0 = sp - im->width;
		u_char *sp1 = sp + im->width;

		for( j = 1 ; j < im->column-1 ; j ++, sp++, sp0++, sp1++ ){
			if( *sp == 0 || *sp >= 0x80 )	continue;

			int n = *(sp0-1) | *(sp0) | *(sp0+1) | 
				 *(sp-1) | *(sp+1) |
				  *(sp1-1) | *(sp1) | *(sp1+1);

			if( n & 0x80 )
				*sp |= 0x40;
		}
	}
}



static void
image1_nn_clear( image_type *sim, int N, image_type *im )
{
	u_char	*sp;
	int	i,	j,	k,	n;




	u_char *tp = im->data;
	for( i = 0 ; i < sim->row ; i += N ){

		u_char *sp0 = IMAGE_PIXEL( sim, i, 0 );
		int k1 = ( i + N < sim->height )? N : sim->height - i;
		for( j = 0 ; j < sim->column ; j += N, sp0 += N, tp++ ){

			if( *tp == 0 || (*tp >= 0x80 ) )
				continue;

			*tp = 0;

			int n1 = ( j + N < sim->width )? N : sim->width - j;
			int align = sim->width - n1;
			for( k = 0, sp = sp0 ; k < k1 ; k++, sp += align )
				for( n = 0 ; n < n1 ; n++, sp++ ){
					*sp = 0;
				}
		}
	}
}