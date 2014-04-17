#include "XmlType.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Ustr.h"
#include "Ucst/CstType.h"
#include "Ugio/GioType.h"

char * XmlDoc2Buffer(xml_type *doc);
char * XmlTag2Buffer(xmlTag_type *t);
char * XmlAtt2Buffer(xmlAtt_type *att);




int 
WriteXmlDocToFile( char *filename, xml_type *doc)
{
	FILE *fp;
	xmlTag_type *t;
	fp = fopen(filename,"wt");
	if ( fp == 0 )
		return 1;
	for (t = doc->header; t != NULL; t = t->next )
		WriteXmlTag(t,fp, 0);
	for (t = doc->root; t != NULL; t = t->next )
		WriteXmlTag(t,fp, 0);
	fclose(fp);
	return 0;
}

/*
trContent_type * XML_doc2Content(xml_type *doc)
{
trContent_type *co;
gio_type * gio;
u_char *data;
int bytes;
	gio = XmlDocToGio(doc);
	bytes = gio_bytes(gio);
	data = (u_char*)malloc(bytes * sizeof(u_char));
	gio_get_buffer(gio, data, bytes);
	co = trContent_from_buffer( data, bytes, NULL, LF_SMIL, 0);	
	gio_close(gio);
	data = NULL;
	return co;
}
*/

gio_type * XmlDocToGio(xml_type *doc)
{
	gio_type *gio;
	char buf[1024];

	
	gp_filename_get_temporary( "SML", 0, ".smil", buf );
	WriteXmlDocToFile(buf,doc);
	gio = gio_open_file_to_read( buf );
	return gio;
}



char * XmlTag2Buffer(xmlTag_type *t)
{
return NULL;
}


/**** XmlAtt2Buffer ****/
/*!
 * returns the xmlAtt_type's text representation
 *
 * @param *att : the xmlAtt_type
 *
 * @return char*: the text representation
 */
char *XmlAtt2Buffer(xmlAtt_type *att)
{
	char *str;
	char *tmp;
	str = strapp(" ", att->name);
	tmp = strapp(str, " = \"");
	free (str);
	str = strapp(tmp, att->value);
	free (tmp);
	tmp = str;
	str = strapp(tmp, "\"");
	free (tmp);	
	return str;
}




void WriteXmlTag(xmlTag_type *t, FILE *fp, int tabs)
{
	int i;
	int SIZEOFCHAR = sizeof(char);
	xmlTag_type *child;
	xmlAtt_type *att;
	char line[1000];
	char * str;
	memset(line,0,1000);
	if ( t == NULL )
		return;
	// create the <name ...> xmlTag_type	
	for ( i = 0 ; i < tabs; i ++ )
		fwrite(" ",SIZEOFCHAR, 1, fp);
	switch ( t->type )
	{
	case enTagComment:
	
		fwrite("<!-- ",SIZEOFCHAR,5,fp);
		fwrite(t->data,SIZEOFCHAR,strlen(t->data), fp);
		fwrite(" -->\n",SIZEOFCHAR,5,fp);
		return;
	case enTagHeader:
		//fwrite("<? ",SIZEOFCHAR,3,fp);
		sprintf(line,"<%c ",(char)t->headerDelimiter);
		fwrite(line,SIZEOFCHAR,strlen(line),fp);
		fwrite(t->data,SIZEOFCHAR,strlen(t->data), fp);
		//fwrite(" ?>\n",SIZEOFCHAR,4,fp);
		if ( t->headerDelimiter == 33 ) // '!'
			sprintf(line, " >\n");
		else
			sprintf(line," %c>\n",(char)t->headerDelimiter);
		fwrite(line,SIZEOFCHAR,strlen(line),fp);		
		return;
	}
	
	fwrite( "<", SIZEOFCHAR, 1, fp);		
	fwrite( t->name, SIZEOFCHAR, strlen(t->name), fp);
	// write the attributes
	att = t->firstAtt;
	while (att != NULL)
	{
		str = XmlAtt2Buffer(att);
		fwrite( str, SIZEOFCHAR, strlen(str), fp);
		free(str);
		att = att->next;
	}	
	child = t->firstChild;
	// 
	if ( child == NULL )
	{// so it's a singletone xmlTag_type, without children, close it with slash.
		if ( t->data == NULL || strlen(t->data) == 0 )
			fwrite("/>\n", SIZEOFCHAR, 3, fp);
		else
		{
			fwrite(">", SIZEOFCHAR, 1, fp);
			fwrite(t->data, SIZEOFCHAR, strlen(t->data), fp);
			fwrite("</", SIZEOFCHAR, 2, fp);
			fwrite( t->name, SIZEOFCHAR, strlen(t->name), fp);
			fwrite(">\n", SIZEOFCHAR, 2, fp);
	
		}

		return;
	}
	else
	{// write all children, and close with </tagname>
		// close the xmlTag_type <tagname att1=... att2=...    >
		fwrite(">", SIZEOFCHAR, 1, fp);
		
		
		// don't add \n before data
		if ( t->data != NULL && strlen(t->data) != 0 ) 
		{
			fwrite(t->data, SIZEOFCHAR, strlen(t->data), fp);			
		}// data != NULL		
		fwrite("\n",SIZEOFCHAR, 1, fp);
		// write the children
		for ( ;child != NULL ; child = child->next)
		{
			WriteXmlTag(child,fp, tabs + 1);// maybe with continuation?
		}		
		for ( i = 0 ; i < tabs; i ++ )
			fwrite(" ",SIZEOFCHAR, 1, fp);
		// close the xmlTag_type again:
		fwrite("</", SIZEOFCHAR, 2, fp);
		fwrite( t->name, SIZEOFCHAR, strlen(t->name), fp);
		fwrite(">\n", SIZEOFCHAR, 2, fp);
	}
}
