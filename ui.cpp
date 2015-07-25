
#include "UiLib.h"

CONSOLE cs;

void initconsole()
{
	cs.hInput = GetStdHandle(STD_INPUT_HANDLE);
	if(cs.hInput)printf("Standard Input Handle OK\n");
	else printf("Standard Input Handle BAD\n");
	cs.hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if(cs.hOutput)printf("Standard Output Handle OK\n");
	else printf("Standard Output Handle BAD\n");
	SetConsoleMode(cs.hInput, ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT | ENABLE_ECHO_INPUT);
	printf("Console Mode SET\n");
	GETCINF;
	printf("Console Mode RECIEVED\n");
	SetConsoleTitle("The Agency - Razor One - Text Prototype"); //////////TITLE - REMOVE BEFORE EXPORT
	printf("Title SET\n");
	GetConsoleCursorInfo(cs.hOutput, &cs.cursinf);
	printf("Cursor Info RECIEVED\n");
	cs.cursinf.bVisible = TRUE;
	cs.cursinf.dwSize = 100;
	SetConsoleCursorInfo(cs.hOutput, &cs.cursinf);		
	printf("Cursor Info SET\n");
	cs.linecounter = 0;
}
void setcolor(WORD attribs)
{
	cs.color = attribs;
	SetConsoleTextAttribute(cs.hOutput, attribs);
}

WORD getcolor()
{
	CONSOLE_SCREEN_BUFFER_INFO coninf;
	GetConsoleScreenBufferInfo(cs.hOutput, &coninf);
	return(coninf.wAttributes);
}

void cursorhide()
{
	cs.cursinf.bVisible = FALSE;
	SetConsoleCursorInfo(cs.hOutput, &cs.cursinf);
}

void cursorshow()
{
	cs.cursinf.bVisible = TRUE;
	SetConsoleCursorInfo(cs.hOutput, &cs.cursinf);
}

void setcursor(int x, int y)
{
	COORD newcoord;

	newcoord.X = x;
	newcoord.Y = y;
	cs.linecounter = y;
	SetConsoleCursorPosition(cs.hOutput, newcoord);
}

void zerocursor()
{
	COORD newcoord;
	
	cs.linecounter = 0;
	newcoord.X = 0;
	newcoord.Y = 0;
	SetConsoleCursorPosition(cs.hOutput, newcoord);
}

void cls(WORD color)
{
		DWORD charswritten;
	DWORD oldcolor;
	cs.cursor.X = 0;
	cs.cursor.Y = 0;
	cs.linecounter = 0;
	SetConsoleCursorPosition(cs.hOutput, cs.cursor);
//	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	GETCINF;
//FillConsoleOutputAttribute(hOutput, FOREGROUND_BLUE | FOREGROUND_INTENSITY, 80*25, cursor, &charswritten);
	oldcolor = cs.color;
	setcolor(0);
FillConsoleOutputCharacter(cs.hOutput, ' ', 80*25, cs.cursor, &charswritten);
FillConsoleOutputAttribute(cs.hOutput, color, 80 * 25, cs.cursor, &charswritten);
setcolor(oldcolor);
}

void cls()
{
//	COORD outcoord;
//	COORD cursor;
	DWORD charswritten;
	DWORD oldcolor;
	cs.cursor.X = 0;
	cs.cursor.Y = 0;
	cs.linecounter = 0;
	SetConsoleCursorPosition(cs.hOutput, cs.cursor);
//	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	GETCINF;
//FillConsoleOutputAttribute(hOutput, FOREGROUND_BLUE | FOREGROUND_INTENSITY, 80*25, cursor, &charswritten);
	oldcolor = cs.color;
	setcolor(0);
FillConsoleOutputCharacter(cs.hOutput, ' ', 80*25, cs.cursor, &charswritten);
setcolor(oldcolor);
	return;
}

void bufferprint(int speed, char* buffer)
//THIS FUNCTION PRINTS OUT A BUFFER AT A GIVEN RATE FOR A "TYPEWRITER" EFFECT.
{
	unsigned int i;
	for (i=0; i < (strlen(buffer)); i++)
	{
		printf("%c", buffer[i]);
		if (speed > 0)
//		PlaySound("sound\\common\\type.wav", NULL, SND_FILENAME | SND_ASYNC);
		Sleep(speed);
	}
	return;
}



int waitforkey()
{
	//HANDLE InputH;
	ULONG count;
	
//	InputH = GetStdHandle(STD_INPUT_HANDLE);
	
//	PlaySound("sound\\common\\beep2.wav", NULL, SND_FILENAME | SND_ASYNC);
	for(;;)
	{
		ReadConsoleInput(cs.hInput, &cs.InputRecord[0], 1, &count);
		if (cs.InputRecord[0].EventType == KEY_EVENT)
		{ 
			
			if(cs.InputRecord[0].Event.KeyEvent.bKeyDown)
				return(cs.InputRecord[0].Event.KeyEvent.uChar.AsciiChar);
		}
	}
}

USHORT waitforVkey()
{
		ULONG count;
	
//	InputH = GetStdHandle(STD_INPUT_HANDLE);
	
//	PlaySound("sound\\common\\beep2.wav", NULL, SND_FILENAME | SND_ASYNC);
	for(;;)
	{
		ReadConsoleInput(cs.hInput, &cs.InputRecord[0], 1, &count);
		if (cs.InputRecord[0].EventType == KEY_EVENT)
		{ 
			
			if(cs.InputRecord[0].Event.KeyEvent.bKeyDown)
				return(cs.InputRecord[0].Event.KeyEvent.wVirtualKeyCode);
		}
	}
}

void flushin()
{
	FlushConsoleInputBuffer(cs.hInput);
}

void writestring(char* string)
{
	
	ULONG written;

	WriteConsole(cs.hOutput, string, strlen(string), &written, NULL);
}

void writechar(char chartowrite)
{

	ULONG written;
	char write[2];
	write[0] = chartowrite;
	write[1] = '\0';

	WriteConsole(cs.hOutput, write, strlen(write), &written, NULL);
}

void fillwithcolor(int color)
{
	ULONG written;

	zerocursor();

	FillConsoleOutputAttribute(cs.hOutput, color, cs.coninf.dwSize.X * cs.coninf.dwSize.Y, cs.cursor,  &written);
}

COORD getconsize()
{
	COORD size;
	size.X = cs.coninf.dwSize.X;
	size.Y = cs.coninf.dwSize.Y;
	return size;
}

HANDLE getconsoleoutputh()
{
	return cs.hOutput;
}

HANDLE getconsoleinputh()
{
	return cs.hInput;
}

int checkforinput()
{
	ULONG numeventsread;
//	int numevents;
//	int i;
	
//	numevents = GetNumberOfConsoleInputEvents(cs->hInput, &numevents);
	//IF WE'VE FOUND SOMETHING ON THE INPUT RECORD USING PEEK, THEN READ IT IN AND REMOVE IT USING READCONSOLEINPUT.

		GetNumberOfConsoleInputEvents(cs.hInput, &numeventsread);
			return numeventsread;	

}

USHORT getinput(int i)
{
	ULONG numeventsread;
	
	if (PeekConsoleInput(cs.hInput, cs.InputRecord, 10, &numeventsread))
	{
		if (cs.InputRecord[i].EventType == KEY_EVENT)
		{
				return cs.InputRecord[i].Event.KeyEvent.wVirtualKeyCode;
		}
	}
	return 0;
}

void clearinputrecords()
{
		memset(&cs.InputRecord, 0, sizeof(INPUT_RECORD) * 10);
}

void DrawHugeNumber(int number, COORD coord)
{
	int i;
	int x;
	char currentnumber[11][13];

	char nonumber[11][13]={
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' '};

	char numberone[11][13]={
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' '};

			char numbertwo[11][13]={
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' '};

		char numberzero[11][13]={
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' '};

					char numberthree[11][13]={
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' '};


				char numberfour[11][13]={
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' '};

			char numberfive[11][13]={
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' '};

		char numbersix[11][13]={
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' '};

			char numberseven[11][13]={
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' '};

				char numbereight[11][13]={
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' '};

			char numbernine[11][13]={
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	'Û',	'Û',	'Û',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	'Û',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',
		' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' ',	' '};



	COORD current;

	current=coord;
	setcursor(coord.X, coord.Y);
	
		switch(number)
	{
		case 0:
			memcpy(currentnumber, numberzero, 11*13*sizeof(char));
			break;
		case 1:
		memcpy(currentnumber, numberone, 11*13*sizeof(char));
			break;
		case 2:
			memcpy(currentnumber, numbertwo, 11*13*sizeof(char));
			break;
			case 3:
			memcpy(currentnumber, numberthree, 11*13*sizeof(char));
			break;
		case 4:
			memcpy(currentnumber, numberfour, 11*13*sizeof(char));
			break;
			case 5:
			memcpy(currentnumber, numberfive, 11*13*sizeof(char));
			break;
			case 6:
			memcpy(currentnumber, numbersix, 11*13*sizeof(char));
			break;
				case 7:
			memcpy(currentnumber, numberseven, 11*13*sizeof(char));
			break;
				case 8:
			memcpy(currentnumber, numbereight, 11*13*sizeof(char));
			break;
				case 9:
			memcpy(currentnumber, numbernine, 11*13*sizeof(char));
			break;
		default:
		case -1:
			memcpy(currentnumber, nonumber, 11*13*sizeof(char));
			break;
		}
		for (i = 0; i < 11; i++){
				for(x = 0; x < 13; x++){
				printf("%c", currentnumber[i][x]);
				}
				current.Y ++;
			setcursor(coord.X, current.Y);
			}
}

void onescreenPrint(char* string)
{


	for (unsigned int i = 0; i < strlen(string); i++)
	{
		if (string[i] == '\n'){
			printf("\n");
			cs.linecounter++;
		}
		else{
	printf("%c", string[i]);
		}
			if (cs.linecounter > (cs.coninf.dwSize.Y - 2)){
			printf("<PRESS A KEY TO CONTINUE>");
			waitforkey();
			cls();
			zerocursor();
			cs.linecounter = 0;
			}
	}
}

void PrintX(char* string)
{
printf("%s", string);
}

COORD retrieveTextCenter(char* string)
{
	int hcenter;
	int thcenter;
	int vcenter;
	COORD retcoord;

	hcenter = cs.coninf.dwSize.X / 2;
	vcenter = cs.coninf.dwSize.Y / 2;
	thcenter = (hcenter - ((strlen(string)) / 2));

	retcoord.X = thcenter;
	retcoord.Y = vcenter;

	return retcoord;
}

int retrieveTextCenterV()
{
	int vcenter;
	vcenter = cs.coninf.dwSize.Y / 2;
	return vcenter;
}

int retrieveTextCenterH(char* string)
{
	int hcenter;
	int thcenter;


	hcenter = cs.coninf.dwSize.X / 2;
	thcenter = (hcenter - ((strlen(string)) / 2));

return thcenter;
}

void StartTimer(PTIMER timer)
{
	timer->tick1 = clock();
}

void EndTimer(PTIMER timer)
{
	timer->tick2 = clock();
	timer->tickdiff = timer->tick2 - timer->tick1;
}

DWORD TimeElapsed(PTIMER timer)
{
	return timer->tickdiff;
}

DWORD SplitElapsed(PTIMER timer)
{
	double splitdouble = (double)timer->tickdiff / (double)CLOCKS_PER_SEC;
	splitdouble *= 100;
	return ((int)splitdouble); //THIS IS WEIRD, WE'LL FIGURE THIS OUT LATER.
}

DWORD SecondsElapsed(PTIMER timer)
{
	return (timer->tickdiff / CLOCKS_PER_SEC ); //THIS IS WEIRD, WE'LL FIGURE THIS OUT LATER.
}

void printline()
{
	printf("-------------------------------------------------------------------------------\n");
}

void cleartext(char* text)
{
	int i;
	int len = strlen(text);

	for (i = 0; i < len; i++){
		printf(" ");
	}
}

void clearcells(int numbertoclear, COORD writecoord)
{
	DWORD written;
	

	FillConsoleOutputCharacter(cs.hOutput, ' ', numbertoclear, writecoord, &written);
}

void ClearRect(RECT recttoclear)
{
	DWORD written;
	COORD writecoord;
	DWORD length;
	int linecounter;
	for (linecounter = recttoclear.top; linecounter <= recttoclear.bottom; linecounter++)
	{
		writecoord.Y = (short)linecounter;
		writecoord.X = (short)recttoclear.left;
		length = recttoclear.right - recttoclear.left;
	FillConsoleOutputCharacter(cs.hOutput, ' ', length, writecoord, &written);
	FillConsoleOutputAttribute(cs.hOutput, 0, length, writecoord, &written);
	}
}

void ClearRect(RECT recttoclear, WORD color)
{
	DWORD written;
	COORD writecoord;
	DWORD length;
	int linecounter;
	for (linecounter = recttoclear.top; linecounter <= recttoclear.bottom; linecounter++)
	{
		writecoord.Y = linecounter;
		writecoord.X = (short)recttoclear.left;
		length = recttoclear.right - recttoclear.left;
	FillConsoleOutputCharacter(cs.hOutput, ' ', length, writecoord, &written);
	FillConsoleOutputAttribute(cs.hOutput, color, length, writecoord, &written);
	}
}

void DrawRect(RECT Rect, POINT Origin, WORD Color)
{
	int Width = Rect.right - Rect.left;
	int Height = Rect.bottom - Rect.top;
	int Counter = 0;
	char HLineChar = (char)196;
	char VLineChar = (char)179;
	int RepeatTimes = Height - 2;
	int PointCounter = 0;
	SMALL_RECT WriteRegion;
	CHAR_INFO* LineBuffer = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * Width);
	COORD BufferSize;
	COORD Zeroed;
	Zeroed.X = 0;
	Zeroed.Y = 0;
	BufferSize.X = Width;
	BufferSize.Y = 1;
	
	
	//NOW, LET'S GO.
	//UNIQUE SECTIONS: TOP, BOTTOM, ONE SLICE OF THE MIDDLE (CAN BE COPIED)


	//////TOP SECTION
	memset(LineBuffer, 0, sizeof(CHAR_INFO) * Width);
	LineBuffer[0].Attributes = Color;
	LineBuffer[0].Char.AsciiChar = (char)218;
	LineBuffer[Width - 1].Attributes = Color;
	LineBuffer[Width - 1].Char.AsciiChar = (char)191;
	for(int WCounter = 1; WCounter < Width - 1; WCounter++)
	{
		LineBuffer[WCounter].Attributes = Color;
			LineBuffer[WCounter].Char.AsciiChar = HLineChar;
	}
	WriteRegion.Top = (short)Origin.y;
	WriteRegion.Bottom = (short)Origin.y;
	WriteRegion.Left = (short)Origin.x;
	WriteRegion.Right = Origin.x + Width;
	WriteConsoleOutput(getconsoleoutputh(), LineBuffer, BufferSize, Zeroed, &WriteRegion);

	/////////MIDDLE SECTIONS
	memset(LineBuffer, 0, sizeof(CHAR_INFO) * Width);
	for(WCounter = 0, PointCounter = 1 + Origin.y; WCounter < RepeatTimes; WCounter++, PointCounter++)
	{
		setcursor(Origin.x, PointCounter);
		setcolor(Color);
		writechar(VLineChar);

		setcursor(Origin.x + Width - 1, PointCounter);
		writechar(VLineChar);
	}

	//BOTTOM SECTION
	memset(LineBuffer, 0, sizeof(CHAR_INFO) * Width);
	LineBuffer[0].Attributes = Color;
	LineBuffer[0].Char.AsciiChar = (char)192;
	LineBuffer[Width - 1].Attributes = Color;
	LineBuffer[Width - 1].Char.AsciiChar = (char)217;
	for(WCounter = 1; WCounter < Width - 1; WCounter++)
	{

		LineBuffer[WCounter].Attributes = Color;
			LineBuffer[WCounter].Char.AsciiChar = HLineChar;
		
	}
	WriteRegion.Top = Origin.y + Height - 1;
	WriteRegion.Bottom = Origin.y + Height - 1;
	WriteRegion.Left = (short)Origin.x;
	WriteRegion.Right = Origin.x + Width;
	WriteConsoleOutput(getconsoleoutputh(), LineBuffer, BufferSize, Zeroed, &WriteRegion);

	free(LineBuffer);

}

POINT ReturnTextCenterInRect(char* text, RECT recttocheck)
{
	POINT retpt;
	retpt.x = 0;
	retpt.y = 0;
	int rectwidth = recttocheck.right - recttocheck.left;
	int rectheight = recttocheck.bottom - recttocheck.top;
	if (!text){
		return retpt;
	}
	int textwidth = strlen(text);
	retpt.x = ((rectwidth / 2) - (textwidth / 2)) + recttocheck.left;
	retpt.y = rectheight / 2 + recttocheck.top;
	return retpt;
}