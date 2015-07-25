#include "agencyinclude.h"
#include "guns.h"


void TrainingCenter(PPLAYER player)
{
	char* LevelStrings[5] = {"Sniping Range", "Hand-To-Hand Sparring Center", "Cryptographic Intrusion", "Lock Intrusion", "Sensor Bypass"};
	PLIST levellist = createList();
	POTYPE levels[5];
	POTYPE exit;
	DOOR door;
	bool startmusicover = false;

	exit.string = (char*) malloc(sizeof(char) * (strlen("Return to Level V") + 1));
	strcpy(exit.string, "Return to Level V");
	addtoList(levellist, exit, StringCompare);
	
	for(int i = 0; i < 5; i++){
		levels[i].string = (char*) malloc(sizeof(char) * (strlen(LevelStrings[i]) + 1));
		strcpy(levels[i].string, LevelStrings[i]);
		addtoList(levellist, levels[i], StringCompare);
	}
	
	while(1)
	{
		cls(0);
		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 0, TRUE);
	//START ELEVATOR SWITCH
		switch(SimpleListDialogBox("Level V Training Center", "Choose Training Type", levellist, &globals.dboxparams, CreateStringDBListItem))
	{
	case 0:
		freeList(levellist);
			return;
	case 1:
		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_LARGE, 0, FALSE);
		StaticDialogBox("Sniper Warfare", "Sniping is a challenge of calmness. When you enter the training area, select a sniper weapon from the available list. You may then proceed immediately to the firing range. When you look through your scope, you will see a pair of red arrows. This represents the horizontal and vertical location of your target. The green arrows represent your horizontal and vertical aiming position. Pressing the arrow keys will set these arrows in motion. The challenge is to line up these arrows and press SPACE to fire your weapon when they line up with the target. The closer you are to the target, the better your shot will be.\nPress ENTER to begin.", "Training Center", &globals.dboxparams);
		LoopSong(globals.musiclist.songs[TRAINING_SONG]);
		SnipingTraining(player);
		LoopOpConMusic(player);
		break;
	case 2:
//	SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, TRUE);
		//StaticDialogBox("Training Center", "It appears that this center is locked up. There's no activity nearby. You guess that it's out of service for the time being.\n Press ENTER to return.", "Sealed", &globals.dboxparams);
			SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_LARGE, 0, FALSE);
	//	StaticDialogBox("Hand-To-Hand Combat", "This is a temporary partial-functionality system of hand-to-hand combat using limited weaponry. Please pardon our progress.\nPress ENTER to begin.", "Training Center", &globals.dboxparams);
		MeleeTrainingSystem(player);
	//	LoopSong(globals.musiclist.songs[TSSI_SONG]);
		break;
	case 3:
		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_LARGE, 0, FALSE);
		StaticDialogBox("Cryptographic Intrusion System", "The CIS is designed to allow an Agent to break highly sophisticated cryptographic locks. When you boot up the CIS, you will see two strings of characters. One string is scrolling at a fixed rate. That is the 'key' string. There is another string, below the key string, that you can set to scroll automatically. You can change the speed of the scrolling by pressing LEFT or RIGHT repeatedly. Your goal is to find a sequence of FOUR DIGITS THAT MATCH EXACTLY. If you believe you have found them, you must synchronize their speeds and align them vertically. At that time, you can press the SPACE BAR to attempt synchronization. If you are successful, the lock will open.\nPress ENTER to begin.", "Training", &globals.dboxparams);
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, TRUE);
		door.lock = 2;
		door.encryptionstrength = NumberDialogBox("Cryptographic", "Enter The Level of Cryptographic Security You Wish This Lock To Have. The Minimum is 1, the Maximum is 9.", "Configuration", 1, &globals.dboxparams);
		if (door.encryptionstrength < MIN_CIS_ENCRYPTIONSTRENGTH){
			door.encryptionstrength = MIN_CIS_ENCRYPTIONSTRENGTH;
		}
		else if (door.encryptionstrength > MAX_CIS_ENCRYPTIONSTRENGTH){
			door.encryptionstrength = MAX_CIS_ENCRYPTIONSTRENGTH;
		}
		if(CISTest(player, &door)){
			SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_SMALL, 0, TRUE);
			StaticDialogBox("Successful", "Congratulations. Intrusion Successful.\nPress ENTER to return to the training center.", "Intrusion", &globals.dboxparams);
		}
		else{
			SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_SMALL, 0, TRUE);
			StaticDialogBox("Failure", "You have failed to intrude through this door. Keep trying!\nPress ENTER.", "Experienced", &globals.dboxparams);
		}
		startmusicover =true;
		break;
	case 4:
		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_LARGE, 0, FALSE);
		StaticDialogBox("Lock Intrusion System", "The LIS system is a small lock intrusion mechanism designed to get an Agent through a mechanically locked door.\nWhen you begin this sequence, you will see a series of rings that represent the tumblers of the lock. On each tumbler is a pin, shown as a ' - ' mark. You may select a tumbler to turn by pressing LEFT or RIGHT. Move the pin by pressing UP or DOWN. Your goal is to match all of the pins on the lock so that they form a line. It's harder than you may think; there are blocks that prevent the movement of the pins. They are invisible to the LIS system, but you will find out quickly where they are. If you believe the lock to be unsolvable, you may press the J key to 'jink' the tumblers and try a different configuration. Good luck.\nPress ENTER.", "Training", &globals.dboxparams);
		door.lock = 1;
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, TRUE);
		door.lockrings = NumberDialogBox("Lock Rings", "Enter The Number Of Rings You Wish This Lock To Have. The Minimum is 3, the Maximum is 6.", "Configuration", 1, &globals.dboxparams);
		if (door.lockrings < MIN_LIS_RINGS){
			door.lockrings = MIN_LIS_RINGS;
		}
		else if (door.lockrings > MAX_LIS_RINGS){
			door.lockrings = MAX_LIS_RINGS;
		}
		if(LISTest(player, &door)){
			SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_SMALL, 0, TRUE);
			StaticDialogBox("Successful", "Congratulations. Intrusion Successful.\nPress ENTER to return to the training center.", "Intrusion", &globals.dboxparams);
		}
		else{
			SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_SMALL, 0, TRUE);
			StaticDialogBox("Failure", "You have failed to intrude through this door. Keep trying!\nPress ENTER.", "Experienced", &globals.dboxparams);
		}
		startmusicover = true;
		break;
	case 5:
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 0, TRUE);
		StaticDialogBox("Training Center", "It appears that this center is locked up. There's no activity nearby. You guess that it's out of service for the time being.\n Press ENTER to return.", "Sealed", &globals.dboxparams);
		break;
	}
	
	//END SWITCH
	if (startmusicover){
		LoopOpConMusic(player);
		startmusicover = false;
	}
	}
	freeList(levellist);
}

void SnipingTraining(PPLAYER player)
{
	PLIST weaponlist = createList();
	PPOTYPE weapons;
	int weaponselection = 0;
	int dbselection = 0;
	int numberweaps = 0;
	int maxrounds;
	char buf[255];
	char wbuf[5000];
	int k = 0;
	int finalscore = 0;
	double finalratioscore = 0.0;
	SNIPERESULT* sr;
	PWEAPON weapon;
	for(int i = 0; i < globals.globalweaponsammo.weaponlist->objcnt; i++){
	weapon = RetrieveGlobalWeaponDataByID(i);
	if (weapon->attacktype & WEAP_ATT_TYPE_SNIPE && IsClearedForWeapon(player, weapon)){
		numberweaps++;
	}
	}
	if (!numberweaps){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_SMALL, 0, TRUE);
		StaticDialogBox("No Sniper", "Unfortunately, you do not have access to any DSA Sniper Weapons at the moment.\nPress ENTER to return.", "Weapons", &globals.dboxparams);
		return;
	}

	weapons = (PPOTYPE) malloc(sizeof(POTYPE) * numberweaps);
	for(i = 0; i < globals.globalweaponsammo.weaponlist->objcnt; i++){
	weapon = RetrieveGlobalWeaponDataByID(i);
	if (weapon->attacktype & WEAP_ATT_TYPE_SNIPE && IsClearedForWeapon(player, weapon)){
		if(weapon->dsaweapon){
			weapons[k].dblistitem.string = (char*) malloc(sizeof(char) * (strlen(weapon->longname) + 5));
		strcpy(weapons[k].dblistitem.string, "DSA ");
		strcat(weapons[k].dblistitem.string, weapon->longname);
		}
		else{
		weapons[k].dblistitem.string = (char*) malloc(sizeof(char) * (strlen(weapon->longname) + 1));
		strcpy(weapons[k].dblistitem.string, weapon->longname);
		}
		weapons[k].dblistitem.id = weapon->id;
		addtoList(weaponlist, weapons[k], CompareDbitemID);
		k++;
	}
	}
	while(1) //SHOW DESCRIPTIONS, CHECK IF THE PLAYER WANTS TO USE.
	{
	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_SMALL, 0, TRUE);
	dbselection = SimpleListDialogBox("Sniper Weapon", "Selection", weaponlist, &globals.dboxparams, CreateStringDBListItem);
	weaponselection = weapons[dbselection].dblistitem.id;
	weapon = RetrieveGlobalWeaponDataByID(weaponselection - 1);
	SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_LARGE, 0, FALSE);
	sprintf(wbuf, "%s\nDo you wish to use this weapon?", weapon->desc);
	if(YesNoDialogBox(weapon->shortname, wbuf, "(Y)es/(N)o", &globals.dboxparams)){
			cls(0);
		break;
	}
	cls(0);
	}

	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_SMALL, 0, TRUE);
	maxrounds = NumberDialogBox("Sniping", "You are going to be evaluated based on an average compiled from the number of rounds you discharge. Please select anywhere from 1 to 15 attempts using your current weapon.", "Setup", 2, &globals.dboxparams);
	cls(0);
	if (maxrounds <= 0 || maxrounds > 15){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_SMALL, 0, TRUE);
		StaticDialogBox("Default", "A Default of 3 Attempts Has Been Set.", "Set", &globals.dboxparams);
		maxrounds = 3;
	}
	SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_MEDIUM, 0, TRUE);
	sr = (PSNIPERESULT) malloc(sizeof(SNIPERESULT) * maxrounds);
	for(i = 0; i < maxrounds; i++){
		playWeaponSound(weapon, WEAP_SOUND_RELOAD, SOUND_MODE_MIX);
		sprintf(buf, "Range Cleared - Attempt %d of %d:\nUsing %s.\nPress ENTER when ready.",i+1, maxrounds, weapon->shortname);
		StaticDialogBox("Next Attempt", buf, "Commencing", &globals.dboxparams);
	sr[i] = snipe(player, weapon, true);
	}
	//CALCULATE FINAL SCORE;
	for (i = 0; i < maxrounds; i++){
		if (sr[i].hitcode == SNIPECODE_BRAINSTEM || sr[i].hitcode == SNIPECODE_HEART){
			finalscore += 100;
		}
		else if (sr[i].hitcode == SNIPECODE_CHEST){
			finalscore += 84;
		}
		else if(sr[i].hitcode == SNIPECODE_ABDOMEN){
			finalscore += 73;
		}
		else if (sr[i].hitcode == SNIPECODE_SHOULDER){
			finalscore += 51;
		}
		else if (sr[i].hitcode == SNIPECODE_ARM || sr[i].hitcode == SNIPECODE_LEG){
			finalscore += 35;
		}
		else if (sr[i].hitcode == SNIPECODE_CHEST_SCRAPE){
			finalscore += 11;
		}
	}
	cls(0);
	finalratioscore = (double) finalscore / (double) maxrounds;
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 0, TRUE);
	sprintf(buf, "Final Results:\n----------------------------\n Probability of Kill (%%PK):\n%2.2f%%\nUsing %s.\nPress ENTER to return.", finalratioscore, weapon->shortname);
	StaticDialogBox("Final", buf, "Assessment", &globals.dboxparams);
	cls(0);
	free (sr);
	freeList(weaponlist);
}

/*
void ElectronicsTraining(PPLAYER player)
{
//	TIMER timer;
	
	char buf[80];
	int choice;
	for (;;)
	{
	
		cls();
		LGREEN;
		printf("Electronics Training Unit\n");
		GREEN;
	
		printf(buf);
		printf("\n");
		printline();
		LBLUE;
		printf("(L)- Lock Intrusion Training\n");
		printf("(C)- Cryptographic Intrusion Training\n");
		printf("(Q)- Return to Training Center\n");
		LCYAN;
		printf("->");
		//WE'RE IN. DISPLAY THE MENU AND SHOW THE TIME, ONCE. EACH TIME THEY COME BACK, 
		//IF LOOPING BACK TO THE MENU, OR QUITTING, CHANGE THE TIME.
		choice = waitforkey();
		switch (choice){
		case 'c':
		case 'C':
			CISTraining(player);
	
			break;
		case 'l':
		case 'L':
			LISTraining(player);
	
			break;
		case 'q':
		case 'Q':
	
			return;
			
		default:
	
			break;
		}
	}
}
*/
/*
void TargetTraining(PPLAYER player)
{
	PWEAPON weapon;
	POTYPE tmpObj;
	BOOL trainingcontinues = TRUE;
	USHORT key;
	int currentweaponindex;
	int clipselection;
	int retval;
	int clip;
	
	InitializePlayerForCombat(player);
	weapon = weaponquickselect(globals.globalweaponsammo.weaponlist, globals.globalweaponsammo.ammolist);
	tmpObj.weapon = *weapon;
	currentweaponindex = player->p_weapons->objcnt; //SO WE CAN TAKE IT BACK LATER.
	tmpObj.weapon.index = currentweaponindex;
	addtoList(player->p_weapons, tmpObj, CompareWeaponIndex);
	do{
		clipselection = SelectClipFromAmmoList(player, globals.globalweaponsammo.ammolist, weapon, TRUE);
		if (clipselection == -1){
			break;
		}
		if (player->weight + CLIP_WEIGHT > MAX_PLAYER_WEIGHT){
			LRED;
			printf("\nThis would put you over your weight limit.");
			LCYAN;
			printf("PRESS A KEY.");
			waitforkey();
			cls();
		}
		if ((retval = GiveClip(player->ammo.cliplist, weapon, clipselection, globals.globalweaponsammo.ammolist, TRUE)) != GETCLIP_OK){
			if (retval == GETCLIPFAILED_NOBULLETS){
				LRED;
				printf("There aren't any bullets left.\n");
				LCYAN;
				printf("PRESS A KEY.");
				waitforkey();
				cls();
			}
			if (retval == GETCLIP_OK_LESSTHANMAX){
				LRED;
				printf("You have recieved a partial clip of ammunition.\n");
				player->weight += CLIP_WEIGHT;
				LCYAN;
				printf("PRESS A KEY.");
				waitforkey();
				cls();
			}
			else{
				LRED;
				printf("INTERNAL ERROR, BOY! CONTACT CHARLES COX ASAP!\n");
				LCYAN;
				printf("PRESS A KEY.");
				waitforkey();
				cls();
			}
		}
		else{
			LGREEN;
			printf("Full Clip Recieved.\n");
			player->weight += CLIP_WEIGHT;
			LCYAN;
			printf("PRESS A KEY.");
			waitforkey();
			cls();
		}
	}while(clipselection != -1);
	
	cls();
	while(trainingcontinues)
	{
		cls();
		GREEN;
		printf("Training Session In Progress\n");
		printline();
		LBLUE;
		printf("Commands:\n");
		LCYAN;
		printf("(L) - Load Clip Into Weapon\n");
		printf("(D) - Drop Clip From Inventory\n");
		printf("(U) - Unload Clip From Weapon\n");
		printf("(Q) - Quit Training Session\n");
		printf("->\n");

		key = waitforVkey();
		
		switch(key){
		case 'L':
		case 'l':
			clip = ChooseClipFromList(player->ammo.cliplist);
			if (LoadClip(player->ammo.cliplist, player->currentweapon, clip) != LOADCLIP_OK){
				LRED;
				printf("Cannot Load Clip.\n");
				printf("Press A Key\n");
				waitforkey();
			}
			else{
				playWeaponSound(player->currentweapon, WEAP_SOUND_RELOAD, SOUND_MODE_MIX);
				LRED;
				printf("Clip Loaded.\n");
				printf("Press A Key\n");
				waitforkey();
			}
			
			break;
		case 'd':
		case 'D':
			clip = ChooseClipFromList(player->ammo.cliplist);
			break;
		case 'u':
		case 'U':
			retval = EjectClip(player->currentweapon, player->ammo.cliplist);
			if (retval == EJECTCLIP_OK_DROPPED){
				LRED;
				printf("Ejected. Empty Clip Dropped.\n");
				printf("Press A Key\n");
				waitforkey();
			}
			else if (retval == EJECTCLIP_OK_SAVED){
				LRED;
				printf("Ejected. Clip Saved.\n");
				printf("Press A Key\n");
				waitforkey();
			}
			else{
				LRED;
				printf("Cannot Eject.\n");
				printf("Press A Key\n");
				waitforkey();
			}
			break;
		case 'q':
		case 'Q':
			trainingcontinues = FALSE;
			break;
		}
	}
	
	FreePlayerFromCombat(player);
}

*/