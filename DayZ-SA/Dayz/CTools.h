#ifndef CTOOLS
#define CTOOLS

#include "stdafx.h"

class CTools
{
public:
	void CreateDebugConsole(LPCWSTR lPConsoleTitle);
	PBYTE WINAPI HookVTableFunction(PDWORD ppVTable, PBYTE pHook, SIZE_T iIndex);
	DWORD FindPattern(DWORD start_offset, DWORD size, BYTE* pattern, char mask[]);
	DWORD GetModuleSize(LPSTR strModuleName);
	int GetDistance(D3DXVECTOR3,D3DXVECTOR3);
};

extern CTools g_Tools;
#endif