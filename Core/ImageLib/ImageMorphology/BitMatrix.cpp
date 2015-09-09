//  Defines the entry point for the console application.
//

//#include <memory.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "Uigp/igp.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"

#include "BitMatrix.h"

#define BFSQ_MODULU 8191

BitMatrix::BitMatrix()
{
	pBuf = NULL;
}

int min2i(int x,int y) { 
	if(x<y) return x;
	else return y;
}
int max2i(int x,int y) { 
	if(x>y) return x;
	else return y;
}
int diff2i(int x,int y){
	if(x<y) return (y-x);
	else return (x-y);
}

BitMatrix::~BitMatrix()
{
}


int BitMatrix::init( int *scratch, int cols, int rows)
{
	pBuf=scratch;
	nRows=rows;
	nCols=cols;
	nPitch=((cols+31)&0x7fffffe0);
	nNumInts=((nPitch*nRows)>>5);

	return (nNumInts); //return size in ints rather than bytes
}


int BitMatrix::init(int cols, int rows)
{
	nRows=rows;
	nCols=cols;
	nPitch=((cols+31)&0x7fffffe0);
	nNumInts=((nPitch*nRows)>>5);

	int	n = (cols + 31 )>>5;
	pBuf = ( int *)malloc( n * nRows*sizeof(int) );

	return( nNumInts );
}


void BitMatrix::Release()
{
	free( pBuf );
}

int BitMatrix::backup[9600]; //640x480bits
	 unsigned char BitMatrix::buf[4*640*480];//640x480 for "matlabSave", "matlabLoad"

bool BitMatrix::isBWdistLinfLargerThan(int Linf)
{
	//cut anassociated rows from the main loop
	int low,high;
	int *start;
	int *end;
	int rowOr = 0;
	int nPitchInts = (nPitch>>5);
	bool flag;
	start = pBuf;
	
	for( low = 0 ; ((rowOr==0)&(low<nCols)) ; low++){
		for ( end = start+nPitchInts ; start<end ; start++)
			rowOr |= (*start);
	}
	if(rowOr==0)
		return 0;
	low--;
	rowOr = 0;
	end = pBuf+nNumInts;
	for ( high = nRows-1 ; (rowOr==0) ; high--){
		for ( start = end-nPitchInts ; start<end ; start++)
			rowOr |= (*start);
		end -= nPitchInts;
	}
	high++;

	if(high<low) //matrix is all zeros
		return (Linf<=0);

	BitMatrix valing;
	valing.init(pBuf+(low*nPitchInts),nCols,(high+1-low) );
	memcpy(backup,valing.pBuf,((valing.nNumInts)<<2));

	valing.erode(Linf);
	flag = valing.isAlive();
	memcpy(valing.pBuf,backup,((valing.nNumInts)<<2));
	return flag;
}

void BitMatrix::actOnEachCC(void op(void *obj,short *x,short *y,int n),void *ourObj,FILE *logPoints) const
{
	int bfsQ[BFSQ_MODULU+1];
	short *x = (short *)buf;
	short *y = ((short *)buf)+(nCols*nRows);

	BitMatrix nonFilled;
	int darr[4] = {-1,1,-nPitch,nPitch};
	int i,j,k,n,c32,r,c,dx,idx,msk,start,end,cur,me,reg32;
	nonFilled.init(backup,nCols,nRows);
	nonFilled.setAllTrue();

	//set filled edges to one, make sure never enter
	for( r = 0 ; r<nRows ; r++){
		nonFilled.unsetBit(0,r);
		nonFilled.unsetBit(nCols-1,r);
	}
	for ( c32 = 0 ; c32<nPitch ; c32+=32){
		nonFilled.set32bAligned(c32,0,0);
		nonFilled.set32bAligned(c32,nRows-1,0);
	}
	nonFilled &= (*this); 

	//main scan for elements 1 in this, 0 in filled.
	for ( j = 0 ; j<nNumInts ; j++){
		reg32 = (nonFilled.pBuf[j]);
		if(reg32){ //has point to get into
			c32 = j<<5;
			for ( c = 0 ; c<32 ; c++ ){
				reg32 = (nonFilled.pBuf[j]);
				cur = (c32|c);
				start = end = 0;
				bfsQ[end] = cur;
				end += ((reg32>>c)&1);
				//kill afterward
				if(end==0) continue;
				nonFilled.pBuf[j] &= ((1<<c)^0xffffffff);
				x[0] = cur%nPitch;
				y[0] = cur/nPitch;
				n = 1;
				while(start!=end)
				{
					me = bfsQ[start];
					start++;
					y[n] = (short)(me/nPitch);
					x[n] = (short)(me%nPitch);
					n++;
					start &= BFSQ_MODULU;
					for ( k = 0 ; k<4 ; k++)
					{
						idx = me+darr[k];
						dx = idx&31;
						i = idx>>5;
						msk = (1<<dx)^0xffffffff;
						bfsQ[end] = idx;
						end += (((nonFilled.pBuf[i])>>dx)&1);
						nonFilled.pBuf[i] &= msk;
						end &= BFSQ_MODULU;
					}
				}//start!=end
				op(ourObj,x,y,n);

			}//loop over case. The loop itself is 32 in resgister of potentials
		}//if register not zero - worth getting in
	}//loop over all ints in bit matrix

}//BitMatrix::actOnEachCC

bool BitMatrix::isAlive()
{
	int j,x = 0;
	for(j=0;j<nNumInts;j++)
		x |= pBuf[j];
	return (x!=0);
}


void BitMatrix::operator &=(const BitMatrix& x)
{
	for(int j = 0 ; j<nNumInts ; j++)
		pBuf[j] &= x.pBuf[j];
}

void BitMatrix::operator |=(const BitMatrix& x)
{
	for(int j = 0 ; j<nNumInts ; j++)
		pBuf[j] |= x.pBuf[j];
}

void BitMatrix::operator ^=(const BitMatrix& x)
{
	for(int j = 0 ; j<nNumInts ; j++)
		pBuf[j] ^= x.pBuf[j];
}

void BitMatrix::operator -=(const BitMatrix& x)
{
	for(int j = 0 ; j<nNumInts ; j++)
		pBuf[j] &= ((x.pBuf[j])^0xffffffff);
}

const BitMatrix& BitMatrix::operator=(const BitMatrix& x)
{
	memcpy(pBuf,x.pBuf,(nNumInts<<2));
	return x;
}

void BitMatrix::flip()
{
	for(int j = 0 ; j<nNumInts ; j++)
		pBuf[j] ^= 0xffffffff;
}

void BitMatrix::setAllFalse()
{
	for(int j = 0 ; j<nNumInts ; j++)
		pBuf[j] = 0;
}

void BitMatrix::setAllTrue()
{
	for(int j = 0 ; j<nNumInts ; j++)
		pBuf[j] = 0xffffffff;
}

void BitMatrix::whichAbove(short *data,short threshold,int pitch)
{
	int cols32 = (nCols>>5);
	int cur,cum,msk,r,j,q;
	int *ptr;
	int *start;
	int *end;

	for ( r = 0 ; r<nRows ; r++ ){
		start = pBuf+((nPitch*r)>>5);
		end = start+cols32;
		for( ptr = start ; ptr<end ; ptr++){
			cum = 0;
			msk = 1;
			for (j = 0 ; j<32 ; ++j){
				cur = threshold-(*data) ;
				++data;
				cum |= ((cur>>31)&msk);
				msk <<= 1;
			}
			*ptr = cum;
		}
		if(nCols<nPitch){ //remaind residue
			cum = 0;
			q = nPitch-nCols;
			cum = 0;
			msk = 1;
			for(j=0;j<q;++j){
				cur = threshold-(*data) ;
				++data;
				cum |= ((cur>>31)&msk);
				msk <<= 1;
			}
			*ptr = cum;
		}
		data += (pitch-nCols);
	}
}

void BitMatrix::erodeDown()
{
	int pitchInts = (nPitch>>5);
	int *self;
	int *above;
	int *last;
	int r;

	for ( r = nRows-1 ; r>0 ; r-- ) 
	{
		self = pBuf + (r*pitchInts);
		above = self-pitchInts;

		for ( last = self+pitchInts ; self<last ; ++self){
			*self &= (*above);
			++above;
		}
	}
}

void BitMatrix::erodeUp()
{
	int pitchInts = (nPitch>>5);
	int *self;
	int *below;
	int *last;
	int rowsMinus1 = nRows-1;
	int r;

	for ( r = 0 ; r<rowsMinus1 ; r++ ) 
	{
		self = pBuf + (r*pitchInts);
		below = self+pitchInts;

		for ( last = self+pitchInts ; self<last ; ++self){
			*self &= (*below);
			++below;
		}
	}
}

void BitMatrix::erodeLeft()
{
	int *curPtr;
	int *nextPtr;
	int *endPtr;
	int cur,next,r;
	int pitchInts = (nPitch>>5);

	for ( r = 0 ; r<nRows ; r++ ) 
	{
		curPtr = pBuf + (r*pitchInts);
		endPtr = curPtr + pitchInts;
		cur = *curPtr;
		for ( nextPtr = curPtr+1 ; nextPtr<endPtr ; nextPtr++ )
		{
			next = *nextPtr;
			cur &= (((cur>>1)&0x7fffffff)|(next<<31));
			*curPtr = cur;
			cur = next;
			curPtr++;
		}
		//last integer
		cur &= ((cur>>1)|0x80000000);
		*curPtr = cur;
	}
}

void BitMatrix::calculateMoments(int& N,float& averCol,float& averRow,float& uxx,float& uyy,float& uxy)
{
	N = 0;
	int Sr = 0;
	int Sc = 0;
	int Srr = 0;
	int Scc = 0;
	int Src = 0;
	int r,c,c32,idx,curInt,curMask,intsPerRow = (nPitch>>5);
	int *curRowPtr;
	int *lastRowPtr;
	int *firstRowPtr;

	for ( r = 0 ; r<nRows ; r++ ) {
		firstRowPtr = pBuf+(r*intsPerRow);
		lastRowPtr = firstRowPtr+intsPerRow;
		for ( curRowPtr=firstRowPtr ; curRowPtr<lastRowPtr ; curRowPtr++ ) 
		{
			curInt = *curRowPtr;
			if(curInt){
				c32 = (int)((curRowPtr-firstRowPtr)<<5);
				curMask = 1;
				for( idx = 0 ; idx<32 ; idx++ ) {
					if(curInt&curMask){
						c = c32|idx;
						N++;
						Sr+=r;Sc+=c;
						Srr += (r*r);
						Scc += (c*c);
						Src += (r*c);
					}
					curMask <<= 1;
				}
			}
		}
	}
	if(N==0)
		averCol = averRow = uxx = uxy = uyy = 0;
	else{
		double NN = N;
		double Norm = 1.0/NN;
		double mC = ((double)Sc)*Norm;
		double mR = ((double)Sr)*Norm;
		//uxx = mean((x-x_).^2)+1/12 = mean(x^2)-(x_^2)+1/12 = 
		uxx = (float) ( ((double)Scc)*Norm - (mC*mC) + 0.083333 ) ;
		uyy = (float) ( ((double)Srr)*Norm - (mR*mR) + 0.083333 ) ;
		//uxy = mean((x-x_)(y-y_))=mean(xy)-x_*y_
		uxy = (float) ( ((double)Src)*Norm - (mR*mC) ) ; 
		averCol = (float)mC;
		averRow = (float)mR;
	}
}

void BitMatrix::erodeRight()
{
	int *curPtr;
	int *prevPtr;
	int *firstPtr;
	int cur,prev,r;
	int pitchInts = (nPitch>>5);

	for ( r = 0 ; r<nRows ; r++ )
	{
		curPtr = pBuf + ((r*pitchInts)+pitchInts-1);//last in row
		firstPtr = pBuf + (r*pitchInts);
		cur = *curPtr;
		for ( prevPtr = curPtr-1 ; curPtr>firstPtr ; prevPtr-- ) 
		{
			prev = *prevPtr;
			cur &= ( (cur<<1)|((prev>>31)&1) ) ; 
			*curPtr = cur;
			cur = prev;
			curPtr--;
		}
		//first integer
		cur &= ((cur<<1)|1);
		*curPtr = cur;
	}
}

void BitMatrix::dilateDown()
{
	int pitchInts = (nPitch>>5);
	int *self;
	int *above;
	int *last;
	int r;

	for ( r = nRows-1 ; r>0 ; r-- ) 
	{
		self = pBuf + (r*pitchInts);
		above = self-pitchInts;

		for ( last = self+pitchInts ; self<last ; ++self){
			*self |= (*above);
			++above;
		}
	}
}

void BitMatrix::dilateUp()
{
	int pitchInts = (nPitch>>5);
	int *self;
	int *below;
	int *last;
	int rowsMinus1 = nRows-1;
	int r;

	for ( r = 0 ; r<rowsMinus1 ; r++ ) 
	{
		self = pBuf + (r*pitchInts);
		below = self+pitchInts;

		for ( last = self+pitchInts ; self<last ; ++self){
			*self |= (*below);
			++below;
		}
	}
}

void BitMatrix::dilateLeft()
{
	int *curPtr;
	int *nextPtr;
	int *endPtr;
	int cur,next,r;
	int pitchInts = (nPitch>>5);

	for ( r = 0 ; r<nRows ; r++ ) 
	{
		curPtr = pBuf + (r*pitchInts);
		endPtr = curPtr + pitchInts;
		cur = *curPtr;
		for ( nextPtr = curPtr+1 ; nextPtr<endPtr ; nextPtr++ )
		{
			next = *nextPtr;
			cur |= (((cur>>1)&0x7fffffff)|(next<<31));
			*curPtr = cur;
			cur = next;
			curPtr++;
		}
		//last integer
		cur |= ((cur>>1)&0x7fffffff);
		*curPtr = cur;
	}
}

void BitMatrix::dilateRight()
{
	int *curPtr;
	int *prevPtr;
	int *firstPtr;
	int cur,prev,r;
	int pitchInts = (nPitch>>5);

	for ( r = 0 ; r<nRows ; r++ )
	{
		curPtr = pBuf + ((r*pitchInts)+pitchInts-1);//last in row
		firstPtr = pBuf + (r*pitchInts);
		cur = *curPtr;
		for ( prevPtr = curPtr-1 ; curPtr>firstPtr ; prevPtr-- ) 
		{
			prev = *prevPtr;
			cur |= ( (cur<<1)|((prev>>31)&1) ) ; 
			*curPtr = cur;
			cur = prev;
			curPtr--;
		}
		//first integer
		cur |= (cur<<1);//or with 0 is same as no-op
		*curPtr = cur;
	}
}

void BitMatrix::dilate(int Linf)
{
	for ( int j = 0 ; j<Linf ; j++ ) {
		dilateUp();
		dilateLeft();
		dilateRight();
		dilateDown();
	}
}

void BitMatrix::erode(int Linf)
{
	for ( int j = 0 ; j<Linf ; j++ ) {
		erodeUp();
		erodeLeft();
		erodeRight();
		erodeDown();
	}
}


void BitMatrix::dilate(int Lx,int Ly)
{
	int j;
	for ( j = 0 ; j<Ly ; j++ ) {
		dilateUp();
		dilateDown();
	}
	for ( j = 0 ; j<Lx ; j++ ) {
		dilateLeft();
		dilateRight();
	}
}

void BitMatrix::erode(int Lx,int Ly)
{
	int j;
	for ( j = 0 ; j<Ly ; j++ ) {
		erodeUp();
		erodeDown();
	}
	for ( j = 0 ; j<Lx ; j++ ) {
		erodeLeft();
		erodeRight();
	}
}

void BitMatrix::whichAbove(unsigned char *data, unsigned char threshold,int pitch)
{
	int cols32 = (nCols>>5);
	int cur,cum,msk,r,j,q;
	int *ptr;
	int *start;
	int *end;

	for ( r = 0 ; r<nRows ; r++ ){
		start = pBuf+((nPitch*r)>>5);
		end = start+cols32;
		for( ptr = start ; ptr<end ; ptr++){
			cum = 0;
			msk = 1;
			for (j = 0 ; j<32 ; ++j){
				cur = threshold-(*data) ;
				++data;
				cum |= ((cur>>31)&msk);
				msk <<= 1;
			}
			*ptr = cum;
		}
		if(nCols<nPitch){ //remaind residue
			cum = 0;
			q = nPitch-nCols;
			cum = 0;
			msk = 1;
			for(j=0;j<q;++j){
				cur = threshold-(*data) ;
				++data;
				cum |= ((cur>>31)&msk);
				msk <<= 1;
			}
			*ptr = cum;
		}
		data += (pitch-nCols);
	}
}


image_type * 
BitMatrix::GetImage( image_type *im )
{
u_char	*tp;

	if( im == NULL )
		im = image_create( nRows, nCols, 1, 1, NULL );

	tp = im->data;
	static unsigned char buf[4*1024*1024];
	for(int r = 0 ; r<nRows ; r++){
		for(int c = 0 ; c<nCols ; c++)
			*tp++ = getBit(c,r)*255;
	}

	return( im );
}


void BitMatrix::Set( image_type *im, float threshold )
{
	if( im->depth == 1 ){
		whichAbove( im->data, threshold, im->width );
		return;
	}

	if( im->depth == 2 ){
		whichAbove( im->data_s, threshold, im->width );
		return;
	}
}


void BitMatrix::Write( char *file )
{
image_type	*im;

	im = GetImage();

	image_write_bmp( im, file );

}



void BitMatrix::Dump( char *name, int index )
{
	image_type	*im;

	im = GetImage();

	//image_dump( im, name, index, NULL );

	image_write_bmp( im, name );

	image_destroy( im, 1 );

}



void BitMatrix::Add( BitMatrix im, int x0, int y0 )
{
	//add's im binary picture to the current picture starting from the point (x0,y0)
	int	i,	j;

	for( i = y0 ; i < (y0 + im.nRows) ; i++ ){
		for( j = x0 ; j < (x0 + im.nCols) ; j++ ){
			 if(getBit(j,i) | im.getBit(j-x0, i-y0))
			 {
				 setBit(j,i);
			 }
		}
	}
}

void BitMatrix::Sum(int& sum)
{
	//this function is & between two binary images.
	int	i,	j;
	sum = 0;
	for( i = 0 ; i < nRows ; i++ ){
		for( j = 0 ; j < nCols ; j++ ){
			if(getBit(j,i)){
			 sum++;
			}
		}
	}
}

void BitMatrix::GravityCenter(int& x,int& y)
{
	//this function is & between two binary images.
	int	i,	j;
	int sum_i = 0;
	int sum_j = 0;
	int num = 0;
	x = -1;
	y = -1;
	for( i = 0 ; i < nRows ; i++ ){
		for( j = 0 ; j < nCols ; j++ ){
			if(getBit(j,i)){
				num++;
				sum_i += i;
				sum_j += j;
			}
		}
	}
	if(num!=0){
		x = sum_j/num;
		y = sum_i/num;
	}
}

void BitMatrix::Intersect(BitMatrix im)
{
	//this function is & between two binary images.
	int	i,	j;
	for( i = 0 ; i < im.nRows ; i++ ){
		for( j = 0 ; j < im.nCols ; j++ ){
			 if(getBit(j,i) & im.getBit(j,i) )
			 {
				 setBit(j,i);
			 }else{
				 unsetBit(j,i);
			 }
		}
	}
}


/*
void BitMatrix::Add( BitMatrix im, int x0, int y0 )
{
	//add's im binary picture to the current picture starting from the point (x0,y0)
	int cur,cum,msk;
	int *ptr, *ptr2;
	int	i,	j,	k,	n;
	u_char	*sp;

	ptr = pBuf;
	int idx = y0*nPitch+x0;
	ptr = (((pBuf[idx>>5])>>(idx&31))&1);
	ptr2 = im.pBuf;
	for( i = y0 ; i < (y0 + im.nRows) ; i++ ){
		for( j = x0 ; j < (x0 + im.nCols) ; j += 32 ){
			if( (n = x0 + im.nCols - j) > 32 )
			{
				(*ptr) = (*ptr2)&(*ptr);
			}else{
				cum = 0;
				msk = 1;
				for( k = 0 ; k < n ; k++  ){
					cum |= (*ptr2)&(*ptr)&msk;
					msk <<= 1;
				}
				*ptr = cum;
			}
			ptr++;
			ptr2++;
		}
	}
}*/


void BitMatrix::Set( image_type *im, int threshold, int x0, int y0, int x1, int y1 )
{
int cur,cum,msk;
int *ptr;
int	i,	j,	k,	n;
u_char	*sp;

	ptr = pBuf;
	for( i = y0 ; i < y1 ; i++ ){
		sp = IMAGE_PIXEL( im, i, x0 );

		for( j = x0 ; j < x1 ; j += 32 ){

			if( (n = x1 - j) > 32 )	n = 32;
			
			cum = 0;
			msk = 1;
			for( k = 0 ; k < n ; k++  ){
				cur = threshold-(*sp++) ;
				cum |= ((cur>>31)&msk);
				msk <<= 1;
			}

			*ptr++ = cum;
		}
	}

	nCols = x1 - x0;
	nRows = y1 - y0;
}


void BitMatrix::CutTheRectangle(int x0, int y0, int x1, int y1)
{
	int	i,	j;
	for( i = y0 ; i < y1 ; i++ ){
		for( j = x0 ; j < x1 ; j++ ){
			unsetBit(j,i);
		}
	}
}


void BitMatrix::Set( image_type *im )
{
//	int cur,cum,msk;
int	cum,	msk;
int *ptr;
int	i,	j,	k,	n;
u_char	*sp;

	ptr = pBuf;
	for( i = 0 ; i < im->height ; i++ ){
		sp = IMAGE_PIXEL( im, i, 0 );

		for( j = 0 ; j < im->width ; j += 32 ){

			if( (n = im->width - j) > 32 )	n = 32;

			cum = 0;
			msk = 1;
			for( k = 0 ; k < n ; k++  ){
				//cur = 0-(*sp++) ;
				//cum |= ((cur>>31)&msk);

				if( *sp++ != 0 )
					cum |= msk;
				msk <<= 1;
			}

			*ptr++ = cum;
		}
	}

	nCols = im->width;
	nRows = im->height;
}


void BitMatrix::Set( image_type *im, int val )
{
	//	int cur,cum,msk;
	int	cum,	msk;
	int *ptr;
	int	i,	j,	k,	n;
	u_char	*sp;

	ptr = pBuf;
	for( i = 0 ; i < im->height ; i++ ){
		sp = IMAGE_PIXEL( im, i, 0 );

		for( j = 0 ; j < im->width ; j += 32 ){

			if( (n = im->width - j) > 32 )	n = 32;

			cum = 0;
			msk = 1;
			for( k = 0 ; k < n ; k++  ){

				if( *sp++ == val )//!= 0 )
					cum |= msk;
				msk <<= 1;
			}

			*ptr++ = cum;
		}
	}

	nCols = im->width;
	nRows = im->height;
}

void BitMatrix::Mask( image_type *im )
{
int	cur,	msk;
int *ptr;
int	i,	j,	k,	n;
u_char	*sp;

	ptr = pBuf;
	for( i = 0 ; i < im->height ; i++ ){
		sp = IMAGE_PIXEL( im, i, 0 );

		for( j = 0 ; j < im->width ; j += 32 ){

			if( (n = im->width - j) > 32 )	n = 32;

			cur = *ptr++;
			msk = 1;
			for( k = 0 ; k < n ; k++, sp++ ){
				if( (cur & msk) == 0 )
					*sp = 0;
				msk <<= 1;
			}
		}
	}
}


void BitMatrix::Mask( image_type *im, int val )
{
	int	cur,	msk;
	int *ptr;
	int	i,	j,	k,	n;
	u_char	*sp;

	ptr = pBuf;
	for( i = 0 ; i < im->height ; i++ ){
		sp = IMAGE_PIXEL( im, i, 0 );

		for( j = 0 ; j < im->width ; j += 32 ){

			if( (n = im->width - j) > 32 )	n = 32;

			cur = *ptr++;
			msk = 1;
			for( k = 0 ; k < n ; k++, sp++ ){
				if( (cur & msk) == 0 ){
					*sp = 0;
				}
				else	if( *sp == 0 )
					*sp = val;
				msk <<= 1;
			}
		}
	}
}


void BitMatrix::UNMask( image_type *im )
{
	int	cur,	msk;
	int *ptr;
	int	i,	j,	k,	n;
	u_char	*sp;

	ptr = pBuf;
	for( i = 0 ; i < im->height ; i++ ){
		sp = IMAGE_PIXEL( im, i, 0 );

		for( j = 0 ; j < im->width ; j += 32 ){

			if( (n = im->width - j) > 32 )	n = 32;

			cur = *ptr++;
			msk = 1;
			for( k = 0 ; k < n ; k++, sp++ ){
				if( (cur & msk) != 0 )
					*sp = 255;
				msk <<= 1;
			}
		}
	}
}



void BitMatrix::Get( image_type *im, int val, int unset_val )
{
	int	cur,	msk;
	int *ptr;
	int	i,	j,	k,	n;
	u_char	*sp;

	ptr = pBuf;
	for( i = 0 ; i < im->height ; i++ ){
		sp = IMAGE_PIXEL( im, i, 0 );

		for( j = 0 ; j < im->width ; j += 32 ){

			if( (n = im->width - j) > 32 )	n = 32;

			cur = *ptr++;
			msk = 1;
			for( k = 0 ; k < n ; k++, sp++ ){
				if( (cur & msk) != 0 ){
					*sp = val;
				}
				else	{
					if( *sp == val )
						*sp = unset_val;
				}

				msk <<= 1;
			}
		}
	}
}


void BitMatrix::MaskN( image_type *im )
{
	int	cur,	msk;
	int *ptr;
	int	i,	j,	k,	n;
	u_char	*sp;

	ptr = pBuf;
	for( i = 0 ; i < im->height ; i++ ){
		sp = IMAGE_PIXEL( im, i, 0 );

		for( j = 0 ; j < im->width ; j += 32 ){

			if( (n = im->width - j) > 32 )	n = 32;

			cur = *ptr++;
			msk = 1;
			for( k = 0 ; k < n ; k++, sp++ ){
				if( (cur & msk) == 0 ){
					*sp = 0;
				}
				else	if( *sp == 0 )
							*sp = 255;
				msk <<= 1;
			}
		}
	}
}



int  BitMatrix::gravity_center_i(int palmx, int palmy,
								 float shoulderx, float shouldery,
								 int width,int length,
								 float nx_norm, float ny_norm)
{
	int ind_i = 0;
	int	array[640];
	memset(array, 0, length);

	int acomulation_sum = 0.0;
	int acomulation_sum_i = 0.0;

	float bx;
	float by;
	int px;
	int py;
	int nVal;
	for (int i = 0; i < length; i++)
	{
		bx = shoulderx - nx_norm*(i + 1);
		by = shouldery - ny_norm*(i + 1) ;

		//bx = 174.47;
		//by = 54.70;
		for (int j = -width; j <= width; j++)
		{

			px = bx + ny_norm*j + 0.5; // rounding
			py = by - nx_norm*j + 0.5;
			if(px >= 0 && py >= 0 && px < nCols && py < nRows)
			{
				nVal = getBit(px, py);
				acomulation_sum = acomulation_sum + nVal;
				acomulation_sum_i = acomulation_sum_i + nVal*(i + 1);
			}
		}
	}

	if(acomulation_sum != 0)
	{
		ind_i = acomulation_sum_i/acomulation_sum;
	}

//	delete [] array;
	return ind_i;
}



void BitMatrix::gravity_center(float &palmx,
							   float &palmy,
							   float shoulderx,
							   float shouldery,
							   int width,
							   float length,
							   float nx_norm,
							   float ny_norm,
							   float from,
							   float to)
{


	float acomulation_sum = 0.0;
	float acomulation_sum_X = 0.0;
	float acomulation_sum_Y = 0.0;

	int bx;
	int by;
	float px;
	float py;
	int nVal;
	for (int i = from; i <= to; i++)
	{
		bx = shoulderx - nx_norm*(i + 1);
		by = shouldery - ny_norm*(i + 1);
		for (int  j =- width; j<= width; j++)
		{

			px = (bx + ny_norm*j) + 0.5;
			py = (by - nx_norm*j) + 0.5;

			if(px > 0 && py > 0 && px < nCols && py < nRows)
			{
				nVal = getBit(px - 1, py - 1);
				acomulation_sum += nVal;
				acomulation_sum_X +=  nVal * px;
				acomulation_sum_Y +=  nVal * py;
			}

		}
	}

	if(acomulation_sum!=0)
	{
		palmx = acomulation_sum_X/acomulation_sum;
		palmy = acomulation_sum_Y/acomulation_sum;
	}
}

void BitMatrix::getMean2(float palmx, float palmy,
						 float shoulderx, float shouldery,
						 int width,
						 float length,
						 float nx_norm,
						 float ny_norm,
						 int *accomulation_array, int *accomulation_weighted_array)
{
	memset(accomulation_array, 0, length);
	memset(accomulation_weighted_array, 0, length);

	float bx;
	float by;
	int acomulate = 0;
	int px;
	int py;

	for(int i =0; i < length; i++)
	{
		bx = shoulderx - nx_norm*(i + 1);
		by = shouldery - ny_norm*(i + 1);
		acomulate = 0;
		for (int j = -width; j <= width; j++)
		{
			px = bx + ny_norm*j + 0.5F; // rounding
			py = by - nx_norm*j + 0.5F;
			if(px > 0 && py > 0 && px < nCols && py < nRows)
			{
				acomulate = acomulate + getBit( px - 1, py - 1);
			}
		}

		if(i == 0)
		{
			accomulation_array[0] = acomulate;
			accomulation_weighted_array[0] = acomulate * (i + 1);
		}
		else
		{
			accomulation_array[i] = accomulation_array[i-1] + acomulate; 
			accomulation_weighted_array[i]= accomulation_weighted_array[i-1] + 
				acomulate*(i + 1);
		}
	}
}

void BitMatrix::findPalm2_bendArms(float &palmx,float &palmy,
						  float shoulderX, float shoulderY,
						  vec2d *v, int width,
						  float cornerX, float cornerY,
						  float anticornerX, float anticornerY,int length)
{

	float t0 = 1000;
	if( v->x > 0 )
		t0 = ( anticornerX - shoulderX )/v->x;
	else	if( v->x < 0 )
				t0 = ( cornerX - shoulderX )/v->x;

	float t1 = 1000;
	if( v->y > 0 )
		t1 = ( anticornerY - shoulderY )/v->y;
	else	if( v->y < 0 )
		t1 = ( cornerY - shoulderY )/v->y;

	float t = MIN( t0, t1 );
	t = MIN(t,length);

	palmx = shoulderX + t * v->x;
	palmy = shoulderY + t * v->y;

	int nLength = sqrt((float) (shoulderX - palmx)*(shoulderX - palmx) + 
		(shoulderY - palmy)*(shoulderY - palmy));
	float nx = shoulderX - palmx;
	float ny = shoulderY - palmy;
	float nx_norm = nx/nLength;
	float ny_norm = ny/nLength;


	//put the internal shoulder to the center.
	int	ind_i = gravity_center_i(palmx,palmy,
		shoulderX,shoulderY,
		width,nLength,
		nx_norm,ny_norm);

	shoulderX = shoulderX - nx_norm*ind_i;
	shoulderY = shoulderY - ny_norm*ind_i;
	nLength = nLength - ind_i;


	int *accomulation_array = new int[nLength + 1];
	int *accomulation_weighted_array = new int[nLength + 1];

	getMean2(palmx,palmy,
		shoulderX,shoulderY,
		width,nLength,
		nx_norm,ny_norm,
		accomulation_array,
		accomulation_weighted_array);


	float meani;
	float delta;
	bool bIsOk = true;

	while(bIsOk)
	{
		//getting the center of gravity.
		meani = (float)accomulation_weighted_array[(int)nLength - 1] /
			accomulation_array[(int)nLength - 1];
		if(meani < nLength/2.0F)
		{
			delta = nLength/2.0F - meani;
			if(delta < 3.0F)
			{
				bIsOk = false;
			}
			else
			{
				palmx = palmx + nx_norm*delta;
				palmy = palmy + ny_norm*delta;
				nLength = nLength - delta;
			}
		}
		else
		{
			bIsOk = false;
		}
	}

	
	gravity_center(palmx,palmy,
		shoulderX,shoulderY,
		width,nLength,
		nx_norm,ny_norm, 
		MAX(1,nLength-10),nLength);


	delete [] accomulation_array;
	delete [] accomulation_weighted_array;
}

void BitMatrix::CenterOfRegion(float& point_x, float& point_y, int width){
	int X = 0;
	int Y = 0;
	int sum = 0;
	int idx,val;
	for(int c = MAX(point_x - width,0); c < MIN(point_x + width, nCols-1); c++){
		for(int r = MAX(point_y - width, 0); r < MIN(point_y + width, nRows-1); r++){
			idx = c + (r*nPitch) ;
			val= (((pBuf[idx>>5])>>(idx&31))&1);
			X += val*c;
			Y += val*r;
			sum += val;
		}
	}
	
	if (sum > 0)
	{
		point_x = X/sum;
		point_y = Y/sum;
	}
}

void BitMatrix::ilya1d_simple_new2(int *bitWidth,int *centers,float x0,float y0,int width,
								   int back_length ,int length,float nx,float ny) const
{
	int cIdx,rIdx,r,c,idx,sum,incount,val,indMean,counter;
	float curBaseX,curBaseY,rfIdx,rf,cf,r_f,c_f;
	float maxC = (float)(nCols-1);
	float maxR = (float)(nRows-1);
	bool allIn;
	indMean=0;
	counter=0;
	
	memset(bitWidth,0,length*sizeof(int));
	memset(centers,0,length*sizeof(int));

	for ( cIdx = back_length ; cIdx<length ; cIdx++ )
	{
		curBaseX = x0 + ((float)(cIdx))*nx ;
		curBaseY = y0 + ((float)(cIdx))*ny ;	

		rfIdx = -((float)width);
		rf = curBaseY-(rfIdx*nx); //try nx on top ny on bottom since r means Y !@#$%
		cf = curBaseX+(rfIdx*ny); //(ny,-nx) perpendicular (nx,ny)
		sum = 0;
		incount = 0;

		//check if both endpoint are in then simpler loop
		allIn = false;
		if((cf*(cf-maxC))<0){ //means c is in
			if((rf*(rf-maxR))<0){
			r_f = curBaseY-((-rfIdx)*nx);
			c_f = curBaseX+((-rfIdx)*ny);
			if ( ((r_f*(r_f-maxR))<0) && ((c_f*(c_f-maxC))<0) )
				allIn = true;
			}
		}
		if(allIn){
			incount = 2*width+1;
			//same loop but without fear of outing bound
			indMean =0; counter=0;
			for (rIdx = -width ; rIdx<=width ; rIdx++ )
			{
				r = ((int)rf);
				c = ((int)cf);
				idx = c + (r*nPitch) ;
				val= (((pBuf[idx>>5])>>(idx&31))&1);
				sum += val; //getBit(idx)
				if(val==1){
					counter++;
					indMean += rIdx;}
				cf += ny;
				rf -= nx;
			}
			bitWidth[cIdx - back_length] = sum;
			if(counter!=0){
				centers[cIdx - back_length] = indMean/counter;}else{centers[cIdx - back_length] = 0;}
			continue;
		}//allIn if was true
		indMean =0; counter=0;
		for (rIdx = -width ; rIdx<=width ; rIdx++ )
		{
			r = ((int)rf);
			c = ((int)cf);
			if ( ((c-nCols)&(r-nRows)&(-c)&(-r))>>31 ) {
				idx = c + (r*nPitch) ;
				val = (((pBuf[idx>>5])>>(idx&31))&1);
				sum += val; //getBit(idx)
				if(val==1){
					counter++;
					indMean += rIdx;}
				incount++;
			}//is (c,r) inside frame
			cf += ny;
			rf -= nx;
		}//inner loop short axis
		bitWidth[cIdx - back_length] = sum;
		if(counter!=0){
				centers[cIdx - back_length] = indMean/counter;}else{centers[cIdx - back_length] = 0;}
		if(incount==0) return;
	}//main loop long axis
}//ilya1d_reduction


#ifdef _AA_
void BitMatrix::buildIntegratedMatrix(BitMatrix SubMatrix, int *bitWidth, float x0, float y0, int width,
								  int length,float nx,float ny, int& integralCount, float& meanX, float& meanY,
								  std::vector< std::pair<int,int> > &raw_points) const
{
	int cIdx,rIdx,r,c,idx,sum,incount,val,indMean,counter;
	float curBaseX,curBaseY,rfIdx,rf,cf,r_f,c_f;
	float maxC = (float)(nCols-1);
	float maxR = (float)(nRows-1);
	bool allIn;
	indMean=0;
	counter=0;
	integralCount = 0;

	memset(bitWidth,0,length*sizeof(int));

	for ( cIdx = 0 ; cIdx<length ; cIdx++ )
	{
		curBaseX = x0 + ((float)(cIdx))*nx ;
		curBaseY = y0 + ((float)(cIdx))*ny ;	

		rfIdx = -((float)width);
		rf = curBaseY-(rfIdx*nx); //try nx on top ny on bottom since r means Y !@#$%
		cf = curBaseX+(rfIdx*ny); //(ny,-nx) perpendicular (nx,ny)
		sum = 0;
		incount = 0;

		//check if both endpoint are in then simpler loop
		allIn = false;
		if((cf*(cf-maxC))<0){ //means c is in
			if((rf*(rf-maxR))<0){
			r_f = curBaseY-((-rfIdx)*nx);
			c_f = curBaseX+((-rfIdx)*ny);
			if ( ((r_f*(r_f-maxR))<0) && ((c_f*(c_f-maxC))<0) )
				allIn = true;
			}
		}

		rf = curBaseY;
		cf = curBaseX;
		int blackCounting = 0;
		int blackThresh = 8;
		if(allIn){
			//same loop but without fear of outing bound
			//if we get more than 8 blacks ,one after another 
			//we stop counting.
			for (rIdx = 0 ; rIdx<=width ; rIdx++ )
			{
				r = ((int)floor(rf)); //assume: (int) - do the same like the floor function.
				c = ((int)floor(cf));
				idx = c + (r*nPitch) ;
				val= (((pBuf[idx>>5])>>(idx&31))&1);
				sum += val; //getBit(idx)
				if(val==0){
					blackCounting++;
					if(blackCounting > blackThresh){
						break;
					}
				}else{
					if(SubMatrix.getBit(c,r)!=1){
						integralCount++;
						SubMatrix.setBit(c,r);
						raw_points.push_back( std::make_pair( c, r ) );
						meanX += c;
						meanY += r;
					}
					blackCounting = 0;
				}

				//also all nearstand values.
				r = (int)(ceil(rf));
				c = (int)(ceil(cf));
				idx = c + (r*nPitch) ;
				val= (((pBuf[idx>>5])>>(idx&31))&1);
				if((val!=0) && (SubMatrix.getBit(c,r)!=1)){
					integralCount++;
					SubMatrix.setBit(c,r);
					raw_points.push_back( std::make_pair( c, r ) );
					meanX += c;
					meanY += r;
				}

				r = (int)(floor(rf));
				c = (int)(ceil(cf));
				idx = c + (r*nPitch) ;
				val= (((pBuf[idx>>5])>>(idx&31))&1);
				if((val!=0) && (SubMatrix.getBit(c,r)!=1)){
					integralCount++;
					SubMatrix.setBit(c,r);
					raw_points.push_back( std::make_pair( c, r ) );
					meanX += c;
					meanY += r;
				}

				r = (int)(ceil(rf));
				c = (int)(floor(cf));
				idx = c + (r*nPitch) ;
				val= (((pBuf[idx>>5])>>(idx&31))&1);
				if((val!=0) && (SubMatrix.getBit(c,r)!=1)){
					integralCount++;
					SubMatrix.setBit(c,r);
					raw_points.push_back( std::make_pair( c, r ) );
					meanX += c;
					meanY += r;
				}
				//end to fill the nearstand values.

				cf += ny;
				rf -= nx;
			}
			blackCounting = 0;
			rf = curBaseY;
			cf = curBaseX;
			for (rIdx = 1 ; rIdx<=width ; rIdx++ )
			{
				r = ((int)floor(rf));
				c = ((int)floor(cf));
				idx = c + (r*nPitch) ;
				val= (((pBuf[idx>>5])>>(idx&31))&1);
				sum += val; //getBit(idx)
				if(val==0){
					blackCounting++;
					if(blackCounting > blackThresh){
						break;
					}
				}else{
					if(SubMatrix.getBit(c,r)!=1){
						integralCount++;
						SubMatrix.setBit(c,r);
						raw_points.push_back( std::make_pair( c, r ) );
						meanX += c;
						meanY += r;
					}
					blackCounting = 0;
				}

				//also all nearstand values.
				r = (int)(ceil(rf));
				c = (int)(ceil(cf));
				idx = c + (r*nPitch) ;
				val= (((pBuf[idx>>5])>>(idx&31))&1);
				if((val!=0) && (SubMatrix.getBit(c,r)!=1)){
					integralCount++;
					SubMatrix.setBit(c,r);
					raw_points.push_back( std::make_pair( c, r ) );
					meanX += c;
					meanY += r;
				}

				r = (int)(floor(rf));
				c = (int)(ceil(cf));
				idx = c + (r*nPitch) ;
				val= (((pBuf[idx>>5])>>(idx&31))&1);
				if((val!=0) && (SubMatrix.getBit(c,r)!=1)){
					integralCount++;
					SubMatrix.setBit(c,r);
					raw_points.push_back( std::make_pair( c, r ) );
					meanX += c;
					meanY += r;
				}

				r = (int)(ceil(rf));
				c = (int)(floor(cf));
				idx = c + (r*nPitch) ;
				val= (((pBuf[idx>>5])>>(idx&31))&1);
				if((val!=0) && (SubMatrix.getBit(c,r)!=1)){
					integralCount++;
					SubMatrix.setBit(c,r);
					raw_points.push_back( std::make_pair( c, r ) );
					meanX += c;
					meanY += r;
				}
				//end to fill the nearstand values.

				cf -= ny;
				rf += nx;
			}
			blackCounting = 0;
			bitWidth[cIdx] = sum;
			continue;
		}//allIn if was true
		for (rIdx = 0 ; rIdx<=width ; rIdx++ )
		{
			r = ((int)floor(rf));
			c = ((int)floor(cf));
			if ( ((c-(nCols-1))&(r-(nRows-1))&(-c)&(-r))>>31 ) {
				idx = c + (r*nPitch) ;
				val = (((pBuf[idx>>5])>>(idx&31))&1);
				sum += val; //getBit(idx)
				if(val==0){
					blackCounting++;
					if(blackCounting > blackThresh){
						break;
					}
				}else{
					if(SubMatrix.getBit(c,r)!=1){
						integralCount++;
						SubMatrix.setBit(c,r);
						raw_points.push_back( std::make_pair( c, r ) );
						meanX += c;
						meanY += r;
					}
					blackCounting = 0;
				}
				
				//also all nearstand values.
				r = (int)(ceil(rf));
				c = (int)(ceil(cf));
				idx = c + (r*nPitch) ;
				val= (((pBuf[idx>>5])>>(idx&31))&1);
				if((val!=0) && (SubMatrix.getBit(c,r)!=1)){
					integralCount++;
					SubMatrix.setBit(c,r);
					raw_points.push_back( std::make_pair( c, r ) );
					meanX += c;
					meanY += r;
				}

				r = (int)(floor(rf));
				c = (int)(ceil(cf));
				idx = c + (r*nPitch) ;
				val= (((pBuf[idx>>5])>>(idx&31))&1);
				if((val!=0) && (SubMatrix.getBit(c,r)!=1)){
					integralCount++;
					SubMatrix.setBit(c,r);
					raw_points.push_back( std::make_pair( c, r ) );
					meanX += c;
					meanY += r;
				}

				r = (int)(ceil(rf));
				c = (int)(floor(cf));
				idx = c + (r*nPitch) ;
				val= (((pBuf[idx>>5])>>(idx&31))&1);
				if((val!=0) && (SubMatrix.getBit(c,r)!=1)){
					integralCount++;
					SubMatrix.setBit(c,r);
					raw_points.push_back( std::make_pair( c, r ) );
					meanX += c;
					meanY += r;
				}
				//end to fill the nearstand values.

				cf += ny;
				rf -= nx;
			}
		}
		blackCounting = 0;
		rf = curBaseY;
		cf = curBaseX;
		for (rIdx = 1 ; rIdx<=width ; rIdx++ )
		{
			r = ((int)floor(rf));
			c = ((int)floor(cf));
			if ( ((c-(nCols-1))&(r-(nRows-1))&(-c)&(-r))>>31 ) {
				idx = c + (r*nPitch) ;
				val = (((pBuf[idx>>5])>>(idx&31))&1);
				sum += val; //getBit(idx)
				if(val==0){
					blackCounting++;
					if(blackCounting > blackThresh){
						break;
					}
				}else{
					if(SubMatrix.getBit(c,r)!=1){
						integralCount++;
						SubMatrix.setBit(c,r);
						raw_points.push_back( std::make_pair( c, r ) );
						meanX += c;
						meanY += r;
					}
					blackCounting = 0;
				}

				//also all nearstand values.
				r = (int)(ceil(rf));
				c = (int)(ceil(cf));
				idx = c + (r*nPitch) ;
				val= (((pBuf[idx>>5])>>(idx&31))&1);
				if((val!=0) && (SubMatrix.getBit(c,r)!=1)){
					integralCount++;
					SubMatrix.setBit(c,r);
					raw_points.push_back( std::make_pair( c, r ) );
					meanX += c;
					meanY += r;
				}

				r = (int)(floor(rf));
				c = (int)(ceil(cf));
				idx = c + (r*nPitch) ;
				val= (((pBuf[idx>>5])>>(idx&31))&1);
				if((val!=0) && (SubMatrix.getBit(c,r)!=1)){
					integralCount++;
					SubMatrix.setBit(c,r);
					raw_points.push_back( std::make_pair( c, r ) );
					meanX += c;
					meanY += r;
				}

				r = (int)(ceil(rf));
				c = (int)(floor(cf));
				idx = c + (r*nPitch) ;
				val= (((pBuf[idx>>5])>>(idx&31))&1);
				if((val!=0) && (SubMatrix.getBit(c,r)!=1)){
					integralCount++;
					SubMatrix.setBit(c,r);
					raw_points.push_back( std::make_pair( c, r ) );
					meanX += c;
					meanY += r;
				}
				//end to fill the nearstand values.

				cf -= ny;
				rf += nx;
			}
		}
		blackCounting = 0;
		bitWidth[cIdx] = sum;
		//if(incount==0) return;
	}//main loop long axis
	meanX /= integralCount;
	meanY /= integralCount;
}//ilya1d_reduction
#endif


void BitMatrix::ilya1d_simple_new3(int *bitWidth,int *centers,float x0,float y0,int width,
								  int length,float nx,float ny) const
{
	int cIdx,rIdx,r,c,idx,sum,incount,val,indMean,counter;
	float curBaseX,curBaseY,rfIdx,rf,cf,r_f,c_f;
	float maxC = (float)(nCols-1);
	float maxR = (float)(nRows-1);
	float prev_r = 0;
	float prev_c = 0;
	bool allIn;
	indMean=0;
	counter=0;

	memset(bitWidth,0,length*sizeof(int));
	memset(centers,0,length*sizeof(int));

	for ( cIdx = 0 ; cIdx<length ; cIdx++ )
	{
		curBaseX = x0 + ((float)(cIdx))*nx ;
		curBaseY = y0 + ((float)(cIdx))*ny ;	

		rfIdx = -((float)width);
		rf = curBaseY-(rfIdx*nx); //try nx on top ny on bottom since r means Y !@#$%
		cf = curBaseX+(rfIdx*ny); //(ny,-nx) perpendicular (nx,ny)
		sum = 0;
		incount = 0;

		//check if both endpoint are in then simpler loop
		allIn = false;
		if((cf*(cf-maxC))<0){ //means c is in
			if((rf*(rf-maxR))<0){
			r_f = curBaseY-((-rfIdx)*nx);
			c_f = curBaseX+((-rfIdx)*ny);
			if ( ((r_f*(r_f-maxR))<0) && ((c_f*(c_f-maxC))<0) )
				allIn = true;
			}
		}

		rf = curBaseY;
		cf = curBaseX;
		int blackCounting = 0;
		int blackThresh = 8;
		if(allIn){
			//same loop but without fear of outing bound
			//if we get more than 8 blacks ,one after another 
			//we stop counting.
			for (rIdx = 0 ; rIdx<=width ; rIdx++ )
			{
				r = ((int)rf);
				c = ((int)cf);

				if((r==prev_r) && (prev_c==c)){
					cf += ny;
					rf -= nx;
					continue;
				}
				prev_r = r;
				prev_c = c;

				idx = c + (r*nPitch) ;
				val= (((pBuf[idx>>5])>>(idx&31))&1);
				sum += val; //getBit(idx)
				if(val==0){
					blackCounting++;
					if(blackCounting > blackThresh){
						break;
					}
				}else{
					blackCounting = 0;
				}
				cf += ny;
				rf -= nx;
			}
			blackCounting = 0;
			rf = curBaseY;
			cf = curBaseX;
			for (rIdx = 1 ; rIdx<=width ; rIdx++ )
			{
				r = ((int)rf);
				c = ((int)cf);
				if((r==prev_r) && (prev_c==c)){
					cf += ny;
					rf -= nx;
					continue;
				}
				prev_r = r;
				prev_c = c;
				idx = c + (r*nPitch) ;
				val= (((pBuf[idx>>5])>>(idx&31))&1);
				sum += val; //getBit(idx)
				if(val==0){
					blackCounting++;
					if(blackCounting > blackThresh){
						break;
					}
				}else{
					blackCounting = 0;
				}
				cf -= ny;
				rf += nx;
			}
			blackCounting = 0;
			bitWidth[cIdx] = sum;
			continue;
		}//allIn if was true
		for (rIdx = 0 ; rIdx<=width ; rIdx++ )
		{
			r = ((int)rf);
			c = ((int)cf);
			if((r==prev_r) && (prev_c==c)){
				cf += ny;
				rf -= nx;
				continue;
			}
			prev_r = r;
			prev_c = c;
			if ( ((c-nCols)&(r-nRows)&(-c)&(-r))>>31 ) {
				idx = c + (r*nPitch) ;
				val = (((pBuf[idx>>5])>>(idx&31))&1);
				sum += val; //getBit(idx)
				if(val==0){
					blackCounting++;
					if(blackCounting > blackThresh){
						break;
					}
				}else{
					blackCounting = 0;
				}
				cf += ny;
				rf -= nx;
			}
		}
		blackCounting = 0;
		rf = curBaseY;
		cf = curBaseX;
		for (rIdx = 1 ; rIdx<=width ; rIdx++ )
		{
			r = ((int)rf);
			c = ((int)cf);
			if((r==prev_r) && (prev_c==c)){
				cf += ny;
				rf -= nx;
				continue;
			}
			prev_r = r;
			prev_c = c;
			if ( ((c-nCols)&(r-nRows)&(-c)&(-r))>>31 ) {
				idx = c + (r*nPitch) ;
				val = (((pBuf[idx>>5])>>(idx&31))&1);
				sum += val; //getBit(idx)
				if(val==0){
					blackCounting++;
					if(blackCounting > blackThresh){
						break;
					}
				}else{
					blackCounting = 0;
				}
				cf -= ny;
				rf += nx;
			}
		}
		blackCounting = 0;
		bitWidth[cIdx] = sum;
		if(incount==0) return;
	}//main loop long axis
}//ilya1d_reduction



void BitMatrix::ilya1d_simple_new(int *bitWidth,int *centers,float x0,float y0,int width,
								  int length,float nx,float ny) const
{
	int cIdx,rIdx,r,c,idx,sum,incount,val,indMean,counter;
	float curBaseX,curBaseY,rfIdx,rf,cf,r_f,c_f;
	float maxC = (float)(nCols-1);
	float maxR = (float)(nRows-1);
	bool allIn;
	indMean=0;
	counter=0;
	
	memset(bitWidth,0,length*sizeof(int));
	memset(centers,0,length*sizeof(int));

	for ( cIdx = 0 ; cIdx<length ; cIdx++ )
	{
		curBaseX = x0 + ((float)(cIdx))*nx ;
		curBaseY = y0 + ((float)(cIdx))*ny ;	

		rfIdx = -((float)width);
		rf = curBaseY-(rfIdx*nx); //try nx on top ny on bottom since r means Y !@#$%
		cf = curBaseX+(rfIdx*ny); //(ny,-nx) perpendicular (nx,ny)
		sum = 0;
		incount = 0;

		//check if both endpoint are in then simpler loop
		allIn = false;
		if((cf*(cf-maxC))<0){ //means c is in
			if((rf*(rf-maxR))<0){
			r_f = curBaseY-((-rfIdx)*nx);
			c_f = curBaseX+((-rfIdx)*ny);
			if ( ((r_f*(r_f-maxR))<0) && ((c_f*(c_f-maxC))<0) )
				allIn = true;
			}
		}
		if(allIn){
			incount = 2*width+1;
			//same loop but without fear of outing bound
			indMean =0; counter=0;
			for (rIdx = -width ; rIdx<=width ; rIdx++ )
			{
				r = ((int)rf);
				c = ((int)cf);
				idx = c + (r*nPitch) ;
				val= (((pBuf[idx>>5])>>(idx&31))&1);
				sum += val; //getBit(idx)
				if(val==1){
					counter++;
					indMean += rIdx;}
				cf += ny;
				rf -= nx;
			}
			bitWidth[cIdx] = sum;
			if(counter!=0){
				centers[cIdx] = indMean/counter;}else{centers[cIdx] = 0;}
			continue;
		}//allIn if was true
		indMean =0; counter=0;
		for (rIdx = -width ; rIdx<=width ; rIdx++ )
		{
			r = ((int)rf);
			c = ((int)cf);
			if ( ((c-nCols)&(r-nRows)&(-c)&(-r))>>31 ) {
				idx = c + (r*nPitch) ;
				val = (((pBuf[idx>>5])>>(idx&31))&1);
				sum += val; //getBit(idx)
				if(val==1){
					counter++;
					indMean += rIdx;}
				incount++;
			}//is (c,r) inside frame
			cf += ny;
			rf -= nx;
		}//inner loop short axis
		bitWidth[cIdx] = sum;
		if(counter!=0){
				centers[cIdx] = indMean/counter;}else{centers[cIdx] = 0;}
		if(incount==0) return;
	}//main loop long axis
}//ilya1d_reduction


void BitMatrix::findPalm2(float &palmx,float &palmy,
						  float shoulderX, float shoulderY,
						  vec2d *v, int width,
						  float cornerX, float cornerY,
						  float anticornerX, float anticornerY)
{
	float t0 = 1000;
	if( v->x > 0 )
		t0 = ( anticornerX - shoulderX )/v->x;
	else	if( v->x < 0 )
				t0 = ( cornerX - shoulderX )/v->x;


	float t1 = 1000;
	if( v->y > 0 )
		t1 = ( anticornerY - shoulderY )/v->y;
	else	if( v->y < 0 )
		t1 = ( cornerY - shoulderY )/v->y;

	float t = MIN( t0, t1 );

	palmx = shoulderX + t * v->x;
	palmy = shoulderY + t * v->y;




	float nLength = sqrt((float) (shoulderX - palmx)*(shoulderX - palmx) + 
		(shoulderY - palmy)*(shoulderY - palmy));

	if(nLength < 2)
	{
		return;
	}

	float nx = shoulderX - palmx;
	float ny = shoulderY - palmy;
	float nx_norm = nx/nLength;
	float ny_norm = ny/nLength;

	//put the internal shoulder to the center.
	int	ind_i = gravity_center_i(palmx,palmy,
		shoulderX,shoulderY,
		width,nLength,
		nx_norm,ny_norm);

	shoulderX = shoulderX - nx_norm*ind_i;
	shoulderY = shoulderY - ny_norm*ind_i;
	nLength = nLength - ind_i;


	int accomulation_array [640];
	int accomulation_weighted_array[640];


	if(nLength < 2)
	{
		palmx = shoulderX;
		palmy = shoulderY;
		return;
	}

	getMean2(palmx,palmy,
		shoulderX,shoulderY,
		width,nLength,
		nx_norm,ny_norm,
		accomulation_array,
		accomulation_weighted_array);

	float meani;
	float delta;




	for( int i = 0 ; i < 10 ; i++ ){

		//getting the center of gravity.
		if(accomulation_array[(int)nLength - 1]!=0)
		{
			meani = (float)accomulation_weighted_array[(int)nLength - 1] /
				accomulation_array[(int)nLength - 1];


			if( meani >= 0.5*nLength )
				break;


			delta = nLength/2.0F - meani;
			if(delta < 3.0F )
				break;

			palmx = palmx + nx_norm*delta;
			palmy = palmy + ny_norm*delta;
			nLength = nLength - delta;
		}

	}

	gravity_center(palmx,palmy,
		shoulderX,shoulderY,
		width,nLength,
		nx_norm,ny_norm, 
		MAX(1,nLength-10),nLength);
}
