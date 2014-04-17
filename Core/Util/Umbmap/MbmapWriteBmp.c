/***************************
	MbmapWriteBmp.c
****************************/



#include	"Uigp/igp.h"
#include	"Umbmap/mbmap.h"


#define BI_RGB  0
#define BI_RLE8 1
#define BI_RLE4 2



static void	putshort( FILE *fp, int i );
static void	putint( FILE *fp, int i );

static void	writeBMP24( FILE *fp, mbmap *r, mbmap *g, mbmap *b );



int 
mbmap_dump_rgb_to_bmp_file( char *dir, char *name,
				mbmap *r, mbmap *g, mbmap *b )
{
char	fname[256];
FILE	*fp;
int     i,	nc, nbits, bperlin;
int	h,	w;


	sprintf( fname, "%s/%s.bmp", dir, name );

	if( (fp = fopen( fname, "wb" )) == NULL )
		return( 0 );

	h = MBMAP_ROW(*r);
	w = MBMAP_COLUMN(*r);

	nc = nbits = 0;


	nbits = 24;
	nc = 0;


	bperlin = ((w * nbits + 31) / 32) * 4;	/* # bytes written per line */

	putc('B', fp);
	putc('M', fp);		/* BMP file magic number */

	/* compute filesize and write it */
	i = 14 +		/* size of bitmap file header */
		40 +		/* size of bitmap info header */
		(nc * 4) +	/* size of colormap */
		bperlin * h;	/* size of image data */

	putint(fp, i);
	putshort(fp, 0);	/* reserved1 */
	putshort(fp, 0);	/* reserved2 */
	putint(fp, 14 + 40 + (nc * 4));	/* offset from BOfile to BObitmap */

	putint(fp, 40);		/* biSize: size of bitmap info header */
	putint(fp, w);		/* biWidth */
	putint(fp, h);		/* biHeight */
	putshort(fp, 1);	/* biPlanes:  must be '1' */
	putshort(fp, nbits);	/* biBitCount: 1,4,8, or 24 */
	putint(fp, BI_RGB);	/* biCompression:  BI_RGB, BI_RLE8 or BI_RLE4 */
	putint(fp, bperlin * h);/* biSizeImage:  size of raw image data */
	putint(fp, 75 * 39);	/* biXPelsPerMeter: (75dpi * 39" per meter) */
	putint(fp, 75 * 39);	/* biYPelsPerMeter: (75dpi * 39" per meter) */
	putint(fp, nc);		/* biClrUsed: # of colors used in cmap */
	putint(fp, nc);		/* biClrImportant: same as above */


	writeBMP24( fp, r, b, g );

	fclose( fp );


	return 0;
}





/*******************************************/
static void 
writeBMP24( FILE *fp, mbmap *r, mbmap *g, mbmap *b )
{
int     i, j,	padb;
u_char	*rp,	*gp,	*bp;
int	h,	w;

	h = MBMAP_ROW(*r);
	w = MBMAP_COLUMN(*r);

	padb = (4 - ((w * 3) % 4)) & 0x03;	/* # of pad bytes to write at
						 * EOscanline */

	for (i = h - 1; i >= 0; i--) {
		rp = MBMAP_PIXEL_ADRESS( *r, i, 0 );
		gp = MBMAP_PIXEL_ADRESS( *g, i, 0 );
		bp = MBMAP_PIXEL_ADRESS( *b, i, 0 );

		for (j = 0; j < w; j++) {
			putc(*gp, fp);
			putc(*bp, fp);
			putc(*rp, fp);
			rp++;
			gp++;
			bp++;
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

