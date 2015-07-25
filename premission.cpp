#include "agencyinclude.h"
#include "guns.h"

void GenerateMissionCode(PMISSION ms)
{
	int r;
	int a;
	int b;
	int c;
	char buf[10];
	//65-90 are the capital letters
	//A-9999-BC FORMAT
	//GO THRU A FEW ITERATIONS
	a = random(65,91);
	b = random(65,91);
	c = random(65,91);
	r = random(500, 4000);
	sprintf(buf, "%c-%4.4d-%c%c", a,r,b,c);
	strcpy(ms->missioncode, buf);
	//	printf("%d\n", phd->serial);
}

void GenerateMissionCode(char** codebuf)
{
	int r;
	int a;
	int b;
	int c;
	char buf[10];
	//65-90 are the capital letters
	//A-9999-BC FORMAT
	//GO THRU A FEW ITERATIONS
	a = random(65,91);
	b = random(65,91);
	c = random(65,91);
	r = random(500, 4000);
	sprintf(buf, "%c-%4.4d-%c%c", a,r,b,c);
	*codebuf = (char*) malloc(sizeof(char) * strlen(buf) + 1);
	strcpy(*codebuf, buf);
	//	printf("%d\n", phd->serial);
}



bool IsClearedForWeapon(PPLAYER player, PWEAPON weapon)
{
	if (!weapon->dsaweapon){
		return true; //ALL NON-DSA WEAPONS ARE A-OKAY.
	}
	
	switch(player->rank){
	case AGENT_RANK_FOR:
	case AGENT_RANK_FO:
		//A AND B WEAPONS ONLY.
		if(weapon->classtype > WEAP_CLASS_B){
			return false;
		}
		return true;
	case AGENT_RANK_CO:
	case AGENT_RANK_COS:
		if(weapon->classtype > WEAP_CLASS_S){
			return false;
		}
		return true;
	case AGENT_RANK_ECOS:
	case AGENT_RANK_ECOSSPECONE:
		return true;
	default:
		return true;
	}
}


void LevelIVPharmacy(PPLAYER player)
{
	//FILL THE THREE SYRINGES: ALPHA, BETA, GAMMA.
	char* DrugNames[4] = {"Oxymethyl Dichloride", "Neurophysical Corticadrenaline", "Remed-K Restructurizer", "Amyl Nitrate/Atropine Mixture"};
	char* DrugDesc[4] = {
		"This drug is used to sharpen the awareness and perception of the Agent who uses it. The user will inherit increased shooting and visual ability from the effects of the drug. The drug wears off as the body becomes accustomed to it.\n\nPlease Confirm Your Choice.",
			"This chemically synthesized adrenaline compound provides the same effects as natural adrenaline for the user. The user inherits increased reflexes and agression. The drug wears off as the body re-absorbs and breaks down the adrenaline.\n\nPlease Confirm Your Choice.",
			"Remed-K allows a wounded Agent to neutralize shock trauma from combat wounding in a relatively quick amount of time. Note that the injection of this compound does not heal the wounds, it merely 'tricks' the body into believing no trauma exists. This can lead to serious complications if overused.\n\nPlease Confirm Your Choice.",
			"This compound is a binary mixture of Atropine (Atropa Belladonna extract) and Amyl Nitrate. Useful as an antitoxin, this mixture is to be applied intravenously before entering a toxic area or during the effects of a toxin. Note that the compound is temporary and may not last the duration of the user's exposure to toxins.\n\nPlease Confirm Your Choice."
	};
	char* SyringeNames[MAX_SYRINGES] = {"Syringe Alpha", "Syringe Beta", "Syringe Gamma"};
	char Buffer[5000];
	int DBSchemes[MAX_SYRINGES] = {DB_SCHEME_COOLBLUELINES, DB_SCHEME_EMERGENCYREDLINES, DB_SCHEME_DIGITALGREENLINES};
	PLIST list;
	CreateDBStringList(&list, DrugNames, 4);
	int currentdrug;
	
	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 1, false);
	StaticDialogBox("Level IV", "Welcome to the Level IV Pharmaceutical Requisition Center. You, as a Level V Operative, have the authority to requisition three syringes worth of DSA-developed drug compounds. Please read the descriptions of each of the drug compounds carefully before use, as they may have side effects that you may be unaccustomed to.\nPress ENTER to Begin.", "Pharmacy", &globals.dboxparams);
	
	for(int i = 0; i < MAX_SYRINGES; i++)
	{
		
		while(1){
			SetGlobalDBoxParam(DBSchemes[i], DB_SMALL, 1, false);
			currentdrug = SimpleListDialogBox(SyringeNames[i], "Drug Selection", list, &globals.dboxparams, CreateStringDBListItem);
			SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_LARGE, 0, FALSE);
			if(YesNoDialogBox(DrugNames[currentdrug], DrugDesc[currentdrug], "(Y)es/(N)o", &globals.dboxparams)){
				sprintf(Buffer, "%s Has Been Loaded With %s.\nPress ENTER to Continue Selection.", SyringeNames[i], DrugNames[currentdrug]);
				player->drugs.drugtype[i] = currentdrug + 1;
				SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, true);
				//PLAY HISS SOUND HERE.
				playMIXsound(SYRINGE_FILL_SOUND);
				StaticDialogBox("Syringe", Buffer, "Loaded", &globals.dboxparams);
				break;
			}
		}
	}
	SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, true);
	StaticDialogBox("Syringes", "Your syringes have been loaded, and are now awaiting you at the Mobilization Center on Level II. You will recieve them prior to your operation. If you have already locked in your requisitions at the Weapon, Ammunition, and Technology Requisition Centers, you may proceed IMMEDIATELY to Level II for Mobilization. Best of Luck To You, Operative.\nPress ENTER to Continue", "Loaded", &globals.dboxparams);
	FreeDBStringList(list);
}
void LevelIVWeaponLockers(PPLAYER player)
{
/*
#define ARMORY_LEFTBOXXORIGIN	0
#define ARMORY_LEFTBOXYORIGIN	0
#define ARMORY_RIGHTBOXXORIGIN	40
#define ARMORY_RIGHTBOXYORIGIN	0
#define ARMORY_TOPBOXESWIDTH	39
#define ARMORY_TOPBOXESHEIGHT	10
#define ARMORY_WEIGHTBOXXORIGIN	0
#define ARMORY_WEIGHTBOXYORIGIN	11
#define ARMORY_WEIGHTBOXWIDTH	80
#define ARMORY_WEIGHTBOXHEIGHT	1
#define ARMORY_DESCBOXXORIGIN	0
#define ARMORY_DESCBOXYORIGIN	15
#define ARMORY_DESCBOXWIDTH		80
#define ARMORY_DESCBOXHEIGHT	9
	*/
	char* LeftText = "LEVEL IV";
	char* RightText = "SYSTEM BOOTING...";
	char* RightOutText = "NO WEAPONS LOADED";
	char* OutOfStockText = "WEAPON CURRENTLY OUT OF STOCK";
	char* ClearanceText = "HIGHER CLEARANCE LEVEL REQUIRED TO ACCESS THIS WEAPON";
	char* ArmoryStock = "DSA ARMORY STOCK";
	char* PersonalStock = "PERSONAL STOCK";
	char* HelpText = "TAB To Switch Between Armory and Personal Stock\nUP/DOWN or PAGEUP/PAGEDOWN To Change Selection\nENTER to Add/Remove Stock\n[ and ] To Scroll Description\nESC To Exit Armory\nPRESS ENTER TO ACTIVATE SYSTEM.";
	RECT leftboxrect;
	leftboxrect.top = ARMORY_LEFTBOXYORIGIN + 1;
	leftboxrect.bottom = leftboxrect.top + ARMORY_TOPBOXESHEIGHT - 1;
	leftboxrect.left = ARMORY_LEFTBOXXORIGIN;
	leftboxrect.right = leftboxrect.left + ARMORY_TOPBOXESWIDTH;
	
	RECT rightboxrect;
	rightboxrect.top = ARMORY_RIGHTBOXYORIGIN + 1;
	rightboxrect.bottom = rightboxrect.top + ARMORY_TOPBOXESHEIGHT - 1;
	rightboxrect.left = ARMORY_RIGHTBOXXORIGIN;
	rightboxrect.right = rightboxrect.left + ARMORY_TOPBOXESWIDTH;
	
	RECT weightboxrect;
	weightboxrect.top = ARMORY_WEIGHTBOXYORIGIN;
	weightboxrect.bottom = weightboxrect.top + ARMORY_WEIGHTBOXHEIGHT;
	weightboxrect.left = ARMORY_WEIGHTBOXXORIGIN;
	weightboxrect.right = weightboxrect.left + ARMORY_WEIGHTBOXWIDTH;
	
	RECT descboxrect;
	descboxrect.top = ARMORY_DESCBOXYORIGIN - 1 ;
	descboxrect.bottom = descboxrect.top + ARMORY_DESCBOXHEIGHT;
	descboxrect.left = ARMORY_DESCBOXXORIGIN;
	descboxrect.right = descboxrect.left + ARMORY_DESCBOXWIDTH;
	
	PWEAPON CurrentWeapon = NULL;
	PPOTYPE CurrentObject = NULL;
	POTYPE TempObject;
	int CurrentLeftSelectionNumber = 0;
	int CurrentLeftLocalSelection = 0;
	int CurrentLeftScrollOffset = 0;
	int CurrentRightLocalSelection = 0;
	int CurrentRightSelectionNumber = 0;
	int CurrentRightScrollOffset = 0;
	int MaxTopItemsToDisplay = leftboxrect.bottom - leftboxrect.top;
	bool RedrawDescription = true;
	bool RedrawArmoryList = true;
	bool RedrawPersonalList = true;
	bool RedrawWeight = true;
	bool CursorOnArmory = true;
	bool ResetDescriptionPages = true;
	int quicksleep = 100;
	int DescPages = 1;
	char ExtendedDescription[50000];
	char ArmoryReport[5000];
	int CurrentDescPage = 0;
	int longsleep = 500;
	int ReturnCode;
	char WeightString[500];
	bool Continuing = true;
	USHORT key;
	//char WeapString[50];
	POINT currentdrawpoint;
	
	if(!player->p_weapons){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_SMALL, 0, TRUE);
		StaticDialogBox("Error", "Player Weapon List Uninitialized!", "Error", &globals.dboxparams);
		return;
	}
	
	
	GREEN;
	printgraphic(globals.graphicslist, globals.zeroed, ARMORY_GRAPHIC_ID);
	
	//LEFT TEXT
	currentdrawpoint = ReturnTextCenterInRect(LeftText, leftboxrect);
	RED;
	setcursor(currentdrawpoint.x, currentdrawpoint.y);
	writestring(LeftText);
	Sleep(quicksleep);
	LRED;
	setcursor(currentdrawpoint.x, currentdrawpoint.y);
	writestring(LeftText);
	Sleep(longsleep);
	
	//RIGHT TEXT
	currentdrawpoint = ReturnTextCenterInRect(RightText, rightboxrect);
	RED;
	setcursor(currentdrawpoint.x, currentdrawpoint.y);
	writestring(RightText);
	Sleep(quicksleep);
	LRED;
	setcursor(currentdrawpoint.x, currentdrawpoint.y);
	writestring(RightText);
	Sleep(longsleep);
	
	COORD descstartcoord;
	descstartcoord.X = (short)descboxrect.left;
	descstartcoord.Y = (short)descboxrect.top;
	RED;
	printinrect(HelpText, descstartcoord);
	Sleep(quicksleep);
	LRED;
	printinrect(HelpText, descstartcoord);
	
	waitforkey();
	ClearRect(leftboxrect);
	ClearRect(rightboxrect);
	ClearRect(descboxrect);
	ClearRect(weightboxrect);
	
	currentdrawpoint = ReturnTextCenterInRect(ArmoryStock, leftboxrect);
	currentdrawpoint.y = leftboxrect.top - 1;
	WHITE;
	setcursor(currentdrawpoint.x, currentdrawpoint.y);
	writestring(ArmoryStock);
	
	currentdrawpoint = ReturnTextCenterInRect(PersonalStock, rightboxrect);
	currentdrawpoint.y = rightboxrect.top - 1;
	WHITE;
	setcursor(currentdrawpoint.x, currentdrawpoint.y);
	writestring(PersonalStock);
	
	//BEGIN SEQUENCE!
	while(Continuing)
	{
		//PRINT THE PLAYER'S WEAPONS ON THE RIGHT SIDE FIRST.
		if(RedrawPersonalList)
		{
			ClearRect(rightboxrect);
			if(player->p_weapons->objcnt < 1){
				currentdrawpoint = ReturnTextCenterInRect(RightOutText, rightboxrect);
				LRED;
				setcursor(currentdrawpoint.x, currentdrawpoint.y);
				writestring(RightOutText);
			}
			else{
				
				ArmoryDrawWeaponList(player, player->p_weapons, rightboxrect, CurrentRightLocalSelection, CurrentRightScrollOffset, !CursorOnArmory, true);
			}
			RedrawPersonalList = false;
		}
		if(RedrawArmoryList)
		{
			ClearRect(leftboxrect);
			ArmoryDrawWeaponList(player, globals.globalweaponsammo.weaponlist, leftboxrect, CurrentLeftLocalSelection, CurrentLeftScrollOffset, CursorOnArmory, false);
			RedrawArmoryList = false;
		}
		if(RedrawWeight)
		{
			player->weight = (float)fabs(player->weight);
			ClearRect(weightboxrect);
			sprintf(WeightString, "Current Weight: %2.2f lbs - MAX WEIGHT: %2.2f lbs", player->weight, MAX_PLAYER_WEIGHT);
			currentdrawpoint = ReturnTextCenterInRect(WeightString,weightboxrect);
			LBLUE;
			setcursor(currentdrawpoint.x, currentdrawpoint.y);
			printf("Current Weight: ");
			if(player->weight > (3.0 * MAX_PLAYER_WEIGHT) / 4.0){
				LRED;
			}
			else if (player->weight > MAX_PLAYER_WEIGHT / 2.0){
				LYELLOW;
			}
			else{
				LGREEN;
			}
			printf("%2.2f lbs", player->weight);
			LBLUE;
			printf(" - MAX WEIGHT: ");
			LRED;
			printf("%2.2f lbs", MAX_PLAYER_WEIGHT);
			RedrawWeight = false;
		}
		if(RedrawDescription)
		{
			if(CursorOnArmory){
				TempObject.weapon.id = CurrentLeftSelectionNumber + 1;
				//TempObject.weapon.id = 7;
				CurrentObject= findObject(globals.globalweaponsammo.weaponlist, &TempObject, CompareWeaponID);
			}
			else{
				TempObject.weapon.index = CurrentRightSelectionNumber;
				CurrentObject = findObject(player->p_weapons, &TempObject, CompareWeaponIndex);
			}
			
			if(!CurrentObject){
				CurrentWeapon = NULL;
			}
			else{
				CurrentWeapon = &CurrentObject->weapon;
			}
			
			if(ResetDescriptionPages)
			{
				CurrentDescPage = 0;
				ResetDescriptionPages = false;
			}
			
			ClearRect(descboxrect);
			if(!IsClearedForWeapon(player, CurrentWeapon))
			{
				currentdrawpoint = ReturnTextCenterInRect(ClearanceText, descboxrect);
				LRED;
				setcursor(currentdrawpoint.x, currentdrawpoint.y);
				writestring(ClearanceText);
			}
			
			else{
				
				ArmoryCreateExtendedDescription(CurrentWeapon, ExtendedDescription);
				LGREEN;
				DescPages = ArmoryDrawDescription(ExtendedDescription, descboxrect, CurrentDescPage);
				if(CurrentDescPage + 1 > DescPages){
					ResetDescriptionPages = true;
				}
				
				//CHECK IF WEAPON IS OUT OF STOCK. IF SO, SUPERIMPOSE THE "OUT OF STOCK" THING.
				if(!CurrentWeapon->weaponavailable){
					currentdrawpoint = ReturnTextCenterInRect(OutOfStockText, descboxrect);
					LRED;
					setcursor(currentdrawpoint.x, currentdrawpoint.y);
					writestring(OutOfStockText);
				}
			}
			RedrawDescription = false;
			
			
		}
		key = waitforVkey();
		switch(key)
		{
		case 'A': //ARMORY REPORT
			if(CursorOnArmory)
			{
				TempObject.weapon.id = CurrentLeftSelectionNumber + 1;
				CurrentObject= findObject(globals.globalweaponsammo.weaponlist, &TempObject, CompareWeaponID);
				//IF OUT, IF NOT CLEARED,DON'T GIVE IT UP.
				if(!CurrentObject || !CurrentObject->weapon.weaponavailable || !IsClearedForWeapon(player, &CurrentObject->weapon)){
					SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
					StaticDialogBox("DSA Data", "This DSA Report References A Weapon That Is Either Out Of Stock or Classified. Report Unavailable.", "Unavailable", &globals.dboxparams);
				}
				else{
					ArmoryCreateAmmoDesc(ArmoryReport, &CurrentObject->weapon);
					SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_LARGE, 1, FALSE);
					StaticDialogBox(CurrentObject->weapon.longname, ArmoryReport, "Ammunition Report", &globals.dboxparams);
				}
			}
			else{
				TempObject.weapon.index = CurrentRightSelectionNumber;
				CurrentObject = findObject(player->p_weapons, &TempObject, CompareWeaponIndex);
				if(CurrentObject){
					ArmoryCreateAmmoDesc(ArmoryReport, &CurrentObject->weapon);
					SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_LARGE, 1, FALSE);
					StaticDialogBox(CurrentObject->weapon.longname, ArmoryReport, "Ammunition Report", &globals.dboxparams);
				}
			}
			break;
		case VK_ESCAPE:
			Continuing = false;
			break;
		case 219:
			RedrawDescription = true;
			CurrentDescPage--;
			CurrentDescPage = CapInclusive(CurrentDescPage, 0, DescPages - 1);
			break;
		case 221:
			RedrawDescription = true;
			CurrentDescPage++;
			CurrentDescPage = CapInclusive(CurrentDescPage, 0, DescPages - 1);
			break;
		case 33:
			if(CursorOnArmory){
				if(CurrentLeftScrollOffset < 1){
					CurrentLeftScrollOffset = CurrentLeftSelectionNumber = 0;
					CurrentLeftLocalSelection = 0;
					CurrentLeftSelectionNumber = 0;
				}
				else{
					CurrentLeftScrollOffset -= MaxTopItemsToDisplay;
					CurrentLeftSelectionNumber -= MaxTopItemsToDisplay;
					CurrentLeftScrollOffset = CapInclusive(CurrentLeftScrollOffset, 0, globals.globalweaponsammo.weaponlist->objcnt - 1);
					CurrentLeftSelectionNumber = CapInclusive(CurrentLeftSelectionNumber, 0, globals.globalweaponsammo.weaponlist->objcnt - 1);
				}
				RedrawArmoryList = true;
			}
			else{
				if(CurrentRightScrollOffset < 1){
					CurrentRightScrollOffset = CurrentRightSelectionNumber = 0;
					CurrentRightLocalSelection = 0;
					CurrentRightSelectionNumber = 0;
				}
				else{
					CurrentRightScrollOffset -= MaxTopItemsToDisplay;
					CurrentRightSelectionNumber -= MaxTopItemsToDisplay;
					CurrentRightScrollOffset = CapInclusive(CurrentRightScrollOffset, 0, player->p_weapons->objcnt - 1);
					CurrentRightSelectionNumber = CapInclusive(CurrentRightSelectionNumber, 0, player->p_weapons->objcnt - 1);
				}
				RedrawPersonalList = true;
			}
			RedrawDescription = true;
			ResetDescriptionPages = true;
			break;
		case 34:
			//ONE PAGE DOWN. WILL ATTEMPT TO MOVE THE SCROLL OFFSET BY THE MAX ITEMS IN THE BOX. IF IT CANNOT, THEN IT WILL MOVE
			//THE SCROLL OFFSET AS FAR AS IT CAN, AND WILL DROP THE LOCALOFFSET TO THE REMAINDER OF THE DISTANCE.
			if(CursorOnArmory){
				if(CurrentLeftScrollOffset + MaxTopItemsToDisplay >= globals.globalweaponsammo.weaponlist->objcnt - MaxTopItemsToDisplay){
					int cap = (MaxTopItemsToDisplay > globals.globalweaponsammo.weaponlist->objcnt ? globals.globalweaponsammo.weaponlist->objcnt : MaxTopItemsToDisplay);
					if(MaxTopItemsToDisplay > globals.globalweaponsammo.weaponlist->objcnt){
					}
					else{
						CurrentLeftScrollOffset = CurrentLeftSelectionNumber = globals.globalweaponsammo.weaponlist->objcnt - MaxTopItemsToDisplay;
					}
					CurrentLeftLocalSelection = cap - 1;
					CurrentLeftSelectionNumber += cap - 1;
					CurrentLeftSelectionNumber = CapInclusive(CurrentLeftSelectionNumber, 0, globals.globalweaponsammo.weaponlist->objcnt - 1);
				}
				else{
					
					CurrentLeftScrollOffset += MaxTopItemsToDisplay;
					CurrentLeftSelectionNumber += MaxTopItemsToDisplay;
					CurrentLeftSelectionNumber = CapInclusive(CurrentLeftSelectionNumber, 0, globals.globalweaponsammo.weaponlist->objcnt - 1);
				}
				RedrawArmoryList = true;
			}
			else{
				if(CurrentRightScrollOffset + MaxTopItemsToDisplay >= player->p_weapons->objcnt - MaxTopItemsToDisplay){
					int cap = (MaxTopItemsToDisplay > player->p_weapons->objcnt ? player->p_weapons->objcnt : MaxTopItemsToDisplay);
					if(MaxTopItemsToDisplay > player->p_weapons->objcnt){
					}
					else{
						CurrentRightScrollOffset = CurrentRightSelectionNumber = player->p_weapons->objcnt - MaxTopItemsToDisplay;
					}
					CurrentRightLocalSelection = cap - 1;
					CurrentRightSelectionNumber += cap - 1;
					CurrentRightSelectionNumber = CapInclusive(CurrentRightSelectionNumber, 0, player->p_weapons->objcnt - 1);
				}
				else{
					
					CurrentRightScrollOffset += MaxTopItemsToDisplay;
					CurrentRightSelectionNumber += MaxTopItemsToDisplay;
					CurrentRightSelectionNumber = CapInclusive(CurrentRightSelectionNumber, 0, player->p_weapons->objcnt - 1);
				}
				RedrawPersonalList = true;
			}
			RedrawDescription = true;
			ResetDescriptionPages = true;
			
			break;
		case VK_UP:
			if(CursorOnArmory)
			{
				
				CurrentLeftSelectionNumber--;
				CurrentLeftSelectionNumber = CapInclusive(CurrentLeftSelectionNumber, 0, globals.globalweaponsammo.weaponlist->objcnt - 1);
				//NOW CHECK IF WE NEED TO SCREW WITH THE SCROLL OFFSET.
				if(CurrentLeftLocalSelection > 0){
					CurrentLeftLocalSelection--;
				}
				else if (CurrentLeftScrollOffset > 0){ 
					CurrentLeftScrollOffset--;
				}
				RedrawArmoryList = true;
			}
			else{
				if(player->p_weapons->objcnt < 1){
					break;
				}
				
				CurrentRightSelectionNumber--;
				CurrentRightSelectionNumber = CapInclusive(CurrentRightSelectionNumber, 0, player->p_weapons->objcnt - 1);
				//NOW CHECK IF WE NEED TO SCREW WITH THE SCROLL OFFSET.
				if(CurrentRightLocalSelection > 0){
					CurrentRightLocalSelection--;
				}
				else if (CurrentRightScrollOffset > 0){ 
					CurrentRightScrollOffset--;
				}
				RedrawPersonalList = true;
			}
			RedrawDescription = true;
			ResetDescriptionPages = true;
			break;
		case VK_DOWN:
			if(CursorOnArmory)
			{
				
				CurrentLeftSelectionNumber++;
				CurrentLeftSelectionNumber = CapInclusive(CurrentLeftSelectionNumber, 0, globals.globalweaponsammo.weaponlist->objcnt - 1);
				//NOW CHECK IF WE NEED TO SCREW WITH THE SCROLL OFFSET.
				if(CurrentLeftLocalSelection < MaxTopItemsToDisplay - 1 && CurrentLeftLocalSelection < globals.globalweaponsammo.weaponlist->objcnt){
					CurrentLeftLocalSelection++;
				}
				else if (CurrentLeftScrollOffset < (globals.globalweaponsammo.weaponlist->objcnt) - MaxTopItemsToDisplay){ 
					CurrentLeftScrollOffset++;
				}
				RedrawArmoryList = true;
			}
			else{
				if(player->p_weapons->objcnt < 1){
					break;
				}
				
				CurrentRightSelectionNumber++;
				CurrentRightSelectionNumber = CapInclusive(CurrentRightSelectionNumber, 0, player->p_weapons->objcnt - 1);
				//NOW CHECK IF WE NEED TO SCREW WITH THE SCROLL OFFSET.
				if(CurrentRightLocalSelection < MaxTopItemsToDisplay - 1 && CurrentRightLocalSelection < player->p_weapons->objcnt - 1){
					CurrentRightLocalSelection++;
				}
				else if (CurrentRightScrollOffset < player->p_weapons->objcnt - MaxTopItemsToDisplay){ 
					CurrentRightScrollOffset++;
				}
				RedrawPersonalList = true;
			}
			RedrawDescription = true;
			ResetDescriptionPages = true;
			break;
		case VK_TAB:
			if(CursorOnArmory)
			{
				if(player->p_weapons->objcnt > 0){
					playMIXsound(ARMORY_SOUND_SELECT);
					CursorOnArmory = false;
					RedrawArmoryList = true;
					RedrawPersonalList = true;
					RedrawDescription = true;
					ResetDescriptionPages = true;
				}
			}
			else{
				playMIXsound(ARMORY_SOUND_SELECT);
				CursorOnArmory = true;
				RedrawArmoryList = true;
				RedrawPersonalList = true;
				RedrawDescription = true;
				ResetDescriptionPages = true;
			}
			break;
		case VK_RETURN: //THE BIG ONE!
			if(CursorOnArmory){ //THEN WE GIVE TO THE PLAYER.
				//FIND OUT INFO ABOUT THE WEAPON.
				TempObject.weapon.id = CurrentLeftSelectionNumber + 1;
				CurrentObject= findObject(globals.globalweaponsammo.weaponlist, &TempObject, CompareWeaponID);
				//IF OUT, IF NOT CLEARED, OR IF TOO HEAVY, DON'T GIVE IT UP.
				if(!CurrentObject->weapon.weaponavailable || !IsClearedForWeapon(player, &CurrentObject->weapon) || player->weight + CurrentObject->weapon.weight > MAX_PLAYER_WEIGHT){
				}
				else{
					ReturnCode = GiveTrainingWeapon(player, CurrentLeftSelectionNumber + 1);
					if(ReturnCode != GIVEARMORYWEAPON_OK){
						SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_SMALL, 1, TRUE);
						StaticDialogBox("Error", "The Weapon Is Not Available.", "Error", &globals.dboxparams);
					}
					else{
						//FIND OUT WHETHER OR NOT THEY WANT EXTRAS.
						//FIND THE WEAPON IN THE PLAYER LIST AGAIN.
						TempObject.weapon.index = player->p_weapons->objcnt - 1;
						CurrentObject = findObject(player->p_weapons, &TempObject, CompareWeaponIndex);
						LevelIVSelectWeaponExtras(&CurrentObject->weapon);
						playMIXsound(ARMORY_SOUND_REMOVEWEAPON);
						player->weight += CurrentObject->weapon.weight;
						RedrawWeight = true;
						RedrawPersonalList = true;
					}
				}
			}
			else{ //THEN WE TAKE AWAY FROM THE PLAYER.
				TempObject.weapon.index = CurrentRightSelectionNumber;
				CurrentObject = findObject(player->p_weapons, &TempObject, CompareWeaponIndex);
				if(CurrentObject){
					if(CurrentRightSelectionNumber == player->p_weapons->objcnt - 1)
					{
						CurrentRightSelectionNumber--;
						CurrentRightLocalSelection--;
					}
					player->weight -= CurrentObject->weapon.weight;
					playMIXsound(ARMORY_SOUND_GETWEAPON);
					if(DoesAnyAmmunitionExistForPlayerWeapon(player, TempObject.weapon.index) && !IsAmmunitionUsedForMoreThanOnePlayerWeapon(player, TempObject.weapon.index))
					{
						PurgeAllAmmunitionForWeaponIndex(player, TempObject.weapon.index);
						SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
						StaticDialogBox("Ammunition Purged", "Our records indicate that ammunition associated with this weapon is also no longer needed. It has been removed.", "Press ENTER", &globals.dboxparams);
					}
					deleteObject(player->p_weapons, CurrentObject);
					ReorderWeaponListIndex(player->p_weapons);
					RedrawPersonalList = true;
					RedrawWeight = true;
					RedrawDescription = true;
					ResetDescriptionPages = true;
					if(player->p_weapons->objcnt < 1){
						CurrentRightSelectionNumber = 0;
						CurrentRightScrollOffset = 0;
						CurrentRightLocalSelection = 0;
						CursorOnArmory = true;
						RedrawArmoryList = true;
						
					}
				}
			}
			break;
		}
	}
	SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, true);
	StaticDialogBox("Weapon Order", "This concludes your weapon order. Your weapon choices have been approved and are currently awaiting you at the Mobilization Center on Level II. You will recieve them prior to your operation. Please proceed directly to the Ammunition Requisition Center to order ammunition for your weaponry.\nPress ENTER to Continue.", "Complete", &globals.dboxparams);
	cls(0);
	return;
}

void LevelIVAmmunitionLockers(PPLAYER player)
{
/*
#define ARMORY_LEFTBOXXORIGIN	0
#define ARMORY_LEFTBOXYORIGIN	0
#define ARMORY_RIGHTBOXXORIGIN	40
#define ARMORY_RIGHTBOXYORIGIN	0
#define ARMORY_TOPBOXESWIDTH	39
#define ARMORY_TOPBOXESHEIGHT	10
#define ARMORY_WEIGHTBOXXORIGIN	0
#define ARMORY_WEIGHTBOXYORIGIN	11
#define ARMORY_WEIGHTBOXWIDTH	80
#define ARMORY_WEIGHTBOXHEIGHT	1
#define ARMORY_DESCBOXXORIGIN	0
#define ARMORY_DESCBOXYORIGIN	15
#define ARMORY_DESCBOXWIDTH		80
#define ARMORY_DESCBOXHEIGHT	9
	*/
	char* LeftText = "LEVEL IV";
	char* RightText = "SYSTEM BOOTING...";
	char* RightOutText = "NO AMMUNITION SELECTED";
	char* OutOfStockText = "NO AMMUNITION REQUIRED";
	char* ClearanceText = "HIGHER CLEARANCE LEVEL REQUIRED TO ACCESS THIS WEAPON";
	char* ArmoryStock = "CURRENT WEAPONS TO LOAD";
	char* PersonalStock = "AMMUNITION FOR WEAPON";
	char* HelpText = "TAB To Switch Between Weapons and Ammunition Stock\nUP/DOWN or PAGEUP/PAGEDOWN To Change Selection\nENTER to Add/Remove Ammunition\n[ and ] To Scroll Description\nESC To Exit Armory\nPRESS ENTER TO ACTIVATE SYSTEM.";
	RECT leftboxrect;
	leftboxrect.top = ARMORY_LEFTBOXYORIGIN + 1;
	leftboxrect.bottom = leftboxrect.top + ARMORY_TOPBOXESHEIGHT - 1;
	leftboxrect.left = ARMORY_LEFTBOXXORIGIN;
	leftboxrect.right = leftboxrect.left + ARMORY_TOPBOXESWIDTH;
	
	RECT rightboxrect;
	rightboxrect.top = ARMORY_RIGHTBOXYORIGIN + 1;
	rightboxrect.bottom = rightboxrect.top + ARMORY_TOPBOXESHEIGHT - 1;
	rightboxrect.left = ARMORY_RIGHTBOXXORIGIN;
	rightboxrect.right = rightboxrect.left + ARMORY_TOPBOXESWIDTH;
	
	RECT weightboxrect;
	weightboxrect.top = ARMORY_WEIGHTBOXYORIGIN;
	weightboxrect.bottom = weightboxrect.top + ARMORY_WEIGHTBOXHEIGHT;
	weightboxrect.left = ARMORY_WEIGHTBOXXORIGIN;
	weightboxrect.right = weightboxrect.left + ARMORY_WEIGHTBOXWIDTH;
	
	RECT descboxrect;
	descboxrect.top = ARMORY_DESCBOXYORIGIN - 1 ;
	descboxrect.bottom = descboxrect.top + ARMORY_DESCBOXHEIGHT;
	descboxrect.left = ARMORY_DESCBOXXORIGIN;
	descboxrect.right = descboxrect.left + ARMORY_DESCBOXWIDTH;
	
	PWEAPON CurrentWeapon = NULL;
	PPOTYPE CurrentObject = NULL;
	POTYPE TempObject;
	int CurrentLeftSelectionNumber = 0;
	int CurrentLeftLocalSelection = 0;
	int CurrentLeftScrollOffset = 0;
	int CurrentRightLocalSelection = 0;
	int CurrentRightSelectionNumber = 0;
	int CurrentRightScrollOffset = 0;
	int MaxTopItemsToDisplay = leftboxrect.bottom - leftboxrect.top;
	bool RedrawDescription = true;
	bool RedrawArmoryList = true;
	bool RedrawPersonalList = true;
	bool RedrawWeight = true;
	bool CursorOnArmory = true;
	bool ResetDescriptionPages = true;
	int quicksleep = 100;
	int DescPages = 1;
	int TotalItems;
	char ExtendedDescription[50000];
//	char ArmoryReport[5000];
	int CurrentDescPage = 0;
	int longsleep = 500;
//	int ReturnCode;
	char WeightString[500];
	bool Continuing = true;
	USHORT key;
	//char WeapString[50];
	POINT currentdrawpoint;
	
	if(!player->p_weapons){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_SMALL, 0, TRUE);
		StaticDialogBox("Error", "Player Weapon List Uninitialized!", "Error", &globals.dboxparams);
		return;
	}
	
	
	RED;
	printgraphic(globals.graphicslist, globals.zeroed, ARMORY_GRAPHIC_ID);
	
	//LEFT TEXT
	currentdrawpoint = ReturnTextCenterInRect(LeftText, leftboxrect);
	RED;
	setcursor(currentdrawpoint.x, currentdrawpoint.y);
	writestring(LeftText);
	Sleep(quicksleep);
	LRED;
	setcursor(currentdrawpoint.x, currentdrawpoint.y);
	writestring(LeftText);
	Sleep(longsleep);
	
	//RIGHT TEXT
	currentdrawpoint = ReturnTextCenterInRect(RightText, rightboxrect);
	RED;
	setcursor(currentdrawpoint.x, currentdrawpoint.y);
	writestring(RightText);
	Sleep(quicksleep);
	LRED;
	setcursor(currentdrawpoint.x, currentdrawpoint.y);
	writestring(RightText);
	Sleep(longsleep);
	
	COORD descstartcoord;
	descstartcoord.X = (short)descboxrect.left;
	descstartcoord.Y = (short)descboxrect.top;
	RED;
	printinrect(HelpText, descstartcoord);
	Sleep(quicksleep);
	LRED;
	printinrect(HelpText, descstartcoord);
	
	waitforkey();
	ClearRect(leftboxrect);
	ClearRect(rightboxrect);
	ClearRect(descboxrect);
	ClearRect(weightboxrect);
	
	currentdrawpoint = ReturnTextCenterInRect(ArmoryStock, leftboxrect);
	currentdrawpoint.y = leftboxrect.top - 1;
	WHITE;
	setcursor(currentdrawpoint.x, currentdrawpoint.y);
	writestring(ArmoryStock);
	
	currentdrawpoint = ReturnTextCenterInRect(PersonalStock, rightboxrect);
	currentdrawpoint.y = rightboxrect.top - 1;
	WHITE;
	setcursor(currentdrawpoint.x, currentdrawpoint.y);
	writestring(PersonalStock);
	
	//BEGIN SEQUENCE!
	while(Continuing)
	{
		//PRINT THE PLAYER'S WEAPONS ON THE RIGHT SIDE FIRST.
		if(RedrawPersonalList)
		{
			ClearRect(rightboxrect);
			if(player->p_weapons->objcnt < 1){
				currentdrawpoint = ReturnTextCenterInRect(RightOutText, rightboxrect);
				LRED;
				setcursor(currentdrawpoint.x, currentdrawpoint.y);
				writestring(RightOutText);
			}
			else{	
				ArmoryAmmunitionDrawAmmunitionList(player, rightboxrect, CurrentLeftSelectionNumber, CurrentRightLocalSelection, CurrentRightScrollOffset, !CursorOnArmory);
			}
			RedrawPersonalList = false;
		}
		if(RedrawArmoryList)
		{
			ClearRect(leftboxrect);
			ArmoryAmmunitionDrawWeaponList(player, leftboxrect, CurrentLeftLocalSelection, CurrentLeftScrollOffset, CursorOnArmory);
			RedrawArmoryList = false;
		}
		if(RedrawWeight)
		{
			player->weight = (float)fabs(player->weight);
			ClearRect(weightboxrect);
			sprintf(WeightString, "Current Weight: %2.2f lbs - MAX WEIGHT: %2.2f lbs", player->weight, MAX_PLAYER_WEIGHT);
			currentdrawpoint = ReturnTextCenterInRect(WeightString,weightboxrect);
			LBLUE;
			setcursor(currentdrawpoint.x, currentdrawpoint.y);
			printf("Current Weight: ");
			if(player->weight > (3.0 * MAX_PLAYER_WEIGHT) / 4.0){
				LRED;
			}
			else if (player->weight > MAX_PLAYER_WEIGHT / 2.0){
				LYELLOW;
			}
			else{
				LGREEN;
			}
			printf("%2.2f lbs", player->weight);
			LBLUE;
			printf(" - MAX WEIGHT: ");
			LRED;
			printf("%2.2f lbs", MAX_PLAYER_WEIGHT);
			RedrawWeight = false;
		}
		if(RedrawDescription)
		{
		
			TempObject.weapon.index = CurrentLeftSelectionNumber;
			//TempObject.weapon.id = 7;
			CurrentObject= findObject(player->p_weapons, &TempObject, CompareWeaponIndex);
						
			if(!CurrentObject){
				CurrentWeapon = NULL;
			}
			else{
				CurrentWeapon = &CurrentObject->weapon;
			}
			
			if(ResetDescriptionPages)
			{
				CurrentDescPage = 0;
				ResetDescriptionPages = false;
			}
			
			ClearRect(descboxrect);
			if(!IsClearedForWeapon(player, CurrentWeapon))
			{
				currentdrawpoint = ReturnTextCenterInRect(ClearanceText, descboxrect);
				LRED;
				setcursor(currentdrawpoint.x, currentdrawpoint.y);
				writestring(ClearanceText);
			}
			
			else{
				
				ArmoryCreateExtendedDescription(CurrentWeapon, ExtendedDescription);
				LGREEN;
				DescPages = ArmoryDrawDescription(ExtendedDescription, descboxrect, CurrentDescPage);
				if(CurrentDescPage + 1 > DescPages){
					ResetDescriptionPages = true;
				}
			}
			RedrawDescription = false;
			
			
		}
		key = waitforVkey();
		switch(key)
		{
		case VK_ESCAPE:
			Continuing = false;
			break;
		case 219:
			RedrawDescription = true;
			CurrentDescPage--;
			CurrentDescPage = CapInclusive(CurrentDescPage, 0, DescPages - 1);
			break;
		case 221:
			RedrawDescription = true;
			CurrentDescPage++;
			CurrentDescPage = CapInclusive(CurrentDescPage, 0, DescPages - 1);
			break;
		case 33:
			if(CursorOnArmory){
				if(CurrentLeftScrollOffset < 1){
					CurrentLeftScrollOffset = CurrentLeftSelectionNumber = 0;
					CurrentLeftLocalSelection = 0;
					CurrentLeftSelectionNumber = 0;
				}
				else{
					CurrentLeftScrollOffset -= MaxTopItemsToDisplay;
					CurrentLeftSelectionNumber -= MaxTopItemsToDisplay;
					CurrentLeftScrollOffset = CapInclusive(CurrentLeftScrollOffset, 0, player->p_weapons->objcnt - 1);
					CurrentLeftSelectionNumber = CapInclusive(CurrentLeftSelectionNumber, 0, player->p_weapons->objcnt - 1);
				}
				RedrawArmoryList = true;
			}
			else{
				if(CurrentRightScrollOffset < 1){
					CurrentRightScrollOffset = CurrentRightSelectionNumber = 0;
					CurrentRightLocalSelection = 0;
					CurrentRightSelectionNumber = 0;
				}
				else{
					CurrentRightScrollOffset -= MaxTopItemsToDisplay;
					CurrentRightSelectionNumber -= MaxTopItemsToDisplay;
					CurrentRightScrollOffset = CapInclusive(CurrentRightScrollOffset, 0, GetNumberOfAmmoUnitsForPlayerWeapon(player, CurrentLeftSelectionNumber) - 1);
					CurrentRightSelectionNumber = CapInclusive(CurrentRightSelectionNumber, 0, GetNumberOfAmmoUnitsForPlayerWeapon(player, CurrentLeftSelectionNumber) - 1);
				}
				RedrawPersonalList = true;
			}
			RedrawDescription = true;
			ResetDescriptionPages = true;
			break;
		case 34:
			//ONE PAGE DOWN. WILL ATTEMPT TO MOVE THE SCROLL OFFSET BY THE MAX ITEMS IN THE BOX. IF IT CANNOT, THEN IT WILL MOVE
			//THE SCROLL OFFSET AS FAR AS IT CAN, AND WILL DROP THE LOCALOFFSET TO THE REMAINDER OF THE DISTANCE.
			if(CursorOnArmory){
				if(CurrentLeftScrollOffset + MaxTopItemsToDisplay >= player->p_weapons->objcnt - MaxTopItemsToDisplay){
					int cap = (MaxTopItemsToDisplay > player->p_weapons->objcnt ? player->p_weapons->objcnt : MaxTopItemsToDisplay);
					if(MaxTopItemsToDisplay > player->p_weapons->objcnt){
					}
					else{
						CurrentLeftScrollOffset = CurrentLeftSelectionNumber = player->p_weapons->objcnt - MaxTopItemsToDisplay;
					}
					CurrentLeftLocalSelection = cap - 1;
					CurrentLeftSelectionNumber += cap - 1;
					CurrentLeftSelectionNumber = CapInclusive(CurrentLeftSelectionNumber, 0, player->p_weapons->objcnt - 1);
				}
				else{
					
					CurrentLeftScrollOffset += MaxTopItemsToDisplay;
					CurrentLeftSelectionNumber += MaxTopItemsToDisplay;
					CurrentLeftSelectionNumber = CapInclusive(CurrentLeftSelectionNumber, 0, player->p_weapons->objcnt - 1);
				}
				RedrawArmoryList = true;
			}
			else{
				TotalItems = GetNumberOfAmmoUnitsForPlayerWeapon(player, CurrentLeftSelectionNumber);
				if(CurrentRightScrollOffset + MaxTopItemsToDisplay >= TotalItems - MaxTopItemsToDisplay){
					int cap = (MaxTopItemsToDisplay > TotalItems ? TotalItems : MaxTopItemsToDisplay);
					if(MaxTopItemsToDisplay > TotalItems){
					}
					else{
						CurrentRightScrollOffset = CurrentRightSelectionNumber = TotalItems - MaxTopItemsToDisplay;
					}
					CurrentRightLocalSelection = cap - 1;
					CurrentRightSelectionNumber += cap - 1;
					CurrentRightSelectionNumber = CapInclusive(CurrentRightSelectionNumber, 0, TotalItems - 1);
				}
				else{
					
					CurrentRightScrollOffset += MaxTopItemsToDisplay;
					CurrentRightSelectionNumber += MaxTopItemsToDisplay;
					CurrentRightSelectionNumber = CapInclusive(CurrentRightSelectionNumber, 0, TotalItems - 1);
				}
				RedrawPersonalList = true;
			}
			RedrawDescription = true;
			ResetDescriptionPages = true;
			
			break;
		case VK_UP:
			if(CursorOnArmory)
			{
				
				CurrentLeftSelectionNumber--;
				CurrentLeftSelectionNumber = CapInclusive(CurrentLeftSelectionNumber, 0, player->p_weapons->objcnt - 1);
				//NOW CHECK IF WE NEED TO SCREW WITH THE SCROLL OFFSET.
				if(CurrentLeftLocalSelection > 0){
					CurrentLeftLocalSelection--;
				}
				else if (CurrentLeftScrollOffset > 0){ 
					CurrentLeftScrollOffset--;
				}
				RedrawArmoryList = true;
				RedrawPersonalList = true;
			}
			else{
					TotalItems = GetNumberOfAmmoUnitsForPlayerWeapon(player, CurrentLeftSelectionNumber);
				if(TotalItems < 1){
					break;
				}
				
				CurrentRightSelectionNumber--;
				CurrentRightSelectionNumber = CapInclusive(CurrentRightSelectionNumber, 0, TotalItems - 1);
				//NOW CHECK IF WE NEED TO SCREW WITH THE SCROLL OFFSET.
				if(CurrentRightLocalSelection > 0){
					CurrentRightLocalSelection--;
				}
				else if (CurrentRightScrollOffset > 0){ 
					CurrentRightScrollOffset--;
				}
				RedrawPersonalList = true;
			}
			RedrawDescription = true;
			ResetDescriptionPages = true;
			break;
		case VK_DOWN:
			if(CursorOnArmory)
			{
				
				CurrentLeftSelectionNumber++;
				CurrentLeftSelectionNumber = CapInclusive(CurrentLeftSelectionNumber, 0, player->p_weapons->objcnt - 1);
				//NOW CHECK IF WE NEED TO SCREW WITH THE SCROLL OFFSET.
				if(CurrentLeftLocalSelection < MaxTopItemsToDisplay - 1 && CurrentLeftLocalSelection < player->p_weapons->objcnt - 1){
					CurrentLeftLocalSelection++;
				}
				else if (CurrentLeftScrollOffset < (player->p_weapons->objcnt) - MaxTopItemsToDisplay){ 
					CurrentLeftScrollOffset++;
				}
				RedrawArmoryList = true;
				RedrawPersonalList = true;
			}
			else{
				TotalItems = GetNumberOfAmmoUnitsForPlayerWeapon(player, CurrentLeftSelectionNumber);
				if(TotalItems < 1){
					break;
				}
				
				CurrentRightSelectionNumber++;
				CurrentRightSelectionNumber = CapInclusive(CurrentRightSelectionNumber, 0, TotalItems - 1);
				//NOW CHECK IF WE NEED TO SCREW WITH THE SCROLL OFFSET.
				if(CurrentRightLocalSelection < MaxTopItemsToDisplay - 1 && CurrentRightLocalSelection < TotalItems - 1){
					CurrentRightLocalSelection++;
				}
				else if (CurrentRightScrollOffset < TotalItems - MaxTopItemsToDisplay){ 
					CurrentRightScrollOffset++;
				}
				RedrawPersonalList = true;
			}
			RedrawDescription = true;
			ResetDescriptionPages = true;
			break;
		case VK_TAB:
			if(CursorOnArmory)
			{
				TotalItems = GetNumberOfAmmoUnitsForPlayerWeapon(player, CurrentLeftSelectionNumber);
				if(TotalItems > 0){
					playMIXsound(ARMORY_SOUND_SELECT);
					CursorOnArmory = false;
					RedrawArmoryList = true;
					RedrawPersonalList = true;
					RedrawDescription = true;
					ResetDescriptionPages = true;
				}
			}
			else{
				playMIXsound(ARMORY_SOUND_SELECT);
				CursorOnArmory = true;
				RedrawArmoryList = true;
				RedrawPersonalList = true;
				RedrawDescription = true;
				ResetDescriptionPages = true;
			}
			break;
		case VK_RETURN: //THE BIG ONE!
			if(CursorOnArmory){
				GiveAmmunitionForWeaponIndex(player, CurrentLeftSelectionNumber);
				RedrawArmoryList = true;
				RedrawPersonalList = true;
				RedrawDescription = true;
				ResetDescriptionPages = true;
				RedrawWeight = true;
			}
			else{
				int nindex = CurrentRightSelectionNumber;
				if(CurrentRightSelectionNumber == GetNumberOfAmmoUnitsForPlayerWeapon(player, CurrentLeftSelectionNumber) - 1)
					{
						CurrentRightSelectionNumber--;
						CurrentRightLocalSelection--;
					}
				RemoveAmmunitionForWeaponIndex(player, CurrentLeftSelectionNumber, nindex);
				RedrawArmoryList = true;
				RedrawPersonalList = true;
				RedrawDescription = true;
				ResetDescriptionPages = true;
				RedrawWeight = true;
				TotalItems = GetNumberOfAmmoUnitsForPlayerWeapon(player, CurrentLeftSelectionNumber);
				if(TotalItems < 1){
						CurrentRightSelectionNumber = 0;
						CurrentRightScrollOffset = 0;
						CurrentRightLocalSelection = 0;
						CursorOnArmory = true;
						RedrawArmoryList = true;
						
				}
			}
			break;
		}
	}
	SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, true);
	StaticDialogBox("Ammunition Order", "This concludes your ammunition order. Your ammunition choices have been approved and are currently awaiting you at the Mobilization Center on Level II. Note that the ammunition has not been loaded into your weapons. Please proceed directly to the Technology Requisition Center to order technology for your mission.\nPress ENTER to Continue.", "Complete", &globals.dboxparams);
	cls(0);
	return;
}

void GiveAmmunitionForWeaponIndex(PPLAYER player, int weaponindex)
{
	POTYPE Finder;
	PPOTYPE Found;
//	PLIST walker;
	PLIST OurList;
	PWEAPON Weapon;
	PAMMO Ammo;
	PBULLET Bullet;
	PSHELL Shell;
	PEXPLOSIVE Explosive;
	char ** AmmoStrings;
	char ** AmmoDescs;
	int * RoundsAvail;
	char tempbuf[500];
	Finder.weapon.index = weaponindex;
	Found = findObject(player->p_weapons, &Finder, CompareWeaponIndex);
	if(!Found){
		return;
	}
	Weapon = &Found->weapon;
	Finder.ammo.id = Weapon->ammotype;
	Found = findObject(globals.globalweaponsammo.ammolist, &Finder, CompareAmmoID);
	if(!Found){
		return;
	}
	Ammo = &Found->ammo;
	bool Clipneeded;
	Clipneeded = (Weapon->loadtype == WEAP_LOADTYPE_CLIP);
	ROUND AssmRound;
	AssmRound.mainammotype = Ammo->id;
	
	//NOW, MALLOC THE STRINGS.
	AmmoStrings = (char**)malloc(sizeof(char*) * Ammo->maxammotypes);
	AmmoDescs = (char**)malloc(sizeof(char*) * Ammo->maxammotypes);
	RoundsAvail = (int*)malloc(sizeof(int) * Ammo->maxammotypes);
	for(int i = 0; i < Ammo->maxammotypes; i++){
		AssmRound.subammotype = i;
		switch(Ammo->type)
		{
		case AMMOTYPE_BULLET:
			Bullet = RetrieveGlobalBulletData(&AssmRound);
			if(!Bullet){
				return;
			}
			if(!Bullet->ammoavailable || Bullet->roundsavailable < 1){
				sprintf(tempbuf, "%s - Not Available", Bullet->longname);
				RoundsAvail[i] = 0;
			}
			else if(Bullet->dsaammo){
				sprintf(tempbuf, "DSA %s - %d Rds. Left", Bullet->longname, Bullet->roundsavailable);
				RoundsAvail[i] = Bullet->roundsavailable;
			}
			else{
				sprintf(tempbuf, "%s - %d Rds. Left", Bullet->longname, Bullet->roundsavailable);
				RoundsAvail[i] = Bullet->roundsavailable;
			}
			AmmoDescs[i] = Bullet->desc;
			break;
		case AMMOTYPE_SHOTGUNSHELL:
			Shell = RetrieveGlobalShellData(&AssmRound);
			if(!Shell){
				return;
			}
			if(!Shell->ammoavailable || Shell->roundsavailable < 1){
				sprintf(tempbuf, "%s - Not Available", Shell->longname);
				RoundsAvail[i] = 0;
			}
			else if(Shell->dsaammo){
				sprintf(tempbuf, "DSA %s - %d Rds. Left", Shell->longname, Shell->roundsavailable);
				RoundsAvail[i] = Shell->roundsavailable;
			}
			else{
				sprintf(tempbuf, "%s - %d Rds. Left", Shell->longname, Shell->roundsavailable);
				RoundsAvail[i] = Shell->roundsavailable;
			}
			AmmoDescs[i] = Shell->desc;
			break;
		case AMMOTYPE_EXPLOSIVE:
			Explosive = RetrieveGlobalExplosiveData(&AssmRound);
			if(!Explosive){
				return;
			}
			if(!Explosive->ammoavailable || Explosive->roundsavailable < 1){
				sprintf(tempbuf, "%s - Not Available", Explosive->longname);
				RoundsAvail[i] = 0;
			}
			else if(Explosive->dsaammo){
				sprintf(tempbuf, "DSA %s - %d Rds. Left", Explosive->longname, Explosive->roundsavailable);
				RoundsAvail[i] = Explosive->roundsavailable;
			}
			else{
				sprintf(tempbuf, "%s - %d Rds. Left", Explosive->longname, Explosive->roundsavailable);
				RoundsAvail[i] = Explosive->roundsavailable;
			}
			AmmoDescs[i] = Explosive->desc;
			break;
		}
		AmmoStrings[i] = (char*)malloc(sizeof(char) * (strlen(tempbuf) + 1));
		strcpy(AmmoStrings[i], tempbuf);
	}
	bool anybullets = false;
	for(int k = 0; k < i; k++){
		if(RoundsAvail[k] > 0){
			anybullets = true;
		}
	}
	if(!anybullets){
		SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
		StaticDialogBox("NO ROUNDS AVAILABLE", "The DSA Armory is currently out of stock on ammunition for this weapon.", "Press ENTER", &globals.dboxparams);
		goto freeit;
	}
	CreateDBStringList(&OurList, AmmoStrings, i);
	int Selected;
	while(1){
		SetGlobalDBoxParam(DB_SCHEME_MONOCHROMATIC, DB_MEDIUM, 1, FALSE);
		Selected = SimpleListDialogBox("Select Ammunition Type", "DSA Armory", OurList, &globals.dboxparams, CreateStringDBListItem);
		if(RoundsAvail[Selected] < 1){
			continue;
		}
		SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_LARGE, 1, FALSE);
		if(YesNoDialogBox("Confirm Your Choice", AmmoDescs[Selected], "(Y)es/(N)o", &globals.dboxparams)){
			break;
		}
	}
	//GIVE THEM THE STUFF HERE.
	AssmRound.subammotype = Selected;
	//ASK THEM HOW MUCH THEY WANT.
	switch(Ammo->type){
	case AMMOTYPE_BULLET:
		Bullet = RetrieveGlobalBulletData(&AssmRound);
		break;
	case AMMOTYPE_SHOTGUNSHELL:
		Shell = RetrieveGlobalShellData(&AssmRound);
		break;
	case AMMOTYPE_EXPLOSIVE:
		Explosive = RetrieveGlobalExplosiveData(&AssmRound);
		break;
	}
	
	if(Clipneeded)
	{
		int RealRoundsAvail = RoundsAvail[Selected];
		while(player->weight + ((float)RealRoundsAvail * ROUND_WEIGHT) > MAX_PLAYER_WEIGHT){
			RealRoundsAvail--;
		}
		int MaxClips = RealRoundsAvail / Weapon->maxammo;
		bool Overflow = ((RealRoundsAvail % Weapon->maxammo) > 0);
		int RequestedClips;

		if(Overflow)
		{
			sprintf(tempbuf, "Given your current weight capacity and DSA Limitations, there is a maximum of %d full clips available for this weapon. You may, however, request one more than this number to recieve a partially-loaded clip. Please enter the number of clips you are requesting.", MaxClips);
		}
		else
		{
			sprintf(tempbuf, "Given your current weight capacity and DSA Limitations, You may requisition up to exactly %d full clips for this weapon. Please enter the number of clips you wish to requisition.", MaxClips);
		}
		SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 5, FALSE);
		RequestedClips = NumberDialogBox("Requisitioning Ammunition", tempbuf, "DSA Armory", 5, &globals.dboxparams);
		RequestedClips = CapInclusive(RequestedClips, 0, MaxClips + Overflow);
		//FINALLY....
		if(RequestedClips > 0){
		for(int clipcounter = 0; clipcounter < RequestedClips; clipcounter++){
			GiveClip(player->ammo.cliplist, Weapon, Selected, globals.globalweaponsammo.ammolist, &player->weight,  false);
		}
		SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_SMALL, 1, true);
		playMIXsound(ARMORY_SOUND_REMOVEWEAPON);
		StaticDialogBox("Thank You!", "Order Processed.", "Press ENTER", &globals.dboxparams);
		}
	}

	else
	{
		int RealRoundsAvail = RoundsAvail[Selected];
		while(player->weight + ((float)RealRoundsAvail * ROUND_WEIGHT) > MAX_PLAYER_WEIGHT){
			RealRoundsAvail--;
		}
		int RequestedRounds;
		AssmRound.subammotype = Selected;
		sprintf(tempbuf, "Given your current weight capacity and DSA Limitations, there are a total of %d rounds of this ammunition type available. Please enter the amount you wish to requsition in the box below.", RealRoundsAvail);
		SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 5, FALSE);
		RequestedRounds = NumberDialogBox("Requisitioning Ammunition", tempbuf, "DSA Armory", 5, &globals.dboxparams);
		RequestedRounds = CapInclusive(RequestedRounds, 0, RealRoundsAvail);
		if(RequestedRounds > 0){
		GiveStrayRounds(globals.globalweaponsammo.ammolist, &AssmRound, RequestedRounds, player->ammo.roundtypes, &player->weight, false);
		SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_SMALL, 1, true);
		playMIXsound(ARMORY_SOUND_REMOVEWEAPON);
		StaticDialogBox("Thank You!", "Order Processed.", "Press ENTER", &globals.dboxparams);
		}
	}

freeit:
		free(RoundsAvail);
		free(AmmoDescs);
		for(int r = 0; r < Ammo->maxammotypes; r++){
			free(AmmoStrings[r]);
		}
		free(AmmoStrings);
	
}

void RemoveAmmunitionForWeaponIndex(PPLAYER player, int Weaponindex, int localselection)
{
	POTYPE Finder;
	PPOTYPE Found;
	PWEAPON Weapon;
	PCLIP Clip;
	ROUND OurRound;
//	PAMMO Ammo;
	PBULLET Bullet;
	PSHELL Shell;
	PEXPLOSIVE Explosive;
	int RoundsToGiveBack;
	PSTRAYROUNDS Rounds;
	Finder.weapon.index = Weaponindex;
	Found = findObject(player->p_weapons, &Finder, CompareWeaponIndex);
	if(!Found){
		return;
	}
	Weapon = &Found->weapon;
	bool Clipneeded;
	Clipneeded = (Weapon->loadtype == WEAP_LOADTYPE_CLIP);

	if(Clipneeded){
		Clip = ArmoryGetClipByWeaponAndSelection(player, Weaponindex, localselection);
		if(!Clip){
			return;
		}
		Finder.clip.id = Clip->id;
		Found = findObject(player->ammo.cliplist, &Finder, CompareClipID);
		if(!Found){
			return;
		}
		else{
			player->weight -= (float)((float)Found->clip.currentrounds * ROUND_WEIGHT);
			OurRound = Found->clip.clipround;
			RoundsToGiveBack = Found->clip.currentrounds;
			deleteObject(player->ammo.cliplist, Found);
		}
	}
	else{
		Rounds = ArmoryGetStrayRoundsByWeaponAndSelection(player, Weaponindex, localselection);
		if(!Rounds){
			return;
		}
		Finder.rounds.id = Rounds->id;
		Found = findObject(player->ammo.roundtypes, &Finder, CompareStrayRoundsID);
		if(!Found){
			return;
		}
		else{
			player->weight -= (float)((float)Found->rounds.numberofroundsleft * ROUND_WEIGHT);
			OurRound.mainammotype = Found->rounds.mainammotype;
			OurRound.subammotype = Found->rounds.subammotype;
			RoundsToGiveBack = Found->rounds.numberofroundsleft;
			deleteObject(player->ammo.roundtypes, Found);
			
		}
	}

	switch(RetrieveGlobalSubAmmoType(&OurRound))
	{
	case AMMOTYPE_BULLET:
		Bullet = RetrieveGlobalBulletData(&OurRound);
		if(!Bullet){
			break;
		}
		Bullet->roundsavailable += RoundsToGiveBack;
		break;
	case AMMOTYPE_SHOTGUNSHELL:
		Shell = RetrieveGlobalShellData(&OurRound);
		if(!Shell){
			break;
		}
		Shell->roundsavailable += RoundsToGiveBack;
		break;
	case AMMOTYPE_EXPLOSIVE:
		Explosive = RetrieveGlobalExplosiveData(&OurRound);
		if(!Explosive){
			break;
		}
		Explosive->roundsavailable += RoundsToGiveBack;
		break;
	}
	playMIXsound(ARMORY_SOUND_GETWEAPON);

}

void ArmoryCreateAmmoDesc(char* outstring, PWEAPON weapon)
{
	char buf[2000];
	PPOTYPE ListObject;
	POTYPE TempObject;
	ROUND Round;
	int roundsleft;
	char* subammoname;
	
	if(weapon->maxammo < 1 || !weapon->attacktype & WEAP_ATT_TYPE_SHOOT){
		sprintf(outstring, "DSA Armory Reports This Weapon Does Not Use Ammunition.");
		return;
	}
	sprintf(outstring, "DSA Ammunition Manifest for %s Follows\n", weapon->shortname);
	TempObject.ammo.id = weapon->ammotype;
	ListObject = findObject(globals.globalweaponsammo.ammolist, &TempObject, CompareAmmoID);
	if(!ListObject)
	{
		sprintf(buf, "Unknown Ammunition Type.\n");
		strcat(outstring, buf);
		return;
	}
	sprintf(buf, "%s Ammunition\n", ListObject->ammo.name);
	strcat(outstring, buf);
	sprintf(buf,"----------------------------------------\n");
	strcat(outstring, buf);
	for(int i = 0; i < ListObject->ammo.maxammotypes; i++){
		Round.mainammotype = weapon->ammotype;
		Round.subammotype = i;
		subammoname = RetrieveSubAmmoLongName(&Round);
		roundsleft = RetrieveSubAmmoNumberRoundsLeft(&Round);
		sprintf(buf, "%d %s", roundsleft, subammoname);
		if(IsSubAmmoDSAOnly(&Round)){
			strcat(buf, " (DSA Custom)");
		}
		strcat(buf, "\n");
		strcat(outstring, buf);
	}
	strcat(outstring, "----------------------------------\n");
	strcat(outstring, "REPORT ENDS");
}

void LevelIVSelectWeaponExtras(PWEAPON weapon)
{
	//FIND OUT WHETHER OR NOT THIS IS ACTUALLY NEEDED.
	if(!(weapon->attacktype & WEAP_ATT_TYPE_SHOOT)){
		return;
	}
	if(!weapon->modifiersallowed.flashsuppressor && !weapon->modifiersallowed.silencer && !weapon->modifiersallowed.lasersight)
	{
		return;
	}
	bool Flash = false;
	bool Silence = false;
	bool Laser = false;
	bool Any = false;

	char buf[500];
	weapon->modifiersactivated.flashsuppressor = false;
	weapon->modifiersactivated.silencer = false;
	weapon->modifiersactivated.lasersight = false;
	if(weapon->modifiersallowed.silencer){
	weapon->modifiersattached.silencer = true;
	Any = true;
	Silence = true;
	}
	if(weapon->modifiersallowed.flashsuppressor){
		weapon->modifiersattached.flashsuppressor = true;
		Any = true;
		Flash  = true;
	}
	if(weapon->modifiersallowed.lasersight){
		weapon->modifiersattached.lasersight = true;
		Any = true;
		Laser = true;
	}

	if(Any){
	SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_MEDIUM, 1 , TRUE);
	strcpy(buf, "The Following Modifiers Have Been Issued With Your Weapon:");
	if(Silence){
		strcat(buf, "\nSilencer");
	}
	if(Flash){
		strcat(buf, "\nFlash Suppressor");
	}
	if(Laser){
		strcat(buf, "\nLaser Sight");
	}
	StaticDialogBox("Modifiers Issued", buf, "Press ENTER", &globals.dboxparams);
	
	}
}

void ArmoryCreateExtendedDescription(PWEAPON weapon, char* OutString)
{
	if(weapon == NULL){
		strcpy(OutString, "WEAPON DATA MISSING. ERROR.");
		return;
	}
	char buf[5000];
	char tempbuf[500];
	PPOTYPE lstObj;
	POTYPE tmpObj;
	BOOL Classified = weapon->dsaweapon;
	//	bool CanLoad;
	sprintf(buf, "%s\n", weapon->longname);
	strcpy(OutString, buf);
	
	
	
	
	if(Classified)
	{
		switch(weapon->classtype)
		{
		case WEAP_CLASS_A:
			sprintf(buf, "Weapon Is DSA Class A - Standard\n");
			break;
		case WEAP_CLASS_B:
			sprintf(buf, "Weapon Is DSA Class B - Assault\n");
			break;
		case WEAP_CLASS_S:
			sprintf(buf, "Weapon Is DSA Class S - Special\n");
			break;
		default:
		case WEAP_CLASS_X:
			sprintf(buf, "Weapon Is DSA Class X - Experimental\n");
			break;
		}
	}
	else{
		sprintf(buf, "Weapon Is Not Currently DSA Classified\n");
	}
	strcat(OutString, buf);
	
	sprintf(buf, "Weapon Is Filed As A(n) ");
	switch(weapon->filingtype)
	{
	case WEAP_FILING_TYPE_NONE:
	default:
		strcat(buf, "Unknown Type\n");
		break;
	case WEAP_FILING_TYPE_MELEE:
		strcat(buf, "Hand Weapon\n");
		break;
	case WEAP_FILING_TYPE_PISTOLS:
		strcat(buf, "Pistol\n");
		break;
	case WEAP_FILING_TYPE_SMGS:
		strcat(buf, "Sub-Machine Gun\n");
		break;
	case WEAP_FILING_TYPE_RIFLES:
		strcat(buf, "Rifle\n");
		break;
	case WEAP_FILING_TYPE_SHOTGUN:
		strcat(buf, "Shotgun\n");
		break;
	case WEAP_FILING_TYPE_HEAVY:
		strcat(buf, "Heavy Weapon\n");
		break;
	case WEAP_FILING_TYPE_SPECIAL:
		strcat(buf, "Special Weapon\n");
		break;
	}
	strcat(OutString, buf);
	tmpObj.ammo.id = weapon->ammotype;
	lstObj = findObject(globals.globalweaponsammo.ammolist, &tmpObj, CompareAmmoID);
	
	if(lstObj == NULL){
		strcat(OutString, "No Ammunition Required\n");
	}
	else{
		strcpy(buf, lstObj->ammo.name);
		switch(lstObj->ammo.type)
		{
		case AMMOTYPE_BULLET:
			strcat(buf, " Bullet Ammunition\n");
			break;
		case AMMOTYPE_SHOTGUNSHELL:
			strcat(buf, " Scatter Ammunition\n");
			break;
		case AMMOTYPE_EXPLOSIVE:
			strcat(buf, " Pyrotechnic Ammunition\n");
			break;
		}
		strcat(OutString, buf);
	}
	
	
	switch(weapon->loadtype)
	{
	case WEAP_LOADTYPE_CLIP:
		sprintf(buf, "Clip Loading Configuration\n");
		break;
	case WEAP_LOADTYPE_MAGAZINE:
		sprintf(buf, "Magazine Loading Configuration\n");
		break;
	case WEAP_LOADTYPE_CYLINDER:
		sprintf(buf, "Cylinder Loading Configuration\n");
		break;
	default:
		sprintf(buf, "No Loading Configuration\n");
		break;
	}
	strcat(OutString, buf);

	strcpy(buf, "Firing Modes: ");
	if(!DoesWeaponUseAmmunition(weapon)){
		strcpy(buf, "N/A");
	}
	else{
		strcat(buf, "SAFE - SINGLE SHOT");
		if(weapon->weapperf.roundsperturn > 1){
			sprintf(tempbuf, " - %d ROUND BURST", weapon->weapperf.roundsperturn);
			strcat(buf, tempbuf);
		}
	}
	strcat(buf, "\n");

	strcat(OutString, buf);
	
	sprintf(buf, "Weight - %2.2f lbs\n", weapon->weight);
	strcat(OutString, buf);
	
	sprintf(buf, "Current DSA Stock Status: ");
	if(weapon->weaponavailable){
		strcat(buf, "AVAILABLE\n");
	}
	else{
		strcat(buf, "NOT AVAILABLE\n");
	}
	strcat(OutString, buf);
	
	strcat(OutString, "---DESCRIPTION FOLLOWS BELOW---\n");
	strcat(OutString, weapon->desc);//DONE!	
}

bool IsAmmunitionUsedForMoreThanOnePlayerWeapon(PPLAYER player, int weaponindex)
{
	POTYPE Finder;
	PPOTYPE Found;
	int count = 0;
	PLIST walker = player->p_weapons;
	PWEAPON Weapon;
	Finder.weapon.index = weaponindex;
	Found = findObject(player->p_weapons, &Finder, CompareWeaponIndex);
	if(!Found){
		return true;
	}
	Weapon = &Found->weapon;

	//SEARCH RIGHT LIST FOR WEAPON THAT USES SAME AMMO TYPE.
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		if(Weapon->loadtype == WEAP_LOADTYPE_CLIP){
			if(Weapon->id == walker->current->object.weapon.id) //SINCE CLIPS ARE UNIQUE TO AN ID
			{
				count++;
			}
		}
		else{
		if(walker->current->object.weapon.ammotype == Weapon->ammotype){
			count++;
		}
		}
	}
	if(count > 1){
		return true;
	}
	return false;
}

void PurgeAllAmmunitionForWeaponIndex(PPLAYER player, int weaponindex)
{
	POTYPE Finder;
	PPOTYPE Found;
	int count = 0;
	PLIST walker;
	PWEAPON Weapon;
	Finder.weapon.index = weaponindex;
	Found = findObject(player->p_weapons, &Finder, CompareWeaponIndex);
	PPOTYPE* Objects;
	if(!Found){
		return;
	}
	Weapon = &Found->weapon;

	if(Weapon->loadtype == WEAP_LOADTYPE_CLIP)
	{
		if(player->ammo.cliplist->objcnt < 1){
			return;
		}
		Objects = (PPOTYPE*)malloc(sizeof(PPOTYPE) * player->ammo.cliplist->objcnt);
		walker = player->ammo.cliplist;
		for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
		{
			if(walker->current->object.clip.weaponid == Weapon->id){
				Objects[count] = &walker->current->object; //DON'T DELETE UNTIL WE'RE DONE WALKIN'.
				count++;
			}
		}
		for(int i = 0; i < count; i++){
			player->weight -= (float)(((double)Objects[i]->clip.currentrounds) * ROUND_WEIGHT);
			deleteObject(player->ammo.cliplist, Objects[i]);
			
		}
		free(Objects);
	}
	else
	{
			if(player->ammo.roundtypes->objcnt < 1){
			return;
		}
		Objects = (PPOTYPE*)malloc(sizeof(PPOTYPE) * player->ammo.roundtypes->objcnt);
		walker = player->ammo.roundtypes;
		for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
		{
			if(walker->current->object.rounds.mainammotype == Weapon->ammotype){
				Objects[count] = &walker->current->object; //DON'T DELETE UNTIL WE'RE DONE WALKIN'.
				count++;
			}
		}
		for(int i = 0; i < count; i++){
			player->weight -= (float)(((double)Objects[i]->rounds.numberofroundsleft) * ROUND_WEIGHT);
			deleteObject(player->ammo.roundtypes, Objects[i]);
			
		}
		free(Objects);
	}

	ReorderPlayerAmmoLists(&player->ammo);
}

void ArmoryDrawGizmoList(PPLAYER player, PLIST armorygizmolist, PLIST gizmolist, RECT boundingrect, int localselection, int scrolloffset, bool hasfocus, bool usingArmorylist)
{
	unsigned int MaxTextLength = (boundingrect.right - boundingrect.left) - 1;
	int MaxItemsToDisplay = (boundingrect.bottom - boundingrect.top);
	POINT CurrentDrawPoint;
	CurrentDrawPoint.x = boundingrect.left;
	int SrcID;
	int DstID;
	char* StringToPrint;
	POTYPE TempObject;
	PPOTYPE ListObject;
	int objectcount;
	//FIRST, WE NEED TO DRAW THE LIST CONTENTS.
	if(usingArmorylist){
		objectcount = armorygizmolist->objcnt;
	}
	else{
		objectcount = gizmolist->objcnt;
	}
	
	for(SrcID = scrolloffset, DstID = 0; SrcID < objectcount && DstID <= MaxItemsToDisplay; SrcID++, DstID++)
	{
		if(usingArmorylist){
			TempObject.armorygizmo.id = SrcID;
			ListObject= findObject(armorygizmolist, &TempObject, CompareArmoryGizmo);
		}
		else{
			TempObject.gizmo.index = SrcID;
			ListObject = findObject(gizmolist, &TempObject, CompareGizmoIndex);
			TempObject.armorygizmo.isgrenade = ListObject->gizmo.isgrenade;
			if(TempObject.armorygizmo.isgrenade){
				TempObject.armorygizmo.type = ListObject->gizmo.unit.grenade.grenadetype;
			}
			else{
				TempObject.armorygizmo.type = ListObject->gizmo.unit.device.devicetype;
			}
			ListObject = findObject(armorygizmolist, &TempObject, CompareArmoryGizmoBothTypes);
		}
		CurrentDrawPoint.y = DstID + boundingrect.top;
		setcursor(CurrentDrawPoint.x, CurrentDrawPoint.y);
		if(!ListObject){
			RED;
			printf("INVALID GIZMO");
		}
		else{
			//NOW PRINT IT, BOY.
			if(strlen(ListObject->armorygizmo.longname) > MaxTextLength){
				StringToPrint = ListObject->armorygizmo.shortname;
			}
			else{
				StringToPrint = ListObject->armorygizmo.longname;
			}
			
			if(!ListObject->armorygizmo.isavailable){ //UNAVAILABLE?
				if(DstID == localselection && hasfocus)
				{
					setcolor(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
				}
				else{
					GRAY;
				}
				printf("%s", StringToPrint);
			}
			
			else{
				if(DstID == localselection && hasfocus){
					setcolor(BACKGROUND_BLUE | BACKGROUND_INTENSITY);
				}
				else{
					LBLUE;
				}
				printf("%s", StringToPrint);
			}
		}
	}
	
	//DRAW ARROWS IF NECESSARY.
	LGREEN;
	char UpArrowChar = (char)24;
	char DnArrowChar = (char)25;
	POINT UpArrowDrawPoint;
	UpArrowDrawPoint.x = boundingrect.right - 1;
	UpArrowDrawPoint.y = boundingrect.top;
	POINT DnArrowDrawPoint;
	DnArrowDrawPoint.x = boundingrect.right - 1;
	DnArrowDrawPoint.y = boundingrect.bottom;
	
	if(scrolloffset > 0){
		setcursor(UpArrowDrawPoint.x, UpArrowDrawPoint.y);
		printf("%c", UpArrowChar);
	}
	if(usingArmorylist){
		if(scrolloffset < armorygizmolist->objcnt - MaxItemsToDisplay){
			setcursor(DnArrowDrawPoint.x, DnArrowDrawPoint.y);
			printf("%c", DnArrowChar);
		}
	}
	else{
		if(scrolloffset < gizmolist->objcnt - MaxItemsToDisplay){
			setcursor(DnArrowDrawPoint.x, DnArrowDrawPoint.y);
			printf("%c", DnArrowChar);
		}
	}
}

void ArmoryAmmunitionDrawWeaponList(PPLAYER player, RECT boundingrect, int localselection, int scrolloffset, bool hasfocus)
{
		unsigned int MaxTextLength = (boundingrect.right - boundingrect.left) - 1;
	int MaxItemsToDisplay = (boundingrect.bottom - boundingrect.top);
	POINT CurrentDrawPoint;
	CurrentDrawPoint.x = boundingrect.left;
	int SrcID;
	int DstID;
	char* StringToPrint;
	POTYPE TempObject;
	PPOTYPE ListObject;
	//FIRST, WE NEED TO DRAW THE LIST CONTENTS.
	for(SrcID = scrolloffset, DstID = 0; SrcID < player->p_weapons->objcnt && DstID <= MaxItemsToDisplay; SrcID++, DstID++)
	{
		
			TempObject.weapon.index = SrcID;
			ListObject= findObject(player->p_weapons, &TempObject, CompareWeaponIndex);
		CurrentDrawPoint.y = DstID + boundingrect.top;
		setcursor(CurrentDrawPoint.x, CurrentDrawPoint.y);
		if(!ListObject){
			RED;
			printf("INVALID WEAPON");
		}
		else{
			//NOW PRINT IT, BOY.
			if(strlen(ListObject->weapon.longname) > MaxTextLength){
				StringToPrint = ListObject->weapon.shortname;
			}
			else{
				StringToPrint = ListObject->weapon.longname;
			}
			if(!DoesWeaponUseAmmunition(&ListObject->weapon))
			{
				if(DstID == localselection && hasfocus){
					setcolor(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
				}
				else{
					GRAY;
				}
				printf("%s", StringToPrint);
			}
			else if(!DoesAnyAmmunitionExistForPlayerWeapon(player, ListObject->weapon.index)){
				if(DstID == localselection && hasfocus)
				{
					setcolor(BACKGROUND_RED);
				}
				else{
					RED;
				}
				printf("%s", StringToPrint);
			}
			
			else{
				if(DstID == localselection && hasfocus){
					setcolor(BACKGROUND_GREEN);
				}
				else{
					LGREEN;
				}
				printf("%s", StringToPrint);
			}
		}
	}
	
	//DRAW ARROWS IF NECESSARY.
	LBLUE;
	char UpArrowChar = (char)24;
	char DnArrowChar = (char)25;
	POINT UpArrowDrawPoint;
	UpArrowDrawPoint.x = boundingrect.right - 1;
	UpArrowDrawPoint.y = boundingrect.top;
	POINT DnArrowDrawPoint;
	DnArrowDrawPoint.x = boundingrect.right - 1;
	DnArrowDrawPoint.y = boundingrect.bottom;
	
	if(scrolloffset > 0){
		setcursor(UpArrowDrawPoint.x, UpArrowDrawPoint.y);
		printf("%c", UpArrowChar);
	}
	if(scrolloffset < player->p_weapons->objcnt - MaxItemsToDisplay){
		setcursor(DnArrowDrawPoint.x, DnArrowDrawPoint.y);
		printf("%c", DnArrowChar);
	}
	
}

void ArmoryAmmunitionDrawAmmunitionList(PPLAYER player, RECT boundingrect, int weaponindex, int localselection, int scrolloffset, bool hasfocus)
{
	POTYPE Finder;
	PPOTYPE Found;
	PLIST walker = player->ammo.cliplist;
	PWEAPON Weapon;
	PCLIP Clip;
	PBULLET Bullet;
	PSHELL Shell;
	PEXPLOSIVE Explosive;
	char * subammoname = NULL;
	bool clipneeded = false;
	PSTRAYROUNDS Rounds;
	Finder.weapon.index = weaponindex;
	Found = findObject(player->p_weapons, &Finder, CompareWeaponIndex);
	if(!Found){
		return;
	}
	Weapon = &Found->weapon;

	unsigned int MaxTextLength = (boundingrect.right - boundingrect.left) - 1;
	int MaxItemsToDisplay = (boundingrect.bottom - boundingrect.top);
	POINT CurrentDrawPoint;
	CurrentDrawPoint.x = boundingrect.left;
	int NumItems = 0;
	int SrcID;
	int DstID;
	char StringToPrint[500];
//	POTYPE TempObject;
//	PPOTYPE ListObject;
	//LOOP UNTIL WE GET A NULL.
	Rounds = ArmoryGetStrayRoundsByWeaponAndSelection(player, weaponindex, scrolloffset);
	Clip = ArmoryGetClipByWeaponAndSelection(player, weaponindex, scrolloffset);
	clipneeded = (Weapon->loadtype == WEAP_LOADTYPE_CLIP);
	if(!Clip && !Rounds){
		return;
	}
	for(SrcID = scrolloffset, DstID = 0; (Clip != NULL || Rounds != NULL) && DstID <= MaxItemsToDisplay; SrcID++, DstID++, NumItems++)
	{
		
		//GET A FINAL STRING FOR EITHER CLIP OR STRAY ROUND.
		Rounds = ArmoryGetStrayRoundsByWeaponAndSelection(player, weaponindex, SrcID);
		Clip = ArmoryGetClipByWeaponAndSelection(player, weaponindex, SrcID);
		if(!Rounds && !Clip){
			break;
		}
		if(clipneeded){
			if(!Clip){
				break;
			}
			switch(RetrieveGlobalSubAmmoType(&Clip->clipround))
			{
			case AMMOTYPE_BULLET:
				Bullet = RetrieveGlobalBulletData(&Clip->clipround);
				if(!Bullet){
					strcpy(StringToPrint, "INVALID BULLET TYPE!");
				}
				else{
					subammoname = Bullet->longname;
				}
				break;
			case AMMOTYPE_SHOTGUNSHELL:
				Shell = RetrieveGlobalShellData(&Clip->clipround);
				if(!Shell){
					strcpy(StringToPrint, "INVALID SHELL TYPE!");
				}
				else{
					subammoname = Shell->longname;
				}
				break;
			case AMMOTYPE_EXPLOSIVE:
				Explosive = RetrieveGlobalExplosiveData(&Clip->clipround);
				if(!Explosive){
					strcpy(StringToPrint, "INVALID EXPLOSIVE TYPE!");
				}
				else{
					subammoname = Explosive->longname;
				}
				break;
			}
			if(!Clip){
				strcpy(StringToPrint, "INVALID CLIP!");
			}
			else if(!subammoname){
			}
			else{
			sprintf(StringToPrint, "Clip of %d %s", Clip->currentrounds, subammoname);
			}
		}
		else
		{
			if(!Rounds){
				break;
			}
			ROUND AssembledRound;
			AssembledRound.mainammotype = Rounds->mainammotype;
			AssembledRound.subammotype = Rounds->subammotype;
				switch(RetrieveGlobalSubAmmoType(&AssembledRound))
			{
			case AMMOTYPE_BULLET:
				Bullet = RetrieveGlobalBulletData(&AssembledRound);
				if(!Bullet){
					strcpy(StringToPrint, "INVALID BULLET TYPE!");
				}
				else{
					subammoname = Bullet->longname;
				}
				break;
			case AMMOTYPE_SHOTGUNSHELL:
				Shell = RetrieveGlobalShellData(&AssembledRound);
				if(!Shell){
					strcpy(StringToPrint, "INVALID SHELL TYPE!");
				}
				else{
					subammoname = Shell->longname;
				}
				break;
			case AMMOTYPE_EXPLOSIVE:
				Explosive = RetrieveGlobalExplosiveData(&AssembledRound);
				if(!Explosive){
					strcpy(StringToPrint, "INVALID EXPLOSIVE TYPE!");
				}
				else{
					subammoname = Explosive->longname;
				}
				break;
			}
			if(!Rounds){
				strcpy(StringToPrint, "INVALID STRAY ROUNDS!");
			}
			else if(!subammoname){
			}
			else{
			sprintf(StringToPrint, "%s (%d Rds)", subammoname, Rounds->numberofroundsleft);
			}
		}
		CurrentDrawPoint.y = DstID + boundingrect.top;
		setcursor(CurrentDrawPoint.x, CurrentDrawPoint.y);
		if(!Rounds && !Clip){
			RED;
			printf("INVALID AMMUNITION");
		}
		else{
			//NOW PRINT IT, BOY.
				if(DstID == localselection && hasfocus){
					setcolor(BACKGROUND_GREEN);
				}
				else{
					LGREEN;
				}
				printf("%s", StringToPrint);
			}
		}
	
	
	//DRAW ARROWS IF NECESSARY.
	LBLUE;
	char UpArrowChar = (char)24;
	char DnArrowChar = (char)25;
	POINT UpArrowDrawPoint;
	UpArrowDrawPoint.x = boundingrect.right - 1;
	UpArrowDrawPoint.y = boundingrect.top;
	POINT DnArrowDrawPoint;
	DnArrowDrawPoint.x = boundingrect.right - 1;
	DnArrowDrawPoint.y = boundingrect.bottom;
	
	if(scrolloffset > 0){
		setcursor(UpArrowDrawPoint.x, UpArrowDrawPoint.y);
		printf("%c", UpArrowChar);
	}
	if(scrolloffset < NumItems - MaxItemsToDisplay){
		setcursor(DnArrowDrawPoint.x, DnArrowDrawPoint.y);
		printf("%c", DnArrowChar);
	}
}

PCLIP ArmoryGetClipByWeaponAndSelection(PPLAYER player, int weaponindex, int localselection)
{
	int counter = 0;
	POTYPE Finder;
	PPOTYPE Found;
	PLIST walker = player->ammo.cliplist;
	PWEAPON Weapon;
	Finder.weapon.index = weaponindex;
	Found = findObject(player->p_weapons, &Finder, CompareWeaponIndex);
	if(!Found){
		return NULL;
	}
	Weapon = &Found->weapon;
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		if(walker->current->object.clip.weaponid == Weapon->id){
			if(counter == localselection){
				return &walker->current->object.clip;
			}
			counter++;
		}
	}
	return NULL;
}

PSTRAYROUNDS ArmoryGetStrayRoundsByWeaponAndSelection(PPLAYER player, int weaponindex, int localselection)
{
int counter = 0;
	POTYPE Finder;
	PPOTYPE Found;
	PLIST walker = player->ammo.roundtypes;
	PWEAPON Weapon;
	Finder.weapon.index = weaponindex;
	Found = findObject(player->p_weapons, &Finder, CompareWeaponIndex);
	if(!Found){
		return NULL;
	}
	Weapon = &Found->weapon;
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		if(walker->current->object.rounds.mainammotype == Weapon->ammotype){
			if(counter == localselection){
				return &walker->current->object.rounds;
			}
			counter++;
		}
	}
	return NULL;
}

bool DoesAnyAmmunitionExistForPlayerWeapon(PPLAYER player, int index)
{
	PWEAPON Weapon;
	POTYPE Finder;
	PPOTYPE Found;
	Finder.weapon.index = index;
	Found = findObject(player->p_weapons, &Finder, CompareWeaponIndex);
	if(!Found){
		return false;
	}
	Weapon = &Found->weapon;
	//NOW, LET'S CHECK THIS GOOOOOOOOD....
	
	//FIRST, THE CLIPS.
	if(Weapon->loadtype == WEAP_LOADTYPE_CLIP){
	Finder.clip.weaponid = Weapon->id;
	Found = findObject(player->ammo.cliplist, &Finder, CompareClipWeaponID);
	if(Found){
		return true;
	}
	return false;
	}
	
	else{
		Finder.rounds.mainammotype = Weapon->ammotype;
		Found = findObject(player->ammo.roundtypes, &Finder, CompareStrayRoundAmmoTypes);
		if(Found){
			return true;
		}
		return false;
	}
}

int GetNumberOfAmmoUnitsForPlayerWeapon(PPLAYER player, int weaponindex)
{
	POTYPE Finder;
	PPOTYPE Found;
	int count = 0;
	PLIST walker;
	PWEAPON Weapon;
	Finder.weapon.index = weaponindex;
	Found = findObject(player->p_weapons, &Finder, CompareWeaponIndex);

	if(!Found){
		return 0;
	}
	Weapon = &Found->weapon;

	if(Weapon->loadtype == WEAP_LOADTYPE_CLIP)
	{
		if(player->ammo.cliplist->objcnt < 1){
			return 0;
		}
		
		walker = player->ammo.cliplist;
		for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
		{
			if(walker->current->object.clip.weaponid == Weapon->id){
			count++;
			}
		}
	
	
	}
	else
	{
			if(player->ammo.roundtypes->objcnt < 1){
			return 0;
		}
		
		walker = player->ammo.roundtypes;
		for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
		{
			if(walker->current->object.rounds.mainammotype == Weapon->ammotype){
				count++;
			}
		}
		
		
	}
	return count;
}

bool DoesWeaponUseAmmunition(PWEAPON weapon)
{
	if(weapon->ammotype < 1 || (!(weapon->attacktype & WEAP_ATT_TYPE_SHOOT) && !(weapon->attacktype & WEAP_ATT_TYPE_SNIPE))){
		return false;
	}
	return true;
}

void ArmoryDrawWeaponList(PPLAYER player, PLIST weaponlist, RECT boundingrect, int localselection, int scrolloffset, bool hasfocus, bool usesIndex)
{
	unsigned int MaxTextLength = (boundingrect.right - boundingrect.left) - 1;
	int MaxItemsToDisplay = (boundingrect.bottom - boundingrect.top);
	POINT CurrentDrawPoint;
	CurrentDrawPoint.x = boundingrect.left;
	int SrcID;
	int DstID;
	char* StringToPrint;
	POTYPE TempObject;
	PPOTYPE ListObject;
	//FIRST, WE NEED TO DRAW THE LIST CONTENTS.
	for(SrcID = scrolloffset, DstID = 0; SrcID < weaponlist->objcnt && DstID <= MaxItemsToDisplay; SrcID++, DstID++)
	{
		if(usesIndex){
			TempObject.weapon.index = SrcID;
			ListObject= findObject(weaponlist, &TempObject, CompareWeaponIndex);
		}
		else{
			TempObject.weapon.id = SrcID + 1;
			ListObject = findObject(weaponlist, &TempObject, CompareWeaponID);
		}
		CurrentDrawPoint.y = DstID + boundingrect.top;
		setcursor(CurrentDrawPoint.x, CurrentDrawPoint.y);
		if(!ListObject){
			RED;
			printf("INVALID WEAPON");
		}
		else{
			//NOW PRINT IT, BOY.
			if(strlen(ListObject->weapon.longname) > MaxTextLength){
				StringToPrint = ListObject->weapon.shortname;
			}
			else{
				StringToPrint = ListObject->weapon.longname;
			}
			if(!IsClearedForWeapon(player, &ListObject->weapon))
			{
				if(DstID == localselection && hasfocus){
					setcolor(BACKGROUND_RED);
				}
				else{
					RED;
				}
				printf("-CLASSIFIED-");
			}
			else if(!ListObject->weapon.weaponavailable){ //UNAVAILABLE?
				if(DstID == localselection && hasfocus)
				{
					setcolor(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
				}
				else{
					GRAY;
				}
				printf("%s", StringToPrint);
			}
			
			else{
				if(DstID == localselection && hasfocus){
					setcolor(BACKGROUND_GREEN);
				}
				else{
					LGREEN;
				}
				printf("%s", StringToPrint);
			}
		}
	}
	
	//DRAW ARROWS IF NECESSARY.
	LBLUE;
	char UpArrowChar = (char)24;
	char DnArrowChar = (char)25;
	POINT UpArrowDrawPoint;
	UpArrowDrawPoint.x = boundingrect.right - 1;
	UpArrowDrawPoint.y = boundingrect.top;
	POINT DnArrowDrawPoint;
	DnArrowDrawPoint.x = boundingrect.right - 1;
	DnArrowDrawPoint.y = boundingrect.bottom;
	
	if(scrolloffset > 0){
		setcursor(UpArrowDrawPoint.x, UpArrowDrawPoint.y);
		printf("%c", UpArrowChar);
	}
	if(scrolloffset < weaponlist->objcnt - MaxItemsToDisplay){
		setcursor(DnArrowDrawPoint.x, DnArrowDrawPoint.y);
		printf("%c", DnArrowChar);
	}
	
}

void LevelIVGizmoLockers(PPLAYER player)
{
/*
#define ARMORY_LEFTBOXXORIGIN	0
#define ARMORY_LEFTBOXYORIGIN	0
#define ARMORY_RIGHTBOXXORIGIN	40
#define ARMORY_RIGHTBOXYORIGIN	0
#define ARMORY_TOPBOXESWIDTH	39
#define ARMORY_TOPBOXESHEIGHT	10
#define ARMORY_WEIGHTBOXXORIGIN	0
#define ARMORY_WEIGHTBOXYORIGIN	11
#define ARMORY_WEIGHTBOXWIDTH	80
#define ARMORY_WEIGHTBOXHEIGHT	1
#define ARMORY_DESCBOXXORIGIN	0
#define ARMORY_DESCBOXYORIGIN	15
#define ARMORY_DESCBOXWIDTH		80
#define ARMORY_DESCBOXHEIGHT	9
	*/
	char* LeftText = "LEVEL IV";
	char* RightText = "SYSTEM BOOTING...";
	char* RightOutText = "NO GIZMOS LOADED";
	char* OutOfStockText = "TECHNOLOGY CURRENTLY OUT OF STOCK";
	char* ArmoryStock = "DSA CENTRAL STOCK";
	char* PersonalStock = "PERSONAL STOCK";
	char* HelpText = "TAB To Switch Between Central and Personal Stock\nUP/DOWN or PAGEUP/PAGEDOWN To Change Selection\nENTER to Add/Remove Stock\n [ and ] To Scroll Description\nESC To Exit Armory\nPRESS ENTER TO ACTIVATE SYSTEM.";
	RECT leftboxrect;
	leftboxrect.top = ARMORY_LEFTBOXYORIGIN + 1;
	leftboxrect.bottom = leftboxrect.top + ARMORY_TOPBOXESHEIGHT - 1;
	leftboxrect.left = ARMORY_LEFTBOXXORIGIN;
	leftboxrect.right = leftboxrect.left + ARMORY_TOPBOXESWIDTH;
	
	RECT rightboxrect;
	rightboxrect.top = ARMORY_RIGHTBOXYORIGIN + 1;
	rightboxrect.bottom = rightboxrect.top + ARMORY_TOPBOXESHEIGHT - 1;
	rightboxrect.left = ARMORY_RIGHTBOXXORIGIN;
	rightboxrect.right = rightboxrect.left + ARMORY_TOPBOXESWIDTH;
	
	RECT weightboxrect;
	weightboxrect.top = ARMORY_WEIGHTBOXYORIGIN;
	weightboxrect.bottom = weightboxrect.top + ARMORY_WEIGHTBOXHEIGHT;
	weightboxrect.left = ARMORY_WEIGHTBOXXORIGIN;
	weightboxrect.right = weightboxrect.left + ARMORY_WEIGHTBOXWIDTH;
	
	RECT descboxrect;
	descboxrect.top = ARMORY_DESCBOXYORIGIN - 1 ;
	descboxrect.bottom = descboxrect.top + ARMORY_DESCBOXHEIGHT;
	descboxrect.left = ARMORY_DESCBOXXORIGIN;
	descboxrect.right = descboxrect.left + ARMORY_DESCBOXWIDTH;
	
	PAGIZMO CurrentGizmo = NULL;
	PPOTYPE CurrentObject = NULL;
	PPOTYPE CurrentObjectTwo = NULL;
	POTYPE TempObject;
	int CurrentLeftSelectionNumber = 0;
	int CurrentLeftLocalSelection = 0;
	int CurrentLeftScrollOffset = 0;
	int CurrentRightLocalSelection = 0;
	int CurrentRightSelectionNumber = 0;
	int CurrentRightScrollOffset = 0;
	int MaxTopItemsToDisplay = leftboxrect.bottom - leftboxrect.top;
	bool RedrawDescription = true;
	bool RedrawArmoryList = true;
	bool RedrawPersonalList = true;
	bool RedrawWeight = true;
	bool CursorOnArmory = true;
	bool ResetDescriptionPages = true;
	int quicksleep = 100;
	int DescPages = 1;
	//	char ExtendedDescription[50000];
	int CurrentDescPage = 0;
	int longsleep = 500;
	//	int ReturnCode;
	char WeightString[500];
	bool Continuing = true;
	USHORT key;
	//char WeapString[50];
	POINT currentdrawpoint;
	
	if(!player->p_gizmos){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_SMALL, 0, TRUE);
		StaticDialogBox("Error", "Player Gizmo List Uninitialized!", "Error", &globals.dboxparams);
		return;
	}
	
	
	BLUE;
	printgraphic(globals.graphicslist, globals.zeroed, ARMORY_GRAPHIC_ID);
	
	//LEFT TEXT
	currentdrawpoint = ReturnTextCenterInRect(LeftText, leftboxrect);
	RED;
	setcursor(currentdrawpoint.x, currentdrawpoint.y);
	writestring(LeftText);
	Sleep(quicksleep);
	LRED;
	setcursor(currentdrawpoint.x, currentdrawpoint.y);
	writestring(LeftText);
	Sleep(longsleep);
	
	//RIGHT TEXT
	currentdrawpoint = ReturnTextCenterInRect(RightText, rightboxrect);
	RED;
	setcursor(currentdrawpoint.x, currentdrawpoint.y);
	writestring(RightText);
	Sleep(quicksleep);
	LRED;
	setcursor(currentdrawpoint.x, currentdrawpoint.y);
	writestring(RightText);
	Sleep(longsleep);
	
	COORD descstartcoord;
	descstartcoord.X = (short)descboxrect.left;
	descstartcoord.Y = (short)descboxrect.top;
	RED;
	printinrect(HelpText, descstartcoord);
	Sleep(quicksleep);
	LRED;
	printinrect(HelpText, descstartcoord);
	
	waitforkey();
	ClearRect(leftboxrect);
	ClearRect(rightboxrect);
	ClearRect(descboxrect);
	ClearRect(weightboxrect);
	
	currentdrawpoint = ReturnTextCenterInRect(ArmoryStock, leftboxrect);
	currentdrawpoint.y = leftboxrect.top - 1;
	WHITE;
	setcursor(currentdrawpoint.x, currentdrawpoint.y);
	writestring(ArmoryStock);
	
	currentdrawpoint = ReturnTextCenterInRect(PersonalStock, rightboxrect);
	currentdrawpoint.y = rightboxrect.top - 1;
	WHITE;
	setcursor(currentdrawpoint.x, currentdrawpoint.y);
	writestring(PersonalStock);
	
	//BEGIN SEQUENCE!
	while(Continuing)
	{
		//PRINT THE PLAYER'S WEAPONS ON THE RIGHT SIDE FIRST.
		if(RedrawPersonalList)
		{
			ClearRect(rightboxrect);
			if(player->p_gizmos->objcnt < 1){
				currentdrawpoint = ReturnTextCenterInRect(RightOutText, rightboxrect);
				LRED;
				setcursor(currentdrawpoint.x, currentdrawpoint.y);
				writestring(RightOutText);
			}
			else{
				
				ArmoryDrawGizmoList(player, globals.armorygizmolist, player->p_gizmos, rightboxrect, CurrentRightLocalSelection, CurrentRightScrollOffset, !CursorOnArmory, false);
			}
			RedrawPersonalList = false;
		}
		if(RedrawArmoryList)
		{
			ClearRect(leftboxrect);
			ArmoryDrawGizmoList(player, globals.armorygizmolist, player->p_gizmos, leftboxrect, CurrentLeftLocalSelection, CurrentLeftScrollOffset, CursorOnArmory, true);
			RedrawArmoryList = false;
		}
		if(RedrawWeight)
		{
			player->weight = (float)fabs(player->weight);
			ClearRect(weightboxrect);
			sprintf(WeightString, "Current Weight: %2.2f lbs - MAX WEIGHT: %2.2f lbs", player->weight, MAX_PLAYER_WEIGHT);
			currentdrawpoint = ReturnTextCenterInRect(WeightString,weightboxrect);
			LBLUE;
			setcursor(currentdrawpoint.x, currentdrawpoint.y);
			printf("Current Weight: ");
			if(player->weight > (3.0 * MAX_PLAYER_WEIGHT) / 4.0){
				LRED;
			}
			else if (player->weight > MAX_PLAYER_WEIGHT / 2.0){
				LYELLOW;
			}
			else{
				LGREEN;
			}
			printf("%2.2f lbs", player->weight);
			LBLUE;
			printf(" - MAX WEIGHT: ");
			LRED;
			printf("%2.2f lbs", MAX_PLAYER_WEIGHT);
			RedrawWeight = false;
		}
		if(RedrawDescription)
		{
			if(CursorOnArmory){
				TempObject.armorygizmo.id = CurrentLeftSelectionNumber;
				//TempObject.weapon.id = 7;
				CurrentObject= findObject(globals.armorygizmolist, &TempObject, CompareArmoryGizmo);
			}
			else{
				TempObject.gizmo.index = CurrentRightSelectionNumber;
				CurrentObject = findObject(player->p_gizmos, &TempObject, CompareGizmoIndex);
				if(!CurrentObject){
					CurrentGizmo = NULL;
				}
				else{
					TempObject.armorygizmo.isgrenade = CurrentObject->gizmo.isgrenade;
					if(CurrentObject->gizmo.isgrenade){
						TempObject.armorygizmo.type = CurrentObject->gizmo.unit.grenade.grenadetype;
					}
					else{
						TempObject.armorygizmo.type = CurrentObject->gizmo.unit.device.devicetype;
					}
					CurrentObject = findObject(globals.armorygizmolist, &TempObject, CompareArmoryGizmoBothTypes);
				}
			}
			
			if(!CurrentObject){
				CurrentGizmo = NULL;
			}
			else{
				CurrentGizmo = &CurrentObject->armorygizmo;
			}
			
			if(ResetDescriptionPages)
			{
				CurrentDescPage = 0;
				ResetDescriptionPages = false;
			}
			
			ClearRect(descboxrect);
			
			
			
			LBLUE;
			DescPages = ArmoryDrawDescription(CurrentGizmo->desc, descboxrect, CurrentDescPage);
			if(CurrentDescPage + 1 > DescPages){
				ResetDescriptionPages = true;
			}
			
			//CHECK IF WEAPON IS OUT OF STOCK. IF SO, SUPERIMPOSE THE "OUT OF STOCK" THING.
			if(!CurrentGizmo->isavailable){
				currentdrawpoint = ReturnTextCenterInRect(OutOfStockText, descboxrect);
				LRED;
				setcursor(currentdrawpoint.x, currentdrawpoint.y);
				writestring(OutOfStockText);
			}
			
			RedrawDescription = false;
			
			
		}
		key = waitforVkey();
		switch(key)
		{
		case VK_ESCAPE:
			Continuing = false;
			break;
		case 219:
			RedrawDescription = true;
			CurrentDescPage--;
			CurrentDescPage = CapInclusive(CurrentDescPage, 0, DescPages - 1);
			break;
			
		case 221:
			RedrawDescription = true;
			CurrentDescPage++;
			CurrentDescPage = CapInclusive(CurrentDescPage, 0, DescPages - 1);
			break;
		case 34:
			//ONE PAGE DOWN. WILL ATTEMPT TO MOVE THE SCROLL OFFSET BY THE MAX ITEMS IN THE BOX. IF IT CANNOT, THEN IT WILL MOVE
			//THE SCROLL OFFSET AS FAR AS IT CAN, AND WILL DROP THE LOCALOFFSET TO THE REMAINDER OF THE DISTANCE.
			if(CursorOnArmory){
				if(CurrentLeftScrollOffset + MaxTopItemsToDisplay >= globals.armorygizmolist->objcnt - MaxTopItemsToDisplay){
					int cap = (MaxTopItemsToDisplay > globals.armorygizmolist->objcnt ? globals.armorygizmolist->objcnt : MaxTopItemsToDisplay);
					if(MaxTopItemsToDisplay > globals.armorygizmolist->objcnt){
					}
					else{
						CurrentLeftScrollOffset = CurrentLeftSelectionNumber = globals.armorygizmolist->objcnt - MaxTopItemsToDisplay;
					}
					CurrentLeftLocalSelection = cap - 1;
					CurrentLeftSelectionNumber += cap - 1;
					CurrentLeftSelectionNumber = CapInclusive(CurrentLeftSelectionNumber, 0, globals.armorygizmolist->objcnt - 1);
				}
				else{
					
					CurrentLeftScrollOffset += MaxTopItemsToDisplay;
					CurrentLeftSelectionNumber += MaxTopItemsToDisplay;
					CurrentLeftSelectionNumber = CapInclusive(CurrentLeftSelectionNumber, 0, globals.armorygizmolist->objcnt - 1);
				}
				RedrawArmoryList = true;
			}
			else{
				if(CurrentRightScrollOffset + MaxTopItemsToDisplay >= player->p_gizmos->objcnt - MaxTopItemsToDisplay){
					int cap = (MaxTopItemsToDisplay > player->p_gizmos->objcnt ? player->p_gizmos->objcnt : MaxTopItemsToDisplay);
					if(MaxTopItemsToDisplay > player->p_gizmos->objcnt){
					}
					else{
						CurrentRightScrollOffset = CurrentRightSelectionNumber = player->p_gizmos->objcnt - MaxTopItemsToDisplay;
					}
					CurrentRightLocalSelection = cap - 1;
					CurrentRightSelectionNumber += cap - 1;
					CurrentRightSelectionNumber = CapInclusive(CurrentRightSelectionNumber, 0, player->p_gizmos->objcnt - 1);
				}
				else{
					
					CurrentRightScrollOffset += MaxTopItemsToDisplay;
					CurrentRightSelectionNumber += MaxTopItemsToDisplay;
					CurrentRightSelectionNumber = CapInclusive(CurrentRightSelectionNumber, 0, player->p_gizmos->objcnt - 1);
				}
				RedrawPersonalList = true;
			}
			RedrawDescription = true;
			ResetDescriptionPages = true;
			break;
		case 33:
				if(CursorOnArmory){
				if(CurrentLeftScrollOffset < 1){
					CurrentLeftScrollOffset = CurrentLeftSelectionNumber = 0;
					CurrentLeftLocalSelection = 0;
					CurrentLeftSelectionNumber = 0;
				}
				else{
					CurrentLeftScrollOffset -= MaxTopItemsToDisplay;
					CurrentLeftSelectionNumber -= MaxTopItemsToDisplay;
					CurrentLeftScrollOffset = CapInclusive(CurrentLeftScrollOffset, 0, globals.armorygizmolist->objcnt - 1);
					CurrentLeftSelectionNumber = CapInclusive(CurrentLeftSelectionNumber, 0, globals.armorygizmolist->objcnt - 1);
				}
				RedrawArmoryList = true;
			}
			else{
				if(CurrentRightScrollOffset < 1){
					CurrentRightScrollOffset = CurrentRightSelectionNumber = 0;
					CurrentRightLocalSelection = 0;
					CurrentRightSelectionNumber = 0;
				}
				else{
					CurrentRightScrollOffset -= MaxTopItemsToDisplay;
					CurrentRightSelectionNumber -= MaxTopItemsToDisplay;
					CurrentRightScrollOffset = CapInclusive(CurrentRightScrollOffset, 0, player->p_gizmos->objcnt - 1);
					CurrentRightSelectionNumber = CapInclusive(CurrentRightSelectionNumber, 0, player->p_gizmos->objcnt - 1);
				}
				RedrawPersonalList = true;
			}
			RedrawDescription = true;
			ResetDescriptionPages = true;
			break;
		case VK_UP:
			if(CursorOnArmory)
			{
				
				CurrentLeftSelectionNumber--;
				CurrentLeftSelectionNumber = CapInclusive(CurrentLeftSelectionNumber, 0, globals.armorygizmolist->objcnt - 1);
				//NOW CHECK IF WE NEED TO SCREW WITH THE SCROLL OFFSET.
				if(CurrentLeftLocalSelection > 0){
					CurrentLeftLocalSelection--;
				}
				else if (CurrentLeftScrollOffset > 0){ 
					CurrentLeftScrollOffset--;
				}
				RedrawArmoryList = true;
			}
			else{
				if(player->p_gizmos->objcnt < 1){
					break;
				}
				
				CurrentRightSelectionNumber--;
				CurrentRightSelectionNumber = CapInclusive(CurrentRightSelectionNumber, 0, player->p_gizmos->objcnt - 1);
				//NOW CHECK IF WE NEED TO SCREW WITH THE SCROLL OFFSET.
				if(CurrentRightLocalSelection > 0){
					CurrentRightLocalSelection--;
				}
				else if (CurrentRightScrollOffset > 0){ 
					CurrentRightScrollOffset--;
				}
				RedrawPersonalList = true;
			}
			RedrawDescription = true;
			ResetDescriptionPages = true;
			break;
		case VK_DOWN:
			if(CursorOnArmory)
			{
				
				CurrentLeftSelectionNumber++;
				CurrentLeftSelectionNumber = CapInclusive(CurrentLeftSelectionNumber, 0, globals.armorygizmolist->objcnt - 1);
				//NOW CHECK IF WE NEED TO SCREW WITH THE SCROLL OFFSET.
				if(CurrentLeftLocalSelection < MaxTopItemsToDisplay - 1 && CurrentLeftLocalSelection < globals.armorygizmolist->objcnt){
					CurrentLeftLocalSelection++;
				}
				else if (CurrentLeftScrollOffset < (globals.armorygizmolist->objcnt) - MaxTopItemsToDisplay){ 
					CurrentLeftScrollOffset++;
				}
				RedrawArmoryList = true;
			}
			else{
				if(player->p_gizmos->objcnt < 1){
					break;
				}
				
				CurrentRightSelectionNumber++;
				CurrentRightSelectionNumber = CapInclusive(CurrentRightSelectionNumber, 0, player->p_gizmos->objcnt - 1);
				//NOW CHECK IF WE NEED TO SCREW WITH THE SCROLL OFFSET.
				if(CurrentRightLocalSelection < MaxTopItemsToDisplay - 1 && CurrentRightLocalSelection < player->p_gizmos->objcnt - 1){
					CurrentRightLocalSelection++;
				}
				else if (CurrentRightScrollOffset < player->p_gizmos->objcnt - MaxTopItemsToDisplay){ 
					CurrentRightScrollOffset++;
				}
				RedrawPersonalList = true;
			}
			RedrawDescription = true;
			ResetDescriptionPages = true;
			break;
		case VK_TAB:
			if(CursorOnArmory)
			{
				if(player->p_gizmos->objcnt > 0){
					playMIXsound(ARMORY_SOUND_SELECT);
					CursorOnArmory = false;
					RedrawArmoryList = true;
					RedrawPersonalList = true;
					RedrawDescription = true;
					ResetDescriptionPages = true;
				}
			}
			else{
				playMIXsound(ARMORY_SOUND_SELECT);
				CursorOnArmory = true;
				RedrawArmoryList = true;
				RedrawPersonalList = true;
				RedrawDescription = true;
				ResetDescriptionPages = true;
			}
			break;
		case VK_RETURN: //THE BIG ONE!
			if(CursorOnArmory){ //THEN WE GIVE TO THE PLAYER.
				//FIND OUT INFO ABOUT THE WEAPON.
				TempObject.armorygizmo.id = CurrentLeftSelectionNumber;
				CurrentObject= findObject(globals.armorygizmolist, &TempObject, CompareArmoryGizmo);
				//IF OUT, IF NOT CLEARED, OR IF TOO HEAVY, DON'T GIVE IT UP.
				if(!CurrentObject->armorygizmo.isavailable || player->weight + CurrentObject->armorygizmo.weight > MAX_PLAYER_WEIGHT){
				}
				else{
					if(CurrentObject->armorygizmo.isgrenade){
						if(DoesListHaveGizmoType(CurrentObject->armorygizmo.type, player->p_gizmos, true) && !CurrentObject->armorygizmo.canhavemultiple){
							SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_SMALL, 1, TRUE);
							StaticDialogBox("Rationed", "This Technology Is Rationed To One Unit Per Operative.", "Press ENTER", &globals.dboxparams);
						}
						else{
							GiveGrenade(player, CurrentObject->armorygizmo.type);
							playMIXsound(ARMORY_SOUND_REMOVEWEAPON);
							player->weight += CurrentObject->armorygizmo.weight;
							RedrawWeight = true;
							RedrawPersonalList = true;
						}
					}
					else{
						if(DoesListHaveGizmoType(CurrentObject->armorygizmo.type, player->p_gizmos, false) && !CurrentObject->armorygizmo.canhavemultiple){
							SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_SMALL, 1, TRUE);
							StaticDialogBox("Rationed", "This Technology Is Rationed To One Unit Per Operative.", "Press ENTER", &globals.dboxparams);
						}
						else{
							GiveGizmo(player, CurrentObject->armorygizmo.type);
							playMIXsound(ARMORY_SOUND_REMOVEWEAPON);
							player->weight += CurrentObject->armorygizmo.weight;
							RedrawWeight = true;
							RedrawPersonalList = true;
						}
					}
					
					
					
				}
			}
			else{ //THEN WE TAKE AWAY FROM THE PLAYER.
				TempObject.gizmo.index = CurrentRightSelectionNumber;
				CurrentObject = findObject(player->p_gizmos, &TempObject, CompareGizmoIndex);
				if(CurrentObject){
					TempObject.armorygizmo.isgrenade = CurrentObject->gizmo.isgrenade;
					if(TempObject.armorygizmo.isgrenade){
						TempObject.armorygizmo.type = CurrentObject->gizmo.unit.grenade.grenadetype;
					}
					else{
						TempObject.armorygizmo.type = CurrentObject->gizmo.unit.device.devicetype;
					}
					CurrentObjectTwo = findObject(globals.armorygizmolist, &TempObject, CompareArmoryGizmoBothTypes);
					
					if(CurrentRightSelectionNumber == player->p_gizmos->objcnt - 1)
					{
						CurrentRightSelectionNumber--;
						CurrentRightLocalSelection--;
					}
					player->weight -= CurrentObjectTwo->armorygizmo.weight;
					playMIXsound(ARMORY_SOUND_GETWEAPON);
					deleteObject(player->p_gizmos, CurrentObject);
					ReorderGizmoListIndex(player->p_gizmos);
					RedrawPersonalList = true;
					RedrawWeight = true;
					RedrawDescription = true;
					ResetDescriptionPages = true;
					if(player->p_gizmos->objcnt < 1){
						CurrentRightSelectionNumber = 0;
						CurrentRightScrollOffset = 0;
						CurrentRightLocalSelection = 0;
						CursorOnArmory = true;
						RedrawArmoryList = true;
						
					}
				}
			}
			break;
		}
	}
	SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, true);
	StaticDialogBox("Technology Order", "This concludes your technology order. Your gizmo choices have been approved and are currently awaiting you at the Mobilization Center on Level II. You will recieve them prior to your operation. Please proceed directly to the Pharmaceutical Requisition Center to order any pharmaceuticals you may need for your operation.\nPress ENTER to Continue.", "Complete", &globals.dboxparams);
	cls(0);
	return;
}

int ArmoryDrawDescription(char* description, RECT boundingrect, int page)
{ //RETURNS TOTAL NUMBER OF DESCRIPTION PAGES!!!
	if(!description){
		return 0;
	}
	int boxheight = boundingrect.bottom - boundingrect.top;
	int boxwidth = (boundingrect.right - boundingrect.left);
	int DescLength = strlen(description);
	int Pages = 1;
	int SrcChar = 0;
	
	int DstChar;
	char UpArrowChar = (char)24;
	char DnArrowChar = (char)25;
	POINT UpArrowDrawPoint;
	UpArrowDrawPoint.x = boundingrect.right - 1;
	UpArrowDrawPoint.y = boundingrect.top;
	POINT DnArrowDrawPoint;
	DnArrowDrawPoint.x = boundingrect.right - 1;
	DnArrowDrawPoint.y = boundingrect.bottom;
	
	RECT TextRect; //NOT THE SAME AS THE RECT PASSED IN!
	TextRect.left = boundingrect.left;
	TextRect.right = boundingrect.right - 2;
	TextRect.top = boundingrect.top;
	TextRect.bottom = boundingrect.bottom;
	
	int TextBoxWidth = TextRect.right - TextRect.left;
	int TextBoxHeight = TextRect.bottom - TextRect.top;
	
	int maxchars = TextBoxWidth * TextBoxHeight; //WE CHECK THE RealText AGAINST THIS AND MAKE CHANGES AS NECESSARY.
	char* TempText = (char*)malloc(sizeof(char) * ((strlen(description)) + 1));
	strcpy(TempText, description);
	WrapStringToWidth(TempText, TextBoxWidth);
	int StringLines = GetNumberOfStringLines(TempText);
	free(TempText);
	Pages = (DescLength / maxchars) + 1; //PAGES SHOULD EQUAL THE MAXIMUM OF THE NUMBER OF LINES OR THE NUMBER OF CHARS.
	int LinePages = (StringLines / TextBoxHeight) + 1;
	if(LinePages > Pages){
		Pages = LinePages;
	}
	
	if(page + 1 > Pages){
		page = Pages - 1;
	}
	
	char* RealText = (char*)malloc(sizeof(char) * ((strlen(description)) + 1)); //NOT THE SAME AS THE TEXT PASSED IN!
	int linecounter = 0;
	int totalcounter;
	int pagecounter = 0;
	
	//MOVE THE SRCCHAR COUNTER TO THE RIGHT SPOT.
	for(pagecounter = 0; pagecounter < page; pagecounter++){
		for(DstChar = 0, totalcounter = 0; description[SrcChar] != '\0' && DstChar < maxchars && totalcounter < maxchars; DstChar++, SrcChar++, totalcounter++)
		{
			if(description[SrcChar] == '\n'){
				
				totalcounter += TextBoxWidth - linecounter;
				linecounter = 0;
			}
			else{
				linecounter++;
			}
		}
	}
	linecounter = 0;
	for(DstChar = 0, totalcounter = 0; description[SrcChar] != '\0' && DstChar < maxchars && totalcounter < maxchars; SrcChar++, DstChar++, totalcounter++)
	{
		if(description[SrcChar] == '\n'){
			
			totalcounter += TextBoxWidth - linecounter;
			linecounter = 0;
		}
		else{
			linecounter++;
		}
		RealText[DstChar] = description[SrcChar];
		
	}
	RealText[DstChar] = '\0';
	
	//WRAP AND DRAW THE STRING.
	WrapStringToWidth(RealText, TextBoxWidth);
	COORD StartCoord;
	StartCoord.X = (short)TextRect.left;
	StartCoord.Y = (short)TextRect.top;
	printinrect(RealText, StartCoord);
	
	//DRAW ARROWS IF NECESSARY.
	//IF CURRENT PAGE > 0, DRAW UP ARROW.
	//IF CURRENT PAGE + 1 < PAGES, DRAW DOWN ARROW.
	LBLUE;
	if(page > 0){
		setcursor(UpArrowDrawPoint.x, UpArrowDrawPoint.y);
		printf("%c", UpArrowChar);
	}
	
	if (page + 1 < Pages){
		setcursor(DnArrowDrawPoint.x, DnArrowDrawPoint.y);
		printf("%c", DnArrowChar);
	}
	
	free(RealText); //KILL IT!
	return Pages;
	
}

void AllWeaponsAvailable(PLIST weaponlist, PLIST ammolist)
{
	PLIST walker;
	int i;
	
	//WEAPONS FIRST
	walker = weaponlist;
	for (walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		walker->current->object.weapon.weaponavailable = TRUE;	
	}
	
	//THEN AMMO
	walker = ammolist;
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		//IF IT'S BULLETS....
		if (walker->current->object.ammo.type == AMMOTYPE_BULLET)
		{
			for(i = 0; i < walker->current->object.ammo.maxammotypes; i++)
			{
				walker->current->object.ammo.specialammo.bulletdata[i].ammoavailable = TRUE;
				walker->current->object.ammo.specialammo.bulletdata[i].roundsavailable = MAX_DEBUG_ROUNDS_AVAILABLE;
			}
		}
		//IF IT'S SHOTGUN SHELLS...
		if (walker->current->object.ammo.type == AMMOTYPE_SHOTGUNSHELL)
		{
			for(i = 0; i < walker->current->object.ammo.maxammotypes; i++)
			{
				walker->current->object.ammo.specialammo.shelldata[i].ammoavailable = TRUE;
				walker->current->object.ammo.specialammo.shelldata[i].roundsavailable = MAX_DEBUG_ROUNDS_AVAILABLE;
			}
		}
		if (walker->current->object.ammo.type == AMMOTYPE_EXPLOSIVE)
		{
			for(i = 0; i < walker->current->object.ammo.maxammotypes; i++)
			{
				walker->current->object.ammo.specialammo.explosivedata[i].ammoavailable = TRUE;
				walker->current->object.ammo.specialammo.explosivedata[i].roundsavailable = MAX_DEBUG_ROUNDS_AVAILABLE;
			}
		}
	}
	
}

void InitializeRealisticWeaponAvailability(PLIST weaponlist, PLIST ammolist, PLIST armorygizmolist)
{
	PLIST walker;
	
	//WEAPONS FIRST
	walker = weaponlist;
	for (walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		RandomizeWeaponAvailability(&walker->current->object.weapon);
	}
	
	//THEN AMMO
	walker = ammolist;
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		RandomizeAmmoAvailability(&walker->current->object.ammo);
	}
	
	walker = armorygizmolist;
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		if(!RandomPercentage(walker->current->object.armorygizmo.frequency)){
			walker->current->object.armorygizmo.isavailable = false;
		}
		else{
			walker->current->object.armorygizmo.isavailable = true;
		}
	}
	
	
}

void RandomizeWeaponAvailability(PWEAPON weapon)
{
	int randomizer;
	
	if (weapon->frequency >= 100){
		weapon->weaponavailable = TRUE;
		return;
	}
	
	randomizer = rand()%100;
	
	if (randomizer <= weapon->frequency){ //IF IT IS LESS THAN THE FREQUENCY (DRAW A NUMBER LINE IF YOU'RE CONFUSED!)
		weapon->weaponavailable = TRUE;
	}
	else{
		weapon->weaponavailable = FALSE;
	}
	
}
void RandomizeAmmoAvailability(PAMMO ammo)
{
	int randomizer;
	int i;
	
	//TO FIGURE OUT HOW MUCH AMMO TO SUPPLY, WE USE THE FOLLOWING EQUATION
	
	//MAXBASECONSTANT - RAND()%SUBTRACTCONSTANT * (100 - FREQUENCY)
	//I.E. MAXBASECONSTANT = 500, SUBTRACTCONSTANT = 6, FREQUENCY 75,
	//500 - RAND()%6(25);
	//500 - RAND()%150;
	//NOT BAD, EH? JUST SCALE THE SUBTRACTCONSTANTS UP BASED ON DIFFICULTY LEVEL!!!!
	
	/************************************************************
	; ALERT! DIFFICULTY LEVEL VARIABLE HERE!!!!! SUBTRACTCONSTANTS CAN BE SCALED USING VARIABLES TO MAKE AMMO SCARCER.
	************************************************************/
	
	
	
	switch (ammo->type){
	case AMMOTYPE_BULLET:
		for (i = 0; i < ammo->maxammotypes; i++)
		{
			if (ammo->specialammo.bulletdata[i].frequency >= 100){
				ammo->specialammo.bulletdata[i].ammoavailable = TRUE;
				ammo->specialammo.bulletdata[i].roundsavailable = MAX_BASE_BULLETS;
				continue;
			}
			randomizer = rand()%100;
			if (randomizer <= ammo->specialammo.bulletdata[i].frequency){
				ammo->specialammo.bulletdata[i].ammoavailable = TRUE;
			}
			else{
				ammo->specialammo.bulletdata[i].ammoavailable = FALSE;
			}
			
			ammo->specialammo.bulletdata[i].roundsavailable = MAX_BASE_BULLETS - (rand()%(SUBTRACTCONSTANT_BULLETS * (100 - ammo->specialammo.bulletdata[i].frequency)));
			if (ammo->specialammo.bulletdata[i].roundsavailable <= 0){
				ammo->specialammo.bulletdata[i].ammoavailable = FALSE;
			}
		}
		break;
		
	case AMMOTYPE_SHOTGUNSHELL:
		for (i = 0; i < ammo->maxammotypes; i++)
		{
			if (ammo->specialammo.shelldata[i].frequency >= 100){
				ammo->specialammo.shelldata[i].ammoavailable = TRUE;
				ammo->specialammo.shelldata[i].roundsavailable = MAX_BASE_SHELLS;
				continue;
			}
			randomizer = rand()%100;
			if (randomizer <= ammo->specialammo.shelldata[i].frequency){
				ammo->specialammo.shelldata[i].ammoavailable = TRUE;
			}
			else{
				ammo->specialammo.shelldata[i].ammoavailable = FALSE;
			}
			
			ammo->specialammo.shelldata[i].roundsavailable = MAX_BASE_SHELLS - (rand()%(SUBTRACTCONSTANT_SHELLS * (100 - ammo->specialammo.shelldata[i].frequency)));
			if (ammo->specialammo.shelldata[i].roundsavailable <= 0){
				ammo->specialammo.shelldata[i].ammoavailable = FALSE;
			}
		}
		break;
		
	case AMMOTYPE_EXPLOSIVE:
		for (i = 0; i < ammo->maxammotypes; i++)
		{
			if (ammo->specialammo.explosivedata[i].frequency >= 100){
				ammo->specialammo.explosivedata[i].ammoavailable = TRUE;
				ammo->specialammo.explosivedata[i].roundsavailable = MAX_BASE_EXPLOSIVE;
				continue;
			}
			randomizer = rand()%100;
			if (randomizer <= ammo->specialammo.explosivedata[i].frequency){
				ammo->specialammo.explosivedata[i].ammoavailable = TRUE;
			}
			else{
				ammo->specialammo.explosivedata[i].ammoavailable = FALSE;
			}
			
			ammo->specialammo.explosivedata[i].roundsavailable = MAX_BASE_EXPLOSIVE - (rand()%(SUBTRACTCONSTANT_EXPLOSIVE * (100 - ammo->specialammo.explosivedata[i].frequency)));
			if (ammo->specialammo.explosivedata[i].roundsavailable <= 0){
				ammo->specialammo.explosivedata[i].ammoavailable = FALSE;
			}
		}
		break;
	}
	
}


//--------------------------------------------------------------------------------------------
// Function Name - GiveAtomicDevice
//
// Description   - Assigns the atomic detcode to the player
//
// Return Type   - void 
//
// Arguments     - PPLAYER player ( Player structure pointer )
//
// Author        - Charles Cox
// Date Modified - 04/24/2000
//--------------------------------------------------------------------------------------------
void GiveAtomicDevice(PPLAYER player)
{
	int i;
	char buf[3000];
	

	//IF ALREADY HAS A BOMB...
	if(player->currentmissioninfo.specifics.cautery.hasbomb){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		sprintf(buf, "You are already assigned DSA Atomic Munition #%d. Your access request to the AMV is DENIED.", player->currentmissioninfo.specifics.cautery.bombnumber);
		StaticDialogBox("Bomb Assigned", buf, "Press ENTER.", &globals.dboxparams);
		return;
	}
	//IF TOO HEAVY
	if(ATOMIC_BOMB_WEIGHT + player->weight > MAX_PLAYER_WEIGHT){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		sprintf(buf, "Agent %s, your current equipment weight stands at %2.2f lbs. A DSA Atomic Munition is exactly %2.2f lbs. This would put you over your weight maximum. Please return when you have room available for this munition.", player->lname, player->weight, ATOMIC_BOMB_WEIGHT);
		StaticDialogBox("Bomb Weight Prohibitive", buf, "Press ENTER.", &globals.dboxparams);
		return;
	}
	//CHECK CODE FOR SEQUENCE.
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
	sprintf(buf, "Agent %s:\nYou are authorized for DSA Atomic Munition #%d. You must realize IMMEDIATELY that you will be in control of an ATOMIC WEAPON. You are solely responsible for the proper use, and consequences of improper use, of this device. To verify that you understand these terms, please enter your access code in the following screen.", player->lname, player->sartumission->missionspecificone);
	StaticDialogBox("Atomic Device Retrieval", buf, "Level VI", &globals.dboxparams);

	if(!CheckAccessCode(player)){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		StaticDialogBox("PERMISSION DENIED", "Your Access Code Is Incorrect. REQUEST FOR ATOMIC MUNITION IS DENIED.", "Press ENTER", &globals.dboxparams);
		return;
	}
	//GENERATING ATOMIC CODE.
	for (i = 0; i < MAX_ATOMIC_CODE_DIGITS; i++){
		player->currentmissioninfo.specifics.cautery.detcode[i] = RandomAlphaNumeric();
	}
	player->currentmissioninfo.specifics.cautery.detcode[i-1] = '\0';

	player->currentmissioninfo.specifics.cautery.bombnumber = player->sartumission->missionspecificone;
	player->currentmissioninfo.specifics.cautery.hasbomb = true;
	player->currentmissioninfo.specifics.cautery.bombactive = false;

	playMIXsound(ATOMIC_RECIEVE_SOUND);
	cls(0);
	sprintf(buf, "Enclosed is the ACCESS CODE to ARM THE ATOMIC DEVICE.\n\n%s\n\nTHIS CODE WILL NEVER BE SHOWN TO YOU AGAIN AFTER THIS MOMENT. MEMORIZE THIS CODE. MEMORIZE THIS CODE.", player->currentmissioninfo.specifics.cautery.detcode);
	StaticDialogBox("DETONATION CODES", buf, "DESTROY AFTER READING", &globals.dboxparams);

	//ADD ON THE WEIGHT.
	player->weight += ATOMIC_BOMB_WEIGHT;
}

void ClearPlayerStats(PPLAYER player)
{
	memset(&player->medals, 0, sizeof(player->medals));
	memset(&player->history, 0, sizeof(player->history));
	memset(&player->psychskill, 0, sizeof(player->psychskill));
	
	player->psychskill.psych.agression = 1;
	player->psychskill.psych.calmness = 1;
	player->psychskill.psych.intelligence = 1;
	player->psychskill.psych.reflex = 1;
	player->psychskill.psych.willpower = 1;
	
	player->psychskill.skills.electronics = 2;
	player->psychskill.skills.mechanical = 2;
	player->psychskill.skills.meleecombatNOweapon = 2;
	player->psychskill.skills.meleecombatWITHweapon = 2;
	player->psychskill.skills.firearms = 2;
	player->psychskill.skills.snipingcombat = 2;
	player->psychskill.skills.stealthcombat = 2;
}

void CreateNewPlayer(PPLAYER player)
{
	player->sealedflag = OPERATIVE_OPEN_FILE;
	player->status = OPERATIVE_STATUS_ACTIVE;
	player->rank = 0;
	ClearPlayerStats(player);
	//	char numbuf[5];
	char buf[255];
	
	while(1){
		SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 20, TRUE);
		StringDialogBox("TSSI Building", "Welcome to the Thiersen Strategic Studies Institute Clearance Center.\nPlease Enter Your Last Name And Press ENTER When Finished.", "Clearance Center", player->lname, 20, &globals.dboxparams);
		cls(0);
		StringDialogBox("TSSI Building", "Thank You.\nPlease Enter Your First Name And Press ENTER When Finished.", "Clearance Center", player->fname, 20, &globals.dboxparams);
		cls(0);
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_LARGE, 20, TRUE);
		sprintf(buf, "You Have Entered The Following As Your Name:\n%s %s\nIf This Is Correct, Press Y. Otherwise, press N.", player->fname, player->lname);
		if(YesNoDialogBox("Clearance", buf, "Confirmation", &globals.dboxparams)){
			cls(0);
			break;
		}
		cls(0);
	}
	
	AssignNewAgentID(player);
	
	SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_LARGE, 20, TRUE);
	sprintf(buf, "Welcome, Agent %s.\nYou have been assigned DSA ID #%d.\nIt is not necessary to memorize this number. It is for DSA tracking purposes only.\nPress ENTER to continue.", player->lname, player->id);
	StaticDialogBox("Welcome To", buf, "The Defense Security Agency",&globals.dboxparams);
	cls(0);
	StaticDialogBox("Code", "You will now be required to input a personal 4-digit security code. Please enter a code you will be able to memorize. Without the code, you will be unable to re-enter the Thiersen Strategic Studies Institute. Press ENTER to begin.", "Clearance", &globals.dboxparams);
	cls(0);
	SetAccessCode(player);
	cls(0);
	SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_LARGE, 20, FALSE);
	StaticDialogBox("Code", "Thank you. You are now cleared to enter the Thiersen Strategic Studies Institute. All levels beginning with Level I and ending with Level VII are accessible 24 hours a day. Please speak with a Level I Security Aegis if you have further questions.\nPress ENTER to continue.", "Stored", &globals.dboxparams);
	cls(0);
	StaticDialogBox("Welcome To", "For DSA records and identification, specific questions are necessary. Please be truthful in your answers. Failure to respond truthfully to these questions may result in operative fatalities. We rely on correct data.\nPress ENTER to begin.", "The Agency", &globals.dboxparams);
	cls(0);
	StaticDialogBox("Military/Intelligence", "We are aware of your previous military and intelligence experience, though we have some specific questions, beginning with your unit. What were your duties during your previous experience?\nPress ENTER to select answer.", "History", &globals.dboxparams);
	SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_MEDIUM, 20, FALSE);
	cls(0);
	GetPlayerMilitaryHistory(player);
	SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_MEDIUM, 20, FALSE);
	StaticDialogBox("Codename", "We've got only one final question before you're assigned. We need a codename that we can refer to you by.\nPress ENTER to begin.", "Entry", &globals.dboxparams);
	cls(0);
	StringDialogBox("Codename", "Please Enter A Codename, and Press ENTER When Finished.", "Entry", player->codename, 30, &globals.dboxparams);
	cls(0);
	SavePlayer(player);
	sprintf(buf, "Thank you, Agent %s. We're assigning you the initial rank of Field Operative Recruit. Report to Level V SARTU center for available missions. Good luck.\nPress ENTER to continue.", player->lname);
	StaticDialogBox("Data Input", buf, "Complete", &globals.dboxparams);
	cls(0);
	//SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_LARGE, 0, FALSE);
	LoopSong(globals.musiclist.songs[TSSI_SONG]);
	ComputerPrintOutString("We've asked you to join us not only because we believe you a useful addition to the talent we have here; but because we believe that you are devoted to the American way of life, the American people, and the American government. By joining the Defense Security Agency, you pledge that you will do whatever is necessary to defend our country; and you will assist in any way possible to insure that our government remains stable, that our secrets remain protected, and that the American people remain positive and safe. You have joined a brotherhood of secrecy and trust. You now hold a level of responsibility far above anything you have ever undertaken before. You are now a member of the most elite group of specialists in the world.\n\nYou are now a member of The Agency.", 30, "Press ENTER to Begin Your Career.");
	SartuDailySession(player, GetNumberOfMissionsPerDay(player));
	SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
	StaticDialogBox("First Day", "Welcome to The Agency. You might want to take the next few days to get acquainted with your new surroundings. We highly recommend you stop by the training center on Level V, as well as reviewing your documentation to learn how to use your interfaces. Feel free to roam as you wish. When you're ready to begin your missions, enter the Network Terminal Center on Level V. Good Luck, And Good Hunting.", "Press ENTER.", &globals.dboxparams);
}

void AssignNewAgentID(PPLAYER player)
{
	char numbuf[20];
	do
	{
		for (int i = 0; i < 5; i++){
			numbuf[i] = RandomNumeric();
		}
		if (numbuf[0] < 1){
			numbuf[0] ++;
		}
		numbuf[i] = '\0';
		//THIS BECOMES THE PLAYER'S NEW ID CODE.
		player->id = atoi(numbuf);
	}while(DoesIDExist(player));
}

void GetPlayerMilitaryHistory(PPLAYER player)
{
	char buf[255];
	int result;
	char* typestrings[6] = { "Firearms Instructor", "Sniper", "Dark Ops", "Close-In Fighting Instructor", "Science and Technology Branch", "Psychological Operations"};
	char* specstrings[9] = {"Hand Weapons", "Handguns", "Sub-Machine Guns", "Rifles", "Shotguns", "Special/Experimental Weapons", "Explosives", "Mechanics", "Electronics"};
	PLIST types = createList();
	POTYPE temptypestrings[6];
	POTYPE tempspecstrings[9];
	
	for(int i = 0; i < 6; i++){
		temptypestrings[i].string = (char*)malloc(sizeof(char) * (strlen(typestrings[i]) + 1));
		strcpy(temptypestrings[i].string, typestrings[i]);
		temptypestrings[i].dblistitem.id = i;
		addtoList(types, temptypestrings[i], CompareDbitemID);
	}
	switch((result = SimpleListDialogBox("Military/Intelligence", "History", types, &globals.dboxparams, CreateStringDBListItem)))
	{
		//CHANGE STATS HERE!!!!
	case 0:
		player->psychskill.psych.agression = 2;
		player->psychskill.psych.reflex = 2;
		player->psychskill.psych.willpower = 1;
		player->psychskill.psych.intelligence = 2;
		player->psychskill.psych.calmness = 1;
		player->psychskill.skills.firearms = 4;
		player->psychskill.skills.snipingcombat = 3;
		player->psychskill.skills.stealthcombat = 2;
		player->psychskill.skills.meleecombatNOweapon = 2;
		player->psychskill.skills.meleecombatWITHweapon = 3;
		player->psychskill.skills.electronics = 3;
		player->psychskill.skills.mechanical = 3;
		break;
	case 1:
		player->psychskill.psych.agression = 1;
		player->psychskill.psych.reflex = 2;
		player->psychskill.psych.willpower = 1;
		player->psychskill.psych.intelligence = 2;
		player->psychskill.psych.calmness = 3;
		player->psychskill.skills.firearms = 4;
		player->psychskill.skills.snipingcombat = 4;
		player->psychskill.skills.stealthcombat = 2;
		player->psychskill.skills.meleecombatNOweapon = 2;
		player->psychskill.skills.meleecombatWITHweapon = 2;
		player->psychskill.skills.electronics = 2;
		player->psychskill.skills.mechanical = 3;
		break;
	case 2:
		player->psychskill.psych.agression = 1;
		player->psychskill.psych.reflex = 1;
		player->psychskill.psych.willpower = 2;
		player->psychskill.psych.intelligence = 2;
		player->psychskill.psych.calmness = 2;
		player->psychskill.skills.firearms = 2;
		player->psychskill.skills.snipingcombat = 3;
		player->psychskill.skills.stealthcombat = 3;
		player->psychskill.skills.meleecombatNOweapon = 3;
		player->psychskill.skills.meleecombatWITHweapon = 3;
		player->psychskill.skills.electronics = 3;
		player->psychskill.skills.mechanical = 3;
		break;
	case 3:
		player->psychskill.psych.agression = 3;
		player->psychskill.psych.reflex = 2;
		player->psychskill.psych.willpower = 1;
		player->psychskill.psych.intelligence = 1;
		player->psychskill.psych.calmness = 1;
		player->psychskill.skills.firearms = 3;
		player->psychskill.skills.snipingcombat = 2;
		player->psychskill.skills.stealthcombat = 3;
		player->psychskill.skills.meleecombatNOweapon = 4;
		player->psychskill.skills.meleecombatWITHweapon = 4;
		player->psychskill.skills.electronics = 2;
		player->psychskill.skills.mechanical = 2;
		break;
	case 4:
		player->psychskill.psych.agression = 1;
		player->psychskill.psych.reflex = 1;
		player->psychskill.psych.willpower = 1;
		player->psychskill.psych.intelligence = 3;
		player->psychskill.psych.calmness = 3;
		player->psychskill.skills.firearms = 3;
		player->psychskill.skills.snipingcombat = 2;
		player->psychskill.skills.stealthcombat = 2;
		player->psychskill.skills.meleecombatNOweapon = 2;
		player->psychskill.skills.meleecombatWITHweapon = 2;
		player->psychskill.skills.electronics = 4;
		player->psychskill.skills.mechanical = 4;
		break;
	case 5:
		player->psychskill.psych.agression = 1;
		player->psychskill.psych.reflex = 3;
		player->psychskill.psych.willpower = 3;
		player->psychskill.psych.intelligence = 3;
		player->psychskill.psych.calmness = 1;
		player->psychskill.skills.firearms = 3;
		player->psychskill.skills.snipingcombat = 2;
		player->psychskill.skills.stealthcombat = 3;
		player->psychskill.skills.meleecombatNOweapon = 2;
		player->psychskill.skills.meleecombatWITHweapon = 2;
		player->psychskill.skills.electronics = 3;
		player->psychskill.skills.mechanical = 2;
		break;
	default:
		break;
	}
	sprintf(buf, "DSA Control CONFIRMS your prior history in %s. We are now interested in any particular weapon preference or specialization you may have. You may choose only one.\n Press ENTER when ready.", typestrings[result]);
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_LARGE, 20, FALSE);
	StaticDialogBox("History", buf, "Confirmed", &globals.dboxparams);
	SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_MEDIUM, 20, FALSE);
	
	freeList(types);
	types = createList();
	
	for(int k = 0; k < 9; k++){
		tempspecstrings[k].string = (char*)malloc(sizeof(char) * (strlen(specstrings[k]) + 1));
		strcpy(tempspecstrings[k].string, specstrings[k]);
		tempspecstrings[k].dblistitem.id = k;
		addtoList(types, tempspecstrings[k], CompareDbitemID);
	}
	cls(0);
	switch((result = SimpleListDialogBox("Weapons", "Specialization", types, &globals.dboxparams, CreateStringDBListItem)))
	{
		//CHANGE SPECIALIZATIONS HERE!!!!
	case 0:
		player->psychskill.specializations.handweapons = 1;
		break;
	case 1:
		player->psychskill.specializations.handguns = 1;
		break;
	case 2:
		player->psychskill.specializations.submachineguns = 1;
		break;
	case 3:
		player->psychskill.specializations.rifles = 1;
		break;
	case 4:
		player->psychskill.specializations.shotguns = 1;
		break;
	case 5:
		player->psychskill.specializations.specialweapons = 1;
		break;
	case 6:
		player->psychskill.specializations.explosives = 1;
		break;
	case 7:
		player->psychskill.specializations.mechanical = 1;
		break;
	case 8:
		player->psychskill.specializations.electronics = 1;
		break;
	default:
		break;
	}
	
	sprintf(buf, "DSA Control CONFIRMS your specialization in %s. Standby.\n Press ENTER to continue.", specstrings[result]);
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_LARGE, 20, FALSE);
	cls(0);
	StaticDialogBox("Specialization", buf, "Confirmed", &globals.dboxparams);
	freeList(types);
	player->numberofdaysincycle = 30;
	player->monthscompleted = 0;
	player->missionsleftthiscycle = 7;
	player->currentlymobilizing = false;
}

void SavePlayer(PPLAYER player)
{
	FILE*fp;
	char string[255];
	sprintf(string, "Agents\\%s%d.agt", player->lname,player->id);
	fp = fopen(string, "w+b");
	fwrite(player, sizeof(*player), 1, fp);
	fclose(fp);
}

bool LoadPlayer(char* filename, PPLAYER player)
{
	char string[255];
	sprintf(string, "Agents\\%s", filename);
	FILE* fp;
	fp = fopen(string, "rb");
	if (fp == NULL){
		return NULL;
	}
	if(!fread(player, sizeof(*player), 1 ,fp)){
		fclose(fp);
		return false;
	}
	fclose(fp);
	//NOW WE NEED TO SET A LOT OF CRAP TO NULL.
	player->ammo.cliplist = NULL;
	player->ammo.roundtypes = NULL;
	player->CurrentActionMode = 0;
	memset(&player->currentmissioninfo, 0, sizeof(player->currentmissioninfo));
	player->currentweapon = NULL;
	player->health = 100;
	
	player->lastlocation = NULL;
	player->p_gizmos = NULL;
	player->p_weapons = NULL;
	player->sartumission = NULL;
	memset(&player->drugs.drugtype, 0, sizeof(player->drugs.drugtype));

	player->currentlymobilizing = false;
	return true;
}

bool DoesIDExist(PPLAYER player)
{
	HANDLE searchhandle;
	WIN32_FIND_DATA FindFileData;
	
	char string[255];
	sprintf(string, "Agents\\%s%d.agt", player->lname,player->id);
	if ((searchhandle  = FindFirstFile(string, &FindFileData)) == INVALID_HANDLE_VALUE){
		FindClose(searchhandle);
		return false;
	}
	FindClose(searchhandle);
	return true;
}

int GetNumberOfAgentFiles()
{
	HANDLE handle;
	WIN32_FIND_DATA	finddata;
	int numberoffiles = 0;
	int returncode = 1;
	char path[255];
	strcpy(path, "Agents\\*.agt");
	
	handle = FindFirstFile(path, &finddata);
	while (handle != INVALID_HANDLE_VALUE && returncode != 0){
		returncode = FindNextFile(handle, &finddata);
		numberoffiles++;
	}
	FindClose(handle);
	return numberoffiles;
}


void InitializePlayerForCombat(PPLAYER player)
{
	player->health = 100;
	player->p_weapons = createList();
	player->currentweapon = NULL;
	player->p_gizmos = createList();
	player->ammo.cliplist = createList();
	player->ammo.roundtypes = createList();
	player->weight = 0.0;
	player->turndirection = TURN_DIRECTION_NORTH;
	player->currentmissioninfo.specifics.cautery.bombactive = false;
	player->currentmissioninfo.specifics.cautery.hasbomb = false;
	player->currentmissioninfo.cansnipe = TRUE;
	player->keycardbitfield = 0;
	memset(&(player->drugs), 0, sizeof(player->drugs));
}

void InitializeEnemyForCombat(PENEMY enemy)
{
	enemy->weaponlist = createList();
	enemy->currentweapon = NULL;
	enemy->ammolist.cliplist = createList();
	enemy->ammolist.roundtypes = createList();
	enemy->turndirection = rand()%MAX_TURN_DIRECTIONS;
}

void FreeEnemyFromCombat(PENEMY enemy)
{
	RemoveAllWeapons(enemy->weaponlist);
	enemy->currentweapon = NULL;
	freeList(enemy->ammolist.cliplist);
	free(enemy->ammolist.cliplist);
	freeList(enemy->ammolist.roundtypes);
	free(enemy->ammolist.roundtypes);
}

void FreePlayerFromCombat(PPLAYER player)
{
	RemoveAllPlayerWeapons(player);
	player->currentweapon = NULL;
	freeList(player->ammo.cliplist);
	freeList(player->ammo.roundtypes);
	freeList(player->p_gizmos);
	free(player->ammo.cliplist);
	free(player->ammo.roundtypes);
	free(player->p_gizmos);
	player->ammo.cliplist = NULL;
	player->ammo.roundtypes = NULL;
	player->p_gizmos = NULL;
}

void SetDefaultOptions()
{
}

DBLISTITEM CreateAgentNameListItem(PPOTYPE object)
{
	DBLISTITEM item;
	char string[255];
	char rankstr[255];
	//	char stringtwo[255];
	//	char namestring[255];
	//	char numberstring[255];
	//	int len = strlen(object->string);
	PLAYER player;
	//	int i;
	strcpy(string, object->string);
	
	LoadPlayer(string, &player);
	
	switch(player.rank){
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
		strcpy(rankstr, "ECOSSPECONE");
		break;
	default:
		strcpy(rankstr, "UNKNOWN");
		break;
	}
	
	
	if (!strcmp(object->string, "Create New Agent") || !strcmp (object->string, "Exit Game") || !strcmp(object->string, "CLOSE DOSSIERS")){
		item.string = (char*) malloc(sizeof(char)* strlen(object->string) + 1);
		item.id = 0;
		strcpy(item.string, object->string);
		return item;
	}
	//STRIP EXTENSION AND SPLIT STRING.
	
	if (!LoadPlayer(string, &player)){
		sprintf(string, "Unknown Agent - Error!");
		item.string = (char*) malloc(sizeof(char)* strlen(string) + 1);
		strcpy(item.string, string);
		return item;
	}
	else{
		if (player.sealedflag){
			sprintf(string, "SEALED FILE - AGENT RECORD CAUTERIZED");
		}
		else{
			sprintf(string, "%s %s - #%d - ", rankstr, player.lname, player.id);
			switch(player.status){
			case OPERATIVE_STATUS_ACTIVE:
				strcat(string, "Active");
				break;
			case OPERATIVE_STATUS_CYANIDED:
				strcat(string, "Self-Terminated");
				break;
			case OPERATIVE_STATUS_NOTOCHORDED:
				strcat(string, "Forcibly Terminated");
				break;
			case OPERATIVE_STATUS_RETIRED:
				strcat(string, "Retired");
				break;
			case OPERATIVE_STATUS_WHEREABOUTS_UNKNOWN:
				strcat(string, "Whereabouts Unknown");
				break;
			}
		}
		item.string = (char*) malloc(sizeof(char)* strlen(string) + 1);
		strcpy(item.string, string);
		return item;
	}
}

bool SelectPlayer(PPLAYER player, bool debug)
{
	PLIST filelist = createList();
	POTYPE* agents;
	POTYPE newop;
	POTYPE quit;
	HANDLE handle;
	WIN32_FIND_DATA finddata;
	char path[255];
	char buf[255];
	int agentcounter = 0;
	int returncode = 1;
	strcpy(path, "Agents\\*.agt");
	
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 0, TRUE);
	
	agents = (PPOTYPE) malloc(sizeof(POTYPE) * GetNumberOfAgentFiles());
	
	newop.string = (char*) malloc(sizeof(char) * (strlen("Create New Agent") + 1));
	strcpy(newop.string, "Create New Agent");
	addtoList(filelist, newop, CompareStringID);
	
	quit.string = (char*) malloc(sizeof(char) * (strlen("Exit Game") + 1));
	strcpy(quit.string, "Exit Game");
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
		SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_MEDIUM, 0, FALSE);
		cls(0);
		int returncodetwo = SimpleListDialogBox("Agent", "Login", filelist, &globals.dboxparams, CreateAgentNameListItem);
		if (returncodetwo == 0){
			cls(0);
			CreateNewPlayer(player);
			freeList(filelist);
			free(quit.string);
			free(newop.string);
			for(int freei = 0; freei < agentcounter; freei++){
				free(agents[freei].string);
			}
			free(agents);
			return true;
		}
		else if (returncodetwo == 1){
			freeList(filelist);
			free(quit.string);
			free(newop.string);
			for(int freei = 0; freei < agentcounter; freei++){
				free(agents[freei].string);
			}
			free(agents);
			return false;
		}
		else{
			cls(0);
			if(!LoadPlayer(ReturnStringListItemByNumber(returncodetwo + 1, filelist), player)){
				StaticDialogBox("Unable To Load Player", "There has been an error. The Agent could not be loaded from disk.", "Error", &globals.dboxparams);
			}
			else if (debug)
			{
				PlayerDataDebugFunctions(player);
			}
			else if (player->sealedflag){
				cls(0);
				SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 0, FALSE);
				StaticDialogBox("SEALED", "THIS FILE IS SEALED. NO DATA IS AVAILABLE.", "FILE", &globals.dboxparams);
			}
			else
			{
				switch(player->status){
				case OPERATIVE_STATUS_ACTIVE:
					if(!CheckAccessCode(player)){
						cls(0);
						SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 0, FALSE);
						StaticDialogBox("DENIED", "The code you have entered is invalid. You are NOT AUTHORIZED to access this Agent's File.\nPress ENTER to continue.", "DENIED", &globals.dboxparams);
					}
					else{
						SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 0, TRUE);
						SartuDailySession(player, rand()%7 + 3);
						ShutDownSartuForTheDay(player, &globals.sartudata);
						SartuDailySession(player, rand()%7 + 3);
						LoopSong(globals.musiclist.songs[TSSI_SONG]);
						sprintf(buf, "Welcome Back, Agent %s.\nYou are currently on Month %d.\n%d Days Remain, With %d Missions Remaining In This Month.\nPress ENTER to enter Operations Control.", player->lname, player->monthscompleted+ 1, player->numberofdaysincycle, player->missionsleftthiscycle);
						StaticDialogBox("Welcome Back To", buf, "The Agency", &globals.dboxparams);
						freeList(filelist);
						free(quit.string);
						free(newop.string);
						for(int freei = 0; freei < agentcounter; freei++){
							free(agents[freei].string);
						}
						free(agents);
						return true;
					}
					break;
				case OPERATIVE_STATUS_CYANIDED:
					cls(0);
					SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, FALSE);
					StaticDialogBox("Agent", "This Agent was forced to self-terminate in the field. You may not retrieve this Agent. You may, however, view this Agent's records in the Office of Records on Level III.", "Terminated", &globals.dboxparams);
					break;
				case OPERATIVE_STATUS_NOTOCHORDED:
					cls(0);
					SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, FALSE);
					StaticDialogBox("Agent", "This Agent was forcibly terminated after compromising DSA security protocols while in the field. You may not retrieve this Agent. You may, however, view this Agent's records in the Office of Records on Level III.", "Terminated", &globals.dboxparams);
					break;
				case OPERATIVE_STATUS_RETIRED:
					cls(0);
					SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, FALSE);
					StaticDialogBox("Agent", "This agent has retired from the Defense Security Agency. You may not retrieve this Agent. You may, however, view this Agent's records in the Office of Records on Level III.", "Retired", &globals.dboxparams);
					break;
				case OPERATIVE_STATUS_WHEREABOUTS_UNKNOWN:
					cls(0);
					SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, FALSE);
					StaticDialogBox("Agent", "This Agent's whereabouts are unknown at the moment. He is assumed to be deceased. You may not retrieve this Agent. You may, however, view this Agent's records in the Office of Records on Level III.", "Whereabouts Unknown", &globals.dboxparams);
					break;
				}
				
			}
		}
	}
}

void PlayerDataDebugFunctions(PPLAYER player)
{
	PLIST choices = createList();
	char initchoicestrings[6][60] = { "Modify Agent Status", "Modify Agent Rank", "Modify Agent Psych", "Modify Agent Skills", "Modify Agent Specializations", "EXIT DEBUG SYSTEM"};
	POTYPE initchoices[6];
	
	for(int i = 0; i < 6; i++){
		initchoices[i].string = (char*) malloc(sizeof(char) * (strlen(initchoicestrings[i]) + 1));
		strcpy(initchoices[i].string, initchoicestrings[i]);
		addtoList(choices, initchoices[i], StringCompare);
	}
	while(1)
	{
		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 0, FALSE);
		switch(SimpleListDialogBox("PLAYER DEBUG", "SYSTEM", choices, &globals.dboxparams, CreateStringDBListItem))
		{
		case 0:
			debugchangecharacterstatus(player);
			break;
		case 1:
			debugchangecharacterrank(player);
			break;
		case 2:
			debugchangepsych(player);
			break;
		case 3:
			debugchangeskills(player);
			break;
		case 4:
			debugchangespecializations(player);
			break;
		case 5:
			freeList(choices);
			return;
		}
	}
}

void CRMC(PPLAYER player)
{
	char buf[500];
	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 0, TRUE);
	sprintf(buf, "Welcome, Agent %s. The Code Retrieval And Modification Center (CRMC) has accessed your file. Do you wish your code to be displayed at this time?", player->lname);
	if(YesNoDialogBox("CRMC", buf, "(Y)es/(N)o", &globals.dboxparams)){
		SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_SMALL, 0, TRUE);
		sprintf(buf, "Your Code Is:\n%d\nPress ENTER.", player->accesscode);
		StaticDialogBox("Code", buf, "Display", &globals.dboxparams);
	}
	SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, TRUE);
	sprintf(buf, "Do you wish to change your code?");
	if(YesNoDialogBox("Code Change?", buf, "(Y)es/(N)o", &globals.dboxparams)){
		SetAccessCode(player);
		StaticDialogBox("CRMC", "Your code has been changed. Thank you.\nPress ENTER.", "Code Change", &globals.dboxparams);
		SavePlayer(player);
	}
	else{
		StaticDialogBox("CRMC", "Your code has not changed. Thank you.\nPress ENTER.", "Code Change", &globals.dboxparams);
	}
}

bool ProcessPlayerDataDebugUnsignedNumericalManipulationBox(LISTCONTROLOUTPUT ls, void* extradata)
{
	int* valuetochange = (int*)extradata;
	switch(ls.key)
	{
	case VK_ADD:
		valuetochange++;
		return true;
	case VK_SUBTRACT:
		valuetochange--;
		if(valuetochange < 0){
			valuetochange = 0;
		}
		return true;
	case VK_ESCAPE:
		return false;
	case VK_RETURN:
		return false;
	default:
		return true;
	}
}

int GetNumberOfSavedAgents()
{
	HANDLE handle;
	WIN32_FIND_DATA finddata;
	int agentcounter = 0;
	char path[255];
	strcpy(path, "Agents\\*.agt");
	int returncode = 1;
	
	handle = FindFirstFile(path, &finddata);
	while (handle != INVALID_HANDLE_VALUE && returncode != 0){
		returncode = FindNextFile(handle, &finddata);
		agentcounter++;
	}
	FindClose(handle);
	return agentcounter;
}


//BE CAREFUL WITH THIS FUNCTION. IT EXPECTS A CORRECTLY-SIZED ARRAY OF PLAYER STRUCTURES.
void FillAgentArray(PPLAYER playerarray)
{
	HANDLE handle;
	WIN32_FIND_DATA finddata;
	int agentcounter = 0;
	char path[255];
	strcpy(path, "Agents\\*.agt");
	//	char name[255];
	int returncode = 1;
	
	handle = FindFirstFile(path, &finddata);
	while (handle != INVALID_HANDLE_VALUE && returncode != 0){
		LoadPlayer(finddata.cFileName, &playerarray[agentcounter]);
		returncode = FindNextFile(handle, &finddata);
		agentcounter++;
	}
	FindClose(handle);
	
}

void ReorderWeaponListIndex(PLIST weaponlist)
{
	if(!weaponlist || weaponlist->objcnt < 1){
		return;
	}
	PLIST walker = weaponlist;
	walker->current = walker->head;
	for(int counter = 0; walker->current != NULL; walker->current = walker->current->nextnode, counter++)
	{
		walker->current->object.weapon.index = counter;
	}
}

void ReorderGizmoListIndex(PLIST gizmolist)
{
	if(!gizmolist || gizmolist->objcnt < 1){
		return;
	}
	PLIST walker = gizmolist;
	walker->current = walker->head;
	for(int counter = 0; walker->current != NULL; walker->current = walker->current->nextnode, counter++)
	{
		walker->current->object.gizmo.index = counter;
	}
}
