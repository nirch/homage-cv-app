#include <string.h>
#include "BmpEncoder.h"


extern "C"
{
#include "ImageType/ImageType.h"
#include "ImageType/PaletteType.h"
#include "Uigp/igp.h"
#include "Ugio/GioType.h"

#include "ImageBmp.h"
}




CBMPEncoder::CBMPEncoder():CImageEncoder()
{

}
CBMPEncoder::~CBMPEncoder()
{

}


/*********  CBMPEncoder::Encode  **********
* @Description	:  Encodes an image to the specific path
* 
* @Param  struct image_type *im :  image to write
* @Param  const char *path :  path to write to
* @Param  int quality:  ignored 
* @Return  int  :  number of bytes written
*****************************************/
int CBMPEncoder::Encode(struct image_type *im,const char *path) 
{
char	file[256];

	m_dataSize = 0;

	strcpy( file, path );
	gp_filename_force_extension( file, ".bmp" );
	int retCode = image_write_bmp( im, file );
	
	if( retCode > 0 )
	{
		m_iErrCode = 0;
		m_dataSize = retCode;
		return m_dataSize;
	}


	switch( retCode )	
	{
	case -1:
		m_iErrCode = IMAGE_CODEC_COULD_NOT_OPEN_STREAM;
		return m_iErrCode;
	default:
		m_iErrCode = IMAGE_CODEC_INTERNAL_ERROR;
		return m_iErrCode;
	}
	
}



/*********  CBMPEncoder::Encode  **********
* @Description	: writes the image into the buffer
* 
* @Param  struct image_type* im :  image to write
* @Param  unsigned char**pTargetBuffer :  pointer to a buffer to allocate
* @Param  int quality :  ignored
* @Return  int  :  actual length of the buffer
*****************************************/
int CBMPEncoder::Encode(struct image_type* im,unsigned char**data) 
{ 
char tmpFile[256];
int	bytes;

	*data = NULL;
	gp_filename_get_temporary( "bmp", 0, ".bmp", tmpFile );
	int ret = Encode(im, tmpFile);
	if ( ret >= 0 )
		gpFile_read_to_buffer( tmpFile, (char**)data, &bytes );

	gpFile_delete(tmpFile);	
	return bytes;
}





bool CBMPEncoder::CanEncode(const char *filename)
{ 
	return true;
};

bool CBMPEncoder::CanEncode(char* data, int bytes, int bCopyData)
{
	return true;
};





