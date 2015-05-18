#include "stdafx.h"

#pragma warning(disable:4996)

EndScene_t oEndScene;
Present_t oPresent;
DrawIndexedPrimitive_t oDrawIndexedPrimitive;
SetStreamSource_t oSetStreamSource;

// --- HOOK END SCENE ---
HRESULT WINAPI hkEndScene( LPDIRECT3DDEVICE9 pDevice )
{	
	__asm nop	

	// Do whatever...
	
	return oEndScene( pDevice );

} // END HOOK END SCENE



//// --- HOOK PRESENT ---
//HRESULT WINAPI hkPresent( LPDIRECT3DDEVICE9 pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion )
//{
//	D3DVIEWPORT9 ViewPort;
//	pDevice->GetViewport(&ViewPort);
//
//	// Rendering code and text goes here....
//	printf("\n\nPresent() Hooked");
//	
//	return oPresent( pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
//}
//// END HOOK PRESENT 


HOOKDEF( HRESULT, WINAPI,    Present, 
		 LPDIRECT3DDEVICE9   pDevice, 
		 const RECT *    pSourceRect, 
		 const RECT *      pDestRect, 
		 HWND    hDestWindowOverride, 
		 const RGNDATA * pDirtyRegion
){
	D3DVIEWPORT9 ViewPort;
	pDevice->GetViewport(&ViewPort);
	
	// Rendering code and text goes here....
	//Log("\n\nPresent() Hooked");

	HRESULT ret = Old_Present( pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion );
	return ret;
}


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
      
	// Do whatever...

	return oSetStreamSource( pDevice, StreamNumber, pStreamData, OffsetInBytes, Stride );
}// END HOOK SET STREAM SOURCE 


// --- DeviceSetup ---
HRESULT DeviceSetup( HWND hwnd, DWORD* Vtab )
{
	//Log( "\nStart Of The DeviceSetup() Function" );
	// Create a Direct3D object. This is a COM interface of type IDirect3D9.
    // Directx api - Direct3DCreate9 - Create an IDirect3D9 object and return an interface to it.
    // 1st arg - The value of this parameter should be D3D_SDK_VERSION.
    // return type - If successful, this function returns a pointer to an IDirect3D9 interface; otherwise, a NULL pointer is returned.
	LPDIRECT3D9 pD3D = NULL;
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
      return E_FAIL;
	}

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

	//Log("\nDevice Created");

	// Put the d3dDevice's pointer as a dword address into pVTable
	DWORD* pVTable = (DWORD*)pd3dDevice;

    // Put the dword address into pVTable for referencing these 118 direct3d devices functions. 
	pVTable = (DWORD*)pVTable[0];

	Log( "\nVTable Pointer: %08X", pVTable );
	Log( "\nVTable[17] Present Address: %08X", pVTable[17] );

    // These values are on the stack as a copy of what is inside the d3d9.h as a virtual function table.
    // I'm only putting reference to what and where I'm referencing this virtual table. Mostly where I'm getting 0, 42, 82, and 100 from.
    // This is our DWORD VTable[ 3 ] = { 0 }; from the Mythread function we saved for later and passed to DX_Init. Now we will fill it.
	Vtab[PR]    = pVTable[17];  // Address into PRE from Vtable. Later used in DetourFunction. Ref: Present inside              d3d9.h line 727
	Vtab[ES]	= pVTable[42];  // Address into ES from Vtable. Later used in DetourFunction.  Ref: End Scene inside            d3d9.h line 752 
	Vtab[DIP]	= pVTable[82];  // Address into DIP from Vtable. Later used in DetourFunction. Ref: DrawIndexedPrimitive inside d3d9.h line 792
	Vtab[SSS]	= pVTable[100]; // Address into SSS from Vtable. Later used in DetourFunction. Ref: SetStreamSource inside      d3d9.h line 810
	
	//Log( "\nFunctions Assigned To VTable" );

	//Log( "\nEnd Of The DeviceSetup Function" );
	return S_OK;
}// --- End of DeviceSetup ---



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



VOID OverlayWindow( DWORD* Vtable )
{
	//Log( "\nStart Of The OverlayWindow() Function");

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

	//Log("\nDevice Setup Done");
	
	// Windows api - DestroyWindow - Destroys the specified window. The function sends WM_DESTROY and WM_NCDESTROY messages to the window to deactivate 
    //                               it and remove the keyboard focus from it. The function also destroys the window's menu, flushes the thread message queue, 
    //                               destroys timers, removes clipboard ownership, and breaks the clipboard viewer chain (if the window is at the top of the viewer chain).
    // 1st arg - A handle to the window to be destroyed. 
    // return type - If the function succeeds, the return value is nonzero.
	DestroyWindow(hWnd);

	//Log("\nEnd Of The OverlayWindow() Function" );
}



DWORD GetModuleSize( LPSTR strModuleName )
{
	MODULEENTRY32 lpme   = { 0 };
	DWORD		  dwSize = 0;
	DWORD		  PID    = GetCurrentProcessId();
	BOOL		  isMod  = 0;
	char		  chModName[ 256 ];

	strcpy_s( chModName, strModuleName );
	_strlwr_s( chModName );

	HANDLE hSnapshotModule = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, PID );
	
	if ( hSnapshotModule ) 
	{
		lpme.dwSize = sizeof( lpme );
		isMod = Module32First( hSnapshotModule, &lpme );
		
		while( isMod ) 
		{
			if ( strcmp( _strlwr( (char*)lpme.szExePath ), chModName ) ) 
			{
				dwSize = (DWORD)lpme.modBaseSize;
				CloseHandle( hSnapshotModule );
				return dwSize;
			}

			isMod = Module32Next( hSnapshotModule, &lpme );
		}
	}

	CloseHandle( hSnapshotModule );

	return 0;
} 