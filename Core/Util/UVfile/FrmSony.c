#include	<stdio.h>
#include	<sys/types.h>
#include        <fcntl.h>
#include        <malloc.h>

#include	"Vfile.h"


typedef struct SONY_HEADER {
	char    h[4];		/* "GPSV"	 */
	int     frame_no;
	int     column;
	int     row;
	char    image_name[20];
	char    comments[28];
}       sony_header;


#define		SONY_HEADER_SIZE	\
			sizeof( sony_header )

#define	FILL_SONY_HEADER( gpsv, f_no, c, r, name, comm )	\
	strncpy( (gpsv).h, "GPSV", 4 ); \
	(gpsv).frame_no = f_no; \
	(gpsv).column = c; \
	(gpsv).row = r; \
	if( name != NULL )	strncpy( (gpsv).image_name, name, 20 ); \
	if( comm != NULL )	strncpy( (gpsv).comments, comm, 28 );


#define	IS_VAILD_GPSV( gpsv )	 ( !strcmp( gpsv.h, "GPSV", 4 ) )

static int      sony_is_file_exist(char *dir, char *name );

static int	sony_open_for_read(char *dir, char *name, Vfile_type * fr);

static int	sony_fdopen_for_read(int fd, char *name, Vfile_type * fr);

static int	sony_read_header(Vfile_type * fr);

static int 	sony_open_for_write(char *dir, char *name, Vfile_type * fr);

static int	sony_fdopen_for_write(int fd, char *name, Vfile_type * fr);

static int	sony_write_header(Vfile_type * fr);

static int	sony_close(Vfile_type * fr);

static int	sony_frame_seek(Vfile_type * file, int frame );


static int	sony_read_next_rgb_mbmap( Vfile_type *fr,
					mbmap *r, mbmap *g, mbmap *b );

static int	sony_write_next_rgb_mbmap( Vfile_type *fr,
					mbmap *r, mbmap *g, mbmap *b );

void		sony_read_next( Vfile_type *fr, mbmap *y, mbmap *q, mbmap *a );
void		sony_write_next( Vfile_type *fr, mbmap *y, mbmap *q, mbmap *a );





Vfile_op Sony = {
	"SONY",
	sony_is_file_exist,
	sony_open_for_read,
	sony_open_for_write,
	sony_fdopen_for_read,
	sony_fdopen_for_write,
	sony_close,
	sony_frame_seek,
	sony_read_header,
	sony_write_header,
	sony_read_next_rgb_mbmap,
	sony_write_next_rgb_mbmap
};



/*
 * Bodies of all that functions
 */


void
sony_file_name(char *dir_name, char *image_name, char *file)
{
	if (dir_name != NULL)
		sprintf(file, "%s/%s.sony", dir_name, image_name);
	else
		sprintf(file, "%s.sony", image_name);
}


static int
sony_is_file_exist(char *dir, char *name )
{
char	file[256];

	sony_file_name(dir, name, file);

	return( gp_is_file_exist( file ) );
}




static int
sony_open_for_read(char *dir, char *name, Vfile_type * fr)
{
char    file[100];

	sony_file_name(dir, name, file);

	fr->fd = open(file, O_RDONLY);
	if (fr->fd == -1)	return (-1);

	fr->image_header = (void *) malloc(SONY_HEADER_SIZE);
	fr->header_size = SONY_HEADER_SIZE;

	fr->curr_frame = -1;

	return( 1 );
}

static int
sony_fdopen_for_read(int fd, char *name, Vfile_type * fr)
{
fr->fd = fd;

	fr->image_header = (void *) malloc(SONY_HEADER_SIZE);
	fr->header_size = SONY_HEADER_SIZE;

	fr->curr_frame = -1;

	return (0);
}



static int
sony_read_header(Vfile_type * fr)
{
sony_header *gpsv;


	read(fr->fd, fr->image_header, SONY_HEADER_SIZE);

	gpsv = (sony_header *) fr->image_header;

	fr->rows = gpsv->row;
	fr->columns = gpsv->column;
	fr->frame_no = gpsv->frame_no;


	fr->frame_size = fr->rows * fr->columns * 1.5;
	fr->image_comment = NULL;

	fr->curr_frame = 0;

	return (0);
}



static int
sony_open_for_write(char *dir, char *name, Vfile_type * fr)
{
char    file[100];

	sony_file_name(dir, name, file);

	fr->fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fr->fd == -1)	return (-1);

	fr->image_header = (void *) malloc(SONY_HEADER_SIZE);
	fr->header_size = SONY_HEADER_SIZE;

	fr->curr_frame = -1;

	return (0);
}

static int
sony_fdopen_for_write(int fd, char *name, Vfile_type * fr)
{
	fr->fd = fd;

	fr->image_header = (void *) malloc(SONY_HEADER_SIZE);
	fr->header_size = SONY_HEADER_SIZE;

	fr->curr_frame = -1;

	return (0);
}


static int
sony_write_header(Vfile_type * fr)
{
sony_header *gpsv;

	gpsv = (sony_header *) fr->image_header;

	FILL_SONY_HEADER(*gpsv, fr->frame_no,
			 fr->columns,
			 fr->rows,
			 fr->name,
			 fr->image_comment);

	write(fr->fd, fr->image_header, SONY_HEADER_SIZE);

	fr->curr_frame = 0;

	return (0);
}



static int
sony_close(Vfile_type * fr)
{
	if (fr->fd > 2)	close(fr->fd);
	free( fr->image_header );

	fr->image_header = NULL;

	return (0);
}



static int
sony_frame_seek( Vfile_type *fr, int frame )
{
long    seek;

	if( fr->curr_frame == frame )	return( 0 );

	fr->curr_frame = frame;

	seek = ( fr->curr_frame == -1 )? 0 :
			SONY_HEADER_SIZE + frame * fr->frame_size;

	if( lseek(fr->fd, seek, SEEK_SET ) != -1 ){
		fr->curr_frame = frame;
		return( 1 );
	}

	return( -1 );
}


static int
sony_read_next_rgb_mbmap( Vfile_type *fr, mbmap *r, mbmap *g, mbmap *b )
{
mbmap	y,	q,	a;
	sony_read_next( fr, &y, &q, &a );

	mbmap_cv_yqa_to_rgb( y, q, a, r, g, b );

	MBMAP_DESTROY( y );
	MBMAP_DESTROY( q );
	MBMAP_DESTROY( a );

	return( 1 );
}



static int
sony_write_next_rgb_mbmap( Vfile_type *fr, mbmap *r, mbmap *g, mbmap *b )
{
mbmap	y,	q,	a;
mbmap	q1,	a1;

	mbmap_cv_rgb_to_yqa( r, g, b, &y, &q1, &a1 );
	q = mbmap_22_sampling( q1 );
	a = mbmap_22_sampling( a1 );

	MBMAP_DESTROY( q1 );
	MBMAP_DESTROY( q1 );

	sony_write_next( fr, &y, &q, &a );

	MBMAP_DESTROY( y );
	MBMAP_DESTROY( q );
	MBMAP_DESTROY( a );

	return( 1 );
}





void
sony_read_next( Vfile_type *fr, mbmap *y, mbmap *q, mbmap *a )
{
	MBMAP_CREATE( *y, fr->rows, fr->columns);
	read(fr->fd, (char *) MBMAP_DATA_ADRESS(*y), MBMAP_DATA_SIZE(*y) );


	MBMAP_CREATE( *q, fr->rows / 2, fr->columns / 2);
	read(fr->fd, (char *) MBMAP_DATA_ADRESS( *q), MBMAP_DATA_SIZE(*q));

	MBMAP_CREATE( *a, fr->rows / 2, fr->columns / 2);
	read(fr->fd, (char *)MBMAP_DATA_ADRESS(*a), MBMAP_DATA_SIZE(*a));

	fr->curr_frame++;
}



void
sony_write_next( Vfile_type *fr, mbmap *y, mbmap *q, mbmap *a )
{
	write(fr->fd, (char *) MBMAP_DATA_ADRESS(*y), MBMAP_DATA_SIZE(*y) );


	write(fr->fd, (char *) MBMAP_DATA_ADRESS( *q), MBMAP_DATA_SIZE(*q));

	write(fr->fd, (char *)MBMAP_DATA_ADRESS(*a), MBMAP_DATA_SIZE(*a));

	fr->curr_frame++;
}
