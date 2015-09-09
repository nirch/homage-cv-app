/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/******************
 ***	igp.h	***
 ******************/

#ifndef         _VAHTTP_CLIENT_
#define         _VAHTTP_CLIENT_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <winsock2.h>
#include <windows.h>

class CVHttpClient  
{
public:
	CVHttpClient( char *url, int port );
	virtual ~CVHttpClient();

	void SetAuthorization( char *user, char *password );

	int Connect();

	int Close();


	int Get( char *command );

	int Get( char *command, char *file );

	int ReadHeader();

	int ReadNextPart( char **data, int *nData );

private:
	
	char m_url[1024];

	int m_port;

	char	m_user[256];
	char	m_password[256];
	char	m_authorization[256];

	char m_command[1024];

	int m_type;
	int m_boundaryLen;
	char m_boundary[256];


	struct cst_type	*m_cst;

	SOCKET m_socket;
};

#endif