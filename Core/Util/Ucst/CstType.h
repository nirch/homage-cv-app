/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/**********************
 ***	CstType.h	***
 **********************/

#ifndef		_CST_TYPE_
#define		_CST_TYPE_


#ifdef __cplusplus
extern "C" {
#endif

#include 	"Uigp/igp.h"

struct bst_type;


typedef struct cst_type {
	u_char	*rp;

	u_char	*wp;

	u_char	*data;

	int	BYTE_NO;

//	int	byte_no;
	
} cst_type;


//#define		CST_GET_SCHAR( cst )	( (char)*( (cst)->rp++) )
#define		CST_GET_SCHAR( cst )	(*( (s_char *)(cst)->rp++) )

#define		CST_GET_UCHAR( cst )	( (u_char)*( (cst)->rp++) )

//#define		CST_GETC( cst )			( (char)*( (cst)->rp++) )
#define		CST_GETC( cst )			( *( (s_char *)(cst)->rp++) )
#define		CST_GETUC( cst )		( (u_char)*( (cst)->rp++) )


#define		CST_PUTC( cst, a )		(*( (cst)->wp++) = (u_char)a)

#define		CST_PUT_CHAR( cst, a )	(*( (cst)->wp++) = (u_char)a )



//#define		CST_BYTES( cst )	( (cst)->wp - (cst)->data )
#define		CST_BYTES( cst )	( (cst)->wp - (cst)->rp )

#define		CST_BYTE_INDEX( cst )	( (cst)->rp - (cst)->data )


//#define     CST_BYTE_AVAILABLE( cst ) ((cst) ->BYTE_NO - CST_BYTES(cst))

#define     CST_BYTE_AVAILABLE( cst ) ((cst) ->BYTE_NO - ((cst)->wp - (cst)->data) )


#define		CST_END_BYTE( cst )  ( cst->wp)
#define		CST_EOS( cst )  ( cst->rp >= cst->wp )
#define		CST_NEOS( cst )  ( cst->rp < cst->wp )
#define		CST_LAST_BYTE(cst)  ( cst->wp )


#define		CST_RTELL( cst )		( cst->rp - cst->data )
#define		CST_WTELL( cst )		( cst->wp - cst->data )
#define		CST_RSEEK( cst, pos )	( cst->rp = cst->data + pos )
#define		CST_WSEEK( cst, pos )	( cst->wp = cst->data + pos )
#define		CST_SKIP( cst, pos )	( cst->rp += pos )



	// CstTool.c
cst_type *	cst_alloc( int byte_no );

cst_type *	cst_create( char data[], int byte_no, int FCopydata );

cst_type *	cst_make_copy( cst_type *cst );



void	cst_clear( cst_type *cst );

void	cst_free( cst_type *cst );

void	cst_destroy( cst_type *cst, int FDeletedata  );

void	cst_realloc( cst_type *cst, int byte_no );

void	cst_extend(cst_type *cst, int byte_extention);

int		cst_put_string( cst_type *cst, char *buf );

void	cst_put_stringA( cst_type *cst, int align, char *name );

int	cst_put_buffer( cst_type *cst, char buf[], int byte_no );

void	cst_rewind( cst_type *cst );

int	cst_get_buffer( cst_type *cst, u_char buf[], int byte_no );


int	cst_put_buffer_int( cst_type *cst, u_char buf[], int byte_no );



void	cst_put_int( cst_type *cst, int a );

int		cst_get_int( cst_type *cst );


struct bst_type *	cst2bst(cst_type* cst, int byte_no);


void	cst_insert( cst_type *cst, char *data, int bytes );

void	cst_rewind_data( cst_type *cst );


	// CstFile.c
void	cst_read( FILE *fp, cst_type *cst, int byte_no );

int		cst_readN( cst_type *cst, int nByte, FILE *fp );

void	cst_write( FILE *fp, cst_type *cst );

cst_type * cst_load_from_file( char *file );

int		cst_read_file( cst_type **cst, char *file );

int		cst_write_to_file(cst_type *cst, char *file);



	// CstXml.c
void	cst_put_xml_tag_int( cst_type *cst, char *name, int value );

void	cst_put_xml_tag_string( cst_type *cst, char *name, char *value);
void	cst_put_xml_tag_stringS( cst_type *cst, char *name, char *value, int size );


void	cst_put_xml_tag_intA( cst_type *cst, int align, char *name, int value );

void	cst_put_xml_tag_floatA( cst_type *cst, int align, char *name, float value );

void	cst_put_xml_tag_stringA( cst_type *cst, int align, char *name, char *value );

void	cst_put_xml_tag_openA( cst_type *cst, int align, char *name );

void	cst_put_xml_tag_closeA( cst_type *cst, int align, char *name );


void	cst_put_xml_tag_intB( cst_type *cst, char *name, int value );

void	cst_put_xml_tag_floatB( cst_type *cst, char *name, float value );

void	cst_put_xml_tag_stringB( cst_type *cst, char *name, char *value );

void	cst_put_xml_tag_openB( cst_type *cst, char *name );

void	cst_put_xml_tag_closeB( cst_type *cst, char *name );


void	cst_put_xml_att_openA( cst_type *cst, int align, char *name );

void	cst_put_xml_att_string( cst_type *cst, char *name, char *value );

void	cst_put_xml_att_int( cst_type *cst, char *name, int value, char *ext );

void	cst_put_xml_att_float( cst_type *cst, char *name, float value );



#ifdef __cplusplus
}
#endif


#endif 
