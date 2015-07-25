#include "weaponsammo.h"


/***********************************************************
;					 Function Name: CompareAmmoID
;								 Synopsis: Compares two ammo POTYPES by their ID number.
;
;				Input Parameters: Two pointers to POTYPES
;	   		 Output Parameters: None
;									Return: 0 if they match ID's, 1 if not.
***********************************************************/

int CompareAmmoID(PPOTYPE itm1,PPOTYPE itm2) 
{
	if(itm1->ammo.id == itm2->ammo.id)
		return (0);
	return (1);
}

int CompareWeaponID(PPOTYPE itm1,PPOTYPE itm2) 
{
	if(itm1->weapon.id == itm2->weapon.id)
		return (0);
	return (1);
}


int CompareExplosiveSubAmmoID(PPOTYPE itm1,PPOTYPE itm2) 
{
if(itm1->explosive.id == itm2->explosive.id)
return (0);
return (1);
}

  int CompareBulletSubAmmoID(PPOTYPE itm1,PPOTYPE itm2) 
  {
  if(itm1->bullet.id == itm2->bullet.id)
		return (0);
		return (1);
		}
		
int CompareShellSubAmmoID(PPOTYPE itm1,PPOTYPE itm2) 
		  {
		  if(itm1->shell.id == itm2->shell.id)
		  return (0);
		  return (1);
 }






/***********************************************************
;					 Function Name: 
;								 Synopsis:
;
;				Input Parameters:
;	   		 Output Parameters:
;									Return:
***********************************************************/

PLIST createAmmoList(FILE* fp)
{
	char buf[MAX_LINESIZE];
	PLIST ammoList;
	//	POTYPE ammotype;
	BOOL endofammofile = FALSE;
	ammoList = createList();
	
	
	/************************************************************
	; Loop to get the start of the AMMO FILE
	************************************************************/
	do
	{
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
	}while (strcmp(buf, "<BEGINAMMOFILE>") && !feof(fp));
	
	
	/************************************************************
	; Start the highest level loop, loops until <ENDOFAMMOFILE> is reached.
	************************************************************/
	
	while (endofammofile == FALSE){
		do
		{
			fgets(buf, MAX_LINESIZE, fp);
			StripNewLine(buf);
		}while (strcmp(buf, "<BEGINMAINAMMO>") && (strcmp(buf, "<ENDAMMOFILE>")) && !feof(fp)); //LOOKING FOR MAIN AMMO OR END OF THE FILE
		
		if (!strcmp (buf, "<ENDAMMOFILE>")){
			endofammofile = TRUE;
			break;
		}
		
		else if (feof(fp)){
			printf("INCORRECTLY TERMINATED FILE - EXPECTED <BEGINMAINAMMO> OR <ENDMAINAMMO>");
			return NULL;
		}
		
		/************************************************************
		; createAmmo deals with each ammo type and sub-ammo type individually.
		************************************************************/
		else{
			createAmmo(ammoList, fp); //run createAmmo on the list.
		}
		
	}
	return ammoList;
}

/***********************************************************
;					 Function Name: 
;								 Synopsis:
;
;				Input Parameters:
;	   		 Output Parameters:
;									Return:
***********************************************************/

void createAmmo(PLIST ammoList, FILE* fp)
{
	char buf[MAX_LINESIZE];
	POTYPE tmpObj;

	memset(&tmpObj, 0, sizeof(POTYPE));
	//	int i;
	
	/************************************************************
	; Getting the ID number.
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	tmpObj.ammo.id = atoi(buf);
	
	
	/************************************************************
	; Finding Out What SubAmmo Type to Use By Identifier
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	tmpObj.ammo.type = atoi(buf);
	
	
	/************************************************************
	; Getting the Ammo Name
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	strncpy(tmpObj.ammo.name, buf, sizeof(tmpObj.ammo.name));
	
	
	/************************************************************
	; Getting Maximum Number of Subammo Types
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	tmpObj.ammo.maxammotypes = atoi(buf);
	
	
	/************************************************************
	; Begin MALLOC computations and sub-ammo creation...
	************************************************************/
	switch (tmpObj.ammo.type){
	case AMMOTYPE_SHOTGUNSHELL:
		tmpObj.ammo.specialammo.shelldata = (PSHELL) malloc(sizeof(SHELL) * tmpObj.ammo.maxammotypes);
		createShellAmmo(&tmpObj, fp);
		break;
	case AMMOTYPE_BULLET:
		tmpObj.ammo.specialammo.bulletdata = (PBULLET) malloc(sizeof(BULLET) * tmpObj.ammo.maxammotypes);
		createBulletAmmo(&tmpObj, fp);
		break;
	case AMMOTYPE_EXPLOSIVE:
		tmpObj.ammo.specialammo.explosivedata = (PEXPLOSIVE) malloc(sizeof(EXPLOSIVE) * tmpObj.ammo.maxammotypes);
		createExplosiveAmmo(&tmpObj, fp);
		break;
	default:
		printf("FILE ERROR - ammo.dat - %s has bad ammo type", tmpObj.ammo.name);
		return;
	}
	
	addtoList(ammoList, tmpObj, CompareAmmoID); //Add the object to the list
}

void createShellAmmo(PPOTYPE mainammo, FILE* fp)
{
	char buf[MAX_LINESIZE];
	char descbuf[MAX_DESC_SIZE];
	int i;
	
	
	/************************************************************
	; Loop for as many expected ammo types as were given
	************************************************************/
	
	for (i = 0; i < mainammo->ammo.maxammotypes; i++)
	{
		
	/************************************************************
	; Find a <BEGINSUBAMMO>
		************************************************************/
		
		do
		{
			fgets(buf, MAX_LINESIZE, fp);
			StripNewLine(buf);
		}while (strcmp(buf, "<BEGINSUBAMMO>") && !feof(fp)); //ONLY STOP IF YOU'VE FOUND A <BEGINSUBAMMO> or EOF
		
		if (feof(fp)){
			printf("INCORRECTLY TERMINATED FILE - EXPECTED <BEGINSUBAMMO>");
			exit (1);
		}
		
		
		/************************************************************
		; Get the ID
		************************************************************/
		
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.shelldata[i].id = atoi(buf);
		
		
		/************************************************************
		;  Get DSA / Not DSA Flag
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.shelldata[i].dsaammo = atoi(buf);
		
		
		/************************************************************
		; Get Short Name of Ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		strncpy(mainammo->ammo.specialammo.shelldata[i].shortname, buf, sizeof(mainammo->ammo.specialammo.shelldata->shortname)); //hmmm IT WORKS
		strcpy(buf, "");
		
		
		/************************************************************
		; Get LOOOOONG name of Ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		strncpy(mainammo->ammo.specialammo.shelldata[i].longname, buf, sizeof(mainammo->ammo.specialammo.shelldata->longname)); //hmmm IT WORKS
		strcpy(buf, "");
		
		
		/************************************************************
		; Get Description of Ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		strncpy(descbuf, buf, MAX_DESC_SIZE); //TRUNCATE TO 500 CHARACTERS
		mainammo->ammo.specialammo.shelldata[i].desc = (char*) malloc((strlen(descbuf) + 1) * sizeof (char)); //MALLOC FOR SMALLER SIZES
		strcpy(mainammo->ammo.specialammo.shelldata[i].desc, descbuf);
		strcpy(buf, "");
		
		/************************************************************
		; Get frequency of ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.shelldata[i].frequency = atoi(buf);
		
		/************************************************************
		; Get Spread of Ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.shelldata[i].spread = atoi(buf);
		
		/************************************************************
		; Get AP of ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.shelldata[i].ap = atoi(buf);
		
		
		/************************************************************
		; Get Energy XMIT of Ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.shelldata[i].energy = atoi(buf);
		
		/************************************************************
		; Get Damage of Ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.shelldata[i].damage = atoi(buf);
		
		
		/************************************************************
		; Get The String To The Sound
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		if (!strcmp(buf, "NULL")){
			mainammo->ammo.specialammo.shelldata[i].impactsound = NULL;
		}
		else{
			mainammo->ammo.specialammo.shelldata[i].impactsound = (char*) malloc(sizeof(char) * (strlen(buf)+1) );
			strcpy(mainammo->ammo.specialammo.shelldata[i].impactsound, buf);
		}
		strcpy(buf, "");
		/************************************************************
		; Find an <ENDSUBAMMO>
		************************************************************/
		
		do
		{
			fgets(buf, MAX_LINESIZE, fp);
			StripNewLine(buf);
		}while (strcmp(buf, "<ENDSUBAMMO>") && !feof(fp)); //ONLY STOP IF YOU'VE FOUND A <BEGINSUBAMMO> or EOF
		
		if (feof(fp)){
			printf("INCORRECTLY TERMINATED FILE - EXPECTED <ENDSUBAMMO>");
		return ;
		}
		
	}
}




void createBulletAmmo(PPOTYPE mainammo, FILE* fp)
{
	char buf[MAX_LINESIZE];
	char descbuf[MAX_DESC_SIZE];
	int i;
	
	for (i = 0; i < mainammo->ammo.maxammotypes; i++)
	{
		do
		{
			fgets(buf, MAX_LINESIZE, fp);
			StripNewLine(buf);
		}while (strcmp(buf, "<BEGINSUBAMMO>") && !feof(fp)); //ONLY STOP IF YOU'VE FOUND A <BEGINSUBAMMO> or EOF
		
		if (feof(fp)){
			printf("INCORRECTLY TERMINATED FILE - EXPECTED <BEGINSUBAMMO>");
		return;
		}
		
		
		/************************************************************
		; Getting the SUB AMMO ID
		************************************************************/
		
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.bulletdata[i].id = atoi(buf);
		
		/************************************************************
		;  Get DSA / Not DSA Flag
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.bulletdata[i].dsaammo = atoi(buf);
		
		
		/************************************************************
		; Get Short Name of Ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		strncpy(mainammo->ammo.specialammo.bulletdata[i].shortname, buf, sizeof(mainammo->ammo.specialammo.shelldata->shortname)); //hmmm IT WORKS
		strcpy(buf, "");
		/************************************************************
		; Get LOOOOONG name of Ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		strncpy(mainammo->ammo.specialammo.bulletdata[i].longname, buf, sizeof(mainammo->ammo.specialammo.shelldata->longname)); //hmmm IT WORKS
		strcpy(buf, "");
		
		/************************************************************
		; Get Description of Ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		strncpy(descbuf, buf, MAX_DESC_SIZE); //TRUNCATE TO 500 CHARACTERS
		mainammo->ammo.specialammo.bulletdata[i].desc = (char*) malloc((strlen(descbuf) + 1) * sizeof (char)); //MALLOC FOR SMALLER SIZES
		strcpy(mainammo->ammo.specialammo.bulletdata[i].desc, descbuf);
		strcpy(buf, "");
		
		/************************************************************
		; Get frequency of ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.bulletdata[i].frequency = atoi(buf);
		
		/************************************************************
		; Get AP of ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.bulletdata[i].ap = atoi(buf);
		
		
		/************************************************************
		; Get Energy XMIT of Ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.bulletdata[i].energy = atoi(buf);
		
		/************************************************************
		; Get Damage of Ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.bulletdata[i].damage = atoi(buf);
		
		
		/************************************************************
		; Get the Latent Damage Integer
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.bulletdata[i].latentdamage = atoi(buf);
		
		/************************************************************
		; Get The String To The Sound
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		if (!strcmp(buf, "NULL")){
			mainammo->ammo.specialammo.bulletdata[i].impactsound = NULL;
		}
		else{
			mainammo->ammo.specialammo.bulletdata[i].impactsound = (char*) malloc(sizeof(char) * (strlen(buf)+1) );
			strcpy(mainammo->ammo.specialammo.bulletdata[i].impactsound, buf);
		}
		strcpy(buf, "");
		/************************************************************
		; Find an <ENDSUBAMMO>
		************************************************************/
		
		do
		{
			fgets(buf, MAX_LINESIZE, fp);
			StripNewLine(buf);
		}while (strcmp(buf, "<ENDSUBAMMO>") && !feof(fp)); //ONLY STOP IF YOU'VE FOUND A <BEGINSUBAMMO> or EOF
		
		if (feof(fp)){
			printf("INCORRECTLY TERMINATED FILE - EXPECTED <ENDSUBAMMO>");
		return;
		}
		
	}
}


void createExplosiveAmmo(PPOTYPE mainammo, FILE* fp)
{
	char buf[MAX_LINESIZE] = "";
	char descbuf[MAX_DESC_SIZE] = "";
	int i;
	
	for (i = 0; i < mainammo->ammo.maxammotypes; i++)
	{
		do
		{
			fgets(buf, MAX_LINESIZE, fp);
			StripNewLine(buf);
		}while (strcmp(buf, "<BEGINSUBAMMO>") && !feof(fp)); //ONLY STOP IF YOU'VE FOUND A <BEGINSUBAMMO> or EOF
		
		if (feof(fp)){
			printf("INCORRECTLY TERMINATED FILE - EXPECTED <BEGINSUBAMMO>");
		return;
		}
		
		
		/************************************************************
		; Get the SUB AMMO ID
		************************************************************/
		
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.explosivedata[i].id = atoi(buf);
		
		/************************************************************
		;  Get DSA / Not DSA Flag
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.explosivedata[i].dsaammo = atoi(buf);
		
		
		/************************************************************
		; Get Short Name of Ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		strncpy(mainammo->ammo.specialammo.explosivedata[i].shortname, buf, sizeof(mainammo->ammo.specialammo.shelldata->shortname)); //hmmm IT WORKS
		strcpy(buf, "");
		
		/************************************************************
		; Get LOOOOONG name of Ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		strncpy(mainammo->ammo.specialammo.explosivedata[i].longname, buf, sizeof(mainammo->ammo.specialammo.shelldata->longname)); //hmmm IT WORKS
		strcpy(buf, "");
		
		/************************************************************
		; Get Description of Ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		strncpy(descbuf, buf, MAX_DESC_SIZE); //TRUNCATE TO 500 CHARACTERS
		mainammo->ammo.specialammo.explosivedata[i].desc = (char*) malloc((strlen(descbuf) + 1) * sizeof (char)); //MALLOC FOR SMALLER SIZES
		strcpy(mainammo->ammo.specialammo.explosivedata[i].desc, descbuf);
		strcpy(buf, "");
		
		/************************************************************
		; Get frequency of ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.explosivedata[i].frequency = atoi(buf);
		
		/************************************************************
		; Get delay of ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.explosivedata[i].delay = atoi(buf);
		
		/************************************************************
		; Get range of ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.explosivedata[i].range = atoi(buf);
		
		/************************************************************
		; Get inital blast radius of ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.explosivedata[i].initialblastradius = atoi(buf);
		
		
		/************************************************************
		; Get initial damage type of ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.explosivedata[i].initialdamagetype = atoi(buf);
		
		/************************************************************
		; Get initial damage strength of ammo
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.explosivedata[i].initialdamagestrength = atoi(buf);
		
		/************************************************************
		; Get lasting damage type
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.explosivedata[i].lastingdamagetype = atoi(buf);
		
		/************************************************************
		; Get lasting damage radius
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.explosivedata[i].lastingdamageradius = atoi(buf);
		
		/************************************************************
		; Get lasting damage time
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		mainammo->ammo.specialammo.explosivedata[i].lastingdamagetime = atoi(buf);
		
		/************************************************************
		; Get The String To The Sound
		************************************************************/
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
		if (!strcmp(buf, "NULL")){
			mainammo->ammo.specialammo.explosivedata[i].impactsound = NULL;
		}
		else{
			mainammo->ammo.specialammo.explosivedata[i].impactsound =(char*) malloc(sizeof(char) * (strlen(buf)+1) );
			strcpy(mainammo->ammo.specialammo.explosivedata[i].impactsound, buf);
		}
		strcpy(buf, "");
		/************************************************************
		; Find an <ENDSUBAMMO>
		************************************************************/
		
		do
		{
			fgets(buf, MAX_LINESIZE, fp);
			StripNewLine(buf);
		}while (strcmp(buf, "<ENDSUBAMMO>") && !feof(fp)); //ONLY STOP IF YOU'VE FOUND A <BEGINSUBAMMO> or EOF
		
		if (feof(fp)){
			printf("INCORRECTLY TERMINATED FILE - EXPECTED <ENDSUBAMMO>");
		return;
		}
		
	}
}


/***********************************************************
;					 Function Name: StripNewLine
;								 Synopsis: replaces the first newline character in a string with a null terminator
;
;				Input Parameters: a char* representing the string to be modified
;	   		 Output Parameters: None
;									Return: the modified string.
***********************************************************/

char* StripNewLine(char* buf)
{
	if(buf[strlen(buf)-1] == '\n') {// Replace new line character with null character
		buf[strlen(buf)-1] = '\0'; 
	}
	return buf;
}

PWEAPON GetWeaponByID(PLIST weaponlist, int id)
{
	POTYPE tmpObj;
	PPOTYPE lstObj;

	tmpObj.weapon.id = id;
	lstObj = findObject(weaponlist, &tmpObj, CompareWeaponID);
	return &lstObj->weapon;
}

PAMMO GetMainAmmoByID(PLIST ammolist, int id)
{
	POTYPE tmpObj;
	PPOTYPE lstObj;
	
	tmpObj.ammo.id = id;
	lstObj = findObject(ammolist, &tmpObj, CompareAmmoID);
	return &lstObj->ammo;
}

PEXPLOSIVE GetExplosiveSubAmmoByID(PAMMO mainammo, int id)
{
	for (int i = 0; i < mainammo->maxammotypes; i++){
		if (id = mainammo->specialammo.explosivedata[i].id){
			return &mainammo->specialammo.explosivedata[i];
		}
	}
	return NULL;
}

PBULLET GetBulletSubAmmoByID(PAMMO mainammo, int id)
{
	for (int i = 0; i < mainammo->maxammotypes; i++){
		if (id = mainammo->specialammo.bulletdata[i].id){
			return &mainammo->specialammo.bulletdata[i];
		}
	}
	return NULL;
}

PSHELL GetShellSubAmmoByID(PAMMO mainammo, int id)
{
	for (int i = 0; i < mainammo->maxammotypes; i++){
		if (id = mainammo->specialammo.shelldata[i].id){
			return &mainammo->specialammo.shelldata[i];
		}
	}
	return NULL;
}
/*
void sampleload()
{
	FILE* fp;
	int key;
	
	initconsole();
	fp = fopen("ammo.dat", "r");
	if (fp == NULL)
	{
		printf("ammo.dat could not be opened.");
		waitforkey();
		exit(1);
	}
	PLIST ammolist = createAmmoList(fp);
	fclose(fp);
	fp = fopen("weapons.dat", "r");
	if (fp == NULL)
	{
		printf("weapons.dat could not be opened!\n");
		printf("<PRESS A KEY>\n");
		waitforkey();
		exit(1);
	}
	PLIST weaponlist = createWeaponList(fp, ammolist); //Create WEAPONLIST needs an ammolist to function.
	fclose(fp);
	printAllAmmo(ammolist);
	key = waitforkey();
	cls();
	zerocursor();
	printAllWeapons(weaponlist, ammolist);
	printf("<PRESS A KEY>\n");
	key = waitforkey();
	FreeAmmoList(ammolist);
	FreeWeaponList(weaponlist);
	
}
*/
PLIST createWeaponList(FILE* fp, PLIST ammolist)
{
	char buf[MAX_LINESIZE];
	PLIST weaponList;
	BOOL endofweaponfile = FALSE;
	weaponList = createList();
	
	
	/************************************************************
	; Loop to get the start of the WEAPON FILE
	************************************************************/
	do
	{
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
	}while (strcmp(buf, "<BEGINWEAPONFILE>") && !feof(fp));
	
	
	/************************************************************
	; Start the highest level loop, loops until <ENDOFWEAPONFILE> is reached.
	************************************************************/
	
	while (endofweaponfile == FALSE){
		do
		{
			fgets(buf, MAX_LINESIZE, fp);
			StripNewLine(buf);
		}while (strcmp(buf, "<BEGINWEAPON>") && (strcmp(buf, "<ENDWEAPON>")) && !feof(fp)); //LOOKING FOR BEGINWEAPON OR END OF THE FILE
		
		if (!strcmp (buf, "<ENDWEAPONFILE>")){
			endofweaponfile = TRUE;
			break;
		}
		
		else if (feof(fp)){
			printf("INCORRECTLY TERMINATED FILE - EXPECTED <BEGINWEAPON> OR <ENDWEAPON>");
	return NULL;
		}
		
		/************************************************************
		; createWeapon deals with each weapon individually.
		************************************************************/
		else{
			createWeapon(weaponList, ammolist, fp); //run createWeapon on the list.
		}
		
	}
	return weaponList;
}


void createWeapon(PLIST weaponList, PLIST ammoList, FILE* fp)
{
	char descbuf[MAX_DESC_SIZE] = "";
	char buf[MAX_LINESIZE] = "";
	char* token;
	POTYPE tmpObj;
	int k;

//	memset(&tmpObj, 0, sizeof(POTYPE));
	/************************************************************
	; Get the ID tag
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	tmpObj.weapon.id = atoi(buf);
	
	/************************************************************
	; Get the DSA Boolean
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	tmpObj.weapon.dsaweapon = atoi(buf);
	
	
	/************************************************************
	; Get the Attack Type
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	tmpObj.weapon.attacktype = atoi(buf);
	if (tmpObj.weapon.attacktype > MAX_ATTACK_VALUE){
	
		printf("Weapon id #%d has bad attack type value!\n", tmpObj.weapon.id);
	return;
	}
	
	
	/************************************************************
	; Get Short Name of Ammo
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	strncpy(tmpObj.weapon.shortname, buf, sizeof(tmpObj.weapon.shortname)); //hmmm IT WORKS
	strcpy(buf, "");
	
	/************************************************************
	; Get LOOOOONG name of Ammo
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	strncpy(tmpObj.weapon.longname, buf, sizeof(tmpObj.weapon.longname)); //hmmm IT WORKS
	strcpy(buf, "");
	
	
	/************************************************************
	; Get description
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	if (strcmp(buf, "#")){
	
		printf("EXPECTED # FOR WEAPON %s\n", tmpObj.weapon.shortname);
	return;
	}
	do
	{
		fgets(buf, MAX_LINESIZE, fp);
		if (!strcmp(buf, "#\n")){
			break;
		}
		strcat(descbuf, buf);
	}while (strcmp(buf, "#\n"));
	//CHANGE THE LAST CHARACTER TO A \0
	descbuf[strlen(descbuf) - 1] = '\0';
	
	tmpObj.weapon.desc = (char*) malloc(sizeof(char) * strlen(descbuf) + 1);
	strcpy(tmpObj.weapon.desc, descbuf);
	//	printf("%s", tmpObj.weapon.desc);
	
	
	/************************************************************
	; Get filing type
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	tmpObj.weapon.filingtype = atoi(buf);
	
	
	/************************************************************
	; Get class type
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	tmpObj.weapon.classtype = atoi(buf);
	
	
	/************************************************************
	; Get frequency
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	tmpObj.weapon.frequency = atoi(buf);
	
	
	/************************************************************
	; Get main ammo ID
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	tmpObj.weapon.ammotype = atoi(buf);
	
	if(!GetMainAmmoByID(ammoList, tmpObj.weapon.ammotype) && tmpObj.weapon.ammotype > 0) {//IF WE CAN'T FIND THAT AMMO TYPE IN THE LIST, SET TO 0.
		tmpObj.weapon.ammotype = 0;
	}
	
	
	/************************************************************
	; Get Max ammo
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	tmpObj.weapon.maxammo = atoi(buf);
	
	
	/************************************************************
	; Get ammo load type
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	tmpObj.weapon.loadtype = atoi(buf);
	
	
	/************************************************************
	; SET DEFAULT WEAPON CONDITION TO 100%
	************************************************************/
	tmpObj.weapon.condition_percent = 100;
	
	
	/************************************************************
	;MOVING ON TO THE WEAPPERFORMANCE
	Set base accuracy %
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	tmpObj.weapon.weapperf.baseaccuracy_percent = atoi(buf);
	
	
	/************************************************************
	; Rounds per Turn
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	tmpObj.weapon.weapperf.roundsperturn = atoi(buf);
	
	
	/************************************************************
	; Flash Level
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	tmpObj.weapon.weapperf.flashlevel = atoi(buf);
	
	
	/************************************************************
	; Sound Level
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	tmpObj.weapon.weapperf.soundlevel = atoi(buf);
	
	
	/************************************************************
	; Drug Type
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	tmpObj.weapon.drug.type = atoi(buf);
	
	
	/************************************************************
	; Drug Doses
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	tmpObj.weapon.drug.doses = atoi(buf);
	
	
	/************************************************************
	; Set dose to TRUE IF DOSE EXITS
	************************************************************/
	if (tmpObj.weapon.drug.doses > 0){
		tmpObj.weapon.drug.doseloaded = TRUE;
		tmpObj.weapon.drug.doses --; //DROP THE REMAINING DOSES
	}
	
	
	/************************************************************
	; Set the weight
	************************************************************/
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	tmpObj.weapon.weight = (float) atof(buf);
	
	/************************************************************
	;	Set the modifier table 
	************************************************************/
	
	fgets(buf, MAX_LINESIZE, fp);
	strtok(buf, "\t");
	if (buf != NULL){
		
		tmpObj.weapon.modifiersallowed.lasersight = atoi(buf);
		
		
		token = strtok(NULL, "\t");
		tmpObj.weapon.modifiersallowed.flashsuppressor = atoi(token);
		
		token = strtok(NULL, "\t");
		tmpObj.weapon.modifiersallowed.silencer = atoi(token);
	}
	
	memset(&tmpObj.weapon.modifiersattached, 0, sizeof (MODIFIER));
	
	/************************************************************
	; Get The Strings To The Sounds
	************************************************************/
	for (k = 0; k < MAX_WEAPON_SOUNDS; k++)
	{
	fgets(buf, MAX_LINESIZE, fp);
	StripNewLine(buf);
	if (!strcmp(buf, "NULL")){
		tmpObj.weapon.soundfilename[k] = NULL;
	}
	else{
		tmpObj.weapon.soundfilename[k] = (char*) malloc(sizeof(char) *strlen(buf) + 1 );
		strcpy(tmpObj.weapon.soundfilename[k], buf);
	}
	}
	strcpy(buf, "");
	



	/************************************************************
	; Set the index to zero
	************************************************************/
	tmpObj.weapon.index = 0;

	/************************************************************
	; Get the ENDWEAPON
	************************************************************/

	do
	{
		fgets(buf, MAX_LINESIZE, fp);
		StripNewLine(buf);
	}while (strcmp(buf, "<ENDWEAPON>") && !feof(fp)); //ONLY STOP IF YOU'VE FOUND A <ENDWEAPON> or EOF
	
	if (feof(fp)){
		printf("INCORRECTLY TERMINATED FILE - EXPECTED <ENDWEAPON>");
		printf("<PRESS A KEY>");
	return;
	}
	
	addtoList(weaponList, tmpObj, CompareWeaponID);
}

void FreeWeaponList(PLIST weaponlist)
{
	PLIST walker;
	walker = weaponlist;
	int i;
	
	
	for(walker->current = weaponlist->head; walker->current != NULL; walker->current = walker->current->nextnode){ //WALK THE LIST
		
	free(walker->current->object.weapon.desc); //FREE THE DESCRIPTION CHAR*
		for (i = 0; i < MAX_WEAPON_SOUNDS; i++){
			if (walker->current->object.weapon.soundfilename[i]){
									free(walker->current->object.weapon.soundfilename[i]);
			}
		}
		
	}
	freeList(weaponlist);
}

void FreeAmmoList(PLIST ammolist)
{
	PLIST walker;
	walker = ammolist;
	int i;
	
	
	for(walker->current = ammolist->head; walker->current != NULL; walker->current = walker->current->nextnode){ //WALK THE LIST
		
		switch (walker->current->object.ammo.type){
		case AMMOTYPE_SHOTGUNSHELL:
			for (i = 0; i < walker->current->object.ammo.maxammotypes; i++){
				if (walker->current->object.ammo.specialammo.shelldata[i].desc){
					free(walker->current->object.ammo.specialammo.shelldata[i].desc);
				}
				if (walker->current->object.ammo.specialammo.shelldata[i].impactsound){
					free(walker->current->object.ammo.specialammo.shelldata[i].impactsound);
				}
			}
			free(walker->current->object.ammo.specialammo.shelldata);
			break;
		case AMMOTYPE_BULLET:
			for (i = 0; i < walker->current->object.ammo.maxammotypes; i++){
				if (walker->current->object.ammo.specialammo.bulletdata[i].desc){
					free(walker->current->object.ammo.specialammo.bulletdata[i].desc);
				}
				if (walker->current->object.ammo.specialammo.bulletdata[i].impactsound){
					free(walker->current->object.ammo.specialammo.bulletdata[i].impactsound);
				}
			}
			free(walker->current->object.ammo.specialammo.bulletdata);		
			break;
		case AMMOTYPE_EXPLOSIVE:
			for (i = 0; i < walker->current->object.ammo.maxammotypes; i++){
				if (walker->current->object.ammo.specialammo.explosivedata[i].desc){
					free(walker->current->object.ammo.specialammo.explosivedata[i].desc);
				}
				if (walker->current->object.ammo.specialammo.explosivedata[i].impactsound){
					free(walker->current->object.ammo.specialammo.explosivedata[i].impactsound);
				}
			}
			free(walker->current->object.ammo.specialammo.explosivedata);
			break;
			
		}
		
		
	
	}
	freeList(ammolist);
}

char *getWeaponClassString(WEAPON weapon)
{
	switch (weapon.classtype){
	case  WEAP_CLASS_A:
		return WEAP_CLASS_A_S;
	case WEAP_CLASS_B:
		return WEAP_CLASS_B_S;
	case WEAP_CLASS_S:
		return WEAP_CLASS_S_S;
	case WEAP_CLASS_X:
		return WEAP_CLASS_X_S;
	default:
		return "UNKNOWN CLASS";
	}
}

char* getWeaponFilingString(WEAPON weapon)
{
	switch(weapon.filingtype){
	default:

	case WEAP_FILING_TYPE_MELEE:
		return WEAP_FILING_TYPE_MELEE_S;
	case WEAP_FILING_TYPE_PISTOLS:
		return WEAP_FILING_TYPE_PISTOLS_S;
	case WEAP_FILING_TYPE_SMGS:
		return WEAP_FILING_TYPE_SMGS_S;
	case WEAP_FILING_TYPE_RIFLES:
		return WEAP_FILING_TYPE_RIFLES_S;
	case WEAP_FILING_TYPE_HEAVY:
		return WEAP_FILING_TYPE_HEAVY_S;
	case WEAP_FILING_TYPE_SPECIAL:
		return WEAP_FILING_TYPE_SPECIAL_S;
	case WEAP_FILING_TYPE_SHOTGUN:
return WEAP_FILING_TYPE_SHOTGUN_S;
	}
}

BOOL WeaponSilencerOn(PWEAPON weapon)
{
	return weapon->modifiersactivated.silencer;
}

BOOL WeaponLasersightOn(PWEAPON weapon)
{
	return weapon->modifiersactivated.lasersight;
}

BOOL WeaponFlashsuppressorOn(PWEAPON weapon)
{
	return weapon->modifiersactivated.flashsuppressor;
}
BOOL WeaponSilencerAllowed(PWEAPON weapon)
{
	return weapon->modifiersallowed.silencer;
}
BOOL WeaponLasersightAllowed(PWEAPON weapon)
{
	return weapon->modifiersallowed.lasersight;
}
BOOL WeaponFlashsuppressorAllowed(PWEAPON weapon)
{
	return weapon->modifiersallowed.flashsuppressor;
}
BOOL WeaponSilencerHeld(PWEAPON weapon)
{
	return weapon->modifiersattached.silencer;
}
BOOL WeaponLasersightHeld(PWEAPON weapon)
{
	return weapon->modifiersattached.lasersight;
}
BOOL WeaponFlashsuppressorHeld(PWEAPON weapon)
{
	return weapon->modifiersattached.flashsuppressor;
}

int CompareWeaponIndex(PPOTYPE itm1,PPOTYPE itm2) 
{
	if(itm1->weapon.index == itm2->weapon.index)
		return (0);
	return (1);
}
//--------------------------------------------------------------------------------------------
// Function Name - CompareClipID
//
// Description   - List object comparison by clip ID
//
// Return Type   - int 
//
// Arguments     - PPOTYPE itm1 ( Item 1 to Compare )
//               - PPOTYPE itm2 ( Item 2 To Compare )
//
// Author        - Charles Cox
// Date Modified - 04/26/2000
//--------------------------------------------------------------------------------------------
int CompareClipID(PPOTYPE itm1,PPOTYPE itm2) 
{
	if(itm1->clip.id == itm2->clip.id)
		return (0);
	return (1);
}

int CompareMainandSubAmmoID(PPOTYPE itm1, PPOTYPE itm2)
{
	if(itm1->rounds.mainammotype == itm2->rounds.mainammotype && itm1->rounds.subammotype == itm2->rounds.subammotype)
		return (0);
	return (1);
}


//--------------------------------------------------------------------------------------------
// Function Name - CompareStrayRoundsID
//
// Description   - List object comparison by round ID
//
// Return Type   - int 
//
// Arguments     - PPOTYPE itm1 ( Item 1 To Compare )
//               - PPOTYPE itm2 ( Item 2 To Compare )
//
// Author        - Charles Cox
// Date Modified - 04/26/2000
//--------------------------------------------------------------------------------------------
int CompareStrayRoundsID(PPOTYPE itm1,PPOTYPE itm2) 
{
	if(itm1->rounds.id == itm2->rounds.id)
		return (0);
	return (1);
}

int CompareStrayRoundsAmmo(PPOTYPE itm1,PPOTYPE itm2) 
{
	if(itm1->rounds.mainammotype == itm2->rounds.mainammotype && itm1->rounds.subammotype == itm2->rounds.subammotype)
		return (0);
	return (1);
}