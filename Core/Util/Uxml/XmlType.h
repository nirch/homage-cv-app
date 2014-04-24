/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef __XML_TYPE_H__
#define __XML_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "Ucst/CstType.h"
#include "Ugio/GioType.h"


#include "Uvec/Vec2d.h"
#include "Uvec/Vec3d.h"







typedef enum TagType {enTagNull=0, enTagHeader, enTagFooter, enTagStandalone, enTagParent, enTagComment} TagType;




typedef struct xmlAtt_type
{
	char name[64];
	char value[256];
	struct xmlAtt_type *next;

} xmlAtt_type;



typedef struct xmlTag_type
{
	TagType type;
	int headerDelimiter;

	char name[64];
	char data[256];

	char	*dataP;

	union {
		xmlAtt_type		*firstAtt;
		xmlAtt_type		*att;
	};

	struct xmlTag_type *firstChild;
	struct xmlTag_type *next;
	int numChildren;

} xmlTag_type;


typedef struct XML_tagPointer_type
{
	xmlTag_type * t;
	struct XML_tagPointer_type *next;

} XML_tagPointer_type;


typedef struct xml_type
{
	xmlTag_type *header;
	xmlTag_type *root;
	char name[64];
}xml_type;


#define XML_attribute_type	xmlAtt_type
#define XML_tag_type		xmlTag_type
#define XML_document_type	xml_type



xml_type * xml_alloc(char *name);

void xml_add_header(xmlTag_type *h, xml_type *doc);

void xml_add_root(xmlTag_type *t, xml_type *doc);

void xml_destroy(xml_type *doc );


	// XmlParser.h
xml_type *	xml_parse_file( char *filename );

xml_type *	xml_parse_data( char *data,int bytes );




	/*XmlType.c*/
xmlTag_type * xmlTag_alloc(char *name);


void xmlTag_destroy( xmlTag_type *t);

void xmlTag_insert_att(xmlTag_type * t, xmlAtt_type *att);

void xmlTag_set_data(xmlTag_type *t, char data[256]);

int xmlTag_delete_att(xmlTag_type *t, char attName[64]);

void xmlTag_add_child(xmlTag_type *parent, xmlTag_type* child, int atIndex);

void xmlTag_delete_child(xmlTag_type *parent, int childIndex);

int xmlTag_delete_last_child(xmlTag_type *pTag );


xmlAtt_type * xmlAtt_alloc(const char *name, const char *value);

void xmlAtt_destroy(xmlAtt_type *att);

char *XML_Att2String(xmlAtt_type *att);

XML_tagPointer_type * XML_tagPointer_alloc();


int xmlTag_prase_vec2f( xmlTag_type *pTag, vec2f_type *p );

int xmlTag_prase_vec3f( xmlTag_type *pTag, vec3f_type *p );


	/*XmlIo.c*/
int WriteXmlDocToFile( char *filename, xml_type *doc);

void WriteXmlTag(xmlTag_type *t, FILE *fp, int tabs);

xmlTag_type *ReadXmlTag(gio_type *gio);

gio_type * XmlDocToGio(xml_type *doc);

int XmlDocByteSize(xml_type *doc);

int XmlTagByteSize(xmlTag_type *tag);


	/*XmlDoc*/
xmlTag_type *XML_GetTag(const char *name, xmlTag_type * t1);
xmlAtt_type *XML_GetAtt(const char *name, xmlTag_type *t);
XML_tagPointer_type *XML_GetAllTags(const char *name, xmlTag_type *t1, XML_tagPointer_type *tp);
XML_tagPointer_type *XML_GetAllTagsWithAtt(const char *attName, xmlTag_type *root, XML_tagPointer_type *tp);
void XML_tagPointer_List_Free(XML_tagPointer_type* tpl);


char *	XML_doc_get_tag_data( xml_type *doc, char * tagName, int Fcopy );

char *	xmlTag_get_attr( xmlTag_type *tag, char *attrName, int Fcopy );

int	xmlTag_get_attr_int( xmlTag_type *tag, char *attrName, int *value );

int	xmlTag_get_childNo( xmlTag_type *baseTag, char *tagName );

xmlTag_type *	xmlTag_get_tag( xmlTag_type *pTag, char *name );


int	xmlFile_get_tag_data( char *dir, char *pName, char *name, char *data );
int	xmlFile_get_tag_data_int( char *dir, char *pName, char *name, int *data );



	// XmlWrite.c
int		xml_write_file( xml_type  *xml, char *file  );

void	xml_write_to_buffer( xml_type  *xmlDoc, int align, char **data, int *bytes );

void	xml_write_to_cst( xmlTag_type *cTag, int align, cst_type *cst );






#ifdef __cplusplus
}
#endif


#endif
