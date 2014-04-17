
#include	<stdio.h>
#include	<math.h>
#include	<string.h>



#include	"Uigp/igp.h"
#include	"Ucst/CstType.h"

#include	"XmlType.h"
#include	"XmlParser.h"

#include "XmlPut.h"



int
xml_write_file( xml_type  *xml, char *file  )
{
FILE	*fp;

	if( ( fp = fopen( file, "wb")) == NULL )
		return( -1 );


	xmlPut_tag( fp,  0, xml->root );

	fclose( fp );

	return( 1 );
}



void
xml_write_to_buffer( xml_type  *xmlDoc, int align, char **data, int *bytes )
{

	cst_type *cst;

	cst = cst_alloc( 100000 );


	xml_write_to_cst( xmlDoc->root, 0, cst );

	*data = cst->data;
	*bytes = CST_BYTES( cst );

	cst_destroy( cst, 0 );
}


void
xml_write_to_cst( xmlTag_type *cTag, int align, cst_type *cst )
{
xmlTag_type *tag;
xmlAtt_type *att;



	if( cTag->firstChild == NULL ){

		if( cTag->firstAtt != NULL ){
			cst_put_xml_att_openA( cst, align, cTag->name );

			for( att = cTag->firstAtt ; att != NULL ; att = att->next )
				cst_put_xml_att_string( cst, att->name, att->value );


			if( cTag->data[0] == 0 ){
				cst_put_string( cst, "/>\n" );
				return;
			}
			
			cst_put_string( cst, ">\n" );
			cst_put_xml_tag_stringA( cst, align, cTag->name, cTag->data );

			cst_put_xml_tag_closeA( cst, align, cTag->name );
			return;
		}



		if( cTag->dataP != NULL )
			cst_put_xml_tag_stringA( cst, align, cTag->name, cTag->dataP );
		else
			cst_put_xml_tag_stringA( cst, align, cTag->name, cTag->data );
		return;
	}



	if( cTag->firstAtt != NULL ){
		cst_put_xml_att_openA( cst, align, cTag->name );

		for( att = cTag->firstAtt ; att != NULL ; att = att->next )
			cst_put_xml_att_string( cst, att->name, att->value );

		cst_put_string( cst, ">\n" );
	}
	else
			cst_put_xml_tag_openA( cst, align, cTag->name );


	for( tag = cTag->firstChild ; tag != NULL ; tag = tag->next )
		xml_write_to_cst( tag, align+1, cst );


	cst_put_xml_tag_closeA( cst, align, cTag->name );
}

