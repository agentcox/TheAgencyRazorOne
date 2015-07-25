#include "agencyinclude.h"
#include "guns.h"



int RetrieveGlobalSubAmmoType(PROUND round)
{
	POTYPE tmpObj;
	PPOTYPE lstObj;
	
	//	int i;
	
	//WE FIRST FIND THE MAIN AMMO BY THE WEAPONS' AMMO ID FIELD.
	tmpObj.ammo.id = round->mainammotype;
	
	lstObj = findObject(globals.globalweaponsammo.ammolist, &tmpObj, CompareAmmoID);
	if (lstObj == NULL){
		return RETRIEVEGLOBALSUBAMMOTYPEFAILED; //NO GOOD. WE COULDN'T GET A MAIN AMMO TYPE MATCH.
	}
	else{
		return lstObj->ammo.type;
	}
}

PSHELL RetrieveGlobalShellData(PROUND round)
{
	POTYPE tmpObj;
	PPOTYPE lstObj;
	
	int i;
	
	//WE FIRST FIND THE MAIN AMMO BY THE WEAPONS' AMMO ID FIELD.
	tmpObj.ammo.id = round->mainammotype;
	
	lstObj = findObject(globals.globalweaponsammo.ammolist, &tmpObj, CompareAmmoID);
	if (lstObj == NULL){
		return NULL; //NO GOOD. WE COULDN'T GET A MAIN AMMO TYPE MATCH.
	}
	for (i = 0 ; i < lstObj->ammo.maxammotypes; i++){
		if (i == round->subammotype){
			return &(lstObj->ammo.specialammo.shelldata[i]); //GOTCHA.
		}
	}
	return NULL;
}
PBULLET RetrieveGlobalBulletData(PROUND round)
{
	POTYPE tmpObj;
	PPOTYPE lstObj;
	
	int i;
	
	//WE FIRST FIND THE MAIN AMMO BY THE WEAPONS' AMMO ID FIELD.
	tmpObj.ammo.id = round->mainammotype;
	
	lstObj = findObject(globals.globalweaponsammo.ammolist, &tmpObj, CompareAmmoID);
	if (lstObj == NULL){
		return NULL; //NO GOOD. WE COULDN'T GET A MAIN AMMO TYPE MATCH.
	}
	for (i = 0 ; i < lstObj->ammo.maxammotypes; i++){
		if (i == round->subammotype){
			return &(lstObj->ammo.specialammo.bulletdata[i]); //GOTCHA.
		}
	}
	return NULL;
}

PEXPLOSIVE RetrieveGlobalExplosiveData(PROUND round)
{
	POTYPE tmpObj;
	PPOTYPE lstObj;
	
	int i;
	
	//WE FIRST FIND THE MAIN AMMO BY THE WEAPONS' AMMO ID FIELD.
	tmpObj.ammo.id = round->mainammotype;
	
	lstObj = findObject(globals.globalweaponsammo.ammolist, &tmpObj, CompareAmmoID);
	if (lstObj == NULL){
		return NULL; //NO GOOD. WE COULDN'T GET A MAIN AMMO TYPE MATCH.
	}
	for (i = 0 ; i < lstObj->ammo.maxammotypes; i++){
		if (i == round->subammotype){
			return &(lstObj->ammo.specialammo.explosivedata[i]); //GOTCHA.
		}
	}
	return NULL;
}



//--------------------------------------------------------------------------------------------
// Function Name - GiveClip
//
// Description   - Give clip to a given clip list. Player/Npc Independent.
//
// Return Type   - int 
//
// Arguments     - PLIST cliplist ( The list of clips to modify )
//               - PWEAPON weapon ( The weapon to add a clip for )
//               - int subammotype ( The sub ammo type requested )
//               - PLIST ammolist ( The list of ammunition to take from )
//				- BOOL	training ( Is this a training mission? )
//
// Author        - Charles Cox
// Date Modified - 04/26/2000
//--------------------------------------------------------------------------------------------
int GiveClip(PLIST cliplist, PWEAPON weapon, int subammotype, PLIST ammolist, float* weighttomodify, BOOL training)
{
	POTYPE tmpObj;
	POTYPE clip;
	PPOTYPE lstObj;
	char buf[500];
	
	int i;
	
	//WE FIRST FIND THE MAIN AMMO BY THE WEAPONS' AMMO ID FIELD.
	tmpObj.ammo.id = weapon->ammotype;
	
	lstObj = findObject(ammolist, &tmpObj, CompareAmmoID);
	if (lstObj == NULL){
		return GETCLIPFAILED_NOMAINAMMO; //NO GOOD. WE COULDN'T GET A MAIN AMMO TYPE MATCH.
	}
	
	//WE NOW HAVE A MAIN AMMO ID. WE SCROLL THRU THE MAIN AMMO'S
	//SUB AMMO TYPES TO TRY AND MATCH THE SUBAMMO TYPE GIVEN.
	//WE NEED TO KNOW THE TYPE.
	
	switch(lstObj->ammo.type){
	case AMMOTYPE_BULLET:
		for (i= 0; i <lstObj->ammo.maxammotypes; i++){
			if (i == subammotype){ //WE HAVE A MATCH. LET'S GIVE HIM A CLIP OF IT.
				if ((lstObj->ammo.specialammo.bulletdata[i].roundsavailable <= 0 || !lstObj->ammo.specialammo.bulletdata[i].ammoavailable) && !training){
					return GETCLIPFAILED_NOBULLETS;
				}
				if (lstObj->ammo.specialammo.bulletdata[i].roundsavailable < weapon->maxammo && !training){
					SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
					sprintf(buf, "Your ammunition request has been met with a warning. This weapon uses clips that hold %d rounds. The armory currently only has %d rounds stocked. Do you wish to be issued a partial clip?", weapon->maxammo, lstObj->ammo.specialammo.bulletdata[i].roundsavailable);
					SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
					if(!YesNoDialogBox("Ammunition Shortage", buf, "(Y)es/(N)o", &globals.dboxparams)){
						return GETCLIPFAILED_NOBULLETS;
					}
					else{
					clip.clip.id = cliplist->objcnt;
					clip.clip.weaponid = weapon->id;
					clip.clip.currentrounds = lstObj->ammo.specialammo.bulletdata[i].roundsavailable;
					clip.clip.clipround.mainammotype = lstObj->ammo.id;
					clip.clip.clipround.subammotype = i;
					if (!training){
						lstObj->ammo.specialammo.bulletdata[i].roundsavailable -= clip.clip.currentrounds;
					}
					if(!training && weighttomodify){
						*weighttomodify += (float)((double)clip.clip.currentrounds * ROUND_WEIGHT);
					}
					addtoList(ammolist, clip, CompareClipID);
					return GETCLIP_OK_LESSTHANMAX;
					}
				}
				//LOAD HIM UP WITH A CLIP!!!
				clip.clip.id = cliplist->objcnt; //ASSIGN UNIQUE ID
				clip.clip.weaponid = weapon->id;
				clip.clip.currentrounds = weapon->maxammo;
				clip.clip.clipround.mainammotype = lstObj->ammo.id;
				clip.clip.clipround.subammotype = i;
				if (!training){
					lstObj->ammo.specialammo.bulletdata[i].roundsavailable -= weapon->maxammo;
				}
				if(!training && weighttomodify){
						*weighttomodify += (float)((double)clip.clip.currentrounds * ROUND_WEIGHT);
					}
				addtoList(cliplist, clip, CompareClipID);
				return GETCLIP_OK;
			}
		}
		return GETCLIPFAILED_NOSUBAMMO;
	case AMMOTYPE_SHOTGUNSHELL:
		for (i= 0; i <lstObj->ammo.maxammotypes; i++){
			if (i == subammotype){ //WE HAVE A MATCH. LET'S GIVE HIM A CLIP OF IT.
				if ((lstObj->ammo.specialammo.shelldata[i].roundsavailable <= 0 || !lstObj->ammo.specialammo.shelldata->roundsavailable)&& !training){
					return GETCLIPFAILED_NOBULLETS;
				}
				if (lstObj->ammo.specialammo.shelldata[i].roundsavailable < weapon->maxammo && !training){
					sprintf(buf, "Your ammunition request has been met with a warning. This weapon uses clips that hold %d rounds. The armory currently only has %d rounds stocked. Do you wish to be issued a partial clip?", weapon->maxammo, lstObj->ammo.specialammo.shelldata[i].roundsavailable);
					if(!YesNoDialogBox("Ammunition Shortage", buf, "(Y)es/(N)o", &globals.dboxparams)){
						return GETCLIPFAILED_NOBULLETS;
					}
					else{
					clip.clip.id = cliplist->objcnt;
					clip.clip.weaponid = weapon->id;
					clip.clip.currentrounds = lstObj->ammo.specialammo.shelldata[i].roundsavailable;
					clip.clip.clipround.mainammotype = lstObj->ammo.id;
					clip.clip.clipround.subammotype = i;
					if(!training && weighttomodify){
						*weighttomodify += (float)((double)clip.clip.currentrounds * ROUND_WEIGHT);
					}
					if (!training){
						lstObj->ammo.specialammo.shelldata[i].roundsavailable -= clip.clip.currentrounds;
					}
					addtoList(ammolist, clip, CompareClipID);
					return GETCLIP_OK_LESSTHANMAX;
					}
				}
				//LOAD HIM UP WITH A CLIP!!!
				clip.clip.id = cliplist->objcnt;
				clip.clip.weaponid = weapon->id;
				clip.clip.currentrounds = weapon->maxammo;
				clip.clip.clipround.mainammotype = lstObj->ammo.id;
				clip.clip.clipround.subammotype = i;
				if(!training && weighttomodify){
						*weighttomodify += (float)((double)clip.clip.currentrounds * ROUND_WEIGHT);
					}
				if(!training){
					lstObj->ammo.specialammo.shelldata[i].roundsavailable -= weapon->maxammo;
				}
				addtoList(cliplist, clip, CompareClipID);
				return GETCLIP_OK;
			}
			
		}
		return GETCLIPFAILED_NOSUBAMMO;
	case AMMOTYPE_EXPLOSIVE:
		for (i= 0; i <lstObj->ammo.maxammotypes; i++){
			if (i == subammotype){ //WE HAVE A MATCH. LET'S GIVE HIM A CLIP OF IT.
				if ((lstObj->ammo.specialammo.explosivedata[i].roundsavailable <= 0 || !lstObj->ammo.specialammo.explosivedata[i].ammoavailable)&& !training){
					return GETCLIPFAILED_NOBULLETS;
				}
				if (lstObj->ammo.specialammo.explosivedata[i].roundsavailable < weapon->maxammo && !training){
					SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
					sprintf(buf, "Your ammunition request has been met with a warning. This weapon uses clips that hold %d rounds. The armory currently only has %d rounds stocked. Do you wish to be issued a partial clip?", weapon->maxammo, lstObj->ammo.specialammo.explosivedata[i].roundsavailable);
					if(!YesNoDialogBox("Ammunition Shortage", buf, "(Y)es/(N)o", &globals.dboxparams)){
						return GETCLIPFAILED_NOBULLETS;
					}
					else{
					clip.clip.id = cliplist->objcnt;
					clip.clip.weaponid = weapon->id;
					clip.clip.currentrounds = lstObj->ammo.specialammo.explosivedata[i].roundsavailable;
					clip.clip.clipround.mainammotype = lstObj->ammo.id;
					clip.clip.clipround.subammotype = i;
					if(!training && weighttomodify){
						*weighttomodify += (float)((double)clip.clip.currentrounds * ROUND_WEIGHT);
					}
					if (!training){
						lstObj->ammo.specialammo.explosivedata[i].roundsavailable -= clip.clip.currentrounds;
					}
					addtoList(ammolist, clip, CompareClipID);
					return GETCLIP_OK_LESSTHANMAX;
					}
				}
				//LOAD HIM UP WITH A CLIP!!!
				clip.clip.id = cliplist->objcnt;
				clip.clip.weaponid = weapon->id;
				clip.clip.currentrounds = weapon->maxammo;
				clip.clip.clipround.mainammotype = lstObj->ammo.id;
				clip.clip.clipround.subammotype = i;
				if (!training){
					lstObj->ammo.specialammo.explosivedata[i].roundsavailable -= weapon->maxammo;
				}
				if(!training && weighttomodify){
						*weighttomodify += (float)((double)clip.clip.currentrounds * ROUND_WEIGHT);
					}
				addtoList(cliplist, clip, CompareClipID);
				
				return GETCLIP_OK;
			}
		}
		return GETCLIPFAILED_NOSUBAMMO;
	}
	return GETCLIPFAILED_DROPPEDOUTOFLOOP;
}

int GiveStrayRounds(PLIST strayroundsourcelist, PROUND round, int numbertoget,PLIST strayroundtargetlist, float* weighttomodify, BOOL training)
{
	POTYPE tmpObj;
	POTYPE tmpObj2;
	//	POTYPE strayround;
	char buf[500];
	PPOTYPE lstObj;
	PPOTYPE lstObj2;
	PLIST ammolist = strayroundsourcelist;
//	PLIST cliplist = armoryammolist->cliplist;
	
	int i;
	tmpObj.ammo.id = round->mainammotype;
	
	lstObj = findObject(ammolist, &tmpObj, CompareAmmoID);
	if (lstObj == NULL){
		return GETSTRAYROUNDSFAILED_NOMAINAMMO; //NO GOOD. WE COULDN'T GET A MAIN AMMO TYPE MATCH.
	}
	
	//WE NOW HAVE A MAIN AMMO ID. WE SCROLL THRU THE MAIN AMMO'S
	//SUB AMMO TYPES TO TRY AND MATCH THE SUBAMMO TYPE GIVEN.
	//WE NEED TO KNOW THE TYPE.
	
	switch(lstObj->ammo.type){
	case AMMOTYPE_BULLET:
		for (i= 0; i <lstObj->ammo.maxammotypes; i++){
			if (i == round->subammotype){ //WE HAVE A MATCH. LET'S GIVE HIM A CLIP OF IT.
				if (lstObj->ammo.specialammo.bulletdata[i].roundsavailable < numbertoget){
					//DISPLAY WARNING HERE.
					SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, TRUE);
					sprintf(buf, "Your request for %d rounds has been met with a warning. There are only %d rounds available. Would you like to accept this number of rounds?", numbertoget, lstObj->ammo.specialammo.bulletdata[i].roundsavailable);
					if(!YesNoDialogBox("Ammunition Shortage", buf, "(Y)es/(N)o", &globals.dboxparams)){
					return GETSTRAYROUNDSFAILED_NOTENOUGHBULLETS;
					}
					else{
						numbertoget = lstObj->ammo.specialammo.bulletdata[i].roundsavailable;
					}
				}
				//LOAD HIM UP!!! FIRST FIND OUT IF WE CAN PUT IT WITH OTHERS.
				if (!training)
				{
					lstObj->ammo.specialammo.bulletdata[i].roundsavailable -= numbertoget;
				}
				tmpObj2.rounds.mainammotype = round->mainammotype;
				tmpObj2.rounds.subammotype = round->subammotype;
				lstObj2 = findObject(strayroundtargetlist, &tmpObj2, CompareStrayRoundsAmmo);
				if(lstObj2 == NULL){
					tmpObj2.rounds.numberofroundsleft = numbertoget;
					tmpObj2.rounds.id = strayroundtargetlist->objcnt;
					if(!training && weighttomodify){
						*weighttomodify += (float)((double)numbertoget * ROUND_WEIGHT);
					}
					addtoList(strayroundtargetlist, tmpObj2, CompareStrayRoundsID);
					return GETSTRAYROUNDS_OK;
				}
				lstObj2->rounds.numberofroundsleft+= numbertoget;
				if(!training && weighttomodify){
						*weighttomodify += (float)((double)numbertoget * ROUND_WEIGHT);
					}
				return GETSTRAYROUNDS_OK;
			}
			
		}
		return GETSTRAYROUNDSFAILED_NOSUBAMMO;
	case AMMOTYPE_SHOTGUNSHELL:
		for (i= 0; i <lstObj->ammo.maxammotypes; i++){
			if (i == round->subammotype){ //WE HAVE A MATCH. LET'S GIVE HIM A CLIP OF IT.
				if (lstObj->ammo.specialammo.shelldata[i].roundsavailable < numbertoget){
					//DISPLAY WARNING HERE.
					SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, TRUE);
					sprintf(buf, "Your request for %d rounds has been met with a warning. There are only %d rounds available. Would you like to accept this number of rounds?", numbertoget, lstObj->ammo.specialammo.shelldata[i].roundsavailable);
					if(!YesNoDialogBox("Ammunition Shortage", buf, "(Y)es/(N)o", &globals.dboxparams)){
					return GETSTRAYROUNDSFAILED_NOTENOUGHBULLETS;
					}
					else{
						numbertoget = lstObj->ammo.specialammo.shelldata[i].roundsavailable;
					}
				}
				//LOAD HIM UP!!! FIRST FIND OUT IF WE CAN PUT IT WITH OTHERS.
				if (!training){
					lstObj->ammo.specialammo.bulletdata[i].roundsavailable -= numbertoget;
				}
				tmpObj2.rounds.mainammotype = round->mainammotype;
				tmpObj2.rounds.subammotype = round->subammotype;
				lstObj2 = findObject(strayroundtargetlist, &tmpObj2, CompareStrayRoundsAmmo);
				if(lstObj2 == NULL){
					tmpObj2.rounds.numberofroundsleft = numbertoget;
					tmpObj2.rounds.id = strayroundtargetlist->objcnt;
					if(!training && weighttomodify){
						*weighttomodify += (float)((double)numbertoget * ROUND_WEIGHT);
					}
					addtoList(strayroundtargetlist, tmpObj2, CompareStrayRoundsID);
					return GETSTRAYROUNDS_OK;
				}
				lstObj2->rounds.numberofroundsleft+= numbertoget;
				if(!training && weighttomodify){
						*weighttomodify += (float)((double)numbertoget * ROUND_WEIGHT);
					}
				return GETSTRAYROUNDS_OK;
			}
			
		}
		return GETSTRAYROUNDSFAILED_NOSUBAMMO;
	case AMMOTYPE_EXPLOSIVE:
		for (i= 0; i <lstObj->ammo.maxammotypes; i++){
			if (i == round->subammotype){ //WE HAVE A MATCH. LET'S GIVE HIM A CLIP OF IT.
				if (lstObj->ammo.specialammo.explosivedata[i].roundsavailable < numbertoget){
						//DISPLAY WARNING HERE.
					SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, TRUE);
					sprintf(buf, "Your request for %d rounds has been met with a warning. There are only %d rounds available. Would you like to accept this number of rounds?", numbertoget, lstObj->ammo.specialammo.explosivedata[i].roundsavailable);
					if(!YesNoDialogBox("Ammunition Shortage", buf, "(Y)es/(N)o", &globals.dboxparams)){
					return GETSTRAYROUNDSFAILED_NOTENOUGHBULLETS;
					}
					else{
						numbertoget = lstObj->ammo.specialammo.explosivedata[i].roundsavailable;
					}
				}
				//LOAD HIM UP!!! FIRST FIND OUT IF WE CAN PUT IT WITH OTHERS.
				if(!training){
					lstObj->ammo.specialammo.bulletdata[i].roundsavailable -= numbertoget;
				}
				tmpObj2.rounds.mainammotype = round->mainammotype;
				tmpObj2.rounds.subammotype = round->subammotype;
				lstObj2 = findObject(strayroundtargetlist, &tmpObj2, CompareStrayRoundsAmmo);
				if(lstObj2 == NULL){
					tmpObj2.rounds.numberofroundsleft = numbertoget;
					tmpObj2.rounds.id = strayroundtargetlist->objcnt;
					if(!training && weighttomodify){
						*weighttomodify += (float)((double)numbertoget * ROUND_WEIGHT);
					}
					addtoList(strayroundtargetlist, tmpObj2, CompareStrayRoundsID);
					return GETSTRAYROUNDS_OK;
				}
				if(!training && weighttomodify){
						*weighttomodify += (float)((double)numbertoget * ROUND_WEIGHT);
					}
				lstObj2->rounds.numberofroundsleft+= numbertoget;
				return GETSTRAYROUNDS_OK;
			}
			
		}
		return GETSTRAYROUNDSFAILED_NOSUBAMMO;
	}
	return GETSTRAYROUNDSFAILED_DROPPEDOUTOFLOOP;
}

void ReorderPlayerAmmoLists(PPLAYERAMMO ammolist)
{
	PLIST clipwalker = ammolist->cliplist;
	PLIST roundwalker = ammolist->roundtypes;
	int counter = 0;
	for(clipwalker->current = clipwalker->head; clipwalker->current != NULL; clipwalker->current = clipwalker->current->nextnode)
	{
		clipwalker->current->object.clip.id = counter;
		counter++;
	}

	for(roundwalker->current = roundwalker->head; roundwalker->current != NULL; roundwalker->current = roundwalker->current->nextnode)
	{
		roundwalker->current->object.rounds.id = counter;
		counter++;
	}
}

AMMORESULT ReturnClipOrStrayRoundByID(PPLAYERAMMO ammolist, int id)
{
	PLIST clipwalker = ammolist->cliplist;
	PLIST roundwalker = ammolist->roundtypes;
	AMMORESULT Ammo;
	Ammo.Clip = NULL;
	Ammo.Rounds = NULL;
	Ammo.isClip = false;
	
	for(clipwalker->current = clipwalker->head; clipwalker->current != NULL; clipwalker->current = clipwalker->current->nextnode)
	{
		if (clipwalker->current->object.clip.id == id){
			Ammo.isClip = true;
			Ammo.Clip = &clipwalker->current->object.clip;
		}
	}

	for(roundwalker->current = roundwalker->head; roundwalker->current != NULL; roundwalker->current = roundwalker->current->nextnode)
	{
		if(roundwalker->current->object.rounds.id == id){
			Ammo.isClip = false;
			Ammo.Rounds = &roundwalker->current->object.rounds;
		}
	}
	return Ammo;
}

int FindMatchingPlayerAmmoIDs(PPLAYERAMMO playerammo, PWEAPON weapon, int** outarray)
{
	PLIST clipwalker = playerammo->cliplist;
	PLIST roundwalker = playerammo->roundtypes;
	//THIS FUNCTION MALLOCS!!!! DON'T FORGET TO FREE!!!!!
	int* TempArray = (int*)malloc(sizeof(int) * (playerammo->cliplist->objcnt + playerammo->roundtypes->objcnt)); //MAX AMOUNT.
	int numberfound = 0;
	//WE'RE ONLY MATCHING THE MAIN AMMO ID'S, WE DON'T TOUCH SUB-AMMO ID'S.
	for(clipwalker->current = clipwalker->head; clipwalker->current != NULL; clipwalker->current = clipwalker->current->nextnode)
	{
		if(clipwalker->current->object.clip.clipround.mainammotype == weapon->ammotype && clipwalker->current->object.clip.weaponid == weapon->id){
			TempArray[numberfound] = clipwalker->current->object.clip.id;
			numberfound++;
		}
	}

	for(roundwalker->current = roundwalker->head; roundwalker->current != NULL; roundwalker->current = roundwalker->current->nextnode)
	{
		if(roundwalker->current->object.rounds.mainammotype == weapon->ammotype)
		{
			TempArray[numberfound] = roundwalker->current->object.rounds.id;
			numberfound++;
		}
	}
	if(numberfound > 0){
	*outarray = (int*)malloc(sizeof(int) * numberfound);
	for(int i =0; i < numberfound; i++){
		(*outarray)[i] = TempArray[i];
	}
	}
	free(TempArray);
	return numberfound;
}


//--------------------------------------------------------------------------------------------
// Function Name - LoadClip
//
// Description   - Loads a clip into a weapon. This is player/npc independent, so no sounds are played, nothing.
//
// Return Type   - int 
//
// Arguments     - PLIST cliplist ( The list of clips to choose from. )
//               - PWEAPON weapon ( The weapon to load clip into. )
//               - int clipid ( The clip id chosen. )
//
// Author        - Charles Cox
// Date Modified - 04/26/2000
//--------------------------------------------------------------------------------------------
int LoadClip(PLIST cliplist, PWEAPON weapon, int clipid)
{
	POTYPE tmpObj;
	PPOTYPE lstObj;
	
	if (weapon->loadtype != WEAP_LOADTYPE_CLIP){
		return LOADCLIPFAILED_WRONGLOADTYPE;
	}
	
	
	if (weapon->loadprocedure.clip.isclipinserted){
		return LOADCLIPFAILED_CLIPALREADYLOADED;
	}
	
	tmpObj.clip.id = clipid;
	lstObj = findObject(cliplist, &tmpObj, CompareClipID);
	if (lstObj == NULL){
		return LOADCLIPFAILED_NOCLIPMATCH;
	}
	
	if (lstObj->clip.weaponid != weapon->id){
		return LOADCLIPFAILED_WRONGCLIPFORWEAPON;
	}
	
	if (lstObj->clip.clipround.mainammotype != weapon->ammotype)
	{
		return LOADCLIPFAILED_WRONGAMMOTYPEFORWEAPON;
	}
	
	//FINALLY. LOAD UP THE RECIEVER WITH THE CLIP BY POINTING TO THE CLIP.
	weapon->loadprocedure.clip.clipinserted = lstObj->clip;
	weapon->loadprocedure.clip.isclipinserted = true;
	deleteObject(cliplist, lstObj);
	return LOADCLIP_OK;
}


//--------------------------------------------------------------------------------------------
// Function Name - EjectClip
//
// Description   - Ejects a clip from a weapon. NPC/Player Independent. 
//	No sounds, but will notify thru return whether or not clip is dropped.
//
// Return Type   - int 
//
// Arguments     - PWEAPON weapon ( The weapon to eject clip from )
//               - PCLIP cliplist ( The list of clips, may be modified )
//
// Author        - Charles Cox
// Date Modified - 04/26/2000
//--------------------------------------------------------------------------------------------
int EjectClip(PWEAPON weapon, PLIST cliplist)
{
	POTYPE tmpObj;
	//	PPOTYPE lstObj;
	
	if (weapon->loadtype != WEAP_LOADTYPE_CLIP){
		return EJECTCLIPFAILED_WRONGLOADTYPE;
	}
	if (weapon->loadprocedure.clip.isclipinserted == FALSE){
		return EJECTCLIPFAILED_NOCLIPLOADED;
	}
	
	if(weapon->loadprocedure.clip.clipinserted.currentrounds < 1){ //IF IT'S OUT...
		weapon->loadprocedure.clip.isclipinserted = FALSE; //EMPTY THE RECIEVER.
		return EJECTCLIP_OK_DROPPED;
	}
	else{
		tmpObj.clip = weapon->loadprocedure.clip.clipinserted;
		tmpObj.clip.id = cliplist->objcnt;
		addtoList(cliplist, tmpObj, CompareClipID); //PUT BACK INTO THE PLAYER LIST
		weapon->loadprocedure.clip.isclipinserted = FALSE; //EMPTY THE RECIEVER.
		return EJECTCLIP_OK_SAVED;
	}
}




//--------------------------------------------------------------------------------------------
// Function Name - FireClipBullet
//
// Description   - Fires a bullet from a clip weapon, loads up the clip structure and specifies how many bullets were fired
//
// Return Type   - int 
//
// Arguments     - PWEAPON weapon ( The weapon to fire from )
//               - PBULLET bulletdata ( The bullet data to load "OUT PARAMETER" )
//               - int* roundsfired ( The number of rounds fired "OUT PARAMETER" )
//
// Author        - Charles Cox
// Date Modified - 04/27/2000
//--------------------------------------------------------------------------------------------
int FireClipBullet(PWEAPON weapon, PBULLET bulletdata, int* roundsfired)
{
	if (weapon->loadtype != WEAP_LOADTYPE_CLIP){
		return FIRECLIPFAILED_WRONGLOADTYPE;
	}
	if (weapon->loadprocedure.clip.isclipinserted == FALSE){
		return FIRECLIPFAILED_NOCLIPINSERTED;
	}
	if (weapon->loadprocedure.clip.clipinserted.currentrounds < 1){
		return FIRECLIPFAILED_OUTOFAMMO;
	}
	bulletdata = RetrieveGlobalBulletData(&weapon->loadprocedure.clip.clipinserted.clipround);
	if (bulletdata == NULL){
		return FIRECLIPFAILED_NOMATCH;
	}
	DecrementClip(weapon, &weapon->loadprocedure.clip.clipinserted);
	if (weapon->loadprocedure.clip.clipinserted.currentrounds < 0){ //IF THIS PUTS US UNDER...
		*roundsfired = 1; //SINCE IT'S A NEGATIVE NUMBER, WE'RE SUBTRACTING.
		return FIRECLIP_OK_LASTROUNDS; //LAST ROUNDS FIRED, MAYBE SOME MISSING.
	}
	*roundsfired = 1;
	return FIRECLIP_OK_ALLROUNDS; //ALL ROUNDS FIRED AWAY
}



//--------------------------------------------------------------------------------------------
// Function Name - FireClipShell
//
// Description   - Fires a shell or shells from a clip. See FireClipBullet
//
// Return Type   - int 
//
// Arguments     - PWEAPON weapon ( The weapon to fire from )
//               - PSHELL shell ( Shell structure to fill in with data "OUT PARAMETER" )
//               - int* roundsfired ( Pointer to the number of rounds actually fired "OUT PARAMETER")
//
// Author        - Charles Cox
// Date Modified - 04/27/2000
//--------------------------------------------------------------------------------------------
int FireClipShell(PWEAPON weapon, PSHELL shell, int* roundsfired)
{
	if (weapon->loadtype != WEAP_LOADTYPE_CLIP){
		return FIRECLIPFAILED_WRONGLOADTYPE;
	}
	if (weapon->loadprocedure.clip.isclipinserted == FALSE){
		return FIRECLIPFAILED_NOCLIPINSERTED;
	}
	if (weapon->loadprocedure.clip.clipinserted.currentrounds < 1){
		return FIRECLIPFAILED_OUTOFAMMO;
	}
	shell = RetrieveGlobalShellData(&weapon->loadprocedure.clip.clipinserted.clipround);
	if (shell == NULL){
		return FIRECLIPFAILED_NOMATCH;
	}
	DecrementClip(weapon, &weapon->loadprocedure.clip.clipinserted);
	if (weapon->loadprocedure.clip.clipinserted.currentrounds < 0){ //IF THIS PUTS US UNDER...
		*roundsfired = 1; //SINCE IT'S A NEGATIVE NUMBER, WE'RE SUBTRACTING.
		return FIRECLIP_OK_LASTROUNDS; //LAST ROUNDS FIRED, MAYBE SOME MISSING.
	}
	*roundsfired = 1;
	return FIRECLIP_OK_ALLROUNDS; //ALL ROUNDS FIRED AWAY
}



//--------------------------------------------------------------------------------------------
// Function Name - FireClipExplosive
//
// Description   - Fires an explosive or explosives from a clip, See FireClipBullet
//
// Return Type   - int 
//
// Arguments     - PWEAPON weapon ( The weapon to fire from )
//               - PEXPLOSIVE explosive ( The explosive structure to fill in "OUT PARAMETER" )
//               - int* roundsfired ( The pointer to rounds fired "OUT PARAMETER" )
//
// Author        - Charles Cox
// Date Modified - 04/27/2000
//--------------------------------------------------------------------------------------------
int FireClipExplosive(PWEAPON weapon, PEXPLOSIVE explosive, int* roundsfired)
{
	if (weapon->loadtype != WEAP_LOADTYPE_CLIP){
		return FIRECLIPFAILED_WRONGLOADTYPE;
	}
	if (weapon->loadprocedure.clip.isclipinserted == FALSE){
		return FIRECLIPFAILED_NOCLIPINSERTED;
	}
	if (weapon->loadprocedure.clip.clipinserted.currentrounds < 1){
		return FIRECLIPFAILED_OUTOFAMMO;
	}
	explosive = RetrieveGlobalExplosiveData(&weapon->loadprocedure.clip.clipinserted.clipround);
	if (explosive == NULL){
		return FIRECLIPFAILED_NOMATCH;
	}
	DecrementClip(weapon, &weapon->loadprocedure.clip.clipinserted);
	if (weapon->loadprocedure.clip.clipinserted.currentrounds < 0){ //IF THIS PUTS US UNDER...
		*roundsfired = 1; //SINCE IT'S A NEGATIVE NUMBER, WE'RE SUBTRACTING.
		return FIRECLIP_OK_LASTROUNDS; //LAST ROUNDS FIRED, MAYBE SOME MISSING.
	}
	*roundsfired = 1;
	return FIRECLIP_OK_ALLROUNDS; //ALL ROUNDS FIRED AWAY
}



//--------------------------------------------------------------------------------------------
// Function Name - DecrementClip
//
// Description   - Decrements the number of rounds in a clip by the weapon's RPT
//
// Return Type   - int 
//
// Arguments     - PWEAPON weapon ( The weapon to decrement )
//               - PCLIP clip ( The clip to decrement )
//
// Author        - Charles Cox
// Date Modified - 04/27/2000
//--------------------------------------------------------------------------------------------
int DecrementClip(PWEAPON weapon, PCLIP clip)
{
	if (weapon->loadtype != WEAP_LOADTYPE_CLIP){
		return DECREMENTCLIPFAILED_WRONGLOADTYPE;
	}
	if (weapon->loadprocedure.clip.isclipinserted == FALSE){
		return DECREMENTCLIPFAILED_NOCLIPINSERTED;
	}
	if (weapon->loadprocedure.clip.clipinserted.currentrounds < 1){
		return DECREMENTCLIPFAILED_OUTOFAMMO;
	}
	weapon->loadprocedure.clip.clipinserted.currentrounds--;
	return DECREMENTCLIP_OK;
}

int LoadMagazine(PWEAPON weapon, PLIST strayroundlist, int roundidentifier, int numbertoload)
{
	POTYPE tmpObj;
	PPOTYPE lstObj;
	int i;
	int roundoffset;
	
	if (weapon->loadtype != WEAP_LOADTYPE_MAGAZINE){
		return LOADMAGAZINEFAILED_WRONGLOADTYPE;
	}
	if (weapon->loadprocedure.magazine.totalroundschambered >= weapon->maxammo){
		return LOADMAGAZINEFAILED_MAGAZINEFULL;
	}
	if (weapon->loadprocedure.magazine.totalroundschambered+numbertoload > weapon->maxammo){
		return LOADMAGAZINEFAILED_TOOMANYROUNDSTOLOAD;
	}
	
	
	tmpObj.rounds.id = roundidentifier;
	lstObj = findObject(strayroundlist, &tmpObj, CompareStrayRoundsID);
	if (lstObj == NULL){
		return LOADMAGAZINEFAILED_NOMATCH;
	}
	if (lstObj->rounds.mainammotype != weapon->ammotype){
		return LOADMAGAZINEFAILED_WRONGAMMOTYPEFORWEAPON;
	}
	if (lstObj->rounds.numberofroundsleft < numbertoload){
		return LOADMAGAZINEFAILED_NOTENOUGHROUNDS;
	}
	roundoffset = weapon->loadprocedure.magazine.totalroundschambered;
	for (i = 0; i <  numbertoload; i++)
	{
		weapon->loadprocedure.magazine.chamberedrounds[i + roundoffset].mainammotype = lstObj->rounds.mainammotype;
		weapon->loadprocedure.magazine.chamberedrounds[i + roundoffset].subammotype = lstObj->rounds.subammotype;
		lstObj->rounds.numberofroundsleft--;
		weapon->loadprocedure.magazine.totalroundschambered++;
	}
	if(lstObj->rounds.numberofroundsleft < 1)
	{
		deleteObject(strayroundlist, lstObj);
		return LOADMAGAZINE_OK_REMOVED_NOROUNDS;
	}
	return LOADMAGAZINE_OK;
}
/*
int UnloadMagazine(PWEAPON weapon, PLIST strayroundlist)
{
	POTYPE tmpObj;
	PPOTYPE lstObj;
	//	int subammo;
	//	int i;
	
	int roundindextounload = weapon->loadprocedure.magazine.totalroundschambered;
	
	if (weapon->loadtype != WEAP_LOADTYPE_MAGAZINE){
		return UNLOADMAGAZINEFAILED_WRONGLOADTYPE;
	}
	if (weapon->loadprocedure.magazine.totalroundschambered < 1){
		return UNLOADMAGAZINEFAILED_MAGAZINEEMPTY;
	}
	tmpObj.rounds.mainammotype = weapon->loadprocedure.magazine.chamberedrounds[roundindextounload].mainammotype;
	tmpObj.rounds.subammotype = weapon->loadprocedure.magazine.chamberedrounds[roundindextounload].subammotype;
	lstObj = findObject(strayroundlist, &tmpObj, CompareMainandSubAmmoID);
	if(lstObj == NULL){
		tmpObj.rounds.numberofroundsleft = 1;
		tmpObj.rounds.id = strayroundlist->objcnt;
		addtoList(strayroundlist, tmpObj, CompareStrayRoundsID);
		return UNLOADMAGAZINE_OK;
		lstObj->rounds.numberofroundsleft++;
		return UNLOADMAGAZINE_OK;
	}
	return UNLOADMAGAZINEFAILED_FELLOUTOFLOOP;
}
*/


int FireMagazineShell(PWEAPON weapon, PSHELL shelldata)
{
	
//	int i;
	//	int k;
	
	if (weapon->loadtype != WEAP_LOADTYPE_MAGAZINE){
		return FIREMAGAZINEFAILED_WRONGLOADTYPE;
	}
	if (weapon->loadprocedure.magazine.totalroundschambered < 1){
		return FIREMAGAZINEFAILED_OUTOFAMMO;
	}
	shelldata = RetrieveGlobalShellData(&weapon->loadprocedure.magazine.chamberedrounds[0]);
	if(shelldata){
			DecrementMagazine(weapon);
			return FIREMAGAZINE_OK;
		}
		return FIREMAGAZINEFAILED_NOSUBMATCH;
	
	return FIREMAGAZINEFAILED_FELLOUTOFLOOP;
}

int FireMagazineBullet(PWEAPON weapon, PBULLET bulletdata)
{
	

//	int i;
	//	int k;
	
	if (weapon->loadtype != WEAP_LOADTYPE_MAGAZINE){
		return FIREMAGAZINEFAILED_WRONGLOADTYPE;
	}
	if (weapon->loadprocedure.magazine.totalroundschambered < 1){
		return FIREMAGAZINEFAILED_OUTOFAMMO;
	}
	bulletdata = RetrieveGlobalBulletData(&weapon->loadprocedure.magazine.chamberedrounds[0]);
	if(bulletdata){
			//WE'VE FILLED THE STRUCTURE, NOW REMOVE THE SHELL.
			DecrementMagazine(weapon);
			return FIREMAGAZINE_OK;
		}
		return FIREMAGAZINEFAILED_NOSUBMATCH;
	
	return FIREMAGAZINEFAILED_FELLOUTOFLOOP;
}
int FireMagazineExplosive(PWEAPON weapon, PEXPLOSIVE explosivedata)
{
	

//	int i;
	//	int k;
	
	if (weapon->loadtype != WEAP_LOADTYPE_MAGAZINE){
		return FIREMAGAZINEFAILED_WRONGLOADTYPE;
	}
	if (weapon->loadprocedure.magazine.totalroundschambered < 1){
		return FIREMAGAZINEFAILED_OUTOFAMMO;
	}
		explosivedata = RetrieveGlobalExplosiveData(&weapon->loadprocedure.magazine.chamberedrounds[0]);
	if(explosivedata){
			//WE'VE FILLED THE STRUCTURE, NOW REMOVE THE SHELL.
			DecrementMagazine(weapon);
			return FIREMAGAZINE_OK;
		}
		return FIREMAGAZINEFAILED_NOSUBMATCH;
	
	return FIREMAGAZINEFAILED_FELLOUTOFLOOP;
}

void DecrementMagazine(PWEAPON weapon)
{
	
	int k;
	
	for (k = 1; k < weapon->loadprocedure.magazine.totalroundschambered; k++){
		weapon->loadprocedure.magazine.chamberedrounds[k-1] = weapon->loadprocedure.magazine.chamberedrounds[k]; //WE'RE SLIDING THE MAGAZINE FORWARD.
	}
	weapon->loadprocedure.magazine.totalroundschambered--;
}

int LoadCylinder(PWEAPON weapon, PLIST strayroundlist, int roundidentifier, int chambertoload)
{
	POTYPE tmpObj;
	PPOTYPE lstObj;
	
	if (weapon->loadtype != WEAP_LOADTYPE_CYLINDER){
		return LOADCYLINDERFAILED_WRONGLOADTYPE;
	}
	if (weapon->loadprocedure.cylinder.statusindicators[chambertoload] != CYLINDER_CHAMBER_EMPTY){
		return LOADCYLINDERFAILED_CYLINDERFULL;
	}
	
	if (chambertoload > weapon->maxammo || chambertoload < 0){
		return LOADCYLINDERFAILED_BADCHAMBERNUMBER;
	}
	
	tmpObj.rounds.id = roundidentifier;
	
	lstObj = findObject(strayroundlist, &tmpObj, CompareStrayRoundsID);
	if (lstObj == NULL){
		return LOADCYLINDERFAILED_NOMATCH;
	}
	if (lstObj->rounds.numberofroundsleft < 1){
		return LOADCYLINDERFAILED_NOAMMOLEFT;
	}
	if (lstObj->rounds.mainammotype != weapon->ammotype){
		return LOADCYLINDERFAILED_WRONGAMMOTYPEFORWEAPON;
	}
	
	lstObj->rounds.numberofroundsleft--;
	weapon->loadprocedure.cylinder.chamberedrounds[chambertoload].mainammotype = lstObj->rounds.mainammotype;
	weapon->loadprocedure.cylinder.chamberedrounds[chambertoload].subammotype = lstObj->rounds.subammotype;
	weapon->loadprocedure.cylinder.statusindicators[chambertoload] = CYLINDER_CHAMBER_FULL;
	if(lstObj->rounds.numberofroundsleft < 1)
		return LOADCYLINDER_OK_REMOVED_NOROUNDS;
	else
	return LOADCYLINDER_OK;
}

int FireCylinderShell(PWEAPON weapon, PSHELL shelldata)
{
	//			POTYPE tmpObj;
	//	PPOTYPE lstObj;
	ROUND roundtofire;
	
	if (weapon->loadtype != WEAP_LOADTYPE_CYLINDER){
		return FIRECYLINDERFAILED_WRONGLOADTYPE;
	}
	if (weapon->loadprocedure.cylinder.statusindicators[weapon->loadprocedure.cylinder.currentposition] == CYLINDER_CHAMBER_EMPTY){
		return FIRECYLINDERFAILED_CYLINDEREMPTY;
	}
	if (weapon->loadprocedure.cylinder.statusindicators[weapon->loadprocedure.cylinder.currentposition] == CYLINDER_CHAMBER_SHELLONLY){
		return FIRECYLINDERFAILED_CYLINDERSHELLONLY;
	}
	roundtofire = weapon->loadprocedure.cylinder.chamberedrounds[weapon->loadprocedure.cylinder.currentposition];
	//SO IT CAN FIRE. LET'S CHECK IT OUT.
	shelldata = RetrieveGlobalShellData(&roundtofire);
	if (shelldata == NULL){
		return FIRECYLINDERFAILED_NOMATCH;
	}
	weapon->loadprocedure.cylinder.statusindicators[weapon->loadprocedure.cylinder.currentposition] = CYLINDER_CHAMBER_SHELLONLY;
	RotateCylinder(weapon);
	return FIRECYLINDER_OK;
}
int FireCylinderBullet(PWEAPON weapon, PBULLET bulletdata)
{
	//			POTYPE tmpObj;
	//	PPOTYPE lstObj;
	ROUND roundtofire;
	
	if (weapon->loadtype != WEAP_LOADTYPE_CYLINDER){
		return FIRECYLINDERFAILED_WRONGLOADTYPE;
	}
	if (weapon->loadprocedure.cylinder.statusindicators[weapon->loadprocedure.cylinder.currentposition] == CYLINDER_CHAMBER_EMPTY){
		return FIRECYLINDERFAILED_CYLINDEREMPTY;
	}
	if (weapon->loadprocedure.cylinder.statusindicators[weapon->loadprocedure.cylinder.currentposition] == CYLINDER_CHAMBER_SHELLONLY){
		return FIRECYLINDERFAILED_CYLINDERSHELLONLY;
	}
	roundtofire = weapon->loadprocedure.cylinder.chamberedrounds[weapon->loadprocedure.cylinder.currentposition];
	//SO IT CAN FIRE. LET'S CHECK IT OUT.
	bulletdata = RetrieveGlobalBulletData(&roundtofire);
	if (bulletdata == NULL){
		return FIRECYLINDERFAILED_NOMATCH;
	}
	weapon->loadprocedure.cylinder.statusindicators[weapon->loadprocedure.cylinder.currentposition] = CYLINDER_CHAMBER_SHELLONLY;
	RotateCylinder(weapon);
	return FIRECYLINDER_OK;
}
int FireCylinderExplosive(PWEAPON weapon, PEXPLOSIVE explosivedata)
{
	//					POTYPE tmpObj;
	//	PPOTYPE lstObj;
	ROUND roundtofire;
	
	if (weapon->loadtype != WEAP_LOADTYPE_CYLINDER){
		return FIRECYLINDERFAILED_WRONGLOADTYPE;
	}
	if (weapon->loadprocedure.cylinder.statusindicators[weapon->loadprocedure.cylinder.currentposition] == CYLINDER_CHAMBER_EMPTY){
		return FIRECYLINDERFAILED_CYLINDEREMPTY;
	}
	if (weapon->loadprocedure.cylinder.statusindicators[weapon->loadprocedure.cylinder.currentposition] == CYLINDER_CHAMBER_SHELLONLY){
		return FIRECYLINDERFAILED_CYLINDERSHELLONLY;
	}
	roundtofire = weapon->loadprocedure.cylinder.chamberedrounds[weapon->loadprocedure.cylinder.currentposition];
	//SO IT CAN FIRE. LET'S CHECK IT OUT.
	explosivedata = RetrieveGlobalExplosiveData(&roundtofire);
	if (explosivedata == NULL){
		return FIRECYLINDERFAILED_NOMATCH;
	}
	weapon->loadprocedure.cylinder.statusindicators[weapon->loadprocedure.cylinder.currentposition] = CYLINDER_CHAMBER_SHELLONLY;
	RotateCylinder(weapon);
	return FIRECYLINDER_OK;
}

int RotateCylinder(PWEAPON weapon)
{
	if (weapon->loadtype != WEAP_LOADTYPE_CYLINDER){
		return ROTATECYLINDERFAILED_WRONGLOADTYPE;
	}
	weapon->loadprocedure.cylinder.currentposition ++;
	if (weapon->loadprocedure.cylinder.currentposition >= weapon->maxammo){
		weapon->loadprocedure.cylinder.currentposition = 0;
	}
	return ROTATECYLINDER_OK;
}





/************************************************************
; THESE FUNCTIONS BELOW MUST BE CALLED UPON RECIEVING NEW WEAPONS FROM
THE LIST, AND AFTER REMOVING THEM. THESE ARE ONLY NECESSARY FOR WEAPONS
THAT ARE GOING TO FIRE, SUCH AS PLAYER WEAPONS OR NPC WEAPONS. THEY
MUST BE FREED AFTER THEY ARE NO LONGER IN USE. THAT MEANS ALL PLAYER
WEAPONS MUST BE CHECKED, ALL NPC WEAPONS, AND ALL WORLD WEAPONS.
************************************************************/

void InitWeaponMagazine(PWEAPON weapon)
{
	if (weapon->loadtype != WEAP_LOADTYPE_MAGAZINE){
		return;
	}
	weapon->loadprocedure.magazine.chamberedrounds = (PROUND) malloc(sizeof(ROUND) * weapon->maxammo);
	weapon->loadprocedure.magazine.totalroundschambered = 0;
}

void FreeWeaponMagazine(PWEAPON weapon)
{
	if (weapon->loadtype != WEAP_LOADTYPE_MAGAZINE){
		return;
	}
	free(weapon->loadprocedure.magazine.chamberedrounds);
}

void InitWeaponClip(PWEAPON weapon)
{
if (weapon->loadtype != WEAP_LOADTYPE_CLIP){
		return;
	}
weapon->loadprocedure.clip.isclipinserted = false;
weapon->loadprocedure.clip.clipinserted.clipround.mainammotype = 0;
weapon->loadprocedure.clip.clipinserted.clipround.subammotype = 0;
weapon->loadprocedure.clip.clipinserted.currentrounds = 0;
weapon->loadprocedure.clip.clipinserted.id = 0;
weapon->loadprocedure.clip.clipinserted.weaponid = 0;
}

void InitWeaponCylinder(PWEAPON weapon)
{
	if (weapon->loadtype != WEAP_LOADTYPE_CYLINDER){
		return;
	}
	weapon->loadprocedure.cylinder.chamberedrounds = (PROUND) malloc(sizeof(ROUND) * weapon->maxammo);
	weapon->loadprocedure.cylinder.statusindicators = (int*) malloc(sizeof(int) * weapon->maxammo);
	for(int i = 0; i < weapon->maxammo; i++){
		weapon->loadprocedure.cylinder.chamberedrounds[i].mainammotype = 0;
		weapon->loadprocedure.cylinder.chamberedrounds[i].subammotype = 0;
		weapon->loadprocedure.cylinder.statusindicators[i] = CYLINDER_CHAMBER_EMPTY;
	}
	weapon->loadprocedure.cylinder.currentposition = rand()%weapon->maxammo;
}

void FreeWeaponCylinder(PWEAPON weapon)
{
	if (weapon->loadtype != WEAP_LOADTYPE_CYLINDER){
		return;
	}
	free(weapon->loadprocedure.cylinder.chamberedrounds);
	free(weapon->loadprocedure.cylinder.statusindicators);
}
/*
int GiveArmoryWeapon(PPLAYER player, int weaponid)
{
	POTYPE tmpObj;
	PPOTYPE lstObj;
	
	tmpObj.weapon.id = weaponid;
	lstObj = findObject(globals.dsaweaponsammo.weaponlist, &tmpObj, CompareWeaponID);
	if (lstObj == NULL){
		return GIVEARMORYWEAPONFAILED_NOMATCH;
	}

	if (lstObj->weapon.loadtype == WEAP_LOADTYPE_CLIP){
		InitWeaponClip(&lstObj->weapon);
	}
	
	else if (lstObj->weapon.loadtype == WEAP_LOADTYPE_MAGAZINE){
		InitWeaponMagazine(&lstObj->weapon);
	}
	else if (lstObj->weapon.loadtype == WEAP_LOADTYPE_CYLINDER){
		InitWeaponCylinder(&lstObj->weapon);
	}
	lstObj->weapon.index = player->p_weapons->objcnt; //GIVE UNIQUE INDEX;
	addtoList(player->p_weapons, *lstObj, CompareWeaponIndex);
	return GIVEARMORYWEAPON_OK;
}
*/
int GiveTrainingWeapon(PPLAYER player, int weaponid)
{
	POTYPE tmpObj;
	PPOTYPE lstObj;
	POTYPE newweapon;

	

	//WE MUST CREATE A NEW WEAPON. THE WEAPON WILL SHARE THE DESCRIPTION OF THE PARENT WEAPON
	//TO SAVE MEMORY, BUT IT MUST BE A UNIQUE WEAPON SO THAT WE DON'T HAVE WEAPON SHARING.
	
	tmpObj.weapon.id = weaponid;
	lstObj = findObject(globals.globalweaponsammo.weaponlist, &tmpObj, CompareWeaponID);
	
	newweapon.weapon.ammotype = lstObj->weapon.ammotype;
	newweapon.weapon.attacktype = lstObj->weapon.attacktype;
	newweapon.weapon.classtype = lstObj->weapon.classtype;
	newweapon.weapon.condition_percent = lstObj->weapon.condition_percent;
	newweapon.weapon.desc = lstObj->weapon.desc;
	newweapon.weapon.drug.doseloaded = lstObj->weapon.drug.doseloaded;
	newweapon.weapon.drug.doses = lstObj->weapon.drug.doses;
	newweapon.weapon.drug.type = lstObj->weapon.drug.type;
	newweapon.weapon.dsaweapon = lstObj->weapon.dsaweapon;
	newweapon.weapon.filingtype = lstObj->weapon.filingtype;
	newweapon.weapon.frequency = lstObj->weapon.frequency;
	newweapon.weapon.id = lstObj->weapon.id;
	newweapon.weapon.index = lstObj->weapon.index;
	newweapon.weapon.maxammo = lstObj->weapon.maxammo;
	newweapon.weapon.modifiersactivated.flashsuppressor = false;
	newweapon.weapon.modifiersactivated.silencer = false;
	newweapon.weapon.modifiersactivated.lasersight = false;
	newweapon.weapon.modifiersattached.flashsuppressor = false;
	newweapon.weapon.modifiersattached.silencer = false;
	newweapon.weapon.modifiersattached.lasersight = false;
	newweapon.weapon.modifiersallowed.flashsuppressor = lstObj->weapon.modifiersallowed.flashsuppressor;
	newweapon.weapon.modifiersallowed.silencer = lstObj->weapon.modifiersallowed.silencer;
	newweapon.weapon.modifiersallowed.lasersight = lstObj->weapon.modifiersallowed.lasersight;
	newweapon.weapon.loadtype = lstObj->weapon.loadtype;
	strcpy(newweapon.weapon.longname, lstObj->weapon.longname);
	strcpy(newweapon.weapon.shortname, lstObj->weapon.shortname);
	for(int i = 0; i < MAX_WEAPON_SOUNDS; i++){
	newweapon.weapon.soundfilename[i] = lstObj->weapon.soundfilename[i];
	}
	newweapon.weapon.weaponavailable = lstObj->weapon.weaponavailable;
	newweapon.weapon.weapperf = lstObj->weapon.weapperf;
	newweapon.weapon.weight = lstObj->weapon.weight;



	if (lstObj == NULL){
		return GIVETRAININGWEAPONFAILED_NOMATCH;
	}
	if (lstObj->weapon.loadtype == WEAP_LOADTYPE_CLIP){
		InitWeaponClip(&newweapon.weapon);
	}
	else if (lstObj->weapon.loadtype == WEAP_LOADTYPE_MAGAZINE){
		InitWeaponMagazine(&newweapon.weapon);
	}
	else if (lstObj->weapon.loadtype == WEAP_LOADTYPE_CYLINDER){
		InitWeaponCylinder(&newweapon.weapon);
	}
	newweapon.weapon.index = player->p_weapons->objcnt; //GIVE UNIQUE INDEX;
	addtoList(player->p_weapons, newweapon, CompareWeaponIndex);
	return GIVETRAININGWEAPON_OK;
}

bool IsWeaponShootable(PWEAPON weapon)
{
	bool shootable = false;

	if (weapon->loadtype == WEAP_LOADTYPE_CLIP && weapon->ammotype){
		
		shootable = true;
	}
	else if (weapon->loadtype == WEAP_LOADTYPE_MAGAZINE && weapon->ammotype){
		
		shootable = true;
	}
	else if (weapon->loadtype == WEAP_LOADTYPE_CYLINDER && weapon->ammotype){
		shootable = true;
	}
	return shootable;
}

bool GiveLoadedTrainingWeapon(PLIST weaponlist, PPLAYERAMMO ammolist, int weaponid, int subammoid)
{
	POTYPE tmpObj;
	PPOTYPE lstObj;
	bool shootable = false;
	int i;
	int clipid;
	ROUND round;
		POTYPE newweapon;

	

	//WE MUST CREATE A NEW WEAPON. THE WEAPON WILL SHARE THE DESCRIPTION OF THE PARENT WEAPON
	//TO SAVE MEMORY, BUT IT MUST BE A UNIQUE WEAPON SO THAT WE DON'T HAVE WEAPON SHARING.
	
	tmpObj.weapon.id = weaponid;
	lstObj = findObject(globals.globalweaponsammo.weaponlist, &tmpObj, CompareWeaponID);

	if (lstObj == NULL){
		return false;
	}
	
	newweapon.weapon.ammotype = lstObj->weapon.ammotype;
	newweapon.weapon.attacktype = lstObj->weapon.attacktype;
	newweapon.weapon.classtype = lstObj->weapon.classtype;
	newweapon.weapon.condition_percent = lstObj->weapon.condition_percent;
	newweapon.weapon.desc = lstObj->weapon.desc;
	newweapon.weapon.drug.doseloaded = lstObj->weapon.drug.doseloaded;
	newweapon.weapon.drug.doses = lstObj->weapon.drug.doses;
	newweapon.weapon.drug.type = lstObj->weapon.drug.type;
	newweapon.weapon.dsaweapon = lstObj->weapon.dsaweapon;
	newweapon.weapon.filingtype = lstObj->weapon.filingtype;
	newweapon.weapon.frequency = lstObj->weapon.frequency;
	newweapon.weapon.id = lstObj->weapon.id;
	newweapon.weapon.index = lstObj->weapon.index;
	newweapon.weapon.maxammo = lstObj->weapon.maxammo;
	newweapon.weapon.modifiersactivated.flashsuppressor = false;
	newweapon.weapon.modifiersactivated.silencer = false;
	newweapon.weapon.modifiersactivated.lasersight = false;
	newweapon.weapon.modifiersattached.flashsuppressor = false;
	newweapon.weapon.modifiersattached.silencer = false;
	newweapon.weapon.modifiersattached.lasersight = false;
	newweapon.weapon.modifiersallowed.flashsuppressor = lstObj->weapon.modifiersallowed.flashsuppressor;
	newweapon.weapon.modifiersallowed.silencer = lstObj->weapon.modifiersallowed.silencer;
	newweapon.weapon.modifiersallowed.lasersight = lstObj->weapon.modifiersallowed.lasersight;
	newweapon.weapon.loadtype = lstObj->weapon.loadtype;
	strcpy(newweapon.weapon.longname, lstObj->weapon.longname);
	strcpy(newweapon.weapon.shortname, lstObj->weapon.shortname);
	for(i = 0; i < MAX_WEAPON_SOUNDS; i++){
	newweapon.weapon.soundfilename[i] = lstObj->weapon.soundfilename[i];
	}
	newweapon.weapon.weaponavailable = lstObj->weapon.weaponavailable;
	newweapon.weapon.weapperf = lstObj->weapon.weapperf;
	newweapon.weapon.weight = lstObj->weapon.weight;
	
	
	
	if (lstObj->weapon.loadtype == WEAP_LOADTYPE_CLIP){
		InitWeaponClip(&newweapon.weapon);
	
	}
	else if (lstObj->weapon.loadtype == WEAP_LOADTYPE_MAGAZINE){
		InitWeaponMagazine(&newweapon.weapon);
		
	}
	else if (lstObj->weapon.loadtype == WEAP_LOADTYPE_CYLINDER){
		InitWeaponCylinder(&newweapon.weapon);
	
	}
	shootable = IsWeaponShootable(&lstObj->weapon);
	newweapon.weapon.index = weaponlist->objcnt; //GIVE UNIQUE INDEX;
	

	//NOW CHECK THE LOADING PROC.
	if(shootable){
	switch(newweapon.weapon.loadtype)
	{
	case WEAP_LOADTYPE_CLIP:
		clipid = ammolist->cliplist->objcnt;
		if (GiveClip(ammolist->cliplist, &newweapon.weapon, subammoid, globals.globalweaponsammo.ammolist, NULL, true) != GETCLIP_OK){
			return false;
		}
		LoadClip(ammolist->cliplist, &newweapon.weapon, clipid);
		break;
	case WEAP_LOADTYPE_MAGAZINE:
		round.mainammotype = newweapon.weapon.ammotype;
		round.subammotype = subammoid;
		if(RetrieveGlobalSubAmmoType(&round) == RETRIEVEGLOBALSUBAMMOTYPEFAILED){
				return false;
			}
		for(i = 0; i < newweapon.weapon.maxammo; i++){//LOAD IT FULL, BABY!
			newweapon.weapon.loadprocedure.magazine.chamberedrounds[i].mainammotype = round.mainammotype;
			newweapon.weapon.loadprocedure.magazine.chamberedrounds[i].subammotype = round.subammotype;
			newweapon.weapon.loadprocedure.magazine.totalroundschambered++;
		}
		break;
	case WEAP_LOADTYPE_CYLINDER:
		round.mainammotype = newweapon.weapon.ammotype;
		round.subammotype = subammoid;
		if(RetrieveGlobalSubAmmoType(&round) == RETRIEVEGLOBALSUBAMMOTYPEFAILED){
				return false;
			}
		for(i = 0; i < newweapon.weapon.maxammo; i++){//LOAD IT FULL, BABY!
			newweapon.weapon.loadprocedure.cylinder.chamberedrounds[i].mainammotype = round.mainammotype;
			newweapon.weapon.loadprocedure.cylinder.chamberedrounds[i].subammotype = round.subammotype;
			newweapon.weapon.loadprocedure.cylinder.statusindicators[i] = CYLINDER_CHAMBER_FULL;
		}
		newweapon.weapon.loadprocedure.cylinder.currentposition = 0;
		break;
	}
	
	
	}
	addtoList(weaponlist, newweapon, CompareWeaponIndex);
	return true;
}

int GiveTrainingWeapon(PLIST weaponlisttomodify, int weaponid)
{
			POTYPE tmpObj;
	PPOTYPE lstObj;
	POTYPE newweapon;
	
	tmpObj.weapon.id = weaponid;
	lstObj = findObject(globals.globalweaponsammo.weaponlist, &tmpObj, CompareWeaponID);
	if (lstObj == NULL){
		return GIVETRAININGWEAPONFAILED_NOMATCH;
	}


	newweapon.weapon.ammotype = lstObj->weapon.ammotype;
	newweapon.weapon.attacktype = lstObj->weapon.attacktype;
	newweapon.weapon.classtype = lstObj->weapon.classtype;
	newweapon.weapon.condition_percent = lstObj->weapon.condition_percent;
	newweapon.weapon.desc = lstObj->weapon.desc;
	newweapon.weapon.drug.doseloaded = lstObj->weapon.drug.doseloaded;
	newweapon.weapon.drug.doses = lstObj->weapon.drug.doses;
	newweapon.weapon.drug.type = lstObj->weapon.drug.type;
	newweapon.weapon.dsaweapon = lstObj->weapon.dsaweapon;
	newweapon.weapon.filingtype = lstObj->weapon.filingtype;
	newweapon.weapon.frequency = lstObj->weapon.frequency;
	newweapon.weapon.id = lstObj->weapon.id;
	newweapon.weapon.index = lstObj->weapon.index;
	newweapon.weapon.maxammo = lstObj->weapon.maxammo;
	newweapon.weapon.modifiersactivated.flashsuppressor = false;
	newweapon.weapon.modifiersactivated.silencer = false;
	newweapon.weapon.modifiersactivated.lasersight = false;
	newweapon.weapon.modifiersattached.flashsuppressor = false;
	newweapon.weapon.modifiersattached.silencer = false;
	newweapon.weapon.modifiersattached.lasersight = false;
	newweapon.weapon.modifiersallowed.flashsuppressor = lstObj->weapon.modifiersallowed.flashsuppressor;
	newweapon.weapon.modifiersallowed.silencer = lstObj->weapon.modifiersallowed.silencer;
	newweapon.weapon.modifiersallowed.lasersight = lstObj->weapon.modifiersallowed.lasersight;
	newweapon.weapon.loadtype = lstObj->weapon.loadtype;
	strcpy(newweapon.weapon.longname, lstObj->weapon.longname);
	strcpy(newweapon.weapon.shortname, lstObj->weapon.shortname);
	for(int i = 0; i < MAX_WEAPON_SOUNDS; i++){
	newweapon.weapon.soundfilename[i] = lstObj->weapon.soundfilename[i];
	}
	newweapon.weapon.weaponavailable = lstObj->weapon.weaponavailable;
	newweapon.weapon.weapperf = lstObj->weapon.weapperf;
	newweapon.weapon.weight = lstObj->weapon.weight;


	if (lstObj->weapon.loadtype == WEAP_LOADTYPE_CLIP){
		InitWeaponClip(&newweapon.weapon);
	}
	else if (lstObj->weapon.loadtype == WEAP_LOADTYPE_MAGAZINE){
		InitWeaponMagazine(&newweapon.weapon);
	}
	else if (lstObj->weapon.loadtype == WEAP_LOADTYPE_CYLINDER){
		InitWeaponCylinder(&newweapon.weapon);
	}
	newweapon.weapon.index = weaponlisttomodify->objcnt; //GIVE UNIQUE INDEX;
	addtoList(weaponlisttomodify, newweapon, CompareWeaponIndex);
	return GIVETRAININGWEAPON_OK;
}

int GiveTrainingWeapon(PENEMY enemy, int weaponid)
{
		POTYPE tmpObj;
	PPOTYPE lstObj;
	POTYPE newweapon;
	
	tmpObj.weapon.id = weaponid;
	lstObj = findObject(globals.globalweaponsammo.weaponlist, &tmpObj, CompareWeaponID);
	if (lstObj == NULL){
		return GIVETRAININGWEAPONFAILED_NOMATCH;
	}


	newweapon.weapon.ammotype = lstObj->weapon.ammotype;
	newweapon.weapon.attacktype = lstObj->weapon.attacktype;
	newweapon.weapon.classtype = lstObj->weapon.classtype;
	newweapon.weapon.condition_percent = lstObj->weapon.condition_percent;
	newweapon.weapon.desc = lstObj->weapon.desc;
	newweapon.weapon.drug.doseloaded = lstObj->weapon.drug.doseloaded;
	newweapon.weapon.drug.doses = lstObj->weapon.drug.doses;
	newweapon.weapon.drug.type = lstObj->weapon.drug.type;
	newweapon.weapon.dsaweapon = lstObj->weapon.dsaweapon;
	newweapon.weapon.filingtype = lstObj->weapon.filingtype;
	newweapon.weapon.frequency = lstObj->weapon.frequency;
	newweapon.weapon.id = lstObj->weapon.id;
	newweapon.weapon.index = lstObj->weapon.index;
	newweapon.weapon.maxammo = lstObj->weapon.maxammo;
	newweapon.weapon.modifiersactivated.flashsuppressor = false;
	newweapon.weapon.modifiersactivated.silencer = false;
	newweapon.weapon.modifiersactivated.lasersight = false;
	newweapon.weapon.modifiersattached.flashsuppressor = false;
	newweapon.weapon.modifiersattached.silencer = false;
	newweapon.weapon.modifiersattached.lasersight = false;
	newweapon.weapon.modifiersallowed.flashsuppressor = lstObj->weapon.modifiersallowed.flashsuppressor;
	newweapon.weapon.modifiersallowed.silencer = lstObj->weapon.modifiersallowed.silencer;
	newweapon.weapon.modifiersallowed.lasersight = lstObj->weapon.modifiersallowed.lasersight;
	newweapon.weapon.loadtype = lstObj->weapon.loadtype;
	strcpy(newweapon.weapon.longname, lstObj->weapon.longname);
	strcpy(newweapon.weapon.shortname, lstObj->weapon.shortname);
	for(int i = 0; i < MAX_WEAPON_SOUNDS; i++){
	newweapon.weapon.soundfilename[i] = lstObj->weapon.soundfilename[i];
	}
	newweapon.weapon.weaponavailable = lstObj->weapon.weaponavailable;
	newweapon.weapon.weapperf = lstObj->weapon.weapperf;
	newweapon.weapon.weight = lstObj->weapon.weight;


	if (lstObj->weapon.loadtype == WEAP_LOADTYPE_CLIP){
		InitWeaponClip(&newweapon.weapon);
	}
	else if (lstObj->weapon.loadtype == WEAP_LOADTYPE_MAGAZINE){
		InitWeaponMagazine(&newweapon.weapon);
	}
	else if (lstObj->weapon.loadtype == WEAP_LOADTYPE_CYLINDER){
		InitWeaponCylinder(&newweapon.weapon);
	}
	newweapon.weapon.index = enemy->weaponlist->objcnt; //GIVE UNIQUE INDEX;
	addtoList(enemy->weaponlist, newweapon, CompareWeaponIndex);
	return GIVETRAININGWEAPON_OK;
}


int RemovePlayerWeapon(PPLAYER player, int index)
{
	POTYPE tmpObj;
	PPOTYPE lstObj;
	
	tmpObj.weapon.index = index;
	lstObj = findObject(player->p_weapons, &tmpObj, CompareWeaponIndex);
	if (lstObj == NULL){
		return REMOVEPLAYERWEAPONFAILED_NOMATCH;
	}
	if (lstObj->weapon.loadtype == WEAP_LOADTYPE_MAGAZINE){
		FreeWeaponMagazine(&lstObj->weapon);
	}
	else if (lstObj->weapon.loadtype == WEAP_LOADTYPE_CYLINDER){
		FreeWeaponCylinder(&lstObj->weapon);
	}
	deleteObject(player->p_weapons, lstObj);
	return REMOVEPLAYERWEAPON_OK;
}

int RemoveWeapon(PLIST weaponlist, int index)
{
	POTYPE tmpObj;
	PPOTYPE lstObj;

	if(!weaponlist){
		return REMOVEWEAPONFAILED_NOMATCH;
	}
	
	tmpObj.weapon.index = index;
	lstObj = findObject(weaponlist, &tmpObj, CompareWeaponIndex);
	if (lstObj == NULL){
		return REMOVEWEAPONFAILED_NOMATCH;
	}
	if (lstObj->weapon.loadtype == WEAP_LOADTYPE_MAGAZINE){
		FreeWeaponMagazine(&lstObj->weapon);
	}
	else if (lstObj->weapon.loadtype == WEAP_LOADTYPE_CYLINDER){
		FreeWeaponCylinder(&lstObj->weapon);
	}
	deleteObject(weaponlist, lstObj);
	return REMOVEWEAPON_OK;
}

void RemoveAllPlayerWeapons(PPLAYER player)
{
	if(!player->p_weapons || player->p_weapons->objcnt < 1){
		return;
	}
	PLIST walker = player->p_weapons;
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		if (walker->current->object.weapon.loadtype == WEAP_LOADTYPE_MAGAZINE){
		FreeWeaponMagazine(&walker->current->object.weapon);
	}
	else if (walker->current->object.weapon.loadtype == WEAP_LOADTYPE_CYLINDER){
		FreeWeaponCylinder(&walker->current->object.weapon);
	}
	}
	freeList(player->p_weapons); //THIS DOESN'T FREE THE GLOBAL DESCRIPTION.
	free(player->p_weapons);
	player->p_weapons = NULL;
}

void RemoveAllWeapons(PLIST list)
{
	if(!list)
	{
		return;
	}
	PLIST walker = list;
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		if (walker->current->object.weapon.loadtype == WEAP_LOADTYPE_MAGAZINE){
		FreeWeaponMagazine(&walker->current->object.weapon);
	}
	else if (walker->current->object.weapon.loadtype == WEAP_LOADTYPE_CYLINDER){
		FreeWeaponCylinder(&walker->current->object.weapon);
	}
	}
	freeList(list); //THIS DOESN'T FREE THE GLOBAL DESCRIPTION.
}


PWEAPON RetrieveGlobalWeaponDataByID(int id){ //YOU CAN GET WEAPON DATA SEQUENTIALLY
	PLIST walker;
	walker = globals.globalweaponsammo.weaponlist;
	walker->current = walker->head;

	for (int i = 0; i < globals.globalweaponsammo.weaponlist->objcnt; i++){
		if (id == i){
			return &walker->current->object.weapon;
		}
		walker->current = walker->current->nextnode;
	}
	return NULL;
}

PWEAPON RetrieveGlobalWeaponDataByIndex(int weaponid){
	POTYPE tmpObj;
	PPOTYPE lstObj;
	
	tmpObj.weapon.id = weaponid;
	lstObj = findObject(globals.globalweaponsammo.weaponlist, &tmpObj, CompareWeaponID);
	return (&lstObj->weapon);
}

char* RetrieveSubAmmoShortName(PROUND round)
{
	int type;
	PSHELL shell;
	PBULLET bullet;
	PEXPLOSIVE explosive;
	
	type = RetrieveGlobalSubAmmoType(round);
		
		switch(type){
		case AMMOTYPE_BULLET:
			bullet = RetrieveGlobalBulletData(round);
			if (bullet != NULL)
			{
				return bullet->shortname;
			}
			return NULL;
		case AMMOTYPE_SHOTGUNSHELL:
			shell = RetrieveGlobalShellData(round);
			if (shell != NULL)
			{
				return shell->shortname;
			}
			return NULL;
		case AMMOTYPE_EXPLOSIVE:
			explosive = RetrieveGlobalExplosiveData(round);
			if (explosive != NULL)
			{
				return explosive->shortname;
			}
			return NULL;
		default:
			return NULL;
	}
	return NULL;
}

BOOL IsSubAmmoDSAOnly(PROUND round)
{
			int type;
	PSHELL shell;
	PBULLET bullet;
	PEXPLOSIVE explosive;
	
	type = RetrieveGlobalSubAmmoType(round);
		
		switch(type){
		case AMMOTYPE_BULLET:
			bullet = RetrieveGlobalBulletData(round);
			if (bullet != NULL)
			{
				return bullet->dsaammo;
			}
			return false;
		case AMMOTYPE_SHOTGUNSHELL:
			shell = RetrieveGlobalShellData(round);
			if (shell != NULL)
			{
				return shell->dsaammo;
			}
			return false;
		case AMMOTYPE_EXPLOSIVE:
			explosive = RetrieveGlobalExplosiveData(round);
			if (explosive != NULL)
			{
				return explosive->dsaammo;
			}
			return false;
		default:
			return false;
	}
	return false;
}

int RetrieveSubAmmoNumberRoundsLeft(PROUND round)
{
		int type;
	PSHELL shell;
	PBULLET bullet;
	PEXPLOSIVE explosive;
	
	type = RetrieveGlobalSubAmmoType(round);
		
		switch(type){
		case AMMOTYPE_BULLET:
			bullet = RetrieveGlobalBulletData(round);
			if (bullet != NULL && bullet->ammoavailable)
			{
				return bullet->roundsavailable;
			}
			return 0;
		case AMMOTYPE_SHOTGUNSHELL:
			shell = RetrieveGlobalShellData(round);
			if (shell != NULL && shell->ammoavailable)
			{
				return shell->roundsavailable;
			}
			return 0;
		case AMMOTYPE_EXPLOSIVE:
			explosive = RetrieveGlobalExplosiveData(round);
			if (explosive != NULL && explosive->ammoavailable)
			{
				return explosive->roundsavailable;
			}
			return 0;
		default:
			return 0;
	}
	return 0;
}

char* RetrieveSubAmmoLongName(PROUND round)
{
	int type;
	PSHELL shell;
	PBULLET bullet;
	PEXPLOSIVE explosive;
	
	type = RetrieveGlobalSubAmmoType(round);
		
		switch(type){
		case AMMOTYPE_BULLET:
			bullet = RetrieveGlobalBulletData(round);
			if (bullet != NULL)
			{
				return bullet->longname;
			}
			return NULL;
		case AMMOTYPE_SHOTGUNSHELL:
			shell = RetrieveGlobalShellData(round);
			if (shell != NULL)
			{
				return shell->longname;
			}
			return NULL;
		case AMMOTYPE_EXPLOSIVE:
			explosive = RetrieveGlobalExplosiveData(round);
			if (explosive != NULL)
			{
				return explosive->longname;
			}
			return NULL;
		default:
			return NULL;
	}
	return NULL;
}

char* RetrieveMainAmmoName(PROUND round)
{
	PAMMO ammo;
	
	ammo = GetMainAmmoByID(globals.globalweaponsammo.ammolist, round->mainammotype);
	if (ammo != NULL)
	{
		return ammo->name;
	}
	
	return NULL;
}

int GetWeightedRandomWeaponID(PLIST weaponlist)
{
		PLIST Walker;
	if(!weaponlist || weaponlist->objcnt < 1){
		return -1;
	}
	int i;
	PWEAPON CurrWeap;

	Walker = weaponlist;

	int FilingType = rand()%(MAX_WEAP_FILING_TYPES - 1);
	FilingType++; //WE'RE IN RANGE.
	//NOW FILL AN ARRAY WITH THE CORRECT NUMBER OF FOUND WEAPON POINTERS OF THIS TYPE.
	PWEAPON* Weapons = (PWEAPON*)malloc(sizeof(PWEAPON) * weaponlist->objcnt);
	for(i = 0, Walker->current = Walker->head; Walker->current != NULL; Walker->current = Walker->current->nextnode)
	{
		if(Walker->current->object.weapon.filingtype == FilingType){
			Weapons[i] = &Walker->current->object.weapon;
			i++;
		}
	}

	if(i < 1){
		return -1;
	}

	if(i == 1){
		return Weapons[0]->id;
	}

	//NOW WE HAVE THE ARRAY, GET THE DATA.
	while(1)
	{
		CurrWeap = Weapons[rand()%i];
		if(!CurrWeap){
			continue;
		}
		if(RandomPercentage(CurrWeap->frequency)){
			return CurrWeap->id;
		}
	}
}

int GetWeightedNONDSARandomWeaponID(PLIST weaponlist)
{
	PLIST Walker;
	if(!weaponlist || weaponlist->objcnt < 1){
		return -1;
	}
	int i;
	PWEAPON CurrWeap;

	Walker = weaponlist;

	int FilingType = rand()%(MAX_WEAP_FILING_TYPES - 1);
	FilingType++; //WE'RE IN RANGE.
	//NOW FILL AN ARRAY WITH THE CORRECT NUMBER OF FOUND WEAPON POINTERS OF THIS TYPE.
	PWEAPON* Weapons = (PWEAPON*)malloc(sizeof(PWEAPON) * weaponlist->objcnt);
	for(i = 0, Walker->current = Walker->head; Walker->current != NULL; Walker->current = Walker->current->nextnode)
	{
		if(Walker->current->object.weapon.filingtype == FilingType && !Walker->current->object.weapon.dsaweapon){
			Weapons[i] = &Walker->current->object.weapon;
			i++;
		}
	}

	if(i < 1){
		return -1;
	}

	if(i == 1){
		return Weapons[0]->id;
	}

	//NOW WE HAVE THE ARRAY, GET THE DATA.
	while(1)
	{
		CurrWeap = Weapons[rand()%i];
		if(!CurrWeap){
			continue;
		}
		if(RandomPercentage(CurrWeap->frequency)){
			return CurrWeap->id;
		}
	}
}

bool GiveRandomExtraAmmo(PPLAYERAMMO Ammolisttomodify, int WeaponID)
{
		POTYPE TempObj;
	PPOTYPE ListObj;
	PWEAPON Weapon;
	TempObj.weapon.id = WeaponID;
	PAMMO Ammo;
	int* IDs;
	int* Frequencies;
	int NumIDs = 0;
	int CorrectID;
	ListObj = findObject(globals.globalweaponsammo.weaponlist, &TempObj, CompareWeaponID);
	if(!ListObj){
		return false;
	}
	Weapon = &ListObj->weapon;
	if(!Weapon){
		return false;
	}
	TempObj.ammo.id = Weapon->ammotype;
	ListObj = findObject(globals.globalweaponsammo.ammolist, &TempObj, CompareAmmoID);
	if(!ListObj){
		return false;
	}
	Ammo = &ListObj->ammo;
	if(!Ammo){
		return false;
	}
	
	IDs = (int*)malloc(sizeof(int) * Ammo->maxammotypes);
	Frequencies = (int*)malloc(sizeof(int) * Ammo->maxammotypes);
	for(int i = 0; i < Ammo->maxammotypes; i++){
		switch(Ammo->type)
		{
		case AMMOTYPE_BULLET:
				IDs[NumIDs] = Ammo->specialammo.bulletdata[i].id;
				Frequencies[NumIDs] = Ammo->specialammo.bulletdata[i].frequency;
				NumIDs++;
			break;
		case AMMOTYPE_SHOTGUNSHELL:
				IDs[NumIDs] = Ammo->specialammo.shelldata[i].id;
				Frequencies[NumIDs] = Ammo->specialammo.shelldata[i].frequency;
				NumIDs++;
			break;
		case AMMOTYPE_EXPLOSIVE:
				IDs[NumIDs] = Ammo->specialammo.explosivedata[i].id;
				Frequencies[NumIDs] = Ammo->specialammo.explosivedata[i].frequency;
				NumIDs++;
			break;
		}
	}
	
	if(NumIDs < 1){
		free(IDs);
		free(Frequencies);
		return false;
	}
	if(NumIDs == 1){
		CorrectID = IDs[0];
	}
	else{ //GET AN ID.............
		while(1){
			CorrectID = IDs[rand()%NumIDs];
			if(RandomPercentage(Frequencies[CorrectID])){
				break;
			}
		}
	}
	
	ROUND Round;
	//NOW GIVE IT TO THEM.
	switch(Weapon->loadtype)
	{
	case WEAP_LOADTYPE_CLIP: //GIVE CLIP
		if(GiveClip(Ammolisttomodify->cliplist, Weapon, CorrectID, globals.globalweaponsammo.ammolist, NULL, true) < GETCLIP_OK){
			free(IDs);
			free(Frequencies);
			return false;
		}
		break;
	default: //GIVE STRAY ROUNDS
		Round.mainammotype = Weapon->ammotype;
		Round.subammotype = CorrectID;
		if(GiveStrayRounds(Ammolisttomodify->roundtypes, &Round, Weapon->maxammo, globals.globalweaponsammo.ammolist, NULL, true) < GETSTRAYROUNDS_OK){
			free(IDs);
			free(Frequencies);
			return false;
		}
		break;
	}
	
	free(IDs);
	free(Frequencies);
	return true;
}

bool GiveRandomNONDSAExtraAmmo(PPLAYERAMMO Ammolisttomodify, int WeaponID)
{
	POTYPE TempObj;
	PPOTYPE ListObj;
	PWEAPON Weapon;
	TempObj.weapon.id = WeaponID;
	PAMMO Ammo;
	int* IDs;
	int* Frequencies;
	int NumIDs = 0;
	int CorrectID;
	ListObj = findObject(globals.globalweaponsammo.weaponlist, &TempObj, CompareWeaponID);
	if(!ListObj){
		return false;
	}
	Weapon = &ListObj->weapon;
	if(!Weapon){
		return false;
	}
	TempObj.ammo.id = Weapon->ammotype;
	ListObj = findObject(globals.globalweaponsammo.ammolist, &TempObj, CompareAmmoID);
	if(!ListObj){
		return false;
	}
	Ammo = &ListObj->ammo;
	if(!Ammo){
		return false;
	}
	
	IDs = (int*)malloc(sizeof(int) * Ammo->maxammotypes);
	Frequencies = (int*)malloc(sizeof(int) * Ammo->maxammotypes);
	for(int i = 0; i < Ammo->maxammotypes; i++){
		switch(Ammo->type)
		{
		case AMMOTYPE_BULLET:
			if(!Ammo->specialammo.bulletdata[i].dsaammo){
				IDs[NumIDs] = Ammo->specialammo.bulletdata[i].id;
				Frequencies[NumIDs] = Ammo->specialammo.bulletdata[i].frequency;
				NumIDs++;
			}
			break;
		case AMMOTYPE_SHOTGUNSHELL:
			if(!Ammo->specialammo.shelldata[i].dsaammo){
				IDs[NumIDs] = Ammo->specialammo.shelldata[i].id;
				Frequencies[NumIDs] = Ammo->specialammo.shelldata[i].frequency;
				NumIDs++;
			}
			break;
		case AMMOTYPE_EXPLOSIVE:
			if(!Ammo->specialammo.explosivedata[i].dsaammo){
				IDs[NumIDs] = Ammo->specialammo.explosivedata[i].id;
				Frequencies[NumIDs] = Ammo->specialammo.explosivedata[i].frequency;
				NumIDs++;
			}
			break;
		}
	}
	
	if(NumIDs < 1){
		free(IDs);
		free(Frequencies);
		return false;
	}
	if(NumIDs == 1){
		CorrectID = IDs[0];
	}
	else{ //GET AN ID.............
		while(1){
			CorrectID = IDs[rand()%NumIDs];
			if(RandomPercentage(Frequencies[CorrectID])){
				break;
			}
		}
	}
	
	ROUND Round;
	//NOW GIVE IT TO THEM.
	switch(Weapon->loadtype)
	{
	case WEAP_LOADTYPE_CLIP: //GIVE CLIP
		if(GiveClip(Ammolisttomodify->cliplist, Weapon, CorrectID, globals.globalweaponsammo.ammolist, NULL, true) < GETCLIP_OK){
			free(IDs);
			free(Frequencies);
			return false;
		}
		break;
	default: //GIVE STRAY ROUNDS
		Round.mainammotype = Weapon->ammotype;
		Round.subammotype = CorrectID;
		if(GiveStrayRounds(Ammolisttomodify->roundtypes, &Round, Weapon->maxammo, globals.globalweaponsammo.ammolist, NULL, true) < GETSTRAYROUNDS_OK){
			free(IDs);
			free(Frequencies);
			return false;
		}
		break;
	}
	
	free(IDs);
	free(Frequencies);
	return true;
}

int CompareClipWeaponID(PPOTYPE itm1, PPOTYPE itm2)
{
	if(itm1->clip.weaponid == itm2->clip.weaponid){
		return 0;
	}
	return 1;
}

int CompareStrayRoundAmmoTypes(PPOTYPE itm1, PPOTYPE itm2)
{
	if(itm1->rounds.mainammotype == itm2->rounds.mainammotype){
		return 0;
	}
	return 1;
}

int CompareBothStrayRoundAmmoTypes(PPOTYPE itm1, PPOTYPE itm2)
{
	if(itm1->rounds.mainammotype == itm2->rounds.mainammotype && itm1->rounds.subammotype == itm2->rounds.subammotype){
		return 0;
	}
	return 1;
}

int GetWeaponLoadedState(PWEAPON weapon)
{
	//CAN RETURN WEAPON_FULLY_LOADED, WEAPON_PARTIALLY_LOADED, WEAPON_UNLOADED, or WEAPON_NOTAMMO
	if(!weapon){
		return WEAPON_NOTAMMO;
	}
	switch(weapon->loadtype){
	case WEAP_LOADTYPE_NONE:
		return WEAPON_NOTAMMO;
	case WEAP_LOADTYPE_CLIP:
		if(!weapon->loadprocedure.clip.isclipinserted || weapon->loadprocedure.clip.clipinserted.currentrounds <= 0){
			return WEAPON_UNLOADED;
		}
		if(weapon->loadprocedure.clip.clipinserted.currentrounds == weapon->maxammo){
			return WEAPON_FULLY_LOADED;
		}
		return WEAPON_PARTIALLY_LOADED;
		break;
	case WEAP_LOADTYPE_MAGAZINE:
		if(weapon->loadprocedure.magazine.totalroundschambered <= 0){
			return WEAPON_UNLOADED;
		}
		if(weapon->loadprocedure.magazine.totalroundschambered == weapon->maxammo){
			return WEAPON_FULLY_LOADED;
		}
		return WEAPON_PARTIALLY_LOADED;
		break;
	case WEAP_LOADTYPE_CYLINDER:
		int ctr = 0;
		for(int i = 0; i < weapon->maxammo; i++){
			if(weapon->loadprocedure.cylinder.statusindicators[i] == CYLINDER_CHAMBER_FULL){
				ctr++;
			}
		}
		if(ctr == weapon->maxammo){
			return WEAPON_FULLY_LOADED;
		}
		if(ctr <= 0){
			return WEAPON_UNLOADED;
		}
		return WEAPON_PARTIALLY_LOADED;
		break;
	}
	return WEAPON_NOTAMMO;
}

int CountTotalRoundsInWeapon(PWEAPON Weapon)
{
		if(!Weapon){
		return 0;
	}
	switch(Weapon->loadtype){
	case WEAP_LOADTYPE_NONE:
		return 0;
	case WEAP_LOADTYPE_CLIP:
		if(!Weapon->loadprocedure.clip.isclipinserted){
			return 0;
		}
		return Weapon->loadprocedure.clip.clipinserted.currentrounds;
		break;
	case WEAP_LOADTYPE_MAGAZINE:
		return Weapon->loadprocedure.magazine.totalroundschambered;
		break;
	case WEAP_LOADTYPE_CYLINDER:
		int ctr = 0;
		for(int i = 0; i < Weapon->maxammo; i++){
			if(Weapon->loadprocedure.cylinder.statusindicators[i] == CYLINDER_CHAMBER_FULL){
				ctr++;
			}
		}
		return ctr;
		break;
	}
	return 0;
}

int GetClipLoadedState(PCLIP clip)
{
	POTYPE Finder;
	PPOTYPE Found;
	if(!clip){
		return CLIP_UNLOADED;
	}
	Finder.weapon.id = clip->weaponid;
	Found = findObject(globals.globalweaponsammo.weaponlist, &Finder, CompareWeaponID);
	if(!Found){
		return CLIP_UNLOADED;
	}
	if(clip->currentrounds == Found->weapon.maxammo){
		return CLIP_FULLY_LOADED;
	}
	if(clip->currentrounds > 0){
		return CLIP_PARTIALLY_LOADED;
	}
	return CLIP_UNLOADED;
}

PCLIP FindNextClipInInventory(PLIST cliplist, PCLIP currentclip, PWEAPON weapon, bool wraparound)
{
	PLIST walker = cliplist;
	if(cliplist->objcnt < 1){
		return NULL;
	}
	if(cliplist->objcnt < 2 && currentclip != NULL){
		return currentclip;
	}
	bool StartFound = false;
	if(currentclip == NULL){ //WE CAN FIND THE FIRST OCCURENCE.
		for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
		{
			if(walker->current->object.clip.weaponid == weapon->id){
				return &walker->current->object.clip;
			}
		}
		return NULL;
		
	}
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		if(&walker->current->object.clip == currentclip){
			StartFound = true;
			break; //WE'RE AT THE STARTING POINT.
			
		}
	}
	if(!StartFound){
		return NULL;
	}
	for(walker->current=walker->current->nextnode;walker->current != NULL; walker->current = walker->current->nextnode)
	{
		if(walker->current->object.clip.weaponid == currentclip->weaponid){
			return (&walker->current->object.clip);
		}
	}
	if(!wraparound){
		return currentclip;
	}
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode){
		if(walker->current->object.clip.weaponid == currentclip->weaponid){
			return (&walker->current->object.clip);
		}
	}
	return NULL;
}

PCLIP FindPreviousClipInInventory(PLIST cliplist, PCLIP currentclip, PWEAPON weapon, bool wraparound)
{
	PLIST walker = cliplist;
	PNODE tail;
	if(cliplist->objcnt < 1){
		return NULL;
	}
	if(cliplist->objcnt < 2 && currentclip != NULL){
		return currentclip;
	}
	bool StartFound = false;
	if(currentclip == NULL){ //WE CAN FIND THE FIRST OCCURENCE.
		for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
		{
			if(walker->current->object.clip.weaponid == weapon->id){
				return &walker->current->object.clip;
			}
		}
		return NULL;
		
	}
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		if(&walker->current->object.clip == currentclip){
			StartFound = true;
			break; //WE'RE AT THE STARTING POINT.
			
		}
	}
	if(!StartFound){
		return NULL;
	}
	for(walker->current=walker->current->prevnode;walker->current != NULL; walker->current = walker->current->prevnode)
	{
		if(walker->current->object.clip.weaponid == currentclip->weaponid){
			return (&walker->current->object.clip);
		}
	}
	if(!wraparound){
		return currentclip;
	}
	for(walker->current = walker->head; walker->current->nextnode != NULL; walker->current = walker->current->nextnode){
	}
	tail = walker->current;
	
	for(; walker->current != NULL; walker->current = walker->current->prevnode){
		if(walker->current->object.clip.weaponid == currentclip->weaponid){
			return (&walker->current->object.clip);
		}
	}
	return NULL;
}

void PlayerDropClip(PPLAYER player, PCLIP clip)
{
	float weighttoremove;
	if(!clip)
	{
		return;
	}
	POTYPE NewClip;
	PPOTYPE DeleteClip;
	NewClip.clip = *clip;
	NewClip.clip.WorldPoint.x = player->location->xcoord;
	NewClip.clip.WorldPoint.y = player->location->ycoord;
	DeleteClip = findObject(player->ammo.cliplist, &NewClip, CompareClipID);
	if(DeleteClip){
		deleteObject(player->ammo.cliplist, DeleteClip);
		if(NewClip.clip.currentrounds > 0){
		NewClip.clip.id = globals.globalmapstruct.clips->objcnt;
		addtoList(globals.globalmapstruct.clips, NewClip, CompareClipID);
	weighttoremove = (float)NewClip.clip.currentrounds * (float)ROUND_WEIGHT;
	player->weight -= weighttoremove;
	if(player->weight < 0.0){
		player->weight = 0.0;
	}
		}
	}

}

PSTRAYROUNDS FindNextStrayRoundsInInventory(PLIST straylist, PSTRAYROUNDS currentrounds, PWEAPON weapon, bool wraparound)
{
	PLIST walker = straylist;
	if(straylist->objcnt < 1){
		return NULL;
	}
	if(straylist->objcnt < 2 && currentrounds != NULL){
		return currentrounds;
	}
	bool StartFound = false;
	if(currentrounds == NULL){ //WE CAN FIND THE FIRST OCCURENCE.
		for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
		{
			if(walker->current->object.rounds.mainammotype == weapon->ammotype){
				return &walker->current->object.rounds;
			}
		}
		return NULL;
		
	}
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		if(&walker->current->object.rounds == currentrounds){
			StartFound = true;
			break; //WE'RE AT THE STARTING POINT.
			
		}
	}
	if(!StartFound){
		return NULL;
	}
	for(walker->current=walker->current->nextnode;walker->current != NULL; walker->current = walker->current->nextnode)
	{
		if(walker->current->object.rounds.mainammotype == weapon->ammotype){
			return (&walker->current->object.rounds);
		}
	}
	if(!wraparound){
		return currentrounds;
	}
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode){
		if(walker->current->object.rounds.mainammotype == currentrounds->mainammotype){
			return (&walker->current->object.rounds);
		}
	}
	return NULL;
}

PSTRAYROUNDS FindPreviousStrayRoundsInInventory(PLIST straylist, PSTRAYROUNDS currentrounds, PWEAPON weapon, bool wraparound)
{
	PLIST walker = straylist;
	PNODE tail;
	if(straylist->objcnt < 1){
		return NULL;
	}
	if(straylist->objcnt < 2 && currentrounds != NULL){
		return currentrounds;
	}
	bool StartFound = false;
	if(currentrounds == NULL){ //WE CAN FIND THE FIRST OCCURENCE.
		for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
		{
			if(walker->current->object.rounds.mainammotype == weapon->ammotype){
				return &walker->current->object.rounds;
			}
		}
		return NULL;
		
	}
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		if(&walker->current->object.rounds == currentrounds){
			StartFound = true;
			break; //WE'RE AT THE STARTING POINT.
			
		}
	}
	if(!StartFound){
		return NULL;
	}
	for(walker->current=walker->current->prevnode;walker->current != NULL; walker->current = walker->current->prevnode)
	{
		if(walker->current->object.rounds.mainammotype == currentrounds->mainammotype){
			return (&walker->current->object.rounds);
		}
	}
	if(!wraparound){
		return currentrounds;
	}
	for(walker->current = walker->head; walker->current->nextnode != NULL; walker->current = walker->current->nextnode){
	}
	tail = walker->current;
	
	for(; walker->current != NULL; walker->current = walker->current->prevnode){
		if(walker->current->object.rounds.mainammotype == currentrounds->mainammotype){
			return (&walker->current->object.rounds);
		}
	}
	return NULL;
}

bool PlayerDropStrayRounds(PPLAYER player, PSTRAYROUNDS rounds, int numtodrop)
{
	
	float weighttoremove;
	if(!rounds || numtodrop < 1)
		return false;
	bool all = (numtodrop >= rounds->numberofroundsleft);
	POTYPE NewRounds;
	PPOTYPE DeleteRounds;
	NewRounds.rounds = *rounds;
	if(!all){
		NewRounds.rounds.numberofroundsleft = numtodrop;
	}
	NewRounds.rounds.WorldPoint.x = player->location->xcoord;
	NewRounds.rounds.WorldPoint.y = player->location->ycoord;
	DeleteRounds = findObject(player->ammo.roundtypes, &NewRounds, CompareStrayRoundsID);
	if(DeleteRounds && all){
		deleteObject(player->ammo.roundtypes, DeleteRounds);
		if(NewRounds.rounds.numberofroundsleft > 0){
		NewRounds.rounds.id = globals.globalmapstruct.strayrounds->objcnt;
		addtoList(globals.globalmapstruct.strayrounds, NewRounds, CompareStrayRoundsID);
		}
	}
	weighttoremove = (float)NewRounds.rounds.numberofroundsleft * (float)ROUND_WEIGHT;
	player->weight -= weighttoremove;
	if(player->weight < 0.0){
		player->weight = 0.0;
	}
	if(!all){
		rounds->numberofroundsleft -= numtodrop;
	return false;
	}
	else
		return true;
}

ROUND GetRoundStructFromStrayRounds(PSTRAYROUNDS R)
{
	ROUND New;
	if(!R){
		New.mainammotype = -1;
		New.subammotype = -1;
		return New;
	}
	New.mainammotype = R->mainammotype;
	New.subammotype = R->subammotype;
	return New;
}

int EjectFromMagazine(PWEAPON weapon, PLIST strayroundlist)
{
	POTYPE tmpObj;
	PPOTYPE lstObj;
	if (weapon->loadtype != WEAP_LOADTYPE_MAGAZINE){
		return EJECTFROMMAGFAILED_WRONGLOADTYPE;
	}
	if (weapon->loadprocedure.magazine.totalroundschambered < 1){
		return EJECTFROMMAGFAILED_NONELOADED;
	}
	
	else{
		tmpObj.rounds.mainammotype = weapon->loadprocedure.magazine.chamberedrounds[weapon->loadprocedure.magazine.totalroundschambered - 1].mainammotype;
		tmpObj.rounds.subammotype = weapon->loadprocedure.magazine.chamberedrounds[weapon->loadprocedure.magazine.totalroundschambered - 1].subammotype;
		lstObj = findObject(strayroundlist, &tmpObj, CompareBothStrayRoundAmmoTypes);
		if(lstObj) //WE'VE MATCHED IT WITH THE SAME AMMO TYPE SOMEWHERE ELSE IN THE PLAYER'S AMMO LIST.
			lstObj->rounds.numberofroundsleft++;
		else{
		tmpObj.rounds.id = strayroundlist->objcnt; //PUT BACK INTO THE PLAYER LIST AS A NEW TYPE.
		tmpObj.rounds.numberofroundsleft = 1;
		addtoList(strayroundlist, tmpObj, CompareStrayRoundsID);
		}
		weapon->loadprocedure.magazine.totalroundschambered--;
		return EJECTFROMMAG_OK;
	}
}

bool FillMagazineWithRounds(PWEAPON weapon, PSTRAYROUNDS rounds, PLIST roundslist)
{
	int NumToFill = (weapon->maxammo - weapon->loadprocedure.magazine.totalroundschambered);
	int RetCode;
	if(NumToFill <= 0){
		SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, true);
		StaticDialogBox("Full Tube", "The magazine tube is full.", "Press ENTER", &globals.dboxparams);
	}
	if(rounds->numberofroundsleft < NumToFill){
		NumToFill = rounds->numberofroundsleft;
	}
	RetCode = LoadMagazine(weapon, roundslist, rounds->id, NumToFill);
	if(RetCode == LOADMAGAZINE_OK_REMOVED_NOROUNDS){
		return true;
	}
	return false;
}

void EmptyCylinderChamber(PWEAPON weapon, PLIST strayroundlist, int Chamber)
{
		POTYPE tmpObj;
	PPOTYPE lstObj;
	if (weapon->loadtype != WEAP_LOADTYPE_CYLINDER){
		return;
	}

	if(weapon->loadprocedure.cylinder.statusindicators[Chamber] == CYLINDER_CHAMBER_SHELLONLY){
		weapon->loadprocedure.cylinder.statusindicators[Chamber] = CYLINDER_CHAMBER_EMPTY;
		return;
	}
	
	if(weapon->loadprocedure.cylinder.statusindicators[Chamber] == CYLINDER_CHAMBER_EMPTY){
		return;
	}
	
		tmpObj.rounds.mainammotype = weapon->loadprocedure.cylinder.chamberedrounds[Chamber].mainammotype;
		tmpObj.rounds.subammotype = weapon->loadprocedure.cylinder.chamberedrounds[Chamber].subammotype;
		lstObj = findObject(strayroundlist, &tmpObj, CompareBothStrayRoundAmmoTypes);
		if(lstObj) //WE'VE MATCHED IT WITH THE SAME AMMO TYPE SOMEWHERE ELSE IN THE PLAYER'S AMMO LIST.
			lstObj->rounds.numberofroundsleft++;
		else{
		tmpObj.rounds.id = strayroundlist->objcnt; //PUT BACK INTO THE PLAYER LIST AS A NEW TYPE.
		tmpObj.rounds.numberofroundsleft = 1;
		addtoList(strayroundlist, tmpObj, CompareStrayRoundsID);
		}
		weapon->loadprocedure.cylinder.statusindicators[Chamber] = CYLINDER_CHAMBER_EMPTY;
		return;
}