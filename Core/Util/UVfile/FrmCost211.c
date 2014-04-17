#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include        <fcntl.h>
#include        <malloc.h>

#include	"Vfile.h"

#include        "cost211.h"


static int	cost211_is_file_exist(char *dir, char *name );

static int	cost211_open_for_read(char *dir, char *name, Vfile_type * fr);

static int	cost211_fdopen_for_read(int fd, char *name, Vfile_type * fr);

static int	cost211_read_header(Vfile_type * fr);

static int 	cost211_open_for_write(char *dir, char *name, Vfile_type * fr);

static int	cost211_fdopen_for_write(int fd, char *name, Vfile_type * fr);

static int	cost211_write_header(Vfile_type * fr);

static int	cost211_close(Vfile_type * fr);

static int	cost211_frame_seek(Vfile_type * file, int frame );


static int	cost211_read_next_rgb_mbmap( Vfile_type *fr,
					mbmap *r, mbmap *g, mbmap *b );

static int	cost211_write_next_rgb_mbmap( Vfile_type *fr,
					mbmap *r, mbmap *g, mbmap *b );

void	cost211_read_next( Vfile_type *fr,
                mbmap *Ye, mbmap *Qe, mbmap *Ae,
                mbmap *Yo, mbmap *Qo, mbmap *Ao );

void	cost211_write_next( Vfile_type *fr,
                mbmap *Ye, mbmap *Qe, mbmap *Ae,
                mbmap *Yo, mbmap *Qo, mbmap *Ao );




Vfile_op Cost211 = {
	"SONY",
	cost211_is_file_exist,
	cost211_open_for_read,
	cost211_open_for_write,
	cost211_fdopen_for_read,
	cost211_fdopen_for_write,
	cost211_close,
	cost211_frame_seek,
	cost211_read_header,
	cost211_write_header,
	cost211_read_next_rgb_mbmap,
	cost211_write_next_rgb_mbmap
};



/*
 * Bodies of all that functions
 */


void
cost211_file_name(char *dir_name, char *image_name, char *file)
{
	if (dir_name != NULL)
		sprintf(file, "%s/%s.cost211", dir_name, image_name);
	else
		sprintf(file, "%s.cost211", image_name);
}


static int 
cost211_is_file_exist(char *dir, char *name ) 
{ 
char	file[256];
 
	cost211_file_name(dir, name, file);
 
	return( gp_is_file_exist( file ) );
}



static int
cost211_open_for_read(char *dir, char *name, Vfile_type * fr)
{
char    file[100];

	cost211_file_name(dir, name, file);

	fr->fd = open(file, O_RDONLY);
	if (fr->fd == -1)	return (-1);

	fr->image_header = (void *) malloc(COST211_HEADER_SIZE);
	fr->header_size = COST211_HEADER_SIZE;

	fr->curr_frame = -1;

	return (1);
}

static int
cost211_fdopen_for_read(int fd, char *name, Vfile_type * fr)
{
fr->fd = fd;

	fr->image_header = (void *) malloc(COST211_HEADER_SIZE);
	fr->header_size = COST211_HEADER_SIZE;

	fr->curr_frame = -1;

	return (0);
}



static int
cost211_read_header(Vfile_type * fr)
{
cost211_header *h;
 
 
        read( fr->fd, fr->image_header, COST211_HEADER_SIZE );
 
        h = (cost211_header *)fr->image_header;
 
        fr->rows = 2*COST211_ROW( h );
        fr->columns = COST211_COLUMN( h );
        fr->frame_no = COST211_FRAME_NO( h );
 
 
        fr->frame_size = fr->rows*fr->columns* 2;

	fr->curr_frame = 0;

	return (0);
}



static int
cost211_open_for_write(char *dir, char *name, Vfile_type * fr)
{
char    file[100];

	cost211_file_name(dir, name, file);

	fr->fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fr->fd == -1)	return (-1);

	fr->image_header = (void *) malloc(COST211_HEADER_SIZE);
	fr->header_size = COST211_HEADER_SIZE;

	fr->curr_frame = -1;

	return (0);
}

static int
cost211_fdopen_for_write(int fd, char *name, Vfile_type * fr)
{
	fr->fd = fd;

	fr->image_header = (void *) malloc(COST211_HEADER_SIZE);
	fr->header_size = COST211_HEADER_SIZE;

	fr->curr_frame = -1;

	return (0);
}


static int
cost211_write_header(Vfile_type * fr)
{
cost211_header *h;

        h = (cost211_header *)fr->image_header;

        COST211_SET_ROW( h, fr->rows/2 );
        COST211_SET_COLUMN( h, fr->columns );
        COST211_SET_FRAME_NO( h, fr->frame_no );

        write( fr->fd, fr->image_header, COST211_HEADER_SIZE );


	fr->curr_frame = 0;

	return (0);
}



static int
cost211_close(Vfile_type * fr)
{
	if (fr->fd > 2)	close(fr->fd);
	free( fr->image_header );

	fr->image_header = NULL;

	return (0);
}



static int
cost211_frame_seek( Vfile_type *fr, int frame )
{
long    seek;

	if( fr->curr_frame == frame )	return( 0 );

	fr->curr_frame = frame;

	seek = ( fr->curr_frame == -1 )? 0 :
			COST211_HEADER_SIZE + frame * fr->frame_size;

	if( lseek(fr->fd, seek, SEEK_SET ) != -1 ){
		fr->curr_frame = frame;
		return( 1 );
	}

	return( -1 );
}


static int
cost211_read_next_rgb_mbmap( Vfile_type *fr, mbmap *r, mbmap *g, mbmap *b )
{
mbmap	ye,	qe,	ae,	yo,     qo,     ao;
mbmap	y,	q,	a;

	cost211_read_next( fr, &ye, &qe, &ae, &yo, &qo, &ao );

	mbmap_fileds_to_frame( ye, yo, &y );

        mbmap_fileds_to_frame( qe, qo, &q );

        mbmap_fileds_to_frame( ae, ao, &a );

	MBMAP_DESTROY( ye );
	MBMAP_DESTROY( qe );
	MBMAP_DESTROY( ae );

	MBMAP_DESTROY( yo );
	MBMAP_DESTROY( qo );
	MBMAP_DESTROY( ao );

	mbmap_cv_yqa1_to_rgb( y, q, a, r, g, b );

	MBMAP_DESTROY( y );
	MBMAP_DESTROY( q );
	MBMAP_DESTROY( a );

	return( 1 );
}



static int
cost211_write_next_rgb_mbmap( Vfile_type *fr, mbmap *r, mbmap *g, mbmap *b )
{
mbmap	ye,	qe,	ae,	yo,     qo,     ao;
mbmap	y,	q,	a;
mbmap	q1,	a1;

	mbmap_cv_rgb_to_yqa( r, g, b, &y, &q1, &a1 );
	q = mbmap_rc_sampling( q1, 1, 2 );
	a = mbmap_rc_sampling( a1, 1, 2 );

	MBMAP_DESTROY( q1 );
	MBMAP_DESTROY( q1 );

	mbmap_frame_to_fileds( y, &ye, &yo );
	mbmap_frame_to_fileds( q, &qe, &qo );
	mbmap_frame_to_fileds( a, &ae, &ao );

	MBMAP_DESTROY( y );
	MBMAP_DESTROY( q );
	MBMAP_DESTROY( a );


	cost211_write_next( fr, &ye, &qe, &ae, &yo, &qo, &ao );


	MBMAP_DESTROY( ye );
	MBMAP_DESTROY( qe );
	MBMAP_DESTROY( ae );

	MBMAP_DESTROY( yo );
	MBMAP_DESTROY( qo );


	return( 1 );
}





void
cost211_read_next( Vfile_type *fr,
		mbmap *Ye, mbmap *Qe, mbmap *Ae,
		mbmap *Yo, mbmap *Qo, mbmap *Ao )
{
	MBMAP_CREATE( *Ye, fr->rows/2, fr->columns);
	read(fr->fd, (char *) MBMAP_DATA_ADRESS(*Ye), MBMAP_DATA_SIZE(*Ye) );

	MBMAP_CREATE( *Qe, fr->rows / 2, fr->columns / 2);
	read(fr->fd, (char *) MBMAP_DATA_ADRESS( *Qe), MBMAP_DATA_SIZE(*Qe));

	MBMAP_CREATE( *Ae, fr->rows / 2, fr->columns / 2);
	read(fr->fd, (char *)MBMAP_DATA_ADRESS(*Ae), MBMAP_DATA_SIZE(*Ae));


	MBMAP_CREATE( *Yo, fr->rows/2, fr->columns);
	read(fr->fd, (char *) MBMAP_DATA_ADRESS(*Yo), MBMAP_DATA_SIZE(*Yo) );

	MBMAP_CREATE( *Qo, fr->rows / 2, fr->columns / 2);
	read(fr->fd, (char *) MBMAP_DATA_ADRESS( *Qo), MBMAP_DATA_SIZE(*Qo));

	MBMAP_CREATE( *Ao, fr->rows / 2, fr->columns / 2);
	read(fr->fd, (char *)MBMAP_DATA_ADRESS(*Ao), MBMAP_DATA_SIZE(*Ao));

	fr->curr_frame++;
}


void
cost211_write_next( Vfile_type *fr,
		mbmap *Ye, mbmap *Qe, mbmap *Ae,
		mbmap *Yo, mbmap *Qo, mbmap *Ao )
{
	write(fr->fd, (char *) MBMAP_DATA_ADRESS(*Ye), MBMAP_DATA_SIZE(*Ye) );

	write(fr->fd, (char *) MBMAP_DATA_ADRESS( *Qe), MBMAP_DATA_SIZE(*Qe));

	write(fr->fd, (char *)MBMAP_DATA_ADRESS(*Ae), MBMAP_DATA_SIZE(*Ae));


	write(fr->fd, (char *) MBMAP_DATA_ADRESS(*Yo), MBMAP_DATA_SIZE(*Yo) );

	write(fr->fd, (char *) MBMAP_DATA_ADRESS( *Qo), MBMAP_DATA_SIZE(*Qo));

	write(fr->fd, (char *)MBMAP_DATA_ADRESS(*Ao), MBMAP_DATA_SIZE(*Ao));

	fr->curr_frame++;
}
