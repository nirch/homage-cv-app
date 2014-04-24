# Microsoft Developer Studio Project File - Name="Image" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Image - Win32 DebugMultithreadedDll
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Image.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Image.mak" CFG="Image - Win32 DebugMultithreadedDll"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Image - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Image - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Image - Win32 ReleaseMultithreadedDll" (based on "Win32 (x86) Static Library")
!MESSAGE "Image - Win32 DebugMultithreadedDll" (based on "Win32 (x86) Static Library")
!MESSAGE "Image - Win32 ReleaseQA" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Gp_Vim/Core/ImageLib", ERKCAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Image - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../ImageLib" /I "../Util" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "TIF_PLATFORM_CONSOLE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Lib\Image.lib"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../ImageLib" /I "../Util" /I "./" /D "_DEBUG" /D "TIF_PLATFORM_CONSOLE" /D "WIN32" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Lib\ImageD.lib"

!ELSEIF  "$(CFG)" == "Image - Win32 ReleaseMultithreadedDll"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Image___Win32_ReleaseMultithreadedDll"
# PROP BASE Intermediate_Dir "Image___Win32_ReleaseMultithreadedDll"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Image___Win32_ReleaseMultithreadedDll"
# PROP Intermediate_Dir "Image___Win32_ReleaseMultithreadedDll"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "../ImageLib" /I "../Util" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../ImageLib" /I "../Util" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "TIF_PLATFORM_CONSOLE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Image - Win32 DebugMultithreadedDll"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Image___Win32_DebugMultithreadedDll1"
# PROP BASE Intermediate_Dir "Image___Win32_DebugMultithreadedDll1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Image___Win32_DebugMultithreadedDll1"
# PROP Intermediate_Dir "Image___Win32_DebugMultithreadedDll1"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../ImageLib" /I "../Util" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../ImageLib" /I "../Util" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "TIF_PLATFORM_CONSOLE" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Image - Win32 ReleaseQA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Image___Win32_ReleaseQA"
# PROP BASE Intermediate_Dir "Image___Win32_ReleaseQA"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseQA"
# PROP Intermediate_Dir "ReleaseQA"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "../ImageLib" /I "../Util" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../ImageLib" /I "../Util" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "TIF_PLATFORM_CONSOLE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Lib\Image.lib"

!ENDIF 

# Begin Target

# Name "Image - Win32 Release"
# Name "Image - Win32 Debug"
# Name "Image - Win32 ReleaseMultithreadedDll"
# Name "Image - Win32 DebugMultithreadedDll"
# Name "Image - Win32 ReleaseQA"
# Begin Group "Dithering"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Dithering\ImageDitherAll.c
# End Source File
# Begin Source File

SOURCE=.\Dithering\ImageDitherErr.c
# End Source File
# Begin Source File

SOURCE=.\Dithering\ImageDitherErrExp.c
# End Source File
# Begin Source File

SOURCE=.\Dithering\ImageDitherFloyd.c
# End Source File
# Begin Source File

SOURCE=.\Dithering\ImageDithering.h
# End Source File
# Begin Source File

SOURCE=.\Dithering\ImageDitherOrd.c
# End Source File
# Begin Source File

SOURCE=.\Dithering\ImageDitherPat.c
# End Source File
# Begin Source File

SOURCE=.\Dithering\ImageEnhancement.c
# End Source File
# Begin Source File

SOURCE=.\Dithering\ImageGrayEnhancement.c
# End Source File
# Begin Source File

SOURCE=.\Dithering\Makefile
# End Source File
# End Group
# Begin Group "Image"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Image\Guasian.c
# End Source File
# Begin Source File

SOURCE=.\Image\Image.c
# End Source File
# Begin Source File

SOURCE=.\Image\Image4to1.c
# End Source File
# Begin Source File

SOURCE=.\Image\Image4Tool.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageAdaptivePalette.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageAdd.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageAlpha.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageAux.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageAverage.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageBackgroundRemoval.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageComparison.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageConvolution.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageCut.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageCutUarray.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageDifference.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageEdgePixel.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageEqual.cpp
# End Source File
# Begin Source File

SOURCE=.\Image\ImageExtend.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageFields.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageFile.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageHistogram.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageHsiTune.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageLogo.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageNegative.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImagePreview.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImagePyramid.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageResample.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageResize.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageRgbYCrCb.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageRgbYiq.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageSample.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageSampleDown.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageSampleDownTransparent.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageSamplingF.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageSline.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageSmartPalette.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageStretch.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageSubtruct.c
# End Source File
# Begin Source File

SOURCE=.\Image\ImageTransparent.c
# End Source File
# Begin Source File

SOURCE=.\Image\Makefile
# End Source File
# Begin Source File

SOURCE=.\Image\PaletteTool.c
# End Source File
# End Group
# Begin Group "Image1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Image1\Image1Convolution.c
# End Source File
# Begin Source File

SOURCE=.\Image1\Image1Tool.c
# End Source File
# Begin Source File

SOURCE=.\Image1\Image1Tool.h
# End Source File
# Begin Source File

SOURCE=.\Image1\ImageInterpolate.c
# End Source File
# End Group
# Begin Group "Image3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Image3\Image3Convolution.c
# End Source File
# Begin Source File

SOURCE=.\Image3\Image3DynamicRange.c
# End Source File
# Begin Source File

SOURCE=.\Image3\Image3Tool.c
# End Source File
# Begin Source File

SOURCE=.\Image3\Image3Tool.h
# End Source File
# End Group
# Begin Group "Image6"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Image6\Image6Convolution.c
# End Source File
# Begin Source File

SOURCE=.\Image6\Image6Deblur.c
# End Source File
# Begin Source File

SOURCE=.\Image6\Image6Tool.c
# End Source File
# Begin Source File

SOURCE=.\Image6\Image6Tool.h
# End Source File
# End Group
# Begin Group "ImageF"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ImageF\ImageFConvolution.c
# End Source File
# Begin Source File

SOURCE=.\ImageF\ImageFTool.c
# End Source File
# Begin Source File

SOURCE=.\ImageF\ImageFTool.h
# End Source File
# Begin Source File

SOURCE=.\ImageF\ImageSum.c
# End Source File
# End Group
# Begin Group "ImageIo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ImageIo\ImageFileFormat.c
# End Source File
# Begin Source File

SOURCE=.\ImageIo\ImageIo.h
# End Source File
# Begin Source File

SOURCE=.\ImageIo\ImageIoTool.c
# End Source File
# Begin Source File

SOURCE=.\ImageIo\ImageRead.c
# End Source File
# Begin Source File

SOURCE=.\ImageIo\ImageWrite.c
# End Source File
# Begin Source File

SOURCE=.\ImageIo\Makefile
# End Source File
# End Group
# Begin Group "ImageType"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ImageType\ImageTool.h
# End Source File
# Begin Source File

SOURCE=.\ImageType\ImageType.h
# End Source File
# Begin Source File

SOURCE=.\ImageType\PaletteType.h
# End Source File
# End Group
# Begin Group "ImageTransform"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ImageTransform\ImageDct.c
# End Source File
# Begin Source File

SOURCE=.\ImageTransform\ImageDct88.c
# End Source File
# Begin Source File

SOURCE=.\ImageTransform\ImageFFT.c
# End Source File
# Begin Source File

SOURCE=.\ImageTransform\ImageFourier.c
# End Source File
# Begin Source File

SOURCE=.\ImageTransform\ImageTransform.h
# End Source File
# Begin Source File

SOURCE=.\ImageTransform\Makefile
# End Source File
# Begin Source File

SOURCE=.\Makefile
# End Source File
# End Group
# Begin Group "Image24To8"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Image24To8\Image24To8.c
# End Source File
# Begin Source File

SOURCE=.\Image24To8\Image24To8DitherFloyd.c
# End Source File
# Begin Source File

SOURCE=.\Image24To8\Image8to24.c
# End Source File
# Begin Source File

SOURCE=.\Image24To8\Makefile
# End Source File
# End Group
# Begin Group "Bmp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BMP\BmpDecoder.cpp
# End Source File
# Begin Source File

SOURCE=.\BMP\BmpDecoder.h
# End Source File
# Begin Source File

SOURCE=.\BMP\BmpEncoder.cpp
# End Source File
# Begin Source File

SOURCE=.\BMP\BmpEncoder.h
# End Source File
# Begin Source File

SOURCE=.\BMP\ImageBmp.h
# End Source File
# Begin Source File

SOURCE=.\BMP\ImageReadBmp.c
# End Source File
# Begin Source File

SOURCE=.\BMP\ImageWriteBmp.c
# End Source File
# Begin Source File

SOURCE=.\BMP\Makefile
# End Source File
# End Group
# Begin Group "Codecs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Codecs\ImageCodec.h
# End Source File
# Begin Source File

SOURCE=.\Codecs\ImageDecoder.cpp
# End Source File
# Begin Source File

SOURCE=.\Codecs\ImageDecoder.h
# End Source File
# Begin Source File

SOURCE=.\Codecs\ImageEncoder.cpp
# End Source File
# Begin Source File

SOURCE=.\Codecs\Makefile
# End Source File
# End Group
# Begin Group "EMS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EMS\EMSCodec.cpp
# End Source File
# Begin Source File

SOURCE=.\EMS\EMSCodec.h
# End Source File
# Begin Source File

SOURCE=.\EMS\EmsDecoder.cpp
# End Source File
# Begin Source File

SOURCE=.\EMS\EmsDecoder.h
# End Source File
# Begin Source File

SOURCE=.\EMS\ImageEms.h
# End Source File
# Begin Source File

SOURCE=.\EMS\ImageReadEms.c
# End Source File
# Begin Source File

SOURCE=.\EMS\ImageWriteEms.c
# End Source File
# Begin Source File

SOURCE=.\EMS\Makefile
# End Source File
# End Group
# Begin Group "Gif"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Gif\GifDecoder.cpp
# End Source File
# Begin Source File

SOURCE=.\Gif\GifDecoder.h
# End Source File
# Begin Source File

SOURCE=.\Gif\GIFEncoder.cpp
# End Source File
# Begin Source File

SOURCE=.\Gif\GIFEncoder.h
# End Source File
# Begin Source File

SOURCE=.\Gif\GifIo.h
# End Source File
# Begin Source File

SOURCE=.\Gif\ImageReadGif.c
# End Source File
# Begin Source File

SOURCE=.\Gif\ImageReadGif.h
# End Source File
# Begin Source File

SOURCE=.\Gif\ImageReadGifFrame.c
# End Source File
# Begin Source File

SOURCE=.\Gif\ImageWriteGif.c
# End Source File
# Begin Source File

SOURCE=.\Gif\ImageWriteGif.h
# End Source File
# Begin Source File

SOURCE=.\Gif\lzw.h
# End Source File
# Begin Source File

SOURCE=.\Gif\LzwEncoding.c
# End Source File
# Begin Source File

SOURCE=.\Gif\Makefile
# End Source File
# End Group
# Begin Group "Jpeg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Jpeg\ImageReadJpeg.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\ImageReadJpeg.h
# End Source File
# Begin Source File

SOURCE=.\Jpeg\ImageWriteJpeg.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\ImageWriteJpeg.h
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jcapimin.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jcapistd.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jccoefct.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jccolor.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jcdctmgr.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jchuff.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jchuff.h
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jcinit.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jcmainct.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jcmarker.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jcmaster.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jcomapi.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jconfig.h
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jcparam.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jcphuff.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jcprepct.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jcsample.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jctrans.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jdapimin.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jdapistd.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jdatadst.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jdatasrc.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jdcoefct.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jdcolor.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jdct.h
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jddctmgr.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jdhuff.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jdhuff.h
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jdinput.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jdmainct.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jdmarker.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jdmaster.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jdmerge.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jdphuff.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jdpostct.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jdsample.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jdtrans.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jerror.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jerror.h
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jfdctflt.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jfdctfst.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jfdctint.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jidctflt.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jidctfst.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jidctint.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jidctred.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jinclude.h
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jmemmgr.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jmemnobs.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jmemsys.h
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=.\Jpeg\JpegAPIHelp.h
# End Source File
# Begin Source File

SOURCE=.\Jpeg\JpegDecoder.cpp
# End Source File
# Begin Source File

SOURCE=.\Jpeg\JpegDecoder.h
# End Source File
# Begin Source File

SOURCE=.\Jpeg\JpegEncoder.cpp
# End Source File
# Begin Source File

SOURCE=.\Jpeg\JpegEncoder.h
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jpegint.h
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jpeglib.h
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jquant1.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jquant2.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jutils.c
# End Source File
# Begin Source File

SOURCE=.\Jpeg\jversion.h
# End Source File
# Begin Source File

SOURCE=.\Jpeg\Makefile
# End Source File
# End Group
# Begin Group "NPM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NPM\ImageNpm.h
# End Source File
# Begin Source File

SOURCE=.\NPM\ImageReadNpm.c
# End Source File
# Begin Source File

SOURCE=.\NPM\ImageWriteNpm.c
# End Source File
# Begin Source File

SOURCE=.\NPM\Makefile
# End Source File
# Begin Source File

SOURCE=.\NPM\NPMCodec.cpp
# End Source File
# Begin Source File

SOURCE=.\NPM\NPMCodec.h
# End Source File
# Begin Source File

SOURCE=.\NPM\NpmDecoder.cpp
# End Source File
# Begin Source File

SOURCE=.\NPM\NpmDecoder.h
# End Source File
# End Group
# Begin Group "Png"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Png\ImageReadPng.c
# End Source File
# Begin Source File

SOURCE=.\Png\ImageReadPng.h
# End Source File
# Begin Source File

SOURCE=.\Png\ImageWritePng.c
# End Source File
# Begin Source File

SOURCE=.\Png\ImageWritePng.h
# End Source File
# Begin Source File

SOURCE=.\Png\libpng.txt
# End Source File
# Begin Source File

SOURCE=.\Png\Makefile
# End Source File
# Begin Source File

SOURCE=.\Png\png.c
# End Source File
# Begin Source File

SOURCE=.\Png\png.h
# End Source File
# Begin Source File

SOURCE=.\Png\pngconf.h
# End Source File
# Begin Source File

SOURCE=.\Png\PngDecoder.cpp
# End Source File
# Begin Source File

SOURCE=.\Png\PngDecoder.h
# End Source File
# Begin Source File

SOURCE=.\Png\PngEncoder.cpp
# End Source File
# Begin Source File

SOURCE=.\Png\PngEncoder.h
# End Source File
# Begin Source File

SOURCE=.\Png\pngerror.c
# End Source File
# Begin Source File

SOURCE=.\Png\pnggccrd.c
# End Source File
# Begin Source File

SOURCE=.\Png\pngget.c
# End Source File
# Begin Source File

SOURCE=.\Png\pngmem.c
# End Source File
# Begin Source File

SOURCE=.\Png\pngpread.c
# End Source File
# Begin Source File

SOURCE=.\Png\pngread.c
# End Source File
# Begin Source File

SOURCE=.\Png\pngrio.c
# End Source File
# Begin Source File

SOURCE=.\Png\pngrtran.c
# End Source File
# Begin Source File

SOURCE=.\Png\pngrutil.c
# End Source File
# Begin Source File

SOURCE=.\Png\pngset.c
# End Source File
# Begin Source File

SOURCE=.\Png\pngtrans.c
# End Source File
# Begin Source File

SOURCE=.\Png\pngvcrd.c
# End Source File
# Begin Source File

SOURCE=.\Png\pngwio.c
# End Source File
# Begin Source File

SOURCE=.\Png\pngwrite.c
# End Source File
# Begin Source File

SOURCE=.\Png\pngwtran.c
# End Source File
# Begin Source File

SOURCE=.\Png\pngwutil.c
# End Source File
# End Group
# Begin Group "PSD"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PSD\ImageReadPSD.c
# End Source File
# Begin Source File

SOURCE=.\PSD\ImageReadPSD.h
# End Source File
# Begin Source File

SOURCE=.\PSD\Makefile
# End Source File
# Begin Source File

SOURCE=.\PSD\PsdRead.c
# End Source File
# Begin Source File

SOURCE=.\PSD\PsdTool.c
# End Source File
# Begin Source File

SOURCE=.\PSD\PsdType.h
# End Source File
# Begin Source File

SOURCE=.\PSD\PsdWrite.c
# End Source File
# End Group
# Begin Group "Mng"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Mng\ImageWriteMng.c
# End Source File
# Begin Source File

SOURCE=.\Mng\ImageWriteMng.h
# End Source File
# Begin Source File

SOURCE=.\Mng\Makefile
# End Source File
# Begin Source File

SOURCE=.\Mng\mng_ImageWritePng.c
# End Source File
# Begin Source File

SOURCE=.\Mng\mng_pngwrite.c
# End Source File
# Begin Source File

SOURCE=.\Mng\MngEncoder.cpp
# End Source File
# Begin Source File

SOURCE=.\Mng\MngEncoder.h
# End Source File
# Begin Source File

SOURCE=.\Mng\MngIo.h
# End Source File
# End Group
# Begin Group "TARGA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TARGA\ImageTarga.c
# End Source File
# Begin Source File

SOURCE=.\TARGA\Makefile
# End Source File
# End Group
# Begin Group "WBMP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\WBMP\ImageReadWbmp.c
# End Source File
# Begin Source File

SOURCE=.\WBMP\ImageWbmp.h
# End Source File
# Begin Source File

SOURCE=.\WBMP\ImageWriteWbmp.c
# End Source File
# Begin Source File

SOURCE=.\WBMP\Makefile
# End Source File
# Begin Source File

SOURCE=.\WBMP\WbmapDecoder.cpp
# End Source File
# Begin Source File

SOURCE=.\WBMP\WbmapDecoder.h
# End Source File
# Begin Source File

SOURCE=.\WBMP\WBMPCodec.cpp
# End Source File
# Begin Source File

SOURCE=.\WBMP\WBMPCodec.h
# End Source File
# End Group
# Begin Group "YUV"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\YUV\Image2YUV.cpp
# End Source File
# Begin Source File

SOURCE=.\YUV\Image2YUV.h
# End Source File
# Begin Source File

SOURCE=.\YUV\ImageReadYuv.c
# End Source File
# Begin Source File

SOURCE=.\YUV\Makefile
# End Source File
# End Group
# End Target
# End Project
