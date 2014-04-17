
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

#ifdef _DEBUG
#define _DUMP 
#endif

#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"
#include	"ImageMark/ImageMark.h"

#include	"ImageMorphology/BitMatrix.h"


#include	"Uvec/Vec3d.h"

#include	"SkinMap.h"


void find_main_segment_bw(float* hist_in, int& bwHand){
	float hist[256];
	memcpy ( hist, hist_in, 256*sizeof(float));
	array_bluring_1d( hist, 10);
	//find the maximum and return it's coordinates.
	float maxVal = 0;
	for(int i=0;i<256;i++){
		if(hist[i]> maxVal){
			maxVal = hist[i];
			bwHand = i;
		}
	}
}


void find_hist(int** hist, image_type *yCbCr, int x0, int y0, int x1, int y1, image_type *bim, int& maskSize){
	u_char* sp;
	u_char* b;
	maskSize = 0;
	for(int i=x0;i<x1;i++){
		for(int j=y0;j<y1;j++){
			b = IMAGE_PIXEL( bim, j, i );
			if((*b)==255){//if the pixel - is part of the silhouette.
				sp = IMAGE_PIXEL( yCbCr, j, i );
				hist[sp[1]][sp[2]]++;
				maskSize++;
			}
		}
	}
}

void find_hist_bw(float* hist, image_type *yCbCr, int x0, int y0, int x1, int y1, image_type *bim){
	u_char* sp;
	u_char* b;
	int total_count = 0;
	for(int i=x0;i<x1;i++){
		for(int j=y0;j<y1;j++){
			b = IMAGE_PIXEL( bim, j, i );
			if((*b)==255){//if the pixel - is part of the silhouette.
				sp = IMAGE_PIXEL( yCbCr, j, i );
				hist[sp[0]]++;
				total_count++;
			}
		}
	}

	for(int i=0;i<256;i++){
		hist[i] = (hist[i])/total_count;
	}
}



void find_main_segment(int** hist_in, int& cbHand, int& crHand){	
	int** hist = new int*[256];
	for( int i = 0 ; i < 256 ; i++ ){
		hist[i] = new int[256];
		for(int j=0;j<256;j++){
			hist[i][j] = hist_in[i][j];
		}
	}
	array_bluring( hist, 10);
	//find the maximum and return it's coordinates.
	int maxVal = 0;
	for(int i=0;i<256;i++){
		for(int j=0;j<256;j++){
			if(hist[i][j] > maxVal){
				maxVal = hist[i][j];
				cbHand = i;
				crHand = j;
			}
		}
	}

	for( int i = 0 ; i < 256 ; i++ ){
		delete[] hist[i];
	}
	delete[] hist;

}



void find_main_segments_bw(int** segmentsSleeve, int* bwSegmentsSleeve, int segmentsNum, image_type *yCbCr, int** sleeve,
						   int** hand, int x0, int y0, int x1, int y1, image_type *bim,int* segmentsMaskSize){
							   memset(bwSegmentsSleeve,0,segmentsNum*sizeof(int));
							   memset(segmentsMaskSize,0,segmentsNum*sizeof(int));
							   u_char* b;
							   u_char* sp;
							   for(int s = 0; s < segmentsNum; s++){
								   float segment_hist[256];
								   memset(segment_hist,0,256*sizeof(float));
								   for(int i=0;i<yCbCr->width;i++){
									   for(int j=0;j<yCbCr->height;j++){
										   b = IMAGE_PIXEL( bim, j, i );
										   if((i<x1) && (i>x0) && (j<y1) && (j>y0) && (*b==255)){//if pixel inside the mask.
											   sp = IMAGE_PIXEL( yCbCr, j, i );
											   int c = sleeve[sp[1]][sp[2]] - hand[sp[1]][sp[2]];
											   if(c > 0){//if pixel more close to sleeve than to hand by the color.
												   //couse this functon ,runs only for sleeve segments
												   int min_ind = 0;
												   int min_val = 99999;
												   for(int k = 0; k < segmentsNum; k++){
													   double d_o = sqrt((double)((segmentsSleeve[k][0] - sp[1])*(segmentsSleeve[k][0] - sp[1]) + 
														   (segmentsSleeve[k][1] - sp[2])*(segmentsSleeve[k][1] - sp[2])));
													   if(d_o < min_val){
														   min_val = d_o;
														   min_ind = k;
													   }
												   }
												   if(min_ind==s){
													   segment_hist[sp[0]]++;
													   segmentsMaskSize[s]++;
												   }
												   //check that the pixels belongs to the current sleeve3 segment s.
											   }
										   }
									   }
								   }

								   find_main_segment_bw(segment_hist, bwSegmentsSleeve[s]);
							   }
}

void find_main_segments(int** hist_in, int*** segmentsInd, int valueThresh, int maxSegmentNum, int& segmentNum){	
	int** hist = new int*[256];
	for( int i = 0 ; i < 256 ; i++ ){
		hist[i] = new int[256];
		for(int j=0;j<256;j++){
			hist[i][j] = hist_in[i][j];
		}
	}

	array_bluring( hist, 7);					// WORK 1
	valueThresh = valueThresh*7*7;
	//find local maximums
	int* in = new int[256*256]; 
	for( int i = 0 ; i < 256 ; i++ ){
		for( int j = 0 ; j < 256 ; j++ ){
			in[i*256+j] = hist[i][j];
		}
	}

	BitMatrix isPick;
	isPick.init(256,256);
	isPick.setAllFalse();

	int neighboorX[8] = {-1,-1,-1,0,0,1,1,1};		// NO WORK 2
	int neighboorY[8] = {-1,0,1,-1,1,-1,0,1};
	for( int i = 1 ; i < 255 ; i++ ){
		for( int j = 1 ; j < 255 ; j++ ){
			int k;
			for(k = 0; k<8; k++){
				if(hist[i][j] < hist[i+neighboorY[k]][j+neighboorX[k]]){break;}
			}
			if(k==8 && (hist[i][j] > valueThresh)){
				isPick.setBit(j,i);
			}
		}
	}

	short* xCenter = new short[256*256];
	short* yCenter = new short[256*256];
	int* value = new int[256*256];
	memset(xCenter,0,256*256*sizeof(short));
	memset(yCenter,0,256*256*sizeof(short));
	memset(value,0,256*256*sizeof(int));

	struct findAllLocalMaximaAux ccInfo; // NO WORK 1
	ccInfo.in = in;
	ccInfo.value = value;
	ccInfo.xCenter = xCenter;
	ccInfo.yCenter = yCenter;
	ccInfo.pitch = 256;
	ccInfo.n = 0;
	ccInfo.origMask = &(isPick); //const
	isPick.actOnEachCC(calcCenterAndValue,&ccInfo,NULL);

	int maxNum = ccInfo.n;

	//this is not have to be.
	if(maxNum <= 0){
	//	assert(maxNum);
		return;
	}

	//sort the results
	int t1;
	short t2;
	for ( int s = 0 ; s<maxNum  ; s++ ) { 
		for ( int l = 0 ; l<maxNum  ; l++ ) {
			if(value[l+1]>value[l]){
				t1 = value[l+1];
				value[l+1] = value[l];
				value[l] = t1;
				t2 = xCenter[l+1];
				xCenter[l+1] = xCenter[l];
				xCenter[l] = t2;
				t2 = yCenter[l+1];
				yCenter[l+1] = yCenter[l];
				yCenter[l] = t2;
			}
		}
	}

	//create the segmentsInd array.
	int ROWS = 1;
	if(maxNum > maxSegmentNum){
		ROWS = maxSegmentNum;	
	}else{
		ROWS = maxNum;
	}
	segmentNum = ROWS;
	(*segmentsInd) = new int*[ROWS];
	for( int i = 0 ; i < ROWS ; i++ ){
		(*segmentsInd)[i] = new int[2];
		(*segmentsInd)[i][0] = yCenter[i];
		(*segmentsInd)[i][1] = xCenter[i];
	}

	isPick.Release();

	for( int i = 0 ; i < 256 ; i++ ){
		delete[] hist[i];
	}
	delete[] hist;

	delete[] xCenter;
	delete[] yCenter;
	delete[] value;
	delete[] in;
}