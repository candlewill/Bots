#include <string.h>
#include <unistd.h>

#include "Dialog.h"

#define MAX_WORD_SIZE 3000
#define INPUT_BUFFER_SIZE 80000
#define ID_SIZE 500

int main(int argc, char * argv[])
{
	chdir((char*)"..");
	ChatBot Bot;

	Bot.InitBot(argc, argv);

	ParameterList* parameter;

	parameter = Bot.GetParameters();

	char user[MAX_WORD_SIZE];

	char loginID[ID_SIZE];
	strcpy(loginID, parameter->loginID);

	char ourMainInputBuffer[INPUT_BUFFER_SIZE];

	char * ourMainOutputBuffer;

	if (!*loginID)
	{
		// loginID 为空才执行
		printf((char*)"%s", (char*)"\r\nEnter user name: ");
		Bot.ReadLine(user, stdin);
		printf((char*)"%s", (char*)"\r\n");
		if (*user == '*') // let human go first  -   say "*bruce
		{
			memmove(user, user + 1, strlen(user));
			printf((char*)"%s", (char*)"\r\nEnter starting input: ");
			Bot.ReadLine(ourMainInputBuffer, stdin);
			printf((char*)"%s", (char*)"\r\n");
		}
	}
	// loginID不为空
	else strcpy(user, loginID);

	//Bot.SetParameters(user, computerID, ourMainInputBuffer);
	Bot.Chat(user, ourMainInputBuffer, ourMainOutputBuffer); // unknown bot, no input,no ip

retry:
	Bot.ProcessInput();
	*ourMainInputBuffer = 0;
	ourMainInputBuffer[1] = 0;
	goto retry;

}