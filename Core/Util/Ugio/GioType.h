/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/**************************
 ***	GioType.h	***
 **************************/

#ifndef		_GIO_TYPE_
#define		_GIO_TYPE_


#ifdef __cplusplus
extern "C" {
#endif


#include 	"Ucst/CstType.h"
#include 	"Uigp/igp.h"


#define	GIO_READ	1
#define	GIO_WRITE	2


typedef struct gio_type {

	FILE *fp;

	cst_type        *cst;

	int	flag;		// 1 - for read 2 for write

	int	bytes;

	int	Fdata;

} 	gio_type;




#define GIO_GETC( gio )  (( gio->fp != NULL )? \
                        fgetc(gio->fp) : CST_GETUC(gio->cst) )

#define GIO_PUTC( gio, c )  (( gio->fp != NULL )? \
                        fputc( c, gio->fp) : CST_PUTC(gio->cst, c) )


#define GIO_END( gio )  (( gio->fp != NULL )? feof(gio->fp) : CST_EOS( gio->cst ))



gio_type *	gio_alloc();

void	gio_close( gio_type *gio );



gio_type *  gio_open_file_to_read( const char *file );  

gio_type *	gio_open_file_to_write( const char *file );

gio_type *  gio_open_buffer_to_read( char *data, int byte_no, int Fdata );

gio_type *	gio_open_buffer_to_write( int byte_no );


gio_type *  imageIo_open_cst( cst_type *cst );



//#define	LITTLE_ENDIAN	0
//#define BIG_ENDIAN		1

void	gio_put_no( gio_type *gio, int a, int nByte, int Fendian );

int	gio_get_no( gio_type *gio, int nByte, int Fendian );

void	gio_put_noM( gio_type *gio, int a, int nByte );

int	gio_get_noM( gio_type *gio, int nByte );


void	gio_put_noL( gio_type *gio, int a, int nByte );

int	gio_get_noL( gio_type *gio, int nByte );


int	gio_get_buffer( gio_type *gio, u_char buf[], int byte_no );

int	gio_put_buffer( gio_type *gio, u_char buf[], int byte_no );

long	gio_ftell( gio_type *gio );

int	gio_seek( gio_type *gio, int pos );

int gio_skip( gio_type *gio, int pos );

long	gio_bytes( gio_type *gio );

char	gio_getc( gio_type *gio );

u_char	gio_getuc( gio_type *gio );



#ifdef __cplusplus
}
#endif

#endif 
