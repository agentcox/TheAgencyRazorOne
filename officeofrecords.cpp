#include "agencyinclude.h"

void OfficeOfRecordsAccessPlayerDossiers()
{
		PLIST filelist = createList();
	POTYPE* agents;
//	POTYPE newop;
	POTYPE quit;
	HANDLE handle;
	WIN32_FIND_DATA finddata;
	char path[255];
//	char buf[255];
	int agentcounter = 0;
	int returncode = 1;
	strcpy(path, "Agents\\*.agt");
	PLAYER playertocheck;
	
	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 0, TRUE);
	
	agents = (PPOTYPE) malloc(sizeof(POTYPE) * GetNumberOfAgentFiles());
	
	quit.string = (char*) malloc(sizeof(char) * (strlen("Exit Game") + 1));
	strcpy(quit.string, "CLOSE DOSSIERS");
	addtoList(filelist, quit, CompareStringID);
	
	handle = FindFirstFile(path, &finddata);
	while (handle != INVALID_HANDLE_VALUE && returncode != 0){
		agents[agentcounter].string = (char*) malloc(sizeof(char) * (strlen(finddata.cFileName) + 1));
		strcpy(agents[agentcounter].string, finddata.cFileName);
		addtoList(filelist, agents[agentcounter], CompareStringID);
		returncode = FindNextFile(handle, &finddata);
		agentcounter++;
	}
	FindClose(handle);

	while(1)
	{
		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 0, FALSE);
		cls(0);
		int returncode = SimpleListDialogBox("Select Agent", "To View", filelist, &globals.dboxparams, CreateAgentNameListItem);
		if(!returncode){
			freeList(filelist);
			return;
		}
		else{ //IF WE'VE SELECTED AN AGENT....
			if(!LoadPlayer(ReturnStringListItemByNumber(returncode + 1, filelist), &playertocheck)){
				SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_SMALL, 0, FALSE);
				StaticDialogBox("Error", "This record does not exist. This is an internal error.", "Error", &globals.dboxparams);
			}
			else{
				if(playertocheck.sealedflag){
					SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_SMALL, 0, FALSE);
				StaticDialogBox("Sealed File", "This Operative's File Has Been Sealed. No Data Is Available.", "Sealed File", &globals.dboxparams);
				}else{
				cls(0);
				OfficeOfRecordsPlayerDossierChooseDossier(&playertocheck);
				}
			}
		}
	}
}

void OfficeOfRecordsPlayerDossierChooseDossier(PPLAYER player)
{
	char *recordtypes[6] = {"RETURN TO MAIN FILES", "Agent Functional Report", "Agent Quantified Mission Report", "Agent Skillset And Specialization Report", "Agent Psychological Profile", "Hardcopy Retrieval"};
	PLIST list;
	int result;
	CreateDBStringList(&list, recordtypes, 6);

	while(1)
	{
		cls(0);
		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 0, FALSE);
	result = SimpleListDialogBox("Agent Reports", "Select Available", list, &globals.dboxparams, CreateStringDBListItem);
	switch(result)
	{
	case 0:
		FreeDBStringList(list);
		return;
	case 1:
		OfficeOfRecordsPlayerDossierOne(player);
		break;
	case 2:
		OfficeOfRecordsPlayerDossierTwo(player);
		break;
	case 3:
		OfficeOfRecordsPlayerDossierThree(player);
		break;
	case 4:
		OfficeOfRecordsPlayerDossierFour(player);
		break;
	case 5:
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_SMALL, 0, TRUE);
		//if(YesNoDialogBox("PRINTER STANDBY", "Are you sure you wish to print this agent's dossier? It will be sent to LPT1 for immediate printing.", "(Y)es/(N)o", &globals.dboxparams))
	//	SendPlayerDossierToPrinterorText(player, true);
		StaticDialogBox("Printers", "The printers are currently down. This feature is unavailable.", "Unavailable", &globals.dboxparams);
		break;
	}
	}
}

void SendPlayerDossierToPrinterorText(PPLAYER player, bool toprinter)
{
	int returncode;
	if(toprinter)
	{
		HANDLE hPrint = CreateFile("lpt1:", GENERIC_WRITE,0,NULL,OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL); 
		HANDLE oldHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		char buffertoprint[5000];
		char buffertohold[5000];
		sprintf(buffertoprint, "DEFENSE SECURITY AGENCY\nVeritas Ventosus Mortis Absolvo\n-----------------------------------\nAgent Report Begins\n");
		strcat(buffertoprint, OfficeOfRecordsGetGeneralInformation(buffertohold, player));
		DWORD iLen; 
		iLen = strlen(buffertoprint);
		
		if(hPrint){
			returncode = SetStdHandle(STD_OUTPUT_HANDLE, hPrint);
			printf(buffertoprint);
			returncode = SetStdHandle(STD_OUTPUT_HANDLE, oldHandle);
			//WriteFile(hPrint, buffertoprint,iLen,&iTest,NULL); 
		CloseHandle(hPrint);
	}
	}
	
}

char* OfficeOfRecordsGetGeneralInformation(char* record, PPLAYER player)
{

	char buf[250];
	sprintf(record, "----General History For Agent %s----\n", player->lname);
	sprintf(buf, "Clearance: Agent %s Is Cleared For Level V.\n", player->lname);
	strcat(record, buf);
	sprintf(buf, "Rank: ");
	strcat(record, buf);
	//GET RANK.
	switch(player->rank){
	case AGENT_RANK_FOR:
		strcat(record, "Field Operative Recruit.");
		break;
	case AGENT_RANK_FO:
			strcat(record, "Field Operative.");
			break;
			case AGENT_RANK_CO:
			strcat(record, "Covert Operative.");
			break;
			case AGENT_RANK_COS:
			strcat(record, "Covert Operations Specialist.");
			break;
			case AGENT_RANK_ECOS:
			strcat(record, "Elite Covert Operations Specialist.");
			break;
			case AGENT_RANK_ECOSSPECONE:
			strcat(record, "Elite Covert Operations Specialist First Class.");
			break;
			default:
				strcat(record, "Operative.");
				break;
	}

	strcat(record, "\n");

	//STATUS
	sprintf(buf, "Status: ");
	strcat(record, buf);
	switch(player->status)
	{
	case OPERATIVE_STATUS_ACTIVE:
		strcat(record, "Currently In Active Service With DSA.");
		break;
	case OPERATIVE_STATUS_CYANIDED:
		strcat(record, "Reported Self-Terminated While On Assignment.");
		break;
	case OPERATIVE_STATUS_NOTOCHORDED:
		strcat(record, "Forcibly Terminated After Breaching DSA Security Protocol 3.1.");
		break;
	case OPERATIVE_STATUS_RETIRED:
		strcat(record, "Honorably Retired From Active Service With DSA.");
		break;
	case OPERATIVE_STATUS_WHEREABOUTS_UNKNOWN:
		strcat(record, "Currently Missing From DSA Operations. Whereabouts Are Unknown.");
		break;
	default:
		strcat(record, "No Data Available.");
		break;
	}

	strcat(record, "\n");

	//CUMULATIVE SCORE.
	sprintf(buf, "Cumulative Rating: Currently %2.2f%%\n", player->history.cumulativemissionrating);
	strcat(record, buf);
	//MEDALS.
	strcat(record, "Commendations:\n");
	sprintf(buf, "%d Defense Medals.\n",  player->medals.defensemedals);
	strcat(record,buf);
	sprintf(buf, "%d Defense Medal(s) With Valor Devices.\n",  player->medals.defensemedalsV);
	strcat(record,buf);
	sprintf(buf, "%d Silver Eagle Commendation(s).\n",  player->medals.silvereagle);
	strcat(record,buf);
	sprintf(buf, "%d Gold Eagle Commendation(s).\n",  player->medals.goldeagle);
	strcat(record,buf);
	sprintf(buf, "%d DSA Honor Commendation(s).\n",  player->medals.honorcommendation);
	strcat(record,buf);
	sprintf(buf, "%d Medal(s) Of The Falcon.\n",  player->medals.medalofthefalcon);
	strcat(record,buf);

	if(player->status == OPERATIVE_STATUS_CYANIDED){
		sprintf(buf, "Agent %s Is A Recipient Of The Medal Of Truth.\n", player->lname);
	strcat(record,buf);
	}
	return record;
}

void OfficeOfRecordsPlayerDossierOne(PPLAYER player)
{
	//RANK
	//STATUS
	//CUMULATIVE %
	//MEDALS
	char record[5000];
	

	//DONE!
	OfficeOfRecordsGetGeneralInformation(record, player);
	strcat(record, "Press ENTER To Close This File.");
	SetGlobalDBoxParam(DB_SCHEME_MONOCHROMATIC, DB_LARGE, 0, FALSE);
	StaticDialogBox("DSA OFFICIAL REPORT", record, "DO NOT DISTRIBUTE", &globals.dboxparams);
}

char* OfficeOfRecordsGetQuantifiedReport(char* record, PPLAYER player)
{
		char buf[250];
	sprintf(record, "----Quantified Mission History For Agent %s----\n", player->lname);

	sprintf(buf, "Missions Attempted: %d\n", player->history.missionsattempted);
	strcat(record, buf);
	sprintf(buf, "Missions Accomplished: %d\n", player->history.missionsaccomplished);
	strcat(record, buf);
	if( (double)player->history.missionsattempted != 0.0){
	sprintf(buf, "Mission Completion Ratio: %2.2f%%\n", (double)player->history.missionsaccomplished * 100.0 / (double)player->history.missionsattempted != 0.0);
	}
	else{
		sprintf(buf, "Mission Completion Ratio: %2.2f%%\n", 0);
	}
	strcat(record, buf);

	sprintf(buf, "----------------------------------\n");
	strcat(record, buf);
	sprintf(buf, "Confirmed Kills: %d\n", player->history.kills);
	strcat(record, buf);
	sprintf(buf, "Dollar Damage in Millions US: $%2.2fM\n", player->history.damageinmillions);
	strcat(record, buf);
	sprintf(buf, "----------------------------------\n");
	strcat(record, buf);
	sprintf(buf, "Times Captured: %d\n", player->history.timescaptured);
	strcat(record,buf);
	sprintf(buf, "Times Escaped: %d\n", player->history.timesescaped);
	strcat(record, buf);
	sprintf(buf, "Times Rescued: %d\n", player->history.timesrescued);
	strcat(record, buf);
	return record;
}

void OfficeOfRecordsPlayerDossierTwo(PPLAYER player)
{

		/*
	Missions Attempted
Missions Accomplished
Kills
Damage In Millions
Times Captured
Times Escaped
Times Rescued

*/

	char record[5000];
	
	OfficeOfRecordsGetQuantifiedReport(record, player);
	strcat(record, "Press ENTER To Close This File.");
	SetGlobalDBoxParam(DB_SCHEME_MONOCHROMATIC, DB_LARGE, 0, FALSE);
	StaticDialogBox("DSA OFFICIAL REPORT", record, "DO NOT DISTRIBUTE", &globals.dboxparams);
}
char* OfficeOfRecordsGetSkillProfile(char* record, PPLAYER player)
{
	char * skilldesc[11] = {"deficient", "untrained", "a novice", "competent", "qualified", "certified", "highly trained", "extremely skilled", "a specialist", "a Class One"};
	char* skills[7] = {"Firearm Combat", "Stealth Combat", "Hand-To-Hand Combat", "Armed Melee Combat", "Sniping", "Mechanical Operation", "Electronics"};
		char buf[500];
			int skillset[7];
			skillset[0] = player->psychskill.skills.firearms;
	skillset[1] = player->psychskill.skills.stealthcombat;
	skillset[2] = player->psychskill.skills.meleecombatNOweapon;
	skillset[3] = player->psychskill.skills.meleecombatWITHweapon;
	skillset[4] = player->psychskill.skills.snipingcombat;
	skillset[5] = player->psychskill.skills.mechanical;
	skillset[6] = player->psychskill.skills.electronics;
	sprintf(record, "----Skillset/Specialization Report For Agent %s----\n", player->lname);
	sprintf(buf, "-----------------SKILL SET-----------------\n");
	strcat(record, buf);
	for(int i = 0; i < 7; i++){
		sprintf(buf, "Agent %s is %s in %s.\n", player->lname, skilldesc[skillset[i]], skills[i]);
		strcat(record, buf);
	}
	return record;
}

char* OfficeOfRecordsGetSpecializationProfile(char* record, PPLAYER player)
{
		char buf[250];
		char* specializations[9] = {"Hand Weapon", "Handgun", "Sub-Machine Gun", "Rifle", "Shotgun", "Special Weapons", "Explosives", "Mechanics", "Electronics"};
	char* speclevels[3] = {"Level One", "Level Two", "Level Three"};
	int specset[9];

	specset[0] = player->psychskill.specializations.handweapons;
	specset[1] = player->psychskill.specializations.handguns;
	specset[2] = player->psychskill.specializations.submachineguns;
	specset[3] = player->psychskill.specializations.rifles;
	specset[4] = player->psychskill.specializations.shotguns;	
	specset[5] = player->psychskill.specializations.specialweapons;
	specset[6] = player->psychskill.specializations.explosives;
	specset[7] = player->psychskill.specializations.mechanical;
	specset[8] = player->psychskill.specializations.electronics;

		sprintf(record, "--------SPECIALIZATIONS--------\n");

	for(int i = 0; i < 9; i++){
		if(specset[i] > 0){
			sprintf(buf, "Agent %s is a %s %s specialist.\n", player->lname, speclevels[specset[i] - 1], specializations[i]);
			strcat(record, buf);
		}
	}
	return record;
}

void OfficeOfRecordsPlayerDossierThree(PPLAYER player)
{
	/*
	All Skills
	*/
	char record[5000];
	


	OfficeOfRecordsGetSkillProfile(record, player);
	strcat(record, "Press ENTER To CONTINUE TO PAGE TWO.");
	SetGlobalDBoxParam(DB_SCHEME_MONOCHROMATIC, DB_LARGE, 0, FALSE);
	StaticDialogBox("DSA OFFICIAL REPORT", record, "DO NOT DISTRIBUTE", &globals.dboxparams);

	OfficeOfRecordsGetSpecializationProfile(record, player);
strcat(record, "Press ENTER To Close This File.");
	SetGlobalDBoxParam(DB_SCHEME_MONOCHROMATIC, DB_LARGE, 0, FALSE);
	StaticDialogBox("DSA OFFICIAL REPORT", record, "DO NOT DISTRIBUTE", &globals.dboxparams);
}

char* OfficeOfRecordsGetPsychProfile(char* record, PPLAYER player)
{
	char buf[250];

		char* tests[5] = {"Turner-Hoffman Aggressivity Scale", "NK Assay For Cognitive Intelligence", "Kearnes Post Stress Rating", "Young-Dwight Reflex Variance", "LeClerc Standard Willpower Analysis"};
	char* psychstrings[5][7] = {{
		"has minimal aggressive impulses",
		"has few aggressive tendencies",
		"has average aggressivity",
		"shows nominal aggression ratings",
		"exhibits above-average aggression",
		"tends toward aggressive approaches",
		"has extremely aggressive capacity"},

		{
				"cannot solve basic logic puzzles",
					"has difficulty with logic concepts",
					"exhibits poor deductive reasoning",
					"tests to average intelligence",
					"exhibits nominal cognizance",
					"shows extensive intelligence",
					"has superior intelligence"	},

		{
			"exhibits severe lack of calmness",
				"holds only marginal calmness",
				"exhibits average calmness",
				"remains fairly stable under distress",
				"exhibits nominal calmness",
				"shows remarkable calmness",
				"remains completely calm under duress"},

			{
				"has extremely poor response time",
					"has below-average reflex score",
					"holds average mean reflex downtime",
					"exhibits nominal reflex capacity",
					"exhibits above-average reflex",
					"shows unusually high reflex score",
					"exhibits extremely high reflex"},

				{
					"has little to no stability",
						"has only marginal resistance",
						"cannot stand up well under pressure",
						"has average willpower score",
						"rates high on willpower scale",
						"shows high willpower score",
						"is extremely resistant to influence"}};

					int psychsets[5] = {player->psychskill.psych.agression, player->psychskill.psych.intelligence, player->psychskill.psych.calmness, player->psychskill.psych.reflex, player->psychskill.psych.willpower};

					sprintf(record, "----Psychological Profile For Agent %s----\n", player->lname);
					sprintf(buf, "Test Results Follow-\n");
					strcat(record, buf);

					for(int i = 0; i < 5; i++){
						sprintf(buf, "----%s Results----\n", tests[i]);
						strcat(record, buf);
						sprintf(buf, "Agent %s %s.\n", player->lname, psychstrings[i][psychsets[i]]);
						strcat(record, buf);
					}
					return record;
}

void OfficeOfRecordsPlayerDossierFour(PPLAYER player){
		char record[5000];
//	char buf[500];


	OfficeOfRecordsGetPsychProfile(record, player);
	strcat(record, "Press ENTER To Close This File.");
	SetGlobalDBoxParam(DB_SCHEME_MONOCHROMATIC, DB_LARGE, 0, FALSE);
	StaticDialogBox("DSA OFFICIAL REPORT", record, "DO NOT DISTRIBUTE", &globals.dboxparams);
			
}


void HonorBoardMainMenu(PPLAYER player)
{
	char* types[4] = {"Return to Level III", "Blue Honor - Most Completed Missions", "Red Honor - Most Kills", "Green Honor - Most Damage in US Dollars"};
	PLIST list;
	int result;
	CreateDBStringList(&list, types, 4);

	while(1)
	{
		cls(0);
		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 0, FALSE);
	result = SimpleListDialogBox("Honor Board", "Select Honor", list, &globals.dboxparams, CreateStringDBListItem);
	switch(result)
	{
	case 0:
		FreeDBStringList(list);
		return;
	case 1:
		HonorBoard(HB_MISSION_SORTING);
		break;
	case 2:
		HonorBoard(HB_KILL_SORTING);
		break;
	case 3:
		HonorBoard(HB_DAMAGE_SORTING);
		break;
	}
	}
}

void HonorBoard(int sorting)
{
	//SORT BY MISSIONS COMPLETED.
	//DISPLAY...
	//[RANK] [LASTNAME] [MISSIONS COMPLETED] [% RATING] [KILLS] [$ DAMAGE]
	PPLAYER playerarray;
	PPLAYER honorplayerarray;
	char finallist[5000];
	char buf[500];
	char rankstr[50];
	int numagents = GetNumberOfSavedAgents();
	int qualifiedagents = numagents;
	playerarray = (PPLAYER) malloc(sizeof(PLAYER) * numagents);
	FillAgentArray(playerarray);
	//NOW, WEED OUT THE NON-EXISTANTS...
	for(int i = 0; i < numagents; i++){
		if(playerarray[i].sealedflag){
			qualifiedagents--;
		}
	}
	honorplayerarray = (PPLAYER) malloc(sizeof(PLAYER) * qualifiedagents);
	int r = 0;
	for(int k = 0; k < numagents; k++){
		if(!playerarray[k].sealedflag){
			honorplayerarray[r] = playerarray[k];
			r++;
		}
	}
	
	//WE NOW HAVE THE TOTAL ARRAY OF HONOR-ELIGIBLE AGENTS. NOW, WE MUST SPLIT TO SORT DIFFERENT WAYS.
	switch(sorting)
	{
	case HB_MISSION_SORTING:
		qsort((void*)honorplayerarray, qualifiedagents, sizeof(PLAYER), HBMissionCompare);
		break;
	case HB_KILL_SORTING:
		qsort((void*)honorplayerarray, qualifiedagents, sizeof(PLAYER), HBKillCompare);
		break;
	case HB_DAMAGE_SORTING:
		qsort((void*)honorplayerarray, qualifiedagents, sizeof(PLAYER), HBDamageCompare);
		break;
	}
	
	//NOW THEY'RE SORTED. TAKE THE TOP TEN AND CREATE THE LIST.
	
	int q = (qualifiedagents > 10 ? 10 : qualifiedagents);

	sprintf(finallist, "   Agent     M. Accomp.   Mission %%   Kills   Dmg. Mil.\n");
	
	for (int m = 0; m < q; m++){
		switch(honorplayerarray[m].rank){
		case AGENT_RANK_FOR:
			strcpy(rankstr, "FOR");
			break;
		case AGENT_RANK_FO:
			strcpy(rankstr, "FO");
			break;
		case AGENT_RANK_CO:
			strcpy(rankstr, "CO");
			break;
		case AGENT_RANK_COS:
			strcpy(rankstr, "COS");
			break;
		case AGENT_RANK_ECOS:
			strcpy(rankstr, "ECOS");
			break;
		case AGENT_RANK_ECOSSPECONE:
			strcpy(rankstr, "ECOS1");
			break;
		default:
			strcpy(rankstr, "UNK");
			break;
		}
		sprintf(buf, "%d. %-15s %d         %2.2f       %d       $%2.2f\n", m+1, honorplayerarray[m].lname, honorplayerarray[m].history.missionsaccomplished, honorplayerarray[m].history.cumulativemissionrating, honorplayerarray[m].history.kills, honorplayerarray[m].history.damageinmillions);
		strcat(finallist, buf);
	}

	switch(sorting)
	{
	case HB_MISSION_SORTING:
			SetGlobalDBoxParam(DB_SCHEME_COOLBLUELINES, DB_LARGE, 0, FALSE);
		StaticDialogBox("Blue Honor Board - Missions", finallist, "PRESS ENTER.", &globals.dboxparams);
		break;
	case HB_KILL_SORTING:
		SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_LARGE, 0, FALSE);
		StaticDialogBox("Red Honor Board - Kills", finallist, "PRESS ENTER.", &globals.dboxparams);
		break;
	case HB_DAMAGE_SORTING:
		SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_LARGE, 0, FALSE);
		StaticDialogBox("Green Honor Board - Damage", finallist, "PRESS ENTER.", &globals.dboxparams);
		break;
	}
	
	free(honorplayerarray);
	free(playerarray);
}

int HBMissionCompare(const void* elem1, const void* elem2)
{
	PLAYER p1 = *(PPLAYER)elem1;
	PLAYER p2 = *(PPLAYER)elem2;

	if ((p1).history.missionsaccomplished > (p2).history.missionsaccomplished){
		return 1;
	}
	else if ((p1).history.missionsaccomplished == (p2).history.missionsaccomplished){
		return 1;
	}
	return -1;
}

int HBKillCompare(const void* elem1, const void* elem2)
{
		if (((PPLAYER)elem1)->history.kills > ((PPLAYER)elem2)->history.kills){
		return 1;
	}
	else if (((PPLAYER)elem1)->history.kills == ((PPLAYER)elem2)->history.kills){
		return 1;
	}
	return -1;
}

int HBDamageCompare(const void* elem1, const void* elem2)
{
		if (((PPLAYER)elem1)->history.damageinmillions > ((PPLAYER)elem2)->history.damageinmillions){
		return 1;
	}
	else if (((PPLAYER)elem1)->history.damageinmillions == ((PPLAYER)elem2)->history.damageinmillions){
		return 1;
	}
	return -1;
}