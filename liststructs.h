#ifndef __liststructs_h
#define __liststructs_h

#include "xa.h"

typedef struct _playerammostruct{
	PLIST cliplist; //POINTER TO A NUMBER OF CLIPS
	PLIST roundtypes; //LIST OF DIFFERENT STRAY ROUND TYPES
}PLAYERAMMO, *PPLAYERAMMO;

typedef struct _playerhistory{
	//CHRONOSTRUCT startchronostruct; //THE CHRONOSTRUCT FOR THE TIME THE AGENT BEGAN HIS CAREER
	//CHRONOSTRUCT birthchronostruct; //THE CHRONOSTRUCT FOR WHEN THE AGENT WAS BORN. HELPS CALCULATE AGE, BIRTHDAYS, ETC.
	int missionsattempted;
	int missionsaccomplished;
	int timescaptured;
	int timesescaped;
	int timesrescued;
	double damageinmillions;
	int kills;
	float cumulativemissionrating;
//	PLIST history; //THE HISTORY OF MISSIONS
//HISTORYMATRIX hmatrix; //THE EARLY HISTORICAL DATA
}PLAYERHISTORY, *PPLAYERHISTORY;

typedef struct _playerstruct
{
//STANDARD DATA
char fname[20]; //FIRST NAME OF PLAYER
char lname[20]; //LAST NAME OF PLAYER
char codename[30]; //CODENAME OF PLAYER
int id; //BOGUS IDENTIFICATIONCODE
int status; //AGENT_ALIVE, AGENT_CYANIDED, AGENT_NOTOCHORDED, AGENT_RETIRED, AGENT_WHEREABOUTS_UNKNOWN
bool sealedflag; //IF ON, PLAYER DOES NOT EXIST.
PSYCHSKILLSTRUCT psychskill; //PSYCH-SKILL STRUCTURE
PLAYERHISTORY history; //HISTORY OF MISSIONS
int rank; //THE PLAYER'S RANK
MEDALS medals; //THE PLAYER'S MEDALS
int accesscode; //THE FOUR DIGIT ACCESS CODE
int monthscompleted; //NUMBER OF MONTHS COMPLETED
int numberofdaysincycle; //NUMBER OF DAYS LEFT IN MONTH
int missionsleftthiscycle; //MISSIONS LEFT TO COMPLETE THIS MONTH
bool currentlymobilizing; //DOES THE PLAYER HAVE A MISSION CURRENTLY?

//MISSION-SPECIFIC
PSARTUMISSION sartumission; //POINTER TO SARTU MISSION.
MISSION currentmissioninfo; //CURRENT MISSION INFORMATION
MELEESTRUCT meleestruct; //HAND-TO-HAND COMBAT INFO
DRUG drugs; //DRUG SYRINGE STRUCTURE
int health; //HEALTH OF PLAYER
int wounds[MAX_BODY_PARTS]; //THE WOUNDS THE PLAYER HAS
PWEAPON currentweapon; //CURRENT PLAYER WEAPON POINTER INTO THE LIST
PMAPCELL location; //POINTER TO CURRENT LOCATION OF PLAYER
PMAPCELL lastlocation; //PREVIOUS LOCATION
PLIST p_weapons; //POINTER TO WEAPONS LIST
PLAYERAMMO ammo; //AMMO LISTING
PLIST p_gizmos; //POINTER TO GIZMOS LIST
float weight;//CURRENT CARRIED WEIGHT
int flashturnsleft; //CAN BE -1 FOR NO FLASH.
int turndirection;
int keycardbitfield; //KEY CARDS CARRIED
POINT ZoomCenter;
POINT LocalCursor;
int CurrentActionMode;
}PLAYER, *PPLAYER;


typedef struct _enemystruct{
	int index; //THE INDEX OF THE ENEMY IN THE LIST
	ENEMYAI AIStruct; //THE ENEMY AI!
	BOOL primary; //THE PRIMARY TARGET?
	BOOL friendly; //WILL HE FOLLOW YOU, FRIENDLY?
	PLIST weaponlist; //WHICH WEAPONS DO THEY HOLD?
	PLAYERAMMO ammolist; //THE AMMO THEY CURRENTLY HAVE
	PWEAPON currentweapon; //WHICH WEAPON ARE THEY USING?
	PMAPCELL location; //WHERE ARE THEY IN THE MAP?
	PSYCHSKILLSTRUCT psychskill; //PSYCH-SKILL STRUCTURE
	MELEESTRUCT meleestruct; //HAND-TO-HAND COMBAT INFO
	int keycardbitfield; //KEY CARDS CARRIED;
	int turndirection;
	int status; //-1 = NOT AN ENEMY 0 = ALIVE 1 = UNCONSCIOUS 2 = DEAD 3 = INCINERATED (HA HAAAA!) 4 = TRAPPED (LIKE A CAPTURED AGENT DOOD)
	int health; //THE HEALTH OF THE ENEMY
	int wounds[MAX_BODY_PARTS]; //THE WOUNDS THE ENEMY HAS
	int flashturnsleft; //CAN BE -1 FOR NO FLASH.
}ENEMY, *PENEMY;

typedef struct _maparmorydata{
	int type;
	POINT location;
	bool destroyed;
	PLIST weaponry;
	PLAYERAMMO ammo;
}MAPARMORY, *PMAPARMORY;

typedef struct _weaponsammostruct
{
	PLIST weaponlist;
	PLIST ammolist;
}WEAPONSAMMOSTRUCT, *PWEAPONSAMMOSTRUCT;

typedef struct _globalmapstruct{
	MAP map; //THE MAP DATA
	PLIST weapons; //THE WEAPONS LOCATED IN THE MAP
	PENEMY enemies; //THE ENEMIES LOCATED IN THE MAP - DYNAMIC ARRAY
	int numenemies;
	PMAPARMORY armories; //THE ARMORIES LOCATED IN THE MAP - DYNAMIC ARRAY
	int numarmories;
	PLIST vehicles; //THE VEHICLES LOCATED IN THE MAP
	PLIST gizmos; //THE GIZMOS LOCATED IN THE MAP
	PLIST clips; //THE CLIPS LOCATED IN THE MAP
	PLIST strayrounds; //THE STRAY ROUNDS LOCATED IN THE MAP
	PLIST bug; //BUGS ON THE MAP
	PLIST sbk; //SBK'S ON THE MAP
	PLIST doors; //DOORS ON THE MAP
	PLIST scriptedevents; //SCRIPTED EVENTS AT CERTAIN TIMES
	PLIST environmental; //SMOKE, FIRE, GAS.
	MISSIONCHRONO totaltime; //TOTAL MISSION TIME
}GMAPSTRUCT, *PGMAPSTRUCT;

typedef struct _options{
	BOOL musicon;
}OPTIONS, *POPTIONS;

typedef struct _globalmeleecombatstruct{
	MISSIONCHRONO timetoturn;
	BOOL playerturn;
	BOOL freeturn;
	BOOL combatcontinues;
	int enemyanger;
	int enemyhonor;
	char playerstatstring[200];
	char enemystatstring[200];
	char actionstring[100];
	int distance;
	int currentcycleaction; //WHICH CYCLE ACTION DOES THE PLAYER HAVE SELECTED?
	PWEAPON enemyweapontodraw;
	PLIST weaponslist;
}GLOBALMELEESTRUCT, *PGLOBALMELEESTRUCT;

typedef struct _sartudatastruct{
	PLIST missioncodesone;
	PLIST missioncodestwo;
	PLIST agentfirstnames;
	PLIST agentlastnames;
	PLIST agentcodenames;
	PLIST technologies;
	PLIST targetfirstnames;
	PLIST targetlastnames;
	PLIST targetextras;
	PLIST gogetemone;
	PLIST gogetemtwo;
	PLIST egogetemone;
	PLIST egogetemtwo;
}SARTUDATASTRUCT, *PSARTUDATASTRUCT;

typedef struct _globalstruct{
	PLIST graphicslist;
	MUSICLIST musiclist;
	WEAPONSAMMOSTRUCT globalweaponsammo; //THIS IS THE LIST OF ALL WEAPONS AND AMMO IN THE GAME
	PLAYER player;
	OPTIONS options;
	GMAPSTRUCT globalmapstruct;
	PLIST sartumissions;
	PVOID XaudioHandle;
	GLOBALMELEESTRUCT globalmeleestruct;
	COORD zeroed;
	BOOL musicthreadopen;
	SARTUDATASTRUCT sartudata;
	HANDLE musicthread;
	ULONG musicthreadid;
	BOOL musicloopstatus;
	XA_Message xamessage;
	BOOL musicmessagewaiting;
	int musicmessage;
	char* musicsong;
	DBOXPARAM dboxparams;
	PLIST armorygizmolist;
	PSYCHSKILLSTRUCT BackupPsychSkill;
}GLOBALSTRUCT, *PGLOBALSTRUCT;


/************************************************************
; EXTERNS HERE
************************************************************/

extern GLOBALSTRUCT globals;
#endif