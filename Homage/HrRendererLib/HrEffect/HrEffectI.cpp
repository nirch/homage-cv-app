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

#include "HrEffectI.h"



CHrEffectI::CHrEffectI()
{
	m_id = -1;
}

CHrEffectI::~CHrEffectI()
{
	DeleteContents();
}





void CHrEffectI::DeleteContents()
{

}





