
/***********************************************************
;						 Project Title: The Agency Weapon Editor 
;									  Team: VDG - Charles Cox
;				Module Filename: weaponeditor.cpp
;				Module Synopsis: The Agency - Text Based Prototype
					Windows-Based Weapon and Ammo Editor.
;;
;								  Version: 1.24
;					  Revision Date: 04/18/2001
;						   Created By: Charles Cox
************************************************************/
#define VERSION "1.24"
#include "weaponeditor.h"
#include "resource.h"

#define LIST_FREE FALSE
#define LIST_ALLOCATED TRUE

PLIST weaponlist;
BOOL weaponlistallocated = LIST_FREE;
PPOTYPE pweapon;
PLIST ammolist;
BOOL ammolistallocated = LIST_FREE;
PPOTYPE pammo;
PLIST shellammolist;
BOOL shellammolistallocated = LIST_FREE;
PPOTYPE pshell;
PLIST bulletammolist;
BOOL bulletammolistallocated = LIST_FREE;
PPOTYPE pbullet;
PLIST explosiveammolist;
BOOL explosiveammolistallocated = LIST_FREE;
PPOTYPE pexplosive;


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
int WINAPI WinMain (HINSTANCE hinstance, HINSTANCE hprev, PSTR cmdline,
        int ishow)
{
	static TCHAR AppName[] = TEXT ("The Agency : Weapon Editor");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hinstance;
	wndclass.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(EDITORICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = AppName;

	if(!RegisterClass(&wndclass)){
		return 1;
	}

//	hwnd = CreateWindow(AppName, AppName, WS_DISABLED, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hinstance, NULL);
//	ShowWindow(hwnd, ishow);
//	UpdateWindow(hwnd);

 DialogBoxParam(hinstance, MAKEINTRESOURCE(MAINMENUDIALOG), NULL, MainMenuProc, 0);
return 1 ;
}



b00l CALLBACK MainMenuProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	int retval;

	switch (msg){
	case WM_INITDIALOG:
		
		return 1;
	
	case WM_COMMAND:
		switch (LOWORD(wparam)){
		case IDQUIT:
			ShowWindow(hwnd, SW_HIDE);
			EndDialog(hwnd, 1);
			return (1);
		case CHOOSEAMMOED_BUTTON:
			ShowWindow(hwnd, SW_HIDE);
			DialogBoxParam(((HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE)), MAKEINTRESOURCE(MAINAMMODIALOG), hwnd, (DLGPROC)AmmoDialogProc,0);
			ShowWindow(hwnd, SW_SHOWNORMAL);
			return (1);
		case CHOOSEWEAPONED_BUTTON:
			ShowWindow(hwnd, SW_HIDE);
			retval = DialogBoxParam(((HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE)),  MAKEINTRESOURCE(WEAPONDIALOG), hwnd, (DLGPROC)WeaponDialogProc,0);
			ShowWindow(hwnd, SW_SHOWNORMAL);
			if (retval == -1){
				LPVOID lpMsgBuf;
				FormatMessage( 
					 FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					 FORMAT_MESSAGE_FROM_SYSTEM | 
					 FORMAT_MESSAGE_IGNORE_INSERTS,
					  NULL,
					  GetLastError(),
						 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
						  (LPTSTR) &lpMsgBuf,
						   0,
						  NULL 
						);
						MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
						// Free the buffer.
							LocalFree( lpMsgBuf );
			}
		
			return (1);
		}
	}
	return 0;
}

b00l CALLBACK WeaponDialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	int returncode = CHECKVALUESOK;
	char buf[256];
	switch (msg){
		
	case WM_INITDIALOG:
		returncode = WEdLoadData();
				if (returncode == -1 || returncode == -2){
				MessageBox(hwnd, "Weapon Editor Non-Accessible Without Ammo.Dat, Please Use Ammo Editor To Create Ammo first.", "Weapon Editor NON-ACCESSIBLE", MB_OK);
				SendMessage(hwnd, WM_DESTROY, 0,0);
				}
		if(returncode == -4){
			SendMessage(hwnd, WM_DESTROY, 0,0);
			return 1;
		}
		WEdLoadBoxes(hwnd);
		WEdClearFields(hwnd);
		return 1;
		
	case WM_DESTROY:
		EndDialog(hwnd, 1);
		if (weaponlistallocated){
		freeList(weaponlist);
		weaponlistallocated = LIST_FREE;
		}
		if (ammolistallocated){
		freeList(ammolist);
		ammolistallocated = LIST_FREE;
		}
		return (1);
		
		
		case WM_COMMAND:
			switch (LOWORD(wparam)){
			case IDQUITNOSAVE:
				SendMessage(hwnd, WM_DESTROY, 0,0);
				return(1);
			
			case IDQUITSAVE:
				SortArmoryWeapons(weaponlist);
				WEdWriteWeaponFile();
				MessageBox(hwnd, "Weapons Sorted and Written. Thank You.", "Good Deal", MB_OK);
				SendMessage(hwnd, WM_DESTROY, 0,0);
				return (1);
				
					case WEAPONLOADBOX:
			if (((int) LOWORD(wparam)) == WEAPONLOADBOX && ((int) HIWORD (wparam)) == CBN_SELCHANGE){
				WEdSelectWeapon(hwnd);
			}
			return 1;
		
			case CLEARBUTTON:
				WEdClearFields(hwnd);
				return(1);

			case REMOVEBUTTON2:
				returncode = SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_GETCURSEL, 0,0);
			if (returncode < 1){
				MessageBox(hwnd, "Cannot delete the NONE weapon!", "Problem...", MB_OK);
				return 1;
			}
			else{
				pweapon = (PPOTYPE) SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_GETITEMDATA, returncode, 0);
				SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_DELETESTRING, returncode ,0);
				free(pweapon->weapon.desc);
				for(int i = 0; i < MAX_WEAP_SOUNDS; i++){
				free(pweapon->weapon.soundfilename[i]);
				}
				deleteObject(weaponlist, pweapon);
				ReorderWeaponListID(weaponlist);
				WEdClearFields(hwnd);
			SendDlgItemMessage(hwnd, TOTALWEAPONSBOX, WM_GETTEXT, 200, (LPARAM) buf);
			wsprintf(buf, "%d", atoi(buf) -1);
			SendDlgItemMessage(hwnd, TOTALWEAPONSBOX, WM_SETTEXT, 0, (LPARAM)buf);
			}
			return(1);

				
			case SUBMITNEWBUTTON:
				
				returncode = (WEdCheckValuesNew(hwnd));
				if (returncode != CHECKVALUESOK){
					return (1);
				}
				else{
					//IF IT'S A SUBMIT NEW, WE NEED TO SWITCH THE WEAPONEDITED NUMBER TO ZERO.
					WEdCreateNewWeapon(hwnd);
					WEdClearFields(hwnd);
					//SEND IT INTO THE LIST, CLEAR FIELDS, INCREMENT NUMBER.
				}
				return 1;

			case SUBMITMODIFIEDBUTTON:
				returncode = WEdCheckValuesModify(hwnd);
				if (returncode != CHECKVALUESOK){
					return (1);
				}
				else{
					WEdModifyWeapon(hwnd);
					WEdClearFields(hwnd);
				}
				return 1;
			}	
	}
	
	return 0;
}

void WEdSelectWeapon(HWND hwnd)
{
	int index;
	char buf[200];

	int weightones;
	int weighttenths;

	index = SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_GETCURSEL, 0,0);
	if (index < 1){
		WEdClearFields(hwnd);
		return;
	}

	pweapon = (PPOTYPE) SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_GETITEMDATA, index, 0);

	wsprintf(buf, "%d", index);
	SendDlgItemMessage(hwnd, CURRENTWEAPONBOX, WM_SETTEXT, 0, (LPARAM) buf);

	SendDlgItemMessage(hwnd, SHORTNAMEBOX, WM_SETTEXT, 0, (LPARAM)pweapon->weapon.shortname);
	SendDlgItemMessage(hwnd, LONGNAMEBOX, WM_SETTEXT, 0, (LPARAM)pweapon->weapon.longname);
	SendDlgItemMessage(hwnd, DESCBOX, WM_SETTEXT, 0, (LPARAM)pweapon->weapon.desc);
	if (pweapon->weapon.dsaweapon == TRUE){
		SendDlgItemMessage(hwnd, DSAONLYCHECK, BM_SETCHECK, (WPARAM) BST_CHECKED, 0);
	}
	else{
		SendDlgItemMessage(hwnd, DSAONLYCHECK, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
	}

	wsprintf(buf, "%d", pweapon->weapon.maxammo);
	SendDlgItemMessage(hwnd, MAXAMMOBOX, WM_SETTEXT, 0, (LPARAM) buf);

	SendDlgItemMessage(hwnd, FILINGTYPEBOX, CB_SETCURSEL, pweapon->weapon.filingtype, 0);
	SendDlgItemMessage(hwnd, CLASSTYPEBOX, CB_SETCURSEL, pweapon->weapon.classtype, 0);
	SendDlgItemMessage(hwnd, AMMOLOADTYPEBOX, CB_SETCURSEL, pweapon->weapon.loadtype, 0);
	SendDlgItemMessage(hwnd, DRUGTYPEBOX, CB_SETCURSEL, pweapon->weapon.drug.type, 0);
	SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_SETCURSEL, pweapon->weapon.ammotype, 0);

	wsprintf(buf, "%d", pweapon->weapon.drug.doses);
	SendDlgItemMessage(hwnd,DRUGDOSESBOX , WM_SETTEXT, 0, (LPARAM) buf);

	wsprintf(buf, "%d", pweapon->weapon.frequency);
	SendDlgItemMessage(hwnd,FREQUENCYBOX , WM_SETTEXT, 0, (LPARAM) buf);

		wsprintf(buf, "%d", pweapon->weapon.weapperf.flashlevel);
	SendDlgItemMessage(hwnd,FLASHLEVELBOX , WM_SETTEXT, 0, (LPARAM) buf);

		wsprintf(buf, "%d", pweapon->weapon.weapperf.soundlevel);
	SendDlgItemMessage(hwnd,SOUNDLEVELBOX , WM_SETTEXT, 0, (LPARAM) buf);

		wsprintf(buf, "%d", pweapon->weapon.weapperf.roundsperturn);
	SendDlgItemMessage(hwnd,ROUNDSPERTURNBOX , WM_SETTEXT, 0, (LPARAM) buf);

		wsprintf(buf, "%d", pweapon->weapon.weapperf.baseaccuracy_percent);
	SendDlgItemMessage(hwnd,BASEACCURACYBOX , WM_SETTEXT, 0, (LPARAM) buf);

	if (pweapon->weapon.modifiersallowed.flashsuppressor){
	SendDlgItemMessage(hwnd, MODIFIERCHECKFLASHSUPPRESSOR, BM_SETCHECK, (WPARAM) BST_CHECKED, 0);
	}
	else{
		SendDlgItemMessage(hwnd, MODIFIERCHECKFLASHSUPPRESSOR, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
	}

		if (pweapon->weapon.modifiersallowed.silencer){
	SendDlgItemMessage(hwnd, MODIFIERCHECKSILENCER, BM_SETCHECK, (WPARAM) BST_CHECKED, 0);
	}
	else{
		SendDlgItemMessage(hwnd, MODIFIERCHECKSILENCER, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
	}

		if (pweapon->weapon.modifiersallowed.lasersight){
	SendDlgItemMessage(hwnd, MODIFIERCHECKLASERSIGHT, BM_SETCHECK, (WPARAM) BST_CHECKED, 0);
	}
	else{
		SendDlgItemMessage(hwnd, MODIFIERCHECKLASERSIGHT, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
	}

	weightones = (int)pweapon->weapon.weight;
	wsprintf(buf, "%d", weightones);
	SendDlgItemMessage(hwnd, WEIGHTONESBOX, WM_SETTEXT, 0,(LPARAM)buf);
	weighttenths =(int) ((pweapon->weapon.weight - (float)weightones) * 10);
	wsprintf(buf, "%d", weighttenths);
	SendDlgItemMessage(hwnd, WEIGHTTENTHSBOX, WM_SETTEXT, 0,(LPARAM)buf);

	if (pweapon->weapon.attacktype & WEAP_ATT_TYPE_SHOOT){
		SendDlgItemMessage(hwnd, WEAPONACTIONCHECKSHOOT, BM_SETCHECK, (WPARAM) BST_CHECKED, 0);
	}
	else{
		SendDlgItemMessage(hwnd, WEAPONACTIONCHECKSHOOT, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
	}

		if (pweapon->weapon.attacktype & WEAP_ATT_TYPE_THROW){
		SendDlgItemMessage(hwnd, WEAPONACTIONCHECKTHROW, BM_SETCHECK, (WPARAM) BST_CHECKED, 0);
	}
	else{
		SendDlgItemMessage(hwnd, WEAPONACTIONCHECKTHROW, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
	}

			if (pweapon->weapon.attacktype & WEAP_ATT_TYPE_CUT){
		SendDlgItemMessage(hwnd, WEAPONACTIONCHECKCUT, BM_SETCHECK, (WPARAM) BST_CHECKED, 0);
	}
	else{
		SendDlgItemMessage(hwnd, WEAPONACTIONCHECKCUT, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
	}

			if (pweapon->weapon.attacktype & WEAP_ATT_TYPE_HIT){
		SendDlgItemMessage(hwnd, WEAPONACTIONCHECKHIT, BM_SETCHECK, (WPARAM) BST_CHECKED, 0);
	}
	else{
		SendDlgItemMessage(hwnd, WEAPONACTIONCHECKHIT, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
	}

			if (pweapon->weapon.attacktype & WEAP_ATT_TYPE_DRUG){
		SendDlgItemMessage(hwnd, WEAPONACTIONCHECKDRUG, BM_SETCHECK, (WPARAM) BST_CHECKED, 0);
	}
	else{
		SendDlgItemMessage(hwnd, WEAPONACTIONCHECKDRUG, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
	}

			if (pweapon->weapon.attacktype & WEAP_ATT_TYPE_SNIPE){
		SendDlgItemMessage(hwnd, WEAPONACTIONCHECKSNIPE, BM_SETCHECK, (WPARAM) BST_CHECKED, 0);
	}
	else{
		SendDlgItemMessage(hwnd, WEAPONACTIONCHECKSNIPE, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
	}


	if (pweapon->weapon.soundfilename[WEAP_SOUND_FIRE] != NULL){
	SendDlgItemMessage(hwnd, FIRESOUNDBOX, WM_SETTEXT, 0, (LPARAM) pweapon->weapon.soundfilename[WEAP_SOUND_FIRE]);
	}
	else{
		SendDlgItemMessage(hwnd, FIRESOUNDBOX, WM_SETTEXT, 0, (LPARAM) "");
	}

		if (pweapon->weapon.soundfilename[WEAP_SOUND_HIT] != NULL){
	SendDlgItemMessage(hwnd, HITSOUNDBOX, WM_SETTEXT, 0, (LPARAM) pweapon->weapon.soundfilename[WEAP_SOUND_HIT]);
	}
	else{
		SendDlgItemMessage(hwnd, HITSOUNDBOX, WM_SETTEXT, 0, (LPARAM) "");
	}

		if (pweapon->weapon.soundfilename[WEAP_SOUND_THROW] != NULL){
	SendDlgItemMessage(hwnd, THROWSOUNDBOX, WM_SETTEXT, 0, (LPARAM) pweapon->weapon.soundfilename[WEAP_SOUND_THROW]);
	}
	else{
		SendDlgItemMessage(hwnd, THROWSOUNDBOX, WM_SETTEXT, 0, (LPARAM) "");
	}

		if (pweapon->weapon.soundfilename[WEAP_SOUND_DROP] != NULL){
	SendDlgItemMessage(hwnd, DROPSOUNDBOX, WM_SETTEXT, 0, (LPARAM) pweapon->weapon.soundfilename[WEAP_SOUND_DROP]);
	}
	else{
		SendDlgItemMessage(hwnd, DROPSOUNDBOX, WM_SETTEXT, 0, (LPARAM) "");
	}


		if (pweapon->weapon.soundfilename[WEAP_SOUND_RELOAD] != NULL){
	SendDlgItemMessage(hwnd, RELOADSOUNDBOX, WM_SETTEXT, 0, (LPARAM) pweapon->weapon.soundfilename[WEAP_SOUND_RELOAD]);
	}
	else{
		SendDlgItemMessage(hwnd, RELOADSOUNDBOX, WM_SETTEXT, 0, (LPARAM) "");
	}

	if (pweapon->weapon.soundfilename[WEAP_SOUND_BREAK]!= NULL){
	SendDlgItemMessage(hwnd, BREAKSOUNDBOX, WM_SETTEXT, 0, (LPARAM) pweapon->weapon.soundfilename[WEAP_SOUND_BREAK]);
	}
	else{
		SendDlgItemMessage(hwnd, BREAKSOUNDBOX, WM_SETTEXT, 0, (LPARAM) "");
	}
	
}

b00l CALLBACK AmmoDialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	int returncode;
	char buf[200];
	
	switch (msg){
		
	case WM_DESTROY:
		if(ammolistallocated){
		freeList(ammolist);
		ammolistallocated = LIST_FREE;
		}
		EndDialog(hwnd, 1);
		return 1;
		
	case WM_INITDIALOG:
		returncode = MAEdLoadData(hwnd);
		if (returncode == -2){
			MessageBox(hwnd, "incorrect ammo.dat cannot be loaded. Please delete or repair the file and try again.", "PROBLEM...", MB_OK);
			SendMessage(hwnd, WM_DESTROY,0,0);
		}
		returncode = WEdLoadData();
		if(returncode == 0){
			weaponlistallocated = LIST_ALLOCATED;
		}
		returncode = MAEdPopulateData(hwnd);
		return 1;
		
	case WM_COMMAND:
		switch (LOWORD(wparam)){
			
		case MAINAMMOBOX:
			if (((int) LOWORD(wparam)) == MAINAMMOBOX && ((int) HIWORD (wparam)) == CBN_SELCHANGE){
				MAEdSelectMainAmmo(hwnd);
			}
			return 1;
			
		case IDQUITNOSAVE:
			SendMessage(hwnd, WM_DESTROY, 0,0);
			return 1;

		case EDITSUBAMMOBUTTON:
			returncode = MAEdCheckSubAmmoValues(hwnd);
			if (returncode){
				return 1;
			}
			returncode = SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_GETCURSEL, 0,0);
			if (returncode < 1){
				MessageBox(hwnd, "Invalid Ammo Type!", "Problem...", MB_OK);
				return 1;
			}
			pammo = (PPOTYPE) SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_GETITEMDATA, returncode,0);
			ShowWindow(hwnd, SW_HIDE);
			switch (pammo->ammo.type){
			
			case AMMOTYPE_BULLET:
					MAEdCreateBulletList();
					DialogBoxParam(((HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE)),  MAKEINTRESOURCE(BULLETDIALOG), hwnd,  (DLGPROC)BulletDialogProc, 0);
					if (bulletammolistallocated){
					freeList(bulletammolist);
					bulletammolistallocated = LIST_FREE;
					}
					break;
			case AMMOTYPE_SHOTGUNSHELL:
					MAEdCreateShellList();
					DialogBoxParam(((HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE)),  MAKEINTRESOURCE(SHELLDIALOG), hwnd,  (DLGPROC)ShellDialogProc, 0);
					if (shellammolistallocated){
					freeList(shellammolist);
					shellammolistallocated = LIST_FREE;
					}
					break;
					case AMMOTYPE_EXPLOSIVE:
					MAEdCreateExplosiveList();
					DialogBoxParam(((HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE)),  MAKEINTRESOURCE(EXPLOSIVEDIALOG), hwnd,  (DLGPROC)ExplosiveDialogProc, 0);
					if (explosiveammolistallocated){
					freeList(explosiveammolist);
					explosiveammolistallocated = LIST_FREE;
					}
					break;
			}
			ShowWindow(hwnd, SW_SHOW);
			MAEdClearFields(hwnd);
			return 1;

		case SUBMITNEWBUTTON:
			returncode = MAEdCheckNewValues(hwnd);
			if (returncode != -1){
				MAEdMakeNewMainAmmo(hwnd);
			}
			return 1;
			
		case CLEARBUTTON:
			MAEdClearFields(hwnd);
			return 1;

		case REMOVEBUTTON:
			returncode = SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_GETCURSEL, 0,0);
			if (returncode < 1){
				MessageBox(hwnd, "Cannot Remove the NONE Ammo Type!", "Problem...", MB_OK);
				return 1;
			}
			pammo = (PPOTYPE) SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_GETITEMDATA, returncode, 0);
			SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_DELETESTRING, returncode ,0);
			deleteObject(ammolist, pammo);
			if(weaponlistallocated == LIST_FREE){
			ReorderAmmoListID(ammolist, NULL);
			}
			else{
				ReorderAmmoListID(ammolist, weaponlist);
			}
			MAEdClearFields(hwnd);
			SendDlgItemMessage(hwnd, TOTALAMMOBOX, WM_GETTEXT, 200, (LPARAM) buf);
			wsprintf(buf, "%d", atoi(buf) -1);
			SendDlgItemMessage(hwnd, TOTALAMMOBOX, WM_SETTEXT, 0, (LPARAM)buf);
			return 1;
			

		case IDQUITSAVE:
			returncode = MAEdCheckWriteValues(hwnd);
			if (!returncode){
				returncode = MAEdWriteAmmoFile(hwnd);
				if (!returncode){
				MessageBox(hwnd, "Values Saved!" ,"Good Deal!", MB_OK);
				SendMessage(hwnd, WM_DESTROY, 0,0);
				return 1;
				}
				else{
					MessageBox(hwnd, "Values NOT Saved! Error!" ,"Problem", MB_OK);
					return 1;
				}
				return 1;
				
			}
			return 1;
			
		}
		
	}
	return 0;
}

b00l CALLBACK ShellDialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{

	int returncode;

		switch (msg){

		case WM_INITDIALOG:
			ShellEdPopulateData(hwnd);
			return 1;
	
	case WM_COMMAND:
		switch (LOWORD(wparam)){
	
				case SHELLEDITCOMBOBOX:
			if (((int) LOWORD(wparam)) == SHELLEDITCOMBOBOX && ((int) HIWORD (wparam)) == CBN_SELCHANGE){
				ShellEdSelectSubAmmo(hwnd);
			}
			return 1;
		case IDQUITNOSAVE:
			EndDialog(hwnd, 1);
			return (1);
		case REMOVEBUTTON:
				returncode = SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_GETCURSEL, 0,0);
			if (returncode < 1){
				MessageBox(hwnd, "Cannot delete the NONE ammo type!", "Problem...", MB_OK);
				return 1;
			}
			else{
				pshell = (PPOTYPE) SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_GETITEMDATA, returncode, 0);
				SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_DELETESTRING, returncode ,0);
				free(pshell->shell.desc);
				free(pshell->shell.impactsound);
				deleteObject(shellammolist, pshell);
				ReorderShellListID(shellammolist);
				ShellEdClearFields(hwnd);
			}
			return(1);
		case SUBMITNEWBUTTON:
			returncode = ShellEdCheckValuesNew(hwnd);
			if (!returncode){
				ShellEdCreateNewAmmo(hwnd);
			}
			return (1);
		case SUBMITMODIFIEDBUTTON:
			returncode = ShellEdCheckValuesModify(hwnd);
			if (!returncode){ //IF OK, GO!
				ShellEdModifyCurrentAmmo(hwnd);
			}
			return(1);
		case CLEARFIELDSBUTTON:
			ShellEdClearFields(hwnd);
			return 1;
		case IDQUITSAVE:
			ShellEdSaveAmmoList(hwnd);
			EndDialog(hwnd, 1);
			return 1;
		}
	}
	return 0;
}

void ShellEdPopulateData(HWND hwnd)
{
		/************************************************************
	; HEY ASSHOLE!!!!!!! MAKE SURE YOU PUT THE ***INDEX*** IN CB_SETITEMDATA AND NOT JUST ZERO!!!!!!
	************************************************************/
		/************************************************************
	; HEY ASSHOLE!!!!!!! MAKE SURE YOU PUT THE ***INDEX*** IN CB_SETITEMDATA AND NOT JUST ZERO!!!!!!
	************************************************************/
		/************************************************************
	; HEY ASSHOLE!!!!!!! MAKE SURE YOU PUT THE ***INDEX*** IN CB_SETITEMDATA AND NOT JUST ZERO!!!!!!
	************************************************************/

//	char buf[200];
	PLIST walker;
	int index = 1;

	//SET UP THE MAIN AMMO BOX
	SendDlgItemMessage(hwnd, MAINAMMONAME, WM_SETTEXT, 0, (LPARAM) pammo->ammo.name);
		//MAKE THE FIRST AMMO TYPE "NONE"
	SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_INSERTSTRING, -1, (LPARAM) "NONE");
	SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_SETITEMDATA, 0, (LPARAM) NULL);

	
	if (shellammolist){ //ONLY WALK THE LIST IF THERE IS ONE.
		//SendDlgItemMessage(hwnd, TOTALAMMOBOX, WM_SETTEXT, 0, ammolist->objcnt);
		walker = shellammolist;
		for (walker->current = shellammolist->head; walker->current != NULL; walker->current = walker->current->nextnode){
			SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_INSERTSTRING, -1, (LPARAM) walker->current->object.shell.longname);
			SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_SETITEMDATA, index, (LPARAM) &walker->current->object);
			index++;
		}
	}
	SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_SETCURSEL,0,0);
}

void ShellEdCreateNewAmmo(HWND hwnd)
{
		char buf[4000];
	int tempint;
	POTYPE tempshell;

	tempint = SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_GETCOUNT ,0,0);
	tempshell.shell.id = tempint;

		SendDlgItemMessage(hwnd, SHELLSHORTNAME, WM_GETTEXT, 4000, (LPARAM) buf);
	strncpy(tempshell.shell.shortname, buf, sizeof(tempshell.shell.shortname) - 1);

	SendDlgItemMessage(hwnd, SHELLLONGNAME, WM_GETTEXT, 4000, (LPARAM) buf);
	strncpy(tempshell.shell.longname, buf, sizeof(tempshell.shell.longname) - 1);

	SendDlgItemMessage(hwnd, SHELLDESCBOX, WM_GETTEXT, 4000, (LPARAM) buf);
//	if (tempshell.shell.desc != NULL){
//		free(tempshell.shell.desc);
//	}
	tempshell.shell.desc = (char*) malloc(sizeof(char) * ((strlen(buf)) +1));
	strcpy(tempshell.shell.desc, buf);

	SendDlgItemMessage(hwnd, SHELLSOUNDBOX, WM_GETTEXT, 4000, (LPARAM) buf);
//	if (tempshell.shell.impactsound != NULL){
//		free(tempshell.shell.impactsound);
//	}
	tempshell.shell.impactsound = (char*) malloc(sizeof(char) * ((strlen(buf)) +1));
	strcpy(tempshell.shell.impactsound, buf);

	tempint = SendDlgItemMessage(hwnd, SHELLCHECKDSAONLY, BM_GETCHECK,  0,0);
	if (tempint == BST_CHECKED){
		tempshell.shell.dsaammo = TRUE;
	}
	else{
		tempshell.shell.dsaammo = FALSE;
	}

	SendDlgItemMessage(hwnd, SHELLFREQUENCYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	tempshell.shell.frequency = tempint;


	SendDlgItemMessage(hwnd, SHELLSPREADBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	tempshell.shell.spread = tempint;


	SendDlgItemMessage(hwnd, SHELLAPBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	tempshell.shell.ap = tempint;

		SendDlgItemMessage(hwnd, SHELLENERGYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	tempshell.shell.energy = tempint;

		SendDlgItemMessage(hwnd, SHELLDAMAGEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	tempshell.shell.damage = tempint;


	tempshell.shell.id = shellammolist->objcnt + 1;
	addtoList(shellammolist, tempshell, CompareShellSubAmmoID);
	pshell = findObject(shellammolist, &tempshell, CompareShellSubAmmoID);

	SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_INSERTSTRING, -1, (LPARAM) pshell->shell.longname);
	tempint = SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_GETCOUNT, 0,0);
	SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_SETITEMDATA, tempint -1, (LPARAM) pshell);


	ShellEdClearFields(hwnd);


}

void ShellEdModifyCurrentAmmo(HWND hwnd)
{
	char buf[4000];
	int tempint;

	SendDlgItemMessage(hwnd, SHELLSHORTNAME, WM_GETTEXT, 4000, (LPARAM) buf);
	strncpy(pshell->shell.shortname, buf, sizeof(pshell->shell.shortname) - 1);

	SendDlgItemMessage(hwnd, SHELLLONGNAME, WM_GETTEXT, 4000, (LPARAM) buf);
	strncpy(pshell->shell.longname, buf, sizeof(pshell->shell.longname) - 1);

	SendDlgItemMessage(hwnd, SHELLDESCBOX, WM_GETTEXT, 4000, (LPARAM) buf);
//	if (pshell->shell.desc != NULL){
//		free(pshell->shell.desc);
//	}
	pshell->shell.desc = (char*) malloc(sizeof(char) * ((strlen(buf)) +1));
	strcpy(pshell->shell.desc, buf);

	SendDlgItemMessage(hwnd, SHELLSOUNDBOX, WM_GETTEXT, 4000, (LPARAM) buf);
//	if (pshell->shell.impactsound != NULL){
//		free(pshell->shell.impactsound);
//	}
	pshell->shell.impactsound = (char*) malloc(sizeof(char) * ((strlen(buf)) +1));
	strcpy(pshell->shell.impactsound, buf);

	tempint = SendDlgItemMessage(hwnd, SHELLCHECKDSAONLY, BM_GETCHECK, 0,0);
	if (tempint == BST_CHECKED){
		pshell->shell.dsaammo = TRUE;
	}
	else{
		pshell->shell.dsaammo = FALSE;
	}

	SendDlgItemMessage(hwnd, SHELLFREQUENCYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	pshell->shell.frequency = tempint;


	SendDlgItemMessage(hwnd, SHELLSPREADBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	pshell->shell.spread = tempint;


	SendDlgItemMessage(hwnd, SHELLAPBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	pshell->shell.ap = tempint;

		SendDlgItemMessage(hwnd, SHELLENERGYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	pshell->shell.energy = tempint;

		SendDlgItemMessage(hwnd, SHELLDAMAGEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	pshell->shell.damage = tempint;

//ENTER IT INTO THE COMBO BOX. RETRIEVE THE INDEX, DELETE IT, THEN ADD IT TO THE INDEX BEFORE IT, ESSENTIALLY REPLACING IT.

	tempint = SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_GETCURSEL, 0,0);
	SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_DELETESTRING, tempint,0);
	SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_INSERTSTRING, tempint, (LPARAM) pshell->shell.longname);
	SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_SETITEMDATA, tempint, (LPARAM) pshell);

	ShellEdClearFields(hwnd);

}

void ShellEdClearFields(HWND hwnd)
{
	pshell = NULL;
SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_SETCURSEL, 0,0);
SendDlgItemMessage(hwnd, SHELLFREQUENCYBOX, WM_SETTEXT, 0, (LPARAM)NULL);
SendDlgItemMessage(hwnd, SHELLSPREADBOX, WM_SETTEXT, 0, (LPARAM)NULL);
SendDlgItemMessage(hwnd, SHELLAPBOX, WM_SETTEXT, 0, (LPARAM)NULL);
SendDlgItemMessage(hwnd, SHELLENERGYBOX, WM_SETTEXT, 0, (LPARAM)NULL);
SendDlgItemMessage(hwnd, SHELLDAMAGEBOX, WM_SETTEXT, 0, (LPARAM)NULL);
SendDlgItemMessage(hwnd, SHELLSHORTNAME, WM_SETTEXT, 0, (LPARAM)NULL);
SendDlgItemMessage(hwnd, SHELLLONGNAME, WM_SETTEXT, 0,(LPARAM)NULL);
SendDlgItemMessage(hwnd, SHELLDESCBOX, WM_SETTEXT, 0, (LPARAM)NULL);
SendDlgItemMessage(hwnd, SHELLSOUNDBOX, WM_SETTEXT, 0, (LPARAM)NULL);
SendDlgItemMessage(hwnd, SHELLCHECKDSAONLY, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
}

void ShellEdSelectSubAmmo(HWND hwnd)
{
int index; 

char buf[200];

index = SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_GETCURSEL,0,0);

if (index < 1){ //IF IT'S NONE...
	ShellEdClearFields(hwnd);
	return;
}
pshell = (PPOTYPE) SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_GETITEMDATA, index, 0);
SendDlgItemMessage(hwnd, SHELLSHORTNAME, WM_SETTEXT, 0, (LPARAM) pshell->shell.shortname);
SendDlgItemMessage(hwnd, SHELLLONGNAME, WM_SETTEXT, 0, (LPARAM) pshell->shell.longname);
SendDlgItemMessage(hwnd, SHELLDESCBOX, WM_SETTEXT, 0, (LPARAM) pshell->shell.desc);
SendDlgItemMessage(hwnd, SHELLSOUNDBOX, WM_SETTEXT, 0, (LPARAM) pshell->shell.impactsound);

if (pshell->shell.dsaammo == TRUE){
SendDlgItemMessage(hwnd, SHELLCHECKDSAONLY, BM_SETCHECK, (WPARAM) BST_CHECKED, 0);
}
else{
	SendDlgItemMessage(hwnd, SHELLCHECKDSAONLY, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
}
wsprintf(buf, "%d", pshell->shell.frequency);
SendDlgItemMessage(hwnd, SHELLFREQUENCYBOX, WM_SETTEXT, 0, (LPARAM) buf);
wsprintf(buf, "%d", pshell->shell.spread);
SendDlgItemMessage(hwnd, SHELLSPREADBOX, WM_SETTEXT, 0, (LPARAM) buf);
wsprintf(buf, "%d", pshell->shell.ap);
SendDlgItemMessage(hwnd, SHELLAPBOX, WM_SETTEXT, 0, (LPARAM) buf);
wsprintf(buf, "%d", pshell->shell.energy);
SendDlgItemMessage(hwnd, SHELLENERGYBOX, WM_SETTEXT, 0, (LPARAM) buf);
wsprintf(buf, "%d", pshell->shell.damage);
SendDlgItemMessage(hwnd, SHELLDAMAGEBOX, WM_SETTEXT, 0, (LPARAM) buf);
}

void MAEdClearFields(HWND hwnd)
{
	SendDlgItemMessage(hwnd, SUBAMMOTYPESBOX, WM_SETTEXT, 0, (LPARAM)NULL);
	SendDlgItemMessage(hwnd, AMMONAME, WM_SETTEXT, 0, (LPARAM)NULL);
	SendDlgItemMessage(hwnd, SUBAMMOTYPE, CB_SETCURSEL, 0, 0);
	SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_SETCURSEL, 0,0);
	SendDlgItemMessage(hwnd, CURRENTAMMOBOX, WM_SETTEXT,0, (LPARAM) NULL);
	pammo = NULL; //SET THE POINTER TO THE AMMO TO NULL, KEEP ANYTHING BAD FROM HAPPENING.
}

int ShellEdCheckValuesNew(HWND hwnd)
{
	char buf[200];
	int tempint;

	SendDlgItemMessage(hwnd, SHELLSHORTNAME, WM_GETTEXT, 200, (LPARAM) buf);
	if (!strcmp(buf, "")){
		MessageBox(hwnd, "Shell Must Have a Short Name!", "PROBLEM...", MB_OK);
		return -1;
	}
	SendDlgItemMessage(hwnd, SHELLLONGNAME, WM_GETTEXT, 200, (LPARAM) buf);
	if (!strcmp(buf, "")){
		MessageBox(hwnd, "Shell Must Have a Long Name!", "PROBLEM...", MB_OK);
		return -1;
	}
	SendDlgItemMessage(hwnd, SHELLFREQUENCYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 1 || tempint > 100){
		MessageBox(hwnd, "Frequency Must Be Between 1 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
		SendDlgItemMessage(hwnd, SHELLSPREADBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Spread Must Be Between 0 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
	SendDlgItemMessage(hwnd, SHELLAPBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 1 || tempint > 100){
		MessageBox(hwnd, "AP Must Be Between 0 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
		SendDlgItemMessage(hwnd, SHELLENERGYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 1 || tempint > 100){
		MessageBox(hwnd, "Energy Must Be Between 0 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
		SendDlgItemMessage(hwnd, SHELLDAMAGEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 1 || tempint > 100){
		MessageBox(hwnd, "Damage Must Be Between 0 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
	return 0;
}

int ShellEdCheckValuesModify(HWND hwnd)
{
		char buf[200];
	int tempint;

	tempint = SendDlgItemMessage(hwnd, SHELLEDITCOMBOBOX, CB_GETCURSEL,0,0);
	if (tempint < 1){
		MessageBox(hwnd, "A Sub-Ammo must be selected to modify!", "PROBLEM!", MB_OK);
		return -1;
	}
	SendDlgItemMessage(hwnd, SHELLSHORTNAME, WM_GETTEXT, 200, (LPARAM) buf);
	if (!strcmp(buf, "")){
		MessageBox(hwnd, "Shell Must Have a Short Name!", "PROBLEM...", MB_OK);
		return -1;
	}
	SendDlgItemMessage(hwnd, SHELLLONGNAME, WM_GETTEXT, 200, (LPARAM) buf);
	if (!strcmp(buf, "")){
		MessageBox(hwnd, "Shell Must Have a Long Name!", "PROBLEM...", MB_OK);
		return -1;
	}
	SendDlgItemMessage(hwnd, SHELLFREQUENCYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 1 || tempint > 100){
		MessageBox(hwnd, "Frequency Must Be Between 1 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
		SendDlgItemMessage(hwnd, SHELLSPREADBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Spread Must Be Between 0 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
	SendDlgItemMessage(hwnd, SHELLAPBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "AP Must Be Between 0 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
		SendDlgItemMessage(hwnd, SHELLENERGYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Energy Must Be Between 0 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
		SendDlgItemMessage(hwnd, SHELLDAMAGEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Damage Must Be Between 0 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
	return 0;
}

void ShellEdSaveAmmoList(HWND hwnd)
{
	PLIST walker;
	int index = 0;

	
	if (shellammolist){ //ONLY WALK THE LIST IF THERE IS ONE.
		//SendDlgItemMessage(hwnd, TOTALAMMOBOX, WM_SETTEXT, 0, ammolist->objcnt);
		walker = shellammolist;

		
	if (pammo->ammo.maxammotypes > 0){
		free(pammo->ammo.specialammo.shelldata);
	}
	pammo->ammo.maxammotypes = shellammolist->objcnt;
	pammo->ammo.specialammo.shelldata = (PSHELL) malloc(sizeof(SHELL) * pammo->ammo.maxammotypes);

		for (walker->current = shellammolist->head; walker->current != NULL; walker->current = walker->current->nextnode){
			pammo->ammo.specialammo.shelldata[index] = walker->current->object.shell;
			index++;
		}
	}
}


b00l CALLBACK BulletDialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	int returncode;

		switch (msg){
	case WM_INITDIALOG:
			BulletEdPopulateData(hwnd);
			return 1;
	
	case WM_COMMAND:
		switch (LOWORD(wparam)){
	
				case BULLETEDITCOMBOBOX:
			if (((int) LOWORD(wparam)) == BULLETEDITCOMBOBOX && ((int) HIWORD (wparam)) == CBN_SELCHANGE){
				BulletEdSelectSubAmmo(hwnd);
			}
			return 1;
		case IDQUITNOSAVE:
			EndDialog(hwnd, 1);
			return (1);
		case REMOVEBUTTON:
				returncode = SendDlgItemMessage(hwnd, BULLETEDITCOMBOBOX, CB_GETCURSEL, 0,0);
			if (returncode < 1){
				MessageBox(hwnd, "Cannot delete the NONE ammo type!", "Problem...", MB_OK);
				return 1;
			}
			else{
				pbullet = (PPOTYPE) SendDlgItemMessage(hwnd, BULLETEDITCOMBOBOX, CB_GETITEMDATA, returncode, 0);
				SendDlgItemMessage(hwnd, BULLETEDITCOMBOBOX, CB_DELETESTRING, returncode ,0);
				free(pbullet->bullet.desc);
				free(pbullet->bullet.impactsound);
				deleteObject(bulletammolist, pbullet);
				ReorderBulletListID(bulletammolist);
				BulletEdClearFields(hwnd);
			}
			return(1);
		case SUBMITNEWBUTTON:
			returncode = BulletEdCheckValuesNew(hwnd);
			if (!returncode){
				BulletEdCreateNewAmmo(hwnd);
			}
			return (1);
		case SUBMITMODIFIEDBUTTON:
			returncode = BulletEdCheckValuesModify(hwnd);
			if (!returncode){ //IF OK, GO!
				BulletEdModifyCurrentAmmo(hwnd);
			}
			return(1);
		case CLEARFIELDSBUTTON:
			BulletEdClearFields(hwnd);
			return 1;
		case IDQUITSAVE:
			BulletEdSaveAmmoList(hwnd);
			EndDialog(hwnd, 1);
			return 1;
		}
	}
	return 0;
}
void BulletEdPopulateData(HWND hwnd)
{
		/************************************************************
	; HEY ASSHOLE!!!!!!! MAKE SURE YOU PUT THE ***INDEX*** IN CB_SETITEMDATA AND NOT JUST ZERO!!!!!!
	************************************************************/
		/************************************************************
	; HEY ASSHOLE!!!!!!! MAKE SURE YOU PUT THE ***INDEX*** IN CB_SETITEMDATA AND NOT JUST ZERO!!!!!!
	************************************************************/
		/************************************************************
	; HEY ASSHOLE!!!!!!! MAKE SURE YOU PUT THE ***INDEX*** IN CB_SETITEMDATA AND NOT JUST ZERO!!!!!!
	************************************************************/

//	char buf[200];
	PLIST walker;
	int index = 1;

	//SET UP THE MAIN AMMO BOX
	SendDlgItemMessage(hwnd, MAINAMMONAME, WM_SETTEXT, 0, (LPARAM) pammo->ammo.name);
		//MAKE THE FIRST AMMO TYPE "NONE"
	SendDlgItemMessage(hwnd, BULLETEDITCOMBOBOX, CB_INSERTSTRING, -1, (LPARAM) "NONE");
	SendDlgItemMessage(hwnd, BULLETEDITCOMBOBOX, CB_SETITEMDATA, 0, (LPARAM) NULL);

	
	if (bulletammolist){ //ONLY WALK THE LIST IF THERE IS ONE.
		//SendDlgItemMessage(hwnd, TOTALAMMOBOX, WM_SETTEXT, 0, ammolist->objcnt);
		walker = bulletammolist;
		for (walker->current = bulletammolist->head; walker->current != NULL; walker->current = walker->current->nextnode){
			SendDlgItemMessage(hwnd, BULLETEDITCOMBOBOX, CB_INSERTSTRING, -1, (LPARAM) walker->current->object.bullet.longname);
			SendDlgItemMessage(hwnd, BULLETEDITCOMBOBOX, CB_SETITEMDATA, index, (LPARAM) &walker->current->object);
			index++;
		}
	}
	SendDlgItemMessage(hwnd, BULLETEDITCOMBOBOX, CB_SETCURSEL,0,0);
}


int BulletEdCheckValuesNew(HWND hwnd)
{
	char buf[200];
	int tempint;

	SendDlgItemMessage(hwnd, BULLETSHORTNAME, WM_GETTEXT, 200, (LPARAM) buf);
	if (!strcmp(buf, "")){
		MessageBox(hwnd, "Bullet Must Have a Short Name!", "PROBLEM...", MB_OK);
		return -1;
	}
	SendDlgItemMessage(hwnd, BULLETLONGNAME, WM_GETTEXT, 200, (LPARAM) buf);
	if (!strcmp(buf, "")){
		MessageBox(hwnd, "Bullet Must Have a Long Name!", "PROBLEM...", MB_OK);
		return -1;
	}
	SendDlgItemMessage(hwnd, BULLETFREQUENCYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 1 || tempint > 100){
		MessageBox(hwnd, "Frequency Must Be Between 1 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
		SendDlgItemMessage(hwnd, BULLETLATENTDAMAGEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Latent Damage Must be Between 0 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
	SendDlgItemMessage(hwnd, BULLETAPBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "AP Must Be Between 0 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
		SendDlgItemMessage(hwnd, BULLETENERGYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Energy Must Be Between 0 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
		SendDlgItemMessage(hwnd, BULLETDAMAGEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Damage Must Be Between 0 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
	return 0;
}
int BulletEdCheckValuesModify(HWND hwnd)
{
		char buf[200];
	int tempint;

		tempint = SendDlgItemMessage(hwnd, BULLETEDITCOMBOBOX, CB_GETCURSEL,0,0);
	if (tempint < 1){
		MessageBox(hwnd, "A Sub-Ammo must be selected to modify!", "PROBLEM!", MB_OK);
		return -1;
	}
	SendDlgItemMessage(hwnd, BULLETSHORTNAME, WM_GETTEXT, 200, (LPARAM) buf);
	if (!strcmp(buf, "")){
		MessageBox(hwnd, "Bullet Must Have a Short Name!", "PROBLEM...", MB_OK);
		return -1;
	}
	SendDlgItemMessage(hwnd, BULLETLONGNAME, WM_GETTEXT, 200, (LPARAM) buf);
	if (!strcmp(buf, "")){
		MessageBox(hwnd, "Bullet Must Have a Long Name!", "PROBLEM...", MB_OK);
		return -1;
	}
	SendDlgItemMessage(hwnd, BULLETFREQUENCYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 1 || tempint > 100){
		MessageBox(hwnd, "Frequency Must Be Between 1 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
		SendDlgItemMessage(hwnd, BULLETLATENTDAMAGEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Latent Damage Must be Between 0 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
	SendDlgItemMessage(hwnd, BULLETAPBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "AP Must Be Between 0 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
		SendDlgItemMessage(hwnd, BULLETENERGYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Energy Must Be Between 0 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
		SendDlgItemMessage(hwnd, BULLETDAMAGEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Damage Must Be Between 0 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}
	return 0;
}

void BulletEdSelectSubAmmo(HWND hwnd)
{
	int index; 

char buf[200];

index = SendDlgItemMessage(hwnd, BULLETEDITCOMBOBOX, CB_GETCURSEL,0,0);

if (index < 1){ //IF IT'S NONE...
	BulletEdClearFields(hwnd);
	return;
}
pbullet = (PPOTYPE) SendDlgItemMessage(hwnd, BULLETEDITCOMBOBOX, CB_GETITEMDATA, index, 0);
SendDlgItemMessage(hwnd, BULLETSHORTNAME, WM_SETTEXT, 0, (LPARAM) pbullet->bullet.shortname);
SendDlgItemMessage(hwnd, BULLETLONGNAME, WM_SETTEXT, 0, (LPARAM) pbullet->bullet.longname);
SendDlgItemMessage(hwnd, BULLETDESCBOX, WM_SETTEXT, 0, (LPARAM) pbullet->bullet.desc);
SendDlgItemMessage(hwnd, BULLETSOUNDBOX, WM_SETTEXT, 0, (LPARAM) pbullet->bullet.impactsound);

if (pbullet->bullet.dsaammo == TRUE){
SendDlgItemMessage(hwnd, BULLETCHECKDSAONLY, BM_SETCHECK, (WPARAM) BST_CHECKED, 0);
}
else{
	SendDlgItemMessage(hwnd, BULLETCHECKDSAONLY, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
}
wsprintf(buf, "%d", pbullet->bullet.frequency);
SendDlgItemMessage(hwnd, BULLETFREQUENCYBOX, WM_SETTEXT, 0, (LPARAM) buf);
wsprintf(buf, "%d", pbullet->bullet.latentdamage);
SendDlgItemMessage(hwnd, BULLETLATENTDAMAGEBOX, WM_SETTEXT, 0, (LPARAM) buf);
wsprintf(buf, "%d", pbullet->bullet.ap);
SendDlgItemMessage(hwnd, BULLETAPBOX, WM_SETTEXT, 0, (LPARAM) buf);
wsprintf(buf, "%d", pbullet->bullet.energy);
SendDlgItemMessage(hwnd, BULLETENERGYBOX, WM_SETTEXT, 0, (LPARAM) buf);
wsprintf(buf, "%d", pbullet->bullet.damage);
SendDlgItemMessage(hwnd, BULLETDAMAGEBOX, WM_SETTEXT, 0, (LPARAM) buf);
}
void BulletEdClearFields(HWND hwnd)
{
		pbullet = NULL;
SendDlgItemMessage(hwnd, BULLETEDITCOMBOBOX, CB_SETCURSEL, 0,0);
SendDlgItemMessage(hwnd, BULLETFREQUENCYBOX, WM_SETTEXT, 0, (long)NULL);
SendDlgItemMessage(hwnd, BULLETLATENTDAMAGEBOX, WM_SETTEXT, 0, (long)NULL);
SendDlgItemMessage(hwnd, BULLETAPBOX, WM_SETTEXT, 0, (long)NULL);
SendDlgItemMessage(hwnd, BULLETENERGYBOX, WM_SETTEXT, 0, (long)NULL);
SendDlgItemMessage(hwnd, BULLETDAMAGEBOX, WM_SETTEXT, 0, (long)NULL);
SendDlgItemMessage(hwnd, BULLETSHORTNAME, WM_SETTEXT, 0, (long)NULL);
SendDlgItemMessage(hwnd, BULLETLONGNAME, WM_SETTEXT, 0,(long) NULL);
SendDlgItemMessage(hwnd, BULLETDESCBOX, WM_SETTEXT, 0, (long)NULL);
SendDlgItemMessage(hwnd, BULLETSOUNDBOX, WM_SETTEXT, 0, (long)NULL);
SendDlgItemMessage(hwnd, BULLETCHECKDSAONLY, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
}
void BulletEdModifyCurrentAmmo(HWND hwnd)
{
		char buf[4000];
	int tempint;

	SendDlgItemMessage(hwnd, BULLETSHORTNAME, WM_GETTEXT, 4000, (LPARAM) buf);
	strncpy(pbullet->bullet.shortname, buf, sizeof(pbullet->bullet.shortname) - 1);

	SendDlgItemMessage(hwnd, BULLETLONGNAME, WM_GETTEXT, 4000, (LPARAM) buf);
	strncpy(pbullet->bullet.longname, buf, sizeof(pbullet->bullet.longname) - 1);

	SendDlgItemMessage(hwnd, BULLETDESCBOX, WM_GETTEXT, 4000, (LPARAM) buf);
//	if (pbullet->bullet.desc != NULL){
//		free(pbullet->bullet.desc);
//	}
	pbullet->bullet.desc = (char*) malloc(sizeof(char) * ((strlen(buf)) +1));
	strcpy(pbullet->bullet.desc, buf);

	SendDlgItemMessage(hwnd, BULLETSOUNDBOX, WM_GETTEXT, 4000, (LPARAM) buf);
//	if (pbullet->bullet.impactsound != NULL){
//		free(pbullet->bullet.impactsound);
//	}
	pbullet->bullet.impactsound = (char*) malloc(sizeof(char) * ((strlen(buf)) +1));
	strcpy(pbullet->bullet.impactsound, buf);

	tempint = SendDlgItemMessage(hwnd, BULLETCHECKDSAONLY, BM_GETCHECK, 0,0);
	if (tempint == BST_CHECKED){
		pbullet->bullet.dsaammo = TRUE;
	}
	else{
		pbullet->bullet.dsaammo = FALSE;
	}

	SendDlgItemMessage(hwnd, BULLETFREQUENCYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	pbullet->bullet.frequency = tempint;


	SendDlgItemMessage(hwnd, BULLETLATENTDAMAGEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	pbullet->bullet.latentdamage = tempint;


	SendDlgItemMessage(hwnd, BULLETAPBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	pbullet->bullet.ap = tempint;

		SendDlgItemMessage(hwnd, BULLETENERGYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	pbullet->bullet.energy = tempint;

		SendDlgItemMessage(hwnd, BULLETDAMAGEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	pbullet->bullet.damage = tempint;

//ENTER IT INTO THE COMBO BOX. RETRIEVE THE INDEX, DELETE IT, THEN ADD IT TO THE INDEX BEFORE IT, ESSENTIALLY REPLACING IT.

	tempint = SendDlgItemMessage(hwnd, BULLETEDITCOMBOBOX, CB_GETCURSEL, 0,0);
	SendDlgItemMessage(hwnd, BULLETEDITCOMBOBOX, CB_DELETESTRING, tempint,0);
	SendDlgItemMessage(hwnd, BULLETEDITCOMBOBOX, CB_INSERTSTRING, tempint, (LPARAM) pbullet->bullet.longname);
	SendDlgItemMessage(hwnd, BULLETEDITCOMBOBOX, CB_SETITEMDATA, tempint, (LPARAM) pbullet);
	BulletEdClearFields(hwnd);
}
void BulletEdCreateNewAmmo(HWND hwnd)
{
			char buf[4000];
	int tempint;
	POTYPE temp;

	SendDlgItemMessage(hwnd, BULLETSHORTNAME, WM_GETTEXT, 4000, (LPARAM) buf);
	strncpy(temp.bullet.shortname, buf, sizeof(temp.bullet.shortname) - 1);

	SendDlgItemMessage(hwnd, BULLETLONGNAME, WM_GETTEXT, 4000, (LPARAM) buf);
	strncpy(temp.bullet.longname, buf, sizeof(temp.bullet.longname) - 1);

	SendDlgItemMessage(hwnd, BULLETDESCBOX, WM_GETTEXT, 4000, (LPARAM) buf);
//	if (temp.bullet.desc != NULL){
//		free(temp.bullet.desc);
//	}
	temp.bullet.desc = (char*) malloc(sizeof(char) * ((strlen(buf)) + 1));
	strcpy(temp.bullet.desc, buf);

	SendDlgItemMessage(hwnd, BULLETSOUNDBOX, WM_GETTEXT, 4000, (LPARAM) buf);
//	if (temp.bullet.impactsound != NULL){
//		free(temp.bullet.impactsound);
//	}
	temp.bullet.impactsound = (char*) malloc(sizeof(char) * ((strlen(buf)) + 1));
	strcpy(temp.bullet.impactsound, buf);

	tempint = SendDlgItemMessage(hwnd, BULLETCHECKDSAONLY, BM_GETCHECK, 0,0);
	if (tempint == BST_CHECKED){
		temp.bullet.dsaammo = TRUE;
	}
	else{
		temp.bullet.dsaammo = FALSE;
	}

	SendDlgItemMessage(hwnd, BULLETFREQUENCYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	temp.bullet.frequency = tempint;


	SendDlgItemMessage(hwnd, BULLETLATENTDAMAGEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	temp.bullet.latentdamage = tempint;


	SendDlgItemMessage(hwnd, BULLETAPBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	temp.bullet.ap = tempint;

		SendDlgItemMessage(hwnd, BULLETENERGYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	temp.bullet.energy = tempint;

		SendDlgItemMessage(hwnd, BULLETDAMAGEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	temp.bullet.damage = tempint;

	temp.bullet.id = bulletammolist->objcnt + 1;

	addtoList(bulletammolist, temp, CompareBulletSubAmmoID);
	pbullet = findObject(bulletammolist, &temp, CompareBulletSubAmmoID);

	SendDlgItemMessage(hwnd, BULLETEDITCOMBOBOX, CB_INSERTSTRING, -1, (LPARAM) pbullet->bullet.longname);
	tempint = SendDlgItemMessage(hwnd, BULLETEDITCOMBOBOX, CB_GETCOUNT, 0,0);
	SendDlgItemMessage(hwnd, BULLETEDITCOMBOBOX, CB_SETITEMDATA, tempint -1, (LPARAM) pbullet);


	BulletEdClearFields(hwnd);
}
void BulletEdSaveAmmoList(HWND hwnd)
{
		PLIST walker;
	int index = 0;

	
	if (bulletammolist){ //ONLY WALK THE LIST IF THERE IS ONE.
		//SendDlgItemMessage(hwnd, TOTALAMMOBOX, WM_SETTEXT, 0, ammolist->objcnt);
		walker = bulletammolist;

		
	if (pammo->ammo.maxammotypes > 0){
		free(pammo->ammo.specialammo.bulletdata);
	}
	pammo->ammo.maxammotypes = bulletammolist->objcnt;
	pammo->ammo.specialammo.bulletdata = (PBULLET) malloc(sizeof(BULLET) * pammo->ammo.maxammotypes);

		for (walker->current = bulletammolist->head; walker->current != NULL; walker->current = walker->current->nextnode){
			pammo->ammo.specialammo.bulletdata[index] = walker->current->object.bullet;
			index++;
		}
	}
}

b00l CALLBACK ExplosiveDialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	int returncode;

		switch (msg){
	case WM_INITDIALOG:
			ExplosiveEdPopulateData(hwnd);
			ExplosiveEdClearFields(hwnd);
			return 1;
	
	case WM_COMMAND:
		switch (LOWORD(wparam)){
	
				case EXPLOSIVEEDITCOMBOBOX:
			if (((int) LOWORD(wparam)) == EXPLOSIVEEDITCOMBOBOX && ((int) HIWORD (wparam)) == CBN_SELCHANGE){
				ExplosiveEdSelectSubAmmo(hwnd);
			}
			return 1;
		case IDQUITNOSAVE:
			EndDialog(hwnd, 1);
			return (1);
		case REMOVEBUTTON:
				returncode = SendDlgItemMessage(hwnd, EXPLOSIVEEDITCOMBOBOX, CB_GETCURSEL, 0,0);
			if (returncode < 1){
				MessageBox(hwnd, "Cannot delete the NONE ammo type!", "Problem...", MB_OK);
				return 1;
			}
			else{
				pexplosive = (PPOTYPE) SendDlgItemMessage(hwnd, EXPLOSIVEEDITCOMBOBOX, CB_GETITEMDATA, returncode, 0);
				SendDlgItemMessage(hwnd, EXPLOSIVEEDITCOMBOBOX, CB_DELETESTRING, returncode ,0);
				free(pexplosive->explosive.desc);
				free(pexplosive->explosive.impactsound);
				deleteObject(explosiveammolist, pexplosive);
				ReorderExplosiveListID(explosiveammolist);
				ExplosiveEdClearFields(hwnd);
			}
			return(1);
		case SUBMITNEWBUTTON:
			returncode = ExplosiveEdCheckValuesNew(hwnd);
			if (!returncode){
				ExplosiveEdCreateNewAmmo(hwnd);
			}
			return (1);
		case SUBMITMODIFIEDBUTTON:
			returncode = ExplosiveEdCheckValuesModify(hwnd);
			if (!returncode){ //IF OK, GO!
				ExplosiveEdModifyCurrentAmmo(hwnd);
			}
			return(1);
		case CLEARFIELDSBUTTON:
			ExplosiveEdClearFields(hwnd);
			return 1;
		case IDQUITSAVE:
			ExplosiveEdSaveAmmoList(hwnd);
			EndDialog(hwnd, 1);
			return 1;
		}
	}
	return 0;
}

int ExplosiveEdCheckValuesNew(HWND hwnd)
{

		char buf[4000];
	int tempint;

		SendDlgItemMessage(hwnd, EXPLOSIVESHORTNAME, WM_GETTEXT, 200, (LPARAM) buf);
	if (!strcmp(buf, "")){
		MessageBox(hwnd, "Explosive Must Have a Short Name!", "PROBLEM...", MB_OK);
		return -1;
	}
	SendDlgItemMessage(hwnd, EXPLOSIVELONGNAME, WM_GETTEXT, 200, (LPARAM) buf);
	if (!strcmp(buf, "")){
		MessageBox(hwnd, "Explosive Must Have a Long Name!", "PROBLEM...", MB_OK);
		return -1;
	}
	
	/*SendDlgItemMessage(hwnd, BULLETFREQUENCYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 1 || tempint > 100){
		MessageBox(hwnd, "Frequency Must Be Between 1 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}*/

	SendDlgItemMessage(hwnd, EXPLOSIVERANGEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Range must be between 0 and 100 - 0 signifies a rocket.", "Problem...", MB_OK);
		return -1;
	}

	SendDlgItemMessage(hwnd, EXPLOSIVEDELAYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Delay must be between 0 and 100 - 0 signifies detonate-on-impact fuse.", "Problem...", MB_OK);
		return -1;
	}


	SendDlgItemMessage(hwnd, EXPLOSIVEINITRADIUSBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Initial radius must be between 0 and 100!", "Problem...", MB_OK);
		return -1;
	}

	SendDlgItemMessage(hwnd, EXPLOSIVEINITSTRENGTHBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
		if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Initial strength must be between 0 and 100!", "Problem...", MB_OK);
		return -1;
	}

		SendDlgItemMessage(hwnd, EXPLOSIVELATENTRADIUSBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Latent radius must be between 0 and 100!", "Problem...", MB_OK);
		return -1;
	}

	SendDlgItemMessage(hwnd, EXPLOSIVELATENTTIMEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
		if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Latent time must be between 0 and 100!", "Problem...", MB_OK);
		return -1;
	}

		return 0; //OK
}
int ExplosiveEdCheckValuesModify(HWND hwnd)
{
			char buf[4000];
	int tempint;

	tempint = SendDlgItemMessage(hwnd, EXPLOSIVEEDITCOMBOBOX, CB_GETCURSEL,0,0);
	if (tempint < 1){
		MessageBox(hwnd, "A Sub-Ammo must be selected to modify!", "PROBLEM!", MB_OK);
		return -1;
	}
	SendDlgItemMessage(hwnd, EXPLOSIVESHORTNAME, WM_GETTEXT, 200, (LPARAM) buf);
	if (!strcmp(buf, "")){
		MessageBox(hwnd, "Explosive Must Have a Short Name!", "PROBLEM...", MB_OK);
		return -1;
	}
	SendDlgItemMessage(hwnd, EXPLOSIVELONGNAME, WM_GETTEXT, 200, (LPARAM) buf);
	if (!strcmp(buf, "")){
		MessageBox(hwnd, "Explosive Must Have a Long Name!", "PROBLEM...", MB_OK);
		return -1;
	}
	
	/*SendDlgItemMessage(hwnd, BULLETFREQUENCYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 1 || tempint > 100){
		MessageBox(hwnd, "Frequency Must Be Between 1 and 100!", "PROBLEM...", MB_OK);
		return -1;
	}*/

	SendDlgItemMessage(hwnd, EXPLOSIVERANGEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Range must be between 0 and 100 - 0 signifies a rocket.", "Problem...", MB_OK);
		return -1;
	}

	SendDlgItemMessage(hwnd, EXPLOSIVEDELAYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Delay must be between 0 and 100 - 0 signifies detonate-on-impact fuse.", "Problem...", MB_OK);
		return -1;
	}


	SendDlgItemMessage(hwnd, EXPLOSIVEINITRADIUSBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Initial radius must be between 0 and 100!", "Problem...", MB_OK);
		return -1;
	}

	SendDlgItemMessage(hwnd, EXPLOSIVEINITSTRENGTHBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
		if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Initial strength must be between 0 and 100!", "Problem...", MB_OK);
		return -1;
	}

		SendDlgItemMessage(hwnd, EXPLOSIVELATENTRADIUSBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Latent radius must be between 0 and 100!", "Problem...", MB_OK);
		return -1;
	}

	SendDlgItemMessage(hwnd, EXPLOSIVELATENTTIMEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
		if (tempint < 0 || tempint > 100){
		MessageBox(hwnd, "Latent time must be between 0 and 100!", "Problem...", MB_OK);
		return -1;
	}

		return 0; //OK
}
void ExplosiveEdPopulateData(HWND hwnd)
{
	char inittypes[4][60] = {"NONE", "CONCUSSION", "PUNCTURE", "FLASH"};
	char latenttypes[4][60] = {"NONE", "GAS", "SMOKE", "FIRE"};
			/************************************************************
	; HEY ASSHOLE!!!!!!! MAKE SURE YOU PUT THE ***INDEX*** IN CB_SETITEMDATA AND NOT JUST ZERO!!!!!!
	************************************************************/
		/************************************************************
	; HEY ASSHOLE!!!!!!! MAKE SURE YOU PUT THE ***INDEX*** IN CB_SETITEMDATA AND NOT JUST ZERO!!!!!!
	************************************************************/
		/************************************************************
	; HEY ASSHOLE!!!!!!! MAKE SURE YOU PUT THE ***INDEX*** IN CB_SETITEMDATA AND NOT JUST ZERO!!!!!!
	************************************************************/

//	char buf[200];
	PLIST walker;
	int index = 1;
	int i;

	//SET UP THE MAIN AMMO BOX
	SendDlgItemMessage(hwnd, MAINAMMONAME, WM_SETTEXT, 0, (LPARAM) pammo->ammo.name);
		//MAKE THE FIRST AMMO TYPE "NONE"
	SendDlgItemMessage(hwnd, EXPLOSIVEEDITCOMBOBOX, CB_INSERTSTRING, -1, (LPARAM) "NONE");
	SendDlgItemMessage(hwnd, EXPLOSIVEEDITCOMBOBOX, CB_SETITEMDATA, 0, (LPARAM) NULL);

	
	if (explosiveammolist){ //ONLY WALK THE LIST IF THERE IS ONE.
		//SendDlgItemMessage(hwnd, TOTALAMMOBOX, WM_SETTEXT, 0, ammolist->objcnt);
		walker = explosiveammolist;
		for (walker->current = explosiveammolist->head; walker->current != NULL; walker->current = walker->current->nextnode){
			SendDlgItemMessage(hwnd, EXPLOSIVEEDITCOMBOBOX, CB_INSERTSTRING, -1, (LPARAM) walker->current->object.explosive.longname);
			SendDlgItemMessage(hwnd, EXPLOSIVEEDITCOMBOBOX, CB_SETITEMDATA, index, (LPARAM) &walker->current->object);
			index++;
		}
	}
	SendDlgItemMessage(hwnd, EXPLOSIVEEDITCOMBOBOX, CB_SETCURSEL,0,0);

	for (i = 0; i < 4; i++){
		SendDlgItemMessage(hwnd, EXPLOSIVEINITTYPEBOX, CB_INSERTSTRING, -1, (LPARAM) inittypes[i]);
		SendDlgItemMessage(hwnd, EXPLOSIVELATENTTYPEBOX, CB_INSERTSTRING, -1, (LPARAM) latenttypes[i]);
	}
}
void ExplosiveEdSelectSubAmmo(HWND hwnd)
{
		int index; 

char buf[200];

index = SendDlgItemMessage(hwnd, EXPLOSIVEEDITCOMBOBOX, CB_GETCURSEL,0,0);

if (index < 1){ //IF IT'S NONE...
	ExplosiveEdClearFields(hwnd);
	return;
}
pexplosive = (PPOTYPE) SendDlgItemMessage(hwnd, EXPLOSIVEEDITCOMBOBOX, CB_GETITEMDATA, index, 0);
SendDlgItemMessage(hwnd, EXPLOSIVESHORTNAME, WM_SETTEXT, 0, (LPARAM) pexplosive->explosive.shortname);
SendDlgItemMessage(hwnd, EXPLOSIVELONGNAME, WM_SETTEXT, 0, (LPARAM) pexplosive->explosive.longname);
SendDlgItemMessage(hwnd, EXPLOSIVEDESCBOX, WM_SETTEXT, 0, (LPARAM) pexplosive->explosive.desc);
SendDlgItemMessage(hwnd, EXPLOSIVESOUNDBOX, WM_SETTEXT, 0, (LPARAM) pexplosive->explosive.impactsound);

if (pexplosive->explosive.dsaammo == TRUE){
SendDlgItemMessage(hwnd, EXPLOSIVECHECKDSAONLY, BM_SETCHECK, (WPARAM) BST_CHECKED, 0);
}
else{
	SendDlgItemMessage(hwnd, EXPLOSIVECHECKDSAONLY, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
}
wsprintf(buf, "%d", pexplosive->explosive.frequency);
SendDlgItemMessage(hwnd, EXPLOSIVEFREQUENCYBOX, WM_SETTEXT, 0, (LPARAM) buf);
wsprintf(buf, "%d", pexplosive->explosive.delay);
SendDlgItemMessage(hwnd, EXPLOSIVEDELAYBOX, WM_SETTEXT, 0, (LPARAM) buf);
wsprintf(buf, "%d", pexplosive->explosive.range);
SendDlgItemMessage(hwnd, EXPLOSIVERANGEBOX, WM_SETTEXT, 0, (LPARAM) buf);

wsprintf(buf, "%d", pexplosive->explosive.initialblastradius);
SendDlgItemMessage(hwnd, EXPLOSIVEINITRADIUSBOX, WM_SETTEXT, 0, (LPARAM) buf);
wsprintf(buf, "%d", pexplosive->explosive.initialdamagestrength);
SendDlgItemMessage(hwnd, EXPLOSIVEINITSTRENGTHBOX, WM_SETTEXT, 0, (LPARAM) buf);
SendDlgItemMessage(hwnd, EXPLOSIVEINITTYPEBOX, CB_SETCURSEL, (WPARAM)pexplosive->explosive.initialdamagetype, 0);

wsprintf(buf, "%d", pexplosive->explosive.lastingdamageradius);
SendDlgItemMessage(hwnd, EXPLOSIVELATENTRADIUSBOX, WM_SETTEXT, 0, (LPARAM) buf);
wsprintf(buf, "%d", pexplosive->explosive.lastingdamagetime);
SendDlgItemMessage(hwnd, EXPLOSIVELATENTTIMEBOX, WM_SETTEXT, 0, (LPARAM) buf);
SendDlgItemMessage(hwnd, EXPLOSIVELATENTTYPEBOX, CB_SETCURSEL, (WPARAM)pexplosive->explosive.lastingdamagetype, 0);
}

void ExplosiveEdClearFields(HWND hwnd)
{
		pexplosive = NULL;
SendDlgItemMessage(hwnd, EXPLOSIVEEDITCOMBOBOX, CB_SETCURSEL, 0,0);

SendDlgItemMessage(hwnd, EXPLOSIVEFREQUENCYBOX, WM_SETTEXT, 0, (LPARAM)"0");
SendDlgItemMessage(hwnd, EXPLOSIVERANGEBOX, WM_SETTEXT, 0, (LPARAM)"0");
SendDlgItemMessage(hwnd, EXPLOSIVEDELAYBOX, WM_SETTEXT, 0, (LPARAM)"0");

SendDlgItemMessage(hwnd, EXPLOSIVEINITRADIUSBOX, WM_SETTEXT, 0, (LPARAM)"0");
SendDlgItemMessage(hwnd, EXPLOSIVEINITSTRENGTHBOX, WM_SETTEXT, 0, (LPARAM)"0");
SendDlgItemMessage(hwnd, EXPLOSIVELATENTRADIUSBOX, WM_SETTEXT, 0, (LPARAM)"0");
SendDlgItemMessage(hwnd, EXPLOSIVELATENTTIMEBOX, WM_SETTEXT, 0, (LPARAM)"0");

SendDlgItemMessage(hwnd, EXPLOSIVESHORTNAME, WM_SETTEXT, 0, (LPARAM)NULL);
SendDlgItemMessage(hwnd, EXPLOSIVELONGNAME, WM_SETTEXT, 0,(LPARAM)NULL);
SendDlgItemMessage(hwnd, EXPLOSIVEDESCBOX, WM_SETTEXT, 0, (LPARAM)NULL);
SendDlgItemMessage(hwnd, EXPLOSIVESOUNDBOX, WM_SETTEXT, 0, (LPARAM)NULL);

SendDlgItemMessage(hwnd, EXPLOSIVEINITTYPEBOX, CB_SETCURSEL, 0,0);
SendDlgItemMessage(hwnd, EXPLOSIVELATENTTYPEBOX, CB_SETCURSEL, 0,0);

SendDlgItemMessage(hwnd, EXPLOSIVECHECKDSAONLY, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
}
void ExplosiveEdModifyCurrentAmmo(HWND hwnd)
{
		char buf[4000];
	int tempint;

	SendDlgItemMessage(hwnd, EXPLOSIVESHORTNAME, WM_GETTEXT, 4000, (LPARAM) buf);
	strncpy(pexplosive->explosive.shortname, buf, sizeof(pexplosive->explosive.shortname) - 1);

	SendDlgItemMessage(hwnd, EXPLOSIVELONGNAME, WM_GETTEXT, 4000, (LPARAM) buf);
	strncpy(pexplosive->explosive.longname, buf, sizeof(pexplosive->explosive.longname) - 1);

	SendDlgItemMessage(hwnd, EXPLOSIVEDESCBOX, WM_GETTEXT, 4000, (LPARAM) buf);
//	if (pexplosive->explosive.desc != NULL){
//		free(pexplosive->explosive.desc);
//	}
	pexplosive->explosive.desc = (char*) malloc(sizeof(char) * ((strlen(buf)) + 1));
	strcpy(pexplosive->explosive.desc, buf);

	SendDlgItemMessage(hwnd, EXPLOSIVESOUNDBOX, WM_GETTEXT, 4000, (LPARAM) buf);
//	if (pexplosive->explosive.impactsound != NULL){
//		free(pexplosive->explosive.impactsound);
//	}
	pexplosive->explosive.impactsound = (char*) malloc(sizeof(char) * ((strlen(buf)) + 1));
	strcpy(pexplosive->explosive.impactsound, buf);

	tempint = SendDlgItemMessage(hwnd, EXPLOSIVECHECKDSAONLY, BM_GETCHECK, 0,0);
	if (tempint == BST_CHECKED){
		pexplosive->explosive.dsaammo = TRUE;
	}
	else{
		pexplosive->explosive.dsaammo = FALSE;
	}

	SendDlgItemMessage(hwnd, EXPLOSIVEFREQUENCYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	pexplosive->explosive.frequency = tempint;

	SendDlgItemMessage(hwnd, EXPLOSIVERANGEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	pexplosive->explosive.range = tempint;

	SendDlgItemMessage(hwnd, EXPLOSIVEDELAYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	pexplosive->explosive.delay = tempint;



	SendDlgItemMessage(hwnd, EXPLOSIVEINITRADIUSBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	pexplosive->explosive.initialblastradius = tempint;

	SendDlgItemMessage(hwnd, EXPLOSIVEINITSTRENGTHBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	pexplosive->explosive.initialdamagestrength = tempint;

	pexplosive->explosive.initialdamagetype = SendDlgItemMessage(hwnd, EXPLOSIVEINITTYPEBOX, CB_GETCURSEL, 0,0);

		SendDlgItemMessage(hwnd, EXPLOSIVELATENTRADIUSBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	pexplosive->explosive.lastingdamageradius= tempint;

	SendDlgItemMessage(hwnd, EXPLOSIVELATENTTIMEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	pexplosive->explosive.lastingdamagetime = tempint;

	pexplosive->explosive.lastingdamagetype = SendDlgItemMessage(hwnd, EXPLOSIVELATENTTYPEBOX, CB_GETCURSEL, 0,0);



//ENTER IT INTO THE COMBO BOX. RETRIEVE THE INDEX, DELETE IT, THEN ADD IT TO THE INDEX BEFORE IT, ESSENTIALLY REPLACING IT.

	tempint = SendDlgItemMessage(hwnd, EXPLOSIVEEDITCOMBOBOX, CB_GETCURSEL, 0,0);
	SendDlgItemMessage(hwnd, EXPLOSIVEEDITCOMBOBOX, CB_DELETESTRING, tempint,0);
	SendDlgItemMessage(hwnd, EXPLOSIVEEDITCOMBOBOX, CB_INSERTSTRING, tempint, (LPARAM) pexplosive->explosive.longname);
	SendDlgItemMessage(hwnd, EXPLOSIVEEDITCOMBOBOX, CB_SETITEMDATA, tempint, (LPARAM) pexplosive);
	ExplosiveEdClearFields(hwnd);
}
void ExplosiveEdCreateNewAmmo(HWND hwnd)
{
			char buf[4000];
	int tempint;
	POTYPE temp;

	SendDlgItemMessage(hwnd, EXPLOSIVESHORTNAME, WM_GETTEXT, 4000, (LPARAM) buf);
	strncpy(temp.explosive.shortname, buf, sizeof(temp.explosive.shortname) - 1);

	SendDlgItemMessage(hwnd, EXPLOSIVELONGNAME, WM_GETTEXT, 4000, (LPARAM) buf);
	strncpy(temp.explosive.longname, buf, sizeof(temp.explosive.longname) - 1);

	SendDlgItemMessage(hwnd, EXPLOSIVEDESCBOX, WM_GETTEXT, 4000, (LPARAM) buf);
//	if (temp.explosive.desc != NULL){
//		free(temp.explosive.desc);
//	}
	temp.explosive.desc = (char*) malloc(sizeof(char) * ((strlen(buf)) + 1));
	strcpy(temp.explosive.desc, buf);

	SendDlgItemMessage(hwnd, EXPLOSIVESOUNDBOX, WM_GETTEXT, 4000, (LPARAM) buf);
//	if (temp.explosive.impactsound != NULL){
//		free(temp.explosive.impactsound);
//	}
	temp.explosive.impactsound = (char*) malloc(sizeof(char) * ((strlen(buf))+1));
	strcpy(temp.explosive.impactsound, buf);

	tempint = SendDlgItemMessage(hwnd, EXPLOSIVECHECKDSAONLY, BM_GETCHECK, 0,0);
	if (tempint == BST_CHECKED){
		temp.explosive.dsaammo = TRUE;
	}
	else{
		temp.explosive.dsaammo = FALSE;
	}

	SendDlgItemMessage(hwnd, EXPLOSIVERANGEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	temp.explosive.range = tempint;

	SendDlgItemMessage(hwnd, EXPLOSIVEDELAYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	temp.explosive.delay = tempint;

		SendDlgItemMessage(hwnd, EXPLOSIVEFREQUENCYBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	temp.explosive.frequency = tempint;

	SendDlgItemMessage(hwnd, EXPLOSIVEINITRADIUSBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	temp.explosive.initialblastradius = tempint;

	SendDlgItemMessage(hwnd, EXPLOSIVEINITSTRENGTHBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	temp.explosive.initialdamagestrength = tempint;

	temp.explosive.initialdamagetype = SendDlgItemMessage(hwnd, EXPLOSIVEINITTYPEBOX, CB_GETCURSEL, 0,0);

		SendDlgItemMessage(hwnd, EXPLOSIVELATENTRADIUSBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	temp.explosive.lastingdamageradius= tempint;

	SendDlgItemMessage(hwnd, EXPLOSIVELATENTTIMEBOX, WM_GETTEXT, 200, (LPARAM) buf);
	tempint = atoi(buf);
	temp.explosive.lastingdamagetime = tempint;

	temp.explosive.lastingdamagetype = SendDlgItemMessage(hwnd, EXPLOSIVELATENTTYPEBOX, CB_GETCURSEL, 0,0);

	temp.explosive.id = explosiveammolist->objcnt + 1;
		addtoList(explosiveammolist, temp, CompareExplosiveSubAmmoID);
	pexplosive = findObject(explosiveammolist, &temp, CompareExplosiveSubAmmoID);

	SendDlgItemMessage(hwnd, EXPLOSIVEEDITCOMBOBOX, CB_INSERTSTRING, -1, (LPARAM) pexplosive->explosive.longname);
	tempint = SendDlgItemMessage(hwnd, EXPLOSIVEEDITCOMBOBOX, CB_GETCOUNT, 0,0);
	SendDlgItemMessage(hwnd, EXPLOSIVEEDITCOMBOBOX, CB_SETITEMDATA, tempint -1, (LPARAM) pexplosive);

	ExplosiveEdClearFields(hwnd);
}
void ExplosiveEdSaveAmmoList(HWND hwnd)
{
		PLIST walker;
	int index = 0;

	
	if (explosiveammolist){ //ONLY WALK THE LIST IF THERE IS ONE.
		//SendDlgItemMessage(hwnd, TOTALAMMOBOX, WM_SETTEXT, 0, ammolist->objcnt);
		walker = explosiveammolist;

		
	if (pammo->ammo.maxammotypes > 0){
		free(pammo->ammo.specialammo.explosivedata);
	}
	pammo->ammo.maxammotypes = explosiveammolist->objcnt;
	pammo->ammo.specialammo.explosivedata = (PEXPLOSIVE) malloc(sizeof(EXPLOSIVE) * pammo->ammo.maxammotypes);

		for (walker->current = explosiveammolist->head; walker->current != NULL; walker->current = walker->current->nextnode){
			pammo->ammo.specialammo.explosivedata[index] = walker->current->object.explosive;
			index++;
		}
	}
}

void WEdLoadBoxes(HWND hwnd){
	int i;
	int index = 0;
	char loadtypes[4][30] = {"NONE", "CLIP", "MAGAZINE", "CYLINDER"};
	char classtypes[5][30] = {"NONE", "CLASS A", "CLASS B", "CLASS S", "CLASS X"};
	char filingtypes[8][30] = {"NONE", "HAND WEAPON", "PISTOL", "SMG", "RIFLE", "SHOTGUN", "HEAVY", "SPECIAL"};
	char drugtypes[3][30] = {"NONE", "CYANIDE", "KETAMINE"};
	char buf[60];
	BOOL badammoflag = FALSE;

	/************************************************************
	; Loading up the Classes, Filing, Loadtype, and Drug Boxes.
	************************************************************/

			PLIST walker;



	
	for (i = 0; i < WEAP_MAX_LOADTYPES; i++){
	index = SendDlgItemMessage(hwnd,AMMOLOADTYPEBOX, CB_INSERTSTRING, -1, (LPARAM) loadtypes[i]);
	SendDlgItemMessage(hwnd,AMMOLOADTYPEBOX, CB_SETITEMDATA, index, (LPARAM) i);
	SendDlgItemMessage(hwnd, AMMOLOADTYPEBOX, CB_SETCURSEL, 0,0);
	}
	for (i = 0; i < MAX_WEAP_CLASSES; i++){
	index = SendDlgItemMessage(hwnd, CLASSTYPEBOX, CB_INSERTSTRING, -1, (LPARAM) classtypes[i]);
	SendDlgItemMessage(hwnd,CLASSTYPEBOX, CB_SETITEMDATA, index, (LPARAM) i);
	SendDlgItemMessage(hwnd, CLASSTYPEBOX, CB_SETCURSEL, 0,0);
	}

	for(i = 0; i < MAX_WEAP_FILING_TYPES; i++){
index= 	SendDlgItemMessage(hwnd, FILINGTYPEBOX, CB_INSERTSTRING, -1, (LPARAM) filingtypes[i]);
	SendDlgItemMessage(hwnd,FILINGTYPEBOX, CB_SETITEMDATA, index, (LPARAM) i);
	SendDlgItemMessage(hwnd, FILINGTYPEBOX, CB_SETCURSEL, 0,0);
	}

	for (i = 0; i < MAX_POISONS; i++)
	{
	index=	SendDlgItemMessage(hwnd, DRUGTYPEBOX, CB_INSERTSTRING, -1, (LPARAM) drugtypes[i]);
	SendDlgItemMessage(hwnd,DRUGTYPEBOX, CB_SETITEMDATA, index, (LPARAM) i);
	SendDlgItemMessage(hwnd, DRUGTYPEBOX, CB_SETCURSEL, 0,0);
	}

	index = SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_INSERTSTRING, -1, (LPARAM) "NONE");
	SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_SETITEMDATA, index, (LPARAM) 0);
	SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_SETCURSEL, 0,0);

		index = SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_INSERTSTRING, -1, (LPARAM) "NONE");
	SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_SETITEMDATA, index, (LPARAM) 0);
	SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_SETCURSEL, 0,0);

	if (!weaponlist){
		SendDlgItemMessage(hwnd, TOTALWEAPONSBOX, WM_SETTEXT, 0, (LPARAM)"0");
	}

	else{
		wsprintf(buf, "%d", weaponlist->objcnt);
		SendDlgItemMessage(hwnd, TOTALWEAPONSBOX, WM_SETTEXT, 0, (LPARAM)buf);
	}
	
		if (ammolist){
		walker = ammolist;
	}

		index = 1;
	for (walker->current = ammolist->head; walker->current != NULL; walker->current = walker->current->nextnode){
			
		SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_INSERTSTRING, -1, (LPARAM) walker->current->object.ammo.name);
		SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_SETITEMDATA, index, (LPARAM) &walker->current->object);
		index++;
		}

	index = 1;
	if (weaponlist){
		walker = weaponlist;
	}

	
	for (walker->current = weaponlist->head; walker->current != NULL; walker->current = walker->current->nextnode){
		if (walker->current->object.weapon.ammotype > ammolist->objcnt){
			walker->current->object.weapon.ammotype = 0;   //ERROR CHECK FOR AMMO THAT WAS REMOVED.
			badammoflag = TRUE;
		}
		SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_INSERTSTRING, -1, (LPARAM) walker->current->object.weapon.longname);
		SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_SETITEMDATA, index, (LPARAM) &walker->current->object);
		index++;
	}
	if (badammoflag == TRUE)
	{
	MessageBox(hwnd, "Recent Ammo Changes / Removals have corrupted some weapon types. Please check that all weapons are correctly registered with their corresponding ammo types.", "Notification...", MB_OK);
	}
}


int WEdCheckValuesNew(HWND hwnd)
{
	int checkvalue;
	int checkvalue2;
	char checkbuf[4000];

	SendDlgItemMessage(hwnd, MAXAMMOBOX, WM_GETTEXT, 10, (LPARAM)checkbuf);
	checkvalue = atoi(checkbuf);
	checkvalue2 = SendDlgItemMessage(hwnd, AMMOLOADTYPEBOX, CB_GETCURSEL, 0,0);
	switch(checkvalue2)
	{
	case WEAP_LOADTYPE_NONE:
		if(checkvalue > 0){
			MessageBox(hwnd, "A weapon can not have a load type if it has max ammo < 0!", "NULL", MB_OK);
	return CHECKVALUESFAILED;
		}
		break;
	case WEAP_LOADTYPE_CYLINDER:
	case WEAP_LOADTYPE_MAGAZINE:
		if(checkvalue > 9){
			MessageBox(hwnd, "Magazine and Cylinder-fed weapons have a max ammo limit of 9.", "NULL", MB_OK);
			return CHECKVALUESFAILED;
		}
		break;
	}

		SendDlgItemMessage(hwnd, FREQUENCYBOX,  WM_GETTEXT, 10, (LPARAM)checkbuf);
	checkvalue = atoi(checkbuf);
	if (checkvalue > 100 || checkvalue < 1){
		MessageBox(hwnd, "The Frequency For This Weapon Must Be Between 1 and 100.", "Weapon Frequency Out Of Bounds", MB_OK);
		return CHECKVALUESFAILED;
	}

		SendDlgItemMessage(hwnd, FLASHLEVELBOX,  WM_GETTEXT, 10, (LPARAM)checkbuf);
	checkvalue = atoi(checkbuf);
	if (checkvalue > 100 || checkvalue < 0){
		MessageBox(hwnd, "The Flash Level For This Weapon Must Be Between 0 and 100.", "Weapon Flash Level Out Of Bounds", MB_OK);
		return CHECKVALUESFAILED;
	}

	SendDlgItemMessage(hwnd, SOUNDLEVELBOX,  WM_GETTEXT, 10, (LPARAM)checkbuf);
	checkvalue = atoi(checkbuf);
	if (checkvalue > 100 || checkvalue < 0){
		MessageBox(hwnd, "The Sound Level For This Weapon Must Be Between 1 and 100.", "Weapon Sound Level Out Of Bounds", MB_OK);
		return CHECKVALUESFAILED;
	}

	SendDlgItemMessage(hwnd, BASEACCURACYBOX,  WM_GETTEXT, 10, (LPARAM)checkbuf);
	checkvalue = atoi(checkbuf);
	if (checkvalue > 100 || checkvalue < 0){
		MessageBox(hwnd, "The Weapon's Base Accuracy Must Be Between 0 and 100.", "Weapon Accuracy Out Of Bounds", MB_OK);
		return CHECKVALUESFAILED;
	}

	SendDlgItemMessage(hwnd, MAXAMMOBOX,  WM_GETTEXT, 10, (LPARAM)checkbuf);
	checkvalue = atoi(checkbuf);
	SendDlgItemMessage(hwnd, ROUNDSPERTURNBOX,  WM_GETTEXT, 10, (LPARAM)checkbuf);
	checkvalue2 = atoi(checkbuf);

	if (checkvalue2 > checkvalue){
		MessageBox(hwnd, "The Rounds Per Turn This Weapon is Capable Of Exceed the Maximum Ammunition Level Of the Weapon. This is in Error.", "Max Ammo < RPT", MB_OK);
		return CHECKVALUESFAILED;
	}

	SendDlgItemMessage(hwnd, WEIGHTONESBOX,  WM_GETTEXT, 10, (LPARAM)checkbuf);
	checkvalue = atoi(checkbuf);
	SendDlgItemMessage(hwnd, WEIGHTTENTHSBOX,  WM_GETTEXT, 10, (LPARAM)checkbuf);
	checkvalue2 = atoi(checkbuf);

	if (checkvalue + checkvalue2 == 0){
		MessageBox(hwnd, "This Weapon Currently Has A Weight Of 0.0 lbs. This is in Error.", "Weightless Weapon", MB_OK);
		return CHECKVALUESFAILED;
	}

	SendDlgItemMessage(hwnd, SHORTNAMEBOX,  WM_GETTEXT, 20, (LPARAM)checkbuf);
	if (!strcmp(checkbuf, "")){
		MessageBox(hwnd, "This Weapon Is Missing A Short Name.", "Weapon Is Not Named", MB_OK);
		return CHECKVALUESFAILED;
	}

	SendDlgItemMessage(hwnd, LONGNAMEBOX,  WM_GETTEXT, 60, (LPARAM)checkbuf);
	if (!strcmp(checkbuf, "")){
		MessageBox(hwnd, "This Weapon Is Missing A Long Name.", "Weapon Is Not Named", MB_OK);
		return CHECKVALUESFAILED;
	}



	return CHECKVALUESOK;
}

int WEdCheckValuesModify(HWND hwnd)
{
	int checkvalue;
	int checkvalue2;

	char checkbuf[4000];

	SendDlgItemMessage(hwnd, MAXAMMOBOX, WM_GETTEXT, 10, (LPARAM)checkbuf);
	checkvalue = atoi(checkbuf);
	checkvalue2 = SendDlgItemMessage(hwnd, AMMOLOADTYPEBOX, CB_GETCURSEL, 0,0);
	switch(checkvalue2)
	{
	case WEAP_LOADTYPE_NONE:
		if(checkvalue > 0){
			MessageBox(hwnd, "This weapon has a loading type, yet does not hold any ammunition (Max Ammo is Zero). This is in error.", "Loading Type / No Max Ammunition", MB_OK);
	return CHECKVALUESFAILED;
		}
		break;
	case WEAP_LOADTYPE_CYLINDER:
	case WEAP_LOADTYPE_MAGAZINE:
		if(checkvalue > 9){
			MessageBox(hwnd, "Magazine and Cylinder-Fed Weapons Have A Maximum Of 9 Rounds.", "Magazine/Cylinder Maxed Out", MB_OK);
			return CHECKVALUESFAILED;
		}
		break;
	}

	checkvalue = SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_GETCURSEL, 0,0);
	if (checkvalue < 1){
	MessageBox(hwnd, "A Weapon Must Be Selected To Modify. No Weapon is Currently Selected.", "No Weapon Selected", MB_OK);
	return CHECKVALUESFAILED;
	}
	SendDlgItemMessage(hwnd, FREQUENCYBOX,  WM_GETTEXT, 10, (LPARAM)checkbuf);
	checkvalue = atoi(checkbuf);
	if (checkvalue > 100 || checkvalue < 1){
		MessageBox(hwnd, "The Frequency For This Weapon Must Be Between 1 and 100.", "Weapon Frequency Out Of Bounds", MB_OK);
		return CHECKVALUESFAILED;
	}

		SendDlgItemMessage(hwnd, FLASHLEVELBOX,  WM_GETTEXT, 10, (LPARAM)checkbuf);
	checkvalue = atoi(checkbuf);
	if (checkvalue > 100 || checkvalue < 0){
		MessageBox(hwnd, "The Flash Level For This Weapon Must Be Between 0 and 100.", "Weapon Flash Level Out Of Bounds", MB_OK);
		return CHECKVALUESFAILED;
	}

	SendDlgItemMessage(hwnd, SOUNDLEVELBOX,  WM_GETTEXT, 10, (LPARAM)checkbuf);
	checkvalue = atoi(checkbuf);
	if (checkvalue > 100 || checkvalue < 0){
		MessageBox(hwnd, "The Sound Level For This Weapon Must Be Between 1 and 100.", "Weapon Sound Level Out Of Bounds", MB_OK);
		return CHECKVALUESFAILED;
	}

	SendDlgItemMessage(hwnd, BASEACCURACYBOX,  WM_GETTEXT, 10, (LPARAM)checkbuf);
	checkvalue = atoi(checkbuf);
	if (checkvalue > 100 || checkvalue < 0){
		MessageBox(hwnd, "The Weapon's Base Accuracy Must Be Between 0 and 100.", "Weapon Accuracy Out Of Bounds", MB_OK);
		return CHECKVALUESFAILED;
	}

	SendDlgItemMessage(hwnd, MAXAMMOBOX,  WM_GETTEXT, 10, (LPARAM)checkbuf);
	checkvalue = atoi(checkbuf);
	SendDlgItemMessage(hwnd, ROUNDSPERTURNBOX,  WM_GETTEXT, 10, (LPARAM)checkbuf);
	checkvalue2 = atoi(checkbuf);

	if (checkvalue2 > checkvalue){
		MessageBox(hwnd, "The Rounds Per Turn This Weapon is Capable Of Exceed the Maximum Ammunition Level Of the Weapon. This is in Error.", "Max Ammo < RPT", MB_OK);
		return CHECKVALUESFAILED;
	}

	SendDlgItemMessage(hwnd, WEIGHTONESBOX,  WM_GETTEXT, 10, (LPARAM)checkbuf);
	checkvalue = atoi(checkbuf);
	SendDlgItemMessage(hwnd, WEIGHTTENTHSBOX,  WM_GETTEXT, 10, (LPARAM)checkbuf);
	checkvalue2 = atoi(checkbuf);

	if (checkvalue + checkvalue2 == 0){
		MessageBox(hwnd, "This Weapon Currently Has A Weight Of 0.0 lbs. This is in Error.", "Weightless Weapon", MB_OK);
		return CHECKVALUESFAILED;
	}

	SendDlgItemMessage(hwnd, SHORTNAMEBOX,  WM_GETTEXT, 20, (LPARAM)checkbuf);
	if (!strcmp(checkbuf, "")){
		MessageBox(hwnd, "This Weapon Is Missing A Short Name.", "Weapon Is Not Named", MB_OK);
		return CHECKVALUESFAILED;
	}

	SendDlgItemMessage(hwnd, LONGNAMEBOX,  WM_GETTEXT, 60, (LPARAM)checkbuf);
	if (!strcmp(checkbuf, "")){
		MessageBox(hwnd, "This Weapon Is Missing A Long Name.", "Weapon Is Not Named", MB_OK);
		return CHECKVALUESFAILED;
	}

	return CHECKVALUESOK;
}

void WEdCreateNewWeapon(HWND hwnd)
{
			char buf[4000];
	int tempint;
	POTYPE temp;
	
	float weight = 0.0;
		int actionvalues = 0;

	memset(&temp, 0, sizeof(POTYPE));

	//tempint = SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_GETCOUNT ,0,0);
	temp.weapon.id = weaponlist->objcnt + 1;

	SendDlgItemMessage(hwnd, SHORTNAMEBOX, WM_GETTEXT, 200, (LPARAM)buf);
	strncpy(temp.weapon.shortname, buf, sizeof(temp.weapon.shortname) - 1);

	SendDlgItemMessage(hwnd, LONGNAMEBOX, WM_GETTEXT, 200, (LPARAM)buf);
	strncpy(temp.weapon.longname, buf, sizeof(temp.weapon.longname) - 1);

	tempint = SendDlgItemMessage(hwnd, FILINGTYPEBOX, CB_GETCURSEL, 0, 0);
	temp.weapon.filingtype = tempint;

	tempint = SendDlgItemMessage(hwnd, CLASSTYPEBOX, CB_GETCURSEL, 0,0);
	temp.weapon.classtype = tempint;

	tempint = SendDlgItemMessage(hwnd, AMMOLOADTYPEBOX, CB_GETCURSEL, 0,0);
	temp.weapon.loadtype = tempint;

	tempint = SendDlgItemMessage(hwnd, DRUGTYPEBOX, CB_GETCURSEL, 0,0);
	temp.weapon.drug.type = tempint;

		tempint = SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_GETCURSEL, 0,0);
	temp.weapon.ammotype = tempint;

		SendDlgItemMessage(hwnd, DESCBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	temp.weapon.desc = (char*) malloc(sizeof(char) * ((strlen(buf)) + 1));
	strcpy(temp.weapon.desc, buf);

		SendDlgItemMessage(hwnd, FIRESOUNDBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	temp.weapon.soundfilename[WEAPONSOUND_SHOOT] = (char*) malloc(sizeof(char) * ((strlen(buf)) + 1));
	strcpy(temp.weapon.soundfilename[WEAPONSOUND_SHOOT], buf);

	SendDlgItemMessage(hwnd, THROWSOUNDBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	temp.weapon.soundfilename[WEAPONSOUND_THROW] = (char*) malloc(sizeof(char) * ((strlen(buf)) + 1));
	strcpy(temp.weapon.soundfilename[WEAPONSOUND_THROW], buf);

		SendDlgItemMessage(hwnd, HITSOUNDBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	temp.weapon.soundfilename[WEAPONSOUND_HIT] = (char*) malloc(sizeof(char) * ((strlen(buf)) + 1));
	strcpy(temp.weapon.soundfilename[WEAPONSOUND_HIT], buf);

		SendDlgItemMessage(hwnd, DROPSOUNDBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	temp.weapon.soundfilename[WEAPONSOUND_DROP] = (char*) malloc(sizeof(char) * ((strlen(buf)) + 1));
	strcpy(temp.weapon.soundfilename[WEAPONSOUND_DROP], buf);

		SendDlgItemMessage(hwnd, RELOADSOUNDBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	temp.weapon.soundfilename[WEAPONSOUND_RELOAD] = (char*) malloc(sizeof(char) * ((strlen(buf)) + 1));
	strcpy(temp.weapon.soundfilename[WEAPONSOUND_RELOAD], buf);
	
	SendDlgItemMessage(hwnd, BREAKSOUNDBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	temp.weapon.soundfilename[WEAPONSOUND_BREAK] = (char*) malloc(sizeof(char) * ((strlen(buf)) + 1));
	strcpy(temp.weapon.soundfilename[WEAPONSOUND_BREAK], buf);

	SendDlgItemMessage(hwnd, MAXAMMOBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	temp.weapon.maxammo = atoi(buf);

	SendDlgItemMessage(hwnd, DRUGDOSESBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	temp.weapon.drug.doses = atoi(buf);

	SendDlgItemMessage(hwnd,FREQUENCYBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	temp.weapon.frequency = atoi(buf);

		SendDlgItemMessage(hwnd, SOUNDLEVELBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	temp.weapon.weapperf.soundlevel = atoi(buf);

	SendDlgItemMessage(hwnd, ROUNDSPERTURNBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	temp.weapon.weapperf.roundsperturn = atoi(buf);

	SendDlgItemMessage(hwnd, FLASHLEVELBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	temp.weapon.weapperf.flashlevel = atoi(buf);

	SendDlgItemMessage(hwnd, BASEACCURACYBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	temp.weapon.weapperf.baseaccuracy_percent = atoi(buf);

	SendDlgItemMessage(hwnd, WEIGHTTENTHSBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	weight += ((float) atoi(buf)) /10;

	SendDlgItemMessage(hwnd, WEIGHTONESBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	weight+= (float) atoi(buf);

	temp.weapon.weight = weight;




	if ((SendDlgItemMessage(hwnd, WEAPONACTIONCHECKSHOOT, BM_GETCHECK, 0,0)) == BST_CHECKED){
		actionvalues |= WEAP_ATT_TYPE_SHOOT;
	}
	if ((SendDlgItemMessage(hwnd, WEAPONACTIONCHECKTHROW, BM_GETCHECK, 0,0)) == BST_CHECKED){
		actionvalues |= WEAP_ATT_TYPE_THROW;
	}
	if ((SendDlgItemMessage(hwnd, WEAPONACTIONCHECKCUT, BM_GETCHECK, 0,0)) == BST_CHECKED){
		actionvalues |= WEAP_ATT_TYPE_CUT;
	}
	if ((SendDlgItemMessage(hwnd, WEAPONACTIONCHECKHIT, BM_GETCHECK, 0,0)) == BST_CHECKED){
		actionvalues |= WEAP_ATT_TYPE_HIT;
	}
	if ((SendDlgItemMessage(hwnd, WEAPONACTIONCHECKDRUG, BM_GETCHECK, 0,0)) == BST_CHECKED){
		actionvalues |= WEAP_ATT_TYPE_DRUG;
	}
	if ((SendDlgItemMessage(hwnd, WEAPONACTIONCHECKSNIPE, BM_GETCHECK, 0,0)) == BST_CHECKED){
		actionvalues |= WEAP_ATT_TYPE_SNIPE;
	}

	temp.weapon.modifiersallowed.flashsuppressor = FALSE;
	temp.weapon.modifiersallowed.lasersight = FALSE;
	temp.weapon.modifiersallowed.silencer = FALSE;

	if ((SendDlgItemMessage(hwnd, MODIFIERCHECKFLASHSUPPRESSOR, BM_GETCHECK, 0,0)) == BST_CHECKED){
		temp.weapon.modifiersallowed.flashsuppressor = TRUE;
	}
	if ((SendDlgItemMessage(hwnd, MODIFIERCHECKLASERSIGHT, BM_GETCHECK, 0,0)) == BST_CHECKED){
		temp.weapon.modifiersallowed.lasersight = TRUE;
	}
	if ((SendDlgItemMessage(hwnd, MODIFIERCHECKSILENCER, BM_GETCHECK, 0,0)) == BST_CHECKED){
		temp.weapon.modifiersallowed.silencer = TRUE;
	}

		if ((SendDlgItemMessage(hwnd, DSAONLYCHECK, BM_GETCHECK, 0,0)) == BST_CHECKED){
		temp.weapon.dsaweapon = TRUE;
	}
	
		else{
			temp.weapon.dsaweapon = FALSE;
		}

		
	temp.weapon.attacktype = actionvalues;
	
	addtoList(weaponlist, temp, CompareWeaponID);
	pweapon = findObject(weaponlist, &temp, CompareWeaponID);

	SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_INSERTSTRING, -1, (LPARAM) pweapon->weapon.longname);
	tempint = SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_GETCOUNT, 0,0);
	SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_SETITEMDATA, tempint -1, (LPARAM) pweapon);

	SendDlgItemMessage(hwnd, TOTALWEAPONSBOX, WM_GETTEXT, 20, (LPARAM) buf);
	wsprintf(buf, "%d", atoi(buf) + 1);
	SendDlgItemMessage(hwnd, TOTALWEAPONSBOX, WM_SETTEXT,0, (LPARAM)buf);


	WEdClearFields(hwnd);
}

void WEdModifyWeapon(HWND hwnd)
{
	
			char buf[4000];
	int tempint;
		float weight = 0.0;
		int actionvalues = 0;

	tempint = SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_GETCURSEL, 0,0);
	pweapon = (PPOTYPE) SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_GETITEMDATA, tempint, 0);


	SendDlgItemMessage(hwnd, SHORTNAMEBOX, WM_GETTEXT, 200, (LPARAM)buf);
	strncpy(pweapon->weapon.shortname, buf, sizeof(pweapon->weapon.shortname) - 1);

	SendDlgItemMessage(hwnd, LONGNAMEBOX, WM_GETTEXT, 200, (LPARAM)buf);
	strncpy(pweapon->weapon.longname, buf, sizeof(pweapon->weapon.longname) - 1);

	tempint = SendDlgItemMessage(hwnd, FILINGTYPEBOX, CB_GETCURSEL, 0, 0);
	pweapon->weapon.filingtype = tempint;

	tempint = SendDlgItemMessage(hwnd, CLASSTYPEBOX, CB_GETCURSEL, 0,0);
	pweapon->weapon.classtype = tempint;

	tempint = SendDlgItemMessage(hwnd, AMMOLOADTYPEBOX, CB_GETCURSEL, 0,0);
	pweapon->weapon.loadtype = tempint;

	tempint = SendDlgItemMessage(hwnd, DRUGTYPEBOX, CB_GETCURSEL, 0,0);
	pweapon->weapon.drug.type = tempint;

		tempint = SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_GETCURSEL, 0,0);
	pweapon->weapon.ammotype = tempint;

		SendDlgItemMessage(hwnd, DESCBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	pweapon->weapon.desc = (char*) malloc(sizeof(char) * ((strlen(buf)) +1));
	strcpy(pweapon->weapon.desc, buf);

		SendDlgItemMessage(hwnd, FIRESOUNDBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	pweapon->weapon.soundfilename[WEAP_SOUND_FIRE] = (char*) malloc(sizeof(char) * ((strlen(buf)) +1));
	strcpy(pweapon->weapon.soundfilename[WEAP_SOUND_FIRE], buf);

	SendDlgItemMessage(hwnd, THROWSOUNDBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	pweapon->weapon.soundfilename[WEAP_SOUND_THROW] = (char*) malloc(sizeof(char) * ((strlen(buf)) +1));
	strcpy(pweapon->weapon.soundfilename[WEAP_SOUND_THROW], buf);

		SendDlgItemMessage(hwnd, HITSOUNDBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	pweapon->weapon.soundfilename[WEAP_SOUND_HIT] = (char*) malloc(sizeof(char) * ((strlen(buf)) +1));
	strcpy(pweapon->weapon.soundfilename[WEAP_SOUND_HIT], buf);

		SendDlgItemMessage(hwnd, DROPSOUNDBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	pweapon->weapon.soundfilename[WEAP_SOUND_DROP] = (char*) malloc(sizeof(char) * ((strlen(buf)) +1));
	strcpy(pweapon->weapon.soundfilename[WEAP_SOUND_DROP], buf);

		SendDlgItemMessage(hwnd, RELOADSOUNDBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	pweapon->weapon.soundfilename[WEAP_SOUND_RELOAD] = (char*) malloc(sizeof(char) * ((strlen(buf)) +1));
	strcpy(pweapon->weapon.soundfilename[WEAP_SOUND_RELOAD], buf);
	
	SendDlgItemMessage(hwnd, BREAKSOUNDBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	pweapon->weapon.soundfilename[WEAP_SOUND_BREAK] = (char*) malloc(sizeof(char) * ((strlen(buf)) +1));
	strcpy(pweapon->weapon.soundfilename[WEAP_SOUND_BREAK], buf);

	SendDlgItemMessage(hwnd, MAXAMMOBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	pweapon->weapon.maxammo = atoi(buf);

	SendDlgItemMessage(hwnd, DRUGDOSESBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	pweapon->weapon.drug.doses = atoi(buf);

	SendDlgItemMessage(hwnd,FREQUENCYBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	pweapon->weapon.frequency = atoi(buf);

		SendDlgItemMessage(hwnd, SOUNDLEVELBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	pweapon->weapon.weapperf.soundlevel = atoi(buf);

	SendDlgItemMessage(hwnd, ROUNDSPERTURNBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	pweapon->weapon.weapperf.roundsperturn = atoi(buf);

	SendDlgItemMessage(hwnd, FLASHLEVELBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	pweapon->weapon.weapperf.flashlevel = atoi(buf);

	SendDlgItemMessage(hwnd, BASEACCURACYBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	pweapon->weapon.weapperf.baseaccuracy_percent = atoi(buf);


	SendDlgItemMessage(hwnd, WEIGHTTENTHSBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	weight += ((float) atoi(buf)) /10;

	SendDlgItemMessage(hwnd, WEIGHTONESBOX, WM_GETTEXT, 4000, (LPARAM) buf);
	weight+= (float) atoi(buf);

	pweapon->weapon.weight = weight;




	if ((SendDlgItemMessage(hwnd, WEAPONACTIONCHECKSHOOT, BM_GETCHECK, 0,0)) == BST_CHECKED){
		actionvalues |= WEAP_ATT_TYPE_SHOOT;
	}
	if ((SendDlgItemMessage(hwnd, WEAPONACTIONCHECKTHROW, BM_GETCHECK, 0,0)) == BST_CHECKED){
		actionvalues |= WEAP_ATT_TYPE_THROW;
	}
	if ((SendDlgItemMessage(hwnd, WEAPONACTIONCHECKCUT, BM_GETCHECK, 0,0)) == BST_CHECKED){
		actionvalues |= WEAP_ATT_TYPE_CUT;
	}
	if ((SendDlgItemMessage(hwnd, WEAPONACTIONCHECKHIT, BM_GETCHECK, 0,0)) == BST_CHECKED){
		actionvalues |= WEAP_ATT_TYPE_HIT;
	}
	if ((SendDlgItemMessage(hwnd, WEAPONACTIONCHECKDRUG, BM_GETCHECK, 0,0)) == BST_CHECKED){
		actionvalues |= WEAP_ATT_TYPE_DRUG;
	}
	if ((SendDlgItemMessage(hwnd, WEAPONACTIONCHECKSNIPE, BM_GETCHECK, 0,0)) == BST_CHECKED){
		actionvalues |= WEAP_ATT_TYPE_SNIPE;
	}

	pweapon->weapon.modifiersallowed.flashsuppressor = FALSE;
	pweapon->weapon.modifiersallowed.lasersight = FALSE;
	pweapon->weapon.modifiersallowed.silencer = FALSE;

	if ((SendDlgItemMessage(hwnd, MODIFIERCHECKFLASHSUPPRESSOR, BM_GETCHECK, 0,0)) == BST_CHECKED){
		pweapon->weapon.modifiersallowed.flashsuppressor = TRUE;
	}
	if ((SendDlgItemMessage(hwnd, MODIFIERCHECKLASERSIGHT, BM_GETCHECK, 0,0)) == BST_CHECKED){
		pweapon->weapon.modifiersallowed.lasersight = TRUE;
	}
	if ((SendDlgItemMessage(hwnd, MODIFIERCHECKSILENCER, BM_GETCHECK, 0,0)) == BST_CHECKED){
		pweapon->weapon.modifiersallowed.silencer = TRUE;
	}

		if ((SendDlgItemMessage(hwnd, DSAONLYCHECK, BM_GETCHECK, 0,0)) == BST_CHECKED){
		pweapon->weapon.dsaweapon = TRUE;
	}
	
		else{
			pweapon->weapon.dsaweapon = FALSE;
		}

	pweapon->weapon.attacktype = actionvalues;
	
		tempint = SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_GETCURSEL, 0,0);
	SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_DELETESTRING, tempint,0);
	SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_INSERTSTRING, tempint, (LPARAM) pweapon->weapon.longname);
	SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_SETITEMDATA, tempint, (LPARAM) pweapon);

	WEdClearFields(hwnd);
}

void WEdWriteWeaponFile()
{
	
	
	FILE* fp;
	PLIST walker;
	int weaponid = 1;
	int i;
	
	
	walker = weaponlist;
	
	
	
	/************************************************************
	; WRITE OPENING BLOCK
	************************************************************/
	
	fp = fopen("data\\weapons.dat", "w");
	fprintf(fp, "<BEGINWEAPONFILE>\n");
	
	
	/************************************************************
	; LOOP THROUGH AMMO
	************************************************************/
	
	for (walker->current = weaponlist->head; walker->current != NULL; walker->current = walker->current->nextnode){
		//RESET THE SUBAMMO ID COUNTER;
		fprintf(fp, "<BEGINWEAPON>\n");
		fprintf(fp, "%d\n", weaponid);
		fprintf(fp, "%d\n", walker->current->object.weapon.dsaweapon);
		fprintf(fp, "%d\n", walker->current->object.weapon.attacktype);
		fprintf(fp, "%s\n", walker->current->object.weapon.shortname);
		fprintf(fp, "%s\n", walker->current->object.weapon.longname);
		fprintf(fp, "#\n");
		fprintf(fp, "%s\n", walker->current->object.weapon.desc);
		fprintf(fp, "#\n");
		fprintf(fp, "%d\n", walker->current->object.weapon.filingtype);
		fprintf(fp, "%d\n", walker->current->object.weapon.classtype);
		fprintf(fp, "%d\n", walker->current->object.weapon.frequency);
		fprintf(fp, "%d\n", walker->current->object.weapon.ammotype);
		fprintf(fp, "%d\n", walker->current->object.weapon.maxammo);	
		fprintf(fp, "%d\n", walker->current->object.weapon.loadtype);
		fprintf(fp, "%d\n", walker->current->object.weapon.weapperf.baseaccuracy_percent);
		fprintf(fp, "%d\n", walker->current->object.weapon.weapperf.roundsperturn);
		fprintf(fp, "%d\n", walker->current->object.weapon.weapperf.flashlevel);
		fprintf(fp, "%d\n", walker->current->object.weapon.weapperf.soundlevel);
		fprintf(fp, "%d\n", walker->current->object.weapon.drug.type);
		fprintf(fp, "%d\n", walker->current->object.weapon.drug.doses);
		fprintf(fp, "%1.1f\n", walker->current->object.weapon.weight);
		fprintf(fp, "%d\t%d\t%d\n", walker->current->object.weapon.modifiersallowed.lasersight, walker->current->object.weapon.modifiersallowed.flashsuppressor, walker->current->object.weapon.modifiersallowed.silencer);
		
		for (i = 0; i < MAX_WEAP_SOUNDS; i ++)
		{
			if (walker->current->object.weapon.soundfilename[i] == NULL){
				fprintf(fp, "NULL\n");
			}
			else{
				if (strcmp(walker->current->object.weapon.soundfilename[i], "")){
					fprintf(fp, "%s\n",walker->current->object.weapon.soundfilename[i]);
				}
				else{
					fprintf(fp, "NULL\n");
				}
			}
		}
		
		fprintf(fp, "<ENDWEAPON>\n");
		weaponid++;
	}
	
	/************************************************************
	; WRITE CLOSING BLOCK
	************************************************************/
	
	fprintf(fp, "<ENDWEAPONFILE>");
	fclose(fp);
	
}



void WEdClearFields(HWND hwnd)
{
		/************************************************************
	; Clearing Out the Weight and Max Ammo Boxes
	************************************************************/

	SendDlgItemMessage(hwnd, WEIGHTONESBOX, WM_SETTEXT, 0, (LPARAM) "0");
	SendDlgItemMessage(hwnd, WEIGHTTENTHSBOX, WM_SETTEXT, 0, (LPARAM) "0");

	SendDlgItemMessage(hwnd, MAXAMMOBOX, WM_SETTEXT, 0, (LPARAM) "0");
	SendDlgItemMessage(hwnd, FLASHLEVELBOX, WM_SETTEXT, 0, (LPARAM) "0");
	SendDlgItemMessage(hwnd, SOUNDLEVELBOX, WM_SETTEXT, 0, (LPARAM) "0");
	SendDlgItemMessage(hwnd, BASEACCURACYBOX, WM_SETTEXT, 0, (LPARAM) "0");
	SendDlgItemMessage(hwnd, ROUNDSPERTURNBOX, WM_SETTEXT, 0, (LPARAM) "0");
	SendDlgItemMessage(hwnd, FREQUENCYBOX, WM_SETTEXT, 0, (LPARAM) "0");
	SendDlgItemMessage(hwnd, DRUGDOSESBOX, WM_SETTEXT, 0, (LPARAM) "0");
	SendDlgItemMessage(hwnd, SHORTNAMEBOX, WM_SETTEXT, 0, (LPARAM) "");
	SendDlgItemMessage(hwnd, LONGNAMEBOX, WM_SETTEXT, 0, (LPARAM) "");
	SendDlgItemMessage(hwnd, FIRESOUNDBOX, WM_SETTEXT, 0, (LPARAM) "");
	SendDlgItemMessage(hwnd, DROPSOUNDBOX, WM_SETTEXT, 0, (LPARAM) "");
	SendDlgItemMessage(hwnd, THROWSOUNDBOX, WM_SETTEXT, 0, (LPARAM) "");
	SendDlgItemMessage(hwnd, RELOADSOUNDBOX, WM_SETTEXT, 0, (LPARAM) "");
	SendDlgItemMessage(hwnd, BREAKSOUNDBOX, WM_SETTEXT, 0, (LPARAM) "");
	SendDlgItemMessage(hwnd, HITSOUNDBOX, WM_SETTEXT, 0, (LPARAM) "");
	SendDlgItemMessage(hwnd, DESCBOX, WM_SETTEXT, 0, (LPARAM) "");
		SendDlgItemMessage(hwnd, DRUGTYPEBOX, CB_SETCURSEL, 0,0);
		SendDlgItemMessage(hwnd, FILINGTYPEBOX, CB_SETCURSEL, 0,0);
		SendDlgItemMessage(hwnd, CLASSTYPEBOX, CB_SETCURSEL, 0,0);
			SendDlgItemMessage(hwnd, AMMOLOADTYPEBOX, CB_SETCURSEL, 0,0);
			SendDlgItemMessage(hwnd, DSAONLYCHECK, BM_SETCHECK, (WPARAM) BST_UNCHECKED,0);
			SendDlgItemMessage(hwnd, MODIFIERCHECKLASERSIGHT, BM_SETCHECK, (WPARAM)BST_UNCHECKED,0);
			SendDlgItemMessage(hwnd, MODIFIERCHECKFLASHSUPPRESSOR, BM_SETCHECK,  (WPARAM) BST_UNCHECKED,0);
			SendDlgItemMessage(hwnd, MODIFIERCHECKSILENCER, BM_SETCHECK,  (WPARAM) BST_UNCHECKED,0);
			SendDlgItemMessage(hwnd, MAINAMMOTYPEBOX, CB_SETCURSEL, 0,0);
			SendDlgItemMessage(hwnd, WEAPONACTIONCHECKSHOOT, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, WEAPONACTIONCHECKHIT, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, WEAPONACTIONCHECKTHROW, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, WEAPONACTIONCHECKDRUG, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, WEAPONACTIONCHECKSNIPE, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, WEAPONACTIONCHECKCUT, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_SETCURSEL, 0,0);
			SendDlgItemMessage(hwnd, WEAPONLOADBOX, CB_SETCURSEL, 0,0);
			SendDlgItemMessage(hwnd, CURRENTWEAPONBOX, WM_SETTEXT, 0, (LPARAM)"");
}

int WEdLoadData()
{
		FILE *fp;

	fp = fopen("data\\ammo.dat", "r");
	if (!fp){
		MessageBox(NULL, "ammo.dat could not be opened. ", "ERROR",  MB_OK);
		WriteBlankAmmoFile();
		fp = fopen("data\\ammo.dat", "r");
		if (!ammolistallocated){
		ammolist = createAmmoList(fp);
		ammolistallocated = LIST_ALLOCATED;
		}
	fclose(fp);
	}
	else{
		if (!ammolistallocated){
		ammolist = createAmmoList(fp);
		ammolistallocated = LIST_ALLOCATED;
		}
	fclose(fp);
	}
	if (!ammolist){
		MessageBox(NULL, "ammo.dat has had an error. Delete the file or repair if possible.","ERROR",  MB_OK);
		weaponlist = NULL;
		return -2;
	}
	else{
		fp = fopen("data\\weapons.dat", "r");
		if (!fp){
			MessageBox(NULL, "weapons.dat could not be opened. Starting a new file...","ERROR",  MB_OK);
			WriteBlankWeaponFile();
				fp = fopen("data\\weapons.dat", "r");
				if (!weaponlistallocated){
				weaponlist = createWeaponList(fp, ammolist);
				weaponlistallocated = LIST_ALLOCATED;
				}
		fclose(fp);
		}
		else{
		if (!weaponlistallocated){
				weaponlist = createWeaponList(fp, ammolist);
				weaponlistallocated = LIST_ALLOCATED;
		}
		fclose(fp);
		}
		if (!weaponlist){
			MessageBox(NULL, "weapons.dat has had an error. Delete the file or repair if possible.", "ERROR", MB_OK);
			return -4;
		}
	}
	return 0;
}

int MAEdLoadData(HWND hwnd)
{
	FILE *fp;

	int index = 1;

	fp = fopen("data\\ammo.dat", "r");
	if (!fp){
		MessageBox(NULL, "ammo.dat could not be opened. creating new...", "ERROR",  MB_OK);
		WriteBlankAmmoFile();
		fp = fopen("data\\ammo.dat", "r");
			if (!ammolistallocated){
		ammolist = createAmmoList(fp);
		ammolistallocated = LIST_ALLOCATED;
		}
	fclose(fp);
	}
	else{
	if (!ammolistallocated){
		ammolist = createAmmoList(fp);
		ammolistallocated = LIST_ALLOCATED;
		}
	fclose(fp);
	}
	if (!ammolist){
		MessageBox(NULL, "ammo.dat has had an error. Delete the file or repair if possible.","ERROR",  MB_OK);
		return -2;
	}
	return 0;
}

int MAEdPopulateData(HWND hwnd)
{	



	char buf[200];
	PLIST walker;
	int index = 1;

		//MAKE THE FIRST AMMO TYPE "NONE"
	SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_INSERTSTRING, -1, (LPARAM) "NONE");
	SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_SETITEMDATA, 0, (LPARAM) NULL);

	
	if (ammolist){ //ONLY WALK THE LIST IF THERE IS ONE.
		//SendDlgItemMessage(hwnd, TOTALAMMOBOX, WM_SETTEXT, 0, ammolist->objcnt);
		walker = ammolist;
		for (walker->current = ammolist->head; walker->current != NULL; walker->current = walker->current->nextnode){
			SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_INSERTSTRING, -1, (LPARAM) walker->current->object.ammo.name);
			SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_SETITEMDATA, index, (LPARAM) &walker->current->object);
			index++;
		}
	}

	wsprintf(buf, "%d", ammolist->objcnt);

	SendDlgItemMessage(hwnd, TOTALAMMOBOX, WM_SETTEXT, 0, (LPARAM) buf);

	SendDlgItemMessage(hwnd, SUBAMMOTYPE, CB_INSERTSTRING, -1, (LPARAM) "BULLET");
	SendDlgItemMessage(hwnd, SUBAMMOTYPE, CB_INSERTSTRING, -1, (LPARAM) "SHELL");
	SendDlgItemMessage(hwnd, SUBAMMOTYPE, CB_INSERTSTRING, -1, (LPARAM) "EXPLOSIVE");
	SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_SETCURSEL,0,0);

return 0; //OK
}



int MAEdCheckNewValues(HWND hwnd)
{

	/************************************************************
	; This version does not worry about the sub ammo type number...because it assumes it hasn't been created yet.
	************************************************************/

	int index;
	char buf[200];
//	POTYPE tempObj;
//	PPOTYPE lstObj;


	SendDlgItemMessage(hwnd, AMMONAME, WM_GETTEXT, 20, (LPARAM) buf);
	if (!strcmp(buf, "")){
		MessageBox(hwnd, "Name must be filled in!", "PROBLEM...", MB_OK);
		return (-1);
	}
	index = SendDlgItemMessage(hwnd, SUBAMMOTYPE, CB_GETCURSEL, 0,0);
	if (index < 0){
		MessageBox(hwnd, "Sub Ammo Type is Invalid!", "Problem...", MB_OK);
		return (-1);
	}
	return 0; //OK
}

void MAEdMakeNewMainAmmo(HWND hwnd)
{
	POTYPE tempObj;
	char buf[60];
	int index;
	
	memset(&tempObj, 0, sizeof(POTYPE));

	tempObj.ammo.id = (ammolist->objcnt)+1;
	tempObj.ammo.maxammotypes = 0;
	SendDlgItemMessage(hwnd, AMMONAME, WM_GETTEXT, 60, (LPARAM) buf);
	strncpy(tempObj.ammo.name, buf, sizeof(tempObj.ammo.name) - 1);
	index = SendDlgItemMessage(hwnd, SUBAMMOTYPE, CB_GETCURSEL,0,0);
	tempObj.ammo.type = index;
	addtoList(ammolist, tempObj, CompareAmmoID);
	SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_INSERTSTRING, -1, (LPARAM)tempObj.ammo.name);
	index = SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_GETCOUNT, 0,0);
	SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_SETITEMDATA, index-1, (LPARAM)GetMainAmmoByID(ammolist, tempObj.ammo.id));
	SendDlgItemMessage(hwnd, TOTALAMMOBOX, WM_GETTEXT, 20, (LPARAM)buf);
	index = atoi(buf);
	index++;
	wsprintf(buf, "%d", index);
	SendDlgItemMessage(hwnd, TOTALAMMOBOX, WM_SETTEXT, 0, (LPARAM)buf);
	MAEdClearFields(hwnd);

}
int MAEdCheckSubAmmoValues(HWND hwnd)
{
	int index; 

	index = SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_GETCURSEL, 0,0);
		if (index <= 0){
			MessageBox(hwnd, "Invalid Main Ammo Type!", "Problem...", MB_OK);
			return (-1);
		}
		return 0;
}
int MAEdCheckSaveValues(HWND hwnd)
{
	int index;
	char buf[200];
	PPOTYPE temp;

	index = SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_GETCURSEL, 0,0);
		if (index <= 0){
			MessageBox(hwnd, "Invalid Main Ammo Type!", "Problem...", MB_OK);
			return (-1);
		}
		temp = (PPOTYPE) SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_GETITEMDATA, index, 0);
		if (temp->ammo.maxammotypes < 1){
			MessageBox(hwnd, "Ammo type MUST have at LEAST ONE sub-ammo type.", "Problem...", MB_OK);
			return (-1);
		}
	SendDlgItemMessage(hwnd, AMMONAME, WM_GETTEXT, 20, (LPARAM) buf);
	if (!strcmp(buf, "")){
		MessageBox(hwnd, "Name must be filled in!", "PROBLEM...", MB_OK);
		return (-1);
	}
	index = SendDlgItemMessage(hwnd, SUBAMMOTYPE, CB_GETCURSEL, 0,0);
	if (index < 0){
		MessageBox(hwnd, "Sub Ammo Type is Invalid!", "Problem...", MB_OK);
		return (-1);
	}
	return 0;
}

int MAEdCheckWriteValues(HWND hwnd)
{
	PLIST walker;
		char buf[2000];
	walker = ammolist;


	for (walker->current = ammolist->head; walker->current != NULL; walker->current = walker->current->nextnode){
		if (walker->current->object.ammo.maxammotypes < 1){
			wsprintf(buf, "Ammo Type: %s has no sub-ammo types! It must have at least ONE before saving process can commence. Please fix and re-try.", walker->current->object.ammo.name);
			MessageBox(hwnd, buf, "Problem...", MB_OK);
			return -1;
		}
	}
	return 0; //OK
}
void MAEdSelectMainAmmo(HWND hwnd)
{	
	int index;
	char buf[200];

	index = SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_GETCURSEL, 0,0);
	pammo = (PPOTYPE) SendDlgItemMessage(hwnd, MAINAMMOBOX, CB_GETITEMDATA, (WPARAM) index, 0);
	
	if (pammo == NULL){ //IF IT'S NONE
	MAEdClearFields(hwnd);
	return;
	}

	else{
		SendDlgItemMessage(hwnd, AMMONAME, WM_SETTEXT, 0, (LPARAM) pammo->ammo.name);
		SendDlgItemMessage(hwnd, SUBAMMOTYPE, CB_SETCURSEL, (WPARAM) pammo->ammo.type, 0);
		wsprintf(buf, "%d", pammo->ammo.maxammotypes);
		SendDlgItemMessage(hwnd, SUBAMMOTYPESBOX, WM_SETTEXT, 0 , (LPARAM) buf);
		wsprintf(buf, "%d", pammo->ammo.id);
			SendDlgItemMessage(hwnd, CURRENTAMMOBOX, WM_SETTEXT, 0, (LPARAM) buf);
	}
}

int MAEdWriteAmmoFile(HWND hwnd)
{


	FILE* fp;
	PLIST walker;
		int mainammoid = 1;
	int subammoid = 1;
	int i;


	walker = ammolist;



	/************************************************************
	; WRITE OPENING BLOCK
	************************************************************/

	fp = fopen("data\\ammo.dat", "w");
	fprintf(fp, "<BEGINAMMOFILE>\n");


	/************************************************************
	; LOOP THROUGH AMMO
	************************************************************/

	for (walker->current = ammolist->head; walker->current != NULL; walker->current = walker->current->nextnode){
	//RESET THE SUBAMMO ID COUNTER;
		subammoid = 1;
	fprintf(fp, "<BEGINMAINAMMO>\n");
	fprintf(fp, "%d\n", mainammoid);
	fprintf(fp, "%d\n", walker->current->object.ammo.type);
	fprintf(fp, "%s\n", walker->current->object.ammo.name);
	fprintf(fp, "%d\n", walker->current->object.ammo.maxammotypes);
	//LOOP SUB-AMMO
	for (i = 0; i < walker->current->object.ammo.maxammotypes; i++){
	fprintf(fp, "<BEGINSUBAMMO>\n");
	fprintf(fp, "%d\n", subammoid);
//BASED ON THE AMMO TYPE, RUN THESE FUNCTIONS.
	switch(walker->current->object.ammo.type){
	case AMMOTYPE_BULLET:
		MAEdWriteBulletFileData(fp, &walker->current->object.ammo.specialammo.bulletdata[i]);
		break;
	case AMMOTYPE_SHOTGUNSHELL:
		MAEdWriteShellFileData(fp, &walker->current->object.ammo.specialammo.shelldata[i]);
		break;
	case AMMOTYPE_EXPLOSIVE:
		MAEdWriteExplosiveFileData(fp, &walker->current->object.ammo.specialammo.explosivedata[i]);
		break;
	}
	fprintf(fp, "<ENDSUBAMMO>\n");
	subammoid++;
	}
	fprintf(fp, "<ENDMAINAMMO>\n");
	mainammoid++;
	}

	/************************************************************
	; WRITE CLOSING BLOCK
	************************************************************/

	fprintf(fp, "<ENDAMMOFILE>");
	fclose(fp);
	

	return 0; //OK
}

void MAEdWriteShellFileData(FILE* fp, PSHELL shell)
{
fprintf(fp, "%d\n", shell->dsaammo);
fprintf(fp, "%s\n", shell->shortname);
fprintf(fp, "%s\n", shell->longname);
fprintf(fp, "%s\n", shell->desc);
fprintf(fp, "%d\n", shell->frequency);
fprintf(fp, "%d\n", shell->spread);
fprintf(fp, "%d\n", shell->ap);
fprintf(fp, "%d\n", shell->energy);
fprintf(fp, "%d\n", shell->damage);
if (shell->impactsound == NULL){
	fprintf(fp, "NULL\n");
}
else{

	if (strcmp(shell->impactsound, ""))
		{
		fprintf(fp, "%s\n", shell->impactsound);
		}
					else{
				fprintf(fp, "NULL\n");
			}
}
}

void MAEdWriteBulletFileData(FILE* fp, PBULLET bullet)
{
	fprintf(fp, "%d\n", bullet->dsaammo);
	fprintf(fp, "%s\n", bullet->shortname);
	fprintf(fp, "%s\n", bullet->longname);
	fprintf(fp, "%s\n", bullet->desc);
	fprintf(fp, "%d\n", bullet->frequency);
	fprintf(fp, "%d\n", bullet->ap);
	fprintf(fp, "%d\n", bullet->energy);
	fprintf(fp, "%d\n", bullet->damage);
	fprintf(fp, "%d\n", bullet->latentdamage);
	if (bullet->impactsound == NULL){
		fprintf(fp, "NULL\n");
	}
	else{
		if (strcmp(bullet->impactsound, ""))
		{
		fprintf(fp, "%s\n", bullet->impactsound);
		}
					else{
				fprintf(fp, "NULL\n");
			}
	}
}

void MAEdWriteExplosiveFileData(FILE* fp, PEXPLOSIVE explosive)
{
	fprintf(fp, "%d\n", explosive->dsaammo);
	fprintf(fp, "%s\n", explosive->shortname);
	fprintf(fp, "%s\n", explosive->longname);
	fprintf(fp, "%s\n", explosive->desc);
	fprintf(fp, "%d\n", explosive->frequency);
	fprintf(fp, "%d\n", explosive->delay);
	fprintf(fp, "%d\n", explosive->range);
	fprintf(fp, "%d\n", explosive->initialblastradius);
	fprintf(fp, "%d\n", explosive->initialdamagetype);
	fprintf(fp, "%d\n", explosive->initialdamagestrength);
	fprintf(fp, "%d\n", explosive->lastingdamagetype);
	fprintf(fp, "%d\n", explosive->lastingdamageradius);
	fprintf(fp, "%d\n", explosive->lastingdamagetime);
	if (explosive->impactsound == NULL){
		fprintf(fp, "NULL\n");
	}
	else{
			if (strcmp(explosive->impactsound, "")){
		fprintf(fp, "%s\n", explosive->impactsound);
		}
			else{
				fprintf(fp, "NULL\n");
			}
	}
}

void MAEdCreateShellList()
{
	int i;
	POTYPE temp;

shellammolist = createList();
	for (i = 0; i < pammo->ammo.maxammotypes; i++){
		temp.shell = pammo->ammo.specialammo.shelldata[i];
		addtoList(shellammolist, temp, CompareShellSubAmmoID);
	}
}

void MAEdCreateBulletList()
{
		int i;
	POTYPE temp;

	bulletammolist = createList();

	for (i = 0; i < pammo->ammo.maxammotypes; i++){
		temp.bullet = pammo->ammo.specialammo.bulletdata[i];
		addtoList(bulletammolist, temp, CompareBulletSubAmmoID);
	}
}

void MAEdCreateExplosiveList()
{
		int i;
	POTYPE temp;

explosiveammolist = createList();
	for (i = 0; i < pammo->ammo.maxammotypes; i++){
		temp.explosive = pammo->ammo.specialammo.explosivedata[i];
		addtoList(explosiveammolist, temp, CompareExplosiveSubAmmoID);
	}
}

void WriteBlankAmmoFile()
{
	FILE* fp;

	fp = fopen("data\\ammo.dat", "w");
	fprintf(fp, "<BEGINAMMOFILE>\n");
	fprintf(fp, "<ENDAMMOFILE>");
	fclose(fp);

}

void WriteBlankWeaponFile()
{
	FILE* fp;

	fp = fopen("data\\weapons.dat", "w");
	fprintf(fp, "<BEGINWEAPONFILE>\n");
	fprintf(fp, "<ENDWEAPONFILE>");
	fclose(fp);

}


void ReorderWeaponListID(PLIST weaponlist)
{
	if(!weaponlist || weaponlist->objcnt < 1){
		return;
	}
	PLIST walker = weaponlist;
	walker->current = walker->head;
	for(int counter = 0; walker->current != NULL; walker->current = walker->current->nextnode, counter++)
	{
		walker->current->object.weapon.id = counter;
	}
}

void ReorderAmmoListID(PLIST ammolist, PLIST weaponlist)
{

	//WE MUST RESET THE WEAPONS TO THE APPROPRIATE AMMO TYPES.
		if(!ammolist || ammolist->objcnt < 1){
		return;
	}
	PLIST walker = ammolist;
	PLIST weaponwalker = weaponlist;
	
	walker->current = walker->head;
	for(int counter = 1; walker->current != NULL; walker->current = walker->current->nextnode, counter++)
	{
		if(weaponlist){
		for(weaponwalker->current = weaponwalker->head; weaponwalker->current != NULL; weaponwalker->current = weaponwalker->current->nextnode){
			if(weaponwalker->current->object.weapon.ammotype == walker->current->object.ammo.id){
				weaponwalker->current->object.weapon.ammotype = counter; //SET THE WEAPON'S AMMO TYPE TO THE NEW AMMO TYPE.
			}
		}
		}
		walker->current->object.ammo.id = counter;
	}
}

void ReorderBulletListID(PLIST bulletlist)
{
		if(!bulletlist || bulletlist->objcnt < 1){
		return;
	}
	PLIST walker = bulletlist;
	walker->current = walker->head;
	for(int counter = 0; walker->current != NULL; walker->current = walker->current->nextnode, counter++)
	{
		walker->current->object.bullet.id = counter;
	}
}

void ReorderShellListID(PLIST shelllist)
{
		if(!shelllist || shelllist->objcnt < 1){
		return;
	}
	PLIST walker = shelllist;
	walker->current = walker->head;
	for(int counter = 0; walker->current != NULL; walker->current = walker->current->nextnode, counter++)
	{
		walker->current->object.shell.id = counter;
	}
}

void ReorderExplosiveListID(PLIST explosivelist)
{
		if(!explosivelist || explosivelist->objcnt < 1){
		return;
	}
	PLIST walker = explosivelist;
	walker->current = walker->head;
	for(int counter = 0; walker->current != NULL; walker->current = walker->current->nextnode, counter++)
	{
		walker->current->object.shell.id = counter;
	}
}