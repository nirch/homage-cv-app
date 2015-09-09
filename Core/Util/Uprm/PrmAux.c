/*********************
 ***	PrmAux.c   ***
 *********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "Uigp/igp.h"
#include "prm.h"     




static void	gp_prm_print_parameter(FILE * fd, GP_prm * prm);


void 
gp_prm_dump_defaults_file(char *filename, GP_prm * params, int n_params)
{
FILE   *fp;
GP_prm *prm;
int     n;

	if ((fp = fopen(filename, "a")) == NULL) {
		return;
	}

	fprintf(fp, "\n\n\n" );
	fprintf(fp, "------------\n" );


	for (n = 0; n < n_params; n++) {
		prm = params + n;

		/* dump name of parameter */
		fprintf(fp, "!  \"%s\",\t\t", prm->name);
		fprintf(fp, "\"%s\"\n", prm->default_str);


		/* dump content of parameter */
	}
	fclose(fp);
}



void 
gp_prm_print_params(FILE * fd, GP_prm * params, int n_params)
{
	int     i;
	GP_prm *prm;

	for (i = 0; i < n_params; i++) {
		prm = params + i;
		fprintf(fd, "\n%s ", prm->name);

		gp_prm_print_parameter(fd, prm);
	}
	fprintf(fd, "\n");
}





static void 
gp_prm_print_parameter(FILE * fd, GP_prm * prm)
{

	switch (prm->type) {
		case PRM_INTEGER:fprintf(fd, prm->format,*((int *) prm->value));
	    break;

	case PRM_BOOLEAN:
		if ((*(int *) prm->value) == PRM_BOOLEAN_FALSE)
			fprintf(fd, "True");
		else
			fprintf(fd, "False");
	    break;

	case PRM_FLOAT:
		fprintf(fd, prm->format, *((float *) prm->value));
	    break;

	case PRM_STRING:
		fprintf(fd, prm->format, prm->value);
	    break;
	}
}
