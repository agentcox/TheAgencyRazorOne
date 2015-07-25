#ifndef __agencystructs_h
#define __agencystructs_h

typedef WORD COLOR;

typedef struct _promptresponse{
	int typeofresponse;
	char textresponse [MAX_RESPONSE_CHARS];
	BOOL yesnoresponse;
	int numberresponse;
}PROMPTRESPONSE, *PPROMPTRESPONSE;

typedef struct _chronostruct{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
}CHRONOSTRUCT, *PCHRONOSTRUCT;

typedef struct _musiclist
{
	char songs[MAXSONGS + 1][255];
}MUSICLIST, *PMUSICLIST;

typedef struct _missionchrono{
	BOOL countdown; //IS IT COUNTING DOWN OR UP?
	int hours;
	int minutes;
	int seconds;
	int split;
}MISSIONCHRONO, *PMISSIONCHRONO;

typedef struct _listcontroloutput{
	int key;
	int id;
}LISTCONTROLOUTPUT, *PLISTCONTROLOUTPUT;

typedef struct _dblistitem{
	char* string;
	int id;
}DBLISTITEM, *PDBLISTITEM;

typedef struct _scriptedevent{
	MISSIONCHRONO TriggerTime;
	bool HasBeenTriggered;
	int Type;
	int Id;
	int ExtraDataOne;
	int ExtraDataTwo;
	void* Context;
}SCRIPTEDEVENT, *PSCRIPTEDEVENT;

typedef struct _dboxparam{
		char RelativeSize;
		short MaxResponseChars;
		COLOR DialogBoxBGColor;
		COLOR DialogBoxBorderColor;
		char DialogBoxBorderType;
		bool DrawBGUnderBorder;
		bool CenteredText;
		COLOR TopStringBGColor;
		COLOR TopStringFGColor;
		COLOR BottomStringBGColor;
		COLOR BottomStringFGColor;
		COLOR CenterStringFGColor;
		COLOR UnselectedObjectTextColor;
		COLOR SelectedObjectTextColor;
		COLOR SelectionBlockColor;
		COLOR TextCursorColor;
		COLOR InputtedTextColor;
}DBOXPARAM, *PDBOXPARAM;


typedef struct _skills{
		int firearms;
		int stealthcombat;
		int snipingcombat;
		int meleecombatNOweapon; 
		int meleecombatWITHweapon;
		int electronics;
		int mechanical;
}SKILLSTRUCT, *PSKILLSTRUCT;


typedef struct _meleestruct{
	int stamina;
	int posture;
	int wounds[MAX_BODY_PARTS];
	int lastaction;
	int lasttarget;
}MELEESTRUCT, *PMELEESTRUCT;

typedef  struct specializations	{
		int	handweapons;
		int handguns;
		int submachineguns;
		int rifles;
		int shotguns;
		int specialweapons;
		int explosives;
		int electronics;
		int mechanical;
}SPECIALIZATIONSTRUCT, *PSPECIALIZATIONSTRUCT;

typedef struct _psychstruct{
	int agression;
	int calmness;
	int intelligence;
	int reflex;
	int willpower;
}PSYCHSTRUCT, *PPSYCHSTRUCT;

typedef struct _environstruct{
	int type; //SMOKE, TOXIN, FIRE
	int id;
	int currentradius;
	int turnsperexpansion; //CAN BE -1
	int currentturnstoexpansion;
	int radiusperexpansion;
	int totalturnsleft; //CANNOT BE -1.
	POINT origin; //WHERE THE EFFECT ORIGINATES.
}ENVIRONSTRUCT, *PENVIRONSTRUCT;


typedef struct _psychskillstruct{

	SKILLSTRUCT skills;
	SPECIALIZATIONSTRUCT specializations;
	PSYCHSTRUCT psych;

}PSYCHSKILLSTRUCT, *PPSYCHSKILLSTRUCT;


typedef struct _bug{
	COORD location;
	int radiochannel;
	BOOL spikemike;
}BUG, *PBUG;

typedef struct _sbk{
	COORD location;
	int radiochannel;
}SBK, *PSBK;


typedef struct _device{
	int devicetype; //THE GIZMO ID, USED TO REFERENCE FUNCTIONALITY
	BOOL active; //IS IT ON?
	int condition; //CONDITION OF THE OBJECT
	int bodypart; //BODY PART ATTACHED TO
	int numberleft; //NUMBER LEFT (FOR BUGS, ETC)
}DEVICE, *PDEVICE;

typedef struct _grenade{
		int grenadetype; //THE GRENADE ID, USED TO REFERENCE FUNCTIONALITY
		int primertime; //-1 IF THE GRENADE IS NOT ACTIVE.
}GRENADE, *PGRENADE;

typedef struct _gizmo{
	int index; //THE INDEX, USED TO FIND IN LIST
	void* Context;
	int AdditionalNumber;
	bool AdditionalBool;
	POINT AdditionalPoint;
	POINT WorldPoint; //IF IN THE WORLD, WHERE IS IT?
	BOOL isgrenade; //IS THIS GIZMO ACTUALLY A GRENADE?
	union {
	DEVICE device;
	GRENADE grenade;
	}unit;

}GIZMO, *PGIZMO;

typedef struct _armorygizmo{
	int id; //THE INDEX ID, NOT USED FOR TYPE.
	int type; //WHAT TO ENTER FOR THE TYPE IN THE PLAYER GIZMO
	BOOL isgrenade;
	char longname[35];
	char shortname[20];
	char* desc;
	float weight;
	int frequency;
	bool canhavemultiple;
	bool isavailable;
}AGIZMO, *PAGIZMO;



typedef struct _sniperesult{
	bool shotfired;
	int hitcode;
	bool critical;
}SNIPERESULT, *PSNIPERESULT;
typedef struct _graphic{
	int id;
	char* graphic;
	int length;
	COORD size;
}GRAPHIC, *PGRAPHIC;

typedef struct _explosive{
	int id; //THE NUMERICAL EXPLOSIVE ID...INDEXING AND AMMO LINKING - LINKED TO type IN AMMOSTRUCT
	BOOL dsaammo; //0 = NOT DSA AMMO, NON-ZERO = DSA AMMO
	char shortname[20]; //THE SHORT NAME OF THE SHELL TYPE.
	char longname[60]; //THE EXTENDED NAME OF THE SHELL TYPE.
	char *desc; //THE DESCRIPTION OF THE SHELL.
	int frequency; //OUT OF 100, THE FREQUENCY WITH WHICH IT'S AVAILABLE
	int delay; //DELAY AFTER FIRING UNTIL DEVICE DETONATES - 0 INDICATES EXPLODES ON IMPACT
	int range; //RANGE IN SQUARES OF ROUND - 0 INDICATES ROCKET WEAPON - FLIES UNTIL DETONATION
	int initialblastradius; //BLAST RADIUS (NOT DIAMETER) IN SQUARES OF THE WEAPON.
	int initialdamagetype; //CONCUSSION , PUNCTURE, FLASH
	int initialdamagestrength; //HOW HEAVY IS THE DAMAGE 0 = NONE, FOR SMOKE, 100 = HUUUUUUUGE!!!
	int lastingdamagetype; //GAS, SMOKE, FIRE
	int lastingdamageradius; //RADIUS (NOT DIAMETER) IN SQUARES OF THE WEAPON'S LASTING EFFECTS.
	int lastingdamagetime; //TIME THAT THE LASTING DAMAGE STICKS AROUND
	char* impactsound; //THE STRING THAT POINTS AT THE SOUND FILE FOR IMPACT
	BOOL ammoavailable; //IS IT AVAILABLE?
	int roundsavailable; //HOW MANY AVAILABLE?
}EXPLOSIVE, *PEXPLOSIVE;

typedef struct _bullet{
	int id; //THE NUMERICAL BULLET ID...INDEXING AND AMMO LINKING - LINKED TO type IN AMMOSTRUCT
		BOOL dsaammo; //0 = NOT DSA AMMO, NON-ZERO = DSA AMMO
char shortname[20]; //THE SHORT NAME OF THE BULLET TYPE.
	char longname[60]; //THE EXTENDED NAME OF THE BULLET TYPE.
	char *desc; //THE DESCRIPTION OF THE BULLET
	int frequency; //OUT OF 100, THE FREQUENCY WITH WHICH IT'S AVAILABLE
	int ap; //OUT OF 100, THE ARMOR-PIERCING CAPABILITY
	int energy;  //OUT OF 100, THE ENERGY TRANSMISSION CAPABILITY
	int damage; //OUT OF 100, THE INSTANT SHOCK DAMAGE TRANSMISSION CAPABILITY
	int latentdamage; //OUT OF 100, THE LATENT DAMAGE CAPABILITY (OPEN WOUNDS, TOXIN, ETC...)
	char* impactsound; //THE STRING THAT POINTS AT THE SOUND FILE FOR IMPACT
	BOOL ammoavailable; //IS IT AVAILABLE?
	int roundsavailable; //HOW MANY AVAILABLE?
}BULLET, *PBULLET;

typedef struct _shelldata{
	int id; //THE NUMERICAL SHELL ID...INDEXING AND AMMO LINKING - LINKED TO type IN AMMOSTRUCT
	BOOL dsaammo; //0 = NOT DSA AMMO, NON-ZERO = DSA AMMO
	char shortname[20]; //THE SHORT NAME OF THE SHELL TYPE.
	char longname[MAX_NAME_CHARS]; //THE EXTENDED NAME OF THE SHELL TYPE.
	char *desc; //THE DESCRIPTION OF THE SHELL.
	int frequency; //OUT OF 100, THE FREQUENCY WITH WHICH IT'S AVAILABLE
	int spread; //OUT OF 100, THE PELLET SPREAD FOR THE SHOTGUN, 0 = SLUG
	int ap; //OUT OF 100, THE ARMOR-PIERCING CAPABILITY
	int energy;  //OUT OF 100, THE ENERGY TRANSMISSION CAPABILITY - ABOVE 50 CAN BLOW LOCKS APART
	int damage; //OUT OF 100, THE INSTANT SHOCK DAMAGE TRANSMISSION CAPABILITY
	char* impactsound; //THE STRING THAT POINTS AT THE SOUND FILE FOR IMPACT
	BOOL ammoavailable; //IS THE AMMO AVAILABLE?
	int roundsavailable; //HOW MANY AVAILABLE?
}SHELL, *PSHELL;

typedef struct _round{
	int mainammotype;
	int subammotype;
}ROUND, *PROUND;

typedef struct _strayrounds{
	POINT WorldPoint;
	int id;
	int mainammotype;
	int subammotype;
	int numberofroundsleft;
}STRAYROUNDS, *PSTRAYROUNDS;

typedef union _specialammo{
		PEXPLOSIVE explosivedata; //POINTER TO ALLOWED EXPLOSIVE ROUNDS
		PBULLET bulletdata; //POINTER TO ALLOWED BULLET ROUNDS
		PSHELL shelldata; //POINTER TO ALLOWED SHELL ROUNDS
}SAMMO, *PSAMMO;

typedef struct _clip{
	POINT WorldPoint;
	int id;
	int weaponid; //WEAPON ID THAT THE CLIP IS USED WITH
	int currentrounds; //NUMBER OF ROUNDS LEFT IN CLIP - INITIALLY SET TO THE MAX THE GUN CAN HOLD
	ROUND clipround; //THE CURRENT TYPE OF THE ROUNDS IN THE CLIP (ONE TYPE PER CLIP)
}CLIP, *PCLIP;

typedef struct _clipreciever{
	BOOL  isclipinserted;
	CLIP clipinserted; //CLIP INSERTED INTO THE WEAPON
}CLIPREC, *PCLIPREC;

typedef struct _magazine{
	int totalroundschambered; //NUMBER OF ROUNDS CHAMBERED
	PROUND chamberedrounds; //POINTER TO THE CURRENT ROUNDS IN THE MAGAZINE
}MAGAZINE, *PMAGAZINE;

typedef struct _cylinder{
	int currentposition; //THE POSITION OF THE CYLINDER (MAX IS THE MAXAMMO OF THE WEAPON)
	PROUND chamberedrounds; //POINTER TO THE CURRENT ROUNDS IN THE MAGAZINE
	int* statusindicators; //POINTER TO STATUS INDICATORS FOR EACH CHAMBER. (EMPTY, FULL, FULL-ALREADYFIRED)
}CYLINDER, *PCYLINDER;

typedef union _loadingprocedure{
	CLIPREC clip; //POINTER TO A CLIP RECIEVER
	MAGAZINE magazine; //POINTER TO A MAGAZINE
	CYLINDER cylinder; //POINTER TO A CYLINDER
}LOADTYPE, *PLOADTYPE;


typedef struct _ammostruct{ //The general ammunition structure - allows access to other special rounds.
	int id; //THE NUMERICAL AMMO ID...FOR INDEXING AND WEAPON LINKING - LINKED TO ammotype IN WEAPSTRUCT
	int type; //THE AMMO TYPE - DETERMINES WHICH FIELD IN THE damagedata UNION TO USE
	char name[MAX_NAME_CHARS]; //THE NAME OF THE BASIC AMMO TYPE
	int maxammotypes; //THE MAXIMUM NUMBER OF SPECIAL AMMO TYPES FOR THIS AMMUNITION
	SAMMO specialammo;
}AMMO, *PAMMO;

typedef struct _ammoresult{
	bool isClip;
	PCLIP Clip;
	PSTRAYROUNDS Rounds;
}AMMORESULT, *PAMMORESULT;

typedef struct _modifiers{
	BOOL lasersight; //LASERSIGHT ATTACHED/ALLOWED
	BOOL flashsuppressor;  //FLASH SUPPRESSOR ATTACHED/ALLOWED
	BOOL silencer;  //SILENCER ATTACHED/ALLOWED
}MODIFIER, *PMODIFIER;

typedef struct _weaponperformance{
int baseaccuracy_percent; //BASE ACCURACY OF WEAPON IN PERCENT
int roundsperturn; //ROUNDS PER TURN OF WEAPON
int flashlevel; //FLASH LEVEL % OF WEAPON
int soundlevel; //SOUND LEVEL % OF WEAPON
}WEAPPERF, *PWEAPPERF;

typedef struct _weapondrug{
	int type; //TYPE OF DRUG
	int doses; //DOSES REMAINING
	BOOL doseloaded; //IS A DOSE LOADED INTO THE CHAMBER? DRUG SYRINGES CAN BE RELOADED!
}WDRUG, *PWDRUG;

typedef struct _weapstruct
{
	POINT WorldPoint;
int id; //THE NUMERICAL WEAPON ID...FOR INDEXING
BOOL dsaweapon; //0 = NOT DSA WEAPON, NON-ZERO = DSA WEAPON
int attacktype; //TYPE OF WEAPON, CAN CONTAIN MORE THAN ONE.
char shortname[20]; //SHORT NAME OF WEAPON
char longname[MAX_NAME_CHARS]; //LONG NAME OF WEAPON
char *desc; //DESCRIPTION OF WEAPON
int filingtype; //THE TYPE TO FILE THE WEAPON UNDER - PREDEFINED - ONLY USED IF DSA
int classtype; //THE CLASS OF THE WEAPON - A, B, S, X - ONLY USED IF DSA 
int frequency; // OUT OF 100, THE FREQUENCY WITH WHICH IT'S AVAILABLE (OR ENEMIES HAVE IT)
int ammotype; //AMMO ID OF WEAPON - MUST MATCH AN ID IN THE AMMO LIST
int maxammo; //MAXIMUM AMOUNT OF AMMO THE WEAPON CAN HOLD
int loadtype; //CLIP, MAGAZINE, CYLINDER
LOADTYPE loadprocedure; //THE ACTUAL CLIP RECIEVER, MAGAZINE, OR CYLINDER
int condition_percent; //CONDITION OF WEAPON IN PERCENT
WEAPPERF weapperf; //WEAPON PERFORMANCE STATS;
WDRUG drug; //DRUG STRUCTURE IF DRUGGED
float weight; //WEIGHT OF WEAPON (USED FOR HITTING AND LOADING)
MODIFIER modifiersallowed; //MODIFIERS ALLOWED
MODIFIER modifiersattached; //MODIFIERS CURRENTLY HELD
MODIFIER modifiersactivated;	//MODIFIERS CURRENTLY ACTIVE
char *soundfilename[MAX_WEAPON_SOUNDS]; //THE SOUND FILE NAMES FOR WEAPON ACTIONS - FIRE, THROW, HIT, DROP, RELOAD, BREAK/JAM
int index; //INDEX INTO THE PLAYER WEAPON LIST
BOOL weaponavailable; //IS IT ACTUALLY AVAILABLE?
}WEAPON, *PWEAPON;

typedef struct _doorstruct
{
	int ID; // THE ID OF THE DOOR.
	POINT location; //WHERE IT IS
	int type; //WOOD, GLASS, METAL, REINFORCED METAL
	BOOL isopen; //IS IT OPEN?
	BOOL vdoor; //IS IT VERTICAL? ELSE IT'S HORIZONTAL.
	BOOL lockactive; //IT'S DEACTIVATED IF THE PLAYER CRACKS IT, BUT NOT IF HE USES A CARD.
	int lock; //NONE, REGULAR LOCK, CRYPTO LOCK
	int lockrings; //MANUAL LOCK NUMBER OF RINGS
	int encryptionstrength; //DETERMINES CIS DIFFICULTY
	BOOL alarm; //IS THERE AN ALARM?
	BOOL keycard; //DOES THIS DOOR USE A KEYCARD;
	int keycardnumber; //WHICH KEYCARD NUMBER?
}DOOR, *PDOOR;

typedef struct _drugstruct
{
int drugtype[3];//TYPE OF DRUG IN SYRINGE DESIGNATED IN ARRAY
}DRUG, *PDRUG;

/************************************************************
	; This one's the big MAMA, everything ties into this.
	************************************************************/
typedef struct	_mapcell
{
	int xcoord; //The x-coordinate of the cell?
	int ycoord; //The y-coordinate of the cell?
	int type; //This is the type AND the symbol reference....two types can NEVER have the same graphical representation.
}MAPCELL, *PMAPCELL;

typedef struct _map
{
	PMAPCELL mapcells; //The pointer to the map cells
	int width; //How many map cells across?
	int height; //How many map cells down?
	POINT pz; //Where's the primary zone?
	POINT rz; //Where's the recovery zone?
}MAP, *PMAP;


/************************************************************
; MISSION RELATED STRUCTURES
************************************************************/

typedef struct _rescuestats{
	char* rescuedfirstname;//THE FIRST NAME OF THE RESCUED
	char* rescuedlastname; //THE LAST NAME OF THE RESCUED
	BOOL rescued; //DID THE OPERATIVE GET RESCUED BY THE PLAYER? THIS COUNTS AS A SUCCESS.
	BOOL cyanided; //DID THE OPERATIVE SUICIDE BEFORE RESCUE? THIS COUNTS AS A SUCCESS.
	BOOL shotattorture; //DID THE PLAYER SHOOT THE OPERATIVE BEFORE HE TALKED? THIS COUNTS AS A SUCCESS.
	BOOL shotwhileescaping; //DID THE OPERATIVE GET KILLED WHILE ESCAPING? THIS COUNTS AS A SUCCESS.
}RESCUESTATS, *PRESCUESTATS;

typedef struct _assassinstats{
	char* targetfirstname;
	char* targetlastname;
	int deathtype; //SHOT, KNIFED, CAR EXPLODED, WHAT?
	int failuretype; //IF FAILED, HOW DID PLAYER FAIL?
}ASSASSINSTATS, *PASSASSINSTATS;

typedef struct _isrstats{
	char* targetfirstname;
	char* targetlastname;
	int failuretype;
}ISRSTATS, *PISRSTATS;

typedef struct _retrievestats{
	int technologyid; //WHICH TECHNOLOGY?
	char prototypenumber[15]; //PROTOTYPE ID
	int failuretype; //DESTROYED, PLAYER DEAD, WHAT?
}RETRIEVESTATS, *PRETRIEVESTATS;

typedef struct _cauterystats{
	int bombnumber;
	bool hasbomb;
	bool bombactive;
	char detcode[MAX_ATOMIC_CODE_DIGITS];
	MISSIONCHRONO timetobombdetonation;
}CAUTERYSTATS, *PCAUTERYSTATS;

typedef union _opstats{
	RESCUESTATS rescue;
	ASSASSINSTATS assassination;
	ISRSTATS isr;
	RETRIEVESTATS retrieval;
	CAUTERYSTATS cautery;
}OPSTATS, *POPSTATS;


typedef struct _mstatsstruct
{
	int covertkills;
	int overtkills;
	int alarmstripped;
	int alarmsbypassed;
	int lockscracked;
	int locksblown;
	int shotsfired;
	int shotshit;
	int damagetaken;
}MSTATS, *PMSTATS;

typedef struct _missionstruct
{
	int missiontype; //THE MISSION TYPE
	char missioncode[15]; //THE MISSION CODE
	char codenameone[CODENAMELENGTH]; 
	char codenametwo[CODENAMELENGTH];
	int classificationindex;
	char classificationletter;
	BOOL radioavailable; //IS THE RADIO AVAILABLE?
	int RadioChannel;
	BOOL antidoteon; //IS YOUR TOXIN ANTIDOTE ON?
	BOOL cansnipe; //CAN YOU SNIPE CURRENTLY?
	BOOL dynamicmode; //DYNAMICMODE ON?
	BOOL primarycomplete; //IF THE PRIMARY HAS BEEN COMPLETED
	int draggingstatus; //0 = NOT DRAGGING ANYBODY 1 = DRAGGING ENEMY 2 = DRAGGING SOMETHING ELLLSE
	int draggingindex; //USED ONLY IF DRAGGING
	MISSIONCHRONO timetorecovery; //HOW MUCH TIME LEFT TO RECOVERY?
	MISSIONCHRONO timetoleftbehind; //HOW MUCH TIME UNTIL THE E-TEAM LEAVES THE PLAYER BEHIND?
	MSTATS missionstats; //GENERAL MISSION STATS
	OPSTATS specifics; //SPECIFICS ABOUT THE MISSION, UNION REFERENCED BY MISSIONTYPE ABOVE.
}MISSION, *PMISSION;

typedef struct _sartumissionstruct{
	int index; //INDEX INTO THE LIST OF SARTU MISSIONS
//	CHRONOSTRUCT timestampcreated; //WHEN MISSION WAS POSTED
//	CHRONOSTRUCT timestampexpires; //WHEN MISSION EXPIRES
	int daysleft; //WHEN MISSION EXPIRES
	int mapwidth; //WIDTH OF MAP
	int mapheight; //HEIGHT OF MAP
	int mapbuildingdensitypercentage; //BUILDING DENSITY % OF MAP
	char* missioncode; //CODE STRING OF MISSION
	int missiontype; //THE MISSION TYPE
	int missionspecificone; //MISSION SPECIFIC IDENTIFIER #1
	int missionspecifictwo; //MISSION SPECIFIC IDENTIFIER #2
	int numberguards; //NUMBER OF GUARDS IN THE MISSION
	int numberarmories; //NUMBER OF ARMORIES IN THE MISSION
	int numbervehicles; //NUMBER OF VEHICLES IN THE MISSION
	int compoundsecuritypercentage; //% OF COMPOUND SECURITY
	int enemyskillpercentage; //SKILL OF ENEMIES
	int codenameone; //CODE NAME #1
	int codenametwo;//CODE NAME #2
	int classificationname; //CLASSIFICATION ONE
	char classificationletter; //CLASSIFICATION LETTER
	char* missiondescription; //THE FULL MISSION DESCRIPTION.
	bool taken; //HAS THE MISSION BEEN TAKEN?
	int takenbywhomfirst; //WHO TOOK THE MISSION FIRST NAME? IF -1, PLAYER TOOK IT.
	int takenbywhomlast; //WHO TOOK THE MISSION LAST NAME? IF -1, PLAYER TOOK IT.
	int takenbywhomid; //WHO TOOK THE MISSION ID? IF -1, PLAYER TOOK IT.
}SARTUMISSION, *PSARTUMISSION;

typedef struct _historystruct{
	BOOL successful;
	int agentstatusafter; //ALIVE? DEAD? CAPTURED? CYANIDED?
	CHRONOSTRUCT datetimestart;
	CHRONOSTRUCT datetimefinish;
	MISSION missiondata;
}HISTORYSTRUCT, *PHISTORYSTRUCT;

typedef struct _historymatrix{
	BOOL previousemployment;
	int previousemploymenttype;
	int employmentspecialization;
	BOOL psyops;
	BOOL projectobserver;
	BOOL vand;
	BOOL university;
	char universityname[MAX_RESPONSE_CHARS];
	char major[MAX_RESPONSE_CHARS];
	BOOL networkcontacted;
}HISTORYMATRIX, *PHISTORYMATRIX;

typedef struct _medals{
	int defensemedals; //HOW MANY DEFENSE MEDALS DOES THE PLAYER HAVE?
	int defensemedalsV; //HOW MANY DEFENSE MEDALS WITH VALOR DEVICES?
	int silvereagle; //HOW MANY SILVER EAGLES?
	int goldeagle; //HOW MANY GOLD EAGLES?
	int honorcommendation; //HOW MANY HONOR COMMENDATIONS?
	int medalofthefalcon; //HOW MANY MEDALS OF THE FALCON?
}MEDALS, *PMEDALS;


typedef struct _vehiclestruct
{
int ID;
int type; //CAR, TRUCK, VAN
int armorlevel; //LEVEL OF ARMOR IN THE CAR
int hitpoints; //'HEALTH' POintS OF THE VEHICLE
int wheelsleft; //SUPPOSED TO BE FOUR, CAN BE LESS IF HIT
BOOL rigged; //IS IT RIGGED TO EXPLODE
BOOL leakinggas; //IS IT LEAKING GAS
BOOL gasjellied; //HAS THE GAS BEEN JELLIED
//PENEMY passengers; //LIST OF ENEMY PASSENGERS ON BOARD
BOOL windows[6]; //WHICH WINDOWS ARE STILL OKAY
BOOL brakelines[4]; //WHICH BRAKELINES STILL WORK
POINT	loc;		//WHERE THE VEHICLE IS LOCATED.
}VEHICLE, *PVEHICLE;

typedef struct _atomiclight
{
	int color;
	int type;
	int status;
	COORD topleft;
	COORD dimensions;
}ATOMICLIGHT, *PATOMICLIGHT;

typedef struct _worldclips{
	COORD mapcoords; //WHERE IS THIS CLIP LOCATED?
	CLIP clip; //THE ACTUAL CLIP DATA.
}WORLDCLIPS, *PWORLDCLIPS;

typedef struct _worldrounds{
	COORD mapcoords; //WHERE IS THIS CLIP LOCATED?
	STRAYROUNDS rounds; //THE ACTUAL CLIP DATA.
}WORLDROUNDS, *PWORLDROUNDS;

typedef struct _worldgizmo{
	COORD mapcoords;
	GIZMO gizmo;
}WORLDGIZMO, *PWORLDGIZMO;

typedef struct _worldweapon{
	COORD mapcoords;
	WEAPON weapon;
}WORLDWEAPON, *PWORLDWEAPON;



typedef struct _trackerarrows{
	bool TrackerArrows[8]; //SIGNIFYING EIGHT DIRECTIONS STARTING FROM NORTH, GOING COUNTER CLOCKWISE.
}TRACKERARROWS, *PTRACKERARROWS;

typedef struct _prefab{
int id; //NUMERICAL ID OF THE PREFAB UNIT
	int width;
int height;
int** prefabdata; //2D ARRAY OF TILES
}PREFAB, *PPREFAB;

typedef struct _enemyai{
	int StandingOrder;
	POINT DefaultPosition;
	int TertiaryNumber;
	POINT LastKnownPosition;
	int ConfidencePercentage;
	int CurrentAlertLevel;
}ENEMYAI, *PENEMYAI;

typedef union __POTYPE {
	int identifier; //A GENERIC IDENTIFIER
	char* string;
	COORD coordinate; //A GENERIC COORDINATE
	AMMO ammo;
	WEAPON weapon;
	ENVIRONSTRUCT environ;
	WORLDCLIPS worldclip;
	WORLDROUNDS worldrounds;
	WORLDWEAPON worldweapon;
	WORLDGIZMO worldgizmo;
	BULLET bullet;
	HISTORYSTRUCT history;
	EXPLOSIVE explosive;
	SHELL shell;
	VEHICLE vehicle;
	GIZMO gizmo;
	GRAPHIC graphic;
	STRAYROUNDS rounds;
	CLIP clip;
	BUG bug;
	SBK sbk;
	SARTUMISSION sartumission;
	DBLISTITEM dblistitem;
	PREFAB prefab;
	DOOR door;
	AGIZMO armorygizmo;
	SCRIPTEDEVENT scriptedevent;
}POTYPE, *PPOTYPE;

typedef struct _MusicMessage{
	int messagecode;
	char* song;
}MusicMessage, *PMusicMessage;

#endif