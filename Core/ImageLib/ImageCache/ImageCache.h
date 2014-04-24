

#ifndef __IMAGE_CACHE_H__
#define __IMAGE_CACHE_H__




#include	"ImageType/ImageType.h"





typedef struct imageHolder_type {

	char	file[256];

	u_int	time;

	image_type *im;

	int	use;


} imageHolder_type;




class CImageCache 
{
public:
	CImageCache( int nIm = 16 );

	virtual ~CImageCache();




	int		Get( char *file, image_type **im );

	int		Release( image_type **im );

	void	Clear();

private:
	void	DeleteContents();

	int		CloseImage();



public:
	int	m_nH;
	int	m_NH;

	imageHolder_type	*m_aH;

};
#endif


