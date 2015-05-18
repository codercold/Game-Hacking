// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// TODO: reference additional headers your program requires here
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <Psapi.h>

#include <cstdio>
#include <fstream>
using namespace std ;
#include <TlHelp32.h>

#include "Log/Log.h"
#include "Hooks.h"
#include "Global.h"

#include "BaseDXHook\BaseDXHook.h"

#include <stdint.h>
#include "CreatedNtApi.h"

//#include "CCanvas/CCanvas.h"