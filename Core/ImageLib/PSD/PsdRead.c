/************************
 *	ImagePSDFile.c	*
 ************************/
//#include	<memory.h>
#include <string.h>


#include	"Uigp/igp.h"
#include	"Ugio/GioType.h"

#include	"ImageType/ImageType.h"

#include	"ImageReadPSD.h"



#define BitmapMode		0
#define GrayscaleMode	1
#define IndexedMode		2
#define CMYKMode		4

#define CMYKColorspace	11

#define PseudoClass		2

#define MAX_LAYERS		256

#define MaxRGB			255

#define IndexQuantum	-3
#define RedQuantum		0
#define GreenQuantum	1
#define BlueQuantum		2
#define BlackQuantum	-2
#define OpacityQuantum  -1 

#define OpaqueOpacity   0





typedef struct ChannelInfo
{
    short int type;
    unsigned long size;
} ChannelInfo;



typedef struct LayerInfoEx
{
    unsigned int	width, height;
    int				x, y;
    image_type		*image;
    unsigned short	channels;
    ChannelInfo		channel_info[24];
    char			blendkey[4];
    int				opacity; //Quantum			
    unsigned char   clipping, flags; 
	//flags & 0x01 - transpar protect( Backgraund ) 
	//flags & 0x02 - invizible
	int				matte;
	int				colors;
	char			name[32];
} LayerInfoEx;


typedef struct colormap_type {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} colormap_type;


typedef struct PSDInfo
{
	char			signature[4];
	unsigned short  channels,  version;
	unsigned char	reserved[6];
	unsigned int	rows,  columns;
	unsigned short  depth,  mode;
	int				colorspace, matte;
	int				storage_class;

	int				colormap_number;
	colormap_type	colormap[256];
} PSDInfo;


static int	psd_read_gio( gio_type *gio, psd_type **ppsd );


static LayerInfoEx *	image_read_PSD_layers( gio_type *gio, PSDInfo *psd_info, int *nLayer, int lengthInfo );

static int	psd_read_layer_info( gio_type *gio, PSDInfo *psd_info, LayerInfoEx *layer_infoEx );

static void psd_read_layer_data( gio_type *gio, PSDInfo *psd_info, LayerInfoEx *layer_infoEx );

//static void	image_read_PSD_correct( PSDInfo *psd_info, LayerInfoEx *layer_infoEx, image_type *image );


static void	psd_read_image_data( gio_type *gio, PSDInfo *psd_info, image_type **image );


static void	psd_read_image_data_compress( gio_type *gio, PSDInfo *psd_info, image_type *im );


static void	psd_read_image_data_uncompress_channel( gio_type *gio,
									PSDInfo *psd_info,
									int channel,  image_type *im );


static void	image_read_PSD_decode( gio_type *gio, PSDInfo *psd_info, 
						image_type *image, int channel );

static void	IndexPixel( PSDInfo *psd_info, u_int pixel, u_int *tp );

static void	DecoderPixel( int channel, u_int pixel, u_int *tp );


int	ReadResources( gio_type *gio );


int	
psd_read_file( char *file, psd_type **psd )
{
gio_type	*gio;
int	ret;

	gio = gio_open_file_to_read( file );
	if( gio == NULL )	return( -1 );

	ret = psd_read_gio( gio, psd );

	gio_close( gio );

	return( ret );
}




static int
psd_read_gio( gio_type *gio, psd_type **ppsd )
{
psd_type *psd;
psdLayer_type	*psdL;
int	nLayer;

PSDInfo psd_info;

int i,	pos,	sizePos;
int length, lengthInfo;

LayerInfoEx *arr_layer_infoEx;


	arr_layer_infoEx = NULL;

	//Header
	gio_get_buffer( gio, psd_info.signature, 4 );
	psd_info.version = (short)gio_get_noM( gio, 2 );

    if ( (memcmp( psd_info.signature, "8BPS", 4) != 0) || (psd_info.version != 1) )
		return( -1 );


	gio_get_buffer( gio, psd_info.reserved, 6 );
	psd_info.channels	= (short)gio_get_noM( gio, 2 ); //1..24
	psd_info.rows		= gio_get_noM( gio, 4 );
	psd_info.columns	= gio_get_noM( gio, 4 );
	psd_info.depth		= (short)gio_get_noM( gio, 2 );
	psd_info.mode		= (short)gio_get_noM( gio, 2 ); 
	//0-Bitmap, 1-Grayscale, 2-Indexed, 3-RGB, 4-CMYK, 7-Multichannel,
	//8-Duetone, 9-Lab

//yoram 13-01-13
//	psd_info.storage_class = PseudoClass;

	if (psd_info.mode == CMYKMode)
		psd_info.colorspace = CMYKColorspace;
	else
		psd_info.matte = psd_info.channels >= 4;


	psd_info.colormap_number = 0;

/////////////////////////////////////////////////	
	//Color mode data
	length = gio_get_noM( gio, 4 );
	if ((psd_info.mode == BitmapMode) || (psd_info.mode == GrayscaleMode) ||
		(psd_info.mode == IndexedMode) || (length > 0))
    {
		psd_info.colormap_number = 256;
		if (length > 0)
        {
			for (i=0; i < psd_info.colormap_number; i++)
				psd_info.colormap[i].red	=	gio_getuc( gio);
			for (i=0; i < psd_info.colormap_number; i++)
				psd_info.colormap[i].green	=	gio_getuc( gio);
			for (i=0; i < psd_info.colormap_number; i++)
				psd_info.colormap[i].blue	=	gio_getuc( gio);
        }
    }

/////////////////////////////////////////////////	
	//Image resources
	length = gio_get_noM( gio, 4 );
	pos = gio_ftell( gio );


//	ReadResources( gio );

	gio_seek( gio, pos + length );

/////////////////////////////////////////////////	

	if GIO_END( gio )
		return( -1 );


/////////////////////////////////////////////////	
	//Layer and mask information
	sizePos = gio_ftell( gio );
	lengthInfo = gio_get_noM( gio, 4 );

	if( lengthInfo <= 0 )	return( -1 );

	
	arr_layer_infoEx = image_read_PSD_layers( gio, &psd_info, &nLayer, lengthInfo );


	psd = *ppsd = psd_alloc( nLayer );
	psd->height = psd_info.rows;
	psd->width = psd_info.columns;
	psd->mode = psd_info.mode;


	for ( i = 0; i < nLayer; i++ ){
		psdL = psdLayer_alloc();
		psdL->x = arr_layer_infoEx[i].x;
		psdL->y = arr_layer_infoEx[i].y;
		psdL->height = arr_layer_infoEx[i].height;
		psdL->width = arr_layer_infoEx[i].width;
		psdL->image = arr_layer_infoEx[i].image;
		memcpy( psdL->name, arr_layer_infoEx[i].name, 32 );

		psdL->flag = arr_layer_infoEx[i].flags;
		psdL->clipping = arr_layer_infoEx[i].clipping;
		psdL->matte = arr_layer_infoEx[i].matte;
//		psdL->blendkey = arr_layer_infoEx[i].blendkey;




		psd_add_layer( psd, psdL );
	}


	free( arr_layer_infoEx );

	pos = gio_ftell( gio );


	return( 1 );

	// image data bloack

//	psd->image = image_create( psd->height, psd->width, 4, 1, NULL );
//	psd_read_layer_data( gio, psd_info, psd->image );

	psd_read_image_data( gio, &psd_info, &psd->image );

	return 1;
}





static LayerInfoEx *
image_read_PSD_layers( gio_type *gio, PSDInfo *psd_info, int *nLayer, int lengthInfo )
{
int size, i;
int	sizePos, pos;
LayerInfoEx *arr_layer_infoEx;


	sizePos = gio_ftell( gio );
    // Read layer and mask block.
	size = gio_get_noM( gio, 4 );

    
	*nLayer = (int)(short)gio_get_noM( gio, 2 );

    *nLayer = ABS( *nLayer );


	arr_layer_infoEx = (LayerInfoEx *) malloc( (*nLayer)*sizeof(LayerInfoEx) );
	if ( arr_layer_infoEx == NULL )
		return NULL;



	memset( arr_layer_infoEx, 0, *nLayer*sizeof(LayerInfoEx) );

    for (i=0; i < *nLayer; i++){

		if( psd_read_layer_info( gio, psd_info, &arr_layer_infoEx[i] ) < 0 ){
			free( arr_layer_infoEx );
			return NULL;
		
		}
    }

    // Read pixel data for each layer.
    for (i=0; i < *nLayer; i++){
		psd_read_layer_data( gio, psd_info, &arr_layer_infoEx[i] );
//		image_read_PSD_correct( psd_info, &arr_layer_infoEx[i], arr_layer_infoEx[i].image );
	}

	pos = gio_ftell( gio );

	// seek after global layer mask info
	gio_seek( gio, pos + lengthInfo - size - 4 ); 

	return arr_layer_infoEx;
}



static int
psd_read_layer_info( gio_type *gio, PSDInfo *psd_info, LayerInfoEx *layer_infoEx )
{
int size,	j;
char type[4];
int sizeMaskData, sizeBlendingData, sizeName;
char name_layer[256];


    layer_infoEx->y			= gio_get_noM( gio, 4 );
    layer_infoEx->x			= gio_get_noM( gio, 4 );
    layer_infoEx->height		= gio_get_noM( gio, 4 ) - layer_infoEx->y;
    layer_infoEx->width		= gio_get_noM( gio, 4 ) - layer_infoEx->x;
    layer_infoEx->channels	= gio_get_noM( gio, 2 );

    if (layer_infoEx->channels > 24 )
		return( -1 );


    for (j=0; j < layer_infoEx->channels; j++){
		layer_infoEx->channel_info[j].type	= gio_get_noM( gio, 2 );
		layer_infoEx->channel_info[j].size	= gio_get_noM( gio, 4 );
    }

    gio_get_buffer( gio, type, 4 );

	if ( memcmp( type, "8BIM", 4) != 0 ) 
		return( -1 );


    gio_get_buffer( gio, layer_infoEx->blendkey, 4 );
    layer_infoEx->opacity	= MaxRGB - gio_getuc(gio);
    layer_infoEx->clipping	= gio_getc(gio);
    layer_infoEx->flags		= gio_getc(gio);
    gio_getc( gio);  // padding 


/* 30.12.03  Variant without name of Layer  */
//   size=gio_get_noM( gio, 4 );
//  for ( j=0; j < size; j++)
//		gio_getc( gio);
/*30.12.03  Variant without name of Layer  */
   size=gio_get_noM( gio, 4 );

    sizeMaskData=gio_get_noM( gio, 4 );
    for ( j=0; j < sizeMaskData; j++)
		gio_getc( gio);

    sizeBlendingData=gio_get_noM( gio, 4 );
    for ( j=0; j < sizeBlendingData; j++)
		name_layer[j] = gio_getc( gio);

	sizeName = gio_getc( gio);
	gio_get_buffer( gio, name_layer, (sizeName/4*4+3) );
	name_layer[sizeName] = 0;
	memcpy( layer_infoEx->name, name_layer, 32 );

	size -= 4+sizeMaskData + 4+sizeBlendingData + (sizeName/4*4+4);
    for ( j=0; j < size; j++)
		gio_getc( gio);


		

    // Allocate layered image.
    layer_infoEx->image = image_create( layer_infoEx->height, layer_infoEx->width, 4, 1, NULL );
	image4_const( layer_infoEx->image, 0xFF000000 );

	layer_infoEx->matte		= psd_info->matte;
    if ( psd_info->mode != CMYKMode)
		layer_infoEx->matte	= layer_infoEx->channels >= 4;

	return( 1 );
}



static void
psd_read_layer_data( gio_type *gio, PSDInfo *psd_info, LayerInfoEx *layer_infoEx )
{
int compression;
int k;


	for( k = 0; k <  layer_infoEx->channels; k++){

		compression = gio_get_noM( gio, 2 );
        if (compression != 0){
			image_read_PSD_decode( gio, psd_info, layer_infoEx->image, 
							layer_infoEx->channel_info[k].type);
			continue;
        }

  
		// Read uncompressed pixel data as separate planes.

		psd_read_image_data_uncompress_channel( gio,
									psd_info,
									layer_infoEx->channel_info[k].type, 
									layer_infoEx->image );
	}
}



static void
psd_read_image_data( gio_type *gio, PSDInfo *psd_info, image_type **image )
{
int compression;


	*image = image_create( psd_info->rows, psd_info->columns, 4, 1, NULL );
	image4_const( *image, 0x00000000 );

	compression = gio_get_noM( gio, 2 );

	if (compression != 0){
		psd_read_image_data_compress( gio, psd_info, *image );
	}
	else	{
		psd_read_image_data_uncompress_channel( gio, psd_info, RedQuantum, *image );
		psd_read_image_data_uncompress_channel( gio, psd_info, GreenQuantum, *image );
		psd_read_image_data_uncompress_channel( gio, psd_info, BlueQuantum, *image );
	}

}



static void
psd_read_image_data_uncompress_channel( gio_type *gio,
									PSDInfo *psd_info,
									int channel,  image_type *im )
{
int packet_size;
int i, j;
u_int *tp, pixel;
int	shift;
u_int r, g, b;




  
	// Read uncompressed pixel data as separate planes.
	packet_size = 1;
	shift = 0;
	if (psd_info->depth > 8 ){
		packet_size++;
		shift = 8;
	}

	if( channel != 0 && channel != 1 && channel != 2 )
		channel = (psd_info->colorspace == CMYKColorspace)? BlackQuantum : OpacityQuantum;

	if( channel == 0 && psd_info->storage_class == PseudoClass)
		channel = IndexQuantum;



	tp = (u_int *)im->data;

    for( i = 0; i < im->row ; i++ )
		for( j = 0; j < im->column; j++, tp++ ){
			pixel = (u_int)gio_get_noM( gio, packet_size );

			pixel = ( pixel >>shift)&0xFF;

			switch( channel ){

			case RedQuantum		: *tp |= (pixel << 16 ); break;
			case GreenQuantum	: *tp |= ( pixel << 8 ); break;
			case BlueQuantum	: *tp |= pixel;			break;

			case BlackQuantum	: *tp |= (pixel << 24 ); break;

			case OpacityQuantum	: 
				*tp = (*tp & 0x00FFFFFF) | ((0xFF - pixel)<<24);
				break;

			case IndexQuantum:
				r = psd_info->colormap[pixel].red;
				g = psd_info->colormap[pixel].green;
				b = psd_info->colormap[pixel].blue;

				*tp = ( (r<<16) | (g<<8) |b );
				break;
			}
		}
}




void image_read_PSD_decode( gio_type *gio, PSDInfo *psd_info, 
						image_type *image, int channel)
{
int count;
long length;

int pixel;

//register IndexPacket *indexes;
register int i, x, y;
int *bytes_count_scan_line;
u_int *q;

	x=0;
	length=image->column*image->row;

	bytes_count_scan_line = (int *)malloc( sizeof(int)*image->row);

	for ( y=0; y < image->row; y++)
		bytes_count_scan_line[y] = gio_get_noM( gio, 2 );

	while (length > 0)
	{
		count = gio_getc( gio);
		if (count >= 128)
			count -= 256;
		if (count < 0)
		{
			if (count == -128)
				continue;
			pixel = gio_getc( gio);
			for ( count=(-count+1); count > 0; count--)
			{
				q = IMAGE4_PIXEL( image, x / image->column, x % image->column );

				//indexes=GetIndexes(image);
				if ( psd_info->storage_class == PseudoClass)
				{
					//*indexes=UpScale(pixel);
					IndexPixel( psd_info, pixel, q );
				}
				else
					DecoderPixel( channel, pixel, q );
				x++;
				length--;
			}
			continue;
		}
		count++;
		for (i=count; i > 0; i--)
		{
			pixel=gio_getc( gio);
			q = IMAGE4_PIXEL( image, x / image->column, x % image->column );
			//indexes=GetIndexes(image);
			if ( psd_info->storage_class == PseudoClass)
			{
				//*indexes=UpScale(pixel);
				IndexPixel( psd_info, pixel, q );
			}
			else
				DecoderPixel( channel, pixel, q );
			x++;
			length--;
		}
	}

	free( bytes_count_scan_line );
}


void DecoderPixel( int channel, u_int pixel, u_int *tp )
{
u_int c;
	
	c = pixel & 0xFF;

	switch ( channel ){
	case OpacityQuantum	: 
		*tp = (*tp & 0x00FFFFFF) | ((0xFF - c )<<24);
		break;
	case RedQuantum		: *tp |= ( c << 16 );break;
	case GreenQuantum	: *tp |= ( c << 8 );	break;
	case BlueQuantum	: *tp |= c;			break;
	case 4		: *tp |= ( c << 24 );break;
	}
}


void IndexPixel( PSDInfo *psd_info, u_int pixel, u_int *tp )
{
u_int r, g, b;

	r = psd_info->colormap[pixel].red;
	g = psd_info->colormap[pixel].green;
	b = psd_info->colormap[pixel].blue;

	*tp = ( (r<<16) | (g<<8) |b );
}





/*
static void
image_read_PSD_correct( PSDInfo *psd_info, LayerInfoEx *layer_infoEx, image_type *image )
{
int	i,	j;
u_int *tp;
u_int R, G, B, tr, bg;


	tp = (u_int *)image->data;
	if (psd_info->colorspace == CMYKColorspace){

		tp = (u_int *)image->data;
		for( i = 0 ; i < image->row ; i++)
			for( j = 0 ; j < image->column; j++, tp++ )
				*tp = 0xFFFFFF - *tp;

		return;
	}



	tp = (u_int *)image->data;

    for( i = 0 ; i < image->row ; i++)
		for( j = 0 ; j < image->column; j++, tp++ ){
			if ( (*tp & 0xFF000000) == 0xFF000000 ){
				*tp = 0xFFFFFFFF;
				continue;
			}

			// for backgraund 0x00FFFFFF
			R = (*tp >> 16) & 0xFF;
			G = (*tp >> 8) & 0xFF;
			B = *tp & 0xFF;

			tr = 0xFF - (*tp >> 24 ); 
			bg = 0xFF*(*tp >> 24 )/255;
			R = bg + ((( R * tr ) >> 8 ) & 0xFF);
			if ( R > 0xFF ) R = 0xFF;
			G = bg + ((( G * tr ) >> 8 ) & 0xFF);
			if ( G > 0xFF ) G = 0xFF;
			B = bg + ((( B * tr ) >> 8 ) & 0xFF);
			if ( B > 0xFF ) B = 0xFF;

			*tp = (*tp & 0xFF000000) | (R<<16) | (G<<8) | B;
		}


	

	if ( layer_infoEx->flags & 0x01 ){
		tp = (u_int *)image->data;
		for( i = 0 ; i < image->row ; i++)
			for( j = 0 ; j < image->column; j++, tp++ )
				*tp &= 0x00FFFFFF;
	}


	if ( layer_infoEx->flags & 0x02 )
		image4_const( image, 0xFFFFFFFF );
}
*/




static void 
psd_read_image_data_compress( gio_type *gio, PSDInfo *psd_info, image_type *im )
{
int count,	len;
int pixel,	nPixels;
int channel,	pos,	i;
u_int *tp;

	pos = gio_ftell( gio );
	gio_seek( gio, pos + im->row* psd_info->channels * 2 );



	nPixels = im->row * im->column;

	for( channel = 0; channel < psd_info->channels ; channel++ ){
						// Read the RLE data.
		count = 0;
		tp = (u_int *)im->data;

		while (count < nPixels){
			len = gio_get_noM( gio, 1 );

			if( len == 128 )	continue;

			if ( 128 > len ){
				len++;
				count += len;

				pixel = gio_getc( gio);

				for( i = 0 ; i < len ; i++ ){
					DecoderPixel( channel, pixel, tp );
					tp++;
				}

				continue;
			}

	
			// Next -len+1 bytes in the dest are replicated from next source byte.
			// (Interpret len as a negative 8-bit int.)
			len ^= 0x0FF;
			len += 2;
			count += len;

			pixel = gio_getc( gio);

			for( i = 0 ; i < len ; i++ ){
				DecoderPixel( channel, pixel, tp );
				tp++;
			}

		}
	}
}

#ifdef _AA_
struct _ImageDataBlock
{
	BYTE Type[4];  /* Always "8BIM" */
	WORD ID;       /* (See table below) */
	BYTE Name[];   /* Even-length Pascal-format string, 2 bytes or longer */
	LONG Size;     /* Length of resource data following, in bytes */
	BYTE Data[];   /* Resource data, padded to even length */
};

#endif

typedef struct _ResolutionInfo
{
	int hRes;              /* Fixed-point number: pixels per inch */
	short hResUnit;          /* 1=pixels per inch, 2=pixels per centimeter */
	short WidthUnit;         /* 1=in, 2=cm, 3=pt, 4=picas, 5=columns */
	int vRes;              /* Fixed-point number: pixels per inch */
	short vResUnit;          /* 1=pixels per inch, 2=pixels per centimeter */
	short HeightUnit;        /* 1=in, 2=cm, 3=pt, 4=picas, 5=columns */
} RESOLUTIONINFO;


int	ReadImageBlock( gio_type *gio );


int	ReadResources( gio_type *gio )
{
	while( 1 )
		ReadImageBlock( gio );
}

int
ReadImageBlock( gio_type *gio )
{
char	type[8],	name[1024];
int	id,	len,	size,	pos;

RESOLUTIONINFO aa;

	gio_get_buffer( gio, type, 4 );
	type[4] = 0;

	id	= (short)gio_get_noM( gio, 2 ); 

	len	= (short)gio_get_noM( gio, 1 ); 
//	if( len > 0 ){
		gio_get_buffer( gio, name, len+1 );
		name[len+1] = 0;
//	}


	size	= gio_get_noM( gio, 4 );

	pos = gio_ftell( gio );

	if( id == 0x03ed ){
		aa.hRes = gio_get_noM( gio, 4 );              /* Fixed-point number: pixels per inch */
		aa.hResUnit = gio_get_noM( gio, 2 );          /* 1=pixels per inch, 2=pixels per centimeter */
		aa.WidthUnit = gio_get_noM( gio, 2 );         /* 1=in, 2=cm, 3=pt, 4=picas, 5=columns */
		aa.vRes = gio_get_noM( gio, 4 );;            /* Fixed-point number: pixels per inch */
		aa.vResUnit = gio_get_noM( gio, 2 );          /* 1=pixels per inch, 2=pixels per centimeter */
		aa.HeightUnit = gio_get_noM( gio, 2 );      /* 1=in, 2=cm, 3=pt, 4=picas, 5=columns */
	}


	if( size &0x01 )	size += 1;
	gio_seek( gio, pos + size );

	return( 1 );
}