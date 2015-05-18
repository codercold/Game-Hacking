#ifndef GLOBAL_H
#define GLOBAL_H

typedef struct tagGlobal
{
	DWORD64 g_Inj_ModuleHandle ;
	DWORD64 g_Game_Mod_Base ;
	HANDLE  g_H_Game_Mod_Base ;
	DWORD64 g_D3D9_Mod_Base ;
	DWORD64 g_Game_Mod_Size ;
	
	HANDLE  g_PseudoHandle ;
	HANDLE  g_AllAccess_Process_Handle ;

} GLOBAL, *PGLOBAL ;

extern PGLOBAL pGlobal ;

DWORD GetModuleSize( LPSTR strModuleName ) ;
void EnableDebugPriv( void ) ;
DWORD Get_PID_From_Process_Handle( HANDLE process_handle ) ;

#endif