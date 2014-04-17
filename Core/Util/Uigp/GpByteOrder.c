/************************
 ***	GpByteOrder.c ***
 ************************/
#include	<string.h>

#include	"Uigp/igp.h"



int
gp_byte_order()
{
char s[5];
union
{
	long longval;
    char charval[4];
} probe;

    probe.longval = 0x41424344L;  /* ABCD in ASCII */
    strncpy( s, probe.charval, sizeof(long) );
    s[4] = '\0';

    if ( strcmp(s, "ABCD") == 0 )
        return BIG_ENDIAN;

    //else if ( strcmp(s, "DCBA") == 0 )
    return LITTLE_ENDIAN;
}




void 
gp_byte_order_swap_short( short *loc, int no )
{
int i;
short thisval;
char *dst, *src;

    src = (char *) &thisval;
    for ( i = 0; i < no; i++ )
    {
        thisval = *loc;
        dst = (char *)loc++;

        dst[0] = src[1];
        dst[1] = src[0];
    }
}

void 
gp_byte_order_swap_int( int *loc, int no )
{
int i;
int thisval;
char *dst, *src;

    src = (char *) &thisval;
    for ( i = 0; i < no; i++ )
    {
        thisval = *loc;
        dst = (char *)loc++;

        dst[0] = src[3];
        dst[1] = src[2];
		dst[2] = src[1];
		dst[3] = src[0];
    }
}

