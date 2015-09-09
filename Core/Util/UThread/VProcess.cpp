#include "VProcess.h"

CVPorcess::CVPorcess(void)
{
}

CVPorcess::~CVPorcess(void)
{
}




int CVPorcess::Execute( char *cmd, int wait )
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;


	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	//	si.wShowWindow = SW_SHOWDEFAULT;
	si.wShowWindow =	SW_HIDE;



	ZeroMemory( &pi, sizeof(pi) );




	// Start the child process. 
	if( !CreateProcess( NULL,   // No module name (use command line)
		cmd,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi )           // Pointer to PROCESS_INFORMATION structure
		) 
	{
		//		printf( "CreateProcess failed (%d).\n", GetLastError() );
		return(- 1 );
	}

	if( wait == 0 )
		return( 2 );

	// Wait until child process exits.
	WaitForSingleObject( pi.hProcess, INFINITE );

	// Close process and thread handles. 
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

	return( 1 );
}