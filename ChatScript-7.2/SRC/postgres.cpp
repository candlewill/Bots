#include "common.h"


#ifndef DISCARDPOSTGRES
#include "postgres/libpq-fe.h"
static  PGconn     *conn; // shared db open stuff
static bool connDummy = false;
static char pguserFilename[MAX_WORD_SIZE];
// user files stored in postgres instead of file system
static  PGconn     *usersconn; // shared db open stuff used instead of files for userwrites
static char* pgfilesbuffer = 0;
char postgresparams[300]; // init string for pguser

#ifdef WIN32
#pragma comment(lib, "../SRC/postgres/libpq.lib")
#endif

void PostgresShutDown() // script opened file
{
	if (conn)  PQfinish(conn);
	conn = NULL;
}

FunctionResult DBCloseCode(char* buffer)
{
	if (!conn) 
	{
		if (connDummy)
		{
			connDummy = false;
			return NOPROBLEM_BIT;
		}
		char* msg = "db not open\r\n";
		SetUserVariable((char*)"$$db_error",msg);	// pass along the error
		Log(STDTRACELOG,msg);
		return FAILRULE_BIT;
	}
	PostgresShutDown();
	return (buffer == NULL) ? FAILRULE_BIT : NOPROBLEM_BIT; // special call requesting error return (not done in script)
}

FunctionResult DBInitCode(char* buffer)
{
	char query[MAX_WORD_SIZE * 2];
	char* ptr = SkipWhitespace(ARGUMENT(1));
	if (!strnicmp(ptr,"EXISTS ",7))
	{
		ptr = ReadCompiledWord(ptr,query);
		if (conn) return NOPROBLEM_BIT;
	}
	if (conn) 
	{
		char* msg = "DB already opened\r\n";
		SetUserVariable((char*)"$$db_error",msg);	// pass along the error
		if (trace & TRACE_SQL && CheckTopicTrace()) Log(STDTRACELOG,msg);
 		return FAILRULE_BIT;
	}
	FunctionResult result;
	*query = 0;
	FreshOutput(ptr,query,result,0, MAX_WORD_SIZE * 2);
	if (result != NOPROBLEM_BIT) return result;
	if (!stricmp(query,(char*)"null"))
	{
		connDummy = true;
		return NOPROBLEM_BIT;
	}

#ifdef WIN32
	if (InitWinsock() == FAILRULE_BIT)
	{
		if (trace & TRACE_SQL && CheckTopicTrace()) Log(STDTRACELOG, "WSAStartup failed\r\n");
		return FAILRULE_BIT;
	}
#endif

    /* Make a connection to the database */
    conn = PQconnectdb(query);

    /* Check to see that the backend connection was successfully made */
    if (PQstatus(conn) != CONNECTION_OK)
    {
		char msg[MAX_WORD_SIZE];
		sprintf(msg,(char*)"%s - %s\r\n",query,PQerrorMessage(conn));
		SetUserVariable((char*)"$$db_error",msg);	// pass along the error
        if (trace & TRACE_SQL && CheckTopicTrace()) Log(STDTRACELOG, "Connection failed: %s",  msg);
		return DBCloseCode(NULL);
    }

	return NOPROBLEM_BIT;
}

char hexbytes[] =  {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

static void AdjustQuotes(char* fix,bool nocloser)
{
	char* start = fix;
	while ((fix = strchr(fix,'\''))) 
	{
		char* end = fix + 1;
		while ((end = strchr(end,'\''))) // finding end
		{
			if (end[1] == ' ' || end[1] == ';' || end[1] == '\t' || end[1] == '\n' || end[1]  == '\r' || end[1]  == ')'|| end[1]  == '('|| end[1]  == ',') // real end of token
			{
				fix = end;
			}
			else // internal ' needs converting
			{
				memmove(fix+1,fix,strlen(fix)+1); 
				fix++; // should we find no real end, this will move us past
			}
			break;
		}
		if (!end && nocloser)
		{
			memmove(fix+1,fix,strlen(fix)+1); 
			fix++; // should we find no real end, this will move us past
		}
		++fix; // always make progress
	}
}

void PGUserFilesCloseCode()
{
	if (!usersconn) return;

	conn = usersconn;
	FunctionResult result = DBCloseCode(NULL);
	InitUserFiles(); // default back to normal filesystem
	usersconn = NULL;
	FreeBuffer();
	pgfilesbuffer = 0;
}

FILE* pguserCreate(const char* name)
{
	strcpy(pguserFilename,name);
	return (FILE*)pguserFilename;
}

FILE* pguserOpen(const char* name)
{
	strcpy(pguserFilename,name);
	return (FILE*)pguserFilename;
}

int pguserClose(FILE*)
{
	return 0;
}

static size_t convertFromHex(unsigned char* ptr,unsigned char* from)
{
	unsigned char* start = ptr;
	*ptr = 0;
	if (!from || !*from) return 0;
	if (*from++ != '\\') return 0;
	else if (*from++ != 'x') return 0;

	while (*from)
	{
		unsigned char c = *from++;
		unsigned char d = *from++;
		c = (c <= '9') ? (c - '0') : (c - 'a' + 10); 
		d = (d <= '9') ? (d - '0') : (d - 'a' + 10); 
		*ptr++ = (c << 4) | d;
		if ((ptr-start+10) > (int)userCacheSize)
		{
			*start = 0;
			ReportBug("User %s file too big",loginID);
			return 0;
		}
	}
	*ptr = 0;
	return ptr - start;
}

size_t pguserRead(void* buf,size_t size, size_t count, FILE* file)
{
	char* buffer = (char*)buf;
	sprintf(buffer,(char*)"SELECT file FROM userfiles WHERE userid = '%s' ;",pguserFilename); // one table per user
	PGresult   *res = PQexec(usersconn, (const char*)buffer);  
	int status = (int) PQresultStatus(res);
	char* msg = PQerrorMessage(usersconn);
	if (status == PGRES_BAD_RESPONSE ||  status == PGRES_FATAL_ERROR || status == PGRES_NONFATAL_ERROR)
    {
		char* msg = PQerrorMessage(conn);
        PQclear(res);
		return 0;
    }
	size = 0;
	*buffer = 0;
	// process answers
	if (status == PGRES_TUPLES_OK && PQntuples(res) == 1)  // shall only be 1 record matching at most and we get back just one field
	{
		size = convertFromHex((unsigned char*)buffer,(unsigned char*)PQgetvalue(res, 0, 0));
	}
	PQclear(res);
	return size;
}

static void convert2Hex(unsigned char* ptr, size_t len, unsigned char* buffer,unsigned int& before,  unsigned int& after)
{
	unsigned char* start = buffer;
	sprintf((char*)buffer,(char*)"INSERT into userfiles VALUES ('%s', ",pguserFilename); 
	buffer += strlen((char*) buffer);
	before = buffer-start;
	strcpy((char*)buffer,(char*)"E'\\\\x");
	buffer += strlen((char*) buffer);
	while (len--)
	{
		unsigned char first = (*ptr) >> 4;
		unsigned char second = *ptr++ & 0x0f;
		*buffer++ = hexbytes[first];
		*buffer++ = hexbytes[second];
	}
	*buffer++ = '\'';
	*buffer++ = ' ';
	after = buffer-start;
	sprintf((char*)buffer,(char*)" );");
 }
 	
size_t pguserWrite(const void* buf,size_t size, size_t count, FILE* file)
{
	unsigned int before, after;
	unsigned char* buffer = (unsigned char*)buf;
	convert2Hex(buffer, size * count,(unsigned char*) pgfilesbuffer,before,after); // does an update
	PGresult   *res = PQexec(usersconn, pgfilesbuffer);  // do insert first (which may fail or succeed)-- want upsert pending postgres 9.5
	int status = (int) PQresultStatus(res);
	char* msg = PQerrorMessage(usersconn);
	PQclear(res);
	if (status == PGRES_FATAL_ERROR) // we dont already have a record
	{
		sprintf((char*)pgfilesbuffer,(char*)"UPDATE userfiles SET file = "); 
		char* at = pgfilesbuffer + strlen(pgfilesbuffer);
		memset(at,' ',(pgfilesbuffer+before-at)); // blank fill
		sprintf(pgfilesbuffer+after,(char*)" WHERE userid = '%s' ;",pguserFilename);
		res = PQexec(usersconn,pgfilesbuffer);  
		status = (int) PQresultStatus(res);
		msg = PQerrorMessage(usersconn);
 		PQclear(res);
	}

    if (status == PGRES_BAD_RESPONSE ||  status == PGRES_FATAL_ERROR || status == PGRES_NONFATAL_ERROR) 
	{
		ReportBug("Postgres filessys write failed for %s",pguserFilename);
	}
	return size * count;
}

void PGUserFilesCode()
{
#ifdef WIN32
	if (InitWinsock() == FAILRULE_BIT) ReportBug((char*)"FATAL: WSAStartup failed\r\n");
#endif
    /* Make a connection to the database */
	char query[MAX_WORD_SIZE];
	sprintf(query,(char*)"%s dbname = users ",postgresparams); 
    usersconn = PQconnectdb(query);
    if (PQstatus(usersconn) != CONNECTION_OK) // users not there yet...
    {
		sprintf(query,(char*)"%s dbname = postgres ",postgresparams);
		usersconn = PQconnectdb(query);
		ConnStatusType statusType = PQstatus(usersconn);
		if (statusType != CONNECTION_OK) // cant reach postgres
		{
			DBCloseCode(NULL);
			ReportBug((char*)"FATAL: Failed to open postgres db %s and root db postgres",postgresparams);
		}
  
		PGresult   *res  = PQexec(usersconn, "CREATE DATABASE users;");
		int status = (int) PQresultStatus(res);
		char* msg;
		if (status == PGRES_BAD_RESPONSE ||  status == PGRES_FATAL_ERROR || status == PGRES_NONFATAL_ERROR) msg = PQerrorMessage(usersconn);
		if (PQstatus(usersconn) != CONNECTION_OK) // cant reach postgres
		{
			DBCloseCode(NULL);
			ReportBug((char*)"FATAL: Failed to open users db %s",postgresparams);
		}
		
		sprintf(query,(char*)"%s dbname = users ",postgresparams);
		usersconn = PQconnectdb(query);
		if (PQstatus(usersconn) != CONNECTION_OK) // users not there yet...
		{
			DBCloseCode(NULL);
			ReportBug((char*)"FATAL: Failed to open users db %s",postgresparams);
		}
	}
	
	// these are dynamically stored, so CS can be a DLL.
	pgfilesbuffer = AllocateBuffer();  // stays globally locked down - may not be big enough
	userFileSystem.userCreate = pguserCreate;
	userFileSystem.userOpen = pguserOpen;
	userFileSystem.userClose = pguserClose;
	userFileSystem.userRead = pguserRead;
	userFileSystem.userWrite = pguserWrite;
	userFileSystem.userDelete = NULL;
	filesystemOverride = POSTGRESFILES;
	
    PGresult   *res  = PQexec(usersconn, "CREATE TABLE userfiles (userid varchar(400) PRIMARY KEY, file bytea);");
	int status = (int) PQresultStatus(res);
	char* msg;
	if (status == PGRES_BAD_RESPONSE ||  status == PGRES_FATAL_ERROR || status == PGRES_NONFATAL_ERROR)  msg = PQerrorMessage(usersconn);
	msg = NULL;
}

FunctionResult DBExecuteCode(char* buffer)
{
	if (!conn) 
	{
		if (connDummy) return NOPROBLEM_BIT;
		if (buffer)
		{
			char* msg = "DB not opened\r\n";
			SetUserVariable((char*)"$$db_error",msg);	// pass along the error
			if (trace & TRACE_SQL && CheckTopicTrace()) Log(STDTRACELOG,msg);
		}
		return FAILRULE_BIT;
	}

	char* arg1 = ARGUMENT(1);
	PGresult   *res;
	FunctionResult result = NOPROBLEM_BIT;

	char query[MAX_WORD_SIZE * 2];
	char fn[MAX_WORD_SIZE];
	char* ptr = GetCommandArg(arg1,buffer,result,OUTPUT_NOQUOTES); 
	if (result != NOPROBLEM_BIT) return result;
	if (strlen(buffer) >= (MAX_WORD_SIZE * 2)) return FAILRULE_BIT;
	strcpy(query,buffer);
	ReadShortCommandArg(ptr,fn,result,OUTPUT_NOQUOTES); 
	if (result != NOPROBLEM_BIT) return result;

	// convert \" to " within params
	char* fix;
	while ((fix = strchr(query,'\\'))) memmove(fix,fix+1,strlen(fix)); // remove protective backslash

	// fix ' to '' inside a param
	AdjustQuotes(query,false);

	// adjust function reference name
	char* function = fn;
	if (*function == '\'') ++function; // skip over the ' 

	unsigned int argflags = 0;
	WORDP FN = (*function) ? FindWord(function) : NULL;
	if (FN) argflags = FN->x.macroFlags;

	if (trace & TRACE_SQL && CheckTopicTrace()) Log(STDTRACELOG, "DBExecute command %s\r\n", query);
    res = PQexec(conn, query);
	int status = (int) PQresultStatus(res);
    if (status == PGRES_BAD_RESPONSE ||  status == PGRES_FATAL_ERROR || status == PGRES_NONFATAL_ERROR)
    {
		char* msg = PQerrorMessage(conn);
		if (buffer)
		{
			SetUserVariable((char*)"$$db_error",msg);	// pass along the error
			if (trace & TRACE_SQL && CheckTopicTrace()) Log(STDTRACELOG, "DBExecute command failed: %s %s status:%d\r\n", arg1,msg,status);
		}
        PQclear(res);
		return FAILRULE_BIT;
     }
	char* psBuffer = AllocateStack(NULL,MAX_BUFFER_SIZE);
	if (*function && status == PGRES_TUPLES_OK) // do something with the answers
	{
		psBuffer[0] = '(';
		psBuffer[1] = ' ';
	
		// process answers
		unsigned int limit = (unsigned int) PQntuples(res);
		unsigned int fields = (unsigned int) PQnfields(res);

		for (unsigned int i = 0; i < limit; i++) // for each row
		{
			char* at = psBuffer+2;
			for (unsigned int j = 0; j < fields; j++) 
			{
				// char *PQfname(const PGresult *res,int column_number); // get colum name
				// int PQfnumber(const PGresult *res, const char *column_name);
				Oid type = PQftype(res, j);
				bool keepQuotes = (argflags & ( 1 << j)) ? 1 : 0; // want to use quotes 

				*at = 0;
				char* val = PQgetvalue(res, i, j);
				size_t len = strlen(val);
				if (len > (maxBufferSize - 100))  // overflow
				{
					PQclear(res);
					ReleaseStack(psBuffer); // short term
					return FAILRULE_BIT;
				}
				if (keepQuotes)
				{
					*at++ = '"';
					strcpy(at,val);
					char* x = at;
					while ((x = strchr(x,'"'))) // protect internal quotes
					{
						memmove(x+1,x,strlen(x)+1);
						*x = '\\';
						x += 2;
					}
					at += strlen(at);
					*at++ = '"';
				}
				else // normal procesing
				{
					sprintf(at,(char*)"%s",val);
					at += strlen(at);
				}
				*at++ = ' ';

				if ((unsigned int)(at - psBuffer) > (maxBufferSize - 100)) 
				{
					ReportBug((char*)"postgres answer overflow %s -> %s\r\n",query,psBuffer);
					break;
				}
			}
			*at = 0;
			strcpy(at,(char*)")"); //  ending paren
			if (trace & TRACE_SQL && CheckTopicTrace()) Log(STDTRACELOG, "DBExecute results %s\r\n", psBuffer);
	
 			if (stricmp(function,(char*)"null")) DoFunction(function,psBuffer,buffer,result); 
			buffer += strlen(buffer);
			if (result != 0) 
			{
				if (result == UNDEFINED_FUNCTION) result = FAILRULE_BIT;
				char msg[MAX_WORD_SIZE];
				sprintf(msg,(char*)"Failed %s%s\r\n",function,psBuffer);
				SetUserVariable((char*)"$$db_error",msg);	// pass along the error
 				if (trace & TRACE_SQL && CheckTopicTrace()) Log(STDTRACELOG,msg);
				break; // failed somehow
			}
		}
	}
	ReleaseStack(psBuffer); // short term
	PQclear(res);
	return result;
} 

#endif
	