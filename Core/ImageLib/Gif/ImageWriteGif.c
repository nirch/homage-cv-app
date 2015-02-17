/****************************
 ***	ImageWriteGif.c   ***
 ****************************/


#include "ImageWriteGif.h"
#include "Ucst/CstType.h"
#include "Ubst/BstType.h"
#include "lzw.h"
#include	"Bmp/ImageBmp.h"

static void	image_write_gif_header( FILE *fp, int row, int col,
							palette_type *p, int BackgroundColor );

static int	image_write_gif_ImageDescriptor( FILE *fp,
								int left, int width, int top,
								int height, palette_type *lp );

static void		image_write_gif_application( FILE *fp );

static void		image_write_gif_force_87a( FILE *fp );


static int log2m1(int n);


int image_write_gif( image_type *im, palette_type *palette, char *dir, char *name )
{
char	file[256];
gifIo_type	*gifIo;
int	ret;

	sprintf( file, "%s/%s.gif", dir, name );


	gifIo = image_write_gifIo_open_file( file, IMAGE_ROW(im), IMAGE_COLUMN(im),
							10, palette, -1, 0, 0 );


	if( gifIo == NULL )	return( -1 );


	ret = image_write_gifIo_add_frame( gifIo, im, 0 );

	image_write_gifIo_close( gifIo );
	
	return ret;
}



gifIo_type *
image_write_gifIo_open_file( char *file, int height, int width,
							int frameDelay,
							palette_type *palette, int transparent_index, int Fdither,
							int sizeLimit )
{
gifIo_type	*gifIo;


	gifIo = (gifIo_type *)malloc( sizeof(gifIo_type) );

	gifIo->column = width;
	gifIo->row = height;

	gifIo->palette = palette;
	gifIo->Fdither = Fdither;

	gifIo->delay_time = frameDelay;
	if( gifIo->delay_time <= 0 )	gifIo->delay_time = 30;
	gifIo->ftell_delayTime = (int *)malloc( 5024*sizeof(int) );

	gifIo->dev = 0;

	gifIo->sizeLimit = sizeLimit;

	gifIo->frameTime = 0;


	gifIo->im = NULL;
	gifIo->im8 = NULL;


	gifIo->transparent_flag = 0;
	gifIo->disposal_method = 0;
	gifIo->transparent_index = transparent_index;

	if( gifIo->transparent_index >= 0 ){
		gifIo->transparent_flag = 1;
		gifIo->disposal_method = 1;
	}



	if( (gifIo->fp = fopen( file, "wb")) == NULL ){
		free( gifIo );
		return NULL;
	}


	image_write_gif_header( gifIo->fp, height, width, palette, 0 );
	gifIo->end_header_location = ftell( gifIo->fp );


	image_write_gif_application( gifIo->fp );


	gifIo->frame_i = 0;
	gifIo->frame_no = 0;
	
	return gifIo;
}




/*********  image_write_gifIo_add_frame  **********
* @Description	:	adds a frame to animated gif structure. 
* 
* @Param   gifIo_type *gifIo :  animated gif structure
* @Param  image_type *rim :  frame to add
* @Param  int Fimage :  flag : 1 - destroy rim when finished with it, 0 - don't destroy.
* @Return  int  :	1	: same frame as the previous one	
					-1	: Size overflow, frame was not written
					anything > 0 is number of bytes written
*****************************************/
int image_write_gifIo_add_frame( gifIo_type *gifIo, image_type *rim, int Fimage )
{
box2i Dif;
int height,	width,	left,	top;
image_type	*im8;
bst_type * bst;
cst_type	cst;
int bytes,	size;
u_char codeLength;
float var,	dev,	average;
//int	a;



	Dif.x0 = Dif.y0 = 0;
	Dif.x1 = rim->column - 1;
	Dif.y1 = rim->row - 1;


	if( gifIo->im != NULL ){
		image_difference_rectangle( gifIo->im, rim,
							&Dif.y0, &Dif.y1, &Dif.x0, &Dif.x1 );

		//a = Dif.x1 - Dif.x0;
		//if( (a & 0x01) != 0  ){
		//	if( Dif.x1 < rim->width-1 )
		//		Dif.x1 += 1;
		//	else
		//		Dif.x0 -= 1;
		//}


		image4_comparisonG( gifIo->im, rim, &var, &dev, &average );
		gifIo->dev += dev;


	
		if( Dif.y0 < 0 || Dif.x1 < 0 )	// the same image
			return( 1 );
	}

/*
	{
		static int no = 0;
		char	name[256];
		sprintf( name, "cc%d", no++ );
		image_write_bmp( rim, "d:/aa/yy", name );
	}
*/


	if( gifIo->Fdither )
			im8 = image_24to8_dithering_floyd(rim, gifIo->palette, &Dif);
	else	im8 = image_24to8(rim, gifIo->palette, &Dif);

//#define _DUMP
#ifdef _DUMP
	{
		static int no = 0;
		image_type *im1;

		im1 = image4_from_y( im8 );

		image_writeFN_bmp( im1, "d:/_out/gif/im8", no++ );
		image_destroy( im1, 1 );
	}
#endif

	//if( gifIo->im8 != NULL && gifIo->transparent_index >= 0 )
	//	image_transparent_set( gifIo->im8, &Dif, im8, gifIo->transparent_index );



//	codeLength = 8;
//	if( gifIo->palette->type == PALETTE_BW) 
//		codeLength = 2;

	codeLength = ( gifIo->palette->nColor == 2 )? 2 : 8;

	cst.data = im8->data;
	cst.BYTE_NO = IMAGE_ROW(im8)* IMAGE_COLUMN(im8);
//	cst.byte_no = cst.BYTE_NO
	cst.wp = cst.data + cst.BYTE_NO;
	cst_rewind( &cst );


	bst = lzw_encoding( &cst, codeLength);


	bytes = BST_BYTES( bst ) + 12 + 8;
	

	size = ftell( gifIo->fp ) + bytes;

	if( gifIo->sizeLimit > 0 && size > gifIo->sizeLimit && gifIo->im != NULL){
		image_destroy(im8, 1 );
		bst_destroy(bst);

// yoram 12-11-05
		if( Fimage == 1 )
			image_destroy( rim, 1 );
// end
		return( -1 );
	}


	if( gifIo->transparent_index >= 0 ){
		if( gifIo->im8 == NULL ){
			gifIo->im8 = im8;
			im8 = NULL;
		}
		else	
			image_transparent_copy_in( gifIo->im8, &Dif, im8, gifIo->transparent_index );
	}

	if( im8 != NULL )
		image_destroy(im8, 1 );


//#ifdef _AA_
	if( Fimage == 1 ){
		if( gifIo->im != NULL )
			image_destroy( gifIo->im, 1 );
		gifIo->im = rim;

//		gifIo->im = image_make_copy( rim, gifIo->im );
	}
	else	{
		if( gifIo->im == NULL )
			gifIo->im = image_create( rim->row, rim->column, 4, 4, NULL );

		memcpy(gifIo->im->data, rim->data, rim->row * rim->column * 4);
	}
//#endif	
//	gifIo->im = image_make_copy( rim, gifIo->im );



	gifIo->ftell_delayTime[ gifIo->frame_no] = ftell( gifIo->fp );

	image_write_gif_GraphicControl( gifIo, gifIo->delay_time,
											gifIo->transparent_index );



	height = Dif.y1 - Dif.y0 + 1;
	width = Dif.x1 - Dif.x0 + 1;
	top = Dif.y0;
	left = Dif.x0;
	image_write_gif_ImageDescriptor( gifIo->fp, left, width, top, height, NULL );


	fputc( codeLength, gifIo->fp);
	bst_write( gifIo->fp, bst);
	fputc( 0, gifIo->fp );



	bst_destroy(bst);


	if( gifIo->frame_no > 0 )
		gifIo->frameTime += gifIo->delay_time * 10;

	gifIo->frame_no++;

	return bytes;
}















// returns number of bytes in the gif
int image_write_gifIo_close( gifIo_type *gifIo )
{
int bytes;


	fputc(0x3b, gifIo->fp);

	bytes = ftell( gifIo->fp);

	if( gifIo->frame_no == 1 )
		image_write_gif_force_87a( gifIo->fp );


	fclose( gifIo->fp);

	if( gifIo->im != NULL )
		image_destroy( gifIo->im, 1 );

	if( gifIo->im8 != NULL )
		image_destroy( gifIo->im8, 1 );


	if( gifIo->ftell_delayTime != NULL )
		free( gifIo->ftell_delayTime );

	free( gifIo );

	return bytes;
}


// return code :number of bytes if ok 
//				-1 - could not open file for writing
//				-2 - error while writing	
/*			
int image_write_gif( image_type *im, palette_type *palette, char *dir, char *name )
{
char	file[256];
FILE *fp;

	sprintf( file, "%s/%s.gif", dir, name );

	fp = image_write_gif_open(file, IMAGE_ROW(im), IMAGE_COLUMN(im), palette );

	if( fp == NULL )	return( -1 );



	if (image_write_gif_add_frame(fp, NULL, im, palette, 0, 0, 0 ) > 0 )
		return image_write_gif_close( fp );
	
	return -2;
}
*/

static int log2m1(int n)
{
int i;
	i = 0;
	while ((2 << i) < n)
		i++;
	return i;
}


FILE * 
image_write_gif_open( char *file, int height, int width,
						palette_type * palette )
{
FILE	*fp;


	if( (fp = fopen( file, "wb")) == NULL )
		return NULL;


	image_write_gif_header( fp, height, width, palette, 0 );

	image_write_gif_application( fp );
	
	return fp;
}


static void
image_write_gif_header( FILE *fp, int row, int col, palette_type *p, int BackgroundColor )
{
u_char buf[13];
int	no;

	memcpy( buf, "GIF", 3);		/* Signature */
	memcpy( buf + 3, "89a", 3);	/* Version */

	buf[6] = (u_char)col;
	buf[7] = (u_char)(col >> 8);
	buf[8] = (u_char)row;
	buf[9] = (u_char)(row >> 8);

	buf[10] = 0;
	if( p != 0 ){
		no = log2m1(p->nColor);
		buf[10] = 0xf0 | (u_char)no;
	}

	buf[11] = BackgroundColor;
	buf[12] = 0;				/* AspectRatio */

	fwrite( buf, 1, 13, fp);


	
	if( p != NULL ){
		no = 1<<(no+1);
		palette_write( p, no, fp );
	}
}


static  void 
image_write_gif_application( FILE *fp )
{
u_char buf[19];

	buf[0] = 0x21;
	buf[1] = 0xff;
	buf[2] = 0x0b;
	memcpy(buf + 3, "NETSCAPE", 8);
	memcpy(buf + 11, "2.0", 3);
	buf[14] = 3;
	buf[15] = 1;
	buf[16] = 0;
	buf[17] = 0;
	buf[18] = 0;
	fwrite(buf, 1, 19, fp);
}

 
void 
image_write_gif_VimatixData( FILE *fp, const u_char *pVimatixData )
{
u_char buf[300];
u_char len;
const u_char *p;

	buf[0] = 0x21;
	buf[1] = 0xfe;

	for ( len = 0, p = pVimatixData; *p && len < 255; len++ )
		buf[3+len] = *p++;

	buf[2] = len;

	buf[3+len] = 0;

	fwrite(buf, 1, 4+len, fp);
}

 



int 
image_write_gif_GraphicControl( gifIo_type *gifIo, int DelayTime, int transparentIndex )
{
u_char buf[8];
int	mask;


	buf[0] = 0x21;
	buf[1] = 0xf9;
	buf[2] = 4;  // BlockSize 


	mask = ( gifIo->frame_no > 0 && transparentIndex >= 0 )? 0x05: 0x4;
	if( transparentIndex > 0 )
		mask = 0x09;

	buf[3] = mask;


	buf[4] = (u_char)DelayTime;
	buf[5] = (u_char)(DelayTime >> 8);

	buf[6] = 0; // Transpart Color Index
	if( transparentIndex >= 0 )
		buf[6] = transparentIndex;

	buf[7] = 0;
	fwrite(buf, 1, 8, gifIo->fp);
	return( 8 );
}
/*
int 
image_write_gif_GraphicControl( FILE *fp, int DelayTime, int transparentIndex )
{
u_char buf[8];
int	mask;


	buf[0] = 0x21;
	buf[1] = 0xf9;
	buf[2] = 4;  // BlockSize 

	mask = ( transparentIndex >= 0 )? 0x05: 0x4;
	buf[3] = mask;

	buf[4] = (u_char)DelayTime;
	buf[5] = (u_char)(DelayTime >> 8);

	buf[6] = 0; // Transpart Color Index
	if( transparentIndex >= 0 )
		buf[6] = transparentIndex;

	buf[7] = 0;
	fwrite(buf, 1, 8, fp);
	return( 8 );
}
*/



/*
int 
image_write_gif_add_frame( FILE *fp, image_type * im,
					image_type * rim, palette_type * palette,
					int sizeLimit, int delay, int Fdither  )
{
box2i Dif;
int height,	width,	left,	top;
image_type	*im8;
bst_type * bst;
cst_type	cst;
int bytes,	size;
u_char codeLength;

static int no = 0;


	Dif.x0 = Dif.y0 = 0;
	Dif.x1 = rim->column - 1;
	Dif.y1 = rim->row - 1;


	if( im != NULL )
		image_difference_rectangle( im, rim,
							&Dif.y0, &Dif.y1, &Dif.x0, &Dif.x1 );



//	if( Fdither )
//			im8 = image_24to8_dithering_floyd(rim, palette, &Dif);
//	else	im8 = image_24to8(rim, palette, &Dif);




	codeLength = 8;

	if( Fdither )
			im8 = image_24to8_dithering_floyd(rim, palette, &Dif);
	else	
	{
		im8 = image_24to8(rim, palette, &Dif);
	}
	if (palette->type == PALETTE_BW) 
		codeLength = 2;


	cst.data = im8->data;
	cst.byte_no = cst.BYTE_NO = IMAGE_ROW(im8)* IMAGE_COLUMN(im8);
	cst_rewind( &cst );


	bst = lzw_encoding( &cst, codeLength);
	image_destroy(im8, 1 );

	bytes = BST_BYTES( bst ) + 12 + 8;
	

	size = ftell( fp ) + bytes;

	if( sizeLimit > 0 && size > sizeLimit && im != NULL){
		bst_destroy(bst);
		return( -1 );
	}



	image_write_gif_GraphicControl( fp, delay, -1 );



	height = Dif.y1 - Dif.y0 + 1;
	width = Dif.x1 - Dif.x0 + 1;
	top = Dif.y0;
	left = Dif.x0;
	image_write_gif_ImageDescriptor( fp, left, width, top, height, NULL );


	fputc(codeLength, fp);
	bst_write(fp, bst);
	fputc(0, fp);


	bst_destroy(bst);

	return bytes;
}
*/


static int 
image_write_gif_ImageDescriptor( FILE *fp, int left, int width,
								int top, int height, palette_type *p )
{
u_char buf[10];

	buf[0] = 0x2c;
	buf[1] = (unsigned char)left;
	buf[2] = (unsigned char)(left >> 8);
	buf[3] = (unsigned char)top;
	buf[4] = (unsigned char)(top >> 8);
	buf[5] = (unsigned char)width;
	buf[6] = (unsigned char)(width >> 8);
	buf[7] = (unsigned char)height;
	buf[8] = (unsigned char)(height >> 8);

	buf[9] = ( p == NULL)? 0 : 0xf0 | (u_char)log2m1(p->nColor);

	fwrite(buf, 1, 10, fp);

	return( 10 );
}







int
image_write_gif_close(FILE * fp)
{
int bytes;
	bytes = ftell( fp );
	fputc(0x3b, fp);

	bytes = ftell(fp);

	image_write_gif_force_87a( fp );

	fclose(fp);

	return bytes;
}


static void
image_write_gif_force_87a( FILE *fp )
{
int	pos;

	pos = ftell( fp );
	fseek( fp, 4, SEEK_SET );
	fputc( '7', fp );

	fseek( fp, pos, SEEK_SET );
}

void
image_write_gif_set_frameDelay( gifIo_type *gifIo, int delayTime )
{
int	i,	pos;
int	tmp;

	gifIo->delay_time = delayTime;

	if( gifIo->frame_no == 0 )	return;

	pos = ftell( gifIo->fp );

	for( i = 0 ; i < gifIo->frame_no ; i++ ){
		fseek( gifIo->fp, gifIo->ftell_delayTime[i] + 4, SEEK_SET );

		tmp = fputc( (delayTime & 0xff), gifIo->fp );
		tmp = fputc( ((delayTime>>8) & 0xff), gifIo->fp );
	}

	fseek( gifIo->fp, pos, SEEK_SET );
}



int
image_write_gif_set_frameDelayA( gifIo_type *gifIo, int delayTime[] )
{
int	i,	pos;
int	tmp;
int	msec;

	msec = 0;
	gifIo->delay_time = delayTime[0];

	if( gifIo->frame_no == 0 )	return(0);

	pos = ftell( gifIo->fp );

	for( i = 0 ; i < gifIo->frame_no ; i++ ){
		fseek( gifIo->fp, gifIo->ftell_delayTime[i] + 4, SEEK_SET );

		tmp = fputc( (delayTime[i] & 0xff), gifIo->fp );
		tmp = fputc( ((delayTime[i]>>8) & 0xff), gifIo->fp );

		msec += delayTime[i];
	}

	fseek( gifIo->fp, pos, SEEK_SET );

	return( 10 * msec );
}



void
image_write_gif_warning_dev( gifIo_type *gifIo )
{
float	dev;
char	buf[100];

	dev = gifIo->dev / (gifIo->frame_no -1 );

	sprintf( buf, "Var: %.2f", dev );
	warning( "Gif Delay ", buf );
}

/*
void
image_write_gif_set_frameDelay( gifIo_type *gifIo, int delayTime )
{
int	no,	Misc,	tell,	frame,	type,	extension;
char	buf[2];

	gifIo->delay_time = delayTime;

	if( gifIo->frame_no == 0 )	return;

	tell = ftell( gifIo->fp );

	fseek( gifIo->fp, gifIo->end_header_location, SEEK_SET );


	frame = 0;

	while( frame < gifIo->frame_no ){
		type = fgetc( gifIo->fp);

		switch( type ){
		case  0x21:
			extension = fgetc( gifIo->fp);

			if( extension != 0xf9 ){
				no = fgetc( gifIo->fp );
				fseek( gifIo->fp, no+1, SEEK_CUR );
				continue;
			}

			
			fgetc( gifIo->fp );//	2
			fgetc( gifIo->fp );//	3


//			no = fputc( (delayTime & 0xff), gifIo->fp );
//			no = fputc( ((delayTime>>8) & 0xff), gifIo->fp );

			buf[0] = delayTime & 0xff;
			buf[1] = delayTime & 0xff;
			fwrite(buf, 1, 2, gifIo->fp);

			fgetc( gifIo->fp );//	6

			fgetc( gifIo->fp );//	7

			break;

		case 0x2c:
				fseek( gifIo->fp, 8, SEEK_CUR );

				Misc = fgetc( gifIo->fp);

				if (Misc & 0x80) {
					no = 1<<((Misc&7)+1);
					fseek( gifIo->fp, no*3, SEEK_CUR );
				}
				fgetc( gifIo->fp);

 
				while( ( no = (fgetc( gifIo->fp)) ) != 0 )
					fseek( gifIo->fp, no, SEEK_CUR );

				frame++;
			break;
		}
	}

	no = ftell( gifIo->fp );
	fseek( gifIo->fp, tell, SEEK_SET );

}
*/
