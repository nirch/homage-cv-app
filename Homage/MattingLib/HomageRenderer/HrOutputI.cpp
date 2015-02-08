//  Defines the entry point for the console application.
//
#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#ifdef _DEBUG
//#define _DUMP
#endif

#include "Ulog/Log.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"

#include "HrOutputI.h"




CHrOutputI::CHrOutputI()
{


}

CHrOutputI::~CHrOutputI()
{
	DeleteContents();
}





void CHrOutputI::DeleteContents()
{
}



