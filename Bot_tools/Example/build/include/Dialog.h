#ifndef __DAILOG_H__
#define __DAILOG_H__
#include <stdio.h>

struct ParameterList {
	char ourMainInputBuffer[80000];
	char loginID[500];
	char computerID[500];
};

class ChatBot
{

public:
	unsigned int InitBot(int argcx, char * argvx[]);  // this work mostly only happens on first startup, not on a restart
	void CloseBot();
	int ReadLine(char* buffer, FILE* in, unsigned int limit = 0);
	int Chat(char* user, char* incoming, char* output); // returns volleycount or 0 if command done or -1 PENDING_RESTART
	void ProcessInput();

	ParameterList* GetParameters();
	void SetParameters(char* user,char* loginID, char* computerID, char* ourMainInputBuffer);

	char* ProcessOutput(char* bufferfrom); // 后处理输出文本

	void ResetInput(); // 重置输入

	char* HandleOOB(char* buffer);

	void NewDialog(); // 开始新的对话

	void Reboot(); // 重启
	
};

#endif // !__DAILOG_H__