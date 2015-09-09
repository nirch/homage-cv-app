#include        "mbmap.h"
#include        "im_hdr.h"

#define         GRAY_LEVEL( g )         ( ( g < 0)? 0: (( g > 255)? 255 : g ) )

void
  mbmap_rgb_to_yqa( mbmap r, mbmap g, mbmap b, mbmap *y, mbmap *q, mbmap *a )
{
  *y = rgb_to_y( r, g, b );
  *q = yrgb_to_q( *y, r );
  *a = yrgb_to_a( *y, b );
}


void
  mbmap_yqa_to_rgb( mbmap y, mbmap q, mbmap a, mbmap *r, mbmap *g, mbmap *b )
{
  *r = yqa_to_r( y, q );
  *g = yqa_to_g( y, q, a );
  *b = yqa_to_b( y, a );
}


mbmap yrgb_to_q(mbmap kbmap,mbmap rbmap)
{
  mbmap	bmap ;
  u_char	*q, *k, *r ;
  long	val ;
  long	pix, size = MBMAP_DATA_SIZE(kbmap) ;
  
  MBMAP_CREATE( bmap, MBMAP_ROW(kbmap), MBMAP_COLUMN(kbmap) );
  
  q = MBMAP_PIXEL_ADRESS( bmap, 0, 0 );
  
  k = MBMAP_PIXEL_ADRESS( kbmap, 0, 0 ) ;
  
  r = MBMAP_PIXEL_ADRESS( rbmap, 0, 0 ) ;
  
  
  
  for (pix=0 ; pix < size ; pix++){
    val = (int)(*k++) - (int)(*r++) + 128 ;
      *q++ = GRAY_LEVEL(val);
  }
  
  return (bmap);
}



mbmap yrgb_to_a(mbmap kbmap,mbmap bbmap)
{
  mbmap   bmap ;
  u_char   *a, *k, *b ;
  long	val ;
  long	pix, size = MBMAP_DATA_SIZE(kbmap) ;
  
  MBMAP_CREATE( bmap, MBMAP_ROW(kbmap), MBMAP_COLUMN(kbmap) );
  
  a = MBMAP_PIXEL_ADRESS( bmap, 0, 0 );
  
  k = MBMAP_PIXEL_ADRESS( kbmap, 0, 0 ) ;
  
  b = MBMAP_PIXEL_ADRESS( bbmap, 0, 0 ) ;
  
  for (pix=0 ; pix < size ; pix++){
    val = (int)(*k++) - (int)(*b++) + 128 ;
    *a++ = GRAY_LEVEL(val) ;
  }
  
  return (bmap);
}

mbmap yqa_to_r(mbmap kbmap, mbmap qbmap)
{
  mbmap   bmap ;
  u_char   *r, *k, *q ;
  long	val ;
  long	pix, size = MBMAP_DATA_SIZE(kbmap) ;
   
  MBMAP_CREATE( bmap, MBMAP_ROW(kbmap), MBMAP_COLUMN(kbmap) );
  
  r = MBMAP_PIXEL_ADRESS( bmap, 0, 0 );
  
  k = MBMAP_PIXEL_ADRESS( kbmap, 0, 0 ) ;
  
  q = MBMAP_PIXEL_ADRESS( qbmap, 0, 0 ) ;
  
  for (pix=0 ; pix < size ; pix++){    
    val = (int)(*k++) - (int)(*q++) + 128 ;
    *r++ = GRAY_LEVEL(val) ;
  }

  return (bmap);
}


mbmap yqa_to_b(mbmap kbmap, mbmap abmap)
{
  mbmap   bmap ;
  u_char   *a, *k, *b ;
  long	val ;
  long	pix, size = MBMAP_DATA_SIZE(kbmap) ;

  
  MBMAP_CREATE( bmap, MBMAP_ROW(kbmap), MBMAP_COLUMN(kbmap) );
  
  b = MBMAP_PIXEL_ADRESS( bmap, 0, 0 );
  
  k = MBMAP_PIXEL_ADRESS( kbmap, 0, 0 ) ;
  
  a = MBMAP_PIXEL_ADRESS( abmap, 0, 0 ) ;
  
  for (pix=0 ; pix < size ; pix++){
      
    val = (int)(*k++) - (int)(*a++) + 128;    
    *b++ = GRAY_LEVEL(val) ;
  }
  
  return (bmap);
}



mbmap yqa_to_g(mbmap kbmap,mbmap qbmap,mbmap abmap)
{
  mbmap   bmap ;
  u_char   *g, *k, *q, *a ;
  long	val ;
  long	pix, size = MBMAP_DATA_SIZE(kbmap) ;
  
  MBMAP_CREATE( bmap, MBMAP_ROW(kbmap), MBMAP_COLUMN(kbmap) );
  
  g = MBMAP_PIXEL_ADRESS( bmap, 0, 0 );
  
  k = MBMAP_PIXEL_ADRESS( kbmap, 0, 0 ) ;
  
  q = MBMAP_PIXEL_ADRESS( qbmap, 0, 0 ) ;
  
  a = MBMAP_PIXEL_ADRESS( abmap, 0, 0 ) ;
  
  for (pix=0 ; pix < size ; pix++){
    /*
      val = (float)*(k++) +  0.509370 * ((float)*(q++)) + 
      0.194208 * ((float)*(a++)) - 0.703578 * 128  ;
      */

    val = 522 * ((long)*(q++)) + 199 * ((long)*(a++)) - 91707 /* 92288 */ ;
  
    val = (val >> 10) + (long)*(k++);
    *g++ = GRAY_LEVEL(val) ;
      
  }
  
  return (bmap);
}


mbmap rgb_to_y(mbmap rbmap,mbmap gbmap,mbmap bbmap)
{
  mbmap   bmap ;
  u_char   *k, *r, *g, *b ;
  long   val ;
  long	pix, size = MBMAP_DATA_SIZE(gbmap) ;
  
  MBMAP_CREATE( bmap, MBMAP_ROW(rbmap), MBMAP_COLUMN(rbmap) );
  
  k = MBMAP_PIXEL_ADRESS( bmap, 0, 0 );
  
  r = MBMAP_PIXEL_ADRESS( rbmap, 0, 0 ) ;
  
  g = MBMAP_PIXEL_ADRESS( gbmap, 0, 0 ) ;
  
  b = MBMAP_PIXEL_ADRESS( bbmap, 0, 0 ) ;
  
  for (pix = 0 ; pix < size ; pix++){
    /*
      val =  0.299* ((float)*(r++)) +  0.587 * ((float)*(g++)) +
      0.114 * ((float)*(b++))  + 0.5;
      */
    /* Coefficiants in the next operator are from previous one,
       multiplied by 1024
       */
    val =  306L * ((long)*(r++)) +  601L * ((long)*(g++)) +
      117L * ((long)*(b++))  + 512;

    val >>= 10;
    *k++ = GRAY_LEVEL(val) ;
  }
  
  return (bmap);
}



mbmap rgb_to_q(mbmap rbmap,mbmap gbmap,mbmap bbmap)
{
  mbmap   bmap ;
  u_char   *q, *r, *g, *b ;
  long   val ;
  long	pix, size = MBMAP_DATA_SIZE(gbmap) ;
  
  MBMAP_CREATE( bmap, MBMAP_ROW(rbmap), MBMAP_COLUMN(rbmap) );
  
  q = MBMAP_PIXEL_ADRESS( bmap, 0, 0 );
  
  r = MBMAP_PIXEL_ADRESS( rbmap, 0, 0 ) ;
  
  g = MBMAP_PIXEL_ADRESS( gbmap, 0, 0 ) ;
  
  b = MBMAP_PIXEL_ADRESS( bbmap, 0, 0 ) ;
  
  for (pix=0 ; pix < size ; pix++){
    /*
      val =  -0.701 * ((float)*(r++)) +  0.587 * ((float)*(g++)) +
      0.114 * ((float)*(b++)) ;
      */

    val =  -718 * ((long)*(r++)) +  601 * ((long)*(g++)) +
      117 * ((long)*(b++)) ;

    val >>= 10;
    *q++ = GRAY_LEVEL(val) ;
    
  }
  
  return (bmap);
}






mbmap rgb_to_a(mbmap rbmap,mbmap gbmap,mbmap bbmap)
{
  mbmap   bmap ;   
  u_char   *a, *r, *g, *b ;
  long   val ;
  long	pix, size = MBMAP_DATA_SIZE(gbmap) ;
  
  MBMAP_CREATE( bmap, MBMAP_ROW(rbmap), MBMAP_COLUMN(rbmap) );
  
  a = MBMAP_PIXEL_ADRESS( bmap, 0, 0 );
  
  r = MBMAP_PIXEL_ADRESS( rbmap, 0, 0 ) ;
  
  g = MBMAP_PIXEL_ADRESS( gbmap, 0, 0 ) ;
  
  b = MBMAP_PIXEL_ADRESS( bbmap, 0, 0 ) ; 
  
  for (pix=0 ; pix < size ; pix++){ 
      
      /* pre-Ansi
	 val =  0.299 * (*r++) +  0.587 * (*g++) -  0.886 * (*b++) ; 
	 */
      
      val =  306 * ((long)*(r++)) +  601 * ((long)*(g++)) - 
	907 * ((long)*(b++)) ; 
      
      val >>= 10;

      *a++ = GRAY_LEVEL(val) ; 
      
    } 
  
  return (bmap); 
} 
