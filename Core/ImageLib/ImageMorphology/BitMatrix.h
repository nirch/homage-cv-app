
#ifndef BIT_MATRIX_H
#define BIT_MATRIX_H


#include "ImageType/ImageType.h"
#include <math.h>
//#include <memory.h>

#ifdef _AA_
#include <vector>
#endif

void IlyaImageSubstruction(unsigned char *bgr1,unsigned char *bgr2,int size,short *res);
	void IlyaImageSubstruction2(unsigned char *bgr1,unsigned char *bgr2,int size,short *res);
	void IlyaImageSubstructionSimple(unsigned short *res,const unsigned char *bgr1,
		const unsigned char *bgr2,int width,int height,int minX,int minY,int maxX,
		int maxY,int startXreduce = 9999,int endXreduce = 9999,int startYreduce = 9999);

class BitMatrix
{
public:

	BitMatrix();
	BitMatrix( int cols,int rows );
	~BitMatrix();

	int init(int *scratch,int cols,int rows);
	int init( int cols,int rows );
	void Release();

	bool isBWdistLinfLargerThan(int Linf);

	int width() {return nCols;}
	int height(){return nRows;}

	void operator &= (const BitMatrix&);
	void operator |= (const BitMatrix&);
	void operator ^= (const BitMatrix&);
	void operator -= (const BitMatrix&);
	const BitMatrix& operator=(const BitMatrix&);
	void flip();
	void setAllTrue();
	void setAllFalse();
	void whichAbove(short *data,short threshold,int pitch);
	void whichBelow(short *data,short threshold,int pitch);
	void whichAbove(int *data,int threshold,int pitch);
	void whichBelow(int *data,int threshold,int pitch);
	void whichAbove(unsigned char *data,unsigned char  threshold,int pitch);
	void whichBelow(unsigned char *data,unsigned char  threshold,int pitch);
	void CutTheRectangle(int x0, int y0, int x1, int y1);
	void actOnEachCC(void op(void *obj,short *x,short *y,int n),void *ourObj,FILE* log) const;
	int get32bAligned(short c,short r ) const {int idx = r*nPitch+c; return pBuf[idx>>5];}
	void set32bAligned(short c,short r,int val) const {int idx = r*nPitch+c; pBuf[idx>>5] = val;}




	void dilateDown();
	void erodeDown();
	void dilateUp();
	void erodeUp();

	void dilateRight();
	void erodeRight();
	void calculateMoments(int& N,float& averCol,float& averRow,float& uxx,float& uyy,float& uxy);
	void dilateLeft();
	void erodeLeft();

	void dilate(int Linf);
	void erode(int Linf);
	void close(int Linf){dilate(Linf);erode(Linf);}
	void open(int Linf){erode(Linf);dilate(Linf);}

	void dilate(int Lx,int Ly);
	void erode(int Lx,int Ly);
	void close(int Lx,int Ly){dilate(Lx,Ly);erode(Lx,Ly);}
	void open(int Lx,int Ly){erode(Lx,Ly);dilate(Lx,Ly);}


	
	void print();


	bool operator==(const BitMatrix&);
	bool operator!=(const BitMatrix&);



	int getBit(int c,int r) const {int idx = r*nPitch+c; return (((pBuf[idx>>5])>>(idx&31))&1); } 
	int getBit(int idx) const { return (((pBuf[idx>>5])>>(idx&31))&1); } 
	void setBit(int idx) { (pBuf[idx>>5]) |= (1<<(idx&31)); } 


	void orBit(int c,int r,int val){int idx = r*nPitch+c; pBuf[idx>>5] |= (val<<(idx&31));}
	void setBit(int c,int r){int idx = r*nPitch+c; pBuf[idx>>5] |= (1<<(idx&31));}
	void unsetBit(int c,int r){int idx = r*nPitch+c; pBuf[idx>>5] &= ((1<<(idx&31))^(-1));}
	void unsetBit(int idx) { pBuf[idx>>5] &= ((1<<(idx&31))^(-1));}
	void flipBit(int c,int r){int idx = r*nPitch+c; pBuf[idx>>5] ^= (1<<(idx&31));}
	void ilya1d_simple_new(int *bitWidth,int *counter,float x0,float y0,int width,int length,float nx,float ny) const;
	void ilya1d_simple_new3(int *bitWidth,int *counter,float x0,float y0,int width,int length,float nx,float ny) const;

#ifdef _AA_
	void buildIntegratedMatrix(BitMatrix SubMatrix, int *bitWidth, float x0, float y0, int width,
								  int length,float nx,float ny, int& integralCount, float& meanX, float& meanY,
								  std::vector< std::pair<int,int> > &raw_points) const;
#endif
	void ilya1d_simple_new2(int *bitWidth,int *counter,float x0,float y0,int width,int back_length,
		int length,float nx,float ny) const;
	void CenterOfRegion(float& point_x, float& point_y, int width);
	
	//int get32bAligned(short c,short r ) const {int idx = r*nPitch+c; return pBuf[idx>>5];}
	//void set32bAligned(short c,short r,int val) const {int idx = r*nPitch+c; pBuf[idx>>5] = val;}
	//short get16bAligned(short c,short r) const {int idx = r*nPitch+c; return ((short *)pBuf)[idx>>4];}
	//void set16bAligned(short c,short r,short val) const {int idx = r*nPitch+c; ((short *)pBuf)[idx>>4] = val;}
	//unsigned char get8bAligned(short c,short r) const {int idx = r*nPitch+c; return ((unsigned char *)pBuf)[idx>>3];}
	//void set8bAligned(short c,short r,unsigned char val) const {int idx = r*nPitch+c; ((unsigned char *)pBuf)[idx>>3] = val;}

	






	// yoram 06-01-08
	image_type * GetImage( image_type *im = NULL );

	void Set( image_type *im, float threshold );
	void Set( image_type *im );
	void Set( image_type *im, int val );

	void Get( image_type *im, int val, int unset_val );

	//void CutTheRectangle(int x0, int y0, int x1, int y1);
	void Set( image_type *im, int threshold, int x0, int y0, int x1, int y1 );
	void Add( BitMatrix im ,int x0, int y0 );
	void Intersect( BitMatrix im);
	void Sum(int& sum);
	void GravityCenter(int& x,int& y);
	void Mask( image_type *im );
	void Mask( image_type *im, int val );
	void UNMask( image_type *im );
	void MaskN( image_type *im );

	void Write( char *file );
	void Dump( char *name, int index );

	int  gravity_center_i(int palmx, int palmy,
		float shoulderx, float shouldery,
		int width,
		int length,
		float nx_norm,
		float ny_norm);

	void gravity_center(float &palmx,float &palmy,
		float shoulderx, float shouldery,
		int width,
		float length,
		float nx_norm,
		float ny_norm,
		float from,
		float to
		);

	//	[accomulation_array,accomulation_weighted_array] = 
	void 	getMean2(float palmx, float palmy,
						float shoulderx, float shouldery,
						int width,
						float length,
						float nx_norm,
						float ny_norm,
						int *accomulation_array, int *accomulation_weighted_array);


	void findPalm2(float &palmx,float &palmy,
						float shoulderX, float shoulderY,
						vec2d *v, int width,
						float cornerX, float cornerY,
						float anticornerX, float anticornerY);

	void findPalm2_bendArms(float &palmx,float &palmy,
						float shoulderX, float shoulderY,
						vec2d *v, int width,
						float cornerX, float cornerY,
						float anticornerX, float anticornerY,int length = 100);
	
	bool isAlive();

//private:
	int *pBuf;
	int nNumInts,nRows,nCols,nPitch;
	static int backup[9600]; //supports 640x480
	static unsigned char buf[4*640*480];//4MB for "matlabSave", "matlabLoad"
};
int diff2i(int x,int y);

int min2i(int x,int y);

int max2i(int x,int y);




void	BitMatrix_dump( class BitMatrix *bm, char *name, int index, char *ext );


#ifdef _DUMP
#define BITMATRIX_DUMP( bm, name, index,ext )	BitMatrix_dump( bm, name, index,ext )
#else
#define BITMATRIX_DUMP( bm, name, index,ext )
#endif



#endif




