#ifndef _UiLib_h
#define _UiLib_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <wincon.h>
#include <float.h>

#define TICKSPERSECOND 18 //WE WILL DETERMINE THIS LATER

typedef struct _consolestruct
{
CONSOLE_SCREEN_BUFFER_INFO coninf;
HANDLE hOutput;
WORD color;
COORD cursor;
INPUT_RECORD InputRecord[10];
HANDLE hInput;
CONSOLE_CURSOR_INFO cursinf;
int linecounter;
}CONSOLE, *PCONSOLE;

typedef struct _timerstruct{
	clock_t tick1;
	clock_t tick2;
	clock_t tickdiff;
}TIMER, *PTIMER;




//UI FUNCTIONS
void initconsole();
void setcolor(WORD);
void cls();
void bufferprint(int, char*);

int waitforkey();
void flushin();
void setcursor(int, int);
void zerocursor();
void writestring(char*);
void fillwithcolor(int);
COORD getconsize();
HANDLE getconsoleoutputh();
HANDLE getconsoleinputh();
int checkforinput();
USHORT getinput(int);
void clearinputrecords();
void DrawHugeNumber(int number, COORD coord);
void onescreenPrint(char* string);
void PrintX(char* string);
void writechar(char chartowrite);
void cursorhide();
void cursorshow();
int retrieveTextCenterH(char* string);
int retrieveTextCenterV();
COORD retrieveTextCenter(char* string);
void StartTimer(PTIMER timer);
void EndTimer(PTIMER timer);
DWORD TimeElapsed(PTIMER timer);
void printline();
DWORD SecondsElapsed(PTIMER timer);
DWORD SplitElapsed(PTIMER timer);
void cleartext(char* text);
void ClearRect(RECT recttoclear);
void clearcells(int numbertoclear, COORD writecoord);
USHORT waitforVkey();
void ClearRect(RECT recttoclear, WORD color);
void cls(WORD color);
POINT ReturnTextCenterInRect(char* text, RECT recttocheck);
WORD getcolor();
void DrawRect(RECT Rect, POINT Origin, WORD Color);

//CONSOLE DEFINES
#define GETCINF GetConsoleScreenBufferInfo(cs.hOutput, &cs.coninf)
#define FLUSH flushin()
#define BLUE setcolor(FOREGROUND_BLUE);
#define LBLUE setcolor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#define RED setcolor(FOREGROUND_RED);
#define LRED setcolor(FOREGROUND_RED | FOREGROUND_INTENSITY);
#define GREEN setcolor(FOREGROUND_GREEN);
#define LGREEN setcolor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#define YELLOW setcolor(FOREGROUND_RED | FOREGROUND_GREEN);
#define LYELLOW setcolor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#define MAGENTA setcolor(FOREGROUND_RED | FOREGROUND_BLUE);
#define LMAGENTA setcolor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#define CYAN setcolor(FOREGROUND_BLUE | FOREGROUND_GREEN);
#define LCYAN setcolor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#define GRAY setcolor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#define WHITE setcolor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
#define BLACK setcolor(0);
#define DGRAY setcolor(FOREGROUND_INTENSITY);


//SOUND FUNCTIONS

void playSYNCsound(char* sound);
void playASYNCsound(char* sound);

//SOUND DEFINES


#define TYPEWRITER 0
#define BEEPA	1
#define BEEPB	2
#define BEEPC	3
#define TWOBEEPS	4
#define TWOBEEPSMOD	5

#endif