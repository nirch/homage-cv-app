LOCAL_PATH := $(call my-dir)

UNAME := $(shell uname)
ifeq ($(UNAME),Windows_NT)
LOCAL_PATH := $(call  my-dir)
else
LOCAL_PATH := $(abspath $(call my-dir)/../Util)
endif

include $(CLEAR_VARS)




LOCAL_C_INCLUDES := $(LOCAL_PATH)/.

$(info $(LOCAL_C_INCLUDES))

LOCAL_SRC_FILES := 	./Ubox/Box2Dump.c \
	./Ucamera/Pt2dApproximateAb.c \
	./Ucamera/Pt2dApproximateLine.c \
	./Ucamera/Pt2dApproximateLineAxB.c \
	./Ucamera/Pt2dApproximateLinePv.c \
	./Ucamera/Pt2dApproximatePolylink.c \
	./Ucamera/Pt2dApproximatePolynom.c \
 	./Ucamera/Pt2dComponnet.c \
	./Ucamera/Pt2dCurvtur.c \
	./Ucamera/Pt2dDensity.c \
	./Ucamera/Pt2dGroup.c \
	./Ucamera/Pt2dGroupV.c \
	./Ucamera/Pt2dGTool.c \
	./Ucamera/Pt2dIo.c \
	./Ucamera/Pt2dLine.c \
	./Ucamera/Pt2dMask.c \
	./Ucamera/Pt2dTool.c \
	./Ucamera/Pt2dTriangleFilter.c \
	./Ucamera/TriangleTool.c \
	./UGeoApp/GappDump.c \
	./UGeoApp/GeoApp2.c \
	./UGeoApp/GeoAppLine.c \
	./Uigp/GpDump.c \
	./Uigp/GpMemoryLeak.c \
	./Uigp/hyputA.c \
	./Uln/CrPlnTool.c \
	./Uln/CrPlnToolN.c \
	./Uln/Dp/DpTool.cpp \
	./Uln/LnBox.c \
	./Uln/LnCircle.c \
	./Uln/LnFromGapp.c \
	./Uln/LnRead.c \
	./Uln/LnSplit.c \
	./Uln/LnTool.c \
	./Uln/LnTool1.c \
	./Uln/LnWrite.c \
	./Uln/PlnGroup.c \
	./Uln/PlnParallel.c \
	./Uln/PlnTool.c \
	./Uln/PlnWrite.c \
	./Ulog/Log.c \
	./Umath/base/MathLinearEquation2.c \
	./Umath/base/MathLinearEquation3.c \
	./Umath/base/MathLinerExquation9.c \
	./Umath/base/MathMatrix.c \
	./Umath/base/MathMatrixInvese.c \
	./Umath/base/MathMatrixInvese4.c \
	./Umath/base/MathMatrixInvese6.c \
	./Umath/Kmean/fKmean.cpp \
	./Umath/LsfTool.c \
	./Umath/Lt2/LT2Io.c \
	./Umath/Lt2/Lt2Similarity.c \
	./Umath/LT2sType.c \
	./Umath/MathVector.c \
	./Umath/MathVectorD.c \
	./Umath/Matrix.c \
	./Umath/Matrix2Tool.c \
	./Umath/MatrixEigen.c \
	./Umath/MatrixSvd.c \
	./Umath/Pca/PcaTool.c \
	./Umath/Quaternion.c \
	./Umedia/MediaDataMultiPart.c \
	./UPoly/Poly1d4Tool.c \
	./UPoly/Poly2d3Tool.c \
	./UPoly/Poly2d4Tool.c \
	./Utime/GpTime.c \
	./Uvec/Vec3dTool.c \
	./Uview/ViewActualBox.c \
	./Uvl/Vl2f_Lt2.c \
	./Uvl/Vl2fTool.c \
	./Uvl/Vl3dIo.c \
	./Uvl/Vl3dTool.c \
	./Uxml/XmlParser.c \
	./Uxml/XmlPut.c \
	./Uxml/XmlPutCst.c \
	./Uxml/XmlWrite.c \
	Ubase64/Base64.cpp \
	Ubox/Box2iTool.c \
	Ubox/Box2Tool.c \
	Ubst/BstFile.c \
	Ubst/BstGet.c \
	Ubst/BstIn.c \
	Ubst/BstPut.c \
	Ubst/BstTool.c \
	Ucfg/CfgParser.c \
	Ucfg/CfgTool.c \
	Ucfg/CfgWrite.c \
	Ucode/code.c \
	Uconfig3d/Config3dIo.c \
	Uconfig3d/Config3dTool.c \
	Uconfig3d/Config3dTrajectory.c \
	Ucst/CstFile.c \
	Ucst/CstTool.c \
	Ucst/CstXml.c \
	Ufbst/FbstGet.c \
	Ufbst/FbstPut.c \
	Ufbst/FbstTool.c \
	UGeoApp/GappTool.c \
	UGeoApp/GeoApp.c \
	UGeoApp/GeoApp1.c \
	UGeoApp/ls_appr.c \
	Ugio/GioTool.c \
	Uheap/HeapSTool.c \
	Uheap/HeapTool.c \
	Uhuffman/Huffman.c \
	Uhuffman/HuffmanGet.c \
	Uhuffman/HuffmanPut.c \
	Uigp/GpBuildNumber.c \
	Uigp/GpByteOrder.c \
	Uigp/GpDir.c \
	Uigp/GpFile.c \
	Uigp/GpFile1.c \
	Uigp/GpFileName.c \
	Uigp/GpFunction.c \
	Uigp/GpIo.c \
	Uigp/GpMsg.c \
	Uigp/GpRelinquish.c \
	Uigp/GpTempFile.c \
	Uinf/inf.c \
	Ujet1/Jet1Tool.c \
	Ujet2/AppJet2.c \
	Ujet2/CuspJmap23.c \
	Ujet2/DiagonalJet2.c \
	Ujet2/FoldJmap23.c \
	Ujet2/InverseJmap23.c \
	Ujet2/Jet23Array.c \
	Ujet2/Jet23Map.c \
	Ujet2/Jet23Tool.c \
	Ujet2/Jet2Array.c \
	Ujet2/Jet2Inverse.c \
	Ujet2/Jet2Io.c \
	Ujet2/Jet2Map.c \
	Ujet2/Jet2Tool.c \
	Ulz/lz_decode.c \
	Ulz/lz_encode.c \
	Ulz/lz12_decode.c \
	Ulz/lz12_encode.c \
	Umat/Mat2fBase.c \
	Umat/Mat2fBaseDct.c \
	Umat/Mat2fBaseJet2.c \
	Umat/Mat2fTool.c \
	Umath/LT2Tool.c \
	Umath/Matrix3Tool.c \
	Umath/Matrix4Tool.c \
	Umedia/MdFormat.cpp \
	Umedia/MdFormatXml.cpp \
	Umedia/MediaDataCo.c \
	Umedia/MediaDataRead.c \
	Umedia/MediaDataTool.c \
	Umedia/MediaFile.c \
	Umedia/MediaFormat.c \
	Uprm/prm.c \
	Uprm/PrmAux.c \
	Uprm/PrmGroup.c \
	Usmil/SmilBuild.c \
	Usmil/SmilParser.c \
	Usmil/SmilTool.c \
	Usmil/SmilUnion.c \
	Uunity/unity.c \
	Uunity/UnityS2.c \
	Uunity/UnityS3.c \
	Uunity/UnityS4.c \
	Uvec/BitVector.c \
	Uvec/Vec2dTool.c \
	Uview/ViewBasePrm.c \
	Uview/ViewCamera.c \
	Uview/ViewConfig.c \
	Uview/ViewCreate.c \
	Uview/ViewCreateNormal.c \
	Uview/ViewDump.c \
	Uview/ViewEulerAngle.c \
	Uview/ViewEulerV.c \
	Uview/ViewJet.c \
	Uview/ViewLoad.c \
	Uview/ViewNormal.c \
	Uview/ViewObject.c \
	Uview/ViewObject1.c \
	Uview/ViewPer.c \
	Uview/ViewPrm.c \
	Uview/ViewRoll.c \
	Uview/ViewSub.c \
	Uview/ViewTool.c \
	Uview/ViewPlane.c \
	Uxml/Ustr.c \
	Uxml/XmlDoc.c \
	Uxml/XmlIO.c \
	Uxml/XmlStr.c \
	Uxml/XmlType.c \
	UZlib/adler32.c \
	UZlib/compress.c \
	UZlib/crc32.c \
	UZlib/deflate.c \
	UZlib/inffast.c \
	UZlib/inflate.c \
	UZlib/inftrees.c \
	UZlib/trees.c \
	UZlib/uncompr.c \
	UZlib/zip.c \
	UZlib/zip_file.c \
	UZlib/zip_files.c \
	UZlib/zip_ioapi.c \
	UZlib/zutil.c				
					
					
LOCAL_CFLAGS := -w
					
					
LOCAL_MODULE := Util

#$(info "Building Static Library")
include $(BUILD_STATIC_LIBRARY)

