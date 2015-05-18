#ifndef HOOKS2_H
#define HOOKS2_H

#define SZ_SIG_MAX   128 /* Maximum size of a Nt*() signature (in bytes) */
#define SZ_PATCH_MAX 32  /* Maximum size of a detour patch (in bytes) */

typedef struct _PATCH_INFO
{

	BYTE* SystemCall;                  /* address of routine being patched */
	BYTE  Signature[SZ_SIG_MAX];       /* byte-signature for sanity check */
	BYTE  SignatureMask[SZ_SIG_MAX];   
	DWORD SignatureSize;               /* actual size of signature */

	BYTE* PrologDetour;                /* address of prologue detour */
	BYTE* EpilogDetour;                /* address of epilogue detour */

	BYTE  PrologPatch[SZ_PATCH_MAX];    /* jump instructions to detour */
	BYTE  PrologOriginal[SZ_PATCH_MAX]; /* bytes that were replaced */
	DWORD SizePrologPatch;             /* size within bytes */
	DWORD PrologPatchOffset;           /* relative location of patch */

	
	/* GameOverlayRenderer Hook Patch Section */
	// Patch Gameoverlay Hook
	BYTE* GameOverlayCall;
	BYTE* GameOverlayPatchCall;
	BYTE  GameOverlayPatch[SZ_PATCH_MAX];
	BYTE  GameOverlayOriginal[SZ_PATCH_MAX];
	BYTE* GameOverlayDetour;
	BYTE* GameOverlayDetour2;
	DWORD SizeGameOverlayPatch;
	DWORD GameOverlayOffset;

	// Rebuild Returning Bytes
	BYTE RebuiltReturn[SZ_PATCH_MAX];
	BYTE GameOverlayReturnOriginal[SZ_PATCH_MAX];
	DWORD SizeReturnPatch;
	DWORD RebuiltReturnOffset;
	/* End Of GameOverlayRenderer Hook Patch Section */


	BYTE  EpilogPatch[SZ_PATCH_MAX];    /* jump instructions to detour */
	BYTE  EpilogOriginal[SZ_PATCH_MAX]; /* bytes that were replaced */
	DWORD SizeEpilogPatch;             /* size in bytes */
	DWORD EpilogPatchOffset;           /* relative location of patch */ 
} PATCH_INFO, *PPATCH_INFO;



typedef struct _RecvFrom_Hook
{	
	/* Address To Function */
	BYTE* bpRecvFrom_Address;                         // Original Function Address

	/* Sanity Check To Find Out When BattlEye Steals My Damn Function */
	BYTE  bBattlEye_Signature_Of_Hook[SZ_SIG_MAX];
	BYTE  bBattlEye_Signature_Of_Hook_Mask[SZ_SIG_MAX];
	DWORD dwBattlEye_Signature_Of_Hook_Size;
	
	/* Store Original Bytes */
	BYTE  bPrologue_BattlEye_Original[SZ_PATCH_MAX];  // Original bytes at prologue of the function your hooking

	/* Store Patch Bytes, Size, and Offset */
	BYTE  bPrologue_BattlEye_Patch[SZ_PATCH_MAX];     // This will be the prologue patch for the function your hooking
	DWORD dwPrologue_BattlEye_Patch_Size;
	DWORD dwPrologue_BattlEye_Patch_Offset;
	
	/* Address To Our Wrapper Function */
	BYTE* bpPrologue_Function_Address;                // Our Wrapper function to grab parameters, and filter things out.

} RECVFROM_HOOK, *PRECVFROM_HOOK;


#endif


