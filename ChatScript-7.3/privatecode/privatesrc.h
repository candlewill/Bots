#ifndef PRIVATESRCH
#define PRIVATESRCH

/*
privatesrc.h: header file. It must at least declare:

void PrivateInit(char* params);  called on startup of CS, passed param: private=
void PrivateRestart(); called when CS is restarting
void PrivateShutdown();   called when CS is exiting.

privatetestingtable.cpp  listing of :debug functions made visible to CS
Debug table entries like this:

{(char*) ":endinfo", EndInfo,(char*)"Display all end information"},
*/
// #include "../SRC/common.h"

void PrivateInit(char* params);  // called on startup of CS, passed param : private =
void PrivateRestart(); // called when CS is restarting
void PrivateShutdown();  // called when CS is exiting.

static FunctionResult CNSegmentCode(char* buffer);

/*
static void C_CNSegment(char* input);
*/
#endif