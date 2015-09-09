#include	<stdio.h>
#include	<fcntl.h>
#include	"Uigp/igp.h"
#include	"Umbmap/mbmap.h"

#define		INTELL_SHORT( a )  \
		( ( (a & 0xff00) >> 8 ) | ((a & 0x00ff) << 8 ) )

struct	TARGA_HEADER {
	unsigned char	id_length;
	unsigned char	cm_type;
	unsigned char	image_type;
	unsigned char	cm_specification[5];

	short	x;
	short	y;
	short	width;
	short	higth;
	unsigned char	depth;
	unsigned char	descriptor;
};


struct	TARGA_FOOTER {
	int	extension_offset;
	int	developer_offset;
};

static void	set_targa_header( struct  TARGA_HEADER *t,
				int row, int column, int depth );

static int	get_targa_header( struct  TARGA_HEADER *t,
				int *row, int *column, int *depth );

static void 	set_targa_footer( struct  TARGA_FOOTER *f );


struct	TARGA_FOOTER	Targa_footer = { 0, 0 };

static char	*Targa_signature = "TRUEVISION-XFILE.";

	

void
dump_rgb_mbmap_to_targa_file( char *dir, char *image_name,
				mbmap r, mbmap g, mbmap b )
{
char	file[200];
struct	TARGA_HEADER	T_header;
int	row,	column,	i,	j,	fd;
unsigned char	*temp,	*pr,	*pg,	*pb,	*p;


	if( dir == NULL )
		sprintf( file, "%s.tga", image_name );
	else	sprintf( file, "%s/%s.tga", dir, image_name );

	if( (fd = open( file , O_WRONLY|O_CREAT, 0644 )) == -1 )
		error( "dump_rgb_mbmap_to_targa_file",
						"Can't open tagat file" );

	column = MBMAP_COLUMN( r );
	row = MBMAP_ROW( r );


	set_targa_header( &T_header, row, column, 24 );
	write( fd, (char *)&T_header , sizeof( struct TARGA_HEADER) );

	/* write data */

	pr = MBMAP_DATA_ADRESS( r );
        pg = MBMAP_DATA_ADRESS( g );
        pb = MBMAP_DATA_ADRESS( b );

	temp = ( unsigned char * )malloc( column *3 );
	for( i = 0 ; i < row ; i++ ){
		for( j = 0 , p = temp ; j < column ; j++ ){
			*p++ = *pb++;
			*p++ = *pg++;
			*p++ = *pr++;
		}
		write( fd, (char *)temp , column*3 );
	}
	free( temp );



	write( fd, (char *)&Targa_footer , sizeof( struct TARGA_FOOTER) );

	write( fd, Targa_signature, 18 );


	close( fd );
}



void
dump_mbmap_to_targa_file8( char *dir, char *image_name, mbmap bmap )
{
char	file[256];
struct	TARGA_HEADER	T_header;
int	row,	column,	fd;


	if( dir == NULL )
		sprintf( file, "%s.tga", image_name );
	else	sprintf( file, "%s/%s.tga", dir, image_name );

	if( (fd = open( file , O_WRONLY|O_CREAT, 0644 )) == -1 )
		error( "dump_mbmap_to_targa_file8", "Can't open tagat file" );

	column = MBMAP_COLUMN( bmap );
	row = MBMAP_ROW( bmap );


	set_targa_header( &T_header, row, column, 8 );
	write( fd, (char *)&T_header , sizeof( struct TARGA_HEADER) );

	/* write data */


	write( fd, (char *)MBMAP_DATA_ADRESS( bmap ), column*row );


	write( fd, (char *)&Targa_footer , sizeof( struct TARGA_FOOTER) );

	write( fd, Targa_signature, 18 );


	close( fd );
}



static void
set_targa_header( struct  TARGA_HEADER *t, int row, int column, int depth )
{
int	i;
	t->id_length = 0;
	t->cm_type = 0;
	t->image_type = (depth == 24 )? 2 : 3;

	for( i = 0 ; i < 5 ; i++  )
		t->cm_specification[i] = 0;

	t->x = 0;
	t->y = 0;

#ifdef _WINDOWS
	t->width = column;
	t->higth = row;
#else
	t->width = INTELL_SHORT( column );
	t->higth = INTELL_SHORT( row );
#endif
	t->depth = depth;

	t->descriptor = 0x20;
}

static int
get_targa_header( struct  TARGA_HEADER *t, int *row, int *column, int *depth )
{
/*
	if( t->descriptor != 0x20 )	return( -1 );
*/
	t->descriptor = 0x20;
#ifdef  _WINDOWS
	*column = t->width;
	*row = t->higth;
#else
	*column = INTELL_SHORT(t->width );
	*row = INTELL_SHORT( t->higth );
#endif
	*depth = t->depth;
	return(1);
}


static void
set_targa_footer( struct  TARGA_FOOTER *f )
{
	f->extension_offset = 0;
	f->developer_offset = 0;

	/*
	strncpy( f->signature, "TRUEVISION-XFILE.\0", 18 );
	*/
}


void
load_rgb_mbmap_to_targa_file( char *dir, char *image_name,
				mbmap *r, mbmap *g, mbmap *b )
{
char	file[200];
struct	TARGA_HEADER	T_header;
int	row,	column,	i,	j,	fd,	depth;
unsigned char	*temp,	*pr,	*pg,	*pb,	*p;
int	crow;


	if( dir == NULL )
		sprintf( file, "%s.tga", image_name );
	else	sprintf( file, "%s/%s.tga", dir, image_name );

#ifdef _WINDOWS
	if( (fd = open(file, O_RDONLY | O_BINARY, 0644 ) ) < 0 )
#else
	if( (fd = open(file, O_RDONLY, 0644 ) ) < 0 )
#endif
		error( "load_rgb_mbmap_to_targa_file", "Can't open tagat file");


	read( fd, (char *)&T_header , sizeof( struct TARGA_HEADER) );
	get_targa_header( &T_header, &row, &column, &depth );
	if( depth != 24 )
		error( "load_rgb_mbmap_to_targa_file", "depth != 24" );

	/* write data */

	MBMAP_CREATE( *r, row, column );
	MBMAP_CREATE( *g, row, column );
	MBMAP_CREATE( *b, row, column );

	pr = MBMAP_DATA_ADRESS( *r );
        pg = MBMAP_DATA_ADRESS( *g );
        pb = MBMAP_DATA_ADRESS( *b );

	temp = ( unsigned char * )malloc( column *3 );
	for( i = 0 ; i < row ; i++ ){
		read( fd, (char *)temp , column*3 );

		crow = ( T_header.descriptor == 0x20 )? i : row -i-1;
		pr =MBMAP_PIXEL_ADRESS( *r, crow, 0 );
		pb =MBMAP_PIXEL_ADRESS( *b, crow, 0 );
		pg =MBMAP_PIXEL_ADRESS( *g, crow, 0 );
		for( j = 0 , p = temp ; j < column ; j++ ){
			*pb++ = *p++;
			*pg++ = *p++;
			*pr++ = *p++;
		}
	}
	free( temp );

	close( fd );
}
