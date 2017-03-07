#ifndef _TESTINGH
#define _TESTINGH
#ifdef INFORMATION
Copyright (C) 2011-2017 by Bruce Wilcox

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#endif

#define TESTING_REPEATALLOWED 1000

#ifndef DISCARDTESTING

typedef void (*COMMANDPTR)(char* input);

typedef struct CommandInfo 
{
	const char* word;			// dictionary word entry
	COMMANDPTR fn;				// function to use to get it
	const char* comment;		// what to say about it
} CommandInfo;

extern CommandInfo commandSet[];

void InitCommandSystem();
TestMode Command(char* input,char* output,bool fromScript);
int CountSet(WORDP D,unsigned int baseStamp);

void Sortit(char* name,int oneline);
void SortTopic(WORDP D,uint64 junk);
void SortTopic0(WORDP D,uint64 junk);
void C_MemStats(char* input);
int Debugger(char* x);
void CheckBreak(char* name,bool in,char* code = NULL,FunctionResult result = NOPROBLEM_BIT);
void CheckAssignment(char* name, char* value);
void CheckAbort(char* msg);
void InitDebugger();
void CheckRuleOutput(int topic, char* label, char* code);
int ProcessAction(char* before, char* after, char* output, FunctionResult result);
#endif

TestMode DoCommand(char* input,char* output,bool authorize=true);
bool VerifyAuthorization(FILE* in);

#endif
