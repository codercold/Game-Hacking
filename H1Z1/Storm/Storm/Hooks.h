#ifndef HOOKS_H
#define HOOKS_H

#define SZ_SIG_MAX   128 /* Maximum size of a Nt*() signature (in bytes) */
#define SZ_PATCH_MAX 32  /* Maximum size of a detour patch (in bytes) */

struct Sync
{
	CRITICAL_SECTION m_cs;
	Sync()
	{
		::InitializeCriticalSection(&m_cs);
	}

	void lock()
	{
		EnterCriticalSection(&m_cs);
	}

	void unlock()
	{
		LeaveCriticalSection(&m_cs);
	}

	~Sync()
	{
		::DeleteCriticalSection(&m_cs);
	}
};


typedef struct _PATCH_INFO
{
	/* Present Hook Patch Section */

	// Present Function
	BYTE* SystemCall;                   /* address of routine being patched */

	// Verify Signature Of Steam Hook
	BYTE  Signature[SZ_SIG_MAX];        /* byte-signature for sanity check */
	BYTE  SignatureMask[SZ_SIG_MAX];    /* byte-signature mask */
	DWORD SignatureSize;                /* actual size of signature */

	// My Hook
	BYTE* PrologDetour;                 /* address of prologue detour */	
	BYTE  PrologPatch[SZ_PATCH_MAX];    /* jump instructions to detour */
	BYTE  PrologOriginal[SZ_PATCH_MAX]; /* bytes that were replaced */
	DWORD SizePrologPatch;              /* size within bytes */
	DWORD PrologPatchOffset;            /* relative location of patch */
		
	/* GameOverlayRenderer Hook Patch Section */
	
	// Function
	BYTE* StartOfGameOverlayCall;	
	DWORD GameOverlayPatchCallOffset;

	// Call Within Function
	BYTE* GameOverlayPatchCall;
	DWORD GameOverlayPatchCallLength;
	BYTE  GameOverlayOriginalCallBytes[SZ_PATCH_MAX];	

	// Return
	BYTE  FirstSetOfStolenBytes[SZ_PATCH_MAX];   /* Bytes stolen from steam hook */
	BYTE  SecondSetOfStolenBytes[SZ_PATCH_MAX];  /* Bytes stolen from my hook */
	BYTE  TotalStolenBytes[SZ_PATCH_MAX];        /* Bytes stolen from Steam Hook & My Hook */
	
	// Detours
	BYTE* GameOverlayDetour;
	//BYTE* RebuildByteDetour;
	
	// Patch Call
	BYTE PatchCall[SZ_PATCH_MAX]; 
	BYTE ReturnCall[SZ_PATCH_MAX]; 

} PATCH_INFO, *PPATCH_INFO;

extern PPATCH_INFO pPatchInfo ;

//================================================================================================
//========= Present() Hook Fixup Information =====================================================
//================================================================================================
bool VerifySignature( BYTE* pcSigMask, BYTE* pbSig, DWORD dSigSize, BYTE* pbFuncAddr ) ;
void MakePatchLegit( BYTE* newRoutine, BYTE* patchCode ) ;
void FixUpSteamHook( PPATCH_INFO pInfo ) ;
void InsertDetour( BYTE* oldRoutine, BYTE* patchCode, DWORD patchSize, DWORD offset ) ;
void FixUpGameOverlayHook( PPATCH_INFO pInfo ) ;
void NewCallAddress( PPATCH_INFO pInfo ) ;
void NewReturnAddress( PPATCH_INFO pInfo ) ;

#endif