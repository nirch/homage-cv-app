
#include "Ustr.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Ucst/CstType.h"
#include "Ugio/GioType.h"



#include "XmlType.h"
#include "XmlParser.h"

#define	XML_CORRUPTED_INPUT		1

static int	xml_parse_tag_data( gio_type *gio, char *buffer, xmlTag_type *t );





xml_type *
xml_parse_file( char *filename )
{
xml_type	*doc;
char	*data;
int	bytes;


	if( gpFile_read_to_buffer( filename, &data, &bytes ) < 0 )
		return( NULL );


	doc = xml_parse_data( data, bytes );

	free( data );

	return( doc );

}


xml_type * 
xml_parse_data( char *data,int bytes )
{	
xml_type *doc;
gio_type *gio;

	gio = gio_open_buffer_to_read( data, bytes, 0 );  // dont copy the data
	if( gio == NULL )	return( NULL );
		
	doc = XML_parse_from_gio(gio);	

	gio_close(gio);

	return doc;
}



xml_type * 
XML_parse_from_gio( gio_type *gio )
{
xmlTag_type *t;
xml_type * doc;

	doc = xml_alloc(NULL); 
	// read first tag:
	
	t = ReadXmlTag(gio);
	if ( t == NULL ){
		xml_destroy(doc);
		doc = NULL;
		return NULL;
	}

	while (  t != NULL && t->type == enTagHeader){
		xml_add_header(t, doc);
		t = ReadXmlTag(gio);
	}

	if( t == NULL ){
		xml_destroy(doc);
		return( NULL );
	}


	xml_add_root(t,doc);
	
	// return the xml_type
	return doc;
}


xmlTag_type* ReadXmlTag(gio_type *gio)
{
char buffer[256];	
//int err;

	xmlTag_type *t = xmlTag_alloc(NULL);
	
	memset(buffer, 0, 68);

	if( S0(gio, buffer, t) < 0 )
		return( t );

//	if ( err < 0 )
//		return t;


	xmlTag_destroy(t);
	return NULL;
}



/**** S0 ****/
/*!
 * Start. Check that xmlTag_type starts with "<"
 *
 * @param *fp : 
 * @param buffer : accumulated buffer
 * @param t : the xmlTag_type to return
 *
 * @return int  : return status
 */
int S0(gio_type *gio, char* buffer, xmlTag_type *t)
{
	int ch;
	memset(buffer ,0,256);
	ch = GIO_GETC(gio);

	while ( isWhiteSpace(ch) )
		ch = GIO_GETC(gio);

	if ( ch == '<' )
		return P1(gio, buffer,t);

	buffer[strlen(buffer)] = (char)ch;		
	return Q0(gio,buffer,t, 1);
}
   





/**** Q0 ****/
/*!
 * End state : error
 *
 * @param *fp : 
 * @param buffer : accumulated buffer
 * @param t : 
 * @param err : the error number
 *
 * @return int  : 1 - xmlTag_type not started with '<'
 *			2 - xmlTag_type name is illeagal
			3 - xmlAtt_type name is illeagal
			4 - xmlTag_type malformed : </name SOMETHING-HERE
			5 - xmlTag_type malformed : <name / SOMETHING-HERE
			6 - XML not valid : wrong footer :<a> ... </b>	
			7 - XML not valid : wrong char in data (& or < or > )
			8 - XML not valid : started comment without ending
			9 - XML not valid : character '?' mismatch in PreRoot tag.
 */	
int Q0(gio_type *gio, char* buffer, xmlTag_type* t, int err)
{
	return err;
}


/**** Q1 ****/
/*!
 * End state : have a well formed xmlTag_type 
 *
 * @param *fp : 
 * @param buffer : accumulated buffer
 * @param t : the xmlTag_type
 * @param status : status of xmlTag_type (should be succes)
 *
 * @return int  : (-1) :the tag is footer tag: </name>
			(-2) :the tag is complete parent tag: <a> <b/> </a>
			(-3) :the tag is standalone tag: <name/>
			(-4) :the tag is a comment tag: <!-- ... -->
			(-5) :the tag is a pre-header tag: <?xml...?>

 */
int Q1(gio_type *gio, char* buffer, xmlTag_type* t, int status)
{
	return status;
}




/**** Q3 ****/
/*!
 * Error in xmlAtt_type 
 *
 * @param *fp : 
 * @param buffer : 
 * @param *t : 
 * @param *att : the xmlAtt_type with the error
 * @param err : the error
 *
 * @return int  : 1 - att name is illeagal
			2 - att is not valid : <tName attName NO '='
			3 - att is not valid : <tName attName = NO '"'
			4 - att value overflow (over 256 chars)
 */
int Q3(gio_type *gio, char* buffer, xmlTag_type *t, xmlAtt_type *att, int err)
{
	return err;
}





/**** P1 ****/
/*!
 * Tag started with "<", check validity of type (name chars) or headers...
 *
 * @param *fp : 
 * @param buffer : accumulated buffer
 * @param t : the xmlTag_type to return
 *
 * @return int  : return status
 */
int P1(gio_type *gio, char* buffer, xmlTag_type *t)
{
	int ch;
	
	ch = GIO_GETC(gio);
	while ( isWhiteSpace(ch) )
		ch = GIO_GETC(gio);
	if ( ch == 33 ) // '!'
	{
		return C1(gio, buffer, t);
	}
	if ( isNameChar(ch) )
	{		
		buffer[strlen(buffer)] = (char)ch;	
		return P2(gio, buffer, t);
	}
	if ( isHeaderChar(ch) )
	{	
	//	buffer[strlen(buffer)] = (char)ch;
		return H1(gio, buffer, t, ch);
	}
	switch (ch)
	{
	case 47:// '/'	
		return P3(gio,buffer,t);
	
	}

	buffer[strlen(buffer)] = (char)ch;		
	return Q0(gio, buffer,t, 1);
}





/**** P2 ****/
/*!
 * Tag started good, get it's name (type)
 *
 * @param *fp : 
 * @param buffer : accumulated buffer
 * @param t : the xmlTag_type to return
 *
 * @return int  : return status
 */
int P2(gio_type *gio, char* buffer, xmlTag_type *t)
{
	int ch;
	
	ch = GIO_GETC(gio);
	while ( isNameChar(ch) )
	{
		buffer[strlen(buffer)] = (char)ch;
		ch = GIO_GETC(gio);
	}
	strncpy(t->name, buffer, 64);
	if ( isWhiteSpace(ch) )
	{
		return P4(gio,buffer,t);
	}
	
	if( ch == 47 )// '/'
		return P6(gio, buffer, t); //"<c/"

	if( ch == 62 )// '>'
//		return P8(gio, buffer, t);
		return xml_parse_tag_data(gio, buffer, t);
	
	buffer[strlen(buffer)] = (char)ch;		
	return Q0(gio, buffer, t, 1);
}


/**** P3 ****/
/*!
 * Tag started with '/', get it's name (type) and check that has no atts
 *
 * @param *fp : 
 * @param buffer : accumulated buffer
 * @param t : the xmlTag_type to return
 *
 * @return int  : return status
 */
int P3(gio_type *gio, char* buffer, xmlTag_type *t)
{
	int ch;	
	ch = GIO_GETC(gio);
	while ( isWhiteSpace(ch) )
	{
		ch = GIO_GETC(gio);
	}

	buffer[strlen(buffer)] = (char)ch;			
	if ( isNameChar(ch) )
	{
		return P5(gio, buffer, t);
	}
	return Q0(gio, buffer, t, 1);
}






/**** P4 ****/
/*!
 * Tag started with '<name ', get it's atts, or find close token (/>)
 *
 * @param *fp : 
 * @param buffer : accumulated buffer
 * @param t : the xmlTag_type to return
 *
 * @return int  : return status
 */
int P4(gio_type *gio, char* buffer, xmlTag_type *t)
{
	int ch;	
	ch = GIO_GETC(gio);
	while ( isWhiteSpace(ch) )
	{
		ch = GIO_GETC(gio);
	}
	if ( ch == 47 )// '/'
	{
		return P6(gio, buffer, t);
	}
	if ( ch == 62 )// '>'
	{
		memset (buffer, 0, 256);
//		return P8(gio, buffer, t);
		return xml_parse_tag_data(gio, buffer, t);
	}
	memset (buffer, 0, 256);
	buffer[strlen(buffer)] = (char)ch;		
	if (isNameChar(ch))
	{
		
		return P9(gio, buffer, t);
	}
	return Q0(gio, buffer, t, 3);
}




/**** P5 ****/
/*!
 * The xmlTag_type started with </ so get the name, and check that there are no atts!!!
 *
 * @param *fp : 
 * @param buffer : accumulated buffer
 * @param t : the xmlTag_type to return
 *
 * @return int  : return status
 */
int P5(gio_type *gio, char* buffer, xmlTag_type *t)
{
	int ch;	
	ch = GIO_GETC(gio);
	while ( isNameChar(ch) )
	{
		buffer[strlen(buffer)] = (char)ch;
		ch = GIO_GETC(gio);
	}
	
	//*** THIS IS STATE P7 ***
	while ( isWhiteSpace(ch) )
		ch = GIO_GETC(gio);
	if ( ch == 62 ) // '>'
	{
		t->type = enTagFooter;
		strncpy(t->name, buffer, 64);
		return Q1(gio, buffer, t, -4);
	}
	buffer[strlen(buffer)] = (char)ch;		
	return Q0(gio, buffer, t, 1);
}




/**** P6 ****/
/*!
 * Tag has pattern of "<name/" so find the ">" and close xmlTag_type.
 *
 * @param *fp : 
 * @param buffer : accumulated buffer
 * @param t : the xmlTag_type to return
 *
 * @return int  : return status
 */
int P6(gio_type *gio, char* buffer, xmlTag_type *t)
{
	int ch;	
	ch = GIO_GETC(gio);
	if ( ch == 62 )// '>'
	{
		t->type = enTagStandalone;
		return Q1(gio, buffer, t, -3);// "<name />" - standalone
	}
	buffer[strlen(buffer)] = (char)ch;		
	return Q0(gio, buffer, t, 5);
}


static int 
xml_parse_tag_data( gio_type *gio, char *buffer, xmlTag_type *t )
{
int ch; 
xmlTag_type *child;
int status;
char cBuff[256];
int HaveWhiteSpace;

char	*sp,	*sp0,	*sp1,	*sp_end;
int	bytes;
	
	ch = GIO_GETC(gio);
	// find text in the xmlTag_type
	while ( isWhiteSpace(ch) )
	{
		buffer[strlen(buffer)] = (char)ch;
		ch = GIO_GETC( gio );
	}

	if ( ch == '>' )
		return( XML_CORRUPTED_INPUT );
//		return Q0(gio,buffer, t, 1);


	HaveWhiteSpace = 0;
	
	



	if ( ch != '<' ){ // start of TAG data

		sp0 = gio->cst->rp-1;
		sp_end = (char*)CST_LAST_BYTE(gio->cst);

		for( sp = sp0+1 ; sp != sp_end && *sp != '<' ; sp++ );
		

		gio->cst->rp = sp;
		if( sp == sp_end )
			return( XML_CORRUPTED_INPUT );

		for( sp1 = sp-1 ; isWhiteSpace(*sp1) ; sp1-- );

		bytes = sp1 - sp0 + 1;

		if( bytes < 256 ){

			memcpy( t->data, sp0, bytes );
			t->data[bytes] = 0;
		}
		else	{
			t->data[0] = 0;
			t->dataP = (char *)malloc( bytes+1 );
			memcpy( t->dataP, sp0, bytes );
			t->dataP[bytes] = 0;
		}
		
		gio->cst->rp = sp+1;

		ch = *sp;
		buffer[0] = 0;
	}



	// start TAG child 
	
	memset(cBuff,0,256);
	child = xmlTag_alloc(NULL);
	status = P1(gio, cBuff, child);
	while ( status < 0  && child->type != enTagFooter )
	{
		xmlTag_add_child(t, child, t->numChildren);
		child = xmlTag_alloc(NULL);
		memset(cBuff,0,256);
		status = S0(gio, cBuff, child);	
	}


	if ( status < 0 )
	{
		if ( strcmp(child->name,t->name) == 0 )
		{
			xmlTag_destroy(child);// clean the footer.
			return Q1(gio,buffer,t, -2);
		}
		else
		{
			xmlTag_destroy(child);
			return Q0(gio,buffer,t, 6);
		}
	}

	xmlTag_destroy(child);
	return Q0(gio, buffer, t, status);
}

//THE METHOD int P7(FILE *fp, char* buffer, xmlTag_type *t) IS INLINE IN P5

/**** P8 ****/
/*!
 * GET DATA and ALL CHILDREN AND FOOTER TAG.
 * 
 *
 * @param *fp : 
 * @param buffer : accumulated buffer
 * @param t : the xmlTag_type to return
 *
 * @return int  : return status
 */
int P8(gio_type *gio, char* buffer, xmlTag_type *t)
{
	int ch; 
	xmlTag_type *child;
	int status;
	char cBuff[256];
	int HaveWhiteSpace, buflen, cBuflen;
	
	ch = GIO_GETC(gio);
	// find text in the xmlTag_type
	while ( isWhiteSpace(ch) )
	{
		buffer[strlen(buffer)] = (char)ch;
		ch = GIO_GETC( gio );
	}
	if ( ch == 62 )
		return Q0(gio,buffer, t, 1);
	HaveWhiteSpace = 0;
	
	
	if ( ch != 60 )// it's not the start of xmlTag_type, but start of raw data
	{
		buflen = 0;
		memset(buffer,0,256);// eyal 11/09 - don't accept white space at the beginning of raw data
		
		while ( buflen  < 256 && ch != 60 )
		{
			HaveWhiteSpace = 0;
			cBuflen = 0;
			while ( isWhiteSpace(ch))// gather all white spaces 
			{
				if ( ch != 13 )
				{
					cBuff[cBuflen++] = (char)ch;
					HaveWhiteSpace = 1;
				}
				ch = GIO_GETC( gio );
			}
			/*if (HaveWhiteSpace == 1)
			{
				buffer[buflen++] = ' ';// just single space
			}*/

			if (ch != 60 )// if it's not the end of the data and start of the next tag
			{
				if ( HaveWhiteSpace )
				{
					memcpy(buffer + buflen,cBuff,cBuflen);
					buflen += cBuflen;
				}
				buffer[buflen++] = (char)ch;
				ch = GIO_GETC( gio );
			}
		}		
		strncpy(t->data, buffer, 256);
		memset(buffer,0,256);
		buflen = 0;		
	}


	
	memset(cBuff,0,256);
	child = xmlTag_alloc(NULL);
	status = P1(gio, cBuff, child);
	while ( status < 0  && child->type != enTagFooter )
	{
		xmlTag_add_child(t, child, t->numChildren);
		child = xmlTag_alloc(NULL);
		memset(cBuff,0,256);
		status = S0(gio, cBuff, child);	
	}
	if ( status < 0 )
	{
		if ( strcmp(child->name,t->name) == 0 )
		{
			xmlTag_destroy(child);// clean the footer.
			return Q1(gio,buffer,t, -2);
		}
		else
		{
			xmlTag_destroy(child);
			return Q0(gio,buffer,t, 6);
		}
	}
	xmlTag_destroy(child);
	return Q0(gio, buffer, t, status);
}





/**** P9 ****/
/*!
 * have the xmlTag_type name, now get all the attributes
 *
 * @param *fp : 
 * @param buffer : accumulated buffer
 * @param t : the xmlTag_type to return
 *
 * @return int  : return status
 */
int P9(gio_type *gio, char* buffer, xmlTag_type *t)
{
	int ch;
	xmlAtt_type *att;
	ch = GIO_GETC( gio );
	// get the xmlAtt_type name
	while ( isNameChar(ch) )
	{
		buffer[strlen(buffer)] = (char)ch;
		ch = GIO_GETC( gio );
	}
	if ( isWhiteSpace(ch) )
	{
		att = xmlAtt_alloc(buffer, "");
		return P10(gio, buffer, t, att);
	}
	if ( ch == 61 )// '='
	{
		att = xmlAtt_alloc(buffer, "");
		strncpy(att->name,buffer,64);
		return P11(gio, buffer, t, att);
	}
	buffer[strlen(buffer)] = (char)ch;		
	return Q3(gio, buffer, t, NULL, 1);
}




/**** P10 ****/
/*!
 * got the name of the xmlAtt_type, now check for '=' 
 *
 * @param *fp : 
 * @param buffer : accumulated buffer
 * @param t : the xmlTag_type to return
 * @param *att: the xmlAtt_type
 *
 * @return int  : return status
 */
int P10(gio_type *gio, char* buffer, xmlTag_type *t, xmlAtt_type *att)
{
	int ch;
	ch = GIO_GETC( gio );
	
	while ( isWhiteSpace(ch) )
		ch = GIO_GETC( gio );
	if ( ch == 61 )// '='
	{
		return P11(gio, buffer, t, att);
	}

	buffer[strlen(buffer)] = (char)ch;		
	return Q3(gio, buffer, t, att, 2);
}



/**** P11 ****/
/*!
 * Get the value of the xmlAtt_type
 *
 * @param *fp : 
 * @param buffer : accumulated buffer
 * @param t : the xmlTag_type to return
 * @param *att: the xmlAtt_type
 *
 * @return int  : return status
 */
int P11(gio_type *gio, char* buffer, xmlTag_type *t, xmlAtt_type *att)
{
	int ch;
	ch = GIO_GETC( gio );
	memset(buffer ,0, 256);
	while ( isWhiteSpace(ch) )
		ch = GIO_GETC( gio );
	if (	( ch == 34 ) || /* the char : |"| */
		( ch == 39 ) || /* the char : |'| */		
		( ch == 96 ) /* the char :|`|*/
		)
		return P12(gio, buffer, t, att, ch);

	
	buffer[strlen(buffer)] = (char)ch;		
	return Q3(gio, buffer, t, att, 3);
}



/**** P12 ****/
/*!
 * Get the value of the xmlAtt_type
 *
 * @param *fp : 
 * @param buffer : accumulated buffer
 * @param t : the xmlTag_type to return
 * @param *att: the xmlAtt_type
 * @param seperator : the seperator for the value (might be |"| or |'| or |`|)
 *
 * @return int  : return status
 */
int P12(gio_type *gio, char* buffer, xmlTag_type *t, xmlAtt_type *att,  int seperator)
{
	int ch;
	memset(buffer, 0 , 256);
	ch = GIO_GETC( gio );
	while ( ch != seperator && strlen(buffer) < 255 )// '"'
	{
		buffer[strlen(buffer)] = (char)ch;
		ch = GIO_GETC( gio );
	}
	if ( ch != seperator )
		return Q3(gio,buffer, t, att, 4);//xmlAtt_type value overflow
	strncpy(att->value,buffer,256);
	xmlTag_insert_att(t, att);
	return P4(gio, buffer, t);
}



/**** P13 ****/
/*!
 * get the data of a xmlTag_type
 *
 * @param *fp : 
 * @param buffer : accumulated buffer
 * @param t : the xmlTag_type to return
 *
 * @return int  : return status
 */
int P13(gio_type *gio, char* buffer, xmlTag_type *t)
{
	int ch, stat;
	xmlTag_type *endTag;
	ch = GIO_GETC( gio );
	while ( ch != 60 && ch != 62 && ch != 38)
	{
		buffer[strlen(buffer)] = (char)ch;
		ch = GIO_GETC( gio );
	}
	if ( ch == 60 )
	{
		strncpy(t->data, buffer, 256);
		memset(buffer,0,256);
		endTag = xmlTag_alloc(NULL);
		stat = P1(gio, buffer, endTag);
		
		if ( stat < 0 && endTag->type == enTagFooter && strcmp(t->name, endTag->name))
		{
			xmlTag_destroy(endTag);
			return Q1(gio, buffer, t, -2);
		}
		xmlTag_destroy(endTag);
		return Q0(gio, buffer, t, stat);

	}

	return Q0(gio, buffer, t, 7);
}




/**** H1 ****/
/*!
 * 
 *
 * @param *gio : 
 * @param buffer : 
 * @param *t : 
 * @param delimiter: which is current delimiter ('?' or '!')
 *
 * @return int  : 
 */
int H1(gio_type *gio, char* buffer, xmlTag_type *t, int delimiter)
{
	int ch;
	
//	memset(buffer ,0,256); !!!!! don't do it - we get here from C1 also, with data in buffer.
	
	while ( strlen(buffer) < 256)
	{
		ch = GIO_GETC( gio );
		while (strlen(buffer) < 256 &&  (ch != delimiter ) && (ch != 62)) //'?' or '!' or '>'
		{
			buffer[strlen(buffer)] = (char)ch;
			ch = GIO_GETC( gio );
		}		
		if ( ch == delimiter ) 
			ch = GIO_GETC( gio );
		while ( isWhiteSpace(ch) ) 
			ch = GIO_GETC( gio );
		if ( ch == 62 ) 
		{
			t->type = enTagHeader;
			strncpy(t->data,buffer,256);
			t->headerDelimiter = delimiter;
			return Q1(gio,buffer,t,-5);
		}
		else
		{			
			buffer[strlen(buffer)] = (char)ch;
			return Q0(gio, buffer, t, 9);
		}
	}
	return Q0(gio,buffer,NULL, 1);
	
}


/**** C1 ****/
/*!
 * Comment state
 *
 * @param *gio : data stream
 * @param buffer : accumulated buffer
 * @param t : the xmlTag_type to return
 *
 * @return int  : return status
 */
int C1(gio_type *gio, char* buffer, xmlTag_type *t)
{
	int ch;
	ch = GIO_GETC( gio );
	if ( ch != 45 ) //'-' so it is a header...
	{	
		buffer[strlen(buffer)] = (char)ch;
		return H1(gio, buffer, t, 33/* '!' */);		
	}
	
	ch = GIO_GETC( gio );
	if ( ch != 45 ) //'-'
		return Q0(gio,buffer,t,2);

	// ok, we have <!--
	// keep gainin data, till meeting "-->" ending
	t->type = enTagComment;
	memset(buffer ,0,256);
	while ( strlen(buffer) < 256 ) 
	{
		ch = GIO_GETC( gio );	
		if ( ch == 45 )// '-'
		{
			ch = GIO_GETC( gio );	
			if ( ch == 45 )// '--'
			{
				ch = GIO_GETC( gio );	
				if ( ch ==  62)// '-->' finished comment			
				{
					strncpy(t->data,buffer,256);
					return Q1(gio,buffer,t,-4);
				}
				else
				{
					buffer[strlen(buffer)] = (char)45;
					buffer[strlen(buffer)] = (char)45;
					buffer[strlen(buffer)] = (char)ch;
				}
			}		
			else
			{
				buffer[strlen(buffer)] = (char)45;
				buffer[strlen(buffer)] = (char)ch;
			}
		}
		else
			buffer[strlen(buffer)] = (char)ch;

	}
	return Q0(gio,buffer,t,8);
	
}