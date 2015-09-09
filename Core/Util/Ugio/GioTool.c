/**************************
 ***	CstTool.c	***
 **************************/
#include	<stdio.h>
#include	"Uigp/igp.h"

#include	"GioType.h"


gio_type *
gio_alloc()
{
gio_type *gio;

	gio = (gio_type *)malloc( sizeof(gio_type) );

	gio->fp = NULL;

	gio->Fdata = 0;
	gio->cst = NULL;

	gio->bytes = -1;

	gio->flag = 0;

	return( gio );
}


void
gio_close( gio_type *gio )
{
	if ( gio == NULL )
		return ;
	if( gio->fp != NULL ){
		fclose( gio->fp );
		gio->fp = NULL;
	}


	if( gio->cst != NULL ){
		cst_destroy( gio->cst, gio->Fdata );
		gio->cst = NULL;
	}

	free( gio );
}



gio_type *
gio_open_file_to_read( const char *file )
{
gio_type	*gio;
FILE	*fp;

	if( (fp = fopen( file, "rb" )) == NULL )
		return( NULL ); 

	gio = gio_alloc();

	gio->fp = fp;
	
	gio_bytes( gio );

	gio->flag = GIO_READ;
	
	return( gio );
}

gio_type *
gio_open_file_to_write( const char *file )
{
gio_type	*gio;
FILE	*fp;

	if( (fp = fopen( file, "wb" )) == NULL )
		return( NULL ); 

	gio = gio_alloc();

	gio->fp = fp;
	
	gio_bytes( gio );

	gio->flag = GIO_WRITE;
	
	return( gio );
}

gio_type *
gio_open_buffer_to_read( char *data, int byte_no, int Fdata )
{
gio_type	*gio;

	gio = gio_alloc();


	gio->cst = cst_create( data, byte_no, Fdata );

	gio->Fdata = Fdata;

	gio_bytes( gio );

	gio->flag = GIO_READ;

	return( gio );
}


gio_type *
gio_open_buffer_to_write( int byte_no )
{
gio_type	*gio;


	gio = gio_alloc();

	gio->cst = cst_alloc( byte_no );

	gio->fp = NULL;

	gio->Fdata = 1;

	gio->flag = GIO_WRITE;

	return( gio );
}



void
gio_put_no( gio_type *gio, int a, int nByte, int Fendian )
{
	if( Fendian == BIG_ENDIAN )
			gio_put_noM( gio, a, nByte );
	else	gio_put_noL( gio, a, nByte );
}

int
gio_get_no( gio_type *gio, int nByte, int Fendian )
{
int	a;

	if( Fendian == BIG_ENDIAN )
			a = gio_get_noM( gio, nByte );
	else	a = gio_get_noL( gio, nByte );

	return( a );
}

void
gio_put_noM( gio_type *gio, int a, int nByte )
{
int     c;
int     i,      s;
 
        for( i = nByte-1, s = 8*(nByte-1) ; i >= 0 ; i--, s -= 8 ){

                c = (((unsigned int) a)>>s) & 0xff;

                GIO_PUTC( gio, c);
        }
} 



int
gio_get_noM( gio_type *gio, int nByte )
{
int     c,      a;
int     i;
 
        a = 0;
        for( i = 0 ; i < nByte ; i++ ){
                c = GIO_GETC( gio);
                a = ( a<<8) | c;
        }
 
        return( a );
}


void
gio_put_noL( gio_type *gio, int a, int nByte )
{
int     c;
int     i,      s;
 
        for( i = nByte-1, s = 0 ; i >= 0 ; i--, s += 8 ){

                c = (((unsigned int) a)>>s) & 0xff;

                GIO_PUTC( gio, c);
        }
} 


int
gio_get_noL( gio_type *gio, int nByte )
{
int     c,      a;
int     i,	s;
 
        a = 0;
        for( i = 0, s = 0 ; i < nByte ; i++, s += 8 ){
                c = GIO_GETC( gio);
                a = ( c<<s) | a;
        }
 

	return( a );
}


int
gio_get_buffer( gio_type *gio, u_char buf[], int byte_no )
{
//int	i;
int	no;


	if( gio->fp != NULL ){
		no = fread( buf, 1, byte_no, gio->fp );
		return( no );
	}

	no = cst_get_buffer( gio->cst, buf, byte_no );
	return( no );

/*
	for( i = 0 ; i < byte_no ; i++ )
		buf[i] = GIO_GETC( gio );

	return byte_no;
*/
}


int
gio_put_buffer(gio_type *gio, u_char buf[], int byte_no )
{
int	i;


	for( i = 0 ; i < byte_no ; i++ )
		GIO_PUTC( gio, buf[i] );

	return byte_no;

}


long
gio_ftell( gio_type *gio )
{
int	pos;

	if( gio->fp != NULL )
		pos = ftell( gio->fp );
	else {
		if( gio->flag == 0 )
			error( "gio_ftell", "gio->flag not valid" );
		if( gio->flag == GIO_READ )
				pos =  CST_RTELL( gio->cst ); //gio->cst->rp - gio->cst->data;
		else	pos =  CST_WTELL( gio->cst );
	}

	if( pos > gio->bytes )	gio->bytes = pos;

	return( pos );
}

int
gio_seek( gio_type *gio, int pos )
{
	if( pos < 0 )	pos = 0;

	gio_bytes( gio );

	if( pos > gio->bytes )	pos = gio->bytes;


	if( gio->fp != NULL )
			fseek( gio->fp, pos, SEEK_SET );
	else	{
			if( gio->flag == 0 )
				error( "gio_ftell", "gio->flag not valid" );
			if( gio->flag == GIO_READ )
				 CST_RSEEK( gio->cst, pos );
			else CST_WSEEK( gio->cst, pos );
	}

	return( 1 );
}


int gio_skip( gio_type *gio, int pos )
{
	if ( gio->fp != NULL ){
		if( fseek( gio->fp, pos, SEEK_CUR  ) == 0 )
			return( 1 );
		return( 0 );
	}


	CST_SKIP( gio->cst, pos );
	if( CST_EOS( gio->cst ) )	return( -1 );
	return( 0 );

}



long
gio_bytes( gio_type *gio )
{
long pos;

	if( gio->bytes > 0 )	return( gio->bytes );


	if( gio->fp != NULL ){

		pos = ftell( gio->fp );
		fseek( gio->fp, 0, SEEK_END );
		gio->bytes = ftell( gio->fp );
		fseek( gio->fp, pos, SEEK_SET );
	}
	else	
	{
		gio->bytes = CST_BYTES( gio->cst ); //MAX(gio->cst->rp - gio->cst->data,gio->cst->byte_no );
	}

	return gio->bytes;
}

char gio_getc( gio_type *gio )
{
	if ( gio->fp != NULL )
       return (char)fgetc(gio->fp);
	
	return CST_GETC(gio->cst);
}

unsigned char gio_getuc( gio_type *gio )
{
	if ( gio->fp != NULL )
       return (unsigned char)fgetc(gio->fp);

	return CST_GETUC(gio->cst);
}

