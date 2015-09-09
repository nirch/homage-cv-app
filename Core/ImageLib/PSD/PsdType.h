#ifndef	_PSD_TYPE_H_
#define	_PSD_TYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

struct image_type;



typedef struct psdLayer_type
{
    unsigned int	width, height;
    int				x, y;
	char   name[32];
    struct image_type *image;


	int	flag;
	int	clipping;
	int	matte;
	int	blendkey;


} psdLayer_type;



typedef struct psd_type {

	int	width,	height;

	int	resolution;


	int	mode;	//0-Bitmap, 1-Grayscale, 2-Indexed, 3-RGB, 4-CMYK, 7-Multichannel, 8-Duetone, 9-Lab


	int		nLayer;
	psdLayer_type	**Layer; 
	
	int		LAYER_NO;



	struct image_type *image;

} psd_type;




psd_type *	psd_alloc( int nLayer );

void	psd_realloc( psd_type *psd, int nLayer );

void	psd_destroy( psd_type *psd );

void	psd_add_layer( psd_type *psd, psdLayer_type *psdL );


psdLayer_type *	psdLayer_alloc();



void	psdLayer_destroy( psdLayer_type *psdL );

psdLayer_type * psdLayer_make_copy( psdLayer_type *psdL0 );


void		psd_build_image( psd_type *psd );

	// PsdRead.c
int		psd_read_file( char *file, psd_type **psd );

	// PsdWrite.c
int		psd_write_file( psd_type *psd, char *file );


#ifdef __cplusplus
}
#endif

#endif
