// dllmain.cpp : Defines the entry point for the DLL application.

// Notes:
/* LPDIRECT3D9 - object is the big boss of the Direct3D library the object that controls everything, */
/* while LPDIRECT3DDEVICE9 represents the video card */

// Includes
#include "stdafx.h"

// Pragmas
#pragma warning(disable:4996) // disables this function is unsafe

// Macros
// To Add:

// Globals
PGLOBAL pGlobal ;
ofstream pFile ;
static DWORD64 VTable[4] = { 0 } ;
PPATCH_INFO pPatchInfo ;
D3DVIEWPORT9 g_Viewport ;
DWORD64 Fixup_CallAddress ;

// .ASM Global Prototypes
extern "C" { LPDIRECT3DDEVICE9 PDevice_Present ;  
			 void Prolog_Present( void ) ;
			 void PrologueMsg( void ) ; }

// Function Prototypes
DWORD WINAPI StormThread1( void ) ;

//D3DXVECTOR3 W2SN( D3DXVECTOR3 in );

DWORD WINAPI StormThread1( void )
{	
	// Local Variables
	int iCount = 0 ;
	//static Sync CritSec;

	Sleep( 1000 ) ; // Sleep 1 second

	Log("\n ====================================================== ") ;	
	Log("\n =============== StormThread1 SEPERATOR =============== ") ;	
	Log("\n ====================================================== ") ;	
	
	do
	{	
		if( iCount != 3 )
		{
			pGlobal->g_Game_Mod_Base   = (DWORD64)( GetModuleHandle( "H1Z1.exe" ) ) ;
			pGlobal->g_H_Game_Mod_Base = (HANDLE)pGlobal->g_Game_Mod_Base ;
			pGlobal->g_D3D9_Mod_Base   = (DWORD64)( GetModuleHandle( "d3d9.dll" ) ) ;			
			Sleep( 100 ) ; // Sleep .1 second
		}else
		{
			Log( "Global Bases Failed: %d\n", iCount ) ;
			break ; 
		}
		iCount++ ;
	}while( !pGlobal->g_Game_Mod_Base && !pGlobal->g_D3D9_Mod_Base ) ;
	//Log( "\nH1Z1.exe - Base Address: %016llX", pGlobal->g_Game_Mod_Base ) ;
	//Log( "\nH1Z1.exe - Handle: %08X", pGlobal->g_H_Game_Mod_Base ) ;
	//Log( "\ng_D3D_Mod_Base: %016llX", pGlobal->g_D3D9_Mod_Base ) ;

	pGlobal->g_Game_Mod_Size = (DWORD64)GetModuleSize( "H1Z1.exe" ) ;
	//Log( "\nH1Z1.exe - Size: %016llX\n", pGlobal->g_Game_Mod_Size ) ;

	// Setup Overlay
	OverlayWindow( VTable ) ;
		
	// Enable Debug Privileges
	EnableDebugPriv() ;

	// OpenProcess with All Access	
	pGlobal->g_PseudoHandle = GetCurrentProcess() ;	
	//Log( "PseudoHandle: %016llX\n", pGlobal->g_PseudoHandle ) ;
	pGlobal->g_AllAccess_Process_Handle = OpenProcess( PROCESS_ALL_ACCESS, FALSE, Get_PID_From_Process_Handle( pGlobal->g_PseudoHandle ) ) ;
	Log( "All Access Handle: %016llX\n", pGlobal->g_AllAccess_Process_Handle ) ;
		
	/* =========================================================================== */
	/* Present() & Gameoverlayrenderer64 HOOK Related Information */
	/* =========================================================================== */
	
	// Define & Initialize Patch Info Structure	
	pPatchInfo = (PPATCH_INFO)malloc( sizeof( PATCH_INFO ) ) ;
	memset( pPatchInfo, 0xEE, sizeof(PATCH_INFO) ) ;
	//Log( "pPatchInfo: %016llX", pPatchInfo ) ;

	// Assign The Present() addess within SystemCall
	pPatchInfo->SystemCall = (BYTE*)VTable[ PR ] ;  // This is Present() within directx 9 sdk
	//Log( "pPatchInfo->SystemCall: %016llX", pPatchInfo->SystemCall ) ;

	// Define Signature & Mask For Present() 
	pPatchInfo->SignatureSize = 0xF ;	
	memcpy( pPatchInfo->Signature, "\xE9\x00\x00\x00\x00\x58\x08\x00\x00\x68\x00\x00\x00\x00\x00", pPatchInfo->SignatureSize ) ;
	memcpy( pPatchInfo->SignatureMask, "x????xx??x?????", pPatchInfo->SignatureSize ) ;		
	//Log( "pPatchInfo->SignatureSize; %016llX", &pPatchInfo->SignatureSize ) ;
	//Log( "pPatchInfo->Signature: %016llX",     pPatchInfo->Signature ) ;
	//Log( "pPatchInfo->SignatureMask: %016llX", pPatchInfo->SignatureMask ) ;
	
	// Setup Prologue Patch 
	pPatchInfo->PrologPatchOffset = 0x0 ;
	pPatchInfo->SizePrologPatch = 0xF ;
	memcpy( pPatchInfo->PrologPatch, "\x48\xB8\xDE\xC0\xBE\xBA\xFE\xCA\xED\xFE\x50\xC3\x90\x90\x90", pPatchInfo->SizePrologPatch ) ;
	//Log( "pPatchInfo->PrologPatchOffset: %016llX", &pPatchInfo->PrologPatchOffset ) ;
	//Log( "pPatchInfo->SizePrologPatch: %016llX", &pPatchInfo->SizePrologPatch ) ;
	//Log( "pPatchInfo->PrologPatch: %016llX", pPatchInfo->PrologPatch ) ;

	// Assign Detour Functions - Prologue & Epilogue Assignment.
	pPatchInfo->PrologDetour = (BYTE*)Prolog_Present ;
	//Log( "pPatchInfo->PrologDetour %016llX", pPatchInfo->PrologDetour ) ;
	
	// Make Sure our System Call Region has the proper RWE permissions.
	DWORD old_protect ;
	if( VirtualProtect( pPatchInfo->SystemCall, pPatchInfo->SignatureSize, PAGE_EXECUTE_READWRITE, &old_protect ) )
	{
		// Verify Signature @ Function We Want To Hook
		if( VerifySignature( pPatchInfo->SignatureMask, pPatchInfo->Signature, pPatchInfo->SignatureSize, pPatchInfo->SystemCall ) )
		{
			// Get Prologues Existing Bytes From Function We Want To Hook
			memcpy( pPatchInfo->PrologOriginal, pPatchInfo->SystemCall, pPatchInfo->SignatureSize ) ;			
			//Log( "pPatchInfo->PrologOriginal: %016llX", pPatchInfo->PrologOriginal ) ;

			// Assign our patches the proper addresses to our Detour Functions.
			// Prologue
			MakePatchLegit( pPatchInfo->PrologDetour, pPatchInfo->PrologPatch ) ;
			//Log( "pPatchInfo->PrologPatch After Fixup: %016llX", pPatchInfo->PrologPatch ) ;

			// Follow Steam Hook
			FixUpSteamHook( pPatchInfo ) ;			
			
			// Prologue Hook Complete 
			InsertDetour( pPatchInfo->SystemCall, pPatchInfo->PrologPatch, pPatchInfo->SizePrologPatch, pPatchInfo->PrologPatchOffset ) ;     /* Activate Hook */
			Log( "First Hook Complete" );

			FixUpGameOverlayHook( pPatchInfo ) ;

		}else
		{
			Log( "Verify Signature Failed" ) ;
		}
	}else
	{
		Log( "First HOOK Virtual Protect Failed" ) ;
	}



	//CritSec.lock() ;

	// Prologue Hook
	//InsertDetour( pPatchInfo->SystemCall, pPatchInfo->PrologPatch, pPatchInfo->SizePrologPatch, pPatchInfo->PrologPatchOffset ) ; /* Activate Hook */
	//Log( "First Hook Complete" ) ;

	//// Call Hook
	//InsertDetour( pPatchInfo->GameOverlayPatchCall, pPatchInfo->PatchCall, 0x6, 0 ) ; /* Activate Hook */
	//Log( "Second HOOK Complete" ) ;

	//CritSec.unlock() ;
	

	//pCanvas = &Canvas ;

	/* =========================================================================== */
	/* Clean-Up Related Information */
	/* =========================================================================== */
	//free( pPatchInfo ) ;
	//pPatchInfo = NULL ;
	
	return 0 ;

} // End Of StormThread1



//================================================================================================
//========= START OF DLL MAIN ====================================================================
//================================================================================================
BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{	
	if( AllocConsole() != 0 )               /* windows.h */
	{
		freopen( "CONOUT$", "w", stdout ) ; /* #include<cstdio> */
	}
	
	pGlobal = (PGLOBAL)malloc( sizeof( GLOBAL ) ) ;
	pGlobal->g_Inj_ModuleHandle = (DWORD64)hModule ;	
	Log( "Global Handle DllMain: %016llX\n", pGlobal->g_Inj_ModuleHandle ) ;
	
	switch( ul_reason_for_call )
	{
		case DLL_PROCESS_ATTACH:			
			CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)StormThread1, 0, 0, 0 ) ;			

		case DLL_PROCESS_DETACH:
			Log( "Detached" ) ;

		default:
			break ;
	}
	
	return TRUE ;
}



void PrologueMsg( void )
{		
	//Log( "PDevice_Present: %016llX\n", PDevice_Present ) ;

	//PDevice_Present->GetViewport( &g_Viewport ) ;
		
	//pCanvas->Init( PDevice_Present ) ;
	//pCanvas->Begin() ;
	
	// Add Hack Information
	printf( "test\n" ) ;

	//pCanvas->End() ;
}



//D3DXVECTOR3 W2SN( D3DXVECTOR3 in )
//{
//	D3DXVECTOR3 out,temp;
//	DWORD dwTransform;
//	DWORD* pScreenMatrix = NULL;
//	ScreenMatrix* smObject;
//
//	dwTransform = ReadAddr(g_pTransformations);
//	//Log( "pTransform: 0x%08X\n", dwTransform );
//	pScreenMatrix = (DWORD*)( dwTransform + 0x94 );	
//	if( pScreenMatrix != NULL && pScreenMatrix != (DWORD*)0x00000094 )
//	{
//		//Log( "pScreenMatrix: 0x%08X\n", pScreenMatrix );
//		smObject = (ScreenMatrix*)*pScreenMatrix;
//		//Log("smObject: 0x%08X\n", smObject );
//		//Log("smObject->InViewRight: %7.3f\n", smObject->InViewRight.x );
//		//Log("smObject->InViewUp: %7.3f\n", smObject->InViewUp.x );
//		//Log("smObject->InViewForward: %7.3f\n", smObject->InViewForward.x );
//		//Log("smObject->InViewTranslation: %7.3f\n", smObject->InViewTranslation.x );
//		//Log("smObject->ViewPortMatrix: %7.3f\n", smObject->ViewPortMatrix.x );
//		//Log("smObject->ProjD1: %7.3f\n", smObject->ProjD1.x );
//		//Log("smObject->ProjD2: %7.3f\n", smObject->ProjD2.x );
//	
//		::D3DXVec3Subtract( &temp, &in, &smObject->InViewTranslation );
//
//		float x = ::D3DXVec3Dot( &temp, &smObject->InViewRight );
//		float y = ::D3DXVec3Dot( &temp, &smObject->InViewUp );
//		float z = ::D3DXVec3Dot( &temp, &smObject->InViewForward );
//
//		out.x =  smObject->ViewPortMatrix.x * ( 1 + ( x / smObject->ProjD1.x / z ) );
//		out.y =  smObject->ViewPortMatrix.y * ( 1 - ( y / smObject->ProjD2.y / z ) );
//		out.z =  z;
//	}
//
//	return out;
//}
