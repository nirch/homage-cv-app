/****************************
 ***	ImageWriteMng.c   ***
 ****************************/
#include <malloc.h>

#include "MngIo.h"
#include "ImageWriteMng.h"
#include "Ucst/CstType.h"
#include "Ubst/BstType.h"



#include "PngLib/png.h"
#include "Png/ImageWritePng.h"


extern PNG_EXPORT(void,mng_png_write_info) PNGARG((png_structp png_ptr,
   png_infop info_ptr));


#define png_jmpbuf(png_ptr) ((png_ptr)->jmpbuf)

int	
image_write_mng_add_frame__png( image_type *image, FILE *fp )
{
png_structp png_ptr;
png_infop info_ptr;
int color_type ;
png_bytep *row_pointers;
int k, j, col, bytes;
u_int * iData;

u_char *tp;

   /* Create and initialize the png_struct with the desired error handler
    * functions.  If you want to use the default stderr and longjump method,
    * you can supply NULL for the last three parameters.  We also check that
    * the library version is compatible with the one used at compile time,
    * in case we are using dynamically linked libraries.  REQUIRED.
    */
   png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

   if (png_ptr == NULL)
   {
      fclose(fp);
      return -3;
   }

   /* Allocate/initialize the image information data.  REQUIRED */
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL)
   {
      fclose(fp);
      png_destroy_write_struct(&png_ptr,  (png_infopp)NULL);
      return -3;
   }

   /* Set error handling.  REQUIRED if you aren't supplying your own
    * error handling functions in the png_create_write_struct() call.
    */
   if (setjmp(png_jmpbuf(png_ptr)))
   {
      /* If we get here, we had a problem reading the file */
      fclose(fp);
      png_destroy_write_struct(&png_ptr, &info_ptr);
      return -2;
   }

   /* set up the output control if you are using standard C streams */
   png_init_io(png_ptr, fp);


   /* Set the image information here.  Width and height are up to 2^31,
    * bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
    * the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
    * PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
    * or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
    * PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
    * currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
    */
   color_type = PNG_COLOR_TYPE_RGB;
   if ( image->palette != NULL )
	   color_type = PNG_COLOR_TYPE_PALETTE;   

   png_set_IHDR(png_ptr, info_ptr, image->column, image->row, 8, color_type,
      PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

   /* set the palette if there is one.  REQUIRED for indexed-color images */
   //palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof (png_color));
   /* ... set palette colors ... */
   if ( image->palette != NULL )
	png_set_PLTE(png_ptr, info_ptr, (struct png_color_struct *)image->palette->data, image->palette->nColor);
   
   /* You must not free palette here, because png_set_PLTE only makes a link to
      the palette that you malloced.  Wait until you are about to destroy
      the png structure. */

   /* optional significant bit chunk */
   /* if we are dealing with a color image then */
   //sig_bit.red = 8;//true_red_bit_depth;
   //sig_bit.green = 8;//true_green_bit_depth;
   //sig_bit.blue = 8;//true_blue_bit_depth;
   /* if the image has an alpha channel then */   
   //sig_bit.alpha = 8;//true_alpha_bit_depth;
   //png_set_sBIT(png_ptr, info_ptr, sig_bit);


   
   /* Optionally write comments into the image 
   text_ptr[0].key = "Title";
   text_ptr[0].text = "Vim Image";
   text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
   text_ptr[1].key = "Author";
   text_ptr[1].text = "Eyal Peleg";
   text_ptr[1].compression = PNG_TEXT_COMPRESSION_NONE;
   text_ptr[2].key = "Description";
   text_ptr[2].text = "Eyal Peleg Is the dog";
   text_ptr[2].compression = PNG_TEXT_COMPRESSION_zTXt;
#ifdef PNG_iTXt_SUPPORTED
   text_ptr[0].lang = NULL;
   text_ptr[1].lang = NULL;
   text_ptr[2].lang = NULL;
#endif
   png_set_text(png_ptr, info_ptr, text_ptr, 3);
*/
   /* other optional chunks like cHRM, bKGD, tRNS, tIME, oFFs, pHYs, */
   /* note that if sRGB is present the gAMA and cHRM chunks must be ignored
    * on read and must be written in accordance with the sRGB profile */


   /* Write the file header information.  REQUIRED */
   // *****************  MNG  *******************
   mng_png_write_info(png_ptr, info_ptr);
   // png_write_info(png_ptr, info_ptr);

   /* If you want, you can write the info in two steps, in case you need to
    * write your private chunk ahead of PLTE:
    *
    *   png_write_info_before_PLTE(write_ptr, write_info_ptr);
    *   write_my_chunk();
    *   png_write_info(png_ptr, info_ptr);
    *
    * However, given the level of known- and unknown-chunk support in 1.1.0
    * and up, this should no longer be necessary.
    */

   /* Once we write out the header, the compression type on the text
    * chunks gets changed to PNG_TEXT_COMPRESSION_NONE_WR or
    * PNG_TEXT_COMPRESSION_zTXt_WR, so it doesn't get written out again
    * at the end.
    */

   /* set up the transformations you want.  Note that these are
    * all optional.  Only call them if you want them.
    */

 
   /* Shift the pixels up to a legal bit depth and fill in
    * as appropriate to correctly scale the image.
    */
   //png_set_shift(png_ptr, &sig_bit);

   /* flip BGR pixels to RGB */
   //png_set_bgr(png_ptr);
  
   /* Get rid of filler (OR ALPHA) bytes, pack XRGB/RGBX/ARGB/RGBA into
    * RGB (4 channels -> 3 channels). The second parameter is not used.
    */

//#ifdef WIN32
//   png_set_filler(png_ptr, 0, PNG_FILLER_AFTER);
//#else
//  png_set_filler(png_ptr, 0, PNG_FILLER_BEFORE);
//#endif

   

   /* The easiest way to write the image (you may have a different memory
    * layout, however, so choose what fits your needs best).  You need to
    * use the first method if you aren't handling interlacing yourself.
    */
   iData = (u_int*)image->data;
   col = image->column;	   
   row_pointers = (u_char**)malloc(image->row * sizeof (u_char *));
   for (k = 0; k < image->row ; k++)
   {
	   
	   row_pointers[k] = (u_char*)malloc(col * sizeof (u_int ));
	   tp = row_pointers[k];


	   for ( j = 0 ; j < col ; j ++ )
	   {
			
		   //*pt++ = (int)IMAGE2PNG_RGB(*iData);
		   *tp++ = IMAGE4_RED(*iData);
		   *tp++ = IMAGE4_GREEN(*iData);
		   *tp++ = IMAGE4_BLUE(*iData);
		   iData++;
	   }
	   
   }

   /* the following output method is REQUIRED */
   png_write_image(png_ptr, row_pointers);   


   /* You can write optional chunks like tEXt, zTXt, and tIME at the end
    * as well.  Shouldn't be necessary in 1.1.0 and up as all the public
    * chunks are supported and you can use png_set_unknown_chunks() to
    * register unknown chunks into the info structure to be written out.
    */

   /* It is REQUIRED to call this to finish writing the rest of the file */
   png_write_end(png_ptr, info_ptr);
   
   /* if you png_malloced any data that you passed in with
      png_set_something(), such as a hist or trans array, free it here,
      when you can be sure that libpng is through with it. */
//   png_free(png_ptr, trans);
 //  trans=NULL;
   for ( k = 0 ; k < image->row ; k++)
   {
	   free(row_pointers[k]);
   }
   free(row_pointers);

   /* clean up after the write, and free any memory allocated */
   png_destroy_write_struct(&png_ptr, &info_ptr);

   /* close the file */
   bytes = ftell(fp);

   /* that's it */
   return bytes;
}


int	
image_write_mng_add_frame__png_T( image_type *image, 
							  palette_type * palette, int fGlobalPalette,
							  int tranparteId, FILE *fp )
{
png_structp png_ptr;
png_infop info_ptr;
int color_type ;
png_bytep *row_pointers;
int k, j, col, bytes;
u_char * iData;

u_char *tp;

	/* Create and initialize the png_struct with the desired error handler
	* functions.  If you want to use the default stderr and longjump method,
	* you can supply NULL for the last three parameters.  We also check that
	* the library version is compatible with the one used at compile time,
	* in case we are using dynamically linked libraries.  REQUIRED.
	*/
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (png_ptr == NULL)
	{
	  fclose(fp);
	  return -3;
	}

	/* Allocate/initialize the image information data.  REQUIRED */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
	  fclose(fp);
	  png_destroy_write_struct(&png_ptr,  (png_infopp)NULL);
	  return -3;
	}

	/* Set error handling.  REQUIRED if you aren't supplying your own
	* error handling functions in the png_create_write_struct() call.
	*/
	if (setjmp(png_jmpbuf(png_ptr)))
	{
	  /* If we get here, we had a problem reading the file */
	  fclose(fp);
	  png_destroy_write_struct(&png_ptr, &info_ptr);
	  return -2;
	}

	/* set up the output control if you are using standard C streams */
	png_init_io(png_ptr, fp);


	/* Set the image information here.  Width and height are up to 2^31,
	* bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
	* the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
	* PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
	* or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
	* PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
	* currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
	*/
	color_type = PNG_COLOR_TYPE_RGB;
	if ( palette != NULL )
	   color_type = PNG_COLOR_TYPE_PALETTE;   

	png_set_IHDR(png_ptr, info_ptr, image->column, image->row, 8, color_type,
	  PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	/* set the palette if there is one.  REQUIRED for indexed-color images */
	//palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof (png_color));
	/* ... set palette colors ... */
	if( palette != NULL )
	png_set_PLTE(png_ptr, info_ptr, (struct png_color_struct *)palette->data, palette->nColor);

	/* You must not free palette here, because png_set_PLTE only makes a link to
	  the palette that you malloced.  Wait until you are about to destroy
	  the png structure. */

	/* optional significant bit chunk */
	/* if we are dealing with a color image then */
	//sig_bit.red = 8;//true_red_bit_depth;
	//sig_bit.green = 8;//true_green_bit_depth;
	//sig_bit.blue = 8;//true_blue_bit_depth;
	/* if the image has an alpha channel then */   
	//sig_bit.alpha = 8;//true_alpha_bit_depth;
	//png_set_sBIT(png_ptr, info_ptr, sig_bit);



	/* Optionally write comments into the image 
	text_ptr[0].key = "Title";
	text_ptr[0].text = "Vim Image";
	text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
	text_ptr[1].key = "Author";
	text_ptr[1].text = "Eyal Peleg";
	text_ptr[1].compression = PNG_TEXT_COMPRESSION_NONE;
	text_ptr[2].key = "Description";
	text_ptr[2].text = "Eyal Peleg Is the dog";
	text_ptr[2].compression = PNG_TEXT_COMPRESSION_zTXt;
	#ifdef PNG_iTXt_SUPPORTED
	text_ptr[0].lang = NULL;
	text_ptr[1].lang = NULL;
	text_ptr[2].lang = NULL;
	#endif
	png_set_text(png_ptr, info_ptr, text_ptr, 3);
	*/


	{
		png_color_16	trans_values;
   		trans_values.index = 0;
		trans_values.blue = 0;
		trans_values.green = 0;
		trans_values.red = 0;


		png_set_bKGD(png_ptr, info_ptr, &trans_values );
	}

	/* other optional chunks like cHRM, bKGD, tRNS, tIME, oFFs, pHYs, */
	/* note that if sRGB is present the gAMA and cHRM chunks must be ignored
	* on read and must be written in accordance with the sRGB profile */

	/*
	if( tranparteId != -1 ){
		png_color_16	trans_values;
		png_byte	trans;

		trans_values.index = tranparteId;
		trans_values.blue = 0;
		trans_values.green = 0;
		trans_values.red = 0;
		trans = tranparteId;


		png_set_tRNS(png_ptr, info_ptr, &trans, 1, &trans_values );
   
	}
	*/
	if( tranparteId != -1 ){
		png_color_16	trans_values;
		png_byte	trans[256];
		int	i;
		for(  i = 0 ; i < 256 ; i++ )
			trans[i] = 255;
		trans[tranparteId] = 0;

		trans_values.index = tranparteId;
		trans_values.blue = 0;
		trans_values.green = 0;
		trans_values.red = 0;
	//		trans = tranparteId;


		png_set_tRNS(png_ptr, info_ptr, trans, 256, &info_ptr->trans_values );
   
	}





	/* Write the file header information.  REQUIRED */
	if ( fGlobalPalette ) {
	   png_ptr->mng_features_permitted |= PNG_FLAG_MNG_EMPTY_PLTE;
	   info_ptr->num_palette = 0;
	}
	mng_png_write_info(png_ptr, info_ptr);

	/* If you want, you can write the info in two steps, in case you need to
	* write your private chunk ahead of PLTE:
	*
	*   png_write_info_before_PLTE(write_ptr, write_info_ptr);
	*   write_my_chunk();
	*   png_write_info(png_ptr, info_ptr);
	*
	* However, given the level of known- and unknown-chunk support in 1.1.0
	* and up, this should no longer be necessary.
	*/

	/* Once we write out the header, the compression type on the text
	* chunks gets changed to PNG_TEXT_COMPRESSION_NONE_WR or
	* PNG_TEXT_COMPRESSION_zTXt_WR, so it doesn't get written out again
	* at the end.
	*/

	/* set up the transformations you want.  Note that these are
	* all optional.  Only call them if you want them.
	*/


	/* Shift the pixels up to a legal bit depth and fill in
	* as appropriate to correctly scale the image.
	*/
	//png_set_shift(png_ptr, &sig_bit);

	/* flip BGR pixels to RGB */
	//png_set_bgr(png_ptr);

	/* Get rid of filler (OR ALPHA) bytes, pack XRGB/RGBX/ARGB/RGBA into
	* RGB (4 channels -> 3 channels). The second parameter is not used.
	*/

	//#ifdef WIN32
	//   png_set_filler(png_ptr, 0, PNG_FILLER_AFTER);
	//#else
	//  png_set_filler(png_ptr, 0, PNG_FILLER_BEFORE);
	//#endif



	/* The easiest way to write the image (you may have a different memory
	* layout, however, so choose what fits your needs best).  You need to
	* use the first method if you aren't handling interlacing yourself.
	*/
	iData = image->data;
	col = image->column;	   
	row_pointers = (u_char**)malloc(image->row * sizeof (u_char *));
	for (k = 0; k < image->row ; k++)
	{
   
	   row_pointers[k] = (u_char*)malloc(col * sizeof (u_char ));
	   tp = row_pointers[k];


	   for ( j = 0 ; j < col ; j ++ )
	   {
		   *tp++ = *iData;
		   iData++;
	   }
   
	}

	/* the following output method is REQUIRED */
	png_write_image(png_ptr, row_pointers);   


	/* You can write optional chunks like tEXt, zTXt, and tIME at the end
	* as well.  Shouldn't be necessary in 1.1.0 and up as all the public
	* chunks are supported and you can use png_set_unknown_chunks() to
	* register unknown chunks into the info structure to be written out.
	*/

	/* It is REQUIRED to call this to finish writing the rest of the file */
	png_write_end(png_ptr, info_ptr);

	/* if you png_malloced any data that you passed in with
	  png_set_something(), such as a hist or trans array, free it here,
	  when you can be sure that libpng is through with it. */
	//   png_free(png_ptr, trans);
	//  trans=NULL;
	for ( k = 0 ; k < image->row ; k++)
	{
	   free(row_pointers[k]);
	}
	free(row_pointers);

	/* clean up after the write, and free any memory allocated */
	png_destroy_write_struct(&png_ptr, &info_ptr);


	bytes = ftell( fp );

	/* that's it */
	return bytes;
}
