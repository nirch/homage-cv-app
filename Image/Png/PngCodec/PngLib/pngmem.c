
/* pngmem.c - stub functions for memory allocation
 *
 * libpng 1.0.12 - June 8, 2001
 * For conditions of distribution and use, see copyright notice in png.h
 * Copyright (c) 1998-2001 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * This file provides a location for all memory allocation.  Users who
 * need special memory handling are expected to supply replacement
 * functions for png_malloc() and png_free(), and to use
 * png_create_read_struct_2() and png_create_write_struct_2() to
 * identify the replacement functions.
 */

#define PNG_INTERNAL
#include "png.h"


/* Allocate memory for a png_struct or a png_info.  The malloc and
   memset can be replaced by a single call to calloc() if this is thought
   to improve performance noticably.*/
png_voidp /* PRIVATE */
png_create_struct(int type)
{
   png_size_t size;
   png_voidp struct_ptr;

   if (type == PNG_STRUCT_INFO)
      size = sizeof(png_info);
   else if (type == PNG_STRUCT_PNG)
      size = sizeof(png_struct);
   else
      return ((png_voidp)NULL);


# if defined(_MSC_VER) && defined(MAXSEG_64K)
   if ((struct_ptr = (png_voidp)halloc(size,1)) != NULL)
# else
   if ((struct_ptr = (png_voidp)malloc(size)) != NULL)
# endif
   {
      png_memset(struct_ptr, 0, size);
   }

   return (struct_ptr);
}


/* Free memory allocated by a png_create_struct() call */
void /* PRIVATE */
png_destroy_struct(png_voidp struct_ptr)
{
   if (struct_ptr != NULL)
   {
# if defined(_MSC_VER) && defined(MAXSEG_64K)
      hfree(struct_ptr);
# else
      free(struct_ptr);
# endif
   }
}


/* Allocate memory.  For reasonable files, size should never exceed
   64K.  However, zlib may allocate more then 64K if you don't tell
   it not to.  See zconf.h and png.h for more information.  zlib does
   need to allocate exactly 64K, so whatever you call here must
   have the ability to do that. */

png_voidp PNGAPI
png_malloc(png_structp png_ptr, png_uint_32 size)
{
   png_voidp ret;
   if (png_ptr == NULL || size == 0)
      return ((png_voidp)NULL);


#ifdef PNG_MAX_MALLOC_64K
   if (size > (png_uint_32)65536L)
      png_error(png_ptr, "Cannot Allocate > 64K");
#endif

# if defined(_MSC_VER) && defined(MAXSEG_64K)
   ret = halloc(size, 1);
# else
   ret = malloc((size_t)size);
# endif

   if (ret == NULL)
      png_error(png_ptr, "Out of Memory");

   return (ret);
}

/* Free a pointer allocated by png_malloc().  If ptr is NULL, return
   without taking any action. */
void PNGAPI
png_free(png_structp png_ptr, png_voidp ptr)
{
   if (png_ptr == NULL || ptr == NULL)
      return;

# if defined(_MSC_VER) && defined(MAXSEG_64K)
   hfree(ptr);
# else
   free(ptr);
# endif
}


png_voidp /* PRIVATE */
png_memcpy_check (png_structp png_ptr, png_voidp s1, png_voidp s2,
   png_uint_32 length)
{
   png_size_t size;

   size = (png_size_t)length;
   if ((png_uint_32)size != length)
      png_error(png_ptr,"Overflow in png_memcpy_check.");

   return(png_memcpy (s1, s2, size));
}

png_voidp /* PRIVATE */
png_memset_check (png_structp png_ptr, png_voidp s1, int value,
   png_uint_32 length)
{
   png_size_t size;

   size = (png_size_t)length;
   if ((png_uint_32)size != length)
      png_error(png_ptr,"Overflow in png_memset_check.");

   return (png_memset (s1, value, size));

}

