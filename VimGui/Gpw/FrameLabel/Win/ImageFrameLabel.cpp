/******************************
 ***   TextFrameLabel.cpp   ***
 ******************************/
#include <windows.h>
#include <atlimage.h>

#include "Utime/GpTime.h"
#include "ImageFrameLabel.h"

#include "Wutil/UWdib/Cdib.h"

#include "Gpw/Gpw.h"

#include "ImageType/ImageType.h"

#include "ImageCodec/ImageRead.h"
#include "Png\PngCodec\Png\ImageReadPng.h"





static int LoadResourcePng( LPCTSTR lpszResourceName, HINSTANCE hinstRes, image_type **im );

static void	imageT_PreAlpahBlend( image_type *sim );


CImageFrameLabel::CImageFrameLabel()
{

	m_hdc = NULL;
	m_hBitmap = NULL;

	m_im = NULL;
}



CImageFrameLabel::~CImageFrameLabel()
{
	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}
}



int  CImageFrameLabel::InitFile( char *inFile, int fx, int fy )
{


	if( (m_im = image_read_png_file( inFile )) == NULL )
		return( -1 );

	imageT_PreAlpahBlend( m_im );


	HBITMAP hb = CreateBitmap( m_im->width, m_im->height, 1, 32, m_im->data );


	int ret = Init( hb, fx, fy );
	return( ret );
}


int  CImageFrameLabel::Init( HINSTANCE hInstance,  int ID, int fx, int fy )
{
	HBITMAP hb = LoadBitmap( hInstance, MAKEINTRESOURCE(ID) );
	if( hb == NULL )	return( -1 );

	int ret = Init( hb, fx, fy );
	return( ret );
}


int  CImageFrameLabel::InitPng( HINSTANCE hInstance, int ID, int fx, int fy )
{

	
	if( LoadResourcePng( MAKEINTRESOURCE(ID), hInstance, &m_im ) < 0 )
		return( -1 );


	imageT_PreAlpahBlend( m_im );


	HBITMAP hb = CreateBitmap( m_im->width, m_im->height, 1, 32, m_im->data );


	int ret = Init( hb, fx, fy );
	return( ret );
}

int  CImageFrameLabel::Init( HBITMAP hBitmap, int fx, int fy )
{
	m_hBitmap = hBitmap;

	BITMAP bm;
	::GetObject( m_hBitmap, sizeof( bm ), &bm );
	m_width = bm.bmWidth;
	m_height = bm.bmHeight;

	SetFocus( fx, fy );

	return( 1 );
}





typedef struct argb_type {
	u_char	b;
	u_char	g;
	u_char	r;
	u_char	a;

} rgba_type;


static void
imageT_PreAlpahBlend( image_type *sim )
{
int	i,	j;

	rgba_type *sp = (rgba_type *)sim->data;
	for( i = 0; i < sim->height ; i++ ){
		for( j = 0; j < sim->width ; j++, sp++ ){

			sp->a = 255 - sp->a;
			if( sp->a == 0 ){
				sp->r = sp->g = sp->b = 0;
				continue;
			}

			if( sp->r == 0 && sp->g == 0 && sp->b == 0 ){
				sp->a = 0;
				continue;
			}


			sp->r = (sp->r *sp->a)/255.0;
			sp->g = (sp->g *sp->a)/255.0;
			sp->b = (sp->b *sp->a)/255.0;
		}
	}

}




void CImageFrameLabel::Draw( HDC hdc )
{
	if( m_show == 0 )
		return;

	if( m_hdc == NULL ){
		m_hdc =  ::CreateCompatibleDC( hdc );

		HGDIOBJ hOld = ::SelectObject( m_hdc, m_hBitmap );
	}


	BLENDFUNCTION ftn;
	ftn.BlendOp = AC_SRC_OVER;
	ftn.BlendFlags = 0;
	ftn.SourceConstantAlpha = 255;//(BYTE)(m_transparent *255);
	ftn.AlphaFormat = AC_SRC_ALPHA;//0;


	int	wx,	wy;
	m_gpw->xy2win( m_x, m_y, &wx, &wy );

	AlphaBlend( hdc, wx - m_fx, wy - m_fy, m_width, m_height, 
		m_hdc, 0, 0, m_width, m_height,
		ftn );

	//AlphaBlend( hdc, m_x - m_fx, m_y - m_fy, m_width, m_height, 
	//			m_hdc, 0, 0, m_width, m_height,
	//			ftn );

}



int CImageFrameLabel::HandleLButtonDown( int x, int y )
{
	if( IsVisable() == 0 )
		return( -1 );

	x -= m_wx;
	y -= m_wy;

	if( x < 0 || x >= m_im->width ||  y < 0 || y >= m_im->height )
		return( -1 );

	u_int *sp = IMAGE4_PIXEL( m_im, y,x );

	if( (*sp & 0xFF000000) == 0 )
		return( -1 );


	return( 1 );
}



int CImageFrameLabel::HandleLButtonUp(u_int nFlags, int x, int y )
{

	//x -= m_x0;
	//y -= m_y0;


	//if( m_show == 1 ){
	//	if( BOX2D_IS_IN( m_xBox, x, y ) ){
	//		SetShow( 0 );
	//		return( 1 );
	//	}
	//}

	return( -1 );
}



/*
//----- load png into CImage from resource
bool Load( CImage * pimage, LPCTSTR lpszResourceName, HINSTANCE hinstRes)
{
	//if (hinstRes == NULL)
	//{
	//	hinstRes = AfxFindResourceHandle(lpszResourceName, _T("PNG") );
	//}

	HRSRC hRsrc = ::FindResource(hinstRes, lpszResourceName, _T("PNG") );
	if (hRsrc == NULL)
	{
		return false;
	}

	HGLOBAL hGlobal = LoadResource(hinstRes, hRsrc);
	if (hGlobal == NULL)
	{
		return false;
	}

	LPBYTE lpBuffer = (LPBYTE) ::LockResource(hGlobal);
	if (lpBuffer == NULL)
	{
		FreeResource(hGlobal);
		return false;
	}

	bool bRes = false;
	{
		UINT uiSize = ::SizeofResource(hinstRes, hRsrc);

		HGLOBAL hRes = ::GlobalAlloc(GMEM_MOVEABLE, uiSize);
		if (hRes != NULL)
		{
			IStream* pStream = NULL;
			LPVOID lpResBuffer = ::GlobalLock(hRes);


			memcpy(lpResBuffer, lpBuffer, uiSize);

			HRESULT hResult = ::CreateStreamOnHGlobal(hRes, TRUE, &pStream);

			if( hResult == S_OK)
			{
				pimage->Load(pStream);
				pStream->Release();
				bRes= true;
			}
		}
	}

	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	return bRes;
}
*/


static int LoadResourcePng( LPCTSTR lpszResourceName, HINSTANCE hinstRes, image_type **im )
{
	*im = NULL;

	HRSRC hRsrc = ::FindResource(hinstRes, lpszResourceName, _T("PNG") );
	if (hRsrc == NULL)
	{
		return( -1 );
	}

	HGLOBAL hGlobal = LoadResource(hinstRes, hRsrc);
	if (hGlobal == NULL)
	{
		return( -1 );
	}

	LPBYTE lpBuffer = (LPBYTE) ::LockResource(hGlobal);
	if (lpBuffer == NULL)
	{
		FreeResource(hGlobal);
		return( -1 );
	}

	bool bRes = false;
	{
		UINT uiSize = ::SizeofResource(hinstRes, hRsrc);

		HGLOBAL hRes = ::GlobalAlloc(GMEM_MOVEABLE, uiSize);
		if (hRes != NULL)
		{
			IStream* pStream = NULL;
			LPVOID lpResBuffer = ::GlobalLock(hRes);
			//			ASSERT (lpResBuffer != NULL);

			memcpy(lpResBuffer, lpBuffer, uiSize);

			*im = image_read_png_buffer( (char *)lpResBuffer, uiSize, 1 );
		}
	}

	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	return( (*im != NULL )? 1 : -1 );
}