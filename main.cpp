#include "agencyinclude.h"
#include "game.h"
#include "guns.h"
//#include <CRTDBG.H>
//#define _CRTDBG_MAP_ALLOC
#define RELEASE true
#define SPECIALDEBUG false
bool release = RELEASE;
/************************************************************
; MUST HAVE A GLOBAL GRAPHICS LIST
************************************************************/
GLOBALSTRUCT globals;

void main(int argc, char**argv)
{

	bool specialdebug = false;
	if(SPECIALDEBUG){
		specialdebug = true;
	}
	//_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF |_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);
	srand((int) time(NULL));

	
	PLAYER player;
	PMAP testmap;

	SetDefaultCmdLineOptions();

	if(argc > 1){
		for(int k = 1; k < argc; k++){
			if(argv[k][0] == '-' && argv[k][1] == 'm'){
				globals.options.musicon = false;
			}

			if(argv[k][0] == '-' && argv[k][1] == 'd'){
				specialdebug = true;
				release = false;
			}
		}
	}

	
	
	//START THE GAME

	bool loadok = GameStart();
	if(!loadok){	
		return;
	}

	ShootSequenceDemoRandom(false);

	//SPECIAL DEBUG CODE

	if(specialdebug)
	{
		 //MAP CODE
		

//		char buf[500];
		SARTUMISSION sm;
		sm.numberarmories = 50;
		sm.numbervehicles = 28;
		sm.missiontype = MISSION_TYPE_CAUTERIZATION;
		sm.compoundsecuritypercentage = 50;
		MISSIONCHRONO mc;
		mc.countdown = true;
		mc.hours = 1;
		mc.minutes = 30;
		mc.seconds = 20;
		mc.split = 50;
		PlayerSetTimeToRecovery(&player, &mc);
		strcpy(player.lname, "Cox");
		player.accesscode = 1034;
		player.psychskill.psych.intelligence =3;
		player.psychskill.psych.reflex = 3;
		
		strcpy(player.codename, "Falcon");	
		player.rank = 4;
		player.weight = 0.0;
		
		InitializePlayerForCombat(&player);
		player.health = 37;
		InitializeRealisticWeaponAvailability(globals.globalweaponsammo.weaponlist, globals.globalweaponsammo.ammolist, globals.armorygizmolist);
	//	LevelIVWeaponLockers(&player);
	//	LevelIVAmmunitionLockers(&player);
	//	MainInventorySelection(&player);
	//	LevelIVWeaponLockers(&player);
		LevelIVGizmoLockers(&player);
	//	LevelIVPharmacy(&player);
	//	MainInventorySelection(&player);
	//	initmusic();
	//	GiveAtomicDevice(&player);

	testmap = initMap(100,100);
//	GiveLoadedTrainingWeapon(player.p_weapons, &player.ammo, 14, 0);
//	GiveLoadedTrainingWeapon(player.p_weapons, &player.ammo, 13, 0);
//	GiveClip(player.ammo.cliplist, &player.p_weapons->head->object.weapon, 1, globals.globalweaponsammo.ammolist, &player.weight, true);
//	GiveLoadedTrainingWeapon(player.p_weapons, &player.ammo, 20, 0);

	createFence(testmap);
	FillMapWithPrefabs(testmap, 20);
	FillMapWithObstructions(testmap,25);
	player.currentmissioninfo.RadioChannel = 0;
	player.sartumission = &sm;
	InitializeGlobalMapDataForCombat(testmap, &player);
	CreateRandomRZ(testmap);
	CreateRandomPZ(testmap);
	MainMission(&player, testmap);
	Surgery(&player);
	FreeGlobalMapDataFromCombat();
	terminateMap(testmap);
	
	FreePlayerFromCombat(&player);
	
	GameEnd();
//	_CrtCheckMemory();
//	_CrtDumpMemoryLeaks();
	}

		
		
		
//STANDARD GAME		
		else{
		
	//AllWeaponsAvailable(globals.globalweaponsammo.weaponlist, globals.globalweaponsammo.ammolist);
	InitializeRealisticWeaponAvailability(globals.globalweaponsammo.weaponlist, globals.globalweaponsammo.ammolist, globals.armorygizmolist);
	if(release){
	PromptForCorrectScreenSize();
	IntroScreen();
	}
while(1)
{
	FadeInTitle("THIERSEN STRATEGIC STUDIES INSTITUTE", "DSA HEADQUARTERS, MAIN ENTRY");
	if(!SelectPlayer(&player, false)){
		GameEnd();
		return; //GAME OVER.
	}
	else{
		//GO FOR IT!!!!! 1/2 PLAYER STRUCTURE IS FILLED, GO TO OPCON!
		PrintDayAndMonth(&player);
		OpConElevator(&player);
		//debugchangecharacterstatus(&player);
	}
	LoopSong(globals.musiclist.songs[INTRO_SONG]);
}

	//_CrtCheckMemory();
	//_CrtDumpMemoryLeaks();
}

}

int initweaponsammo(PLIST *weaponlist, PLIST *ammolist)
{
		FILE* fp;
//	int key;
	
	initconsole();
	fp = fopen("data\\ammo.dat", "r");
	if (fp == NULL)
	{
		printf("ammo.dat could not be opened.");
		waitforkey();
		exit(1);
	}
	*ammolist = createAmmoList(fp);
	fclose(fp);
	fp = fopen("data\\weapons.dat", "r");
	if (fp == NULL)
	{
		printf("weapons.dat could not be opened!\n");
		printf("<PRESS A KEY>\n");
		waitforkey();
		exit(1);
	}
	*weaponlist = createWeaponList(fp, *ammolist); //Create WEAPONLIST needs an ammolist to function.
	fclose(fp);
	return 1;
}

void freeweaponsammo(PLIST *weaponlist, PLIST *ammolist)
{
	FreeWeaponList(*weaponlist);
	FreeAmmoList(*ammolist);
	free(*weaponlist);
	free(*ammolist);
}

bool GameStart()
{
	initconsole();
	SetDefaultOptions();
	globals.graphicslist = loadgamegraphics();
	if(globals.graphicslist == NULL){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		StaticDialogBox("Graphics List", "Razor One Has Had A Graphics Initialization Error. Press a Key To Terminate.\n", "Error", &globals.dboxparams);
		return false;
	}
	globals.sartumissions = createList();
	assert(LoadSartuInputLists(&globals.sartudata));
	initweaponsammo(&globals.globalweaponsammo.weaponlist, &globals.globalweaponsammo.ammolist);
	SortArmoryWeapons(globals.globalweaponsammo.weaponlist);
	if(!LoadArmoryGizmoList()){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		StaticDialogBox("Gizmo List", "Razor One Has Had A Gizmo List Initialization Error. Press a Key To Terminate.\n", "Error", &globals.dboxparams);
		return false;
	}

	if(!initmusic()){
		return false;
	}
	
	return true;
}

void GameEnd()
{
	freeweaponsammo(&globals.globalweaponsammo.weaponlist, &globals.globalweaponsammo.ammolist);
	FreeArmoryGizmoList();
	terminatemusic();
	freegamegraphics(globals.graphicslist);
	FreeSartuInputLists(&globals.sartudata);
	TerminateSartu();
}

void SetDefaultCmdLineOptions()
{
	globals.options.musicon = true;
}