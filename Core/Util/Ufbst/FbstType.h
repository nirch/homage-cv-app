/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/**************************
 ***	FbstType.h	***
 **************************/

#ifndef		_FBST_TYPE_
#define		_FBST_TYPE_

#include	<stdio.h>
#include 	"Uigp/igp.h"


typedef struct fbst_type {
	int	MaskIndex;
	int	mask;

	u_char	byte;
	int	byte_i;

	FILE	*fp;
	
} fbst_type;


#define		FBST_BITS( bst )	( bst->byte_i*8 + bst->MaskIndex )


	/* BstTool.c */
fbst_type *	fbst_alloc();

void	fbst_free( fbst_type *bst );

fbst_type *	fbst_open_to_write( char *file_name );

fbst_type *	fbst_open_to_read( char *file_name );
fbst_type *	fbst_set_to_read( FILE *fp );

void	fbst_close( fbst_type *bst, int Fwrite );




	/* BstfGet.c */
int	fbst_get( fbst_type *bst );

int	fbst_get_int( fbst_type *bst, int bit_no );

void	fbst_get_int1( fbst_type *bst, int *cno, int bit_no );

void	fbst_get_algin( fbst_type *bst );

int	fbst_get_algin_char( fbst_type *bst );


	/* BstfPut.c */
void	fbst_put( fbst_type *bst, int bit );

void	fbst_put_int( fbst_type *bst, int code, int bit_no );

void	fbst_put_algin( fbst_type *bst );

void	fbst_put_algin_char( fbst_type *bst, int a );



#endif 
