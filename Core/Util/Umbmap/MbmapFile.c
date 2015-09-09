/****************************************************************
 *								*
 *		Module:  mbmap_io.c				*
 *								*
 ****************************************************************/

#include	<stdio.h>

#include	<fcntl.h>

#include	"Uigp/igp.h"
#include	"mbmap.h"

#ifdef   MSDOS
#define  SEPARATOR  "."
#else
#define  SEPARATOR  "_"
#endif


void mbmap_gp_bmap_file_name(char *dir,char *image,char *color,char *file)
{
  if( dir == NULL )
    if( color == NULL )
      sprintf( file, "%s", image );
    else
	sprintf( file, "%s%s%s", image, SEPARATOR, color );
  else	if( color == NULL )
    sprintf( file, "%s/%s", dir, image );
  else	sprintf( file, "%s/%s%s%s", dir, image, SEPARATOR, color);
}



int
mbmap_is_gp_bmap_file_exist(char *dir_name,char *image_name,char *color)
{
  struct  IM_HEADER       im_h;
  char	file[100];
  short	fd;
  
  mbmap_gp_bmap_file_name( dir_name, image_name, color, file );
  
#ifdef _WINDOWS
   if( (fd = (short)open(file, O_RDONLY | O_BINARY, 0644 ) ) < 0 )
#else
   if( (fd = (short)open(file, O_RDONLY, 0644 ) ) < 0 )
#endif
    return( 0 );
  
  
  mbmap_get_im_header_from_gp_bmap_fd( fd, &im_h );
  
  close( fd );
  
  if( !IM_IS_GP_BMAP_FILE( im_h ) )	return( 0 );
  
  return( 1 );
}



int
mbmap_is_gp_rgb_bmap_exist(char *dir, char *image_name )
{
	if( mbmap_is_gp_bmap_file_exist( dir, image_name, "R" ) == 0 )
		return( 0 );

	if( mbmap_is_gp_bmap_file_exist( dir, image_name, "G" ) == 0 )
		return( 0 );

	if( mbmap_is_gp_bmap_file_exist( dir, image_name, "B" ) == 0 )
		return( 0 );

	return( 1 );
}




void
mbmap_gp_bmap_file_size(char *dir_name,char *image_name,char *color,
				int *row,int *column)
{
  struct  IM_HEADER       im_h;
  char	file[100];
  short	fd;
  
  
  mbmap_gp_bmap_file_name( dir_name, image_name, color, file );
  
#ifdef _WINDOWS
   if( (fd = (short)open(file, O_RDONLY | O_BINARY, 0644 ) ) < 0 )
#else
   if( (fd = (short)open(file, O_RDONLY, 0644 ) ) < 0 )
#endif
    error("bmap_size", "can't open bmap file" );
  
  mbmap_get_im_header_from_gp_bmap_fd( fd, &im_h );
  
  *row = (int)im_h.row;
  *column = (int)im_h.column;
  
  close( fd );
}


void
mbmap_get_im_header_from_gp_bmap_fd( int fd, 
					 struct IM_HEADER * im_h )
{
  u_char buf[ IM_HEADER_SIZE ];
  int  row, col;
  
  
  if ( read( (short)fd, (char *)buf, IM_HEADER_SIZE ) != IM_HEADER_SIZE )
    error("mbmap_get_im_header_from_gp_bmap_fd",
	       "cannot read header");
  
  /* 
    For portability reasons "row" and "column" are stored as a
    series of chars
    */ 
  
  
  col = ( int )( buf[6] | 0x00 )  << 8;     /* get 8 MSB of column */
  col = ( int )( buf[7] | col );            /* get 8 LSB of column */
  
  row = ( int )( buf[10] | 0x00 )  << 8;    /* get 8 MSB of row */
  row = ( int )( buf[11] | row );           /* get 8 LSB of row */
  
  FILL_IM_HEADER( (*im_h), col, row,
		 (( char * ) &buf[12] ), (( char * ) &buf[32] ) );
  strncpy( im_h->h, ( char * ) &buf[0], 4 );
  
  
}



void
mbmap_put_im_header_into_gp_bmap_fd( int fd, struct IM_HEADER * im_h )
{
  u_char buf[ IM_HEADER_SIZE ];
  
  
  
  if ( im_h->h != NULL) 
    memcpy( ( char * ) &buf[0], im_h->h, 4 );
  
  if ( im_h->image_name != NULL)
    memcpy( ( char * ) &buf[12], im_h->image_name, 20);
  
  if ( im_h->comments != NULL) 
    memcpy( ( char * ) &buf[32], im_h->comments, 32 );
  
  
  
  /* 
    For portability reasons "row" and "column" are stored as a
    series of chars
    */ 
  
  memcpy( ( char * ) &buf[4], "\0\0", 2 );     /* leading zeros */
  buf[6] = 
    ( u_char )( ( im_h->column & 0xff00 )  >> 8 ); /* 8 MSB of column */
  buf[7] = 
    ( u_char )( im_h->column & 0xff );         /* 8 LSB of column */
  
  memcpy( ( char * ) &buf[8], "\0\0", 2 );     /* leading zeros */
  buf[10] = 
    ( u_char )( ( im_h->row & 0xff00 )  >> 8 );    /* 8 MSB of row */
  buf[11] = 
    ( u_char )( im_h->row & 0xff );            /* 8 LSB of row */
  
  if( write( fd, (char *)buf, IM_HEADER_SIZE ) != IM_HEADER_SIZE )
    error("mbmap_put_im_header_intom_gp_bmap_fd",
	       "cannot write header");
}



void
mbmap_dump_to_gp_bmap_file( char *dir_name, char *image_name,
				char *color,  mbmap	bmap )
{
  struct  IM_HEADER       im_h;
  char	file[100];
  short	fd;
  
  mbmap_gp_bmap_file_name( dir_name, image_name, color, file );
  
#ifdef _WINDOWS
  if( ( fd = open( file, O_WRONLY | O_CREAT | O_TRUNC| O_BINARY,0644)) == -1 )
#else
  if( ( fd = open( file, O_WRONLY | O_CREAT | O_TRUNC, 0644 )) == -1 )
#endif
    error("mbmap_dump_to_gp_bmap_file", "Can't open image file" );
  
  
  FILL_IM_HEADER( im_h, MBMAP_COLUMN(bmap), MBMAP_ROW( bmap ), 
		 image_name, NULL );
  
  mbmap_put_im_header_into_gp_bmap_fd( fd, &im_h );
  
  write( fd, (char *)MBMAP_DATA_ADRESS(bmap), MBMAP_DATA_SIZE( bmap ) );
  
  close( fd );
}


void
mbmap_dump_to_open_gp_bmap_file(int fd,char *image_name,struct MBMAP bmap)
{
  struct  IM_HEADER       im_h;
  
  FILL_IM_HEADER( im_h, MBMAP_COLUMN(bmap), MBMAP_ROW( bmap ), 
		 image_name, NULL );
  
  mbmap_put_im_header_into_gp_bmap_fd( fd, &im_h );
  
  write( (short)fd, (char *)MBMAP_DATA_ADRESS(bmap), MBMAP_DATA_SIZE( bmap ) );
}



mbmap
mbmap_load_from_gp_bmap_file(char *dir_name,char *image_name,
				   char *color )
{
  mbmap	bmap;
  struct  IM_HEADER       im_h;
  char	file[100];
  short	fd;
  
  mbmap_gp_bmap_file_name( dir_name, image_name, color, file );
  
#ifdef _WINDOWS
   if( (fd = (short)open(file, O_RDONLY | O_BINARY, 0644 ) ) < 0 )
#else
   if( (fd = (short)open(file, O_RDONLY, 0644 ) ) < 0 )
#endif

    error("mbmap_load_from_gp_bmap_file", "Can't open image file");
  
  
  
  mbmap_get_im_header_from_gp_bmap_fd( fd, &im_h );
  
  MBMAP_CREATE( bmap, im_h.row, im_h.column );
  

  read( fd, (char *)MBMAP_DATA_ADRESS(bmap), MBMAP_DATA_SIZE( bmap ) );
  
  close( fd );
  
  return( bmap );
}



int
mbmap_open_to_read_gp_bmap_file( char *dir_name,char *image_name, char *color, 
			int *row, int *column )
{
char	file[100];
struct  IM_HEADER  im_h;
int	fd;
  
	mbmap_gp_bmap_file_name( dir_name, image_name, color, file );
  
#ifdef _WINDOWS
	if( (fd = (short)open(file, O_RDONLY | O_BINARY, 0644 ) ) < 0 )
#else
	if( (fd = (short)open(file, O_RDONLY, 0644 ) ) < 0 )
#endif
		return( fd );
  
	mbmap_get_im_header_from_gp_bmap_fd( fd, &im_h );
	*row = im_h.row;
	*column = im_h.column;
  
	return( fd );
}


int
mbmap_open_to_write_gp_bmap_file(char *dir_name,char *image_name,
			      char *color, int row, int col )
{
char	file[100];
int	fd;
struct IM_HEADER	im_h;
  

	mbmap_gp_bmap_file_name( dir_name, image_name, color, file );
  
#ifdef _WINDOWS
    if( ( fd = open( file, O_WRONLY | O_CREAT | O_TRUNC| O_BINARY,0644)) == -1 )
#else
    if( ( fd = open( file, O_WRONLY | O_CREAT | O_TRUNC, 0644 )) == -1 )
#endif
		return( -1 );
  
	FILL_IM_HEADER( im_h, col, row,	image_name, NULL );
  
	mbmap_put_im_header_into_gp_bmap_fd( fd, &im_h );
  
	return( fd );
}








int
mbmap_open_gp_bmap_file(char *dir_name,char *image_name,
			    char *color, 
			    struct  IM_HEADER  *im_h  )
{
  char	file[100];
  int	fd;
  
  mbmap_gp_bmap_file_name( dir_name, image_name, color, file );
  
#ifdef _WINDOWS
	if( (fd = (short)open(file, O_RDONLY | O_BINARY, 0644 ) ) < 0 )
#else
	if( (fd = (short)open(file, O_RDONLY, 0644 ) ) < 0 )
#endif
    		error("mbmap_open_gp_bmap_file", "Can't open image file");
  
  
  
  mbmap_get_im_header_from_gp_bmap_fd( fd, im_h );
  
  return( fd );
}


int
mbmap_create_gp_bmap_file(char *dir_name,char *image_name,
			      char *color, long row, long col )
{
  char	file[100];
  int	fd;
  struct IM_HEADER im_h;
  
  mbmap_gp_bmap_file_name( dir_name, image_name, color, file );
  
#ifdef _WINDOWS
    if( ( fd = open( file, O_WRONLY | O_CREAT | O_TRUNC| O_BINARY,0644)) == -1 )
#else
    if( ( fd = open( file, O_WRONLY | O_CREAT | O_TRUNC, 0644 ) == -1 ))
#endif
    error("mbmap_create_gp_bmap_file", 
	       "Can't create image file");
  
  FILL_IM_HEADER( im_h, col, row,	image_name, NULL );
  
  mbmap_put_im_header_into_gp_bmap_fd( fd, &im_h );
  
  return( fd );
}



void 
mbmap_load_mbmap_with_gp_bmap_file_segment( int fd, int row0, int col0,
					     struct MBMAP  * bmap )
{
  long jump;
  long size;
  
  jump = IM_HEADER_SIZE +
    (long) MBMAP_COLUMN( (*bmap) ) * row0 + col0;
  
  if ( lseek( fd, jump, SEEK_SET ) == -1L )
    error("mbmap_load_mbmap_with_gp_bmap_file_segment",
	       "Bad File seek");
  
  size = MBMAP_ROW( *(bmap) ) * MBMAP_COLUMN( (*bmap) );
  read( (short)fd, (char *)MBMAP_DATA_ADRESS( *(bmap)), size );
  
}



void 
mbmap_append_mbmap_to_gp_bmap_file( int fd, struct MBMAP  * bmap )
{
  
  write( fd, (char *)MBMAP_DATA_ADRESS( ( *bmap ) ) ,
		   (long)MBMAP_ROW( ( *bmap ) ) * MBMAP_COLUMN( ( *bmap ) ) );
  
}




mbmap
mbmap_load_from_gp_sub_bmap_file(char *dir_name,char *image_name,
				       char *color,int x0,int y0,
				       int row,int column)
{
  
  struct  IM_HEADER       im_h;
  int     i;
  short	  fd;
  char    file[100];
  mbmap   bmap;
  
  
  mbmap_gp_bmap_file_name( dir_name, image_name, color, file );
  
#ifdef _WINDOWS
   if( (fd = (short)open(file, O_RDONLY | O_BINARY, 0644 ) ) < 0 )
#else
   if( (fd = (short)open(file, O_RDONLY, 0644 ) ) < 0 )
#endif
    error("mbmap_load_from_gp_bmap_file", "Can't open image file");
  
  MBMAP_CREATE(bmap,row,column);

  mbmap_get_im_header_from_gp_bmap_fd( fd, &im_h );  
  
  lseek((int)fd,x0*im_h.column+y0+IM_HEADER_SIZE,0);
  
  for ( i=0;i<row ; i++ ) {
    
    read(fd, (char *)MBMAP_PIXEL_ADRESS(bmap,i,0),column);
    
    lseek((int)fd,im_h.column-column,1);
    
  }
  
  close(fd);
  
  return(bmap);
  
}
