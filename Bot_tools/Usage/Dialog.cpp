#include "common.h" 
#include "evserver.h"

#include "Dialog.h"

unsigned int ChatBot::InitBot(int argcx, char * argvx[])
{
	InitSystem(argcx, argvx);
}

void ChatBot::CloseBot()
{
	CloseSystem();
}

int ChatBot::Chat(char* _user,char* incoming, char* output)
{
	strcpy(ourMainInputBuffer, incoming);
	PerformChat(_user, computerID, ourMainInputBuffer, NULL, ourMainOutputBuffer);
	
	output = (char*)malloc(outputsize);
	strcpy(output, ourMainOutputBuffer);
}

void ChatBot::ProcessInput()
{
	ProcessInputFile();
}

int ChatBot::ReadLine(char* buffer, FILE* in)
{
	ReadALine(buffer, in);
}

void ChatBot::SetParameters(char* _user, char* _loginID, char* _computerID, char* _ourMainInputBuffer)
{
	char user[MAX_WORD_SIZE];
	strcpy(user, _user); // array
	strcpy(loginID, _loginID);
	strcpy(computerID , _computerID);
	strcpy(ourMainInputBuffer, _ourMainInputBuffer);
}

ParameterList* ChatBot::GetParameters()
{	
	ParameterList *result = new ParameterList();

	*(result->ourMainInputBuffer) = 0;
	strcpy(result->loginID, loginID);
	strcpy(result->computerID, computerID);
	printf(computerID);
	printf("<----------------\n");

	return result;
}