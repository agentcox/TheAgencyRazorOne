#ifndef WEAPONSAMMO_H
#define WEAPONSAMMO_H

#include "agencyinclude.h"

//COMPARISON FUNCTIONS
int CompareAmmoID(PPOTYPE itm1,PPOTYPE itm2) ; //Comparison fn
int CompareWeaponID(PPOTYPE itm1,PPOTYPE itm2) ;
int CompareExplosiveSubAmmoID(PPOTYPE itm1,PPOTYPE itm2); 
int CompareBulletSubAmmoID(PPOTYPE itm1,PPOTYPE itm2); 
int CompareShellSubAmmoID(PPOTYPE itm1,PPOTYPE itm2); 
//COMPARISON FUNCTIONS
int CompareWeaponIndex(PPOTYPE itm1,PPOTYPE itm2);
int CompareClipID(PPOTYPE itm1,PPOTYPE itm2);
int CompareStrayRoundsID(PPOTYPE itm1,PPOTYPE itm2);
int CompareMainandSubAmmoID(PPOTYPE itm1, PPOTYPE itm2);
int CompareStrayRoundsAmmo(PPOTYPE itm1,PPOTYPE itm2);

//DATA RETRIEVAL FUNCTIONS
PWEAPON GetWeaponByID(PLIST weaponlist, int id);
PAMMO GetMainAmmoByID(PLIST ammolist, int id);
PSHELL GetShellSubAmmoByID(PAMMO mainammo, int id);
PEXPLOSIVE GetExplosiveSubAmmoByID(PAMMO mainammo, int id);
PBULLET GetBulletSubAmmoByID(PAMMO mainammo, int id);
char *getWeaponClassString(WEAPON weapon);
char* getWeaponFilingString(WEAPON weapon);
BOOL WeaponSilencerOn(PWEAPON weapon);
BOOL WeaponLasersightOn(PWEAPON weapon);
BOOL WeaponFlashsuppressorOn(PWEAPON weapon);
BOOL WeaponSilencerAllowed(PWEAPON weapon);
BOOL WeaponLasersightAllowed(PWEAPON weapon);
BOOL WeaponFlashsuppressorAllowed(PWEAPON weapon);
BOOL WeaponSilencerHeld(PWEAPON weapon);
BOOL WeaponLasersightHeld(PWEAPON weapon);
BOOL WeaponFlashsuppressorHeld(PWEAPON weapon);

//LIST MANAGEMENT FUNCTIONS
int SortCompareWeaponArmory(PPOTYPE WOne, PPOTYPE WTwo);
void SwapWeaponIndex(PPOTYPE WOne, PPOTYPE WTwo);
void SwapWeaponID(PPOTYPE WOne, PPOTYPE WTwo);
void SortArmoryWeapons(PLIST armorylist);
void SortPersonalWeapons(PLIST weaponlist);
void ReorderWeaponArmoryIDs(PLIST armorylist);
void PrintWeaponStuff(PPOTYPE Object);

//OUTPUT FUNCTIONS
void printAllAmmo(PLIST ammolist);
void printAllWeapons(PLIST weaponList, PLIST ammoList);

//UTILITY FUNCTIONS
char* StripNewLine(char* buf);
void sampleload();

//char* loopuntilfind(FILE* fp, char* buf, char* findstring); NOT USED AT THE MOMENT

//DATA STRUCTURE CREATION/TERMINATION FUNCTIONS
PLIST createAmmoList(FILE* fp);
PLIST createWeaponList(FILE* fp, PLIST ammolist);
void createWeapon(PLIST weaponList, PLIST ammoList, FILE* fp);
void createAmmo(PLIST ammoList, FILE* fp);
void createShellAmmo(PPOTYPE tmpObj, FILE* fp);
void createBulletAmmo(PPOTYPE tmpObj, FILE* fp);
void createExplosiveAmmo(PPOTYPE tmpObj, FILE* fp);
void FreeAmmoList(PLIST ammolist);
void FreeWeaponList(PLIST weaponlist);

#endif