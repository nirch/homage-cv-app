# -*- cmake -*-
# written by Darren Ha(darren.ha@samsung.com)

cmake_minimum_required(VERSION 2.6.2)

# replace Image with binary name. e.g. mbase or FBase
SET (this_target Image)
PROJECT(${this_target})

## section: include directory

INCLUDE_DIRECTORIES(
  ../ImageLib
	../Util
	../../Image
	$(IPPROOT)/include
  )

## section: source files
# Add your source files here (one file per line), please SORT in alphabetical order for future maintenance
SET (${this_target}_SOURCE_FILES
	./Clustring/ImageClustringIsodata.c
	./Codecs/ImageDecoder.cpp
	./Codecs/ImageEncoder.cpp
	./Histogram/Histogram.c
	./Histogram/HistogramClustringIsoda.c
	./Histogram/HistogramOrder.c
	./Image/Image4CopyIn.c
	./Image/image4From.c
	./Image/ImageAppend.c
	./Image/ImageBand.c
	./Image/ImageCreate.c
	./Image/ImageCrop.c
	./Image/ImageDewarpLt2.c
	./Image/ImageFlipH.c
	./Image/ImageFlipV.c
	./Image/ImageInterlive.c
	./Image/ImageInterpolatePv.c
	./Image/ImageIo.c
	./Image/ImageLeak.c
	./Image/ImageLevel.c
	./Image/ImageLinearCombination.c
	./Image/ImageMask.c
	./Image/ImagePixel.c
	./Image/ImageToR.c
	./Image/ImageToY.c
	./Image1/Image1Average.c
	./Image1/Image1Bluring.c
	./Image1/Image1Convolution.c
	./Image1/Image1ConvolutionM.c
	./Image1/Image1ConvolutionSeparably.c
	./Image1/Image1CornerFilter.c
	./Image1/Image1EqualNeighbor.c
	./Image1/Image1FilterBox.c
	./Image1/Image1FilterIir.c
	./Image1/Image1FisherMatrix.c
	./Image1/Image1From.c
	./Image1/Image1Gravity.c
	./Image1/Image1Histogram.c
	./Image1/Image1Integral.c
	./Image1/Image1Interpolate.c
	./Image1/Image1Majority.c
	./Image1/Image1Mask.c
	./Image1/Image1Median.c
	./Image1/Image1NoiseEstimation.c
	./Image1/Image1Pixel.c
	./Image1/Image1Sample.c
	./Image1/Image1Sample2.c
	./Image1/Image1Ssd.c
	./Image2/Image2Color.c
	./Image2/Image2DynamicRange.c
	./Image2/Image2Edge.c
	./Image2/Image2EdgeSobol.c
	./Image2/Image2Interpolate.c
	./Image2/Image2Log.cpp
	./Image2/Image2Ssd.c
	./Image3/Image3Bilnear.c
	./Image3/Image3Bluring.c
	./Image3/Image3Color.c
	./Image3/Image3DewarpLt2.c
	./Image3/Image3Distance.c
	./Image3/Image3Extend.c
	./Image3/Image3From.c
	./Image3/Image3MapTransform.c
	./Image3/Image3Resize43.c
	./Image3/Image3Sample2_1331.c
	./Image3/Image3YCrCb.c
	./Image6/Image6Convolution.c
	./Image6/Image6From.c
	./Image6/Image6Tool.c
	./Image6/Image6YCrCb.c
	./ImageApp/ImageApp1.c
	./ImageApp/ImageApp2.c
	./ImageApp/ImageAppW.c
	./ImageC/ImageCTool.c
	./ImageF/ImageAFEigen.cpp
	./ImageF/ImageFAccumulate.c
	./ImageF/ImageFArithmetic.cpp
	./ImageF/ImageFDensity.c
	./ImageF/ImageFDynamicRange.c
	./ImageF/ImagefFilterBox.cpp
	./ImageF/ImageFHistogram.c
	./ImageF/ImageFLog.cpp
	./ImageF/ImageFMedian.c
	./ImageF/ImageFPyramid.c
	./ImageF/ImageFResizeBilinear.c
	./ImageF/ImageFYCrCb.c
	./ImageLabel/Image1Blob.c
	./ImageLabel/Image1Label.c
	./ImageLabel/Image1LabelInletFilter.c
	./ImageLabel/Image2BwLabel.c
	./ImageMark/ImageDraw.c
	./ImageMark/ImagePrint.c
	./ImageMorphology/BitMatrix.cpp
	./ImageMorphology/Image1Morphology.cpp
	./ImageResize/ImageResample.c
	./ImageResize/ImageResize.c
	./ImageResize/ImageResizeBilinear.c
	./ImageResize/ImageResizeIpp.c
	./ImageResize/imageSample.c
	./ImageResize/imageSampleColumn.c
	./ImageResize/ImageSampleDown.c
	./ImageResize/ImageSampleDownTransparent.c
	./ImageResize/ImageSampleG.c
	./ImageResize/ImageSampleNew.c
	./ImageResize/ImageSamplingF.c
	./ImageTransform/Image3KLTransform.c
	./ImageUS/ImageUSInterpolate.c
	./ImageUS/ImageUSInterpolateV.c
	./ImageUS/ImageUSTool.c
	./VegetationIndex/ImageExG.c
	./VegetationIndex/ImageNdi.c
	./VegetationIndex/ImageNdrgi.c
	./VegetationIndex/ImageNdrgiF.c
	./YUV/ImageYUVSampling.c.cpp
	BMP/BmpDecoder.cpp
	BMP/BmpEncoder.cpp
	BMP/ImageReadBmp.c
	BMP/ImageWriteBmp.c
	BMP/Makefile
	Dithering/ImageDitherAll.c
	Dithering/ImageDitherErr.c
	Dithering/ImageDitherErrExp.c
	Dithering/ImageDitherFloyd.c
	Dithering/ImageDitherOrd.c
	Dithering/ImageDitherPat.c
	Dithering/ImageEnhancement.c
	Dithering/ImageGrayEnhancement.c
	Dithering/Makefile
	Gif/GifDecoder.cpp
	Gif/GifEncoder.cpp
	Gif/ImageReadGif.c
	Gif/ImageReadGifFrame.c
	Gif/ImageWriteGif.c
	Gif/LzwEncoding.c
	Gif/Makefile
	Image/Guasian.c
	Image/Image.c
	Image/Image4to1.c
	Image/Image4Tool.c
	Image/ImageAdaptivePalette.c
	Image/ImageAdd.c
	Image/ImageAlpha.c
	Image/ImageAux.c
	Image/ImageBackgroundRemoval.c
	Image/ImageComparison.c
	Image/ImageCut.c
	Image/ImageCutUarray.c
	Image/ImageDifference.c
	Image/ImageEqual.cpp
	Image/ImageExtend.c
	Image/ImageFields.c
	Image/ImageFile.c
	Image/ImageHistogram.c
	Image/ImageHsiTune.c
	Image/ImageLogo.c
	Image/ImageNegative.c
	Image/ImagePreview.c
	Image/ImagePyramid.c
	Image/ImageRgbYCrCb.c
	Image/ImageRgbYiq.c
	Image/ImageSline.c
	Image/ImageSmartPalette.c
	Image/ImageStretch.c
	Image/ImageSubtruct.c
	Image/ImageTransparent.c
	Image/Makefile
	Image/PaletteTool.c
	Image1/Image1EdgeSobol.c
	Image1/Image1Tool.c
	Image1/Makefile
	Image2/Image2Accumulate.c
	Image2/Image2Bilateral.c
	Image2/Image2Convolution.c
	Image2/Image2Derivative.c
	Image2/Image2Pyramid.c
	Image2/Image2Tool.c
	Image24To8/Image24To8.c
	Image24To8/Image24To8DitherFloyd.c
	Image24To8/Image8to24.c
	Image24To8/Makefile
	Image3/Image3Convolution.c
	Image3/Image3DynamicRange.c
	Image3/Image3Tool.c
	Image3/Makefile
	ImageApp/ImageApp.c
	ImageDump/ImageDump.c
	ImageF/ImageFConvolution.c
	ImageF/ImageFTool.c
	ImageTransform/ImageDct.c
	ImageTransform/ImageDct88.c
	ImageTransform/ImageFFT.c
	ImageTransform/ImageFourier.c
	ImageTransform/Makefile
	Makefile
	PSD/ImageReadPSD.c
	PSD/Makefile
	PSD/PsdRead.c
	PSD/PsdTool.c
	PSD/PsdWrite.c
	YUV/Image2YUV.cpp
	YUV/ImageReadYuv.c
	YUV/Makefile
	)

## section: header files
# Add your header files here(one file per line), please SORT in alphabetical order for future maintenance!
SET(${this_target}_HEADER_FILES
	./Clustring/ImageClustring.h
	./Codecs/ImageCodec.h
	./Codecs/ImageDecoder.h
	./Histogram/Histogram.h
	./Image/ImageLeak.h
	./Image6/Image6Tool.h
	./ImageApp/ImageApp1.h
	./ImageApp/ImageApp2.h
	./ImageC/ImageCTool.h
	./ImageLabel/BwLabel.h
	./ImageLabel/ImageLabel.h
	./ImageMark/ImageMark.h
	./ImageMorphology/BitMatrix.h
	./ImageMorphology/ImageMorphology.h
	./ImageResize/ImageResize.h
	./ImageUS/ImageUSTool.h
	./VegetationIndex/VegetationIndex.h
	BMP/BmpDecoder.h
	BMP/BmpEncoder.h
	BMP/ImageBmp.h
	Dithering/ImageDithering.h
	Gif/GifDecoder.h
	Gif/GifEncoder.h
	Gif/GifIo.h
	Gif/ImageReadGif.h
	Gif/ImageWriteGif.h
	Gif/lzw.h
	Image1/Image1Tool.h
	Image2/Image2Tool.h
	Image3/Image3Tool.h
	ImageApp/ImageApp.h
	ImageDump/ImageDump.h
	ImageF/ImageFTool.h
	ImageTransform/ImageTransform.h
	ImageType/ImageTool.h
	ImageType/ImageType.h
	ImageType/PaletteType.h
	PSD/ImageReadPSD.h
	PSD/PsdType.h
	YUV/Image2YUV.h
	)

SET_SOURCE_FILES_PROPERTIES(${this_target}_HEADER_FILES
                            PROPERTIES HEADER_FILE_ONLY TRUE)
LIST(APPEND ${this_target}_SOURCE_FILES ${${this_target}_HEADER_FILES})

## section: add definitions
# 	add prefix -D. example> -DSHP
#  - DO NOT add  the following definitions(already defined in ${OSP_DEFINITIONS}:
# 	-DSHP, -DWIN32, -D_WINDOWS, -D_DEBUG, -D_USRDLL, -D_CRT_SECURE_NO_DEPRECATE
ADD_DEFINITIONS(
	)

## section: add target
ADD_LIBRARY (${this_target} STATIC ${${this_target}_SOURCE_FILES} )
#ADD_EXECUTABLE (${this_target} ${${this_target}_SOURCE_FILES} )



## section: add dependency
# dependency determines overall build order.
#ADD_DEPENDENCIES(${this_target} )

## section: set link libraries
TARGET_LINK_LIBRARIES( ${this_target}
		)
