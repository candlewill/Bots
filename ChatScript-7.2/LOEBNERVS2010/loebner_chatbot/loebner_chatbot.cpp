// loebner_chatbot.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "loebner_chatbot.h"
#include <Shlobj.h>
#include <stdio.h>
#include <string.h>
#include "..\..\src\common.h"
#define MAX_LOADSTRING 100
static char* computerName = "Rose";
static 	char* pathname;
char inputMessage[30000];
char* msgPtr = inputMessage;
char response[30000];
static 	HWND hwnd;
char lastName[1000];
UINT startid;

int delays[] ={

	182,53,97,211,45,127,137,67,188,162,
	188,149,168,70,166,96,226,154,188,86,
	330,121,154,165,161,147,131,121,188,104,
	396,78,128,140,240,66,121,112,181,89,
};
int spaceDelays[] = {
	93,78,125,125,156,156,63,203,94,93,250,109,109,
	171,125,156,78,78,109,156,78,62,94,62,141,78,280,
	109,125,94,156,109,156,78,468,79,
	93,78,125,125,156,156,63,203,94,93,250,109,109,
	};
int periodDelays[] = {
234,312,172,124,234,312,172,124,234,312,172,124,
234,312,172,124,234,312,172,124,234,312,172,124,
234,312,172,124,234,312,172,124,234,312,172,124,
};

int delayIndex = 0;


//#include <windows.h>
static char specialChar[] = 
{
'{',
'}',
'[',
']',
'(',
')',
' ',
',',
'.',
'>',
'<',
'/',
'\\',
'|',
'"',
 '\'',
'\t',
'=',
'_',
'+',
'-',
'!',
'@',
'#',
'$',
'%',
'*',
'^',
'~',
'`',
'&',
'\n',
':',
';',
'?',
8,
0,
};

static char* specialName[] =
{
"braceleft",
"braceright",
"bracketleft",
"bracketright",
"parenleft",
"parenright",
"space",
"comma",
"period",
"greater",
"less",
"slash",
"backslash",
"bar",
"quotedbl",
"quoteright",
"Tab",
"equal",
"underscore",
"plus",
"minus",
"exclam",
"at",
"numbersign",
"dollar",
"percent",
"asterisk",
"asciicircum",
"asciitilde",
"quoteleft",
"ampersand",
"Return",
"colon",
"semicolon",
"question",
"BackSpace",
NULL
};

static int sequence = 0;

void InitChatbot(char* name)
{
	// initialize system
	InitSystem(0,NULL);
	InitStandalone();
	SetUserVariable("$loebner","1");

	// treat each invocation as a new judge
	FILE* in = fopen("counter.txt","rb");
	int judgeid = 0;
	char buffer[1000];
	if (in)
	{
		fread(buffer,1,100,in);
		fclose(in);
		judgeid = atoi(buffer);
	}
	++judgeid;
	FILE* out = fopen("counter.txt","wb");
	fprintf(out,"%d\r\n",judgeid);
	fclose(out);

	// read in last sequence id to hopefully send ok on restart.
	in = fopen("sequence", "rb");
	if (in)
	{
		fgets(buffer,1000,in);
		fclose(in);
		sequence = atoi(buffer);
	}
	
	sprintf(buffer,"judge%d",judgeid);
    strcpy(loginID,buffer);
	ourMainInputBuffer[0] = 0;
	PerformChat(loginID,"",ourMainInputBuffer,NULL,ourMainOutputBuffer);     // start up chat with a judge, swallow first message
	ProcessOOB(ourMainOutputBuffer); // process relevant out of band messaging and remove
}

void Stall(int ms)
{
	ms -= 20;
	if (ms < 40) ms = 40;
	DWORD start = GetTickCount();
	while (1)
	{
		int stall = GetTickCount()- start;
		if (stall > ms) break;
	}
}

bool SendChar(char c,char next,int inputCount)
{
	static char lastChar = 0;
	bool answer = true;
	int delay;
	if ( c == ' ') delay = spaceDelays[delayIndex++];
	else if ( c == '.' || c == ',') delay = spaceDelays[delayIndex];
	else if ( c == '-') delay = 300;
	else  delay = spaceDelays[delayIndex++];
	delay = (delay * 2) / 3; // speed it up a bit
	if (lastChar == '.' || lastChar == ',') delay += spaceDelays[delayIndex] / 5;
	if ( (inputCount % 7) == 0) delay += spaceDelays[delayIndex] / 2; 
	lastChar = c;
	if (delayIndex == 30) delayIndex = 0;
	delay = (4 * delay) / 5;	// 20% faster
	Stall(delay);// uneven typing delays

	int returnValue; 
	char word[1000];
	int i = 0;
	while (specialChar[i])
	{
		if (c == specialChar[i]) 
		{
			sprintf(word,"%s\\%010d.%s.other",pathname,sequence++,specialName[i]);
			returnValue = CreateDirectory(word, NULL);
			// hesitate after sentences conjoined
			if ((c == '.' || c == '!' || c == '?') && next == ' ') Stall(200);
			return answer;
		}
		++i;
	}

	// not a special character
	if ( c >= 'A' && c <= 'Z');
	else if ( c >= 'a' && c <= 'z');
	else if ( c >= '0' && c <= '9');
	else return answer; // a nonalpha char we dont care about

	sprintf(word,"%s\\%010d.%c.other",pathname,sequence++,c);
	returnValue = CreateDirectory(word, NULL);

	return answer;
}

char ReadChar()
{
	// find filename of lowest value from judge
	HANDLE fh;
	char	 path[MAX_PATH];
	sprintf(path,"%s/*",pathname);
retry:

	// read all file names to find the least one
	uint64 data[ 6400 / 64]; // force alignment of memory block
	_WIN32_FIND_DATAA* fd = (_WIN32_FIND_DATAA*)data;
	fh = FindFirstFile((LPCSTR) path,fd);
	char bestName[1000];
	strcpy(bestName,"z"); // higher than any sequence number
	char badname[1000];
	strcpy(badname,"z");
	char name[1000];
	if (fh != INVALID_HANDLE_VALUE)
	{
		int n = 0;
		do
		{
			strcpy(name,fd->cFileName);
			char* at = strchr(name,'.'); // end of sequence
			if (!at || at == name) continue;
			char* judge = strchr(at+1,'.'); // end of char 
			if (!judge) continue;

			if (stricmp(judge+1,"judge")) continue;	 // wrong player

			// this is a judge file-- is it old or new and if new, is it the earliest one
			// < means name1 less than name2
			if ( *lastName != 'z' && strcmp(name,lastName) <= 0)   // shouldnt be happening
			{
				strcpy(badname,name);
				break;
			}
			else if ( strcmp(name,bestName) < 0) strcpy(bestName,name); // found earlier name
		}
		while(FindNextFile(fh,fd));
		FindClose(fh);
	}

	if (*badname != 'z') // did see an out of sequence character from before? Get rid of it
	{
		sprintf(path,"%s/%s",pathname,badname); // failed to delete before?
		int ok = RemoveDirectory(path);
		if (!ok) RemoveDirectory(path); // try again
		goto retry;
	}

	if (*bestName == 'z') return 0;	// nothing found

	char* at = strchr(bestName,'.')+1; // find end of sequencer
	char* period = strchr(at,'.');		// find end of character

	// found a character
	strcpy(lastName,bestName);	// prevent repeat even if delete not yet happened
	sprintf(path,"%s/%s",pathname,bestName);
	int ok = RemoveDirectory(path);
	if (!ok) RemoveDirectory(path); // try again
	*period = 0;
	int i = 0;
	while (specialName[i])
	{
		if (!stricmp(at,specialName[i])) return specialChar[i];
		++i;
	}
	return *at;
}

#include <direct.h>
#define GetCurrentDir _getcwd

VOID CALLBACK TimeCheck( HWND hwnd, UINT uMsg,UINT_PTR idEvent,DWORD dwTime) // every 50 ms
{
	char c;
	static int inputCount = 0;

	static int counter = 0;
	++counter; // increament each timer interrupt, tracking how long since last key input from user
	static char lastchar = 0;
	static bool keyhit = false;
	while ((c = ReadChar())) // returns 0 if nothing found
	{
		keyhit = true; // user input seen since last output
		RECT rect;
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, TRUE);
		if ( c == 8) // backspace
		{
			if ( msgPtr != inputMessage) *--msgPtr = 0;
		}
		else if ( c == '\n' || c == '\r') // prepare for NEW input and get response
		{
			if (msgPtr == inputMessage) continue;	// ignore empty lines
			*msgPtr = 0;
			counter = 100;	// treat as end of input
			break;	// dont read until we have responded
		}
		else if (msgPtr == inputMessage && (c == ' ' || c == '\t' )) continue;	// ignore leading whitespace.
		else // accept new character
		{
			*msgPtr++ = c;
			lastchar = c;
			*msgPtr = 0;
		}
		counter = 0; // start time wait over again
	}

	// do we have something we want to respond to?
	bool trigger = false;
	if (counter >= 30 && *inputMessage && (lastchar == '.' || lastchar == '?' || lastchar == '!')) trigger = true; // 3 sec passed and have input ended on closer 
	else if (counter >= 70 && *inputMessage) trigger = true; // 7 seconds + have lingering input
	// or timer oob goes off
	char oob[MAX_WORD_SIZE];
	*oob = 0;
	ProcessInputDelays(oob,keyhit); 

	if (trigger || *oob)
	{
		if (*oob) strcpy(ourMainInputBuffer,oob); // priority is to alarm callback. others will be only if no user input anyway
		else  
		{
			strcpy(ourMainInputBuffer,inputMessage);
			// clear all signs of user input
			lastchar = 0;	
			*inputMessage = 0;
			msgPtr = inputMessage;
			keyhit = false;
		}

		PerformChat(loginID,computerID,ourMainInputBuffer,NULL,ourMainOutputBuffer);
		strcpy(response,ourMainOutputBuffer);
		callBackDelay = 0; // now turned off after an output
		ProcessOOB(ourMainOutputBuffer); // process relevant out of band messaging and remove

		char word[MAX_WORD_SIZE];
		char* p = SkipWhitespace(ourMainOutputBuffer);
		ReadCompiledWord(p,word);
		if (!*word) strcpy(p,"huh?"); // in case we fail to generate output

		// transmit message to user
		++inputCount;
		while (*p) 
		{
			if (SendChar(*p,p[1],inputCount)) p++; // got sent
		}
		SendChar('\n',0,inputCount);
		if (loopBackDelay) loopBackTime = ElapsedMilliseconds() + loopBackDelay; // resets every output

		// write out last sequence id
		FILE* out = fopen("sequence", "wb");
		fprintf(out,"%d",sequence);
		fclose(out);

		counter = 0;
		RECT rect;
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, TRUE);
	}
}

VOID CALLBACK StartCheck( HWND hwnd, UINT uMsg,UINT_PTR idEvent,DWORD dwTime) 
{
	KillTimer(hwnd,startid);
	static bool init = false;
	if (!init)
	{
		init = true;
		InitChatbot(computerName);
		RECT rect;
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, TRUE);
		strcpy(response,"Chatbot initialization complete");
		UINT id = SetTimer ( hwnd, 1, 50, TimeCheck ); // 50 ms timeouts
	}
}


// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	computerName = lpCmdLine; // name of computer identity to use
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LOEBNER_CHATBOT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LOEBNER_CHATBOT));
	strcpy(lastName,"z");
	BROWSEINFO bi = { 0 };
    bi.lpszTitle = _T("Pick a Chatbot Communication Directory ");
    LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
    TCHAR path[MAX_PATH];
	path[0] = 0;
	pathname = path;
    if ( pidl != 0 )
    {
        // get the name of the folder
        if ( SHGetPathFromIDList ( pidl, path ) )
        {
            printf ( "Selected Folder: %s\n", pathname );
        }

        // free memory used
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
        {
            imalloc->Free ( pidl );
            imalloc->Release ( );
        }
    }
	
	hwnd = FindWindow(szWindowClass,NULL);
	RECT rect;
	GetClientRect(hwnd, &rect);
	InvalidateRect(hwnd, &rect, TRUE);

	startid = SetTimer ( hwnd, 1, 50, StartCheck ); 
	strcpy(response,"Chatbot beginning initialization");

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LOEBNER_CHATBOT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_LOEBNER_CHATBOT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (*inputMessage) TextOut(hdc, 20, 100, inputMessage, _tcslen(inputMessage));  // partial input in progress
		else  TextOut(hdc, 20, 100, ourMainInputBuffer, _tcslen(ourMainInputBuffer));  // last input seen
		TextOut(hdc, 20, 130, response, _tcslen(response)); // current response
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
