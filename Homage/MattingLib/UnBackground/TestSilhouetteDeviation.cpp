#include <stdio.h>
#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include "Ulog/Log.h"
#include	"UnBackground.h"


int CUnBackground::TestSilhouetteDeviation()
{
	// The Logic of this test is:
	// 1. Testing only horizontal deviation
	// 2. Using thesholds:
	//		a. Number of squars (8X8 pixal) that count as deviation
	//		b. Number of consecutive lines that count as deviation
	//		c. Number of occurances of the above the count as deviation

	// Thersholds
	int DEVIATION_SQUARES_THRESH = 5;
	int DEVIATION_LINES_THRESH = 5;
	int DEVIATION_OCCURANCE_THRESH = 1;

	int consecutiveLineDeviation = 0;
	int deviationOccurance = 0;

	sbA_type biggerContour = SelectBiggerContour(m_asB);

	// Looping over data structure that represents the homogeneous of the image (each loop is a line)
	// m_as0 - Left side of the homogeneous sequence (j0: begining of sequence; j1: end of sequence)
	// m_as1 - Right side
	// m_asB - Contour
	for (int i = 0; i < m_as0.nA; ++i)
	{
		sb_type contourLine = biggerContour.a[i];
		sb_type leftHomogeneousLine = m_as0.a[i];
		sb_type rightHomogeneousLine = m_as1.a[i];

		// Checking that there is a contour is this line - we are not interested in vertical deviation
		if (contourLine.j0 != -1 && contourLine.j1 != -1)
		{
			int leftDeviation = leftHomogeneousLine.j1 == -1 ? 0 : contourLine.j0 - leftHomogeneousLine.j1;
			int rightDeviation = rightHomogeneousLine.j0 == -1 ? 0 : rightHomogeneousLine.j0 - contourLine.j1;

			if (leftDeviation >= DEVIATION_SQUARES_THRESH || rightDeviation >= DEVIATION_SQUARES_THRESH)
			{
				++consecutiveLineDeviation;
			}
			else
			{
				consecutiveLineDeviation = 0;
			}

			if (consecutiveLineDeviation >= DEVIATION_LINES_THRESH)
			{
				++deviationOccurance;
			}
			else
			{
				deviationOccurance = 0;
			}

			if (deviationOccurance >= DEVIATION_OCCURANCE_THRESH)
			{
				fprintf( stdout, "Deviation found in line: %d\n", i );
				return 1;
			}
		}
	}

	return 0;
}

sbA_type CUnBackground::SelectBiggerContour(sbA_type contourArray[4])
{
	int numberOfLines = contourArray[0].nA;
	int contourTotalLength[4] = {0, 0, 0, 0};

	for (int i = 0; i < numberOfLines; ++i)
	{
		for (int j= 0; j < m_nM; ++j)
		{
			int length = contourArray[j].a[i].j1 - contourArray[j].a[i].j0;
			contourTotalLength[j] += length;
		}
	}

	int biggerContourIndex = 0;
	int maxLength = contourTotalLength[biggerContourIndex];
	for (int i = 1; i < m_nM; ++i)
	{
		if (contourTotalLength[i] > maxLength)
		{
			biggerContourIndex = i;
			maxLength = contourTotalLength[i];
		}
	}


	fprintf( stdout, "bigger contour index is: %d\n", biggerContourIndex );
	return contourArray[biggerContourIndex];
}