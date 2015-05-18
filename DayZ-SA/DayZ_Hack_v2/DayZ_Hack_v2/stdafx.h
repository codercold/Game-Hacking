// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>


// TODO: reference additional headers your program requires here
#include <cstdio>
#include <TlHelp32.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <detours.h>
#include <math.h>

#include "BaseDXHook\BaseDXHook.h"

#include <iostream>
#include <fstream>
#include <map>
using namespace std;
#include "Log\Log.h"


// HOOKS Folder
#include <stdint.h>
#include <WinSock2.h>
#include <vector>
#include <direct.h>
#include "Hooks\CreatedNtApi.h"
#include "Hooks\distorm.h"
#include "Hooks\Hooking.h"
#include "Hooks\Hooks.h"
#include "Hooks\Ignore.h"
#include "Hooks\Logger.h"

#include "Hooks\mnemonics.h"
#include "Hooks\Log.h"

#include "Hooks\Hooks2.h"

#include "CSignatures.h"
#include "Classes.h"

#include "CTools\CTools.h"

#include "CCanvas\CCanvas.h"

/* VM Map */
//#include "VMMap\VMQuery.h
//#include "..\..\..\..\..\Documents\Windows_VIA_C_CPP_Source\Windows-via-CPlusPlus-Code\CommonFiles\CmnHdr.h"
//#include "..\..\..\..\..\Documents\Windows_VIA_C_CPP_Source\Windows-via-CPlusPlus-Code\CommonFiles\Toolhelp.h"
//#include <psapi.h>
//#include <windowsx.h>
//#include <tchar.h>
//#include "Resource.h"
//#include "VMQuery.h"
//#include <strsafe.h>


#include "WinControls.h"




#include <CommCtrl.h>
#include "resource1.h"



