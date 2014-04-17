#include "XmlType.h"
#include "string.h"
#include "stdlib.h"
#include "stdlib.h"




/**** *GetTag ****/
/*!
 * Returns the first xmlTag_type with name "name"
 *
 * @param *name : the name to search for
 * @param *t1 : the root
 *
 * @return xmlTag_type  : the xmlTag_type or NULL
 */
xmlTag_type *XML_GetTag(const char *name, xmlTag_type *t1)
{
	xmlTag_type *t;
	xmlTag_type *child = NULL;
	t = t1;
	while ( t != NULL && gp_stricmp(t->name, name) != 0 )
	{
		if ( t->firstChild != NULL )
			child = XML_GetTag(name,t->firstChild);
		if ( child != NULL )
			return child;
		t = t->next;
	}
	return t;
}


/**** *XML_GetAllTags ****/
/*!
 * Returns all the tags with name "name" in the tree starts at "root" 
 * 
 *
 * @param *name : name of tag
 * @param *root : root for searh
 * @param *tp : in first call MUST be NULL
 *
 * @return XML_tagPointer_type  : a list of tag pointers
 */
XML_tagPointer_type *XML_GetAllTags(const char *name, xmlTag_type *root, XML_tagPointer_type *tp)
{
	xmlTag_type *t;
	XML_tagPointer_type *ntp;
	t = root;
	/*if ( strcmp(root->name, name) == 0 )
	{			
		ntp = XML_tagPointer_alloc();
		ntp->t = t;
		ntp->next = tp;
		tp = ntp;
	} 
	*/
	while ( t != NULL )
	{		
			
		if ( strcmp(t->name, name) == 0 )
		{	
			ntp = tp;
			while ( ntp && ntp->next != NULL )
				ntp = ntp->next;
			if ( ntp )
			{
				ntp->next = XML_tagPointer_alloc();
				ntp = ntp->next;
			}
			else // create first tagPointer:
			{
				ntp = tp = XML_tagPointer_alloc();
			}			
			ntp->t = t;			
		} 
		if ( t->firstChild != NULL )
				tp = XML_GetAllTags(name,t->firstChild, tp);
		
		t = t->next;

	}
	
	return tp;
}



/**** *XML_GetAllTagsWithAtt ****/
/*!
 * Returns a list of all tags which contains attribute with name as attName
 *
 * @param *attName : the name of the attribute to be contained in the tag
 * @param *t1 : the root
 * @param *tp : list to add to.
 *
 * @return XML_tagPointer_type  : 
 */
XML_tagPointer_type *XML_GetAllTagsWithAtt(const char *attName, xmlTag_type *root, XML_tagPointer_type *tp)
{
	xmlTag_type *t;
	XML_tagPointer_type *ntp;
	xmlAtt_type *att;
	t = root;

	
	while ( t != NULL )
	{		
		att = XML_GetAtt(attName, t);
		//if ( strcmp(t->name, name) == 0 )
		if ( att != NULL )
		{	
			ntp = tp;
			while ( ntp && ntp->next != NULL )
				ntp = ntp->next;
			if ( ntp )
			{
				ntp->next = XML_tagPointer_alloc();
				ntp = ntp->next;
			}
			else // create first tagPointer:
			{
				ntp = tp = XML_tagPointer_alloc();
			}			
			ntp->t = t;			
		} 
		if ( t->firstChild != NULL )
				tp = XML_GetAllTagsWithAtt(attName,t->firstChild, tp);
		
		t = t->next;

	}
	
	return tp;
}

void XML_tagPointer_List_Free(XML_tagPointer_type* tpl)
{
	XML_tagPointer_type* tp0,*tp1;
	if ( tpl == NULL )
		return;
	tp0 = tpl;	

	while ( tp0 != NULL )
	{
		tp1 = tp0->next; 
		free(tp0);
		tp0 = tp1;
	}



}


/**** *GetAtt ****/
/*!
 * Get the first xmlAtt_type in that xmlTag_type, with name "name"
 *
 * @param *name : the xmlAtt_type's name
 * @param *t : the xmlTag_type to search in
 *
 * @return xmlAtt_type  : xmlAtt_type or NULL
 */
xmlAtt_type *XML_GetAtt(const char *name, xmlTag_type *t)
{
	xmlAtt_type *att;
	char * tName;
	char * lName = gp_strlwr((char*)name);

	for ( att = t->firstAtt; att != NULL; att = att->next)
	{
		tName = gp_strlwr(strdup(att->name));
		if ( strcmp(tName, lName) == 0 )
		{
			free(tName);
			return att;
		}
		free(tName);			
	}
	return NULL;
}





char *
XML_doc_get_tag_data( xml_type *doc, char * tagName, int Fcopy )
{
xmlTag_type *tag;

	if( (tag = XML_GetTag(tagName, doc->root ) ) != NULL ){

		if( Fcopy )
			return( strdup( tag->data ) );
		else	return( tag->data );
	}

	return( NULL );
}




/**** *GetTag ****/
/*!
* Returns the first xmlTag_type with name "name"
*
* @param *name : the name to search for
* @param *t1 : the root
*
* @return xmlTag_type  : the xmlTag_type or NULL
*/
xmlTag_type *
xmlTag_get_tag( xmlTag_type *pTag, char *name )
{
xmlTag_type *tag,	*t;

	if( gp_stricmp( pTag->name, name ) == 0 )
		return( pTag );
 
	for( tag = pTag->firstChild ; tag != NULL ; tag = tag->next ){
		if( gp_stricmp( tag->name, name ) == 0 )
			return( tag );

		if( tag->firstChild != NULL ){
			t = xmlTag_get_tag(  tag->firstChild, name );
			if( t != NULL )	return( t );
		}
	}

	return( NULL );
}




char *
xmlTag_get_attr( xmlTag_type *tag, char *attrName, int Fcopy )
{
xmlAtt_type	*at;

	for( at = tag->firstAtt ; at != NULL ; at = at->next ){
		if (gp_stricmp(at->name, attrName ) == 0 ){
		if( Fcopy )
			return( strdup( at->value ) );
		else	return( at->value );
		}
	}

	return( NULL );
}


int
xmlTag_get_attr_int( xmlTag_type *tag, char *attrName, int *value )
{
xmlAtt_type	*at;

	for( at = tag->firstAtt ; at != NULL ; at = at->next ){
		if (gp_stricmp(at->name, attrName ) == 0 ){
			*value = atoi( at->value );
			return( 1 );
		}
	}

	return( -1 );
}


int
xmlTag_get_childNo( xmlTag_type *baseTag, char *tagName )
{
xmlTag_type	*tag;
int	no;

	for( tag = baseTag->firstChild, no = 0 ; tag != NULL ; tag = tag->next ){

		if( tagName == NULL || gp_stricmp(tag->name, tagName) == 0 )
			no++;
	}

	return( no );
}