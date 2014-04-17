#include "XmlType.h"
#include "string.h"
#include "stdlib.h"


#include "Uvec/Vec2d.h"
#include "Uvec/Vec3d.h"


xml_type * xml_alloc(char *name)
{
	xml_type *doc = (xml_type*)malloc(sizeof(xml_type));
	if ( name != NULL )
		strncpy(doc->name,name, 64);
	else
		memset(doc->name, 0, 64);
	doc->root = NULL;
	doc->header = NULL;
	return doc;
}


void xml_add_header(xmlTag_type *h, xml_type *doc)
{
	xmlTag_type *t;
	if ( doc->header == NULL )
	{
		doc->header = h;
		return;
	}
	else
	{
		for ( t = doc->header; t->next != NULL ; t = t->next )
			;
		t->next = h;
	}
}

void xml_add_root(xmlTag_type *h, xml_type *doc)
{
	xmlTag_type *t;
	if ( doc->root == NULL )
	{
		doc->root = h;
		return;
	}
	else
	{
		for ( t = doc->root; t->next != NULL ; t = t->next )
			;
		t->next = h;
	}
}

void xml_destroy(xml_type *doc)
{
	xmlTag_type *t;
	t = doc->header;

	while (t != NULL){
		doc->header = t->next;
		xmlTag_destroy(t);
		t = doc->header;
	}

	t = doc->root;
	while (t != NULL){

		doc->root = t->next;
		xmlTag_destroy(t);
		t = doc->root;
	}
	free(doc);	
}







xmlTag_type * xmlTag_alloc( char *name )
{
xmlTag_type *t;
		
	t = (xmlTag_type*)malloc(sizeof (xmlTag_type));

	t->dataP = NULL;

	if ( name == NULL )
		memset(t->name,0,64);
	else
		strncpy(t->name, name, 64);

	memset(t->data,0,256);
	t->firstChild = NULL;
	t->firstAtt = NULL;
	t->next = NULL;	
	t->type = enTagNull;
	t->numChildren = 0;
	return t;
}

void xmlTag_destroy(xmlTag_type *t)
{
xmlTag_type *c;
xmlAtt_type *att;

	if( t->dataP != NULL )
		free( t->dataP );

	while( t->firstChild != NULL ){
		c = t->firstChild;
		t->firstChild = t->firstChild->next;

		xmlTag_destroy( c );
	}


	
	while( t->firstAtt != NULL ){
		att = t->firstAtt;
		t->firstAtt = t->firstAtt->next;
		xmlAtt_destroy( att );
	}



	free (t);
}



void xmlTag_set_data(xmlTag_type *t, char data[256])
{
	if ( t == NULL ) return;
	strncpy(t->data,data,256);

}



void xmlTag_insert_att(xmlTag_type * t, xmlAtt_type *att)
{
	xmlAtt_type *tmp = t->firstAtt;
	if ( tmp == NULL )
	{
		t->firstAtt = att;
		return ;
	}
	while ( tmp->next != NULL )
		tmp = tmp->next;
	tmp->next = att;	
}



int xmnlTag_delete_att(xmlTag_type *t, char attName[64])
{
	xmlAtt_type *att = t->firstAtt;
	xmlAtt_type *pAtt = NULL;
	while ( att != NULL )
	{
		if ( strcmp(att->name, attName) == 0 )
		{
			// found:
			if ( pAtt == NULL )
			{// delete the first att:
				t->firstAtt = att->next;
				xmlAtt_destroy(att);
				return 1;
			}
			else 
			{
				pAtt->next = att->next;
				xmlAtt_destroy(att);
				return 1;
			}
		}
		att = att->next;
	}
	return 0;
}




/**** XML_tag_add_child ****/
/*!
 * inserts a child to the parent tag
 *
 * @param *parent : the parent tag
 * @param child : the new tag to add
 * @param atIndex : location in the list of childs
 *
 * @return void  : 
 */
void xmlTag_add_child(xmlTag_type *parent, xmlTag_type* child, int atIndex)
{
	int i;
	xmlTag_type *t;
	parent->numChildren++;
	parent->type = enTagParent;
	if ( parent->firstChild == NULL )
	{
		parent->firstChild = child;
		return;
	}
	else 
	{
		if ( atIndex == 0 )
		{
			child->next = parent->firstChild ;
			parent->firstChild = child;
			return;
		}
		// not at head of the list
		for ( t = parent->firstChild , i = 1 ; i < atIndex && t->next != NULL; i++, t = t->next)
			;
		child->next = t->next;
		t->next = child;
		return;
	}
}

void xmlTag_delete_child(xmlTag_type *parent, int childIndex)
{
	int i;
	xmlTag_type *t = parent->firstChild;
	xmlTag_type *pt = NULL;

	if ( childIndex == 0 )
	{
		parent->firstChild = t->next;
		xmlTag_destroy(t);
		parent->numChildren--;
		if ( parent->firstChild == NULL )
			parent->type = enTagStandalone;
		return;
	}
	for ( i = 1, pt = t, t = t->next; i < childIndex && t != NULL ; pt = t, t = t->next )
		;
	pt->next = t->next; 
	xmlTag_destroy(t);	
	parent->numChildren--;
}


int xmlTag_delete_last_child(xmlTag_type *pTag )
{
xmlTag_type	*tag, *pt;

	if( pTag->firstChild == NULL )
		return( -1 );


	pt = NULL;
	for( tag = pTag->firstChild ; tag->next != NULL ; pt = tag, tag = tag->next );

	if( pt == NULL )
		pTag->firstChild = NULL;
	else
		pt->next = NULL;

	xmlTag_destroy( tag );

	return( 1 );
}


int xmlTag_prase_vec2f( xmlTag_type *pTag, vec2f_type *p )
{
xmlAtt_type	*at;

	for( at = pTag->firstAtt ; at != NULL ; at = at->next ){

		if ( gp_stricmp( at->name, "x") == 0 ){
			p->x = atof(at->value );
			continue;
		}
		if ( gp_stricmp( at->name, "y") == 0 ){
			p->y = atof(at->value );
			continue;
		}
	}
	return( 1 );
}

int xmlTag_prase_vec3f( xmlTag_type *pTag, vec3f_type *p )
{
	xmlAtt_type	*at;

	for( at = pTag->firstAtt ; at != NULL ; at = at->next ){

		if ( gp_stricmp( at->name, "x") == 0 ){
			p->x = atof(at->value );
			continue;
		}
		if ( gp_stricmp( at->name, "y") == 0 ){
			p->y = atof(at->value );
			continue;
		}
		if ( gp_stricmp( at->name, "z") == 0 ){
			p->z = atof(at->value );
			continue;
		}
	}
	return( 1 );
}


xmlAtt_type * xmlAtt_alloc(const char *name, const char *value)
{
	xmlAtt_type *att = (xmlAtt_type*)(malloc(sizeof (xmlAtt_type)));
	strncpy(att->name, name, 64);
	strncpy(att->value, value, 64);
	att->next = NULL;
	return att;
}

void xmlAtt_destroy(xmlAtt_type *att)
{
	free (att);
}


XML_tagPointer_type  *XML_tagPointer_alloc()
{
	XML_tagPointer_type *tp;
	tp = (XML_tagPointer_type *)malloc(sizeof(XML_tagPointer_type ));
	tp->next = NULL;
	tp->t = NULL;
	return tp;
}