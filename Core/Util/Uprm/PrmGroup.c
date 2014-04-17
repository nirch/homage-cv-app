/***********************
 ***	PrmGroup.c   ***
 ***********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "Uigp/igp.h"
#include "prm.h"     




void 
gp_prm_group_set_defaults(GP_prm_group * prm_group, int n_groups)
{
	int     i;
	GP_prm *prm;
	int     prm_no;
	void    (*check_func) ();

	for (i = 0; i < n_groups; i++) {
		(prm_group[i].get_prm) (&prm, &prm_no, &check_func);
		gp_prm_set_defaults(prm, prm_no);
	}
}




void 
gp_prm_group_parse_defaults_file(char *filename,
			 GP_prm_group * prm_group, int n_groups)
{
int     i;
GP_prm *prm;
int     prm_no;
void    (*check_func) ();

	for (i = 0; i < n_groups; i++) {
		(prm_group[i].get_prm) (&prm, &prm_no, &check_func);
		gp_prm_parse_defaults_file(filename, prm, prm_no);
	}
}






void 
gp_prm_append(GP_prm prm[], int *prm_no,
	      void (*get_prm) (GP_prm ** prm, int *prm_no,
			       void (**check_func) ()))
{
	GP_prm *cprm;
	int     cprm_no;
	void    (*ccheck_func) ();

	(get_prm) (&cprm, &cprm_no, &ccheck_func);
	memcpy((void *) &prm[*prm_no], (void *) cprm, sizeof(GP_prm) * cprm_no);
	*prm_no += cprm_no;
}
