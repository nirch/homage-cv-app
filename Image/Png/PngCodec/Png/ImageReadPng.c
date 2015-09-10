#include "ImageReadPng.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <malloc.h>


#include "png.h"
#include "ImageType/ImageType.h"

#define png_jmpbuf(png_ptr) ((png_ptr)->jmpbuf)

u_char *png_image_build_data(u_char *data,u_char* row_pointer, int depth, int row_length,int colorMethod, png_infop info_ptr );
u_char *png_image_build_data_from_depth_1_2_4(u_char *data,u_char* row_pointer, int row_length,int colorMethod , png_infop info_ptr, int depth);
u_char *png_image_build_data_from_depth_1(u_char *data,u_char* row_pointer, int row_length,int colorMethod , png_infop info_ptr, int depth);
u_char *png_image_build_data_from_depth_2(u_char *data,u_char* row_pointer, int row_length,int colorMethod , png_infop info_ptr, int depth);
u_char *png_image_build_data_from_depth_4(u_char *data,u_char* row_pointer, int row_length,int colorMethod , png_infop info_ptr, int depth);
u_char *png_image_build_data_from_depth_8(u_char *data,u_char* row_pointer, int row_length,int colorMethod, png_infop info_ptr);
u_char *png_image_build_data_from_depth_16(u_char *data,u_char* row_pointer, int row_length,int colorMethod);

//#define PNG_GRAYSCALE 0
//#define PNG_RGB	2
//#define PNG_PALETTE 3
//#define PNG_GRAYSCALE_ALPHA 4
//#define PNG_RGB_ALPHA 6

//PNG_COLOR_TYPE_GRAY 
//PNG_COLOR_TYPE_PALETTE  
//define PNG_COLOR_TYPE_RGB        
//define PNG_COLOR_TYPE_RGB_ALPHA  
//define PNG_COLOR_TYPE_GRAY_ALPHA


/****** IsPNG *****
Description:Check if the string is a PNG signature. The str length must be 8 bytes
		The signature is (char*){211,'P','N','G','\r','\n',32,'\n'}
Params: sig - the signature to check	  
Returns:
*****************************/
static unsigned int IsPNG(const char *sig)
{
	int length = strlen(sig);
	if (length < 8)
		return(0);
	if (memcmp(sig,"\211PNG\r\n\032\n",8) == 0)
		return(1);
	return(0);
}
static png_color_16  my_background;
static int	my_background_exist;


int
image_read_png_is_valid_SINGATURE( char *data, int bytes )
{
	if( IsPNG( data  ) == 1 )
		return( 1 );

	return( -1 );
}

image_type *
image_read_png_buffer( char *data, int bytes, int Fdata )
{
image_type	*im;
char filename[256];

	gp_filename_get_temporary("png",0,".png",filename);

	gpFile_write_buffer(filename,data,bytes);

	im = image_read_png_file((char*)filename);
	unlink(filename);


	return( im );
}


image_type *image_read_png_file(char *file_name)  /* We need to open the file */
{
	image_type * image = NULL;
	u_char *data, *dataPtr;	
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int y;
	png_uint_32 width, height, row;
	png_color_16 *image_background = NULL ;
	int bit_depth, color_type, interlace_type;
	int number_passes ;
	int pass;
	char line[256];
	FILE *fp;
	int kkss;



	png_bytep *row_pointers; 


	if ((fp = fopen(file_name, "rb")) == NULL)
		return (NULL);
	
	// check if this is a PNG file:
	fread(line,1,8,fp);//read signature
	if ( ! IsPNG(line) )
	{
		fclose(fp);
		return NULL;
	}

	/* Create and initialize the png_struct with the desired error handler
	* functions.  If you want to use the default stderr and longjump method,
	* you can supply NULL for the last three parameters.  We also supply the
	* the compiler header file version, so that we know if the application
	* was compiled with a compatible version of the library.  REQUIRED
	*/
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL , NULL, NULL);

	if (png_ptr == NULL)
	{
		fclose(fp);
		return (NULL);
	}

	/* Allocate/initialize the memory for image information.  REQUIRED. */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fclose(fp);
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		return (NULL);
	}

	/* Set error handling if you are using the setjmp/longjmp method (this is
	* the normal method of doing things with libpng).  REQUIRED unless you
	* set up your own error handlers in the png_create_read_struct() earlier.
	*/
	if (setjmp(png_jmpbuf(png_ptr)))
	{
	  /* Free all of the memory associated with the png_ptr and info_ptr */
	  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
	  fclose(fp);
	  /* If we get here, we had a problem reading the file */
	  return (NULL);
	}

	/* Set up the input control if using standard C streams */
	png_init_io(png_ptr, fp);

	/* We have already read some of the signature - 8 bytes*/
	png_set_sig_bytes(png_ptr, 8);


	/*The call to png_read_info() gives us all of the information from the
	* PNG file before the first IDAT (image data chunk).  REQUIRED
	*/
	png_read_info(png_ptr, info_ptr);

	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
	   &interlace_type, NULL, NULL);
//	sprintf(line,"depth:%d color:%d",(int)bit_depth,(int)color_type);
//	warning("png",line);
	
	/*
	*	Set up the data transformations you want.  
	*	Note that these are all optional.  
	*	Only call them if you want/need them.  
	*	Many of the transformations only work on specific types of images, 
	*	and many are mutually exclusive.	
	*/

	/* Set the background color to draw transparent and alpha images over.
	* It is possible to set the red, green, and blue components directly
	* for paletted images instead of supplying a palette index.  Note that
	* even if the PNG file supplies a background, you are not required to
	* use it - you should use the (solid) application background if it has one.
	*/

	
	if (png_get_bKGD(png_ptr, info_ptr, &image_background))
	{		
		kkss = bit_depth - 8;
		my_background.red = image_background->red >> kkss;
		my_background.green = image_background->green >> kkss;
		my_background.blue = image_background->blue >> kkss;
		my_background.gray = image_background->gray >> kkss;
		my_background.index = image_background->index;

		my_background_exist = 1;

	}
	else
	{
		my_background.blue = 0xdf;
		my_background.red = 0xdf;
		my_background.green = 0xdf;
		my_background.index = 0;
		my_background.gray = 0xdf;		
		my_background_exist = 0;
	}
 
	

	row_pointers = (png_bytep*)malloc(height*sizeof(png_bytep));

	/* Turn on interlace handling.  REQUIRED if you are not using
	* png_read_image().  To see how to handle interlacing passes,
	* see the png_read_row() method below:
	*/
	number_passes = png_set_interlace_handling(png_ptr);


	/* Allocate the memory to hold the image using the fields of info_ptr. */
	

	for (row = 0; row < height; row++)
	{
		row_pointers[row] = (unsigned char*)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
	}



	/* The other way to read images - deal with interlacing: */
	for (pass = 0; pass < number_passes; pass++)
	{
		/* Read the image a single row at a time */
		for (y = 0; y < height; y++)
		{
			png_read_rows(png_ptr, &row_pointers[y], NULL, 1);
		}
	}// for pass

	png_read_end(png_ptr, info_ptr);
	/* At this point you have read the entire image */

	dataPtr = data = (u_char*)malloc(height * width * 4 * sizeof(u_char));
	
	
	/* put the image in a IMAGE_TYPE struct */
	for ( y = 0 ; y < height ; y++)
	{
		dataPtr = png_image_build_data(dataPtr,row_pointers[y],bit_depth,width,color_type, info_ptr);
		png_free(png_ptr, row_pointers[y]);
	}
	image = image_create(height, width, 4 /*our bit_depth is byte_depth, and we have 4 bytes per pixel*/, 
			1, data);

	/* clean up after the read, and free any memory allocated - REQUIRED */
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
	
	free ( row_pointers);

	/* close the file */
	fclose(fp);

	/* that's it */
	return (image);

}//read



/****** png_image_build_data *****
Description:The method accepts a row pointer, and adds it to the data. 
		It then returns pointer to the data array at the end point of insertion.
		
Params:	data		- the image data array, with sufficient space to insert data from 
				  row_pointer array.
		row_pointer - the data to add.
		depth		- depth of pixel
		row_length	- length of the row in bytes
		colorMethod - RGB, palette, grayscale, RGB + alpha, grayscale + alpha
		info_ptr	- holds the info of the png (including palette)
Returns:    pointer to the data array at the end point of insertion.
*****************************/
u_char *png_image_build_data(u_char *data,u_char* row_pointer, int depth, int row_length,int colorMethod, png_infop info_ptr)
{
	switch (depth)
	{
	case 1:	// 2 color 
	case 2:	// 4 colors
	case 4:	// 16 colors
		return png_image_build_data_from_depth_1_2_4(data,row_pointer,row_length, colorMethod, info_ptr,depth);
		break;
	case 8:	// 256 GRAY TINTS or 256 RGB COLORS, or PALETTE (might include ALPHA)
		return png_image_build_data_from_depth_8(data,row_pointer,row_length,colorMethod, info_ptr);
		break;
	case 16:	// GRAY TINTS or RGB COLORS (might include ALPHA)
		return png_image_build_data_from_depth_16(data,row_pointer,row_length,colorMethod);		
		break;
	}	
	return data;
}

// 16 gray colors
u_char *png_image_build_data_from_depth_1_2_4(u_char *data,u_char* row_pointer, int row_length,int colorMethod, png_infop info_ptr, int depth)
{
	int i, counter, index,	r, g, b;
	u_char mask,start_mask;
	u_char value;
	u_int *iData;
	float color_gap;
	
	switch (depth)
	{
	case 1: 
		start_mask = 128;
		color_gap = 255.0f;
		break;
	case 2:
		start_mask = 192;
		color_gap = 85.33f;
		break;
	case 4:
		start_mask = 240;
		color_gap = 17.067f;
		break;
	}
	iData = (u_int*)data;
	if ( colorMethod ==  PNG_COLOR_TYPE_PALETTE)
	{
		for ( i = 0; i < row_length;  )
		{
			for ( counter = 0,mask = start_mask;
				i < row_length && (counter < 8); 
				mask = (u_char)mask>>depth, i ++, counter += depth)
			{
				value = (*row_pointer) & mask;
				index = value >> (8 - depth - counter);
				r = (int)info_ptr->palette[index].red;
				g = (int)info_ptr->palette[index].green;
				b = (int)info_ptr->palette[index].blue; 
				*iData++ = IMAGE4_RGB( r, g, b );
			}
			row_pointer ++ ;	
		}
	}
	else // PNG_GRAYSCALE
	{
		for ( i = 0; i < row_length;) 	
		{	
			for ( counter = 0,mask = start_mask;
				i < row_length && (counter < 8); 
				mask = (u_char)mask>>depth, i ++, counter += depth)
			{
				value = (*row_pointer) & mask;
				value = (u_char)(color_gap *(value>> (8 - depth - counter)));
				*iData++ = IMAGE4_RGB( value, value, value);
				
			}
			row_pointer ++ ;	
		}
	}//else
	return (u_char*)iData;
	//return data;
}//depth 1,2,4



u_char *png_image_build_data_from_depth_8(u_char *data,u_char* row_pointer, int row_length,int colorMethod, png_infop info_ptr)
{
u_int	color[256];
	int i, index;
	u_int *iData;
	int r, g, b;
	u_char value;
	int iValue, iAlpha, max_pixel_value, gray_bg;
	int red_bg = my_background.red, blue_bg = my_background.blue,green_bg = my_background.green;
	gray_bg = my_background.gray;
	max_pixel_value = 0xff;
	iData = (u_int*)data;




	// grayscale with no alpha
	switch ( colorMethod )
	{
	case PNG_COLOR_TYPE_GRAY:
		for ( i = 0; i < row_length; i++) 
		{
			value = *row_pointer;
			*iData++ = IMAGE4_RGB( value, value, value);
			row_pointer ++ ;
			
		}
		break;
	//case PNG_COLOR_TYPE_RGB_ALPHA:
	case PNG_COLOR_TYPE_RGB:
		// RGB with no alpha
		for ( i = 0; i < row_length; i++) 
		{		
		
			r = (int)*row_pointer  ;
			g = (int)*(row_pointer + 1);
			b = (int)*(row_pointer + 2);
			*iData++ = IMAGE4_RGB( r, g, b);
			row_pointer += 3 ;
			
		}
		break;
/*
	case PNG_COLOR_TYPE_PALETTE:
		for ( i = 0; i < row_length; i ++ )
		{
			index = *row_pointer;
			r = (int)info_ptr->palette[index].red;
			g = (int)info_ptr->palette[index].green;
			b = (int)info_ptr->palette[index].blue; 
			*iData++ = IMAGE4_RGB( r, g, b );
			row_pointer ++ ;
			
		}
*/

	case PNG_COLOR_TYPE_PALETTE:
		for( i = 0 ; i < info_ptr->num_palette ; i++ ){
			r = (int)info_ptr->palette[i].red;
			g = (int)info_ptr->palette[i].green;
			b = (int)info_ptr->palette[i].blue; 
			color[i] = IMAGE4_RGB( r, g, b );

			if( info_ptr->trans != NULL && info_ptr->trans[i] == 0 )
				color[i] |= 0xff000000;
		}


		for ( i = 0; i < row_length; i ++ ){
			index = *row_pointer;
			*iData++ = color[index];
			row_pointer ++ ;
			
		}
		break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		// grayscale with alpha!!! (merge with black)
		for ( i = 0; i < row_length; i++) 
		{
			iValue = *(row_pointer );
			iAlpha = *( row_pointer + 1);
			value = (int)((iAlpha * iValue + (max_pixel_value - iAlpha)* gray_bg)/max_pixel_value);
			
			*iData++ = IMAGE4_RGB( value, value, value);
			row_pointer += 2 ;
			
		}
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		// RGB with alpha
		if( my_background_exist == 0 ){
			for ( i = 0, iData = (u_int*)data ; i < row_length; i++){
				iAlpha = *(row_pointer + 3);

				r = (*row_pointer);//r
				g = *(row_pointer + 1);//g
				 
				b = *(row_pointer + 2);//b
				
				*iData++ = IMAGE4_RGB(r, g, b) | (( 255 - iAlpha)<<24);
				row_pointer += 4 ;
			}

			return (u_char*)iData;
		}


		for ( i = 0; i < row_length; i++) 
		{
			iAlpha = *(row_pointer + 3);

//			*(data + 3) = 0;

			iValue = (*row_pointer);//r
			r = (int)((iAlpha * iValue + (max_pixel_value - iAlpha)* red_bg)/max_pixel_value);
			

			iValue = *(row_pointer + 1);//g
			g = (int)((iAlpha * iValue + (max_pixel_value - iAlpha)* green_bg)/max_pixel_value);
			 
			iValue = *(row_pointer + 2);//b
			b = (int)((iAlpha * iValue + (max_pixel_value - iAlpha)* blue_bg)/max_pixel_value);
			
			*iData++ = IMAGE4_RGB(r, g, b);
			row_pointer += 4 ;
			
		}
		break;				

	}//switch

	
	return (u_char*)iData;
	//return data;
}
u_char *png_image_build_data_from_depth_16(u_char *data,u_char* row_pointer, int row_length,int colorMethod)
{
	u_int *iData;
	int r, g, b;
	int i;
	int max_pixel_value = 0xff, gray_bg = my_background.gray;
	int iValue, iAlpha;
	u_char value;	
	int red_bg = my_background.red, blue_bg = my_background.blue, green_bg = my_background.green;
	iData = (u_int*)data;
	switch ( colorMethod )
	{
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		// grayscale with alpha!!! (merge with black)
		for ( i = 0; i < row_length; i++) 
		{
			iAlpha = *(row_pointer + 2);
			value = (int)((iAlpha * (*row_pointer )+ (max_pixel_value - iAlpha)* gray_bg)/max_pixel_value);
			*iData++ = IMAGE4_RGB(value, value, value);
			row_pointer += 4 ;
			
		}
		break;
	case PNG_COLOR_TYPE_GRAY:	
		// grayscale with no alpha
		for ( i = 0; i < row_length; i++) 
		{
			value = (int)*row_pointer;
			*iData++ = IMAGE4_RGB(value, value, value);
			row_pointer += 2 ;
			
		}
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		// RGB with alpha
		for ( i = 0; i < row_length; i++) 
		{
			iAlpha = *(row_pointer + 6);

			
			iValue = (*row_pointer);//r
			r = (int)((iAlpha * iValue + (max_pixel_value - iAlpha)* red_bg)/max_pixel_value);
			

			iValue = *(row_pointer + 2);//g
			g = (int)((iAlpha * iValue + (max_pixel_value - iAlpha)* green_bg)/max_pixel_value);
			 
			iValue = *(row_pointer + 4);//b
			b = (int)((iAlpha * iValue + (max_pixel_value - iAlpha)* blue_bg)/max_pixel_value);
			
			*iData++ = IMAGE4_RGB(r, g, b);
			row_pointer += 8 ;			
		}
		break;	
	case PNG_COLOR_TYPE_RGB:
		// RGB with no alpha
		for ( i = 0; i < row_length; i++) 
		{
			r = (int)(*row_pointer);
			g = (int)(*(row_pointer + 2));
			b = (int)(*(row_pointer + 4));
			*iData++ = IMAGE4_RGB(r, g, b);
		
			row_pointer += 6 ;
			
		}
		break;
	}//switch
	return (u_char*)iData;
	
}

