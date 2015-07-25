#include "agencyinclude.h"
#include "guns.h"

void ClearEnemyStats(PENEMY enemy)
{
	memset(&enemy->psychskill, 0, sizeof(enemy->psychskill));
	
	enemy->psychskill.psych.agression = 1;
	enemy->psychskill.psych.calmness = 1;
	enemy->psychskill.psych.intelligence = 1;
	enemy->psychskill.psych.reflex = 1;
	enemy->psychskill.psych.willpower = 1;
	
	enemy->psychskill.skills.electronics = 2;
	enemy->psychskill.skills.mechanical = 2;
	enemy->psychskill.skills.meleecombatNOweapon = 2;
	enemy->psychskill.skills.meleecombatWITHweapon = 2;
	enemy->psychskill.skills.firearms = 2;
	enemy->psychskill.skills.snipingcombat = 2;
	enemy->psychskill.skills.stealthcombat = 2;
}

bool EnemyTakePlayerWeapon(PENEMY enemy, PPLAYER player, PWEAPON* weapon)
{
		POTYPE tmpObj;
	PPOTYPE lstObj;
	
	if(weapon == NULL || enemy == NULL || player == NULL || enemy->weaponlist == NULL || player->p_weapons == NULL){
		return false;
	}
	tmpObj.weapon.index = (*weapon)->index;
	lstObj = findObject(player->p_weapons, &tmpObj, CompareWeaponIndex);
	if (lstObj == NULL){
		return false;
	}
	int oldindex = lstObj->weapon.index;
	int newindex = lstObj->weapon.index = enemy->weaponlist->objcnt; //GIVE UNIQUE INDEX;
	addtoList(enemy->weaponlist, *lstObj, CompareWeaponIndex); //GIVE TO ENEMY
	lstObj->weapon.index = oldindex;
	deleteObject(player->p_weapons, lstObj); //ELIMINATE FROM PLAYER STRUCT
	tmpObj.weapon.index = newindex;
	lstObj = findObject(enemy->weaponlist, &tmpObj, CompareWeaponIndex);
	*weapon = &lstObj->weapon;
	return true;
}

bool PlayerTakeEnemyWeapon(PENEMY enemy, PPLAYER player, PWEAPON* weapon)
{
	POTYPE tmpObj;
	PPOTYPE lstObj;
	
	if(weapon == NULL || enemy == NULL || player == NULL || enemy->weaponlist == NULL || player->p_weapons == NULL){
		return false;
	}
	tmpObj.weapon.index = (*weapon)->index;
	lstObj = findObject(enemy->weaponlist, &tmpObj, CompareWeaponIndex);
	if (lstObj == NULL){
		return false;
	}
	int oldindex = lstObj->weapon.index;
	int newindex = lstObj->weapon.index = player->p_weapons->objcnt; //GIVE UNIQUE INDEX;
	addtoList(player->p_weapons, *lstObj, CompareWeaponIndex); //GIVE TO PLAYER
	lstObj->weapon.index = oldindex;
	deleteObject(enemy->weaponlist, lstObj); //ELIMINATE FROM ENEMY STRUCT
	tmpObj.weapon.index = newindex;
	lstObj = findObject(player->p_weapons, &tmpObj, CompareWeaponIndex);
	*weapon = &lstObj->weapon;
	return true;
}

bool GeneratePrimaryObjectiveEnemy(bool primaryfriendly, bool primarystatus, PSARTUMISSION missioninfo, PMAP map)
{
/*
	//THIS GUY ALWAYS GOES IN THE ZERO SLOT.
	//TO MAKE THINGS EASIER, POINT AT HIM.
	PENEMY pte = &(globals.globalmapstruct.enemies[0]);
	pte->health = 100;
	pte->index = 0;
	pte->flashturnsleft = -1;
	pte->friendly = primaryfriendly;
	pte->keycardbitfield = rand()%(1 << MAX_DOOR_KEYCARDS); //I'M SO SLICK.
	pte->wounds = 0;
	pte->primary = true;
	pte->AIStruct.ConfidencePercentage = 0;
	pte->AIStruct.CurrentAlertLevel = 0;
	pte->AIStruct.TertiaryNumber = 10; //HE CAN GO ABOUT 10 IN ANY DIRECTION.
	pte->AIStruct.StandingOrder = AI_DEFAULT_BEHAVIOR_PRIMARY;
	
	//NOW TO DECIDE WHERE HE NEEDS TO BE.
	//IF PRIMARYFRIENDLY, THEN...
	if(primaryfriendly)
	{
		if(missioninfo->missiontype == MISSION_TYPE_ESCAPE)
		{
			if(missioninfo->missionspecificone){
				pte->status = 2; //DIED TRYING TO RESCUE YOU.
				pte->health = 0;
			}
			else{
				pte->status = 0;
				pte->health = rand()%50 + 50;
			}
			
			//GIVE HIM WEAPONS?
			int NumWeaps = rand()%4;
			int extramags;
			int weapid;
			for(int i = 0; i < NumWeaps; i++)
			{
				extramags = rand()%3;
				weapid = GetWeightedRandomWeaponID(globals.globalweaponsammo.weaponlist);
				GiveLoadedTrainingWeapon(pte->weaponlist, pte->ammolist, weapid, 0);
				for(int k = 0; k < extramags; k++){
				GiveRandomExtraAmmo(pte->ammolist, weapid);
				}
				pte->currentweapon = &(pte->weaponlist->head->object.weapon); //HE GETS THE FIRST WEAPON IN THE LIST.
			}
		}
	}

	//ELSE HE GETS NOOOOOOOOOTHING!!!!!!!!

	//WE MUST DECIDE WHERE TO PLACE HIM.
	PMAPCELL cl = GetRandomBuildingFloorCell(map);
	if(!cl)
		return false;

	pte->location = cl;
	pte->AIStruct.DefaultPosition.x = cl->xcoord;
	pte->AIStruct.DefaultPosition.y = cl->ycoord;
	pte->turndirection = rand()%4;

	GenerateRandomPsychSkill(&(pte->psychskill));

	//AND THAT'S IT.
	*/
	return true;
}

void GenerateRandomPsychSkill(PPSYCHSKILLSTRUCT ps)
{
}

PMAPCELL GetRandomBuildingFloorCell(PMAP map)
{
	PMAPCELL out;
	POINT pt;
	int iterations = 0;
	while(iterations < 50000)
	{
		pt.x = random(2, map->width - 1);
		pt.y = random(2, map->height - 1);
		out = FindMapCellByCoordinate(map, pt);
		if(out)
		{
			if(out->type == OBJECT_SYM_FLOOR && !DoesCellBlockMovement(out))
			{
				return out;
			}
		}
		iterations++;
	}
	return NULL;
}

bool GenerateEnemy(PMAP map, PSARTUMISSION missioninfo, int CurrentEnemyCounter)
{
	//NOW, WE GENERATE A STANDARD ENEMY.
	//FIRST, WE MUST DECIDE...
	return false;
}

void AssignRandomKeyCards(PSARTUMISSION missioninfo)
{
	/*
	#define DOOR_KEYCARD_TAU		1
#define DOOR_KEYCARD_THETA		2
#define DOOR_KEYCARD_OMICRON	4
#define DOOR_KEYCARD_EPSILON	8
#define DOOR_KEYCARD_PHI		16
#define DOOR_KEYCARD_RHO		32
	*/
	//THE HIGHER THE SECURITY, THE LESS DUPLICATES THERE ARE.
	int duplicates = (MAX_KEYCARD_DUPLICATES * missioninfo->compoundsecuritypercentage) / 100;

	//WE WILL NOT DO ANY CHECKING TO ENSURE COMPLETELY UNIQUE DUPLICATE HANDOUTS, THERE MAY BE OVERLAP.
	//PICK A RANDOM GUY, NOT NUMBER ZERO, NEVER HIM.
	int k,i;
	int enemyid;
	for(i = 0; i <= duplicates; i++)
	{
		for(k = 0; k < MAX_DOOR_KEYCARDS; k++)
		{
			enemyid = random(1, missioninfo->numberguards);
			GiveKeyCard(&(globals.globalmapstruct.enemies[enemyid].keycardbitfield), 1 << k);
		}
	}
	
}

void GiveKeyCard(int* keycardfield, int keycardvalue)
{
	(*keycardfield) |= keycardvalue;
}

void RemoveKeyCard(int* keycardfield, int keycardvalue)
{
	(*keycardfield)  &= (~keycardvalue);
}

bool HasKeyCard(int* keycardfield, int keycardtolookfor)
{
	return (*keycardfield) & keycardtolookfor;
}

void PrintKeyCardsOnHand(int* keycardfield, char* buf, char delineator)
{
	char tb[500];
	char names[6][100] = {
		"Tau",
			"Theta",
			"Omicron",
			"Epsilon",
			"Phi",
			"Rho",
	};
	int num = 0;
	for(; num < MAX_DOOR_KEYCARDS; num++)
	{
		if((*keycardfield) & 1 << num){
			sprintf(tb, "%s%c", names[num], delineator);
			strcat(buf, tb);
		}
	}
}
