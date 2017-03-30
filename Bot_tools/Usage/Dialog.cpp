#include "common.h" 
#include "evserver.h"

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
		MainLoop();
	}
	else if (quitting) { ; } // boot load requests quit
	CloseSystem();
}
