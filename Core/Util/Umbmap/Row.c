/****************************************************************
 *								*
 *		Module:  mbmap_io.c				*
 *								*
 ****************************************************************/

#include	<stdio.h>

#include	<fcntl.h>

#include	"Uigp/igp.h"
#include	"mbmap.h"


void
mbmap_dump_rgb_to_row_line( int fd, mbmap r, mbmap g, mbmap b )
{
int	i;
u_char	*rp,	*gp,	*bp;

	rp = MBMAP_DATA_ADRESS(r);
	gp = MBMAP_DATA_ADRESS(g);
	bp = MBMAP_DATA_ADRESS(b);

	for( i = 0 ; i < MBMAP_ROW( r ) ; i++ ){
  		write( fd, (char *)rp, MBMAP_COLUMN( r ) );
		rp += MBMAP_COLUMN( r );

  		write( fd, (char *)gp, MBMAP_COLUMN( g ) );
		gp += MBMAP_COLUMN( g );

  		write( fd, (char *)bp, MBMAP_COLUMN( b ) );
		bp += MBMAP_COLUMN( b );

	}
}


void
mbmap_dump_rgb_to_row_pixel( int fd, mbmap r, mbmap g, mbmap b )
{
int	i,	j,	column;
u_char	*rp,	*gp,	*bp,	*buf,	*p;



	column = MBMAP_COLUMN( r );
	if( (buf = (u_char *)malloc((u_long)3* column ) ) == NULL )
		error( "mbmap_dump_rgb_to_row_pixel", "malloc return Null" );

	rp = MBMAP_DATA_ADRESS(r);
	gp = MBMAP_DATA_ADRESS(g);
	bp = MBMAP_DATA_ADRESS(b);

	for( i = 0 ; i < MBMAP_ROW( r ) ; i++ ){

		for( j = 0, p = buf ; j < column ; j++ ){
			*p++ = *rp++;
			*p++ = *gp++;
			*p++ = *bp++;
		}

  		write( fd, (char *)buf, 3* column );
	}

	free( (char *) buf );
}
