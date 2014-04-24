/************************
 *	ImageBmpFile.c	*
 ************************/

#include	<string.h>

#include	"Uigp/igp.h"

#include	"Ugio/GioType.h"

#include	"ImageType/ImageType.h"
#include	"ImageBmp.h"




#define BI_RGB  0
#define BI_RLE8 1
#define BI_RLE4 2
#define BI_BITFIELDS 3


#define WIN_OS2_OLD 12
#define WIN_NEW     40
#define OS2_NEW     64


static void	image4_readBMP4( gio_type *gio, int colormap[], image_type *im);

static void	image4_readBMP8( gio_type *gio, int colormap[], image_type *im);

static void	image4_readBMP24( gio_type *gio, image_type *im );

static image_type *		image_readBMP16( gio_type *gio, int row, int column,
												int Rmask, int Gmask, int Bmask );

static image_type *		image_readBMP32( gio_type *gio, int row, int column );


image_type *
image1_read_bmp_file( char *file )
{
image_type *im,	*im8;

	if( (im = image_read_bmp_file( file )) == NULL )
		return( NULL );

	im8 = image4_to_y( im, NULL );

	image_destroy( im, 1 );

	return( im8 );
}


image_type *
image3_read_bmp_file( char *file )
{
image_type *im,	*im3;

	if( (im = image_read_bmp_file( file )) == NULL )
		return( NULL );

	im3 = image3_from_image4( im, NULL );

	image_destroy( im, 1 );

	return( im3 );
}



image_type *
image_read_bmp_file( char *file )
{
gio_type	*gio;
image_type	*im;

	gio = gio_open_file_to_read( file );
	if( gio == NULL )	return( NULL );

	im = image_read_bmp( gio );

	gio_close( gio );

	return( im );
}



image_type *
image_read_bmp_buffer( char *data, int bytes, int Fdata )
{
gio_type	*gio;
image_type	*im;

	gio = gio_open_buffer_to_read( data, bytes, Fdata );

	im = image_read_bmp( gio );

	gio_close( gio );

	return( im );
}



int
image_read_bmp_is_valid_SINGATURE( char *data, int bytes )
{
	if( strncmp( data, "BM", 2 ) == 0 )
		return( 1 );


	return( -1 );
}


image_type *
image_read_bmp( gio_type *gio )

{
image_type	*im;
int	biSize,	biPlanes,	biCompression;
int     i,	nc, nbits;
int	row,	column,	depth,	size,	offset,	pad;
int	c,	c1;
int	biColorUsed;
int	Rmask,	Gmask,	Bmask;
int	colormap[256];
int	flip;

	nc = nbits = 0;

	nbits = 24;
	nc = 0;

	c = GIO_GETC( gio);
	c1 = GIO_GETC( gio);
  	if( c != 'B' || c1 != 'M' )
		return( NULL );


	size = gio_get_noL( gio, 4 );

	gio_get_noL( gio, 2 ); /* reserved1 */
	gio_get_noL( gio, 2 );	/* reserved2 */

	offset = gio_get_noL( gio, 4 ); /* offset from BOfile to BObitmap */

	biSize = gio_get_noL( gio, 4 ); /* biSize: size of bitmap info header */

	if (biSize == WIN_NEW || biSize == OS2_NEW){
		column = gio_get_noL( gio, 4 ); 
		row = gio_get_noL( gio, 4 ); 

		biPlanes = gio_get_noL( gio, 2 ); /* biPlanes:  must be '1' */
		depth =  gio_get_noL( gio, 2 );	/* biBitCount: 1,4,8, or 24 */

		biCompression = gio_get_noL( gio, 4 ); 
			/* biCompression:  BI_RGB, BI_RLE8 or BI_RLE4 or BI_BITFIELDS*/

		gio_get_noL( gio, 4 );	/* biSizeImage:  size of raw image data */
		gio_get_noL( gio, 4 ); 	/* biXPelsPerMeter: (75dpi * 39" per meter) */
		gio_get_noL( gio, 4 ); 	/* biYPelsPerMeter: (75dpi * 39" per meter) */
		biColorUsed = gio_get_noL( gio, 4 ); /* biClrUsed: # of colors used in cmap */
		nc = gio_get_noL( gio, 4 ); /* biClrImportant: same as above */

		pad = offset - (biSize + 14);
	}
	else	{
		column     = gio_get_noL( gio, 4 ); 
		row        = gio_get_noL( gio, 4 ); 
		biPlanes   =  gio_get_noL( gio, 2 );
		depth      =  gio_get_noL( gio, 2 );

		biCompression = BI_RGB;
		biColorUsed = 0;

		pad = 0;
	}

	flip = 0;
	if( row < 0 ){
		row = -row;
		flip = 1;
	}


	if( biCompression == BI_BITFIELDS ){
		Rmask = gio_get_noL( gio, 4 );
		Gmask = gio_get_noL( gio, 4 );
		Bmask = gio_get_noL( gio, 4 );
	}

	if( depth == 16 ){
		im = image_readBMP16( gio, row, column, Rmask, Gmask, Bmask );
		return( im );
	}

	if( depth == 32 ){
		im = image_readBMP32( gio, row, column );
		return( im );
	}


	/* read COLORMAP if exist */
	if( depth != 24 ){
		int	r,	g,	b;
		if( biColorUsed == 0 )	biColorUsed = ( 1 << depth );

		for( i = 0 ; i < biColorUsed ; i++ ){
			b = GIO_GETC( gio);
			g = GIO_GETC( gio);
			r = GIO_GETC( gio);
			colormap[i] = IMAGE4_RGB( r, g, b );
			if (biSize != WIN_OS2_OLD) {
				GIO_GETC( gio);
				pad -= 4;
      
			}  
		}
	}


	if( biSize != WIN_OS2_OLD )
		for( i = 0 ; i < pad ; i++ )	GIO_GETC( gio);



	im = NULL;

	switch( depth ){
	case 24:
		if( biCompression == BI_RGB ){
//			im = image_create( row, column, 4, 1, 0 );
			im = image_alloc( column, row, 4, IMAGE_TYPE_U8, 1 );
			image4_readBMP24( gio, im );
		}
		else	warning( "image_read_bmp_file",
				"Compression type: not supply yet" );
	    break;
	case 8:
		if( biCompression == BI_RGB ){
//			im = image_create( row, column, 4, 1, 0 );
			im = image_alloc( column, row, 4, IMAGE_TYPE_U8, 1 );
			image4_readBMP8( gio, colormap, im );
		}
		else
			warning( "image_read_bmp_file",
					"Compression 8 bit : not supply yet" );
	    break;

	case 4:
		if( biCompression == BI_RGB ){
//			im = image_create( row, column, 4, 1, 0 );
			im = image_alloc( column, row, 4, IMAGE_TYPE_U8, 1 );
			image4_readBMP4( gio, colormap, im );
		}
		else
			warning( "image_read_bmp_file",
					"Compression 4 bit : not supply yet" );
	    break;

	case 1:
		warning( "image_read_bmp_file", "1 bit : not supply yet" );
	    break;
	}
	

	if( flip == 1 )
		image4_Vflip( im );

	return( im );
}


static image_type * 
image_readBMP32( gio_type *gio, int row, int column )
{
image_type *im;
int     i, j;
u_int	*p;
int	r,	g,	b;

//	im = image_create( row, column, 4, 1, 0 );
	im = image_alloc( column, row, 4, IMAGE_TYPE_U8, 1 );


//	padb = (4 - ((w * 3) % 4)) & 0x03;	/* # of pad bytes to write at
//						 * EOscanline */

	for (i = row - 1; i >= 0; i--) {
		p = IMAGE4_PIXEL( im, i, 0 );

		for (j = 0; j < column; j++) {
			b = GIO_GETC( gio );
			g = GIO_GETC( gio );
			r = GIO_GETC( gio );
			GIO_GETC( gio );

			*p++ = IMAGE4_RGB( r, g, b );
		}

//		for (j = 0; j < padb; j++)	GIO_GETC( gio);
	}

	return( im );
}


static void 
image4_readBMP24_O( gio_type *gio, image_type *im )
{
int     i, j,	padb;
u_int	*p;
int	h,	w;
int	r,	g,	b;

	h = IMAGE_ROW( im );
	w = IMAGE_COLUMN( im );

	padb = (4 - ((w * 3) % 4)) & 0x03;	/* # of pad bytes to write at
						 * EOscanline */

	for (i = h - 1; i >= 0; i--) {
		p = IMAGE4_PIXEL( im, i, 0 );

		for (j = 0; j < w; j++) {
			b = GIO_GETC( gio );
			g = GIO_GETC( gio );
			r = GIO_GETC( gio );

			*p++ = IMAGE4_RGB( r, g, b );
		}

		for (j = 0; j < padb; j++)	GIO_GETC( gio);
	}
}


static void 
image4_readBMP24( gio_type *gio, image_type *im )
{
int     i, j;
u_int	*p;
int	r,	g,	b;
u_char	*data,	*sp;
int	bytes;


	bytes = ((3*im->width+3)/4) * 4;

	data = (u_char *)malloc( bytes+2 );

	for (i = im->height - 1; i >= 0; i--) {
		p = IMAGE4_PIXEL( im, i, 0 );

		gio_get_buffer( gio, data, bytes );

		for (j = 0, sp = data ; j < im->width ; j++) {


			b = *sp++;
			g = *sp++;
			r = *sp++;


			*p++ = IMAGE4_RGB( r, g, b );
		}
	}

	free( data );
}


static image_type * 
image_readBMP16( gio_type *gio, int row, int column, int Rmask, int Gmask, int Bmask )
{
image_type	*im;
int     i, j,	padb;
u_int	*p;

int	r,	g,	b;
int	bs,	rs,	gs;
int	bb,	rb,	gb;
int	val,	val0,	val1;


	if( Bmask != 0 && Rmask != 0 && Gmask != 0 ){
		for( bs = 0 ; ( Bmask &(1<<bs)) == 0 ; bs++ );
		for( bb = 0 ; ( Bmask &(1<<(bs+bb))) ; bb++ );
		for( gs = 0 ; ( Gmask &(1<<gs)) == 0 ; gs++ );
		for( gb = 0 ; ( Gmask &(1<<(gs+gb))) ; gb++ );
		for( rs = 0 ; ( Rmask &(1<<rs)) == 0 ; rs++ );
		for( rb = 0 ; ( Rmask &(1<<(rs+rb))) ; rb++ );
	}
	else	{
		gs = 0;
		Gmask = 31;
		bs = 5 ; Gmask = 31 << bs;
		rs = 5 ; Rmask = 31 << rs;

		bb = gb = rb = 5;
	}


//	im = image_create( row, column, 4, 1, 0 );
	im = image_alloc( column, row, 4, IMAGE_TYPE_U8, 1 );


	padb = (4 - ((column * 2) % 4)) & 0x03;
		


	for (i = row - 1; i >= 0; i--) {
		p = IMAGE4_PIXEL( im, i, 0 );

		for (j = 0; j < column; j++) {
			val0 = GIO_GETC( gio );
			val1 = GIO_GETC( gio );
			val = (val1 << 8) | val0;
			b = ( val & Bmask ) >> bs;
			g = ( val & Gmask ) >> gs;
			r = ( val & Rmask ) >> rs;

			b <<= (8-bb);
			g <<= (8-gb);
			r <<= (8-rb);

			*p++ = IMAGE4_RGB( r, g, b );
		}

		for (j = 0; j < padb; j++)	GIO_GETC( gio);
	}

	return( im );
}





static void 
image4_readBMP8( gio_type *gio, int colormap[], image_type *im )
{
int     i, j,	padb;
u_int	*p;
int	h,	w;
int	c;

	h = IMAGE_ROW( im );
	w = IMAGE_COLUMN( im );

	padb = (4 - ((w) % 4)) & 0x03;	/* # of pad bytes to write at
						 * EOscanline */

	for (i = h - 1; i >= 0; i--) {
		p = IMAGE4_PIXEL( im, i, 0 );

		for (j = 0; j < w; j++) {
			c = GIO_GETC( gio );
			*p++ = colormap[c];
		}

		for (j = 0; j < padb; j++)	GIO_GETC( gio);
	}
}


static void 
image4_readBMP4( gio_type *gio, int colormap[], image_type *im )
{
int     i, j,	pad;
u_int	*p;
int	h,	w,	byte;
int	c,	c1;

	h = IMAGE_ROW( im );
	w = IMAGE_COLUMN( im );

	byte = (w+1)/2;
	pad = (4 - (byte % 4)) & 0x03;	/* # of pad bytes */

	for (i = h - 1; i >= 0; i--) {
		p = IMAGE4_PIXEL( im, i, 0 );

		for (j = 0; j < w ; j += 2 ) {
			c = GIO_GETC( gio );

			c1 = (c >> 4) & 0x0f;
			*p++ = colormap[c1];

			if( j+1 < w ){
				c1 = c & 0x0f;
				*p++ = colormap[c1];
			}
		}

		for (j = 0; j < pad; j++)	GIO_GETC( gio);
	}
}
