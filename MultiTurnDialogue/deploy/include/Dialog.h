#ifndef __DAILOG_H__
#define __DAILOG_H__
#include <stdio.h>

class ChatBot
{

public:
	unsigned int InitBot(char* filePath);  // this work mostly only happens on first startup, not on a restart
	
	void CloseBot();

	int ReadLine(char* buffer, FILE* in, unsigned int limit = 0);

	int Chat(char* user, char* incoming, char* output); // returns volleycount or 0 if command done or -1 PENDING_RESTART
	
	void ProcessInput();

	void SetParameters(char* user,char* loginID, char* computerID, char* ourMainInputBuffer);

	char* ProcessOutput(char* bufferfrom); // 后处理输出文本

	char* HandleOOB(char* buffer);

	void NewDialog(); // 开始新的对话
};

#endif // !__DAILOG_H__