/********************************
 ***	ImageSmartPalette.c   ***
 ********************************/

#include	<stdlib.h>
#include	"Uigp/igp.h"
#include	"Ubox/Box2d.h"
#include	"ImageType/ImageType.h"


#define NBITS 5
#define PARTS (1 << (3 * NBITS))


typedef struct cell
{
	int shows;
	int rgb;
	int visited;
}cell;


static int	image_smart_paltte_get_max( int *h, int no );
static void bubble_sort_cell(cell*H, int NUM);
static void bubble_sort_cellPtr(cell**cellPtr, int NUM);



palette_type * 
image_createSmartPalette(int n, image_type * im)
{
palette_type	*pal;
u_int	*p;
int	i, j;
int r, g, b;
int *H;
int shift2;
int shiftRight;
int	k;

	shiftRight = 8 - NBITS;
	shift2 = NBITS << 1;
	pal = palette_alloc(n);
	i = PARTS;

	H = (int *)malloc(sizeof(int)*  PARTS);

	for (i = 0; i < PARTS;  i++)
		H[i] = 0;


	p = (unsigned long *)im->data;

	for( i = 0; i < im->row; i++){
		for( j = 0; j < im->column; j++, p++){

			if ( (*p) & 0xFF000000 )	continue;

			r = IMAGE4_RED(*p);
			g = IMAGE4_GREEN(*p);
			b = IMAGE4_BLUE(*p);

			k = ((r >> shiftRight) << shift2) | ((g >> shiftRight) << NBITS) | (b >> shiftRight);

			H[k]++;
		}
	}



	for (i = 0; i < n; i++){
		k = image_smart_paltte_get_max( H, PARTS );
		if( k < 0 )	break;


		pal->data[i].Red   = (unsigned char)((k >> shift2) << shiftRight);
		pal->data[i].Green = (unsigned char)(((k >> NBITS) << shiftRight) & 0xff);
		pal->data[i].Blue  = (unsigned char)((k << shiftRight) & 0xff);


		H[k] = 0;
	}


	free(H);

	pal->type = PALETTE_UNKNOWN;
	return( pal );
}




palette_type * 
image_createSmartPaletteTransparent2( int n, 
				int nImages, image_type *images[] )
{
image_type	*im;
palette_type	*pal;
u_int	*p;
int	i, j;
int r, g, b;
cell *H;
int shift2, shiftRight;
int	k;
cell **peaks;
int index = 0, lastI, palI, pInd;
int s;


	shiftRight = 8 - NBITS;
	shift2 = NBITS << 1;
	pal = palette_alloc(n);
	i = PARTS;

	H = (cell *)malloc(sizeof(cell)*  PARTS);
	peaks = (cell **)malloc(sizeof(cell*)*PARTS);

	for (i = 0; i < PARTS;  i++)
	{
		H[i].rgb = i;
		H[i].shows = H[i].visited = 0;
	}

	
	

	for( s = 0 ; s < nImages ; s ++ )
	{
		im = images[s];

		p = (unsigned long *)im->data;

		for( i = 0; i < im->row; i++){
			for( j = 0; j < im->column; j++, p++){

				if ( (*p) & 0xFF000000 )	continue;

				r = IMAGE4_RED(*p);
				g = IMAGE4_GREEN(*p);
				b = IMAGE4_BLUE(*p);
				//k = IMAGE4_RGB(r,g,b);
				k = ((r >> shiftRight) << shift2) | ((g >> shiftRight) << NBITS) | (b >> shiftRight);

				H[k].shows++;
			}
		}
	}

	
	
	for (i = 1 ; i < PARTS ;  )
	{
		lastI = i - 1 ;
		// search for next peak
		for (; i < PARTS ; i ++ )
		{
			if ( H[i].shows <= 50 )
				continue;
			if ( H[i].shows < H[lastI].shows)
			{
				peaks[index++] = &H[lastI];
				lastI = i;
				i++;
				break;
			}
			lastI = i;
		}
		// go down till bottom.
		for (; i < PARTS ; i ++ )		
		{
			if ( H[i].shows == 0  )
				continue;
			if ( H[i].shows > H[lastI].shows)
			{
				i++;
				break;
			}
			lastI = i;
		}
	}

	//if (index > n)	
	bubble_sort_cellPtr(peaks,index);

	palI = 0;

	if ( index > n/2)
		pInd = n/2;
	else
		pInd = index;

	/*if (index > n )
	{// get the first  peaks
		for ( i = 0; i < n; i++)
		{
			k = peaks[i]->rgb;
				//r = IMAGE4_RED(*p);
				//g = IMAGE4_GREEN(*p);
				//b = IMAGE4_BLUE(*p);
			pal->data[palI].Red   = (unsigned char)((k >> shift2) << shiftRight);
			pal->data[palI].Green = (unsigned char)(((k >> NBITS) << shiftRight) & 0xff);
			pal->data[palI++].Blue  = (unsigned char)((k << shiftRight) & 0xff);
		}

	}
	else
	*/

	//red
	pal->data[palI].Red   = 255;
	pal->data[palI].Green = 0;
	pal->data[palI++].Blue  = 0;
	//green
	pal->data[palI].Red   = 0;
	pal->data[palI].Green = 255;
	pal->data[palI++].Blue  = 0;
	//blue
	pal->data[palI].Red   = 0;
	pal->data[palI].Green = 0;
	pal->data[palI++].Blue  = 255;
	//black
	pal->data[palI].Red   = 255;
	pal->data[palI].Green = 255;
	pal->data[palI++].Blue  = 255;
	//white
	if ( n > 4)
	{
		pal->data[palI].Red   = 0;
		pal->data[palI].Green = 0;
		pal->data[palI++].Blue  = 0;
	}	



	{	// get last peaks!!! (because first peaks would appear at high viewed
		for ( i = 0; i < pInd && palI < n; i++)
		{
			k = peaks[index-i-1]->rgb;
			
			pal->data[palI].Red   = (unsigned char)((k >> shift2) << shiftRight);
			pal->data[palI].Green = (unsigned char)(((k >> NBITS) << shiftRight) & 0xff);
			pal->data[palI++].Blue  = (unsigned char)((k << shiftRight) & 0xff);
			peaks[index-i-1]->visited = 1;
		}
		bubble_sort_cell(H,PARTS);
		// get rest of the colors, from most appearing to least appearing
		for (i = 0 ; i < PARTS  && palI < n; i ++)
		{
			if ( H[i].visited == 0)
			{
				k = H[i].rgb;				
				pal->data[palI].Red   = (unsigned char)((k >> shift2) << shiftRight);
				pal->data[palI].Green = (unsigned char)(((k >> NBITS) << shiftRight) & 0xff);
				pal->data[palI++].Blue  = (unsigned char)((k << shiftRight) & 0xff);
				H[i].visited = 1;
			}
		}	
		while ( palI < n )
		{
			pal->data[palI].Red   = 0;
			pal->data[palI].Green = 0;
			pal->data[palI++].Blue  = 0;

		}
		
	}
	

	free(H);
	free(peaks);

	pal->type = PALETTE_UNKNOWN;
	return( pal );

}


static void bubble_sort_cell(cell*H, int NUM)
{
	int i,j,high,indHigh;
	cell tmp;
	for ( i = 0 ; i < NUM - 1 ; i ++)
	{
		high = H[i].shows;
		indHigh = i;
		for ( j = i + 1 ; j < NUM ; j ++)
		{
			if ( H[j].shows > high)
			{
				high = H[j].shows;
				indHigh = j;
			}			
		}// for j
		tmp = H[indHigh];
		H[indHigh] = H[i];
		H[i] = tmp;
	}// for i
}//bubble_sort_cell
static void bubble_sort_cellPtr(cell**cellPtr, int NUM)
{
	int i,j,high,indHigh;
	cell *tmp;
	for ( i = 0 ; i < NUM - 1 ; i ++)
	{
		high = cellPtr[i]->shows;
		indHigh = i;
		for ( j = i + 1 ; j < NUM ; j ++)
		{
			if ( cellPtr[j]->shows > high)
			{
				high = cellPtr[j]->shows;
				indHigh = j;
			}			
		}// for j
		tmp = cellPtr[indHigh];
		cellPtr[indHigh] = cellPtr[i];
		cellPtr[i] = tmp;
	}// for i
}



palette_type * 
image_createSmartPaletteTransparent( int n, 
				int nImages, image_type *images[] )
{
image_type	*im;
palette_type	*pal;
u_int	*p;
int	i, j;
int r, g, b;
int *H;
int shift2;
int shiftRight;
int	k,	s;

	shiftRight = 8 - NBITS;
	shift2 = NBITS << 1;
	pal = palette_alloc(n);
	i = PARTS;

	H = (int *)malloc(sizeof(int)*  PARTS);

	for (i = 0; i < PARTS;  i++)
		H[i] = 0;


	for( s = 0 ; s < nImages ; s++ ){
		im = images[s];

		p = (unsigned long *)im->data;

		for( i = 0; i < im->row; i++){
			for( j = 0; j < im->column; j++, p++){

				if ( (*p) & 0xFF000000 )	continue;

				r = IMAGE4_RED(*p);
				g = IMAGE4_GREEN(*p);
				b = IMAGE4_BLUE(*p);

				k = ((r >> shiftRight) << shift2) | ((g >> shiftRight) << NBITS) | (b >> shiftRight);

				H[k]++;
			}
		}
	}

	

	for (i = 0; i < n; i++){
		k = image_smart_paltte_get_max( H, PARTS );
		if( k < 0 )	break;


		pal->data[i].Red   = (unsigned char)((k >> shift2) << shiftRight);
		pal->data[i].Green = (unsigned char)(((k >> NBITS) << shiftRight) & 0xff);
		pal->data[i].Blue  = (unsigned char)((k << shiftRight) & 0xff);


		H[k] = 0;
	}


	free(H);

	pal->type = PALETTE_UNKNOWN;
	return( pal );
}



static int
image_smart_paltte_get_max( int *h, int no )
{
int	i;
int	max,	k;

	max = 0;
	k = -1;
	for (i = 0; i < no;  i++ )
		if( h[i] > max ){
			max = h[i];
			k = i;
		}

	return( k );
}
