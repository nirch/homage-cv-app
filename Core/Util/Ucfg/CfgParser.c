/************************
 ***   SmilParser.c   ***
 ************************/

#include <stdlib.h>
#include <string.h>

#include	"Uigp/igp.h"

#include	"Ucst/CstType.h"


#include	"CfgType.h"

#define		IS_SPACE0( p )	( (p) == ' ' || (p) == '\t' )

#define		IS_SPACE( p )	( (p) == ' ' || (p) == '\t' || (p) == '\r' || (p) == '\n' )

static int	cfg_parser( cfg_type *cfg, cst_type *cst );

static int	cfg_parser_token( cst_type *cst, char *name, char *data );

static int	cst_skip_space( cst_type *cst );
static int	cst_skip_space0( cst_type *cst );



static int	cst_skip_line( cst_type *cst );

static int	LineNo;
static int	ContainerLineNo;

#define	CFG_EOF								-10
#define	CFG_UNEXPETED_EOF					-11
#define	CFG_TOKEN_TO_LONG					-12
#define CFG_UNEXPETED_EOL					-13
#define CFG_CONTAINER_NOT_CLOSE				-14
#define CFG_UNEXPETED_CLOSE_CONTAINER_TAG	-15
#define CFG_EMPTY_DATA						-16


cfg_type *
cfg_parser_file( char *file )
{
cst_type *cst;
cfg_type *cfg;
int	ret;
	
	LineNo = 0;

	if( (cst = cst_load_from_file(  file )) == NULL ){
		fprintf( stderr, "Cfg Parser: Could Not open config file" );
		return( NULL );
	}

	cfg = cfg_alloc();


	ret = cfg_parser( cfg, cst );
	cst_destroy( cst, 1 );

	if( ret == CFG_EOF )	return( cfg );
/*
	if( ret == -2 || cfg->pAttr == NULL && cfg->pCfg == NULL ){
		cfg_destroy( cfg );
		return( NULL );
	}


	return( cfg );
*/

	switch( ret ){
	case CFG_UNEXPETED_EOF:
			fprintf( stderr, "Cfg Parser: Unexpeted end of file\n" );
		break;

	case CFG_TOKEN_TO_LONG:
		fprintf( stderr, "Cfg Parser: Line: %d - String too long\n", LineNo+1 );
		break;

	case CFG_UNEXPETED_EOL:
		fprintf( stderr, "Cfg Parser: Line: %d - Unexpeted end of line\n", LineNo+1 );
		break;

	case CFG_CONTAINER_NOT_CLOSE:
		fprintf( stderr, "Cfg Parser: Line: %d - No closing tag\n", ContainerLineNo+1 );
		break;

	case CFG_UNEXPETED_CLOSE_CONTAINER_TAG:
		fprintf( stderr, "Cfg Parser: Line: %d - Non matching closing tag\n", LineNo+1 );
		break;

//	case CFG_EMPTY_DATA:
//		fprintf( stderr, "Cfg Parser: Line: %d - Empty data", LineNo+1 );
//		break;
		
	}

	return( NULL );
}


static int
cfg_parser( cfg_type *cfg, cst_type *cst )
{
char	name[CFG_MAX_NAME],	data[CFG_MAX_DATA];
cfg_type *c;
cfgAttr_type *a;
int	ret;

	while( 1 ){
		ret = cfg_parser_token( cst, name, data );
		if( ret < 0 )	return( ret );

		switch( ret ){
		case 1:
			c = cfg_alloc();
			strncpy( c->name, name, CFG_MAX_NAME );
			ContainerLineNo = LineNo;

			if( (ret = cfg_parser( c, cst )) < 0 ){
				cfg_destroy( c );
				if( ret == -1 )	return( CFG_CONTAINER_NOT_CLOSE );
				return( ret );
			}

			c->next = cfg->pCfg;
			cfg->pCfg = c;
			break;

		case 2:
			if( strcmp( cfg->name, name ) == 0 )	return( 1 );
			return( CFG_UNEXPETED_CLOSE_CONTAINER_TAG );
			break;

		case 3:
			a = cfgAttr_alloc();
			strncpy( a->name, name, CFG_MAX_NAME );
			strncpy( a->data, data, CFG_MAX_DATA );

			a->next = cfg->pAttr;
			cfg->pAttr = a;
			break;
		}
	}
}


static int
cfg_parser_token( cst_type *cst, char *name, char *data )
{
u_char	*end = CST_END_BYTE(cst);//cst->data + cst->byte_no;
int	i,		ret;
char	*p,	*p0,	*p1;

	if( (ret = cst_skip_space( cst )) < 0 )
		return( ret );


	while( *cst->rp == '#' ||  *cst->rp == '/' && *(cst->rp+1) == '/' ){
		if( (ret = cst_skip_line( cst )) < 0 )	return( ret );
		if( (ret = cst_skip_space( cst )) < 0 )	return( ret );
	}


	if( *cst->rp == '<' ){
		ret = 1;
		cst->rp++;
		if( *cst->rp == '/' ){
			ret = 2;
			cst->rp++;
		}

		for( i = 0 ; i < CFG_MAX_NAME && *cst->rp != '>' ; i++, cst->rp++ ){
			if( cst->rp == end )
				return( CFG_UNEXPETED_EOF );
			name[i] = *cst->rp;
		}
		if( i == CFG_MAX_NAME )	return( CFG_TOKEN_TO_LONG );
		cst->rp++;
		name[i] = 0;
		return( ret );
	}



	for( i = 0 ; i < CFG_MAX_NAME && *cst->rp != '=' ; i++, cst->rp++ ){
		if( cst->rp == end ){
			if( i == 0 )	return( CFG_EOF );
			return( CFG_UNEXPETED_EOF );
		}
		if( *cst->rp == '\n' )	return( CFG_UNEXPETED_EOL );
		name[i] = *cst->rp;
	}
	if( i == CFG_MAX_NAME )	return( CFG_TOKEN_TO_LONG );

	for( ; i > 0 && IS_SPACE(name[i-1]) ; i-- );
	name[i] = 0;
	cst->rp++;


	if( (ret = cst_skip_space0( cst )) < 0 )	return( ret );

	p0 = (char *)cst->rp;
	ret = cst_skip_line( cst );
	p1 = (char *)cst->rp;

//	for( p1 = p0 ; *p1 != '\n' && *p1 != '/' && *(p1+1) != '/' ; p1++ )
//		if( p1 == cst->data + cst->byte_no )	break;
		

	for( p = p0 ; p != p1 && (*p != '/' || *(p+1) != '/') ; p++ );
	p1 = p;

	for( ; p1  != p0 && IS_SPACE(*(p1-1)) ; p1-- );
	
	for( i = 0 ; i < CFG_MAX_DATA && p0 != p1 ; i++, p0++ ){
		if( cst->rp == end )
			return( CFG_UNEXPETED_EOF );
		data[i] = *p0;
	}

/*
	for( i = 0 ; i < CFG_MAX_DATA && !IS_SPACE(*cst->rp) ; i++, cst->rp++ ){
		if( cst->rp == end )
			return( CFG_UNEXPETED_EOF );
		data[i] = *cst->rp;
	}
*/

	if( i == CFG_MAX_DATA )	return( CFG_TOKEN_TO_LONG );
//	if( i == 0 )			return( CFG_EMPTY_DATA );
	data[i] = 0;
//	cst->rp++;

	return( 3 );
}


static int
cst_skip_space0( cst_type *cst )
{
u_char	*end = CST_END_BYTE(cst);//cst->data + cst->byte_no;

	for( ;  IS_SPACE0( *cst->rp ) ; cst->rp++ ){
		if( cst->rp == end )	return( CFG_EOF );
	}
	return( 1 );
}


static int
cst_skip_space( cst_type *cst )
{
u_char	*end = CST_END_BYTE(cst);//cst->data + cst->byte_no;

	for( ;  IS_SPACE( *cst->rp ) ; cst->rp++ ){
		if( cst->rp == end )	return( CFG_EOF );
		if( *cst->rp == '\n' )	LineNo++;
	}
	return( 1 );
}


static int
cst_skip_line( cst_type *cst )
{
u_char	*end = CST_END_BYTE(cst);//cst->data + cst->byte_no;

	for( ;  *cst->rp != '\n' ; cst->rp++ )
		if( cst->rp == end )	return( CFG_EOF );

	cst->rp++;
	LineNo++;

	return( 1 );
}

