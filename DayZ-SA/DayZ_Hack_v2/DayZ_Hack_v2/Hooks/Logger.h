#ifndef LOGGER_H
#define LOGGER_H

#pragma warning( disable: 4996 )

#define STR_MAX 2048

class String;
class logger;

class String
{
	public:
		char string[STR_MAX];

		String( char *buffer="" )
		{
			strcpy( string, buffer );
		}
		operator char*()
		{
			return string;
		}
		String operator = (char *str)
		{
			strcpy( string, str );
			return *this;
		}
};

// --
// Is an array of strings
// --
typedef std::vector<String> stringArray;

//--
// Will return an array of strings depending on the number of token(s) found in the input string
//--
static stringArray tokenizer( char *buffer, char *tokenizer ) //will fill 'ret' upon returning
{
	stringArray strArray; 
	strArray.clear();
	String strTemp;
	strcpy( strTemp.string, buffer );
	char *token = strtok( strTemp.string, tokenizer );

	while( token != NULL )
	{
		strcpy( strTemp.string, token );
		strArray.push_back( strTemp );

		token = strtok( NULL, tokenizer );
	}
	return strArray;
}

class logger
{
	private:
		String m_foldername;
		Sync m_cs;

	public:

		logger(char *folder_name)
		{
			_mkdir(folder_name);
			strcpy(m_foldername.string,folder_name);
		}

		void Log( const char *str, int len, const char *str1, int len2, DWORD Threadid = ::GetCurrentThreadId() )
		{
			try
			{
				m_cs.lock();

				String b;
				::GetModuleFileNameA( 0, b, sizeof(b) );
				static stringArray strArr = tokenizer( b, "\\" );
			
				String filename;
				SYSTEMTIME st;
				GetLocalTime( &st );
				sprintf( filename.string, "%s\\_%d_%d_%d_PID_%u_%s_.log", m_foldername.string, st.wDay, st.wMonth, st.wYear, ::GetCurrentProcessId(), strArr[ strArr.size() - 1 ].string );

				FILE *fp = fopen( filename.string, "a" );
				if( fp )
				{
					if( str1 != 0 )
					{
						for( int k = 0; k < len2; k++ )
						{
							fprintf( fp, "%c", str1[k] );
						}
					}

					fprintf( fp," TIME:[%02d:%02d:%02d] TID:[%x] +---+ Len[%d]\n", st.wHour, st.wMinute, st.wSecond, Threadid, len );

					for( int i=0; i < len; i++ )
					{
						fprintf( fp, "%02X | ", (unsigned char)str[i] );
					}

					fprintf( fp, "\n\n" );

					fclose( fp );
				}
				else
				{
					_mkdir( m_foldername );
				}

				m_cs.unlock();

			}catch(...)
			{
				//genral exception
			}
		}
};

static void LogString(const char *str, int len, const char *str1, int len2 )
{
	static logger Logger("c:\\Game_Network_Sniffer");
	
	if( len > 0 )
		Logger.Log( str, len, str1, len2 );
}

#endif