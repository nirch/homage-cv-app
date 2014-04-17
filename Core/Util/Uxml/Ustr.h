#ifndef __USTR_H__
#define __USTR_H__

char *strapp( char *strDestination, const char *strSource );
int isWhiteSpace(int ch);
int isLetter(int ch);
int isDigit(int ch);
int isCharacter(int ch);
int isExtendedChar(int ch);
int isNameChar(int ch);
int isHeaderChar(int ch);

#endif //__USTR_H__