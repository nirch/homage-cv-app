#include	<stdio.h>
#include	<sys/types.h>
#include        <fcntl.h>


typedef struct COST211_HEADER	{
	char pic_name[16] ;
	char lines_per_field[4] ;
	char pixels_per_line[4] ;
	char lines_per_record[4] ;
	char num_frames[4] ;
	char nature_of_pic[2] ;
	char source[1] ;
	char field_rate[4] ;
	char interlace_ratio[1] ;
	char sampling_frequency[8] ;
	char h_sam_r1[1] ;
	char h_sam_r2[1] ;
	char h_sam_r3[1] ;
	char h_sam_r4[1] ;

	char rest[668] ;
} cost211_header;



#define		COST211_HEADER_SIZE	sizeof( cost211_header )

#define		COST211_ROW( h )	atoi( h->lines_per_field )
#define		COST211_COLUMN( h )	atoi( h->pixels_per_line )
#define		COST211_FRAME_NO( h )	atoi( h->num_frames )


#define		COST211_SET_ROW( h, row ) \
				sprintf( h->lines_per_field, "%d", row )

#define		COST211_SET_COLUMN( h, column )\
				sprintf( h->pixels_per_line, "%d", column )

#define		COST211_SET_FRAME_NO( h, frame_no )\
				sprintf( h->num_frames,"%d", frame_no )

#define		COST211_SET_IMAGE_NAME( h, name )\
				strncpy( (h)->pic_name, name, 16 );





/*
static int	   cost211_open_for_read(char *dir, char *name,
							Frame_file *fr);

static int	   cost211_read_header(Frame_file *fr);

static int	   cost211_open_for_write(char *dir, char *name,
							Frame_file *fr);

static int	   cost211_write_header(Frame_file *fr);

static int	   cost211_close(Frame_file *fr);

static int	   cost211_frame_seek(Frame_file *file, int offset,int whence);


static Frame_image *cost211_read_frame(Frame_file *file);

static int	   cost211_write_frame(Frame_file *file, Frame_image *image);





static Fr_file_op  Sony = {
  "COST211",
  cost211_open_for_read,
  cost211_open_for_write,
  cost211_close,
  cost211_frame_seek,
  cost211_read_header,
  cost211_write_header,
  cost211_read_frame,
  cost211_write_frame
} ;


Fr_file_op *COST211 = &Cost211;

*/
