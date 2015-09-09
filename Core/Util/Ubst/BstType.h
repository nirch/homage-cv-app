/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/**************************
 ***	BstType.h	***
 **************************/

#ifndef		_BST_TYPE_
#define		_BST_TYPE_

#ifdef __cplusplus
extern "C" {
#endif


#include	<stdio.h>
#include 	"Uigp/igp.h"

#include 	"Ucode/code.h"


typedef struct bst_type {
	int	MaskIndex;
	int	mask;

	int	byte_i;
	u_char	*byte_ptr;

	u_char	*data;
	int	BYTE_NO;
	int	byte_no;
	
} bst_type;


#define		BST_BITS( bst )		( bst->byte_i*8 + bst->MaskIndex )

#define		BST_BYTES( bst )	(( bst->MaskIndex == 0 )? \
						bst->byte_i : bst->byte_i +1 )



	/* BstTool.c */
bst_type *	bst_alloc( int byte_no );

void		bst_realloc( bst_type *bst, int byte_no );

void		bst_extend(bst_type *bst, int byte_extention);

void		bst_rewind( bst_type *bst );

void    	bst_copy( bst_type *bst_in, bst_type * bst_out, int byte_no); 

void		bst_clear( bst_type *bst );

void		bst_destroy( bst_type *bst );

bst_type *	bst_create( char data[], int byte_no, int FCopydata );

bst_type *	bst_make_copy( bst_type *bst_in );


	// BstFile.c 
bst_type *	bst_read_from_file( char *file );

int		bst_write_to_file( bst_type *bst, char *file );

bst_type *	bst_read( FILE *fp, int byte_no );

void		bst_write( FILE *fp, bst_type *bst );


	/* BstGet.c */
int	bst_get( bst_type *bst );

int	bst_get_inv( bst_type *bst );//arie

int		bst_get_int( bst_type *bst, int bit_no );

void	bst_get_align( bst_type *bst );

int		bst_get_align_char( bst_type *bst );

void	bst_get_string( bst_type *bst, char *s, int len );

char *	bst_get_Astring(  bst_type *bst, int len );

void	bst_get_buff( bst_type *bst, char *s, int len );

int     bst_get_align_num( bst_type *bst, int byte_no );

int		bst_get_sint( bst_type *bst, int bits );

float	bst_get_float_code( bst_type *bst, code *cd, int bit );

float	bst_get_float( bst_type *bst, float d, int bit );

bst_type *	bst_get_align_bst( bst_type *bst_in, int byte_no );



	/* BstPut.c */
void	bst_put( bst_type *bst, int bit );

void	bst_put_inv( bst_type *bst, int bit );//arie

void	bst_put_int( bst_type *bst, int code, int bit_no );

void	bst_put_align( bst_type *bst );

void	bst_put_align_char( bst_type *bst, int a );

void	bst_put_string( bst_type *bst, char *s, int len );

void	bst_put_align_num( bst_type *bst, int value, int byte_no );

void	bst_put_buff( bst_type *bst, char *s, int len );

void	bst_reput_align_num( bst_type *bst, int i, int value, int byte_no );

void	bst_put_float_code( bst_type *bst, float a, code *cd, int bit );

void	bst_put_float( bst_type *bst, float a, float d,  int bit );



	/* BstIn.c - like BstGet but in defrennt diretion */
int	bst_InputBit( bst_type *bst );

int	bst_InputBits( bst_type *bst, int bit_no );



void	bst_OutputBit( bst_type *bst, int bit );

void	bst_OutputBits( bst_type *bst, int value, int nBits );

void	bst_OutputAlignByte( bst_type *bst, int value );

void	bst_OutputAlignBytes( bst_type *bst, char *data, int bytes );


void	bst_OutputAlignInt( bst_type *bst, int value, int bytes );





void	bst_in_align( bst_type *bst );

void	bst_in_align_seek( bst_type *bst, int bytes );

#ifdef __cplusplus
}
#endif


#endif 
