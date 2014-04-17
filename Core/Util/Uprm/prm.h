/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef _GP_PRM_H_
#define _GP_PRM_H_

#include	<stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *   Handling parameters .
 *      Usefull for non-interface algorithms as well as for interface .
 * /


/*
 *   When using the parameters-handling utility in the interface, the user
 *        MUST obey the following restrictions:
 *   ------------------------------------------
 *     1. Do NOT define PANEL_EVENT_PROC for the panel item for which the
 *           parameters window is attached (since the utility does it for you).
 *     2. Do NOT define PANEL_CLIENT_DATA for that panel item, since the
 *           utilty uses this attribute for internal use.
 *     3. Recomandation: Pass a VALID parent frame ( not NULL ) for the function
 *           gp_prm_mmi_attach_to_panel_item(), in order to cause the parameters
 *           window to be a SUB-frame. This prevents destroying the window
 *           using the frame's title bar - a situation the utility is not
 *           informed about.
 */


/*
 *   Supported parameters types. 
 */

#define		PRM_INT		0
#define		PRM_BOOLEAN	1
#define		PRM_FLOAT	2
#define		PRM_STRING	3

#define		PRM_INTEGER	PRM_INT

/*
 *   Other constants.
 */

#define PRM_DEFAULT_STRING_MAX_SIZE 80

#define PRM_BOOLEAN_FALSE 0
#define PRM_BOOLEAN_TRUE  1

#define PRM_INVALID       0
#define PRM_VALID         1

/* 
 *   Interface constants.  
 */

#define PRM_DISPLAY_LENGTH       10
#define PRM_FRAME_LABEL_MAX_SIZE 100

/*
 *    A macro to descover number of elements in an array of structures.
 *      The array size must be known at compile time.
 */

#define PRM_NUMBER(array)   ( sizeof(array) / sizeof(array[0]) )
#define PRM_GROUP_SIZE(array)   ( sizeof(array) / sizeof(array[0]) )



/*
 *    Typedefs.
 */

typedef struct {
    char    *name;        /* A string containing the parameter's name    */
    char    *value;       /* A pointer to the parameter (strings memory 
                               must be allocated before).                */
    int     type;         /* One of the Supported types above            */
    char    *format;      /* A string format like in 'fprintf'.          */
    char    default_str[PRM_DEFAULT_STRING_MAX_SIZE + 1];
            /* A string with the default value.  */
}GP_prm;



typedef struct {
	char *name;
	void (*get_prm)( GP_prm **prm, int *prm_no, void (**check_func)());
} GP_prm_group;





typedef void (*GetPrmProc)( GP_prm **prm, int *prm_no, void (**check_func)());
typedef void (*GetPrmGroupProc)( GP_prm_group **prm_group, int *prm_group_no );





/*
 *     declaration of exported functions.
 */

int	gp_prm_init( char *file,
		void (*get_prm)(GP_prm **prm, int *prm_no, void (**check)() ));



int	gp_prm_parse_defaults_file( char  *file, GP_prm *prm, int prm_no );


void	gp_prm_set_defaults( GP_prm *prm, int prm_no );


int	gp_prm_validity_check( GP_prm *prm);



	/* PrmGroup.c */

void	gp_prm_group_set_defaults( GP_prm_group *prm_group, int n_groups );

void	gp_prm_group_parse_defaults_file( char *file,
			GP_prm_group *prm_group, int group_no );

void	gp_prm_append( GP_prm  prm[], int *prm_no,
		   void (*get_prm)(GP_prm **prm, int *prm_no,
				  void (**check_func)() ) 
		   );
 



	/* PrmAux.c */
void	gp_prm_dump_defaults_file( char *file, GP_prm *prm , int prm_no);

void	gp_prm_print( FILE *fd, GP_prm *params, int n_params );


#ifdef __cplusplus
}
#endif

#endif
