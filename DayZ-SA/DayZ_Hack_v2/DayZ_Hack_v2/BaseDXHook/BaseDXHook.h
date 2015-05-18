#ifndef BASEDXHOOK_H
#define BASEDXHOOK_H
#pragma once

#define HOOK4( func, addy )	o##func = ( func##_t )DetourFunction( (PBYTE)addy, (PBYTE)hk##func )

#define PR         0
#define ES         1
#define DIP        2
#define SSS        3

LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	

/* =================== HOOKS AND TYPEDEFS ============================================================================================================================================================== */


typedef HRESULT ( WINAPI * EndScene_t )( LPDIRECT3DDEVICE9 pDevice );
extern EndScene_t oEndScene;
HRESULT WINAPI hkEndScene( LPDIRECT3DDEVICE9 pDevice );

typedef HRESULT ( WINAPI * DrawIndexedPrimitive_t )( LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount );
extern DrawIndexedPrimitive_t oDrawIndexedPrimitive;
HRESULT WINAPI hkDrawIndexedPrimitive( LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount );

typedef HRESULT   ( WINAPI * SetStreamSource_t )( LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, IDirect3DVertexBuffer9 * pStreamData, UINT OffsetInBytes, UINT Stride );
extern SetStreamSource_t oSetStreamSource;
HRESULT WINAPI hkSetStreamSource( LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride );

typedef HRESULT ( WINAPI * Present_t )( LPDIRECT3DDEVICE9 pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion );
extern Present_t oPresent;
HRESULT WINAPI hkPresent( LPDIRECT3DDEVICE9 pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion );

/* ===================================================================================================================================================================================================== */

DWORD GetModuleSize( LPSTR strModuleName );
VOID  OverlayWindow( DWORD* Vtable );
HRESULT DeviceSetup( HWND hwnd, DWORD* Vtab );		

#endif