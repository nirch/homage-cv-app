#include	<stdio.h>
#include	<math.h>

#include	"uinf.h"


void inf_init( inf *st )
{
	*st = Inf_init;
}


void inf_update( float 	val, inf	*sinf )
{
 	sinf->num ++;
	sinf->sum += val;
	sinf->sum2 += val*val;

	if( val > 0 )
		sinf->abs_sum += val;
	else	sinf->abs_sum -= val;

	if (sinf->min > val)
		sinf->min = val ;  

        if (sinf->max < val)
		sinf->max = val ;
}



void
inf_print(  FILE *fp, char *s, inf *sinf, float nf )
{
float	av,	var,	var2;

	if( sinf->num == 0 ){
		fprintf(fp, "%-10.10s: %5d    M: %6.2f %6.2f    Av: %6.2f   A: %6.2f   V: %6.2f\n", 
			s,
			0, 0.0, 0.0, 0.0, 0.0, 0.0 );
		return;
	}


	av = sinf->sum*nf/sinf->num;
	var2 = sinf->sum2*nf*nf/sinf->num - av*av;
	var = ( var2 < 0 )? 0 : (float)sqrt(var2);

	fprintf(fp, "%-10.10s: %5d    M: %6.2f %6.2f    Av: %6.2f   A: %6.2f   V: %6.2f\n", 
			s,
			sinf->num,
			sinf->min*nf,
			sinf->max*nf, 
			av,
			sinf->abs_sum/ sinf->num,
			var );
}


void
inf_print_array( FILE *fp, char *s, int a[], int no )
{
int	i,	sum0,	sum1;	

	sum0 = sum1 = 0;

	fprintf( fp, "\n\n%s\n", s );
	for( i = 0 ; i < no ; i++ ){
		if( a[i] == 0 )	continue;
		fprintf( fp, "     %2d   - %5d\n", i, a[i] );
		sum1 += a[i];
		sum0 += a[i] * i;
	}

	fprintf( fp, "%7d   - %5d\n", sum0, sum1 );
}



void
inf_print_bits( FILE *fp, char *s, int bits, int no )
{
	fprintf( fp,"%s: %d{%.2f:%d}",
		s,
		bits,
		(no == 0 )? 0 : bits/(float)no,
		no );
}