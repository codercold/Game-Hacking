#ifndef CLASSES
#define CLASSES

class ArmaString
{
	public:
		int refCount;
		int Size;
		char Name[744];
};

class SB_Player
{
	public:
		DWORD sb_PlayerID;
		ArmaString* sb_PlayerName;
};

class Entity
{
	public:
		DWORD* EntityArray;
		DWORD  SizeEntityArray;
};


class World
{
	public:
		char padding[0x0768];
		Entity* Entity;
}; 


class CArmaMain
{
	public:		
		static CArmaMain* GetInstance()
		{
			return *reinterpret_cast<CArmaMain**>( g_dwArmaMainOffset + 0x2 );
		}
}; 


class ScreenMatrix
{
	public:
		char _0x0000[4];
			D3DXVECTOR3 InViewRight;       //0x0004 
			D3DXVECTOR3 InViewUp;          //0x0010 
			D3DXVECTOR3 InViewForward;     //0x001C 
			D3DXVECTOR3 InViewTranslation; //0x0028 
		char _0x0034[32];
			D3DXVECTOR3 ViewPortMatrix;    //0x0054 
		char _0x0060[108];
			D3DXVECTOR3 ProjD1;            //0x00CC 
			D3DXVECTOR3 ProjD2;            //0x00D8 
		char _0x00E4[336];
};


class Transformations
{
	public:
		//ScreenMatrixObject* ScreenMatrixObjectPtr; //0x0000 
		static Transformations* GetInstance()
		{
			return *reinterpret_cast<Transformations**>( g_dwTransformsOffset + 0x5 );
		}		
}; 


class ScoreBoard
{
	public:
		DWORD* ScoreBoardArray;
		int NumOfPlayers;
		int MaxNumOfPlayers;

		static ScoreBoard* GetInstance()
		{
			return *reinterpret_cast<ScoreBoard**>( g_dwScoreBoardOffset + 0x1 );
		}
};


#endif