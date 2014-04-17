/************************
 ***   SmilParser.c   ***
 ************************/

#include <stdlib.h>
#include <string.h>

#include	"Uigp/igp.h"

#include	"Ucst/CstType.h"


#include	"CfgType.h"


static void		cst_put_cfg_stringA( cst_type *cst, int align, char *name, char *value );


static void	cfg_build( cfg_type *cfg, int align, cst_type *cst );



int
cfg_write_file( cfg_type *cfg, char *file )
{
cst_type *cst;
	

	cst = cst_alloc( 10000 );

	cfg_build( cfg, 0, cst );


	cst_write_to_file( cst, file );

	cst_destroy( cst, 1 );

	return( 1 );
}


static void
cfg_build( cfg_type *cfg, int align, cst_type *cst )
{
cfg_type *c;
cfgAttr_type	*a;

	for( a = cfg->pAttr ; a != NULL ; a = a->next )
		cst_put_cfg_stringA( cst, align, a->name, a->data );


	for( c = cfg->pCfg ; c != NULL ; c = c->next ){

		cst_put_xml_tag_openA( cst, align, c->name );

		cfg_build( c, align+1, cst );

		cst_put_xml_tag_closeA( cst, align, c->name );
	}
}


static void	
cst_put_cfg_stringA( cst_type *cst, int align, char *name, char *value )
{
int	i;

	for( i = 0 ; i < align ; i++ )
		CST_PUT_CHAR( cst , '\t' );

	cst_put_string( cst, name );
	CST_PUT_CHAR( cst , '=' );

	cst_put_string( cst, value );

	CST_PUT_CHAR( cst , '\n' );
}
