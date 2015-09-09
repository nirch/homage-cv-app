/**********************
 ***	CstXml.c   ***
 **********************/
#include	"Uigp/igp.h"

#include	"CstType.h"



void	
cst_put_xml_tag_int( cst_type *cst, char *name, int value )
{
char	buf[100];

	sprintf( buf, "<%s>%d</%s>\n", name, value, name );
	cst_put_string( cst, buf );
}

void	
cst_put_xml_tag_string( cst_type *cst, char *name, char *value )
{
char buf [256];
	if (value)
		sprintf( buf, "<%s>%s</%s>\n", name, value, name );
	else
		sprintf( buf, "<%s>0</%s>\n", name,name );
	cst_put_string( cst, buf );
}

void	
cst_put_xml_tag_stringS( cst_type *cst, char *name, char *value, int byte_no )
{
char	buf[256];

	sprintf( buf, "<%s>\n", name );
	cst_put_string( cst, buf );

	cst_put_buffer( cst, value, byte_no );


	sprintf( buf, "</%s>\n", name );
	cst_put_string( cst, buf );
}

void	
cst_put_xml_att_string( cst_type *cst, char *name, char *value )
{
char	*p;
	CST_PUT_CHAR( cst , ' ' );
	for( p = name ; *p != 0 ; p++ )
		CST_PUT_CHAR( cst , *p);

	CST_PUT_CHAR( cst , '=' );

	CST_PUT_CHAR( cst , '\"' );
	for( p = value ; *p != 0 ; p++ )
		CST_PUT_CHAR( cst , *p);

	CST_PUT_CHAR( cst , '\"' );
}


void	
cst_put_xml_att_int( cst_type *cst, char *name, int value, char *ext )
{
char	buf[64],	*p;
	CST_PUT_CHAR( cst , ' ' );
	for( p = name ; *p != 0 ; p++ )
		CST_PUT_CHAR( cst , *p);

	CST_PUT_CHAR( cst , '=' );

	CST_PUT_CHAR( cst , '\"' );

//	itoa( value, buf, 10 );
	sprintf( buf, "%d", value );
	for( p = buf ; *p != 0 ; p++ )
		CST_PUT_CHAR( cst , *p);

	if( ext != NULL )
		for( p = ext ; *p != 0 ; p++ )
			CST_PUT_CHAR( cst , *p);

	CST_PUT_CHAR( cst , '\"' );
}


void	
cst_put_xml_att_float( cst_type *cst, char *name, float value )
{
char	buf[64],	*p;
	CST_PUT_CHAR( cst , ' ' );
	for( p = name ; *p != 0 ; p++ )
		CST_PUT_CHAR( cst , *p);

	CST_PUT_CHAR( cst , '=' );

	CST_PUT_CHAR( cst , '\"' );

	sprintf( buf, "%f", value );
	for( p = buf ; *p != 0 ; p++ )
		CST_PUT_CHAR( cst , *p);

	CST_PUT_CHAR( cst , '\"' );
}

void	
cst_put_xml_att_openA( cst_type *cst, int align, char *name )
{
char	buf[100];
int	i;

	for( i = 0 ; i < align ; i++ )	buf[i] = '\t';

	sprintf( buf+align, "<%s", name );
	cst_put_string( cst, buf );
}

void	
cst_put_xml_tag_openA( cst_type *cst, int align, char *name )
{
char	buf[100];
int	i;

	for( i = 0 ; i < align ; i++ )	buf[i] = '\t';

	sprintf( buf+align, "<%s>\n", name );
	cst_put_string( cst, buf );
}

void	
cst_put_xml_tag_closeA( cst_type *cst, int align, char *name )
{
char	buf[100];
int	i;

	for( i = 0 ; i < align ; i++ )	buf[i] = '\t';

	sprintf( buf+align, "</%s>\n", name );
	cst_put_string( cst, buf );
}


void	
cst_put_xml_tag_intA( cst_type *cst, int align, char *name, int value )
{
char	buf[100];
int	i;

	for( i = 0 ; i < align ; i++ )	buf[i] = '\t';

	sprintf( buf+align, "<%s>%d</%s>\n", name, value, name );
	cst_put_string( cst, buf );
}

void	
cst_put_xml_tag_floatA( cst_type *cst, int align, char *name, float value )
{
char	buf[100];
int	i;

	for( i = 0 ; i < align ; i++ )	buf[i] = '\t';

	sprintf( buf+align, "<%s>%.4f</%s>\n", name, value, name );
	cst_put_string( cst, buf );
}


void	
cst_put_xml_tag_stringA( cst_type *cst, int align, char *name, char *value )
{
//char	buf[100];
int	i;

	for( i = 0 ; i < align ; i++ )
		CST_PUT_CHAR( cst , '\t' );

	CST_PUT_CHAR( cst , '<' );
	cst_put_string( cst, name );
	CST_PUT_CHAR( cst , '>' );

	cst_put_string( cst, value );


	CST_PUT_CHAR( cst , '<' );
	CST_PUT_CHAR( cst , '//' );
	cst_put_string( cst, name );
	CST_PUT_CHAR( cst , '>' );
	CST_PUT_CHAR( cst , '\n' );
}



void	
cst_put_xml_tag_openB( cst_type *cst,char *name )
{
char	buf[100];

	sprintf( buf, "<%s>", name );
	cst_put_string( cst, buf );
}

void	
cst_put_xml_tag_closeB( cst_type *cst, char *name )
{
char	buf[100];

	sprintf( buf, "</%s>", name );
	cst_put_string( cst, buf );
}


void	
cst_put_xml_tag_intB( cst_type *cst, char *name, int value )
{
char	buf[100];


	sprintf( buf, "<%s>%d</%s>", name, value, name );
	cst_put_string( cst, buf );
}

void	
cst_put_xml_tag_floatB( cst_type *cst, char *name, float value )
{
char	buf[100];


	sprintf( buf, "<%s>%.4f</%s>", name, value, name );
	cst_put_string( cst, buf );
}


void	
cst_put_xml_tag_stringB( cst_type *cst, char *name, char *value )
{
	CST_PUT_CHAR( cst , '<' );
	cst_put_string( cst, name );
	CST_PUT_CHAR( cst , '>' );

	cst_put_string( cst, value );


	CST_PUT_CHAR( cst , '<' );
	CST_PUT_CHAR( cst , '//' );
	cst_put_string( cst, name );
	CST_PUT_CHAR( cst , '>' );
}
