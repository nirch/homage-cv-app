#include "stdio.h"
#include "stdlib.h"
#include "GpBuildNumber.h"
#include "string.h"

//const char *g_szDate = __DATE__;
//const char *g_szTime = __TIME__;


void gp_encode_build_number(char *pBuild, const char *pBuildDate, const char *pBuildTime);


/*
	gets the time stamp number from global preprocessor defined strings
	__DATE__ and __TIME__
	
	example :
	212291626 - 2(2002), 12(DEC), 29, 16:26

*/
u_int
gp_buildNumber_encode( char *date_, char *time_ )
{
	unsigned long lNum;
	char buffBuild[255];
	char   *stopstring;

	gp_encode_build_number((char *)buffBuild, date_, time_ );



   lNum = strtoul( buffBuild, &stopstring, 10 );
   //lNum = atol(buffBuild);

	return lNum;
}




/*
*/
void gp_transform_month_to_num(char *pMonth)
{
	if		( strcmp( pMonth,"Jan") == 0) strcpy(pMonth,"01");
	else if ( strcmp( pMonth,"Feb") == 0) strcpy( pMonth,"02");
	else if ( strcmp( pMonth,"Mar") == 0) strcpy( pMonth,"03");
	else if ( strcmp( pMonth,"Apr") == 0) strcpy( pMonth,"04");
	else if ( strcmp( pMonth,"May") == 0) strcpy( pMonth,"05");
	else if ( strcmp( pMonth,"Jun") == 0) strcpy( pMonth,"06");
	else if ( strcmp( pMonth,"Jul") == 0) strcpy( pMonth,"07");
	else if ( strcmp( pMonth,"Aug") == 0) strcpy( pMonth,"08");
	else if ( strcmp( pMonth,"Sep") == 0) strcpy( pMonth,"09");
	else if ( strcmp( pMonth,"Oct") == 0) strcpy( pMonth,"10");
	else if ( strcmp( pMonth,"Nov") == 0) strcpy( pMonth,"11");
	else if ( strcmp( pMonth,"Dec") == 0) strcpy( pMonth,"12");
	else
	{
		//ASSERT(0);
	}
}

/*
*/
void gp_transform_num_to_month(char *pMonth)
{
	if		( strcmp( pMonth,"02") == 0) strcpy(pMonth,"Jan");
	else if ( strcmp( pMonth,"02") == 0) strcpy( pMonth,"Feb");
	else if ( strcmp( pMonth,"03") == 0) strcpy( pMonth,"Mar");
	else if ( strcmp( pMonth,"04") == 0) strcpy( pMonth,"Apr");
	else if ( strcmp( pMonth,"05") == 0) strcpy( pMonth,"May");
	else if ( strcmp( pMonth,"06") == 0) strcpy( pMonth,"Jun");
	else if ( strcmp( pMonth,"07") == 0) strcpy( pMonth,"Jul");
	else if ( strcmp( pMonth,"08") == 0) strcpy( pMonth,"Aug");
	else if ( strcmp( pMonth,"09") == 0) strcpy( pMonth,"Sep");
	else if ( strcmp( pMonth,"10") == 0) strcpy( pMonth,"Oct");
	else if ( strcmp( pMonth,"11") == 0) strcpy( pMonth,"Nov");
	else if ( strcmp( pMonth,"12") == 0) strcpy( pMonth,"Dec");
	else
	{
		//ASSERT(0);
	}
}




/*
	remove all occurences of the symbol from string
*/
int gp_string_remove_char(char *pString, char chRemove)
{
	char *pstrSource	= pString;
	char *pstrDest		= pString;
	int nCount;

	char  *pstrEnd = pString + strlen(pString);

	while (pstrSource < pstrEnd)
	{
		if (*pstrSource != chRemove)
		{
			*pstrDest = *pstrSource;
			pstrDest = pstrDest + 1;
		}
		pstrSource = pstrSource + 1;
	}
	*pstrDest = '\0';

	nCount = pstrSource - pstrDest;

	return nCount;
}


     
/*
	encode predefined 	__DATE__ and __TIME__ to Build Number String	
	example :
	212291626 - 2(2002), 12(DEC), 29, 16:26
*/       
void gp_encode_build_number(char *pBuild, const char *pBuildDate, const char *pBuildTime)
{
	char buffMin[3];
	char buffHour[3];
	char buffDay[3];
	char buffMonth[4];
	char buffYear[3];
	int iLength;



	// Time - 18:46:24
	iLength = strlen(pBuildTime);

	// hours
	strncpy(buffHour, pBuildTime , 2);
	buffHour[2] = '\0';

	//
	strncpy(buffMin, pBuildTime + 3 , 2);
	buffMin[2] = '\0';

	//
	strncpy(buffMonth, pBuildDate , 3);
	buffMonth[3] = '\0';
	gp_transform_month_to_num(buffMonth);

	//
	strncpy(buffDay, pBuildDate + 4, 2);
	if (buffDay[0] == ' ')
		buffDay[0] = '0';

	buffDay[2] = '\0';


	//
	iLength = strlen(pBuildDate);
	strncpy(buffYear, pBuildDate + ( iLength - 2 ), 2);
	buffYear[2] = '\0';

	sprintf(pBuild, "%s%s%s%s%s", buffYear, buffDay, buffMonth, buffHour, buffMin);
}



/*
	decode Build Number String	
	example :
	212291626 - 29 DEC 2002, 16:26
*/      
void
gp_buildNumber_decode( u_int lBuild, char *pBuild )
{
	char buffMin[3];
	char buffHour[3];
	char buffDay[3];
	char buffMonth[3];
	char buffYear[3];
	char *pScan;
	int iLength;
	char buffNum[20];



	// start decode from the end because at start it may be the number of the year 
	// with one or two symbols	
	sprintf(buffNum,"%ld", (long)lBuild);
	//eyal ultoa is not ANSI   _ultoa(lBuild, buffNum, 10);

	iLength = strlen(buffNum);
	pScan = buffNum + (iLength - 2);
	strcpy(buffMin, pScan);
	//
	pScan = buffNum + (iLength - 4);
	strncpy(buffHour, pScan, 2);
	buffHour[2] = '\x0';
	//
	pScan = buffNum + (iLength - 6);
	strncpy(buffMonth, pScan, 2);
	buffMonth[2] = '\x0';
	gp_transform_num_to_month(buffMonth);

	//
	pScan = buffNum + (iLength - 8);
	strncpy(buffDay, pScan, 2);
	buffDay[2] = '\x0';
	//
	pScan = buffNum;
	if ( iLength - 8 == 1)
	{
		buffYear[0] = '0';
		buffYear[1] = *pScan;
	}
	else
		strncpy(buffYear, pScan, iLength - 8);

	buffYear[2] = '\x0';

	sprintf(pBuild, "%s %s %s %s:%s", buffDay, buffMonth, buffYear, buffHour, buffMin);
}