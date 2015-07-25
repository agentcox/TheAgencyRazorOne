#include "agencyinclude.h"


int FillSquelchChannel(int Max, bool Lowest)
{
	int SNR;
	if(Lowest){
		SNR = Max - random(60, 90);
	}
	else if(RandomPercentage(10)){
		SNR = Max - random(3, 5);
	}
	
	else if(RandomPercentage(10)){
		SNR = Max - random(50, 60);
	}
	else if(RandomPercentage(10)){
		SNR = Max - random(40, 50);
	}
	else if(RandomPercentage(15)){
		SNR = Max - random(30, 40);
	}
	else if(RandomPercentage(20)){
		SNR = Max - random(10, 20);
	}
	else{
		SNR = Max - random(20, 30);
	}
	SNR = CapInclusive(SNR, 0, Max);
	return SNR;
}

int GetMaxSNR(PPLAYER player, PMAP map)
{
	int Max = random(90, 100);
	
	//NOW, THE SPECIAL CASES.
	
	//IF INDOORS, -5 to 10
	if(player->location->type == OBJECT_SYM_FLOOR){
		Max -= random(5,10);
	}
	
	//IF IN SMOKE OR TOXIN, -5 to 10
	return Max;
}

void ReinitSquelchChannels(PPLAYER player, PMAP map, int** SquelchChannels, int CurrentChannel)
{
	int Max = GetMaxSNR(player, map);
	for(int i = 0; i < RADIO_MAX_SQUELCH_CHANNELS; i++){
		((*SquelchChannels)[i]) = FillSquelchChannel(Max, false);
	}
	((*SquelchChannels)[CurrentChannel]) = FillSquelchChannel(Max, true);
}

bool RadioInterface(PPLAYER player, PMAP map, PLIST TransmissionList)
{
	int i;
	PNODE CurrentXmitListPos = TransmissionList->head;
	char* LosingTransmission = "TRANSMISSION FAILURE IN - ";
	char* FrequencyHop = "FREQUENCY HOP IN - ";
	char* CurrentXmit = NULL;
	int RadioXOrigin = 1;
	int RadioYOrigin = 1;
	int MaxChars;
	int CurrentChar;
	int CurrentXChar;
	char CursorChar = (char)219;
	RECT ScrollRect;
	ScrollRect.top = RadioYOrigin;
	ScrollRect.left = RadioXOrigin;
	ScrollRect.bottom = ScrollRect.top + RADIO_WORDS_HEIGHT;
	ScrollRect.right = ScrollRect.left + RADIO_WORDS_WIDTH;
	TIMER MTimer;
	int MaxNumberOfTransmissions = TransmissionList->objcnt;
	int CurrentTransmission = 0;
	int TransmissionPauseTimer = 0;
	int MaxTransmissionPauseTime = RADIO_TRANSMISSION_PAUSE_TIME;
	int CurrentSquelchChannel = 0;
	int* SquelchChannels = (int*)malloc(sizeof(int) * RADIO_MAX_SQUELCH_CHANNELS);
	ReinitSquelchChannels(player, map, &SquelchChannels, CurrentSquelchChannel);
	int SignalLossTimer = 0;
	int MaxSignalLossTime = RADIO_TRANSMISSION_SIGNAL_LOSS_TIME;
	int MaxFrequencyHopTime = random(RADIO_MIN_FREQHOP_TIME, RADIO_MAX_FREQHOP_TIME);
	int CurrentFrequencyHopTime = 0;
	int numevents;
	bool NeedNewSNR = true;
	char Buf[500];
	RECT SquelchRect;
	SquelchRect.left = RADIO_SQUELCH_XORIGIN;
	SquelchRect.right = RADIO_SQUELCH_XORIGIN + RADIO_SQUELCH_WIDTH;
	SquelchRect.top = RADIO_STATUSLINE_YORIGIN;
	SquelchRect.bottom = SquelchRect.top;
	RECT SNRRect;
	SNRRect.left = RADIO_SNR_XORIGIN;
	SNRRect.right = RADIO_SNR_XORIGIN + RADIO_SNR_WIDTH;
	SNRRect.top = RADIO_STATUSLINE_YORIGIN;
	SNRRect.bottom = SNRRect.top;
	RECT StatusRect;
	StatusRect.left = RADIO_STATUS_XORIGIN;
	StatusRect.right = RADIO_STATUS_XORIGIN + RADIO_STATUS_WIDTH;
	StatusRect.top = RADIO_STATUSLINE_YORIGIN;
	StatusRect.bottom = StatusRect.top;
	USHORT key;
	
	WHITE;
	printgraphic(globals.graphicslist, globals.zeroed, RADIO_GRAPHIC_ID);
	for(CurrentTransmission = 0; CurrentTransmission < MaxNumberOfTransmissions; CurrentTransmission++){
		CurrentChar = 0;
		CurrentXChar = 0;
		
		
		MaxChars = strlen(CurrentXmitListPos->object.string);
		CurrentXmit = (char*)malloc(sizeof(char)* (MaxChars + 1));
		strcpy(CurrentXmit, CurrentXmitListPos->object.string);
		WrapStringToWidth(CurrentXmit, RADIO_WORDS_WIDTH);
		while(CurrentChar < MaxChars){
			//THE MAIN LOOP THING.
			//////////////TIME PASSES SECTION//////////////////
			StartTimer(&MTimer);
			Sleep(RADIO_SLEEP);
			EndTimer(&MTimer);
			//////////////PRINTING SECTION//////////////////////
			//FIRST, CHECK FOR IF WE NEED TO SCROLL UP A LINE.
			if(CurrentXmit[CurrentChar] == '\n'){
				setcursor(CurrentXChar + RadioXOrigin, RadioYOrigin + RADIO_WORDS_HEIGHT - 1);
				writechar(' ');
				ScrollRadioWords(ScrollRect);
				CurrentXChar = 0;
				setcursor(CurrentXChar + RadioXOrigin, RadioYOrigin + RADIO_WORDS_HEIGHT - 1);
				GREEN;
				writechar(CursorChar);
				
			}
			//YUK, A BAD CHAR. PRINT *.
			else if(SquelchChannels[CurrentSquelchChannel] < 90 && RandomPercentage((100 - SquelchChannels[CurrentSquelchChannel]) / 2))
			{
				//BAD PRINT SOUND.
				LGREEN;
				setcursor(CurrentXChar + RadioXOrigin, RadioYOrigin + RADIO_WORDS_HEIGHT - 1);
				writechar('*');
				GREEN;
				setcursor(CurrentXChar + RadioXOrigin + 1, RadioYOrigin + RADIO_WORDS_HEIGHT - 1);
				writechar(CursorChar);
				CurrentXChar++;
				
			}
			else{
				LGREEN;
				setcursor(CurrentXChar + RadioXOrigin, RadioYOrigin + RADIO_WORDS_HEIGHT - 1);
				writechar(CurrentXmit[CurrentChar]);
				GREEN;
				setcursor(CurrentXChar + RadioXOrigin + 1, RadioYOrigin + RADIO_WORDS_HEIGHT - 1);
				writechar(CursorChar);
				CurrentXChar++;
			}
			
			if(NeedNewSNR){
				CurrentSquelchChannel = CapInclusive(CurrentSquelchChannel, 0 , RADIO_MAX_SQUELCH_CHANNELS - 1);
				//CLEAR THE SQUELCH RECT AND PRINT NEW SQUELCH.
				ClearRect(SquelchRect, 0);
				LGREEN;
				setcursor(SquelchRect.left, SquelchRect.top);
				printf("%3.3d", CurrentSquelchChannel);
				ClearRect(SNRRect, 0);
				setcursor(SNRRect.left, SNRRect.top);
				PrintSNRString(SquelchChannels[CurrentSquelchChannel]);
				NeedNewSNR = false;
			}
			
			//PRINT STATUS.
			ClearRect(StatusRect, 0);
			setcursor(StatusRect.left, StatusRect.top);
			if(SquelchChannels[CurrentSquelchChannel] < RADIO_TRANSMISSIONLOST_SNR){
				LRED;
				sprintf(Buf, "%s%d", LosingTransmission, MaxSignalLossTime - SignalLossTimer);
			}
			else{
				LGREEN;
				sprintf(Buf, "%s%d", FrequencyHop, MaxFrequencyHopTime-CurrentFrequencyHopTime);
			}
			writestring(Buf);
			
			
			///////////////STATUS SECTION/////////////////////
			
			//////////////INPUT SECTION//////////////////////
			numevents = checkforinput();
			if (numevents > 1)
			{
				for (i = 1; i <= numevents; i++)
				{
					key = getinput(i);
					
					if (key == VK_LEFT){
						CurrentSquelchChannel --;
						NeedNewSNR = true;
					}
					else if (key == VK_RIGHT){
						CurrentSquelchChannel++;
						NeedNewSNR = true;
					}
					else if (key == VK_UP){
						CurrentSquelchChannel++;
						NeedNewSNR = true;
					}
					else if (key == VK_DOWN){
						CurrentSquelchChannel --;
						NeedNewSNR = true;
					}
					else if (key == VK_ESCAPE){
						if(CurrentXmit){
							free(CurrentXmit);
							CurrentXmit = NULL;
						}
						FLUSH;
						clearinputrecords();
						cls(0);
						FadeInTitle("TRANSMISSION", "LOST");
						free(SquelchChannels);
						return false;
					}
					FLUSH;
				}
				clearinputrecords();
				
			}
			
			
			///////////TIMER OVERRUN SECTION/////////////
			if(SquelchChannels[CurrentSquelchChannel] < RADIO_TRANSMISSIONLOST_SNR){
				SignalLossTimer += SplitElapsed(&MTimer);
			}
			else{
				SignalLossTimer = 0;
			}
			
			//IF THE SIGNAL IS LOST........
			if(SignalLossTimer > MaxSignalLossTime){
				cls(0);
				free(SquelchChannels);
				FadeInTitle("TRANSMISSION", "LOST");
				return false;
			}
			
			CurrentFrequencyHopTime += SplitElapsed(&MTimer);
			
			if(CurrentFrequencyHopTime > MaxFrequencyHopTime){
				ReinitSquelchChannels(player, map, &SquelchChannels, CurrentSquelchChannel);
				MaxFrequencyHopTime = random(RADIO_MIN_FREQHOP_TIME, RADIO_MAX_FREQHOP_TIME);
				CurrentFrequencyHopTime = 0;
				NeedNewSNR = true;
			}
			
			
			CurrentChar++;	
		}
		if(CurrentXmit){
			free(CurrentXmit);
			CurrentXmit = NULL;
		}
		if(CurrentXmitListPos->nextnode == NULL){
			break;
		}
		CurrentXmitListPos = CurrentXmitListPos->nextnode;
		
		//PLAY NEXT XMIT SOUND.
		setcursor(CurrentXChar + RadioXOrigin, RadioYOrigin + RADIO_WORDS_HEIGHT - 1);
		writechar(' ');
		ScrollRadioWords(ScrollRect);
		CurrentXChar = 0;
		GREEN;
		setcursor(CurrentXChar + RadioXOrigin, RadioYOrigin + RADIO_WORDS_HEIGHT - 1);
		writechar(CursorChar);
		Sleep(20);
		setcursor(CurrentXChar + RadioXOrigin, RadioYOrigin + RADIO_WORDS_HEIGHT - 1);
		writechar(' ');
		ScrollRadioWords(ScrollRect);
		setcursor(CurrentXChar + RadioXOrigin, RadioYOrigin + RADIO_WORDS_HEIGHT - 1);
		writechar(CursorChar);
	}
	
	ClearRect(StatusRect, 0);
	setcursor(StatusRect.left, StatusRect.top);
	LBLUE;
	writestring("TRANSMISSION CONCLUDED");
	while(1){
		key = waitforkey();
		if(key == VK_RETURN || key == ' ' || key == VK_ESCAPE){
			cls(0);
			free(SquelchChannels);
			return true;
		}
	}
	
}

void ScrollRadioWords(RECT RadioScrollRect)
{
	COORD HigherUp;
	SMALL_RECT Crap;
	Crap.Bottom = (short)RadioScrollRect.bottom;
	Crap.Top = (short)RadioScrollRect.top;
	Crap.Left = (short)RadioScrollRect.left;
	Crap.Right = (short)RadioScrollRect.right;
	HigherUp.X = (short)RadioScrollRect.left;
	HigherUp.Y = (short)RadioScrollRect.top - 1;
	CHAR_INFO Inf;
	Inf.Attributes = 0;
	Inf.Char.AsciiChar = (char)219;
	ScrollConsoleScreenBuffer(getconsoleoutputh(), &Crap, &Crap, HigherUp, &Inf);
}

void PrintSNRString(int SNR)
{
	if(SNR > 80){
		LGREEN;
	}
	else if(SNR > 60){
		LYELLOW;
	}
	else if(SNR > 40){
		LRED;
	}
	else{
		RED;
	}
	printf("%3.3d%%", SNR);
}

void MainRadio(PPLAYER player, PMAP map)
{
	if(!player->currentmissioninfo.radioavailable){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_SMALL, 1, FALSE);
		StaticDialogBox("Radio Unavailable", "You don't have a radio!", "Press ENTER", &globals.dboxparams);
		return;
	}
	char * RadioChoices[6] = {"Switch Channels", "E-Team Communication", "Amplified Tracking", "NRO Satellite Scan", "Radio Detonation", "End Transmission"};
	PLIST MyList;
	CreateDBStringList(&MyList, RadioChoices, 6);
	char buf[500];
	int Selection;
	SetGlobalDBoxParam(DB_SCHEME_MONOCHROMATIC, DB_MEDIUM, 1, TRUE);
	playMIXsound(RADIO_ON_SOUND);
	StaticHoldDialogBox("JSR Radio Link", "Finding Secure Frequency...", "Please Hold", &globals.dboxparams, 200);
	while(1){
		SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_SMALL, 1, FALSE);
	sprintf(buf, "Using %c-Band", ReturnRadioLetter(player->currentmissioninfo.RadioChannel));
	Selection = SimpleListDialogBox("JSR Radio", buf, MyList, &globals.dboxparams, CreateStringDBListItem);
	switch(Selection){
	case 0:
		SwitchRadioChannel(player);
		break;
	case 1:
		ETeamCommunicationRequest(player);
		break;
	case 2:
		AmplifiedTrackingSystemRequest(player);
		break;
	case 3:
		NROScanRequest(player);
		break;
	case 4:
		if(RadioBombDetonation(player, map)){
			FreeDBStringList(MyList);
			return;
		}
		break;
	case 5:
		FreeDBStringList(MyList);
		playMIXsound(RADIO_OFF_SOUND);
		return;
	}
	}


}

void SwitchRadioChannel(PPLAYER player)
{
	char* Channels[RADIO_MAX_CHANNELS] = {"Recieve A-Band", "Recieve B-Band", "Receive C-Band", "Recieve D-Band", "Recieve E-Band"};
	char buf[500];
	PLIST MyList;
	int Selection;

	CreateDBStringList(&MyList, Channels, RADIO_MAX_CHANNELS);
	sprintf(buf, "Using %c-Band", ReturnRadioLetter(player->currentmissioninfo.RadioChannel));
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
	Selection = SimpleListDialogBox("Choose Channel", buf, MyList, &globals.dboxparams, CreateStringDBListItem);
	player->currentmissioninfo.RadioChannel = Selection;
	FreeDBStringList(MyList);
	playMIXsound(RADIO_SWITCHCHANNEL_SOUND);
	return;
}

void AmplifiedTrackingSystemRequest(PPLAYER player)
{
	if(!IsLeftBehindTimeLeft(player) && !IsRecoveryTimeLeft(player)){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		playMIXsound(RADIO_STATIC_SOUND);
		StaticDialogBox("Radio Link Unavailable", "You try to get through to DSA, but it would seem that your transmission feed has been remotely cut. You're on your own.", "Press ENTER", &globals.dboxparams);
		return;
	}
	playMIXsound(RADIO_OPENCHANNEL_SOUND);
	if(DoesScriptedSequenceListHaveType(SCRIPTEDEVENT_TYPE_AMPLIFIEDTRACKING)){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		StaticDialogBox("Amplified Tracking In Progress", "There is an Amplified Tracking Scan in progress. You must hold for the results of this scan before requesting another.", "Press ENTER", &globals.dboxparams);
	}
	else{
		int minutes = rand()%2 + 2;
		char buf[500];
		sprintf(buf, "***DSA QUICKCONFIRM***\nYour request has been recieved. Expect reception of amplified tracking in approximately %d minutes.", minutes);
		SCRIPTEDEVENT Event;
		Event.Type = SCRIPTEDEVENT_TYPE_AMPLIFIEDTRACKING;
		Event.TriggerTime = globals.globalmapstruct.totaltime;
		Event.TriggerTime.minutes += minutes;
		calibratemissionchrono(&Event.TriggerTime);
		AddScriptedEvent(&Event);
		SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_MEDIUM, 1, FALSE);
		StaticDialogBox("Amplified Tracking Online",buf,"Press ENTER", &globals.dboxparams);
	}
}

void IncomingRadioMessage()
{
	SetGlobalDBoxParam(DB_SCHEME_COOLBLUELINES, DB_MEDIUM, 1, TRUE);
	playMIXsound(RADIO_ON_SOUND);
	StaticHoldDialogBox("JSR Radio Link", "Incoming Message...", "Please Hold", &globals.dboxparams, 200);
}

void AmplifiedTrackingSystemResponse(PPLAYER player, PMAP map)
{
	IncomingRadioMessage();
}

void NROScanRequest(PPLAYER player)
{
	if(!IsLeftBehindTimeLeft(player) && !IsRecoveryTimeLeft(player)){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		playMIXsound(RADIO_STATIC_SOUND);
		StaticDialogBox("Radio Link Unavailable", "You try to get through to DSA, but it would seem that your transmission feed has been remotely cut. You're on your own.", "Press ENTER", &globals.dboxparams);
		return;
	}
	playMIXsound(RADIO_OPENCHANNEL_SOUND);
}

void ETeamCommunicationRequest(PPLAYER player)
{
	if(!IsLeftBehindTimeLeft(player) && !IsRecoveryTimeLeft(player)){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		playMIXsound(RADIO_STATIC_SOUND);
		StaticDialogBox("Radio Link Unavailable", "You try to get through to DSA, but it would seem that your transmission feed has been remotely cut. You're on your own.", "Press ENTER", &globals.dboxparams);
		return;
	}
	playMIXsound(RADIO_OPENCHANNEL_SOUND);
}

bool RadioBombDetonation(PPLAYER player, PMAP map)
{
	SCRIPTEDEVENT Ev;
	char* Freqs[4] = {"Abort", "900 GHz", "1200 GHz", "1500 GHz"};
	
	PLIST MyList;
	int Selection;

	CreateDBStringList(&MyList, Freqs, 4);
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
	Selection = SimpleListDialogBox("Remote Detonation", "Choose Frequency", MyList, &globals.dboxparams, CreateStringDBListItem);
	if(Selection){
		Ev.TriggerTime = globals.globalmapstruct.totaltime;
	Ev.TriggerTime.split += 30;
	Ev.ExtraDataOne = Selection;
	calibratemissionchrono(&Ev.TriggerTime);
	Ev.Type = SCRIPTEDEVENT_TYPE_REMOTEBOMBDETONATE;
	AddScriptedEvent(&Ev);
	playSYNCsound(RADIO_DETONATE_SOUND);
	FreeDBStringList(MyList);
	return true;
	}

	FreeDBStringList(MyList);
	return false;
	
}

char ReturnRadioLetter(int channel)
{
	return (char)(channel + 65);
}

