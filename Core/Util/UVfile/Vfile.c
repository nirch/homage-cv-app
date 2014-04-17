#include	<stdio.h>
#include	<malloc.h>
#include	<memory.h>


#include	"Vfile.h"

extern Vfile_op Sony;
extern Vfile_op Cost211;
extern Vfile_op	Vrgb;


static Vfile_op * Vfile_op_get( int format );


int 
Vfile_is_file_exist(char *dir, char *name, int format )
{
Vfile_op	*operations;
int	ret;

	operations = Vfile_op_get( format );
	ret = (*operations->is_file_exist) (dir, name );

	return( ret );
}


int 
Vfile_info(char *dir, char *name, int format,
	     int *frame_no, int *rows, int *columns)
{
Vfile_type *file;

	file = (Vfile_type *) calloc(1, sizeof(Vfile_type));

	file->format = format;
	file->read_write = VFILE_READ;

	file->operations = Vfile_op_get( format );
	sprintf( file->name, "%s", name );

	if ((*file->operations->open_for_read) (dir, name, file) < 0)
		return (-1);

	(*file->operations->read_header) (file);


	*frame_no = VFILE_FRAME_NUMBER(file);
	*rows = VFILE_ROWS(file);
	*columns = VFILE_COLUMNS(file);

	Vfile_close(file);

	return (1);
}


Vfile_type *
Vfile_open_for_read(char *dir, char *name, int format )
{
Vfile_type *file;

	file = (Vfile_type *) calloc(1, sizeof(Vfile_type));

	file->format = format;
	file->read_write = VFILE_READ;

	file->operations = Vfile_op_get( format );
	sprintf( file->name, "%s", name );

	if ((*file->operations->open_for_read) (dir, name, file) < 0)
		error("Vfile_open_for_read", "Can't open file");

	(*file->operations->read_header) (file);

	file->curr_frame = 0;

	return (file);
}


Vfile_type *
Vfile_open_for_write(char *dir, char *name, int format,
		       int frames, int rows, int columns)
{
Vfile_type *file;

	file = (Vfile_type *) calloc(1, sizeof(Vfile_type));

	file->read_write = VFILE_WRITE;

	file->format = format;
	file->operations = Vfile_op_get( format );
	sprintf( file->name, "%s", name );

	if ((*file->operations->open_for_write) (dir, name, file) < 0)
		error("Vfile_open_for_write", "Can't open file");

	file->rows = rows;
	file->columns = columns;
	file->frame_no = frames;

	(*file->operations->write_header) (file);

	file->curr_frame = 0;

	return (file);
}


Vfile_type *
Vfile_fdopen_for_read(int fd, char *name, int format)
{
	Vfile_type *file;

	file = (Vfile_type *) calloc(1, sizeof(Vfile_type));

	file->read_write = VFILE_READ;

	file->format = format;
	file->operations = Vfile_op_get( format );

	sprintf( file->name, "%s", name );

	if (file->operations->fdopen_for_read == NULL ||
	    (*file->operations->fdopen_for_read) (fd, name, file) < 0)
		error("Fr_file_fdopen_for_read", "Can't open file");

	(*file->operations->read_header) (file);

	file->curr_frame = 0;
	return (file);
}


Vfile_type *
Vfile_fdopen_for_write(int fd, char *name, int format,
			 int frames, int rows, int columns )
{
	Vfile_type *file;

	file = (Vfile_type *) calloc(1, sizeof(Vfile_type));

	file->read_write = VFILE_WRITE;

	file->format = format;
	file->operations = Vfile_op_get( format );

	sprintf( file->name, "%s", name );

	if(file->operations->fdopen_for_write == NULL ||
	    (*file->operations->fdopen_for_write) (fd, name, file) < 0)
		error("Fr_file_fdopen_for_write", "Can't open file");

	file->rows = rows;
	file->columns = columns;

	file->frame_no = frames;

	(*file->operations->write_header) (file);

	file->curr_frame = 0;

	return (file);
}

int 
Vfile_close(Vfile_type * file)
{
	if (file->read_write == VFILE_WRITE && file->fd > 2) {

		/* Updating of file_header */
		Vfile_seek( file, -1 );

		(*file->operations->write_header) (file);
	}

	if ((*file->operations->close) (file) != 0)
		error("Vfile_close", "Can't close file");


/*
	if (file->image_header != NULL)
		free(file->image_header);
*/

	free(file);

	return (0);
}


int 
Vfile_seek(Vfile_type * file, int frame )
{
int     result;

	result = (*file->operations->frame_seek) (file, frame );

	return( result );
}


int
Vfile_read_frame(Vfile_type *file, mbmap *r, mbmap *g, mbmap *b )
{
int	ret;

	ret = (*file->operations->read_next_rgb_mbmap)(file, r, g, b );
	return( ret );
}


int 
Vfile_write_frame(Vfile_type *file, mbmap *r, mbmap *g, mbmap *b )
{
int      ret ;

	ret = (*file->operations->write_next_rgb_mbmap)(file, r, g, b );

	return( ret );
}


void 
Vfile_set_image_name(Vfile_type * file, char *name)
{
	sprintf( file->name, "%s", name );
}


Vfile_op *VGP;

static Vfile_op *
Vfile_op_get( int format )
{
	switch( format ){
	case FORMAT_COST211:
		return( &Cost211 );

	case FORMAT_SONY:
		return( &Sony );

	case FORMAT_VRGB:
		return( &Vrgb );
	}
}
