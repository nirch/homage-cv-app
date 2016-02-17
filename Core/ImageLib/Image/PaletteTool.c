/**************************
 ***	PaletteTool.c   ***
 **************************/
#include	<stdio.h>
#include	<string.h>

#include	"Uigp/igp.h"

#include	"ImageType/PaletteType.h"
#include	"ImageType/ImageType.h"




palette_type *
palette_alloc( int n)
{
int	i;

	palette_type * p = (palette_type *)malloc(sizeof(palette_type));

	for( i = 0 ; i < 8 ; i ++ )
		if( (1<<i) >= n )	break;
	p->bitPerPixel = i;


	p->ALLOCATED = 1<< p->bitPerPixel;

	p->NA = n;
	p->nA = 0;

	p->data = (color_type *)calloc(p->ALLOCATED * sizeof(color_type), 1);

	p->table = NULL;

	return( p );
}

palette_type *
palette_copy( palette_type *palette, palette_type *tp )
{
	if( tp != NULL )
		palette_destroy( tp );

	if( palette == NULL )
		return( NULL );

	tp = palette_alloc( palette->nA);

	tp->bitPerPixel = palette->bitPerPixel;

	tp->type = palette->type;
	tp->nA = palette->nA;

	memcpy(tp->data, palette->data, (tp->nA) * sizeof(color_type));



	return( tp );
}

void
palette_destroy( palette_type *p )
{
	free(p->data);

	if( p->table != NULL )	free( p->table );

	free(p);
}



palette_type *
palette_create( int type )
{
palette_type	*p;
int	i;


	switch( type ){
	case PALETTE_HALFTONE:

			p = palette_alloc( 216 );
			p->nA = 216;
			for (i = 0; i < 216; i++){
				p->data[i].Red = (unsigned char)(i % 6 * 51);
				p->data[i].Green = (unsigned char)(i / 6 % 6 * 51);
				p->data[i].Blue = (unsigned char)(i / 36 * 51);
			}
			p->type = PALETTE_HALFTONE;

		break;


	case PALETTE_BW:

			p = palette_alloc(2);
			p->data[0].Blue = p->data[0].Green = p->data[0].Red = 0;
			p->data[1].Blue = p->data[1].Green = p->data[1].Red = 0xff;
			p->type = PALETTE_BW;
			p->nA = 2;
		break;

	case PALETTE_WB:

			p = palette_alloc(2);
			p->nA = 2;
			p->data[0].Blue = p->data[0].Green = p->data[0].Red = 0xff;
			p->data[1].Blue = p->data[1].Green = p->data[1].Red = 0x00;
			p->type = PALETTE_BW;
		break;



	case PALETTE_GRAY:

			//p = palette_alloc(4);
			//p->data[0].Blue = p->data[0].Green = p->data[0].Red = 0;
			//p->data[1].Blue = p->data[1].Green = p->data[1].Red = 0xff;
			//p->data[2].Blue = p->data[2].Green = p->data[2].Red = 0x60;
			//p->data[3].Blue = p->data[3].Green = p->data[3].Red = 0xa0;

			p = palette_alloc( 256 );
			p->nA = 256;

			for (i = 0; i < p->nA; i++){
				p->data[i].Red = i;
				p->data[i].Green = i;
				p->data[i].Blue = i;
			}

			p->type = PALETTE_GRAY;

		break;

	case	PALETTE_WINDOW_SYSTEM:

			p = palette_create_WINDOW_SYSTEM();
		break;

	}

	return( p );
}

palette_type *
palette_create_222( )
{
palette_type	*p;
int	i,	r,	g,	b;
	
	p = palette_alloc(64);
	
	for (i = 0 ; i < p->NA ; i++) {
		r = ( ((i) >> 4 ) &0x3 );
		g = ( ((i) >> 2 ) &0x3 );
		b = ( (i) &0x3 );

		r = (r << 6) + 32;
		g = (g << 6) + 32;
		b = (b << 6) + 32;

		p->data[i].Red = r;
		p->data[i].Green = g;
		p->data[i].Blue = b;
	}

	p->nA = i;

	p->type = PALETTE_222;

	return ( p );
}

palette_type *  
palette_create_gray( int bit )
{
palette_type	*p;
int	i;

	p = palette_alloc(1 << bit);
	p->bitPerPixel = bit;

	p->nA = p->NA;
	for (i = 0; i < p->nColor ; i++)
		p->data[i].Blue = p->data[i].Green = p->data[i].Red = (( 255 * i ) / ( p->nColor - 1 ));
	
	p->type = PALETTE_GRAY;

	return ( p );
}

palette_type *  
palette_create_HALFTONE()
{
palette_type	*p;
int	i;
	
	p = palette_alloc( 216 );
	p->nA = p->NA;

	for (i = 0; i < 216; i++){
		p->data[i].Red = (unsigned char)(i % 6 * 51);
		p->data[i].Green = (unsigned char)(i / 6 % 6 * 51);
		p->data[i].Blue = (unsigned char)(i / 36 * 51);
	}
	p->type = PALETTE_HALFTONE;

	return ( p );
}


void 
palette_write( palette_type *p, int no, FILE * fp )
{
int	i;

	for( i = p->nColor ; i < p->ALLOCATED ; i ++ )
		p->data[i].Blue = p->data[i].Green = p->data[i].Red = 0;

	for( i = 0 ; i < no ; i++ ){
		fputc( p->data[i].Red, fp );
		fputc( p->data[i].Green, fp );
		fputc( p->data[i].Blue, fp );
	}


	//fwrite(p->data, no, sizeof(color_type), fp);
}

int 
palette_write_to_file( palette_type *p, char *file )
{
FILE * fp;
//int	i;
	if( (fp = fopen( file, "w" )) == NULL )	return(0);


//	fprintf( fp, "%4d\n", p->nColor );

//	for( i = 0 ; i < p->nColor ; i++ )
//		fprintf( fp, "%4d,  %4d,  %4d,\n", (int)p->data[i].Red, (int)p->data[i].Green, (int)p->data[i].Blue );

	palette_write_to_open_file( p, fp );

	fclose(fp );

	return( 1 );
}



int 
palette_write_to_open_file( palette_type *p, FILE *fp )
{
int	i;

	fprintf( fp, "%4d\n", p->nColor );

	for( i = 0 ; i < p->nColor ; i++ )
		fprintf( fp, "%3d  %3d  %3d\n", (int)p->data[i].Red, (int)p->data[i].Green, (int)p->data[i].Blue );

	return( 1 );
}



int 
palette_write_hexa_to_file( palette_type *p, char *file )
{
	int	i;

	FILE * fp;
	//int	i;
	if( (fp = fopen( file, "w" )) == NULL )
		return(1);

	fprintf( fp, "PAL-X %4d\n", p->nColor );

	for( i = 0 ; i < p->nColor ; i++ )
		fprintf( fp, "%.8X\n", COLOR_RGB( p->data[i].Red, (int)p->data[i].Green, (int)p->data[i].Blue) );

	fclose( fp );

	return( 1 );
}


palette_type *
palette_read_file( char *file )
{
FILE	*fp;
int	i,	k,	k0,	k1;
int	r,	g,	b;
float	t;
int	nColor;
palette_type *p;

	p = palette_alloc( 256 );

	if( (fp = fopen( file, "rb")) == NULL )
		return( NULL );

	fscanf( fp, "%d", &nColor );

	k0 = -1;
	for( i = 0 ; i < nColor ; i++ ){
		fscanf( fp, "%d  %d  %d  %d", &k1, &r, &g, &b );

		p->data[k1].Red = r;
		p->data[k1].Green = g;
		p->data[k1].Blue = b;

		if( i == 0){
			k0 = k1;
			continue;
		}

		for( k = k0 + 1 ; k < k1 ; k++ ){
			t = (k - k0 )/ (float)(k1 - k0);
			p->data[k].Red   = (1-t) * p->data[k0].Red   + t * p->data[k1].Red;
			p->data[k].Green = (1-t) * p->data[k0].Green + t * p->data[k1].Green;
			p->data[k].Blue  = (1-t) * p->data[k0].Blue  + t * p->data[k1].Blue;
		}

		k0 = k1;
	}

	p->nA = k0+1;

	fclose( fp );


	return( p );
}


palette_type *
palette_read_heax_data( char *data )
{
	int	i;
	int	r,	g,	b,	color;
	int	nColor;
	palette_type *p;
	char	signature[256];


	
	if( gp_strtok_string( data, " \t\r\n", signature ) < 0 )
		return( NULL );

	if( gp_strtok_int( NULL, " \t\r\n", &nColor ) < 0 )
		return(NULL );


	p = palette_alloc( 256 );



	for( i = 0 ; i < nColor ; i++ ){

		if( gp_strtok_hexa( NULL, " \t\r\n", &color ) < 0 )
			break;


		r = IMAGE4_RED( color );
		g = IMAGE4_GREEN( color );
		b = IMAGE4_BLUE( color );




		p->data[i].Red = r;
		p->data[i].Green = g;
		p->data[i].Blue = b;

	}

	if( i < 0 ){
		palette_destroy( p );
		return( NULL );
	}

	p->nA = i;


	return( p );
}



palette_type *
palette_read_data( char *data )
{
int	i,	k,	k0,	k1;
int	r,	g,	b,	color;
float	t;
int	nColor;
palette_type *p;



	if( gp_strtok_int( data, " \t\r\n", &nColor ) < 0 )
		return(NULL );


	p = palette_alloc( 256 );


	k0 = -1;
	for( i = 0 ; i < nColor ; i++ ){

		if( gp_strtok_hexa( NULL, " \t\r\n", &color ) < 0 )
			break;


		r = IMAGE4_RED( color );
		g = IMAGE4_GREEN( color );
		b = IMAGE4_BLUE( color );


		k1 = 255 * (float)i / (nColor-1) + 0.5;

	
		p->data[k1].Red = r;
		p->data[k1].Green = g;
		p->data[k1].Blue = b;


		if( i == 0){
			k0 = k1;
			continue;
		}

		for( k = k0 + 1 ; k < k1 ; k++ ){
			t = (k - k0 )/ (float)(k1 - k0);
			p->data[k].Red   = (1-t) * p->data[k0].Red   + t * p->data[k1].Red;
			p->data[k].Green = (1-t) * p->data[k0].Green + t * p->data[k1].Green;
			p->data[k].Blue  = (1-t) * p->data[k0].Blue  + t * p->data[k1].Blue;
		}

		k0 = k1;
	}

	if( i < 0 ){
		palette_destroy( p );
		return( NULL );
	}

	p->nA = k0+1;


	return( p );
}

palette_type *
palette_copy_sample( palette_type *spl, int nColor )
{
palette_type	*pl;
int	i,	k;

	pl = palette_alloc( nColor+1 );

	

	pl->data[pl->nA++].color = 0;
	for( i = 0 ; i < nColor ; i++ ){

	
		k = 255 * (float)i / (nColor-1) + 0.5;


		pl->data[pl->nA++].color = spl->data[k].color;

	}

	return( pl );
}



#define MASK 0xf8
#define NBITS 5

int
palette_index( palette_type *palette, 
			int red, int green, int blue )
{
int i, k, m, sh1,	r;
int	rt,	bt,	gt;

	switch (palette->type){
	case PALETTE_SMART:
		if (palette->table)
		{
			sh1 = 8 - NBITS;
			i = ((((red >> sh1) << NBITS) + (green >> sh1)) << NBITS) + (blue >> sh1);
			return palette->table[i];
		}
		red &= MASK;
		green &= MASK;
		blue &= MASK;
		for (i = 0, k = 0xff; i < palette->nColor; i++)
		{
			m = MAX(ABS(palette->data[i].Red - red), ABS(palette->data[i].Green - green));
			m = MAX(m, ABS(palette->data[i].Blue - blue));
			if (m == 0)	return i;
			if (m < k)
			{
				k = m;
				r = i;
			}
		}
		return r;

	case PALETTE_BW:
			if( red == 0 )	return( 0 );
			else	return( 1 );
		break;

	case PALETTE_WB:
			if( red == 0 )	return( 1 );
			else	return( 0 );
		break;

	case PALETTE_GRAY:
		switch (red)
		{
		case 0:
			return 0;
		case 0xff:
			return 1;
		case 0x60:
			return 2;
		default:
			return 3;
		}
	case PALETTE_HALFTONE:
		return 
			(int)(((blue + 50) / 51) * 36 + ((green + 50) / 51) * 6 + (red + 50) / 51);

	default:
		for (i = 0, k = 0xff; i < palette->nColor; i++){
			if( (rt = (int)palette->data[i].Red - red) < 0 )		rt = -rt;
			if( (gt = (int)palette->data[i].Green - green) < 0 )	gt = -gt;
			if( (bt = (int)palette->data[i].Blue - blue) < 0 )		bt = -bt;
			
			m = MAX( rt, gt );
			if( bt > m )	m = bt;

			if (m == 0)	return i;
			if( m < k){
				k = m;
				r = i;
			}
		}
		return r;
		break;
	
	}
	return 0;
}

static const u_char Window_system_palette[256*3] = {
   0,     0,     0,
 128,     0,     0,
   0,   128,     0,
 128,   128,     0,
   0,     0,   128,
 128,     0,   128,
   0,   128,   128,
 128,   128,   128,
 192,   220,   192,
 166,   202,   240,
  42,    63,   170,
  42,    63,   255,
  42,    95,     0,
  42,    95,    85,
  42,    95,   170,
  42,    95,   255,
  42,   127,     0,
  42,   127,    85,
  42,   127,   170,
  42,   127,   255,
  42,   159,     0,
  42,   159,    85,
  42,   159,   170,
  42,   159,   255,
  42,   191,     0,
  42,   191,    85,
  42,   191,   170,
  42,   191,   255,
  42,   223,     0,
  42,   223,    85,
  42,   223,   170,
  42,   223,   255,
  42,   255,     0,
  42,   255,    85,
  42,   255,   170,
  42,   255,   255,
  85,     0,     0,
  85,     0,    85,
  85,     0,   170,
  85,     0,   255,
  85,    31,     0,
  85,    31,    85,
  85,    31,   170,
  85,    31,   255,
  85,    63,     0,
  85,    63,    85,
  85,    63,   170,
  85,    63,   255,
  85,    95,     0,
  85,    95,    85,
  85,    95,   170,
  85,    95,   255,
  85,   127,     0,
  85,   127,    85,
  85,   127,   170,
  85,   127,   255,
  85,   159,     0,
  85,   159,    85,
  85,   159,   170,
  85,   159,   255,
  85,   191,     0,
  85,   191,    85,
  85,   191,   170,
  85,   191,   255,
  85,   223,     0,
  85,   223,    85,
  85,   223,   170,
  85,   223,   255,
  85,   255,     0,
  85,   255,    85,
  85,   255,   170,
  85,   255,   255,
 127,     0,     0,
 127,     0,    85,
 127,     0,   170,
 127,     0,   255,
 127,    31,     0,
 127,    31,    85,
 127,    31,   170,
 127,    31,   255,
 127,    63,     0,
 127,    63,    85,
 127,    63,   170,
 127,    63,   255,
 127,    95,     0,
 127,    95,    85,
 127,    95,   170,
 127,    95,   255,
 127,   127,     0,
 127,   127,    85,
 127,   127,   170,
 127,   127,   255,
 127,   159,     0,
 127,   159,    85,
 127,   159,   170,
 127,   159,   255,
 127,   191,     0,
 127,   191,    85,
 127,   191,   170,
 127,   191,   255,
 127,   223,     0,
 127,   223,    85,
 127,   223,   170,
 127,   223,   255,
 127,   255,     0,
 127,   255,    85,
 127,   255,   170,
 127,   255,   255,
 170,     0,     0,
 170,     0,    85,
 170,     0,   170,
 170,     0,   255,
 170,    31,     0,
 170,    31,    85,
 170,    31,   170,
 170,    31,   255,
 170,    63,     0,
 170,    63,    85,
 170,    63,   170,
 170,    63,   255,
 170,    95,     0,
 170,    95,    85,
 170,    95,   170,
 170,    95,   255,
 170,   127,     0,
 170,   127,    85,
 170,   127,   170,
 170,   127,   255,
 170,   159,     0,
 170,   159,    85,
 170,   159,   170,
 170,   159,   255,
 170,   191,     0,
 170,   191,    85,
 170,   191,   170,
 170,   191,   255,
 170,   223,     0,
 170,   223,    85,
 170,   223,   170,
 170,   223,   255,
 170,   255,     0,
 170,   255,    85,
 170,   255,   170,
 170,   255,   255,
 212,     0,     0,
 212,     0,    85,
 212,     0,   170,
 212,     0,   255,
 212,    31,     0,
 212,    31,    85,
 212,    31,   170,
 212,    31,   255,
 212,    63,     0,
 212,    63,    85,
 212,    63,   170,
 212,    63,   255,
 212,    95,     0,
 212,    95,    85,
 212,    95,   170,
 212,    95,   255,
 212,   127,     0,
 212,   127,    85,
 212,   127,   170,
 212,   127,   255,
 212,   159,     0,
 212,   159,    85,
 212,   159,   170,
 212,   159,   255,
 212,   191,     0,
 212,   191,    85,
 212,   191,   170,
 212,   191,   255,
 212,   223,     0,
 212,   223,    85,
 212,   223,   170,
 212,   223,   255,
 212,   255,     0,
 212,   255,    85,
 212,   255,   170,
 212,   255,   255,
 255,     0,    85,
 255,     0,   170,
 255,    31,     0,
 255,    31,    85,
 255,    31,   170,
 255,    31,   255,
 255,    63,     0,
 255,    63,    85,
 255,    63,   170,
 255,    63,   255,
 255,    95,     0,
 255,    95,    85,
 255,    95,   170,
 255,    95,   255,
 255,   127,     0,
 255,   127,    85,
 255,   127,   170,
 255,   127,   255,
 255,   159,     0,
 255,   159,    85,
 255,   159,   170,
 255,   159,   255,
 255,   191,     0,
 255,   191,    85,
 255,   191,   170,
 255,   191,   255,
 255,   223,     0,
 255,   223,    85,
 255,   223,   170,
 255,   223,   255,
 255,   255,    85,
 255,   255,   170,
 204,   204,   255,
 255,   204,   255,
  51,   255,   255,
 102,   255,   255,
 153,   255,   255,
 204,   255,   255,
   0,   127,     0,
   0,   127,    85,
   0,   127,   170,
   0,   127,   255,
   0,   159,     0,
   0,   159,    85,
   0,   159,   170,
   0,   159,   255,
   0,   191,     0,
   0,   191,    85,
   0,   191,   170,
   0,   191,   255,
   0,   223,     0,
   0,   223,    85,
   0,   223,   170,
   0,   223,   255,
   0,   255,    85,
   0,   255,   170,
  42,     0,     0,
  42,     0,    85,
  42,     0,   170,
  42,     0,   255,
  42,    31,     0,
  42,    31,    85,
  42,    31,   170,
  42,    31,   255,
  42,    63,     0,
  42,    63,    85,
 255,   251,   240,
 160,   160,   164,
 128,   128,   128,
 255,     0,     0,
   0,   255,     0,
 255,   255,     0,
   0,     0,   255,
 255,     0,   255,
   0,   255,   255,
   255,   255,   255 };


palette_type *
palette_create_WINDOW_SYSTEM()
{
palette_type	*p;

	p = palette_alloc( 256 );
	p->nA =256;
	memcpy( p->data, Window_system_palette, 256*3 );

	p->type = PALETTE_WINDOW_SYSTEM;

	return( p );
}



/*********  palette_color_space  **********
* @Description	:  returns the color space according to the string
* 
* @Param  char * colorStr :  the string to describe the color space
* @Return  int  :  -1 if error, color space otherwise
*****************************************/
int palette_color_space(char * colorStr)
{
	char str[256];
	if ( str == NULL)
		return -1;
	strcpy(str,colorStr);
	gp_strlwr(str);
	if ( strcmp(str,"rgb") == 0 )
		return PALETTE_RGB;
	if ( strcmp(str,"bw") == 0 )
		return PALETTE_BW;
	if ( strcmp(str,"wb") == 0 )
		return PALETTE_WB;
	if ( strcmp(str,"gray") == 0 )
		return PALETTE_GRAY;
	if ( strcmp(str,"adaptive") == 0 )
		return PALETTE_SMART;
	if ( strcmp(str,"WINSYS") == 0 )
		return PALETTE_WINDOW_SYSTEM;
	return -1;

}

