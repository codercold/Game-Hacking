#include "stdafx.h"

// --- LOG ---
VOID WINAPIV Log( CHAR* szFormat, ... )
{
	CHAR szBuf[256]; 
	va_list list; 
	va_start( list, szFormat );
	vsprintf_s( szBuf, szFormat, list ); 
	va_end( list ); 

	pFile.open( "c://DayZ_Hack_v2.log", std::ios::app);
	if( pFile.fail() )
	{
		printf( "\nCould Not Open Log\n" );
		return;
	}
	pFile << szBuf << std::endl;
	pFile.close();
}// END OF LOG

VOID WINAPIV ClearLog( void )
{
	//pFile.open( "
	
}