/*******************
 ***	GpIo.c	***
 *******************/


#include <stdio.h>



#ifndef WINCE
#include <fcntl.h>                        /* for O_BINARY */
#endif

#include	"igp.h"


void
gp_fwrite_num( FILE *fp, int a, int nByte )
{
int	c;
int	i,	s;

	for( i = nByte-1, s = 8*(nByte-1) ; i >= 0 ; i--, s -= 8 ){
		c = (((unsigned int) a)>>s) & 0xff;
		putc( c, fp);
	}
}


int
gp_fread_num( FILE *fp, int nByte )
{
int	c,	a;
int	i;

	a = 0;
	for( i = 0 ; i < nByte ; i++ ){
		c = getc( fp );
		a = ( a<<8) | c;
	}

	return( a );
}




void
gp_fwrite_int( FILE *fp, int a )
{
char	c[4];

	c[0] = (((unsigned int) a)>>24) & 0xff;
	c[1] = (((unsigned int) a)>>16) & 0xff;
	c[2] = (((unsigned int) a)>>8) & 0xff;
	c[3] = (((unsigned int) a)) & 0xff;

	putc(c[0], fp);   putc(c[1],fp);  putc(c[2],fp);  putc(c[3],fp);
}


int
gp_fread_int( FILE *fp )
{
int	c[4];
int	a;

	c[0] = getc( fp );
	c[1] = getc( fp );
	c[2] = getc( fp );
	c[3] = getc( fp );

	a = (((unsigned int) c[0])<<24) +
		(((unsigned int) c[1])<<16) +
		(((unsigned int) c[2])<<8) +
		(((unsigned int) c[3]));

	return( a );
}


int
gp_fread_intArr( FILE *fp, int *value )
{
u_char	c[4];

	if( fread(c, sizeof(u_char), 4, fp) < 4 )
		return( -1 );

	*value = (((unsigned int) c[0])<<24) +
			(((unsigned int) c[1])<<16) +
			(((unsigned int) c[2])<<8) +
			(((unsigned int) c[3]));

	return( 1 );
}


void
gp_fwrite_int3( FILE *fp, int a )
{
char	c[3];

	c[0] = (((unsigned int) a)>>16) & 0xff;
	c[1] = (((unsigned int) a)>>8) & 0xff;
	c[2] = (((unsigned int) a)) & 0xff;

	putc(c[0], fp);   putc(c[1],fp);  putc(c[2],fp);
}



int
gp_fread_int3( FILE *fp )
{
int	c[3];
int	a;

	c[0] = getc( fp );
	c[1] = getc( fp );
	c[2] = getc( fp );

	a = (((unsigned int) c[0])<<16) +
		(((unsigned int) c[1])<<8) +
		(((unsigned int) c[2]));

	return( a );
}



void
gp_fwrite_float( FILE *fp, float p )
{
int	*a;
char	c[4];

	a = (int *)&p;

	c[0] = (((unsigned int) *a)>>24) & 0xff;
	c[1] = (((unsigned int) *a)>>16) & 0xff;
	c[2] = (((unsigned int) *a)>>8) & 0xff;
	c[3] = (((unsigned int) *a)) & 0xff;

	putc(c[0], fp);   putc(c[1],fp);  putc(c[2],fp);  putc(c[3],fp);
}



float
gp_fread_float( FILE *fp )
{
int	c[4];
int	a;
float	*p;

	c[0] = getc( fp );
	c[1] = getc( fp );
	c[2] = getc( fp );
	c[3] = getc( fp );

	a = (((unsigned int) c[0])<<24) +
		(((unsigned int) c[1])<<16) +
		(((unsigned int) c[2])<<8) +
		(((unsigned int) c[3]));

	p = (float *)&a;
	return( *p );
}




int
gp_fread_short( FILE *fp )
{
int	c[2];
int	a;

	c[0] = getc( fp );
	c[1] = getc( fp );

	a = (((unsigned int) c[0])<<8) +
		(((unsigned int) c[1]));
	return( a );
}



void
gp_fwrite_short( FILE *fp, int a )
{
char	c[2];

	c[0] = (((unsigned int) a)>>8) & 0xff;
	c[1] = (((unsigned int) a)) & 0xff;

	putc(c[0], fp);   putc(c[1],fp);
}



#ifndef WINCE

int
gp_read_int( int fd )
{
u_char	c[4];
int	a;

	read( fd, ( char *)c, 4 );

	a = (((unsigned int) c[0])<<24) +
		(((unsigned int) c[1])<<16) +
		(((unsigned int) c[2])<<8) +
		(((unsigned int) c[3]));

	return( a );
}

int
gp_read_short( int fd )
{
u_char	c[2];
int	a;

	read( fd, ( char *)c, 2 );

	a = (((unsigned int) c[0])<<8) +
		(((unsigned int) c[1]));

	return( a );
}


void
gp_write_int( int fd, int a )
{
char	c[4];

	c[0] = (((unsigned int) a)>>24) & 0xff;
	c[1] = (((unsigned int) a)>>16) & 0xff;
	c[2] = (((unsigned int) a)>>8) & 0xff;
	c[3] = (((unsigned int) a)) & 0xff;

	write( fd, c, 4 );
}


void
gp_write_short( int fd, int a )
{
char	c[2];

	c[1] = (((unsigned int) a)>>8) & 0xff;
	c[2] = (((unsigned int) a)) & 0xff;

	write( fd, c, 2 );
}



int
gp_read_int_lsbf( int fd )
{
u_char	c[4];
int	a;

	read( fd, ( char *)c, 4 );

	a = (((unsigned int) c[3])<<24) +
		(((unsigned int) c[1])<<16) +
		(((unsigned int) c[1])<<8) +
		(((unsigned int) c[0]));

	return( a );
}

int
gp_read_short_lsbf( int fd )
{
u_char	c[2];
int	a;

	read( fd, ( char *)c, 2 );

	a = (((unsigned int) c[1])<<8) +
		(((unsigned int) c[2]));

	return( a );
}


void
gp_write_int_lsbf( int fd, int a )
{
char	c[4];

	c[3] = (((unsigned int) a)>>24) & 0xff;
	c[2] = (((unsigned int) a)>>16) & 0xff;
	c[1] = (((unsigned int) a)>>8) & 0xff;
	c[0] = (((unsigned int) a)) & 0xff;

	write( fd, c, 4 );
}


void
gp_write_short_lsbf( int fd, int a )
{
char	c[2];

	c[2] = (((unsigned int) a)>>8) & 0xff;
	c[1] = (((unsigned int) a)) & 0xff;

	write( fd, c, 2 );
}

#endif

