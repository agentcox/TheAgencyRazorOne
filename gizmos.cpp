#include "agencyinclude.h"


//--------------------------------------------------------------------------------------------
// Function Name - CISDebug
//
// Description   - CIS Debugging Utility. Inputs default values to CIS, passes on success result
//
// Return Type   - BOOL 
//
//
// Author        - Charles Cox
// Date Modified - 04/18/2000
//--------------------------------------------------------------------------------------------
BOOL CISDebug()
{
	PLAYER playerdebug;
	DOOR doordebug;
	MISSIONCHRONO mchron;
	
	mchron.minutes = 3;
	mchron.seconds = 59;
	mchron.split = 30;
	
	doordebug.lock = DOOR_LOCK_CIS;
	doordebug.encryptionstrength = 2;
	playerdebug.psychskill.skills.electronics = 2;
	playerdebug.psychskill.specializations.electronics = FALSE;
	return CISTest(&playerdebug, &doordebug);
}


//--------------------------------------------------------------------------------------------
// Function Name - CISTest
//
// Description   - The CIS Decryption Puzzle. Returns success or failure.
//
// Return Type   - BOOL 
//
// Arguments     - PPLAYER player ( The player structure. )
//               - PDOOR door ( The door that the lock is on. )
//
// Author        - Charles Cox
// Date Modified - 04/12/2000
//--------------------------------------------------------------------------------------------
BOOL CISTest(PPLAYER player, PDOOR door)
{
	//	#define CIS_MAX_REFRESH_TICKS 50
	
	
	/************************************************************
	; TO DO'S
	
	  INCORPORATE TIMER, SO IF IT RUNS OUT OR YOU SYNCHRONIZE, END AND EVALUATE. IF TIME'S UP, YOU FAIL.
	  
	************************************************************/
	
	MISSIONCHRONO timeleft;
	TIMER timer;
	int timeskill;
	char timebuf[80];
	
	//THE ACTUAL STRINGS.
	char *targetstring;
	char *playerstring;
	char targetdisplaystring[CIS_MAX_DISPLAYABLE_DIGITS + 1];
	char playerdisplaystring[CIS_MAX_DISPLAYABLE_DIGITS + 1];
	char statusmessages[CIS_NUMBER_OF_STATUS_MESSAGES][CIS_MAX_DISPLAYABLE_DIGITS] = {"AWAITING SYNCHRONIZATION" , "SYNCHRONIZING", "SYNCHRONIZATION FAILURE", "ALL CIRCUITS SYNCHRONIZED"};
	
	
	/************************************************************
	; THE CHECK VARIABLES. EACH OF THESE IN ONE SET MUST EQUAL THE OTHERS IN THE OTHER SET.
	************************************************************/
	
	//THE STATUS MESSAGE VARIABLES
	
	int statusmessagefade = CIS_STATUS_MESSAGEFADE_FADEIN; //IN, HOLD, OUT, NONE
	COORD statuscoord;
	int len;
	
	
	//THE TARGET SET
	int targetstringoffset;
	int targetstringvelocity;
	BOOL targetstringdirection;
	//THE PLAYER SET
	int playerstringoffset;
	int playerstringvelocity;
	BOOL playerstringdirection;
	
	//SOME CHECK VARIABLES
	int stringlength;
	char specialdigits[CIS_MAX_SPECIAL_DIGITS + 1] = "";
	int i;
	//	DWORD tickcount1;
	//	DWORD tickcount2;
	//	DWORD tickcount = 0;
	int iterations = 0;
	USHORT key;
	int numevents;
	BOOL debugon = FALSE;
	BOOL synchronize = FALSE;
	BOOL startsynchronize = FALSE;
	int synchronizetimer = CIS_MAX_SYNCHRONIZE_TIME;
	
	//DISPLAY COORDINATING VARIABLES
	COORD displaycoord;
	
	displaycoord.Y = CIS_GRAPHIC_OFFSET_Y_FROM_TOP;
	displaycoord.X = CIS_GRAPHIC_OFFSET_X_FROM_LEFT;
	
	//PRINT THE CIS BOX
	LoopSong(globals.musiclist.songs[ELECTRONICS_SONG]);
	cls(0);
	RED;
	printgraphic(globals.graphicslist, displaycoord, CIS_GRAPHIC_ID);
	
	//SET UP DIFFICULTY AND MALLOC ACCORDINGLY
	int estrength = door->encryptionstrength;
	if (estrength < 3){
		estrength = 3;
	}
	if (estrength > 10){
		estrength = 10;
	}
	stringlength = ((estrength * 20) - (3 * player->psychskill.skills.electronics) - (20 * (player->psychskill.specializations.electronics))) + 70;
	targetstring = (char*) malloc((stringlength + 1) * sizeof(char));
	playerstring = (char*) malloc((stringlength + 1) * sizeof(char));
	
	//SET UP THE INITIAL CODE;
	for (i = 0; i < CIS_MAX_SPECIAL_DIGITS; i++)
	{
		specialdigits[i] = RandomCISDigit();
	}
	specialdigits[i] = '\0';
	
	strcpy(playerstring, specialdigits);
	strcpy(targetstring, specialdigits);
	
	//DON'T SET i EQUAL TO ANYTHING. JUST PICK UP WHERE YOU LEFT OFF.
	for (i = i; i < stringlength; i++)
	{
		targetstring[i] = RandomCISDigit();
		playerstring[i] = RandomCISDigit();
		if (playerstring[i] == targetstring[i])
		{
			playerstring[i] = playerstring[i] / 2 + 1; //SHOULD TAKE CARE OF ALL INSTANCES OF DUPLICATE DIGITS
		}
	}
	targetstring[i] = '\0';
	playerstring[i] = '\0';
	
	//NOW, BEHIND THE SCENES, THE "KEY DIGITS" ARE AT THE FRONT OF THE STRINGS. TO CHECK OUT IF THEY MATCH, WE SIMPLY KEEP A LOCAL INTEGER
	//COUNTER FOR EACH STRING. IF THEY MATCH WHEN THE "SYNCHRONIZE" BUTTON IS PRESSED, AND THE SPEEDS MATCH, WE'RE GOOD TO GO.
	
	//SET UP THE INTIAL SPEED FOR THE TARGETSTRING
	targetstringvelocity = 1;
	targetstringdirection = rand()%2; //LEFT-RIGHT;
	playerstringvelocity = 0;
	//SET UP THE INITIAL OFFSETS.
	targetstringoffset = rand()%stringlength;
	playerstringoffset = rand()%stringlength;
	
	timeskill = ((120 * player->psychskill.skills.electronics )/ MAX_SKILL) + (20 * player->psychskill.specializations.electronics);
	
	//SET UP THE INITIAL TIMER.
	timeleft.countdown = TRUE;
	timeleft.hours = 0;
	timeleft.minutes = timeskill / 60; //FOR NOW
	timeleft.seconds = timeskill % 60;
	timeleft.split = 89;
	
	while(!synchronize)
	{
		StartTimer(&timer);
		
		/************************************************************
		; //MOVE THE STRINGS, MANAGE WRAPAROUND
		************************************************************/
		
		//	tickcount1 = GetTickCount();
		//	if ((tickcount) > CIS_MAX_REFRESH_TICKS)
		//	{
		//		tickcount = 0;
		//PLAYER STRING
		if (playerstringdirection == CIS_VELOCITY_LEFT){
			playerstringoffset -= playerstringvelocity;
			if (playerstringoffset < 0)
			{
				playerstringoffset = stringlength + playerstringoffset; //+ because it's a negative offset.
			}
			if (playerstringoffset > stringlength)
			{
				playerstringoffset = playerstringoffset - stringlength;
			}
		}
		else if (playerstringdirection == CIS_VELOCITY_RIGHT)
		{
			playerstringoffset += playerstringvelocity;
			if (playerstringoffset < 0)
			{
				playerstringoffset = stringlength + playerstringoffset;
			}
			if (playerstringoffset > stringlength)
			{
				playerstringoffset = playerstringoffset - stringlength;
			}
		}
		
		//TARGET STRING
		if (targetstringdirection == CIS_VELOCITY_LEFT){
			targetstringoffset -= targetstringvelocity;
			if (targetstringoffset < 0)
			{
				targetstringoffset = stringlength + targetstringoffset; //+ because it's a negative offset.
			}
		}
		else if (targetstringdirection == CIS_VELOCITY_RIGHT)
		{
			targetstringoffset += targetstringvelocity;
			if (targetstringoffset > stringlength)
			{
				targetstringoffset = targetstringoffset - stringlength;
			}
		}
		
		//COPY THE STRINGS INTO THE BUFFERS.
		for (i = 0; i < CIS_MAX_DISPLAYABLE_DIGITS; i++)
		{
			playerdisplaystring[i] = cyclechars(playerstring, playerstringoffset + i, stringlength);
			targetdisplaystring[i] = cyclechars(targetstring, targetstringoffset + i, stringlength);
		}
		playerdisplaystring[i] = '\0';
		targetdisplaystring[i] = '\0';
		
		
		//PRINT THE STRINGS
		zerocursor();
		LYELLOW;
		printf("Time Remaining: ");
		SPrintMissionChrono(timebuf, &timeleft);
		printf(timebuf);
		
		if (!startsynchronize){
			LCYAN;
		}
		else{
			LRED;
		}
		setcursor(CIS_GRAPHIC_OFFSET_X_FROM_LEFT + CIS_TSTRING_OFFSET_X_FROM_LEFT, CIS_GRAPHIC_OFFSET_Y_FROM_TOP + CIS_TSTRING_OFFSET_Y_FROM_TOP);
		printf("%s", targetdisplaystring);
		if (!startsynchronize)
		{
			LGREEN;
		}
		else{
			LRED;
		}
		setcursor(CIS_GRAPHIC_OFFSET_X_FROM_LEFT + CIS_PSTRING_OFFSET_X_FROM_LEFT, CIS_GRAPHIC_OFFSET_Y_FROM_TOP + CIS_PSTRING_OFFSET_Y_FROM_TOP);
		printf("%s", playerdisplaystring);
		if (debugon)
		{
			LYELLOW;
			setcursor(0,0);
			printf("Key String: %s", specialdigits);
		}
		
		
		
		
		/************************************************************
		; Do the correct message fade.
		************************************************************/
		
		if (!startsynchronize)
		{
		/************************************************************
		; Set the cursor to the correct position based on the string.
			************************************************************/
			statuscoord.X = retrieveTextCenterH(statusmessages[CIS_STATUS_MESSAGE_AWAITING]);
			statuscoord.Y = CIS_GRAPHIC_OFFSET_Y_FROM_TOP + CIS_STATUS_OFFSET_Y_FROM_TOP;
			
			setcursor(statuscoord.X, statuscoord.Y);
			
			switch(statusmessagefade){
			case CIS_STATUS_MESSAGEFADE_FADEIN:
			case CIS_STATUS_MESSAGEFADE_FADEOUT:
				GREEN;
				printf("%s", statusmessages[CIS_STATUS_MESSAGE_AWAITING]);
				statusmessagefade++;
				break;
			case CIS_STATUS_MESSAGEFADE_HOLD:
				LGREEN;
				printf("%s", statusmessages[CIS_STATUS_MESSAGE_AWAITING]);
				statusmessagefade++;
				break;
			case CIS_STATUS_MESSAGEFADE_NONE:
				len = strlen(statusmessages[CIS_STATUS_MESSAGE_AWAITING]);
				while (len > 0){
					printf(" ");
					len--;
				}
				statusmessagefade = CIS_STATUS_MESSAGEFADE_FADEIN;
				break;
			}
		}
		else
		{
			
		/************************************************************
		; Set the cursor to the correct position based on the string.
			************************************************************/
			statuscoord.X = retrieveTextCenterH(statusmessages[CIS_STATUS_MESSAGE_SYNCHRONIZING]);
			statuscoord.Y = CIS_GRAPHIC_OFFSET_Y_FROM_TOP + CIS_STATUS_OFFSET_Y_FROM_TOP;
			
			setcursor(statuscoord.X, statuscoord.Y);
			
			switch(statusmessagefade){
			case CIS_STATUS_MESSAGEFADE_FADEIN:
			case CIS_STATUS_MESSAGEFADE_FADEOUT:
				RED;
				printf("%s", statusmessages[CIS_STATUS_MESSAGE_SYNCHRONIZING]);
				statusmessagefade++;
				break;
			case CIS_STATUS_MESSAGEFADE_HOLD:
				LRED;
				printf("%s", statusmessages[CIS_STATUS_MESSAGE_SYNCHRONIZING]);
				statusmessagefade++;
				break;
			case CIS_STATUS_MESSAGEFADE_NONE:
				len = strlen(statusmessages[CIS_STATUS_MESSAGE_AWAITING]);
				while (len > 0){
					printf(" ");
					len--;
				}
				statusmessagefade = CIS_STATUS_MESSAGEFADE_FADEIN;
				break;
			}
		}
		//		}
		
		
		if (!startsynchronize)
		{
			numevents = checkforinput();
			if (numevents > 1)
			{
				for (i = 1; i <= numevents; i++)
				{
					key = getinput(i);
					
					if (key == ' ')
					{
						startsynchronize = TRUE;
						//CLEAR OUT THE STATUSLINE BUFFER
						statuscoord.X = CIS_GRAPHIC_OFFSET_X_FROM_LEFT + CIS_STATUS_OFFSET_X_FROM_LEFT;
						statuscoord.Y = CIS_GRAPHIC_OFFSET_Y_FROM_TOP + CIS_STATUS_OFFSET_Y_FROM_TOP;
						setcursor(statuscoord.X, statuscoord.Y);
						for (i = 0; i < CIS_MAX_DISPLAYABLE_DIGITS; i++){
							printf(" ");
						}
						break;
						
					}
					else if (key == VK_RIGHT)
					{
						if (!playerstringvelocity)
						{
							playerstringdirection = CIS_VELOCITY_LEFT;
							playerstringvelocity++;
						}
						else if (playerstringdirection == CIS_VELOCITY_LEFT){
							if (playerstringvelocity < CIS_MAX_STRING_VELOCITY)
							{
								playerstringvelocity ++;
							}
						}
						else if (playerstringdirection == CIS_VELOCITY_RIGHT)
						{
							playerstringvelocity --;
							if (!playerstringvelocity){
								playerstringdirection = CIS_VELOCITY_LEFT;
							}
						}
						
						
					}
					else if (key == VK_LEFT)
					{
						
						if (!playerstringvelocity)
						{
							playerstringdirection = CIS_VELOCITY_RIGHT;
							playerstringvelocity++;
						}
						else if (playerstringdirection == CIS_VELOCITY_RIGHT){
							if (playerstringvelocity < CIS_MAX_STRING_VELOCITY)
							{
								playerstringvelocity ++;
							}
						}
						else if (playerstringdirection == CIS_VELOCITY_LEFT)
						{
							playerstringvelocity --;
							if (!playerstringvelocity){
								playerstringdirection = CIS_VELOCITY_RIGHT;
							}
						}
						
						
					}
					else if (key == 'D')
					{
						bToggle(&debugon);
					}
					else if (key == VK_ESCAPE)
				{
				return FALSE;
				}
					FLUSH;
				}
				clearinputrecords();
			}
		}
		if (startsynchronize)
		{
			synchronizetimer--;
			Sleep(20);
			if (synchronizetimer <= 0)
			{
				synchronize = TRUE; //SYNCHRONIZED.
			}
		}
		else{
			//		tickcount2 = GetTickCount();
			//		tickcount += tickcount2 - tickcount1;
			
			/************************************************************
			; THE MAIN SLEEP
			************************************************************/
			
			Sleep(90);
			EndTimer(&timer);
			timeleft.split -= SplitElapsed(&timer);
			if (MissionChronoTimeUp(&timeleft)){
				free(playerstring);
				free(targetstring);
				StopSong();
				return FAILURE;
			}
			calibratemissionchrono(&timeleft);
			
		}
	}
	
	//CHECK THE VARIABLES FOR SUCCESS...
	if (targetstringoffset == playerstringoffset && abs(targetstringvelocity) == abs(playerstringvelocity) && targetstringdirection == playerstringdirection)
	{
		Sleep(500);
		free(playerstring);
		free(targetstring);
		statuscoord.X = CIS_GRAPHIC_OFFSET_X_FROM_LEFT + CIS_STATUS_OFFSET_X_FROM_LEFT;
		statuscoord.Y = CIS_GRAPHIC_OFFSET_Y_FROM_TOP + CIS_STATUS_OFFSET_Y_FROM_TOP;
		setcursor(statuscoord.X, statuscoord.Y);
		for (i = 0; i < CIS_MAX_DISPLAYABLE_DIGITS; i++){
			printf(" ");
		}
		/************************************************************
		; Set the cursor to the correct position based on the string.
		************************************************************/
		statuscoord.X = retrieveTextCenterH(statusmessages[CIS_STATUS_MESSAGE_SUCCESS]);
		statuscoord.Y = CIS_GRAPHIC_OFFSET_Y_FROM_TOP + CIS_STATUS_OFFSET_Y_FROM_TOP;
		RED;
		setcursor(statuscoord.X, statuscoord.Y);
		printf("%s", statusmessages[CIS_STATUS_MESSAGE_SUCCESS]);
		Sleep(100);
		LRED;
		setcursor(statuscoord.X, statuscoord.Y);
		printf("%s", statusmessages[CIS_STATUS_MESSAGE_SUCCESS]);
		Sleep(1000);
		StopSong();
		return SUCCESS;
	}
	else
	{
		Sleep(500);
		statuscoord.X = CIS_GRAPHIC_OFFSET_X_FROM_LEFT + CIS_STATUS_OFFSET_X_FROM_LEFT;
		statuscoord.Y = CIS_GRAPHIC_OFFSET_Y_FROM_TOP + CIS_STATUS_OFFSET_Y_FROM_TOP;
		setcursor(statuscoord.X, statuscoord.Y);
		for (i = 0; i < CIS_MAX_DISPLAYABLE_DIGITS; i++){
			printf(" ");
		}
		free(playerstring);
		free(targetstring);
		/************************************************************
		; Set the cursor to the correct position based on the string.
		************************************************************/
		statuscoord.X = retrieveTextCenterH(statusmessages[CIS_STATUS_MESSAGE_FAILURE]);
		statuscoord.Y = CIS_GRAPHIC_OFFSET_Y_FROM_TOP + CIS_STATUS_OFFSET_Y_FROM_TOP;
		RED;
		setcursor(statuscoord.X, statuscoord.Y);
		printf("%s", statusmessages[CIS_STATUS_MESSAGE_FAILURE]);
		Sleep(100);
		LRED;
		setcursor(statuscoord.X, statuscoord.Y);
		printf("%s", statusmessages[CIS_STATUS_MESSAGE_FAILURE]);
		Sleep(1000);
		StopSong();
		return FAILURE;
	}
	
	StopSong();
	return SUCCESS;
}


//--------------------------------------------------------------------------------------------
// Function Name - RandomCISDigit
//
// Description   - Returns a random alpha/number/special character within the ASCII range of 47 - 123
//
// Return Type   - char 
//
//
// Author        - Charles Cox
// Date Modified - 04/18/2000
//--------------------------------------------------------------------------------------------
char RandomCISDigit()
{
	return random(47, 123);
}


//--------------------------------------------------------------------------------------------
// Function Name - 
//
// Description   - Will return the next character at a given position within a string of characters, 
//	even if the number is past the end of the array. It loops around.
//
// Return Type   - char cyclechars 
//
// Arguments     - char* array ( The string to return a character from )
//               - int pos ( The position in the array (CAN BE OUT OF BOUNDS) )
//               - int max ( The number of characters in the string )
//
// Author        - Charles Cox
// Date Modified - 04/18/2000
//--------------------------------------------------------------------------------------------
char cyclechars (char* array, int pos, int max)
{
	return(array[pos%max]);
}


//--------------------------------------------------------------------------------------------
// Function Name - LISTest
//
// Description   - The LIS Test. Returns a success boolean
//
// Return Type   - BOOL 
//
// Arguments     - PPLAYER player ( The pointer to the player structure )
//               - PDOOR door ( The door containing the lock. )
//
// Author        - Charles Cox
// Date Modified - 04/18/2000
//--------------------------------------------------------------------------------------------
BOOL LISTest(PPLAYER player, PDOOR door)
{
	
	
/************************************************************
; TO DO'S
TIMER
VICTORY / FAILURE SEQUENCE
SOUND
CATCHY LIS GRAPHICS
	************************************************************/
	MISSIONCHRONO timeleft;
	TIMER timer;
	char timebuf[80];
	LISSYSTEM locksystem;
	int i, k, currentblock, currentblock2, ydrawcoord,key, numevents, prospectivepoint, pincheck1, pinchecks;
	BOOL endLISloop = FALSE;
	BOOL success = FALSE;
	BOOL debugOn = FALSE;
	COORD extraoffset = {10,2}; //ANY EXTRA OFFSET WE WANT TO ADD
	COORD zeroed = {0,0};
	int timeskill;
	
	
	//CLEAR THE SCREEN. LET'S GO.
	LoopSong(globals.musiclist.songs[ELECTRONICS_SONG]);
	cls(0);
	LGREEN;
	printgraphic(globals.graphicslist, zeroed, PROMPTRESPONSE_GRAPHIC_ID);
	
	
	//TO GENERATE THE LOCKS, WE NEED A NUMBER OF RINGS EQUAL TO THE NUMBER OF LOCK RINGS ON THE DOOR. WE THEN SET THE TIMELEFT
	//TO A BASE LEVEL PLUS BONUSES FOR MECHANICAL SKILL.
	locksystem.pointsperring = LIS_MAX_RINGPOINTS;
	locksystem.numberofrings = door->lockrings;
	locksystem.currentring = 0;
	locksystem.rings = (PLISRING) malloc(sizeof(LISRING) * locksystem.numberofrings);
	
	timeskill = ((90 * player->psychskill.skills.electronics )/ MAX_SKILL) + (20 * player->psychskill.specializations.electronics);
	
	
	/************************************************************
	; INITIALIZING EACH RING
	************************************************************/
	
	for (i = 0; i < locksystem.numberofrings; i++)
	{
		locksystem.rings[i].xoffset = LIS_X_OFFSET * i;
		locksystem.rings[i].ringnumber = i;
		locksystem.rings[i].points = (PLISPOINT) malloc(sizeof(LISPOINT) * locksystem.pointsperring);
		ydrawcoord = 0;
		/************************************************************
		; INITIALIZE AND SET UP DRAWING COORDINATES FOR ALL POINTS IN A RING
		************************************************************/
		for (k =0; k < locksystem.pointsperring; k++){
			
			//WE SET THE BOOLEANS TO FALSE...THESE ARE INITIALIZED LATER.
			locksystem.rings[i].points[k].point = k;
			locksystem.rings[i].points[k].blocked = FALSE;
			/************************************************************
			; SPECIAL DRAWING CASES
			************************************************************/
			if(k == 0){
				locksystem.rings[i].points[k].relativedrawcoord.X = 1;
				locksystem.rings[i].points[k].relativedrawcoord.Y = 1;
				ydrawcoord++;
			}
			else if (k == locksystem.pointsperring / 2){
				locksystem.rings[i].points[k].relativedrawcoord.X = 1;
				locksystem.rings[i].points[k].relativedrawcoord.Y = locksystem.pointsperring / 2;
			}
			
			/************************************************************
			; GENERAL CASES OF POINT INITIALIZATION
			************************************************************/
			else{
				if (k < locksystem.pointsperring /2){ //IF IT'S NOT A MULTIPLE OF 2, DRAW IT ON THE LEFT SIDE. ELSE, DRAW ON RIGHT SIDE.
					locksystem.rings[i].points[k].relativedrawcoord.X = 0;
					ydrawcoord++;
				}
				else{
					locksystem.rings[i].points[k].relativedrawcoord.X = 2;
					ydrawcoord--;
				}
				locksystem.rings[i].points[k].relativedrawcoord.Y = ydrawcoord;
			}
		}
	}
	
	
	/************************************************************
	; Randomize The Block Location
	************************************************************/
	//FIRST BLOCK ON EACH RING
	for (i = 0; i < locksystem.numberofrings; i++)
	{
		currentblock = rand()%locksystem.pointsperring;
		locksystem.rings[i].points[currentblock].blocked = TRUE;
		
		//GET A UNIQUE SECOND BLOCK
		for(;;){
			if ((currentblock2 = rand()%locksystem.pointsperring) != currentblock){
				locksystem.rings[i].points[currentblock2].blocked = TRUE;
				break;
			}
		}
	}
	
	/************************************************************
	; Randomize The Pin Location
	************************************************************/
	for (i = 0; i < locksystem.numberofrings; i++)
	{
		for(;;){
			locksystem.rings[i].pinpoint = rand()%locksystem.pointsperring;
			if (!locksystem.rings[i].points[locksystem.rings[i].pinpoint].blocked){
				break;
			}
		}
	}
	
	//WE ARE ALL SET!!!!!!!!!!!!!
	
	//DRAW THE FIRST ITERATION.
	for (i = 0; i < locksystem.numberofrings; i++){
		if (i == locksystem.currentring){
			DrawLISRing(&locksystem, i, extraoffset, TRUE, debugOn);
		}
		else{
			DrawLISRing(&locksystem, i, extraoffset, FALSE, debugOn);
		}
	}
	
	//SET UP THE INITIAL TIMER.
	timeleft.countdown = TRUE;
	timeleft.hours = 0;
	timeleft.minutes = timeskill / 60; //FOR NOW
	timeleft.seconds = timeskill % 60;
	timeleft.split = 89;
	
	
	
	
	
	/************************************************************
	; BEGIN THE LOOP!!!
	************************************************************/
	
	while(!endLISloop) {//END-LESS-LOOP? HAHAHAHA oh. sorry.
		//CHECK INPUT, DO TIME, CHECK FOR TIME UP, DO GRAPHICS.
		zerocursor();
		setcursor(1, 1);
		LYELLOW;
		printf("Time Remaining: ");
		SPrintMissionChrono(timebuf, &timeleft);
		printf(timebuf);
		StartTimer(&timer);
		numevents = checkforinput();
		if (numevents > 1)
		{
			//IF INPUT CHECK TYPE.
			for (i = 1; i <= numevents; i++)
			{
				key = getinput(i);
				
				//IF PIN MOVE, CHECK FOR BLOCK, MOVE PIN, DRAW RING, CHECK FOR WIN.
				if (key == VK_UP)
				{
					prospectivepoint = locksystem.rings[locksystem.currentring].pinpoint;
					prospectivepoint--;
					if (prospectivepoint < 0){
						prospectivepoint = locksystem.pointsperring - 1;
					}
					if (!locksystem.rings[locksystem.currentring].points[prospectivepoint].blocked){
						LYELLOW;
						setcursor(locksystem.rings[locksystem.currentring].points[locksystem.rings[locksystem.currentring].pinpoint].relativedrawcoord.X + locksystem.rings[locksystem.currentring].xoffset + extraoffset.X ,locksystem.rings[locksystem.currentring].points[locksystem.rings[locksystem.currentring].pinpoint].relativedrawcoord.Y + extraoffset.Y);
						if (locksystem.rings[locksystem.currentring].pinpoint == 0 || locksystem.rings[locksystem.currentring].pinpoint == locksystem.pointsperring / 2){
							printf("/");
						}
						
						else{
							printf("|");
						}
						locksystem.rings[locksystem.currentring].pinpoint = prospectivepoint;
						LRED;
						setcursor(locksystem.rings[locksystem.currentring].points[locksystem.rings[locksystem.currentring].pinpoint].relativedrawcoord.X + locksystem.rings[locksystem.currentring].xoffset + extraoffset.X ,locksystem.rings[locksystem.currentring].points[locksystem.rings[locksystem.currentring].pinpoint].relativedrawcoord.Y + extraoffset.Y);
						printf("-");
						
					}
					else{ //YOU HIT A BLOCK, PLAY A SOUND.
					}
				}
				if (key == VK_DOWN)
				{
					prospectivepoint = locksystem.rings[locksystem.currentring].pinpoint;
					prospectivepoint++;
					if (prospectivepoint > locksystem.pointsperring - 1){
						prospectivepoint = 0;
					}
					if (!locksystem.rings[locksystem.currentring].points[prospectivepoint].blocked){
						LYELLOW;
						setcursor(locksystem.rings[locksystem.currentring].points[locksystem.rings[locksystem.currentring].pinpoint].relativedrawcoord.X + locksystem.rings[locksystem.currentring].xoffset + extraoffset.X ,locksystem.rings[locksystem.currentring].points[locksystem.rings[locksystem.currentring].pinpoint].relativedrawcoord.Y + extraoffset.Y);
						if (locksystem.rings[locksystem.currentring].pinpoint == 0 || locksystem.rings[locksystem.currentring].pinpoint == locksystem.pointsperring /2){
							printf("/");
						}
						
						else{
							printf("|");
						}
						locksystem.rings[locksystem.currentring].pinpoint = prospectivepoint;
						LRED;
						setcursor(locksystem.rings[locksystem.currentring].points[locksystem.rings[locksystem.currentring].pinpoint].relativedrawcoord.X + locksystem.rings[locksystem.currentring].xoffset + extraoffset.X ,locksystem.rings[locksystem.currentring].points[locksystem.rings[locksystem.currentring].pinpoint].relativedrawcoord.Y + extraoffset.Y);
						printf("-");
						
					}
					else{ //YOU HIT A BLOCK, PLAY A SOUND.
					}
				}
				//IF RING MOVE, CHECK BOUNDS, MOVE RING, DRAW LAST RING IN MUTED COLORS, NEW RING IN BOLD COLORS.
				else if (key == VK_RIGHT)
				{
					if (locksystem.currentring < locksystem.numberofrings  -1){
						DrawLISRing(&locksystem, locksystem.currentring, extraoffset, FALSE, debugOn);
						locksystem.currentring++;
						DrawLISRing(&locksystem, locksystem.currentring, extraoffset, TRUE, debugOn);
					}
				}
				else if (key == VK_LEFT)
				{
					if (locksystem.currentring > 0){
						DrawLISRing(&locksystem, locksystem.currentring, extraoffset, FALSE, debugOn);
						locksystem.currentring--;
						DrawLISRing(&locksystem, locksystem.currentring, extraoffset, TRUE, debugOn);
					}
				}
				else if (key == 'D')
				{
					bToggle(&debugOn);
					for (i = 0; i < locksystem.numberofrings; i++){
						if (i == locksystem.currentring){
							DrawLISRing(&locksystem, i, extraoffset, TRUE, debugOn);
						}
						else{
							DrawLISRing(&locksystem, i, extraoffset, FALSE, debugOn);
						}
					}
				}
				else if (key == 'J')
				{
				/************************************************************
				; Randomize The Block Location
					************************************************************/
					//FIRST BLOCK ON EACH RING
					for (i = 0; i < locksystem.numberofrings; i++)
					{
						for (k = 0; k < locksystem.pointsperring; k++){
							locksystem.rings[i].points[k].blocked = FALSE;
						}
						currentblock = rand()%locksystem.pointsperring;
						locksystem.rings[i].points[currentblock].blocked = TRUE;
						
						//GET A UNIQUE SECOND BLOCK
						for(;;){
							if ((currentblock2 = rand()%locksystem.pointsperring) != currentblock){
								locksystem.rings[i].points[currentblock2].blocked = TRUE;
								break;
							}
						}
					}
					
					/************************************************************
					; Randomize The Pin Location
					************************************************************/
					for (i = 0; i < locksystem.numberofrings; i++)
					{
						for(;;){
							locksystem.rings[i].pinpoint = rand()%locksystem.pointsperring;
							if (!locksystem.rings[i].points[locksystem.rings[i].pinpoint].blocked){
								break;
							}
						}
					}
					
					
					for (i = 0; i < locksystem.numberofrings; i++){
						if (i == locksystem.currentring){
							DrawLISRing(&locksystem, i, extraoffset, TRUE, debugOn);
						}
						else{
							DrawLISRing(&locksystem, i, extraoffset, FALSE, debugOn);
						}
					}
					
				}
				else if (key == VK_ESCAPE)
				{
					endLISloop = TRUE;
					success = FALSE;
				}
				FLUSH;
			}
			
			//CHECK FOR VICTORY HERE.
			pincheck1 = locksystem.rings[0].pinpoint;
			pinchecks = 1;
			for (i = 1; i < locksystem.numberofrings; i++){
				if (pincheck1 != locksystem.rings[i].pinpoint){
					break; //NOT VICTORY.
				}
				pinchecks++;
			}
			if (pinchecks >= locksystem.numberofrings){
				endLISloop = TRUE;
				success = TRUE;
			}
			clearinputrecords();
		}
		
		Sleep(80);
		EndTimer(&timer);
		timeleft.split -= SplitElapsed(&timer);
		if (MissionChronoTimeUp(&timeleft)){
			
			endLISloop = TRUE;
			success = FALSE;
			break;
		}
		calibratemissionchrono(&timeleft);
		
		}
		
		/************************************************************
		; GARBAGE CLEANUP!
		************************************************************/
		for (i = 0; i < locksystem.numberofrings; i++)
		{
			free(locksystem.rings[i].points);
		}
		free(locksystem.rings);
		StopSong();
		/************************************************************
		; DO VICTORY HERE
		************************************************************/
		if (success == TRUE){
			//DO SUCCESS SHIT THEN RETURN SUCCESS
			return SUCCESS;
		}
		
		
		else{ //WE'VE ENDED THE LOOP WITHOUT SUCCESS. WHAT'S UP?
			return FAILURE;
		}
		/************************************************************
		; 
		************************************************************/
		
		
		
}


//--------------------------------------------------------------------------------------------
// Function Name - DrawLISRing
//
// Description   - Draws a ring of the LIS at an offset, with or without bolding and/or blocks shown.
//
// Return Type   - void 
//
// Arguments     - PLISSYSTEM lock ( The lock system structure pointer )
//               - int ringnumber ( The number of the ring you wish to print )
//               - COORD extraoffset ( Any extra screen offset you want )
//               - BOOL bolded ( Whether or not to show this ring as "ACTIVE" )
//               - BOOL showblocks ( Whether or not to show the blocks on this ring )
//
// Author        - Charles Cox
// Date Modified - 04/18/2000
//--------------------------------------------------------------------------------------------
void DrawLISRing(PLISSYSTEM lock, int ringnumber, COORD extraoffset, BOOL bolded, BOOL showblocks)
{
	PLISRING ring = &lock->rings[ringnumber];
	int i;
	
	
	for (i =0; i < lock->pointsperring; i++){
		setcursor(ring->points[i].relativedrawcoord.X + ring->xoffset + extraoffset.X, ring->points[i].relativedrawcoord.Y + extraoffset.Y);
		if (i == ring->pinpoint){ //IF IT'S THE PIN
			LRED;
			printf("-");
		}

		else if (ring->points[i].blocked && showblocks){
			LBLUE;
			printf("*");
		}

		else if (i == 0 || i == lock->pointsperring /2){ //IF IT'S THE TOP OR BOTTOM
			if (bolded){
				LYELLOW;
			}
			else{
				GRAY;
			}
			printf("/");
		}
		else{ //IF IT'S ANYTHING ELSE
			if (bolded){
				LYELLOW;
			}
			else{
				GRAY;
			}
			printf("|");
		}
	}
}


//--------------------------------------------------------------------------------------------
// Function Name - LISDebug
//
// Description   - LIS Debugging Utility. Enters standard values, passes on success boolean.
//
// Return Type   - BOOL 
//
//
// Author        - Charles Cox
// Date Modified - 04/18/2000
//--------------------------------------------------------------------------------------------
BOOL LISDebug()
{
	PLAYER playerdebug;
	DOOR doordebug;
	
	doordebug.lock = DOOR_LOCK_LIS;
	doordebug.lockrings = 5;
	playerdebug.psychskill.skills.electronics;
	playerdebug.psychskill.specializations.electronics = FALSE;
	return LISTest(&playerdebug, &doordebug);
}


//--------------------------------------------------------------------------------------------
// Function Name - AtomicActivation
//
// Description   - Activation of atomic device, will return TRUE if successfully activated
//
// Return Type   - BOOL 
//
// Arguments     - PPLAYER player ( The pointer to the player structure )
//
// Author        - Charles Cox
// Date Modified - 04/24/2000
//--------------------------------------------------------------------------------------------
BOOL AtomicActivation(PPLAYER player)
{
	
	
	
	BOOL sequencecomplete = FALSE;
	BOOL settimer = FALSE;
	BOOL devicearmed = FALSE;
//	PROMPTRESPONSE response;
	int currentcodedigit = 0;
	int key, numevents, i;
	int endmessagetimer;
	int atomictimer = 15;
	char timerstring[MAX_ATOMIC_CODE_DIGITS] = "";
	char codesequence[MAX_ATOMIC_CODE_DIGITS] = "";
	COORD devicedrawcoords = {ATOMIC_DEVICE_X_OFFSET, ATOMIC_DEVICE_Y_OFFSET};

	if(!player->currentmissioninfo.specifics.cautery.hasbomb){
		return false;
	}
	
	//VARIABLES FOR THE LIGHTRACK
	ATOMICLIGHT lightrack1;
	ATOMICLIGHT lightrack2;
	ATOMICLIGHT lightrack3;
	
	//VARIABLES FOR THE INDEPENDENT LIGHTS
	ATOMICLIGHT singlelight1;
	ATOMICLIGHT singlelight2;
	
	/************************************************************
	; SET UP THE LIGHTS
	************************************************************/
	//LIGHT RACK
	lightrack1.status = ATOMIC_LIGHT_OFF;
	lightrack1.type = ATOMIC_LIGHT_FADE;
	lightrack1.color = FOREGROUND_GREEN;
	lightrack1.topleft.X = ATOMIC_LIGHTRACK_X_OFFSET;
	lightrack1.topleft.Y = ATOMIC_LIGHTRACK_Y_OFFSET;
	lightrack1.dimensions.X = ATOMIC_LIGHT_WIDTH;
	lightrack1.dimensions.Y = ATOMIC_LIGHT_HEIGHT;
	
	lightrack2.status = ATOMIC_LIGHT_DIMUP;
	lightrack2.type = ATOMIC_LIGHT_FADE;
	lightrack2.color = FOREGROUND_GREEN;
	lightrack2.topleft.X = ATOMIC_LIGHTRACK_X_OFFSET + ATOMIC_LIGHT_WIDTH + 1;
	lightrack2.topleft.Y = ATOMIC_LIGHTRACK_Y_OFFSET;
	lightrack2.dimensions.X = ATOMIC_LIGHT_WIDTH;
	lightrack2.dimensions.Y = ATOMIC_LIGHT_HEIGHT;
	
	lightrack3.status = ATOMIC_LIGHT_BRIGHT;
	lightrack3.type = ATOMIC_LIGHT_FADE;
	lightrack3.color = FOREGROUND_GREEN;
	lightrack3.topleft.X = (ATOMIC_LIGHTRACK_X_OFFSET + (ATOMIC_LIGHT_WIDTH * 2)) + 2;
	lightrack3.topleft.Y = ATOMIC_LIGHTRACK_Y_OFFSET;
	lightrack3.dimensions.X = ATOMIC_LIGHT_WIDTH;
	lightrack3.dimensions.Y = ATOMIC_LIGHT_HEIGHT;
	
	//SINGLE LIGHTS
	singlelight1.status = ATOMIC_LIGHT_BRIGHT;
	singlelight1.type = ATOMIC_LIGHT_FLASH;
	singlelight1.color = FOREGROUND_RED;
	singlelight1.topleft.X = ATOMIC_SINGLELIGHT1_X_OFFSET;
	singlelight1.topleft.Y = ATOMIC_SINGLELIGHT1_Y_OFFSET;
	singlelight1.dimensions.X = ATOMIC_LIGHT_WIDTH;
	singlelight1.dimensions.Y = ATOMIC_LIGHT_HEIGHT;
	
	singlelight2.status = ATOMIC_LIGHT_OFF;
	singlelight2.type = ATOMIC_LIGHT_FLASH;
	singlelight2.color = FOREGROUND_GREEN;
	singlelight2.topleft.X = ATOMIC_SINGLELIGHT2_X_OFFSET;
	singlelight2.topleft.Y = ATOMIC_SINGLELIGHT2_Y_OFFSET;
	singlelight2.dimensions.X = ATOMIC_LIGHT_WIDTH;
	singlelight2.dimensions.Y = ATOMIC_LIGHT_HEIGHT;
	
	
		//DRAW THE DEVICE
	GRAY;
	printgraphic(globals.graphicslist, devicedrawcoords, ATOMICSCREEN_GRAPHIC_ID);
	RED;
	setcursor(ATOMIC_DEVICE_X_OFFSET + ATOMIC_MESSAGES_X_OFFSET, ATOMIC_DEVICE_Y_OFFSET + ATOMIC_MESSAGES_Y_OFFSET);
	printf("ARMING CODE REQUIRED");
	
	
	for(;;)
	{
		//WE WANT TO DO GLITZY STUFF HERE
		AtomicLight(&lightrack1);
		AtomicLight(&lightrack2);
		AtomicLight(&lightrack3);
		AtomicLight(&singlelight1);
		AtomicLight(&singlelight2);
		
		
		/************************************************************
		; THE INPUT LOOP
		************************************************************/
		if (!sequencecomplete)
		{
			numevents = checkforinput();
			if (numevents > 1)
			{
				//IF INPUT CHECK TYPE.
				for (i = 1; i <= numevents; i++)
				{
					key = getinput(i);
					if (isdigit(key) || isalpha(key))
					{
						LCYAN;
						setcursor(currentcodedigit+ATOMIC_CODE_X_OFFSET + ATOMIC_DEVICE_X_OFFSET, ATOMIC_DEVICE_Y_OFFSET + ATOMIC_CODE_Y_OFFSET);
						printf("%c", key);
						//DO NOT USE A SYNCHRONOUS BEEP
						
						codesequence[currentcodedigit]  = key;
						currentcodedigit++;
					}
					else if (key == VK_BACK)
					{
						setcursor(ATOMIC_CODE_X_OFFSET + ATOMIC_DEVICE_X_OFFSET, ATOMIC_DEVICE_Y_OFFSET + ATOMIC_CODE_Y_OFFSET);
						for (i = 0; i < MAX_ATOMIC_CODE_DIGITS -1; i++){
							
							printf(" ");
						}
						currentcodedigit = 0;
					}
					if (currentcodedigit >= MAX_ATOMIC_CODE_DIGITS - 1){
						codesequence[MAX_ATOMIC_CODE_DIGITS - 1] = '\0';
						sequencecomplete = TRUE;
						
						
						setcursor(ATOMIC_DEVICE_X_OFFSET + ATOMIC_MESSAGES_X_OFFSET, ATOMIC_DEVICE_Y_OFFSET + ATOMIC_MESSAGES_Y_OFFSET);
						if (!strcmp(codesequence, player->currentmissioninfo.specifics.cautery.detcode)){
							//SHOW GLITZY ACTIVATED THING
							endmessagetimer = 20;
							for (i = 0; i < ATOMIC_MESSAGES_LENGTH; i++){
								printf(" ");
							}
							settimer = TRUE;
							lightrack1.color = FOREGROUND_RED;
							lightrack2.color = FOREGROUND_RED;
							lightrack3.color = FOREGROUND_RED;
							singlelight1.color = FOREGROUND_RED;
							singlelight2.color = FOREGROUND_RED;
							lightrack1.status = ATOMIC_LIGHT_OFF;
							lightrack2.status = ATOMIC_LIGHT_OFF;
							lightrack3.status = ATOMIC_LIGHT_OFF;
							LRED;
							setcursor(ATOMIC_DEVICE_X_OFFSET + ATOMIC_MESSAGES_X_OFFSET, ATOMIC_DEVICE_Y_OFFSET + ATOMIC_MESSAGES_Y_OFFSET);
							playMIXsound(ATOMIC_PRIME_SOUND);
							printf("DEVICE PRIMED");
						}
						else{
							endmessagetimer = 40;
							for (i = 0; i < ATOMIC_MESSAGES_LENGTH; i++){
								printf(" ");
							}
							LGREEN;
							lightrack1.color = FOREGROUND_GREEN;
							lightrack2.color = FOREGROUND_GREEN;
							lightrack3.color = FOREGROUND_GREEN;
							singlelight1.color = FOREGROUND_GREEN;
							singlelight2.color = FOREGROUND_GREEN;
							lightrack1.status = ATOMIC_LIGHT_OFF;
							lightrack2.status = ATOMIC_LIGHT_OFF;
							lightrack3.status = ATOMIC_LIGHT_OFF;
							setcursor(ATOMIC_DEVICE_X_OFFSET + ATOMIC_MESSAGES_X_OFFSET, ATOMIC_DEVICE_Y_OFFSET + ATOMIC_MESSAGES_Y_OFFSET);
							playMIXsound(ATOMIC_SAFE_SOUND);
							printf("DEVICE SAFTIED");
						}
					}
					FLUSH;
					clearinputrecords();
				}
			}
		}
		
		
		/************************************************************
		; SLEEP FOR A PARTICULAR AMOUNT OF TIME
		************************************************************/
		Sleep(90);
		if (sequencecomplete)
		{
			endmessagetimer--;
			if (endmessagetimer < 1){
				break;
			}
		}
	}
	
	if (settimer)
	{
		atomictimer = 15;
		sprintf(timerstring, "%2.2d:00", atomictimer);
		
		setcursor(ATOMIC_CODE_X_OFFSET + ATOMIC_DEVICE_X_OFFSET, ATOMIC_DEVICE_Y_OFFSET + ATOMIC_CODE_Y_OFFSET);
		for (i = 0; i < MAX_ATOMIC_CODE_DIGITS; i++){
			
			printf(" ");
		}
		setcursor(ATOMIC_DEVICE_X_OFFSET + ATOMIC_MESSAGES_X_OFFSET, ATOMIC_DEVICE_Y_OFFSET + ATOMIC_MESSAGES_Y_OFFSET);
		for (i = 0; i < ATOMIC_MESSAGES_LENGTH; i++){
			printf(" ");
		}
		setcursor(ATOMIC_DEVICE_X_OFFSET + ATOMIC_MESSAGES_X_OFFSET, ATOMIC_DEVICE_Y_OFFSET + ATOMIC_MESSAGES_Y_OFFSET);
		printf("SET TIMER");
		LRED;
		setcursor(ATOMIC_CODE_X_OFFSET + ATOMIC_DEVICE_X_OFFSET, ATOMIC_DEVICE_Y_OFFSET + ATOMIC_CODE_Y_OFFSET);
		printf(timerstring);
		while(!devicearmed)
		{
			AtomicLight(&lightrack1);
			AtomicLight(&lightrack2);
			AtomicLight(&lightrack3);
			AtomicLight(&singlelight1);
			AtomicLight(&singlelight2);
			
			
			numevents = checkforinput();
			if (numevents > 1)
			{
				//IF INPUT CHECK TYPE.
				for (i = 1; i <= numevents; i++)
				{
					key = getinput(i);
					if (key == VK_UP)
					{
						if (atomictimer < 90)
						{
							atomictimer++;
							sprintf(timerstring, "%d:00", atomictimer);
							LRED;
							setcursor(ATOMIC_CODE_X_OFFSET + ATOMIC_DEVICE_X_OFFSET, ATOMIC_DEVICE_Y_OFFSET + ATOMIC_CODE_Y_OFFSET);
							printf(timerstring);
						}
					}
					else if (key == VK_DOWN)
					{
						if (atomictimer > 1){
							atomictimer--;
							sprintf(timerstring, "%d:00", atomictimer);
							LRED;
							setcursor(ATOMIC_CODE_X_OFFSET + ATOMIC_DEVICE_X_OFFSET, ATOMIC_DEVICE_Y_OFFSET + ATOMIC_CODE_Y_OFFSET);
							printf(timerstring);
						}
					}
					else if (key == VK_RETURN)
					{
						if (atomictimer > 0)
						{
							LRED;
							setcursor(ATOMIC_DEVICE_X_OFFSET + ATOMIC_MESSAGES_X_OFFSET, ATOMIC_DEVICE_Y_OFFSET + ATOMIC_MESSAGES_Y_OFFSET);
							playMIXsound(ATOMIC_ACTIVATE_SOUND);
							LoopSong(globals.musiclist.songs[ATOMIC_SONG]);
							printf("DEVICE ARMED");
							lightrack1.status = ATOMIC_LIGHT_BRIGHT;
							lightrack1.type = ATOMIC_LIGHT_FLASH;
							lightrack2.status = ATOMIC_LIGHT_OFF;
							lightrack2.type = ATOMIC_LIGHT_FLASH;
							lightrack3.status = ATOMIC_LIGHT_BRIGHT;
							lightrack3.type = ATOMIC_LIGHT_FLASH;
							player->currentmissioninfo.specifics.cautery.timetobombdetonation.minutes = atomictimer - 1;
							player->currentmissioninfo.specifics.cautery.timetobombdetonation.seconds = 59;
							player->currentmissioninfo.specifics.cautery.timetobombdetonation.split = 99;
							player->currentmissioninfo.specifics.cautery.timetobombdetonation.countdown = FALSE;
							calibratemissionchrono(&player->currentmissioninfo.specifics.cautery.timetobombdetonation);
							player->currentmissioninfo.specifics.cautery.timetobombdetonation.countdown = TRUE;
							player->currentmissioninfo.specifics.cautery.bombactive = true;
							player->currentmissioninfo.specifics.cautery.hasbomb = false;
							player->currentmissioninfo.primarycomplete = true;
							devicearmed = TRUE;
						}
					}
				}
				FLUSH;
				clearinputrecords();
			}
			Sleep(90);
		}
		endmessagetimer = 60;
		while(1)
		{
			AtomicLight(&lightrack1);
			AtomicLight(&lightrack2);
			AtomicLight(&lightrack3);
			AtomicLight(&singlelight1);
			AtomicLight(&singlelight2);
			endmessagetimer--;
			Sleep(90);
			if (endmessagetimer < 1){
				break;
			}
			
		}
	}
	
	
	
	
	if (!strcmp(codesequence, player->currentmissioninfo.specifics.cautery.detcode)){
		return TRUE;
	}
	else{
		return FALSE;
	}
}


//--------------------------------------------------------------------------------------------
// Function Name - AtomicLight
//
// Description   - Used with arming the atomic device, this draws and updates lights
//
// Return Type   - void 
//
// Arguments     - PATOMICLIGHT light ( Pointer to the light to update and draw )
//
// Author        - Charles Cox
// Date Modified - 04/25/2000
//--------------------------------------------------------------------------------------------
void AtomicLight(PATOMICLIGHT light)
{
	
	int i;
	
	setcursor(light->topleft.X + ATOMIC_DEVICE_X_OFFSET, light->topleft.Y + ATOMIC_DEVICE_Y_OFFSET);
	
	if (light->type == ATOMIC_LIGHT_FLASH){
		if (light->status != ATOMIC_LIGHT_OFF){
			setcolor(light->color | FOREGROUND_INTENSITY);
			for (i = 0; i < light->dimensions.X; i++){
				printf("%c", BLOCK_CHARACTER);
			}
			light->status = ATOMIC_LIGHT_OFF;
			return;
		}
		else{
			for (i = 0; i < light->dimensions.X; i++){
				//REMOVE THE LIGHT
				printf(" ");
			}
			light->status = ATOMIC_LIGHT_BRIGHT;
			return;
			
		}
	}
	else if (light->type == ATOMIC_LIGHT_FADE){
		if (light->status == ATOMIC_LIGHT_OFF){
			for (i = 0; i < light->dimensions.X; i++){
				//REMOVE THE LIGHT
				printf(" ");
			}
			light->status = ATOMIC_LIGHT_DIMUP;
			return;		
		}
		else if (light->status == ATOMIC_LIGHT_DIMUP){
			setcolor(light->color);
			for (i = 0; i < light->dimensions.X; i++){
				printf("%c", BLOCK_CHARACTER);
			}
			light->status = ATOMIC_LIGHT_BRIGHT;
			return;
		}
		else if(light->status == ATOMIC_LIGHT_BRIGHT){
			setcolor(light->color | FOREGROUND_INTENSITY);
			for (i = 0; i < light->dimensions.X; i++){
				printf("%c", BLOCK_CHARACTER);
			}
			light->status = ATOMIC_LIGHT_DIMDOWN;
			return;
		}
		else if (light->status  == ATOMIC_LIGHT_DIMDOWN){
			setcolor(light->color);
			for (i = 0; i < light->dimensions.X; i++){
				printf("%c", BLOCK_CHARACTER);
			}
			light->status = ATOMIC_LIGHT_OFF;
			return;
		}
	}
}

bool LoadArmoryGizmoList()
{
	POTYPE TempObject;
	FILE* fp;
	char buf[500];
	int counter = 0;
	int maxgizmos = MAX_GRENADES + MAX_GIZMOS;
	char bigbuf[50000];
	globals.armorygizmolist = createList();
	fp = fopen(ARMORY_GIZMO_FILE, "r");
	if(!fp){
		return false;
	}
	while(!feof(fp) && counter < maxgizmos)
	{
		//BEGIN.
		TempObject.armorygizmo.id = counter;
		fgets(buf, 500, fp);
		StripNewLine(buf);
		TempObject.armorygizmo.isgrenade = (atoi(buf) != 0);
		fgets(buf, 500, fp);
		StripNewLine(buf);
		TempObject.armorygizmo.type = atoi(buf);
		fgets(buf, 500, fp);
		StripNewLine(buf);
		strcpy(TempObject.armorygizmo.longname, buf);
		fgets(buf, 500, fp);
		StripNewLine(buf);
		strcpy(TempObject.armorygizmo.shortname, buf);
		fgets(bigbuf, 50000, fp);
		StripNewLine(bigbuf);
		TempObject.armorygizmo.desc = (char*)malloc(sizeof(char) * ((strlen(bigbuf)) + 1));
		strcpy(TempObject.armorygizmo.desc, bigbuf);
		fgets(buf, 500, fp);
		StripNewLine(buf);
		TempObject.armorygizmo.canhavemultiple = (atoi(buf) != 0);
		fgets(buf, 500, fp);
		StripNewLine(buf);
		TempObject.armorygizmo.weight = (float)atof(buf);
		fgets(buf, 500, fp);
		StripNewLine(buf);
		TempObject.armorygizmo.frequency = atoi(buf);
		TempObject.armorygizmo.isavailable = true;
		addtoList(globals.armorygizmolist, TempObject, CompareArmoryGizmo);
		counter++;
	}
	fclose(fp);
	return true;
}

void FreeArmoryGizmoList()
{
	PLIST walker = globals.armorygizmolist;
	walker->current = walker->head;
	for(;walker->current != NULL; walker->current = walker->current->nextnode)
	{
		free(walker->current->object.armorygizmo.desc);
	}
	freeList(globals.armorygizmolist);
	free(globals.armorygizmolist);
}


void GiveGrenade(PPLAYER player, int grenadetype)
{
	POTYPE gren;
	gren.gizmo.isgrenade = true;
	gren.gizmo.unit.grenade.primertime = -1;
	gren.gizmo.unit.grenade.grenadetype = grenadetype;
	gren.gizmo.isgrenade = true;
	gren.gizmo.index = player->p_gizmos->objcnt;
	addtoList(player->p_gizmos, gren, CompareGizmoIndex);
}


void GiveGizmo(PPLAYER player, int gizmotype)
{
	POTYPE giz;
	switch(gizmotype)
	{
	case GIZMO_TYPE_BUG:
		giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 10;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = BODY_TORSO;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_SPIKEMIKE:
		giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 5;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = BODY_TORSO;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_NVG:
		giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 1;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = BODY_HEAD;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_TAS:
		giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 1;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = BODY_HEAD;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_XRIFLE:
			giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 1;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = BODY_TORSO;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_FOVS:
			giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 5;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = BODY_TORSO;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_LIS:
				giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 1;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = BODY_TORSO;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_CIS:
				giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 1;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = BODY_TORSO;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_SBK:
		giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 3;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = BODY_TORSO;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_LENSES:
		giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 1;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = BODY_HEAD;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_RING:
		giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 1;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = 0;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_WATCH:
		giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 1;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = BODY_LSIDE;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_LEGPACKS:
		giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 1;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = BODY_ABDOMEN;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_BINOCULARS:
			giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 1;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = BODY_TORSO;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_C4TIMED:
			giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 1;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = 0;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_C4SWITCH:
			giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 1;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = 0;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_C4PROXIMITY:
			giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 1;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = 0;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_C4ELECTRONIC:
			giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 1;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = 0;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_OCTYLDICHLORIDE:
			giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 3;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = BODY_TORSO;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_GASMASK:
			giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = false;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 1;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = BODY_HEAD;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_TRAUMAPLATES:
			giz.gizmo.isgrenade = false;
		giz.gizmo.unit.device.active = true;
		giz.gizmo.unit.device.condition = 100;
		giz.gizmo.unit.device.devicetype = gizmotype;
		giz.gizmo.unit.device.numberleft = 1;
		giz.gizmo.index = player->p_gizmos->objcnt;
		giz.gizmo.unit.device.bodypart = BODY_TORSO;
		addtoList(player->p_gizmos, giz, CompareGizmoIndex);
	}
}

BOOL DoesListHaveGizmoType(int type, PLIST gizmolist, BOOL isgrenade)
{
	POTYPE tmpObj;
	PPOTYPE lstObj;
	
	
	if (gizmolist == NULL){
		return FALSE;
	}
	if (gizmolist->objcnt < 1){
		return FALSE;
	}
	
	if(!isgrenade){
		tmpObj.gizmo.unit.device.devicetype = type;
		
		
		lstObj = findObject(gizmolist, &tmpObj, CompareGizmoDeviceType);
		if (lstObj == NULL){
			return FALSE;
		}
		return TRUE;
	}
	
	else {
		tmpObj.gizmo.unit.grenade.grenadetype = type;
		lstObj = findObject(gizmolist, &tmpObj, CompareGizmoGrenadeType);
		if (lstObj == NULL){
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
	
}

int CompareGizmoIndex(PPOTYPE itm1, PPOTYPE itm2)
{
		if(itm1->gizmo.index == itm2->gizmo.index)
		return (0);
	return (1);
}
int CompareGizmoDeviceType(PPOTYPE itm1,PPOTYPE itm2) 
{
	if(itm1->gizmo.unit.device.devicetype == itm2->gizmo.unit.device.devicetype && !itm2->gizmo.isgrenade && !itm1->gizmo.isgrenade)
		return (0);
	return (1);
}

int CompareArmoryGizmo(PPOTYPE itm1, PPOTYPE itm2)
{
	if(itm1->armorygizmo.id == itm2->armorygizmo.id){
		return 0;
	}
	return 1;
}

int CompareArmoryGizmoBothTypes(PPOTYPE itm1, PPOTYPE itm2)
{
	if(itm1->armorygizmo.type == itm2->armorygizmo.type && itm1->armorygizmo.isgrenade == itm2->armorygizmo.isgrenade){
		return 0;
	}
	return 1;
}

int CompareGizmoGrenadeType(PPOTYPE itm1, PPOTYPE itm2)
{
	if(itm1->gizmo.unit.grenade.grenadetype == itm2->gizmo.unit.grenade.grenadetype){
		return 0;
	}
	return 1;
}

PGIZMO FindActiveGizmoOfType(int Type, PLIST GizmoList)
{
	
	if(!GizmoList){
		return NULL;
	}
	PLIST Walker = GizmoList;
	
	for(Walker->current = Walker->head; Walker->current != NULL; Walker->current = Walker->current->nextnode){
		if(Walker->current->object.gizmo.unit.device.devicetype == Type && Walker->current->object.gizmo.unit.device.active == TRUE && Walker->current->object.gizmo.isgrenade == false){
			return (&Walker->current->object.gizmo);
		}
	}
	return NULL;
}

void RemoveWorldGizmo(int index)
{
	POTYPE Finder;
	PPOTYPE Found;

	Finder.gizmo.index = index;
	Found = findObject(globals.globalmapstruct.gizmos, &Finder, CompareGizmoIndex);
	if(Found)
	{
		deleteObject(globals.globalmapstruct.gizmos, Found);
	}
	ReorderGizmoListIndex(globals.globalmapstruct.gizmos);
}

void DrawC4Shell(char* StringInBox, bool GreenLight, bool NeedTotalRedraw)
{
	RECT C4TotalRect;
	RECT TextBoxRect;
	RECT RedLightRect;
	RECT GreenLightRect;
	int C4GraphicWidth = getconsize().X / 2;
	int C4GraphicHeight = getconsize().Y / 3;
	int C4XOrigin = (getconsize().X / 2) - (C4GraphicWidth / 2);
	int C4YOrigin = (getconsize().Y / 2) - (C4GraphicHeight / 2);
	C4TotalRect.top = C4YOrigin;
	C4TotalRect.left = C4XOrigin;
	C4TotalRect.right = C4TotalRect.left + C4GraphicWidth;
	C4TotalRect.bottom = C4TotalRect.top + C4GraphicHeight;

	int TextBoxXOrigin = C4XOrigin + 1;
	int TextBoxYOrigin = C4TotalRect.bottom - 4;
	int TextBoxWidth = C4GraphicWidth / 4;
	int TextBoxHeight = 2;
	TextBoxRect.top = TextBoxYOrigin;
	TextBoxRect.left = TextBoxXOrigin;
	TextBoxRect.bottom = TextBoxRect.top + TextBoxHeight;
	TextBoxRect.right = TextBoxRect.left + TextBoxWidth;

	int TextXOrigin = TextBoxXOrigin;
	int TextYOrigin = TextBoxHeight / 2 + TextBoxYOrigin;
	if(StringInBox && strlen(StringInBox) > 0){
		TextXOrigin = TextBoxXOrigin + (TextBoxWidth / 2) - (strlen(StringInBox) / 2);
	}

	int TopWireXOrigin = TextBoxXOrigin + TextBoxWidth;
	int TopWireYOrigin = TextBoxYOrigin;
	int BottomWireXOrigin = TextBoxXOrigin + TextBoxWidth;
	int BottomWireYOrigin = TopWireYOrigin + 1;
	int BottomWireWidth = (C4GraphicWidth + C4XOrigin) - BottomWireXOrigin;
	int RedGreenLightWidth = 2;
	int RedGreenLightHeight = 0;
	int RedLightXOrigin = TextBoxXOrigin;
	int RedLightYOrigin = TextBoxYOrigin - 2;
	RedLightRect.top = RedLightYOrigin;
	RedLightRect.left = RedLightXOrigin;
	RedLightRect.bottom = RedLightRect.top + RedGreenLightHeight;
	RedLightRect.right = RedLightRect.left + RedGreenLightWidth;
	int GreenLightXOrigin = RedLightXOrigin + RedGreenLightWidth + 1;
	int GreenLightYOrigin = RedLightYOrigin;
	GreenLightRect.top = GreenLightYOrigin;
	GreenLightRect.left = GreenLightXOrigin;
	GreenLightRect.bottom = GreenLightRect.top + RedGreenLightHeight;
	GreenLightRect.right = GreenLightRect.left + RedGreenLightWidth;

	char* wirestring = (char*)malloc(sizeof(char) * BottomWireWidth + 1);
	char linechar = (char)196;
	for(int i = 0; i < BottomWireWidth; i++){
		wirestring[i] = linechar;
	}
	wirestring[i] = '\0';

	//NOW DRAW.
	if(NeedTotalRedraw){
	ClearRect(C4TotalRect, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	//DRAW WIRES.
	setcolor(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED);
	setcursor(TopWireXOrigin, TopWireYOrigin);
	writestring(wirestring);
	setcolor(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_GREEN);
	setcursor(BottomWireXOrigin, BottomWireYOrigin);
	writestring(wirestring);
	}

	//THE REST NEEDS TO BE DONE EACH TIME.
	ClearRect(TextBoxRect, 0);
	if(GreenLight){
	ClearRect(RedLightRect, 0);
	}
	else{
		ClearRect(RedLightRect, BACKGROUND_RED);
	}
	if(GreenLight){
		ClearRect(GreenLightRect, BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	}
	else{
		ClearRect(GreenLightRect, 0);
	}
	//WRITE THE TEXT STRING.
	LRED;
	setcursor(TextXOrigin, TextYOrigin);
	writestring(StringInBox);
	free(wirestring);
}

void PlantAtomicBomb(PPLAYER player, PMAP map)
{
	if(player->sartumission->missiontype != MISSION_TYPE_CAUTERIZATION){
		return;
	}
	if(!player->currentmissioninfo.specifics.cautery.hasbomb){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		StaticDialogBox("No Atomic Weapon", "You don't have a DSA atomic device to arm at the moment.", "Press ENTER", &globals.dboxparams);
		return;
	}
	if(player->location->xcoord != map->pz.x || player->location->ycoord != map->pz.y){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		StaticDialogBox("Incorrect Location", "This is not the correct location for the bomb. Blast effects of the bomb will be insufficient unless the bomb is placed at the Primary Zone.", "Press ENTER", &globals.dboxparams);
		return;
	}
	SaveMainSnapShot();
	cls(0);
	if(AtomicActivation(player)){
	RestoreMainSnapShot();
	InsertHistoryString(player, "emergency - atomic device activation detected", true);
	}
	else{
		RestoreMainSnapShot();
	}
}

void ActivatePlayerGizmo(PPLAYER player, int index)
{
	bool ExclusiveGizmo = true;
	bool RemovesWeapon = true;
	char BigBuf[500];
	char TopBuf[500];
	char * Sound;
	int i;
	int TotalActivatingGizmos = 6;
	int TotalActivatingIDs[6] = {GIZMO_TYPE_XRIFLE, GIZMO_TYPE_GASMASK, GIZMO_TYPE_SPIKEMIKE, GIZMO_TYPE_NVG, GIZMO_TYPE_TAS,GIZMO_TYPE_BINOCULARS};
	POTYPE Finder;
	PPOTYPE Found;
	PGIZMO ConflictingGizmo;
	Finder.gizmo.index = index;
	Found = findObject(player->p_gizmos, &Finder, CompareGizmoIndex);
	if(!Found){
		return;
	}
	if(Found->gizmo.isgrenade){
		ActivatePlayerGrenade(player, index);
		return;
	}
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);

	switch(Found->gizmo.unit.device.devicetype)
	{
	case GIZMO_TYPE_XRIFLE:
		ExclusiveGizmo = false;
		RemovesWeapon = false;
		Sound = XRIFLE_ON_SOUND;
		break;
	case GIZMO_TYPE_GASMASK: //CANNOT SNIPE WITH THIS THING.
		RemovesWeapon = false;
		ExclusiveGizmo = true;
		Sound = GASMASK_ON_SOUND;
		player->CurrentActionMode = ACTION_MODE_MOVE;
		break;
	case GIZMO_TYPE_SPIKEMIKE:
		RemovesWeapon = true;
		ExclusiveGizmo = false;
		Sound = SPIKEMIKE_ON_SOUND;
		break;
	case GIZMO_TYPE_NVG:
		RemovesWeapon = false;
		Sound = NVG_ON_SOUND;
		break;
	case GIZMO_TYPE_TAS:
		RemovesWeapon = false;
		Sound = THERM_ON_SOUND;
		break;
	case GIZMO_TYPE_BINOCULARS:
		Sound = BINOCULARS_ON_SOUND;
		break;
	}

	if(RemovesWeapon)
	{
		if(player->currentweapon){
			player->currentweapon = NULL;
			playMIXsound(PICKUP_WEAPON_SOUND);
			strcpy(BigBuf, "You holster your weapon, as your current technology will occupy your shooting hand.");
			strcpy(TopBuf, "Weapon Holstered");
			StaticDialogBox(TopBuf, BigBuf, "Press ENTER", &globals.dboxparams);
		}
		ConflictingGizmo = FindActiveGizmoOfType(GIZMO_TYPE_XRIFLE, player->p_gizmos);
		if(ConflictingGizmo){
			ConflictingGizmo->unit.device.active = false;
			playMIXsound(XRIFLE_OFF_SOUND);
			strcpy(BigBuf, "You disconnect your X-Rifle interface, since your current technology will interfere with your sniping ability.");
			strcpy(TopBuf, "X-Rifle Disconnected");
			StaticDialogBox(TopBuf, BigBuf, "Press ENTER", &globals.dboxparams);
		}
	}

	bool Excluded = true;

	if(ExclusiveGizmo)
	{
		do
		{
			Excluded = true;
			for(i = 0; i < TotalActivatingGizmos; i++){
			ConflictingGizmo = FindActiveGizmoOfType(TotalActivatingIDs[i], player->p_gizmos);
			if(ConflictingGizmo){
				Excluded = false;
				ConflictingGizmo->unit.device.active = false;
			}
			}
		}while(!Excluded);
	}

	playMIXsound(Sound);
	Found->gizmo.unit.device.active = true;
}

double GetActiveGrenadeWeight(PPLAYER player)
{
	PLIST Walker;
	POTYPE Finder;
	PPOTYPE Found;
	Walker = player->p_gizmos;
	for(Walker->current = Walker->head; Walker->current != NULL; Walker->current = Walker->current->nextnode)
	{
		if(Walker->current->object.gizmo.isgrenade && Walker->current->object.gizmo.unit.grenade.primertime != -1)
		{
			//HERE'S OUR BOY.
			Finder.armorygizmo.isgrenade = true;
			Finder.armorygizmo.type = Walker->current->object.gizmo.unit.grenade.grenadetype;
			Found = findObject(globals.armorygizmolist, &Finder, CompareArmoryGizmoBothTypes);
			if(Found){
				return Found->armorygizmo.weight;
			}
			return 0.0;
		}
	}
	return 0.0;
}

bool AreThereActiveGrenadesInInventory(PPLAYER player)
{
	PLIST Walker;
	Walker = player->p_gizmos;
	for(Walker->current = Walker->head; Walker->current != NULL; Walker->current = Walker->current->nextnode)
	{
		if(Walker->current->object.gizmo.isgrenade && Walker->current->object.gizmo.unit.grenade.primertime != -1)
		{
			return true;
		}
	}
	return false;
}

PGIZMO GetActivePlayerGrenade(PPLAYER player)
{
	PLIST Walker;
	Walker = player->p_gizmos;
	for(Walker->current = Walker->head; Walker->current != NULL; Walker->current = Walker->current->nextnode)
	{
		if(Walker->current->object.gizmo.isgrenade && Walker->current->object.gizmo.unit.grenade.primertime != -1)
		{
			return &Walker->current->object.gizmo;
		}
	}
	return NULL;
}

void DeletePlayerActiveGrenade(PPLAYER player)
{
	PLIST Walker;
	POTYPE ArmoryFinder;
	PPOTYPE ArmoryFound;
	Walker = player->p_gizmos;
	for(Walker->current = Walker->head; Walker->current != NULL; Walker->current = Walker->current->nextnode)
	{
		if(Walker->current->object.gizmo.isgrenade && Walker->current->object.gizmo.unit.grenade.primertime != -1)
		{
			ArmoryFinder.armorygizmo.type = Walker->current->object.gizmo.unit.grenade.grenadetype;
			ArmoryFinder.armorygizmo.isgrenade = true;
			ArmoryFound = findObject(globals.armorygizmolist, &ArmoryFinder, CompareArmoryGizmoBothTypes);
			if(ArmoryFound){
				player->weight -= ArmoryFound->armorygizmo.weight;
				deleteObject(player->p_gizmos, &Walker->current->object);
				ReorderGizmoListIndex(player->p_gizmos);
				return;
			}
		}
	}
}

void ActivatePlayerGrenade(PPLAYER player, int index)
{

	char BigBuf[500];
	POTYPE Finder;
	PPOTYPE Found;
	PGIZMO ConflictingGizmo;
	Finder.gizmo.index = index;
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
	Found = findObject(player->p_gizmos, &Finder, CompareGizmoIndex);
	if(!Found || !Found->gizmo.isgrenade){
		return;
	}

	if(AreThereActiveGrenadesInInventory(player)){
		StaticDialogBox("CANNOT PRIME", "You already have a grenade primed!", "Press ENTER", &globals.dboxparams);
		return;
	}

		if(player->currentweapon){
			player->currentweapon = NULL;
			playMIXsound(PICKUP_WEAPON_SOUND);
			strcpy(BigBuf, "You holster your weapon, and take out the grenade.");
			StaticDialogBox("Weapon Holstered", BigBuf, "Press ENTER", &globals.dboxparams);
		}
		ConflictingGizmo = FindActiveGizmoOfType(GIZMO_TYPE_SPIKEMIKE, player->p_gizmos);
		if(ConflictingGizmo){
			ConflictingGizmo->unit.device.active = false;
			playMIXsound(PICKUP_WEAPON_SOUND);
			strcpy(BigBuf, "You put away your spikemike launcher.");
			StaticDialogBox("Spikemike Launcher Deactivated", BigBuf, "Press ENTER", &globals.dboxparams);
		}

	playMIXsound(PICKUP_WEAPON_SOUND);
	int WhatTime = NumberDialogBox("Prime Grenade", "You need to set the fuse length for this grenade. You may enter 1-30 to use a time activation mechanism that will arm upon contact, or you may enter 0 to use a contact fuse that ignites upon contact with any object once thrown.", "Enter Fuse Time", 2, &globals.dboxparams);
	playMIXsound(GRENADE_PRIME_SOUND);
	if(WhatTime > 30){
		WhatTime = 30;
	}
	if(WhatTime){
		sprintf(BigBuf, "The fuse has been set to ignite the grenade %d seconds after contact.", WhatTime);
	}
	else{
		strcpy(BigBuf, "This grenade has been set to ignite IMMEDIATELY upon contact.");
	}
	StaticDialogBox("Grenade Primed", BigBuf, "Press ENTER", &globals.dboxparams);
	Found->gizmo.unit.grenade.primertime = WhatTime;
}

void DeactivatePlayerGizmo(PPLAYER player, int index)
{
	POTYPE Finder;
	PPOTYPE Found;
	char* Sound;
	
	Finder.gizmo.index = index;
	Found = findObject(player->p_gizmos, &Finder, CompareGizmoIndex);
	if(!Found){
		return;
	}
	if(Found->gizmo.isgrenade){
		DeactivatePlayerGrenade(player, index);
		return;
	}
	SetGlobalDBoxParam(DB_SCHEME_MONOCHROMATIC, DB_MEDIUM, 1, FALSE);

	switch(Found->gizmo.unit.device.devicetype)
	{
	case GIZMO_TYPE_XRIFLE:
		Sound = XRIFLE_OFF_SOUND;
		break;
	case GIZMO_TYPE_GASMASK:
		Sound = GASMASK_OFF_SOUND;
		break;
	case GIZMO_TYPE_SPIKEMIKE:
		Sound = SPIKEMIKE_OFF_SOUND;
		break;
	case GIZMO_TYPE_NVG:
		Sound = NVG_OFF_SOUND;
		break;
	case GIZMO_TYPE_TAS:
		Sound = THERM_OFF_SOUND;
		break;
	case GIZMO_TYPE_BINOCULARS:
		Sound = BINOCULARS_OFF_SOUND;
		break;
	}

	playMIXsound(Sound);
	Found->gizmo.unit.device.active = false;
}

void DeactivatePlayerGrenade(PPLAYER player, int index)
{
	POTYPE Finder;
	PPOTYPE Found;
	Finder.gizmo.index = index;
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
	Found = findObject(player->p_gizmos, &Finder, CompareGizmoIndex);
	if(!Found || !Found->gizmo.isgrenade){
		return;
	}
	playMIXsound(PICKUP_WEAPON_SOUND);
	StaticDialogBox("Grenade Safed", "You replace the pin and stuff the grenade back in your vest.", "Press ENTER", &globals.dboxparams);
	Found->gizmo.unit.grenade.primertime = -1;
}

void PlayAppropriateZoomSound(PPLAYER player)
{
	//FIRST, CHECK FOR ELECTRONICS.
	bool play = false;
	char * sound;
	PGIZMO Giz;
	bool SnipeWeapon = false;
	if(player->currentweapon && (player->currentweapon->attacktype & WEAP_ATT_TYPE_SNIPE)){
		SnipeWeapon = true;
		sound = SCOPE_ZOOM_SOUND;
		play = true;
	}
	if((Giz = FindActiveGizmoOfType(GIZMO_TYPE_NVG, player->p_gizmos))){
		if((Giz = FindActiveGizmoOfType(GIZMO_TYPE_XRIFLE, player->p_gizmos)) && SnipeWeapon){
			sound = XRIFLE_ZOOM_SOUND;
			play = true;
		}
		else{
		sound = ELECTRONIC_ZOOM_SOUND;
		play = true;
		}
	}
	if((Giz = FindActiveGizmoOfType(GIZMO_TYPE_TAS, player->p_gizmos))){
		if((Giz = FindActiveGizmoOfType(GIZMO_TYPE_XRIFLE, player->p_gizmos)) && SnipeWeapon){
			sound = XRIFLE_ZOOM_SOUND;
			play = true;
		}
		else{
		sound = ELECTRONIC_ZOOM_SOUND;
		play = true;
		}
	}
	if((Giz = FindActiveGizmoOfType(GIZMO_TYPE_BINOCULARS, player->p_gizmos))){
		sound = ELECTRONIC_ZOOM_SOUND;
		play = true;
	}
	
	if(play)
	{
		playMIXsound(sound);
	}
}

void DropPlayerGizmo(PPLAYER player, int index)
{
	POTYPE Finder;
	PPOTYPE Found;
	POTYPE ArmoryFinder;
	PPOTYPE ArmoryFound;
	float weighttoremove;
	Finder.gizmo.index = index;
	Found = findObject(player->p_gizmos, &Finder, CompareGizmoIndex);
	if(!Found){
		return;
	}
	ArmoryFinder.armorygizmo.id = Found->gizmo.unit.device.devicetype;
	ArmoryFound = findObject(globals.armorygizmolist, &ArmoryFinder, CompareArmoryGizmo);
	if(!ArmoryFound){
		weighttoremove = 0.0;
	}
	else{
		weighttoremove = ArmoryFound->armorygizmo.weight;
	}
	POTYPE NewGizmo = *Found;
	NewGizmo.gizmo.WorldPoint.x = player->location->xcoord;
	NewGizmo.gizmo.WorldPoint.y = player->location->ycoord;
	NewGizmo.gizmo.unit.device.active = false;
	NewGizmo.gizmo.index = globals.globalmapstruct.gizmos->objcnt;
	deleteObject(player->p_gizmos, Found);
	addtoList(globals.globalmapstruct.gizmos, NewGizmo, CompareGizmoIndex);
	playMIXsound(GIZMO_DROP_SOUND);
	ReorderGizmoListIndex(player->p_gizmos);
	player->weight -= weighttoremove;
	if(player->weight < 0.0){
		player->weight = 0.0;
	}
}

bool SpawnLiveGrenade(POINT loc, int GrenadeType, int FuseLength)
{
	POTYPE NewG;
	PPOTYPE RefoundG;
	NewG.gizmo.index = globals.globalmapstruct.gizmos->objcnt;
	NewG.gizmo.isgrenade = true;
	NewG.gizmo.WorldPoint.x = loc.x;
	NewG.gizmo.WorldPoint.y = loc.y;
	NewG.gizmo.unit.grenade.grenadetype = GrenadeType;
	NewG.gizmo.unit.grenade.primertime = FuseLength;

	addtoList(globals.globalmapstruct.gizmos, NewG, CompareGizmoIndex);
	RefoundG = findObject(globals.globalmapstruct.gizmos, &NewG, CompareGizmoIndex);
	if(!RefoundG){
		return false;
	}
	
	SCRIPTEDEVENT Se;
	Se.Type = SCRIPTEDEVENT_TYPE_GRENADEDETONATE;
	Se.Context = (void*)RefoundG;
	Se.TriggerTime = globals.globalmapstruct.totaltime;
	Se.TriggerTime.seconds += NewG.gizmo.unit.grenade.primertime + 1;
	calibratemissionchrono(&Se.TriggerTime);
	AddScriptedEvent(&Se);
	playMIXsound(GRENADE_BOUNCE_SOUND);
	return true;
	
}

void DetonateGrenade(PGIZMO Gren, PMAP map, PPLAYER player)
{
	ENVIRONSTRUCT EvOne;
	ENVIRONSTRUCT EvTwo;
	POINT ExplosionPoint;
	ExplosionPoint = Gren->WorldPoint;
	switch(Gren->unit.grenade.grenadetype)
	{
	case GRENADE_FRAG:
		EvOne.totalturnsleft = 1;
		EvOne.turnsperexpansion = 1;
		EvOne.currentradius = 2;
		EvOne.radiusperexpansion = 0;
		EvOne.origin = ExplosionPoint;
		EvOne.type = ENVIRON_TYPE_FIRE;
		EvTwo = EvOne;
		EvTwo.currentradius = 1;
		EvTwo.totalturnsleft = 3;
		EvTwo.turnsperexpansion = 1;
		EvTwo.radiusperexpansion = 1;
		EvTwo.type = ENVIRON_TYPE_SMOKE;
		playMIXsound(GRENADE_EXPLODE_FRAG);
		DestroyObjectsInRadius(map, player, EvOne.origin, EvOne.currentradius, 50, 5);
		InsertEnvironmentalEffect(&EvOne);
		InsertEnvironmentalEffect(&EvTwo);
		break;
	case GRENADE_SMOKE:
		EvOne.totalturnsleft = 12;
		EvOne.turnsperexpansion = 2;
		EvOne.currentradius = 2;
		EvOne.radiusperexpansion = 1;
		EvOne.origin = ExplosionPoint;
		EvOne.type = ENVIRON_TYPE_SMOKE;
		playMIXsound(GRENADE_EXPLODE_SMOKE);
		InsertEnvironmentalEffect(&EvOne);
		break;
	case GRENADE_TOXIN:
		EvOne.totalturnsleft = 6;
		EvOne.turnsperexpansion = 3;
		EvOne.currentradius = 2;
		EvOne.radiusperexpansion = 1;
		EvOne.origin = ExplosionPoint;
		EvOne.type = ENVIRON_TYPE_TOXIN;
		playMIXsound(GRENADE_EXPLODE_TOXIN);
		InsertEnvironmentalEffect(&EvOne);
		break;
	case GRENADE_FLASH:
		//FLASH CALCULATIONS HERE.
		playMIXsound(GRENADE_EXPLODE_FLASH);
		break;
	default:
		return;
	}
	POTYPE DelThing;
	PPOTYPE DelFound;
	DelThing.gizmo.index = Gren->index;
	DelFound = findObject(globals.globalmapstruct.gizmos, &DelThing, CompareGizmoIndex);
	deleteObject(globals.globalmapstruct.gizmos, DelFound);
	ReorderGizmoListIndex(globals.globalmapstruct.gizmos);
	Sleep(rand()%300 + 100);
}