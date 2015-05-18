// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"


/* LPDIRECT3D9 - object is the big boss of the Direct3D library, */
/* the object that controls everything, while LPDIRECT3DDEVICE9  */
/* represents the video card                                     */

#pragma warning(disable:4996)


DWORD dwBase = NULL;
HANDLE hBase =  NULL;
DWORD dwD3DBase = NULL;	
HANDLE openHandle;

//Winsock 2 Hook
DWORD* pRecvFrom;
DWORD dw_WS2_Base;
HANDLE h_WS2_32;
HMODULE hBEClient;
DWORD dwFlag;

static DWORD VTable[4] = {0};
IDirect3DStateBlock9* pBlock = NULL;
D3DVIEWPORT9 g_Viewport;
HINSTANCE vmTest;
ofstream pFile;
PPATCH_INFO pPatchInfo; 
MEMORY_BASIC_INFORMATION test;

//Winsock 2 Hook
RECVFROM_HOOK  RF_Hook_Info;
PRECVFROM_HOOK pRF_Hook_Info;

LPDIRECT3DDEVICE9 PDevice_Present;


CArmaMain* g_pArmaMain;
DWORD g_dwArmaMainOffset;

Transformations* g_pTransformations;
DWORD g_dwTransformsOffset;

ScoreBoard* g_pScoreBoard;
DWORD g_dwScoreBoardOffset;


CTools g_Tools;

#define Naked __declspec(naked)
bool  VerifySignature( BYTE* pcSigMask, BYTE* pbSig, DWORD dSigSize, BYTE* pbFuncAddr );
void  Prolog_Present( void );
void PrologueMsg( void );
void  Epilog_Present( void );
void  EpilogueMsg( void );
void  MakePatchLegit( BYTE* newRoutine, BYTE* patchCode );
DWORD Fixup_GameOverlayStolenBytes;
DWORD Fixup_GameOverlayFunctionToRebuild;
DWORD Fixup_RebuildBytes;
DWORD Fixup_ReturnOriginal;
DWORD Fixup_GameOverlay;
DWORD Fixup_GameOverlayReturn;
void  FixUpPresent( PPATCH_INFO pInfo );
void  InsertDetour( BYTE* oldRoutine, BYTE* patchCode, DWORD patchSize, DWORD offset );
void FixUpGameOverlayHook( PPATCH_INFO pInfo );
DWORD pid_from_process_handle(HANDLE process_handle);
void EnableDebugPriv( void );
void MakePatchLegitGameOver( BYTE* newRoutine, BYTE* patchCode );
void GameOverlay_Detour( void );
void GameOverlay_Detour2( void );
void MakePatchLegitGameOver2( BYTE* newRoutine, BYTE* patchCode );
DWORD ReadAddr( void* pdwPointer );
ScoreBoard* GetScoreBoard( ScoreBoard* sbGlobal );
D3DXVECTOR3 W2SN( D3DXVECTOR3 in );
void Prologue_RecvFrom( void );
DWORD Fixup_RecvFrom;
void FixUpRecvFrom( PRECVFROM_HOOK pObject );
int iFrameCount;
DWORD dwOld_RecvFromProtect;







D3DXVECTOR3 testCords;
map<int, D3DXVECTOR3> mapCoords;
map<int, char*> mapEntityName;
int EntitySize ;


//================================================================================================
//========= START OF HOOKING LIBRARY =============================================================
//================================================================================================

#define HOOK(library, funcname) {L###library, #funcname, NULL, \
    &New_##funcname, (void **) &Old_##funcname }

#define HOOK1( library, funcname, hkaddr ) { (NULL), (#funcname), (hkaddr), \
    ( &New_##funcname ), ( (void **) &Old_##funcname ) }

#define HOOK2(library, funcname, recursion) {L###library, #funcname, NULL, \
    &New2_##funcname, (void **) &Old2_##funcname, recursion}

hook_t g_hooks[] = {

    //
    // Special Hooks
    //
    // NOTE: due to the fact that the "special" hooks don't use a hook count
    // (whereas the "normal" hooks, those with allow_hook_recursion set to
    // zero, do) we have to hook the "special" hooks first. Otherwise the
    // execution flow will end up in an infinite loop, because of hook count
    // and whatnot.
    //
    // In other words, do *NOT* place "special" hooks behind "normal" hooks.
    //

    //HOOK2(ntdll, LdrLoadDll, TRUE),
    //HOOK2(kernel32, CreateProcessInternalW, TRUE),

    ////
    //// File Hooks
    ////

    //HOOK(ntdll, NtCreateFile),
    //HOOK(ntdll, NtOpenFile),
    //HOOK(ntdll, NtReadFile),
    //HOOK(ntdll, NtWriteFile),
    //HOOK(ntdll, NtDeleteFile),
    //HOOK(ntdll, NtDeviceIoControlFile),
    //HOOK(ntdll, NtQueryDirectoryFile),
    //HOOK(ntdll, NtQueryInformationFile),
    //HOOK(ntdll, NtSetInformationFile),
    //HOOK(ntdll, NtOpenDirectoryObject),
    //HOOK(ntdll, NtCreateDirectoryObject),

    //// CreateDirectoryExA calls CreateDirectoryExW
    //// CreateDirectoryW does not call CreateDirectoryExW
    //HOOK(kernel32, CreateDirectoryW),
    //HOOK(kernel32, CreateDirectoryExW),

    //HOOK(kernel32, RemoveDirectoryA),
    //HOOK(kernel32, RemoveDirectoryW),

    //// lowest variant of MoveFile()
    //HOOK(kernel32, MoveFileWithProgressW),

    //HOOK(kernel32, FindFirstFileExA),
    //HOOK(kernel32, FindFirstFileExW),

    //// Covered by NtCreateFile() but still grap this information
    //HOOK(kernel32, CopyFileA),
    //HOOK(kernel32, CopyFileW),
    //HOOK(kernel32, CopyFileExW),

    //// Covered by NtSetInformationFile() but still grap this information
    //HOOK(kernel32, DeleteFileA),
    //HOOK(kernel32, DeleteFileW),

    ////
    //// Registry Hooks
    ////
    //// Note: Most, if not all, of the Registry API go natively from both the
    //// A as well as the W versions. In other words, we have to hook all the
    //// ascii *and* unicode APIs of those functions.
    ////

    //HOOK(advapi32, RegOpenKeyExA),
    //HOOK(advapi32, RegOpenKeyExW),

    //HOOK(advapi32, RegCreateKeyExA),
    //HOOK(advapi32, RegCreateKeyExW),

    //// Note that RegDeleteKeyEx() is available for 64bit XP/Vista+
    //HOOK(advapi32, RegDeleteKeyA),
    //HOOK(advapi32, RegDeleteKeyW),

    //// RegEnumKeyA() calls RegEnumKeyExA(), but RegEnumKeyW() does *not*
    //// call RegEnumKeyExW()
    //HOOK(advapi32, RegEnumKeyW),
    //HOOK(advapi32, RegEnumKeyExA),
    //HOOK(advapi32, RegEnumKeyExW),

    //HOOK(advapi32, RegEnumValueA),
    //HOOK(advapi32, RegEnumValueW),

    //HOOK(advapi32, RegSetValueExA),
    //HOOK(advapi32, RegSetValueExW),

    //HOOK(advapi32, RegQueryValueExA),
    //HOOK(advapi32, RegQueryValueExW),

    //HOOK(advapi32, RegDeleteValueA),
    //HOOK(advapi32, RegDeleteValueW),

    //HOOK(advapi32, RegQueryInfoKeyA),
    //HOOK(advapi32, RegQueryInfoKeyW),

    //HOOK(advapi32, RegCloseKey),

    ////
    //// Native Registry Hooks
    ////

    //HOOK(ntdll, NtCreateKey),
    //HOOK(ntdll, NtOpenKey),
    //HOOK(ntdll, NtOpenKeyEx),
    //HOOK(ntdll, NtRenameKey),
    //HOOK(ntdll, NtReplaceKey),
    //HOOK(ntdll, NtEnumerateKey),
    //HOOK(ntdll, NtEnumerateValueKey),
    //HOOK(ntdll, NtSetValueKey),
    //HOOK(ntdll, NtQueryValueKey),
    //HOOK(ntdll, NtQueryMultipleValueKey),
    //HOOK(ntdll, NtDeleteKey),
    //HOOK(ntdll, NtDeleteValueKey),
    //HOOK(ntdll, NtLoadKey),
    //HOOK(ntdll, NtLoadKey2),
    //HOOK(ntdll, NtLoadKeyEx),
    //HOOK(ntdll, NtQueryKey),
    //HOOK(ntdll, NtSaveKey),
    //HOOK(ntdll, NtSaveKeyEx),

    ////
    //// Window Hooks
    ////

    //HOOK(user32, FindWindowA),
    //HOOK(user32, FindWindowW),
    //HOOK(user32, FindWindowExA),
    //HOOK(user32, FindWindowExW),

    ////
    //// Sync Hooks
    ////

    //HOOK(ntdll, NtCreateMutant),
    //HOOK(ntdll, NtOpenMutant),
    //HOOK(ntdll, NtCreateNamedPipeFile),

    ////
    //// Process Hooks
    ////

    //HOOK(ntdll, NtCreateProcess),
    //HOOK(ntdll, NtCreateProcessEx),
    //HOOK(ntdll, NtCreateUserProcess),
    //HOOK(ntdll, RtlCreateUserProcess),
    ////HOOK(ntdll, NtOpenProcess),
    //HOOK(ntdll, NtTerminateProcess),
    //HOOK(ntdll, NtCreateSection),
    //HOOK(ntdll, NtMakeTemporaryObject),
    //HOOK(ntdll, NtMakePermanentObject),
    //HOOK(ntdll, NtOpenSection),
    ////HOOK(kernel32, CreateProcessInternalW),
    //HOOK(ntdll, ZwMapViewOfSection),
    //HOOK(kernel32, ExitProcess),

    //// all variants of ShellExecute end up in ShellExecuteExW
    //HOOK(shell32, ShellExecuteExW),
    //HOOK(ntdll, NtUnmapViewOfSection),
    //// HOOK(ntdll, NtAllocateVirtualMemory),
    //HOOK(ntdll, NtReadVirtualMemory),
    //HOOK(kernel32, ReadProcessMemory),
    //HOOK(ntdll, NtWriteVirtualMemory),
    //HOOK(kernel32, WriteProcessMemory),
    //HOOK(ntdll, NtProtectVirtualMemory),
    //HOOK(kernel32, VirtualProtectEx),
    //HOOK(ntdll, NtFreeVirtualMemory),
    ////HOOK(kernel32, VirtualFreeEx),

    //HOOK(msvcrt, system),

    ////
    //// Thread Hooks
    ////

    //HOOK(ntdll, NtCreateThread),
    //HOOK(ntdll, NtCreateThreadEx),
    //HOOK(ntdll, NtOpenThread),
    //HOOK(ntdll, NtGetContextThread),
    //HOOK(ntdll, NtSetContextThread),
    //HOOK(ntdll, NtSuspendThread),
    //HOOK(ntdll, NtResumeThread),
    //HOOK(ntdll, NtTerminateThread),
    //HOOK(kernel32, CreateThread),
    //HOOK(kernel32, CreateRemoteThread),
    //HOOK(kernel32, ExitThread),
    //HOOK(ntdll, RtlCreateUserThread),

    ////
    //// Misc Hooks
    ////

    //HOOK(user32, SetWindowsHookExA),
    //HOOK(user32, SetWindowsHookExW),
    //HOOK(user32, UnhookWindowsHookEx),
    ////HOOK(ntdll, LdrLoadDll),
    //HOOK(ntdll, LdrGetDllHandle),
    //HOOK(ntdll, LdrGetProcedureAddress),
    //HOOK(kernel32, DeviceIoControl),
    //HOOK(user32, ExitWindowsEx),
    //HOOK(kernel32, IsDebuggerPresent),
    //HOOK(advapi32, LookupPrivilegeValueW),
    ////HOOK(ntdll, NtClose),
    //HOOK(kernel32, WriteConsoleA),
    //HOOK(kernel32, WriteConsoleW),
    //HOOK(user32, GetSystemMetrics),
    //HOOK(user32, GetCursorPos),

    ////
    //// Network Hooks
    ////

    //HOOK(urlmon, URLDownloadToFileW),
    //HOOK(wininet, InternetOpenA),
    //HOOK(wininet, InternetOpenW),
    //HOOK(wininet, InternetConnectA),
    //HOOK(wininet, InternetConnectW),
    //HOOK(wininet, InternetOpenUrlA),
    //HOOK(wininet, InternetOpenUrlW),
    //HOOK(wininet, HttpOpenRequestA),
    //HOOK(wininet, HttpOpenRequestW),
    //HOOK(wininet, HttpSendRequestA),
    //HOOK(wininet, HttpSendRequestW),
    //HOOK(wininet, InternetReadFile),
    //HOOK(wininet, InternetWriteFile),
    //HOOK(wininet, InternetCloseHandle),

    //HOOK(dnsapi, DnsQuery_A),
    //HOOK(dnsapi, DnsQuery_UTF8),
    //HOOK(dnsapi, DnsQuery_W),
    //HOOK( ws2_32, getaddrinfo ),
    //HOOK( ws2_32, GetAddrInfoW ),
     //HOOK( ws2_32, send ),
	 //HOOK( ws2_32, recv )

    ////
    //// Service Hooks
    ////

    //HOOK(advapi32, OpenSCManagerA),
    //HOOK(advapi32, OpenSCManagerW),

    //HOOK( advapi32, CreateServiceA ),

    //HOOK(advapi32, CreateServiceW),

    //HOOK( advapi32, OpenServiceA )

    //HOOK(advapi32, OpenServiceW),
    //HOOK(advapi32, StartServiceA),
    //HOOK(advapi32, StartServiceW),
    //HOOK(advapi32, ControlService),
    //HOOK(advapi32, DeleteService),

    ////
    //// Sleep Hooks
    ////

    //HOOK(ntdll, NtDelayExecution),
    //HOOK(kernel32, GetLocalTime),
    //HOOK(kernel32, GetSystemTime),
    //HOOK(kernel32, GetTickCount),
    //HOOK(ntdll, NtQuerySystemTime),

    ////
    //// Socket Hooks
    ////

    //HOOK(ws2_32, WSAStartup),
    //HOOK(ws2_32, gethostbyname),
    //HOOK(ws2_32, socket),
    //HOOK(ws2_32, connect),
    //HOOK(ws2_32, send),
    HOOK(ws2_32, sendto),
    //HOOK(ws2_32, recv),
    HOOK(ws2_32, recvfrom)
    //HOOK(ws2_32, accept),
    //HOOK(ws2_32, bind),
    //HOOK(ws2_32, listen),
    //HOOK(ws2_32, select),
    //HOOK(ws2_32, setsockopt),
    //HOOK(ws2_32, ioctlsocket),
    //HOOK(ws2_32, closesocket),
    //HOOK(ws2_32, shutdown),

    //HOOK(ws2_32, WSARecv),
    //HOOK(ws2_32, WSARecvFrom),
    //HOOK(ws2_32, WSASend),
    //HOOK(ws2_32, WSASendTo),
    //HOOK(ws2_32, WSASocketA),
    //HOOK(ws2_32, WSASocketW),

    //// HOOK(wsock32, connect),
    //// HOOK(wsock32, send),
    //// HOOK(wsock32, recv),

    //HOOK(mswsock, ConnectEx),
    //HOOK(mswsock, TransmitFile),

	////
    //// DirectX Hooks
    ////
	//HOOK( ?, Present),
	//HOOK1( NULL, Present, (void*)VTable[ PR ] )

};

// get a random hooking method, except for hook_jmp_direct
//#define HOOKTYPE randint(HOOK_NOP_JMP_DIRECT, HOOK_MOV_EAX_INDIRECT_PUSH_RETN)
// error testing with hook_jmp_direct only
#define HOOKTYPE HOOK_MOV_EAX_PUSH_RETN

void set_hooks_dll(const wchar_t *library, int len)
{
    for (int i = 0; i < ARRAYSIZE( g_hooks ); i++) {
        if( !wcsnicmp( g_hooks[i].library, library, len ) ) {
            hook_api( &g_hooks[i], HOOKTYPE );
        }
    }
}

void set_hooks()
{
	Log("\nInside set_hooks");
    // the hooks contain executable code as well, so they have to be RWX
    DWORD old_protect;
    VirtualProtect( g_hooks, sizeof(g_hooks), PAGE_EXECUTE_READWRITE, &old_protect );

    hook_disable();

    // now, hook each api :)
    for ( int i = 0; i < ARRAYSIZE( g_hooks ); i++ ) 
	{
        if( g_hooks[i].allow_hook_recursion != FALSE ) 
		{
            hook_api( &g_hooks[i], HOOKTYPE );
        }
        else 
		{
            hook_api( &g_hooks[i], HOOKTYPE );
        }
    }

    hook_enable();
}
//================================================================================================
//========= END OF HOOKING LIBRARY ===============================================================
//================================================================================================


























DWORD WINAPI MainThread( void )
{	
	DWORD dwDayZSize = NULL;
	int iCount = 0;
	
	AllocConsole();                    /* windows.h */
	freopen( "CONOUT$", "w", stdout ); /* #include<cstdio> */
	
	Log("\n ============================================= ");	
	Log("\n =============== LOG SEPERATOR =============== ");	
	Log("\n ============================================= ");	

	do
	{	
		if( iCount != 3 )
		{
			dwBase = (DWORD)( GetModuleHandle( "DayZ.exe" ) );
			hBase = (HANDLE)dwBase;
			dwD3DBase  = (DWORD)( GetModuleHandle( "d3d9.dll" ) );
			Sleep( 100 );
		}
		Log("\n DayZ Base Failed");
		Log("\n DayZ Handle Failed");
		Log("\n D3D9 Base Failed");
		break;
	}while( !dwBase && !hBase && !dwD3DBase );
	//Log( "\nDayZ.exe - Base Address: %08X", dwBase );
	//Log( "\nDayZ.exe - Handle: %08X", hBase );
	//Log( "\ndwD3DBase: %08X", dwD3DBase );

	dwDayZSize = (DWORD)GetModuleSize("DayZ.exe");
	//Log( "\nDayZ.exe - Size: %08X", dwDayZSize );
	
	// Setup Overlay
	OverlayWindow( VTable );

	// Network Logger
	//set_hooks();

	// Enable Debug Privileges
	EnableDebugPriv();

	// GetCurrentProcess Handle and OpenProcess with All Access
	HANDLE test3 = GetCurrentProcess();	
	Log( "Pseudo Handle: %08X\n", test3 ) ;
	openHandle = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pid_from_process_handle( test3 ) );
	Log( "openHandle: %08X", openHandle ) ;
	 
	
	/* =========================================================================== */
	/* DayZ Present() & Gameoverlayrenderer HOOK Related Information */
	/* =========================================================================== */
	//Define & Initialize Patch Info Structure	
	pPatchInfo = (PPATCH_INFO)malloc( sizeof( PATCH_INFO ) );
	memset( pPatchInfo, 0xEE, sizeof(PATCH_INFO) );
	Log( "pPatchInfo: %08X", pPatchInfo );

	// Assign The Present() addess within SystemCall
	pPatchInfo->SystemCall = (BYTE*)VTable[ PR ];  // This is Present() within directx 9 sdk
	Log( "pPatchInfo->SystemCall: %08X", pPatchInfo->SystemCall );

	// Define Signature & Mask For Present() 
	pPatchInfo->SignatureSize = 0x7;	
	memcpy( pPatchInfo->Signature, "\xE9\x00\x00\x00\x00\x56\x57", pPatchInfo->SignatureSize );
	memcpy( pPatchInfo->SignatureMask, "x????xx", pPatchInfo->SignatureSize );		
	Log( "pPatchInfo->SignatureSize; %08X", &pPatchInfo->SignatureSize );
	Log( "pPatchInfo->Signature: %08X",     pPatchInfo->Signature );
	Log( "pPatchInfo->SignatureMask: %08X", pPatchInfo->SignatureMask );
	
	// Setup Prologue Patch 
	pPatchInfo->PrologPatchOffset = 0x0;
	pPatchInfo->SizePrologPatch = 0x7;
	memcpy( pPatchInfo->PrologPatch, "\xB8\xBE\xBA\xFE\xCA\x50\xC3", pPatchInfo->SizePrologPatch );
	Log( "pPatchInfo->PrologPatchOffset: %08X", &pPatchInfo->PrologPatchOffset );
	Log( "pPatchInfo->SizePrologPatch: %08X", &pPatchInfo->SizePrologPatch );
	Log( "pPatchInfo->PrologPatch: %08X", pPatchInfo->PrologPatch );

	// Assign Detour Functions - Prologue & Epilogue Assignment.
	pPatchInfo->PrologDetour = (BYTE*)Prolog_Present;
	Log( "pPatchInfo->PrologDetour %08X", pPatchInfo->PrologDetour );
	pPatchInfo->EpilogDetour = (BYTE*)Epilog_Present;
	Log( "pPatchInfo->EpilogDetour %08X", pPatchInfo->EpilogDetour );

	// Make Sure our System Call Region has the proper RWE permissions.
	DWORD old_protect;
	if( VirtualProtect( pPatchInfo->SystemCall, pPatchInfo->SignatureSize, PAGE_EXECUTE_READWRITE, &old_protect ) )
	{
		// Verify Signature @ Function We Want To Hook
		if( VerifySignature( pPatchInfo->SignatureMask, pPatchInfo->Signature, pPatchInfo->SignatureSize, pPatchInfo->SystemCall ) )
		{
			// Get Prologues Existing Bytes From Function We Want To Hook
			memcpy( pPatchInfo->PrologOriginal, pPatchInfo->SystemCall, pPatchInfo->SignatureSize );			
			Log( "pPatchInfo->PrologOriginal: %08X", pPatchInfo->PrologOriginal );
			
			// Setup Epilogue Patch 
			// Size of Present Function is 52h or 82d. ( 82 - 6 = 76 )
			pPatchInfo->EpilogPatchOffset = 0x4C;
			pPatchInfo->SizeEpilogPatch = 0x6;
			memcpy( pPatchInfo->EpilogPatch, "\x68\xBE\xBA\xFE\xCA\xC3", pPatchInfo->SizeEpilogPatch );
			Log( "pPatchInfo->EpilogPatchOffset: %08X", &pPatchInfo->EpilogPatchOffset );
			Log( "pPatchInfo->EpilogPatch: %08X", pPatchInfo->EpilogPatch );			
			Log( "pPatchInfo->SizeEpilogPatch: %08X", &pPatchInfo->SizeEpilogPatch );

			// Get Epilogues Existing Bytes From Function We Want To Hook
			void* dEpilogue = NULL;
			dEpilogue = malloc( sizeof( DWORD ) );
			memset( dEpilogue, 0x00, sizeof( DWORD ) );
			dEpilogue = (BYTE*)(pPatchInfo->SystemCall + pPatchInfo->EpilogPatchOffset);
			memcpy( pPatchInfo->EpilogOriginal, dEpilogue, pPatchInfo->SizeEpilogPatch );
			//free( dEpilogue );
			//dEpilogue = NULL;
			//Log( "pPatchInfo->EpilogOriginal: %08X", pPatchInfo->EpilogOriginal );

			// Assign our patches the proper addresses to our Detour Functions.
			MakePatchLegit( pPatchInfo->PrologDetour, pPatchInfo->PrologPatch );
			Log( "pPatchInfo->PrologDetour After Fixup: %08X", pPatchInfo->PrologDetour );
			MakePatchLegit( pPatchInfo->EpilogDetour, pPatchInfo->EpilogPatch );
			//Log( "pPatchInfo->EpilogDetour After Fixup: %08X", pPatchInfo->EpilogDetour );

			FixUpPresent( pPatchInfo );			

			// Prologue Hook Complete :) Except for two bytes
			InsertDetour( pPatchInfo->SystemCall, pPatchInfo->PrologPatch, pPatchInfo->SizePrologPatch, pPatchInfo->PrologPatchOffset );

			FixUpGameOverlayHook( pPatchInfo );			

			// Epilogue Hook Next
			// to do...
		}
	}




	///* =========================================================================== */
	///* DayZ RecvFrom() HOOK Related Information */
	///* =========================================================================== */
	//int iWS2Count = 0;
	//do
	//{	
	//	if( iWS2Count != 3 )
	//	{
	//		dw_WS2_Base = (DWORD)( GetModuleHandle( "ws2_32.dll" ) );
	//		h_WS2_32    = (HANDLE)dw_WS2_Base;
	//		pRecvFrom   = (DWORD*)GetProcAddress( (HMODULE)dw_WS2_Base, "recvfrom" );
	//		Sleep( 100 );
	//	}
	//	Log("\n Win_Sock_2 dw_WS2_Base Failed");
	//	Log("\n Win_Sock_2 h_WS2_32 Handle Failed");		
	//	break;
	//}while( !dw_WS2_Base && !h_WS2_32 && !pRecvFrom );
	//Log("pRecvFrom: 0x%08X", pRecvFrom );
	//Log("h_WS2_32: 0x%08X", h_WS2_32 );
	//Log("dw_WS2_Base: 0x%08X", dw_WS2_Base );

	//pRF_Hook_Info = (PRECVFROM_HOOK)malloc( sizeof( _RecvFrom_Hook ) );
	//memset( pRF_Hook_Info, 0xEE, sizeof( _RecvFrom_Hook ) );
	//Log("pRF_Hook_Info: 0x%08x", pRF_Hook_Info );

	///* Address To Function */
	//pRF_Hook_Info->bpRecvFrom_Address = (BYTE*)pRecvFrom;

	///* Sanity Check To Find Out When BattlEye Steals My Damn Function */
	//pRF_Hook_Info->dwBattlEye_Signature_Of_Hook_Size = 0x8;
	//memcpy( pRF_Hook_Info->bBattlEye_Signature_Of_Hook, "\xE9\xBE\xBA\xFE\xCA\x50\xC3\x90", pRF_Hook_Info->dwBattlEye_Signature_Of_Hook_Size );
	//Log("pRF_Hook_Info->bBattlEye_Signature_Of_Hook: 0x%08X", pRF_Hook_Info->bBattlEye_Signature_Of_Hook );
	//memcpy( pRF_Hook_Info->bBattlEye_Signature_Of_Hook_Mask, "x????xxx", pRF_Hook_Info->dwBattlEye_Signature_Of_Hook_Size );
	//Log("pRF_Hook_Info->bBattlEye_Signature_Of_Hook_Mask: 0x%08X", pRF_Hook_Info->bBattlEye_Signature_Of_Hook_Mask );

	///* Store Patch Bytes, Size, and Offset */
	//pRF_Hook_Info->dwPrologue_BattlEye_Patch_Offset = 0x0;
	//pRF_Hook_Info->dwPrologue_BattlEye_Patch_Size   = 0x8;
	//memcpy( pRF_Hook_Info->bPrologue_BattlEye_Patch, "\xB8\xBE\xBA\xFE\xCA\x50\xC3\x90", pRF_Hook_Info->dwPrologue_BattlEye_Patch_Size );
	//Log("pRF_Hook_Info->bPrologue_BattlEye_Patch: 0x%08X", pRF_Hook_Info->bPrologue_BattlEye_Patch );

	///* Address To Our Wrapper Function */
	//pRF_Hook_Info->bpPrologue_Function_Address = (BYTE*)Prologue_RecvFrom;
	//Log("pRF_Hook_Info->bpPrologue_Function_Address: 0x%08x", pRF_Hook_Info->bpPrologue_Function_Address );

	//// Set flag for BEClient.dll
	//dwFlag = 0x0;








	/* =========================================================================== */
	/* DayZ & Present() Related Memory Information */
	/* =========================================================================== */
	pCanvas = &Canvas;
	
	//CArmaMain Signature
	g_dwArmaMainOffset = g_Tools.FindPattern( dwBase, dwDayZSize, (PBYTE)szArmaMainSignature, const_cast<char*>(szArmaMainMask) );
	if( g_dwArmaMainOffset )
	{
		g_pArmaMain = CArmaMain::GetInstance();
	}
	Log( "\n****** Arma Main ****** \n" );
	Log( "g_dwArmaMainOffset 0x%08X\n", g_dwArmaMainOffset );
	Log( "g_pArmaMain 0x%08X\n", g_pArmaMain );


	//Transformations Signature
	g_dwTransformsOffset = g_Tools.FindPattern( dwBase, dwDayZSize, (PBYTE)szTransformSignature, const_cast<char*>(szTransformMask) );
	if( g_dwTransformsOffset )
	{
		do
		{
			g_pTransformations = Transformations::GetInstance();
			Sleep(10);			
		}
		while(!g_pTransformations);
	}
	Log( "\n****** Transformations ****** \n" );
	Log( "g_dwTransformationsOffset 0x%08X\n", g_dwTransformsOffset );
	Log( "g_pTransformations 0x%08X\n",   g_pTransformations );


	//ScoreBoard Signature
	g_dwScoreBoardOffset = g_Tools.FindPattern( dwBase, dwDayZSize, (PBYTE)szScoreBoardSignature, const_cast<char*>(szScoreBoardMask) );
	if( g_dwScoreBoardOffset )
	{
		g_pScoreBoard = ScoreBoard::GetInstance();
	}
	Log( "\n****** ScoreBoard ****** \n" );
	Log( "g_dwScoreBoardOffset 0x%08X\n", g_dwScoreBoardOffset );
	Log( "g_pScoreBoard 0x%08X\n", g_pScoreBoard );


	//free( pRF_Hook_Info );
	//pRF_Hook_Info = NULL;
	
	//free( pPatchInfo );
	//pPatchInfo = NULL;
	
	return 0;
}



Naked void Prologue_RecvFrom( void )
{
	__asm
	{
		JMP [Fixup_RecvFrom]
	}
}





bool VerifySignature( BYTE* pcSigMask, BYTE* pbSig, DWORD dSigSize, BYTE* pbFuncAddr )
{
	//Log("Inside VerifySignature");

	//copy bytes from address into temp
	BYTE temp[128] = {NULL};
	int count = NULL; 
	memcpy( &temp, pbFuncAddr, dSigSize );
	
	//check bytes to signature
	for( DWORD i = 0; i < dSigSize; i++ )
	{
		if( pcSigMask[i] == 'x' )
		{
			//Log( "pcSigMask: %c", *pcSigMask );
			if( temp[i] != pbSig[i] )
			{ 				
				return false;
			}
		   count++;			   	   
		}
	}

	//Log("Count: %d", count );
	return true;
}



void EpilogueMsg( void )
{
	Log( "Displaying Epilogue Message" );
}

























void PrologueMsg( void )
{	
	// ESP + C is Device for Present()

	D3DXVECTOR3 drawScreenPos;
		
	PDevice_Present->GetViewport( &g_Viewport );
		
	pCanvas->Init( PDevice_Present );
	pCanvas->Begin();
	

	// Start Of Hack
	/*===============================================================*/
	/*======================  Score Board  ==========================*/
	/*===============================================================*/
	SB_Player* newPlayer = new SB_Player() ;
	memset ( newPlayer, 0xEE, sizeof( SB_Player ) ) ;
	vector<SB_Player*> sbPlayer;
	ScoreBoard* pScoreBoard = GetScoreBoard( g_pScoreBoard );
	if( pScoreBoard != NULL && pScoreBoard != (ScoreBoard*)0xEEEEEEEE && pScoreBoard != (ScoreBoard*)0x0000000C )
	{
		if( pScoreBoard->NumOfPlayers != NULL && pScoreBoard->NumOfPlayers != 0xEEEEEEFE )
		{
			for( int k = 0; k < pScoreBoard->NumOfPlayers; k++ )
			{
				newPlayer->sb_PlayerID   = *( pScoreBoard->ScoreBoardArray + ( ( ( k * 0xE8 ) + 0x4  ) / 4 ) );
				//Log("newPlayer->sb_PlayerID: %d\n", newPlayer->sb_PlayerID );
				newPlayer->sb_PlayerName = (ArmaString*)*( pScoreBoard->ScoreBoardArray + ( ( ( k * 0xE8 ) + 0x80 ) / 4 ) );
				//Log("newPlayer->sb_PlayerName: %s\n", newPlayer->sb_PlayerName->Name );
				sbPlayer.push_back( newPlayer );
			}
		}
	}// Scoreboard check
	
	
	/*===============================================================*/
	/*======================  Recvfrom Hook =========================*/
	/*===============================================================*/	
	//Log("iFrameCount: %d", iFrameCount );
	//Log("Inside PrologueMsg pRF_Hook_Info: 0x%08x", pRF_Hook_Info );
	hBEClient = GetModuleHandle("BEClient.dll");
	//if( iFrameCount == 600 )
	if( hBEClient != NULL )
	{
		if( pRF_Hook_Info != NULL && dwFlag == 0x0 )
		{
			Log( "Inside PrologueMsg Checking pRF_Hook_Info: 0x%08X", pRF_Hook_Info );
			/*Everything written here will have to be inside the game loop checking each frame for battleye's damn hook*/		
			if( VirtualProtect( pRF_Hook_Info->bpRecvFrom_Address, pRF_Hook_Info->dwBattlEye_Signature_Of_Hook_Size, PAGE_EXECUTE_READWRITE, &dwOld_RecvFromProtect ) )
			{
				if( VerifySignature( pRF_Hook_Info->bBattlEye_Signature_Of_Hook_Mask, 
									 pRF_Hook_Info->bBattlEye_Signature_Of_Hook, 
									 pRF_Hook_Info->dwBattlEye_Signature_Of_Hook_Size, 
									 pRF_Hook_Info->bpRecvFrom_Address ) )
				{
					/* Store Original Bytes */
					memcpy( pRF_Hook_Info->bPrologue_BattlEye_Original, 
							pRF_Hook_Info->bpRecvFrom_Address, 
							pRF_Hook_Info->dwBattlEye_Signature_Of_Hook_Size );
					Log( "Original Bytes: 0x%08X", pRF_Hook_Info->bPrologue_BattlEye_Original );
			
					/* Make Patch Legit */
					MakePatchLegit( pRF_Hook_Info->bpPrologue_Function_Address, 
									pRF_Hook_Info->bPrologue_BattlEye_Patch );

					/* Fixup for Return Address */
					FixUpRecvFrom( pRF_Hook_Info );

					/* Insert Our Object and Its members */
					InsertDetour( pRF_Hook_Info->bpRecvFrom_Address, 
								  pRF_Hook_Info->bPrologue_BattlEye_Patch, 
								  pRF_Hook_Info->dwPrologue_BattlEye_Patch_Size, 
								  pRF_Hook_Info->dwPrologue_BattlEye_Patch_Offset );		

					// set flag, that we have ran this area of code.
					dwFlag = 0x1;
				}
			}
		}
		hBEClient   = NULL;
		//iFrameCount = 0x0;
	}
	//iFrameCount++;
	
	
	/*===============================================================*/
	/*======================   Entities   ===========================*/
	/*===============================================================*/
	vector<DWORD*> entities;
	DWORD *player = NULL, *location = NULL, *cords = NULL;
	D3DXVECTOR3 vec3Cords, vecPosition;
	char coordinates[500];
	char coordinates1[500];
	int ycoord = 0;

	DWORD dwWorld = ReadAddr( g_pArmaMain );	
	World*  wObject = (World*)dwWorld;

	//printf ( "World: 0x%08X\n", wObject ) ;
	//printf("SizeEntityArray: %d\n", wObject->Entity->SizeEntityArray );
	for( int i = 0; i < wObject->Entity->SizeEntityArray; i++ )
	{
		entities.push_back( ( wObject->Entity->EntityArray + ( ( i * 0x2C ) / 4 ) ) );
		//Log( "*( wObject-Entity + %d ) : %08X\n", i, ( wObject->Entity->EntityArray + ( ( i * 0x2C ) / 4 ) ) );
	}
	
	EntitySize = entities.size() ;

	printf("Vector Size: %d\n", entities.size() ) ;
	for( int j = 0; j < entities.size(); j++ )                 // This is the main entity loop here ???
	{
		// Grab Entity player, location, and coordinates
		player   = (DWORD*)( ReadAddr( entities[ j ] ) + 0x4  ); // Edit during update
		location = (DWORD*)( ReadAddr( player        ) + 0x1C ); // Edit during update		
		cords    = (DWORD*)( ReadAddr( location      ) + 0x28 ); // Edit during update
		
		//Grab Entity ePlayerID
		DWORD *pePlayerID;		
		pePlayerID = (DWORD*)( ReadAddr( player ) + 0x07F0 );
		if( pePlayerID != NULL && pePlayerID != (DWORD*)0x000007F0 )
		{
			//Log( "pePlayerID: 0x%08X\n", pePlayerID );
			//Log( "ePlayerID: %d\n", *pePlayerID );		

			// Grab Entity Type Name
			DWORD *ppEntityName = NULL, *pEntityName = NULL, *pEntityType = NULL;		
			ArmaString *sArma;
			ppEntityName = (DWORD*)( ReadAddr( player       ) + 0x50 ); // Edit during update
			pEntityName  = (DWORD*)( ReadAddr( ppEntityName ) + 0x70 );	// Edit during update	
			pEntityType  = (DWORD*)( ReadAddr( pEntityName  )        );   
			sArma        = (ArmaString*)pEntityType;

			if ( sArma != NULL && sArma != (ArmaString*)0xEEEEEEEE )
			{		
				if( cords != NULL && cords != (DWORD*)0xEEEEEF16 && cords != (DWORD*)0xEEEEEF15 )
				{
					vec3Cords = *(D3DXVECTOR3*)cords;
				}		
				
				/*===============================================================*/
				/*======================   3D World Entities  ===================*/
				/*===============================================================*/
				drawScreenPos = W2SN( vec3Cords ) ;
				if( drawScreenPos.x == 1 )
				{								
					do
					{
						drawScreenPos =  W2SN( vec3Cords ) ;
					}
					while( drawScreenPos.x != 1 ) ;
				}

				if( drawScreenPos.z > 0.01 )
				{			
					bool namecheck2 = false ;
					char *namePlayer2 ;
					for( int d = 0; d < sbPlayer.size(); d++ )
					{
						//Log("SB_PlayerID: %d\n", sbPlayer[d]->sb_PlayerID );
						//Log("E_PlayerID: %d\n", *pePlayerID );
						if( *pePlayerID == sbPlayer[d]->sb_PlayerID )
						{							
							namePlayer2 = sbPlayer[d]->sb_PlayerName->Name ;	
							//Log( "W2SN sbPlayer[%d]->sb_PlayerName: %s\n", d, sbPlayer[d]->sb_PlayerName );
							namecheck2 = true ;
						}
					}

					if( namecheck2 )
					{
						//printf("namePlayer2: %s\n", namePlayer2 );
						// Display To Screen
						sprintf( coordinates1, "[%02d][%s]", j, namePlayer2 ) ;
					}
					else
					{
						//printf("sArma->Name: %s\n", sArma->Name );
						// Display To Screen
						sprintf( coordinates1, "[%02d][%-1.1s]", j, sArma->Name ) ;	
					}
					
					pCanvas->Text( coordinates1, drawScreenPos.x, drawScreenPos.y, D3DCOLOR_RGBA(255, 255, 0, 0) ) ;
				}
				
				/*===============================================================*/
	            /*======================   2D Screen Entities   =================*/
	            /*===============================================================*/
				// Fixup For DayZ Map Online
				vecPosition.z = (float)15360 - vec3Cords.z ;									
				vecPosition.y = vec3Cords.y   / 100 ;
				vecPosition.x = vec3Cords.x   / 100 ;
				vecPosition.z = vecPosition.z / 100 ;	
				
				// List Window Test
				testCords.x = vecPosition.x ; 
				testCords.y = vecPosition.y ;
				testCords.z = vecPosition.z ;
				printf ( "dllmain testCords.x: %7.3f\n", testCords.x ) ;
								
				bool namecheck = false ;
				char *namePlayer ;
				for( int d = 0; d < sbPlayer.size(); d++ )
				{
					//Log("sbPlayer[d]->sb_PlayerID: %d\n", sbPlayer[d]->sb_PlayerID );
					//Log("*pePlayerID: %d\n", *pePlayerID );
					if( *pePlayerID == sbPlayer[d]->sb_PlayerID )
					{
						namePlayer = sbPlayer[d]->sb_PlayerName->Name ;
						//Log( "sbPlayer[%d]->sb_PlayerName: %s\n", d, sbPlayer[d]->sb_PlayerName );
						namecheck = true ;
					}
				}

				if( namecheck )
				{
					// Display To Screen
					sprintf( coordinates, "[%02d][%s]X[%7.3f]Y[%7.3f]", j, namePlayer, vecPosition.x, vecPosition.z ) ;
					mapEntityName[ j ] = namePlayer ;
					mapCoords[ j ] = testCords ;
				}
				else
				{
					// Display To Screen
					sprintf( coordinates, "[%02d][%-1.1s]X[%7.3f]Y[%7.3f]", j, sArma->Name, vecPosition.x, vecPosition.z ) ;	
					mapEntityName[ j ] = sArma->Name ;
					mapCoords[ j ] = testCords ;
				}
			
				// Fixup Display Position Of Text
				if( j == 0 ){ ycoord = 10 ;  }
				else{ ycoord = ycoord + 18 ;	}

				//Red is yellow	//Green is purple //blue is red											
				pCanvas->Text( coordinates, 10, ycoord, D3DCOLOR_RGBA( 255, 255, 0, 0 ) ) ;		

			}// end of sArma check
		
		} //ePlayerID check
	
	}// End Of For Loop Entities
	

	Log ( "DLLMAIN mapCoords Size: %d\n", mapCoords.size() ) ;

	// End Of Hack

	// Clean UP
	delete newPlayer ; 
	newPlayer = nullptr ;

	pCanvas->End() ;
}




















D3DXVECTOR3 W2SN( D3DXVECTOR3 in )
{
	D3DXVECTOR3 out,temp;
	DWORD dwTransform;
	DWORD* pScreenMatrix = NULL;
	ScreenMatrix* smObject;

	dwTransform = ReadAddr(g_pTransformations);
	//Log( "pTransform: 0x%08X\n", dwTransform );
	pScreenMatrix = (DWORD*)( dwTransform + 0x94 );	
	if( pScreenMatrix != NULL && pScreenMatrix != (DWORD*)0x00000094 )
	{
		//Log( "pScreenMatrix: 0x%08X\n", pScreenMatrix );
		smObject = (ScreenMatrix*)*pScreenMatrix;
		//Log("smObject: 0x%08X\n", smObject );
		//Log("smObject->InViewRight: %7.3f\n", smObject->InViewRight.x );
		//Log("smObject->InViewUp: %7.3f\n", smObject->InViewUp.x );
		//Log("smObject->InViewForward: %7.3f\n", smObject->InViewForward.x );
		//Log("smObject->InViewTranslation: %7.3f\n", smObject->InViewTranslation.x );
		//Log("smObject->ViewPortMatrix: %7.3f\n", smObject->ViewPortMatrix.x );
		//Log("smObject->ProjD1: %7.3f\n", smObject->ProjD1.x );
		//Log("smObject->ProjD2: %7.3f\n", smObject->ProjD2.x );
	
		::D3DXVec3Subtract( &temp, &in, &smObject->InViewTranslation );

		float x = ::D3DXVec3Dot( &temp, &smObject->InViewRight );
		float y = ::D3DXVec3Dot( &temp, &smObject->InViewUp );
		float z = ::D3DXVec3Dot( &temp, &smObject->InViewForward );

		out.x =  smObject->ViewPortMatrix.x * ( 1 + ( x / smObject->ProjD1.x / z ) );
		out.y =  smObject->ViewPortMatrix.y * ( 1 - ( y / smObject->ProjD2.y / z ) );
		out.z =  z;
	}

	return out;
}




ScoreBoard* GetScoreBoard( ScoreBoard* sbGlobal )
{
	DWORD* ppScoreBoard = NULL, *pScoreBoard = NULL;
	ScoreBoard* sbObject = NULL;
	ppScoreBoard = (DWORD*)( ( (DWORD)sbGlobal ) + 0x28 );
	if( ppScoreBoard != NULL && ppScoreBoard != (DWORD*)0xEEEEEEEE )
	{
		ppScoreBoard = (DWORD*)( ReadAddr( ppScoreBoard ) );
		//Log( "ppScoreBoard 0x%08X\n", ppScoreBoard );		
		ppScoreBoard = (DWORD*)( ( (DWORD)ppScoreBoard ) + 0xC );
		sbObject = (ScoreBoard*)ppScoreBoard;

		if( sbObject != NULL && sbObject != (ScoreBoard*)0xEEEEEEEE && sbObject != (ScoreBoard*)0x0000000C )
		{
			//Log( "sbOject: 0x%08X", sbObject );
			//Log( "sbObject->ScoreBoardArray: 0x%08X\n", sbObject->ScoreBoardArray );
			//Log( "sbObject->NumOfPlayers: %d\n", sbObject->NumOfPlayers );
			//Log( "sbObject->MaxNumOfPlayers: %d\n", sbObject->MaxNumOfPlayers );
			return sbObject;
		}
	}

	return sbObject;
}




DWORD ReadAddr( void* pdwPointer )
{
	DWORD result;
	DWORD dwBytesRead;
	DWORD* pTemp = (DWORD*)malloc( sizeof(DWORD) );
	memset( pTemp, 0xEE, sizeof(DWORD) );
	if( ( ReadProcessMemory( openHandle, (LPCVOID)pdwPointer, pTemp, sizeof(DWORD), &dwBytesRead ) ) != 0 )
	{		
		//Log("*pTemp: %08X", *pTemp);
	}
	
	result = *pTemp;

	free( pTemp );
	pTemp = NULL;

	return result;
}




Naked void GameOverlay_Detour( void )
{
	__asm
	{
		JMP [Fixup_ReturnOriginal];
	}
}


Naked void GameOverlay_Detour2( void )
{
	__asm
	{
		JMP [Fixup_GameOverlayStolenBytes];
	}
}


Naked void Prolog_Present( void )
{
	__asm
	{
		// Could grab device to present() here too.
		// ESP + 4
		MOV EAX, [ESP + 4]
		MOV PDevice_Present, EAX		
		CALL PrologueMsg		
	}
	
	/* Normal Rebuilding of the function */
	/* Not used as this function is hooked already */
	// Rebuilding Stolen Bytes
	//__asm
	//{
	//	JMP [Fixup_RebuildBytes]
	//	PUSH ESI
	//	PUSH EDI
	//}

	//// Returning Skip Patch
	//__asm
	//{
	//	PUSH [Fixup_ReturnOriginal]
	//	RET
	//}

	/* Being that this function is hooked already all we have to do is call the jmp */
	__asm
	{
		// Jump to gameoverlayrenderer hook within present()
		JMP [Fixup_RebuildBytes]
	}

}


Naked void Epilog_Present( void )
{
	__asm
	{
		CALL EpilogueMsg
	}
}

void MakePatchLegit( BYTE* newRoutine, BYTE* patchCode )
{
	DWORD  dwAddress;
	DWORD* pdwNewAddress;

	dwAddress      = (DWORD)newRoutine;     // Copy Address of newRoutine into dwAddress.
	pdwNewAddress  = (DWORD*)&patchCode[1]; // Copy Address to the first byte of cafebabe.
	*pdwNewAddress = dwAddress;             // Assign new Address to the dereferenced address of cafebabe.
	return;
}


void FixUpRecvFrom( PRECVFROM_HOOK pObject )
{	
	// We can't just return back to our original function we have to jump into the hooked functions code.
	Fixup_RecvFrom =  *((DWORD*)&( pObject->bPrologue_BattlEye_Original[1] ));	
	Fixup_RecvFrom += (DWORD)pObject->bpRecvFrom_Address;	
	Fixup_RecvFrom += 0x5;	                                 //
	Log("Fixup_RecvFrom: 0x%08X", Fixup_RecvFrom );
	return;
}


void FixUpPresent( PPATCH_INFO pInfo )
{
	Fixup_RebuildBytes =  *((DWORD*)&(pInfo->PrologOriginal[1]));	
	Fixup_RebuildBytes += (DWORD)pInfo->SystemCall;
	Fixup_RebuildBytes += 5;		
	//Log( "Fixup_RebuildBytes: %08X", Fixup_RebuildBytes );	
	return;
}




void FixUpGameOverlayHook( PPATCH_INFO pInfo )
{
	BYTE temp[10] = { NULL };
	HANDLE hGame;
	DWORD dwBase;
	DWORD dwBytesRead;	
	DWORD* tempPointer;		

	// Beginning of gameoverlay code
	memcpy( temp, (void*)Fixup_RebuildBytes, 0x5 );
	//Log( "temp: %08X", &temp );
	Fixup_GameOverlay = *(DWORD*)&temp[1];	
	Fixup_GameOverlay += Fixup_RebuildBytes;	
	Fixup_GameOverlay += 5;
	//Log( "Fixup_GameOverlay: %08X", Fixup_GameOverlay );
	pInfo->GameOverlayCall = (BYTE*)Fixup_GameOverlay;

	// offset 142 hex into gameoverlay
	pInfo->GameOverlayOffset = 0x142;
	pInfo->SizeGameOverlayPatch = 0x6;
	pInfo->GameOverlayPatchCall = pInfo->GameOverlayCall + pInfo->GameOverlayOffset;
	//Log( "GameOverlayPatchCall: %08X", pInfo->GameOverlayPatchCall );
	
	// Grab GameOverlayRenderer.dll Handle
	int	iCount = 0;
	do
	{
		if( iCount != 3 )
		{
			hGame = (HANDLE)(GetModuleHandle( "GameOverlayRenderer.dll" ) );		
			Sleep( 10 );
		}
		//Log("\n GameOverlay Base Failed");
		break;
	}while( !hGame );
	//Log( "hGame: %08X", hGame );

	// Assign base also; from handle	
	dwBase = (DWORD)hGame;

	// Change address access rights at our Patch Call
	DWORD old_protect;
	if( VirtualProtect( pInfo->GameOverlayPatchCall, 0x148, PAGE_EXECUTE_READWRITE, &old_protect ) )
	{	
		// See if our access rights have taken place
		//if( ( VirtualQueryEx( openHandle, pInfo->GameOverlayPatchCall, &test, sizeof( test ) ) ) == sizeof( test ) )
		//{
		//	Log( "PMEMORY_BASIC_INFORMATION: %08X", &test );
		//	Log( "Protection Of Region Initially: %08X", test.AllocationProtect );
		//	Log( "Protection Of Pages In Region: %08x", test.Protect );
		//}

		// Save Original Bytes into structure at patch call
		if( ( ReadProcessMemory( openHandle, (LPCVOID)pInfo->GameOverlayPatchCall, pInfo->GameOverlayOriginal, pInfo->SizeGameOverlayPatch, &dwBytesRead ) ) != 0 )
		{
			//Log( "RPM - pInfo->GameOverlayOriginal: %08X", pInfo->GameOverlayOriginal );
			//Log( "bytes: %d", dwBytesRead );
		}
		
		// Dereference it to get pointer
		tempPointer = (DWORD*)*(DWORD*)&pInfo->GameOverlayOriginal[2];
		Fixup_GameOverlayStolenBytes = *tempPointer;
		//Log( "Fixup_GameOverlayStolenBytes: %08X", Fixup_GameOverlayStolenBytes );
		
		// Move toward empty memory.
		Fixup_GameOverlayStolenBytes += 0x13;
		//Log( "Fixup_GameOverlayStolenBytes + 0x13: %08X", Fixup_GameOverlayStolenBytes );

		// Modify call to call our new code
		// Use GameOverlayPatchCall for function address 
		// &  GameOverlayOriginal for patching
		// then detour the function
		Fixup_GameOverlayFunctionToRebuild = (DWORD)pInfo->GameOverlayPatchCall;
		//Log( "Fixup_GameOverlayFunctionToRebuild: %08X", Fixup_GameOverlayFunctionToRebuild );
		pInfo->GameOverlayDetour2 = (BYTE*)GameOverlay_Detour2;
		//Log( "GameOverlay_Detour2: %08X", pInfo->GameOverlayDetour2 );

		// Patch GameOverlay Function To Rebuild Old Bytes Stolen From Present()
		//memcpy( pInfo->GameOverlayOriginal, "\xFF\x15\xBE\xBA\xFE\xCA", 0x6 );		
		MakePatchLegitGameOver2( pInfo->GameOverlayDetour2, pInfo->GameOverlayOriginal );
		InsertDetour( (BYTE*)Fixup_GameOverlayFunctionToRebuild, pInfo->GameOverlayOriginal, 0x6, 0 );
		
		// Rebuilding Stolen Bytes From Original Present Function Here 
		memcpy( pInfo->GameOverlayPatch, "\x8B\xFF\x55\x8B\xEC\x56\x57\x68\xBE\xBA\xFE\xCA\xC3", 0xD );
		//Log( "pInfo->GameOverlayPatch: %08X", pInfo->GameOverlayPatch );		
		pInfo->GameOverlayDetour = (BYTE*)GameOverlay_Detour;
		//Log( "pInfo->GameOverlayDetour: %08X", pInfo->GameOverlayDetour );

		// Fixup patch to correct return patch address.
		Fixup_ReturnOriginal = ((DWORD)pInfo->SystemCall) + pInfo->SizePrologPatch;
		//Log( "Fixup_ReturnOriginal: %08X", Fixup_ReturnOriginal );		
		MakePatchLegitGameOver( pInfo->GameOverlayDetour, pInfo->GameOverlayPatch );
		InsertDetour( (BYTE*)Fixup_GameOverlayStolenBytes, pInfo->GameOverlayPatch, 0xD, 0 );
	}
		

	return;
}


void MakePatchLegitGameOver2( BYTE* newRoutine, BYTE* patchCode )
{
	DWORD  dwAddress;
	DWORD* pdwNewAddress;
	DWORD* fuck;
	fuck = (DWORD*)malloc( sizeof( DWORD ) );
	memset( fuck, 0xEE, sizeof( DWORD ) );
	dwAddress      = (DWORD)newRoutine;     // Copy Address of newRoutine into dwAddress.
	pdwNewAddress  = (DWORD*)&patchCode[2]; // Copy Address to the first byte of cafebabe.	
	*fuck = dwAddress;
	//Log( "*fuck: %08X", *fuck );
	//Log( "fuck: %08X", fuck );
	*pdwNewAddress = (DWORD)fuck;             // Assign new Address to the dereferenced address of cafebabe.

	//free( fuck );
	//fuck = NULL;
	return;
}

void MakePatchLegitGameOver( BYTE* newRoutine, BYTE* patchCode )
{
	DWORD  dwAddress;
	DWORD* pdwNewAddress;

	dwAddress      = (DWORD)newRoutine;     // Copy Address of newRoutine into dwAddress.
	pdwNewAddress  = (DWORD*)&patchCode[8]; // Copy Address to the first byte of cafebabe.
	*pdwNewAddress = dwAddress;             // Assign new Address to the dereferenced address of cafebabe.
	return;
}

void EnableDebugPriv( void )
{
    HANDLE              hToken;
    LUID                SeDebugNameValue;
    TOKEN_PRIVILEGES    TokenPrivileges;

    if( OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
    {
        if( LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &SeDebugNameValue ) )
        {
			// New State Privileges, used in AdjustTokenPrivileges().. See MSDN...
            TokenPrivileges.PrivilegeCount              = 1;
            TokenPrivileges.Privileges[0].Luid          = SeDebugNameValue;
            TokenPrivileges.Privileges[0].Attributes    = SE_PRIVILEGE_ENABLED;

            if( AdjustTokenPrivileges( hToken, FALSE, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL ) )
            {
				Log( "Adjustment Of Privileges Completed..." );
                CloseHandle(hToken);
            }
            else
            {
                CloseHandle(hToken);
                Log( "AdjustTokenPrivileges() Failed!" );              
            }
        }
        else
        {
            CloseHandle( hToken );
            Log( "LookupPrivilegeValue() Failed!" );
        }
    }
    else
    {
		DWORD error;
		error = GetLastError();
		Log( "Error: %08X", error );
        Log( "OpenProcessToken() Failed!" );
    }
}

DWORD pid_from_process_handle(HANDLE process_handle)
{
    PROCESS_BASIC_INFORMATION pbi = {}; 
	ULONG ulSize;

    LONG ( WINAPI *NtQueryInformationProcess )( HANDLE ProcessHandle, ULONG ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength );
    *(FARPROC *)&NtQueryInformationProcess = GetProcAddress( GetModuleHandle("ntdll"), "NtQueryInformationProcess" );

    if( NtQueryInformationProcess != NULL && NtQueryInformationProcess( process_handle, 0, &pbi, sizeof( pbi ), &ulSize ) >= 0 && ulSize == sizeof( pbi ) ) 
	{
        return pbi.UniqueProcessId;
    }
    return 0;
}

void InsertDetour( BYTE* oldRoutine, BYTE* patchCode, DWORD patchSize, DWORD offset )
{
	DWORD i;
	for( i = 0; i < patchSize; i++ )
	{
		oldRoutine[ i + offset ] = patchCode[ i ];
	}
	return;
}
















//================================================================================================
//========= START OF DLL MAIN ====================================================================
//================================================================================================
BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	
	inj_moduleHandle = hModule ;
	//Log( "Handle DllMain: %08X\n", inj_moduleHandle ) ;

	switch( ul_reason_for_call )
	{
		case DLL_PROCESS_ATTACH:			
			CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)MainThread, 0, 0, 0 ) ;
			CreateThread ( 0, 0, (LPTHREAD_START_ROUTINE)Thread2, 0, 0, 0 ) ;

		case DLL_PROCESS_DETACH:
			//ClearLog();

		default:
			break;
	}
	
	return TRUE;
}

