/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/******************
 ***	igp.h	***
 ******************/

#ifndef         _GP_UTIL_
#define         _GP_UTIL_

#ifdef __cplusplus
extern "C" {
#endif

#include  <stdio.h>


#include	<stdlib.h>

#ifdef __GNUC__
#include <inttypes.h>
#include <stdarg.h>
#endif


#ifndef WINCE 
#ifdef WIN32
#include  <io.h>
#else
#include  <unistd.h>
#endif
#endif

#ifdef WIN32
#pragma warning(disable :4204)
#pragma warning(disable:4244)
#pragma warning(disable:4305)
#pragma warning(disable:4996)
#pragma warning(disable:4995)
#pragma warning(disable:4275)
#pragma warning(disable:4251)
#endif

#ifdef __ANDROID__
#include <stdint.h>
#define __STDC_INT64__
#endif

#ifdef __SYMBIAN32__
	#define hypot(x,y) sqrt((x)*(x) + (y)*(y))
#endif // __SYMBIAN32__

#ifdef WINCE
#define hypot(x,y) sqrt((x)*(x) + (y)*(y))
#define strdup( p )	gpStr_strdup( p )
#endif // __SYMBIAN32__


#define		TRUE	1
#define		FALSE	0



#define         F_BEGIN		0
#define         F_END		1

#define         F_NEXT		0
#define         F_PREV		1


#define         F_RIGHT		0
#define         F_LEFT		1

#define         INV_FLAG(flag)		( flag ^ 1 )

#define         BIT_TOGGLE( flag, bit)	( flag ^= bit )
#define         BIT_SET( flag, bit)	( flag |= bit )
#define         BIT_IS_SET( flag, bit)	( flag & bit )




#define         SP_NO           3

#define         SP_Y            0
#define         SP_Q            1
#define         SP_A            2

#define         SP_R            0
#define         SP_G            1
#define         SP_B            2


	// byte order
#define		LITTLE_ENDIAN		0
#define		BIG_ENDIAN			1


#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif

#define		ANGLE_D2R( d )	( (d) * M_PI / 180.0 )
#define		ANGLE_R2D( r )	( (r) * 180  / M_PI )

#ifndef ABS
#define		ABS( a )        (( (a) < 0 )? -(a) : (a) )
#endif

#ifndef MAX
#define		MAX( a, b )        (( (a) < (b) )? (b) : (a) )
#endif

#ifndef MIN
#define		MIN( a, b )        (( (a) < (b) )? (a) : (b) )
#endif

#define		NINT( a )	( ( (a) > 0 )? ((int)((a)+0.5)) : ((int)((a)-0.5)) )

#define		IN_RANGE( x, a, b )        ( (a) <= (x) && (x) < (b) )

//#ifndef ROUND
//#define		ROUND( a )        ( ( (a) < 0 )? ((int)((a)-1)) : ((int)((a))) )
//#endif

#define		PUSH_TO_RANGE( x, a, b )   \
	      		( ( (x) <= (a) )? (a) : (( (x) > (b) )? (b) : (x) ) )


#define		UPDATE_RANGE( t, min, max )  \
				{ \
					if( t < min )	min = t; \
					else	if( t > max )	max = t; \
				}

#define		CYCLIC_NEXT( i, N )		( (i < N-1 )? i+1 : 0 )


#ifdef  WIN32
typedef	signed char		s_char;

typedef	unsigned char   u_char;
typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef unsigned long   u_long;

#else
	#ifndef PALMOS
		typedef	signed char	s_char;
		#include	<sys/types.h>
	#endif
#endif

#ifndef __ANDROID__
#ifdef WIN32
#ifndef int64_t
typedef __int64  int64_t;
#endif
#ifndef uint64_t
typedef unsigned __int64  uint64_t;
#endif
#endif
#endif

#ifdef CE
#define hypot(x,y) sqrt((x)*(x) + (y)*(y))
#define strdup( x )	_strdup(x )
#endif 


#ifdef WIN32
#define		RGB_COLOR( r, g, b ) 	(((r)<<16) | ((g)<<8) | (b) )
#elif __linux
#define		RGB_COLOR( r, g, b ) 	(((r)<<16) | ((g)<<8) | (b) )
#endif

	
#define RGB_REVERSE( color ) ((((color) & 0xff)<<16) | ((((color) >> 8) & 0xff)<<8) | (((color) >> 16) & 0xff) )

#ifdef WIN32
#define STOCHAR(x) (LPTSTR)LPCSTR(x) 
#define S2CHAR(x) (LPTSTR)LPCSTR(x) 
#endif



typedef void (*LogCallback)( const char *logFormat, ... );

//typedef void (*ProgressCallback)( void *data, int iStep, int nStep );


	/* GpMsg.c */
void	error(char *from, char *s );
void	gpError( char *sender, char *fmt, ... );

void	warning(char *from, char *s );
void	gpWarning( char *sender, char *fmt, ... );
int		gpYesNo( char *sender, char *fmt, ... );
	
void	gpSendWndMessage( char *inData, int state );


void	wno( char * name, int no );

void	wnof( char * name, float	no );


	/* GpIo.c */
void    gp_fwrite_num( FILE *fp, int a, int nByte );
 
int     gp_fread_num( FILE *fp, int nByte ); 


/*  put intger no in the given file in a MSB first order */
void	gp_fwrite_int( FILE *fp, int a );
int		gp_fread_int( FILE *fp );
int		gp_fread_intArr( FILE *fp, int *value );

void	gp_fwrite_int_lsb( FILE *fp, int a );
int		gp_fread_int_lsb( FILE *fp );
int		gp_fread_short_lsb( FILE *fp );



void	gp_fwrite_int3( FILE *fp, int a );
int	gp_fread_int3( FILE *fp );

void	gp_fwrite_float( FILE *fp, float a );
float	gp_fread_float( FILE *fp );

int	gp_fread_short( FILE *fp );
void	gp_fwrite_short( FILE *fp, int a );

int	gp_read_int( int fd );
int	gp_read_short( int fd );

void	gp_write_int( int fd, int a );
void	gp_write_short( int fd, int a );





int	gp_read_int_lsbf( int fd );
int	gp_read_short_lsbf( int fd );

void	gp_write_int_lsbf( int fd, int a );
void	gp_write_short_lsbf( int fd, int a );







	/* GpFile.c */

int	gp_filename_extand( char *file1, char *file2 );
int	gp_string_cut_extension( char *s1, char *extansion, char *s2 );





int 	gpFile_exist( char *file );

int		gpFile_size( char *file );

long	gpFile_mtime( char *file );

int 	gpDir_exist( char *dir );

int		gp_getcwd( char *dir, int size );
int		gp_mkdir( char *dir );
int		gp_rmdir( char *dir );

int		gpFile_rename( char *oldName, char *newName );

int		gpFile_delete( char *file );


int		gpDir_force_exist( char *dir );

int		gpDir_unuse_id( char *base, int i0, char *dir );
int		gpFile_unuse_id( char *base, int i0, char *file );

char *	gpFile_fullpath( char *file, char *fullpath );


	// GpFile1.c
int		gp_file_create( char *data, int bytes, char *file );

int		gpFile_copy( char *sfile, char *tfile );

int		gpFile_copy_to( char *sfile, char *dir );

int		gp_file_append( char *file, int i0, FILE *fp );

int		gpFile_compare( char *file1, char *file2 );

int		gpFile_lines( char *file );

//int		gp_write_buffer_to_file(char *file, char *data, int bytes );

int		gpFile_write_buffer(char *file, char *data, int bytes );
//#define gp_write_buffer_to_file  gpFile_write_buffer

int		gpFile_read_to_buffer(char *szFile, char **data, int *bytes );

int		gp_write_INTtable_to_file(char *file, char *name, int table[], int no );

void	gp_fwrite_bits( FILE *fp, int code, int no );

int		gpFile_fseek( FILE *fp, char *token );

int		gpFile_find_token( FILE *fp, char *token, char line[1024] );
int		gpFile_find_itoken( FILE *fp, char *token, char line[1024] );

int		gpFile_read_path( FILE *fp, char *dir, char *path );

int		gpFile_sread_path( char *str, char *dir, char *path );

int		gpFile_abs_path( char *fname, char *dir, char *path );

int		gpFile_find( char *dir, char *extension, char *path );

int		gpFile_read_path_line( FILE *fp, char *token, char *relativeDir, char *path );

		/* GpFileName.c */
void	gp_filename( char *dir, char *fname, char *extension, char *file );

void	gp_filename_split( char *fname, char *dir, char *name, char *extension);

void	gp_filename_extract_name( char *fname, char *name, int len );

void	gp_filename_extract_FullName( char *fname, char *name, int len );

void	gpFilename_extension( char *fname, char *extension );

void	gp_filename_extract_extension( char *fname, char *extension, int len );

void	gp_filename_extract_dir(char *fname, char *dir, int len);

int		gpFilename_dir(char *fname, char *dir );
void	gpFilename_name( char *fname, char *name );
void	gpFilename_fullname( char *fname, char *name );



void	gp_filename_force_extension( char *file, char *extension );

void	gpFilename_force_extension( char *fname, char *extension, char *file );

void	gpFilename_force_dir( char *inFile, char *dir, char *file );


void	gpFilename_remove_extension( char *fname );

void	gp_filename_remove_extension( char *fname );

int		gpFilename_compare_extension( char *fname, char *extension );

void	gp_filename_slash( char *fname );

void	gpFilename_backslash( char *sfile, char *tfile );


void	gp_filename_outFile( char *outFile, char *inFile, char *extension );

void	gpFilename_outFile( char *outName, char *inFile, char *extension, char *outFile );


void	gp_filename_outDir( char *outDir, char *inFile );

int		gpFilename_path( char *fname, char *dir, char *path );

int	gpFilename_is_number( char *file );



	/* GpDir.c */
int	gpDir_get_directory( char *dir, char *precede, char *extension,
                                        char *name[], int max, int fullPath  );

int	gp_directory_dir( char *dir, char *name[], int max  );

int gpDir_get_files_list_W( char *dir,  char *prefix, char *ext,
						   char *files[], int max_num_of_files, int bFullPath );

int gpDir_get_files_list( char *dir,  char *prefix, char *ext,
						 char *files[], int max_num_of_files, int bFullPath );

int gpDir_get_files( char *dir,  char *prefix, char *suffix,
					char *files[], int NFiles, int fullPath );


int gpDir_get_file( char *dir, char *prefix, char *ext, char *file );

int	gp_directory_childs( char *dir, char *precede,
				char *extension, char *name[], int max  );

void	gpDir_delete_files( char *dir, char *prefix, char *ext );

int	gpDir_copy_to( char *sdir, char *dir );
int	gpDir_copy( char *sdir, char *dir );



	/* GpRelinquish.c */
void	gp_relinquish_cpu();


	/* GpByteOrder.c */
int		gp_byte_order();

void	gp_byte_order_swap_short( short *loc, int no );

void	gp_byte_order_swap_int( int *loc, int no );



	/* GpFunction.c  */
int		gp_ilog2( int a );
int		gp_bound2( int a );
float	gpNormalDistribution( float average, float var, float x );



char	* gp_strlwr(char *szString);


char *	gp_strichr( char *s, int c );

int		gp_strnicmp(const char *szString1, const char *szString2, int count);

int		gp_stricmp(const char *s1, const char *s2);

char *	gpStr_strdup( char *p );

int		gpStr_indexOf( char *str, char a );

int		gpStr_lastIndexOf( char *str, char a );


char *	gpStr_union( char *s[], int nS, char **data, int *byte );

char *	gpStr_token( char *data, char *end, char *token, int token_len );

char *	gpStr_token_skip( char *data, char *end, char *token, int token_len );


void	gpStrA_replace_c( char *as[], int nS, char c1, char c2 );

void	gpStr_replace_c( char *s, char c1, char c2 );



char	*gp_strcasestr(const char *haystack, const char *needle);


int		gp_strtok_float( char *data, char *seps, float *a);

int		gp_strtok_int( char *data, char *seps, int *a);

int		gp_strtok_hexa( char *data, char *seps, int *a);


int		gp_strtok_string( char *data, char *seps, char *a);



int		gpStr_remove_end_apace( char *text );

int		gpStr_remove_head_apace( char *text );

u_short *	gpStr_char2short( char *text );

void	gpStr_sort( char *str[], int no  );

int		gpStr_replace( char *in, char *s0, char *s1, char *out );

int		gpStr_strtok_float( char *data, char *seps, float *a );

int		gpStr_strtok_double( char *data, char *seps, double *a );

int		gpStr_strtok_int( char *data, char *seps, int *a );

int		gpStr_strtok_hexa( char *data, char *seps, int *a );


int		gpStr_strtok_string( char *data, char *seps, char *a );

int	gpStr_strtok_skip( char *data, char *seps, int skip );
int	gpStr_strtok_istring( char *data, char *seps, int iToken,  char *token );

int	gpStr_strtok_ifloat( char *data, char *seps, int iToken, float *token );


float	gpSeg_overlap( float b0, float e0, float b1, float e1 );



	/* GpTempFile.c */
int		gp_unique_id();

int		gp_filename_set_temporary_dir( char *dir );

int		gp_filename_set_temporary_dirName( char *base, char *name );


void	gp_filename_get_temporary( char *prefix, int unique, char *extension, char *filename );

void	gp_filename_get_temporaryD( char *prefix, int unique,
								   char *dir, char *name );



	// GpDump.c
void	gpDump_set_dir( char *dump_dir );

void	gpDump_set_dirR( char *baseDir, char *subDir );


int		gpDump_is_set();

char *	gpDump_get_dir();


int		gpDump_get_sub_dir( char *subDir, char *dir );

int		gpDump_filename( char *prefix, int index, char *suffix, char *extension, char *file );	


void	gpDump_delete(  char *prefix, char *suffix );

void	gpDump_double( double a[], int nA, char *prefix, int index, char *suffix );

void	gpDump_float( float a[], int nA, char *prefix, int index, char *suffix );

void	gpDump_int( int a[], int nA, char *prefix, int index, char *suffix );

	// hyputA.c
float	hypotA( float x, float y );


	// GpProgram.c
//int		gpProgram_path( char *path );
//
//int		gpProgram_dir( char *dir );



#ifdef __cplusplus
}
#endif
#endif
