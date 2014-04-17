/************************
 *	ImagePSDFile.c	*
 ************************/
#include	<memory.h>


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

#define IndexQuantum	0
#define RedQuantum		0
#define GreenQuantum	1
#define BlueQuantum		2
#define BlackQuantum	-1
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



static image_type *	image_read_PSD( gio_type *gio );

static LayerInfoEx * image_read_PSD_layers( gio_type *gio, PSDInfo *psd_info, 
						int *number_layers );

static void image_read_PSD_layer1( gio_type *gio, PSDInfo *psd_info, 
					  LayerInfoEx *layer_infoEx );

static void image_read_PSD_correct( PSDInfo *psd_info, image_type *image,
							  LayerInfoEx *layer_infoEx );

static void image_read_PSD_set_pixel( PSDInfo *psd_info, int type, 
							u_int *pData, u_int pixel, int packet_size );

static void QuantumPixel( int Quantum, u_int pixel, 
						int packet_size, u_int *pData );

static void	IndexPixel( PSDInfo *psd_info, u_int pixel, u_int *pData );

static void image_read_PSD_image( gio_type *gio, PSDInfo *psd_info, 
						image_type *image, int number_layers );

static void	image_read_PSD_decode( gio_type *gio, PSDInfo *psd_info, 
						image_type *image, int channel);

static void	DecoderPixel( int channel, u_int pixel, u_int *pData );


static void	image4_copy_in_transparent( image_type *sim, image_type *im, 
						   int row0, int col0, u_int opacity );

static void	image4_clear_transparent( image_type *image );




static psdLayer_type *	image_read_PSDlayers( gio_type *gio, int *number_layers, 
											 int *row, int *column);


int	
psd_read_fileO( char *file, psd_type **psd )
{
psdLayer_type *imL,	*psdL;
int	height,	width,	nLayer;
int	i;

	nLayer = image_read_PSDLayer_file( file, &imL, &height, &width );
	if( nLayer == 0 )	return( -1 );


	*psd = psd_alloc( nLayer );
	(*psd)->width = width;
	(*psd)->height = height;

	for( i = 0 ; i < nLayer ; i++ ){
		psdL = psdLayer_make_copy( &imL[i] );

		psd_add_layer( *psd, psdL );

		image_destroy( imL[i].image, 1 );
	}

	free( imL );

	return( 1 );
}




int	
image_read_PSDLayer_file( char *file, psdLayer_type **imL, int *row, int *column )
{
gio_type	*gio;
int	layer_no;

	gio = gio_open_file_to_read( file );
	if( gio == NULL )	return( 0 );

	layer_no = 0;
	*imL = ( psdLayer_type *)image_read_PSDlayers( gio, &layer_no, row, column );
	gio_close( gio );

	return( layer_no );
}





image_type *
image_read_PSD_file( char *file )
{
gio_type	*gio;
image_type	*im;

	gio = gio_open_file_to_read( file );
	if( gio == NULL )	return( NULL );

	im = image_read_PSD( gio );

	gio_close( gio );

	return( im );
}


image_type *
image_read_PSD_buffer( char *data, int bytes, int Fdata )
{
gio_type	*gio;
image_type	*im;

	gio = gio_open_buffer_to_read( data, bytes, Fdata );

	im = image_read_PSD( gio );

	gio_close( gio );

	return( im );
}


static image_type *
image_read_PSD( gio_type *gio )
{
image_type	*im;
PSDInfo psd_info;

int i;
int length;

int number_layers = 0;
LayerInfoEx *arr_layer_infoEx;

long pos1;

/////////////////////////////////////////////////	
	//Header
	gio_get_buffer( gio, psd_info.signature, 4 );
	psd_info.version = (short)gio_get_noM( gio, 2 );

    if ( (memcmp( psd_info.signature, "8BPS", 4) != 0) ||
         (psd_info.version != 1) )
		return NULL;


	gio_get_buffer( gio, psd_info.reserved, 6 );
	psd_info.channels	= (short)gio_get_noM( gio, 2 ); //1..24
	psd_info.rows		= gio_get_noM( gio, 4 );
	psd_info.columns	= gio_get_noM( gio, 4 );
	psd_info.depth		= (short)gio_get_noM( gio, 2 );
	psd_info.mode		= (short)gio_get_noM( gio, 2 ); 
	//0-Bitmap, 1-Grayscale, 2-Indexed, 3-RGB, 4-CMYK, 7-Multichannel,
	//8-Duetone, 9-Lab

	//psd_info.storage_class = ;

	if (psd_info.mode == CMYKMode)
		psd_info.colorspace = CMYKColorspace;
	else
		psd_info.matte = psd_info.channels >= 4;

	im = image_create( psd_info.rows, psd_info.columns, 4, 1, NULL );
	image4_const( im, 0 );

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
	if (length > 0)
    {
		unsigned char *data;

		data = (unsigned char *)malloc( length );
		if (data == NULL)
		{
			image_destroy( im, 1 );
			return NULL;
		}

		gio_get_buffer( gio, data, length );

		// If we want all picture ( no layer ) - used data
		// Now data no iteresting our
		/*if ( memcmp( data, "8BIM", 4) != 0 ) 
		{
			image_destroy( im, 1 );
			return NULL;
		}*/

		free( data );
		//iptc_profile.info = data;
		//iptc_profile.length = length;
	}
/////////////////////////////////////////////////	

	if GIO_END( gio )
		return im;


/////////////////////////////////////////////////	
	//Layer and mask information
	length = gio_get_noM( gio, 4 );
	pos1 = gio_ftell( gio );

	if (length > 0)
		 arr_layer_infoEx = image_read_PSD_layers( gio, &psd_info, &number_layers );

	gio_seek( gio, pos1 + length );


/////////////////////////////////////////////////	
	//Image data
	image_read_PSD_image( gio, &psd_info, im, number_layers );
	image_read_PSD_correct( &psd_info, im, NULL );

	//IsMatteImage(image);

	for (i=0; i < number_layers; i++)
	{// Composite layer onto image.
    
		if ((arr_layer_infoEx[i].width != 0) && (arr_layer_infoEx[i].height != 0))
		{
			image4_copy_in_transparent( arr_layer_infoEx[i].image, im, 
						arr_layer_infoEx[i].y, arr_layer_infoEx[i].x,
						arr_layer_infoEx[i].opacity );
			image_destroy( arr_layer_infoEx[i].image, 1 );
			arr_layer_infoEx[i].image = NULL;
		}
	}

	free( arr_layer_infoEx );

	psd_info.matte = 0;
	if (psd_info.colorspace != CMYKColorspace)
		psd_info.matte = psd_info.channels >= 4;

	return im;
}









static psdLayer_type *
image_read_PSDlayers( gio_type *gio, int *number_layers, 
					   int *row, int *column)
{
PSDInfo psd_info;

int i;
int length;

LayerInfoEx *arr_layer_infoEx;
psdLayer_type *arr_layer_info;

	*number_layers = 0;
	arr_layer_infoEx = NULL;

	//Header
	gio_get_buffer( gio, psd_info.signature, 4 );
	psd_info.version = (short)gio_get_noM( gio, 2 );

    if ( (memcmp( psd_info.signature, "8BPS", 4) != 0) ||
         (psd_info.version != 1) )
		return NULL;


	gio_get_buffer( gio, psd_info.reserved, 6 );
	psd_info.channels	= (short)gio_get_noM( gio, 2 ); //1..24
	psd_info.rows		= gio_get_noM( gio, 4 );
	psd_info.columns	= gio_get_noM( gio, 4 );
	psd_info.depth		= (short)gio_get_noM( gio, 2 );
	psd_info.mode		= (short)gio_get_noM( gio, 2 ); 
	//0-Bitmap, 1-Grayscale, 2-Indexed, 3-RGB, 4-CMYK, 7-Multichannel,
	//8-Duetone, 9-Lab
	*row = psd_info.rows;
	*column = psd_info.columns;

	//psd_info.storage_class = ;

	if (psd_info.mode == CMYKMode)
		psd_info.colorspace = CMYKColorspace;
	else
		psd_info.matte = psd_info.channels >= 4;

	//im = image_create( psd_info.rows, psd_info.columns, 4, 1, NULL );
	//image4_const( im, 0 );

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
	if (length > 0)
    {
		unsigned char *data;

		data = (unsigned char *)malloc( length );
		if (data == NULL)
			return NULL;
		gio_get_buffer( gio, data, length );
		free( data );
	}
/////////////////////////////////////////////////	

	if GIO_END( gio )
		return NULL;


/////////////////////////////////////////////////	
	//Layer and mask information
	length = gio_get_noM( gio, 4 );

	if (length > 0)
		 arr_layer_infoEx = image_read_PSD_layers( gio, &psd_info, number_layers );

	arr_layer_info = (psdLayer_type *)malloc( (*number_layers)*sizeof(psdLayer_type) );
	for ( i = 0; i < *number_layers; i++ )
	{
		arr_layer_info[i].x = arr_layer_infoEx[i].x;
		arr_layer_info[i].y = arr_layer_infoEx[i].y;
		arr_layer_info[i].height = arr_layer_infoEx[i].height;
		arr_layer_info[i].width = arr_layer_infoEx[i].width;
		arr_layer_info[i].image = arr_layer_infoEx[i].image;
		memcpy( arr_layer_info[i].name, arr_layer_infoEx[i].name, 32 );
	}

	free( arr_layer_infoEx );

	return arr_layer_info;
}


int
image_read_PSDlayers_number( char *file )//gio_type *gio)
{
PSDInfo psd_info;

int length;
long pos1;

int number_layers = 0;

gio_type	*gio;

	gio = gio_open_file_to_read( file );
	if( gio == NULL )	
		return 0;

	//Header
	gio_get_buffer( gio, psd_info.signature, 4 );
	psd_info.version = (short)gio_get_noM( gio, 2 );

    if ( (memcmp( psd_info.signature, "8BPS", 4) != 0) ||
         (psd_info.version != 1) )
	{
		gio_close( gio );
		return 0;
	}


	gio_get_buffer( gio, psd_info.reserved, 6 );
	psd_info.channels	= (short)gio_get_noM( gio, 2 ); //1..24
	psd_info.rows		= gio_get_noM( gio, 4 );
	psd_info.columns	= gio_get_noM( gio, 4 );
	psd_info.depth		= (short)gio_get_noM( gio, 2 );
	psd_info.mode		= (short)gio_get_noM( gio, 2 ); 
	//0-Bitmap, 1-Grayscale, 2-Indexed, 3-RGB, 4-CMYK, 7-Multichannel,
	//8-Duetone, 9-Lab

	//psd_info.storage_class = ;

	if (psd_info.mode == CMYKMode)
		psd_info.colorspace = CMYKColorspace;
	else
		psd_info.matte = psd_info.channels >= 4;

	//im = image_create( psd_info.rows, psd_info.columns, 4, 1, NULL );
	//image4_const( im, 0x0 );

	psd_info.colormap_number = 0;

/////////////////////////////////////////////////	
	//Color mode data
	length = gio_get_noM( gio, 4 );
	pos1 = gio_ftell( gio );
/*
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
*/
	gio_seek( gio, pos1 + length );

/////////////////////////////////////////////////	
	//Image resources
	length = gio_get_noM( gio, 4 );
	pos1 = gio_ftell( gio );
/*
	if (length > 0)
    {
		unsigned char *data;

		data = (unsigned char *)malloc( length );
		if (data == NULL)
		{
			gio_close( gio );
			return 0;
		}
		gio_get_buffer( gio, data, length );
		free( data );
	}
*/
	gio_seek( gio, pos1 + length );
/////////////////////////////////////////////////	

	if GIO_END( gio )
	{
		gio_close( gio );
		return 0;
	}


/////////////////////////////////////////////////	
	//Layer and mask information
	number_layers = 0;
	length = gio_get_noM( gio, 4 );

	if (length > 0)
	{
		length = gio_get_noM( gio, 4 );
		number_layers = (int)(short)gio_get_noM( gio, 2 );
		number_layers = ABS( number_layers );
	}

	gio_close( gio );
	return number_layers;
}


static LayerInfoEx *
image_read_PSD_layers( gio_type *gio, PSDInfo *psd_info, 
					   int *number_layers )
{
int size, i, j;
char type[4];

LayerInfoEx *arr_layer_infoEx;
int sizeMaskData, sizeBlendingData, sizeName;
char name_layer[256];


	*number_layers = 0;
    // Read layer and mask block.
	size = gio_get_noM( gio, 4 );

    
	*number_layers = (int)(short)gio_get_noM( gio, 2 );

    *number_layers = ABS( *number_layers );

	arr_layer_infoEx = (LayerInfoEx *) malloc( (*number_layers)*sizeof(LayerInfoEx) );
	if ( arr_layer_infoEx == NULL )
		return NULL;

	memset( arr_layer_infoEx, 0, *number_layers*sizeof(LayerInfoEx) );

    for (i=0; i < *number_layers; i++)
    {
        arr_layer_infoEx[i].y			= gio_get_noM( gio, 4 );
        arr_layer_infoEx[i].x			= gio_get_noM( gio, 4 );
        arr_layer_infoEx[i].height		= gio_get_noM( gio, 4 ) - arr_layer_infoEx[i].y;
        arr_layer_infoEx[i].width		= gio_get_noM( gio, 4 ) - arr_layer_infoEx[i].x;
        arr_layer_infoEx[i].channels	= gio_get_noM( gio, 2 );

        if (arr_layer_infoEx[i].channels > 24)
		{
			free( arr_layer_infoEx );
			return NULL;
		}

        for (j=0; j < arr_layer_infoEx[i].channels; j++)
        {
          arr_layer_infoEx[i].channel_info[j].type	= gio_get_noM( gio, 2 );
          arr_layer_infoEx[i].channel_info[j].size	= gio_get_noM( gio, 4 );
        }

        gio_get_buffer( gio, type, 4 );
	
		if ( memcmp( type, "8BIM", 4) != 0 ) 
		{
			free( arr_layer_infoEx );
			return NULL;
		}

        gio_get_buffer( gio, arr_layer_infoEx[i].blendkey, 4 );
        arr_layer_infoEx[i].opacity	= MaxRGB - gio_getuc(gio);
        arr_layer_infoEx[i].clipping	= gio_getc(gio);
        arr_layer_infoEx[i].flags		= gio_getc(gio);
        gio_getc( gio);  // filler 

/* 30.12.03  Variant without name of Layer  
        size=gio_get_noM( gio, 4 );
        for ( j=0; j < size; j++)
			gio_getc( gio);
30.12.03  Variant without name of Layer  */
        size=gio_get_noM( gio, 4 );
        sizeMaskData=gio_get_noM( gio, 4 );
        for ( j=0; j < sizeMaskData; j++)
			gio_getc( gio);
        sizeBlendingData=gio_get_noM( gio, 4 );
        for ( j=0; j < sizeBlendingData; j++)
			gio_getc( gio);
		sizeName = gio_getc( gio);
		gio_get_buffer( gio, name_layer, (sizeName/4*4+3) );
		name_layer[sizeName] = 0;
		size -= 4+sizeMaskData + 4+sizeBlendingData + (sizeName/4*4+4);
        for ( j=0; j < size; j++)
			gio_getc( gio);
		memcpy( arr_layer_infoEx[i].name, name_layer, 32 );
//reading name layer
		
        // Allocate layered image.
        arr_layer_infoEx[i].image = image_create(
			arr_layer_infoEx[i].height, arr_layer_infoEx[i].width, 
			4, 1, NULL );
		image4_const( arr_layer_infoEx[i].image, 0xFF000000 );

		arr_layer_infoEx[i].matte		= psd_info->matte;
        if ( psd_info->mode != CMYKMode)
			arr_layer_infoEx[i].matte	= arr_layer_infoEx[i].channels >= 4;
    }

    // Read pixel data for each layer.
    for (i=0; i < *number_layers; i++)
	{
		image_read_PSD_layer1( gio, psd_info, &arr_layer_infoEx[i] );
		image_read_PSD_correct( psd_info, arr_layer_infoEx[i].image,
										&arr_layer_infoEx[i] );
	}

    for ( i = 0; i < 16; i++)
        gio_getc( gio);

	return arr_layer_infoEx;
}


static void
image_read_PSD_layer1( gio_type *gio, PSDInfo *psd_info, LayerInfoEx *layer_infoEx )
{
int j, x, y;
int compression, packet_size;
u_int *pData, pixel;


	for (j = 0; j <  layer_infoEx->channels; j++)
    {
		compression = gio_get_noM( gio, 2 );
        if (compression != 0)
        {
			image_read_PSD_decode( gio, psd_info, layer_infoEx->image, 
							layer_infoEx->channel_info[j].type);
        }
        else
        {// Read uncompressed pixel data as separate planes.
			packet_size=1;
            /*if (layer_infoEx->storage_class == PseudoClass)
            {
				if (layer_infoEx->colors > 256)
                    packet_size++;
            }
            else*/
                if (psd_info->depth > 8 )
					packet_size++;

			pData = (u_int *)layer_infoEx->image->data;

            for ( y=0; y < layer_infoEx->image->row; y++)
            for ( x=0; x < layer_infoEx->image->column; x++, pData++ )
            {
                pixel = (u_int)gio_get_noM( gio, packet_size );
				image_read_PSD_set_pixel( psd_info, layer_infoEx->channel_info[j].type, 
					pData, pixel, packet_size );
			}
		}
	}
}


static void
image_read_PSD_correct( PSDInfo *psd_info, image_type *image, 
							  LayerInfoEx *layer_infoEx )
{
int x, y;
u_int *pData;
u_int R, G, B, tr, bg;

	pData = (u_int *)image->data;

	if (psd_info->colorspace == CMYKColorspace)
	{// Correct CMYK levels.
		pData = (u_int *)image->data;

        for ( y=0; y < image->row; y++)
        for ( x=0; x < image->column; x++, pData++ )
			*pData = 0xFFFFFF - *pData;

		return;
	}

	/*
	if (layer_infoEx && layer_infoEx->opacity != OpaqueOpacity)
	{ // Correct for opacity level.
		pData = (u_int *)image->data;

        for ( y=0; y < layer_infoEx->image->row; y++)
        for ( x=0; x < layer_infoEx->image->column; x++, pData++ )
			;//pData->opacity=(unsigned long)(pData->opacity*layer_infoEx[i].opacity)/MaxRGB;
	}
	*/
	pData = (u_int *)image->data;

    for ( y=0; y < layer_infoEx->image->row; y++)
    for ( x=0; x < layer_infoEx->image->column; x++, pData++ )
	{
		if ( (*pData & 0xFF000000) == 0xFF000000 )
			*pData = 0xFFFFFFFF;
		else 
		{
			// for backgraund 0x00FFFFFF
			R = (*pData >> 16) & 0xFF;
			G = (*pData >> 8) & 0xFF;
			B = *pData & 0xFF;

			tr = 0xFF - (*pData >> 24 ); 
			bg = 0xFF*(*pData >> 24 )/255;
			R = bg + ((( R * tr ) >> 8 ) & 0xFF);
			if ( R > 0xFF ) R = 0xFF;
			G = bg + ((( G * tr ) >> 8 ) & 0xFF);
			if ( G > 0xFF ) G = 0xFF;
			B = bg + ((( B * tr ) >> 8 ) & 0xFF);
			if ( B > 0xFF ) B = 0xFF;

			*pData = (*pData & 0xFF000000) | (R<<16) | (G<<8) | B;

		}
	}

	pData = (u_int *)image->data;

	if ( layer_infoEx->flags & 0x01 )
    for ( y=0; y < layer_infoEx->image->row; y++)
    for ( x=0; x < layer_infoEx->image->column; x++, pData++ )
		*pData &= 0x00FFFFFF;

	if ( layer_infoEx->flags & 0x02 )
		image4_const( layer_infoEx->image, 0xFFFFFFFF );
}


void 
image_read_PSD_set_pixel( PSDInfo *psd_info, int type,  
				u_int *pData, u_int pixel, int packet_size )
{
	switch (type)
    {
		case 0:
        {
			if (psd_info->storage_class == PseudoClass)
				IndexPixel( psd_info, pixel, pData ); //IndexQuantum, 
            else
				QuantumPixel( RedQuantum, pixel, packet_size, pData );
        }
        break;
        case 1:
		{
			QuantumPixel( GreenQuantum, pixel, packet_size, pData);
		}
		break;
		case 2:
		{
			QuantumPixel( BlueQuantum, pixel, packet_size, pData);
		}
		break;
		case 3:
		default:
		{
			if (psd_info->colorspace == CMYKColorspace)
				QuantumPixel( BlackQuantum, pixel, packet_size, pData);
			else
				QuantumPixel( OpacityQuantum, pixel, packet_size, pData);
		}
		break;
	}
}


void QuantumPixel( int Quantum, u_int pixel, 
				   int packet_size, u_int *pData )
{
u_int c;
	
	c = pixel & 0xFF;
	if ( packet_size == 2 )
		c = (pixel >> 8) & 0xFF;

	switch ( Quantum )
	{
		case OpacityQuantum	: 
			//if ( c ) *pData &=  0x00FFFFFF; 
			*pData &=  (( (0xFF - c) << 24) | 0x00FFFFFF); 
			break;
		case RedQuantum		: *pData |= (c << 16 ); break;
		case GreenQuantum	: *pData |= ( c << 8 ); break;
		case BlueQuantum	: *pData |= c;			break;
	}

}


void IndexPixel( PSDInfo *psd_info, u_int pixel, u_int *pData )
{
u_int r, g, b;

	r = psd_info->colormap[pixel].red;
	g = psd_info->colormap[pixel].green;
	b = psd_info->colormap[pixel].blue;

	*pData = ( (r<<16) | (g<<8) |b );
}


static void 
image_read_PSD_image( gio_type *gio, PSDInfo *psd_info, 
					  image_type *image, int number_layers )
{
int i, x, y;
int compression, packet_size;
u_int *pData, pixel;

	compression = gio_get_noM( gio, 2 );
	//SetImage(image,OpaqueOpacity);

	if (compression != 0)
    {//Read Packbit encoded pixel data as separate planes.
		for ( i = 0; i < psd_info->channels; i++)
			image_read_PSD_decode( gio, psd_info, image, 
										psd_info->matte ? i-1 : i);
    }
	else
    {// Read uncompressed pixel data as separate planes.
      
		packet_size=1;
		/*if ( psd_info->storage_class == PseudoClass )
		{
			if (image->colors > 256)
				packet_size++;
        }
		else*/ if (psd_info->depth > 8)
			packet_size++;

	    for ( i=0; i < (int) psd_info->channels; i++)
		{
			pData = (u_int *)image->data;

			for ( y=0; y < image->row; y++)
			for ( x=0; x < image->column; x++, pData++ )
			{
				pixel = (u_int)gio_get_noM( gio, packet_size );
				image_read_PSD_set_pixel( psd_info, i, 
								pData, pixel, packet_size );
			}
		}
		if ( psd_info->matte && (number_layers != 0) )
			;//MatteImage( image, TransparentOpacity);
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


void DecoderPixel( int channel, u_int pixel, u_int *pData )
{
u_int c;
u_int o;
	
	c = pixel & 0xFF;

	switch ( channel )
	{
		case OpacityQuantum	: 
			//if ( c )  *pData &=  0x00FFFFFF; 
			//*pData &=  (( (0xFF - c) << 24) | 0x00FFFFFF); 
			o = ((0xFF - c) << 24) | 0x00FFFFFF;
			*pData &= o; 
			break;
		case RedQuantum		: *pData |= ( c << 16 );break;
		case GreenQuantum	: *pData |= ( c << 8 );	break;
		case BlueQuantum	: *pData |= c;			break;
	}
}


static void
image4_copy_in_transparent( image_type *sim, image_type *im, 
						   int row0, int col0, u_int opacity )
{
///////////////////////////////////////
// Error if row<0 || col<0 || im<sim
///////////////////////////////////////

u_int	*p,	*sp, trans;
int	i,	j,	algin;
u_int R, G, B, Rb, Gb, Bb;

	
	algin = im->column - sim->column;
	sp = (u_int *)sim->data;
	p = IMAGE4_PIXEL( im, row0, col0 );

	for( i = 0 ; i < sim->row ; i++, p += algin ){
		for( j = 0 ; j < sim->column ; j++, p++, sp++ )
			if ( !(*sp & 0xFF000000 ) && (opacity==0) )
				*p = *sp;
			else if ( (*sp & 0xFF000000 ) != 0xFF000000 &&
				      (opacity!=0xFF) )
			{
				R = ((*sp) >> 16) & 0xFF; 
				G = ((*sp) >> 8) & 0xFF;
				B = (*sp) & 0xFF;

				Rb = ((*p) >> 16) & 0xFF; 
				Gb = ((*p) >> 8) & 0xFF;
				Bb = (*p) & 0xFF;

				trans = ((*sp) >> 24) + opacity;
				if ( trans > 255 ) trans = 255;

				R = ( R - ((R-Rb)*trans)/255) & 0xFF;
				G = ( G - ((G-Gb)*trans)/255) & 0xFF;
				B = ( B - ((B-Bb)*trans)/255) & 0xFF;

				*p = IMAGE4_RGB(R,G,B);
			}
	}
}


static void
image4_clear_transparent( image_type *image )
{
int x, y;
u_int *pData;

	pData = (u_int *)image->data;

    for ( y=0; y < image->row; y++)
    for ( x=0; x < image->column; x++, pData++ )
		*pData &= 0x00FFFFFF;
}


