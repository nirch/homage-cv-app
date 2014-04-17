/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef __XML_STR_H__
#define __XML_STR_H__


char *	xmlStr_extarct_tag_data( char *s, char *tagName,
							char **data, int *bytes, int Falloc );


char *	xmlStr_skip_tag( char *s, char *tagName );



#endif 
