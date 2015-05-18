#include "stdafx.h"

// --- LOG ---
VOID WINAPIV Log( CHAR* szFormat, ... )
{
	CHAR szBuf[256]; 
	va_list list; 
	va_start( list, szFormat );
	vsprintf_s( szBuf, szFormat, list ); 
	va_end( list ); 

	pFile.open( "c://H1Z1.log", ios::out | ios::in |ios::app ) ;
	if( pFile.fail() )
	{
		printf( "\nCould Not Open Log\n" );
		return;
	}
	
	if( strlen(szBuf) != 0 )
	{
		pFile << szBuf << std::endl ;
	}

	pFile.close();

}// END OF LOG