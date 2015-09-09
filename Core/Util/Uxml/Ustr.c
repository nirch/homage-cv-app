#include <string.h>
#include "Ustr.h"
#include <stdlib.h>
#include "Uigp/igp.h"

#include "Ustr.h"

/**** *strapp ****/
/*!
 * creates a new string formed as concatenation of s1 and s2
 *
 * @param *s1 : first string
 * @param *s2 : second string
 *
 * @return char  : a new string.
 */
char *strapp( char *s1, const char *s2 )
{
	char * str;
	int n1, n2;
	if ( s1 == NULL )
	{
		str = (char*)malloc(strlen(s2) * sizeof(char));
		strcpy(str,s2);
		return str;
	}
	if ( s2 == NULL )
	{
		str = (char*)malloc(strlen(s1) * sizeof(char));
		strcpy(str,s1);
		return str;
	}
	n1 = strlen(s1);
	n2 =  strlen(s2);
	str = (char*)malloc((1 + n1 + n2)*sizeof(char));
	strcpy(str, s1);
	strcat(str, s2);
	return str;
}



/**** whiteSpcae ****/
/*!
 * The method returns true if the char is a white space
 *
 * @param ch : the char
 *
 * @return int  : 1 - it is a white space
			0 - it is not
 */
int isWhiteSpace(int ch)
{
	if ( ch == 32 || ch == 14 || ch == 13 || ch == 10 || ch == 9 )
		return 1;
	return 0;
}


int isLetter(int ch)
{
	if (( ch >= 65 && ch <= 90 ) || (ch >=97 && ch <=122))
		return 1;
	return 0;
}



int isDigit(int ch)
{
	if ( ch >= 48 && ch <= 57 )
		return 1;
	return 0;
}


int isNameChar(int ch)
{
	if ( isLetter(ch) || isDigit(ch) ||
		ch == 95 || ch == 45 || ch == 46) //a-z, A-Z, 0-9, "-", ".", "_" 
		return 1;
	return 0;
}
int isExtendedChar(int ch)
{
	return 0;
}

int isCharacter(int ch)
{
	return (isDigit(ch) | isLetter(ch));
}

int isHeaderChar(int ch)
{
	if ( ch == 63 || ch == 33) // '?', '!'
		return 1;
	return 0;
}