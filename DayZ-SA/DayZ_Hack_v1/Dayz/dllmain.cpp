// dllmain.cpp
// Project Settings: Character Set Multibyte

#include "stdafx.h"
#include "DayZHiResMap.h"

// If your source code calls out a function that is within both librarys
// you will need to specify which one to use by calling d3dx9.lib first
// then after loading d3d9.lib. As d3dx9.lib takes a higher priority.
// MSDN - Places a library-search record in the object file. This comment type must be accompanied by a commentstring parameter containing 
//        the name (and possibly the path) of the library that you want the linker to search. The library name follows the default library-search 
//        records in the object file; the linker searches for this library just as if you had named it on the command line provided that the library is 
//        not specified with /nodefaultlib. You can place multiple library-search records in the same source file; each record appears in the object file 
//        in the same order in which it is encountered in the source file.
//        If the order of the default library and an added library is important, compiling with the /Zl switch will prevent the default library name from being 
//        placed in the object module. A second comment pragma then can be used to insert the name of the default library after the added library. The libraries 
//        listed with these pragmas will appear in the object module in the same order they are found in the source code. 
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")

#pragma warning( disable : 4996 )
#pragma warning( disable : 4244 )


//=========== DEFINES =======================================================================================
// the ## is just a concatenation operator
// PBYTE WINAPI DetourFunction(PBYTE pbTargetFunction, PBYTE pbDetourFunction);
#define HOOK4( func, addy )	o##func = ( func##_t )DetourFunction( (PBYTE)addy, (PBYTE)hk##func )
#define PR         0
#define ES         1
#define DIP        2
#define SSS        3
//===========================================================================================================


//=================== HOOKS AND TYPEDEFS ==============================================================================================================================================================
typedef HRESULT ( WINAPI * EndScene_t )( LPDIRECT3DDEVICE9 pDevice );
EndScene_t oEndScene;
HRESULT WINAPI hkEndScene( LPDIRECT3DDEVICE9 pDevice );

typedef HRESULT ( WINAPI * DrawIndexedPrimitive_t )( LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount );
DrawIndexedPrimitive_t  oDrawIndexedPrimitive;
HRESULT WINAPI hkDrawIndexedPrimitive( LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount );

typedef HRESULT   ( WINAPI * SetStreamSource_t )( LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, IDirect3DVertexBuffer9 * pStreamData, UINT OffsetInBytes, UINT Stride );
SetStreamSource_t       oSetStreamSource;
HRESULT WINAPI hkSetStreamSource( LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride );

typedef HRESULT ( WINAPI * Present_t )( LPDIRECT3DDEVICE9 pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion );
Present_t oPresent;
HRESULT WINAPI hkPresent( LPDIRECT3DDEVICE9 pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion );
//=====================================================================================================================================================================================================


//========================== COLORS =========================================================
// D3DCOLORS come in hex, octal, decimal values...
// Decimal RGB values
CONST D3DCOLOR Red        = D3DCOLOR_ARGB(255, 255, 0, 0);
CONST D3DCOLOR Green      = D3DCOLOR_ARGB(255, 0, 255, 0);
CONST D3DCOLOR Orange     = D3DCOLOR_ARGB(255, 255, 165, 0);
CONST D3DCOLOR Blue       = D3DCOLOR_ARGB(255, 0, 0, 255);
CONST D3DCOLOR Yellow     = D3DCOLOR_ARGB(255, 255, 255, 0);
CONST D3DCOLOR Black      = D3DCOLOR_ARGB(255, 0, 0, 0);
CONST D3DCOLOR Grey       = D3DCOLOR_ARGB(255, 84, 84, 84);
CONST D3DCOLOR Gold       = D3DCOLOR_ARGB(255, 255, 215, 0);
CONST D3DCOLOR SpicyPink  = D3DCOLOR_ARGB(255, 255, 28, 174);
CONST D3DCOLOR Purple     = D3DCOLOR_ARGB(255, 128, 0, 128);
CONST D3DCOLOR White      = D3DCOLOR_ARGB(255, 255, 255, 255);
CONST D3DCOLOR Cyan       = D3DCOLOR_ARGB(255, 0, 255, 255);
CONST D3DCOLOR Magenta    = D3DCOLOR_ARGB(255, 255, 0, 255);
//tVec4 colorFirebrick3 = {1, .80, .15, .15};
//tVec4 colorChocolate1 = { 1, 1, .5, .14 };
//tVec4 colorBlack      = { 0, 0, 0, 1 };
//tVec4 colorRed        = { 1, 0, 0, 1 };
//tVec4 colorGreen      = { 0, 1, 0, 1 };
//tVec4 colorBlue       = { 0, 0, 1, 1 };
//tVec4 colorYellow     = { 1, 1, 0, 1 };
//tVec4 colorOrange     = { 1, 0.5, 0, 1 };
//tVec4 colorMagenta    = { 1, 0, 1, 1 };
//tVec4 colorCyan       = { 0, 1, 1, 1 };
//tVec4 colorWhite      = { 1, 1, 1, 1 };
//tVec4 colorLtGrey     = { 0.75, 0.75, 0.75, 1 };
//tVec4 colorMdGrey     = { 0.5, 0.5, 0.5, 1 };
//tVec4 colorDkGrey     = { 0.25, 0.25, 0.25, 1 };
//===========================================================================================




// ===================== Prototype's ==================================================
DWORD WINAPI ThreadProc( LPVOID lpParameter );
VOID OverlayWindow( DWORD* Vtable );
LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK WindowProc2( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
HRESULT DeviceSetup( HWND hwnd, DWORD* Vtab );
//bool ProjectScreen( Vector Origin , float * x, float * y );
//float dotproduct3( Vector one , Vector two );
DWORD GetModuleSize(LPSTR strModuleName);
void PlayerCoordinates();
D3DXVECTOR3 vecToD3DVec( Vector vec );
D3DXVECTOR3 checkLocalPlayerCoordinatesPointers();
void localPlayerCoordinate();
D3DXVECTOR3 checkEntityCoordinatesPointers( int i );
void listBoxWindow();
DWORD WINAPI ThreadProc2( LPVOID lpParameter );
HWND CreateListView (HWND hWndParent);
int CALLBACK ListViewCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
bool pointsAreSame( double a, double b );
bool AreSame(double a, double b);
inline bool isEqual(double x, double y);
bool checkPointer( void* arg );
//========================================================================================================================================

//================ GLOBAL VARIABLES AND ARRAYS ===========================================================================================
ofstream pFile;
LPD3DXFONT g_Font = NULL; // font object
D3DXFONT_DESC FontDesc = { 16, 0, 400, 0, false, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_PITCH, "Arial" };
LPD3DXLINE              g_pLine; 
HANDLE hBase = NULL;
DWORD dwBase = NULL;
HMODULE inj_moduleHandle;
HWND prnt_hWnd;
HWND hButton, hCombo, hEdit, hList, hScroll, hStatic,  hListView ;
LVCOLUMN LvCol; // Make Coluom struct for ListView
LVITEM LvItem;  // ListView Item struct
LRESULT NotifyHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND hwndLootWindow;
// =======================================================================================================================================




// --- LOG ---
VOID WINAPIV Log( CHAR* szFormat, ... )
{
	CHAR szBuf[256]; 
	va_list list; 
	va_start( list, szFormat );
	vsprintf_s( szBuf, szFormat, list ); 
	va_end( list ); 

	pFile.open( "c://BlackOp.log", std::ios::app);
	if( pFile.fail() )
	{
		printf( "\nCould Not Open Log\n" );
		return;
	}
	pFile << szBuf << std::endl;
	pFile.close();
}// END OF LOG




// --- COLOR TRANSFORM ---
void COLOR_TRANSFORM( float * colin , DWORD Color ) {
	colin[0] = (float)((Color)>>16 & 0xff)/255.0f;//R
	colin[1] = (float)((Color)>>8  & 0xff)/255.0f;//G
	colin[2] = (float)((Color)     & 0xff)/255.0f;//B
	colin[3] = (float)((Color)>>24 & 0xff)/255.0f;//A
}// END COLOR TRANSFORM




int key_pressed( int key )
{
    return ( ( GetAsyncKeyState( key ) & 0x8000 ) != 0 );
}



DWORD GetModuleSize(LPSTR strModuleName)
{
	MODULEENTRY32	lpme= {0};
	DWORD			dwSize=0;
	DWORD			PID=GetCurrentProcessId();
	BOOL			isMod=0;
	char			chModName[256];

	strcpy_s(chModName,strModuleName);
	_strlwr_s(chModName);

	HANDLE hSnapshotModule=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE ,PID);
	if (hSnapshotModule) 
	{
		lpme.dwSize=sizeof(lpme);
		isMod=Module32First(hSnapshotModule,&lpme);
		while(isMod) 
		{
			if (strcmp(_strlwr((char*)lpme.szExePath),chModName)) 
			{
				dwSize=(DWORD)lpme.modBaseSize;
				CloseHandle(hSnapshotModule);
				return dwSize;
			}
		isMod=Module32Next(hSnapshotModule,&lpme);
		}
	}
	CloseHandle(hSnapshotModule);

	return 0;
} 

IDirect3DStateBlock9* pBlock = NULL;
LPDIRECT3DTEXTURE9 radarTexture = NULL;
LPD3DXSPRITE Sprite;
D3DVIEWPORT9 g_Viewport;

DWORD g_dwArmaMainOffset;
CArmaMain* g_pArmaMain;

Transformations* g_pTransformations;
DWORD g_dwTransformsOffset;

bool NearLootTest;
bool PlayerAnimalZombieCoords;
bool localPlayerCoords;
bool worldPositions;
DWORD hookaddress;

static DWORD VTable[4] = {0};
DWORD* VTablePointer;
void* VTablePointerVoid;

CTools g_Tools;
//CTransformations* g_pTransformations;

void InitResources(LPDIRECT3DDEVICE9 pDevice);

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
	 // HOOK( ws2_32, recv ),

    ////
    //// Service Hooks
    ////

    //HOOK(advapi32, OpenSCManagerA),
    //HOOK(advapi32, OpenSCManagerW),

    //HOOK( advapi32, CreateServiceA ),

    //HOOK(advapi32, CreateServiceW),

    HOOK( advapi32, OpenServiceA )

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
    //HOOK(ws2_32, sendto),
    //HOOK(ws2_32, recv),
    //HOOK(ws2_32, recvfrom),
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

};

// get a random hooking method, except for hook_jmp_direct
//#define HOOKTYPE randint(HOOK_NOP_JMP_DIRECT, HOOK_MOV_EAX_INDIRECT_PUSH_RETN)
// error testing with hook_jmp_direct only
#define HOOKTYPE HOOK_JMP_INDIRECT

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
	printf("Inside set_hooks\n");
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


// Start of DllMain
BOOL WINAPI DllMain( HMODULE hMod, DWORD dwReason, LPVOID lpReserved )
{
    if( dwReason == DLL_PROCESS_ATTACH )
    {
		inj_moduleHandle = hMod;
		// hide our module from peb
		hide_module_from_peb( hMod );
        CreateThread( 0, 0, ThreadProc, 0, 0, 0 );
		//CreateThread( 0, 0, ThreadProc2, 0, 0, 0 );
    }
    
    return TRUE;

} // End of DllMain

void InitResources(LPDIRECT3DDEVICE9 pDevice)
{
	if(!pBlock)
		pDevice->CreateStateBlock( D3DSBT_ALL, &pBlock );

	if( !radarTexture )
	{
		if( D3D_OK != D3DXCreateTextureFromFileInMemory( pDevice, &maphireshexData, sizeof(maphireshexData), &radarTexture ) ) 
		{
			std::cout <<"D3DXCreateTextureFromFileInMemory Failed";
		}

		D3DXCreateSprite( pDevice, &Sprite );
	}

}

DWORD WINAPI ThreadProc2( LPVOID lpParameter )
{
	listBoxWindow();

	return 0;
}

// Start of ThreadProc
DWORD WINAPI ThreadProc( LPVOID lpParameter )
{
	// Partial Initialization of VTable
	//DWORD VTable[4] = {0};
	DWORD DayZSize;
	DWORD dwD3DBase = NULL;
	//DWORD loopTest = 1;

	AllocConsole();
	freopen("CONOUT$","w",stdout);

	printf("Thread Created\n\n");

	// ========================================================
	// START LOGGING SIGNATURES OF STRUCTURES AND FUNCTIONS
	// ========================================================
	for ( ; dwBase == NULL; Sleep(100) )           
		dwBase = (DWORD)GetModuleHandle("DayZ.exe");

	for ( ; hBase == NULL; Sleep(100) )           
		hBase = GetModuleHandle("DayZ.exe");	

	DayZSize = (DWORD)GetModuleSize("DayZ.exe");

	//printf("dwBase: %ld\n", dwBase);
	//printf("hBase: %X\n", hBase);
	//printf("DayZSize: %X\n", DayZSize);	

	// ========================================================
	// END OF LOGGING SIGNATURES OF STRUCTURES AND FUNCTIONS
	// ========================================================

	//// Find handle to d3d9.dll ( By Brute Forcing :) )
	//while( GetModuleHandle( "d3d9.dll" ) == NULL )
	//{
	//	// Suspends the execution of the current thread until the time-out interval elapses.
	//	// Sleep for ( 250 milliseconds ) = .250 of a second ( 1/4 of a second )
	//	Sleep( 250 );
	//}

	do
	{
		dwD3DBase = reinterpret_cast<DWORD>( GetModuleHandle( "d3d9.dll" ) );
		Sleep( 10 );
	}
	while(!dwD3DBase);

	//printf("dwD3DBase: %X\n", dwD3DBase);

	// Setup Overlay
	OverlayWindow( VTable );	

	// Detour our functions
    // These 3 lines of code use a macro defined above that will add a o onto each 1st argument after DetourFunction returns. This will hold a unsigned char result.
    // The macro assigns the detour return type to oEndScene, oDrawIndexedPrimitive, and oSetStreamSource.
    // The macro above will define how DetourFunction will call these functions above starting with orginal address from (VTable ES, then call hkEndScene), (Vtable DIP, then call hkDrawIndexedPrimitive),
    // and (Vtable SSS, then call hkSetStreamSource.)
    // If you understand this part, you should be able to follow the rest of the program. Good Luck.
    // maybe make a wall hack or chams with it. Draw a radar.
	HOOK4( Present, VTable[ PR ] );
	//HOOK( EndScene, VTable[ ES ] ); // oEndScene, hkEndScene
	//HOOK( DrawIndexedPrimitive, VTable[ DIP ] );
	//HOOK( SetStreamSource, VTable[ SSS ] );
	//

	//printf( "\nVtable[ PR ] Value: %X\n", VTable[ PR ] );
	//VTablePointer = &VTable[ PR ];
	//VTablePointerVoid = VTablePointer;

	//// Testing Pointers and seeing how void* works.
	//DWORD* test;
	//test = &VTable[ PR ];

	//void* test2;
	//test2 = test; 
	//
	//DWORD* test3;	
	//test3 = (DWORD*)test2;

	//printf( "\nDWORD* test: %X\n", test );
	//printf( "Dereference *test: %X\n", *test );
	//printf( "Void* test2: %X\n", test2 );
	//printf( "Address of &test2: %X\n", &test2 );
	//printf( "DWORD* test3: %X\n", test3 );
	//printf( "Dereference *test3: %X\n\n", *test3 );
	//// End of testing void pointers.
	//

	//hook_t* vtableHook;
	//DWORD old_protect;
 //   VirtualProtect( vtableHook, sizeof(vtableHook), PAGE_EXECUTE_READWRITE, &old_protect );
	//
	//vtableHook->addr = VTablePointerVoid;
	//vtableHook->funcname = "Present";
	//vtableHook->new_func = &New_Present;
	//vtableHook->old_func = (void**)&Old_Present;
	//vtableHook->is_hooked = 0;	

	//printf( "vtableHook->addr: %X\n", vtableHook->addr );
	//hook_api( vtableHook, HOOKTYPE );

	//// restore the old protection
 //   VirtualProtect( vtableHook, sizeof(vtableHook), old_protect, &old_protect );

	set_hooks();

	pCanvas = &Canvas;

	
	//CArmaMain signature
	g_dwArmaMainOffset = g_Tools.FindPattern( dwBase, DayZSize, (PBYTE)szArmaMainSignature, const_cast<char*>(szArmaMainMask) );
	if(g_dwArmaMainOffset)
	{
		g_pArmaMain = CArmaMain::GetInstance();
	}	
	//printf("\n\ng_dwArmaMainOffset 0x%X", g_dwArmaMainOffset);
	//printf("\n\ng_pArmaMain 0x%X", g_pArmaMain);

	//Transformations signature
	g_dwTransformsOffset = g_Tools.FindPattern( dwBase, DayZSize, (PBYTE)szTransformSignature, const_cast<char*>(szTransformMask) );
	if( g_dwTransformsOffset )
	{
		do
		{
			g_pTransformations = Transformations::GetInstance();
			Sleep(10);			
		}
		while(!g_pTransformations);
	}
	//printf("\n\ng_dwTransformsOffset 0x%X", g_dwTransformsOffset);
	//printf("\n\ng_pTransformations 0x%X", g_pTransformations);
	
	return 0;
}// End of ThreadProc




#pragma region overlay
VOID OverlayWindow( DWORD* Vtable )
{
	//printf("\n\nInside OverlayWindow\n" );

	// --- Setup Windows Class Information.
   // WNDCLASSEX - Contains window class information. It is used with the RegisterClassEx and GetClassInfoEx  functions. 
   // 1st member  - Size in bytes of structure
   // 2nd member  - style
   // 3rd member  - pointer to Window Procedure.
   // 4th member  - The number of extra bytes to allocate following the window-class structure. The system initializes the bytes to zero.
   //               0L An unsigned long value. (This 'long' is _exactly_ 32 bits, which may differ from  what a local C compiler calls 'long'. If you want native-length longs, use the '!' suffix.)
   // 5th member  - The number of extra bytes to allocate following the window instance. The system initializes the bytes to zero.
   // 6th member  - A handle to the instance that contains the window procedure for the class. GetModuleHandle() has 1 arg - The name of the loaded module (either a .dll or .exe)
   //               If this parameter is NULL, GetModuleHandle returns a handle to the file used to create the calling process (.exe file).
   //               If the function succeeds, the return value is a handle to the specified module.
   // 7th member  - A handle to the class icon. This member must be a handle to an icon resource. If this member is NULL, the system provides a default icon.
   // 8th member  - A handle to the class cursor. This member must be a handle to a cursor resource. If this member is NULL, an application must 
   //               explicitly set the cursor shape whenever the mouse moves into the application's window. 
   // 9th member  - A handle to the class background brush. When this member is NULL, an application must paint its own background whenever it is requested 
   //               to paint in its client area. To determine whether the background must be painted, an application can either process the WM_ERASEBKGND message 
   //               or test the fErase member of the PAINTSTRUCT structure filled by the BeginPaint function.
   // 10th member - Pointer to a null-terminated character string that specifies the resource name of the class menu, as the name appears in the resource file. 
   //               If this member is NULL, windows belonging to this class have no default menu.
   // 11th member - A pointer to a null-terminated string or is an atom. If lpszClassName is a string, it specifies the window class name. The class name 
   //               can be any name registered with RegisterClass or RegisterClassEx, or any of the predefined control-class names. The maximum length for lpszClassName is 256.
   // 12th member - A handle to a small icon that is associated with the window class. If this member is NULL, the system searches the icon resource specified by the hIcon 
   //               member for an icon of the appropriate size to use as the small icon.
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc, 0, 0, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "OW", NULL };

   // api - RegisterClassEx - Registers a window class for subsequent use in calls to the CreateWindow or CreateWindowEx function. 
   // 1st arg - A pointer to a WNDCLASSEX structure. You must fill the structure with the appropriate class attributes before passing it to the function.
   // return type - If the function succeeds, the return value is a class atom that uniquely identifies the class being registered. This atom can 
   //               only be used by the CreateWindow, CreateWindowEx, GetClassInfo, GetClassInfoEx, FindWindow, FindWindowEx, and 
   //               UnregisterClass functions and the IActiveIMMap::FilterClientWindows method. 
	RegisterClassEx(&wc);

   // api - CreateWindow - Creates an overlapped, pop-up, or child window. It specifies the window class, window title, window style, and (optionally) the 
   //                      initial position and size of the window. The function also specifies the window's parent or owner, if any, and the window's menu.
   // 1st arg  - A null-terminated string or a class atom created by a previous call to the RegisterClass or RegisterClassEx function.
   // 2nd arg  - The window name. If the window style specifies a title bar, the window title pointed to by lpWindowName is displayed in the title bar. 
   //           When using CreateWindow to create controls, such as buttons, check boxes, and static controls, use lpWindowName to specify the text of the control.
   // 3rd arg  - The style of the window being created. This parameter can be a combination of the window style values -> Window Styles (Windows)
   // 4th arg  - X coord - The initial horizontal position of the window. For an overlapped or pop-up window, the x parameter is the initial x-coordinate of the window's 
   //            upper-left corner, in screen coordinates. 
   // 5th arg  - Y coord - The initial vertical position of the window. For an overlapped or pop-up window, the y parameter is the initial y-coordinate of 
   //            the window's upper-left corner, in screen coordinates.
   // 6th arg  - The width, in device units, of the window. For overlapped windows, nWidth is either the window's width, in screen coordinates
   // 7th arg  - The height, in device units, of the window. For overlapped windows, nHeight is the window's height, in screen coordinates.
   // 8th arg  - A handle to the parent or owner window of the window being created. GetDesktopWindow() - Retrieves a handle to the desktop window. 
   //            The desktop window covers the entire screen. The desktop window is the area on top of which other windows are painted. Returns - The return value is a handle to the desktop window. 
   // 9th arg  - A handle to a menu, or specifies a child-window identifier depending on the window style. For an overlapped or pop-up window, hMenu 
   //            identifies the menu to be used with the window; it can be NULL if the class menu is to be used. 
   // 10th arg - A handle to the instance of the module to be associated with the window. Number 6 from above wc structure.
   // 11th arg - A pointer to a value to be passed to the window through the CREATESTRUCT structure (lpCreateParams member) pointed to by the lParam param of the WM_CREATE message.
   //             lpParam may be NULL if no additional data is needed.
   // return type - If the function succeeds, the return value is a handle to the new window.
	HWND hWnd = CreateWindow( "OW", NULL, WS_OVERLAPPED, 0, 0, 800, 600, GetDesktopWindow(), NULL, wc.hInstance, NULL );

	// Directx Device Setup
	DeviceSetup(hWnd, Vtable);

	//printf("\n\nDevice Setup Done");
	
	// Windows api - DestroyWindow - Destroys the specified window. The function sends WM_DESTROY and WM_NCDESTROY messages to the window to deactivate 
    //                               it and remove the keyboard focus from it. The function also destroys the window's menu, flushes the thread message queue, 
    //                               destroys timers, removes clipboard ownership, and breaks the clipboard viewer chain (if the window is at the top of the viewer chain).
    // 1st arg - A handle to the window to be destroyed. 
    // return type - If the function succeeds, the return value is nonzero.
	DestroyWindow(hWnd);
}


void listBoxWindow()
{
	MSG messages;
	//char Temp[255];

	// --- Setup Windows Class Information.
   // WNDCLASSEX - Contains window class information. It is used with the RegisterClassEx and GetClassInfoEx  functions. 
   // 1st member  - Size in bytes of structure
   // 2nd member  - style
   // 3rd member  - pointer to Window Procedure.
   // 4th member  - The number of extra bytes to allocate following the window-class structure. The system initializes the bytes to zero.
   //               0L An unsigned long value. (This 'long' is _exactly_ 32 bits, which may differ from  what a local C compiler calls 'long'. If you want native-length longs, use the '!' suffix.)
   // 5th member  - The number of extra bytes to allocate following the window instance. The system initializes the bytes to zero.
   // 6th member  - A handle to the instance that contains the window procedure for the class. GetModuleHandle() has 1 arg - The name of the loaded module (either a .dll or .exe)
   //               If this parameter is NULL, GetModuleHandle returns a handle to the file used to create the calling process (.exe file).
   //               If the function succeeds, the return value is a handle to the specified module.
   // 7th member  - A handle to the class icon. This member must be a handle to an icon resource. If this member is NULL, the system provides a default icon.
   // 8th member  - A handle to the class cursor. This member must be a handle to a cursor resource. If this member is NULL, an application must 
   //               explicitly set the cursor shape whenever the mouse moves into the application's window. 
   // 9th member  - A handle to the class background brush. When this member is NULL, an application must paint its own background whenever it is requested 
   //               to paint in its client area. To determine whether the background must be painted, an application can either process the WM_ERASEBKGND message 
   //               or test the fErase member of the PAINTSTRUCT structure filled by the BeginPaint function.
   // 10th member - Pointer to a null-terminated character string that specifies the resource name of the class menu, as the name appears in the resource file. 
   //               If this member is NULL, windows belonging to this class have no default menu.
   // 11th member - A pointer to a null-terminated string or is an atom. If lpszClassName is a string, it specifies the window class name. The class name 
   //               can be any name registered with RegisterClass or RegisterClassEx, or any of the predefined control-class names. The maximum length for lpszClassName is 256.
   // 12th member - A handle to a small icon that is associated with the window class. If this member is NULL, the system searches the icon resource specified by the hIcon 
   //               member for an icon of the appropriate size to use as the small icon.
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc2, 0, 0, GetModuleHandle(NULL), NULL, NULL, (HBRUSH) (GetStockObject(GRAY_BRUSH)), NULL, "OLW", NULL };


	// http://msdn.microsoft.com/en-us/library/windows/desktop/bb775507(v=vs.85).aspx
	// INITCOMMONCONTROLSEX structure carries information used to load common control classes from the dynamic-link library (DLL).
	// This structure is used with the InitCommonControlsEx function
	// ICC_WIN95_CLASSES -- Load animate control, header, hot key, list-view, progress bar, status bar, tab, tooltip, toolbar, trackbar, tree-view, and up-down control classes.
	// InitCommonControlsEx() -- Ensures that the common control DLL(Comctl32.dll) is loaded, and registers specific common control
	// classes from the DLL. An application must call this function before creating a common control.
	// make sure to load Comctl32.lib
	INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
    icex.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx( &icex );
	
	//prnt_hWnd = FindWindow( NULL, "DayZ" );
	
	//if( prnt_hWnd )
	//{
	 //       // A previous instance of this application is running.

	 //       // Activate the previous instance, tell it what the user
	 //       // requested this instance to do, then abort initialization
	 //       // of this instance.

	 //       if (IsIconic (prnt_hWnd))
	 //           ShowWindow (prnt_hWnd, SW_RESTORE) ;

	 //       SetForegroundWindow (prnt_hWnd) ;

	 //       // Send an application-defined message to the previous
	 //       // instance (or use some other type of IPC mechanism)
	 //       // to tell the previous instance what to do.

	 //       // Determining what to do generally depends on how the
	 //       // user started this instance.

	 //              // ... <some application-specific code here>

	 //       // Abort this instance's initialization
	 //       //return FALSE ;
	 //   }



   // api - RegisterClassEx - Registers a window class for subsequent use in calls to the CreateWindow or CreateWindowEx function. 
   // 1st arg - A pointer to a WNDCLASSEX structure. You must fill the structure with the appropriate class attributes before passing it to the function.
   // return type - If the function succeeds, the return value is a class atom that uniquely identifies the class being registered. This atom can 
   //               only be used by the CreateWindow, CreateWindowEx, GetClassInfo, GetClassInfoEx, FindWindow, FindWindowEx, and 
   //               UnregisterClass functions and the IActiveIMMap::FilterClientWindows method. 
	RegisterClassEx(&wc);

	int desktopwidth  = GetSystemMetrics( SM_CXSCREEN );
	int desktopheight = GetSystemMetrics( SM_CYSCREEN );
	
	// create unowned window
	/*Sends WM_GETMINMAXINFO, WM_NCCREATE, WM_NCCALCSIZE, WM_CREATE */
	HWND hwnd = CreateWindow( "OLW",															//name: wnd 'class'
							  "Common Controls - Listview",										//wnd title
							  WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_POPUPWINDOW | WS_CAPTION,	//wnd style
							  desktopwidth / 4,													//position:left
							  desktopheight / 4,												//position: top
							  desktopwidth / 2,													//width
							  desktopheight / 2,												//height
							  NULL,														//parent wnd handle
							  0,																//menu handle/wnd id
							  wc.hInstance,													//app instance
							  0 );																//user defined info


	//hwndLootWindow = hwnd;


	//SendMessage( hListView, LVM_SETTEXTBKCOLOR, 0, (LPARAM)CLR_NONE );							
	//SendMessage( hListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT ); // Set style
	//						
	//// Here we put the info on the Coulom headers
	//// this is not data, only name of each header we like
	//memset( &LvCol, 0, sizeof( LvCol ) ); 			// Reset Coluomn
	//LvCol.mask = LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM; // Type of mask
	//LvCol.cx = 0x28;                                // width between each coloum
	//LvCol.pszText = "Item";                     	// First Header
	//LvCol.cx = 0x42;

	//// Inserting Columns as much as we want
	//SendMessage( hListView, LVM_INSERTCOLUMN, 0, (LPARAM)&LvCol ); // Insert/Show the column
	//			
	//LvCol.pszText = "Sub Item1";			// Next column				
	//SendMessage( hListView, LVM_INSERTCOLUMN, 1, (LPARAM)&LvCol ); 
	//			
	//LvCol.pszText = "Sub Item2";                       				
	//SendMessage( hListView, LVM_INSERTCOLUMN, 2, (LPARAM)&LvCol ); 
	//			
	//LvCol.pszText = "Sub Item3";                              				
	//SendMessage( hListView, LVM_INSERTCOLUMN, 3, (LPARAM)&LvCol ); 
	//			
	//LvCol.pszText = "Sub Item4";                            				
	//SendMessage( hListView, LVM_INSERTCOLUMN, 4, (LPARAM)&LvCol ); 
	//			
	//LvCol.pszText = "Sub Item5";                      				
	//SendMessage( hListView, LVM_INSERTCOLUMN, 5, (LPARAM)&LvCol ); 

	//memset( &LvItem, 0, sizeof(LvItem) ); // Reset Item Struct				
	////  Setting properties Of Items:
	//LvItem.mask = LVIF_TEXT;   // Text Style
	//LvItem.cchTextMax = 256; // Max size of test                
	//LvItem.iItem = 0;          // choose item  
	//LvItem.iSubItem = 0;       // Put in first coluom
	//LvItem.pszText = "Item 0"; // Text to display (can be from a char variable) (Items)
 //               
	//SendMessage( hListView, LVM_INSERTITEM, 0, (LPARAM)&LvItem ); // Send to the Listview
	//			
	//for( int i = 1; i <= 5; i++ ) // Add SubItems in a loop
	//{
	//	LvItem.iSubItem=i;
	//	sprintf( Temp, "SubItem %d", i );
	//	LvItem.pszText = Temp;
	//	SendMessage( hListView, LVM_SETITEM, 0, (LPARAM)&LvItem ); // Enter text to SubItems
	//}
	//			
	//// lets add a new Item:
	//LvItem.iItem=1;            // choose item  
	//LvItem.iSubItem=0;         // Put in first coluom
	//LvItem.pszText="Item 1";   // Text to display (can be from a char variable) (Items)
	//SendMessage( hListView, LVM_INSERTITEM, 0, (LPARAM)&LvItem ); // Send to the Listview

	//for( int i = 1; i <= 5; i++ ) // Add SubItems in a loop
	//{
	//	LvItem.iSubItem=i;
	//	sprintf( Temp, "SubItem %d", i );
	//	LvItem.pszText=Temp;
	//	SendMessage( hListView, LVM_SETITEM, 0, (LPARAM)&LvItem ); // Enter text to SubItems
	//}
	






	/* Sends WM_SHOWWINDOW, WM_WINDOWPOSCHANGING, WM_ACTIVATEAPP, WM_NCACTIVATE, */
	/* Sends WM_GETTEXT, WM_ACTIVATE,  WM_SETFOCUS, WM_NCPAINT, WM_ERASEBKGND, */
	/* WM_WINDOWPOSCHANGED, WM_SIZE, WM_MOVE */
	ShowWindow( hwnd, SW_NORMAL ); 
    UpdateWindow( hwnd );  /* Sends a WM_PAINT message */

	while ( GetMessage( &messages, NULL, 0, 0 ) )
	{
		TranslateMessage( &messages );
        DispatchMessage( &messages );
	}

	// Windows api - DestroyWindow - Destroys the specified window. The function sends WM_DESTROY and WM_NCDESTROY messages to the window to deactivate 
    //                               it and remove the keyboard focus from it. The function also destroys the window's menu, flushes the thread message queue, 
    //                               destroys timers, removes clipboard ownership, and breaks the clipboard viewer chain (if the window is at the top of the viewer chain).
    // 1st arg - A handle to the window to be destroyed. 
    // return type - If the function succeeds, the return value is nonzero.
	//DestroyWindow(hwnd);
}


// --- WindowProc2 ---
// Return type - The return value is the result of the message processing and depends on the message sent.
// Callback - _stdcall
// 1st arg - A handle to the window. 
// 2nd arg - The message -> http://msdn.microsoft.com/en-us/library/ms644927(v=VS.85).aspx#system_defined
// 3rd arg - Additional message-specific information. The contents depend on the value of uMsg.
// 4th arg - Additional message-specific information. The contencts depend on the value of uMsg.
LRESULT CALLBACK WindowProc2( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//HWND hButton, hCombo, hEdit, hList, hScroll, hStatic,  hListView ;
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;

	switch( uMsg )
    {
		case WM_CREATE:

			hListView = CreateListView( hwnd );

            hButton = CreateWindowEx(
                    NULL,
                    "Button",
                    "Button Example",
                    WS_BORDER | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    10, 10,
                    150, 30,
                    hwnd, NULL,
                    NULL,
                    NULL);

            hCombo  = CreateWindowEx(
                    NULL,
                    "ComboBox",
                    "darkblue",
                    WS_BORDER | WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
                    10, 40,
                    150, 100,
                    hwnd, NULL,
                    NULL,
                    NULL);

            hEdit   = CreateWindowEx(
                    NULL,
                    "Edit",
                    "edit box example",
                    WS_BORDER | WS_CHILD | WS_VISIBLE,
                    10, 70,
                    150, 30,
                    hwnd, NULL,
                    NULL,
                    NULL);

            //hList   = CreateWindowEx(
            //        NULL,
            //        "ListBox",
            //        "db db db",
            //        WS_BORDER | WS_CHILD | WS_VISIBLE,
            //        200, 10,
            //        200, 200,
            //        hwnd, NULL,
            //        NULL,
            //        NULL);

			//    // Create the list-view window in report view with label editing enabled.
			//hListView = CreateWindowEx( NULL, 
   //                                     "OLW",
			//							"",
   //                                     WS_CHILD | WS_VISIBLE,
   //                                     520,  10,
   //                                     200,  200,
   //                                     hwnd, NULL,
   //                                     NULL,
   //                                     NULL ); 

            //hScroll = CreateWindowEx(
            //        NULL,
            //        "ScrollBar",
            //        "",
            //        WS_BORDER | WS_CHILD | WS_VISIBLE | SBS_VERT,
            //        410, 10,
            //        100, 200,
            //        hwnd, NULL,
            //        NULL,
            //        NULL);

            //hStatic = CreateWindowEx(
            //        NULL,
            //        "Static",
            //        "",
            //        WS_BORDER | WS_CHILD | WS_VISIBLE | SS_BLACKRECT,
            //        10, 100,
            //        100, 30,
            //        hwnd, NULL,
            //        NULL,
            //        NULL);
            break;

		case WM_NOTIFY:
			return( NotifyHandler( hwnd, uMsg, wParam, lParam ) );
			break;

		case WM_PAINT:
			hdc = BeginPaint ( hwnd, &ps );
			GetClientRect ( hwnd, &rect );
			DrawText ( hdc, TEXT ("Hello, Noobcake!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
			EndPaint ( hwnd, &ps );
			return 0;

		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;

		default:
			// Calls the default window procedure to provide default processing for any window messages that an application does not process.
			// Ensures that every message is processed.
			return DefWindowProc( hwnd, uMsg, wParam, lParam );
    }

	return 0;
}// --- End of WindowProc2 ---


// structures
typedef struct tagHOUSEINFO
{
    char szAddress[64];
    char szCity[16];
    int iPrice;
    int iBeds;
    int iBaths;
} HOUSEINFO;

HOUSEINFO rgHouseInfo[] = 
{
	{"100 Main Street", "Redmond", 175000, 3, 2 },
	{"523 Pine Lake Road", "Redmond", 125000, 4, 2},
	{"1212 112th Place SE", "Redmond",200000, 4, 3},
	{"22 Lake Washington Blvd", "Bellevue", 2500000, 4, 4},
	{"33542 116th Ave. NE", "Bellevue", 180000, 3, 2},
	{"64134 Nicholas Lane", "Bellevue", 250000, 4, 3},
	{"33 Queen Anne Hill", "Seattle", 350000, 3, 2},
	{"555 SE Fifth St", "Seattle", 140000, 3, 2},
	{"446 Mariners Way", "Seattle", 225000, 4, 3}
};




/****************************************************************************
* 
*    FUNCTION: CreateListView(HWND)
*
*    PURPOSE:  Creates the list view window and initializes it
*
****************************************************************************/
HWND CreateListView (HWND hWndParent)                                     
{
	HWND hWndList;				// Handle to the list view window	
	int index;					// Index used in for loops	
	LV_COLUMN lvC;				// List View Column structure	
	LV_ITEM lvI;				// List view item structure
	char szText[MAX_PATH];		// Place to store some text
	int iSubItem;				// Index for inserting sub items
	
	// ========================================================================================================================================================
	// ====================== LIST VIEW WINDOW CONTROL CREATION ===============================================================================================
	// ========================================================================================================================================================

	// Ensure that the common control DLL is loaded.
	InitCommonControls();

	// Create the list view window that starts out in report view and allows label editing.
	hWndList = CreateWindowEx(  0L,
								WC_LISTVIEW,                // list view class
								"",                         // no default text
								WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_EDITLABELS | WS_EX_CLIENTEDGE,
								200, 10,
								650, 200,
								hWndParent,
								NULL,
								NULL,
								NULL );

	if (hWndList == NULL )
		return NULL;

	// ========================================================================================================================================================
	// ====================== END OF LIST VIEW WINDOW CONTROL CREATION ========================================================================================
	// ========================================================================================================================================================

	// ========================================================================================================================================================
	// ====================== HEADER TITLES OF EACH COLUMN ====================================================================================================
	// ========================================================================================================================================================

	// LV_COLUMN
	// Now we initialize the columns by Initializing the LV_COLUMN structure
	// the mask specifies the format, width, text, and subitem members of the structure are valid
	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvC.fmt = LVCFMT_LEFT;  // left align the column
	lvC.cx = 75;            // width of the column, in pixels
	lvC.pszText = szText;   // If column information is being set, this member is the address of a null-terminated string that contains the column header text.

	// Add the columns and loop through the string table inside the resource file to fill szText in lvC structure by index number
	for( index = 0; index <= 5; index++ )
	{
		// Index of subitem associated with the column.
		lvC.iSubItem = index;

		// http://msdn.microsoft.com/en-us/library/windows/desktop/ms647486(v=vs.85).aspx
		// use dll handle because it is the executable file that contains the string reference.
		LoadString( inj_moduleHandle, IDS_LOOTID + index, szText, sizeof(szText) );

		// use list view window handle
		// http://msdn.microsoft.com/en-us/library/windows/desktop/bb775032(v=vs.85).aspx
		// Inserts a new column in a list-view control
		// returns the index of the new column if successful, or -1 otherwise.
		if( ListView_InsertColumn( hWndList, index, &lvC ) == -1 )
			return NULL;
	}

	// ========================================================================================================================================================
	// ====================== END OF HEADER TITLES OF EACH COLUMN =============================================================================================
	// ========================================================================================================================================================
	
	// ========================================================================================================================================================
	// ====================== SET ITEMS IN EACH ROW ===========================================================================================================
	// ========================================================================================================================================================

	// LV_ITEM
	// http://msdn.microsoft.com/en-us/library/windows/desktop/bb774760(v=vs.85).aspx
	// Finally, let's add the actual items to the control. Fill in the LV_ITEM structure for each of the items to add to the list. 
	// The mask specifies the the .pszText, .iImage, .lParam and .state members of the LV_ITEM structure are valid.
	// LVIF_TEXT - The pszText member is valid or must be set.
	// LVIF_PARAM - The lParam member is valid or must be set.
	// LVIF_STATE - The state member is valid or must be set.
	lvI.mask = LVIF_TEXT | LVIF_PARAM;
	//lvI.state = 0;      // Indicates the item's state, state image, and overlay image. The stateMask member indicates the valid bits of this member.
	//lvI.stateMask = 0;  // Value specifying which bits of the state member will be retrieved or modified.

	for( index = 0; index < 9; index++ )
	{
		lvI.iItem = index; // Zero-based index of the item to which this structure refers.
		lvI.iSubItem = 0;  // One-based index of the subitem to which this structure refers, or zero if this structure refers to an item rather than a subitem.

		// The parent window is responsible for storing the text. The List view window will send a LVN_GETDISPINFO when it needs the text to display
		lvI.pszText = LPSTR_TEXTCALLBACK;
		lvI.cchTextMax = 64; // Number of TCHARs in the buffer pointed to by pszText, including the terminating NULL.
		lvI.lParam = (LPARAM)&rgHouseInfo[ index ];

		// http://msdn.microsoft.com/en-us/library/windows/desktop/bb775038(v=vs.85).aspx
		if( ListView_InsertItem( hWndList, &lvI ) == -1 )
			return NULL;

		//printf("After ListView_InsertItem\n\n");

		for( iSubItem = 1; iSubItem < 5; iSubItem++ )
		{
			ListView_SetItemText( hWndList,	index, iSubItem, LPSTR_TEXTCALLBACK );
		}
	}

	// ========================================================================================================================================================
	// ====================== END OF SET ITEMS IN EACH ROW ====================================================================================================
	// ========================================================================================================================================================

	return( hWndList );
}






/****************************************************************************
* 
*    FUNCTION: ListViewCompareProc(LPARAM, LPARAM, LPARAM)
*
*    PURPOSE: Callback function that sorts depending on the column click 
*
****************************************************************************/
int CALLBACK ListViewCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	HOUSEINFO *pHouse1 = (HOUSEINFO *)lParam1;
	HOUSEINFO *pHouse2 = (HOUSEINFO *)lParam2;
	LPSTR lpStr1, lpStr2;
	int iResult;


	if (pHouse1 && pHouse2)
	{
		switch( lParamSort)
		{
			case 0:     // sort by Address
				lpStr1 = pHouse1->szAddress;
				lpStr2 = pHouse2->szAddress;
				iResult = strcmpi(lpStr1, lpStr2);
				break;

			case 1:     // sort by City
				lpStr1 = pHouse1->szCity;
				lpStr2 = pHouse2->szCity;
				iResult = lstrcmpi(lpStr1, lpStr2);
				break;

			case 2:     // sort by Price
				iResult = pHouse1->iPrice - pHouse2->iPrice;
				break;

			case 3:     // sort by the number of bedrooms
				iResult = pHouse1->iBeds - pHouse2->iBeds;
				break;

			case 4:     // sort by the number of bathrooms
				iResult = pHouse1->iBaths - pHouse2->iBaths;
				break;

			default:
				iResult = 0;
				break;

		}

	}
	return(iResult);
}









/****************************************************************************
* 
*    FUNCTION: NotifyHandler(HWND, UINT, UINT, LONG)
*
*    PURPOSE: This function is the handler for the WM_NOTIFY that is 
*    sent to the parent of the list view window.
*
****************************************************************************/
LRESULT NotifyHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	printf("NotifyHandler\n\n");

	LV_DISPINFO *pLvdi = (LV_DISPINFO *)lParam;
	NM_LISTVIEW *pNm = (NM_LISTVIEW *)lParam;
	HOUSEINFO *pHouse = (HOUSEINFO *)( pLvdi->item.lParam );
	static TCHAR szText[10];

	//if( wParam != 1000 )
		//return 0L;

	//printf("pLvdi->hdr.code: %d\n\n", pLvdi->hdr.code );

	// NM_ code pLvdi->hdr.code
	switch( pLvdi->hdr.code )
	{
		case LVN_GETDISPINFO:
			// iSubItem - One-based index of the subitem to which this structure refers, 
			//            or zero if this structure refers to an item rather than a subitem.
			switch( pLvdi->item.iSubItem )
			{
				case 0:     // Address
					pLvdi->item.pszText = pHouse->szAddress;
					break;

				case 1:     // City
					pLvdi->item.pszText = pHouse->szCity;
					break;

				case 2:     // Price
					sprintf( szText, "$%u", pHouse->iPrice );
					pLvdi->item.pszText = szText;
					break;

				case 3:     // Number of bedrooms
					sprintf( szText, "%u", pHouse->iBeds );
					pLvdi->item.pszText = szText;
					break;

				case 4:     // Number of bathrooms
					sprintf( szText, "%u", pHouse->iBaths );
					pLvdi->item.pszText = szText;
					break;

				default:
					break;
			}
			break;

        case LVN_BEGINLABELEDIT:
            {
                HWND hWndEdit;
                // Get the handle to the edit box.
                hWndEdit = (HWND)SendMessage( hWnd, LVM_GETEDITCONTROL, 0, 0 );
                // Limit the amount of text that can be entered.
                SendMessage( hWndEdit, EM_SETLIMITTEXT, (WPARAM)20, 0 );
            }
            break;

        case LVN_ENDLABELEDIT:
            // Save the new label information
            if( (pLvdi->item.iItem != -1) && (pLvdi->item.pszText != NULL) )
			    lstrcpy( pHouse->szAddress, pLvdi->item.pszText );
            break;

		case LVN_COLUMNCLICK:
			// The user clicked on one of the column headings - sort by this column.
			ListView_SortItems( pNm->hdr.hwndFrom, ListViewCompareProc, (LPARAM)( pNm->iSubItem ) );
			break;

		default:
			printf("Inside Default: pLvdi->hdr.code\n\n");
			break;
	}
	return 0L;
}





























// --- WindowProc ---
// Return type - The return value is the result of the message processing and depends on the message sent.
// Callback - _stdcall
// 1st arg - A handle to the window. 
// 2nd arg - The message -> http://msdn.microsoft.com/en-us/library/ms644927(v=VS.85).aspx#system_defined
// 3rd arg - Additional message-specific information. The contents depend on the value of uMsg.
// 4th arg - Additional message-specific information. The contencts depend on the value of uMsg.
LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// Calls the default window procedure to provide default processing for any window messages that an application does not process.
	// Ensures that every message is processed.
	return DefWindowProc( hwnd, uMsg, wParam, lParam );
}// --- End of WindowProc ---




// --- DeviceSetup ---
HRESULT DeviceSetup( HWND hwnd, DWORD* Vtab )
{

	//printf("\n\nInside DeviceSetup\n");
	// Create a Direct3D object. This is a COM interface of type IDirect3D9.
    // Directx api - Direct3DCreate9 - Create an IDirect3D9 object and return an interface to it.
    // 1st arg - The value of this parameter should be D3D_SDK_VERSION.
    // return type - If successful, this function returns a pointer to an IDirect3D9 interface; otherwise, a NULL pointer is returned.
	LPDIRECT3D9 pD3D = NULL;
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION )))
      return E_FAIL;

    // --- Presentation Parameters ---
    // Create an object of the structure and then zero the memory. 
    // ZeroMemory Macro - Fills a block of memory with zeros.
    // 3rd member - The back buffer format. For more information about formats, see D3DFORMAT. This value must be one of the render-target formats as validated 
    //              by CheckDeviceType. You can use GetDisplayMode to obtain the current format. In fact, D3DFMT_UNKNOWN can be specified for the BackBufferFormat 
    //              while in windowed mode. This tells the runtime to use the current display-mode format and eliminates the need to call GetDisplayMode.
    // 7th member - Member of the D3DSWAPEFFECT enumerated type structure. The runtime will guarantee the implied semantics concerning buffer swap behavior; 
    //              therefore, if Windowed is TRUE and SwapEffect is set to D3DSWAPEFFECT_FLIP, the runtime will create one extra back buffer and 
    //              copy whichever becomes the front buffer at presentation time. D3DSWAPEFFECT_DISCARD will be enforced in the debug runtime by filling any buffer with noise after it is presented.
    //        D3DSWAPEFFECT Structure - Defines swap effects.
    //              When a swap chain is created with a swap effect of D3DSWAPEFFECT_FLIP 
    //              or D3DSWAPEFFECT_COPY, the runtime will guarantee that an IDirect3DDevice9::Present operation will not affect the content of any of 
    //              the back buffers. Unfortunately, meeting this guarantee can involve substantial video memory or processing overheads, especially when 
    //              implementing flip semantics for a windowed swap chain or copy semantics for a full-screen swap chain. An application may use the 
    //              D3DSWAPEFFECT_DISCARD swap effect to avoid these overheads and to enable the display driver to select the most efficient presentation 
    //              technique for the swap chain. This is also the only swap effect that may be used when specifying a value other than D3DMULTISAMPLE_NONE 
    //              for the MultiSampleType member of D3DPRESENT_PARAMETERS. 
    // 9th member - Windowed - TRUE if the application runs windowed; FALSE if the application runs full-screen. 
	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // just sets it to zero
	d3dpp.SwapEffect       = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed         = TRUE;
	
    // Create the most important object: the Direct3D device. This is a COM interface of type IDirect3DDevice9 and it’s essential if you expect
    // to actually draw anything on the screen. It represents a computer’s graphics card or some other graphical hardware.
	LPDIRECT3DDEVICE9 pd3dDevice;

    // Directx api - CreateDevice - Creates a device to represent the display adapter.
    // 1st arg - Ordinal number that denotes the display adapter. D3DADAPTER_DEFAULT is always the primary display adapter. 
    // 2nd arg - Member of the D3DDEVTYPE enumerated type that denotes the desired device type. If the desired device type is not available, the method will fail. 
    // 3rd arg - The focus window alerts Direct3D when an application switches from foreground mode to background mode. 
    // 4th arg - Combination of one or more options that control device creation. For more information, see D3DCREATE.
    // 5th arg - Pointer to a D3DPRESENT_PARAMETERS structure, describing the presentation parameters for the device to be created.
    // 6th arg - Address of a pointer to the returned IDirect3DDevice9 interface, which represents the created device.
    // return - If the method succeeds, the return value is D3D_OK. If the method fails, the return value can be one of the following: D3DERR_DEVICELOST, D3DERR_INVALIDCALL, D3DERR_NOTAVAILABLE, D3DERR_OUTOFVIDEOMEMORY
    if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pd3dDevice )))
    {
       return D3DERR_INVALIDCALL;
    }

	//printf("\n\nDevice Created");

	// Put the d3dDevice's pointer as a dword address into pVTable
	DWORD* pVTable = (DWORD*)pd3dDevice;

    // Put the dword address into pVTable for referencing these 118 direct3d devices functions. 
	pVTable = (DWORD*)pVTable[0];

	//printf("pVtable: %X\n", pVTable);

	//printf("VTable[17]: %X\n", pVTable[17] );

    // These values are on the stack as a copy of what is inside the d3d9.h as a virtual function table.
    // I'm only putting reference to what and where I'm referencing this virtual table. Mostly where I'm getting 0, 42, 82, and 100 from.
    // This is our DWORD VTable[ 3 ] = { 0 }; from the Mythread function we saved for later and passed to DX_Init. Now we will fill it.
	Vtab[PR]    = pVTable[17]; // Address into PRE from Vtable. Later used in DetourFunction. Ref: Present inside d3d9.h line 727
	Vtab[ES]	= pVTable[42]; // Address into ES from Vtable. Later used in DetourFunction. Ref: End Scene inside d3d9.h line 752 
	Vtab[DIP]	= pVTable[82]; // Address into DIP from Vtable. Later used in DetourFunction. Ref: DrawIndexedPrimitive inside d3d9.h line 792
	Vtab[SSS]	= pVTable[100]; // Address into SSS from Vtable. Later used in DetourFunction. Ref: SetStreamSource inside d3d9.h line 810
	
	//printf("\n\nFunctions Inside pVTable");

	return S_OK;
}// --- End of DeviceSetup ---
#pragma endregion 


// --- HOOK END SCENE ---
HRESULT WINAPI hkEndScene( LPDIRECT3DDEVICE9 pDevice )
{	
	__asm nop	
	//if( !g_pLine )
 //       D3DXCreateLine( pDevice, &g_pLine );

	//if( !g_Font )
	//	D3DXCreateFontIndirect( pDevice, &FontDesc, &g_Font );

	//======== DRAW VERSION NUMBER OF HACK ========================
	//FontPosition.top = 0;
	//FontPosition.left = 0;
	//FontPosition.right = refdef->Width;
	//FontPosition.bottom = refdef->Height;
	//      
	//g_Font->DrawText( NULL,
	//				  "text",
	//				  -1,
	//				  &FontPosition,
	//				  DT_CENTER,
	//				  0xffffffff ); 
	//=============================================================
	
	return oEndScene( pDevice );

} // END HOOK END SCENE


#pragma region otherhooks

//CArmaMain* g_pArmaMain  = (CArmaMain*)0xDE4EE8;


D3DXVECTOR3 vecToD3DVec( Vector vec )
{
	D3DXVECTOR3 temp;

	temp.x = vec.x;
	temp.y = vec.y;
	temp.z = vec.z;

	return temp;
}





D3DXVECTOR3 checkLocalPlayerCoordinatesPointers()
{
	D3DXVECTOR3 tempVec1;
		
	if( checkPointer( g_pArmaMain ) ) 
	{			
		if( checkPointer( g_pArmaMain->BaseObjectWorld ) )
		{
			if( checkPointer( g_pArmaMain->BaseObjectWorld->LocalPlayerLevel0Ptr ) && 
				checkPointer( g_pArmaMain->BaseObjectWorld->LocalPlayerLevel0Ptr->LocalPlayerLevel1Ptr ) && 
				checkPointer( g_pArmaMain->BaseObjectWorld->LocalPlayerLevel0Ptr->LocalPlayerLevel1Ptr->LocalPlayerCoordinatesPtr ) &&
				( !( g_pArmaMain->BaseObjectWorld->LocalPlayerLevel0Ptr->LocalPlayerLevel1Ptr->LocalPlayerCoordinatesPtr->LocalPos.x <= 0 ) && !( g_pArmaMain->BaseObjectWorld->LocalPlayerLevel0Ptr->LocalPlayerLevel1Ptr->LocalPlayerCoordinatesPtr->LocalPos.x > 15360 ) ) &&
				( !( g_pArmaMain->BaseObjectWorld->LocalPlayerLevel0Ptr->LocalPlayerLevel1Ptr->LocalPlayerCoordinatesPtr->LocalPos.y <= 0 ) && !( g_pArmaMain->BaseObjectWorld->LocalPlayerLevel0Ptr->LocalPlayerLevel1Ptr->LocalPlayerCoordinatesPtr->LocalPos.y > 15360 ) ) && 
				( !( g_pArmaMain->BaseObjectWorld->LocalPlayerLevel0Ptr->LocalPlayerLevel1Ptr->LocalPlayerCoordinatesPtr->LocalPos.z <= 0 ) && !( g_pArmaMain->BaseObjectWorld->LocalPlayerLevel0Ptr->LocalPlayerLevel1Ptr->LocalPlayerCoordinatesPtr->LocalPos.z > 15360 ) ) )
			{
				tempVec1 = g_pArmaMain->BaseObjectWorld->LocalPlayerLevel0Ptr->LocalPlayerLevel1Ptr->LocalPlayerCoordinatesPtr->LocalPos;
			}
		}
	}

	return tempVec1;
}

D3DXVECTOR3 checkEntityCoordinatesPointers( int i )
{
	D3DXVECTOR3 tempVec2;

	// Check element & N205AABBE together first
	// Check ( element & N205AABBE ) answer with EntityPtr
	if( ( ( checkPointer( g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element ) && !( g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element->N205AABBE <= 0x0 ) )  &&
		    checkPointer( g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element->EntityPtr ) )                    &&

		(   checkPointer( g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element->EntityPtr->CoordinatePtr )       &&

	    ( ( !( g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element->EntityPtr->CoordinatePtr->pos.x <= 0 )      && 
		    !( g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element->EntityPtr->CoordinatePtr->pos.x > 15360 ) ) &&

		  ( !( g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element->EntityPtr->CoordinatePtr->pos.y <= 0 )      && 
		    !( g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element->EntityPtr->CoordinatePtr->pos.y > 15360 ) ) &&

		  ( !( g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element->EntityPtr->CoordinatePtr->pos.z <= 0 )      && 
		    !( g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element->EntityPtr->CoordinatePtr->pos.z > 15360 ) ) ) )
	)
	{
		//printf( "\nObjectTableArrayElement[%d]: %X", i, g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i]);
		//printf( "\nelement: %X", g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element );
		//printf( "\nEntityPtr: %X", g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element->EntityPtr );
		//printf( "\nCoordinatePtr: %X", g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element->EntityPtr->CoordinatePtr ); 
		tempVec2 = g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element->EntityPtr->CoordinatePtr->pos;
	}

	return tempVec2;
}

//#define EPSILON    (1E-5)
//bool pointsAreSame( double a, double b )
//{
//	return fabs( a - b ) < EPSILON;
//}





inline bool isEqual( double x, double y )
{
  const double epsilon = 1e-5/* some small number such as 1e-5 */;
  return std::abs(x - y) <= epsilon * std::abs(x);
  // see Knuth section 4.2.2 pages 217-218
}


//void versionCheck()
//{
//	BaseObjectWorld* pVersionObject;	
//	pVersionObject = g_pArmaMain->BaseObjectWorld;
//
//	pVersionObject->Level0PointerToVersionNumber->Level1PointerToVersionNumber->VersionNumber
//
//
//
//}






//#define VERYSMALL  (1.0E-150)
//#define EPSILON    (1.0E-8)
//bool AreSame(double a, double b)
//{
//    double absDiff = fabs(a - b);
//    if (absDiff < VERYSMALL)
//    {
//        return true;
//    }
//
//	double maxAbs  = fmax( fabs(a) - fabs(b) );
//    return (absDiff/maxAbs) < EPSILON;
//}

bool checkPointer( void* arg )
{
	if( arg != 0 && arg != (void*)0xFFFFFFF && arg != (void*)0x0fffffff && !( arg <= (void*)0xf ) )
		return true;

	return false;
}

void PlayerCoordinates()
{	
	char coordinates[500];
	char coordinates1[500];
	char* name;
	char* soldier;
	D3DXVECTOR3	vecPosition;
	D3DXVECTOR3 vecPosition1;
	D3DXVECTOR3 drawScreenPos;
	D3DXVECTOR3 worldScreenPos;
	D3DXVECTOR3 localCoordinates2;
	int ycoord = 0;
	float dx, dy, dz;
	float dist;
	bool twoDOnScreenCoords;	

	//printf("\n\nBefore g_pArmaMain");
	if( checkPointer( g_pArmaMain ) ) 
	{
		//printf("\n\nBefore BaseObjectWorld");
		if( checkPointer( g_pArmaMain->BaseObjectWorld ) )
		{
			//printf("\n\nBefore ObjectTablePtr");
			if( checkPointer( g_pArmaMain->BaseObjectWorld->ObjectTablePtr ) )
			{
				//printf("\n\nBefore ObjectTableArrayPtr");
				if( checkPointer( g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr ) )
				{					
					//printf("\n\nBefore Check ObjectTableSize");				

					if( !( g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableSize <= 0 ) )
					{
						printf("\n\nObjectTableSize Player/Animal/Zombie ArraySize: %d", g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableSize );

						for( int i = 0; i < g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableSize; i++ )
						{
							if( !( g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].CheckEntity <= 1 ) )
							{	
								if( checkPointer( g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element ) && checkPointer( g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element->EntityPtr ) )
								{
									if( checkPointer( g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element->EntityPtr->EntityTextureAndNamePtr ) && checkPointer( g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element->EntityPtr->CoordinatePtr ) )
									{
										if( checkPointer( g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element->EntityPtr->EntityTextureAndNamePtr->EntNamePtr ) ) 
										{							
									  
											name = g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr->ObjectTableArrayElement[i].element->EntityPtr->EntityTextureAndNamePtr->EntNamePtr->cEntName;														
										    						
											localCoordinates2 = checkLocalPlayerCoordinatesPointers();								

											//printf("\n\nObjectTableSize: %d", g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableSize );
											//printf( "\ng_pArmaMain: %X", g_pArmaMain );
											//printf( "\nBaseObjectWorld: %X", g_pArmaMain->BaseObjectWorld );	
											//printf( "\nObjectTablePtr - 0x0744: %X", g_pArmaMain->BaseObjectWorld->ObjectTablePtr );							
											//printf( "\nObjectTableArrayPtr - 0x000C %X", g_pArmaMain->BaseObjectWorld->ObjectTablePtr->ObjectTableArrayPtr );
								
											vecPosition = checkEntityCoordinatesPointers( i );													

											dx = ( localCoordinates2.x - vecPosition.x );
											dy = ( localCoordinates2.y - vecPosition.y );
											dz = ( localCoordinates2.z - vecPosition.z );
										
											dist = sqrt( (dx*dx) + (dy*dy) + (dz*dz) );
										
											drawScreenPos = g_pTransformations->W2SN( vecPosition );
											if( drawScreenPos.x == 1 )
											{								
												do
												{
													drawScreenPos =  g_pTransformations->W2SN( vecPosition );
												}
												while( drawScreenPos.x != 1 );
											}

											if( drawScreenPos.z > 0.01 )
											{									
												sprintf( coordinates1, "[%-1.1s][%7.3fm]", name, dist );
												pCanvas->Text( coordinates1, drawScreenPos.x, drawScreenPos.y, D3DCOLOR_RGBA(255, 255, 0, 0) );
											}

											//printf("\n\n\n\ng_pTransformations Address [%X]", g_pTransformations);
											//printf("\nScreenMatrixObjectPtr Address [%X]", g_pTransformations->ScreenMatrixObjectPtr);
											//printf("\nScreenMatrix Address[%X]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix);
											//				
											//printf("\n\nInViewRight (Displayed As Bytes) [%X]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->InViewRight);
											//printf("\nInViewRight.x [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->InViewRight.x);
											//printf("\nInViewRight.y [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->InViewRight.y);
											//printf("\nInViewRight.z [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->InViewRight.z);

											//printf("\n\nInViewUp (Displayed As Bytes) [%X]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->InViewUp);
											//printf("\nInViewUp.x [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->InViewUp.x);
											//printf("\nInViewUp.y [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->InViewUp.y);
											//printf("\nInViewUp.z [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->InViewUp.z);

											//printf("\n\nInViewForward (Displayed As Bytes) [%X]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->InViewForward);
											//printf("\nInViewForward.x [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->InViewForward.x);
											//printf("\nInViewForward.y [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->InViewForward.y);
											//printf("\nInViewForward.z [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->InViewForward.z);

											//printf("\n\nInViewTranslation (Displayed As Bytes) [%X]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->InViewTranslation);
											//printf("\nInViewTranslation.x [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->InViewTranslation.x);
											//printf("\nInViewTranslation.y [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->InViewTranslation.y);
											//printf("\nInViewTranslation.z [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->InViewTranslation.z);

											//printf("\n\nViewPortMatrix (Displayed As Bytes) [%X]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->ViewPortMatrix);
											//printf("\nViewPortMatrix.x [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->ViewPortMatrix.x);
											//printf("\nViewPortMatrix.y [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->ViewPortMatrix.y);
											//printf("\nViewPortMatrix.z [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->ViewPortMatrix.z);	
											//															
											//printf("\n\nProjD1 (Displayed As Bytes) [%X]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->ProjD1);
											//printf("\nProjD1.x [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->ProjD1.x);
											//printf("\nProjD1.y [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->ProjD1.y);
											//printf("\nProjD1.z [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->ProjD1.z);

											//printf("\n\nProjD2 (Displayed As Bytes) [%X]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->ProjD2);
											//printf("\nProjD2.x [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->ProjD2.x);
											//printf("\nProjD2.y [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->ProjD2.y);
											//printf("\nProjD2.z [%f]", g_pTransformations->ScreenMatrixObjectPtr->ScreenMatrix->ProjD2.z);
													
											// ------------------------------------------------------------------------------------------------------------------
											// ------------- DISPLAYING 2D COORDINATES FOR PLAYERS ON MAP -------------------------------------------------------
											// ------------------------------------------------------------------------------------------------------------------
											//printf("\n\nEntity x: %7.3f\n", vecPosition.x);
											//printf("Local x: %7.3f\n", localCoordinates2.x);

											if( isEqual( vecPosition.x, localCoordinates2.x ) )
											{													

											}
											else
											{
												vecPosition.z = (float)15360 - vecPosition.z;									
												vecPosition.y = vecPosition.y / 100;
												vecPosition.x = vecPosition.x / 100;
												vecPosition.z = vecPosition.z / 100;											
													
												if( strcmp( "soldier", name ) == 0 )
												{			
													sprintf( coordinates, "[%-1.1s]X[%7.3f]Y[%7.3f]", name, vecPosition.x, vecPosition.z );	

													//Red is yellow	//Green is purple //blue is red											
													if( i == 0 )
													{
														ycoord = 10;
													}
													else
													{
														ycoord = ycoord + 18;
													}

													pCanvas->Text( coordinates, 10, ycoord, D3DCOLOR_RGBA( 255, 255, 0, 0 ) );														
												}													
											}
											// ------------------------------------------------------------------------------------------------------------------
											// ------------- END OF DISPLAYING 2D COORDINATES FOR PLAYERS ON MAP ------------------------------------------------
											// ------------------------------------------------------------------------------------------------------------------
													
												
											
										}
									}
								}
							}
						}// End of Check ObjectTableSize Loop
					}// End of Check ObjectTableSize				

					//if(g_pTransformations)
					//{
					//	if(g_pTransformations->m_pTransformationData)
					//	{
					//		DrawESP();
					//	}
					//}
					//drawPlayerOnRadar(vecPosition.x, vecPosition.z, 2);
				}
			}
		}// End of World Check
	}// End of Arma Check
}

void localPlayerCoordinate()
{
	char localname[256] = "LocalPlayer";				
	char localbuff[500];
	int yLocalCoord = 0;
	D3DXVECTOR3 localCoordinates;

	localCoordinates = checkLocalPlayerCoordinatesPointers();

	//printf( "\n\nLocalPosition: %f, %f, %f", localCoordinates.x, localCoordinates.y, localCoordinates.z );
					
	localCoordinates.z = (float)15360 - localCoordinates.z;					

	localCoordinates.y = localCoordinates.y / 100;
	localCoordinates.x = localCoordinates.x / 100;
	localCoordinates.z = localCoordinates.z / 100;
					
	sprintf( localbuff, "[%-11.11s], X:[%7.3f], Y:[%7.3f]", localname, localCoordinates.x, localCoordinates.z );
								
	//Red is yellow	//Green is purple //blue is red
	yLocalCoord = 10;					
	pCanvas->Text( localbuff, 950, yLocalCoord, D3DCOLOR_RGBA(255, 255, 0, 0) );
}


vector<char*> g_vLootNames;

void nearLoot()
{
	int   lootIdent, lengthOfLootName;	
	char* lootname;
	char  lootbuff[ 500 ];
	char  nearLootcoords[ 500 ];
	D3DXVECTOR3 lootCoordinates;
	D3DXVECTOR3 localCoordinates3;
	D3DXVECTOR3 drawScreenPos;
	int   yLootCoord = 0;
	float dx, dy, dz;
	float dist;
	bool  twoDOnScreenLootCoords = false;

	//printf("\n\nBefore ArmaMain");
	if( checkPointer( g_pArmaMain ) ) 
	{		
		//printf("\n\nBefore BaseObjectWorld");
		if( checkPointer( g_pArmaMain->BaseObjectWorld ) )
		{
			//printf("\n\nBefore NearLootTablePtr");
			if( checkPointer( g_pArmaMain->BaseObjectWorld->NearLootTablePtr ) )
			{
					//if( g_pArmaMain->BaseObjectWorld->SizeOfNearLootTable == 0 )
					//{
					//	if( g_vLootNames[0] != 0 )
					//		//fill( *g_vLootNames.begin(), *g_vLootNames.end(), NULL );
					//}

				//printf("\n\nBefore SizeOfNearLootTable Check");
				if( !( g_pArmaMain->BaseObjectWorld->SizeOfNearLootTable <= 0 ) )
				{
					printf("\n\nSizeOfNeatLootTable: %d", g_pArmaMain->BaseObjectWorld->SizeOfNearLootTable );

					//printf("\n\nBefore NearLootTable Loop");
					for( int k = 0; k < g_pArmaMain->BaseObjectWorld->SizeOfNearLootTable; k++ )
					{						
						if(	checkPointer( g_pArmaMain->BaseObjectWorld->NearLootTablePtr->NearLootTableArray[k].ptrLootElement ) &&
							checkPointer( g_pArmaMain->BaseObjectWorld->NearLootTablePtr->NearLootTableArray[k].ptrLootElement->LootNameAndOperationLevel0Ptr ) &&
							checkPointer( g_pArmaMain->BaseObjectWorld->NearLootTablePtr->NearLootTableArray[k].ptrLootElement->LootNameAndOperationLevel0Ptr->LootNameAndOperationLevel1Ptr ) &&
							checkPointer( g_pArmaMain->BaseObjectWorld->NearLootTablePtr->NearLootTableArray[k].ptrLootElement->LootNameAndOperationLevel0Ptr->LootNameAndOperationLevel1Ptr->LootNameAndOperationPtr ) )
							{
								lengthOfLootName = g_pArmaMain->BaseObjectWorld->NearLootTablePtr->NearLootTableArray[k].ptrLootElement->LootNameAndOperationLevel0Ptr->LootNameAndOperationLevel1Ptr->LootNameAndOperationPtr->LengthOfLootName;
								lootname = g_pArmaMain->BaseObjectWorld->NearLootTablePtr->NearLootTableArray[k].ptrLootElement->LootNameAndOperationLevel0Ptr->LootNameAndOperationLevel1Ptr->LootNameAndOperationPtr->LootName;
								lootIdent = g_pArmaMain->BaseObjectWorld->NearLootTablePtr->NearLootTableArray[k].ptrLootElement->LootNameAndOperationLevel0Ptr->LootNameAndOperationLevel1Ptr->LootNameAndOperationPtr->Identifier;
								
								//g_vLootNames.push_back( g_pArmaMain->BaseObjectWorld->NearLootTablePtr->NearLootTableArray[k].ptrLootElement->LootNameAndOperationLevel0Ptr->LootNameAndOperationLevel1Ptr->LootNameAndOperationPtr->LootName );
								
								if( checkPointer( g_pArmaMain->BaseObjectWorld->NearLootTablePtr->NearLootTableArray[k].ptrLootElement->LootCoordinatePtr ) &&
									( !( g_pArmaMain->BaseObjectWorld->NearLootTablePtr->NearLootTableArray[k].ptrLootElement->LootCoordinatePtr->lootpos.x <= 0 ) && !( g_pArmaMain->BaseObjectWorld->NearLootTablePtr->NearLootTableArray[k].ptrLootElement->LootCoordinatePtr->lootpos.x > 15360 ) ) &&
									( !( g_pArmaMain->BaseObjectWorld->NearLootTablePtr->NearLootTableArray[k].ptrLootElement->LootCoordinatePtr->lootpos.y <= 0 ) && !( g_pArmaMain->BaseObjectWorld->NearLootTablePtr->NearLootTableArray[k].ptrLootElement->LootCoordinatePtr->lootpos.y > 15360 ) ) &&
									( !( g_pArmaMain->BaseObjectWorld->NearLootTablePtr->NearLootTableArray[k].ptrLootElement->LootCoordinatePtr->lootpos.z <= 0 ) && !( g_pArmaMain->BaseObjectWorld->NearLootTablePtr->NearLootTableArray[k].ptrLootElement->LootCoordinatePtr->lootpos.z > 15360 ) ) )
								{
									lootCoordinates = g_pArmaMain->BaseObjectWorld->NearLootTablePtr->NearLootTableArray[k].ptrLootElement->LootCoordinatePtr->lootpos;
									localCoordinates3 = checkLocalPlayerCoordinatesPointers();									

									dx = ( localCoordinates3.x - lootCoordinates.x );
									dy = ( localCoordinates3.y - lootCoordinates.y );
									dz = ( localCoordinates3.z - lootCoordinates.z );
										
									dist = sqrt( (dx*dx) + (dy*dy) + (dz*dz) );
										
									drawScreenPos = g_pTransformations->W2SN( lootCoordinates );
									if( drawScreenPos.x == 1 )
									{								
										do
										{
											drawScreenPos =  g_pTransformations->W2SN( lootCoordinates );
										}
										while( drawScreenPos.x != 1 );
									}

									if( drawScreenPos.z > 0.01 )
									{									
										sprintf( nearLootcoords, "[%7.3fm][%-26.26s]", dist, lootname );
										pCanvas->Text( nearLootcoords, drawScreenPos.x, drawScreenPos.y, D3DCOLOR_ARGB(255, 255, 0, 0) );
									}
									
									//================================================================================================
									
									//if( key_pressed( VK_F10 ) )
									//{										
									//	twoDOnScreenLootCoords = !twoDOnScreenLootCoords; // if false set to true
									//}
		
									//if( twoDOnScreenLootCoords )
									//{									
									//	lootCoordinates.z = (float)15360 - lootCoordinates.z;
									//	
									//	lootCoordinates.y = lootCoordinates.y / 100;
									//	lootCoordinates.x = lootCoordinates.x / 100;
									//	lootCoordinates.z = lootCoordinates.z / 100;
									//	
									//	sprintf( lootbuff, "[%-26.26s], X:[%7.3f], Y:[%7.3f]", lootname, lootCoordinates.x, lootCoordinates.z );
								
									//	//Red is yellow	//Green is purple //blue is red									
									//	if( k == 0 )
									//	{
									//		yLootCoord = 10;
									//	}
									//	else
									//	{
									//		yLootCoord = yLootCoord + 18;
									//	}
									//	pCanvas->Text( lootbuff, 400, yLootCoord, D3DCOLOR_ARGB(255, 255, 215, 0) );
									//}

									//if( key_pressed( VK_F11 ) )
									//{
									//	twoDOnScreenLootCoords = false;
									//}
								}							
							}						
						
					}// Loop SizeOfNearLootTable
					
					//// run and display vector to command line
					//for( int index = 0; index < (int)g_vLootNames.size(); ++index )
					//	cout << "Element " << index << ": " << g_vLootNames.at(index) << endl;

				}//End Check for size of near loot table						
			}// NearLootTablePtr Check			
		}// BaseObjectWorld Check
	}// g_pArmaMain Check

}// End of NearLoot


// --- HOOK PRESENT ---
HRESULT WINAPI hkPresent( LPDIRECT3DDEVICE9 pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion )
//HOOKDEF( HRESULT, WINAPI, Present,
//	       LPDIRECT3DDEVICE9 pDevice, 
//	  _In_ const RECT *pSourceRect, 
//	  _In_ const RECT *pDestRect, 
//	  _In_ HWND hDestWindowOverride, 
//	  _In_ const RGNDATA *pDirtyRegion 
//)
{	
	//bool Hack = false;
	//__asm nop

	//// Do whatever...
	if ( !pBlock ) 
		InitResources( pDevice );

	pDevice->GetViewport( &g_Viewport );
	//drawRadar(pDevice);

	pBlock->Capture();
	D3DVIEWPORT9 ViewPort;
	pDevice->GetViewport( &ViewPort );

	pCanvas->Init( pDevice );
	pCanvas->Begin();

	// Rendering code and text goes here....
	printf( "\n\nvTable (Present) Hooked " );

	//bool worldPositions;

	//======= TOGGLE PlayerAnimalZombieCoords ESP =========
	//if( GetAsyncKeyState( VK_F9 ) &1 )
	//{
	//	PlayerAnimalZombieCoords = true;
	//}
	//	
	//if( PlayerAnimalZombieCoords )
	//{
		//PlayerCoordinates();
	//}
	//=====================================================
	
	//======= TOGGLE NearLootTest ESP =====================
	//if( GetAsyncKeyState( VK_F10 ) &1 )
	//{
	//	NearLootTest = true;
	//}
	//	
	//if( NearLootTest )
	//{
		//nearLoot();
	//}
	//=====================================================

	//======= TOGGLE localPlayerCoords ESP ================
	//if( GetAsyncKeyState( VK_F11 ) &1 )
	//{
	//	localPlayerCoords = true;
	//}
	//	
	//if( localPlayerCoords )
	//{
		//localPlayerCoordinate();
	//}
	//=====================================================
	
/*	if( GetAsyncKeyState( VK_F12 ) &1 )
	{
		PlayerAnimalZombieCoords = false;
		localPlayerCoords = false;
		NearLootTest = false;
	}	*/		
		
	//if( key_pressed( VK_F7 ) )
	//{
	//		Hack = true; // if false set to true
	//}
	//else
	//{
	//	Hack = false;
	//}

	//if( Hack )
	//{									
	localPlayerCoordinate();
	PlayerCoordinates();
	nearLoot();
	//}

	//if( key_pressed( VK_F12 ) )
	//{
	//	Hack = false;
	//}
	
	pCanvas->End();
	pBlock->Apply();

	//HRESULT ret = Old_Present( pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion );

	return oPresent( pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}
// END HOOK PRESENT 


// --- HOOK DRAW INDEXED PRIMITIVE ---
HRESULT WINAPI hkDrawIndexedPrimitive( LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount )
{
	__asm nop	
    
	// Do whatever...

	return oDrawIndexedPrimitive( pDevice, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount );

}// END HOOK DRAW INDEXED PRIMITIVE



// --- HOOK SET STREAM SOURCE ---
HRESULT WINAPI hkSetStreamSource( LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride )
{
	__asm nop
      

	return oSetStreamSource( pDevice, StreamNumber, pStreamData, OffsetInBytes, Stride );
}// END HOOK SET STREAM SOURCE 

#pragma endregion

//// --- DOT PRODUCT ---
//float dotproduct3( Vector one , Vector two ) 
//{
//	// Dot Product ( A.x * B.x ) + ( A.y * B.y ) + ( A.z * B.z )
//	// The result of the Dot Product is not a vector, it is a real number. For this reason it is sometimes refered to as the ( Scalar Product or Inner Product )
//	return ( ( one.x * two.x ) + ( one.y * two.y ) + ( one.z * two.z ) ); 
//
//}// END DOT PRODUCT	


//// --- PROJECT SCREEN ---
//bool ProjectScreen( Vector Origin , float * x, float * y )
//{
//	float forward, right, up;
//	Vector delta;
//	
//	// each characters x, y, z subtracted from my x, y, z gives my absolute location into delta
//	// ||delta|| = Origin - ViewOrigin
//	//delta = Origin - gViewOrigin;
//	// I changed this back, because the other way it seemed to lag my system hella for some reason
//	// Not sure what was going on but when I changed it back, it stopped lagging.
//
//	delta.x = Origin.x - refdef->ViewOrigin.x;
//	delta.y = Origin.y - refdef->ViewOrigin.y;
//	delta.z = Origin.z - refdef->ViewOrigin.z;
//
//	// Find out what direction the delta vector is going in.
//
//	right   = dotproduct3( delta, refdef->DirAngles[0] );	
//	forward = dotproduct3( delta, refdef->DirAngles[1] );
//	up      = dotproduct3( delta, refdef->DirAngles[2] );
//
//	// Test to see if our x-axis vector is positive or negative.
//
//	if (right < 0.1f ) 
//		return false;
//	
//	// floor rounds down the overall number
//	// Y (Forward) / FovX / X (Right)
//	// Z (Up)      / FovY / X (Right)
//	// refdef->Width * 0.5f is our center of the viewport.
//	// refdef->Width / 2 is the same thing our center of the viewport.	
//
//	*x = floor( refdef->Width  * 0.5f * ( 1.f - ( forward / refdef->FOV.x / right ))) ; 
//	*y = floor( refdef->Height * 0.5f * ( 1.f - ( up      / refdef->FOV.y / right ))) ;
//	
//	return true;
//
//}// END PROJECT SCREEN


// --- LINE ---
//void line( float x , float y , float w , float h , float * pColor  )
//{
//	float pThird = w/3;
//	float hThird = h/3;
//
//	//int white = RegisterShader("white");
//	//DrawStretchPic( x, y, w, h, 0, 0, 1.0f, 1.0f, pColor, white );
//
//}// END LINE
//
//// --- RECTANGLE EDGE ----
//void rectangleEdge( float x , float y , float w , float h , float fThick ,float * pColor )
//{
//	float wThird = w/3;
//	float hThird = h/3;	
//
//	line( x, y, w, fThick, pColor );
//
//	y += fThick;
//	line( x, y, fThick, h - ( 2 * fThick ), pColor);
//
//	y += ( h - ( 2 * fThick ) );
//	line( x, y, w, fThick, pColor );
//
//	y -= ( h - ( 2 * fThick ) );
//	x += ( w - fThick );
//	line( x, y, fThick, h - ( 2 * fThick ), pColor );
//
//}// END RECTANGLE EDGE


