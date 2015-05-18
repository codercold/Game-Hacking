#include "stdafx.h"

//void  CTools::CreateDebugConsole(LPCWSTR lPConsoleTitle)
//{
//	
//	HANDLE lStdHandle = 0; 
//    int hConHandle = 0;   
//    FILE *fp = 0;
//    AllocConsole( );
//    lStdHandle = GetStdHandle( STD_OUTPUT_HANDLE );
//    hConHandle = _open_osfhandle( PtrToUlong( lStdHandle ), _O_TEXT );
//	SetConsoleTitle(lPConsoleTitle);
//	SetConsoleTextAttribute(lStdHandle,FOREGROUND_GREEN|FOREGROUND_INTENSITY|BACKGROUND_RED);
//    fp = _fdopen( hConHandle, "w" );
//    *stdout = *fp;
//    setvbuf( stdout, NULL, _IONBF, 0 ); 
//}


PBYTE WINAPI CTools::HookVTableFunction(PDWORD ppVTable, PBYTE pHook, SIZE_T iIndex)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)((ppVTable) + iIndex), 4, PAGE_EXECUTE_READWRITE, &dwOld);
	PBYTE pOrig = ((PBYTE)(ppVTable)[iIndex]); 
	(ppVTable)[iIndex] = (DWORD)pHook;

	VirtualProtect((void*)((ppVTable) + iIndex), 4, dwOld, &dwOld);

	return pOrig;
}

DWORD CTools::FindPattern(DWORD start_offset, DWORD size, BYTE* pattern, char mask[] )
{

    DWORD pos = 0;
    int searchLen = strlen(mask) - 1;
    for( DWORD retAddress = start_offset; retAddress < start_offset + size; retAddress++ )
    {
        if( *(BYTE*)retAddress == pattern[pos] || mask[pos] == '?' ){
            if( mask[pos+1] == '\0' )
                return (retAddress - searchLen);
            pos++;
        } 
        else 
            pos = 0;
    }
    return NULL;
}

int CTools::GetDistance(D3DXVECTOR3 one, D3DXVECTOR3 two)
{
float dx,dy,dz;

dx = one.x-two.x;
dy = one.y-two.y;
dz = one.z-two.z;

return abs(sqrt(dx*dx + dy*dy + dz*dz)); 
}


DWORD CTools::GetModuleSize(LPSTR strModuleName)
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