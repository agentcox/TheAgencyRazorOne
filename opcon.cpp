#include "agencyinclude.h"
#include "guns.h"

bool OpConElevator(PPLAYER player)
{
	char* LevelStrings[7] = {"Level I - General - Main Floor", "Level II - Logistics", "Level III - Information Operations", "Level IV - Research and Development", "Level V - Operations Control", "Level VI - Nuclear and Chemical Weapons", "Level VII - Biological And Special Weapons"};
	PLIST levellist = createList();
	POTYPE levels[7];
	POTYPE exit;
	bool startmusicover = false;



	exit.string = (char*) malloc(sizeof(char) * (strlen("Exit DSA") + 1));
	strcpy(exit.string, "Exit DSA");
	addtoList(levellist, exit, StringCompare);
	
	for(int i = 0; i < 7; i++){
		levels[i].string = (char*) malloc(sizeof(char) * (strlen(LevelStrings[i]) + 1));
		strcpy(levels[i].string, LevelStrings[i]);
		addtoList(levellist, levels[i], StringCompare);
	}
	
	while(1)
	{
			if (player->sealedflag || player->status != OPERATIVE_STATUS_ACTIVE){
				PlaySong(globals.musiclist.songs[ENDGAME_SONG]);
				FinalCable(player);
				FinalAnalysis(player);
		return true; //YOU GOTTA GO!
	}
			playMIXsound(ELEVATOR_SOUND);
			FadeInTitle("SECURED ELEVATOR", "DEFENSE SECURITY AGENCY");
		cls(0);
		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 0, TRUE);
	//START ELEVATOR SWITCH
		switch(SimpleListDialogBox("Defense Security Agency", "Choose Floor", levellist, &globals.dboxparams, CreateStringDBListItem))
	{
	case 0:
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, TRUE);
		if(player->currentlymobilizing){
			if(YesNoDialogBox("CONFIRM", "Exiting Now Will Count The Current Mission As A *FAILURE*. Are you sure you want this?", "(Y)es/(N)o", &globals.dboxparams))
			{
				FreePlayerFromCombat(player);
				freeList(levellist);
				
				SavePlayer(player);
				return true;
			}
		}
		else{
		if (YesNoDialogBox("Confirm", "Are you sure you wish to exit DSA? Your current character data WILL be saved.", "(Y)es/(N)o", &globals.dboxparams)){
		freeList(levellist);
		SavePlayer(player);
			return true;
		}
		}
		break;
	case 1:
		FadeInTitle("LEVEL I", "GENERAL SERVICES");
		playMIXsound(BELLS_SOUND);
		LevelOne(player);
		
		break;
	case 2:
		FadeInTitle("LEVEL II", "LOGISTICS");
		playMIXsound(BELLS_SOUND);
		LevelTwo(player);
		
		break;
	case 3:
		FadeInTitle("LEVEL III", "INFORMATION OPERATIONS");
		playMIXsound(BELLS_SOUND);
		LevelThree(player);
				
		break;
	case 4:
		FadeInTitle("LEVEL IV", "RESEARCH AND DEVELOPMENT");
		playMIXsound(BELLS_SOUND);
		LevelFour(player);
		break;
	case 5:
		FadeInTitle("LEVEL V", "OPERATIONS CONTROL");
		playMIXsound(BELLS_SOUND);
		LevelFive(player);
		break;
	case 6:
		FadeInTitle("LEVEL VI", "NUCLEAR AND CHEMICAL WEAPONS");
		playMIXsound(BELLS_SOUND);
		LevelSix(player);
		break;
	case 7:
	SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, TRUE);
	playMIXsound(TWOBEEPS_SOUND);
	StaticDialogBox("Level", "The button depresses...but does not light. You see a small reader blinking on the elevator wall. Apparently, Level VII is sealed off to all but the most highly-cleared personnel.\nPress ENTER.","Sealed", &globals.dboxparams);
	}
		
	//END SWITCH
	if (startmusicover){
		LoopOpConMusic(player);
		startmusicover = false;
	}
	}
}

void LoopOpConMusic(PPLAYER player)
{
	if(player->currentlymobilizing){
		LoopSong(globals.musiclist.songs[MOBILIZE_SONG]);
	}
	else{
		LoopSong(globals.musiclist.songs[TSSI_SONG]);
	}
}

void LevelOne(PPLAYER player)
{
		char* LevelStrings[5] = {"Game Options", "Main Door - Head Home", "The Silent Wall Memorial", "DSA Founder's Hall (Credits)", "Directors Office (Debug)"};
		PLIST levellist = createList();
	POTYPE levels[5];
	POTYPE exit;
	bool startmusicover = false;

	exit.string = (char*) malloc(sizeof(char) * (strlen("Back To Elevator") + 1));
	strcpy(exit.string, "Back To Elevator");
	addtoList(levellist, exit, StringCompare);
	
	for(int i = 0; i < 5; i++){
		levels[i].string = (char*) malloc(sizeof(char) * (strlen(LevelStrings[i]) + 1));
		strcpy(levels[i].string, LevelStrings[i]);
		addtoList(levellist, levels[i], StringCompare);
	}
	
	while(1)
	{
			if (player->sealedflag || player->status != OPERATIVE_STATUS_ACTIVE){
		return; //YOU GOTTA GO!
	}
		cls(0);
		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 0, TRUE);
	//START ELEVATOR SWITCH
		switch(SimpleListDialogBox("Level I", "Choose Department", levellist, &globals.dboxparams, CreateStringDBListItem))
	{
	case 0:
		freeList(levellist);
			return;
	case 1:
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, TRUE);
		playMIXsound(TWOBEEPSMOD_SOUND);
		StaticDialogBox("Options", "Game Options Are Currently Unavailable.\nPress ENTER to return.", "Currently Unavailable", &globals.dboxparams);
		break;
	case 2:
		if(HeadHome(player)){
			if (player->numberofdaysincycle <= 0){
				WrapUpMonth(player); //WE'RE DONE WITH THIS MONTH.
				if (player->sealedflag){
					return; //YOU GOT FIRED!!!!
				}
			
			}
				PrintDayAndMonth(player);
		return;
		}
		break;
	case 3:
		LoopSong(globals.musiclist.songs[CAPTIVITY_SONG]);
		SilentWall();
		startmusicover = true;
		break;
	case 4:
		CreditScreen();
		startmusicover = true;
		break;
	case 5:
		PlayerDataDebugFunctions(player);
		break;
		}
	
	//END SWITCH
	if (startmusicover){
		LoopOpConMusic(player);
		startmusicover = false;
	}
	}
}

void LevelTwo(PPLAYER player)
{
char* LevelStrings[1] = {"Mobilize - Begin Mission"};
	PLIST levellist = createList();
	POTYPE levels[1];
	POTYPE exit;
	bool startmusicover = false;

	exit.string = (char*) malloc(sizeof(char) * (strlen("Back To Elevator") + 1));
	strcpy(exit.string, "Back To Elevator");
	addtoList(levellist, exit, StringCompare);
	
	for(int i = 0; i < 1; i++){
		levels[i].string = (char*) malloc(sizeof(char) * (strlen(LevelStrings[i]) + 1));
		strcpy(levels[i].string, LevelStrings[i]);
		addtoList(levellist, levels[i], StringCompare);
	}
	
	while(1)
	{
			if (player->sealedflag || player->status != OPERATIVE_STATUS_ACTIVE){
		return; //YOU GOTTA GO!
	}
		cls(0);
		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 0, TRUE);
	//START ELEVATOR SWITCH
		switch(SimpleListDialogBox("Level II", "Choose Department", levellist, &globals.dboxparams, CreateStringDBListItem))
	{
	case 0:
		freeList(levellist);
			return;
	case 1:
		if(!player->currentlymobilizing){
			SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, FALSE);
			StaticDialogBox("Center", "It doesn't look like any of the mobilization bays are green. Nobody is waiting for you. It occurs to you that you haven't assigned yourself to any missions currently. You should head to Level V to take care of that immediately.\nPress ENTER to return.", "Unoccupied", &globals.dboxparams);
		}
		else if(!YesNoDialogBox("Prepare To Mobilize", "Are you absolutely sure that you are ready to mobilize? After this, there is no turning back. Hit Y only if you are sure that you have all of the weapons, ammunition, and technology that you think you'll need.", "(Y)es/(N)o", &globals.dboxparams))
		{
			break;
		}
		else if(!FinalMobilizationChecks(player)){
			break;
		}
		else{
			SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_MEDIUM, 1, FALSE);
	StaticDialogBox("Final Checks Complete", "All final mobilization checks are complete. You're clear to proceed into the Operations Van. Good luck, Operative.", "Press ENTER", &globals.dboxparams);
		//GET AMOUNT OF TIME!
			
		//BEGIN THE MISSION!!!!!!!!
			int RetVal = Mobilization(player);
			StopSong();
			startmusicover = true;
			player->currentlymobilizing = false;

			//MISSION CONCLUDED. GET FINAL STATS.
			AnalyzeMissionEndCode(player, RetVal);
	
	
	return;
	}
		break;
	}
	
	//END SWITCH
	if (startmusicover){
		LoopOpConMusic(player);
		startmusicover = false;
	}
	}
}

void LevelThree(PPLAYER player)
{
	char* LevelStrings[3] = {"The Office of Records", "Honor Board", "Code Retrieval And Modification Center"};
	PLIST levellist = createList();
	POTYPE levels[3];
	POTYPE exit;
	bool startmusicover = false;

	exit.string = (char*) malloc(sizeof(char) * (strlen("Back To Elevator") + 1));
	strcpy(exit.string, "Back To Elevator");
	addtoList(levellist, exit, StringCompare);
	
	for(int i = 0; i < 3; i++){
		levels[i].string = (char*) malloc(sizeof(char) * (strlen(LevelStrings[i]) + 1));
		strcpy(levels[i].string, LevelStrings[i]);
		addtoList(levellist, levels[i], StringCompare);
	}
	
	while(1)
	{
			if (player->sealedflag || player->status != OPERATIVE_STATUS_ACTIVE){
		return; //YOU GOTTA GO!
	}
		cls(0);
		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 0, TRUE);
	//START ELEVATOR SWITCH
		switch(SimpleListDialogBox("Level III", "Choose Department", levellist, &globals.dboxparams, CreateStringDBListItem))
	{
	case 0:
		freeList(levellist);
			return;
	case 1:
		OfficeOfRecordsAccessPlayerDossiers();
		//ENTER THE OFFICE OF RECORDS.
		break;
	case 2:
		HonorBoardMainMenu(player);
		break;
	case 3:
		CRMC(player);
		break;
	}
	
	//END SWITCH
	if (startmusicover){
		LoopOpConMusic(player);
		startmusicover = false;
	}
	}
}

void LevelFour(PPLAYER player)
{
	char* LevelStrings[4] = {"Weapons Requisition Center", "Ammunition Requisition Center", "Technology Requisition Center", "Pharmaceutical Requisiton Center"};
	PLIST levellist = createList();
	POTYPE levels[4];
	POTYPE exit;
	bool startmusicover = false;

	exit.string = (char*) malloc(sizeof(char) * (strlen("Back To Elevator") + 1));
	strcpy(exit.string, "Back To Elevator");
	addtoList(levellist, exit, StringCompare);
	
	for(int i = 0; i < 4; i++){
		levels[i].string = (char*) malloc(sizeof(char) * (strlen(LevelStrings[i]) + 1));
		strcpy(levels[i].string, LevelStrings[i]);
		addtoList(levellist, levels[i], StringCompare);
	}
	
	while(1)
	{
			if (player->sealedflag || player->status != OPERATIVE_STATUS_ACTIVE){
		return; //YOU GOTTA GO!
	}
		cls(0);
		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 0, TRUE);
	//START ELEVATOR SWITCH
		switch(SimpleListDialogBox("Level IV", "Choose Department", levellist, &globals.dboxparams, CreateStringDBListItem))
	{
	case 0:
		freeList(levellist);
			return;
	case 1:
		if (!player->currentlymobilizing){
			SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, FALSE);
			playMIXsound(TWOBEEPSMOD_SOUND);
			StaticDialogBox("Armory", "Your clearance card doesn't work here...you realize you haven't accepted a mission that you can be outfitted for. Your best bet is to head back to Level V and get assigned.\nPress ENTER to return.", "Restricted", &globals.dboxparams);
		}
		else{
			//WEAPON OUTFITTING!
			LevelIVWeaponLockers(player);

		}
		break;
	case 2:
	if (!player->currentlymobilizing){
			SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, FALSE);
			playMIXsound(TWOBEEPSMOD_SOUND);
			StaticDialogBox("Armory", "Your clearance card doesn't work here...you realize you haven't accepted a mission that you can be outfitted for. Your best bet is to head back to Level V and get assigned.\nPress ENTER to return.", "Restricted", &globals.dboxparams);
		}
		else{
			//AMMO OUTFITTING!
			if(player->p_weapons->objcnt < 1){
				SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, FALSE);
			playMIXsound(TWOBEEPSMOD_SOUND);
			StaticDialogBox("No Ammunition", "You currently have no weapons selected for your assignment. No ammunition may be assigned without weaponry. Please proceed to the Weapons Requisition Center for weaponry outfitting.\nPress ENTER to return.", "Necessary", &globals.dboxparams);
			}
			else{
				LevelIVAmmunitionLockers(player);
			}
		}
		break;
	case 3:
			if (!player->currentlymobilizing){
			SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, FALSE);
			playMIXsound(TWOBEEPSMOD_SOUND);
			StaticDialogBox("Armory", "Your clearance card doesn't work here...you realize you haven't accepted a mission that you can be outfitted for. Your best bet is to head back to Level V and get assigned.\nPress ENTER to return.", "Restricted", &globals.dboxparams);
		}
		else{
			//GIZMO OUTFITTING!
			LevelIVGizmoLockers(player);

		}
		break;
	case 4:
			if (!player->currentlymobilizing){
			SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, FALSE);
			playMIXsound(TWOBEEPSMOD_SOUND);
			StaticDialogBox("Armory", "Your clearance card doesn't work here...you realize you haven't accepted a mission that you can be outfitted for. Your best bet is to head back to Level V and get assigned.\nPress ENTER to return.", "Restricted", &globals.dboxparams);
		}
		else{
			//SYRINGE OUTFITTING!
			LevelIVPharmacy(player);
		}
		break;
	}
	
	//END SWITCH
	if (startmusicover){
		LoopOpConMusic(player);
		startmusicover = false;
	}
	}
}

void LevelFive(PPLAYER player)
{
	char* LevelStrings[2] = {"Secure Network Terminal (SARTU)", "Training Center"};
	PLIST levellist = createList();
	POTYPE levels[2];
	POTYPE exit;
	bool startmusicover = false;

	exit.string = (char*) malloc(sizeof(char) * (strlen("Back To Elevator") + 1));
	strcpy(exit.string, "Back To Elevator");
	addtoList(levellist, exit, StringCompare);
	
	for(int i = 0; i < 2; i++){
		levels[i].string = (char*) malloc(sizeof(char) * (strlen(LevelStrings[i]) + 1));
		strcpy(levels[i].string, LevelStrings[i]);
		addtoList(levellist, levels[i], StringCompare);
	}
	
	while(1)
	{
			if (player->sealedflag || player->status != OPERATIVE_STATUS_ACTIVE){
		return; //YOU GOTTA GO!
	}
		cls(0);
		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 0, TRUE);
	//START ELEVATOR SWITCH
		switch(SimpleListDialogBox("Level V", "Choose Department", levellist, &globals.dboxparams, CreateStringDBListItem))
	{
	case 0:
		freeList(levellist);
			return;
	case 1:
		//SARTU
		if(player->currentlymobilizing){
	SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, FALSE);
	playMIXsound(TWOBEEPSMOD_SOUND);
	StaticDialogBox("Mobilization", "You enter the Network Terminal Center, only to realize that you're denied access to today's mission lists. You are currently in mobilization phase with a mission assigned to you. You had better get moving on that mission.", "Phase", &globals.dboxparams);
	break;
		}
		else{
			SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, TRUE);
			playMIXsound(TWOBEEPSMOD_SOUND);
			//THE SARTU NETWORK!
			LoopSong(globals.musiclist.songs[SARTU_SONG]);
			SartuTerminal(player);
		}
			break;
	case 2:
		//TRAINING CENTER
		if(player->currentlymobilizing){
	SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, FALSE);
	playMIXsound(TWOBEEPSMOD_SOUND);
	StaticDialogBox("Mobilization", "You are denied access to the Training Center, as you are currently mobilizing for a mission.\nPress ENTER.", "Phase", &globals.dboxparams);
	break;
		}
		else{
		TrainingCenter(player);
		}
		break;
	}
	
	//END SWITCH
	if (startmusicover){
		LoopOpConMusic(player);
		startmusicover = false;
	}
	}
}

void LevelSix(PPLAYER player)
{
	char* LevelStrings[1] = {"Atomic Munitions Vault"};
	PLIST levellist = createList();
	POTYPE levels[1];
	POTYPE exit;
	bool startmusicover = false;

	exit.string = (char*) malloc(sizeof(char) * (strlen("Back To Elevator") + 1));
	strcpy(exit.string, "Back To Elevator");
	addtoList(levellist, exit, StringCompare);
	
	for(int i = 0; i < 1; i++){
		levels[i].string = (char*) malloc(sizeof(char) * (strlen(LevelStrings[i]) + 1));
		strcpy(levels[i].string, LevelStrings[i]);
		addtoList(levellist, levels[i], StringCompare);
	}
	
	while(1)
	{
			if (player->sealedflag || player->status != OPERATIVE_STATUS_ACTIVE){
		return; //YOU GOTTA GO!
	}
		cls(0);
		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 0, TRUE);
	//START ELEVATOR SWITCH
		switch(SimpleListDialogBox("Level VI", "Choose Department", levellist, &globals.dboxparams, CreateStringDBListItem))
	{
	case 0:
		freeList(levellist);
			return;
	case 1:
		if (!player->currentlymobilizing || player->currentmissioninfo.missiontype != MISSION_TYPE_CAUTERIZATION){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, FALSE);
		playMIXsound(TWOBEEPSMOD_SOUND);
		StaticDialogBox("AMV", "Your card does not allow you access into the Atomic Munitions Vault. It occurs to you that you have no business within the vault yet. Perhaps at another time.\nPress ENTER to return.", "Restricted", &globals.dboxparams);
		}
		else{
			//GIVE BOMB.
			GiveAtomicDevice(player);
		}
			break;
	}
	
	//END SWITCH
	if (startmusicover){
		LoopOpConMusic(player);
		startmusicover = false;
	}
	}
}

bool HeadHome(PPLAYER player)
{
	char buf[255];
	int daystotakeoff = 0;
	if (player->currentlymobilizing){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, FALSE);
		StaticDialogBox("Mobilization", "You are currently in mobilization phase for an assigned mission. There's no going home yet; you must complete your mission.\nPress ENTER to return.", "Phase", &globals.dboxparams);
		return false;
	}
	cls(0);
	sprintf(buf, "Are you sure you wish to head home? You currently have %d missions left to complete this month, and %d days in which to complete them.", player->missionsleftthiscycle, player->numberofdaysincycle);
	if(!YesNoDialogBox("Heading Home", buf, "(Y)es/(N)o", &globals.dboxparams)){
		return false;
	}
	while(1)
	{
		cls(0);
		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 0, FALSE);
	sprintf(buf, "How many days do you wish to take off? You may take up to %d days this month.", player->numberofdaysincycle);
	daystotakeoff = NumberDialogBox("Heading", buf, "Home", 2, &globals.dboxparams);
	if(daystotakeoff == 0){
		return false;
	}
	if (daystotakeoff <= player->numberofdaysincycle){
		break;
	}
	}
	
	if (daystotakeoff == 1)
	{
		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_LARGE, 0, FALSE);
		ComputerPrintOutString("You scan out of the system and pick up your firearm on the first floor. The door opens to another cloudy Virginia night. It's time to go home for a while. Arriving at your apartment, you pour a drink and slip off your jacket. Your temples hurt and your mind seems elsewhere. As you sip from your drink, you reflect on your life. Taking your pistol from your holster, you place it by your bed and prepare to turn in for the night. Hopefully sleep will come easily tonight.", 30, "Press ENTER to Return To The Agency.");
	}
	else if (daystotakeoff <=5 && daystotakeoff >= 2)
	{
		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_LARGE, 0, FALSE);
		ComputerPrintOutString("You check out of the system on leave and pick up your firearm on the first floor. You step out into the rainy night. You don't know where exactly you'll go...it seems like an awfully large world all of a sudden. You haven't got much time until you'll have to be back. A trip to the coast sounds nice...you figure you'll start tomorrow. You slide your pistol into your holster; it's nice to know it's there, just in case.", 30, "Press ENTER to Return To The Agency.");
	}
	else if (daystotakeoff >= 6)
	{
				SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_LARGE, 0, FALSE);
		ComputerPrintOutString("You've already booked your flight electronically and received pre-confirmation by the time you check out of the DSA computer and pick up your firearm on the first floor. You're booked on the red-eye out of Virginia. \"Anywhere but here\", you thought when you booked your flight. Hopefully, it'll be relaxing...you've had about all you can stand of this place for the moment. As you get into your car, you feel free for the first time in a long while.",30, "Press ENTER to Return To The Agency.");
	}

	player->numberofdaysincycle -= daystotakeoff;
	CheckForHomePsychIncrease(player, daystotakeoff);

	//THIS IS GOING TO BE TIME CONSUMING....
	int numbermissions;
	double percent = 0.0;
	double percentincrement = 100/ (daystotakeoff);

	for(int k = 0; k < daystotakeoff; k++){
	numbermissions = GetNumberOfMissionsPerDay(player);
	PrintPercentageComplete("Retrieving Network Traffic", (int)percent);
	SartuDailySession(player, numbermissions);
	ShutDownSartuForTheDay(player, &globals.sartudata);
		percent += percentincrement;
	}
	InitializeRealisticWeaponAvailability(globals.globalweaponsammo.weaponlist, globals.globalweaponsammo.ammolist, globals.armorygizmolist);
	return true;
}


void CheckForHomePsychIncrease(PPLAYER player, int daystotakeoff)
{
	BOOL statincrease;
	int* modifypsych;
	char psychbuf[20];
	char increasebuf[200];
	//BASE CHANCE IS 5%.
	//RAISE AS HIGH AS 15%.
	double basechance = 5.0;
	basechance += ((double)daystotakeoff / 30.0) * 50.0;
	statincrease = RandomPercentage((int)basechance);

	if(!statincrease)
	{return;}
		switch(rand()%5)
		{
		case 0:
			modifypsych = &player->psychskill.psych.agression;
			strcpy(psychbuf, "Aggression");
			break;
		case 1:
			modifypsych = &player->psychskill.psych.calmness;
			strcpy(psychbuf, "Calmness");
			break;
		case 2:
			modifypsych = &player->psychskill.psych.intelligence;
			strcpy(psychbuf, "Intelligence");
			break;
		case 3:
			modifypsych = &player->psychskill.psych.reflex;
			strcpy(psychbuf, "Reflex");
			break;
		case 4:
		default:
			modifypsych = &player->psychskill.psych.willpower;
			strcpy(psychbuf, "Willpower");
				break;
		}
		if(*modifypsych < MAX_PSYCH){
		*modifypsych += 1;
		sprintf(increasebuf, "Your time off has done you good.\n%s Increased!\nPress ENTER.", psychbuf);
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_SMALL, 0, TRUE);
		StaticDialogBox("PSYCHOLOGICAL", increasebuf, "SHARPENING", &globals.dboxparams);
		}
}

void SilentWall()
{
	PPLAYER DeadOperatives;
	PLAYER TempPlayer;
	int NumAgents = 0;
	POTYPE* agents;

	HANDLE handle;
	WIN32_FIND_DATA finddata;
	char path[255];
	char buf[255];
	char buftwo[255];
	int agentcounter = 0;
	int returncode = 1;
	strcpy(path, "Agents\\*.agt");
	
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 0, TRUE);
	
	agents = (PPOTYPE) malloc(sizeof(POTYPE) * GetNumberOfAgentFiles());
	DeadOperatives = (PPLAYER) malloc(sizeof(PLAYER) * GetNumberOfAgentFiles());

	handle = FindFirstFile(path, &finddata);
	while (handle != INVALID_HANDLE_VALUE && returncode != 0){
		agents[agentcounter].string = (char*) malloc(sizeof(char) * (strlen(finddata.cFileName) + 1));
		strcpy(agents[agentcounter].string, finddata.cFileName);
		returncode = FindNextFile(handle, &finddata);
		agentcounter++;
	}
	FindClose(handle);

	for(int i = 0; i < agentcounter; i++)
	{
		if(LoadPlayer(agents[i].string, &TempPlayer)){
		if(TempPlayer.status == OPERATIVE_STATUS_CYANIDED)
		{
			DeadOperatives[NumAgents] = TempPlayer;
			NumAgents++;
		}
		}
		free(agents[i].string);
		
	}
	free(agents);

	//NOW, THE WALL.
	ComputerPrintOutString("The Silent Wall holds the last rememberances of Agents passed away under their own hand whilst in active service. Their sacrifices serve as a reminder to us all to cherish life, and death, as sacred. These brave souls will not be forgotten; The Agency honors their memory.\nVERITAS VENTOSUS MORTIS ABSOLVO\nTruth Is Relative, Death Is Absolute\n\nBrethren of The Agency, may your souls rest in peace.", 20, "Press ENTER to View The Silent Wall.");

	if(!NumAgents){
		FadeInTitle("No Names", "Appear Visible On The Wall.");
	}
	
	for(i = 0; i < NumAgents; i++)
	{
		sprintf(buf, "Agent %s %s", DeadOperatives[i].fname, DeadOperatives[i].lname);
		sprintf(buftwo, "\"%s\"", DeadOperatives[i].codename);
		FadeInTitle(buf, buftwo);
		Sleep(1000);
		cls(0);
	}

	FadeInTitle("Ashes To Ashes", "Dust To Dust");
	Sleep(1000);
	//END THE WALL.
	free(DeadOperatives);
}