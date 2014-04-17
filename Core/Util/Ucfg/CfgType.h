/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef __CFG_TYPE__
#define __CFG_TYPE__

#ifdef __cplusplus
extern "C" {
#endif




#define	CFG_MAX_NAME	64
#define	CFG_MAX_DATA	256




typedef struct cfgAttr_type 
{
	char name[CFG_MAX_NAME];
	char data[CFG_MAX_DATA];


	struct cfgAttr_type	*next;

}	cfgAttr_type;




typedef struct cfg_type 
{
	char name[CFG_MAX_NAME];


	cfgAttr_type	*pAttr;

	struct cfg_type	*pCfg;


	struct cfg_type	*next;

}	cfg_type;



	/* CfgTool.c */
cfg_type *	cfg_alloc();

cfgAttr_type	*	cfgAttr_alloc();

void	cfg_destroy( cfg_type *cfg );


cfg_type	*	cfg_get_container ( cfg_type *cfg, char *name );
cfg_type	*	cfg_get_next_container ( cfg_type *cfg, char *name, cfg_type *pcfg );


cfgAttr_type	*	cfg_get_attribute( cfg_type *cfg, char *name );


	/* CfgParser.c */
cfg_type *	cfg_parser_file( char *file );

	/* CfgWrite.c */
int		cfg_write_file( cfg_type *cfg, char *file );



#ifdef __cplusplus
}
#endif

#endif
