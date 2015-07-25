#include "..\\agencyinclude.h"
#include "..\\weaponsammo.h"


#define CHECKVALUESOK 0
#define CHECKVALUESFAILED -1


typedef BOOL b00l;



/************************************************************
; UTILITY FUNCTIONS
************************************************************/

void WriteBlankAmmoFile();
void WriteBlankWeaponFile();

void ReorderExplosiveListID(PLIST explosivelist);
void ReorderShellListID(PLIST shelllist);
void ReorderBulletListID(PLIST bulletlist);
void ReorderAmmoListID(PLIST ammolist, PLIST weaponlist);
void ReorderWeaponListID(PLIST weaponlist);


/************************************************************
; PROCESS CALLBACKS
************************************************************/

b00l CALLBACK MainMenuProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam); //FINISHED WAY EARLY ON
b00l CALLBACK WeaponDialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
b00l CALLBACK AmmoDialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
b00l CALLBACK ShellDialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam); //FINISHED 3/20/00
b00l CALLBACK BulletDialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
b00l CALLBACK ExplosiveDialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


/************************************************************
; WEAPON EDITOR SUB-FUNCTIONS - FINISHED! 3/24/00
************************************************************/

void WEdLoadBoxes(HWND hwnd);
void WEdClearFields(HWND hwnd);
int WEdCheckValuesNew(HWND hwnd);
int WEdCheckValuesModify(HWND hwnd);
void WEdCreateNewWeapon(HWND hwnd);
void WEdModifyWeapon(HWND hwnd);
int WEdLoadData();
void WEdSelectWeapon(HWND hwnd);
void WEdWriteWeaponFile();




/************************************************************
; MAIN AMMO EDITOR SUB-FUNCTIONS - FINISHED! 3/21/00
************************************************************/

int MAEdLoadData(HWND hwnd);
int MAEdPopulateData(HWND hwnd);
void MAEdClearFields(HWND hwnd);
void MAEdSelectMainAmmo(HWND hwnd);
int MAEdCheckNewValues(HWND hwnd);
int MAEdCheckSaveValues(HWND hwnd);
void MAEdMakeNewMainAmmo(HWND hwnd);
int MAEdCheckSubAmmoValues(HWND hwnd);
void MAEdCreateExplosiveList();
void MAEdCreateShellList();
void MAEdCreateBulletList();
void MAEdClearFields(HWND hwnd);
void MAEdWriteShellFileData(FILE* fp, PSHELL shell);
void MAEdWriteBulletFileData(FILE* fp, PBULLET bullet);
void MAEdWriteExplosiveFileData(FILE* fp, PEXPLOSIVE explosive);
int MAEdWriteAmmoFile(HWND hwnd);
int MAEdCheckWriteValues(HWND hwnd);


/************************************************************
; SHELL EDITOR SUB-FUNCTIONS - FINISHED! 3/20/00
************************************************************/
int ShellEdCheckValuesNew(HWND hwnd);
int ShellEdCheckValuesModify(HWND hwnd);
void ShellEdPopulateData(HWND hwnd);
void ShellEdSelectSubAmmo(HWND hwnd);
void ShellEdClearFields(HWND hwnd);
void ShellEdModifyCurrentAmmo(HWND hwnd);
void ShellEdCreateNewAmmo(HWND hwnd);
void ShellEdSaveAmmoList(HWND hwnd);


/************************************************************
; BULLET EDITOR SUB-FUNCTIONS - FINISHED! 3/21/00
************************************************************/
int BulletEdCheckValuesNew(HWND hwnd);
int BulletEdCheckValuesModify(HWND hwnd);
void BulletEdPopulateData(HWND hwnd);
void BulletEdSelectSubAmmo(HWND hwnd);
void BulletEdClearFields(HWND hwnd);
void BulletEdModifyCurrentAmmo(HWND hwnd);
void BulletEdCreateNewAmmo(HWND hwnd);
void BulletEdSaveAmmoList(HWND hwnd);


/************************************************************
; EXPLOSIVE EDITOR SUB-FUNCTIONS - FINISHED! 3/21/00
************************************************************/
int ExplosiveEdCheckValuesNew(HWND hwnd);
int ExplosiveEdCheckValuesModify(HWND hwnd);
void ExplosiveEdPopulateData(HWND hwnd);
void ExplosiveEdSelectSubAmmo(HWND hwnd);
void ExplosiveEdClearFields(HWND hwnd);
void ExplosiveEdModifyCurrentAmmo(HWND hwnd);
void ExplosiveEdCreateNewAmmo(HWND hwnd);
void ExplosiveEdSaveAmmoList(HWND hwnd);