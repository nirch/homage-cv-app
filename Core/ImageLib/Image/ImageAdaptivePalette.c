/********************************
 ***	ImageSmartPalette.c   ***
 ********************************/
#include	<stdlib.h>
#include	"Uigp/igp.h"
#include	"Ubox/Box2d.h"
#include	"ImageType/ImageType.h"


#define NBITS 4
#define CELL_NO (1 << (3 * NBITS))

 
#define		COLOR_TO_INDEX4( r, g, b )		( ((r&0xf0)<<4) | ((g&0xf0)<<0) | ((b&0xf0)>>4))

#ifdef WIN32
#define		COLOR_TO_INDEX4F( color )		( ((color&0xf00000)>>12) | ((color&0xf000)>>8) | ((color&0xf0)>>4))
#else
#define		COLOR_TO_INDEX4F( color )		( ((color&0xf00000)>>12) | ((color&0xf000)>>8) | ((color&0xf0)>>4))
///#define		COLOR_TO_INDEX4F( color )		( ((color&0xf00000)<<4) | ((color&0xf000)>>8) | ((color&0xf0)>>20))
#endif

#define		INDEX4_TO_RED( index )			( ( index >>4) & 0xf0 )
#define		INDEX4_TO_GREEN( index )			( ( index >>0) & 0xf0 )
#define		INDEX4_TO_BLUE( index )			( ( index <<4) & 0xf0 )





typedef struct	cell_type {
	int	r;
	int	g;
	int	b;

	int	n;

}	cell_type;


static int	image_adaptive_palette_get_max( cell_type *h, int no,
										   palette_type *palette, int delta );


static void	image_adaptive_palette_inverse( palette_type *pal );



void 
image_adaptive_palette( image_type *images[], int nImage,
					   palette_type *palette, int nColor )
{
image_type	*im;
u_int	*p;
int	i, j,	k,	n;
int r, g, b;
cell_type *H;
int	ncolor;



	H = (cell_type *)malloc(sizeof(cell_type)*  CELL_NO);

	for (i = 0; i < CELL_NO;  i++){
		H[i].n = 0;
		H[i].r = H[i].b = H[i].g = 0;
	}


	for( n = 0 ; n < nImage ; n++ ){
		im = images[n];

		p = (u_int *)im->data;
		for( i = 0; i < im->row; i++){
			for( j = 0; j < im->column; j++, p++){

				if ( (*p) & 0xFF000000 )	continue;

				r = IMAGE4_RED(*p);
				g = IMAGE4_GREEN(*p);
				b = IMAGE4_BLUE(*p);

				k = COLOR_TO_INDEX4( r, g, b );


				H[k].r += r;
				H[k].g += g;
				H[k].b += b;
				H[k].n++;
			}
		}
	}


	for( i = 0 ; i < palette->nColor ; i++ ){
		k = COLOR_TO_INDEX4( palette->data[i].r, palette->data[i].g, palette->data[i].b );
		H[k].n = 0;
	}

	ncolor = MIN( nColor, 64 );
	for( i = palette->nColor; i < ncolor ; i++ ){
		k = image_adaptive_palette_get_max( H, CELL_NO, palette, 32 );
		if( k < 0 )	break;


		palette->data[i].Red   = H[k].r / H[k].n;
		palette->data[i].Green = H[k].g / H[k].n;
		palette->data[i].Blue  = H[k].b / H[k].n;

		palette->nColor++;

		//fprintf( stdout, "%d   ", H[k].n );

		H[k].n = 0;
	}


	for( i = palette->nColor; i < nColor ; i++ ){
		k = image_adaptive_palette_get_max( H, CELL_NO, palette, 0 );
		if( k < 0 )	break;


		palette->data[i].Red   = H[k].r / H[k].n;
		palette->data[i].Green = H[k].g / H[k].n;
		palette->data[i].Blue  = H[k].b / H[k].n;

		palette->nColor++;

		H[k].n = 0;
	}

	free(H);

//	palette->type = PALETTE_UNKNOWN;

	palette->type = PALETTE_ADAPTIVE4;

	image_adaptive_palette_inverse( palette );

}



static int
image_adaptive_palette_get_max( cell_type *h, int no, palette_type *palette, int delta )
{
int	i,	j;
int	max,	k,	flag;
int	r,	g,	b;
int	rt,	gt,	bt;

	max = 0;
	k = -1;
	for (i = 0; i < no;  i++ ){
		if( h[i].n <= max )	continue;

		if( delta > 0 ){
			r = INDEX4_TO_RED( i )+8;
			g = INDEX4_TO_GREEN( i )+8;
			b = INDEX4_TO_BLUE( i)+8;


			flag = 0;
			for( j = 0 ; j < palette->nColor ; j++ ){
				if( (rt = r - palette->data[j].Red ) < 0 )	rt = -rt;
				if( (gt = g - palette->data[j].Green ) < 0 )	gt = -gt;
				if( (bt = b - palette->data[j].Blue ) < 0 )	bt = -bt;

				if( rt < delta && gt < delta && bt < delta ){
					flag = 1;
					break;
				}
			}

			if( flag == 1 )	continue;
		}

		max = h[i].n;
		k = i;
	}

	if( delta > 0 && max < 25 )	return( -1 );

	return( k );
}



static void
image_adaptive_palette_inverse( palette_type *pal )
{
int	r,	g,	b,	rt,	gt,	bt;
int	i,	j,	k,	max;

	pal->table = (u_char *)malloc( CELL_NO );

	for( i = 0 ; i < CELL_NO ; i++ ){
		r = INDEX4_TO_RED( i )+8;
		g = INDEX4_TO_GREEN( i )+8;
		b = INDEX4_TO_BLUE( i)+8;

		max = 200000;
		k = -1;
		for( j = 0 ; j < pal->nColor ; j++ ){

/*
			if( (rt = r - pal->data[j].Red ) < 0 )	rt = -rt;
			if( (gt = g - pal->data[j].Green ) < 0 )	gt = -gt;
			if( (bt = b - pal->data[j].Blue ) < 0 )	bt = -bt;

			if( rt < bt )	rt = bt;
			if( rt < gt ) rt = gt;
*/

			rt = r - pal->data[j].Red;
			gt = g - pal->data[j].Green;
			bt = b - pal->data[j].Blue;

			rt = rt*rt + gt*gt + bt*bt; 

			if( rt > max )	continue;

			max = rt;
			k = j;
		}

		pal->table[i] = k;
	}
}




image_type *
image_color_quntaize( image_type *sim )
{
image_type	*im;
u_int	*tp,	*sp;
int	i,	j,	r,	g,	b;

	im = image_create( sim->row, sim->column, 4, 1, NULL );

	sp = (u_int *)sim->data;
	tp = (u_int *)im->data;

	for( i = 0; i < im->row; i++){
		for( j = 0; j < im->column; j++){

			r = IMAGE4_RED(*sp);
			g = IMAGE4_GREEN(*sp);
			b = IMAGE4_BLUE(*sp);

			sp++;

			r &= 0xf0;
			g &= 0xf0;
			b &= 0xf0;
		

			*tp++ = IMAGE4_RGB( r, g, b );
		}
	}

	return( im );
}
