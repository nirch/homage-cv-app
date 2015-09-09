#include	"mbmap.h"
#include	"im_hdr.h"
#include 	<fcntl.h>





mbmap
mbmap_neg( mbmap bmap)
{
  mbmap	tbmap;
  int	row,	column;  
  u_char 	*t,	*s;
  
  MBMAP_CREATE( tbmap, MBMAP_ROW(bmap), MBMAP_COLUMN(bmap) );
  
  t = MBMAP_DATA_ADRESS( tbmap );
  s = MBMAP_DATA_ADRESS( bmap );
  
  for (row=0 ; row < MBMAP_ROW(bmap) ; row++)
    for (column=0 ; column < MBMAP_COLUMN(bmap) ; column++)
      
      *t++ = 255 - ( *s++);
  
  return ( tbmap );
}

mbmap mbmap_row_inverse( mbmap bmap)
{
  mbmap	newbmap;
  int	row,	column;  
  u_char 	*t,	*s;
  
  MBMAP_CREATE( newbmap, MBMAP_ROW(bmap), MBMAP_COLUMN(bmap) );
  
  t = MBMAP_PIXEL_ADRESS( newbmap, 0, 0 ); 
  
  for (row=0 ; row < MBMAP_ROW(bmap) ; row++){
    
    s =MBMAP_PIXEL_ADRESS( bmap, MBMAP_ROW(bmap)-row-1, 0 )  ;
    
    for (column=0 ; column < MBMAP_COLUMN(bmap) ; column++)
      
      *t++ = *s++;
    
  }
  
  return (newbmap);
}



mbmap
mbmap_column_inverse( mbmap bmap)
{
  mbmap	newbmap;
  int	row,	column;  
  u_char 	*t,	*s;
  
  MBMAP_CREATE( newbmap, MBMAP_ROW(bmap), MBMAP_COLUMN(bmap) );
  
  t = MBMAP_PIXEL_ADRESS( newbmap, 0, 0 ); 
  
  for (row=0 ; row < MBMAP_ROW(bmap) ; row++){
    
    s = MBMAP_PIXEL_ADRESS( bmap, row,  MBMAP_COLUMN(bmap)-1 )  ;
    
    for (column=0 ; column < MBMAP_COLUMN(bmap) ;  column++  )
      
      *t++ = *s--;
    
  }
  
  return (newbmap);
}





mbmap
mbmap_y_filter( mbmap bmap)
{
  mbmap   newbmap;
  int     row,    column;
  u_char   *s,     *t;
  
  MBMAP_CREATE(newbmap,MBMAP_ROW(bmap),MBMAP_COLUMN(bmap)/2);
  
  for (row=0;row<MBMAP_ROW(bmap);row++){
    
    t=MBMAP_PIXEL_ADRESS(newbmap,row,0);
    
    s=MBMAP_PIXEL_ADRESS(bmap,row,0);
    
    *t=*(s+1);
    
    for (column=0;column<(MBMAP_COLUMN(bmap)-8)/2;column++){
      
      s+=2;
      /* pre-Ansi
       *t++=(*(s-3)*(-29)+*(s-1)*88+*s*138+*(s+1)*88+*(s+3)*(-29))/256;
       */      
      
      *t++=((float)*(s-3)*(-29) + (float)*(s-1)*88 + (float)*s*138 +
	    (float)*(s+1)*88 + (float)*(s+3)*(-29))/256;
      
    }
    
  }
  
  return ( newbmap );
  
}





mbmap
mbmap_a_q_filter( mbmap bmap)
{
  mbmap   newbmap;
  int     row,    column;
  u_char   *s,     *t;
  
  MBMAP_CREATE(newbmap,MBMAP_ROW(bmap),MBMAP_COLUMN(bmap)/2);
  
  for (row=0;row<MBMAP_ROW(bmap);row++){
    
    t=MBMAP_PIXEL_ADRESS(newbmap,row,0);
    
    s=MBMAP_PIXEL_ADRESS(bmap,row,0);
    
    *t=*s;
    
    for (column=0;column<(MBMAP_COLUMN(bmap)-4)/2;column++){ 
      
      s+=2;
/* pre-Ansi
     *t++=(*(s-2)*(-29)+*(s-1)*157 +*s*157+*(s+1)*(-29))/256;
*/      
      *t++=((float)*(s-2)*(-29) + (float)*(s-1)*157 + (float)*s*157 +
	    (float)*(s+1)*(-29)) /256;
      
      
    }
    
    *t=*s;
    
  }
  
  return ( newbmap );
  
}
