#ifndef _UINF_

#define _UINF_

#include	<float.h>



typedef struct inf {
	int	num	;
	float	min	;   
	float	max	;  
        float   sum	;
        float   abs_sum	;
        float   sum2	;
} inf;

static inf	Inf_init = { 0, FLT_MAX, -FLT_MAX, 0.0, 0.0, 0.0 };


/* uinf.c  */

void	inf_init( inf *st );

void	inf_update( float val, inf *sinf );

void	inf_print(  FILE *fp, char *s, inf *sinf, float	nf );

void	inf_print_array( FILE *fp, char *s, int a[], int no );

void	inf_print_bits( FILE *fp, char *s, int bits, int no );


 		
#endif /* #ifndef _UINF_ */
