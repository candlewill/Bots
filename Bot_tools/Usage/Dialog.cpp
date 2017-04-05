#include "common.h" 
#include "evserver.h"

// char sourceInput_1[200];

void ChatLoop() //   local machine loop
{
	
	char user[MAX_WORD_SIZE];
	*ourMainInputBuffer = 0;
	sourceFile = stdin;
	// if (*sourceInput_1)	sourceFile = FopenReadNormal(sourceInput_1);
	// else 
	if (userInitFile) sourceFile = userInitFile;
	if (!*loginID)
	{
		printf((char*)"%s", (char*)"\r\nEnter user name: ");
		ReadALine(user, stdin);
		printf((char*)"%s", (char*)"\r\n");
		if (*user == '*') // let human go first  -   say "*bruce
		{
			memmove(user, user + 1, strlen(user));
			printf((char*)"%s", (char*)"\r\nEnter starting input: ");
			ReadALine(ourMainInputBuffer, stdin);
			printf((char*)"%s", (char*)"\r\n");
		}
	}
	else strcpy(user, loginID);
	PerformChat(user, computerID, ourMainInputBuffer, NULL, ourMainOutputBuffer); // unknown bot, no input,no ip
	printf("%s %s", "output: ", ourMainOutputBuffer);

retry:
	ProcessInputFile();
	sourceFile = stdin;
	*ourMainInputBuffer = 0;
	ourMainInputBuffer[1] = 0;
	if (!quitting) goto retry;
}

int main(int argc, char * argv[])
{
	for (int i = 1; i < argc; ++i)
	{
		if (!strnicmp(argv[i], "root=", 5))
		{
			chdir((char*)argv[i] + 5);
		}
	}

	FILE* in = FopenStaticReadOnly((char*)"SRC/dictionarySystem.h"); // SRC/dictionarySystem.h
	if (!in) // if we are not at top level, try going up a level
	{
		chdir((char*)"..");
	}
	else FClose(in);
	if (InitSystem(argc, argv)) myexit((char*)"failed to load memory\r\n");
	if (!server)
	{
		quitting = false; // allow local bots to continue regardless
		// MainLoop();
		ChatLoop();
	}
	else if (quitting) { ; } // boot load requests quit
	CloseSystem();
}
