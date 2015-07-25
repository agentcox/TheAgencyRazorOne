#include "agencyinclude.h"
#include "handtohand.h"
#include "guns.h"



/************************************************************
; HAND-TO-HAND SEQUENCES SHOULD UTILIZE THE AGENCY THEME FOR THE MUSIC
************************************************************/



//--------------------------------------------------------------------------------------------
// Function Name - MeleeCombatMain
//
// Description   - Wrapper function for melee combat.
//
// Return Type   - int 
//
// Arguments     - PMAPCELL mapcell ( The pointer to the map cell the battle is in. )
//               - PPLAYER player ( The pointer to the player )
//               - PENEMY enemy ( The pointer to the enemy )
//               - BOOL playersurprisedenemy ( Did the player surprise the enemy? )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
int MeleeCombatMain(PMAPCELL mapcell, PPLAYER player, PENEMY enemy, BOOL playersurprisedenemy)
{
	int retval;
	
	MeleeCombatInitialize(mapcell, player, enemy, playersurprisedenemy);
	while ((retval = (MeleeCombatLoop(mapcell, player, enemy))) == MELEE_CONTINUE){
	}
	FLUSH;
	clearinputrecords();
	return MeleeCombatEnd(mapcell, player,enemy, retval);
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleeCombatInitialize
//
// Description   - Initializes melee combat.
//
// Return Type   - void 
//
// Arguments     - PMAPCELL mapcell ( The pointer to the mapcell the battle is in )
//               - PPLAYER player ( The pointer to the player )
//               - PENEMY enemy ( The pointer to the enemy )
//               - BOOL playersurprisedenemy ( Did the player surprise the enemy? )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void MeleeCombatInitialize(PMAPCELL mapcell, PPLAYER player, PENEMY enemy, BOOL playersurprisedenemy)
{
	
	
	COORD drawcoord = {0,2};
	COORD playerboxcoord = {MELEE_PLAYERBOX_TOPLEFT_X, MELEE_PLAYERBOX_TOPLEFT_Y + 2};
	COORD playerboxdimensions = {MELEE_PLAYERBOX_TOPLEFT_X + MELEE_PLAYERBOX_WIDTH, MELEE_PLAYERBOX_TOPLEFT_Y + MELEE_PLAYERBOX_HEIGHT + 2};
	COORD enemyboxcoord = {MELEE_ENEMYBOX_TOPLEFT_X, MELEE_ENEMYBOX_TOPLEFT_Y + 2};
	COORD enemyboxdimensions = {MELEE_ENEMYBOX_TOPLEFT_X + MELEE_ENEMYBOX_WIDTH, MELEE_ENEMYBOX_TOPLEFT_Y + MELEE_ENEMYBOX_HEIGHT + 2};
	LoopSong(globals.musiclist.songs[HANDTOHAND_SONG]);
	cls();
	
	globals.globalmeleestruct.enemyanger = enemy->psychskill.psych.agression * 100 / MAX_PSYCH;
	globals.globalmeleestruct.enemyhonor = (enemy->psychskill.psych.willpower + (MAX_PSYCH - enemy->psychskill.psych.intelligence)) * 100 / MAX_PSYCH;
	
	
	player->meleestruct.stamina = player->health;
	enemy->meleestruct.stamina = enemy->health;
	globals.globalmeleestruct.currentcycleaction = 1;
	
	//WE START WITH NO WOUNDS, EVEN IF THERE ARE PREVIOUS ONES.
	memset(&player->meleestruct.wounds, 0, sizeof(player->meleestruct.wounds));
	memset(&enemy->meleestruct.wounds, 0, sizeof(enemy->meleestruct.wounds));
	
	if (playersurprisedenemy){
		
		if (rand()%2){ //50 - 50 chance of tackle
			if (enemy->currentweapon != NULL)
			{
				playMIXsound(MELEE_SOUND_SPECIALHIT);
				DSAPlayerPrompt("You tackle the guard from behind, your weight throwing him to the ground. As he turns to his side before impact, his grip loosens on his weapon and it sails out of his hands.", PROMPTANSWER_ANYKEY);
				//BE SURE TO PUT IT IN THE WEAPONS LIST, TOO.
				enemy->currentweapon = NULL; //KNOCK IT OUT OF HIS HANDS!
			}
			else{
				playMIXsound(MELEE_SOUND_SPECIALHIT);
				DSAPlayerPrompt("You tackle the guard from behind. Your momentum sends him to the ground.", PROMPTANSWER_ANYKEY);
			}
			enemy->meleestruct.posture = MELEE_POSTURE_PRONE;
			player->meleestruct.posture = MELEE_POSTURE_KNEELING;
			globals.globalmeleestruct.playerturn = TRUE;
			globals.globalmeleestruct.freeturn = TRUE;
			globals.globalmeleestruct.distance = 4;
		}
		else
		{
			if (enemy->currentweapon != NULL)
			{
				playMIXsound(MELEE_SOUND_SPECIALHIT);
				DSAPlayerPrompt("You leap at the guard and tear the weapon out of his hands as he turns toward you. Your momentum carries you and the weapon past the guard, and you hit the floor. The weapon skids out of your hands. As you turn to face him, you see a blur to your side.", PROMPTANSWER_ANYKEY);
				//BE SURE TO PUT IT IN THE WEAPONS LIST, TOO.
				enemy->currentweapon = NULL; //KNOCK IT OUT OF HIS HANDS!
				
			}
			else
			{
				playMIXsound(MELEE_SOUND_SPECIALHIT);
				DSAPlayerPrompt("You leap at the guard. Unfortunately, he turns too soon, and you glance off of him and hit the floor. As you turn to face him, you see a blur to your side.", PROMPTANSWER_ANYKEY);
			}
			enemy->meleestruct.posture = MELEE_POSTURE_STANDING;
			player->meleestruct.posture = MELEE_POSTURE_KNEELING;
			globals.globalmeleestruct.playerturn = FALSE;
			globals.globalmeleestruct.freeturn = FALSE;
			globals.globalmeleestruct.distance = 10;
		}
	}

	else{
		enemy->meleestruct.posture = MELEE_POSTURE_STANDING;
			player->meleestruct.posture = MELEE_POSTURE_STANDING;
			globals.globalmeleestruct.playerturn = RandomPercentage(50);
			globals.globalmeleestruct.freeturn = false;
			globals.globalmeleestruct.distance = 4;
			
	}
	
	cls();
	GREEN;
	printgraphic(globals.graphicslist, drawcoord, MELEE_GRAPHIC_ID);
	

//	MeleePrintActionStringHold("STANDARD GRAPHICS LOADED"); //!!!!!!!!!!!!!!!!!!!
	
	MeleeMakeStatusString(globals.globalmeleestruct.enemystatstring, &enemy->meleestruct, enemy->currentweapon, FALSE);
	MeleeMakeStatusString(globals.globalmeleestruct.playerstatstring, &player->meleestruct, player->currentweapon, TRUE);

//	MeleePrintActionStringHold("STATUS STRINGS CREATED"); //!!!!!!!!!!!!!!!!!!!
	
	GREEN;
	printwordwrapcoordinate(globals.globalmeleestruct.playerstatstring, playerboxdimensions, playerboxcoord);
	RED;
	printwordwrapcoordinate(globals.globalmeleestruct.enemystatstring, enemyboxdimensions, enemyboxcoord);
	Sleep(80);
	LGREEN;
	printwordwrapcoordinate(globals.globalmeleestruct.playerstatstring, playerboxdimensions, playerboxcoord);
	LRED;
	printwordwrapcoordinate(globals.globalmeleestruct.enemystatstring, enemyboxdimensions, enemyboxcoord);

//	MeleePrintActionStringHold("STATUS STRINGS DISPLAYED"); //!!!!!!!!!!!!!!!!!!!

	
	
	MeleePrintDistance();
//	MeleePrintActionStringHold("DISTANCE DISPLAYED"); //!!!!!!!!!!!!!!!!!!!
	MeleePrintCycleMove(player, globals.globalmeleestruct.currentcycleaction);
//	MeleePrintActionStringHold("CYCLE MOVE DISPLAYED"); //!!!!!!!!!!!!!!!!!!!
	Sleep(2500);
	
	globals.globalmeleestruct.combatcontinues = TRUE;
	if(!playersurprisedenemy){
		MeleeCycleTurn();
	}
		MeleeCalculateTimeToTurn(globals.globalmeleestruct.distance, &globals.globalmeleestruct.timetoturn);
	
}

void MeleeCombatRedraw(PPLAYER player, PENEMY enemy)
{
	COORD drawcoord = {0,2};
	COORD playerboxcoord = {MELEE_PLAYERBOX_TOPLEFT_X, MELEE_PLAYERBOX_TOPLEFT_Y + 2};
	COORD playerboxdimensions = {MELEE_PLAYERBOX_TOPLEFT_X + MELEE_PLAYERBOX_WIDTH, MELEE_PLAYERBOX_TOPLEFT_Y + MELEE_PLAYERBOX_HEIGHT + 2};
	COORD enemyboxcoord = {MELEE_ENEMYBOX_TOPLEFT_X, MELEE_ENEMYBOX_TOPLEFT_Y + 2};
	COORD enemyboxdimensions = {MELEE_ENEMYBOX_TOPLEFT_X + MELEE_ENEMYBOX_WIDTH, MELEE_ENEMYBOX_TOPLEFT_Y + MELEE_ENEMYBOX_HEIGHT + 2};
	
	cls(0);
	
	LGREEN;
	printgraphic(globals.graphicslist, drawcoord, MELEE_GRAPHIC_ID);
	
	
	MeleeMakeStatusString(globals.globalmeleestruct.enemystatstring, &enemy->meleestruct, enemy->currentweapon, FALSE);
	MeleeMakeStatusString(globals.globalmeleestruct.playerstatstring, &player->meleestruct, player->currentweapon, TRUE);
	
	LRED;
	printwordwrapcoordinate(globals.globalmeleestruct.playerstatstring, playerboxdimensions, playerboxcoord);
	printwordwrapcoordinate(globals.globalmeleestruct.enemystatstring, enemyboxdimensions, enemyboxcoord);
	
	MeleePrintDistance();
	MeleePrintCycleMove(player, globals.globalmeleestruct.currentcycleaction);
}

bool MeleePlayerDrawWeapon(PPLAYER player)
{
	PLIST weaponslist = createList();
	PPOTYPE weapons;
	PWEAPON weapon;
	char resultstring[500];
	PPOTYPE ppotype;
	int i = 0;
	int dbselection = 0;
	
	if(player->p_weapons != NULL && player->p_weapons->objcnt > 0){
		weapons = (PPOTYPE) malloc(sizeof(POTYPE) * (player->p_weapons->objcnt + 1));
		
		weapons[0].dblistitem.id = -1;
		weapons[0].dblistitem.string = (char*) malloc(sizeof(char) * (strlen("None") + 1));
		strcpy(weapons[0].dblistitem.string, "None");
		addtoList(weaponslist, weapons[i], CompareDbitemID);

		for(i = 1; i < player->p_weapons->objcnt + 1; i++){
			ppotype = ReturnListItemByNumber(i-1, player->p_weapons);
			weapon = &ppotype->weapon;
			weapons[i].dblistitem.string = (char*) malloc(sizeof(char) * (strlen(weapon->longname) + 1));
			strcpy(weapons[i].dblistitem.string, weapon->longname);
			weapons[i].dblistitem.id = weapon->index;
			addtoList(weaponslist, weapons[i], CompareDbitemID);
		}
		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_SMALL, 0, TRUE);
		dbselection = SimpleListDialogBox("Choose", "Weapon", weaponslist, &globals.dboxparams, AlreadyDBListItem);
		if(!dbselection)
		{
			if(player->currentweapon){
				playMIXsound(PICKUP_WEAPON_SOUND);
			MeleePrintActionStringHold("You holster your weapon.");
			globals.globalmeleestruct.currentcycleaction = MELEE_ACTION_PUNCH;
		MeleePrintCycleMove(player, MELEE_ACTION_PUNCH);
			}
			player->currentweapon = NULL;
			return false;
		}
		ppotype = ReturnListItemByNumber(dbselection - 1, player->p_weapons);
		player->currentweapon = &ppotype->weapon;
		//	cls(0);
		playMIXsound(PICKUP_WEAPON_SOUND);
		sprintf(resultstring, "You draw your %s.", player->currentweapon->shortname);
		globals.globalmeleestruct.currentcycleaction = MELEE_ACTION_PUNCH;
		MeleePrintCycleMove(player, MELEE_ACTION_PUNCH);
		MeleePrintActionStringHold(resultstring);
		freeList(weaponslist);
		free(weaponslist);
		free(weapons);
		return true;
	}
	else{
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_SMALL, 0, TRUE);
		StaticDialogBox("No Weapons", "You have no weapons available!\nPress ENTER.", "Available", &globals.dboxparams);
		return false;
	}
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleeCombatEnd
//
// Description   - Ends the melee combat.
//
// Return Type   - int 
//
// Arguments     - PMAPCELL mapcell ( The map cell the battle is in. )
//               - PPLAYER player ( The pointer to the player )
//               - PENEMY enemy ( The pointer to the enemy )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
int MeleeCombatEnd(PMAPCELL mapcell, PPLAYER player, PENEMY enemy, int deathtype)
{
	
	if (deathtype == MELEE_END_PLAYERBLUNTUNCONSCIOUS){
		DSAPlayerPrompt("You lay upon the ground, flickering in and out of conciousness. You feel yourself being dragged somewhere...and soon the movements fade to a haze of blackness.", PROMPTANSWER_ANYKEY);
		
	}
	else if (deathtype == MELEE_END_PLAYERBLOODLOSSDEAD){
		DSAPlayerPrompt("You lay on the ground. Blood drools from your wounds as you feel your life slipping away. As you fade away, you notice only the faintest shadow of a person near you.", PROMPTANSWER_ANYKEY);
	}
	else if (deathtype == MELEE_END_PLAYERSHOTDEAD){
		DSAPlayerPrompt("You curl up in pain on the ground. Slowly, you bring your finger to the ragged bullet wound that dropped you. As you feel the blood pour around your hand, you fall unconscious.", PROMPTANSWER_ANYKEY);
	}
	else if (deathtype == MELEE_END_PLAYERSTRANGLEDDEAD){
		DSAPlayerPrompt("Your life is slipping away as your oxygen is cut off. You haven't the strength to resist it. Your neck muscles release and the hands slip over your trachea, crushing your last attempts at air. Your eyes close as it all goes black.", PROMPTANSWER_ANYKEY);
	}
	else if (deathtype ==  MELEE_END_PLAYERDRUGDEAD){
		DSAPlayerPrompt("You stagger on the floor as the poison works its way through your blood. You spit out white foam that seems to ooze from your mouth, and before you can take your next breath, a wrenching pain in your chest pushes you over the brink into blackness.", PROMPTANSWER_ANYKEY);
	}
	else if (deathtype == MELEE_END_PLAYERDRUGUNCONSCIOUS){
		DSAPlayerPrompt("You feel completely powerless; your muscles fail to respond, your eyelids grow heavy, and every movement seems to take a lifetime. Finally, you can fight the drug no longer, and collapse into darkness.", PROMPTANSWER_ANYKEY);
	}
	else if (deathtype == MELEE_END_PLAYERCYANIDE){
		DSAPlayerPrompt("The bitter contents spill out inside your mouth. The foaming reaction begins instantly, your mouth instantly going numb as the potassium cyanide enters your bloodstream. You sink to your knees; the pain is greater than you thought it would be. For a moment, a sense of panic sets in as your breathing is choked off. With one last gasp, foam falls from your lips, and your life ends.", PROMPTANSWER_ANYKEY);
	}
	else{
		
		
		if (deathtype == MELEE_END_ENEMYBLUNTUNCONSCIOUS){
			DSAPlayerPrompt("Your adversary lay on the ground, his eyes fluttering in a strange unconscious response. You silently wait as his eyes shut and he drifts off into blackness. As his motions still, you prepare to move on.", PROMPTANSWER_ANYKEY);
			enemy->status = ENEMY_STATUS_UNCONSCIOUS;
		}
		else if (deathtype == MELEE_END_ENEMYBLOODLOSSDEAD){
			DSAPlayerPrompt("Your opponent is collapsed on the ground, his wounds spurting blood below him. It appears to have been too much; his motions become slower and shallower, and you notice his breathing stop. He's gone.", PROMPTANSWER_ANYKEY);
			enemy->status = ENEMY_STATUS_DEAD;
		}
		else if (deathtype == MELEE_END_ENEMYSHOTDEAD){
			DSAPlayerPrompt("Your opponent lies lifeless in a heap, the bullet having taken his last ounce of strength. A final spasm racks his body, and then, nothing. It's over.", PROMPTANSWER_ANYKEY);
			enemy->status = ENEMY_STATUS_DEAD;
		}
		else if (deathtype == MELEE_END_ENEMYSTRANGLEDDEAD){
			DSAPlayerPrompt("You step up and away from the body. The eyes are locked open in a death stare, and the hands lie limp against the ground. You silently steady yourself and prepare to move on.", PROMPTANSWER_ANYKEY);
			enemy->status = ENEMY_STATUS_DEAD;
		}
		else if (deathtype == MELEE_END_ENEMYDRUGDEAD){
			DSAPlayerPrompt("You quickly step away from your opponent as he starts to spasm from the drug. You hear a crack as his head bangs against the ground. Spittle flies from his mouth, and he shakes a final time before collapsing, utterly dead.", PROMPTANSWER_ANYKEY);
			enemy->status = ENEMY_STATUS_DEAD;
		}
		else if (deathtype == MELEE_END_ENEMYDRUGUNCONSCIOUS){
			DSAPlayerPrompt("As your opponent realizes he's been injected, he moves back quickly in alarm, but slows down almost instantly, finally stopping on the ground and closing his eyes as the drug takes its course. He's out.", PROMPTANSWER_ANYKEY);
			enemy->status = ENEMY_STATUS_UNCONSCIOUS;
			
			
		}
		//SO IF THE PLAYER LIVES...
		MeleeConvertPlayerDamageAtEndOfCombat(player);
	}
	
	return deathtype;
	
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleeCombatLoop
//
// Description   - The main loop for melee combat. Returns result when finished.
//
// Return Type   - int 
//
// Arguments     - PMAPCELL mapcell ( The pointer to the map cell the battle is in. )
//               - PPLAYER player ( The pointer to the player. )
//               - PENEMY enemy ( The pointer to the enemy. )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
int MeleeCombatLoop(PMAPCELL mapcell, PPLAYER player, PENEMY enemy)
{
	BOOL actionperformed = FALSE;
	TIMER timer;
	int numevents, key, i, deathvalue;
	
	while (globals.globalmeleestruct.combatcontinues)
	{
		
		if (MeleeResolvePlayerPostures(player))
		{
			MeleePrintPlayerandEnemyStats(player,enemy);
			MeleeCycleTurn();
		}
		if (MeleeResolveEnemyPostures(enemy))
		{
			MeleePrintPlayerandEnemyStats(player,enemy);
			MeleeCycleTurn();
		}
		MeleeStartTimer();
		//IF IT'S THE ENEMIES TURN, HAVE THEM MAKE A MOVE.
		if (!globals.globalmeleestruct.playerturn)
		{
			MeleePrintPlayerandEnemyStats(player,enemy);
			MeleeGetEnemyMoveOffensive(player, enemy);
		}
		else{
			MeleePrintActionString("GO!");
		}
		
		for(;;)
		{
			StartTimer(&timer);
			//GET THE PLAYER INPUT HERE
			
			
			numevents = checkforinput();
			if (numevents > 1)
			{
				for (i = 1; i <= numevents; i++)
				{
					key = getinput(i);
					if (MeleeGetPlayerMove(player, key) == MELEE_VALID_ACTION){ //VALID_ACTIONS FREEZE THE TIMER AND CYCLE. NOT ALL ACTIONS ARE "VALID".
						actionperformed = TRUE;
						FLUSH;
						clearinputrecords();
						
						break;
					}
				}
				FLUSH;
				clearinputrecords();
			}
			
			//DONE WITH PLAYER INPUT
			Sleep(40);
			
			EndTimer(&timer);
			globals.globalmeleestruct.timetoturn.split -= SplitElapsed(&timer);
			
			if (MissionChronoTimeUp(&globals.globalmeleestruct.timetoturn) || actionperformed)
			{
				MeleePrintPlayerandEnemyStats(player,enemy);
				
				if (MissionChronoTimeUp(&globals.globalmeleestruct.timetoturn))
				{
					globals.globalmeleestruct.timetoturn.seconds = 0;
					globals.globalmeleestruct.timetoturn.split = 0;
					RED;
					MeleePrintTimeString();
					player->meleestruct.lastaction = MELEE_NO_ACTION;
				}
				else
				{
					
					MeleePrintPlayerandEnemyStats(player,enemy);
					LBLUE;
					MeleePrintTimeString();
				}
				if (globals.globalmeleestruct.playerturn)
				{
					MeleeResolvePlayerOffensiveAction(player, enemy);
					if ((deathvalue = MeleeResolveDeath(player, enemy)) != MELEE_CONTINUE){
						return deathvalue;
					}
					MeleePrintPlayerandEnemyStats(player,enemy);
					MeleeCycleTurn();
				}
				else
				{	
					MeleeResolveEnemyOffensiveAction(enemy, player);
					if ((deathvalue = MeleeResolveDeath(player, enemy)) != MELEE_CONTINUE){
						return deathvalue;
					}
					MeleePrintPlayerandEnemyStats(player,enemy);
					MeleeCycleTurn();
				}
				
				actionperformed = FALSE;
				break;
			}
			
			else
			{
				calibratemissionchrono(&globals.globalmeleestruct.timetoturn);
				MeleePrintTimeString();
				
			}
		}
		
	}
	
	return MeleeResolveDeath(player, enemy);
	
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleeCycleTurn
//
// Description   - Cycles to the next turn.
//
// Return Type   - void 
//
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void MeleeCycleTurn()
{
	
	
	FLUSH;
	clearinputrecords();

	if (globals.globalmeleestruct.freeturn){
		if (globals.globalmeleestruct.playerturn)
		{
			MeleePrintStringInHelpBox(MELEE_ATTACK_HELP_STRING);
			MeleePrintActionStringHold("You have a free action! Make another move!");
		}
		else
		{
			MeleePrintStringInHelpBox(MELEE_BLOCK_HELP_STRING);
			MeleePrintActionStringHold("The enemy has another move!");
		}
		globals.globalmeleestruct.freeturn = FALSE;
	}
	else
	{
		if (globals.globalmeleestruct.playerturn)
		{
			globals.globalmeleestruct.playerturn = FALSE;
			MeleePrintStringInHelpBox(MELEE_BLOCK_HELP_STRING);
			MeleePrintActionStringHold("Prepare to Defend!");
		}
		else
		{
			globals.globalmeleestruct.playerturn = TRUE;
			MeleePrintStringInHelpBox(MELEE_ATTACK_HELP_STRING);
			MeleePrintActionStringHold("Prepare to Attack!");
		}
	}
	MeleeCalculateTimeToTurn(globals.globalmeleestruct.distance, &globals.globalmeleestruct.timetoturn);
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleeCalculateTimeToTurn
//
// Description   - Calculates the time to the next melee turn.
//
// Return Type   - void 
//
// Arguments     - int distance ( The distance between the combatants. )
//               - PMISSIONCHRONO timetoturn ( The structure to fill in. )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void MeleeCalculateTimeToTurn(int distance, PMISSIONCHRONO timetoturn)
{
	
	timetoturn->countdown = FALSE;
	timetoturn->hours = 0;
	timetoturn->minutes = 0;
	timetoturn->seconds = 0;
	timetoturn->split = distance * MELEE_SPLITPERDISTANCEUNIT + MELEE_CONSTANTSPLITADDITIONAL;
	if (globals.globalmeleestruct.playerturn){
		timetoturn->split += MELEE_ATTACKBONUSSPLIT;
	}
	calibratemissionchrono(timetoturn);
	timetoturn->countdown = TRUE;
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleePrintTimeString
//
// Description   - Prints the time remaining in the appropriate place.
//
// Return Type   - void 
//
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void MeleePrintTimeString()
{
	char timestring[20];
	sprintf(timestring, "%2.2d.%2.2d", globals.globalmeleestruct.timetoturn.seconds, globals.globalmeleestruct.timetoturn.split);
	setcursor(MELEE_TIMEBOX_TOPLEFT_X, MELEE_TIMEBOX_TOPLEFT_Y+2);
	printf(timestring);
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleeStartTimer
//
// Description   - Shows a start timer sequence which flashes the timer in different colors.
//
// Return Type   - void 
//
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void MeleeStartTimer()
{
	COORD drawcoord = {MELEE_TIMEBOX_TOPLEFT_X, MELEE_TIMEBOX_TOPLEFT_Y +2};
	clearcells(MELEE_TIMEBOX_WIDTH - 1, drawcoord);
	LBLUE;
	MeleePrintTimeString();
	Sleep(70);
	LRED;
	MeleePrintTimeString();
	Sleep(70);
	LBLUE;
	MeleePrintTimeString();
	Sleep(70);
	LRED;
	MeleePrintTimeString();
	Sleep(70);
	FLUSH;
	clearinputrecords();
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleeMakeStatusString
//
// Description   - Makes a status string for either enemy or player, and will output to the string given.
//
// Return Type   - void 
//
// Arguments     - char* string ( The string to output to. )
//               - PMELEESTRUCT ms ( The meleestruct pointer. )
//               - PWEAPON weapon ( The weapon in the hand. )
//               - BOOL player ( Whether or not this is generating player stats )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void MeleeMakeStatusString(char* string, PMELEESTRUCT ms, PWEAPON weapon, BOOL player)
{
	if (player){
		strcpy(string, "Player Stats\n");
	}
	else{
		strcpy(string, "Enemy Stats\n");
	}
	
	strcat(string, "Stamina: ");
	if (ms->stamina >= 90)
	{
		strcat(string, "High");
	}
	else if (ms->stamina >= 60)
	{
		strcat(string, "Fair");
	}
	else if (ms->stamina >= 45)
	{
		strcat(string, "Weak");
	}
	else if (ms->stamina >= 20)
	{
		strcat(string, "Failing");
	}
	else
	{
		strcat(string, "Abysmal");
	}
	
	strcat(string, "\nPosture: ");
	if (ms->posture == MELEE_POSTURE_STANDING)
	{
		strcat(string, "Standing");
	}
	else if (ms->posture == MELEE_POSTURE_KNEELING)
	{
		strcat(string, "Kneeling");
	}
	else
	{
		strcat(string, "Prone");
	}
	strcat(string, "\nWeapon: ");
	
	if (weapon == NULL)
	{
		strcat(string, "None");
	}
	
	else
	{
		
		if (weapon->attacktype & WEAP_ATT_TYPE_SHOOT){
			strcat(string, "Firearm");
		}
		else if (weapon->attacktype & WEAP_ATT_TYPE_DRUG){
			strcat(string, "Injector");
		}
		else if (weapon->attacktype & WEAP_ATT_TYPE_CUT){
			strcat(string, "Knife");
		}
		else if (weapon->attacktype & WEAP_ATT_TYPE_HIT){
			strcat(string, "Blunt Obj");
		}
	}
	
	
	
	//ADD MORE WEAPON FUNCTIONS
	
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleeDebug
//
// Description   - Simple Debugging Session
//
// Return Type   - void 
//
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
int MeleeDebug()
{
	PLAYER player;
	ENEMY enemy;
	MAPCELL mapcell;
	int result;
	
	player.health = 100;
	enemy.health = 100;
	player.currentweapon = NULL;
	enemy.currentweapon = NULL;
	
	ClearPlayerStats(&player);
	ClearEnemyStats(&enemy);
	
	player.psychskill.skills.meleecombatNOweapon = 3;
	player.psychskill.skills.meleecombatWITHweapon = 1;
	
	enemy.psychskill.skills.meleecombatNOweapon = 2;
	enemy.psychskill.skills.meleecombatWITHweapon = 3;
	enemy.psychskill.psych.agression = 4;
	enemy.psychskill.psych.reflex = 3;
	enemy.psychskill.skills.stealthcombat = 2;


	player.psychskill.psych.reflex = 4;
	
	
	player.psychskill.psych.agression = 3;
	
	
	player.psychskill.skills.stealthcombat = 10;
	
	
	
	
	
	mapcell.xcoord = 0;
	mapcell.ycoord = 0;
	InitializePlayerForCombat(&player);
	InitializeEnemyForCombat(&enemy);
	GiveTrainingWeapon(&player, 2);
	GiveTrainingWeapon(&player, 12);
	GiveTrainingWeapon(&player, 11);
	GiveClip(player.ammo.cliplist, &player.p_weapons->head->object.weapon,1, globals.globalweaponsammo.ammolist, &player.weight, true);
	LoadClip(player.ammo.cliplist, &player.p_weapons->head->object.weapon, 0);
//	player.p_weapons->head->object.weapon.modifiersattached.lasersight = true;
	GiveGizmo(&player, GIZMO_TYPE_RING);
	GiveGizmo(&player, GIZMO_TYPE_WATCH);

	GiveTrainingWeapon(&enemy, 1);
	GiveTrainingWeapon(&enemy, 12);
	GiveTrainingWeapon(&enemy, 11);
	GiveClip(enemy.ammolist.cliplist, &enemy.weaponlist->head->object.weapon,2, globals.globalweaponsammo.ammolist, NULL, true);
	LoadClip(enemy.ammolist.cliplist, &enemy.weaponlist->head->object.weapon, 0);

	enemy.currentweapon = &enemy.weaponlist->head->object.weapon;
	
	result = MeleeCombatMain(&mapcell, &player, &enemy, TRUE); //THE MELEE LOOP
	
	FreePlayerFromCombat(&player);
	FreeEnemyFromCombat(&enemy);

	return result;
}

void MeleeTrainingSystem(PPLAYER player)
{
	ENEMY enemy;
	MAPCELL mapcell;
	
	
	
	MeleeCreateAverageOpponent(&enemy);
	
	
	mapcell.xcoord = 0;
	mapcell.ycoord = 0;
	
	
	//int result;
	int combatresult;
	bool startsongover = false;
	bool combatgo = false;
	bool ambush = false;
	
	
	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 0, FALSE);
	StaticDialogBox("Melee Combat", "Welcome to Melee Combat training. Due to the complexity of the melee combat system, we urge you to study your DSA Procedures Manual (In Particular, Addendum Circular A-602xN - Melee Combat) for specific information on how to use the Melee Interface. Through this menu, you will be able to access a variety of scenarios that will pit you and your opponent against each other in a variety of settings. Note that any damage taken during these exercises is merely artificial, and in no way impacts your performance. Good luck!\nPress ENTER to continue.", "Training System", &globals.dboxparams);
	
	while(1)
	{
		player->health = 100;
	enemy.health = 100;
		InitializePlayerForCombat(player);
		InitializeEnemyForCombat(&enemy);
		ambush = MeleeCustomScenario(player, &enemy);
		combatgo = true;
		startsongover = true;
		
		if (combatgo){
			//DO COMBAT
			combatresult = MeleeCombatMain(&mapcell, player, &enemy, ambush);
			//SAY INSPIRATIONAL MESSAGE
			MeleeTrainingMessage(combatresult);
			//REMOVE COMBATGO.
			LoopOpConMusic(player);
			combatgo = false;
			FreePlayerFromCombat(player);
			FreeEnemyFromCombat(&enemy);
			if(startsongover)
			{
				LoopOpConMusic(player);
				startsongover = false;
				
			}
		}

		SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_SMALL, 1, TRUE);
		cls(0);
		if(!YesNoDialogBox("Continue", "Do You Wish To Attempt Another Training Session?", "(Y)es/(N)o", &globals.dboxparams)){
			break;
		}
	}
}

bool MeleeCustomScenario(PPLAYER player, PENEMY enemy)
{
	PLIST list;
	char buf[2000];
	char * enemyskill[4] = {"Poorly Skilled Opponent", "Average Skilled Opponent", "Highly Skilled Opponent", "Elite Opponent"};
		LoopSong(globals.musiclist.songs[MOBILIZE_SONG]);
	cls(0);
	SetGlobalDBoxParam(DB_SCHEME_COOLBLUELINES, DB_MEDIUM, 1, FALSE);
	StaticDialogBox("Custom", "You are authorized to set up your own melee combat training scenario. Please follow the directions carefully. Good luck!\nPress ENTER to continue.", "Simulation", &globals.dboxparams);
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
	StaticDialogBox("Enemy", "Your first task is to outfit the enemy you'd like to fight. We first ask that you choose a skill level for this enemy. Press ENTER to do so now.", "Creation", &globals.dboxparams);
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_SMALL, 1, FALSE);
	CreateDBStringList(&list, enemyskill, 4);
	switch(SimpleListDialogBox("Melee Combat", "Select Scenario", list, &globals.dboxparams, CreateStringDBListItem))
	{
	case 0:
		MeleeCreatePoorOpponent(enemy);
		break;
	case 1:
		MeleeCreateAverageOpponent(enemy);
		break;
	case 2:
		MeleeCreateSkilledOpponent(enemy);
		break;
	case 3:
		MeleeCreateEliteOpponent(enemy);
		break;
	} //ENEMY SKILL DONE.
	FreeDBStringList(list);
	//ENEMY WEAPONS!
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
	StaticDialogBox("Enemy Armament", "We now require you to select a weapon loadout for your enemy. You may choose any combination of weapons and ammunition that you see listed. Of course, the types of weapons available may be restricted by your clearance level.", "Press ENTER", &globals.dboxparams);

	for(int weaponschosen = 0; weaponschosen < MELEE_MAX_TRAINING_WEAPONS; weaponschosen++){
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_LARGE, 1, FALSE);
	if(MeleeTrainingSelectWeapon(player, enemy->weaponlist, &enemy->ammolist)){
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_SMALL, 1, TRUE);
	sprintf(buf, "Enemy Weapon Has Been Armed. You may choose up to %d more weapons.", MELEE_MAX_TRAINING_WEAPONS - weaponschosen - 1);
	//------------PLAY A WEAPON DEPLOYMENT SOUND HERE?
	playMIXsound(PICKUP_WEAPON_SOUND);
	StaticDialogBox("Weapon Active.", buf, "Press ENTER", &globals.dboxparams);
	}
	else{
		break;
	}
	}


	

	
	//YOUR WEAPONS
	SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_MEDIUM, 1, FALSE);
	StaticDialogBox("Weapon", "You may now choose your weapons. Please choose wisely and with regard for your current situation and educational objectives. Of course, weapon selection may be restricted due to clearance level.\nPress ENTER to Begin.", "Selection", &globals.dboxparams);
	for(weaponschosen = 0; weaponschosen < MELEE_MAX_TRAINING_WEAPONS; weaponschosen++){
	SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_LARGE, 1, FALSE);
	if(MeleeTrainingSelectWeapon(player, player->p_weapons, &player->ammo)){
	SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_SMALL, 1, TRUE);
	sprintf(buf, "Your Weapon Has Been Armed. You may choose up to %d more weapons.", MELEE_MAX_TRAINING_WEAPONS - weaponschosen - 1);
	//------------PLAY A WEAPON DEPLOYMENT SOUND HERE?
	playMIXsound(PICKUP_WEAPON_SOUND);
	StaticDialogBox("Weapon Active.", buf, "Press ENTER", &globals.dboxparams);
	}
	else{
		break;
	}
	}

	SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_MEDIUM, 1, FALSE);
	StaticDialogBox("Gizmo", "You will now be given a choice as to whether or not you wish to use a set of melee-combat specific devices or 'gizmos'.\nPress ENTER to begin.", "Selection", &globals.dboxparams);
	//YOUR GIZMOS
	SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_SMALL, 1, TRUE);
	if(YesNoDialogBox("Blade Ring", "Will you be using a Blade Ring?", "(Y)es/(N)o", &globals.dboxparams)){
			GiveGizmo(player, GIZMO_TYPE_RING);
	}
	if (YesNoDialogBox("Strangling Wire Watch", "Will you be using a Strangling Wire Watch?", "(Y)es/(N)o", &globals.dboxparams)){
		GiveGizmo(player, GIZMO_TYPE_WATCH);
	}
	if (YesNoDialogBox("Body Armor Trauma Plates", "Will you be wearing Trauma Plates?", "(Y)es/(N)o", &globals.dboxparams)){
		GiveGizmo(player, GIZMO_TYPE_TRAUMAPLATES);
	}

	//LAST BIT FOR ENEMY.
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
	bool ambush = YesNoDialogBox("Enemy Creation", "Your last task in scenario setup is to determine whether or not you wish to ambush your opponent. Ambushing gives you an advantage and provides a different combat scenario.\nDo you wish to ambush your opponent?.", "(Y)es/(N)o", &globals.dboxparams);

	//WE'RE READY.
	SetGlobalDBoxParam(DB_SCHEME_COOLBLUELINES, DB_SMALL, 1, TRUE);
	StaticDialogBox("PRESS ENTER", "You're Going In.", "TO BEGIN", &globals.dboxparams);
	StopSong();
	return ambush;
}

bool MeleeTrainingSelectWeapon(PPLAYER player, PLIST weaponlist, PPLAYERAMMO ammostruct)
{
	char**names;
	PLIST list;
	char buf[2000];
	char* finished = "FINISHED WITH SELECTION";
	int* numref; //THE NUMBERREFERENCE;
	int walkerref = 0;
	PLIST walker = globals.globalweaponsammo.weaponlist;
	int numavailableweapons = MeleeTrainingGetNumAvailWeapons(player);
	int i = 1;
	int weaponid;
	int subammo;
	POTYPE tempobject;
	PPOTYPE listobject;
	PWEAPON weapon;
	walker->current = walker->head;
	names = (char**)malloc(sizeof(char*) * (numavailableweapons + 1));
	numref = (int*) malloc(sizeof(int) * (numavailableweapons + 1));
	
	//FOR THE EXIT STRING.
	names[0] = (char*) malloc(sizeof(char*) * (strlen(finished) + 1));
	strcpy(names[0], finished);
	numref[0] = 0;
	
	while(walker->current != NULL)
	{
		if(IsClearedForWeapon(player, &walker->current->object.weapon)){
			names[i] = (char*) malloc(sizeof(char) * (strlen(walker->current->object.weapon.longname) +10)); //A LITTLE EXTRA
			if(walker->current->object.weapon.dsaweapon){
				strcpy(names[i], "DSA-");
			}
			else{
				strcpy(names[i], "");
			}
			strcat(names[i], walker->current->object.weapon.longname);
			numref[i] = walkerref;
			i++;
			
		}
		walker->current = walker->current->nextnode;
		walkerref++;
	}
	
	CreateDBStringList(&list, names, numavailableweapons + 1);
	while(1)
	{
		int result = SimpleListDialogBox("Weapon Selection", "System", list, &globals.dboxparams, CreateStringDBListItem);
		
		if(!result){ //IF IT'S THE FIRST ENTRY..DONE.
			//	FreeDBStringList(list);
		//	for(int k = 0; k < numavailableweapons; k++){
		//		free(names[k]);
		//	}
		//	free(names);
			free(numref);
			return false;
		}
		else{
			weaponid = numref[result];
			listobject = ReturnListItemByNumber(weaponid, globals.globalweaponsammo.weaponlist);
			if(!listobject){
				//	FreeDBStringList(list);
				free(numref);
				return false;
			}
			weapon = &listobject->weapon;
			sprintf(buf, "%s\n\nPlease Confirm Your Choice.", weapon->desc);
			if(!YesNoDialogBox(weapon->longname, buf, "(Y)es/(N)o", &globals.dboxparams)){
				continue;
			}
			
			subammo = MeleeTrainingSelectSubAmmoTypeForWeapon(weapon);
			int weapref = weaponlist->objcnt;
			if(!GiveLoadedTrainingWeapon(weaponlist, ammostruct, weaponid + 1, subammo)){
				//	FreeDBStringList(list);
				free(numref);
				return false;
			}
			tempobject.weapon.index = weapref;
			listobject = findObject(weaponlist, &tempobject, CompareWeaponIndex);
		
			//	FreeDBStringList(list);
			free(numref);
			return true;
		}
	}
}

int MeleeTrainingGetNumAvailWeapons(PPLAYER player)
{
	int availweapons = 0;
	PLIST walker = globals.globalweaponsammo.weaponlist;
	walker->current = walker->head;
	while(walker->current != NULL)
	{
		if(IsClearedForWeapon(player, &walker->current->object.weapon)){
			availweapons++;
		}
		walker->current = walker->current->nextnode;
	}
	return availweapons;
}

int MeleeTrainingSelectSubAmmoTypeForWeapon(PWEAPON weapon)
{
	PPOTYPE mainammo;
	int numammotypes;
	char** ammotypes;
	char* ammonamestr;
	BOOL dsaammo;
	PLIST list;
	char * ammodesc;
	char buf[2000];
	int result;
	if (!IsWeaponShootable(weapon)){
	return 0;
	}
	else{
		mainammo = ReturnListItemByNumber(weapon->ammotype - 1, globals.globalweaponsammo.ammolist);
		if(!mainammo){
			return 0;
		}
		else{
			numammotypes = mainammo->ammo.maxammotypes;
			ammotypes = (char**) malloc(sizeof(char*) * numammotypes);
			for(int i = 0; i < numammotypes; i++){
				switch(mainammo->ammo.type)
				{
				case AMMOTYPE_BULLET:
				ammonamestr = mainammo->ammo.specialammo.bulletdata[i].longname;
				dsaammo = mainammo->ammo.specialammo.bulletdata[i].dsaammo;
					break;
				case AMMOTYPE_SHOTGUNSHELL:
					ammonamestr = mainammo->ammo.specialammo.shelldata[i].longname;
				dsaammo = mainammo->ammo.specialammo.shelldata[i].dsaammo;
					break;
				case AMMOTYPE_EXPLOSIVE:
					ammonamestr = mainammo->ammo.specialammo.explosivedata[i].longname;
				dsaammo = mainammo->ammo.specialammo.explosivedata[i].dsaammo;
					break;
				}
					ammotypes[i] = (char*) malloc(sizeof(char*) * (strlen(ammonamestr)+ 10));
					if(dsaammo){
						strcpy(ammotypes[i], "DSA-");
					}
					else{
						strcpy(ammotypes[i], "");
					}
					strcat(ammotypes[i], ammonamestr);
			}
			CreateDBStringList(&list, ammotypes, numammotypes);
			while(1)
			{
				result = SimpleListDialogBox("Ammunition", "Selection", list, &globals.dboxparams, CreateStringDBListItem);
				switch(mainammo->ammo.type)
				{
				case AMMOTYPE_BULLET:
				ammodesc = mainammo->ammo.specialammo.bulletdata[result].desc;
				ammonamestr = mainammo->ammo.specialammo.bulletdata[result].longname;
					break;
				case AMMOTYPE_SHOTGUNSHELL:
					ammodesc = mainammo->ammo.specialammo.shelldata[result].desc;
					ammonamestr = mainammo->ammo.specialammo.shelldata[result].longname;
					break;
				case AMMOTYPE_EXPLOSIVE:
					ammodesc = mainammo->ammo.specialammo.explosivedata[result].desc;
					ammonamestr = mainammo->ammo.specialammo.explosivedata[result].longname;
					break;
				}
				sprintf(buf, "%s\nPlease confirm your choice.", ammodesc);
				if(!YesNoDialogBox(ammonamestr, buf, "(Y)es/(N)o", &globals.dboxparams)){
					continue;
				}
			//	FreeDBStringList(list);
				SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_SMALL, 1, FALSE);
				playWeaponSound(weapon, WEAP_SOUND_RELOAD, SOUND_MODE_MIX);
				sprintf(buf, "The %s has been loaded with %s.\nPress ENTER to continue.", weapon->longname, ammonamestr);
				StaticDialogBox("Weapon", buf, "Loaded", &globals.dboxparams);
				return result;
			}
		}
	}
}

void MeleeCreatePoorOpponent(PENEMY enemy)
{
		ClearEnemyStats(enemy);
	enemy->psychskill.skills.firearms = 2;
	enemy->psychskill.skills.stealthcombat = 1;
		enemy->psychskill.skills.meleecombatNOweapon = 2;
	enemy->psychskill.skills.meleecombatWITHweapon = 1;
	enemy->psychskill.psych.agression = 2;
	enemy->psychskill.psych.willpower = 1;
	enemy->psychskill.psych.intelligence = 2;
	enemy->psychskill.psych.calmness = 1;
	enemy->psychskill.psych.reflex = 2;
}
void MeleeCreateAverageOpponent(PENEMY enemy)
{
		ClearEnemyStats(enemy);
	enemy->psychskill.skills.firearms = 3;
	enemy->psychskill.skills.stealthcombat = 2;
		enemy->psychskill.skills.meleecombatNOweapon = 2;
	enemy->psychskill.skills.meleecombatWITHweapon = 3;
	enemy->psychskill.psych.agression = 4;
	enemy->psychskill.psych.willpower = 3;
	enemy->psychskill.psych.calmness = 3;
	enemy->psychskill.psych.intelligence = 4;
	enemy->psychskill.psych.reflex = 3;
}
void MeleeCreateSkilledOpponent(PENEMY enemy)
{
		ClearEnemyStats(enemy);
	enemy->psychskill.skills.firearms = 4;
	enemy->psychskill.skills.stealthcombat = 4;
		enemy->psychskill.skills.meleecombatNOweapon = 5;
	enemy->psychskill.skills.meleecombatWITHweapon = 5;
	enemy->psychskill.psych.agression = 4;
	enemy->psychskill.psych.willpower = 5;
	enemy->psychskill.psych.calmness = 5;
	enemy->psychskill.psych.intelligence = 5;
	enemy->psychskill.psych.reflex = 5;
}
void MeleeCreateEliteOpponent(PENEMY enemy)
{
			ClearEnemyStats(enemy);
	enemy->psychskill.skills.firearms = 7;
	enemy->psychskill.skills.stealthcombat = 8;
		enemy->psychskill.skills.meleecombatNOweapon = 8;
	enemy->psychskill.skills.meleecombatWITHweapon = 7;
	enemy->psychskill.psych.agression = 8;
	enemy->psychskill.psych.willpower = 7;
	enemy->psychskill.psych.calmness = 8;
	enemy->psychskill.psych.intelligence = 8;
	enemy->psychskill.psych.reflex = 7;
}

void MeleeTrainingMessage(int result)
{
#define RGB_RED 0
#define RGB_GREEN 1
#define RGB_BLUE 2
	char buf[2000];
	
	int rgb = RGB_RED;
	switch(result){
	case MELEE_END_PLAYERCYANIDE:
		strcpy(buf, "Simulation Ended.\nHad this been a real close-quarters combat scenario, you would have expired after self-administration of your cyanide. Note that OFFICIAL DSA POLICY MANDATES that you self-administer cyanide prior to capture if you are in a weakened state. The alternative is torture, compromised national security, and a painful death. While the possibility of escape from imprisonment may present itself, DSA does not see this as a consistently viable method of operation. Ultimately, the call is yours. Do as you see fit.\nPress ENTER to Continue.");
		rgb = RGB_BLUE;
		break;
	case MELEE_END_PLAYERBLUNTUNCONSCIOUS:
		strcpy(buf, "Simulation Ended.\nHad this been a real close-quarters combat scenario, you would have been captured and taken prisoner. What would follow would be torture in an attempt to get you to divulge information about DSA. The OFFICIAL DSA POLICY in this instance is to try and escape any way possible. However, it is best to avoid this situation entirely.\nSome Tips:\nTry to block your opponent's hits.\nStay a fair distance away to ensure maximum reaction time.\nIf in a weakened position with no hope for reversal, self-administer cyanide.\n\nPress ENTER to Continue.");
		rgb = RGB_RED;
		break;
	case MELEE_END_PLAYERBLOODLOSSDEAD:
		strcpy(buf, "Simulation Ended.\nHad this been a real close-quarters combat scenario, you would have been captured and taken prisoner. What would follow would be torture in an attempt to get you to divulge information about DSA. The OFFICIAL DSA POLICY in this instance is to try and escape any way possible. However, it is best to avoid this situation entirely.\nSome Tips:\nTry to block your opponent's hits.\nAttempt hits on shoulders and legs to cripple and force a weapon drop.\nDraw and use your own weapon to counter the opponent's weapon.\nIf in a weakened position with no hope for reversal, self-administer cyanide.\n\nPress ENTER to Continue.");
		rgb = RGB_RED;
		break;
	case MELEE_END_PLAYERSHOTDEAD:
		strcpy(buf, "Simulation Ended.\nHad this been a real close-quarters combat scenario, you would have been captured and taken prisoner. What would follow would be torture in an attempt to get you to divulge information about DSA. The OFFICIAL DSA POLICY in this instance is to try and escape any way possible. However, it is best to avoid this situation entirely.\nSome Tips:\nGet in close to force your opponent to miss.\nTry to disarm your opponent, or utilize your own firearm.\nLunge at your opponent and attempt strangulation if the situation presents itself.\nIf in a weakened position with no hope for reversal, self-administer cyanide.\n\nPress ENTER to Continue.");
		rgb = RGB_RED;
		break;
	case MELEE_END_PLAYERSTRANGLEDDEAD:
		strcpy(buf, "Simulation Ended.\nHad this been a real close-quarters combat scenario, you would have been captured and taken prisoner. What would follow would be torture in an attempt to get you to divulge information about DSA. The OFFICIAL DSA POLICY in this instance is to try and escape any way possible. However, it is best to avoid this situation entirely.\nSome Tips:\nGain distance to give yourself reaction time against strangulation.\nWatch your stamina! If you are weakened, you are an easy target for strangulation.\nIf in a weakened position with no hope for reversal, self-administer cyanide.\n\nPress ENTER to Continue.");
		rgb = RGB_RED;
		break;
	case MELEE_END_PLAYERDRUGUNCONSCIOUS:
	case MELEE_END_PLAYERDRUGDEAD:
		strcpy(buf, "Simulation Ended.\nHad this been a real close-quarters combat scenario, you would have been captured and taken prisoner. What would follow would be torture in an attempt to get you to divulge information about DSA. The OFFICIAL DSA POLICY in this instance is to try and escape any way possible. However, it is best to avoid this situation entirely.\nSome Tips:\nGain distance to keep the syringe away from your body.\nTry to use a firearm to force your opponent to drop the syringe.\nIf in a weakened position with no hope for reversal, self-administer cyanide.\n\nPress ENTER to Continue.");
		rgb = RGB_RED;
		break;
	case MELEE_END_ENEMYBLUNTUNCONSCIOUS:
		strcpy(buf, "Simulation Ended.\nHad this been a real close-quarters combat scenario, your opponent would have expired due to massive blunt trauma. Congratulations on your kill. This method of attack alone is difficult, but for the most part silent. Attempt to utilize heavier objects for greater impact.\n\nPress ENTER to Continue.");
		rgb = RGB_GREEN;
		break;
	case MELEE_END_ENEMYBLOODLOSSDEAD:
		strcpy(buf, "Simulation Ended.\nHad this been a real close-quarters combat scenario, your opponent would have expired due to blood loss and massive nerve trauma. The use of sharp objects for cutting is an incredible advantage in any melee combat situation. The creation of open wounds allows for attacks to those vulnerable areas for an increase in damage. Congratulations on a job well done.\n\nPress ENTER to Continue.");
		rgb = RGB_GREEN;
		break;
	case MELEE_END_ENEMYSHOTDEAD:
		strcpy(buf, "Simulation Ended.\nHad this been a real close-quarters combat scenario, your opponent would have expired due to blood loss and massive nerve trauma. The use of firearms in melee combat is risky due to the short distances involved, but it can tip the balance of the fight immediately. As a note, should you find the use of firearms necessary, use silenced weapons so as not to direct attention to yourself. Congratulations on an excellent kill.\n\nPress ENTER to Continue.");
		rgb = RGB_GREEN;
		break;
	case MELEE_END_ENEMYSTRANGLEDDEAD:
		strcpy(buf, "Simulation Ended.\nHad this been a real close-quarters combat scenario, your opponent would have expired due to lack of oxygen to the brain on account of strangulation. Strangulation is a deciding factor in melee combat, but can backfire quickly. Always make sure your opponent is physically weakened before attempting strangulation. Congratulations on your kill.\n\nPress ENTER to Continue.");
		rgb = RGB_GREEN;
		break;
	case MELEE_END_ENEMYDRUGUNCONSCIOUS:
	case MELEE_END_ENEMYDRUGDEAD:
		strcpy(buf, "Simulation Ended.\nHad this been a real close-quarters combat scenario, your opponent would have expired due to heart stoppage on account of poisoning. Drugs introduce an interesting element to melee combat. The use of a drug is an instant deciding factor in combat, though a risky one. Congratulations on a kill well executed.\n\nPress ENTER to Continue.");
		
		rgb = RGB_GREEN;
		break;
	default:
		strcpy(buf, "Simulation Ended.\nNo Data Is Available.\n\nPress ENTER to Continue.");
		rgb = RGB_BLUE;
		break;
	}
	switch(rgb){
	case RGB_BLUE:
		SetGlobalDBoxParam(DB_SCHEME_COOLBLUELINES, DB_LARGE, 0, FALSE);
		break;
	case RGB_RED:
		SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_LARGE, 0, FALSE);
		break;
	case RGB_GREEN:
		SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_LARGE, 0, FALSE);
		break;
	}
	StaticDialogBox("Simulation", buf, "Results", &globals.dboxparams);
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleePrintActionString
//
// Description   - Fades in a given string into the melee action box.
//
// Return Type   - void 
//
// Arguments     - char* string ( The string to print )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void MeleePrintActionString(char* string)
{
	
	COORD textboxdimensions;
	COORD textboxtopleft;
	RECT textbox;
	
	textbox.left = MELEE_TEXTBOX_TOPLEFT_X;
	textbox.top = MELEE_TEXTBOX_TOPLEFT_Y + 2;
	textbox.right = textbox.left + MELEE_TEXTBOX_WIDTH - 1;
	textbox.bottom = textbox.top + MELEE_TEXTBOX_HEIGHT - 1;
	
	textboxtopleft.X = (short)textbox.left;
	textboxtopleft.Y = (short)textbox.top;
	
	textboxdimensions.X = MELEE_TEXTBOX_WIDTH - 1;
	textboxdimensions.Y = MELEE_TEXTBOX_HEIGHT - 1;
	
	
	
	if (strlen(string) > 0 && strlen(string) < MELEE_TEXTBOX_WIDTH - 1)
	{
		ClearRect(textbox);
		RED;
		setcursor(retrieveTextCenterH(string), MELEE_TEXTBOX_TOPLEFT_Y + 2);
		printf(string);
		Sleep(40);
		LRED;
		setcursor(retrieveTextCenterH(string), MELEE_TEXTBOX_TOPLEFT_Y + 2);
		printf(string);
		strncpy(globals.globalmeleestruct.actionstring, string, sizeof(globals.globalmeleestruct.actionstring));
	}
	else if (strlen(string) > 0 && strlen(string) >= MELEE_TEXTBOX_WIDTH - 1){
		
		
		
		ClearRect(textbox);
		RED;
		printwordwrapcoordinate(string, textboxdimensions, textboxtopleft);
		Sleep(40);
		LRED;
		printwordwrapcoordinate(string, textboxdimensions, textboxtopleft);
	}
	
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleePrintActionStringHold
//
// Description   - Prints a melee action string and waits.
//
// Return Type   - void 
//
// Arguments     - char* string ( The string to print )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void MeleePrintActionStringHold(char* string)
{
	MeleePrintActionString(string);
	Sleep(1500);
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleeGetPlayerMove
//
// Description   - Given a key, change the player's last action and return "valid" or not.
//
// Return Type   - int 
//
// Arguments     - PPLAYER player ( The pointer to the player )
//               - int key ( The key pressed )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
int MeleeGetPlayerMove(PPLAYER player, int key)
{	
				//DO SOME MORE CHECKING HERE, BASED ON CURRENT ACTION, BEFORE RETURNING VALID.
	if (key == VK_NUMPAD8){
		player->meleestruct.lasttarget = MELEE_TARGET_HEAD;
		player->meleestruct.lastaction = MELEE_ACTION_ATTACK;
		return MELEE_VALID_ACTION;
	}
	else if (key == VK_NUMPAD7){
		player->meleestruct.lasttarget = MELEE_TARGET_LSHOULDER;
		player->meleestruct.lastaction = MELEE_ACTION_ATTACK;
		return MELEE_VALID_ACTION;
	}
	else if(key == VK_NUMPAD9){
		player->meleestruct.lasttarget = MELEE_TARGET_RSHOULDER;
		player->meleestruct.lastaction = MELEE_ACTION_ATTACK;
		return MELEE_VALID_ACTION;
	}
	else if(key == VK_NUMPAD5){
		player->meleestruct.lasttarget = MELEE_TARGET_TORSO;
		player->meleestruct.lastaction = MELEE_ACTION_ATTACK;
		return MELEE_VALID_ACTION;
	}
	else if(key == VK_NUMPAD4){
		player->meleestruct.lasttarget = MELEE_TARGET_LSIDE;
		player->meleestruct.lastaction = MELEE_ACTION_ATTACK;
		return MELEE_VALID_ACTION;
	}
	else if(key == VK_NUMPAD6){
		player->meleestruct.lasttarget = MELEE_TARGET_RSIDE;
		player->meleestruct.lastaction = MELEE_ACTION_ATTACK;
		return MELEE_VALID_ACTION;
	}
	else if(key == VK_NUMPAD1){
		player->meleestruct.lasttarget = MELEE_TARGET_LLEG;
		player->meleestruct.lastaction = MELEE_ACTION_ATTACK;
		return MELEE_VALID_ACTION;
	}
	else if(key == VK_NUMPAD2){
		player->meleestruct.lasttarget = MELEE_TARGET_ABDOMEN;
		player->meleestruct.lastaction = MELEE_ACTION_ATTACK;
		return MELEE_VALID_ACTION;
	}
	else if(key == VK_NUMPAD3){
		player->meleestruct.lasttarget = MELEE_TARGET_RLEG;
		player->meleestruct.lastaction = MELEE_ACTION_ATTACK;
		return MELEE_VALID_ACTION;
	}
	
	// IF IT'S NONE OF THOSE....THEN CHECK FOR SPECIAL OFFENSIVE KEYS
				if (globals.globalmeleestruct.playerturn) //THESE KEYS CAN ONLY BE USED FOR OFFENSE
				{
					if (key == VK_RETURN){
						player->meleestruct.lastaction = MELEE_ACTION_CHANGEMOVE;
						MeleePlayerChangeMove(player);
						return MELEE_NO_ACTION; //IT'S NOT A "VALID" ACTION
					}
					else if (key == VK_ADD){
						player->meleestruct.lastaction = MELEE_ACTION_GAINDISTANCE;
						return MELEE_VALID_ACTION;
					}
					else if (key == VK_SUBTRACT){
						player->meleestruct.lastaction = MELEE_ACTION_CLOSEDISTANCE;
						return MELEE_VALID_ACTION;
					}
					else if (key == VK_NUMPAD0){
						player->meleestruct.lastaction = MELEE_ACTION_DRAWWEAPON;
						
						return MELEE_VALID_ACTION;
					}

					else if (key == VK_MULTIPLY){
						player->meleestruct.lastaction = MELEE_ACTION_STRANGLE;
						return MELEE_VALID_ACTION;
					}
					else if (key == 191){
						if (player->currentweapon == NULL){
							MeleePrintActionString("You Have No Drugged Weapon!");
							return MELEE_NO_ACTION;
						}
						if (!(player->currentweapon->attacktype & WEAP_ATT_TYPE_DRUG) || player->currentweapon->drug.type == 0){
							MeleePrintActionString("This Weapon Does Not Contain A Drug!");
							return MELEE_NO_ACTION;
						}
						if (!player->currentweapon->drug.doseloaded){
							//ALLOW THEM TO INJECT AIR?
							MeleePrintActionString("This Weapon Does Not Have A Drug Loaded!");
							return MELEE_NO_ACTION;
						}
						if (player->currentweapon->drug.type == POISON_KETAMINE){
							player->meleestruct.lastaction = MELEE_ACTION_DRUGUNCONSCIOUS;
							return MELEE_VALID_ACTION;
						}
						else{
							player->meleestruct.lastaction = MELEE_ACTION_DRUGDEATH;
							return MELEE_VALID_ACTION;
						}
						
					}
					else if (key == 'C'){
						player->meleestruct.lastaction = MELEE_ACTION_CYANIDE;
						return MELEE_VALID_ACTION;
					}
					
				}
				FLUSH;
				
				clearinputrecords();
				return MELEE_NO_ACTION;
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleeResolvePlayerPostures
//
// Description   - Resolves Player Posture Changes
//
// Return Type   - BOOL 
//
// Arguments     - PPLAYER player ( The pointer to the player )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
BOOL MeleeResolvePlayerPostures(PPLAYER player)
{
	if (player->meleestruct.posture == MELEE_POSTURE_PRONE)
	{
		globals.globalmeleestruct.playerturn = FALSE;
		globals.globalmeleestruct.freeturn = TRUE;
		player->meleestruct.posture = MELEE_POSTURE_KNEELING;
		MeleePrintActionStringHold("You get up from the ground to a kneel.");
		return TRUE;
	}
	else if (player->meleestruct.posture == MELEE_POSTURE_KNEELING)
	{
		globals.globalmeleestruct.playerturn = FALSE;
		globals.globalmeleestruct.freeturn = TRUE;
		player->meleestruct.posture = MELEE_POSTURE_STANDING;
		MeleePrintActionStringHold("You get up to your feet.");
		return TRUE;
	}
	else if (player->meleestruct.posture == MELEE_POSTURE_STANDING)
	{
		return FALSE;
	}
	return FALSE;
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleeResolveEnemyPostures
//
// Description   - Resolves enemy posture changes.
//
// Return Type   - int 
//
// Arguments     - PENEMY enemy ( The pointer to the enemy. )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
int MeleeResolveEnemyPostures(PENEMY enemy)
{
	if (enemy->meleestruct.posture == MELEE_POSTURE_PRONE)
	{
		globals.globalmeleestruct.playerturn = TRUE;
		globals.globalmeleestruct.freeturn = TRUE;
		enemy->meleestruct.posture = MELEE_POSTURE_KNEELING;
		MeleePrintActionStringHold("You see the enemy rise to a kneel.");
		return TRUE;
	}
	else if (enemy->meleestruct.posture == MELEE_POSTURE_KNEELING)
	{
		globals.globalmeleestruct.playerturn = TRUE;
		globals.globalmeleestruct.freeturn = TRUE;
		enemy->meleestruct.posture = MELEE_POSTURE_STANDING;
		MeleePrintActionStringHold("You see the enemy rise to their feet.");
		return TRUE;
	}
	else if (enemy->meleestruct.posture == MELEE_POSTURE_STANDING)
	{
		return FALSE;
	}
	return FALSE;
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleeResolveDeath
//
// Description   - Returns the type of death based on the last actions performed.
//
// Return Type   - int 
//
// Arguments     - PPLAYER player ( The pointer to the player )
//               - PENEMY enemy ( The pointer to the enemy )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
int MeleeResolveDeath(PPLAYER player, PENEMY enemy)
{
/*
#define MELEE_ACTION_NONE				0
#define MELEE_ACTION_ATTACK				1
#define MELEE_ACTION_FIREWEAPON			2
#define MELEE_ACTION_CUT				3
#define MELEE_ACTION_STRANGLE			4
#define MELEE_ACTION_DRUGDEATH			5
#define MELEE_ACTION_DRUGUNCONSCIOUS	6
#define MELEE_ACTION_CYANIDE			7
#define MELEE_ACTION_CLOSEDISTANCE		8
#define MELEE_ACTION_GAINDISTANCE		9
#define MELEE_ACTION_DRAWWEAPON			10
#define MELEE_ACTION_THROWWEAPON		11
	*/
	if (player->meleestruct.stamina > 0 && enemy->meleestruct.stamina > 0)
	{
		return MELEE_CONTINUE;
	}
	if(player->meleestruct.lastaction == MELEE_ACTION_CYANIDE){
		return MELEE_END_PLAYERCYANIDE;
	}
	if (player->meleestruct.stamina <= 0)
	{
		if (enemy->meleestruct.lastaction == MELEE_ACTION_PUNCH || enemy->meleestruct.lastaction == MELEE_ACTION_KICK || enemy->meleestruct.lastaction == MELEE_ACTION_HITWITHWEAPON || enemy->meleestruct.lastaction == MELEE_ACTION_THROWWEAPON){
			return MELEE_END_PLAYERBLUNTUNCONSCIOUS;
		}
		else if (enemy->meleestruct.lastaction == MELEE_ACTION_CUT){
			return MELEE_END_PLAYERBLOODLOSSDEAD;
		}
		else if (enemy->meleestruct.lastaction == MELEE_ACTION_FIREWEAPON){
			return MELEE_END_PLAYERSHOTDEAD;
		}
		else if (enemy->meleestruct.lastaction == MELEE_ACTION_STRANGLE){
			return MELEE_END_PLAYERSTRANGLEDDEAD;
		}
		else if (enemy->meleestruct.lastaction == MELEE_ACTION_DRUGUNCONSCIOUS){
			return MELEE_END_PLAYERDRUGUNCONSCIOUS;
		}
		else if(enemy->meleestruct.lastaction == MELEE_ACTION_DRUGDEATH){
			return MELEE_END_PLAYERDRUGDEAD;
		}
		else{
			return MELEE_END_PLAYERBLUNTUNCONSCIOUS;
		}
	}
	else if (enemy->meleestruct.stamina <= 0)
	{
		if (player->meleestruct.lastaction == MELEE_ACTION_PUNCH || player->meleestruct.lastaction == MELEE_ACTION_KICK || player->meleestruct.lastaction == MELEE_ACTION_HITWITHWEAPON || player->meleestruct.lastaction == MELEE_ACTION_THROWWEAPON){
			return MELEE_END_ENEMYBLUNTUNCONSCIOUS;
		}
		else if (player->meleestruct.lastaction == MELEE_ACTION_CUT){
			return MELEE_END_ENEMYBLOODLOSSDEAD;
		}
		else if (player->meleestruct.lastaction == MELEE_ACTION_FIREWEAPON){
			return MELEE_END_ENEMYSHOTDEAD;
		}
		else if (player->meleestruct.lastaction == MELEE_ACTION_STRANGLE){
			return MELEE_END_ENEMYSTRANGLEDDEAD;
		}
		else if (player->meleestruct.lastaction == MELEE_ACTION_DRUGUNCONSCIOUS){
			return MELEE_END_ENEMYDRUGUNCONSCIOUS;
		}
		else if(player->meleestruct.lastaction == MELEE_ACTION_DRUGDEATH){
			return MELEE_END_ENEMYDRUGDEAD;
		}
		else{
			return MELEE_END_ENEMYBLUNTUNCONSCIOUS;
		}
		
	}
	return MELEE_CONTINUE;
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleeGetEnemyMoveOffensive
//
// Description   - Retrieves an enemy offensive move. THIS IS WHERE AI DECIDES WHAT TO DO NEXT!!!!
//
// Return Type   - void 
//
// Arguments     - PENEMY enemy ( The pointer to the enemy )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void MeleeGetEnemyMoveOffensive(PPLAYER player, PENEMY enemy)
{
	char bodyparts[MAX_BODY_PARTS][60] = {
			"Right Leg",
			"Abdomen",
			"Left Leg",
			"Right Side",
			"Chest",
			"Left Side",
			"Right Shoulder",
			"Head",
			"Left Shoulder"};

			char stabbodyparts[MAX_BODY_PARTS][60] = {
			"Right Leg",
			"Abdomen",
			"Left Leg",
			"Right Side",
			"Chest",
			"Left Side",
			"Right Shoulder",
			"Neck",
			"Left Shoulder"};
		
		char resultstring[500];
		int priority;
		bool angerwins;
		int angerhonorfactor;
		int actionreference;
		
		
		//FIRST, GET PRIORITY.
		priority = MeleeAIGetCurrentPriority(player, enemy);
		angerhonorfactor = (3 * globals.globalmeleestruct.enemyanger / 5) - (2 * globals.globalmeleestruct.enemyhonor / 5);
		
		switch(priority)
		{
				case 1: //PRIORITY ONE - PLAYER ON GROUND.
			angerwins = (angerhonorfactor >= 15 ? true : false);
			if (angerwins){
				if(HasMeleeAmmunition(enemy->currentweapon)){
					actionreference = MELEEAI_SHOOTPLAYER_CRITICAL;
				}
				else if(enemy->currentweapon != NULL && (enemy->currentweapon->attacktype & WEAP_ATT_TYPE_CUT || enemy->currentweapon->attacktype & WEAP_ATT_TYPE_HIT)){
					actionreference = MELEEAI_WEAPON_ATTACK_CRITICAL;
				}
				else if(MeleeHasAnyAcceptableWeapons(enemy->weaponlist, WEAP_ATT_TYPE_CUT)){
					actionreference = MELEEAI_DRAWKNIFE;
				}
				else if(MeleeHasAnyAcceptableWeapons(enemy->weaponlist, WEAP_ATT_TYPE_HIT)){
					actionreference = MELEEAI_DRAWSTANDARD;
				}
				else{
					actionreference = MELEEAI_STRANGLE_PLAYER;
				}
				
			}
			else{ //HONOR WINS
				if(RandomPercentage(50) && globals.globalmeleestruct.distance > MELEE_MIN_DISTANCE){
					actionreference = MELEEAI_CLOSEDISTANCE;
				}
				else if (globals.globalmeleestruct.distance < MELEE_MAX_DISTANCE){
					actionreference = MELEEAI_GAINDISTANCE;
				}
				else{
					actionreference = MELEEAI_CLOSEDISTANCE;
				}
			}
			break;
		case 2: //PRIORITY TWO - PLAYER HAS SYRINGE OR GUN DRAWN
			angerwins = (angerhonorfactor >= 10 ? true : false);
			if (angerwins){
				if(HasMeleeAmmunition(enemy->currentweapon)){
					if(RandomPercentage(50)){
						actionreference=  MELEEAI_SHOOTPLAYER_CRITICAL;
					}
					else{
						actionreference= MELEEAI_SHOOTPLAYER_DISABLE;
					}
				}
				else if (enemy->currentweapon != NULL && enemy->currentweapon->attacktype & WEAP_ATT_TYPE_THROW && RandomPercentage(80)){
					if(RandomPercentage(30)){
						actionreference = MELEEAI_WEAPON_THROW_CRITICAL;
					}
					else if (enemy->currentweapon != NULL && enemy->currentweapon->attacktype & WEAP_ATT_TYPE_HIT ||enemy->currentweapon->attacktype & WEAP_ATT_TYPE_CUT ){
						actionreference=  MELEEAI_WEAPON_ATTACK_CRITICAL;
					}
				}
				else if (RandomPercentage(20)){
					actionreference = MELEEAI_STRANGLE_PLAYER;
				}
				else if (RandomPercentage(30))
				{
					if(MeleeHasAnyAcceptableWeapons(enemy->weaponlist, WEAP_ATT_TYPE_SHOOT)){
						actionreference = MELEEAI_DRAWGUN;
					}
					else if(MeleeHasAnyAcceptableWeapons(enemy->weaponlist, WEAP_ATT_TYPE_CUT)){
					actionreference = MELEEAI_DRAWKNIFE;
					}
					else if(RandomPercentage(50) && MeleeHasAnyAcceptableWeapons(enemy->weaponlist, WEAP_ATT_TYPE_HIT)){
						actionreference = MELEEAI_DRAWSTANDARD;
					}
					else{
						actionreference = MELEEAI_KICKPLAYER_DISABLE;
					}
				}
				else{
					actionreference = MELEEAI_PUNCHPLAYER_DISABLE;
				}
			}
			else{ //HONOR WINS
				if(RandomPercentage(50)){
					actionreference = MELEEAI_PUNCHPLAYER_DISABLE;
				}
				else{
					actionreference = MELEEAI_KICKPLAYER_DISABLE;
				}
			}
			
			break;
	
		case 3: //PRIORITY THREE - SELF NEARLY DEAD
			angerwins = (angerhonorfactor >= 20 ? true : false);
			if (angerwins){
				if(enemy->currentweapon != NULL && HasMeleeAmmunition(enemy->currentweapon)){
					actionreference = MELEEAI_SHOOTPLAYER_CRITICAL;
				}
				else if(MeleeCurrentWeaponHasAction(enemy->currentweapon, WEAP_ATT_TYPE_THROW) && RandomPercentage(20)){
					actionreference= MELEEAI_WEAPON_THROW_ANYWHERE;
				}
				else if(MeleeCurrentWeaponHasAction(enemy->currentweapon, WEAP_ATT_TYPE_CUT)){
					actionreference= MELEEAI_WEAPON_ATTACK_ANYWHERE;
				}
				
				else if(MeleeCurrentWeaponHasAction(enemy->currentweapon, WEAP_ATT_TYPE_HIT)){
					actionreference= MELEEAI_WEAPON_ATTACK_ANYWHERE;
				}
				else if (MeleeHasAnyAcceptableWeapons(enemy->weaponlist, WEAP_ATT_TYPE_SHOOT)){
					actionreference = MELEEAI_DRAWGUN;
				}
				else{
					actionreference = MELEEAI_KICKPLAYER_CRITICAL;
				}
			}
			else{//HONOR WINS
				
				if(MeleeCurrentWeaponHasAction(enemy->currentweapon, WEAP_ATT_TYPE_HIT)){
					actionreference = MELEEAI_WEAPON_ATTACK_ANYWHERE;
				}
				
				else if (RandomPercentage(40)){ //JUST ATTACK
					if(RandomPercentage(50)){
						actionreference = MELEEAI_PUNCHPLAYER_ANYWHERE;
					}
					else{
						actionreference = MELEEAI_KICKPLAYER_ANYWHERE;
					}
				}
				else{ //DRAW A WEAPON
					if(MeleeHasAnyAcceptableWeapons(enemy->weaponlist, WEAP_ATT_TYPE_HIT) && RandomPercentage(50)){
						actionreference=  MELEEAI_DRAWSTANDARD;
					}
					else if (MeleeHasAnyAcceptableWeapons(enemy->weaponlist, WEAP_ATT_TYPE_CUT)){
						actionreference = MELEEAI_DRAWKNIFE;
					}
				}
			}
			break;
		case 4: //PRIORITY FOUR - PLAYER HAS KNIFE OR STANDARD DRAWN
			angerwins = (angerhonorfactor >= 25 ? true : false);
			if (angerwins){
				if(enemy->currentweapon != NULL && HasMeleeAmmunition(enemy->currentweapon)){
					if(RandomPercentage(50)){//SHOOT FIREARM ANYWHERE
						actionreference = MELEEAI_SHOOTPLAYER_ANYWHERE;
					}
					else{//SHOOT FIREARM DISABLE
						actionreference = MELEEAI_SHOOTPLAYER_DISABLE;
					}
					
				}
				
				//ATTACK STANDARD ANYWHERE.
				else if(MeleeCurrentWeaponHasAction(enemy->currentweapon, WEAP_ATT_TYPE_CUT) || MeleeCurrentWeaponHasAction(enemy->currentweapon, WEAP_ATT_TYPE_HIT)){
					if(RandomPercentage(50)){
						actionreference= MELEEAI_WEAPON_ATTACK_ANYWHERE;
					}
					else{	//ATTACK STANDARD DISABLE
						actionreference = MELEEAI_WEAPON_ATTACK_DISABLE;
					}
				}
				
				//DRAW FIREARM
				
				else if (MeleeHasAnyAcceptableWeapons(enemy->weaponlist, WEAP_ATT_TYPE_SHOOT) && RandomPercentage(50))
				{
					actionreference= MELEEAI_DRAWGUN;
				}
				
				//DRAW STANDARD
				else if(MeleeHasAnyAcceptableWeapons(enemy->weaponlist, WEAP_ATT_TYPE_HIT) && RandomPercentage(50)){
					actionreference=  MELEEAI_DRAWSTANDARD;
				}
				else if (MeleeHasAnyAcceptableWeapons(enemy->weaponlist, WEAP_ATT_TYPE_CUT)){
					actionreference = MELEEAI_DRAWKNIFE;
				}
				else{
					actionreference = MELEEAI_KICKPLAYER_ANYWHERE;
				}
				
			}
			else{ //HONOR WINS
				if(RandomPercentage(70)){ //ATTACK
					if(RandomPercentage(50)){
						actionreference = MELEEAI_KICKPLAYER_DISABLE;
					}
					else{
						actionreference = MELEEAI_KICKPLAYER_ANYWHERE;
					}
				}
				else{ //GAIN/CLOSE
					if(RandomPercentage(50) && globals.globalmeleestruct.distance > MELEE_MIN_DISTANCE){
						actionreference = MELEEAI_CLOSEDISTANCE;
					}
					else if(globals.globalmeleestruct.distance < MELEE_MAX_DISTANCE){
						actionreference = MELEEAI_GAINDISTANCE;
					}
					else{
						actionreference = MELEEAI_CLOSEDISTANCE;
					}
				}
			}
			break;
		case 5:  //CASE FIVE AND SIX MERGED
		case 6:
			angerwins = (angerhonorfactor >= 35 ? true : false);
			if (angerwins){
				
				//ATTACK WITH STANDARD WEAPON
				//THROW STANDARD WEAPON
				//DRAW STANDARD WEAPON
				if(RandomPercentage(50)){
					if(MeleeCurrentWeaponHasAction(enemy->currentweapon, WEAP_ATT_TYPE_THROW) && RandomPercentage(20)){
						actionreference= MELEEAI_WEAPON_THROW_ANYWHERE;
					}
					else if(MeleeCurrentWeaponHasAction(enemy->currentweapon, WEAP_ATT_TYPE_CUT)){
						actionreference= MELEEAI_WEAPON_ATTACK_ANYWHERE;
					}
					
					else if(MeleeCurrentWeaponHasAction(enemy->currentweapon, WEAP_ATT_TYPE_HIT)){
						actionreference= MELEEAI_WEAPON_ATTACK_ANYWHERE;
					}
					
					else{ //TRY TO DRAW STANDARD WEAPON.
						if(MeleeHasAnyAcceptableWeapons(enemy->weaponlist, WEAP_ATT_TYPE_HIT) && RandomPercentage(50)){
							actionreference=  MELEEAI_DRAWSTANDARD;
						}
						else if (MeleeHasAnyAcceptableWeapons(enemy->weaponlist, WEAP_ATT_TYPE_CUT)){
							actionreference = MELEEAI_DRAWKNIFE;
						}
					}
				}
				
				else{
				}
				if(RandomPercentage(30)){
					//CLOSE DISTANCE
				//GAIN DISTANCE
						if(RandomPercentage(50) && globals.globalmeleestruct.distance > MELEE_MIN_DISTANCE){
						actionreference = MELEEAI_CLOSEDISTANCE;
					}
					else if(globals.globalmeleestruct.distance < MELEE_MAX_DISTANCE){
						actionreference = MELEEAI_GAINDISTANCE;
					}
					else{
						actionreference = MELEEAI_CLOSEDISTANCE;
					}
				}
				else{
					//PUNCH, KICK
					if(RandomPercentage(50)){
						actionreference = MELEEAI_KICKPLAYER_ANYWHERE;
					}
					else{
						actionreference = MELEEAI_PUNCHPLAYER_ANYWHERE;
					}
				}
			}
			else{ //HONOR WINS
				if(RandomPercentage(50)){
					if(RandomPercentage(50)){
						actionreference = MELEEAI_KICKPLAYER_ANYWHERE;
					}
					else{
						actionreference = MELEEAI_PUNCHPLAYER_ANYWHERE;
					}
				}
				else{
							if(RandomPercentage(50) && globals.globalmeleestruct.distance > MELEE_MIN_DISTANCE){
						actionreference = MELEEAI_CLOSEDISTANCE;
					}
					else if(globals.globalmeleestruct.distance < MELEE_MAX_DISTANCE){
						actionreference = MELEEAI_GAINDISTANCE;
					}
					else{
						actionreference = MELEEAI_CLOSEDISTANCE;
					}
				}
			}
			break;
		}
		
		

		//ACTION RESOLUTION SWITCH....
		MeleeAIActionReferenceResolution(enemy, actionreference);
		
		//PRINT OUT HIS ACTION!!!!
		switch(enemy->meleestruct.lastaction)
		{
		case MELEE_ACTION_KICK:
			sprintf(resultstring, "He Kicks Toward Your %s!", bodyparts[enemy->meleestruct.lasttarget]);
			MeleePrintActionString(resultstring);
			break;
		case MELEE_ACTION_PUNCH:
			sprintf(resultstring, "He Throws A Punch At Your %s!", bodyparts[enemy->meleestruct.lasttarget]);
			MeleePrintActionString(resultstring);
			break;
		case MELEE_ACTION_DRAWWEAPON:
			MeleePrintActionString("You See Him Reach In His Vest...");
			break;
		case MELEE_ACTION_STRANGLE:
			MeleePrintActionString("He Goes For Your Throat!");
			break;
		case MELEE_ACTION_CUT:
			sprintf(resultstring, "He Swings Toward Your %s With His %s!", stabbodyparts[enemy->meleestruct.lasttarget], enemy->currentweapon->shortname);
			MeleePrintActionString(resultstring);
			break;
		case MELEE_ACTION_HITWITHWEAPON:
			sprintf(resultstring, "He Strikes At Your %s With His %s!", bodyparts[enemy->meleestruct.lasttarget], enemy->currentweapon->shortname);
			MeleePrintActionString(resultstring);
			break;
		case MELEE_ACTION_THROWWEAPON:
			sprintf(resultstring, "He Throws His %s At Your %s!", enemy->currentweapon->shortname, stabbodyparts[enemy->meleestruct.lasttarget]);
			playMIXsound(MELEE_SOUND_KNIFETHROW);
			MeleePrintActionString(resultstring);
			break;
		case MELEE_ACTION_FIREWEAPON:
			sprintf(resultstring, "He Aims At Your %s And Pulls The Trigger...", bodyparts[enemy->meleestruct.lasttarget]);
			MeleePrintActionString(resultstring);
			break;
		case MELEE_ACTION_CLOSEDISTANCE:
		case MELEE_ACTION_GAINDISTANCE:
			MeleePrintActionString("You See Him Shift His Weight.");
			break;
		default:
			MeleePrintActionString("He Does Something ELSE!!!!");
			break;
		}	
}

void MeleeAIPrepareToDrawSuitableWeapon(PENEMY enemy, int attacktype)
{
	PWEAPON* weaponarray; //AN ARRAY OF WEAPON POINTERS
	int numweapons = 0;
	PLIST list = enemy->weaponlist;
	if (list == NULL){
		return;
	}
	if (enemy->weaponlist->objcnt < 1){
		return;
	}


	//IF THE ATTACKTYPE IS SET TO NOTHING, INVERT IT.
	if (!attacktype){
		attacktype ^= attacktype;
	}

	list->current = list->head;
	while(list->current != NULL){
		if(list->current->object.weapon.attacktype & attacktype){
			numweapons++;
		}
		list->current = list->current->nextnode;
	}

	if(numweapons > 0){
		weaponarray = (PWEAPON*)malloc(sizeof(PWEAPON) * numweapons);
	}
	list->current = list->head;
	int i = 0;
	while(list->current != NULL){
		if(list->current->object.weapon.attacktype & attacktype){
			weaponarray[i] = &list->current->object.weapon;
			i++;
		}
		list->current = list->current->nextnode;
	}
	i = rand()%numweapons;
	globals.globalmeleestruct.enemyweapontodraw = weaponarray[i];
}

void MeleeAIActionReferenceResolution(PENEMY enemy, int actionref)
{
	switch(actionref)
	{
		
	case MELEEAI_DRAWKNIFE:
		enemy->meleestruct.lastaction = MELEE_ACTION_DRAWWEAPON;
		MeleeAIPrepareToDrawSuitableWeapon(enemy, WEAP_ATT_TYPE_CUT);
		enemy->meleestruct.lasttarget = BODY_HEAD;
		break;
	case MELEEAI_DRAWGUN:
		enemy->meleestruct.lastaction = MELEE_ACTION_DRAWWEAPON;
		MeleeAIPrepareToDrawSuitableWeapon(enemy, WEAP_ATT_TYPE_SHOOT);
		enemy->meleestruct.lasttarget = BODY_HEAD;
		break;
	case MELEEAI_DRAWSTANDARD:
		enemy->meleestruct.lastaction = MELEE_ACTION_DRAWWEAPON;
		MeleeAIPrepareToDrawSuitableWeapon(enemy, WEAP_ATT_TYPE_HIT);
		enemy->meleestruct.lasttarget = BODY_HEAD;
		break;
	case MELEEAI_DRAWANY:
		enemy->meleestruct.lastaction = MELEE_ACTION_DRAWWEAPON;
		MeleeAIPrepareToDrawSuitableWeapon(enemy, 0);
		enemy->meleestruct.lasttarget = BODY_HEAD;
		break;
	case MELEEAI_WEAPON_THROW_CRITICAL:
		enemy->meleestruct.lastaction = MELEE_ACTION_THROWWEAPON;
		enemy->meleestruct.lasttarget = MeleeGetCriticalBodyPart();
		break;
	case MELEEAI_WEAPON_THROW_DISABLE:
		enemy->meleestruct.lastaction = MELEE_ACTION_THROWWEAPON;
		enemy->meleestruct.lasttarget = MeleeGetDisableBodyPart();
		break;
	case MELEEAI_WEAPON_THROW_ANYWHERE:
		enemy->meleestruct.lastaction = MELEE_ACTION_THROWWEAPON;
		enemy->meleestruct.lasttarget = rand()%MAX_BODY_PARTS;
		break;
	case MELEEAI_SHOOTPLAYER_CRITICAL:
		enemy->meleestruct.lastaction = MELEE_ACTION_FIREWEAPON;
		enemy->meleestruct.lasttarget = MeleeGetCriticalBodyPart();
		break;
	case MELEEAI_SHOOTPLAYER_ANYWHERE:
		enemy->meleestruct.lastaction = MELEE_ACTION_FIREWEAPON;
		enemy->meleestruct.lasttarget = rand()%MAX_BODY_PARTS;
		break;
	case MELEEAI_SHOOTPLAYER_DISABLE:
		enemy->meleestruct.lastaction = MELEE_ACTION_FIREWEAPON;
		enemy->meleestruct.lasttarget = MeleeGetDisableBodyPart();
		break;
	case MELEEAI_STRANGLE_PLAYER:
		enemy->meleestruct.lastaction = MELEE_ACTION_STRANGLE;
		enemy->meleestruct.lasttarget = BODY_HEAD;
		break;
	case MELEEAI_CLOSEDISTANCE:
		enemy->meleestruct.lastaction = MELEE_ACTION_CLOSEDISTANCE;
		enemy->meleestruct.lasttarget = BODY_HEAD;
		break;
	case MELEEAI_GAINDISTANCE:
		enemy->meleestruct.lastaction = MELEE_ACTION_GAINDISTANCE;
		enemy->meleestruct.lasttarget = BODY_HEAD;
		break;
	case MELEEAI_WEAPON_ATTACK_ANYWHERE:
		if(MeleeCurrentWeaponHasAction(enemy->currentweapon, WEAP_ATT_TYPE_CUT)){
				enemy->meleestruct.lastaction = MELEE_ACTION_CUT;
		}
		else if (MeleeCurrentWeaponHasAction(enemy->currentweapon, WEAP_ATT_TYPE_HIT)){
		enemy->meleestruct.lastaction = MELEE_ACTION_HITWITHWEAPON;
		}
		else{
			enemy->meleestruct.lastaction = MELEE_ACTION_KICK;
		}
		enemy->meleestruct.lasttarget = rand()%MAX_BODY_PARTS;
		break;
	case MELEEAI_WEAPON_ATTACK_CRITICAL:
		if(MeleeCurrentWeaponHasAction(enemy->currentweapon, WEAP_ATT_TYPE_CUT)){
				enemy->meleestruct.lastaction = MELEE_ACTION_CUT;
		}
		else if (MeleeCurrentWeaponHasAction(enemy->currentweapon, WEAP_ATT_TYPE_HIT)){
		enemy->meleestruct.lastaction = MELEE_ACTION_HITWITHWEAPON;
		}
		else{
			enemy->meleestruct.lastaction = MELEE_ACTION_KICK;
		}
		enemy->meleestruct.lasttarget = MeleeGetCriticalBodyPart();
		break;
	case MELEEAI_WEAPON_ATTACK_DISABLE:
		if(MeleeCurrentWeaponHasAction(enemy->currentweapon, WEAP_ATT_TYPE_CUT)){
				enemy->meleestruct.lastaction = MELEE_ACTION_CUT;
		}
		else if (MeleeCurrentWeaponHasAction(enemy->currentweapon, WEAP_ATT_TYPE_HIT)){
		enemy->meleestruct.lastaction = MELEE_ACTION_HITWITHWEAPON;
		}
		else{
			enemy->meleestruct.lastaction = MELEE_ACTION_KICK;
		}
		enemy->meleestruct.lasttarget = MeleeGetDisableBodyPart();
		break;
	case MELEEAI_KICKPLAYER_CRITICAL:
		enemy->meleestruct.lastaction = MELEE_ACTION_KICK;
		enemy->meleestruct.lasttarget = MeleeGetCriticalBodyPart();
		break;
	case MELEEAI_KICKPLAYER_DISABLE:
		enemy->meleestruct.lastaction = MELEE_ACTION_KICK;
		enemy->meleestruct.lasttarget = MeleeGetDisableBodyPart();
		break;
	case MELEEAI_KICKPLAYER_ANYWHERE:
		enemy->meleestruct.lastaction = MELEE_ACTION_KICK;
		enemy->meleestruct.lasttarget = rand()%MAX_BODY_PARTS;
		break;
	case MELEEAI_PUNCHPLAYER_CRITICAL:
		enemy->meleestruct.lastaction = MELEE_ACTION_PUNCH;
		enemy->meleestruct.lasttarget = MeleeGetCriticalBodyPart();
		break;
	case MELEEAI_PUNCHPLAYER_DISABLE:
		enemy->meleestruct.lastaction = MELEE_ACTION_PUNCH;
		enemy->meleestruct.lasttarget = MeleeGetDisableBodyPart();
		break;
	case MELEEAI_PUNCHPLAYER_ANYWHERE:
	default:
		enemy->meleestruct.lastaction = MELEE_ACTION_PUNCH;
		enemy->meleestruct.lasttarget = rand()%MAX_BODY_PARTS;
		break;
	}
}

int MeleeGetDisableBodyPart()
{
	//LSHOULDER, RSHOULDER, LKNEE, RKNEE
	int part = rand()%4;
	switch(part)
	{
	case 0:
		return BODY_RSHOULDER;
	case 1:
		return BODY_LSHOULDER;
	case 2:
		return BODY_LLEG;
	case 3:
		return BODY_RLEG;
	}
	return BODY_RSHOULDER;
}

int MeleeGetCriticalBodyPart()
{
	//HEAD, TORSO, LSIDE, RSIDE
		int part = rand()%4;
	switch(part)
	{
	case 0:
		return BODY_HEAD;
	case 1:
		return BODY_TORSO;
	case 2:
		return BODY_LSIDE;
	case 3:
		return BODY_RSIDE;
	}
	return BODY_TORSO;
}

bool HasMeleeAmmunition(PWEAPON weapon)
{
	if (weapon == NULL){
		return false;
	}
	else if (!(weapon->attacktype & WEAP_ATT_TYPE_SHOOT)){
		return false;
	}
	switch(weapon->loadtype)
	{
	case WEAP_LOADTYPE_CLIP:
		if(!weapon->loadprocedure.clip.isclipinserted){
			return false;
		}
		else if (weapon->loadprocedure.clip.clipinserted.currentrounds < 1){
			return false;
		}
		else if (RetrieveGlobalSubAmmoType(&weapon->loadprocedure.clip.clipinserted.clipround) == AMMOTYPE_EXPLOSIVE){
			return false;
		}
		return true;
		break;
	case WEAP_LOADTYPE_MAGAZINE:
		if(weapon->loadprocedure.magazine.totalroundschambered < 1){
			return false;
		}
		else if (RetrieveGlobalSubAmmoType(&weapon->loadprocedure.magazine.chamberedrounds[0]) == AMMOTYPE_EXPLOSIVE){
			return false;
		}
		return true;
		break;
	case WEAP_LOADTYPE_CYLINDER:
		if(weapon->loadprocedure.cylinder.statusindicators[weapon->loadprocedure.cylinder.currentposition] != CYLINDER_CHAMBER_FULL){
			return false;
		}
		else if(RetrieveGlobalSubAmmoType(&weapon->loadprocedure.cylinder.chamberedrounds[weapon->loadprocedure.cylinder.currentposition]) == AMMOTYPE_EXPLOSIVE){
			return false;
		}
		return true;
		break;
	default:
		return false;
	}
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleeGetEnemyMoveDefensive
//
// Description   - Retrieves an enemy defensive move.THIS IS WHERE AI DECIDES WHAT TO DO NEXT!!!!
//
// Return Type   - void 
//
// Arguments     - PENEMY enemy ( The pointer to the enemy )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void MeleeGetEnemyMoveDefensive(PENEMY enemy)
{
	//NEED MORE MOVES AND A DECENT WAY TO PICK THEM
	
	enemy->meleestruct.lasttarget = MELEE_TARGET_HEAD;
	MeleePrintActionString("The enemy quickly shields his head!");
}

void MeleeResolveEnemyOffensiveAction(PENEMY enemy, PPLAYER player)
{
	char buf[250];

	
	
	if (enemy->meleestruct.lastaction == MELEE_ACTION_FIREWEAPON)
	{
		MeleeCalculateFirearmAttack(enemy->meleestruct.lasttarget, player, enemy, false);
	}
	else if (enemy->meleestruct.lastaction == MELEE_ACTION_DRAWWEAPON)
	{
		sprintf(buf, "He draws a %s!", globals.globalmeleestruct.enemyweapontodraw->shortname);
		playMIXsound(PICKUP_WEAPON_SOUND);
		enemy->currentweapon = globals.globalmeleestruct.enemyweapontodraw;
		MeleePrintActionStringHold(buf);
	}
	else if (enemy->meleestruct.lastaction == MELEE_ACTION_GAINDISTANCE)
	{
		MeleePrintActionStringHold("He Backs Away From You.");
			globals.globalmeleestruct.distance += enemy->psychskill.psych.reflex;
			if (globals.globalmeleestruct.distance > MELEE_MAX_DISTANCE){
				globals.globalmeleestruct.distance = MELEE_MAX_DISTANCE;
			}
			MeleePrintDistance();
	}
	else if (enemy->meleestruct.lastaction == MELEE_ACTION_CLOSEDISTANCE){
			MeleePrintActionStringHold("He Closes In On You!");
			globals.globalmeleestruct.distance -= enemy->psychskill.psych.reflex;
			if (globals.globalmeleestruct.distance < MELEE_MIN_DISTANCE){
				globals.globalmeleestruct.distance = MELEE_MIN_DISTANCE;
			}
			MeleePrintDistance();
		}
	else{
		MeleeCalculateEnemyAttack(enemy->meleestruct.lastaction, enemy->meleestruct.lasttarget, enemy->meleestruct.lasttarget == player->meleestruct.lasttarget, player, enemy);
	}
}

void MeleeCalculateEnemyAttack(int attacktype, int location, bool correctlyblocked, PPLAYER player, PENEMY enemy)
{
	switch(attacktype)
	{
	case MELEE_ACTION_KICK:
		MeleeCalculateKick(location, correctlyblocked, player, enemy, false);
		return;
	case MELEE_ACTION_PUNCH:
	default:
		MeleeCalculatePunch(location, false, correctlyblocked, player, enemy, false);
		return;
	case MELEE_ACTION_STRANGLE:
		MeleeCalculateStrangle(player, enemy, false);
		return;
	case MELEE_ACTION_HITWITHWEAPON:
		MeleeCalculateHitWithWeapon(location, correctlyblocked, player, enemy, false);
		return;
	case MELEE_ACTION_CUT:
		MeleeCalculateCutWithWeapon(location, correctlyblocked, player, enemy, false);
		return;
	case MELEE_ACTION_THROWWEAPON:
		MeleeCalculateThrowWeapon(location, correctlyblocked, player, enemy, false);
		return;
	}
	return;
}

bool MeleeHasAnyAcceptableWeapons(PLIST weaponlist, int attacktype)
{
	PLIST walker;
	walker = weaponlist;
	walker->current = walker->head;
	
	if(weaponlist == NULL){
		return false;
	}
	
	while(walker->current != NULL){
		if(walker->current->object.weapon.attacktype & attacktype){
			return true;
		}
		walker->current = walker->current->nextnode;
	}
	return false;
}

bool MeleeCurrentWeaponHasAction(PWEAPON weapon, int attacktype)
{
	if(weapon == NULL){
		return false;
	}
	if(weapon->attacktype & attacktype){
		return true;
	}
	return false;
	
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleeResolvePlayerOffensiveAction
//
// Description   - This is the mathematical combat resolution algorithm for player attacks.
//
// Return Type   - void 
//
// Arguments     - PPLAYER player ( The pointer to the player )
//               - PENEMY enemy ( The pointer to the enemy )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void MeleePlayerDropWeapon(PPLAYER player)
{
	
	if(player->currentweapon){
		playWeaponSound(player->currentweapon, WEAP_SOUND_DROP, SOUND_MODE_MIX);
		player->currentweapon = NULL;
		
		globals.globalmeleestruct.currentcycleaction = MELEE_ACTION_PUNCH;
		MeleePrintCycleMove(player, MELEE_ACTION_PUNCH);
		
	}
}

void MeleeEnemyDropWeapon(PENEMY enemy)
{
	if(enemy->currentweapon){
		playWeaponSound(enemy->currentweapon, WEAP_SOUND_DROP, SOUND_MODE_MIX);
		enemy->currentweapon = NULL;
		enemy->meleestruct.lastaction = MELEE_ACTION_ATTACK;
	}
}

void MeleeResolvePlayerOffensiveAction(PPLAYER player, PENEMY enemy)
{
	char bodyparts[MAX_BODY_PARTS][60] = {
			"Right Leg",
			"Abdomen",
			"Left Leg",
			"Right Side",
			"Chest",
			"Left Side",
			"Right Shoulder",
			"Head",
			"Left Shoulder"};

				char stabbodyparts[MAX_BODY_PARTS][60] = {
			"Right Leg",
			"Abdomen",
			"Left Leg",
			"Right Side",
			"Chest",
			"Left Side",
			"Right Shoulder",
			"Neck",
			"Left Shoulder"};
		
		char resultstring[400];
		
		if (player->meleestruct.lastaction == MELEE_ACTION_ATTACK){
			MeleeModifyPlayerOffensiveAction(player);
		}
		
		if (player->meleestruct.lastaction == MELEE_ACTION_DRAWWEAPON)
		{
			
			if(!MeleePlayerDrawWeapon(player)){
				
				//	MeleeCombatRedraw(player, enemy);
			}
			else{
				MeleeAIDropHonor();
				//	MeleeCombatRedraw(player, enemy);
			}
			return;
		}
		
		if (player->meleestruct.lastaction == MELEE_ACTION_PUNCH){
			MeleeAIRaiseHonor();
			sprintf(resultstring, "You Throw A Punch At His %s!", bodyparts[player->meleestruct.lasttarget]);
			MeleePrintActionStringHold(resultstring);
			//MATHEMATICAL CALCULATION TO CHECK FOR PUNCH HIT
			MeleeCalculatePlayerAttack(player->meleestruct.lastaction,player->meleestruct.lasttarget, player, enemy);
			return;
		}
		else if (player->meleestruct.lastaction == MELEE_ACTION_KICK){
			MeleeAIRaiseHonor();
			sprintf(resultstring,"You Swing Your Foot At His %s!", bodyparts[player->meleestruct.lasttarget]);
			MeleePrintActionStringHold(resultstring);
			//MATHEMATICAL CALCULATION TO CHECK FOR KICK HIT, WHICH IS DIFFERENT THAN PUNCH HIT
			MeleeCalculatePlayerAttack(player->meleestruct.lastaction,player->meleestruct.lasttarget, player, enemy);
			return;
		}
		else if (player->meleestruct.lastaction == MELEE_ACTION_FIREWEAPON){
			MeleeAIRaiseAnger();
			MeleeAIDropHonor();
			if (player->currentweapon != NULL){
				sprintf(resultstring,"You Aim At His %s And Pull The Trigger!", bodyparts[player->meleestruct.lasttarget]);
				MeleePrintActionStringHold(resultstring);
			}
			MeleeCalculateFirearmAttack(player->meleestruct.lasttarget, player, enemy, true);
			//RUN THE APPROPRIATE DISCHARGE FUNCTION, CHECK FOR NO AMMO.
			
			//MATHEMATICAL CALCULATION TO CHECK FOR SHOT HIT
			return;
		}
		else if (player->meleestruct.lastaction == MELEE_ACTION_CUT){
			MeleeAIRaiseAnger();
			if (player->currentweapon != NULL){
				sprintf(resultstring, "You Swing The %s At His %s!", player->currentweapon->shortname, stabbodyparts[player->meleestruct.lasttarget]);
				MeleePrintActionStringHold(resultstring);
			}
			//MATHEMATICAL CALCULATION TO CHECK FOR KNIFE HIT
			MeleeCalculatePlayerAttack(player->meleestruct.lastaction,player->meleestruct.lasttarget, player, enemy);
			return;
		}
		else if (player->meleestruct.lastaction == MELEE_ACTION_HITWITHWEAPON){
			MeleeAIRaiseAnger();
			if (player->currentweapon != NULL){
				sprintf(resultstring, "You Strike At His %s With Your %s!", bodyparts[player->meleestruct.lasttarget], player->currentweapon->shortname);
				MeleePrintActionStringHold(resultstring);
			}
			//MATHEMATICAL CALCULATION TO CHECK FOR OBJECT HIT
			MeleeCalculatePlayerAttack(player->meleestruct.lastaction,player->meleestruct.lasttarget, player, enemy);
			return;
		}
		else if (player->meleestruct.lastaction == MELEE_ACTION_THROWWEAPON){
			MeleeAIRaiseAnger();
			MeleeAIDropHonor();
			if (player->currentweapon != NULL){
				sprintf(resultstring, "You Throw The %s At His %s!", player->currentweapon->shortname, stabbodyparts[player->meleestruct.lasttarget]);
				playMIXsound(MELEE_SOUND_KNIFETHROW);
				MeleePrintActionStringHold(resultstring);
			}
			//MATHEMATICAL CALCULATION TO CHECK FOR THROWN OBJECT HIT (AND REMOVE FROM INVENTORY)
			MeleeCalculateThrowWeapon(player->meleestruct.lasttarget, false, player, enemy, true);
			return;
		}
		else if (player->meleestruct.lastaction == MELEE_ACTION_GAINDISTANCE){
			MeleeAIDropAnger();
			MeleePrintActionStringHold("You Back Away From Your Opponent.");
			globals.globalmeleestruct.distance += player->psychskill.psych.reflex;
			if (globals.globalmeleestruct.distance > MELEE_MAX_DISTANCE){
				globals.globalmeleestruct.distance = MELEE_MAX_DISTANCE;
				MeleePrintActionStringHold("You Are As Far As You Can Safely Get.");
			}
			MeleePrintDistance();
			return;
		}
		else if (player->meleestruct.lastaction == MELEE_ACTION_CLOSEDISTANCE){
			MeleeAIRaiseHonor();
			MeleePrintActionStringHold("You Close In On Your Opponent.");
			globals.globalmeleestruct.distance -= player->psychskill.psych.reflex;
			if (globals.globalmeleestruct.distance < MELEE_MIN_DISTANCE){
				globals.globalmeleestruct.distance = MELEE_MIN_DISTANCE;
				MeleePrintActionStringHold("You Are As Close As You Can Get.");
			}
			MeleePrintDistance();
			return;
		}
		
		else if (player->meleestruct.lastaction == MELEE_ACTION_STRANGLE){
			MeleeAIRaiseAnger();
			MeleeCalculatePlayerAttack(player->meleestruct.lastaction,player->meleestruct.lasttarget, player, enemy);
		}
		else if (player->meleestruct.lastaction == MELEE_ACTION_DRUGDEATH || player->meleestruct.lastaction == MELEE_ACTION_DRUGUNCONSCIOUS){
			MeleeAIRaiseAnger();
			MeleeAIDropHonor();
			MeleeCalculateDrug(player->meleestruct.lasttarget, false, player, enemy, true);
		}

		else if (player->meleestruct.lastaction == MELEE_ACTION_CYANIDE){
			StopSong();
			MeleePrintActionString("You Bite Down, Cracking The Cyanide Capsule.");
			playSYNCsound(MELEE_SOUND_STRANGLEDEATH);
			player->meleestruct.stamina = 0;
		}
}

int MeleeCalculateThrowWeapon(int location, bool correctlyblocked, PPLAYER player, PENEMY enemy, bool playerattack)
{
	char bodyparts[MAX_BODY_PARTS][60] = {
		
		"Right Leg",
			"Abdomen",
			"Left Leg",
			"Right Side",
			"Chest",
			"Left Side",
			"Right Shoulder",
			"Neck",
			"Left Shoulder"};
		
		
		bool gizmo = false;
		BOOL gizmobonus = FALSE;
		BOOL specialhit = FALSE;
		BOOL hitmiss = FALSE; 
		BOOL turnaround = FALSE;
		BOOL knockdown = FALSE;
		BOOL attackerdropweapon = FALSE;
		BOOL defenderdropweapon = FALSE;
		BOOL openwoundhit = FALSE;
		char actionstring[500];
		PSYCHSKILLSTRUCT* psychskill;
		PSYCHSKILLSTRUCT* targetpsychskill;
		PWEAPON weapon;
		bool cuttingweapon = false;
		int finalchance;
		double modifierchance; //SO WE CAN WORK ON IT WITHOUT ROUNDING
		int damage;
		
		if(playerattack)
		{
			psychskill = &player->psychskill;
			targetpsychskill = &enemy->psychskill;
			if(gizmo){
				gizmobonus = true;
			}
			weapon = player->currentweapon;
		}
		
		else{
			targetpsychskill = &player->psychskill;
			psychskill = &enemy->psychskill;
			weapon = enemy->currentweapon;
			gizmobonus = false;
		}
		
		if(weapon->attacktype & WEAP_ATT_TYPE_CUT){
			cuttingweapon = true;
		}
		
		
		//CALCULATE CHANCE.
			modifierchance = MELEE_THROW_HITMISS_BASE_SUCCESS;
			modifierchance += (double)globals.globalmeleestruct.distance * MELEE_THROW_HITMISS_DISTANCE_MODIFIER / (double)MELEE_MAX_DISTANCE;
			modifierchance += (double)psychskill->skills.stealthcombat * MELEE_THROW_HITMISS_STEALTHCOMBAT_MODIFIER / (double)MAX_SKILL;
			modifierchance += (double)psychskill->skills.meleecombatWITHweapon * MELEE_THROW_HITMISS_ARMEDCOMBAT_MODIFIER / (double)MAX_SKILL;
			modifierchance += psychskill->specializations.handweapons * MELEE_THROW_HITMISS_SPECIALIZATION_MODIFIER;
				if(correctlyblocked && !playerattack){
			modifierchance -= MELEE_THROW_BLOCK_MODIFIER;
		}
			finalchance = (int)modifierchance;
			hitmiss = RandomPercentage(finalchance);

		if(hitmiss)
		{
			//CHECK FOR OPENWOUNDS
			if(cuttingweapon){
				openwoundhit = false;
			}
			else{
				if(playerattack){
					if(enemy->meleestruct.wounds[player->meleestruct.lasttarget] & WOUND_CUT){
						openwoundhit = true;
					}
				}
				else{
					if(player->meleestruct.wounds[enemy->meleestruct.lasttarget] & WOUND_CUT){
						openwoundhit = true;
					}
				}
			}
		//CALCULATE SPECIALHIT.
			modifierchance = MELEE_THROW_SPECIALHIT_BASE_SUCCESS;
				modifierchance += (double)globals.globalmeleestruct.distance * MELEE_THROW_SPECIALHIT_DISTANCE_MODIFIER / (double)MELEE_MAX_DISTANCE;
				modifierchance += (double)psychskill->skills.stealthcombat * MELEE_THROW_SPECIALHIT_STEALTHCOMBAT_MODIFIER / (double)MAX_SKILL;
				finalchance = (int)modifierchance;
				specialhit = RandomPercentage(finalchance);
			if(specialhit)
			{
				if(cuttingweapon){
					damage = MELEE_THROW_CUTTINGWEAPON_SPECIALHITDAMAGE;
					//A SPECIAL HIT TO THE HEAD ALWAYS KILLS
					if(playerattack){
						if(player->meleestruct.lasttarget == BODY_HEAD && cuttingweapon){
							StopSong();
							enemy->meleestruct.stamina = 0;
						}
					}
					else{
						if(enemy->meleestruct.lasttarget == BODY_HEAD && cuttingweapon){
							StopSong();
							player->meleestruct.stamina = 0;
						}
					}
				
				}
				else{
					damage = MELEE_THROW_BLUNTWEAPON_SPECIALHITDAMAGE;
						if(openwoundhit){	
						damage += MELEE_THROW_OPENWOUND_BONUS;
					}
				}

				//CALCULATE KNOCKDOWN
				if(cuttingweapon){
					knockdown = RandomPercentage(MELEE_THROW_KNOCKDOWN_CUTTINGWEAPON_CHANCE);
				}
				else{
					knockdown = RandomPercentage(MELEE_THROW_KNOCKDOWN_BLUNTWEAPON_CHANCE);
				}
				if (knockdown){
					if(playerattack){
						enemy->meleestruct.posture = MELEE_POSTURE_PRONE;
					}
					else{
						player->meleestruct.posture = MELEE_POSTURE_PRONE;
					}
					
				}

			
				
			}//END SPECIAL HIT
			
			//CALCULATE STANDARD ATTACK.
			if(cuttingweapon)
			{
				damage = random(MELEE_THROW_CUTTINGWEAPON_MINDAMAGE, MELEE_THROW_CUTTINGWEAPON_MAXDAMAGE);
				damage += (globals.globalmeleestruct.distance * 30 / MELEE_MAX_DISTANCE);
			}
			else{
				damage = random(MELEE_THROW_BLUNTWEAPON_MINDAMAGE, MELEE_THROW_BLUNTWEAPON_MAXDAMAGE);
				damage += (globals.globalmeleestruct.distance * 20 / MELEE_MAX_DISTANCE);
				if(openwoundhit){	
					damage += MELEE_THROW_OPENWOUND_BONUS;
				}
			}
		}
		
		
		if(playerattack)
		{
			
			
			if(!hitmiss)
			{
			playWeaponSound(weapon, WEAP_SOUND_DROP, SOUND_MODE_MIX);
				sprintf(actionstring, "The %s Shoots By Your Opponent!",weapon->shortname);
				MeleePrintActionStringHold(actionstring);

			}
			else{
				if(specialhit)
				{
					MeleeAIRaiseAnger();
					if (cuttingweapon)
					{
						
						playMIXsound(MELEE_SOUND_SPECIALSTAB);
						MeleePrintActionStringHold("Critical Hit!");
						if(player->meleestruct.lasttarget == BODY_LLEG || player->meleestruct.lasttarget == BODY_RLEG){
							sprintf(actionstring, "The %s Drills Into His Kneecap, Sending Him To The Ground!", weapon->shortname);
							enemy->meleestruct.posture = MELEE_POSTURE_PRONE;
							MeleePrintActionStringHold(actionstring);
						}
						else if((player->meleestruct.lasttarget == BODY_LSHOULDER|| player->meleestruct.lasttarget == BODY_RSHOULDER) && enemy->currentweapon){
							sprintf(actionstring, "The %s Impales His Shoulder, Forcing Him To Drop His Weapon!", weapon->shortname);
							MeleeEnemyDropWeapon(enemy);
							MeleePrintActionStringHold(actionstring);
						}
						else{
						sprintf(actionstring, "The %s Buries Itself Deep In His %s!", weapon->shortname, bodyparts[player->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
						}
					}
					else{
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("Critical Hit!");
					sprintf(actionstring, "The %s Crashes Into His %s!", weapon->shortname, bodyparts[player->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
					enemy->meleestruct.wounds[player->meleestruct.lasttarget] |= WOUND_BONEBROKEN;
					}
				}
				else{
						if (cuttingweapon)
					{
						playMIXsound(MELEE_SOUND_STAB);
						sprintf(actionstring, "The %s Sticks Into His %s!", weapon->shortname, bodyparts[player->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
					}
					else{
					playMIXsound(MELEE_SOUND_HITWITHWEAPON);
					sprintf(actionstring, "Your %s Connects With His %s!", weapon->shortname, bodyparts[player->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
					}
				}
				if(openwoundhit){
					playMIXsound(MELEE_SOUND_SCREAM);
					MeleePrintActionStringHold("Open Wound Hit!");
					damage += MELEE_HITWITHWEAPON_OPENWOUND_BONUS;
				}
				if(knockdown)
				{	
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("KNOCKDOWN!!!");
					MeleePrintActionStringHold("The Force of The Impact Knocks Him To The Floor.");
				}
			}	
		}
		//FINAL PRINTING AREA FOR ENEMY ATTACK.
		
		else
		{
			if(!hitmiss)
			{
				if(correctlyblocked){
					playWeaponSound(weapon, WEAP_SOUND_DROP, SOUND_MODE_MIX);
					sprintf(actionstring, "You Dodge Out Of The Way Of The %s!", weapon->shortname);
					MeleePrintActionStringHold(actionstring);
				}
				else{
					playWeaponSound(weapon, WEAP_SOUND_DROP, SOUND_MODE_MIX);
					sprintf(actionstring, "His %s Shoots Past You!", weapon->shortname);
					MeleePrintActionStringHold(actionstring);
				}
			}
			else{
				if(specialhit)
				{
					if(cuttingweapon){
						MeleeAIDropAnger();
					playMIXsound(MELEE_SOUND_SPECIALSTAB);
					MeleePrintActionStringHold("Critical Hit!");
					if(enemy->meleestruct.lasttarget == BODY_LLEG || enemy->meleestruct.lasttarget == BODY_RLEG){
							sprintf(actionstring, "The %s Drills Into Your Kneecap, Sending You To The Ground!", weapon->shortname);
						 player->meleestruct.posture = MELEE_POSTURE_PRONE;
							MeleePrintActionStringHold(actionstring);
						}
						else if((enemy->meleestruct.lasttarget == BODY_LSHOULDER|| enemy->meleestruct.lasttarget == BODY_RSHOULDER) && player->currentweapon){
							sprintf(actionstring, "The %s Impales Your Shoulder, Forcing You To Drop Your Weapon!", weapon->shortname);
							MeleePlayerDropWeapon(player);
							MeleePrintActionStringHold(actionstring);
						}
						else{
					sprintf(actionstring, "The %s Buries Itself Deep In Your %s!", weapon->shortname, bodyparts[enemy->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
						}
					}
					else{
					MeleeAIDropAnger();
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("Critical Hit!");
					sprintf(actionstring, "The %s Slams Into Your %s, Cracking Something!", weapon->shortname, bodyparts[enemy->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
					player->meleestruct.wounds[enemy->meleestruct.lasttarget] |= WOUND_BONEBROKEN;
					}
				}
				else{
					if (cuttingweapon){
						playMIXsound(MELEE_SOUND_STAB);
						sprintf(actionstring, "His Weapon Sticks Into Your %s!", bodyparts[enemy->meleestruct.lasttarget]);
						MeleePrintActionStringHold(actionstring);
					}
					else{
					playMIXsound(MELEE_SOUND_HITWITHWEAPON);
					sprintf(actionstring, "His Weapon Connects With Your %s!", bodyparts[enemy->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
					}
				}
				if(openwoundhit){
					playMIXsound(MELEE_SOUND_SCREAM);
					MeleePrintActionStringHold("Open Wound Hit!");
					damage += MELEE_HITWITHWEAPON_OPENWOUND_BONUS;
				}
				if(knockdown)
				{	
					MeleeAIRaiseHonor();
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("KNOCKDOWN!!!");
					MeleePrintActionStringHold("You Are Flung to the Floor By The Impact!");
				}
			}	
		}
		
		//THE WEAPONS CHANGE OWNERSHIP.
		if(playerattack)
		{
			EnemyTakePlayerWeapon(enemy, player, &weapon);
			player->currentweapon = NULL;
			globals.globalmeleestruct.currentcycleaction = MELEE_ACTION_PUNCH;
		MeleePrintCycleMove(player, MELEE_ACTION_PUNCH);
		}
		else{
			PlayerTakeEnemyWeapon(enemy, player, &weapon);
			enemy->currentweapon = NULL;
		}

		//DEALIN' THE DAMAGE.
		if(hitmiss){
			if(playerattack){
			enemy->meleestruct.stamina -= damage;
			if(cuttingweapon){
				enemy->meleestruct.wounds[player->meleestruct.lasttarget] |= WOUND_CUT;
			}
			}
			else{
				player->meleestruct.stamina -= damage;
				if(cuttingweapon){
			player->meleestruct.wounds[enemy->meleestruct.lasttarget] |= WOUND_CUT;
			}
			}
		}
		
		
		//DEATH RESOLUTION
		if (player->meleestruct.stamina <= 0){
			playMIXsound(MELEE_SOUND_FALLDOWN);
			StopSong();
			MeleePrintActionStringHold("You Collapse To The Ground.");
			return - 1;
		}
		else if (enemy->meleestruct.stamina <=0)
		{
			playMIXsound(MELEE_SOUND_FALLDOWN);
			StopSong();
			MeleePrintActionStringHold("He Falls To The Ground.");
			return -1;
		}
		
		//ELSE PLAYER OR ENEMY PULLS IT OUT OF HIMSELF IF A STABBING WEAPON. OTHERWISE GET IT OFF GROUND.
		if(playerattack){
			if(hitmiss){
				if(cuttingweapon){
					playMIXsound(MELEE_SOUND_REMOVEKNIFE);
					sprintf(actionstring, "He Pulls The Weapon From His %s.", bodyparts[player->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
				}
			}
			if(!hitmiss || !cuttingweapon){
				playMIXsound(PICKUP_WEAPON_SOUND);
				sprintf(actionstring, "He Picks The Weapon Up Off The Ground.");
				MeleePrintActionStringHold(actionstring);
			}
		}
		else{
			if(hitmiss){
				if(cuttingweapon){
					playMIXsound(MELEE_SOUND_REMOVEKNIFE);
					sprintf(actionstring, "You Painfully Remove The Weapon From Your %s.", bodyparts[enemy->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
				}
			}
			if(!hitmiss || !cuttingweapon){
				playMIXsound(PICKUP_WEAPON_SOUND);
				sprintf(actionstring, "You Pick The Weapon Up Off The Ground.");
				MeleePrintActionStringHold(actionstring);
			}
		}
		return 0;
}

int MeleeCalculatePunch(int location, bool gizmo, bool correctlyblocked, PPLAYER player, PENEMY enemy, bool playerattack)
{
	char bodyparts[MAX_BODY_PARTS][60] = {
			"Right Leg",
			"Abdomen",
			"Left Leg",
			"Right Side",
			"Chest",
			"Left Side",
			"Right Shoulder",
			"Head",
			"Left Shoulder"};
		
		BOOL gizmobonus = FALSE;
		BOOL specialhit = FALSE;
		BOOL hitmiss = FALSE; 
		BOOL turnaround = FALSE;
		BOOL knockdown = FALSE;
		BOOL attackerdropweapon = FALSE;
		BOOL defenderdropweapon = FALSE;
		BOOL openwoundhit = FALSE;
		char actionstring[500];
		PSYCHSKILLSTRUCT* psychskill;
		PSYCHSKILLSTRUCT* targetpsychskill;
		int finalchance, finalmindamage, finalmaxdamage;
		double modifierchance, modifiermindamage, modifiermaxdamage; //SO WE CAN WORK ON IT WITHOUT ROUNDING
		int damage;
		
		if(playerattack)
		{
			psychskill = &player->psychskill;
			targetpsychskill = &enemy->psychskill;
			if(gizmo){
				gizmobonus = true;
			}
		}
		
		else{
			targetpsychskill = &player->psychskill;
			psychskill = &enemy->psychskill;
			gizmobonus = false;
		}
		
		/************************************************************
		; PLAYER PUNCH ALGORITHM
		************************************************************/
		
		
		
		modifierchance = MELEE_PUNCH_HITMISS_BASE_SUCCESS;
		modifierchance = (modifierchance - ((MELEE_PUNCH_HITMISS_DISTANCE_MODIFIER) * ((double) globals.globalmeleestruct.distance)));
		modifierchance = (modifierchance + (MELEE_PUNCH_HITMISS_SKILL_MODIFIER * ((double) psychskill->skills.meleecombatNOweapon)));
		if(correctlyblocked && !playerattack){
			modifierchance -= MELEE_PUNCH_BLOCK_MODIFIER;
		}
		finalchance = (int)modifierchance; //THIS IS THE HIT/MISS CHANCE;
		hitmiss = RandomPercentage(finalchance);
		
		
		if (!hitmiss){ //ATTACK MISSED!!!
			
			//SORRY, NO DAMAGE KID.
			//CHECK FOR A TURNAROUND!!!!!!!!!!!!!!!!!!!!!!!!!!
			modifierchance = MELEE_PUNCH_TURNAROUND_BASE_SUCCESS;
			modifierchance = (modifierchance - (MELEE_PUNCH_TURNAROUND_DISTANCE_MODIFIER * ((double)globals.globalmeleestruct.distance)));
			modifierchance = (modifierchance + (MELEE_PUNCH_TURNAROUND_REFLEX_MODIFIER * ((double)enemy->psychskill.psych.reflex)));
			if(correctlyblocked && !playerattack){
				modifierchance -= MELEE_PUNCH_BLOCK_MODIFIER;
			}
			finalchance = (int)modifierchance;
			turnaround = RandomPercentage(finalchance);
			if(turnaround)
			{
				if(playerattack)
				{
					player->meleestruct.stamina -= MELEE_TURNAROUND_DAMAGE;
					player->meleestruct.posture = MELEE_POSTURE_PRONE;
					if(player->currentweapon && RandomPercentage(80 - (psychskill->skills.meleecombatWITHweapon * 5))){
						attackerdropweapon = true;
					}
				}
				else{
					enemy->meleestruct.stamina -= MELEE_TURNAROUND_DAMAGE;
					enemy->meleestruct.posture = MELEE_POSTURE_PRONE;
					if(enemy->currentweapon && RandomPercentage(80 - (psychskill->skills.meleecombatWITHweapon * 5))){
						attackerdropweapon = true;
					}
				}
			}
			
			//IF NOTHING ELSE, RETURN. WE'RE DONE HERE.
		}
		
		else{ //WE'VE HIT. WHAT NOW? CHECK FOR SPECIAL HITS.
			//CHECKING FOR SPECIAL HITS.
			if (playerattack && enemy->meleestruct.wounds[player->meleestruct.lasttarget] & WOUND_CUT){
				openwoundhit = true;
			}
			else if (!playerattack && player->meleestruct.wounds[enemy->meleestruct.lasttarget] & WOUND_CUT){
				openwoundhit = true;
			}
			
			if (playerattack && gizmobonus){
				enemy->meleestruct.wounds[player->meleestruct.lasttarget] |= WOUND_CUT;
			}
			
			
			modifierchance = MELEE_PUNCH_SPECIALHIT_BASE_SUCCESS;
			modifierchance = (modifierchance + (MELEE_PUNCH_SPECIALHIT_DISTANCE_MODIFIER * (double)globals.globalmeleestruct.distance));
			modifierchance = (modifierchance + (MELEE_PUNCH_SPECIALHIT_SKILL_MODIFIER * (double)psychskill->skills.stealthcombat));
			finalchance = (int) modifierchance;
			specialhit = RandomPercentage(finalchance);
		
			if (specialhit){ //SPECIAL HIT!!!!!!!!
				
				
				damage = MELEE_PUNCH_SPECIALHIT_DAMAGE;
				
				
				
				/************************************************************
				; THIS FUNCTION BELOW DETERMINES SPECIAL HIT DAMAGE MODIFICATION, DROPPED WEAPONS, ETC
				************************************************************/
				
				MeleeSpecialHitDamageCorrection(player, enemy, damage, playerattack); //THIS TAKES CARE OF SPECIAL SITUATIONS
				
				modifierchance  = MELEE_PUNCH_KNOCKDOWN_BASE_SUCCESS;
				modifierchance  = (modifierchance + (MELEE_PUNCH_KNOCKDOWN_AGGRESSION_MODIFIER * (double)psychskill->psych.agression));
				finalchance = (int) modifierchance;
				knockdown = RandomPercentage(finalchance);
				
				if (knockdown){
					if(playerattack){
						enemy->meleestruct.posture = MELEE_POSTURE_PRONE;
					}
					else{
						player->meleestruct.posture = MELEE_POSTURE_PRONE;
					}
					
				}
				if(openwoundhit)
				{
					damage += MELEE_PUNCH_OPENWOUND_BONUS;
				}
			}//END SPECIAL HIT
			
			else{ //CALCULATE REGULAR DAMAGE, PLUS OPEN WOUND PENALTIES.
				
				modifiermindamage = MELEE_PUNCH_HITDAMAGE_BASE_MINIMUM;
				modifiermaxdamage = MELEE_PUNCH_HITDAMAGE_BASE_MAXIMUM;
				modifiermaxdamage = modifiermaxdamage + (MELEE_PUNCH_HITDAMAGE_SKILL_MODIFIER * ((double)psychskill->skills.meleecombatNOweapon));
				modifiermindamage = modifiermindamage + (MELEE_PUNCH_HITDAMAGE_AGGRESSION_MODIFIER * ((double)psychskill->psych.agression));
				modifiermindamage = modifiermindamage + (MELEE_PUNCH_HITDAMAGE_DISTANCE_MODIFIER * ((double)globals.globalmeleestruct.distance));
				
				finalmindamage = (int) modifiermindamage;
				finalmaxdamage = (int) modifiermaxdamage;
				damage = random(finalmindamage, finalmaxdamage);
				
				if(openwoundhit)
				{
					damage += MELEE_PUNCH_OPENWOUND_BONUS;
				}
			}
		}
		
		
		//RESOLVE DAMAGE
		if(playerattack && hitmiss){
			enemy->meleestruct.stamina -= damage;
		}
		else if (!playerattack && hitmiss){
			player->meleestruct.stamina -= damage;
		}
		
		
		//FINAL PRINTING AREA FOR PLAYER ATTACK.
		if(playerattack)
		{
			
			
			if(!hitmiss)
			{
				MeleePlayRandomMissSoundAsync();
				MeleePrintActionStringHold("Your Punch Goes Wide And Sails By Your Opponent.");
				if (turnaround){
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("TURNAROUND!!!");
					MeleePrintActionStringHold("Your Opponent Quickly Grabs Your Punch And Throws You To The Ground.");
					if (attackerdropweapon){
						MeleePrintActionStringHold("You dropped your weapon!");
						MeleePlayerDropWeapon(player);
					}
				}
			}
			else{
				if(specialhit)
				{
					MeleeAIRaiseAnger();
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("Critical Hit!");
					sprintf(actionstring, "You Hit His %s Hard. You Hear Something Give.", bodyparts[player->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
					enemy->meleestruct.wounds[player->meleestruct.lasttarget] |= WOUND_BONEBROKEN;
				}
				else{
					MeleePlayRandomHitSoundAsync();
					sprintf(actionstring, "Your Fist Connects With His %s!", bodyparts[player->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
				}
				if(openwoundhit){
					playMIXsound(MELEE_SOUND_SCREAM);
					MeleePrintActionStringHold("Open Wound Hit!");
					damage += MELEE_PUNCH_OPENWOUND_BONUS;
				}
				else if (gizmobonus){
					sprintf(actionstring, "The Blade Ring Cuts Deep Into His %s!", bodyparts[player->meleestruct.lasttarget]);
					playMIXsound(MELEE_SOUND_RING);
					MeleePrintActionStringHold(actionstring);
				}
				if(knockdown)
				{	
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("KNOCKDOWN!!!");
					MeleePrintActionStringHold("Your Hit Knocks Him To The Floor.");
				}
			}	
		}
		//FINAL PRINTING AREA FOR ENEMY ATTACK.
		
		else
		{
			if(!hitmiss)
			{
				if(correctlyblocked){
					playMIXsound(MELEE_SOUND_CATCHPUNCH);
					MeleePrintActionStringHold("You Catch His Fist As It Comes Toward You!");
				}
				else{
					MeleePlayRandomMissSoundAsync();
					MeleePrintActionStringHold("His Punch Swings By You.");
				}
				if (turnaround){
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("TURNAROUND!!!");
					MeleePrintActionStringHold("You Grab His Arm and Throw Him To The Ground!");
					if (attackerdropweapon){
						MeleePrintActionStringHold("He drops his weapon!");
						MeleeEnemyDropWeapon(enemy);
					}
				}
			}
			else{
				if(specialhit)
				{
					MeleeAIDropAnger();
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("Critical Hit!");
					sprintf(actionstring, "He Hits Your %s With A Tremendous Punch.", bodyparts[enemy->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
					player->meleestruct.wounds[enemy->meleestruct.lasttarget] |= WOUND_BONEBROKEN;
				}
				else{
					MeleePlayRandomHitSoundAsync();
					sprintf(actionstring, "His Fist Connects With Your %s!", bodyparts[enemy->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
				}
				if(openwoundhit){
					playMIXsound(MELEE_SOUND_SCREAM);
					MeleePrintActionStringHold("Open Wound Hit!");
					damage += MELEE_PUNCH_OPENWOUND_BONUS;
				}
				if(knockdown)
				{	
					MeleeAIRaiseHonor();
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("KNOCKDOWN!!!");
					MeleePrintActionStringHold("You Are Flung to the Floor By His Punch!");
				}
			}	
		}
		
		//DEATH RESOLUTION
		if (player->meleestruct.stamina <= 0){
			playMIXsound(MELEE_SOUND_FALLDOWN);
			StopSong();
			MeleePrintActionStringHold("You Collapse To The Ground.");
			return - 1;
		}
		else if (enemy->meleestruct.stamina <=0)
		{
			playMIXsound(MELEE_SOUND_FALLDOWN);
			StopSong();
			MeleePrintActionStringHold("He Falls To The Ground.");
			return -1;
		}
		else return 0;
}



int MeleeCalculateKick(int location,  bool correctlyblocked, PPLAYER player, PENEMY enemy, bool playerattack)
{
	char bodyparts[MAX_BODY_PARTS][60] = {
	
			"Right Leg",
			"Abdomen",
			"Left Leg",
			"Right Side",
			"Chest",
			"Left Side",
			"Right Shoulder",
			"Head",
			"Left Shoulder"};
		
		bool gizmo = false;
		BOOL gizmobonus = FALSE;
		BOOL specialhit = FALSE;
		BOOL hitmiss = FALSE; 
		BOOL turnaround = FALSE;
		BOOL knockdown = FALSE;
		BOOL attackerdropweapon = FALSE;
		BOOL defenderdropweapon = FALSE;
		BOOL openwoundhit = FALSE;
		char actionstring[500];
		PSYCHSKILLSTRUCT* psychskill;
		PSYCHSKILLSTRUCT* targetpsychskill;
		int finalchance, finalmindamage, finalmaxdamage;
		double modifierchance, modifiermindamage, modifiermaxdamage; //SO WE CAN WORK ON IT WITHOUT ROUNDING
		int damage;
		
		if(playerattack)
		{
			psychskill = &player->psychskill;
			targetpsychskill = &enemy->psychskill;
			if(gizmo){
				gizmobonus = true;
			}
		}
		
		else{
			targetpsychskill = &player->psychskill;
			psychskill = &enemy->psychskill;
			gizmobonus = false;
		}
		
		/************************************************************
		; PLAYER KICK ALGORITHM
		************************************************************/
		
		
		
		modifierchance = MELEE_KICK_HITMISS_BASE_SUCCESS;
		modifierchance = (modifierchance - ((MELEE_KICK_HITMISS_DISTANCE_MODIFIER) * ((double) globals.globalmeleestruct.distance)));
		modifierchance = (modifierchance + (MELEE_KICK_HITMISS_SKILL_MODIFIER * ((double) psychskill->skills.meleecombatNOweapon)));
		if(correctlyblocked && !playerattack){
			modifierchance -= MELEE_PUNCH_BLOCK_MODIFIER;
		}
		finalchance = (int)modifierchance; //THIS IS THE HIT/MISS CHANCE;
		hitmiss = RandomPercentage(finalchance);
		
		
		if (!hitmiss){ //ATTACK MISSED!!!
			
			//SORRY, NO DAMAGE KID.
			//CHECK FOR A TURNAROUND!!!!!!!!!!!!!!!!!!!!!!!!!!
			modifierchance = MELEE_KICK_TURNAROUND_BASE_SUCCESS;
			modifierchance = (modifierchance - (MELEE_KICK_TURNAROUND_DISTANCE_MODIFIER * ((double)globals.globalmeleestruct.distance)));
			modifierchance = (modifierchance + (MELEE_KICK_TURNAROUND_REFLEX_MODIFIER * ((double)enemy->psychskill.psych.reflex)));
			if(correctlyblocked && !playerattack){
				modifierchance -= MELEE_PUNCH_BLOCK_MODIFIER;
			}
			finalchance = (int)modifierchance;
			turnaround = RandomPercentage(finalchance);
			if(turnaround)
			{
				if(playerattack)
				{
					player->meleestruct.stamina -= MELEE_TURNAROUND_DAMAGE;
					player->meleestruct.posture = MELEE_POSTURE_PRONE;
					if(player->currentweapon && RandomPercentage(80 - (psychskill->skills.meleecombatWITHweapon * 5))){
						attackerdropweapon = true;
					}
				}
				else{
					enemy->meleestruct.stamina -= MELEE_TURNAROUND_DAMAGE;
					enemy->meleestruct.posture = MELEE_POSTURE_PRONE;
						if(enemy->currentweapon && RandomPercentage(80 - (psychskill->skills.meleecombatWITHweapon * 5))){
						attackerdropweapon = true;
					}
				}
			}
			
			//IF NOTHING ELSE, RETURN. WE'RE DONE HERE.
		}
		
		else{ //WE'VE HIT. WHAT NOW? CHECK FOR SPECIAL HITS.
			//CHECKING FOR SPECIAL HITS.
			if (playerattack && enemy->meleestruct.wounds[player->meleestruct.lasttarget] & WOUND_CUT){
				openwoundhit = true;
			}
			else if (!playerattack && player->meleestruct.wounds[enemy->meleestruct.lasttarget] & WOUND_CUT){
				openwoundhit = true;
			}
			
			if (playerattack && gizmobonus){
				enemy->meleestruct.wounds[player->meleestruct.lasttarget] |= WOUND_CUT;
			}
			
			
			modifierchance = MELEE_KICK_SPECIALHIT_BASE_SUCCESS;
			modifierchance = (modifierchance + (MELEE_KICK_SPECIALHIT_DISTANCE_MODIFIER * (double)globals.globalmeleestruct.distance));
			modifierchance = (modifierchance + (MELEE_KICK_SPECIALHIT_SKILL_MODIFIER * (double)psychskill->skills.stealthcombat));
			finalchance = (int) modifierchance;
			specialhit = RandomPercentage(finalchance);
			if (specialhit){ //SPECIAL HIT!!!!!!!!
				
				
				damage = MELEE_KICK_SPECIALHIT_DAMAGE;
				
				
				
				/************************************************************
				; THIS FUNCTION BELOW DETERMINES SPECIAL HIT DAMAGE MODIFICATION, DROPPED WEAPONS, ETC
				************************************************************/
				
				MeleeSpecialHitDamageCorrection(player, enemy, damage, playerattack); //THIS TAKES CARE OF SPECIAL SITUATIONS
				
				modifierchance  = MELEE_KICK_KNOCKDOWN_BASE_SUCCESS;
				modifierchance  = (modifierchance + (MELEE_KICK_KNOCKDOWN_AGGRESSION_MODIFIER * (double)psychskill->psych.agression));
				finalchance = (int) modifierchance;
				knockdown = RandomPercentage(finalchance);
				
				if (knockdown){
					if(playerattack){
						enemy->meleestruct.posture = MELEE_POSTURE_PRONE;
					}
					else{
						player->meleestruct.posture = MELEE_POSTURE_PRONE;
					}
					
				}
				if(openwoundhit)
				{
					damage += MELEE_KICK_OPENWOUND_BONUS;
				}
			}//END SPECIAL HIT
			
			else{ //CALCULATE REGULAR DAMAGE, PLUS OPEN WOUND PENALTIES.
				
				modifiermindamage = MELEE_KICK_HITDAMAGE_BASE_MINIMUM;
				modifiermaxdamage = MELEE_KICK_HITDAMAGE_BASE_MAXIMUM;
				modifiermaxdamage = modifiermaxdamage + (MELEE_KICK_HITDAMAGE_SKILL_MODIFIER * ((double)psychskill->skills.meleecombatNOweapon));
				modifiermindamage = modifiermindamage + (MELEE_KICK_HITDAMAGE_AGGRESSION_MODIFIER * ((double)psychskill->psych.agression));
				modifiermindamage = modifiermindamage + (MELEE_KICK_HITDAMAGE_DISTANCE_MODIFIER * ((double)globals.globalmeleestruct.distance));
				
				finalmindamage = (int) modifiermindamage;
				finalmaxdamage = (int) modifiermaxdamage;
				damage = random(finalmindamage, finalmaxdamage);
				
				if(openwoundhit)
				{
					damage += MELEE_KICK_OPENWOUND_BONUS;
				}
			}
		}
		
		
		//RESOLVE DAMAGE
		if(playerattack && hitmiss){
			enemy->meleestruct.stamina -= damage;
		}
		else if (!playerattack && hitmiss){
			player->meleestruct.stamina -= damage;
		}
		
		
		//FINAL PRINTING AREA FOR PLAYER ATTACK.
		if(playerattack)
		{
			
			
			if(!hitmiss)
			{
				MeleePlayRandomMissSoundAsync();
				MeleePrintActionStringHold("Your Kick Goes Wide And Sails By Your Opponent.");
				if (turnaround){
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("TURNAROUND!!!");
					MeleePrintActionStringHold("Your Opponent Quickly Grabs Your Leg And Throws You To The Ground.");
					if (attackerdropweapon){
						MeleePrintActionStringHold("You dropped your weapon!");
						MeleePlayerDropWeapon(player);
					}
				}
			}
			else{
				if(specialhit)
				{
					MeleeAIRaiseAnger();
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("Critical Hit!");
					sprintf(actionstring, "You Hit His %s Hard. You Hear Something Give.", bodyparts[player->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
					enemy->meleestruct.wounds[player->meleestruct.lasttarget] |= WOUND_BONEBROKEN;
				}
				else{
					MeleePlayRandomHitSoundAsync();
					sprintf(actionstring, "Your Boot Connects With His %s!", bodyparts[player->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
				}
				if(openwoundhit){
					playMIXsound(MELEE_SOUND_SCREAM);
					MeleePrintActionStringHold("Open Wound Hit!");
					damage += MELEE_KICK_OPENWOUND_BONUS;
				}
				else if (gizmobonus){
					sprintf(actionstring, "The Blade Ring Cuts Deep Into His %s!", bodyparts[player->meleestruct.lasttarget]);
					playMIXsound(MELEE_SOUND_RING);
					MeleePrintActionStringHold(actionstring);
				}
				if(knockdown)
				{	
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("KNOCKDOWN!!!");
					MeleePrintActionStringHold("Your Hit Knocks Him To The Floor.");
				}
			}	
		}
		//FINAL PRINTING AREA FOR ENEMY ATTACK.
		
		else
		{
			if(!hitmiss)
			{
				if(correctlyblocked){
					playMIXsound(MELEE_SOUND_CATCHPUNCH);
					MeleePrintActionStringHold("You Catch His Boot As It Comes Toward You!");
				}
				else{
					MeleePlayRandomMissSoundAsync();
					MeleePrintActionStringHold("His Kick Swings By You.");
				}
				if (turnaround){
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("TURNAROUND!!!");
					MeleePrintActionStringHold("You Grab His Leg and Throw Him To The Ground!");
					if (attackerdropweapon){
						MeleePrintActionStringHold("He drops his weapon!");
						MeleeEnemyDropWeapon(enemy);
					}
				}
			}
			else{
				if(specialhit)
				{
					MeleeAIDropAnger();
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("Critical Hit!");
					sprintf(actionstring, "He Hits Your %s With A Tremendous Kick.", bodyparts[enemy->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
					player->meleestruct.wounds[enemy->meleestruct.lasttarget] |= WOUND_BONEBROKEN;
				}
				else{
					MeleePlayRandomHitSoundAsync();
					sprintf(actionstring, "His Boot Connects With Your %s!", bodyparts[enemy->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
				}
				if(openwoundhit){
					playMIXsound(MELEE_SOUND_SCREAM);
					MeleePrintActionStringHold("Open Wound Hit!");
					damage += MELEE_KICK_OPENWOUND_BONUS;
				}
				if(knockdown)
				{	
					MeleeAIRaiseHonor();
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("KNOCKDOWN!!!");
					MeleePrintActionStringHold("You Are Flung to the Floor By His Kick!");
				}
			}	
		}
		
		//DEATH RESOLUTION
		if (player->meleestruct.stamina <= 0){
			playMIXsound(MELEE_SOUND_FALLDOWN);
			StopSong();
			MeleePrintActionStringHold("You Collapse To The Ground.");
			return - 1;
		}
		else if (enemy->meleestruct.stamina <=0)
		{
			playMIXsound(MELEE_SOUND_FALLDOWN);
			StopSong();
			MeleePrintActionStringHold("He Falls To The Ground.");
			return -1;
		}
		else return 0;
}

void MeleeCalculatePlayerAttack(int attacktype, int location, PPLAYER player, PENEMY enemy)
{
	/*
	char bodyparts[MAX_BODY_PARTS][60] = {
			"Right Leg",
			"Abdomen",
			"Left Leg",
			"Right Side",
			"Chest",
			"Left Side",
			"Right Shoulder",
			"Head",
			"Left Shoulder"};
		*/
	//	char actionstring[400];
		bool gizmobonus = FALSE;
	//	BOOL specialhit = FALSE;
	//	BOOL hitmiss = FALSE; 
	//	BOOL turnaround = FALSE;
	//	BOOL knockdown = FALSE;
	//	PSYCHSKILLSTRUCT* psychskill;
	//	int finalchance, finalmindamage, finalmaxdamage;
	//	double modifierchance, modifiermindamage, modifiermaxdamage; //SO WE CAN WORK ON IT WITHOUT ROUNDING
	//	int damage;
		
		
//		psychskill = &player->psychskill;
		
		
		if(attacktype == MELEE_ACTION_PUNCH)
		{
			if(DoesListHaveGizmoType(GIZMO_TYPE_RING, player->p_gizmos, false)){
				gizmobonus = true;
			}
			MeleeCalculatePunch(location, gizmobonus, false, player, enemy, true);
		}
	
		if(attacktype == MELEE_ACTION_KICK)
		{
			MeleeCalculateKick(location, false, player, enemy, true);
		}

		if(attacktype == MELEE_ACTION_STRANGLE)
		{
			MeleeCalculateStrangle(player, enemy, true);
		}
		
		/************************************************************
		; PLAYER WEAPON STRIKE ALGORITHM
		************************************************************/
		if (attacktype == MELEE_ACTION_HITWITHWEAPON){
			MeleeCalculateHitWithWeapon(location, false, player, enemy, true);
		}

		if(attacktype == MELEE_ACTION_CUT){
			MeleeCalculateCutWithWeapon(location, false, player, enemy, true);
		}

		
		
}

int MeleeCalculateCutWithWeapon(int location, bool correctlyblocked, PPLAYER player, PENEMY enemy, bool playerattack)
{
		char bodyparts[MAX_BODY_PARTS][60] = {
	
			"Right Leg",
			"Abdomen",
			"Left Leg",
			"Right Side",
			"Chest",
			"Left Side",
			"Right Shoulder",
			"Neck",
			"Left Shoulder"};
		
		bool gizmo = false;
		BOOL gizmobonus = FALSE;
		BOOL specialhit = FALSE;
		BOOL hitmiss = FALSE; 
		BOOL turnaround = FALSE;
		BOOL knockdown = FALSE;
		BOOL attackerdropweapon = FALSE;
		BOOL defenderdropweapon = FALSE;
		BOOL openwoundhit = FALSE;
		char actionstring[500];
		PSYCHSKILLSTRUCT* psychskill;
		PSYCHSKILLSTRUCT* targetpsychskill;
		PWEAPON weapon;
		int finalchance, finalmindamage, finalmaxdamage;
		double modifierchance, modifiermindamage, modifiermaxdamage; //SO WE CAN WORK ON IT WITHOUT ROUNDING
		int damage;
		
		if(playerattack)
		{
			psychskill = &player->psychskill;
			targetpsychskill = &enemy->psychskill;
			if(gizmo){
				gizmobonus = true;
			}
			weapon = player->currentweapon;
		}
		
		else{
			targetpsychskill = &player->psychskill;
			psychskill = &enemy->psychskill;
			weapon = enemy->currentweapon;
			gizmobonus = false;
		}
		
		/************************************************************
		; CUTTING ALGORITHM
		************************************************************/
		
		
		
		modifierchance = MELEE_CUT_HITMISS_BASE_SUCCESS;
	modifierchance = (modifierchance + ((MELEE_CUT_HITMISS_DISTANCE_MODIFIER) * ((double) globals.globalmeleestruct.distance)));
			modifierchance = (modifierchance + (MELEE_CUT_HITMISS_SKILL_MODIFIER * ((double) psychskill->skills.meleecombatWITHweapon)));
			modifierchance = (modifierchance - (weapon->weight / 2));
			//PLUS UP TO 15 PERCENT FOR SPECIALIZATION
			modifierchance = modifierchance + (psychskill->specializations.handweapons * 5);
		if(correctlyblocked && !playerattack){
			modifierchance -= MELEE_PUNCH_BLOCK_MODIFIER;
		}
		finalchance = (int)modifierchance; //THIS IS THE HIT/MISS CHANCE;
		hitmiss = RandomPercentage(finalchance);
		
		
		if (!hitmiss){ //ATTACK MISSED!!!
			
			//SORRY, NO DAMAGE KID.
			//CHECK FOR A TURNAROUND!!!!!!!!!!!!!!!!!!!!!!!!!!
			modifierchance = MELEE_CUT_TURNAROUND_BASE_SUCCESS;
			modifierchance = (modifierchance - (MELEE_CUT_TURNAROUND_DISTANCE_MODIFIER * ((double)globals.globalmeleestruct.distance)));
			modifierchance = (modifierchance + (MELEE_CUT_TURNAROUND_REFLEX_MODIFIER * ((double)enemy->psychskill.psych.reflex)));
			if(correctlyblocked && !playerattack){
				modifierchance -= MELEE_PUNCH_BLOCK_MODIFIER;
			}
			finalchance = (int)modifierchance;
			turnaround = RandomPercentage(finalchance);
			if(turnaround)
			{
				if(playerattack)
				{
					player->meleestruct.stamina -= MELEE_TURNAROUND_DAMAGE;
					player->meleestruct.posture = MELEE_POSTURE_PRONE;
					if(player->currentweapon && RandomPercentage(80 - (psychskill->skills.meleecombatWITHweapon * 5))){
						attackerdropweapon = true;
					}
				}
				else{
					enemy->meleestruct.stamina -= MELEE_TURNAROUND_DAMAGE;
					enemy->meleestruct.posture = MELEE_POSTURE_PRONE;
				}
			}
			
			//IF NOTHING ELSE, RETURN. WE'RE DONE HERE.
		}
		
		else{ //WE'VE HIT. WHAT NOW? CHECK FOR SPECIAL HITS.
		
	
				//WE KNOW WE'VE HIT, PUT A CUT WOUND IN THE LOCATION.
			if(playerattack)
			{
				enemy->meleestruct.wounds[player->meleestruct.lasttarget] |= WOUND_CUT;
			}
			else{
				player->meleestruct.wounds[enemy->meleestruct.lasttarget] |= WOUND_CUT;
			}
			
//			if (playerattack && gizmobonus){
	//			enemy->meleestruct.wounds[player->meleestruct.lasttarget] |= WOUND_CUT;
	//		}
			
			
			modifierchance = MELEE_CUT_SPECIALHIT_BASE_SUCCESS;
			modifierchance = (modifierchance + (MELEE_CUT_SPECIALHIT_DISTANCE_MODIFIER * (double)globals.globalmeleestruct.distance));
			modifierchance = (modifierchance + (MELEE_CUT_SPECIALHIT_SKILL_MODIFIER * (double)psychskill->skills.stealthcombat));
			finalchance = (int) modifierchance;
			specialhit = RandomPercentage(finalchance);
		
			
			if (specialhit){ //SPECIAL HIT!!!!!!!!
				
				
				damage = MELEE_CUT_SPECIALHIT_DAMAGE;
				
				
				
				/************************************************************
				; THIS FUNCTION BELOW DETERMINES SPECIAL HIT DAMAGE MODIFICATION, DROPPED WEAPONS, ETC
				************************************************************/
				
		//		MeleeSpecialHitDamageCorrection(player, enemy, damage, playerattack); //THIS TAKES CARE OF SPECIAL SITUATIONS
				
			//	modifierchance  = MELEE_CUT_KNOCKDOWN_BASE_SUCCESS;
		//		modifierchance  = (modifierchance + (MELEE_CUT_KNOCKDOWN_AGGRESSION_MODIFIER * (double)psychskill->psych.agression));
			//	finalchance = (int) modifierchance;
				//knockdown = RandomPercentage(finalchance);
				knockdown = false;
			//	if (knockdown){
				//	if(playerattack){
					//	enemy->meleestruct.posture = MELEE_POSTURE_PRONE;
				//	}
				//	else{
				//		player->meleestruct.posture = MELEE_POSTURE_PRONE;
				//	}
					
			//	}
				
			}//END SPECIAL HIT
			
			else{ //CALCULATE REGULAR DAMAGE, PLUS OPEN WOUND PENALTIES.
				
			modifiermindamage = MELEE_CUT_HITDAMAGE_BASE_MINIMUM;
					modifiermaxdamage = MELEE_CUT_HITDAMAGE_BASE_MAXIMUM;
					modifiermaxdamage = modifiermaxdamage + (MELEE_CUT_HITDAMAGE_SKILL_MODIFIER * ((double)psychskill->skills.meleecombatWITHweapon));
					modifiermaxdamage = modifiermaxdamage + (MELEE_CUT_HITDAMAGE_WEIGHT_MODIFIER * ((double)weapon->weight / 30));
					modifiermindamage = modifiermindamage + (MELEE_CUT_HITDAMAGE_AGGRESSION_MODIFIER * ((double)psychskill->psych.agression));
					modifiermindamage = modifiermindamage + (MELEE_CUT_HITDAMAGE_DISTANCE_MODIFIER * ((double)globals.globalmeleestruct.distance));
					
					
					
				
				finalmindamage = (int) modifiermindamage;
				finalmaxdamage = (int) modifiermaxdamage;
				damage = random(finalmindamage, finalmaxdamage);
				
				if(openwoundhit)
				{
					damage += MELEE_CUT_OPENWOUND_BONUS;
				}
			}
		}
		
		
		//RESOLVE DAMAGE
		if(playerattack && hitmiss){
			enemy->meleestruct.stamina -= damage;
		}
		else if (!playerattack && hitmiss){
			player->meleestruct.stamina -= damage;
		}
		
		
		//FINAL PRINTING AREA FOR PLAYER ATTACK.
		if(playerattack)
		{
			
			
			if(!hitmiss)
			{
				MeleePlayRandomMissSoundAsync();
				MeleePrintActionStringHold("Your Swing Goes Wide And Sails By Your Opponent.");
				if (turnaround){
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("TURNAROUND!!!");
					MeleePrintActionStringHold("Your Opponent Quickly Grabs Your Arm And Throws You To The Ground.");
					if (attackerdropweapon){
						MeleePrintActionStringHold("You dropped your weapon!");
						MeleePlayerDropWeapon(player);
					}
				}
			}
			else{
				if(specialhit)
				{
					MeleeAIRaiseAnger();
					playMIXsound(MELEE_SOUND_SPECIALSTAB);
					MeleePrintActionStringHold("Critical Hit!");
					sprintf(actionstring, "You Slice Through His %s, Feeling Tendons Rip!", bodyparts[player->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
					enemy->meleestruct.wounds[player->meleestruct.lasttarget] |= WOUND_BONEBROKEN;
				}
				else{
					playMIXsound(MELEE_SOUND_STAB);
					sprintf(actionstring, "Your Weapon Slashes Into His %s!", bodyparts[player->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
				}
				if(openwoundhit){
					playMIXsound(MELEE_SOUND_SCREAM);
					MeleePrintActionStringHold("Open Wound Hit!");
					damage += MELEE_CUT_OPENWOUND_BONUS;
				}
				if(knockdown)
				{	
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("KNOCKDOWN!!!");
					MeleePrintActionStringHold("Your Hit Knocks Him To The Floor.");
				}
			}	
		}
		//FINAL PRINTING AREA FOR ENEMY ATTACK.
		
		else
		{
			if(!hitmiss)
			{
				if(correctlyblocked){
					playMIXsound(MELEE_SOUND_CATCHPUNCH);
					MeleePrintActionStringHold("You Catch His Arm As It Comes Toward You!");
				}
				else{
					MeleePlayRandomMissSoundAsync();
					MeleePrintActionStringHold("His Swing Sweeps Past You.");
				}
				if (turnaround){
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("TURNAROUND!!!");
					MeleePrintActionStringHold("You Grab His Arm and Throw Him To The Ground!");
					if (attackerdropweapon){
						MeleePrintActionStringHold("He drops his weapon!");
						MeleeEnemyDropWeapon(enemy);
					}
				}
			}
			else{
				if(specialhit)
				{
					MeleeAIDropAnger();
					playMIXsound(MELEE_SOUND_SPECIALSTAB);
					MeleePrintActionStringHold("Critical Hit!");
					sprintf(actionstring, "His Weapon Cuts Deeply Into Your %s!", bodyparts[enemy->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
					player->meleestruct.wounds[enemy->meleestruct.lasttarget] |= WOUND_BONEBROKEN;
				}
				else{
					playMIXsound(MELEE_SOUND_STAB);
					sprintf(actionstring, "His Weapon Slices Into Your %s!", bodyparts[enemy->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
				}
				if(openwoundhit){
					playMIXsound(MELEE_SOUND_SCREAM);
					MeleePrintActionStringHold("Open Wound Hit!");
					damage += MELEE_CUT_OPENWOUND_BONUS;
				}
				if(knockdown)
				{	
					MeleeAIRaiseHonor();
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("KNOCKDOWN!!!");
					MeleePrintActionStringHold("You Are Flung to the Floor By His Blow!");
				}
			}	
		}
		
		//DEATH RESOLUTION
		if (player->meleestruct.stamina <= 0){
			playMIXsound(MELEE_SOUND_FALLDOWN);
			StopSong();
			MeleePrintActionStringHold("You Collapse To The Ground.");
			return - 1;
		}
		else if (enemy->meleestruct.stamina <=0)
		{
			playMIXsound(MELEE_SOUND_FALLDOWN);
			StopSong();
			MeleePrintActionStringHold("He Falls To The Ground.");
			return -1;
		}
		else return 0;

	
}

int MeleeCalculateHitWithWeapon(int location, bool correctlyblocked, PPLAYER player, PENEMY enemy, bool playerattack)
{
	
		char bodyparts[MAX_BODY_PARTS][60] = {
	
			"Right Leg",
			"Abdomen",
			"Left Leg",
			"Right Side",
			"Chest",
			"Left Side",
			"Right Shoulder",
			"Head",
			"Left Shoulder"};
		
		bool gizmo = false;
		BOOL gizmobonus = FALSE;
		BOOL specialhit = FALSE;
		BOOL hitmiss = FALSE; 
		BOOL turnaround = FALSE;
		BOOL knockdown = FALSE;
		BOOL attackerdropweapon = FALSE;
		BOOL defenderdropweapon = FALSE;
		BOOL openwoundhit = FALSE;
		char actionstring[500];
		PSYCHSKILLSTRUCT* psychskill;
		PSYCHSKILLSTRUCT* targetpsychskill;
		PWEAPON weapon;
		int finalchance, finalmindamage, finalmaxdamage;
		double modifierchance, modifiermindamage, modifiermaxdamage; //SO WE CAN WORK ON IT WITHOUT ROUNDING
		int damage;
		
		if(playerattack)
		{
			psychskill = &player->psychskill;
			targetpsychskill = &enemy->psychskill;
			if(gizmo){
				gizmobonus = true;
			}
			weapon = player->currentweapon;
		}
		
		else{
			targetpsychskill = &player->psychskill;
			psychskill = &enemy->psychskill;
			weapon = enemy->currentweapon;
			gizmobonus = false;
		}
		
		/************************************************************
		; PLAYER KICK ALGORITHM
		************************************************************/
		
		
		
		modifierchance = MELEE_HITWITHWEAPON_HITMISS_BASE_SUCCESS;
	modifierchance = (modifierchance + ((MELEE_HITWITHWEAPON_HITMISS_DISTANCE_MODIFIER) * ((double) globals.globalmeleestruct.distance)));
			modifierchance = (modifierchance + (MELEE_HITWITHWEAPON_HITMISS_SKILL_MODIFIER * ((double) psychskill->skills.meleecombatWITHweapon)));
			modifierchance = (modifierchance - (weapon->weight / 2));
			//PLUS UP TO 15 PERCENT FOR SPECIALIZATIONS
			modifierchance = modifierchance + (psychskill->specializations.handweapons * 5);
		if(correctlyblocked && !playerattack){
			modifierchance -= MELEE_PUNCH_BLOCK_MODIFIER;
		}
		finalchance = (int)modifierchance; //THIS IS THE HIT/MISS CHANCE;
		hitmiss = RandomPercentage(finalchance);
		
		
		if (!hitmiss){ //ATTACK MISSED!!!
			
			//SORRY, NO DAMAGE KID.
			//CHECK FOR A TURNAROUND!!!!!!!!!!!!!!!!!!!!!!!!!!
			modifierchance = MELEE_HITWITHWEAPON_TURNAROUND_BASE_SUCCESS;
			modifierchance = (modifierchance - (MELEE_HITWITHWEAPON_TURNAROUND_DISTANCE_MODIFIER * ((double)globals.globalmeleestruct.distance)));
			modifierchance = (modifierchance + (MELEE_HITWITHWEAPON_TURNAROUND_REFLEX_MODIFIER * ((double)enemy->psychskill.psych.reflex)));
			if(correctlyblocked && !playerattack){
				modifierchance -= MELEE_PUNCH_BLOCK_MODIFIER;
			}
			finalchance = (int)modifierchance;
			turnaround = RandomPercentage(finalchance);
			if(turnaround)
			{
				if(playerattack)
				{
					player->meleestruct.stamina -= MELEE_TURNAROUND_DAMAGE;
					player->meleestruct.posture = MELEE_POSTURE_PRONE;
					if(player->currentweapon && RandomPercentage(80 - (psychskill->skills.meleecombatWITHweapon * 5))){
						attackerdropweapon = true;
					}
				}
				else{
					enemy->meleestruct.stamina -= MELEE_TURNAROUND_DAMAGE;
					enemy->meleestruct.posture = MELEE_POSTURE_PRONE;
						if(enemy->currentweapon && RandomPercentage(80 - (psychskill->skills.meleecombatWITHweapon * 5))){
						attackerdropweapon = true;
					}
				}
			}
			
			//IF NOTHING ELSE, RETURN. WE'RE DONE HERE.
		}
		
		else{ //WE'VE HIT. WHAT NOW? CHECK FOR SPECIAL HITS.
			//CHECKING FOR SPECIAL HITS.
			if (playerattack && enemy->meleestruct.wounds[player->meleestruct.lasttarget] & WOUND_CUT){
				openwoundhit = true;
			}
			else if (!playerattack && player->meleestruct.wounds[enemy->meleestruct.lasttarget] & WOUND_CUT){
				openwoundhit = true;
			}
			
			if (playerattack && gizmobonus){
				enemy->meleestruct.wounds[player->meleestruct.lasttarget] |= WOUND_CUT;
			}
			
			
			modifierchance = MELEE_HITWITHWEAPON_SPECIALHIT_BASE_SUCCESS;
			modifierchance = (modifierchance + (MELEE_HITWITHWEAPON_SPECIALHIT_DISTANCE_MODIFIER * (double)globals.globalmeleestruct.distance));
			modifierchance = (modifierchance + (MELEE_HITWITHWEAPON_SPECIALHIT_SKILL_MODIFIER * (double)psychskill->skills.stealthcombat));
			finalchance = (int) modifierchance;
			specialhit = RandomPercentage(finalchance);
		
			
			if (specialhit){ //SPECIAL HIT!!!!!!!!
				
				
				damage = MELEE_HITWITHWEAPON_SPECIALHIT_DAMAGE;
				
				
				
				/************************************************************
				; THIS FUNCTION BELOW DETERMINES SPECIAL HIT DAMAGE MODIFICATION, DROPPED WEAPONS, ETC
				************************************************************/
				
				MeleeSpecialHitDamageCorrection(player, enemy, damage, playerattack); //THIS TAKES CARE OF SPECIAL SITUATIONS
				
				modifierchance  = MELEE_HITWITHWEAPON_KNOCKDOWN_BASE_SUCCESS;
				modifierchance  = (modifierchance + (MELEE_HITWITHWEAPON_KNOCKDOWN_AGGRESSION_MODIFIER * (double)psychskill->psych.agression));
				finalchance = (int) modifierchance;
				knockdown = RandomPercentage(finalchance);
				
				if (knockdown){
					if(playerattack){
						enemy->meleestruct.posture = MELEE_POSTURE_PRONE;
					}
					else{
						player->meleestruct.posture = MELEE_POSTURE_PRONE;
					}
					
				}
					if(openwoundhit)
				{
					damage += MELEE_HITWITHWEAPON_OPENWOUND_BONUS;
				}
				
			}//END SPECIAL HIT
			
			else{ //CALCULATE REGULAR DAMAGE, PLUS OPEN WOUND PENALTIES.
				
			modifiermindamage = MELEE_HITWITHWEAPON_HITDAMAGE_BASE_MINIMUM;
					modifiermaxdamage = MELEE_HITWITHWEAPON_HITDAMAGE_BASE_MAXIMUM;
					modifiermaxdamage = modifiermaxdamage + (MELEE_HITWITHWEAPON_HITDAMAGE_SKILL_MODIFIER * ((double)psychskill->skills.meleecombatWITHweapon));
					modifiermaxdamage = modifiermaxdamage + (MELEE_HITWITHWEAPON_HITDAMAGE_WEIGHT_MODIFIER * ((double)weapon->weight / 30));
					modifiermindamage = modifiermindamage + (MELEE_HITWITHWEAPON_HITDAMAGE_AGGRESSION_MODIFIER * ((double)psychskill->psych.agression));
					modifiermindamage = modifiermindamage + (MELEE_HITWITHWEAPON_HITDAMAGE_DISTANCE_MODIFIER * ((double)globals.globalmeleestruct.distance));
					
					
					
				
				finalmindamage = (int) modifiermindamage;
				finalmaxdamage = (int) modifiermaxdamage;
				damage = random(finalmindamage, finalmaxdamage);
				
				if(openwoundhit)
				{
					damage += MELEE_HITWITHWEAPON_OPENWOUND_BONUS;
				}
			}
		}
		
		
		//RESOLVE DAMAGE
		if(playerattack && hitmiss){
			enemy->meleestruct.stamina -= damage;
		}
		else if (!playerattack && hitmiss){
			player->meleestruct.stamina -= damage;
		}
		
		
		//FINAL PRINTING AREA FOR PLAYER ATTACK.
		if(playerattack)
		{
			
			
			if(!hitmiss)
			{
				MeleePlayRandomMissSoundAsync();
				MeleePrintActionStringHold("Your Swing Goes Wide And Sails By Your Opponent.");
				if (turnaround){
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("TURNAROUND!!!");
					MeleePrintActionStringHold("Your Opponent Quickly Grabs Your Arm And Throws You To The Ground.");
					if (attackerdropweapon){
						MeleePrintActionStringHold("You dropped your weapon!");
						MeleePlayerDropWeapon(player);
					}
				}
			}
			else{
				if(specialhit)
				{
					MeleeAIRaiseAnger();
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("Critical Hit!");
					sprintf(actionstring, "You Hit His %s Hard. You Hear Something Give.", bodyparts[player->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
					enemy->meleestruct.wounds[player->meleestruct.lasttarget] |= WOUND_BONEBROKEN;
				}
				else{
					playMIXsound(MELEE_SOUND_HITWITHWEAPON);
					sprintf(actionstring, "Your Weapon Connects With His %s!", bodyparts[player->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
				}
				if(openwoundhit){
					playMIXsound(MELEE_SOUND_SCREAM);
					MeleePrintActionStringHold("Open Wound Hit!");
					damage += MELEE_HITWITHWEAPON_OPENWOUND_BONUS;
				}
				else if (gizmobonus){
					sprintf(actionstring, "The Blade Ring Cuts Deep Into His %s!", bodyparts[player->meleestruct.lasttarget]);
					playMIXsound(MELEE_SOUND_RING);
					MeleePrintActionStringHold(actionstring);
				}
				if(knockdown)
				{	
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("KNOCKDOWN!!!");
					MeleePrintActionStringHold("Your Hit Knocks Him To The Floor.");
				}
			}	
		}
		//FINAL PRINTING AREA FOR ENEMY ATTACK.
		
		else
		{
			if(!hitmiss)
			{
				if(correctlyblocked){
					playMIXsound(MELEE_SOUND_CATCHPUNCH);
					MeleePrintActionStringHold("You Catch His Arm As It Comes Toward You!");
				}
				else{
					MeleePlayRandomMissSoundAsync();
					MeleePrintActionStringHold("His Swing Sweeps Past You.");
				}
				if (turnaround){
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("TURNAROUND!!!");
					MeleePrintActionStringHold("You Grab His Arm and Throw Him To The Ground!");
					if (attackerdropweapon){
						MeleePrintActionStringHold("He drops his weapon!");
						MeleeEnemyDropWeapon(enemy);
					}
				}
			}
			else{
				if(specialhit)
				{
					MeleeAIDropAnger();
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("Critical Hit!");
					sprintf(actionstring, "He Hits Your %s Hard. Something Cracks.", bodyparts[enemy->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
					player->meleestruct.wounds[enemy->meleestruct.lasttarget] |= WOUND_BONEBROKEN;
				}
				else{
					playMIXsound(MELEE_SOUND_HITWITHWEAPON);
					sprintf(actionstring, "His Weapon Connects With Your %s!", bodyparts[enemy->meleestruct.lasttarget]);
					MeleePrintActionStringHold(actionstring);
				}
				if(openwoundhit){
					playMIXsound(MELEE_SOUND_SCREAM);
					MeleePrintActionStringHold("Open Wound Hit!");
					damage += MELEE_HITWITHWEAPON_OPENWOUND_BONUS;
				}
				if(knockdown)
				{	
					MeleeAIRaiseHonor();
					playMIXsound(MELEE_SOUND_SPECIALHIT);
					MeleePrintActionStringHold("KNOCKDOWN!!!");
					MeleePrintActionStringHold("You Are Flung to the Floor By His Blow!");
				}
			}	
		}
		
		//DEATH RESOLUTION
		if (player->meleestruct.stamina <= 0){
			playMIXsound(MELEE_SOUND_FALLDOWN);
			StopSong();
			MeleePrintActionStringHold("You Collapse To The Ground.");
			return - 1;
		}
		else if (enemy->meleestruct.stamina <=0)
		{
			playMIXsound(MELEE_SOUND_FALLDOWN);
			StopSong();
			MeleePrintActionStringHold("He Falls To The Ground.");
			return -1;
		}
		else return 0;

	
}

void MeleeCalculateStrangle(PPLAYER player, PENEMY enemy, bool playerattack)
{
	//STRANGLING!!!
	//IF STRANGLING WATCH, ADD 20%
	//BASE CHANCE = 3%.
	//ADD UP TO 25% FOR HEALTH
	//ADD UP TO 10% FOR RANGE
	//ADD UP TO 15% FOR STEALTH COMBAT.
	double basechance = 8.0;
	MELEESTRUCT* attackerms;
	MELEESTRUCT* defenderms;
	PSYCHSKILLSTRUCT* attackerps;
//	PSYCHSKILLSTRUCT* defenderps;

	if(playerattack){
		attackerms = &player->meleestruct;
		defenderms = &enemy->meleestruct;
		attackerps = &player->psychskill;
	}
	else{
		defenderms = &player->meleestruct;
		attackerms = &enemy->meleestruct;
		attackerps = &enemy->psychskill;
	}
	
	if(playerattack){
	if(DoesListHaveGizmoType(GIZMO_TYPE_WATCH, player->p_gizmos, false)){
		playMIXsound(MELEE_SOUND_WATCH);
		MeleePrintActionStringHold("You release your strangling cord from your watch, pulling it taut around his neck.");
		basechance += 20.0;
	}
	else{
		MeleePrintActionStringHold("You go for his throat!");
	}
	
	MeleePrintActionString("You squeeze, feeling the cartilage give...");
	}

	else{
		MeleePrintActionString("He Clasps Your Throat...You Feel Your Muscles Weaken.");
	}
	playSYNCsound(MELEE_SOUND_STRANGLEATTEMPT);
	
	double healthratio = (double) (100 - defenderms->stamina) / 100.0;
	if (defenderms->stamina == 100 || defenderms->stamina == 99){
		healthratio= 0;
	}
	healthratio *= 25.0; //THERE WE GO.
	double skillratio = (double)attackerps->skills.stealthcombat / (double)MAX_SKILL;
	skillratio *= 15.0;

	double distanceratio = (double)(MELEE_MAX_DISTANCE - globals.globalmeleestruct.distance) / (double)MELEE_MAX_DISTANCE;
	distanceratio *= 10.0;

	
	basechance += skillratio;
	basechance += distanceratio;
	basechance += healthratio;
	
	if(!RandomPercentage((int)basechance))
	{
		playMIXsound(MELEE_SOUND_SPECIALHIT);
		MeleePrintActionStringHold("TURNAROUND!!!");
		
		if(playerattack){
		MeleePrintActionStringHold("Your Opponent Twists You Away and Flips You To the Ground.");
		if (player->currentweapon && RandomPercentage(80 - (player->psychskill.skills.meleecombatWITHweapon * 5))){
			MeleePrintActionStringHold("You dropped your weapon!");
			MeleePlayerDropWeapon(player);
		}
		}

		else{
			MeleePrintActionStringHold("You Twist Your Opponent Away To The Ground.");
		if (enemy->currentweapon && RandomPercentage(80 - (enemy->psychskill.skills.meleecombatWITHweapon * 5))){
			MeleePrintActionStringHold("He Dropped His Weapon!");
			MeleeEnemyDropWeapon(enemy);
		}
		}
		
		attackerms->stamina -= MELEE_TURNAROUND_DAMAGE;
		attackerms->posture = MELEE_POSTURE_PRONE;
		if (attackerms->stamina <= 0){
			playMIXsound(MELEE_SOUND_FALLDOWN);
			StopSong();
			if(playerattack){
			MeleePrintActionStringHold("You Collapse To The Ground.");
			}
			else{
				MeleePrintActionStringHold("He Collapses To The Ground.");
			}
			return;
		}
		
		
	}
	else
	{
		StopSong();
		if(playerattack){
		MeleePrintActionStringHold("You feel his life ebb as your grip tightens.");
		playSYNCsound(MELEE_SOUND_STRANGLEDEATH);
		enemy->meleestruct.stamina = 0;
		}
		else{
			MeleePrintActionStringHold("You Feel Yourself Slipping Away...");
			playSYNCsound(MELEE_SOUND_STRANGLEDEATH);
		player->meleestruct.stamina = 0;
		}
	}
	
}

void MeleeCalculateDrug(int location,  bool correctlyblocked, PPLAYER player, PENEMY enemy, bool playerattack)
{
	//DRUGGING!
	
	char actionstring[1000];
	double basechance = 10.0;
	MELEESTRUCT* attackerms;
	MELEESTRUCT* defenderms;
	PSYCHSKILLSTRUCT* attackerps;
//	PSYCHSKILLSTRUCT* defenderps;

	if(playerattack){
		attackerms = &player->meleestruct;
		defenderms = &enemy->meleestruct;
		attackerps = &player->psychskill;
	}
	else{
		defenderms = &player->meleestruct;
		attackerms = &enemy->meleestruct;
		attackerps = &enemy->psychskill;
	}
	
	if(playerattack){
	MeleePrintActionStringHold("You Maneuver To Try and Inject Him!");
	playMIXsound(MELEE_SOUND_DRUGATTEMPT);
	sprintf(actionstring, "You Attempt To Plunge the %s In His Neck!", player->currentweapon->shortname);
	MeleePrintActionStringHold(actionstring);
	}

	else{
		playMIXsound(MELEE_SOUND_DRUGATTEMPT);
	sprintf(actionstring,"He Tries To Inject You With The %s!", enemy->currentweapon->shortname);
	MeleePrintActionStringHold(actionstring);
	}
	
	
	double healthratio = (double) (100 - defenderms->stamina) / 100.0;
	if (defenderms->stamina == 100 || defenderms->stamina == 99){
		healthratio= 0;
	}
	healthratio *= 10.0; //THERE WE GO.
	double skillratio = (double)attackerps->skills.stealthcombat / (double)MAX_SKILL;
	skillratio *= 20.0;

	double distanceratio = (double)(MELEE_MAX_DISTANCE - globals.globalmeleestruct.distance) / (double)MELEE_MAX_DISTANCE;
	distanceratio *= 30.0;

	
	basechance += skillratio;
	basechance += distanceratio;
	basechance += healthratio;

	basechance += (double)(5 * attackerps->specializations.specialweapons);
	
	if(!RandomPercentage((int)basechance))
	{
		playMIXsound(MELEE_SOUND_SPECIALHIT);
		MeleePrintActionStringHold("TURNAROUND!!!");
		
		if(playerattack){
		MeleePrintActionStringHold("Your Opponent Twists You Away and Flips You To the Ground.");
		if (player->currentweapon && RandomPercentage(80 - (player->psychskill.skills.meleecombatWITHweapon * 5))){
			MeleePrintActionStringHold("You dropped your weapon!");
			MeleePlayerDropWeapon(player);
		}
		}

		else{
			MeleePrintActionStringHold("You Twist Your Opponent Away To The Ground.");
		if (enemy->currentweapon && RandomPercentage(80 - (enemy->psychskill.skills.meleecombatWITHweapon * 5))){
			MeleePrintActionStringHold("He Dropped His Weapon!");
			MeleeEnemyDropWeapon(enemy);
		}
		}
		
		attackerms->stamina -= MELEE_TURNAROUND_DAMAGE;
		attackerms->posture = MELEE_POSTURE_PRONE;
		if (attackerms->stamina <= 0){
			playMIXsound(MELEE_SOUND_FALLDOWN);
			StopSong();
			if(playerattack){
			MeleePrintActionStringHold("You Collapse To The Ground.");
			}
			else{
				MeleePrintActionStringHold("He Collapses To The Ground.");
			}
			return;
		}
		
		
	}
	else
	{
		StopSong();
		if(playerattack){
		MeleePrintActionString("You've Injected Him.");
		player->currentweapon->drug.doses--;
		playSYNCsound(MELEE_SOUND_DRUGDEATH);
		enemy->meleestruct.stamina = 0;
		}
		else{
			MeleePrintActionString("You've Just Been Injected.");
			enemy->currentweapon->drug.doses--;
			playSYNCsound(MELEE_SOUND_DRUGDEATH);
		player->meleestruct.stamina = 0;
		}
	}
		//DEATH RESOLUTION
		if (player->meleestruct.stamina <= 0){
			playMIXsound(MELEE_SOUND_FALLDOWN);
	//		StopSong();
			MeleePrintActionStringHold("You Collapse To The Ground.");
			return;
		}
		else if (enemy->meleestruct.stamina <=0)
		{
			playMIXsound(MELEE_SOUND_FALLDOWN);
		//	StopSong();
			MeleePrintActionStringHold("He Falls To The Ground.");
			return;
		}
}

void MeleeCalculateFirearmAttack(int location, PPLAYER player, PENEMY enemy, BOOL playerattack)
{
	
	char bodyparts[MAX_BODY_PARTS][60] = {
			"Right Leg",
			"Abdomen",
			"Left Leg",
			"Right Side",
			"Chest",
			"Left Side",
			"Right Shoulder",
			"Head",
			"Left Shoulder"};
		
		
		bool CLICK = false; //HAHA, CLICK!!!!
		bool CaughtInPlate = false; //TRAUMA PLATE CATCH
		int firetype = -1;
		int ammotype = -1;
		PWEAPON currentweapon = NULL;
		PPSYCHSKILLSTRUCT firerpsych = NULL;
		PPSYCHSKILLSTRUCT targetpsych = NULL;
		PBULLET bulletdata = NULL;
		PSHELL shelldata = NULL;
		PROUND round = NULL;
		int roundsfired;
		double hitchance = 0.0;
		double damage = 0.0;
		double knockdownbonus = 0.0;
		double weaponlossbonus = 15.0;
		bool knockdown = false;
		int stance;
		
		if(playerattack)
		{
			currentweapon = player->currentweapon;
			firerpsych = &player->psychskill;
			targetpsych = &enemy->psychskill;
			stance = enemy->meleestruct.posture;
		}
		else{
			currentweapon = enemy->currentweapon;
			firerpsych = &enemy->psychskill;
			targetpsych = &player->psychskill;
			stance = player->meleestruct.posture;
		}
		
		if(currentweapon == NULL){
			return;
		}
		if (currentweapon->loadtype == WEAP_LOADTYPE_NONE)
		{
			CLICK = true;
		}
		//CLIP WEAPONS
		if(currentweapon->loadtype == WEAP_LOADTYPE_CLIP)
		{
			if(!currentweapon->loadprocedure.clip.isclipinserted)
			{
				CLICK = true;
			}
			else if (currentweapon->loadprocedure.clip.clipinserted.currentrounds < 1){
				CLICK = true;
			}
			else{//GUESS WE CAN FIRE...NEVER DONE THIS BEFORE...
				switch (RetrieveGlobalSubAmmoType(&currentweapon->loadprocedure.clip.clipinserted.clipround))
				{
				case AMMOTYPE_EXPLOSIVE://CANNOT FIRE EXPLOSIVES!!!!!!!!!!
					MeleePrintActionStringHold("This weapon uses explosives! You'll blow yourself to pieces!");
					return;
				case AMMOTYPE_BULLET:
					
					ammotype = AMMOTYPE_BULLET;
					break;
				case AMMOTYPE_SHOTGUNSHELL:
					ammotype = AMMOTYPE_SHOTGUNSHELL;
					
					break;
				}
				firetype = WEAP_LOADTYPE_CLIP;
				round = &currentweapon->loadprocedure.clip.clipinserted.clipround;
			}
		}
		//MAGAZINE WEAPON
		else if (currentweapon->loadtype == WEAP_LOADTYPE_MAGAZINE)
		{
			if (currentweapon->loadprocedure.magazine.totalroundschambered < 1){
				CLICK = true;
			}
			else{ //PREPARE TO FIRE.
				switch (RetrieveGlobalSubAmmoType(&currentweapon->loadprocedure.magazine.chamberedrounds[0]))
				{
				case AMMOTYPE_EXPLOSIVE://CANNOT FIRE EXPLOSIVES!!!!!!!!!!
					MeleePrintActionStringHold("This weapon uses explosives! You'll blow yourself to pieces!");
					return;
				case AMMOTYPE_BULLET:
					ammotype = AMMOTYPE_BULLET;
					break;
				case AMMOTYPE_SHOTGUNSHELL:
					ammotype = AMMOTYPE_SHOTGUNSHELL;
					break;
				}
				firetype = WEAP_LOADTYPE_MAGAZINE;
				round = &currentweapon->loadprocedure.magazine.chamberedrounds[0];
			}
		}
		
		//CYLINDER WEAPON
		else if (currentweapon->loadtype == WEAP_LOADTYPE_CYLINDER)
		{
			if (currentweapon->loadprocedure.cylinder.statusindicators[currentweapon->loadprocedure.cylinder.currentposition] != CYLINDER_CHAMBER_FULL){
				CLICK = true;
				RotateCylinder(currentweapon);
			}
			else{ //PREPARE TO FIRE.
				switch (RetrieveGlobalSubAmmoType(&currentweapon->loadprocedure.cylinder.chamberedrounds[currentweapon->loadprocedure.cylinder.currentposition]))
				{
				case AMMOTYPE_EXPLOSIVE://CANNOT FIRE EXPLOSIVES!!!!!!!!!!
					MeleePrintActionStringHold("This weapon uses explosives! You'll blow yourself to pieces!");
					return;
				case AMMOTYPE_BULLET:
					ammotype = AMMOTYPE_BULLET;
					break;
				case AMMOTYPE_SHOTGUNSHELL:
					ammotype = AMMOTYPE_SHOTGUNSHELL;
					break;
				}
				firetype = WEAP_LOADTYPE_CYLINDER;
				round = &currentweapon->loadprocedure.cylinder.chamberedrounds[currentweapon->loadprocedure.cylinder.currentposition];
			}
		}
		
		
		if (CLICK)
		{
			playMIXsound(WEAPON_SOUND_CLICK);
			MeleePrintActionStringHold("CLICK.");
			return;
		}
		
		else{ //HERE WE GO!
			switch(firetype)
			{
				//CLIPS
			case WEAP_LOADTYPE_CLIP:
				switch(ammotype){
				case AMMOTYPE_BULLET:
					if(FireClipBullet(currentweapon, bulletdata, &roundsfired) < FIRECLIP_OK_ALLROUNDS){
						playMIXsound(WEAPON_SOUND_CLICK);
						MeleePrintActionStringHold("CLICK.");
						return;
					}
					break;
				case AMMOTYPE_SHOTGUNSHELL:
					if(FireClipShell(currentweapon, shelldata, &roundsfired) < FIRECLIP_OK_ALLROUNDS){
						playMIXsound(WEAPON_SOUND_CLICK);
						MeleePrintActionStringHold("CLICK.");
						return;
					}
					break;
					
				}
				break;
				//MAGAZINES
				case WEAP_LOADTYPE_MAGAZINE:
					switch(ammotype){
					case AMMOTYPE_BULLET:
						if(FireMagazineBullet(currentweapon, bulletdata) != FIREMAGAZINE_OK){
							playMIXsound(WEAPON_SOUND_CLICK);
							MeleePrintActionStringHold("CLICK.");
							return;
						}
						break;
					case AMMOTYPE_SHOTGUNSHELL:
						if(FireMagazineShell(currentweapon, shelldata) != FIREMAGAZINE_OK){
							playMIXsound(WEAPON_SOUND_CLICK);
							MeleePrintActionStringHold("CLICK.");
							return;
						}
						break;
					}
					break;
					//CYLINDERS
					case WEAP_LOADTYPE_CYLINDER:
						switch(ammotype){
						case AMMOTYPE_BULLET:
							if(FireCylinderBullet(currentweapon, bulletdata) != FIRECYLINDER_OK){
								playMIXsound(WEAPON_SOUND_CLICK);
								MeleePrintActionStringHold("CLICK.");
								return;
							}
							break;
						case AMMOTYPE_SHOTGUNSHELL:
							if(FireCylinderShell(currentweapon, shelldata) != FIRECYLINDER_OK){
								playMIXsound(WEAPON_SOUND_CLICK);
								MeleePrintActionStringHold("CLICK.");
								return;
							}
							break;
							
						}
						break;
						
			}
			
			//CALC DAMAGE HERE!
			hitchance = MeleeGetGunShotHitChance(currentweapon, firerpsych, targetpsych, stance);
		//	MeleePrintDebugNumber("CHANCE", hitchance);
			if(!RandomPercentage((int)hitchance)){
				if(!currentweapon->modifiersattached.silencer){
					playWeaponSound(currentweapon, WEAPONSOUND_SHOOT, SOUND_MODE_SYNC);
				}
				else{
					playWeaponSound(currentweapon, WEAPONSOUND_SILENCEDSHOOT, SOUND_MODE_SYNC);
				}
				playRicochet(SOUND_MODE_MIX);
				MeleePrintActionStringHold("MISS!");
				return;
			}
			
			//WE'VE HIT. CALCULATE DAMAGE.
			
			hitchance = MeleeGetGunShotDamage(round, firerpsych, targetpsych);
	//		MeleePrintDebugNumber("DAMAGE", hitchance);
			if(location == MELEE_TARGET_HEAD){
				hitchance += 20;
			}
			else if (location == MELEE_TARGET_LLEG || location == MELEE_TARGET_RLEG){
				hitchance -= 10;
				knockdownbonus += 15;
			}
			else if(location == MELEE_TARGET_LSHOULDER || location == MELEE_TARGET_RSHOULDER){
				hitchance -= 10; 
				weaponlossbonus += 25;
			}

			if(FindActiveGizmoOfType(GIZMO_TYPE_TRAUMAPLATES, player->p_gizmos) && !playerattack && enemy->meleestruct.lasttarget == MELEE_TARGET_TORSO && RandomPercentage(80))
			{
				CaughtInPlate = true;
			}
			
			if(playerattack)
			{
				
				enemy->meleestruct.wounds[player->meleestruct.lasttarget] |= WOUND_SHOT;
				if (hitchance >= enemy->meleestruct.stamina){ //INSTANTLY LETHAL SHOT
					StopSong();
					
					enemy->meleestruct.stamina = 0;
					
					if(!currentweapon->modifiersattached.silencer){
						playWeaponSound(currentweapon, WEAPONSOUND_SHOOT, SOUND_MODE_MIX);
					}
					else{
						playWeaponSound(currentweapon, WEAPONSOUND_SILENCEDSHOOT, SOUND_MODE_MIX);
					}
					//FINISH HIM OFF.
					MeleePrintActionStringHold("The shot is instantly lethal.");
					Sleep(1500);
					playMIXsound(MELEE_SOUND_FALLDOWN);
					MeleePrintActionStringHold("He Falls To The Ground.");
					return;
				}
				else{//STANDARD SHOT.
					if(!currentweapon->modifiersattached.silencer){
						playWeaponSound(currentweapon, WEAPONSOUND_SHOOT, SOUND_MODE_SYNC);
					}
					else{
						playWeaponSound(currentweapon, WEAPONSOUND_SILENCEDSHOOT, SOUND_MODE_SYNC);
					}
					playAmmoImpactSound(*round, SOUND_MODE_MIX);
					sprintf(globals.globalmeleestruct.actionstring, "The %s tears into his %s!", RetrieveSubAmmoShortName(round), bodyparts[player->meleestruct.lasttarget]);
					
					MeleePrintActionStringHold(globals.globalmeleestruct.actionstring);
					
					enemy->meleestruct.stamina -= (int)hitchance;
					//CALCULATE KNOCKDOWN.
					if(MeleeGetGunShotKnockdown(round, firerpsych, targetpsych)){
						playMIXsound(MELEE_SOUND_SPECIALHIT);
						MeleePrintActionStringHold("Your shot throws your opponent to the ground!");
						enemy->meleestruct.posture = MELEE_POSTURE_PRONE;
					}
					
					if(RandomPercentage((int)weaponlossbonus) && enemy->currentweapon != NULL){
						MeleePrintActionStringHold("He drops his weapon!");
						MeleeEnemyDropWeapon(enemy);
					}
					
					
					
				}
			}	

		
			
			else//ENEMY ATTACK
			{
			
				if(!CaughtInPlate){
				player->meleestruct.wounds[enemy->meleestruct.lasttarget] |= WOUND_SHOT;
				}
				if (hitchance >= player->meleestruct.stamina && !CaughtInPlate){ //INSTANTLY LETHAL SHOT
					StopSong();
					
					player->meleestruct.stamina = 0;
					
					if(!currentweapon->modifiersattached.silencer){
						playWeaponSound(currentweapon, WEAPONSOUND_SHOOT, SOUND_MODE_MIX);
					}
					else{
						playWeaponSound(currentweapon, WEAPONSOUND_SILENCEDSHOOT, SOUND_MODE_MIX);
					}
					//FINISH HIM OFF.
					MeleePrintActionStringHold("The shot is instantly crippling.");
					Sleep(1500);
					playMIXsound(MELEE_SOUND_FALLDOWN);
					MeleePrintActionStringHold("You Fall To The Ground.");
					return;
				}
				else{//STANDARD SHOT.
					if(!currentweapon->modifiersattached.silencer){
						playWeaponSound(currentweapon, WEAPONSOUND_SHOOT, SOUND_MODE_SYNC);
					}
					else{
						playWeaponSound(currentweapon, WEAPONSOUND_SILENCEDSHOOT, SOUND_MODE_SYNC);
					}
					if(!CaughtInPlate){
					playAmmoImpactSound(*round, SOUND_MODE_MIX);
					sprintf(globals.globalmeleestruct.actionstring, "His shot tears into your %s!", bodyparts[enemy->meleestruct.lasttarget]);
					MeleePrintActionStringHold(globals.globalmeleestruct.actionstring);
					
					player->meleestruct.stamina -= (int)hitchance;
					}
					else{
						playMIXsound(TRAUMAPLATE_HIT_SOUND);
						sprintf(globals.globalmeleestruct.actionstring, "Your trauma plate absorbs the shot!", bodyparts[enemy->meleestruct.lasttarget]);
						MeleePrintActionStringHold(globals.globalmeleestruct.actionstring);
					}
					//CALCULATE KNOCKDOWN.
					if(MeleeGetGunShotKnockdown(round, firerpsych, targetpsych)){
						playMIXsound(MELEE_SOUND_SPECIALHIT);
						MeleePrintActionStringHold("His shot sends you sprawling to the ground!");
						player->meleestruct.posture = MELEE_POSTURE_PRONE;
					}
					if(RandomPercentage((int)weaponlossbonus) && player->currentweapon != NULL){
						MeleePrintActionStringHold("You drop your weapon!");
						MeleePlayerDropWeapon(player);
					}
					
				}
			}
			
	}
}


int MeleeGetGunShotHitChance(PWEAPON weapon, PPSYCHSKILLSTRUCT firerpsych, PPSYCHSKILLSTRUCT targetpsych, int targetstance)
{
	//FIRST, CHECK IF HIT AT ALL. FACTORS, IF PRONE, +20%, IF KNEEL, +10% FIREARM COMBAT 20%, GUN TYPE SPECIALIZATION +5%, IF SILENCER -5%, TARGET REFLEX 15%, BASE WEAPON ACCURACY 10%, LASER SIGHT +15%, CALMNESS 10%
	double hitchance;
	
	hitchance = MELEE_SHOOT_HITMISS_BASE_SUCCESS;
	hitchance += 20.0 * firerpsych->skills.firearms / (double)MAX_SKILL;
	switch(weapon->filingtype)
	{
	case WEAP_FILING_TYPE_PISTOLS:
		hitchance += 5.0 * firerpsych->specializations.handguns;
		break;
	case WEAP_FILING_TYPE_SMGS:
		hitchance += 5.0 * firerpsych->specializations.submachineguns;
		break;
	case WEAP_FILING_TYPE_SHOTGUN:
		hitchance += 5.0 * firerpsych->specializations.shotguns;
		break;
	case WEAP_FILING_TYPE_RIFLES:
		hitchance += 5.0 * firerpsych->specializations.rifles;
		break;
	}
	
	if (weapon->modifiersattached.silencer){
		hitchance -= 5.0;
	}
	
	if (weapon->modifiersattached.lasersight){
		hitchance += 15.0;
	}
	
	hitchance -= 10 * (100 - weapon->weapperf.baseaccuracy_percent) / 100.0;
	
	hitchance -= 15 * targetpsych->psych.reflex / (double)MAX_PSYCH;
	
	hitchance += 10 * firerpsych->psych.calmness / (double)MAX_PSYCH;
	
	if (targetstance == MELEE_POSTURE_PRONE){
		hitchance += 20.0;
	}
	else if (targetstance == MELEE_POSTURE_KNEELING){
		hitchance += 10.0;
	}
	
	return (int)hitchance;
}

bool MeleeGetGunShotKnockdown(PROUND round, PPSYCHSKILLSTRUCT firerpsych, PPSYCHSKILLSTRUCT targetpsych)
{
	PBULLET bullet;
	PSHELL shell;
	double hitchance = 20.0;
	//BULLETS FIRST. FACTORS - ENERGY TRANSMISSION 50%, TARGET WILLPOWER 20%
	switch(RetrieveGlobalSubAmmoType(round)){
	case AMMOTYPE_BULLET:
		bullet = RetrieveGlobalBulletData(round);
		hitchance += 50.0 * bullet->energy / 100.0;
		break;
		//SHOTGUNSHELLS NEXT. FACTORS - ENERGY TRANSMISSION 50%, SPREAD 10%, TARGET WILLPOWER 20%
	case AMMOTYPE_SHOTGUNSHELL:
		shell = RetrieveGlobalShellData(round);
		hitchance += 50.0 * shell->energy / 100.0;
		hitchance -= 10.0 * shell->spread / 100.0;
		break;
	}
	
	hitchance -= 20.0 * targetpsych->psych.willpower / (double)MAX_PSYCH;
	
	if (RandomPercentage((int)hitchance)){
		return true;
	}
	return false;
	
}

int MeleeGetGunShotDamage(PROUND round, PPSYCHSKILLSTRUCT firerpsych, PPSYCHSKILLSTRUCT targetpsych)
{
	//CHECK DAMAGE.
	PBULLET bullet;
	PSHELL shell;
	//BULLETS FIRST. FACTORS. AP -10%, DAMAGE 50%, TARGET AGGRESSION 15%, DISTANCE 15%
	double hitdamage = 20.0;
	
	switch(RetrieveGlobalSubAmmoType(round)){
	case AMMOTYPE_BULLET:
		bullet = RetrieveGlobalBulletData(round);
		hitdamage += 50.0 * (double)bullet->damage / 100.0;
		hitdamage += 20.0 * (double)bullet->latentdamage / 100.0;
		hitdamage -= 10.0 * (double)bullet->ap / 100.0;
		break;
		//SHOTGUNSHELLS NEXT. FACTORS - DAMAGE 50%, AP -10%, SPREAD +10% , TARGET AGGRESSION 15%, DISTANCE 15%
	case AMMOTYPE_SHOTGUNSHELL:
		shell = RetrieveGlobalShellData(round);
		hitdamage += 50.0 * (double)shell->damage / 100.0;
		hitdamage += 10.0 * (double)shell->spread / 100.0;
		hitdamage -= 10.0 *(double) shell->spread / 100.0;
		break;
	}
	
	hitdamage -= 15 * globals.globalmeleestruct.distance / (double)MELEE_MAX_DISTANCE;
	hitdamage -= 15 * targetpsych->psych.agression / (double)MAX_PSYCH;
	
	return (int)hitdamage;
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleePrintPlayerandEnemyStats
//
// Description   - Prints player and enemy stats in their respective boxes.
//
// Return Type   - void 
//
// Arguments     - PPLAYER player ( The pointer to the player )
//               - PENEMY enemy ( The pointer to the enemy )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void MeleePrintPlayerandEnemyStats(PPLAYER player, PENEMY enemy)
{
	COORD playerboxcoord = {MELEE_PLAYERBOX_TOPLEFT_X, MELEE_PLAYERBOX_TOPLEFT_Y + 2};
	COORD playerboxdimensions = {MELEE_PLAYERBOX_TOPLEFT_X + MELEE_PLAYERBOX_WIDTH, MELEE_PLAYERBOX_TOPLEFT_Y + MELEE_PLAYERBOX_HEIGHT + 2};
	COORD enemyboxcoord = {MELEE_ENEMYBOX_TOPLEFT_X, MELEE_ENEMYBOX_TOPLEFT_Y + 2};
	COORD enemyboxdimensions = {MELEE_ENEMYBOX_TOPLEFT_X + MELEE_ENEMYBOX_WIDTH, MELEE_ENEMYBOX_TOPLEFT_Y + MELEE_ENEMYBOX_HEIGHT + 2};
	RECT enemybox;
	RECT playerbox;
	
	enemybox.left = MELEE_ENEMYBOX_TOPLEFT_X;
	enemybox.top = MELEE_ENEMYBOX_TOPLEFT_Y + 2;
	enemybox.right = MELEE_ENEMYBOX_TOPLEFT_X + MELEE_ENEMYBOX_WIDTH - 1;
	enemybox.bottom = MELEE_ENEMYBOX_TOPLEFT_Y + 2 + MELEE_ENEMYBOX_HEIGHT - 1;
	
	playerbox.left = MELEE_PLAYERBOX_TOPLEFT_X;
	playerbox.top = MELEE_PLAYERBOX_TOPLEFT_Y + 2;
	playerbox.right = MELEE_PLAYERBOX_TOPLEFT_X + MELEE_PLAYERBOX_WIDTH- 1;
	playerbox.bottom = MELEE_PLAYERBOX_TOPLEFT_Y + 2 + MELEE_PLAYERBOX_HEIGHT - 1;
	
	ClearRect(enemybox);
	ClearRect(playerbox);
	
	
	
	MeleeMakeStatusString(globals.globalmeleestruct.enemystatstring, &enemy->meleestruct, enemy->currentweapon, FALSE);
	MeleeMakeStatusString(globals.globalmeleestruct.playerstatstring, &player->meleestruct, player->currentweapon, TRUE);
	
	
	LGREEN;
	printwordwrapcoordinate(globals.globalmeleestruct.playerstatstring, playerboxdimensions, playerboxcoord);
	LRED;
	printwordwrapcoordinate(globals.globalmeleestruct.enemystatstring, enemyboxdimensions, enemyboxcoord);
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleePlayerChangeMove
//
// Description   - Called when the cycle move key is pressed, it changes the value of the cycled move
//
// Return Type   - void 
//
// Arguments     - PPLAYER player ( The pointer to the player )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void MeleePlayerChangeMove(PPLAYER player)
{
	
	
	do
	{
		globals.globalmeleestruct.currentcycleaction++;
		if (globals.globalmeleestruct.currentcycleaction > MELEE_MAX_CYCLE_ACTIONS){
			globals.globalmeleestruct.currentcycleaction = 1;
		}
	}while(!MeleeCanPlayerPerformMove(player, globals.globalmeleestruct.currentcycleaction));
	MeleePrintCycleMove(player, globals.globalmeleestruct.currentcycleaction);
}



//--------------------------------------------------------------------------------------------
// Function Name - MeleePrintStringInHelpBox
//
// Description   - Prints a given string in the help box at the bottom of the melee screen.
//
// Return Type   - void 
//
// Arguments     - char* string ( The string to print. )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void MeleePrintStringInHelpBox(char* string)
{
	COORD topleft;
	COORD dimensions;
	
	RECT recttoclear;
	
	topleft.X = MELEE_HELPBOX_TOPLEFT_X;
	topleft.Y = MELEE_HELPBOX_TOPLEFT_Y + 2;
	
	dimensions.X = MELEE_HELPBOX_WIDTH -1;
	dimensions.Y =  MELEE_HELPBOX_HEIGHT -1;
	
	recttoclear.left = MELEE_HELPBOX_TOPLEFT_X;
	recttoclear.top = MELEE_HELPBOX_TOPLEFT_Y + 2;
	recttoclear.right = MELEE_HELPBOX_WIDTH + MELEE_HELPBOX_TOPLEFT_X -1;
	recttoclear.bottom = MELEE_HELPBOX_HEIGHT + MELEE_HELPBOX_TOPLEFT_Y + 2 -1;
	
	ClearRect(recttoclear);
	RED;
	printwordwrapcoordinate(string, dimensions, topleft);
	Sleep(80);
	LRED;
	printwordwrapcoordinate(string, dimensions, topleft);
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleePrintCycleMove
//
// Description   - Prints the currently selected cycle move in the appropriate place.
//
// Return Type   - void 
//
// Arguments     - PPLAYER player ( The pointer to the player )
//               - int move ( The move currently selected )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void MeleePrintCycleMove(PPLAYER player, int move)
{
	//	int i;
	char moves[MELEE_MAX_CYCLE_ACTIONS][19] = {"Punch", "Kick", "Fire Weapon", "Slash", "Strike w/ Weapon", "Throw Weapon"};
	
	COORD moveboxcoord;
	moveboxcoord.X = MELEE_MOVEBOX_TOPLEFT_X;
	moveboxcoord.Y = MELEE_MOVEBOX_TOPLEFT_Y + 2;
	
	clearcells(MELEE_MOVEBOX_WIDTH -1, moveboxcoord);
	
	if (MeleeCanPlayerPerformMove(player, move))
	{
		RED;
		setcursor(moveboxcoord.X, moveboxcoord.Y);
		printf(moves[move - 1]);
		Sleep(80);
		LRED;
		setcursor(moveboxcoord.X, moveboxcoord.Y);
		printf(moves[move-1]);
	}
	else
	{
		GRAY;
		setcursor(moveboxcoord.X, moveboxcoord.Y);
		printf(moves[move-1]);
	}
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleeCanPlayerPerformMove
//
// Description   - Checks if the player can perform a specific move and returns TRUE or FALSE
//
// Return Type   - BOOL 
//
// Arguments     - PPLAYER player ( The pointer to the player )
//               - int move ( The move to check )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
BOOL MeleeCanPlayerPerformMove(PPLAYER player, int move)
{
	if (move == MELEE_ACTION_PUNCH || move == MELEE_ACTION_KICK){
		return TRUE;
	}
	else if (move == MELEE_ACTION_FIREWEAPON){
		if (player->currentweapon == NULL){
			return FALSE;
		}
		else if (!(player->currentweapon->attacktype & WEAP_ATT_TYPE_SHOOT)){
			return FALSE;
		}
		return TRUE;
		
	}
	else if (move == MELEE_ACTION_CUT){
		if (player->currentweapon == NULL){
			return FALSE;
		}
		else if (!(player->currentweapon->attacktype & WEAP_ATT_TYPE_CUT)){
			return FALSE;
		}
		return TRUE;
	}
	else if (move == MELEE_ACTION_HITWITHWEAPON){
		if (player->currentweapon == NULL){
			return FALSE;
		}
		if (!(player->currentweapon->attacktype & WEAP_ATT_TYPE_HIT)){
			return FALSE;
		}
		return TRUE;
	}
	else if (move == MELEE_ACTION_THROWWEAPON){
		if (player->currentweapon == NULL){
			return FALSE;
		}
		if (!(player->currentweapon->attacktype & WEAP_ATT_TYPE_THROW)){
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleePrintDistance
//
// Description   - Prints the melee distance in the appropriate position.
//
// Return Type   - void 
//
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void MeleePrintDistance()
{
	COORD drawcoords = {0,0};
	char distancestring[80];
	clearcells(getconsize().X, drawcoords);
	sprintf(distancestring, "Distance : %2.2dft", globals.globalmeleestruct.distance);
	drawcoords.X = retrieveTextCenterH(distancestring);
	setcursor(drawcoords.X, drawcoords.Y);
	LGREEN;
	printf(distancestring);
}


//--------------------------------------------------------------------------------------------
// Function Name - MeleeModifyPlayerOffensiveAction
//
// Description   - Will change the action to the current cycle action if an attack.
//
// Return Type   - void 
//
// Arguments     - PPLAYER player ( The pointer to the player )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void MeleeModifyPlayerOffensiveAction(PPLAYER player)
{
	if (player->meleestruct.lastaction == MELEE_ACTION_ATTACK){
		player->meleestruct.lastaction = globals.globalmeleestruct.currentcycleaction;
	}
	
	else{
		return;
	}
}

void MeleePlayRandomHitSoundSync()
{
	int sound = random(1,5);
	
	if (sound == 1)
	{
		playSYNCsound(MELEE_SOUND_HIT1);
		return;
	}
	if (sound == 2)
	{
		playSYNCsound(MELEE_SOUND_HIT2);
		return;
	}
	if (sound == 3)
	{
		playSYNCsound(MELEE_SOUND_HIT3);
		return;
	}
	if (sound == 4)
	{
		playSYNCsound(MELEE_SOUND_HIT4);
		return;
	}
	if (sound == 5)
	{
		playSYNCsound(MELEE_SOUND_HIT5);
		return;
	}
}

void MeleePlayRandomHitSoundAsync()
{
	int sound = random(1,5);
	
	if (sound == 1)
	{
		playMIXsound(MELEE_SOUND_HIT1);
		return;
	}
	if (sound == 2)
	{
		playMIXsound(MELEE_SOUND_HIT2);
		return;
	}
	if (sound == 3)
	{
		playMIXsound(MELEE_SOUND_HIT3);
		return;
	}
	if (sound == 4)
	{
		playMIXsound(MELEE_SOUND_HIT4);
		return;
	}
	if (sound == 5)
	{
		playMIXsound(MELEE_SOUND_HIT5);
		return;
	}
}

void MeleeSpecialHitDamageCorrection(PPLAYER player, PENEMY enemy, int damage, BOOL playerattack)
{
/*
#define BODY_HEAD		8
#define BODY_LSHOULDER	9
#define BODY_RSHOULDER	7
#define BODY_TORSO		5
#define BODY_RSIDE		4
#define BODY_LSIDE		6
#define BODY_ABDOMEN	2
#define BODY_LLEG		3
#define BODY_RLEG		1
	*/
	
	
	bool dropweapon = false;
	bool smashkneecap = false;
	bool headshot = false;
	MELEESTRUCT* attackermelee;
	MELEESTRUCT* defendermelee;
	PSYCHSKILLSTRUCT* attackerpsych;
	PSYCHSKILLSTRUCT* defenderpsych;
	
	if(playerattack)
	{
		attackerpsych = &player->psychskill;
		defenderpsych =&enemy->psychskill;
		attackermelee = &player->meleestruct;
		defendermelee = &enemy->meleestruct;
	}
	else
	{
		attackerpsych = &enemy->psychskill;
		defenderpsych =&player->psychskill;
		attackermelee = &enemy->meleestruct;
		defendermelee = &player->meleestruct;
	}
	
	
	if (attackermelee->lastaction == MELEE_ACTION_PUNCH) //PUNCHING
	{
		switch(attackermelee->lasttarget)
		{
		case BODY_HEAD:
			damage = (int)((double) damage * MELEE_HEAD_DAMAGE_SCALAR);
			headshot = true;
			break;
		case BODY_ABDOMEN:
		case BODY_RSIDE:
		case BODY_LSIDE:
		case BODY_TORSO:
			break;
			
		case BODY_LLEG:
		case BODY_RLEG:
			damage = (int)((double) damage * MELEE_LEG_DAMAGE_SCALAR);
			if (RandomPercentage(MELEE_PUNCH_LEG_SPECIAL_CHANCE)){ //IF WE DID A SPECIAL CHANCE HIT
				smashkneecap = true;
			}
			break;
			
			
		case BODY_LSHOULDER:
		case BODY_RSHOULDER:
			damage = (int)((double) damage * MELEE_SHOULDER_DAMAGE_SCALAR);
			if (RandomPercentage(MELEE_PUNCH_SHOULDER_SPECIAL_CHANCE)){ //IF WE DID A SPECIAL CHANCE HIT
				dropweapon = true;
			}
			break;
			
		}
	}
	else if (attackermelee->lastaction == MELEE_ACTION_KICK) //KICKING
	{
		switch(attackermelee->lasttarget)
		{
		case BODY_HEAD:
			damage = (int)((double) damage * MELEE_HEAD_DAMAGE_SCALAR);
			headshot = true;
			break;
		case BODY_ABDOMEN:
		case BODY_RSIDE:
		case BODY_LSIDE:
		case BODY_TORSO:
			break; //THESE AREN'T MODIFIED
		case BODY_LLEG:
		case BODY_RLEG:
			damage = (int)((double) damage * MELEE_LEG_DAMAGE_SCALAR);
			if (RandomPercentage(MELEE_KICK_LEG_SPECIAL_CHANCE)){ //IF WE DID A SPECIAL CHANCE HIT
				smashkneecap = true;
			}
			break;
			
		case BODY_LSHOULDER:
		case BODY_RSHOULDER:
			damage = (int)((double) damage * MELEE_SHOULDER_DAMAGE_SCALAR);
			if (RandomPercentage(MELEE_KICK_SHOULDER_SPECIAL_CHANCE)){ //IF WE DID A SPECIAL CHANCE HIT
				dropweapon = true;
			}
			break;
		}
	}
	
	else if (attackermelee->lastaction == MELEE_ACTION_HITWITHWEAPON)
	{
		switch(attackermelee->lasttarget)
		{
		case BODY_HEAD:
			damage = (int)((double) damage * MELEE_HEAD_DAMAGE_SCALAR);
			headshot = true;
			break;
		case BODY_ABDOMEN:
		case BODY_RSIDE:
		case BODY_LSIDE:
		case BODY_TORSO:
			break; //THESE AREN'T MODIFIED
		case BODY_LLEG:
		case BODY_RLEG:
			damage = (int)((double) damage * MELEE_LEG_DAMAGE_SCALAR);
			if (RandomPercentage(MELEE_KICK_LEG_SPECIAL_CHANCE)){ //IF WE DID A SPECIAL CHANCE HIT
				smashkneecap = true;
			}
			break;
			
		case BODY_LSHOULDER:
		case BODY_RSHOULDER:
			damage = (int)((double) damage * MELEE_SHOULDER_DAMAGE_SCALAR);
			if (RandomPercentage(MELEE_KICK_SHOULDER_SPECIAL_CHANCE)){ //IF WE DID A SPECIAL CHANCE HIT
				dropweapon = true;
			}
			
			break;
		}
	}
	
	
	
	if(playerattack)
	{
		if(smashkneecap)
		{
			playMIXsound(MELEE_SOUND_FALLDOWN);
			MeleePrintActionStringHold("You Smash His Kneecap And He Sinks To The Ground.");
			enemy->meleestruct.posture = MELEE_POSTURE_KNEELING;
		}
		if(dropweapon)
			if(enemy->currentweapon != NULL){
				playWeaponSound(enemy->currentweapon, WEAP_SOUND_DROP, SOUND_MODE_MIX);
				MeleePrintActionStringHold("Your Strike Sends His Weapon Flying To The Ground!");
				MeleeEnemyDropWeapon(enemy);
			}
			if (headshot)
			{
		//		MeleePrintActionStringHold("Your Strike Causes Severe Cranial Damage!");
			}
	}
	
	
	else //ENEMY ATTACKS
	{
		if(smashkneecap)
		{
			playMIXsound(MELEE_SOUND_FALLDOWN);
			MeleePrintActionStringHold("He Smashes Your Kneecap And You Sink To The Ground.");
			player->meleestruct.posture = MELEE_POSTURE_KNEELING;
		}
		if(dropweapon)
			if(enemy->currentweapon != NULL){
				playWeaponSound(enemy->currentweapon, WEAP_SOUND_DROP, SOUND_MODE_MIX);
				MeleePrintActionStringHold("Your Weapon Flies Out Of Your Hands As You Recieve The Blow!");
				MeleePlayerDropWeapon(player);
			}
			if (headshot)
			{
//			MeleePrintActionStringHold("Your Head Is Severely Damaged!");
			}
	}
}

void MeleeConvertPlayerDamageAtEndOfCombat(PPLAYER player)
{
	int gunshotpercentage = 0;
	int cutpercentage = 0;
	int i;
	double finalconversionpercentage;
	
	for (i = 0; i < MAX_BODY_PARTS; i++){
		if (player->meleestruct.wounds[i] & WOUND_CUT){
			cutpercentage += MELEE_CUT_CONVERSION_PERCENTAGE;
		}
		if (player->meleestruct.wounds[i] & WOUND_SHOT){
			gunshotpercentage += MELEE_SHOT_CONVERSION_PERCENTAGE;
		}
	}
	
	if (cutpercentage + gunshotpercentage > 1)
	{
		finalconversionpercentage = (double)(gunshotpercentage + cutpercentage);
		finalconversionpercentage /= 100; //THIS IS THE MODIFIER TO MULTIPLY BY.
		player->health = (int)((double)player->health * finalconversionpercentage);
		DSAPlayerPrompt("You took some nasty scrapes in that last fight. You tend your wounds as best you can, but you know that they're serious enough to affect your overall performance.", PROMPTANSWER_ANYKEY);
	}
	
	else{
		DSAPlayerPrompt("You don't seem to have any wounds from this last fight that require immediate attention. You consider yourself lucky.", PROMPTANSWER_ANYKEY);
	}
	
	
	
}


void MeleeAIDropHonor()
{
	globals.globalmeleestruct.enemyhonor -= rand()%10 + 5;
	if (globals.globalmeleestruct.enemyhonor < 1){
		globals.globalmeleestruct.enemyhonor = 1;
	}
}

void MeleeAIRaiseHonor()
{
	globals.globalmeleestruct.enemyhonor += rand()%10 + 5;
	if (globals.globalmeleestruct.enemyhonor > 100){
		globals.globalmeleestruct.enemyhonor = 100;
	}
}

void MeleeAIRaiseAnger()
{
	globals.globalmeleestruct.enemyanger += rand()%10 + 5;
	if (globals.globalmeleestruct.enemyanger > 100){
		globals.globalmeleestruct.enemyanger = 100;
	}
}

void MeleeAIDropAnger()
{
	globals.globalmeleestruct.enemyanger -= rand()%10 + 5;
	if (globals.globalmeleestruct.enemyanger < 1){
		globals.globalmeleestruct.enemyanger = 1;
	}
}

void MeleePlayRandomMissSoundAsync()
{
	switch (rand()%3){
	case 0:
		playMIXsound(MELEE_SOUND_MISS1);
		break;
	case 1:
		playMIXsound(MELEE_SOUND_MISS2);
		break;
	case 2:
		playMIXsound(MELEE_SOUND_MISS3);
		break;
	}
}

void MeleePlayRandomMissSoundSync()
{
	switch (rand()%3){
	case 0:
		playSYNCsound(MELEE_SOUND_MISS1);
		break;
	case 1:
		playSYNCsound(MELEE_SOUND_MISS2);
		break;
	case 2:
		playSYNCsound(MELEE_SOUND_MISS3);
		break;
	}
}

int MeleeAIGetCurrentPriority(PPLAYER player, PENEMY enemy)//THIS RETRIEVES THE CURRENT PRIORITY STATUS FOR AI REFERENCE.
{
	
	if (player->meleestruct.posture > MELEE_POSTURE_STANDING){
		return 1;
	}
	else if(player->currentweapon != NULL && (player->currentweapon->attacktype & WEAP_ATT_TYPE_SHOOT || player->currentweapon->attacktype & WEAP_ATT_TYPE_DRUG)){
		return 2;
	}

	else if (enemy->meleestruct.stamina < 15){
		return 3;
	}
	else if (player->currentweapon != NULL && (player->currentweapon->attacktype & WEAP_ATT_TYPE_HIT || player->currentweapon->attacktype & WEAP_ATT_TYPE_CUT)){
		return 4;
	}
	else if (globals.globalmeleestruct.distance >= 10){
		return 5;
	}
	else{
		return 6;
	}
}

void MeleePrintDebugNumber(char* name, int number)
{
	char buf[200];
	sprintf(buf, "%s IS %d.", name, number);
	MeleePrintActionStringHold(buf);
}