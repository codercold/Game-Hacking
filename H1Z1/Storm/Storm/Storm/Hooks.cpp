#include "stdafx.h"



extern "C" { DWORD64 Fixup_Steam_Jump_Bytes ; 
			 DWORD64 Fixup_CallAddress ; 
			 DWORD64 Fixup_ReturnOriginal ;  
			 void GameOverlay_Detour( void ) ;
			 void RebuildByte_Detour( void ) ; }

DWORD64 Fixup_GameOverlay ;
DWORD64 Fixup_GameOverlay_Call_Bytes ;
DWORD64 Fixup_Rebuild_Stolen_Byte_Location ;



//================================================================================================
//========= Present() Hook Fixup Information =====================================================
//================================================================================================
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



void MakePatchLegit( BYTE* newRoutine, BYTE* patchCode )
{
	DWORD64  dwAddress ;
	DWORD64* pdwNewAddress ;
	Log( " NewRoutine: %016llX\n", newRoutine ) ;
	dwAddress      = (DWORD64)newRoutine ;              // Copy Address of newRoutine into dwAddress.
	pdwNewAddress  = (DWORD64*)&patchCode[2] ;          // Copy Address to the first byte of cafebabe.
	*pdwNewAddress = dwAddress ;                        // Assign new Address to the dereferenced address of cafebabe.
	Log( " pdwNewAddress: %016llX\n", pdwNewAddress ) ;
	return ;
}



void FixUpSteamHook( PPATCH_INFO pInfo )
{	
	Fixup_Steam_Jump_Bytes = *( (DWORD*)&( pInfo->PrologOriginal[1] ) ) ;	
	Fixup_Steam_Jump_Bytes += (DWORD64)pInfo->SystemCall ;
	Fixup_Steam_Jump_Bytes += 5 ;		
	Log( "Fixup_Steam_Jump_Bytes: %016llX", Fixup_Steam_Jump_Bytes ) ;	
	return ;
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



void FixUpGameOverlayHook( PPATCH_INFO pInfo )
{	
	// Locals
	HANDLE  hGameOverlay ;
	DWORD64 dwGameOverlayBase ;
	DWORD64 dwBytesRead = 0 ;	
	DWORD64* tempPointer = NULL, *tempPointer2 = NULL, *tempPointer3 = NULL, *tempPointer4 = NULL;
	tempPointer = (DWORD64*)malloc( sizeof( DWORD64 ) ) ;

	// Beginning of gameoverlay code	
	Fixup_GameOverlay = Fixup_Steam_Jump_Bytes + 0x6 ;
	*tempPointer = Fixup_GameOverlay ;
	Fixup_GameOverlay = *(DWORD64*)*tempPointer ;
	free( tempPointer ) ;
	tempPointer = NULL ;
	//Log( "Fixup_GameOverlay: %016llX", Fixup_GameOverlay ) ;
	pInfo->StartOfGameOverlayCall = (BYTE*)Fixup_GameOverlay ;

	// Offset 19D hex into gameoverlay
	pInfo->GameOverlayPatchCallOffset = 0x19D ;
	pInfo->GameOverlayPatchCallLength = 0x6 ;
	pInfo->GameOverlayPatchCall = pInfo->StartOfGameOverlayCall + pInfo->GameOverlayPatchCallOffset ;
	//Log( "GameOverlayPatchCall: %016llX", pInfo->GameOverlayPatchCall ) ;
	
	// Grab GameOverlayRenderer64.dll Handle
	int	iCount = 0 ;
	do
	{
		if( iCount < 3 )
		{
			hGameOverlay = (HANDLE)(GetModuleHandle( "gameoverlayrenderer64.dll" ) ) ;		
			Sleep( 100 ) ; // Sleep .1 second
		}else
		{
			Log("\n GameOverlay Base Failed") ;
			break ;
		}		
		iCount++ ;
	}while( !hGameOverlay ) ;
	Log( "hGame: %016llX", hGameOverlay ) ;

	// Assign Base From Handle	
	dwGameOverlayBase = (DWORD64)hGameOverlay ;

	// Change address access rights at our Patch Call
	DWORD old_protect;
	if( VirtualProtect( pInfo->StartOfGameOverlayCall, pInfo->GameOverlayPatchCallOffset, PAGE_EXECUTE_READWRITE, &old_protect ) )
	{	
		// Save Original GameOverlay Call Bytes Into Structure
		if( ( ReadProcessMemory( pGlobal->g_AllAccess_Process_Handle, (LPCVOID)pInfo->GameOverlayPatchCall, pInfo->GameOverlayOriginalCallBytes, pInfo->GameOverlayPatchCallLength, &dwBytesRead ) ) != 0 )
		{			
			Log( "First RPM - SUCCESS: BytesRead: %d\n", dwBytesRead ) ;
		}else
		{
			Log( "First RPM - FAILED: BytesRead: %d\n", dwBytesRead ) ;
		}
		Log( "GameOverlayOriginal: %016llX\n", pInfo->GameOverlayOriginalCallBytes ) ;

		// Strip Off The First Two Bytes From Original GameOverlay Call
		tempPointer2 = (DWORD64*)malloc( sizeof( DWORD64 ) ) ;
		*tempPointer2 = (DWORD)*(DWORD*)&pInfo->GameOverlayOriginalCallBytes[2] ; /* Skip FF 15 */		
		Fixup_GameOverlay_Call_Bytes = *tempPointer2 ;
		free( tempPointer2 ) ;
		tempPointer2 = NULL ;
		Log( "Fixup_GameOverlay_Call_Bytes: %016llX", Fixup_GameOverlay_Call_Bytes ) ;
				
		// Add Call Address + Call Bytes To Get Area We Need To Rebuild Our Overwritten Bytes		
        Fixup_Rebuild_Stolen_Byte_Location = (DWORD64)( ( pInfo->GameOverlayPatchCall + Fixup_GameOverlay_Call_Bytes ) + 0x6 ) ;
		//Log( "Fixup_Rebuild_Stolen_Byte_Location: %016llX\n", Fixup_Rebuild_Stolen_Byte_Location ) ;
		tempPointer3 = (DWORD64*)malloc( sizeof( DWORD64 ) ) ;
		*tempPointer3 = Fixup_Rebuild_Stolen_Byte_Location ;
		//Log( "tempPointer: %016llX\n", tempPointer3 ) ;
		Fixup_Rebuild_Stolen_Byte_Location = *(DWORD64*)*tempPointer3 ;
		free( tempPointer3 ) ;
		tempPointer3 = NULL ;
		Log( "Fixup_Rebuild_Stolen_Byte_Location: %016llX\n", Fixup_Rebuild_Stolen_Byte_Location ) ;
				
		// Copy Return Bytes
		dwBytesRead = 0 ;
		if( ( ReadProcessMemory( pGlobal->g_AllAccess_Process_Handle, (LPCVOID)Fixup_Rebuild_Stolen_Byte_Location, pInfo->FirstSetOfStolenBytes, 0x7, &dwBytesRead ) ) != 0 )
		{			
			Log( "Second RPM - SUCCESS: BytesRead: %d\n", dwBytesRead );
		}else
		{
			Log( "Second RPM - FAILED: BytesRead: %d\n", dwBytesRead ) ;
		}
				
		// Move Toward Empty Memory aka code cave
		Fixup_CallAddress  = Fixup_Rebuild_Stolen_Byte_Location + 0x1A ;
		Log( "Fixup_Rebuild_Stolen_Byte_Location: %016llX\n", Fixup_Rebuild_Stolen_Byte_Location ) ;	
		Log( "Fixup_CallAddress: %016llX\n", Fixup_CallAddress ) ;

		// Assemble Stolen Bytes From Present()
		for( int i = 7; i <= 14; i++ )
		{
			pInfo->SecondSetOfStolenBytes[i] = pInfo->PrologOriginal[i] ;
		}
	    Log( "SecondSetOfStolenBytes: %016llX\n", pInfo->SecondSetOfStolenBytes ) ;
		
		for( int k = 0; k <= 14; k++ )
		{
			if( k < 7 )
			{
				pInfo->TotalStolenBytes[k] = pInfo->FirstSetOfStolenBytes[k] ;
			}else
			{
				pInfo->TotalStolenBytes[k] = pInfo->SecondSetOfStolenBytes[k] ;
			}
		}
		Log( "TotalStolenBytes: %016llX\n", pInfo->TotalStolenBytes ) ;
		
		// Direct GameOverlay Call To Rebuild Stolen Bytes From Present()		
		pInfo->GameOverlayDetour = (BYTE*)GameOverlay_Detour ;

		//Log( "GameOverlay_Detour: %016llX", pInfo->GameOverlayDetour ) ;		
		//memcpy( pInfo->PatchCall, "\xFF\x15\xBE\xBA\xFE\xCA", 0x6 ) ;
		memcpy( pInfo->PatchCall, "\xE8\xBE\xBA\xFE\xCA\x90", 0x6 ) ;
		NewCallAddress( pInfo ) ;		
		InsertDetour( pInfo->GameOverlayPatchCall, pInfo->PatchCall, 0x6, 0 ) ;      /* Activate Hook */
		Log( "Second HOOK Complete" ) ;
		
		// Rebuilding Stolen Bytes From Original Present Function Here 	
		memcpy( pInfo->ReturnCall, "\xDE\xC0\xBE\xBA\xFE\xCA\xED\xFE\xDE\xC0\xBE\xBA\xFE\xCA\xED\xFE\x90\x90\x90", 0x13 ) ;				
		NewReturnAddress( pInfo ) ;		
		Fixup_ReturnOriginal = ((DWORD64)pInfo->SystemCall) + pInfo->SizePrologPatch ;
		Log( "Fixup_ReturnOriginal: %016llX", Fixup_ReturnOriginal ) ;
		InsertDetour( (BYTE*)Fixup_CallAddress, pInfo->ReturnCall, 0x13 + 1, 0 ) ;
		Log( "Rebuilding Complete" ) ;

	}else
	{
		Log( "Second HOOK Virtual Protect Failed" ) ;
	}
	
	return;
}



void NewCallAddress( PPATCH_INFO pInfo )
{
	DWORD64 offset = 0 ;
	DWORD* pNewAddress ;
		
	if( (DWORD64)pInfo->GameOverlayPatchCall > (DWORD64)pInfo->GameOverlayDetour )
	{
		offset = (DWORD64)pInfo->GameOverlayPatchCall - (DWORD64)pInfo->GameOverlayDetour ;
	}else
	{
		offset = (DWORD64)pInfo->GameOverlayDetour - (DWORD64)pInfo->GameOverlayPatchCall ; 
	}	

	if( (DWORD64)pInfo->GameOverlayDetour == ( (DWORD64)pInfo->GameOverlayPatchCall + offset ) )
	{
		pNewAddress = (DWORD*)&( pInfo->PatchCall[1] ) ;
		*pNewAddress = offset - 5 ;		
		Log( "Offset: %016llX\n", offset ) ;	
		Log( "PatchCall: %016llX", pInfo->PatchCall ) ;	

	}else if ( (DWORD64)pInfo->GameOverlayDetour == ( (DWORD64)pInfo->GameOverlayPatchCall + (-offset) ) )
	{
		pNewAddress = (DWORD*)&( pInfo->PatchCall[1] ) ;
		*pNewAddress = (-offset) - 5 ;		
		Log( "Offset: %016llX\n", (-offset) ) ;	
		Log( "PatchCall: %016llX", pInfo->PatchCall ) ;	

	}else
	{
		Log( "GameOverlayDetour Failed" ) ;
	}
	
	return ;
}



void NewReturnAddress( PPATCH_INFO pInfo )
{
	DWORD64* pNewAddress ;
	DWORD64 temp, offset ;
	DWORD* pReturnAddress ;
	pNewAddress = (DWORD64*)&( pInfo->ReturnCall[0] ) ;		
	memcpy( pNewAddress, pInfo->TotalStolenBytes, sizeof( pInfo->TotalStolenBytes ) ) ;

	pInfo->ReturnCall[15] = 0xE9 ;
	
	pReturnAddress = (DWORD*)&( pInfo->ReturnCall[16] ) ;
	temp = ((DWORD64)pInfo->SystemCall) + pInfo->SizePrologPatch ;
	offset = (DWORD)( ( Fixup_CallAddress + 0xf ) - temp ) ;
	*pReturnAddress = (-offset) - 5 ;
	//Log( "Final Return Address: %016llX\n", *pReturnAddress ) ;

	return ;
}