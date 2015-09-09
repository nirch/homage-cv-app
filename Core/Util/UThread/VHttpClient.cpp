// CVHttpClient.cpp: implementation of the CVHttpClient class.
//
//////////////////////////////////////////////////////////////////////

#include "Uigp/igp.h"
#include <winsock2.h>

#include "VHttpClient.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Ucst/CstType.h"
#include "Ubase64/Base64.h"


#pragma comment(lib, "ws2_32.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVHttpClient::CVHttpClient( char *url, int port )
{

	strcpy( m_url, url );

	m_authorization[0] = 0;
	m_user[0] = 0;
	m_password[0] = 0;

	m_port = port;

	m_cst = cst_alloc( 10000000 );

	m_socket  = 0;
}


CVHttpClient::~CVHttpClient()
{

	if( m_cst != NULL ){
		cst_destroy( m_cst,1  );
		m_cst = NULL;
	}
}


void CVHttpClient::SetAuthorization( char *user, char *password )
{
	char	buf[256],	*au;
	int	n;

	strcpy( m_user, user );
	strcpy( m_password, password );

	sprintf( buf, "%s:%s", m_user, m_password );
	base64_encode( (u_char*)buf, strlen(buf), (u_char **)&au, &n );
	strcpy( m_authorization, au);

	delete( au );
}


int CVHttpClient::Connect()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
//		cout << "WSAStartup failed.\n";
//		system("pause");
		return( -1 );
	}

	m_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	struct hostent *host;
	host = gethostbyname( m_url );

	SOCKADDR_IN SockAddr;
	SockAddr.sin_port=htons( m_port );
	SockAddr.sin_family=AF_INET;
	SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

//	cout << "Connecting...\n";
	
	if(connect( m_socket,(SOCKADDR*)(&SockAddr),sizeof(SockAddr)) != 0){
//		cout << "Could not connect";
//		system("pause");
		return( -1 );
	}


	return( 1 );
}


int CVHttpClient::Close()
{
	closesocket( m_socket );
	WSACleanup();

	return( 1 );
}


int CVHttpClient::Get( char *command )
{
	if( Connect() < 0 )
		return( -1 );

	if( command != NULL )
		strcpy( m_command, command );

	char reqeustS[1024];

	char	au[256];// = "admin:admin";
	sprintf( au, "%s:%s", m_user, m_password );
	u_char	*au64;
	int	nAu64;
	base64_encode( (u_char*)au, strlen(au), &au64, &nAu64 );



	int n = sprintf( reqeustS, "GET /%s HTTP/1.1\r\nHost: %s\r\n", m_command, m_url );
	if(m_authorization[0] != 0 )
		n += sprintf( &reqeustS[n], "Authorization: Basic %s\r\n", m_authorization );

	n += sprintf( &reqeustS[n], "Connection: Close\r\n\r\n" );


	int ret = send( m_socket, reqeustS, strlen( reqeustS), 0 );

	return( ret );





	return( 1 );
}


int CVHttpClient::ReadHeader()
{
char	data[1000];
int	nData;
char	*p1;


	while ((nData = recv( m_socket, data, 1000, 0) ) > 0){

		if( CST_BYTE_AVAILABLE( m_cst ) < nData )
			cst_extend( m_cst, nData );

		u_char *p0 = m_cst->wp;
		cst_put_buffer( m_cst, data, nData );

		

		p1 = gpStr_token( (char *)p0, (char *)m_cst->wp, (char *)"\r\n\r\n", -1 );
		
		if( p1 != NULL )
			break;
			continue;

	}

	if( nData < 0 )
		return( -1 );

	
	p1 += 4;

	m_type = 0;
	m_boundary[0] = 0;

	char *p = (char *)gpStr_token( (char *)m_cst->rp, (char *)m_cst->wp, (char *)"boundary=", 9 );
	if( p != NULL ){

		m_type = 1;

		int i;
		m_boundary[0] = '-';
		m_boundary[1] = '-';
		for( i = 2, p = p + 9 ; *p != '\r'  && *p != '\n' ; p++, i++ )
			m_boundary[i] = *p;

		m_boundary[i] = 0;

		m_boundaryLen = i;
	}



	m_cst->rp = (u_char *)( p1 + m_boundaryLen);
	cst_rewind_data( m_cst );

	return( 1 );
}



int CVHttpClient::ReadNextPart( char **data, int *nData )
{
	char	buf[10000];
	int	nBuf;
	char	*p1;

	cst_rewind_data( m_cst );

	while ((nBuf = recv( m_socket, buf, 10000, 0) ) > 0){

		u_char *p0 = m_cst->wp;
		cst_put_buffer( m_cst, buf, nBuf );



		p1 = gpStr_token( (char *)p0-20, (char *)m_cst->wp, (char *)m_boundary, -1 );

		if( p1 != NULL )
			break;
		continue;

	}

	char *p0 = gpStr_token( (char *)m_cst->rp, (char *)m_cst->wp, (char *)"\r\n\r\n", -1 );


	m_cst->rp = (u_char *)(p0 + 4);
	*data = (char *)m_cst->rp;
	//*data = (char *)(p1 + 4);
	*nData = p1 - (char *)m_cst->rp;


	m_cst->rp = (u_char *)( p1 + m_boundaryLen);
//	cst_rewind_data( m_cst );


	return( 1 );
}



int CVHttpClient::Get( char *command, char *file )
{
	if( Connect() < 0 )
		return( -1 );

	if( command != NULL )
		strcpy( m_command, command );

	char reqeustS[1024];
	//sprintf( reqeustS, "GET /%s HTTP/1.1\r\nHost: %s:80\r\nConnection: Close\r\n\r\n", m_command, m_url );


	char	au[256] = "admin:admin";
	u_char	*au64;
	int	nAu64;
	base64_encode( (u_char*)au, strlen(au), &au64, &nAu64 );

//	sprintf( reqeustS, "GET /%s HTTP/1.1\r\nHost: %s\r\nAuthorization: Basic %s\r\nConnection: Close\r\n\r\n", m_command, m_url, au64 );


	int n = sprintf( reqeustS, "GET /%s HTTP/1.1\r\nHost: %s\r\n", m_command, m_url );
	if(m_authorization[0] != 0 )
		n += sprintf( &reqeustS[n], "Authorization: Basic %s\r\n", m_authorization );

	n += sprintf( &reqeustS[n], "Connection: Close\r\n\r\n" );


	int a = send( m_socket, reqeustS, strlen( reqeustS), 0 );

	FILE *fp;
	if( ( fp = fopen( file, "wb")) == NULL )
		return( -1 );
	n = 0;

	char buffer[10000];
	int nDataLength;



	while ((nDataLength = recv( m_socket, buffer, 10000, 0) ) > 0){  

		fwrite( buffer, 1, nDataLength, fp );
		n += nDataLength;

		fprintf( stdout, "." );

		if( n > 1000000)
			break;

		//int i = 0;
		//while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {

		//	i += 1;
		//}


	}
	fprintf( stdout, "\n" );

	fclose( fp );

	return( n );
}