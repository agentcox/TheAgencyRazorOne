//RECIEVING CLIPS
#define GETCLIP_OK	0
#define GETCLIP_OK_LESSTHANMAX 1
#define GETCLIPFAILED_NOMAINAMMO	-1
#define GETCLIPFAILED_NOSUBAMMO	-2
#define GETCLIPFAILED_DROPPEDOUTOFLOOP -3
#define GETCLIPFAILED_NOBULLETS -4

//CONVERTING TO CLIPS
//LOADING CLIPS
#define LOADCLIP_OK	0
#define LOADCLIPFAILED_WRONGLOADTYPE	-1
#define LOADCLIPFAILED_CLIPALREADYLOADED	-2
#define LOADCLIPFAILED_NOCLIPMATCH	-3
#define LOADCLIPFAILED_WRONGAMMOTYPEFORWEAPON -4
#define LOADCLIPFAILED_WRONGCLIPFORWEAPON -5
//EJECTING CLIPS
#define EJECTCLIP_OK_SAVED 0
#define EJECTCLIP_OK_DROPPED 1
#define EJECTCLIPFAILED_NOCLIPLOADED	-1
#define EJECTCLIPFAILED_WRONGLOADTYPE -2
#define EJECTCLIPFAILED_NOMATCHTODROP -3

//GIVING ARMORY WEAPONS
#define GIVEARMORYWEAPON_OK	0
#define GIVEARMORYWEAPONFAILED_NOMATCH -1
//GIVING TRAINING WEAPONS
#define GIVETRAININGWEAPON_OK 0
#define GIVETRAININGWEAPONFAILED_NOMATCH	-1
//REMOVING WEAPONS
#define REMOVEPLAYERWEAPON_OK	0
#define REMOVEPLAYERWEAPONFAILED_NOMATCH -1
#define	REMOVEWEAPON_OK			0
#define REMOVEWEAPONFAILED_NOMATCH -1

//LOADING MAGAZINES
#define LOADMAGAZINE_OK_REMOVED_NOROUNDS 1
#define LOADMAGAZINE_OK	0
#define LOADMAGAZINEFAILED_WRONGLOADTYPE -1
#define LOADMAGAZINEFAILED_MAGAZINEFULL -2
#define LOADMAGAZINEFAILED_TOOMANYROUNDSTOLOAD -3
#define LOADMAGAZINEFAILED_NOMATCH -4
#define LOADMAGAZINEFAILED_NOTENOUGHROUNDS -5
#define LOADMAGAZINEFAILED_WRONGAMMOTYPEFORWEAPON -6

//UNLOADING A MAGAZINE
#define UNLOADMAGAZINE_OK	0
#define UNLOADMAGAZINEFAILED_WRONGLOADTYPE -1
#define UNLOADMAGAZINEFAILED_MAGAZINEEMPTY -2
#define UNLOADMAGAZINEFAILED_FELLOUTOFLOOP -3

#define	EJECTFROMMAG_OK						0
#define EJECTFROMMAGFAILED_NONELOADED		-1
#define EJECTFROMMAGFAILED_WRONGLOADTYPE	-2

//FIRING A MAGAZINE
#define FIREMAGAZINE_OK	0
#define FIREMAGAZINEFAILED_WRONGLOADTYPE -1
#define FIREMAGAZINEFAILED_OUTOFAMMO -2
#define FIREMAGAZINEFAILED_NOMATCH -3
#define FIREMAGAZINEFAILED_NOSUBMATCH -4
#define FIREMAGAZINEFAILED_FELLOUTOFLOOP -5

//FIRING A CLIP
#define FIRECLIP_OK_ALLROUNDS 0
#define FIRECLIP_OK_LASTROUNDS 1
#define FIRECLIPFAILED_WRONGLOADTYPE -1
#define FIRECLIPFAILED_NOCLIPINSERTED -2
#define FIRECLIPFAILED_OUTOFAMMO -3
#define FIRECLIPFAILED_NOMATCH -4

//DECREMENTING A CLIP
#define DECREMENTCLIP_OK	0
#define DECREMENTCLIPFAILED_WRONGLOADTYPE -1
#define DECREMENTCLIPFAILED_NOCLIPINSERTED -2
#define DECREMENTCLIPFAILED_OUTOFAMMO -3

//ROTATING A CYLINDER
#define ROTATECYLINDER_OK	0
#define ROTATECYLINDERFAILED_WRONGLOADTYPE -1

//LOADING A CYLINDER
#define LOADCYLINDER_OK_REMOVED_NOROUNDS	1
#define LOADCYLINDER_OK	0
#define LOADCYLINDERFAILED_WRONGLOADTYPE -1
#define LOADCYLINDERFAILED_CYLINDERFULL -2
#define LOADCYLINDERFAILED_BADCHAMBERNUMBER -3
#define LOADCYLINDERFAILED_NOMATCH -4
#define LOADCYLINDERFAILED_NOAMMOLEFT -5
#define LOADCYLINDERFAILED_WRONGAMMOTYPEFORWEAPON -6
//FIRING A CYLINDER
#define FIRECYLINDER_OK	0
#define FIRECYLINDERFAILED_WRONGLOADTYPE -1
#define FIRECYLINDERFAILED_CYLINDEREMPTY -2
#define FIRECYLINDERFAILED_CYLINDERSHELLONLY -3
#define FIRECYLINDERFAILED_NOMATCH -4
#define FIRECYLINDERFAILED_NOSUBMATCH -5
#define FIRECYLINDERFAILED_FELLOUTOFLOOP -6

//DATA RETRIEVAL
#define RETRIEVEGLOBALSUBAMMOTYPEFAILED -1

//GETTING STRAYROUNDS
#define GETSTRAYROUNDS_OK	0
#define GETSTRAYROUNDSFAILED_NOMAINAMMO	-1
#define GETSTRAYROUNDSFAILED_NOTENOUGHBULLETS	-2
#define GETSTRAYROUNDSFAILED_NOSUBAMMO	-3
#define GETSTRAYROUNDSFAILED_DROPPEDOUTOFLOOP -4

//CLIP MANIPULATION DIALOG MODES
#define CLIPMD_MODE_VIEWCLIPS 0
#define CLIPMD_MODE_LOADCLIPS 1
#define CLIPMD_MODE_DROPCLIPS 2
#define CLIPMD_MODE_TAKECLIPS 3

//CHECKING QUANTITY DEFINES
#define WEAPON_NOTAMMO	-1
#define WEAPON_UNLOADED	0
#define WEAPON_PARTIALLY_LOADED	1
#define WEAPON_FULLY_LOADED		2

#define CLIP_UNLOADED			0
#define CLIP_PARTIALLY_LOADED	1
#define CLIP_FULLY_LOADED		2

/************************************************************
; FUNCTIONS BEGIN HERE
************************************************************/

//CLIP FUNCTIONS
int GiveClip(PLIST cliplist, PWEAPON weapon, int subammotype, PLIST ammolist, float* weighttomodify, BOOL training);
int LoadClip(PLIST cliplist, PWEAPON weapon, int clipid);
int EjectClip(PWEAPON weapon, PLIST cliplist);
int FireClipBullet(PWEAPON weapon, PBULLET bulletdata, int* roundsfired);
int FireClipShell(PWEAPON weapon, PSHELL shell, int* roundsfired);
int FireClipExplosive(PWEAPON weapon, PEXPLOSIVE explosive, int* roundsfired);
int DecrementClip(PWEAPON weapon, PCLIP clip);
int SelectClipFromAmmoList(PPLAYER player, PLIST ammolist, PWEAPON weapon, BOOL training);
void ClipManipulationDialog(PLIST sourceammolist, PLIST destinationammolist, int mode);
int ChooseClipFromList(PLIST cliplist);
void InitWeaponClip(PWEAPON weapon);
int CompareClipWeaponID(PPOTYPE itm1, PPOTYPE itm2);
void PlayerDropClip(PPLAYER player, PCLIP clip);

//STRAYROUND FUNCTIONS
int GiveStrayRounds(PLIST strayroundsourcelist, PROUND round, int numbertoget,PLIST strayroundtargetlist, float* weighttomodify, BOOL training);
int CompareStrayRoundAmmoTypes(PPOTYPE itm1, PPOTYPE itm2);
int CompareBothStrayRoundAmmoTypes(PPOTYPE itm1, PPOTYPE itm2);
bool PlayerDropStrayRounds(PPLAYER player, PSTRAYROUNDS rounds, int numtodrop);
//MAGAZINE FUNCTIONS
int LoadMagazine(PWEAPON weapon, PLIST strayroundlist, int roundidentifier, int numbertoload);
int UnloadMagazine(PWEAPON weapon);
int FireMagazineShell(PWEAPON weapon, PSHELL shelldata);
int FireMagazineBullet(PWEAPON weapon, PBULLET bulletdata);
int FireMagazineExplosive(PWEAPON weapon, PEXPLOSIVE explosivedata);
void DecrementMagazine(PWEAPON weapon);
void InitWeaponMagazine(PWEAPON weapon);
void FreeWeaponMagazine(PWEAPON weapon);
int GetClipLoadedState(PCLIP clip);
int EjectFromMagazine(PWEAPON weapon, PLIST strayroundlist);
bool FillMagazineWithRounds(PWEAPON weapon, PSTRAYROUNDS rounds, PLIST roundslist);
//CYLINDER FUNCTIONS
void InitWeaponCylinder(PWEAPON weapon);
void FreeWeaponCylinder(PWEAPON weapon);
int LoadCylinder(PWEAPON weapon, PLIST strayroundlist, int roundidentifier, int chambertoload);
int FireCylinderShell(PWEAPON weapon, PSHELL shelldata);
int FireCylinderBullet(PWEAPON weapon, PBULLET bulletdata);
int FireCylinderExplosive(PWEAPON weapon, PEXPLOSIVE explosivedata);
int RotateCylinder(PWEAPON weapon);
void EmptyCylinderChamber(PWEAPON weapon, PLIST strayroundslist, int Chamber);
//WEAPON FUNCTIONS
int GiveArmoryWeapon(PPLAYER player, int weaponid);
int GiveTrainingWeapon(PPLAYER player, int weaponid);
int GiveTrainingWeapon(PENEMY enemy, int weaponid);
int GiveTrainingWeapon(PLIST weaponlisttomodify, int weaponid);
int GetWeightedRandomWeaponID(PLIST weaponlist);
int GetWeightedNONDSARandomWeaponID(PLIST weaponlisttosearch);
int RemovePlayerWeapon(PPLAYER player, int index);
void RemoveAllWeapons(PLIST list);
int RemoveWeapon(PLIST weaponlist, int index);
void RemoveAllPlayerWeapons(PPLAYER player);
bool GiveLoadedTrainingWeapon(PLIST weaponlist, PPLAYERAMMO ammolist, int weaponid, int subammoid);
bool GiveRandomExtraAmmo(PPLAYERAMMO Ammolisttomodify, int WeaponID);
bool GiveRandomNONDSAExtraAmmo(PPLAYERAMMO Ammolisttomodify, int WeaponID);
//DATA RETRIEVAL
int RetrieveGlobalSubAmmoType(PROUND round);
PSHELL RetrieveGlobalShellData(PROUND round);
PBULLET RetrieveGlobalBulletData(PROUND round);
PEXPLOSIVE RetrieveGlobalExplosiveData(PROUND round);
PWEAPON RetrieveGlobalWeaponDataByIndex(int weaponid);
BOOL IsSubAmmoDSAOnly(PROUND round);
int RetrieveSubAmmoNumberRoundsLeft(PROUND round);
char* RetrieveSubAmmoShortName(PROUND round);
char* RetrieveSubAmmoLongName(PROUND round);
char* RetrieveMainAmmoName(PROUND round);
PWEAPON RetrieveGlobalWeaponDataByID(int id);
bool IsWeaponShootable(PWEAPON weapon);
AMMORESULT ReturnClipOrStrayRoundByID(PPLAYERAMMO ammolist, int id);
int FindMatchingPlayerAmmoIDs(PPLAYERAMMO playerammo, PWEAPON weapon, int** outarray);
void ReorderPlayerAmmoLists(PPLAYERAMMO ammolist);
int GetWeaponLoadedState(PWEAPON weapon);
int CountTotalRoundsInWeapon(PWEAPON Weapon);
PCLIP FindNextClipInInventory(PLIST cliplist, PCLIP currentclip, PWEAPON weapon, bool wraparound);
PCLIP FindPreviousClipInInventory(PLIST cliplist, PCLIP currentclip, PWEAPON weapon, bool wraparound);
PSTRAYROUNDS FindPreviousStrayRoundsInInventory(PLIST straylist, PSTRAYROUNDS currentrounds, PWEAPON weapon, bool wraparound);
PSTRAYROUNDS FindNextStrayRoundsInInventory(PLIST straylist, PSTRAYROUNDS currentrounds, PWEAPON weapon, bool wraparound);
ROUND GetRoundStructFromStrayRounds(PSTRAYROUNDS R);