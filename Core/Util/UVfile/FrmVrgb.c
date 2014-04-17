#include	<stdio.h>
#include	<sys/types.h>
#include        <fcntl.h>
#include        <malloc.h>

#include	"Vfile.h"


typedef struct VRGB_HEADER {
	char    h[4];		/* "VRGB"	 */
	int     column;
	int     row;
	int     frame_no;
	char    comments[48];
}       vrgb_header;


#define		VRGB_HEADER_SIZE	sizeof( vrgb_header )





static int	vrgb_is_file_exist(char *dir, char *name );

static int	vrgb_open_for_read(char *dir, char *name, Vfile_type * fr);

static int	vrgb_fdopen_for_read(int fd, char *name, Vfile_type * fr);

static int	vrgb_read_header(Vfile_type * fr);

static int 	vrgb_open_for_write(char *dir, char *name, Vfile_type * fr);

static int	vrgb_fdopen_for_write(int fd, char *name, Vfile_type * fr);

static int	vrgb_write_header(Vfile_type * fr);

static int	vrgb_close(Vfile_type * fr);

static int	vrgb_frame_seek(Vfile_type * file, int frame );


static int	vrgb_read_next_rgb_mbmap( Vfile_type *fr,
					mbmap *r, mbmap *g, mbmap *b );

static int	vrgb_write_next_rgb_mbmap( Vfile_type *fr,
					mbmap *r, mbmap *g, mbmap *b );

void		vrgb_read_next( Vfile_type *fr, mbmap *r, mbmap *g, mbmap *b );
void		vrgb_write_next( Vfile_type *fr, mbmap *r, mbmap *g, mbmap *b );





Vfile_op Vrgb = {
	"VRGB",
	vrgb_is_file_exist,
	vrgb_open_for_read,
	vrgb_open_for_write,
	vrgb_fdopen_for_read,
	vrgb_fdopen_for_write,
	vrgb_close,
	vrgb_frame_seek,
	vrgb_read_header,
	vrgb_write_header,
	vrgb_read_next_rgb_mbmap,
	vrgb_write_next_rgb_mbmap
};



/*
 * Bodies of all that functions
 */


void
vrgb_file_name(char *dir_name, char *image_name, char *file)
{
	if (dir_name != NULL)
		sprintf(file, "%s/%s.vrgb", dir_name, image_name);
	else
		sprintf(file, "%s.vrgb", image_name);
}


static int
vrgb_is_file_exist(char *dir, char *name )
{
char    file[256];

	vrgb_file_name(dir, name, file);

	return( gp_is_file_exist( file ) );
}



static int
vrgb_open_for_read(char *dir, char *name, Vfile_type * fr)
{
char    file[100];

	vrgb_file_name(dir, name, file);

	fr->fd = open(file, O_RDONLY);
	if (fr->fd == -1)	return (-1);

	fr->image_header = (void *) malloc(VRGB_HEADER_SIZE);
	fr->header_size = VRGB_HEADER_SIZE;

	fr->curr_frame = -1;

	return (1);
}

static int
vrgb_fdopen_for_read(int fd, char *name, Vfile_type * fr)
{
fr->fd = fd;

	fr->image_header = (void *) malloc(VRGB_HEADER_SIZE);
	fr->header_size = VRGB_HEADER_SIZE;

	fr->curr_frame = -1;

	return (0);
}



static int
vrgb_read_header(Vfile_type * fr)
{
vrgb_header *gpsv;


	read(fr->fd, fr->image_header, VRGB_HEADER_SIZE);

	gpsv = (vrgb_header *) fr->image_header;

	fr->rows = gpsv->row;
	fr->columns = gpsv->column;
	fr->frame_no = gpsv->frame_no;


	fr->frame_size = fr->rows * fr->columns * 3;
	fr->image_comment = NULL;

	fr->curr_frame = 0;

	return (0);
}



static int
vrgb_open_for_write(char *dir, char *name, Vfile_type * fr)
{
char    file[100];

	vrgb_file_name(dir, name, file);

	fr->fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fr->fd == -1)	return (-1);

	fr->image_header = (void *) calloc(VRGB_HEADER_SIZE, 1);
	fr->header_size = VRGB_HEADER_SIZE;

	fr->curr_frame = -1;

	return (0);
}

static int
vrgb_fdopen_for_write(int fd, char *name, Vfile_type * fr)
{
	fr->fd = fd;

	fr->image_header = (void *) calloc(VRGB_HEADER_SIZE, 1);
	fr->header_size = VRGB_HEADER_SIZE;

	fr->curr_frame = -1;

	return (0);
}


static int
vrgb_write_header(Vfile_type * fr)
{
vrgb_header *h;

	h = (vrgb_header *) fr->image_header;

	strncpy( h->h, "VRGB", 4 );
	h->frame_no = fr->frame_no;
	h->column = fr->columns;
	h->row = fr->rows;

	memset( h->comments, '\0', 48 );
	if( fr->image_comment != NULL )
		strncpy( h->comments, fr->image_comment, 48 );


	write(fr->fd, fr->image_header, VRGB_HEADER_SIZE);

	fr->curr_frame = 0;

	return (0);
}



static int
vrgb_close(Vfile_type * fr)
{
	if (fr->fd > 2)	close(fr->fd);
	free( fr->image_header );

	fr->image_header = NULL;

	return (0);
}



static int
vrgb_frame_seek( Vfile_type *fr, int frame )
{
long    seek;

	if( fr->curr_frame == frame )	return( 0 );

	fr->curr_frame = frame;

	seek = ( fr->curr_frame == -1 )? 0 :
			VRGB_HEADER_SIZE + frame * fr->frame_size;

	if( lseek(fr->fd, seek, SEEK_SET ) != -1 ){
		fr->curr_frame = frame;
		return( 1 );
	}

	return( -1 );
}


static int
vrgb_read_next_rgb_mbmap( Vfile_type *fr, mbmap *r, mbmap *g, mbmap *b )
{
	vrgb_read_next( fr, r, g, b );

	return( 1 );
}



static int
vrgb_write_next_rgb_mbmap( Vfile_type *fr, mbmap *r, mbmap *g, mbmap *b )
{
	vrgb_write_next( fr, r, g, b );

	return( 1 );
}





void
vrgb_read_next( Vfile_type *fr, mbmap *r, mbmap *g, mbmap *b )
{
	MBMAP_RECREATE( *r, fr->rows, fr->columns);
	read(fr->fd, (char *) MBMAP_DATA_ADRESS(*r), MBMAP_DATA_SIZE(*r) );


	MBMAP_RECREATE( *g, fr->rows, fr->columns);
	read(fr->fd, (char *) MBMAP_DATA_ADRESS( *g), MBMAP_DATA_SIZE(*g));

	MBMAP_RECREATE( *b, fr->rows, fr->columns);
	read(fr->fd, (char *)MBMAP_DATA_ADRESS(*b), MBMAP_DATA_SIZE(*b));

	fr->curr_frame++;
}



void
vrgb_write_next( Vfile_type *fr, mbmap *r, mbmap *g, mbmap *b )
{
	write(fr->fd, (char *) MBMAP_DATA_ADRESS(*r), MBMAP_DATA_SIZE(*r) );

	write(fr->fd, (char *) MBMAP_DATA_ADRESS( *g), MBMAP_DATA_SIZE(*g));

	write(fr->fd, (char *)MBMAP_DATA_ADRESS(*b), MBMAP_DATA_SIZE(*b));

	fr->curr_frame++;
}
