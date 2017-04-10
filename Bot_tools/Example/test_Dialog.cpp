#include <string.h>
#include <unistd.h>

#include "Dialog.h"

#define MAX_WORD_SIZE 3000
#define INPUT_BUFFER_SIZE 80000
#define ID_SIZE 500
#define PENDING_RESTART -1	// perform chat returns this flag on turn

int main(int argc, char * argv[])
{
	ChatBot Bot;

	Bot.InitBot();

	char user[MAX_WORD_SIZE];

	char loginID[ID_SIZE];
	strcpy(loginID, "user");

	char ourMainInputBuffer[INPUT_BUFFER_SIZE];

	char ourMainOutputBuffer[INPUT_BUFFER_SIZE];

	Bot.Chat(loginID, ourMainInputBuffer, ourMainOutputBuffer); // unknown bot, no input,no ip

	int turn = 0;
	while (true)
	{
		printf(" [Bot]: ");
		printf(ourMainOutputBuffer);
		printf("\n\r");
		printf("[User]: ");

		Bot.NewDialog();
		if (Bot.ReadLine(ourMainInputBuffer, stdin, INPUT_BUFFER_SIZE - 100) < 0) break; // end of input

		turn = Bot.Chat(loginID, ourMainInputBuffer, ourMainOutputBuffer);
	}

}
/*
打开芈月传

[intent: tv_play] 打开芈月传
*/