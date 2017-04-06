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

int ChatBot::Chat(char* _user, char* incoming,  char* output)
{
	int turn = 0;
	strcpy(ourMainInputBuffer+1, incoming);
	turn = PerformChat(_user, computerID, ourMainInputBuffer, NULL, ourMainOutputBuffer);

	strcpy(output, ourMainOutputBuffer);

	return turn;
}

void ChatBot::ProcessInput()
{
	ProcessInputFile();
}

int ChatBot::ReadLine(char* buffer, FILE* in, unsigned int limit)
{
	if (limit == 0)
		ReadALine(buffer, in);
	else
		ReadALine(buffer, in, limit);
}

void ChatBot::SetParameters(char* _user, char* _loginID, char* _computerID, char* _ourMainInputBuffer)
{
	char user[MAX_WORD_SIZE];
	strcpy(user, _user); // array
	strcpy(loginID, _loginID);
	strcpy(computerID, _computerID);
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

char* ChatBot::ProcessOutput(char* bufferfrom)
{
	if ((!silent) && (bufferfrom != NULL) && (bufferfrom[0] == '\0'))
		return UTF2ExtendedAscii(bufferfrom);
	else
		return (char*)"";
}

void ChatBot::ResetInput()
{
	*ourMainInputBuffer = 0;
	ourMainInputBuffer[1] = 0;
}

char* ChatBot::HandleOOB(char* buffer)
{
	if ((!silent) && (buffer != NULL) && (buffer[0] == '\0'))
	{
		ProcessOOB(buffer);
	}
	callBackDelay = 0; // now turned off after an output
	return buffer;
}

void ChatBot::NewDialog() // 开始新的对话
{
	*ourMainInputBuffer = ' '; // leave space at start to confirm NOT a null init message, even if user does only a cr
	ourMainInputBuffer[1] = 0;

	if (loopBackDelay)
		loopBackTime = ElapsedMilliseconds() + loopBackDelay; // resets every output
}

void ChatBot::Reboot()
{
	ourMainInputBuffer[0] = ourMainInputBuffer[1] = 0;
	Restart();
}