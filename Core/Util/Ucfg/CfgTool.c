#include	<stdlib.h>
#include	<string.h>

#include	"Uigp/igp.h"
#include	"CfgType.h"




cfg_type *
cfg_alloc()
{
cfg_type	*cfg;

	cfg = ( cfg_type *)malloc( sizeof( cfg_type) );

	cfg->pAttr = NULL;
	cfg->pCfg = NULL;
	cfg->next = NULL;

	return( cfg );
}




cfgAttr_type	*
cfgAttr_alloc()
{
cfgAttr_type	*a;

	a = ( cfgAttr_type *)malloc( sizeof( cfgAttr_type) );

	a->next = NULL;
	return( a );
}



cfg_type	*
cfg_get_container ( cfg_type *cfg, char *name )
{
cfg_type *a;

	for( a = cfg->pCfg ; a != NULL ; a = a->next )
		if( gp_stricmp( a->name, name ) == 0 )	return( a );

	return( NULL );
}

cfg_type	*
cfg_get_next_container ( cfg_type *cfg, char *name, cfg_type *pcfg )
{
cfg_type *a;

	
	for( a = (pcfg == NULL)? cfg->pCfg: pcfg->next ; a != NULL ; a = a->next )
		if( gp_stricmp( a->name, name ) == 0 )	return( a );

	return( NULL );
}

cfgAttr_type	*
cfg_get_attribute( cfg_type *cfg, char *name )
{
cfgAttr_type *a;

	for( a = cfg->pAttr ; a != NULL ; a = a->next )
		if( gp_stricmp( a->name, name ) == 0 )	return( a );

	return( NULL );
}




void
cfg_destroy( cfg_type *cfg )
{
cfgAttr_type	*a;
cfg_type	*c;

	while( (a = cfg->pAttr) != NULL ){
		cfg->pAttr = cfg->pAttr->next;
		free( a );
	}

	while( (c = cfg->pCfg) != NULL ){
		cfg->pCfg = cfg->pCfg->next;
		cfg_destroy( c );
	}

	free( cfg );
}



