/************************
 *	ImageBmpFile.c	*
 ************************/



#include	"Uigp/igp.h"
#include	"ImageType/ImageType.h"
#include	"ImageBmp.h"

#define BI_RGB  0
#define BI_RLE8 1
#define BI_RLE4 2


static int	image4_write_bmp( image_type *im, char *file );


static void	putshort( FILE *fp, int i );
static void	putint( FILE *fp, int i );

static void	image_writeBMP24( FILE *fp, image_type *im );
static void	image4_writeBMP24( FILE *fp, image_type *im );
static void image_writeBMP8( FILE *fp, image_type *im );



//retCode : -1 - could not open outFile
//			number of bytes written on success

int
image_writeN_bmp( image_type *im, char *fname, int i )
{
char	file[256];

	if( i >= 0 )
			sprintf( file, "%s-%.2d.bmp", fname, i );
	else	sprintf( file, "%s.bmp", fname );

	return image_write_bmp( im, file );
}



int
image_write_bmp( image_type *sim, char *file )
{
image_type	*im;
int	ret;

	if( IMAGE_CHANNEL(sim) == 1 ){
		im = image1_from( sim, NULL );
		
		if( im->palette == NULL )
			im->palette = palette_create( PALETTE_GRAY );
		
		ret = image4_write_bmp( im, file );

		if( im != sim )
			image_destroy( im, 1 );
	
		return( ret );
	}


	if( sim->depth == 4 || sim->depth == 3 ){
		ret = image4_write_bmp( sim, file );
		return( ret );
	}

	if( sim->depth == 1 ) {
		if( sim->palette == NULL )
			sim->palette = palette_create( PALETTE_GRAY );
		ret = image4_write_bmp( sim, file );
		return( ret );
	}

	im = image3_from( sim, NULL );
	ret = image4_write_bmp( im, file );
	image_destroy( im,1 );


	return( ret );
}


static int 
image4_write_bmp( image_type *im, char *file )
{
FILE	*fp;
int     i,	nc, nbits, bperlin, bytes;
int	row,	col;

	if( (fp = fopen( file, "wb" )) == NULL )
		return( -1 );

	row = IMAGE_ROW( im );
	col = IMAGE_COLUMN( im );

	nc = nbits = 0;

	nbits = (im->palette == NULL )? 24 : 8;

	nc = (im->palette == NULL )? 0 : im->palette->nColor;

	bperlin = ((col * nbits + 31) / 32) * 4;
				/* # bytes written per line */

	putc('B', fp);
	putc('M', fp);		/* BMP file magic number */

	/* compute filesize and write it */
	i = 14 +		/* size of bitmap file header */
		40 +		/* size of bitmap info header */
		(nc * 4) +	/* size of colormap */
		bperlin * row;	/* size of image data */

	putint(fp, i);
	putshort(fp, 0);	/* reserved1 */
	putshort(fp, 0);	/* reserved2 */
	putint(fp, 14 + 40 + (nc * 4));	/* offset from BOfile to BObitmap */

	putint(fp, 40);		/* biSize: size of bitmap info header */
	putint(fp, col);		/* biWidth */
	putint(fp, row);		/* biHeight */
	putshort(fp, 1);	/* biPlanes:  must be '1' */
	putshort(fp, nbits);	/* biBitCount: 1,4,8, or 24 */
	putint(fp, BI_RGB);	/* biCompression:  BI_RGB, BI_RLE8 or BI_RLE4 */
	putint(fp, bperlin * row);/* biSizeImage:  size of raw image data */
	putint(fp, 75 * 39);	/* biXPelsPerMeter: (75dpi * 39" per meter) */
	putint(fp, 75 * 39);	/* biYPelsPerMeter: (75dpi * 39" per meter) */
	putint(fp, nc);		/* biClrUsed: # of colors used in cmap */
	putint(fp, nc);		/* biClrImportant: same as above */

/* write COLORMAP if exist */
	if( im->palette != NULL ){
		int	r,	g,	b;
		for( i = 0 ; i < nc ; i++ ){
			b = im->palette->data[i].Blue;
			g = im->palette->data[i].Green;
			r = im->palette->data[i].Red;
			putc(b, fp);
			putc(g, fp);
			putc(r, fp);
			putc(0, fp);
		}
	}

	if( im->depth == 4 )
		image4_writeBMP24( fp, im );
	else if( im->depth == 1 )
		image_writeBMP8( fp, im );
	else	image_writeBMP24( fp, im );

	bytes = ftell(fp);
	fclose( fp );


	return bytes;
}

static void 
image_writeBMP8( FILE *fp, image_type *im )
{
int     i, j,	padb;
u_char	*p;
int	h,	w;

	h = IMAGE_ROW( im );
	w = IMAGE_COLUMN( im );

	padb = (4 - ((w * 3) % 4)) & 0x03;	/* # of pad bytes to write at
						 * EOscanline */

	padb = (4 - ((w ) % 4)) & 0x03;	/* # of pad bytes to write at
						 * EOscanline */

	for (i = h - 1; i >= 0; i--) {
		p = IMAGE_PIXEL( im, i, 0 );

		for (j = 0; j < w; j++) {
			putc(*(p), fp);		
			p++;
		}

		for (j = 0; j < padb; j++)	putc(0, fp);
	}
}



static void 
image_writeBMP24( FILE *fp, image_type *im )
{
int     i, j,	padb;
u_char	*p;
int	h,	w;

	h = IMAGE_ROW( im );
	w = IMAGE_COLUMN( im );

	padb = (4 - ((w * 3) % 4)) & 0x03;	/* # of pad bytes to write at
						 * EOscanline */

	for (i = h - 1; i >= 0; i--) {
		p = IMAGE_PIXEL( im, i, 0 );

		for (j = 0; j < w; j++) {
			putc(*(p+2), fp);	// blue
			putc(*(p+1), fp);	// green
			putc(*(p), fp);		// red

			p++;
			p++;
			p++;
		}

		for (j = 0; j < padb; j++)	putc(0, fp);
	}
}


static void 
image4_writeBMP24( FILE *fp, image_type *im )
{
int     i, j,	padb;
u_int	*p;
int	h,	w;

	h = IMAGE_ROW( im );
	w = IMAGE_COLUMN( im );

	padb = (4 - ((w * 3) % 4)) & 0x03;	/* # of pad bytes to write at
						 * EOscanline */

	for (i = h - 1; i >= 0; i--) {
		p = IMAGE4_PIXEL( im, i, 0 );

		for (j = 0; j < w; j++) {
/*
			putc( ((*p)>>16)&0xff, fp);
			putc( ((*p)>>8)&0xff, fp);
			putc( (*p)&0xff, fp);
*/
			putc( IMAGE4_BLUE(*p), fp );
			putc( IMAGE4_GREEN(*p), fp );
			putc( IMAGE4_RED(*p), fp );

			p++;
		}

		for (j = 0; j < padb; j++)	putc(0, fp);
	}
}




static void
putshort( FILE *fp, int i )
{
int	c,	c1;

	c = ((unsigned int ) i) & 0xff;  c1 = (((unsigned int) i)>>8) & 0xff;
	putc(c, fp);   putc(c1,fp);
}



static void
putint( FILE *fp, int i )
{
int	c,	c1,	c2,	c3;

	c  = ((unsigned int ) i)      & 0xff;
	c1 = (((unsigned int) i)>>8)  & 0xff;
	c2 = (((unsigned int) i)>>16) & 0xff;
	c3 = (((unsigned int) i)>>24) & 0xff;

	putc(c, fp);   putc(c1,fp);  putc(c2,fp);  putc(c3,fp);
}

