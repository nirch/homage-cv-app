/**********************
 ***	CstXml.c   ***
 **********************/
#include	<stdio.h>
#include	"Uigp/igp.h"
#include "Uvec/Vec3d.h"
#include "Uvec/Vec2d.h"

#include "Ucst/CstType.h"


#include "XmlPut.h"
#include "XmlType.h"



void	
xmlCst_tag_string( cst_type *cst, int align, char *name, char *value )
{
char	buf[256];
	int	i;

	for(  i = 0 ; i < align ; i++ )
		CST_PUT_CHAR( cst , '\t' );
		

	sprintf( buf, "<%s>%s</%s>\n", name, value, name );
	cst_put_string( cst, buf );
}



void	
xmlCst_tag_int( cst_type *cst, int align, char *name, int value )
{
char	buf[256];
int	i;

	for(  i = 0 ; i < align ; i++ )
		CST_PUT_CHAR( cst , '\t' );

	sprintf( buf, "<%s>%d</%s>\n", name, value, name );
	cst_put_string( cst, buf );
}





void	
xmlCst_tag_float( cst_type *cst, int align, char *name, float value )
{
int	i;
char	buf[256];

	for(  i = 0 ; i < align ; i++ )
		CST_PUT_CHAR( cst , '\t' );

	sprintf( buf, "<%s>%f</%s>\n", name, value, name );
	cst_put_string( cst, buf );
}


void	
xmlCst_tag_double( cst_type *cst, int align, char *name, double value )
{
int	i;
char	buf[256];

	for(  i = 0 ; i < align ; i++ )
		CST_PUT_CHAR( cst , '\t' );

	sprintf( buf, "<%s>%lf</%s>\n", name, value, name );
	cst_put_string( cst, buf );
}


void	
xmlCst_tag_open( cst_type *cst, int align, char *name )
{
int	i;
char	buf[256];

	for( i = 0 ; i < align ; i++ )
		CST_PUT_CHAR( cst , '\t' );

	sprintf( buf, "<%s>\n", name );
	cst_put_string( cst, buf );

}

void	
xmlCst_tag_close( cst_type *cst, int align, char *name )
{
	int	i;
	char	buf[256];

	for( i = 0 ; i < align ; i++ )
		CST_PUT_CHAR( cst , '\t' );

	sprintf( buf, "</%s>\n", name );
	cst_put_string( cst, buf );
}







void	
xmlCst_tag_open_att( cst_type *cst, int align, char *name )
{
	int	i;
	char	buf[256];

	for( i = 0 ; i < align ; i++ )
		CST_PUT_CHAR( cst , '\t' );


	sprintf( buf, "<%s", name );
	cst_put_string( cst, buf );
}


void	
xmlCst_att_string( cst_type *cst, char *name, char *value )
{ 
char	buf[256];
	
	sprintf( buf, " %s=\"%s\"", name, value );
	cst_put_string( cst, buf );
}



void	
xmlCst_att_int( cst_type *cst, char *name, int value, char *unit )
{ 
char	buf[256];

	if( unit == NULL )

		sprintf( buf, " %s=\"%d\"", name, value );
	else
		sprintf( buf, " %s=\"%d%s\"", name, value, unit );

	cst_put_string( cst, buf );
}


void	
xmlCst_att_float( cst_type *cst, char *name, float value, char *unit )
{ 
char	buf[256];

	if( unit == NULL )
		sprintf( buf, " %s=\"%f\"", name, value );
	else
		sprintf( buf, " %s=\"%f%s\"", name, value, unit );

	cst_put_string( cst, buf );
}





void	
xmlCst_tag_close_att( cst_type *cst )
{
char	buf[256];

	sprintf( buf, ">\n" );

	cst_put_string( cst, buf );
}

void	
xmlCst_tag_closeA( cst_type *cst )
{
char	buf[256];

	sprintf( buf, " />\n" );

	cst_put_string( cst, buf );
}


#ifdef _AA
void
xmlCst_tag( cst_type *cst, int align, xmlTag_type *cTag )
{
	xmlTag_type *tag;
	xmlAtt_type *att;


	if( cTag->firstChild == NULL ){

		if( cTag->firstAtt != NULL ){
			xmlCst_tag_open_att( fp, align, cTag->name );

			for( att = cTag->firstAtt ; att != NULL ; att = att->next )
				xmlCst_att_string( fp, att->name, att->value );


			if( cTag->data[0] == 0 ){
				fprintf( fp, "/>\n" );
				return;
			}

			fprintf( fp, ">\n" );

			fprintf( fp, "%s", cTag->data );

			xmlCst_tag_close( fp, align, cTag->name );
			return;
		}



		if( cTag->dataP != NULL )
			xmlCst_tag_string( fp, align, cTag->name, cTag->dataP );
		else
			xmlCst_tag_string( fp, align, cTag->name, cTag->data );
		return;
	}





	if( cTag->firstAtt != NULL ){
		xmlCst_tag_open_att( fp, align, cTag->name );

		for( att = cTag->firstAtt ; att != NULL ; att = att->next )
			xmlCst_att_string( fp, att->name, att->value );

		fprintf( fp, ">\n" );
	}
	else
		xmlCst_tag_open( fp, align, cTag->name );


	for( tag = cTag->firstChild ; tag != NULL ; tag = tag->next )
		xmlCst_tag( fp, align+1, tag );


	xmlCst_tag_close( fp, align, cTag->name );
}
#endif


void	
xmlCst_tag_vec3f( cst_type *cst, int algin, char *name, vec3f_type *p )
{

	xmlCst_tag_open_att( cst, algin, name );

	xmlCst_att_float( cst, "x", p->x, NULL );
	xmlCst_att_float( cst, "y", p->y, NULL );
	xmlCst_att_float( cst, "z", p->z, NULL );

	xmlCst_tag_closeA( cst );
}

void	
xmlCst_tag_vec2f( cst_type *cst, int algin, char *name, vec2f_type *p )
{

	xmlCst_tag_open_att( cst, algin, name );

	xmlCst_att_float( cst, "x", p->x, NULL );
	xmlCst_att_float( cst, "y", p->y, NULL );


	xmlCst_tag_closeA( cst );
}