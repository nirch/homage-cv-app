/******************
 ***	prm.c   ***
 ******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "Uigp/igp.h"
#include "prm.h"     



#define MAX_STR 100



static int	valid_default(char *buffer, char *name, char *default_val );

static int	match_char(char *buffer, int from, char c, int *location);

static void	update_default(GP_prm * params, int n_params, char *name,
		       char *default_val );



void 
gp_prm_set_defaults(GP_prm * params, int n_params)
{
int     i;
GP_prm *prm;
char    c;

	for (i = 0; i < n_params; i++) {

		prm = params + i;

		gp_prm_validity_check(prm);

		switch (prm->type) {
		case PRM_INTEGER:
			*((int *) prm->value) = atoi(prm->default_str);
		    break;

		case PRM_BOOLEAN:
//			c = tolower(prm->default_str[0]);
			c = prm->default_str[0];
			*((int *) prm->value) = ( c == 'y' || c == 'Y' || c == 't' || c == 'T') ?
				PRM_BOOLEAN_TRUE : PRM_BOOLEAN_FALSE;
		    break;
		case PRM_FLOAT:
			*((float *) prm->value) =
					(float) atof(prm->default_str);
		    break;
		case PRM_STRING:
			strcpy((char *) prm->value, prm->default_str);
		    break;
		}
	}
}


int 
gp_prm_parse_defaults_file(char *filename, GP_prm *prm, int prm_no )
{
FILE   *fp;
char    buffer[MAX_STR];
char    name[MAX_STR], default_val[MAX_STR];
#ifdef _DEBUG1
	char	file[256];
#endif
	if( filename == NULL )
		return( -1 );


	if ((fp = fopen(filename, "r")) == NULL) {
/*
#ifdef _DEBUG1
		sprintf(buffer, "Cannnot open file : %s", filename);
		warning("gp_prm_parse_defaults_file", buffer);
#endif
*/
		return( -1 );
	}

	/* parse   */
	while (fgets(buffer, MAX_STR, fp))
		if( valid_default(buffer, name, default_val ) )
			update_default( prm, prm_no, name, default_val );

	fclose(fp);


#ifdef _PRM_DUMP
	sprintf( file, "%s.dump", filename );
	gp_prm_dump_defaults_file( file, prm, prm_no );
#endif
	return( 1 );
}




int 
gp_prm_validity_check(GP_prm * prm)
{
int     validity;

	validity = PRM_VALID;

	if (prm->name == NULL)
		return( PRM_INVALID );

	if (prm->value == NULL)
		return( PRM_INVALID );

	if (prm->type != PRM_INTEGER &&
	    prm->type != PRM_BOOLEAN &&
	    prm->type != PRM_FLOAT &&
	    prm->type != PRM_STRING) 
		return( PRM_INVALID );

	if (prm->format == NULL)
		return( PRM_INVALID );

	if (prm->default_str == NULL)
		return( PRM_INVALID );

	if (strlen(prm->default_str) > PRM_DEFAULT_STRING_MAX_SIZE)
		return( PRM_INVALID );

	return (validity);
}




/*
 * Private functions, Not exported.
 */


static int
valid_default(char *buffer, char *name, char *default_val )
{
int     start, end;

	if( *buffer != '!' )	return( 0 );


	/* get the name  */
	if (!match_char(buffer, 0, '\"', &start))
		return (PRM_INVALID);

	if (!match_char(buffer, start + 1, '\"', &end))
		return (PRM_INVALID);

	strncpy(name, buffer + start + 1, end - start - 1);
	name[end - start - 1] = '\0';

	/* get the default  */
	if (!match_char(buffer, end + 1, '\"', &start))
		return (PRM_INVALID);

	if (!match_char(buffer, start + 1, '\"', &end))
		return (PRM_INVALID);

	strncpy(default_val, buffer + start + 1, end - start - 1);
	default_val[end - start - 1] = '\0';

	return (PRM_VALID);
}



static int
match_char(char *buffer, int from, char c, int *location)
{

	*location = from;

	while (buffer[*location])
		if (buffer[*location] == c)
			return (PRM_VALID);
		else
			(*location)++;

	return (PRM_INVALID);
}



static void
update_default(GP_prm * params, int n_params, char *name,
	       char *default_val )
{
int     i;

        if (strlen(default_val) > PRM_DEFAULT_STRING_MAX_SIZE)
                return;

	for (i = 0; i < n_params; i++) {

		if( strcmp( name, params[i].name ) != 0 )	continue;
		

		strcpy( params[i].default_str, default_val );
	}
}





int
gp_prm_init(char *file,
       void (*get_prm) (GP_prm ** prm, int *prm_no, void (**check_func) ()))
{
GP_prm *prm;
int     prm_no;
void    (*check_func) ();

	(get_prm) (&prm, &prm_no, &check_func);
	if (prm) {
		//if( gp_prm_parse_defaults_file(file, prm, prm_no) < 0 )
		//	return( -1 );
		gp_prm_parse_defaults_file(file, prm, prm_no );
		gp_prm_set_defaults(prm, prm_no);
	}

	return( 1 );
}
