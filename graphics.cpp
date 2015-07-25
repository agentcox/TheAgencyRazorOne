#include "agencyinclude.h"

int CompareGraphicID(PPOTYPE itm1,PPOTYPE itm2) 
{
	if(itm1->graphic.id == itm2->graphic.id)
		return (0);
	return (1);
}

PLIST loadgamegraphics()
{
	POTYPE tmpObj;
	PLIST glist;
	FILE* fp;
	
	//CREATION OF THE LIST.
	glist = createList();
	
	
	/************************************************************
	; CIS BLOCK
	************************************************************/
	fp = fopen(CIS_GRAPHIC_FILE, "r");
	if (fp == NULL){
		return NULL;
	}
	loadgraphic(fp, &tmpObj.graphic);
	fclose(fp);
	tmpObj.graphic.id = CIS_GRAPHIC_ID;
	addtoList(glist, tmpObj, CompareGraphicID);
	
	
	/************************************************************
	; SNIPER SCOPE BLOCK
	************************************************************/
	fp = fopen(SNIPE_GRAPHIC_FILE, "r");
	if (fp == NULL){
		return NULL;
	}
	loadgraphic(fp, &tmpObj.graphic);
	fclose(fp);
	tmpObj.graphic.id = SNIPE_GRAPHIC_ID;
	addtoList(glist, tmpObj, CompareGraphicID);
	
	
	/************************************************************
	; CODESCREEN BLOCK
	************************************************************/
	fp = fopen(CODESCREEN_GRAPHIC_FILE, "r");
	if (fp == NULL){
		return NULL;
	}
	loadgraphic(fp, &tmpObj.graphic);
	fclose(fp);
	tmpObj.graphic.id = CODESCREEN_GRAPHIC_ID;
	addtoList(glist, tmpObj, CompareGraphicID);

		/************************************************************
	; ATOMIC SCREEN BLOCK
	************************************************************/
	fp = fopen(ATOMICSCREEN_GRAPHIC_FILE, "r");
	if (fp == NULL){
		return NULL;
	}
	loadgraphic(fp, &tmpObj.graphic);
	fclose(fp);
	tmpObj.graphic.id = ATOMICSCREEN_GRAPHIC_ID;
	addtoList(glist, tmpObj, CompareGraphicID);


	/************************************************************
	; PROMPT SCREEN BLOCK
	************************************************************/
	fp = fopen(PROMPT_GRAPHIC_FILE, "r");
	if (fp == NULL){
		return NULL;
	}
	loadgraphic(fp, &tmpObj.graphic);
	fclose(fp);
	tmpObj.graphic.id = PROMPT_GRAPHIC_ID;
	addtoList(glist, tmpObj, CompareGraphicID);

	
	/************************************************************
	; PROMPT RESPONSE  BOX BLOCK
	************************************************************/
		fp = fopen(PROMPTRESPONSE_GRAPHIC_FILE, "r");
	if (fp == NULL){
		return NULL;
	}
	loadgraphic(fp, &tmpObj.graphic);
	fclose(fp);
	tmpObj.graphic.id = PROMPTRESPONSE_GRAPHIC_ID;
	addtoList(glist, tmpObj, CompareGraphicID);


	/************************************************************
	; MELEE COMBAT BOX BLOCK
	************************************************************/
	fp = fopen(MELEE_GRAPHIC_FILE, "r");
	if (fp == NULL){
		return NULL;
	}
	loadgraphic(fp, &tmpObj.graphic);
	fclose(fp);
	tmpObj.graphic.id = MELEE_GRAPHIC_ID;
	addtoList(glist, tmpObj, CompareGraphicID);

	/************************************************************
	; SARTU BOX BLOCK
	************************************************************/
	fp = fopen(SARTU_GRAPHIC_FILE, "r");
	if (fp == NULL){
		return NULL;
	}
	loadgraphic(fp, &tmpObj.graphic);
	fclose(fp);
	tmpObj.graphic.id = SARTU_GRAPHIC_ID;
	addtoList(glist, tmpObj, CompareGraphicID);

	/************************************************************
	; ARMORY BOX BLOCK
	************************************************************/
	fp = fopen(ARMORY_GRAPHIC_FILE, "r");
	if (fp == NULL){
		return NULL;
	}
	loadgraphic(fp, &tmpObj.graphic);
	fclose(fp);
	tmpObj.graphic.id = ARMORY_GRAPHIC_ID;
	addtoList(glist, tmpObj, CompareGraphicID);

	/************************************************************
	; MAINUI BOX BLOCK
	************************************************************/
	fp = fopen(MAINUI_GRAPHIC_FILE, "r");
	if (fp == NULL){
		return NULL;
	}
	loadgraphic(fp, &tmpObj.graphic);
	fclose(fp);
	tmpObj.graphic.id = MAINUI_GRAPHIC_ID;
	addtoList(glist, tmpObj, CompareGraphicID);

	/************************************************************
	; RADIO BOX BLOCK
	************************************************************/
	fp = fopen(RADIO_GRAPHIC_FILE, "r");
	if (fp == NULL){
		return NULL;
	}
	loadgraphic(fp, &tmpObj.graphic);
	fclose(fp);
	tmpObj.graphic.id = RADIO_GRAPHIC_ID;
	addtoList(glist, tmpObj, CompareGraphicID);

	/************************************************************
	; SILHOUETTE BLOCK
	************************************************************/
	fp = fopen(SHOOTING_SILHOUETTE_CLEAR_FILE, "r");
	if (fp == NULL){
		return NULL;
	}
	loadgraphic(fp, &tmpObj.graphic);
	fclose(fp);
	tmpObj.graphic.id = SHOOTING_SILHOUETTE_CLEAR_ID;
	addtoList(glist, tmpObj, CompareGraphicID);

	fp = fopen(SHOOTING_SILHOUETTE_FULLCOVER_FILE, "r");
	if (fp == NULL){
		return NULL;
	}
	loadgraphic(fp, &tmpObj.graphic);
	fclose(fp);
	tmpObj.graphic.id = SHOOTING_SILHOUETTE_FULLCOVER_ID;
	addtoList(glist, tmpObj, CompareGraphicID);

	fp = fopen(SHOOTING_SILHOUETTE_PARTIALCOVER_FILE, "r");
	if (fp == NULL){
		return NULL;
	}
	loadgraphic(fp, &tmpObj.graphic);
	fclose(fp);
	tmpObj.graphic.id = SHOOTING_SILHOUETTE_PARTIALCOVER_ID;
	addtoList(glist, tmpObj, CompareGraphicID);

	
	return glist;
	
	
}

void loadgraphic(FILE* fp, PGRAPHIC graphic)
{
	int charcounter = 0;
	int cols = 0;
	int rows = 0;
	char junk; //TO HOLD CHARACTERS TO GET SIZE.
	char holder;
	BOOL EOFencountered = FALSE;
	

	/************************************************************
	; Get File Length
	************************************************************/

	while(!feof(fp))
	{
		fscanf(fp, "%c", &junk);
		charcounter++;
	}
	graphic->graphic = (char*) malloc(sizeof(char) * charcounter); //SINCE WE HIT AN EOF, THAT SHOULD COVER THE +1 NULL TERMINATOR ON THE STRING.
	memset(graphic->graphic, 0, sizeof(char) * charcounter);
	graphic->length = charcounter;

	charcounter = 0;
	rewind(fp);

	/************************************************************
	; Get the actual data, determine the size of the object.
	************************************************************/

	for (rows = 0; !feof(fp) && EOFencountered == FALSE; rows++)
	{
		cols = 0;
		for(;;)
		{
			fscanf(fp, "%c", &holder);
			if (holder == '\n' || feof(fp)){
				graphic->size.X = cols;
				
				if (feof(fp)){
					EOFencountered = TRUE;
				}
				break;
			}
			graphic->graphic[charcounter] = holder;
			charcounter++;
			cols++;
		}
	}
	graphic->graphic[charcounter] = '\0';
	graphic->size.Y = rows;
	
}

void freegamegraphics(PLIST glist)
{
	PLIST walker;
	walker = glist;
//	int i;
	
	walker->current = glist->head;
	while(walker->current != NULL){ //WALK THE LIST
		
		//	free(walker->current->object.weapon.desc); //FREE THE DESCRIPTION CHAR*
		if (walker->current->object.graphic.graphic){
			free(walker->current->object.graphic.graphic);
		}
		walker->current = walker->current->nextnode;
	}
	freeList(glist);
	free(glist);
}

void printgraphic(PLIST graphicslist, COORD drawcoord, int graphicid)
{
	POTYPE tmpObj;
	PPOTYPE lstObj;
	COORD localcoords;
	int cols;
	int rows;
	int charcounter = 0;

	tmpObj.graphic.id = graphicid;
	lstObj = findObject(graphicslist, &tmpObj, CompareGraphicID);
	if (lstObj == NULL){
		return;
	}
	localcoords = drawcoord;
	setcursor(drawcoord.X, drawcoord.Y);
	
	/************************************************************
	; We "MITE" print to a backbuffer and swap in, here.
	************************************************************/

	for (rows = 0; rows < lstObj->graphic.size.Y; rows++)
	{
		for (cols = 0; cols < lstObj->graphic.size.X; cols++)
		{
		printf("%c", lstObj->graphic.graphic[charcounter]);
		charcounter++;
		}
		localcoords.Y++;
		setcursor(localcoords.X, localcoords.Y);
	}

}

COORD getgraphicdimensions(PLIST graphicslist, int graphicid)
{
		POTYPE tmpObj;
	PPOTYPE lstObj;
	COORD zerocoord = {0,0};

	tmpObj.graphic.id = graphicid;
	lstObj = findObject(graphicslist, &tmpObj, CompareGraphicID);
	if (lstObj == NULL){
		return zerocoord;
	}
	return lstObj->graphic.size;
}