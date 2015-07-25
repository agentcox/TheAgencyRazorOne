#include "agencyinclude.h"
#include "prefabs.h"
#include "maplib.h"


//STANDARD OPERATIONS
bool InitializePrefab(PPREFAB prefab, int width, int height)
{
	if (height < 1 || width < 1){
		return false;
	}
	prefab->width = width;
	prefab->height = height;
	prefab->prefabdata = (int**)malloc(sizeof(int*) * height);
	for(int i = 0; i < height; i++){
		prefab->prefabdata[i] = (int*)malloc(sizeof(int) * width);
		for(int k = 0; k < width; k++){
			prefab->prefabdata[i][k] = OBJECT_SYM_GROUND; //FILL WITH GROUND
		}
	}
	return true;
}

bool FillPrefab(PPREFAB prefab, int** tiles, int width, int height)
{
	if (height > prefab->height || width > prefab->width){
		return false;
	}
	for(int i = 0; i < height; i++){
		for(int k = 0; k < width; k++){
			prefab->prefabdata[i][k] = tiles[i][k];
		}
	}
	return true;
}

void TerminatePrefab(PPREFAB prefab)
{
	for(int i = 0; i < prefab->height; i++){
		free(prefab->prefabdata[i]);
	}
	free(prefab->prefabdata);
	prefab->prefabdata = NULL;
}
//TILEWISE OPERATIONS
void RotatePrefabTiles(PPREFAB prefab)
{
	int* currenttile;
	for(int h = 0; h < prefab->height; h++){
		for(int w = 0; w < prefab->width; w++){
			currenttile = &prefab->prefabdata[h][w];
			switch(*currenttile){
			case BUILDING_SYM_BLCORNER:
				*currenttile = BUILDING_SYM_TLCORNER;
				break;
			case BUILDING_SYM_TRCORNER:
				*currenttile = BUILDING_SYM_BRCORNER;
				break;
			case BUILDING_SYM_BRCORNER:
				*currenttile = BUILDING_SYM_BLCORNER;
				break;
			case BUILDING_SYM_TLCORNER:
				*currenttile = BUILDING_SYM_TRCORNER;
				break;
			case BUILDING_SYM_VWALL:
				*currenttile = BUILDING_SYM_HWALL;
				break;
			case BUILDING_SYM_HWALL:
				*currenttile = BUILDING_SYM_VWALL;
				break;
			case BUILDING_SYM_VDOOR:
				*currenttile = BUILDING_SYM_HDOOR;
				break;
			case BUILDING_SYM_HDOOR:
				*currenttile = BUILDING_SYM_VDOOR;
				break;
			}
		}
	}
}
void HFlipPrefabTiles(PPREFAB prefab)
{
	int* currenttile;
	for(int h = 0; h < prefab->height; h++){
		for(int w = 0; w < prefab->width; w++){
			currenttile = &prefab->prefabdata[h][w];
			switch(*currenttile){
			case BUILDING_SYM_BLCORNER:
				*currenttile = BUILDING_SYM_BRCORNER;
				break;
			case BUILDING_SYM_TRCORNER:
				*currenttile = BUILDING_SYM_TLCORNER;
				break;
			case BUILDING_SYM_BRCORNER:
				*currenttile = BUILDING_SYM_BLCORNER;
				break;
			case BUILDING_SYM_TLCORNER:
				*currenttile = BUILDING_SYM_TRCORNER;
				break;
			}
		}
	}
}
void VFlipPrefabTiles(PPREFAB prefab)
{
		int* currenttile;
	for(int h = 0; h < prefab->height; h++){
		for(int w = 0; w < prefab->width; w++){
			currenttile = &prefab->prefabdata[h][w];
			switch(*currenttile){
			case BUILDING_SYM_BLCORNER:
				*currenttile = BUILDING_SYM_TLCORNER;
				break;
			case BUILDING_SYM_TRCORNER:
				*currenttile = BUILDING_SYM_BRCORNER;
				break;
			case BUILDING_SYM_BRCORNER:
				*currenttile = BUILDING_SYM_TRCORNER;
				break;
			case BUILDING_SYM_TLCORNER:
				*currenttile = BUILDING_SYM_BLCORNER;
				break;
			}
		}
	}
}

//FULL PREFAB OPERATIONS
void RotatePrefab(PPREFAB prefab)
{
	int oldvertcounter;
	int newvertcounter;
	int oldhorizcounter;
	int newhorizcounter;
	PREFAB tempprefab;
	//FIRST, CREATE COPY.
		InitializePrefab(&tempprefab, prefab->width, prefab->height);
	FillPrefab(&tempprefab, prefab->prefabdata, prefab->width, prefab->height);
	//ROTATE ALL TILES IN THE NEW PREFAB.
	RotatePrefabTiles(&tempprefab);

	//NOW, TERMINATE OLD PREFAB AND RE-ALLOCATE TO REVERSED SIZE.
	TerminatePrefab(prefab);
	InitializePrefab(prefab, tempprefab.height, tempprefab.width);

	//ALL SET TO ITERATE. START FROM OLD BOTTOM, ITERATE FORWARD.
	//YOU MUST FILL VERTICALLY IN NEW PREFAB THOUGH.
	for(oldvertcounter = tempprefab.height  -1, newhorizcounter = 0; newhorizcounter < prefab->width; newhorizcounter++, oldvertcounter--)
	{
		for(oldhorizcounter =0, newvertcounter = 0; oldhorizcounter < tempprefab.width; oldhorizcounter++, newvertcounter++){
			prefab->prefabdata[newvertcounter][newhorizcounter] = tempprefab.prefabdata[oldvertcounter][oldhorizcounter];
		}
	}

}
void HFlipPrefab(PPREFAB prefab)
{
	int linecounter;
	int forwardcount;
	int backwardcount;
	PREFAB tempprefab;
	//THIS ONE'S RELATIVELY EASY.
	//CREATE A NEW PREFAB THE EXACT SAME SIZE AS THE OLD ONE. COPY CONTENTS INTO IT.
	InitializePrefab(&tempprefab, prefab->width, prefab->height);
	FillPrefab(&tempprefab, prefab->prefabdata, prefab->width, prefab->height);
	//HFLIP ALL TILES IN THE NEW PREFAB.
	HFlipPrefabTiles(&tempprefab);
	//COPY FROM NEW PREFAB INTO OLD PREFAB, TOP TO BOTTOM, RIGHT TO LEFT.
	for(linecounter = 0; linecounter < prefab->height; linecounter++){
		for(forwardcount = 0, backwardcount = prefab->width - 1; forwardcount < prefab->width; forwardcount++, backwardcount--)
		{
			prefab->prefabdata[linecounter][forwardcount] = tempprefab.prefabdata[linecounter][backwardcount];
		}
	}

}
void VFlipPrefab(PPREFAB prefab)
{
	int forwardlinecounter;
	int backwardlinecounter;
	int forwardxcount;
	PREFAB tempprefab;
	//THIS ONE'S LIKE THE HORIZONTAL FLIP.
	//CREATE A NEW PREFAB THE EXACT SAME SIZE AS THE OLD ONE. COPY CONTENTS INTO IT.
	InitializePrefab(&tempprefab, prefab->width, prefab->height);
	FillPrefab(&tempprefab, prefab->prefabdata, prefab->width, prefab->height);
	//VFLIP ALL TILES IN THE NEW PREFAB.
	VFlipPrefabTiles(&tempprefab);
	//COPY FROM NEW PREFAB INTO OLD PREFAB, BOTTOM TO TOP, LEFT TO RIGHT.
	for(forwardlinecounter = 0, backwardlinecounter = prefab->height - 1; forwardlinecounter < prefab->height; forwardlinecounter++, backwardlinecounter--){
		for(forwardxcount = 0; forwardxcount < prefab->width; forwardxcount++)
		{
			prefab->prefabdata[forwardlinecounter][forwardxcount] = tempprefab.prefabdata[backwardlinecounter][forwardxcount];
		}
	}

}


//DEBUGGING OPERATIONS
PREFAB CreateDebugPrefab()
{
	PREFAB temp;
	InitializePrefab(&temp, 5, 3);
	temp.prefabdata[0][0] = BUILDING_SYM_TLCORNER;
	temp.prefabdata[0][1] = BUILDING_SYM_HWALL;
	temp.prefabdata[0][2] = BUILDING_SYM_HWALL;
	temp.prefabdata[0][3] = BUILDING_SYM_HWALL;
	temp.prefabdata[0][4] = BUILDING_SYM_TRCORNER;
	temp.prefabdata[1][0] = BUILDING_SYM_BLCORNER;
	temp.prefabdata[1][1] = BUILDING_SYM_TRCORNER;
	temp.prefabdata[1][2] = OBJECT_SYM_FLOOR;
	temp.prefabdata[1][3] = OBJECT_SYM_FLOOR;
	temp.prefabdata[1][4] = BUILDING_SYM_VWALL;
	temp.prefabdata[2][0] = OBJECT_SYM_GROUND;
	temp.prefabdata[2][1] = BUILDING_SYM_BLCORNER;
	temp.prefabdata[2][2] = BUILDING_SYM_HWALL;
	temp.prefabdata[2][3] = BUILDING_SYM_HWALL;
	temp.prefabdata[2][4] = BUILDING_SYM_BRCORNER;
	return temp;
}
void CreatePrefabString(PPREFAB prefab, char* out)
{
	char tempbuf[2000];
	char quickbuf[2];
	sprintf(out, "");
	for(int h = 0; h < prefab->height; h++)
	{
		sprintf(tempbuf, "");
		for(int w = 0; w < prefab->width; w++){
			sprintf(quickbuf, "%c", (char)prefab->prefabdata[h][w]);
			strcat(tempbuf, quickbuf);
		}
		strcat(out, tempbuf);
		strcat(out, "\n");
	}
}

int LoadPrefabArrayFromDisk(PPREFAB* output, char* prefabfile) //RETURNS NUMBER OF ELEMENTS, IF NOTHING OR CORRUPT, RETURNS ZERO.
{
	FILE* fp;
	int numberofprefabs;
	
	int mTileSize;
	int mWidth;
	int mHeight;
//	int* mTempTiles;
//	int** mTempDoubleArray;
	int numcheck = 0;
	int mId;

//	PPREFAB easytoreadarray;
	
	
	if(prefabfile == NULL){
		return 0;
	}
	if((fp = fopen(prefabfile, "rb")) == NULL){
		return 0;
	}
	//THE FIRST NUMBER CORRESPONDS TO THE NUMBER OF PREFABS IN THE FILE. LET'S READ THAT IN.
	if(!fread((void*)&numberofprefabs, sizeof(int), 1, fp)){
		fclose(fp);
		return 0;
	}
	if(numberofprefabs < 1){
		fclose(fp);
		return 0;
	}
	//NOW, MOVING ON. MALLOC THE ARRAY OF PREFABS.
	*output = (PPREFAB) malloc(sizeof(PREFAB) * numberofprefabs);
	
	//NOW, WE MUST COMPLETE A SPECIAL LOADING SEQUENCE ONCE FOR EACH PREFAB.
	for(int prefabcounter = 0; prefabcounter < numberofprefabs; prefabcounter++)
	{
		//THE SEQUENCE IS THIS:
		//FIRST INTEGER IS ID.
		if(!fread((void*)&mId, sizeof(int), 1, fp)){
			fclose(fp);
			return 0;
		}
		//SECOND INTEGER IS WIDTH.
		if(!fread((void*)&mWidth, sizeof(int), 1, fp)){
			fclose(fp);
			return 0;
		}
		//THIRD INTEGER IS HEIGHT.
		if(!fread((void*)&mHeight, sizeof(int), 1, fp)){
			fclose(fp);
			return 0;
		}
		
		InitializePrefab(&((*output)[prefabcounter]), mWidth, mHeight);
		mTileSize = mWidth * mHeight;

		PREFAB tempfab;
		InitializePrefab(&tempfab, mWidth, mHeight);
		int kw;
		(*output)[prefabcounter].id = mId;
			for(int m = 0; m < mHeight; m++){
				fread((void*)tempfab.prefabdata[m], sizeof(int) * mWidth, 1, fp);
				//CONVERT ALL TILE TYPES TO ACTUAL DATA.
				for(kw = 0; kw < mWidth; kw++){
					tempfab.prefabdata[m][kw] = TileTypeToPrefabData(tempfab.prefabdata[m][kw]);
				}
		}
			
		FillPrefab(&(*output)[prefabcounter], tempfab.prefabdata, mWidth, mHeight);
		TerminatePrefab(&tempfab);
		//AND BEGIN AGAIN. INCREMENT THE FAILSAFE COUNTER.
		numcheck++;
	}
	fclose(fp);
	if(numcheck < numberofprefabs){
		TerminatePrefabArray(*output, numcheck);
		return 0;
	}
	return numberofprefabs;
}

bool SavePrefabArrayToDisk(PPREFAB array, int numberelements, char* filename)
{
	FILE* fp;
	if(!(fp = fopen(filename, "wb"))){
		return false;
	}
	fwrite((void*)&numberelements, sizeof(int),1,fp); //WRITE THE NUMBER OF ELEMENTS.
	for(int i = 0; i < numberelements; i++){
		//WRITE THE ID
		fwrite((void*)&(array[i].id), sizeof(int), 1, fp);
		//WRITE THE WIDTH
		fwrite((void*)&(array[i].width), sizeof(int), 1, fp);
		//WRITE THE HEIGHT
		fwrite((void*)&(array[i].height), sizeof(int), 1, fp);
		//WRITE THE DATA.
		int kw;
		for(int h = 0; h < array[i].height; h++){
			for(kw = 0; kw < array[i].width; kw++){
				array[i].prefabdata[h][kw] = PrefabDataToTileType(array[i].prefabdata[h][kw]);
			}
				fwrite((void*)array[i].prefabdata[h], sizeof(int) * array[i].width, 1, fp);
		}
	}
	fclose(fp);
	return true;
}

void TerminatePrefabArray(PPREFAB array, int numberofelements)
{
	for(int i = 0; i < numberofelements; i++){
		TerminatePrefab(&(array[i]));
//		free(&(array[i]));
	}
	free(array);
}

int PrefabDataToTileType(int data)
{
	switch(data){
	case OBJECT_SYM_GROUND:
		return 8;
	case OBJECT_SYM_FLOOR:
		return 9;
	case BUILDING_SYM_BLCORNER:
		return 2;
	case BUILDING_SYM_TRCORNER:
		return 1;
	case BUILDING_SYM_BRCORNER:
		return 3;
	case BUILDING_SYM_TLCORNER:
		return 0;
	case BUILDING_SYM_VWALL:
		return 4;
	case BUILDING_SYM_HWALL:
		return 5;
	case BUILDING_SYM_VDOOR:
		return 6;
	case BUILDING_SYM_HDOOR:
		return 7;
	default:
		return 9;
	}
}

int TileTypeToPrefabData(int type)
{
	switch(type){
	case 9:
		return OBJECT_SYM_FLOOR;
	case 8:
		return OBJECT_SYM_GROUND;
	case 2:
		return BUILDING_SYM_BLCORNER;
	case 1:
		return BUILDING_SYM_TRCORNER;
	case 3:
		return BUILDING_SYM_BRCORNER;
	case 0:
		return BUILDING_SYM_TLCORNER;
	case 5:
		return BUILDING_SYM_HWALL;
	case 4:
		return BUILDING_SYM_VWALL;
	case 7:
		return BUILDING_SYM_HDOOR;
	case 6:
		return BUILDING_SYM_VDOOR;
	default:
		return OBJECT_SYM_FLOOR;
	}
	return OBJECT_SYM_FLOOR;
}