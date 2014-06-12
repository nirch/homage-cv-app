# -*- cmake -*-
# written by Darren Ha(darren.ha@samsung.com)

cmake_minimum_required(VERSION 2.6.2)

# replace Util with binary name. e.g. mbase or FBase
SET (this_target Util)
PROJECT(${this_target})

## section: include directory

INCLUDE_DIRECTORIES(
	.
	$(Z3RD)/jni
	$(Z3RD)/jni/win32
	$(IPPROOT)/include
	)

## section: source files
# Add your source files here (one file per line), please SORT in alphabetical order for future maintenance
SET (${this_target}_SOURCE_FILES
	./Ubase64/Base64.cpp 
	./Ubox/Box2Dump.c 
	./Ubox/Box2iTool.c 
	./Ubox/Box2Tool.c 
	./Ubst/BstFile.c 
	./Ubst/BstGet.c 
	./Ubst/BstIn.c 
	./Ubst/BstPut.c 
	./Ubst/BstTool.c 
	./Ucfg/CfgParser.c 
	./Ucfg/CfgTool.c 
	./Ucfg/CfgWrite.c 
	./Ucode/code.c 
	./Uconfig3d/Config3dIo.c 
	./Uconfig3d/Config3dTool.c 
	./Uconfig3d/Config3dTrajectory.c 
	./Ucst/CstFile.c 
	./Ucst/CstTool.c 
	./Ucst/CstXml.c 
	./UGeoApp/GappDump.c 
	./UGeoApp/GappTool.c 
	./UGeoApp/GeoApp.c 
	./UGeoApp/GeoApp1.c 
	./UGeoApp/GeoApp2.c 
	./UGeoApp/GeoAppLine.c 
	./UGeoApp/ls_appr.c 
	./Ugio/GioTool.c 
	./Uheap/HeapSTool.c 
	./Uheap/HeapTool.c 
	./Uhuffman/Huffman.c 
	./Uhuffman/HuffmanGet.c 
	./Uhuffman/HuffmanPut.c 
	./Ujet1/Jet1Tool.c 
	./Uigp/GpBuildNumber.c 
	./Uigp/GpByteOrder.c 
	./Uigp/GpDir.c 
	./Uigp/GpDump.c 
	./Uigp/GpFile.c 
	./Uigp/GpFile1.c 
	./Uigp/GpFileName.c 
	./Uigp/GpFunction.c 
	./Uigp/GpIo.c 
	./Uigp/GpMemoryLeak.c 
	./Uigp/GpMsg.c 
	./Uigp/GpRelinquish.c 
	./Uigp/GpTempFile.c 
	./Uigp/hyputA.c 
	./Uinf/inf.c 
	./Ujet2/AppJet2.c 
	./Ujet2/CuspJmap23.c 
	./Ujet2/DiagonalJet2.c 
	./Ujet2/FoldJmap23.c 
	./Ujet2/InverseJmap23.c 
	./Ujet2/Jet23Array.c 
	./Ujet2/Jet23Map.c 
	./Ujet2/Jet23Tool.c 
	./Ujet2/Jet2Array.c 
	./Ujet2/Jet2Inverse.c 
	./Ujet2/Jet2Io.c 
	./Ujet2/Jet2Map.c 
	./Ujet2/Jet2Tool.c 
	./Ujni/VimJni.cpp 
	./Ujni/VimJvm.cpp 
	./Uln/PlnEigen.cpp 
	./Uln/PlnFit.cpp 
	./Uln/PlnFitA.cpp 
	./Uln/PlnGroup.c 
	./Uln/PlnParallel.c 
	./Ulz/lz12_decode.c 
	./Ulz/lz12_encode.c 
	./Ulz/lz_decode.c 
	./Ulz/lz_encode.c 
	./Umath/base/MathLinearEquation2.c 
	./Umath/base/MathLinearEquation3.c 
	./Umath/base/MathLinerExquation9.c 
	./Umath/base/MathMatrix.c 
	./Umath/base/MathMatrixInvese.c 
	./Umath/base/MathMatrixInvese4.c 
	./Umath/base/MathMatrixInvese6.c 
	./Umath/Eigen2dTool.c 
	./Umath/Sl/SlTool.c 
	./Umat/Mat2fBase.c 
	./Umat/Mat2fBaseDct.c 
	./Umat/Mat2fBaseJet2.c 
	./Umat/Mat2fTool.c 
	./Umath/LsfTool.c 
	./Umath/LT2sType.c 
	./Umath/LT2Tool.c 
	./Umath/MathVector.c 
	./Umath/MathVectorD.c 
	./Umath/Matrix.c 
	./Umath/Matrix2Tool.c 
	./Umath/Matrix3Tool.c 
	./Umath/Matrix4Tool.c 
	./Umath/MatrixEigen.c 
	./Umath/Quaternion.c 
	./Umath/Lt2/LT2Io.c 
	./Umath/Lt2/Lt2Similarity.c 
	./Umath/Pca/PcaTool.c 
	./Umedia/MdFormat.cpp 
	./Umedia/MdFormatXml.cpp 
	./Umedia/MediaDataCo.c 
	./Umedia/MediaDataMultiPart.c 
	./Umedia/MediaDataRead.c 
	./Umedia/MediaDataTool.c 
	./Umedia/MediaFile.c 
	./Umedia/MediaFormat.c 
	./Uprm/prm.c 
	./Uprm/PrmAux.c 
	./Uprm/PrmGroup.c 
	./Upt/Pt3fIo.c 
	./Upt/Pt3fTool.c 
	./Usmil/SmilBuild.c 
	./Usmil/SmilParser.c 
	./Usmil/SmilTool.c 
	./Usmil/SmilUnion.c 
	./Usynch/usynch.c 
	./UThread/VExcute.cpp 
	./UThread/VPipeTask.cpp 
	./Utime/GpTime.c 
	./Uunity/unity.c 
	./Uunity/UnityS2.c 
	./Uunity/UnityS3.c 
	./Uunity/UnityS4.c 
	./Uvec/BitVector.c 
	./Uvec/Vec2dTool.c 
	./Uvec/Vec3dApp.c 
	./Uvec/Vec3dTool.c 
	./Uview/ViewActualBox.c 
	./Uview/ViewBasePrm.c 
	./Uview/ViewCamera.c 
	./Uview/ViewConfig.c 
	./Uview/ViewCreate.c 
	./Uview/ViewCreateNormal.c 
	./Uview/ViewDump.c 
	./Uview/ViewEulerAngle.c 
	./Uview/ViewEulerV.c 
	./Uview/ViewJet.c 
	./Uview/ViewLoad.c 
	./Uview/ViewNormal.c 
	./Uview/ViewObject.c 
	./Uview/ViewObject1.c 
	./Uview/ViewPer.c 
	./Uview/ViewPlane.c 
	./Uview/ViewPrm.c 
	./Uview/ViewRoll.c 
	./Uview/ViewSub.c 
	./Uview/ViewTool.c 
	./Uvl/TfMean.cpp 
	./Uvl/TfTool.cpp 
	./Uvl/Vl2fTool.c 
	./Uvl/Vl2f_Lt2.c 
	./Uvl/Vl3dIo.c 
	./Uvl/Vl3dStl.c 
	./Uvl/Vl3dTool.c 
	./Uxml/Ustr.c 
	./Uxml/XmlDoc.c 
	./Uxml/XmlIO.c 
	./Uxml/XmlParser.c 
	./Uxml/XmlPut.c 
	./Uxml/XmlPutCst.c 
	./Uxml/XmlStr.c 
	./Uxml/XmlType.c 
	./Uxml/XmlWrite.c 
	./UZlib/adler32.c 
	./UZlib/compress.c 
	./UZlib/crc32.c 
	./UZlib/deflate.c 
	./UZlib/inffast.c 
	./UZlib/inflate.c 
	./UZlib/inftrees.c 
	./UZlib/trees.c 
	./UZlib/uncompr.c 
	./UZlib/zip.c 
	./UZlib/zip_file.c 
	./UZlib/zip_files.c 
	./UZlib/zip_ioapi.c 
	./UZlib/zip_iowin32.c 
	./UZlib/zutil.c 
	./Ucamera/Pt2dApproximateAb.c 
	./Ucamera/Pt2dApproximateLine.c 
	./Ucamera/Pt2dApproximateLineAxB.c 
	./Ucamera/Pt2dApproximateLinePv.c 
	./Ucamera/Pt2dApproximatePolylink.c 
	./Ucamera/Pt2dApproximatePolynom.c 
	./Ucamera/Pt2dComponnet.c 
	./Ucamera/Pt2dCurvtur.c 
	./Ucamera/Pt2dGroup.c 
	./Ucamera/Pt2dGTool.c 
	./Ucamera/Pt2dIo.c 
	./Ucamera/Pt2dLine.c 
	./Ucamera/Pt2dMask.c 
	./Ucamera/Pt2dTool.c 
	./Ucamera/Pt2dTriangleFilter.c 
	./Ucamera/TriangleTool.c 
	./Ulog/Log.c 
	./UPoly/Poly1d4Tool.c 
	./UPoly/Poly2d3Tool.c 
	./UPoly/Poly2d4Tool.c 
	./Uln/CrPlnTool.c 
	./Uln/LnBox.c 
	./Uln/LnCircle.c 
	./Uln/LnFromGapp.c 
	./Uln/LnRead.c 
	./Uln/LnSplit.c 
	./Uln/LnTool.c 
	./Uln/LnTool1.c 
	./Uln/LnWrite.c 
	./Uln/PlnTool.c 
	./Uln/PlnWrite.c 
	./Ufbst/FbstGet.c 
	./Ufbst/FbstPut.c 
	./Ufbst/FbstTool.c 
	./UThread/VCriticalSection.cpp 
	./UThread/VEvent.cpp 
	./UThread/VSemaphore.cpp 
	)

## section: header files
# Add your header files here(one file per line), please SORT in alphabetical order for future maintenance!
SET(${this_target}_HEADER_FILES
	./Ubase64/Base64.h 
	./Ubox/Box2d.h 
	./Ubox/Box3d.h 
	./Ubst/BstType.h 
	./Ucfg/CfgType.h 
	./Ucode/code.h 
	./Uconfig3d/Config3d.h 
	./Ucst/CstType.h 
	./UGeoApp/GeoApp.h 
	./UGeoApp/ls_appr.h 
	./Ugio/GioType.h 
	./Uheap/HeapSType.h 
	./Uheap/HeapType.h 
	./Uhuffman/Huffman.h 
	./Uhuffman/HuffmanIn.h 
	./Ujet1/Jet1Type.h 
	./Uigp/GpBuildNumber.h 
	./Uigp/GpMemoryLeak.h 
	./Uigp/igp.h 
	./Uigp/M64Type.h 
	./Uinf/uinf.h 
	./Ujet2/Jet23Type.h 
	./Ujet2/Jet2Type.h 
	./Ujni/VimJni.h 
	./Ujni/VimJvm.h 
	./Ulz/lz.h 
	./Umath/EigenType.h 
	./Umath/Sl/pLineType.h 
	./Umat/Mat2fType.h 
	./Umath/LsfType.h 
	./Umath/LT2sType.h 
	./Umath/LT2Type.h 
	./Umath/Matrix.h 
	./Umath/Matrix2Type.h 
	./Umath/Matrix3Type.h 
	./Umath/Matrix4Type.h 
	./Umath/umath.h 
	./Umath/Pca/PcaType.h 
	./Umath/Sl/SlType.h 
	./Umedia/MdFormat.h 
	./Umedia/MdFormatIn.h 
	./Umedia/MediaDataType.h 
	./Uprm/prm.h 
	./Upt/Pt3fType.h 
	./Usmil/SmilType.h 
	./UThread/VExcute.h 
	./UThread/VPipeTask.h 
	./Utime/GpTime.h 
	./Uunity/unity.h 
	./Uvec/BitVector.h 
	./Uvec/Vec2d.h 
	./Uvec/Vec3d.h 
	./Uview/ViewType.h 
	./Uvl/TfType.h 
	./Uvl/Vl2fType.h 
	./Uvl/Vl3dType.h 
	./Uxml/Ustr.h 
	./Uxml/XmlParser.h 
	./Uxml/XmlPut.h 
	./Uxml/XmlStr.h 
	./Uxml/XmlType.h 
	./UZlib/deflate.h 
	./UZlib/inffast.h 
	./UZlib/inffixed.h 
	./UZlib/inflate.h 
	./UZlib/inftrees.h 
	./UZlib/trees.h 
	./UZlib/zconf.h 
	./UZlib/zip.h 
	./UZlib/zip_crypt.h 
	./UZlib/zip_file.h 
	./UZlib/zip_ioapi.h 
	./UZlib/zip_iowin32.h 
	./UZlib/zlib.h 
	./UZlib/zutil.h 
	./Ucamera/Pt2dGroupType.h 
	./Ucamera/Pt2dType.h 
	./Ucamera/TriangleType.h 
	./Ulog/Log.h 
	./UPoly/Poly1d4Type.h 
	./UPoly/Poly2d3Type.h 
	./UPoly/Poly2d4Type.h 
	./Uln/CrPlnType.h 
	./Uln/LnType.h 
	./Uln/PlnType.h 
	./Ufbst/FbstType.h 
	./UThread/VCriticalSection.h 
	./UThread/VEvent.h 
	./UThread/VSemaphore.h 
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
