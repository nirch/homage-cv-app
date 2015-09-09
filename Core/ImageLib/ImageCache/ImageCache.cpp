#include <string.h>

#include "Utime/GpTime.h"

#include "ImageCodec/ImageRead.h"
//#include "Gtiff/ImageReadGtiff.h"


#include "ImageCache.h"





CImageCache::CImageCache( int nIm )
{
	m_NH = nIm;

//	if( m_NH <= 0 )	m_NH = 16;

	m_aH = ( imageHolder_type *)malloc( sizeof( imageHolder_type) );
	m_nH = 0;
}



CImageCache::~CImageCache()
{
	DeleteContents();
}


void CImageCache::DeleteContents()
{
	imageHolder_type	*h;

	for( int i = 0 ; i < m_nH ; i++ ){
		h = &m_aH[i];
		image_destroy( h->im, 1 );
	}

	m_nH = 0;

}


void CImageCache::Clear()
{
	DeleteContents();
}


int CImageCache::Get( char *file, image_type **im )
{
	imageHolder_type	*h;

	for( int i = 0 ; i < m_nH ; i++ ){
		h = &m_aH[i];

		if( gp_stricmp( h->file, file) == 0 ){
			h->use++;
			*im = h->im;
			h->time = gp_time(0);
			return( 1 );
		}
	}

	if( m_nH >= m_NH-1 ){
		if( CloseImage() < 0 )
			return( -1 );
	}

	h = &m_aH[m_nH];

	h->im = image3_read_file( file );
//	utm_type *utm;
//	image3_read_gtiff( file, &h->im, &utm );
	if( h->im == NULL ){
		*im = NULL;
		return( -1 );
	}


	strcpy( h->file, file );
	h->use = 0;
	m_nH++;



	h->use++;
	h->time = gp_time(0);
	*im = h->im;


	return( 1 );
}


int CImageCache::Release( image_type **im )
{
	imageHolder_type	*h;

	for( int i = 0 ; i < m_nH ; i++ ){
		h = &m_aH[i];
		if( h->im == *im ){
			*im = NULL;
			h->use--;
			return( 1 );
		}
	}

	return( -1 );
}

int CImageCache::CloseImage()
{
imageHolder_type	*h;
int	iMin;
u_int tMin;

	iMin = -1;

	tMin = 0; 
	for( int i = 0 ; i < m_nH ; i++ ){
		h = &m_aH[i];

		if( h->use > 0 )	continue;

		if( iMin < 0 || h->time < tMin ){
			tMin = h->time;
			iMin = i;
		}
	}

	if( iMin < 0 )	return( -1 );

	image_destroy( m_aH[iMin].im, 1 );
	m_nH--;
	m_aH[iMin] = m_aH[m_nH];

	return( 1 );

}

