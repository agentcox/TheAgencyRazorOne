#include "agencyinclude.h"

char* Directions[8] =
{
	"North",
		"South",
		"East",
		"West",
		"Northwest",
		"Southwest",
		"Northeast",
		"Southeast"
};

void SartuDailySession(PPLAYER player, int numberofmissionstocreate)
{

	//IF THERE ARE MORE THAN 70 MISSIONS, DO NOT GENERATE ANYTHING.
	if (globals.sartumissions->objcnt > 70){
		return;
	}
	
	//OPERATE ON THESE, MAKE MISSIONS
	CreateSartuMissions(player, &globals.sartudata, numberofmissionstocreate);
	//FREE UP!
	
	//BLAH. DONE.
}

char* ReturnDirection(int direction)
{
	int dir = CapInclusive(direction, 0, 7);
	return Directions[dir];
}

char* ReturnMissionTypeString(int missiontype)
{
	switch(missiontype)
	{
	case MISSION_TYPE_TECHNOLOGYRETRIEVAL:
		return MISSION_STRING_TECHNOLOGYRETRIEVAL;
	case MISSION_TYPE_SURVEILLANCEDEVICEPLACEMENT:
		return MISSION_STRING_SURVEILLANCEDEVICEPLACEMENT;
	case MISSION_TYPE_ISR:
		return MISSION_STRING_ISR;
	case MISSION_TYPE_ASSASSINATION:
		return MISSION_STRING_ASSASSINATION;
	case MISSION_TYPE_NOTOCHORD:
		return MISSION_STRING_NOTOCHORD;
	case MISSION_TYPE_RESCUE:
		return MISSION_STRING_RESCUE;
	case MISSION_TYPE_CAUTERIZATION:
	default:
		return MISSION_STRING_CAUTERIZATION;
	}
}

int GetHighestAvailableMissionType(PPLAYER player)
{
	
	switch(player->rank)
	{
	case AGENT_RANK_FOR:
		return MISSION_TYPE_SURVEILLANCEDEVICEPLACEMENT;
	case AGENT_RANK_FO:
		return MISSION_TYPE_ISR;
	case AGENT_RANK_CO:
		return MISSION_TYPE_ASSASSINATION;
	case AGENT_RANK_COS:
		return MISSION_TYPE_NOTOCHORD;
	case AGENT_RANK_ECOS:
	case AGENT_RANK_ECOSSPECONE:
	default:
		return MISSION_TYPE_CAUTERIZATION;
	}
}

int CreateSartuMissions(PPLAYER player, PSARTUDATASTRUCT sdata, int nummissions)
{
	int missionscreated = 0;
	POTYPE tempmission;
	PSARTUMISSION mission;
	for(int i =0; i < nummissions; i++)
	{
		//LET'S MAKE A MISSION!
		mission = &tempmission.sartumission;
		CreateSartuMission(mission, player, sdata, false);
		addtoList(globals.sartumissions, tempmission, CompareSartuMissionID);
	}
	return missionscreated;
}

int CompareSartuMissionID(PPOTYPE itm1,PPOTYPE itm2) 
{
	if(itm1->sartumission.index == itm2->sartumission.index)
		return (0);
	return (1);
}

void SartuTerminal(PPLAYER player) //THE SARTU UPLINK!
{
	int currentdigit = 0;
//	char digitbuf[5];
	char codekey[5];
	PLIST walker = globals.sartumissions;
	walker->current = walker->head;
	bool accessmode = false;
	int key;
	bool continuing = true;
	char brief[5000];
	bool newbriefing = true;
	
	if(globals.sartumissions->objcnt < 1){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_SMALL, 1, TRUE);
		StaticDialogBox("No Missions", "There are no missions currently available today.", "Available", &globals.dboxparams);
		return;
	}
	
	RECT briefingrect;
	RECT coderect;
	briefingrect.left = SARTU_SCREEN_BRIEFORIGINX;
	briefingrect.top = SARTU_SCREEN_BRIEFORIGINY;
	briefingrect.right = briefingrect.left + SARTU_SCREEN_BRIEFWIDTH;
	briefingrect.bottom = briefingrect.top + SARTU_SCREEN_BRIEFHEIGHT;
	coderect.left = SARTU_SCREEN_CODEX;
	coderect.right = SARTU_SCREEN_CODEX + 4;
	coderect.top = SARTU_SCREEN_CODEY;
	coderect.bottom = coderect.top;
	SartuDrawShell(player);
	SartuPrintSartuActive();
	while(continuing)
	{
		if(newbriefing){
			strcpy(brief, walker->current->object.sartumission.missiondescription);
			WrapStringToWidth(brief, SARTU_SCREEN_BRIEFWIDTH - 1);
			ClearRect(briefingrect, 0);
			ClearRect(coderect, 0);
			GREEN;
			WriteDialogString(brief, SARTU_SCREEN_BRIEFORIGINX, SARTU_SCREEN_BRIEFORIGINY, SARTU_SCREEN_BRIEFWIDTH, false);
			Sleep(20);
			LGREEN;
			WriteDialogString(brief, SARTU_SCREEN_BRIEFORIGINX, SARTU_SCREEN_BRIEFORIGINY, SARTU_SCREEN_BRIEFWIDTH, false);
			if(walker->current->object.sartumission.taken){
				SartuPrintTakenMissionData(&walker->current->object.sartumission);
			}
			newbriefing = false;
		}
		
		key = waitforkey();
		if(!accessmode)
		{
			if(key == 43){ //FORWARD
				if(walker->current->nextnode != NULL){
					walker->current = walker->current->nextnode;
					newbriefing = true;
					playMIXsound(SARTU_KEYBOARD_SOUND);
				}
				else{
					onebeepa();
				}
				
			}
			else if(key == 45){ //BACKWARD
				if(walker->current->prevnode != NULL){
					walker->current = walker->current->prevnode;
					newbriefing = true;
					playMIXsound(SARTU_KEYBOARD_SOUND);
				}
				else{
				onebeepa();
				}
			}
			else if (key == VK_ESCAPE)
			{
				
				LoopOpConMusic(player);
				ClearRect(briefingrect, 0);
			ClearRect(coderect, 0);
			SartuPrintSartuDeactivated();
				return;
			}
			else if (key == 97)
			{
				if(!walker->current->object.sartumission.taken)
				{
					SartuPrintPleaseEnterCode(player);
					accessmode = true;
				}
			}
		}
		else
		{
			//IF WE'RE DOING THE CODE THING...
			if (key == '0' || key =='1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9'){
			//	sprintf(digitbuf, "%c", key);
				codekey[currentdigit] = (key);
				codekey[currentdigit+1] = '\0';
				currentdigit++;
				setcursor(SARTU_SCREEN_CODEX, SARTU_SCREEN_CODEY);
				LBLUE;
				printf("%s", codekey);
				onebeepb();
			}

			if(currentdigit == 4){ //LAST DIGIT WAS JUST PRESSED.
				if(atoi(codekey) == player->accesscode){ //CODE WAS RIGHT...
					//GO!
					LoopSong(globals.musiclist.songs[MOBILIZE_SONG]);
					Sleep(500);
					player->currentlymobilizing = true;
					player->currentmissioninfo.missiontype = walker->current->object.sartumission.missiontype;
					player->history.missionsattempted++;
					player->missionsleftthiscycle--;
					ClearRect(briefingrect, 0);
					SartuPrintMissionAcceptance(player);
					player->sartumission = &walker->current->object.sartumission;
					InitializePlayerForCombat(player); ////////////////INITIALIZE PLAYER FOR COMBAT!!!! (KOMBAT)
					walker->current->object.sartumission.taken = true;
					walker->current->object.sartumission.takenbywhomfirst = -1;
					walker->current->object.sartumission.takenbywhomlast = -1;
					walker->current->object.sartumission.takenbywhomid = -1;
					
					return;
				}
				else{ //CODE WAS WRONG...
					ClearRect(briefingrect, 0);
			ClearRect(coderect, 0);
					SartuPrintCodeWrong(player);
					newbriefing = true;
					accessmode = false;
					currentdigit = 0;
				}
			}
		}
	}
}

void SartuPrintPleaseEnterCode(PPLAYER player)
{
	char wrongone[500];
	char wrongtwo[500];
	POINT wrongonept;
	POINT wrongtwopt;
	int onelen = 0;
	int twolen = 0;

	sprintf(wrongone, "-OPERATIVE #%d-", player->id);
	sprintf(wrongtwo, "-E N T E R   C O D E-");

	wrongonept.y = wrongtwopt.y = retrieveTextCenterV() - 4;
	wrongonept.y--;
	wrongonept.x = retrieveTextCenterH(wrongone);
	wrongtwopt.x = retrieveTextCenterH(wrongtwo);

	onelen = strlen(wrongone);
	twolen = strlen(wrongtwo);

	for (int i = 0; i < onelen; i++){
		RED;
		setcursor(wrongonept.x + i, wrongonept.y);
		printf("%c", wrongone[i]);
		Sleep(30);
		LRED;
		setcursor(wrongonept.x + i, wrongonept.y);
		printf("%c", wrongone[i]);
	}
	Sleep(500);
	for (i = 0; i < twolen; i++){
		RED;
		setcursor(wrongtwopt.x + i, wrongtwopt.y);
		printf("%c", wrongtwo[i]);
		Sleep(30);
		LRED;
		setcursor(wrongtwopt.x + i, wrongtwopt.y);
		printf("%c", wrongtwo[i]);
	}
}

void SartuPrintTakenMissionData(PSARTUMISSION mission)
{
	char wrongone[500];
	char wrongtwo[500];
	POINT wrongonept;
	POINT wrongtwopt;
	int onelen = 0;
	int twolen = 0;

	LRED;
	sprintf(wrongone, "-Mission Previously Accepted-");
	sprintf(wrongtwo, "By Agent %s - ID #%d", ReturnListItemByNumber(mission->takenbywhomlast, globals.sartudata.agentlastnames)->string, mission->takenbywhomid);

	wrongonept.y = wrongtwopt.y = retrieveTextCenterV() - 4;
	wrongonept.y--;
	wrongonept.x = retrieveTextCenterH(wrongone);
	wrongtwopt.x = retrieveTextCenterH(wrongtwo);

	onelen = strlen(wrongone);
	twolen = strlen(wrongtwo);

	setcursor(wrongonept.x, wrongonept.y);
	printf("%s", wrongone);
	setcursor(wrongtwopt.x, wrongtwopt.y);
	printf("%s", wrongtwo);
}
void SartuPrintMissionAcceptance(PPLAYER player)
{
	char wrongone[500];
	char wrongtwo[500];
	POINT wrongonept;
	POINT wrongtwopt;
	int onelen = 0;
	int twolen = 0;

	sprintf(wrongone, "Mission Accepted Agent %s - ID#%d", player->lname, player->id);
	sprintf(wrongtwo, "YOUR STATUS IS NOW RAZOR ONE");

	wrongonept.y = wrongtwopt.y = retrieveTextCenterV() - 4;
	wrongonept.y--;
	wrongonept.x = retrieveTextCenterH(wrongone);
	wrongtwopt.x = retrieveTextCenterH(wrongtwo);

	onelen = strlen(wrongone);
	twolen = strlen(wrongtwo);

	for (int i = 0; i < onelen; i++){
		RED;
		setcursor(wrongonept.x + i, wrongonept.y);
		printf("%c", wrongone[i]);
		Sleep(30);
		LRED;
		setcursor(wrongonept.x + i, wrongonept.y);
		printf("%c", wrongone[i]);
	}
	Sleep(500);
	for (i = 0; i < twolen; i++){
		RED;
		setcursor(wrongtwopt.x + i, wrongtwopt.y);
		printf("%c", wrongtwo[i]);
		Sleep(30);
		LRED;
		setcursor(wrongtwopt.x + i, wrongtwopt.y);
		printf("%c", wrongtwo[i]);
	}
	Sleep(3000);
}

void SartuPrintCodeWrong(PPLAYER player)
{
	char wrongone[500];
	char wrongtwo[500];
	POINT wrongonept;
	POINT wrongtwopt;
	int onelen = 0;
	int twolen = 0;

	sprintf(wrongone, "-Agent %s-", player->lname);
	sprintf(wrongtwo, "-I N C O R R E C T    C O D E-");

	wrongonept.y = wrongtwopt.y = retrieveTextCenterV() - 4;
	wrongonept.y--;
	wrongonept.x = retrieveTextCenterH(wrongone);
	wrongtwopt.x = retrieveTextCenterH(wrongtwo);

	onelen = strlen(wrongone);
	twolen = strlen(wrongtwo);

	for (int i = 0; i < onelen; i++){
		RED;
		setcursor(wrongonept.x + i, wrongonept.y);
		printf("%c", wrongone[i]);
		Sleep(30);
		LRED;
		setcursor(wrongonept.x + i, wrongonept.y);
		printf("%c", wrongone[i]);
	}
	Sleep(500);
	for (i = 0; i < twolen; i++){
		RED;
		setcursor(wrongtwopt.x + i, wrongtwopt.y);
		printf("%c", wrongtwo[i]);
		Sleep(30);
		LRED;
		setcursor(wrongtwopt.x + i, wrongtwopt.y);
		printf("%c", wrongtwo[i]);
	}
	Sleep(3000);
}

void SartuPrintSartuActive()
{
	char wrongone[500];
	char wrongtwo[500];
	POINT wrongonept;
	POINT wrongtwopt;
	int onelen = 0;
	int twolen = 0;

	sprintf(wrongone, "SECURE ACCESS RECIEVE / TRANSMIT UPLINK");
	sprintf(wrongtwo, "DEFENSE SECURITY AGENCY TRUNK - NOW ACTIVE");

	wrongonept.y = wrongtwopt.y = retrieveTextCenterV() - 4;
	wrongonept.y--;
	wrongonept.x = retrieveTextCenterH(wrongone);
	wrongtwopt.x = retrieveTextCenterH(wrongtwo);

	onelen = strlen(wrongone);
	twolen = strlen(wrongtwo);

	for (int i = 0; i < onelen; i++){
		RED;
		setcursor(wrongonept.x + i, wrongonept.y);
		printf("%c", wrongone[i]);
		Sleep(30);
		LRED;
		setcursor(wrongonept.x + i, wrongonept.y);
		printf("%c", wrongone[i]);
	}
	Sleep(500);
	for (i = 0; i < twolen; i++){
		RED;
		setcursor(wrongtwopt.x + i, wrongtwopt.y);
		printf("%c", wrongtwo[i]);
		Sleep(30);
		LRED;
		setcursor(wrongtwopt.x + i, wrongtwopt.y);
		printf("%c", wrongtwo[i]);
	}
	Sleep(2000);
}

void SartuPrintSartuDeactivated()
{
	char wrongone[500];
	char wrongtwo[500];
	POINT wrongonept;
	POINT wrongtwopt;
	int onelen = 0;
	int twolen = 0;

	sprintf(wrongone, "SECURE ACCESS RECIEVE / TRANSMIT UPLINK");
	sprintf(wrongtwo, "CONNECTION CLOSED");

	wrongonept.y = wrongtwopt.y = retrieveTextCenterV() - 4;
	wrongonept.y--;
	wrongonept.x = retrieveTextCenterH(wrongone);
	wrongtwopt.x = retrieveTextCenterH(wrongtwo);

	onelen = strlen(wrongone);
	twolen = strlen(wrongtwo);

	for (int i = 0; i < onelen; i++){
		RED;
		setcursor(wrongonept.x + i, wrongonept.y);
		printf("%c", wrongone[i]);
		Sleep(30);
		LRED;
		setcursor(wrongonept.x + i, wrongonept.y);
		printf("%c", wrongone[i]);
	}
	Sleep(500);
	for (i = 0; i < twolen; i++){
		RED;
		setcursor(wrongtwopt.x + i, wrongtwopt.y);
		printf("%c", wrongtwo[i]);
		Sleep(30);
		LRED;
		setcursor(wrongtwopt.x + i, wrongtwopt.y);
		printf("%c", wrongtwo[i]);
	}
	Sleep(2000);
}

void SartuDrawShell(PPLAYER player)
{
	char buf[500];
	GREEN;
	printgraphic(globals.graphicslist, globals.zeroed, SARTU_GRAPHIC_ID);
	LGREEN;
	sprintf(buf, "%c%s.sartu.trunkV.dsa.gov", player->fname[0], player->lname);
	setcursor(SARTU_SCREEN_LOGINX, SARTU_SCREEN_LOGINY);
	printf("%s", buf);

}

int CreateSartuMission(PSARTUMISSION mission, PPLAYER player, PSARTUDATASTRUCT sdata, bool priority, int prioritymissiontype, int priorityspecificidone, int priorityspecificidtwo)
{
	if(priority){
		mission->index = -1; //PRIORITY ID
		
	}
	else{
		mission->index = globals.sartumissions->objcnt; //UNIQUE ID.
	}
		mission->classificationname = rand()%(MAX_GREEK_CHARACTERS - 1); //RANDOM GREEK CHAR.
		mission->classificationletter = RandomAlpha(); //CLASSIFICATION LETTER ALL SET.
		mission->daysleft = random(1, 6); //SET AN EXPIRATION DATE.
		mission->taken = false;
		GenerateMissionCode(&mission->missioncode); //SET UP THE MISSION CODE.
		mission->codenameone = rand()%sdata->missioncodesone->objcnt; //CODENAME ONE
		mission->codenametwo = rand()%sdata->missioncodestwo->objcnt; //CODENAME TWO
		//SETTING UP MISSION TYPES AND SPECIFIC INFO
		if (priority)
		{
			mission->missiontype = prioritymissiontype;
			mission->missionspecificone = -1;
			mission->missionspecifictwo = -1;
		}
		else{
		mission->missiontype = GetHighestAvailableMissionType(player);
		mission->missiontype = rand()%(mission->missiontype + 1);
		switch(mission->missiontype)
		{
		case MISSION_TYPE_TECHNOLOGYRETRIEVAL:
			mission->missionspecificone = rand()%sdata->technologies->objcnt;
			mission->missionspecifictwo = rand()%90000 + 10000;
			break;
		case MISSION_TYPE_SURVEILLANCEDEVICEPLACEMENT:
			mission->missionspecificone = rand()%sdata->targetlastnames->objcnt;  //LAST NAME OF OUR TARGET.
			mission->missionspecifictwo = rand()%sdata->targetextras->objcnt; //SOMETHING ABOUT WHO WE'RE WATCHING.
			break;
		case MISSION_TYPE_ISR:
		case MISSION_TYPE_ASSASSINATION:
			mission->missionspecificone = rand()%sdata->targetfirstnames->objcnt; //FIRST NAME OF TARGET
			mission->missionspecifictwo = rand()%sdata->targetlastnames->objcnt; //LAST NAME OF TARGET
			break;
		case MISSION_TYPE_RESCUE:
		case MISSION_TYPE_NOTOCHORD:
			mission->missionspecificone = rand()%sdata->agentcodenames->objcnt; //CODE NAME OF AGENT
			mission->missionspecifictwo = rand()%90000 + 10000;
			break;
		case MISSION_TYPE_CAUTERIZATION:
			mission->missionspecificone = rand()%90000 + 10000; //ID OF BOMB
			mission->missionspecifictwo = 0;
			break;
		}
		}


		//NOW, THE HARD NUMBERS.

		//HEIGHT AND WIDTH
		mission->mapheight = random(MIN_MAP_HEIGHT, MAX_MAP_HEIGHT); 
		mission->mapwidth = random(MIN_MAP_WIDTH, MAX_MAP_WIDTH);
		double mapsize = (double)(mission->mapheight * mission->mapwidth);

		//GUARD AMOUNT - DENSITY
		int gdensity = random((int)(MIN_GUARD_DENSITY * GUARD_DENSITY_SCALE_FACTOR), (int)(MAX_GUARD_DENSITY * GUARD_DENSITY_SCALE_FACTOR));
		double dgdensity = (double) gdensity / (double)GUARD_DENSITY_SCALE_FACTOR;
		mission->numberguards = (int)(mapsize * dgdensity) / DENSITY_SCALEDOWN_FACTOR;

		//ARMORY AMOUNT - DENSITY
		int adensity = random((int)(MIN_ARMORY_DENSITY * ARMORY_DENSITY_SCALE_FACTOR), (int)(MAX_ARMORY_DENSITY * ARMORY_DENSITY_SCALE_FACTOR));
		double dadensity = (double) adensity / (double)ARMORY_DENSITY_SCALE_FACTOR;
		mission->numberarmories = (int)(mapsize * dadensity) / DENSITY_SCALEDOWN_FACTOR;

		//VEHICLE AMOUNT - DENSITY
		int vdensity = random((int)(MIN_VEHICLE_DENSITY * VEHICLE_DENSITY_SCALE_FACTOR), (int)(MAX_VEHICLE_DENSITY * VEHICLE_DENSITY_SCALE_FACTOR));
		double dvdensity = (double) vdensity / (double)VEHICLE_DENSITY_SCALE_FACTOR;
		mission->numbervehicles = (int)(mapsize * dvdensity) / DENSITY_SCALEDOWN_FACTOR;

		//BUILDING DENSITY
		mission->mapbuildingdensitypercentage = random(0, 50);
		
		//SECURITY GRID - BASED ON RANK
		int security = 15;
		double randomsec;
		randomsec = (double)player->rank / (double)MAX_RANK;
		randomsec *= 80.0;
		randomsec += 1.0;
		security += random(0, (int)randomsec);
		security = CapInclusive(security, 0, 100);
		mission->compoundsecuritypercentage = security;
		
		//ENEMY SKILL - BASED ON RANK
		security = 10;
		randomsec = (double)player->rank / (double)MAX_RANK;
		randomsec *= 80.0;
		randomsec += 1.0;
		security += random(0, (int)randomsec);
		security = CapInclusive(security, 0, 100);
		mission->enemyskillpercentage = security;

		SartuGenerateBriefing(mission, player, sdata, priority, prioritymissiontype, priorityspecificidone, priorityspecificidtwo);
		//ALL DONE.
		return 0;
}

void SartuGenerateBriefing (PSARTUMISSION mission, PPLAYER player, PSARTUDATASTRUCT sdata, bool priority, int prioritymissiontype, int priorityspecificidone, int priorityspecificidtwo)
{
	//THIS MIGHT BE UGLY. IT'S BASICALLY JUST GENERATING A MISSION BRIEFING FROM PIECES.
	char finalbuf[10000];
	char tempbuf[1000];
	//START WITH THE STANDARD FLASH TRAFFIC LINE.
	if(priority){
		sprintf(tempbuf, "Operative %s:\n------------------------------------\n", player->lname);
		strcpy(finalbuf, tempbuf);
	}
	else{
	sprintf(finalbuf, "DSA SPEC ONE OPS ORDER FLASH TRAFFIC - MISSION EXPIRES IN %d DAYS\n------------------------------------\n", mission->daysleft);
	}
	//CLASSIFICATION CODES
	sprintf(tempbuf, "Operation Classified %s-%c\n", RetrieveGreekString(mission->classificationname), mission->classificationletter);
	strcat(finalbuf, tempbuf);
	//MISSION CODENAME
	sprintf(tempbuf, "%s - %s %s\n", mission->missioncode, ReturnListItemByNumber(mission->codenameone, sdata->missioncodesone)->string,ReturnListItemByNumber(mission->codenametwo, sdata->missioncodestwo)->string );
	strcat(finalbuf, tempbuf);
	if(priority){
		strcat(finalbuf, "Emergency ");
	}
	else{
		strcat(finalbuf, "DSA Requests ");
	}
	sprintf(tempbuf, "%s Operation At Compound Located In %sern United States.\n", ReturnMissionTypeString(mission->missiontype), ReturnDirection(rand()%8));
	strcat(finalbuf, tempbuf);

	//NOW IT STARTS TO SPLIT OFF A BIT.
	//DESCRIBE THE TARGET.
	if(priority)
	{
		switch(mission->missiontype)
		{
				case MISSION_TYPE_TECHNOLOGYRETRIEVAL:
				sprintf(tempbuf, "You are ordered to retrieve DSA Nuclear Device #%d, which failed to detonate. Recovery PRIORITY ONE.", priorityspecificidone);
			break;
		case MISSION_TYPE_ISR:
			sprintf(tempbuf, "TARGET IS %s %s. Bring this individual into DSA custody.\n", ReturnListItemByNumber(priorityspecificidone, sdata->targetfirstnames)->string, ReturnListItemByNumber(priorityspecificidtwo, sdata->targetlastnames)->string);
			break;
		case MISSION_TYPE_ASSASSINATION:
			sprintf(tempbuf, "TARGET IS %s %s. Cessation of target vital signs CRITICAL.\n", ReturnListItemByNumber(priorityspecificidone, sdata->targetfirstnames)->string, ReturnListItemByNumber(priorityspecificidtwo, sdata->targetlastnames)->string);
			break;
		default:
			sprintf(tempbuf, "This mission is unsupported.\n");
			break;
		}
	}
	else{
	switch(mission->missiontype)
	{
			case MISSION_TYPE_TECHNOLOGYRETRIEVAL:
				sprintf(tempbuf, "You will be recovering a(n) %s, ID #%d.\n", ReturnListItemByNumber(mission->missionspecificone, sdata->technologies)->string, mission->missionspecifictwo);
			break;
		case MISSION_TYPE_SURVEILLANCEDEVICEPLACEMENT:
			sprintf(tempbuf, "You will aid in the monitoring of a Mr. %s, %s, as part of a DSA investigation.\n", ReturnListItemByNumber(mission->missionspecificone, sdata->targetlastnames)->string, ReturnListItemByNumber(mission->missionspecifictwo, sdata->targetextras)->string);
			break;
		case MISSION_TYPE_ISR:
			sprintf(tempbuf, "Your target is %s %s, %s. The target must remain ALIVE.\n", ReturnListItemByNumber(mission->missionspecificone, sdata->targetfirstnames)->string, ReturnListItemByNumber(mission->missionspecifictwo, sdata->targetlastnames)->string,ReturnListItemByNumber(rand()%sdata->targetextras->objcnt, sdata->targetextras)->string);
			break;
		case MISSION_TYPE_ASSASSINATION:
			sprintf(tempbuf, "Your target is %s %s, %s. The target is to be ELIMINATED.\n", ReturnListItemByNumber(mission->missionspecificone, sdata->targetfirstnames)->string, ReturnListItemByNumber(mission->missionspecifictwo, sdata->targetlastnames)->string,ReturnListItemByNumber(rand()%sdata->targetextras->objcnt, sdata->targetextras)->string);
			break;
		case MISSION_TYPE_RESCUE:
			sprintf(tempbuf, "Target is DSA Operative #%d, Codename '%s'. You must transfer this Operative back into DSA custody.\n", mission->missionspecifictwo, ReturnListItemByNumber(mission->missionspecificone, sdata->agentcodenames)->string);
			break;
		case MISSION_TYPE_NOTOCHORD:
			sprintf(tempbuf, "Target is DSA Operative #%d, Codename '%s'. Your orders are to TERMINATE this Operative.\n", mission->missionspecifictwo, ReturnListItemByNumber(mission->missionspecificone, sdata->agentcodenames)->string);
			break;
		case MISSION_TYPE_CAUTERIZATION:
		default:
			sprintf(tempbuf, "You will be assigned DSA Thermonuclear Device #%d. Total destruction of the facility is essential.\n", mission->missionspecificone);
			break;
	}
	}
	strcat(finalbuf, tempbuf);
	if (priority){
		sprintf(tempbuf, "%s, %s.", ReturnListItemByNumber(rand()%sdata->egogetemone->objcnt, sdata->egogetemone)->string, ReturnListItemByNumber(rand()%sdata->egogetemtwo->objcnt, sdata->egogetemtwo)->string);
	}
	else{
	sprintf(tempbuf, "%s, %s.\n", ReturnListItemByNumber(rand()%sdata->gogetemone->objcnt, sdata->gogetemone)->string, ReturnListItemByNumber(rand()%sdata->gogetemtwo->objcnt, sdata->gogetemtwo)->string);
	}
	strcat(finalbuf, tempbuf);

	SartuTacticalSituation(mission, tempbuf);
	strcat(finalbuf, tempbuf);
	//COPY OVER FINALBUF
	mission->missiondescription = (char*) malloc(sizeof(char) * (strlen(finalbuf) + 1));
	strcpy(mission->missiondescription, finalbuf);
	zerocursor();

}

void SartuTacticalSituation(PSARTUMISSION mission, char* buf)
{
	char size[100];
	char density[100];
	char security[100];
	char guards[100];
	

	int mapsize = mission->mapheight * mission->mapwidth;
	int maxsize = (MAX_MAP_HEIGHT * MAX_MAP_WIDTH);

	if(mapsize > 4* maxsize/ 5){
		strcpy(size, "Compound is extensive; ");
	}
	if (mapsize > 3* maxsize / 4){
		strcpy(size, "Compound is large; ");
	}
	if (mapsize > maxsize / 4){
		strcpy(size, "Compound is moderately sized; ");
	}
	else{
		strcpy(size, "Compound is small; ");
	}
	//DENSITY
	if(mission->mapbuildingdensitypercentage >45){
		strcpy(density, "numerous buildings exist inside the compound.");
	}
	else if (mission->mapbuildingdensitypercentage > 25){
		strcpy(density, "several buildings are contained within.");
	}
	else if (mission->mapbuildingdensitypercentage > 15){
		strcpy(density, "there exists an equal amount of cover and open space within.");
	}
	else{
		strcpy(density, "for the most part open land.");
	}

	//SECURITY PROTOCOLS
	if (mission->compoundsecuritypercentage > 80){
		strcpy(security, "A prohibitive amount of security exists,");
	}
	else if (mission->compoundsecuritypercentage > 60){
		strcpy(security, "Advanced security systems lay within,");
	}
	else if (mission->compoundsecuritypercentage > 40){
		strcpy(security, "The compound contains average security,");
	}
	else{
		strcpy(security, "The compound has flawed security,");
	}

	//GUARD DENSITY
	double guarddensity = ((double)(mission->numberguards) / (double)(mission->mapheight * mission->mapwidth));
	guarddensity *= DENSITY_SCALEDOWN_FACTOR;
	if(guarddensity >= 4.0 * MAX_GUARD_DENSITY / 5.0){
		strcpy(guards, "with an excessive patrol force guarding the area.");
	}
	else if (guarddensity >= MAX_GUARD_DENSITY  / 2.0){
		strcpy(guards, "with a formidable defensive patrol force.");
	}
	else if (guarddensity >= MAX_GUARD_DENSITY / 4.0){
		strcpy(guards, "with a fair amount of patrolling guards.");
	}
	else{
		strcpy(guards, "with only a diminuitive protective guard force.");
	}

	sprintf(buf, "Tactical: %s%s %s %s", size, density, security, guards);
}

void ShutDownSartuForTheDay(PPLAYER player, PSARTUDATASTRUCT sdata)
{
	PLIST walker = globals.sartumissions;
	bool deleted = false;
	PPOTYPE listitem;
	int numbertotake = 0;
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		walker->current->object.sartumission.daysleft--;
		if(walker->current->object.sartumission.daysleft <= 0 || walker->current->object.sartumission.taken){ //WE'RE OUT OF TIME ON THIS MISSION. GET RID OF IT.
			FreeSartuMission(&walker->current->object.sartumission);
			deleteObject(globals.sartumissions, &walker->current->object);
			deleted = true;
		}
	}
	if (deleted)
	{
		ReorderSartuList();
	}
	//NOW HAVE A FEW PEOPLE TAKE SOME OF THE MISSIONS.
	if(walker->objcnt > 5){
	numbertotake = walker->objcnt / (rand()%5 + 1);
	}
	for(int k = 0; k < numbertotake; k++){
		listitem = ReturnListItemByNumber(rand()%walker->objcnt, globals.sartumissions);
		if(listitem)
		{
			listitem->sartumission.taken = true;
			listitem->sartumission.takenbywhomfirst = rand()%sdata->agentfirstnames->objcnt;
			listitem->sartumission.takenbywhomlast = rand()%sdata->agentlastnames->objcnt;
			listitem->sartumission.takenbywhomid = rand()%90000 + 10000;
		}
	}
}

void ReorderSartuList()
{
PLIST walker = globals.sartumissions;
	int currentid = 0;
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		walker->current->object.sartumission.index = currentid;
		currentid++;
	}
}

void FreeSartuMission(PSARTUMISSION mission)
{
	free(mission->missioncode);
	free(mission->missiondescription);
}

void TerminateSartu()
{
	PLIST walker = globals.sartumissions;
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		FreeSartuMission(&walker->current->object.sartumission);
	}
	freeList(globals.sartumissions);
	free(globals.sartumissions);
}

PPOTYPE RetrieveRandomObject(PLIST list)
{
	int randomizer = list->objcnt - 1;
	int index;
	PNODE node;
	
	index = rand()%randomizer;

	node = list->head;
	for(int i = 0; i < index; i++){
	node = GetNextListEntry(node);
	}
	return (&node->object);
}

int StringCompare(PPOTYPE itm1,PPOTYPE itm2)
{
	return 1;
}

bool LoadSartuInputLists(PSARTUDATASTRUCT sdata)
{
	FILE* fp;

	
	//START WITH MISSION CODES
	if(!(fp = fopen(MISSIONCODENAMESONEFILE, "r"))){
		return false;
	}
	if(!LoadNLDStrings(&sdata->missioncodesone, fp)){
		fclose(fp);
		return false;
	}
	fclose(fp);

	//MISSION CODES TWO
	if(!(fp = fopen(MISSIONCODENAMESTWOFILE, "r"))){
		return false;
	}
	if(!LoadNLDStrings(&sdata->missioncodestwo, fp)){
		fclose(fp);
		return false;
	}
	fclose(fp);

	//DSA FIRST NAMES
	if(!(fp = fopen(DSAFIRSTNAMESFILE, "r"))){
		return false;
	}
	if(!LoadNLDStrings(&sdata->agentfirstnames, fp)){
		fclose(fp);
		return false;
	}
	fclose(fp);

	//DSA LAST NAMES
	if(!(fp = fopen(DSALASTNAMESFILE, "r"))){
		return false;
	}
	if(!LoadNLDStrings(&sdata->agentlastnames, fp)){
		fclose(fp);
		return false;
	}
	fclose(fp);

	//DSA CODE NAMES
	if(!(fp = fopen(DSACODENAMESFILE, "r"))){
		return false;
	}
	if(!LoadNLDStrings(&sdata->agentcodenames, fp)){
		fclose(fp);
		return false;
	}
	fclose(fp);

	//TARGET FIRST NAMES
	if(!(fp = fopen(TARGETFIRSTNAMESFILE, "r"))){
		return false;
	}
	if(!LoadNLDStrings(&sdata->targetfirstnames, fp)){
		fclose(fp);
		return false;
	}
	fclose(fp);

	//TARGET LAST NAMES
	if(!(fp = fopen(TARGETLASTNAMESFILE, "r"))){
		return false;
	}
	if(!LoadNLDStrings(&sdata->targetlastnames, fp)){
		fclose(fp);
		return false;
	}
	fclose(fp);

	//TARGET EXTRA DATA
	if(!(fp = fopen(TARGETEXTRADATAFILE, "r"))){
		return false;
	}
	if(!LoadNLDStrings(&sdata->targetextras, fp)){
		fclose(fp);
		return false;
	}
	fclose(fp);

		//TECHNOLOGIES DATA
	if(!(fp = fopen(TECHNOLOGYNAMESFILE, "r"))){
		return false;
	}
	if(!LoadNLDStrings(&sdata->technologies, fp)){
		fclose(fp);
		return false;
	}
	fclose(fp);

		//GO GET EM DATA
	if(!(fp = fopen(GOGETEMONEFILE, "r"))){
		return false;
	}
	if(!LoadNLDStrings(&sdata->gogetemone, fp)){
		fclose(fp);
		return false;
	}
	fclose(fp);

		//GO GET EM TWO DATA
	if(!(fp = fopen(GOGETEMTWOFILE, "r"))){
		return false;
	}
	if(!LoadNLDStrings(&sdata->gogetemtwo, fp)){
		fclose(fp);
		return false;
	}
	fclose(fp);
	
	//E GO GET EM ONE DATA
	if(!(fp = fopen(EMERGENCYGOGETEMONEFILE, "r"))){
		return false;
	}
	if(!LoadNLDStrings(&sdata->egogetemone, fp)){
		fclose(fp);
		return false;
	}
	fclose(fp);

	//E GO GET EM TWO DATA
	if(!(fp = fopen(EMERGENCYGOGETEMTWOFILE, "r"))){
		return false;
	}
	if(!LoadNLDStrings(&sdata->egogetemtwo, fp)){
		fclose(fp);
		return false;
	}
	fclose(fp);

return true;
}

void FreeSartuInputLists(PSARTUDATASTRUCT sdata)
{
	FreeStringList(sdata->agentcodenames);
	FreeStringList(sdata->agentfirstnames);
	FreeStringList(sdata->agentlastnames);
	FreeStringList(sdata->missioncodesone);
	FreeStringList(sdata->missioncodestwo);
	FreeStringList(sdata->technologies);
	FreeStringList(sdata->targetfirstnames);
	FreeStringList(sdata->targetlastnames);
	FreeStringList(sdata->targetextras);
	FreeStringList(sdata->gogetemone);
	FreeStringList(sdata->gogetemtwo);
	FreeStringList(sdata->egogetemone);
	FreeStringList(sdata->egogetemtwo);
}

void FreeStringList(PLIST list)
{
	PNODE node;
	node = (list)->head;
	while(node != NULL){
		free(node->object.string);
		node = GetNextListEntry(node);
	}
	freeList(list);
	free(list);
}



bool LoadNLDStrings(PLIST* list, FILE* fp) //LOAD NEWLINE DELINEATED STRINGS
{
	POTYPE thing;
	char buffer[255];

	*list = createList();
	if (*list == NULL){
		return false;
	}
	while(!feof(fp)){
		fgets(buffer, 255, fp);
		for(int i = 0; i < 255; i++){
			if (buffer[i] == '\n'){
				buffer[i] = '\0';
				break;
			}
		}
		if(!strcmp(buffer, ""))
			break;
		thing.string = (char*) malloc(sizeof(char) * (strlen(buffer) + 1));
		if(thing.string == NULL){
			return false;
		}
		strcpy(thing.string, buffer);
	
		addtoList(*list, thing, StringCompare);
	}
	return true;
}

int GetNumberOfMissionsPerDay(PPLAYER player)
{
	int min;
	int max;

	switch(player->rank)
	{
	case AGENT_RANK_FOR:
		min = 5;
		max = 8;
		break;
	case AGENT_RANK_FO:
		min = 7;
		max = 10;
		break;
	case AGENT_RANK_CO:
		min = 10;
		max = 13;
		break;
	case AGENT_RANK_COS:
		min = 13;
		max = 16;
		break;
	case AGENT_RANK_ECOS:
	case AGENT_RANK_ECOSSPECONE:
	default:
		min = 16;
		max = 20;
		break;
	}

	return random(min, max);
}