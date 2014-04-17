/******************
 ***	code.h	***
 ******************/

#ifndef		_UCODE_
#define		_UCODE_

#include        <stdio.h>   

#include       "Ucst/CstType.h"
/*
extern struct cst_type;
*/



typedef	struct code_in {
	float	min,	max,	d;
	float	a, f;
} code_in;



typedef	struct code {
	float	min,	max,	d,	inv_d;
	float	a;

	int	imax,	hr,	rbit;

	char	fname[64];

	FILE	*fp;
	int	bits;
	int	msgs;

	struct cst_type	*cst;
} code;

typedef	code code_type;


typedef	struct cfile_type {
	char	fname[64];
	int	fd;
	FILE	*fp;
	int	bits;
	int	msgs;
	struct cst_type	*cst;
} cfile_type;






#define	CODE( x, c )	 	( (x - (c).min) *(c).inv_d )
#define	UNCODE( cno, c )	(( (cno) + 0.5) *(c).d + (c).min )
#define QCODE( x, c )		( ( (x) < 0 )?  \
				(( ((int)((x)*(c).inv_d)) - 0.5) *(c).d ) : \
				(( ((int)((x)*(c).inv_d)) + 0.5) *(c).d ) )


#define	ZCODE( x, c )	 	( ( (x) < 0 )? \
				  ( ( (x) - 0.5*(c).d )*(c).inv_d ) : \
				  ( ( (x) + 0.5*(c).d )*(c).inv_d ) )
#define	UNZCODE( cno, c )	( (cno) *(c).d )

#define	QZCODE( x, c )		( ((int)ZCODE( x, c )) *(c).d )



void	code_set( code *c, float min, float max, float d, float avarege, float nf );


void	coding_set_lprm( code_in *c_in, code *c, float	nf );

/*
void	coding_set_data_lprm( code_in *c_in, code *c, float nf,
                                                        char *file_name );
*/
void	code_file_set( cfile_type *c, char *file_name );

int	coding_ilog2( int a );


#endif  /* of _UCODE_ */
