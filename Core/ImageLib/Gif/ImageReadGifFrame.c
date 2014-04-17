/********************************
 ***	ImageReadGifFrame.c   ***
 ********************************/
#include "Uigp/igp.h"
#include	"ImageType/ImageType.h"
#include	"ImageReadGif.h"





#define INTERLACEMASK 0x40
#define COLORMAPMASK  0x80

  
typedef struct gifData_type {

	u_char *Raster;			/* The raster data stream, unblocked */
	int	BitOffset;		/* Bit Offset of next code */
	int	CodeSize;			/* Code size, read from GIF header */
	int	ReadMask;			/* Code AND mask for current code size */

	int	XC,	YC;		/* Output X and Y coords of current pixel */
	int	Pass;	/* Used by output routine if interlaced pic */

	u_char *interlace_ptr;
	int   oldYC;

} gifData_type;






static void	doInterlace( gifData_type *gd, image_type *im, int Index );
static int	readCode( gifData_type *gd );


#define	NEXTBYTE( fp )	( (u_char)fgetc(fp) )




image_type *
image_read_gif_imageData( gifIo_type *gifIo, int *top, int *left, palette_type **pal )
{
int	width, height;
register u_char *ptr, *picptr;
int	i, npixels, maxpixels;
int	tmp1,	tmp2,	no;
int Misc,	Interlace;
image_type	*im;

gifData_type gd;

int    InitCodeSize;		/* Starting code size, used during Clear */
int    Code;			/* Value returned by ReadCode */
int    MaxCode;			/* limiting value for current code size */
int    ClearCode;			/* GIF clear code */
int    EOFCode;		/* GIF end-of-information code */
int    CurCode, OldCode, InCode;	/* Decompressor variables */
int    FirstFree;			/* First free code, generated per GIF spec */
int    FreeCode;			/* Decompressor,next free slot in hash table */
int    FinChar;			/* Decompressor variable */
int    BitMask;			/* AND mask for data size */


int OutCode[4097],	OutCount;
int Prefix[4096];
int Suffix[4096];

	npixels = maxpixels = 0;

	gd.BitOffset = gd.XC = gd.YC = gd.Pass = OutCount = 0; 

	gd.interlace_ptr = NULL;
	gd.oldYC = -1;

  /* read in values from the image descriptor */
  
	tmp1 = GIO_GETC( gifIo->gio);
	tmp2 = GIO_GETC( gifIo->gio);
	*left = tmp1 + 0x100 * tmp2;

	tmp1 = GIO_GETC( gifIo->gio);
	tmp2 = GIO_GETC( gifIo->gio);
	*top = tmp1 + 0x100 * tmp2;

	tmp1 = GIO_GETC( gifIo->gio);
	tmp2 = GIO_GETC( gifIo->gio);
	width = tmp1 + 0x100 * tmp2;

	tmp1 = GIO_GETC( gifIo->gio);
	tmp2 = GIO_GETC( gifIo->gio);
	height = tmp1 + 0x100 * tmp2;


	Misc = GIO_GETC( gifIo->gio);

	Interlace = ((Misc & INTERLACEMASK) ? 1 : 0);

	*pal = NULL;
	if (Misc & 0x80) {
		no = 1<<((Misc&7)+1);
//		if( *pal != NULL )	palette_destroy( *pal );
		*pal = palette_alloc( no );
		for( i = 0 ; i < no ; i++ ){
			(*pal)->data[i].Red = GIO_GETC( gifIo->gio);
			(*pal)->data[i].Green = GIO_GETC( gifIo->gio);
			(*pal)->data[i].Blue = GIO_GETC( gifIo->gio);
		}


		palette_destroy( gifIo->palette );
		gifIo->palette = *pal;
	}


	if ( *pal != NULL )
		BitMask = (*pal)->nColor-1;
	else
		BitMask = gifIo->palette->nColor -1;
  
  /* Start reading the raster data. First we get the intial code size
   * and compute decompressor constant values, based on this code size.
   */
  
	gd.CodeSize = GIO_GETC( gifIo->gio);

	ClearCode = (1 << gd.CodeSize);
	EOFCode = ClearCode + 1;
	FreeCode = FirstFree = ClearCode + 2;
  
  /* The GIF spec has it that the code size is the code size used to
   * compute the above values is the code size given in the file, but the
   * code size used in compression/decompression is the code size given in
   * the file plus one. (thus the ++).
   */
  
	gd.CodeSize++;
	InitCodeSize = gd.CodeSize;
	MaxCode = (1 << gd.CodeSize);
	gd.ReadMask = MaxCode - 1;
  


  /* UNBLOCK:
   * Read the raster data.  Here we just transpose it from the GIF array
   * to the Raster array, turning it from a series of blocks into one long
   * data stream, which makes life much easier for readCode().
   */
  
	gd.Raster = malloc( 2*width* height + 100 );

	ptr = gd.Raster;
	while( ( no = GIO_GETC( gifIo->gio) ) != 0 )
		while( no-- )	*ptr++ = GIO_GETC( gifIo->gio);


  

  /* Allocate the 'pic' */

	im = image_create( height, width, 1, 1, NULL );
	picptr = im->data;
	maxpixels = height * width;
	OutCount = 0;
  
  /* Decompress the file, continuing until you see the GIF EOF code.
   * One obvious enhancement is to add checking for corrupt files here.
   */
  
	Code = readCode( &gd );
	while (Code != EOFCode) {



    /* Clear code sets everything back to its initial value, then reads the
     * immediately subsequent code as uncompressed data.
     */
		if (Code == ClearCode) {
			gd.CodeSize = InitCodeSize;
			MaxCode = (1 << gd.CodeSize);
			gd.ReadMask = MaxCode - 1;
			FreeCode = FirstFree;
			Code = readCode( &gd );
			CurCode = OldCode = Code;
			FinChar = CurCode & BitMask;
			if (!Interlace) *picptr++ = FinChar;
				 else doInterlace( &gd, im, FinChar);
			npixels++;

		}
		else {
			/* If not a clear code, must be data: save same as CurCode and InCode */

			/* if we're at maxcode and didn't get a clear, stop loading */
			if (FreeCode>=4096)
				break;

			CurCode = InCode = Code;
      
			/* If greater or equal to FreeCode, not in the hash table yet;
			 * repeat the last character decoded
			 */
      
			if (CurCode >= FreeCode) {
				CurCode = OldCode;
				if (OutCount > 4096)   break;
				OutCode[OutCount++] = FinChar;
			}
      
			/* Unless this code is raw data, pursue the chain pointed to by CurCode
			 * through the hash table to its end; each code in the chain puts its
			 * associated output code on the output queue.
			 */
      
			while (CurCode > BitMask) {
				if (OutCount > 4096) break;
				OutCode[OutCount++] = Suffix[CurCode];
				CurCode = Prefix[CurCode];
			}
      
			if (OutCount > 4096)
				break;
      
			/* The last code in the chain is treated as raw data. */
      
			FinChar = CurCode & BitMask;
			OutCode[OutCount++] = FinChar;
      
			/* Now we put the data out to the Output routine.
			 * It's been stacked LIFO, so deal with it that way...
			 */

			/* safety thing:  prevent exceeding range of 'pic8' */
			if (npixels + OutCount > maxpixels) OutCount = maxpixels-npixels;
	
			npixels += OutCount;
			if (!Interlace) for (i=OutCount-1; i>=0; i--) *picptr++ = OutCode[i];
			else  for (i=OutCount-1; i>=0; i--) doInterlace( &gd, im, OutCode[i]);
			OutCount = 0;

			/* Build the hash table on-the-fly. No table is stored in the file. */
      
			Prefix[FreeCode] = OldCode;
			Suffix[FreeCode] = FinChar;
			OldCode = InCode;
      
			/* Point to the next slot in the table.  If we exceed the current
			 * MaxCode value, increment the code size unless it's already 12.  If it
			 * is, do nothing: the next code decompressed better be CLEAR
			 */
      
			FreeCode++;
			if (FreeCode >= MaxCode) {
				if (gd.CodeSize < 12) {
					gd.CodeSize++;
					MaxCode *= 2;
					gd.ReadMask = (1 << gd.CodeSize) - 1;
				}
			}
		}

		Code = readCode( &gd );
		if (npixels >= maxpixels) break;
	}


	if( gd.Raster != NULL )	free( gd.Raster );
	return( im );
}



/* Fetch the next code from the raster data stream.  The codes can be
 * any length from 3 to 12 bits, packed into 8-bit bytes, so we have to
 * maintain our location in the Raster array as a BIT Offset.  We compute
 * the byte Offset into the raster array by dividing this by 8, pick up
 * three bytes, compute the bit Offset into our 24-bit chunk, shift to
 * bring the desired code to the bottom, then mask it off and return it. 
 */

static int
readCode( gifData_type *gd )
{
  int RawCode, ByteOffset;
  
  ByteOffset = gd->BitOffset / 8;
  RawCode = gd->Raster[ByteOffset] + (gd->Raster[ByteOffset + 1] << 8);
  if ( gd->CodeSize >= 8)
    RawCode += ( ((int) gd->Raster[ByteOffset + 2]) << 16);
  RawCode >>= ( gd->BitOffset % 8);
  gd->BitOffset += gd->CodeSize;

  return(RawCode & gd->ReadMask);
}


/***************************/

static void
doInterlace( gifData_type *gd, image_type *im, int Index )
{
	
  if( gd->oldYC != gd->YC ){
	  gd->interlace_ptr = im->data + gd->YC * im->column;
	  gd->oldYC = gd->YC;
  }
  

  if( gd->YC < im->row )
    *gd->interlace_ptr++ = Index;
  
  /* Update the X-coordinate, and if it overflows, update the Y-coordinate */
  
  if (++gd->XC == im->column) {
    
    /* deal with the interlace as described in the GIF
     * spec.  Put the decoded scan line out to the screen if we haven't gone
     * past the bottom of it
     */
    
    gd->XC = 0;
    
    switch (gd->Pass) {
    case 0:
      gd->YC += 8;
      if (gd->YC >= im->row) { gd->Pass++; gd->YC = 4; }
      break;
      
    case 1:
      gd->YC += 8;
      if (gd->YC >= im->row) { gd->Pass++; gd->YC = 2; }
      break;
      
    case 2:
      gd->YC += 4;
      if (gd->YC >= im->row) { gd->Pass++; gd->YC = 1; }
      break;
      
    case 3:
      gd->YC += 2;  break;
      
    default:
      break;
    }
  }
}



void
image_read_gif_imageData_skip( gifIo_type *gifIo )
{
int	i;
int	no;
int Misc;

	for ( i = 0 ; i < 8 ; i++ )
		GIO_GETC( gifIo->gio);

	Misc = GIO_GETC( gifIo->gio);

	if (Misc & 0x80) {
		no = 1<<((Misc&7)+1);
		for( i = 0 ; i < no ; i++ ){
			GIO_GETC( gifIo->gio);
			GIO_GETC( gifIo->gio);
			GIO_GETC( gifIo->gio);
		}
	}

	GIO_GETC( gifIo->gio);
 
	while( ( no = GIO_GETC( gifIo->gio) ) != 0 )
		while( no-- )	GIO_GETC( gifIo->gio);


}