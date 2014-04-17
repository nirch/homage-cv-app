/**********************
 ***	CstXml.c   ***
 **********************/
#include	<stdio.h>
#include	"Uigp/igp.h"
#include "Uvec/Vec3d.h"
#include "Uvec/Vec2d.h"

#include "XmlType.h"
#include "XmlPut.h"




void	
xmlPut_tag_string( FILE *fp, int align, char *name, char *value )
{
	int	i;

	for(  i = 0 ; i < align ; i++ )
		fputc( '\t', fp );

	fprintf( fp, "<%s>%s</%s>\n", name, value, name );
}


void	
xmlPut_tag_int( FILE *fp, int align, char *name, int value )
{
	int	i;

	for(  i = 0 ; i < align ; i++ )
		fputc( '\t', fp );

	fprintf( fp, "<%s>%d</%s>\n", name, value, name );
}




void	
xmlPut_tag_float( FILE *fp, int align, char *name, float value )
{
	int	i;

	for(  i = 0 ; i < align ; i++ )
		fputc( '\t', fp );

	fprintf( fp, "<%s>%f</%s>\n", name, value, name );
}


void	
xmlPut_tag_double( FILE *fp, int align, char *name, double value )
{
	int	i;

	for(  i = 0 ; i < align ; i++ )
		fputc( '\t', fp );

	fprintf( fp, "<%s>%lf</%s>\n", name, value, name );
}


void	
xmlPut_tag_open( FILE *fp, int align, char *name )
{
int	i;

	for( i = 0 ; i < align ; i++ )
		fputc( '\t', fp );

	fprintf( fp, "<%s>\n", name );
}

void	
xmlPut_tag_close( FILE *fp, int align, char *name )
{
	int	i;

	for( i = 0 ; i < align ; i++ )
		fputc( '\t', fp );

	fprintf( fp, "</%s>\n", name );
}







void	
xmlPut_tag_open_att( FILE *fp, int align, char *name )
{
	int	i;

	for( i = 0 ; i < align ; i++ )
		fputc( '\t', fp );


	fprintf( fp, "<%s", name );
}


void	
xmlPut_att_string( FILE *fp, char *name, char *value )
{ 
	fprintf( fp, " %s=\"%s\"", name, value );
}



void	
xmlPut_att_int( FILE *fp, char *name, int value, char *unit )
{ 
	//int	i;

	//for(  i = 0 ; i < align ; i++ )
	//	fputc( '\t', fp );

	if( unit == NULL )

		fprintf( fp, " %s=\"%d\"", name, value );
	else
		fprintf( fp, " %s=\"%d%s\"", name, value, unit );

}


void	
xmlPut_att_float( FILE *fp, char *name, float value, char *unit )
{ 
	//int	i;

	//for(  i = 0 ; i < align ; i++ )
	//	fputc( '\t', fp );

	if( unit == NULL )
		fprintf( fp, " %s=\"%f\"", name, value );
	else
		fprintf( fp, " %s=\"%f%s\"", name, value, unit );

}





void	
xmlPut_tag_close_att( FILE *fp )
{
	fprintf( fp, ">\n" );
}

void	
xmlPut_tag_closeA( FILE *fp )
{
	fprintf( fp, "\\>\n" );
}


void
xmlPut_tag( FILE *fp, int align, xmlTag_type *cTag )
{
	xmlTag_type *tag;
	xmlAtt_type *att;


	if( cTag->firstChild == NULL ){

		if( cTag->firstAtt != NULL ){
			xmlPut_tag_open_att( fp, align, cTag->name );

			for( att = cTag->firstAtt ; att != NULL ; att = att->next )
				xmlPut_att_string( fp, att->name, att->value );


			if( cTag->data[0] == 0 ){
				fprintf( fp, "/>\n" );
				return;
			}

			fprintf( fp, ">\n" );

			fprintf( fp, "%s", cTag->data );

			xmlPut_tag_close( fp, align, cTag->name );
			return;
		}



		if( cTag->dataP != NULL )
			xmlPut_tag_string( fp, align, cTag->name, cTag->dataP );
		else
			xmlPut_tag_string( fp, align, cTag->name, cTag->data );
		return;
	}





	if( cTag->firstAtt != NULL ){
		xmlPut_tag_open_att( fp, align, cTag->name );

		for( att = cTag->firstAtt ; att != NULL ; att = att->next )
			xmlPut_att_string( fp, att->name, att->value );

		fprintf( fp, ">\n" );
	}
	else
		xmlPut_tag_open( fp, align, cTag->name );


	for( tag = cTag->firstChild ; tag != NULL ; tag = tag->next )
		xmlPut_tag( fp, align+1, tag );


	xmlPut_tag_close( fp, align, cTag->name );
}



void	
xmlPut_tag_vec3f( FILE *fp, int algin, char *name, vec3f_type *p )
{

	xmlPut_tag_open_att( fp, algin, name );

	xmlPut_att_float( fp, "x", p->x, NULL );
	xmlPut_att_float( fp, "y", p->y, NULL );
	xmlPut_att_float( fp, "z", p->z, NULL );

	xmlPut_tag_closeA( fp );
}

void	
xmlPut_tag_vec2f( FILE *fp, int algin, char *name, vec2f_type *p )
{

	xmlPut_tag_open_att( fp, algin, name );

	xmlPut_att_float( fp, "x", p->x, NULL );
	xmlPut_att_float( fp, "y", p->y, NULL );


	xmlPut_tag_closeA( fp );
}