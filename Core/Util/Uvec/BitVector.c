#include "BitVector.h"
#include <stdlib.h>

BitVector * createBitVector(unsigned short k)
{
	BitVector * p = (BitVector *)malloc(sizeof(BitVector));
	if (!p) return p;
	p->length = k;
	p->data = (unsigned long *)calloc((p->length + 31) / 32, sizeof(unsigned long));
	if (!p->data)
	{
		free(p);
		return NULL;
	}
	return p;
}

void deleteBitVector(BitVector * p)
{
	if (p->data)
		free(p->data);
	free(p);
}

void setBitVector(BitVector * pBV, int i)
{
	if ((i < 0)||(i >= pBV->length))
		return;
	pBV->data[i / 32] |= (1L << (i % 32));
}

int firstSetBitOfVector(BitVector * pBV)
{
	unsigned long dw;
	int i, j, k;

	k = -1;
	for (i = 0; i < (pBV->length + 31) / 32; i++)
	{
		if (dw = pBV->data[i])
		{
			for (j = 0; j < 32; j++)
			{
				if (dw & (1L << j))
				{
					k = i * 32 + j;
					break;
				}
			}
			break;
		}
	}
	return k;
}

int lastSetBitOfVector(BitVector * pBV)
{
	unsigned long dw;
	int i, j, k;
	k = -1;

	for (i = (pBV->length + 31) / 32 - 1; i >= 0; i--)
	{
		if (dw = pBV->data[i])
		{
			for (j = 31; j >= 0; j--)
			{
				if (dw & (1L << j))
				{
					k = i * 32 + j;
					break;
				}
			}
			break;
		}
	}
	return k;
}
