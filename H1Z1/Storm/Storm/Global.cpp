#include "stdafx.h"

DWORD GetModuleSize( LPSTR strModuleName )
{
	MODULEENTRY32 lpme   = { 0 } ;
	DWORD		  dwSize = 0 ;
	DWORD		  PID    = GetCurrentProcessId() ;
	BOOL		  isMod  = 0 ;
	char		  chModName[ 256 ] ;

	strcpy_s( chModName, strModuleName ) ;
	_strlwr_s( chModName ) ;

	HANDLE hSnapshotModule = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, PID ) ;
	
	if ( hSnapshotModule ) 
	{
		lpme.dwSize = sizeof( lpme ) ;
		isMod = Module32First( hSnapshotModule, &lpme ) ;
		
		while( isMod ) 
		{
			if ( strcmp( _strlwr( (char*)lpme.szExePath ), chModName ) ) 
			{
				dwSize = (DWORD)lpme.modBaseSize ;
				CloseHandle( hSnapshotModule ) ;
				return dwSize ;
			}
			isMod = Module32Next( hSnapshotModule, &lpme ) ;
		}
	}

	CloseHandle( hSnapshotModule ) ;
	return 0 ;
} 



void EnableDebugPriv( void )
{
    HANDLE           hToken ;
    LUID             SeDebugNameValue ;
    TOKEN_PRIVILEGES TokenPrivileges ;

    if( OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
    {
        if( LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &SeDebugNameValue ) )
        {
			// New State Privileges, used in AdjustTokenPrivileges().. See MSDN...
            TokenPrivileges.PrivilegeCount           = 1 ;
            TokenPrivileges.Privileges[0].Luid       = SeDebugNameValue ;
            TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED ;

            if( AdjustTokenPrivileges( hToken, FALSE, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL ) )
            {
				Log( "Adjustment Of Privileges Completed..." ) ;
                CloseHandle(hToken) ;
            }
            else
            {
                CloseHandle(hToken) ;
                Log( "AdjustTokenPrivileges() Failed!" ) ;              
            }
        }
        else
        {
            CloseHandle( hToken ) ;
            Log( "LookupPrivilegeValue() Failed!" ) ;
        }
    }
    else
    {
		DWORD error ;
		error = GetLastError() ;
		Log( "Error: %08X", error ) ;
        Log( "OpenProcessToken() Failed!" ) ;
    }
}



DWORD Get_PID_From_Process_Handle( HANDLE process_handle )
{
    PROCESS_BASIC_INFORMATION pbi = {} ; 
	ULONG ulSize ;

    LONG ( WINAPI *NtQueryInformationProcess )( HANDLE ProcessHandle, ULONG ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength ) ;
    *(FARPROC *)&NtQueryInformationProcess = GetProcAddress( GetModuleHandle("ntdll"), "NtQueryInformationProcess" ) ;

    if( NtQueryInformationProcess != NULL && NtQueryInformationProcess( process_handle, 0, &pbi, sizeof( pbi ), &ulSize ) >= 0 && ulSize == sizeof( pbi ) ) 
	{
        return pbi.UniqueProcessId ;
    }
    return 0 ;
}