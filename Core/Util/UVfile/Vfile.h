#ifndef		_VFILE_H_
#define		_VFILE_H_

#include	"Umbmap/mbmap.h"


enum Fr_read_write {
	VFILE_READ, VFILE_WRITE
};


typedef struct Vfile_type Vfile_type;


typedef struct Vfile_op {
	char   *format_name;

	int	(*is_file_exist)( char *dir, char *name);

	int     (*open_for_read) (char *dir, char *name, Vfile_type *);

	int     (*open_for_write) (char *dir, char *name, Vfile_type *);

	int     (*fdopen_for_read) (int fd, char *name, Vfile_type *);

	int     (*fdopen_for_write) (int fd, char *name, Vfile_type *);

	int     (*close) (Vfile_type *);

	int     (*frame_seek) (Vfile_type *, int frame );

	int     (*read_header) (Vfile_type *);

	int     (*write_header) (Vfile_type *);

	int	(*read_next_rgb_mbmap)(Vfile_type *, mbmap *, mbmap *, mbmap *);

	int     (*write_next_rgb_mbmap)(Vfile_type *, mbmap *, mbmap *,mbmap *);
}       Vfile_op;



/*
 * Format dependent interface fonctions:
 * 
 * 
 * open_for_read - 
 *	1)physically open a file "dir/name[extention]" with a default
 *	extention of specific format as READ_ONLY  and set a fd
 *	(file descriptor) in Frame file structure

 *	2) leave file ready for reading file_header;
 *
 * 
 * open_for_write-
 *	1) physically open a file "dir/name[extention]" with a default
 *	extention of specific format as WRITE_ONLY and set a fd
 *	(file descriptor) in Frame file structure
 *	2) leave file ready for writing file_header;
 *	3) set fields: frame_size, header_size;
 * 
 * fdopen_for_read -
 *	1) set a fd (file descriptor) in Frame file structure
 *	2) leave file ready for reading file_header;
 * 
 * fdopen_for_write-
 *	1) set a fd (file descriptor) in Frame file structure
 *	2) leave file ready for writing file_header;
 *	3) set fields: frame_size, header_size;
 * 
 * read_header  - 
 *	1) read a file header,
 *	2) set fields: rows, columns, frame_number, frame_size,
 *			header_size, image_name, image_comment;
 *	3) leave file ready for reading the first frame
 * 
 * write_header - 
 *	1) write (or rewrite) a file header,
 *	2) leave file ready for writing the first frame
 */


struct Vfile_type {
	/* Private part */
	char	name[256];

	int     fd;
	int     read_write;	/* read or write */
	int	format;

	int     frame_no;
	int     curr_frame;

	int     rows, columns;

	int     frame_size,	header_size;
	char   *image_comment;


	/* Format dependent functions */
	void   *image_header;
	Vfile_op *operations;
};




/* User interface functions */

int	Vfile_is_file_exist(char *dir, char *name, int format );

int		Vfile_info(char *dir, char *name, int format,
				int *frame_no, int *rows, int *columns);

Vfile_type *	Vfile_open_for_read(char *dir, char *name, int format );


Vfile_type *	Vfile_fdopen_for_read(int fd, char *name, int format);


Vfile_type *	Vfile_open_for_write(char *dir, char *name, int format,
				int frames, int rows, int columns );

Vfile_type *	Vfile_fdopen_for_write(int fd, char *name, int format,
			 int frames, int rows, int columns );



int		Vfile_close( Vfile_type * );

int		Vfile_seek(Vfile_type *, int frame );


int	Vfile_read_frame(Vfile_type *file, mbmap *r, mbmap *g, mbmap *b );

int	Vfile_write_frame(Vfile_type *file, mbmap *r, mbmap *g, mbmap *b );

void    Vfile_set_image_name(Vfile_type *, char *name);




#define VFILE_ROWS(file)		((file)->rows)
#define VFILE_COLUMNS(file)		((file)->columns)
#define VFILE_IMAGE_NAME(file)		((file)->name)
#define VFILE_FORMAT(file)		((file)->operatione->format_name)
#define VFILE_FRAME_NUMBER(file)	((file)->frame_no)
#define VFILE_CURRENT_FRAME(file)	((file)->curr_frame)


#define		FORMAT_COST211	0
#define		FORMAT_SONY	1
#define		FORMAT_VRGB	2

#endif
