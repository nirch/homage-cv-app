#ifndef         _MBMAP_

#define         _MBMAP_

#include        <stdio.h>
#include        <memory.h>
#include        <malloc.h>

#include        "Uigp/igp.h"

#include        "Uimage/ImageType.h"

#include        "im_hdr.h"
struct	MBMAP {

	int	column;
	int	row;
	u_char	*data;
};

typedef		struct MBMAP 	mbmap;


#define		MBMAP_ROW( bmap )	((bmap).row)
#define		MBMAP_COLUMN( bmap )	((bmap).column)
#define		MBMAP_DATA_ADRESS( bmap )	((bmap).data )


#define		MBMAP_DATA_SIZE( bmap )	\
			( (long)MBMAP_ROW( bmap )* \
			  (long)MBMAP_COLUMN( bmap ) )



#define		MBMAP_PIXEL( bmap, row, column )	\
			*( (bmap).data + (row)* MBMAP_COLUMN( bmap ) + column )


#define		MBMAP_PIXEL_ADRESS( bmap, row, column )	\
			( (bmap).data + (row)* MBMAP_COLUMN( bmap ) + column )


#define		MBMAP_INIT( bmap )	\
		{ \
		MBMAP_ROW( bmap ) = -1;	\
		MBMAP_COLUMN( bmap ) = -1;	\
		(bmap).data = NULL; \
		};


#define		MBMAP_CREATE( bmap, row, column )	\
		{ \
		MBMAP_ROW( bmap ) = (int)row;	\
		MBMAP_COLUMN( bmap ) = (int)column;	\
		(bmap).data =(u_char *)malloc( (row)* (column) ); \
		};


#define		MBMAP_RECREATE( bmap, rows, columns )	\
		{ \
			if( (bmap).row != rows || (bmap).column != columns){ \
				if( (bmap).data != NULL ) \
					MBMAP_DESTROY( (bmap) ); \
				MBMAP_CREATE( (bmap), rows, columns ); \
			} \
		};
				


#define		MBMAP_DESTROY( bmap )	\
			{ \
			free( (bmap).data );	\
			(bmap).data = NULL;	\
			MBMAP_ROW( bmap ) = -1;	\
			MBMAP_COLUMN( bmap ) = 0; \
			};


	/* rgb_yqa.c */
void	mbmap_yqa_to_rgb( mbmap y, mbmap q, mbmap a,
					mbmap *r, mbmap *g, mbmap *b );
void	mbmap_rgb_to_yqa( mbmap r, mbmap g, mbmap b,
					mbmap *y, mbmap *q, mbmap *a );
void	mbmap_cv_rgb_to_yqa(mbmap *r, mbmap *g, mbmap *b,
					mbmap *y, mbmap *q, mbmap *a );

mbmap	yrgb_to_q( mbmap kbmap, mbmap rbmap);
mbmap	yrgb_to_a( mbmap kbmap, mbmap bbmap);
mbmap	yqa_to_r( mbmap kbmap, mbmap qbmap);
mbmap	yqa_to_b( mbmap kbmap, mbmap abmap);
mbmap	yqa_to_g( mbmap kbmap, mbmap qbmap, mbmap abmap);

mbmap	rgb_to_y( mbmap rbmap, mbmap gbmap, mbmap bbmap);
mbmap	rgb_to_q( mbmap rbmap, mbmap gbmap, mbmap bbmap);
mbmap	rgb_to_a( mbmap rbmap, mbmap gbmap, mbmap bbmap);

	/* chen_mbm.c */
mbmap	mbmap_neg( mbmap bmap);
mbmap	mbmap_row_inverse( mbmap bmap);
mbmap	mbmap_column_inverse( mbmap bmap);
mbmap	mbmap_y_filter( mbmap bmap);
mbmap	mbmap_a_q_filter( mbmap bmap);


	/*	MbmapFile.c	*/

void	mbmap_gp_bmap_file_name(char *dir, char *image, char *color,char *file);

int	mbmap_is_gp_bmap_file_exist(char *dir,char *image_name,char *color);
int	mbmap_is_gp_rgb_bmap_exist(char *dir, char *image_name );

void	mbmap_gp_bmap_file_size(char *dir, char *image, char *color,
							int *row,int *column);

void	mbmap_get_im_header_from_gp_bmap_fd( int fd, struct IM_HEADER * im_h );
void	mbmap_put_im_header_into_gp_bmap_fd( int fd, struct IM_HEADER * im_h );

void	mbmap_dump_to_gp_bmap_file( char *dir_name, char *image_name,
				       char *color,  mbmap bmap );
 
int	mbmap_open_to_read_gp_bmap_file( char *dir, char *image, char *color,
                        	int *row, int *column );

int	mbmap_open_to_write_gp_bmap_file( char *dir, char *image, char *color,
                        	int row, int column );

int	mbmap_open_gp_bmap_file(char *dir_name,char *image_name,
				    char *color, struct  IM_HEADER  *im_h  );

int	mbmap_create_gp_bmap_file(char *dir_name,char *image_name,
				     char *color, long row, long col );

void	mbmap_append_mbmap_to_gp_bmap_file( int fd, mbmap  *bmap );

void	mbmap_dump_to_open_gp_bmap_file(int fd,char *image_name, mbmap bmap);

mbmap	mbmap_load_from_gp_bmap_file( char *dir, char *image,char *color );

void	mbmap_load_mbmap_with_gp_bmap_file_segment( int fd, int row0, int col0,
					    mbmap  * bmap );

mbmap	mbmap_load_from_gp_sub_bmap_file(char *dir_name,char *image_name,
			char *color,int x0,int y0,int row,int column);


	/* mbmap.c */

mbmap	mbmap_copy_sub_mbmap( mbmap bmap,int x0,int y0,int row,int column);

void	mbmap_const( mbmap bmap, u_char val );

mbmap	mbmap_dup_mbmap( mbmap bmap, int dup);

mbmap	mbmap_duprc_mbmap( mbmap bmap, int row_dup, int col_dup );

mbmap	mbmap_sampling( mbmap bmap, int dup);

mbmap	mbmap_rc_sampling(mbmap bmap, int row_dup, int col_dup);

mbmap	mbmap_rc_sampling_new(mbmap bmap,
			int r0, int c0, int rows, int columns,
			int row_dup, int col_dup);

mbmap	mbmap_22_sampling( mbmap bmap );

mbmap	mbmap_subtract( mbmap bmap1, mbmap bmap2 );

mbmap	mbmap_mask( mbmap bmap, int mask );

mbmap	mbmap_affine_sum( mbmap bmap1, mbmap bmap2,
					double a0, double a1, double a2 );

void	mbmap_minmax( mbmap bmap, int *min, int *max );

mbmap	mbmap_stretch( mbmap bmap, double a, double b );

void	mbmap_copy_sub_mbmap_to_uarray( mbmap bmap,
				int x0,int y0,int row,int column,
				u_char *arry,int acolumn);

void    mbmap_ecopy_sub_mbmap_to_uarray(mbmap bmap, 
                        int x0,int y0,int row,int column, 
                        u_char *arry,int acolumn);


void	mbmap_copy_uarry_to_sub_mbmap( u_char *arry,int acolumn,
				mbmap bmap,int x0,int y0,int row,int column);

void	mbmap_put_mbmap_in_mbmap( mbmap sbmap, mbmap tbmap,int row,int column);


	/* co_mbmap.c */

void	mbmap_cv_yqa_to_rgb( mbmap y, mbmap q, mbmap a,
					mbmap *r, mbmap *g, mbmap *b);

void	mbmap_cv_yqa1_to_rgb( mbmap y, mbmap q, mbmap a,
					mbmap *r, mbmap *g, mbmap *b);

void	mbmap_print_ascii_bmap( mbmap bmap, FILE *fp,int n );

void	mbmap_frame_to_fileds( mbmap bmap, mbmap *f1, mbmap *f2);

void	mbmap_fileds_to_frame( mbmap f1, mbmap f2, mbmap *frame );

void	mbmap_copy_to_exist_mbmap( mbmap s, mbmap t );

void	mbmap_copy_sub_mbmap_to_farray( mbmap bmap,
					int x0,int y0,int row,int column,
					float *arry,int acolumn);

void	mbmap_copy_farry_to_sub_mbmap(float *arry,int acolumn,
				mbmap bmap,int x0,int y0,int row,int column);


mbmap	mbmap_create_vertical_barred_bmap( int rows, int cols, 
			  int fg_gval, int bg_gval, 
			  int bar_type_no, int *bar_width, int *bar_quant );

mbmap	mbmap_create_horizontal_barred_bmap( int rows, int cols, 
			  int fg_gval, int bg_gval, 
			  int bar_type_no, int *bar_width, int *bar_quant );

mbmap	mbmap_transpose( mbmap bmap );



	/* PixelDup.c */
void	mbmap_dcopy_sub_mbmap_to_uarray( mbmap bmap,
                        int x0, int y0, int row, int column,
                        u_char *arry, int acolumn );



	/* MbRgbYCrCb.c */
void	mbmap_Rgb_YCrCb_init ();

void	mbmap_Rgb_YCrCb( mbmap R, mbmap G, mbmap B, int row, int col,
                        mbmap *Y, mbmap *Cr, mbmap *Cb, int alloc_flag );

void	mbmap_Rgb_YCrCb411( mbmap R, mbmap G, mbmap B, int row, int col,
                        mbmap *Y, mbmap *Cr, mbmap *Cb, int alloc_flag );



	/* MbYCrCbRgb.c */

void	mbmap_YCrCb_Rgb_init();
void	mbmap_YCrCb_Rgb( mbmap Y, mbmap Cr, mbmap Cb, int row, int col,
                        mbmap *R, mbmap *G, mbmap *B, int alloc_flag );

void	mbmap_YCrCb411_Rgb( mbmap Y, mbmap Cr, mbmap Cb, int row, int col,
                        mbmap *R, mbmap *G, mbmap *B, int alloc_flag );


	/* Convol.c */

void	mbmap_convol_3x3( mbmap bmap, float Matr_conv[3][3],
		int row, int col, mbmap *bmap_conv, int alloc_flag );

void	mbmap_convolution( mbmap bmap, float *Matr_conv, int no,
                int row, int col, mbmap *bmap_conv, int alloc_flag );



	/* Compare.c */

float	mbmap_variability( mbmap bmap1, mbmap bmap2,
					int x0,int y0, int row, int column );
float	mbmap_average( mbmap bmap1, mbmap bmap2,
					int x0,int y0,int row,int column );
 
	/* Row.c */

void	mbmap_dump_rgb_to_row_line( int fd, mbmap r, mbmap g, mbmap b );

void	mbmap_dump_rgb_to_row_pixel( int fd, mbmap r, mbmap g, mbmap b );

	/* Targa.c */
void	dump_rgb_mbmap_to_targa_file( char *dir, char *image_name,
                                mbmap r, mbmap g, mbmap b );

void	dump_mbmap_to_targa_file8( char *dir, char *image_name, mbmap bmap );

void	load_rgb_mbmap_to_targa_file( char *dir, char *image_name,
                                mbmap *r, mbmap *g, mbmap *b );


	/* MbmapWriteBmp.c */
int	mbmap_dump_rgb_to_bmp_file( char *dir, char *name,
				mbmap *r, mbmap *g, mbmap *b );


	/* Mbmap2Image.c */
int	mbmap_from_image( image_type *im, mbmap bmap[], int flag );

image_type *	mbmap_to_image( mbmap bmap[] );

#endif   /* _MBMAP_ */
