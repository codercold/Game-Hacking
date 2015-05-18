#ifndef LOG_H
#define LOG_H

VOID WINAPIV Log( CHAR* szFormat, ... );
extern ofstream pFile;

VOID WINAPIV ClearLog( void );

#endif