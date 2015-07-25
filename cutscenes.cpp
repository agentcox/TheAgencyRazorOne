#include "agencyinclude.h"

void FinalMobilizationCutscene()
{
	int numbeeps = rand()%5 + 3;
	cls(0);
	Sleep(1000);
	FadeInTitle("STANDBY", "FOR INGRESS");
	cls(0);
	for(int i = 0; i < numbeeps; i++){
	playMIXsound(VAN_DOORBEEP_SOUND);
	cls(0);
	cls(BACKGROUND_RED);
	Sleep(100);
	cls(BACKGROUND_RED | BACKGROUND_INTENSITY);
	Sleep(300);
	cls(BACKGROUND_RED);
	Sleep(100);
	cls(0);
	Sleep(1000);
	}
	playMIXsound(VAN_DOOROPEN_SOUND);
	cls(BACKGROUND_GREEN);
	cls(BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	Sleep(1000);
}


void FadeInTitle(char* title, char* subtitle)
{
	COORD titledrawcoords;
	COORD subtitledrawcoords;
	
	cls(0);
	zerocursor();
	
	if (title == NULL){
		return;
	}
	if (!strcmp(title, "")){
		return;
	}
	
	titledrawcoords = retrieveTextCenter(title);
	titledrawcoords.Y--;
	GREEN;
	setcursor(titledrawcoords.X, titledrawcoords.Y);
	printf(title);
	Sleep(80);
	LGREEN;
	setcursor(titledrawcoords.X, titledrawcoords.Y);
	printf(title);
	Sleep(80);
	Sleep(1000);
	
	if (subtitle != NULL)
	{
		if (strcmp(title, "")){
			
			
			subtitledrawcoords = retrieveTextCenter(subtitle);
			GREEN;
			setcursor(subtitledrawcoords.X, subtitledrawcoords.Y);
			printf(subtitle);
			Sleep(80);
			LGREEN;
			setcursor(subtitledrawcoords.X, subtitledrawcoords.Y);
			printf(subtitle);
			Sleep(80);
			Sleep(1000);
		}
	}
	
	Sleep(1000);
	GREEN;
	setcursor(titledrawcoords.X, titledrawcoords.Y);
	printf(title);
	if (subtitle != NULL)
	{
		if (strcmp(title, "")){
			GREEN;
			setcursor(subtitledrawcoords.X, subtitledrawcoords.Y);
			printf(subtitle);
			
		}
	}
	Sleep(80);
	cls();
	
}

void ComputerPrintOutString(char* string, int sleeptime, char* finalstring)
{
	COORD consize = getconsize();
	int width = consize.X - 3;
	if(!string){
		return;
	}
	char* ourstring = (char*) malloc((strlen(string) + 1) * sizeof(char));
	strcpy(ourstring, string);
	WrapStringToWidth(ourstring, width);
	int height = GetNumberOfStringLines(ourstring);
	int finalYorigin = (consize.Y / 2) - (height / 2); //THIS IS WHERE THE TEXT SHOULD BE SCROLLED UP TO.
	int finalXorigin = 1;
	int bottomYorigin = consize.Y - 1;
	int currentYorigin = finalYorigin;
	int currentXorigin = finalXorigin;
	SMALL_RECT scrollrect;
	scrollrect.Bottom = consize.Y - 1;
	scrollrect.Left = finalXorigin;
	scrollrect.Right = width;
	scrollrect.Top = currentYorigin;
	SMALL_RECT cliprect;
	cliprect.Bottom = consize.Y - 2;
	cliprect.Top = finalYorigin;
	cliprect.Right = width;
	cliprect.Left = finalXorigin;
	CHAR_INFO fill;
	fill.Attributes = FOREGROUND_RED;
	fill.Char.AsciiChar = (char)219;
	COORD neworigin;
	bool Skipped = false;
	int i;
	int numevents;
	USHORT key;
	neworigin.X = finalXorigin;
	neworigin.Y = currentYorigin - 1;
	
	unsigned int len = strlen(ourstring);
	unsigned int currentchar;
	char cursorchar = (char)219;
	unsigned int currentlocalcharacter = 1;
	HANDLE consoleout = getconsoleoutputh();
	
	for(currentchar = 1; currentchar <= len; currentchar++)
	{
		
		numevents = checkforinput();
			if (numevents > 1)
			{
				for (i = 1; i <= numevents; i++)
				{
					key = getinput(i);
					if(key == VK_ESCAPE){
						FLUSH;
			clearinputrecords();
						Skipped = true;
					}
				}
			}
			FLUSH;
			clearinputrecords();
		//1. DISPLAY PREVIOUS CHARACTER IN FULL GREEN
		LGREEN;
		setcursor(currentlocalcharacter - 1 + currentXorigin, currentYorigin);
		writechar(ourstring[currentchar - 1]);
		//2a. IF CURRENT CHARACTER IS \n, SCROLL TEXT AND RESET XCURSOR
		if(ourstring[currentchar] == '\n'){
			//GO KILL THAT CURSOR.
			
			setcolor(0);
			setcursor(currentlocalcharacter + currentXorigin, currentYorigin);
			writechar(cursorchar);
			currentXorigin = finalXorigin;
			currentYorigin++;
			scrollrect.Top = currentYorigin;
			neworigin.Y = currentYorigin;
			currentlocalcharacter = 0;
			//ScrollConsoleScreenBuffer(consoleout, &scrollrect, &cliprect, neworigin, &fill);
		}
		//2. ELSE, DISPLAY CURRENT CHARACTER IN FADED GREEN.
		else{
			if(currentchar < len){
			GREEN;
			setcursor(currentlocalcharacter + currentXorigin, currentYorigin);
			writechar(ourstring[currentchar]);
			currentlocalcharacter++;
			}
				//3. DRAW CURSOR.
		GREEN;
		setcursor(currentlocalcharacter + currentXorigin, currentYorigin);
		writechar(cursorchar);
		if(!Skipped){
		Sleep(sleeptime);
		}
		}
		
			
	}

	//NOW, WHILE WE'RE NOT LINED UP WITH THE Y ORIGIN....
	//SCROLL UP.
	/*
	for(;currentYorigin > finalYorigin; currentYorigin--)
	{
		setcolor(0);
			setcursor(currentlocalcharacter + currentXorigin, bottomYorigin);
			writechar(cursorchar);
			currentXorigin = finalXorigin;
			currentYorigin--;
			scrollrect.Top = currentYorigin;
			neworigin.Y = currentYorigin;
			currentlocalcharacter = 0;
			ScrollConsoleScreenBuffer(consoleout, &scrollrect, &cliprect, neworigin, &fill);
	}
*/
	if(!Skipped){
	Sleep(1000);
	}
	GREEN;
	setcursor(finalXorigin, finalYorigin + height);
	writestring(finalstring);
	waitforkey();
	
}

void StartMissionChronoSequence(PMISSIONCHRONO mchrono)
{
	COORD mcdrawcoords;
	TIMER timer;
	char mctimebuf[80];
	int i;
	
	
	mchrono->countdown = TRUE;
	sprintf(mctimebuf, "%2.2d:%2.2d.%2.2d", mchrono->minutes, mchrono->seconds, mchrono->split);
	mcdrawcoords = retrieveTextCenter(mctimebuf);
	GREEN;
	
	setcursor(mcdrawcoords.X, mcdrawcoords.Y);
	printf(mctimebuf);
	Sleep(80);
	LGREEN;
	setcursor(mcdrawcoords.X, mcdrawcoords.Y);
	printf(mctimebuf);
	Sleep(1000);
	LRED;
	setcursor(mcdrawcoords.X, mcdrawcoords.Y);
	printf(mctimebuf);
	Sleep(500);
	for (i = 0; i < 150; i++){
		if(i == 148){
			RED;
		}
		StartTimer(&timer);
		sprintf(mctimebuf, "%2.2d:%2.2d.%2.2d", mchrono->minutes, mchrono->seconds, mchrono->split);
		setcursor(mcdrawcoords.X, mcdrawcoords.Y);
		printf(mctimebuf);
		Sleep(50);
		EndTimer(&timer);
		mchrono->split -= SplitElapsed(&timer);
		calibratemissionchrono(mchrono);
	}
	cls();
}

void SynchronizeChronoSequence(PCHRONOSTRUCT chrono, PMISSIONCHRONO mchrono)
{
	CHRONOSTRUCT localchron;
	MISSIONCHRONO localmchron;
	COORD mcdrawcoords;
	COORD cdrawcoords;
	COORD synccoords;
	char ctimebuf[80];
	char mctimebuf[80];
	unsigned int iterations = 0;
	int velocity = 1;
	int velocitycounter = 0;
	char* synctext = "S Y N C H R O N I Z I N G";
	char* syncdonetext = "S Y N C H R O N I Z E D";
	int speed = 1;
	
	memset(&localchron, 0, sizeof(CHRONOSTRUCT));
	memset(&localmchron, 0, sizeof(MISSIONCHRONO));
	localmchron.countdown = FALSE;
	
	SPrintMissionChrono(mctimebuf, &localmchron);
	SPrintChrono(ctimebuf, &localchron);
	
	cdrawcoords = retrieveTextCenter(ctimebuf);
	mcdrawcoords = retrieveTextCenter(mctimebuf);
	synccoords = retrieveTextCenter(synctext);
	
	cdrawcoords.Y++; //SPLIT THEM UP
	mcdrawcoords.Y--; 
	
	setcursor(synccoords.X, synccoords.Y);
	RED;
	printf(synctext);
	
	while(iterations < 2000000){
		iterations++;
		if (localmchron.hours < mchrono->hours || localmchron.minutes < mchrono->minutes || localmchron.seconds < mchrono->seconds || localmchron.split <= mchrono->split)
		{
			localmchron.split ++;
			calibratemissionchrono(&localmchron);
		}
		else{
			localmchron = *mchrono;
		}
		if (localchron.hour < chrono->hour || localchron.minute < chrono->minute || localchron.second < chrono->second){
			localchron.second ++;
			calibratechrono(&localchron);
		}
		else
		{
			localchron = *chrono;
		}
		
		speed++;
		
		if (speed % velocity == 0)
		{
			SPrintChrono(ctimebuf, &localchron);
			SPrintMissionChrono(mctimebuf, &localmchron);
			LRED;
			setcursor(mcdrawcoords.X, mcdrawcoords.Y);
			printf(mctimebuf);
			LGREEN;
			setcursor(cdrawcoords.X, cdrawcoords.Y);
			printf(ctimebuf);
			
		}
		
		velocitycounter++;
		if (velocitycounter > 400){
			velocitycounter = 0;
			velocity++;
		}
		
		
		
		if (localmchron.hours >= mchrono->hours && localmchron.minutes >= mchrono->minutes && localmchron.seconds >= mchrono->seconds && localmchron.split >= mchrono->split && localchron.hour >= chrono->hour && localchron.minute >= chrono->minute && localchron.second >= chrono->second){
			SPrintChrono(ctimebuf, &localchron);
			SPrintMissionChrono(mctimebuf, &localmchron);
			LRED;
			setcursor(mcdrawcoords.X, mcdrawcoords.Y);
			printf(mctimebuf);
			LGREEN;
			setcursor(cdrawcoords.X, cdrawcoords.Y);
			printf(ctimebuf);
			WHITE;
			setcursor(synccoords.X, synccoords.Y);
			cleartext(synctext);
			synccoords = retrieveTextCenter(syncdonetext);
			setcursor(synccoords.X, synccoords.Y);
			printf(syncdonetext);
			
			break;
		}
	}
	Sleep(2000);
	GRAY;
	setcursor(synccoords.X, synccoords.Y);
	printf(syncdonetext);
	RED;
	setcursor(mcdrawcoords.X, mcdrawcoords.Y);
	printf(mctimebuf);
	GREEN;
	setcursor(cdrawcoords.X, cdrawcoords.Y);
	printf(ctimebuf);
	Sleep(80);
	cls();
}

void opcontitles(int titlenumber)
{
	
	COORD consize;
	
	COORD lastdatepos;
//	char datestring[60];
	
	COORD lasttitlepos;
	
	COORD lastquotepos;
	int quotenumber = rand()%3;
	
	COORD lastlevelpos;
	
	char quotestring[TS_FINAL][3][60] = 
	{
		{"-They Sense Your Every Move-", "-They Know Your Every Thought-", "-They Brought You To Life-"},
		{"-You Do Not Officially Exist-", "-You Have No Lifeline-", "-You Can Never Go Back-"},
		{"-Silence Is Your Only Ally-", "-There Is No Sanctuary-", "-They've Programmed You-"},
		{"-They Took Everything From You-", "-Mortis Absolvo-", "-They Can Take It All Away-"},
		{"-Veritas Ventosus-", "-Nobody Can Know The Whole Truth-", "-They Promised You Everything-"},
		{"-Ashes To Ashes, Dust To Dust-", "-In The End We Are All Alone-", "-We're All Slaves To Someone-"},
		{"-In The End, All Is Ruin-", "-Never Shall The Two Meet-", "-Eternity Lies Beyond-"}
	};
	char titlestring[TS_FINAL][60] = 
	{"OPERATIONS CONTROL", "OFFICE OF PERSONNEL AND RECORDS", "ARMORY AND OUTFITTING CENTER", "TRAINING SIMULATOR CENTER", "SARTU NETWORK UPLINK CENTER", "THE SILENT WALL", "NUCLEAR WEAPONS STORAGE"};
	char levelstring[TS_FINAL][60] = 
	{"DSA - LEVEL V", "DSA - LEVEL I", "DSA - LEVEL IV", "DSA - LEVEL V", "DSA - LEVEL III", "DSA - LEVEL I", "DSA-LEVEL VI"};
	
	
	cls();
	Sleep(500);
	
	if (titlenumber < 0 && titlenumber > TS_FINAL)
	{
		printf("ILLEGAL TITLE!!!!!!\n");
		printf("<PRESS A KEY>");
		waitforkey();
		return;
	}
	
	
//	sprintf(datestring, "%2.2d/%2.2d/%4.4d - %2.2d%2.2d HOURS", chronostruct->month, chronostruct->day, chronostruct->year, chronostruct->hour, chronostruct->minute);
	
	consize = getconsize();
	lastdatepos.X = 0;
	lastdatepos.Y = consize.Y - 1;
	
	lastlevelpos.Y = retrieveTextCenterV() + 1;
	lastlevelpos.X = retrieveTextCenterH(levelstring[titlenumber]);
	
	lastquotepos.Y = retrieveTextCenterV() - 1;
	lastquotepos.X = retrieveTextCenterH(quotestring[titlenumber][quotenumber]);
	
	lasttitlepos = retrieveTextCenter(titlestring[titlenumber]);
	
	setcursor(lastquotepos.X, lastquotepos.Y);
	GREEN;
	printf("%s", quotestring[titlenumber][quotenumber]);
	Sleep(150);
	LGREEN;
	setcursor(lastquotepos.X, lastquotepos.Y);
	printf("%s", quotestring[titlenumber][quotenumber]);
	
	Sleep(1000);
	
				setcursor(lastquotepos.X, lastquotepos.Y);
				GREEN;
				printf("%s", quotestring[titlenumber][quotenumber]);
				Sleep(150);
				
				cls();
				Sleep(1000);
				
				setcursor(lasttitlepos.X, lasttitlepos.Y);
				GREEN;
				printf("%s", titlestring[titlenumber]);
				setcursor(lastlevelpos.X, lastlevelpos.Y);
				printf("%s", levelstring[titlenumber]);
				Sleep(150);
				setcursor(lasttitlepos.X, lasttitlepos.Y);
				LGREEN;
				printf("%s", titlestring[titlenumber]);
				setcursor(lastlevelpos.X, lastlevelpos.Y);
				printf("%s", levelstring[titlenumber]);
				
				Sleep(500);
				
				setcursor(lastdatepos.X, lastdatepos.Y);
	//			bufferprint(50, datestring);
				
				Sleep(3000);
				
				
				/************************************************************
				; FADE OUT
				************************************************************/
				
				setcursor(lastdatepos.X, lastdatepos.Y);
				GREEN;
			//	printf("%s", datestring);
				setcursor(lasttitlepos.X, lasttitlepos.Y);
				GREEN;
				printf("%s", titlestring[titlenumber]);
				setcursor(lastlevelpos.X, lastlevelpos.Y);
				printf("%s", levelstring[titlenumber]);
				
				Sleep(150);
				cls();
				return;
				
				
}

void atomicdetonation()
{
	//TIMER RUNS DOWN, SHOWS "DETONATION", THEN A FLASH AND BOOM. FLASH THE CONSOLE WITH GREY - WHITE - GREY - BLACK
	
	int seconds;
	int split;
	int hcenter;
	//	unsigned int i;
	int thcenter;
	int vcenter;
	//00:00.01 kinda like that
	char timebuf[20];
	char *detonationstring = "D E T O N A T I O N";
	char *missioncompletestring = "M I S S I O N    C O M P L E T E";
	COORD consize;
	
	cls();
	consize = getconsize();
	//	cursorhide();
	hcenter = ((consize.X) / 2);
	vcenter = ((consize.Y)/ 2);
	seconds = 2;
	split = 99;
	
	while (seconds > 0 || split > 0){
		split--;
		if (split < 0){
			seconds --;
			split = 99;
		}
		sprintf(timebuf, "00:%2.2d.%2.2d", seconds, split);
		thcenter = hcenter - (strlen(timebuf) / 2);
		setcursor(thcenter, vcenter);
		LRED;
		printf("%s", timebuf);
		Sleep(10);
	}
	thcenter = hcenter - (strlen(detonationstring) / 2);
	setcursor(thcenter, vcenter);
	RED;
	printf("%s", detonationstring);
	Sleep(200);
	
	setcursor(thcenter, vcenter);
	LRED;
	printf("%s", detonationstring);
	Sleep(500);
	cls();
	
	
	fillwithcolor(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	Sleep(300);
	fillwithcolor(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
	Sleep(300);
	fillwithcolor(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	Sleep(300);
	fillwithcolor(0);
	Sleep(2000);
	//	cursorshow();
	
	thcenter = hcenter - (strlen(missioncompletestring) / 2);
	setcursor(thcenter, vcenter);
	RED;
	printf("%s", missioncompletestring);
	Sleep(200);
	
	setcursor(thcenter, vcenter);
	LRED;
	printf("%s", missioncompletestring);
	Sleep(2000);
	cls();
}



void CreditScreen()
{
#define MAX_DUTIES 21
#define MAX_CREDIT_LENGTH 400
	
#define CREDIT_REDFLASH 0
#define CREDIT_GREENFADE 1
	
	//#define PROMPT_TEXT_SPACE_X_OFFSETFROMLEFT 16
	//#define PROMPT_TEXT_SPACE_Y_OFFSETFROMTOP 4
	//#define PROMPT_TEXT_WIDTH	46
	//#define PROMPT_TEXT_HEIGHT 15
	
	RECT boxrect;
	int creditcounter;
	char duties[MAX_DUTIES][MAX_CREDIT_LENGTH] = {
		"THE AGENCY",
			"Based On \"The Agency\" Espionage Universe", 
			"Designer",
			"Lead Programmer",
			"Glamour Model",
			"Box / Manual Design",
			"Special Thanks To:", 
			"Special Thanks To:",
			"We Extend A Special Thank You To:",
			"We Extend A Special Thank You To:",
			"We Extend A Special Thank You To:",
			"Vorpal Design Group is a small west-coast game design group centered around the intelligence and military gaming enthusiast.",
			"The Agency is an original concept created by Charles Cox. For franchising information, and for info on how you can become a part of The Agency Universe, please contact Charles Cox, President of VDG at: agency.one@gte.net",
			"We will be bringing The Agency : Razor One to you in full 3d very soon. Please stay tuned to the company that brings the intelligence and military world to your computer:",
			"Vorpal",
			"VDG   ",
			"Charles Cox",
			"Wyatt Jackson",
			"Eddy Irwin",
			"Jennifer Dygert",
			"THANKS FOR PLAYING!"
	};
	
	char names[MAX_DUTIES][MAX_CREDIT_LENGTH] = {
		"RAZOR ONE",
			"By Charles Cox", 
			"Charles Cox",
			"Charles Cox",
			"Charlie \"Flowers\" From Jersey City",
			"Charles Cox",
			"Wyatt Jackson (Dr. J)",
			"\"Fast\" Eddy Irwin From Jersey City",
			"\"Big\" Bird",
			"Big \"Bird\"",
			"Bert \"and\" Ernie",
			"",
			"",
			"",
			"Design Group",
			"   IS",
			"President, CEO, Lead Designer",
			"Programmer",
			"Programmer / Designer",
			"Craft Services",
			"PRESS A KEY TO RETURN."
	} ; 
	
	int styles[MAX_DUTIES] = {
		CREDIT_REDFLASH,
			CREDIT_REDFLASH,
			CREDIT_REDFLASH,
			CREDIT_REDFLASH,
			CREDIT_REDFLASH,
			CREDIT_REDFLASH,
			CREDIT_REDFLASH,
			CREDIT_REDFLASH,
			CREDIT_REDFLASH,
			CREDIT_REDFLASH,
			CREDIT_REDFLASH,
			CREDIT_GREENFADE,
			CREDIT_GREENFADE,
			CREDIT_GREENFADE,
			CREDIT_REDFLASH,
			CREDIT_REDFLASH,
			CREDIT_REDFLASH,
			CREDIT_REDFLASH,
			CREDIT_REDFLASH,
			CREDIT_REDFLASH,
			CREDIT_REDFLASH,
	};
	
	COORD dutydrawcoords;
	COORD namedrawcoords;
	COORD boxsize = {PROMPT_TEXT_WIDTH, PROMPT_TEXT_HEIGHT};
	COORD boxstart = {PROMPT_TEXT_SPACE_X_OFFSETFROMLEFT, PROMPT_TEXT_SPACE_Y_OFFSETFROMTOP};
	COORD zeroed = {0,0};
	
	
	boxrect.left = PROMPT_TEXT_SPACE_X_OFFSETFROMLEFT;
	boxrect.top = PROMPT_TEXT_SPACE_Y_OFFSETFROMTOP;
	boxrect.right = PROMPT_TEXT_SPACE_X_OFFSETFROMLEFT+PROMPT_TEXT_WIDTH;
	boxrect.bottom = PROMPT_TEXT_SPACE_Y_OFFSETFROMTOP + PROMPT_TEXT_HEIGHT;
	
	
	LoopSong(globals.musiclist.songs[CREDITS_SONG]);
	
	styles[MAX_DUTIES - 1] = CREDIT_REDFLASH; //THE LAST CREDIT MUST ALWAYS BE REDFLASH.
	
	GREEN;
	printgraphic(globals.graphicslist, zeroed,  PROMPT_GRAPHIC_ID);
	for (creditcounter = 0; creditcounter < MAX_DUTIES; creditcounter++){
		if (styles[creditcounter] == CREDIT_REDFLASH)
		{
			ClearRect(boxrect);
			dutydrawcoords = retrieveTextCenter(duties[creditcounter]);
			dutydrawcoords.Y --;
			namedrawcoords = retrieveTextCenter(names[creditcounter]);
			RED;
			setcursor(dutydrawcoords.X, dutydrawcoords.Y);
			bufferprint(1, duties[creditcounter]);
			setcursor(dutydrawcoords.X, dutydrawcoords.Y);
			LRED;
			bufferprint(1, duties[creditcounter]);
			Sleep(200);
			RED;
			setcursor(namedrawcoords.X, namedrawcoords.Y);
			bufferprint(1, names[creditcounter]);
			setcursor(namedrawcoords.X, namedrawcoords.Y);
			LRED;
			bufferprint(1, names[creditcounter]);
			if (creditcounter < MAX_DUTIES - 1)
			{
				Sleep(3500);
				
				RED;
				setcursor(dutydrawcoords.X, dutydrawcoords.Y);
				printf(duties[creditcounter]);
				setcursor(dutydrawcoords.X, dutydrawcoords.Y);
				RED;
				setcursor(namedrawcoords.X, namedrawcoords.Y);
				printf(names[creditcounter]);
				setcursor(namedrawcoords.X, namedrawcoords.Y);
				Sleep(80);
				setcursor(namedrawcoords.X, namedrawcoords.Y);
				cleartext(names[creditcounter]);
				setcursor(dutydrawcoords.X, dutydrawcoords.Y);
				cleartext(duties[creditcounter]);
				Sleep(1000);
			}
			else{
				FLUSH;
				clearinputrecords();
				waitforkey();
				StopSong();
				cls();
				return;
			}
		}
		else if (styles[creditcounter] == CREDIT_GREENFADE)
		{
			ClearRect(boxrect);
			GREEN;
			
			printwordwrapcoordinate(duties[creditcounter], boxsize, boxstart);
			Sleep(80);
			LGREEN;
			
			printwordwrapcoordinate(duties[creditcounter], boxsize, boxstart);
			Sleep(10000);
			GREEN;
			
			printwordwrapcoordinate(duties[creditcounter], boxsize, boxstart);
			Sleep(80);
			ClearRect(boxrect);
			Sleep(1000);
		}
	}
	
}

void PromptForCorrectScreenSize()
{
	char* prompt = "Please Press ALT + ENTER To Maximize Your Screen Size";
	char* prompt2 = "When Ready, Press SPACE To Begin";
	int key;
	COORD promptdrawcoords;

	promptdrawcoords = retrieveTextCenter(prompt);
	LRED;
	setcursor(promptdrawcoords.X, promptdrawcoords.Y);
	printf(prompt);
	promptdrawcoords = retrieveTextCenter(prompt2);
	promptdrawcoords.Y++;
	setcursor(promptdrawcoords.X, promptdrawcoords.Y);
	printf(prompt2);
	
	while(1)
	{
		key = waitforkey();
		if (key == ' '){
			cls();
			return;
		}
	}
}
void IntroScreen()
{
#define MAX_TITLES 7
#define MAX_CREDIT_LENGTH 400
	
#define CREDIT_REDFLASH 0
	
	//#define PROMPT_TEXT_SPACE_X_OFFSETFROMLEFT 16
	//#define PROMPT_TEXT_SPACE_Y_OFFSETFROMTOP 4
	//#define PROMPT_TEXT_WIDTH	46
	//#define PROMPT_TEXT_HEIGHT 15
	
	RECT boxrect;
	int creditcounter;
	int numevents;
	int key;
	int i;
	
	char *agencystring = "T H E    A G E N C Y";
	char * loadingstring ="STANDBY FOR LOGIN...";
	
	char titles[MAX_TITLES][MAX_CREDIT_LENGTH] = {
		"Every country, whether sooner or later,",
			"Communication, Expression, Freedom;",
			"So that the society can better be harnessed",
			"The United States of America",
			"Perhaps the greatest testament to secrecy",
			"Within the dark and silent walls, deep below ground,",
			"Those who have known its power have respectfully called it..."
			
	};
	
	char subtitles[MAX_TITLES][MAX_CREDIT_LENGTH] = {
		"has had to rely on the strength of secrecy.",
			"these principles have been silently put aside...",
			"for the real work to be done.",
			"is no exception.",
			"can be found here, in Virginia...",
			"lies America's most secretive government branch.",
			""
			
	} ; 
	
	
	COORD dutydrawcoords;
	COORD namedrawcoords;
	COORD boxsize = {PROMPT_TEXT_WIDTH, PROMPT_TEXT_HEIGHT};
	COORD boxstart = {PROMPT_TEXT_SPACE_X_OFFSETFROMLEFT, PROMPT_TEXT_SPACE_Y_OFFSETFROMTOP};
	COORD zeroed = {0,0};
	
	
	LoopSong(globals.musiclist.songs[INTRO_SONG]);
	
	boxrect.left = PROMPT_TEXT_SPACE_X_OFFSETFROMLEFT;
	boxrect.top = PROMPT_TEXT_SPACE_Y_OFFSETFROMTOP;
	boxrect.right = PROMPT_TEXT_SPACE_X_OFFSETFROMLEFT+PROMPT_TEXT_WIDTH;
	boxrect.bottom = PROMPT_TEXT_SPACE_Y_OFFSETFROMTOP + PROMPT_TEXT_HEIGHT;
	
	for (creditcounter = 0; creditcounter < MAX_TITLES; creditcounter++){

			numevents = checkforinput();
			if (numevents > 1)
			{
				for (i = 1; i <= numevents; i++)
				{
					key = getinput(i);
					if(key == VK_ESCAPE){
						FLUSH;
			clearinputrecords();
						return;
					}
				}
			}
			FLUSH;
			clearinputrecords();
		
		ClearRect(boxrect);
		dutydrawcoords = retrieveTextCenter(titles[creditcounter]);
		dutydrawcoords.Y --;
		namedrawcoords = retrieveTextCenter(subtitles[creditcounter]);
		RED;
		setcursor(dutydrawcoords.X, dutydrawcoords.Y);
		bufferprint(1, titles[creditcounter]);
		setcursor(dutydrawcoords.X, dutydrawcoords.Y);
		LRED;
		bufferprint(1, titles[creditcounter]);
		Sleep(1000);
		RED;
		setcursor(namedrawcoords.X, namedrawcoords.Y);
		bufferprint(1, subtitles[creditcounter]);
		setcursor(namedrawcoords.X, namedrawcoords.Y);
		LRED;
		bufferprint(1, subtitles[creditcounter]);
		
		Sleep(3500);
		
		RED;
		setcursor(dutydrawcoords.X, dutydrawcoords.Y);
		printf(titles[creditcounter]);
		setcursor(dutydrawcoords.X, dutydrawcoords.Y);
		RED;
		setcursor(namedrawcoords.X, namedrawcoords.Y);
		printf(subtitles[creditcounter]);
		setcursor(namedrawcoords.X, namedrawcoords.Y);
		Sleep(80);
		setcursor(namedrawcoords.X, namedrawcoords.Y);
		cleartext(subtitles[creditcounter]);
		setcursor(dutydrawcoords.X, dutydrawcoords.Y);
		cleartext(titles[creditcounter]);
		Sleep(1000);
		
	}
	
	dutydrawcoords = retrieveTextCenter(agencystring);
	RED;
	setcursor(dutydrawcoords.X, dutydrawcoords.Y);
	printf(agencystring);
	LRED;
	Sleep(150);
	setcursor(dutydrawcoords.X, dutydrawcoords.Y);
	printf(agencystring);
	Sleep(2000);
	RED;
	setcursor(dutydrawcoords.X, dutydrawcoords.Y);
	printf(agencystring);
	Sleep(150);
	cls();
		dutydrawcoords = retrieveTextCenter(loadingstring);
	RED;
	setcursor(dutydrawcoords.X, dutydrawcoords.Y);
	printf(loadingstring);
	LRED;
	Sleep(150);
	setcursor(dutydrawcoords.X, dutydrawcoords.Y);
	printf(loadingstring);
	Sleep(2000);
	RED;
	setcursor(dutydrawcoords.X, dutydrawcoords.Y);
	printf(loadingstring);
	Sleep(150);
	cls();
}

void PrintDayAndMonth(PPLAYER player)
{
	char monthbuf[255];
	char daybuf[255];

	sprintf(monthbuf, "MONTH %d", player->monthscompleted + 1);
				sprintf(daybuf, "DAY %d", 30 - player->numberofdaysincycle + 1);
				FadeInTitle(monthbuf, daybuf);
}

void PrintPercentageComplete(char* text, int percentage)
{
	char buf[200];
	int timetosleep = 30;
	POINT pt;
	sprintf(buf, "%s - %2.2d%%", text, percentage);
	pt.x = retrieveTextCenterH(buf);
	pt.y = retrieveTextCenterV();
	setcursor(pt.x, pt.y);
	LRED;
	printf("%s", buf);
//	Sleep(timetosleep);
}

void PrintFinalData(char* stringone, char* stringtwo, bool bottomred, int topsleep, int bottomsleep, int intervalsleep, int finalsleep)
{
	int topHCenter;
	int topLen = 0;
	int topVCenter;
	int botHCenter;
	int botVCenter;
	int botLen = 0;


	if(!stringone){
		return;
	}
	topLen = strlen(stringone);
	topVCenter = botVCenter = retrieveTextCenterV();
	botVCenter++;

	topHCenter = retrieveTextCenterH(stringone);
	if(stringtwo){
		botHCenter = retrieveTextCenterH(stringtwo);
		botLen = strlen(stringtwo);
	}

	for(int i = 0; i < topLen; i++){
		setcursor(topHCenter + i, topVCenter);
		GREEN;
		
		writechar(stringone[i]);
		Sleep(topsleep);
		LGREEN;
		setcursor(topHCenter + i, topVCenter);
		writechar(stringone[i]);
	}
	if(stringtwo){
	Sleep(intervalsleep);
	for(i = 0; i < botLen; i++){
		setcursor(botHCenter + i, botVCenter);
		if(!bottomred){
		GREEN;
		}
		else{
			RED;
		}
		
		writechar(stringtwo[i]);
		Sleep(bottomsleep);
		if(!bottomred){
		LGREEN;
		}
		else{
			LRED;
		}
		setcursor(botHCenter + i, botVCenter);
		writechar(stringtwo[i]);
	}
	}

	Sleep(finalsleep);
	setcursor(topHCenter, topVCenter);
	GREEN;
	writestring(stringone);
	if(stringtwo){
		setcursor(botHCenter, botVCenter);
		if(bottomred){
			RED;
		}
		else{
			GREEN;
		}
		writestring(stringtwo);
	}
	Sleep(bottomsleep);
	cls(0);
	Sleep(intervalsleep);
}