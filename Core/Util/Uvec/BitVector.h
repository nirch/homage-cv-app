#if !defined(BITVECTOR_H__INCLUDED_)
#define BITVECTOR_H__INCLUDED_

/************************* BitVector **********************************/

typedef struct _BitVector
{
	unsigned long * data;
	unsigned short length;
} BitVector;

/************************* prototypes *********************************/

BitVector * createBitVector(unsigned short k);
void deleteBitVector(BitVector * pBV);
void setBitVector(BitVector * pBV, int i);
int firstSetBitOfVector(BitVector * pBV);
int lastSetBitOfVector(BitVector * pBV);


#endif