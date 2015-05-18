#ifndef CLASSES
#define CLASSES

#include "stdafx.h"

class CArmaMain;
class BaseObjectWorld;
class ObjectTable;
class Elements;
class PaddedElementsPtr;
class ObjectTableArray;
class Entity;
class Coordinate;
class PlayerInfoTable;
class PlayerInfoTable1;
class Transformations;
class TransformationsPtr1;
class EntityPlus70;
class EntityPlus70Plus40;
class EntityPlus70Plus40Plus30;
class ObjectName;

class N05A6F083;
class N05B47638;
class N05B581AF;
class N0D4BFB4A;
class N002689E1;
class N002C7CBD;
class N0048F003;
class FarLootTable;
class N004A42FE;
class N0057E18F;
class N0079E98D;
class NearLootTable;
class LootElementTableArrayPtr;
class N00C3F85D;
class N00C4BCE5;
class LootElement;
class LootCoordinate;
class LootNameAndOperationLevel0;
class LootNameAndOperationLevel1;
class LootNameAndOperation;
class N0169D3F2;
class N016B2C60;
class N016C4508;
class N016CD670;

class Level0ToVersionNumber;
class Level1ToVersionNumer;
class LocalPlayerLevel0;
class LocalPlayerLevel1;
class N04959297;
class N049795AC;
class LocalPlayerCoordinates;
class ScreenMatrix;

class ScoreBoard;
class PlayerTableArraylevel0;
class PlayerTableArray;
class PaddedNameAndIdTable;
class NameID_Element;
class Name_Element;

class CArmaMain
{
	public:
		BaseObjectWorld* BaseObjectWorld; //0x0000
		static CArmaMain* GetInstance()
		{
			return *reinterpret_cast<CArmaMain**>(g_dwArmaMainOffset+0x1);
		}
};//Size=0x0004

class LootNameAndOperation
{
	public:
		__int32 Identifier; //0x0000 
		__int32 LengthOfLootName; //0x0004 
		char LootName[256]; //0x0008 
	char _0x001A[1062];

};//Size=0x0440

class LootCoordinate
{
	public:
		char _0x0000[40];
		D3DXVECTOR3 lootpos;
		char _0x0034[12];

};//Size=0x0040

class LootNameAndOperationLevel1
{
	public:
		char _0x0000[4];
			LootNameAndOperation* LootNameAndOperationPtr; //0x0004 
		char _0x0008[120];

};//Size=0x0080

class LootNameAndOperationLevel0
{
	public:
		char _0x0000[64];
			LootNameAndOperationLevel1* LootNameAndOperationLevel1Ptr; //0x0040 
};//Size=0x0080

class LootElement
{
	public:
		char _0x0000[112];
			LootNameAndOperationLevel0* LootNameAndOperationLevel0Ptr; //0x0070 
		char _0x0074[40];
			LootCoordinate* LootCoordinatePtr; //0x009C 
		char _0x00A0[928];

};//Size=0x0440

class LootElementTableArrayPtr
{
	public:
		LootElement* ptrLootElement;
		LootElementTableArrayPtr() : ptrLootElement(0){}
};

class NearLootTable
{
	public:
		LootElementTableArrayPtr NearLootTableArray[200];	
};//Size=0x0440

class N00C3F85D
{
	public:
		char _0x0000[1088];

};//Size=0x0440

class N0079E98D
{
	public:
		char _0x0000[4];
			N00C3F85D* N0079E98F; //0x0004 
		char _0x0008[1080];

};//Size=0x0440

class N0057E18F
{
	public:
	char _0x0000[60];
		N0079E98D* N0057E19F; //0x003C 
	char _0x0040[3072];

};//Size=0x0C40

class N004A42FE
{
	public:
			N0057E18F* N004A42FF; //0x0000 
		char _0x0004[108];
			N0057E18F* N00586ECE; //0x0070 
		char _0x0074[1908];
			char N00762A38[20]; //0x07E8 
		char _0x07FC[4];
			char N00762A3E[14]; //0x0800 
			char N0078BF4F[20]; //0x080E 
		char _0x0822[2];
			char N00762A47[20]; //0x0824 
			char N007839B3[20]; //0x0838 
		char _0x084C[2036];

};//Size=0x1040

class FarLootTable
{
	public:
		N004A42FE* N0049FC2D; //0x0000 
	char _0x0004[1084];

};//Size=0x0440

class N002C7CBD
{
	public:
		char _0x0000[3136];

};//Size=0x0C40

class N0048F003
{
	public:
		char _0x0000[12];
			N004A42FE* N0048F007; //0x000C 
		char _0x0010[1072];

};//Size=0x0440

class N05B581AF
{
	public:
		N0048F003* N05B581B0; //0x0000 
		N0079E98D* N05B581B1; //0x0004 
	char _0x0008[1080];

};//Size=0x0440

class N0D4BFB4A
{
	public:
		char _0x0000[8];
			N05B581AF* N0D4BFB4D; //0x0008 
		char _0x000C[1076];

};//Size=0x0440

class LocalPlayerCoordinates
{
	public:
		char _0x0000[40];
			//float LocalX; //0x0028 
			//float LocalZ; //0x002C 
			//float LocalY; //0x0030 
			D3DXVECTOR3 LocalPos;
		char _0x0034[12];
};//Size=0x0040

class LocalPlayerLevel1
{
	public:
		char _0x0000[32];
			LocalPlayerCoordinates* LocalPlayerCoordinatesPtr; //0x0020 
		char _0x0024[156];
};//Size=0x00C0

class LocalPlayerLevel0
{
	public:
		LocalPlayerLevel1* LocalPlayerLevel1Ptr; //0x0000 
	char _0x0004[60];

};//Size=0x0040

class ToVersionNumberName
{
	public:
		char _0x0000[8];
			char VersionNumber[12]; //0x0008 
		char _0x0014[44];
};//Size=0x0040

class ToVersionNumber
{
	public:
	char _0x0000[20];
		ToVersionNumberName* pNextVersionNumber; //0x0014 
	char _0x0018[40];
};//Size=0x0040

class BaseObjectWorld
{
	public:
		char _0x0000[32];
			ToVersionNumber* pNextVersionNumber; //0x0020 
		char _0x0024[1848];
			ObjectTable* ObjectTablePtr; //0x075C 
		char _0x0748[760];
			LocalPlayerLevel0* LocalPlayerLevel0Ptr; //0x0A58 
		char _0x0A44[1348];
			NearLootTable* NearLootTablePtr; //0x0FA0 
			volatile __int32 SizeOfNearLootTable; //0x0FA4 
		char _0x0F90[160];
			FarLootTable* FarLootTablePtr; //0x1048 
			__int32 SizeOfFarLootTable; //0x104C 	
};//Size=0x1840

class ObjectTable
{
	public:
	char _0x0000[4];
		volatile __int32 ObjectTableSize; //0x0004 
		__int32 SizeOfPlayerArray; //0x0008 
		ObjectTableArray* ObjectTableArrayPtr; //0x000C 
	char _0x0010[2160];
};//Size=0x0880

class Elements
{
	public:
		__int32 N205AABBE; //0x0000 
		Entity* EntityPtr; //0x0004 	
};//Size=0x0080

//class PlayerArray
//{
//public:
//	Elements* element;
//	char pad[40];
//};

class PaddedElementsPtr
{
	public:
	  Elements* element;
	  char _0x0088[20];
	  __int32 CheckEntity; //0x009C 
	  char _0x00A0[16];
	  PaddedElementsPtr() : element(0){}
};

class ObjectTableArray 
{
	public:
	  PaddedElementsPtr ObjectTableArrayElement[100];
};




class NameTexture
{
	public:
		__int32 iModelIndex; //0x0000 
		__int32 szNameTexture; //0x0004 
		char cNameTexture[256]; //0x0008 
};//Size=0x0240

class EntName
{
	public:
	char _0x0000[4];
		__int32 szEntName; //0x0004 
		char cEntName[256]; //0x0008 
};//Size=0x0040

class EntityTextureAndName
{
	public:
	char _0x0000[52];
		NameTexture* NameTexturePtr; //0x0034 
	char _0x0038[56];
		EntName* EntNamePtr; //0x0070 
};//Size=0x0480


class Entity
{
	public:
		char _0x0000[4];
			__int32 N205C828F; //0x0004 
		char _0x0008[24];
			Coordinate* CoordinatePtr; //0x0020 
		char _0x0024[76];
			EntityTextureAndName* EntityTextureAndNamePtr; //0x0070 		
};//Size=0x0880




class Coordinate
{
	public:
		char _0x0000[40];
		D3DXVECTOR3 pos;
		char _0x0034[228];
			D3DXVECTOR3 HeadVector; //0x0118 
			D3DXVECTOR3 BodyVector; //0x0124 
		char _0x0130[2128];
};//Size=0x0980







class PlayerInfoTable
{
	public:
			PlayerInfoTable1* PlayerInfoTablePtr; //0x0000 
		char _0x0004[60];
};//Size=0x0040

class PlayerInfoTable1
{
	public:
		char _0x0000[2112];
};//Size=0x0840

class ScreenMatrix
{
	public:
		char _0x0000[4];
			D3DXVECTOR3 InViewRight; //0x0004 
			D3DXVECTOR3 InViewUp; //0x0010 
			D3DXVECTOR3 InViewForward; //0x001C 
			D3DXVECTOR3 InViewTranslation; //0x0028 
		char _0x0034[32];
			D3DXVECTOR3 ViewPortMatrix; //0x0054 
		char _0x0060[108];
			D3DXVECTOR3 ProjD1; //0x00CC 
			D3DXVECTOR3 ProjD2; //0x00D8 
		char _0x00E4[336];
};//Size=0x0234

class ScreenMatrixObject
{
	public:
		char _0x0000[148];
			ScreenMatrix* ScreenMatrix; //0x0094 
		char _0x0098[1960];	

};//Size=0x0840

class Transformations
{
	public:
			ScreenMatrixObject* ScreenMatrixObjectPtr; //0x0000 
		char _0x0004[1084];

		static Transformations* GetInstance()
		{
			//
			//return *reinterpret_cast<Transformations**>(*reinterpret_cast<DWORD*>(g_dwTransformsOffset+0xB));
		
			// Allows use of ScreenMatrixObjectPtr
			return *reinterpret_cast<Transformations**>( g_dwTransformsOffset + 0xB );
		}

		D3DXVECTOR3 W2SN(D3DXVECTOR3 in)
		{
			D3DXVECTOR3 out,temp;

			::D3DXVec3Subtract( &temp, &in, &ScreenMatrixObjectPtr->ScreenMatrix->InViewTranslation );

			float x = ::D3DXVec3Dot( &temp, &ScreenMatrixObjectPtr->ScreenMatrix->InViewRight );
			float y = ::D3DXVec3Dot( &temp, &ScreenMatrixObjectPtr->ScreenMatrix->InViewUp );
			float z = ::D3DXVec3Dot( &temp, &ScreenMatrixObjectPtr->ScreenMatrix->InViewForward );

			out.x =  ScreenMatrixObjectPtr->ScreenMatrix->ViewPortMatrix.x * ( 1 + ( x / ScreenMatrixObjectPtr->ScreenMatrix->ProjD1.x / z ) );
			out.y =  ScreenMatrixObjectPtr->ScreenMatrix->ViewPortMatrix.y * ( 1 - ( y / ScreenMatrixObjectPtr->ScreenMatrix->ProjD2.y / z ) );
			out.z =  z;

			return out;
		}
		
};//Size=0x0440

class TransformationsPtr1
{
	public:
		char _0x0000[2112];
};//Size=0x0840

class EntityPlus70
{
	public:
		char _0x0000[64];
			EntityPlus70Plus40* EntityPlus70Plus40; //0x0040 
		char _0x0044[1084];
};//Size=0x0480

class EntityPlus70Plus40
{
	public:
		char _0x0000[48];
			EntityPlus70Plus40Plus30* EntityPlus70Plus40Plus30; //0x0030 
		char _0x0034[56];
			ObjectName* ObjectNamePtr; //0x006C 
		char _0x0070[16];
};//Size=0x0080

class EntityPlus70Plus40Plus30
{
	public:
		char _0x0000[8];
			char N00DE32B6[20]; //0x0008 
		char _0x001C[100];
};//Size=0x0080

class ObjectName
{
	public:
		char _0x0000[4];
			__int32 lengthOfEntName; //0x0004 
			char entName[32]; //0x0008 		
};//Size=0x0080

class NameID_Element
{
	public:
		char _0x0000[8];
			char PlayerID[20]; //0x0008 
		char _0x001C[36];

};//Size=0x0040

class Name_Element
{
	public:
		char _0x0000[8];
			char PlayerName[16]; //0x0008 
		char _0x0018[104];

};//Size=0x0080

class PaddedNameAndIdTable
{
	public:
		NameID_Element* idElement;
		char pad[112];
		Name_Element* nameElement;
		char pad1[112];
		PaddedNameAndIdTable() : idElement(0), nameElement(0){}
};

class PlayerTableArray
{
	public:
		char pad[12];
		PaddedNameAndIdTable NameAndIdTable[45];

};

class PlayerTableArraylevel0
{
	public:
		char _0x0000[12];
			PlayerTableArray* PlayerTableArrayPtr; //0x000C 
			__int32 PlayerCount; //0x0010 
		char _0x0014[44];

};//Size=0x0040

class ScoreBoard
{
	public:
		char _0x0000[36];
			PlayerTableArraylevel0* PlayerTableArrayLevel0Ptr; //0x0024 
		char _0x0028[24];

};//Size=0x0040

















#endif