#include "agencyinclude.h"
#include "maplib.h"
#include "prefabs.h"
#include "guns.h"
CHAR_INFO* g_HistorySnapshot = NULL;
CHAR_INFO* g_MainSnapshot = NULL;
WORD TempUnderCursorObjectColor;
char TempUnderCursorObjectChar;



int FillMapWithPrefabs(PMAP map, int DensityPercentage)
{
	PPREFAB PrefabArray;
	int numelements;
	PREFAB OurPrefab;
	bool** LockOutArray;
	int OurArrayIndex;
	bool* PrefabTriedArray;
	int CurrentDensity = 0;
	int TotalTiles = map->width * map->height;
	int FilledTiles = 0;
	int PlaceResult;
	int PlaceIterations = 0;
	int NumberRotations = 0;
	int returnvalue = PREFAB_FILL_OK;
	int TotalTried = 0;
	char path[255];
	//	char dbuf[255];
	POINT PlaceOrigin;
	bool PrefabPlaced = false;
	bool NoMoreCombinationsPossible = false;
	
	sprintf(path, "data\\%s", PREFABFILENAME);
	if((numelements = LoadPrefabArrayFromDisk(&PrefabArray, path)) < 1){
		return 0;
	}
	//WE HAVE A PREFAB ARRAY.
	InitDoubleArray(&LockOutArray, map->width, map->height);
	PrefabTriedArray = (bool*) malloc(sizeof(bool) * numelements);
	
	while(CurrentDensity < DensityPercentage && !NoMoreCombinationsPossible) //THE MAIN LOOP
	{
		
		memset((void*)PrefabTriedArray, 0, sizeof(bool) * numelements); //CLEARING THE ARRAY
		TotalTried = 0;
		while(!PrefabPlaced)
		{
			
			if(TotalTried >= numelements - 1)
			{
				NoMoreCombinationsPossible = true;
				break;
			}
			
			do
			{
				OurArrayIndex = rand()%numelements;
			}while(PrefabTriedArray[OurArrayIndex]); //WHILE IT'S ALREADY BEEN TRIED...
			InitializePrefab(&OurPrefab, PrefabArray[OurArrayIndex].width, PrefabArray[OurArrayIndex].height);
			FillPrefab(&OurPrefab, PrefabArray[OurArrayIndex].prefabdata, PrefabArray[OurArrayIndex].width, PrefabArray[OurArrayIndex].height);
			PrefabTriedArray[OurArrayIndex] = true;
			TotalTried++;
			if(OurPrefab.width > map->width || OurPrefab.height > map->height){
				PrefabPlaced = false;
				TerminatePrefab(&OurPrefab);
				break; //IT'S TOO HUUUUGE!
			}
			NumberRotations = rand()%4;
			for(int rotatecount = 0; rotatecount < NumberRotations; rotatecount++){
				RotatePrefab(&OurPrefab);
			}
			if(RandomPercentage(50))
			{
				HFlipPrefab(&OurPrefab);
			}
			if(RandomPercentage(50))
			{
				VFlipPrefab(&OurPrefab);
			}
			
			do
			{
				PlaceOrigin.x = random(3, map->width - OurPrefab.width - 4);
				PlaceOrigin.y = random(3, map->height - OurPrefab.height - 4);
				PlaceResult = TryToPlacePrefab(&OurPrefab, map, PlaceOrigin, &LockOutArray);
				PlaceIterations++;
			}while(PlaceResult != PREFAB_PLACED_OK && PlaceResult != PREFAB_PLACED_FAILURE_NULLCELL && PlaceIterations < 100);
			
			
			
			if(PlaceResult == PREFAB_PLACED_FAILURE_NULLCELL){
				returnvalue = PREFAB_FILL_FAILURE_NULLCELL;
				NoMoreCombinationsPossible = true;
				break;
			}
			else if (PlaceResult == PREFAB_PLACED_OK){
				FilledTiles += OurPrefab.height * OurPrefab.width;
				CurrentDensity = (int)(((double)FilledTiles / (double)TotalTiles) * 100.0);
				PrintPercentageComplete("Creating Map Structure-", CurrentDensity * 2);
				PrefabPlaced = true;
			}
			else{
				PrefabPlaced = false;
			}
			TerminatePrefab(&OurPrefab);
			PlaceIterations = 0;
		}
		PrefabPlaced = false;
	}
	
	//FREE OUR ARRAYS!
	TerminatePrefabArray(PrefabArray, numelements);
	FreeDoubleArray(&LockOutArray, map->height);
	free(PrefabTriedArray);
	cls(0);
	return returnvalue;
}

int TryToPlacePrefab(PPREFAB prefabtoplace, PMAP maptoplaceon, POINT originpoint, bool*** Plockoutarray)
{
	//	int checkcelltype;
	PMAPCELL Cell;
	POINT hwpoint;
	int iterateheight = originpoint.y + prefabtoplace->height;
	int iteratewidth = originpoint.x + prefabtoplace->width;
	bool lockedout = false;
	int ph;
	int pw;
	int h;
	int w;
	if(originpoint.x + prefabtoplace->width >= maptoplaceon->width || originpoint.y + prefabtoplace->height >= maptoplaceon->height){
		return PREFAB_PLACED_FAILURE_OUTOFBOUNDS;
	}
	for(h = originpoint.y; h < iterateheight; h++){
		for(w = originpoint.x; w < iteratewidth; w++){
			hwpoint.x = w;
			hwpoint.y = h;
			lockedout = (*Plockoutarray)[h][w];
			if(lockedout){
				return PREFAB_PLACED_FAILURE_LOCKEDOUT;
			}
		}
	}
	
	//WELL. WE MADE IT THIS FAR, I GUESS WE'RE CLEAR!
	for(h = originpoint.y, ph = 0; h < iterateheight; h++, ph++){
		for(w = originpoint.x, pw = 0; w < iteratewidth; w++, pw++){
			hwpoint.x = w;
			hwpoint.y = h;
			Cell = FindMapCellByCoordinate(maptoplaceon, hwpoint);
			if(Cell == NULL){
				return PREFAB_PLACED_FAILURE_NULLCELL;
			}
			Cell->type = prefabtoplace->prefabdata[ph][pw]; //SET THE TYPE
			
		}
	}
	
	//LOCK OUT THE LARGER SQUARE.
	for(h = originpoint.y - 1; h < iterateheight + 1; h++){
		for(w = originpoint.x -1; w < iteratewidth + 1; w++){
			hwpoint.x = w;
			hwpoint.y = h;
			Cell = FindMapCellByCoordinate(maptoplaceon, hwpoint);
			if(Cell == NULL){
				return PREFAB_PLACED_FAILURE_NULLCELL;
			}
			(*Plockoutarray)[h][w] = true; //LOCK IT OUT!
		}
	}
	
	return PREFAB_PLACED_OK; //WELL DONE.
}

void FillMapWithObstructions(PMAP map, int DensityPercentage)
{
	double CurrentDensity = 0;
	double TargetDensity = (double)DensityPercentage / 50.0;
	double TotalTiles = map->width * map->height;
	double FilledTiles = 0;
	POINT CellPoint;
	PMAPCELL Cell;
	unsigned int iterations;
	bool TilePlaced = false;
	
	while(CurrentDensity < TargetDensity)
	{
		iterations = 0;
		TilePlaced = false;
		do
		{
			CellPoint.x = random(2, map->width - 2);
			CellPoint.y = random(2, map->height - 2);
			Cell = FindMapCellByCoordinate(map, CellPoint);
			if(!Cell){
				return;
			}
			if(Cell->type == OBJECT_SYM_GROUND){
				if(RandomPercentage(50)){
					Cell->type = OBJECT_SYM_HIOBSTRUCT;
				}
				else{
					Cell->type = OBJECT_SYM_LOOBSTRUCT;
				}
				TilePlaced = true;
				FilledTiles++;
				CurrentDensity = (FilledTiles * 100.0) / TotalTiles;
				PrintPercentageComplete("Placing Objects-", (int)(CurrentDensity * 100.0 / TargetDensity));
			}
			iterations++;
		}while(!TilePlaced && iterations < 1000);
		if(iterations > 1000){
			return; //CAN'T FIT NO MO'!
		}
	}
}

void ClearDoubleArray(bool *** array, int width, int height)
{
	for(int h =0; h < height; h++)
	{
		for(int w= 0; w < width; w++)
		{
			(*array)[h][w] = 0;
		}
	}
}

void DebugMapViewer(PPLAYER player, PMAP map)
{
	POINT playerloc;
	playerloc.x = map->rz.x;
	playerloc.y = map->rz.y;
	POINT drawoffset;
	drawoffset.x = 0;
	drawoffset.y = 0;
	int key;
	while(1)
	{
		//DrawMapViewport(map, drawoffset, getconsize().X - 1,getconsize().Y - 1, playerloc);
		DrawMapViewport(player, map, drawoffset, 10, 8, playerloc, playerloc, false);
		zerocursor();
		printf("(%d,%d) - ESC TO QUIT", playerloc.x, playerloc.y);
		key = waitforVkey();
		if(key == VK_ESCAPE){
			return;
		}
		if (key == VK_DOWN){
			playerloc.y++;
			
		}
		if (key == VK_RIGHT){
			playerloc.x++;
			
		}
		if(key == VK_LEFT){
			playerloc.x--;
		}
		if (key == VK_UP){
			playerloc.y--;
		}
		playerloc.x = CapInclusive(playerloc.x, 1, map->width - 2);
		playerloc.y = CapInclusive(playerloc.y, 1, map->height - 2);
	}
}

void DrawMapCellLineAtCoordinates(PPLAYER player, PMAP map, POINT* mapcells, POINT drawcoordinateorigin, int numelements, POINT localcursorloc, bool drawcursor, bool nv, bool therm)
{
	PMAPCELL cell; 
	//WE NEED TO MAKE ARRAYS OUT OF OUR LISTS TO SAVE COMPUTATION TIME.
	//ONLY INSERT INTO THE ARRAYS, OBJECTS WHICH HAVE RELEVANCE TO THE CURRENT COORDINATES.
	CHAR_INFO* cellstodraw = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * numelements);
	COORD cellssize;
	cellssize.X = numelements;
	cellssize.Y = 1;
	SMALL_RECT targetrect;
	targetrect.Top = (short)drawcoordinateorigin.y;
	targetrect.Bottom = (short)drawcoordinateorigin.y;
	targetrect.Left = (short)drawcoordinateorigin.x;
	targetrect.Right = (short)targetrect.Left + numelements;
	
	if(!mapcells)
	{
		return;
	}
	for(int i = 0; i < numelements; i++)
	{
		
		//NOW ASSEMBLE THE STRING AND PRINT OUT.
		cell = FindMapCellByCoordinate(map, mapcells[i]);
		
		if(!cell){
			return;
		}
		
		//BEFORE ANY PRIORITIES COMES THE CURSOR.
		if(drawcursor){
			if(localcursorloc.x == cell->xcoord && localcursorloc.y == cell->ycoord)
			{
				cellstodraw[i].Char.AsciiChar = (char)219;
				cellstodraw[i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			}
			else{
				cellstodraw[i] = GetMapCharacterToDraw(map, player, cell, nv, therm);
			}
		}
		else{
			cellstodraw[i] = GetMapCharacterToDraw(map, player, cell, nv, therm);
		}
		
		
	}
	//setcursor(drawcoordinateorigin.x, drawcoordinateorigin.y);
	WriteConsoleOutput(getconsoleoutputh(), cellstodraw, cellssize, globals.zeroed, &targetrect);
	free(cellstodraw);
}

CHAR_INFO GetMapCharacterToDraw(PMAP map, PPLAYER player, PMAPCELL cell, bool nv, bool therm)
{
	CHAR_INFO OurChar;
	PVEHICLE Vehicle;
	PENEMY Enemy;
	POINT Thing;
	Thing.x = cell->xcoord;
	Thing.y = cell->ycoord;
	POINT MePoint;
	MePoint.x = player->location->xcoord;
	MePoint.y = player->location->ycoord;
	bool Priority = false;
	bool Environ = false;
	if(DoesEnvironmentalEffectExistAtPoint(MePoint, -1)){
		Environ = true;
	}
	switch(cell->type)
	{
	case OBJECT_SYM_GROUND:
		if(therm){
			OurChar.Attributes = FOREGROUND_RED;
		}
		else if(nv){
			OurChar.Attributes = FOREGROUND_GREEN;
		}
		else{
			OurChar.Attributes = FOREGROUND_RED | FOREGROUND_GREEN;
		}
		break;
	case OBJECT_SYM_FLOOR:
		if(therm){
			OurChar.Attributes = FOREGROUND_RED;
		}
		else if(nv){
			OurChar.Attributes = FOREGROUND_GREEN;
		}
		else{
			OurChar.Attributes = FOREGROUND_INTENSITY;
		}
		break;
	case OBJECT_SYM_FENCE:
	case OBJECT_SYM_GATE:
		if(therm){
			OurChar.Attributes = FOREGROUND_RED;
		}
		else if(nv){
			OurChar.Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		}
		else{
			OurChar.Attributes = FOREGROUND_INTENSITY;
		}	
	case BUILDING_SYM_BLCORNER:
	case BUILDING_SYM_TRCORNER:
	case BUILDING_SYM_BRCORNER:
	case BUILDING_SYM_TLCORNER:
	case BUILDING_SYM_VWALL:
	case BUILDING_SYM_HWALL:
		if(therm){
			OurChar.Attributes = FOREGROUND_RED;
		}
		else if(nv){
			OurChar.Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		}
		else{
			OurChar.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		}
		break;
	case BUILDING_SYM_VDOOR:
	case BUILDING_SYM_HDOOR:
		if(therm){
			OurChar.Attributes = FOREGROUND_RED;
		}
		else if(nv){
			OurChar.Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		}
		else{
			OurChar.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		}
		break;
	case OBJECT_SYM_HIOBSTRUCT:
	case OBJECT_SYM_LOOBSTRUCT:
		if(therm){
			OurChar.Attributes = FOREGROUND_RED;
		}
		else if(nv){
			OurChar.Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		}
		else{
			OurChar.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		}
		break;
	}
	

	
	//WEAPONS
	if(DoesMapCellHaveAnyItems(cell) && IsTypeOfObjectWithinVisionRange(RAYCAST_OBJECT_ITEMS, map, &player->psychskill, player->location, cell, nv, therm, Environ)){
			if(therm){
			OurChar.Attributes = FOREGROUND_RED;
		}
		else if(nv){
			OurChar.Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		}
		else{
			OurChar.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		}
		OurChar.Char.AsciiChar = (char)OBJECT_SYM_ITEMS;
	}
	
	//GIZMOS
	if(GetMapCellGizmo(cell) && IsTypeOfObjectWithinVisionRange(RAYCAST_OBJECT_GIZMOS, map, &player->psychskill, player->location, cell, nv, therm, Environ)){
		if(therm){
			OurChar.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
		}
		else if(nv){
			OurChar.Attributes = FOREGROUND_GREEN;
		}
		else{
			OurChar.Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		}
		
		OurChar.Char.AsciiChar = (char)OBJECT_SYM_PLAYERGIZMO;
		Priority = true;
	}
	
	//ARMORIES
	if(GetMapCellArmory(cell) && IsTypeOfObjectWithinVisionRange(RAYCAST_OBJECT_GIZMOS, map, &player->psychskill, player->location, cell, nv, therm, Environ)){
		if(therm){
			OurChar.Attributes = FOREGROUND_RED;
		}
		else if(nv){
			OurChar.Attributes = FOREGROUND_GREEN;
		}
		else{
			OurChar.Attributes = FOREGROUND_BLUE;
		}
		OurChar.Char.AsciiChar = 'A';
		Priority = true;
	}
	
	//VEHICLES
	if((Vehicle =GetMapCellVehicle(cell)) != NULL && IsTypeOfObjectWithinVisionRange(RAYCAST_OBJECT_VEHICLES, map, &player->psychskill, player->location, cell, nv, therm, Environ)){
		if(Vehicle->hitpoints < 1){
			OurChar.Char.AsciiChar = VEHICLE_SYM_DEADVEHICLE;
			if(therm){
				OurChar.Attributes = FOREGROUND_RED;
			}
			else if(nv){
				OurChar.Attributes = FOREGROUND_GREEN;
			}
			else{
				OurChar.Attributes = FOREGROUND_INTENSITY;
			}
		}
		else{
			OurChar.Char.AsciiChar = VEHICLE_SYM_LIVEVEHICLE;
			if(therm){
				OurChar.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
			}
			else if(nv){
				OurChar.Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			}
			else{
				OurChar.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			}
		}
		Priority = true;
	}
	
	if((Enemy = GetMapCellEnemy(cell)) != NULL){
		if(Enemy->status != ENEMY_STATUS_ALIVE && IsTypeOfObjectWithinVisionRange(RAYCAST_OBJECT_DEADHUMANS, map, &player->psychskill, player->location, cell, nv, therm, Environ)){
			OurChar.Char.AsciiChar = ENEMY_SYM_DEADENEMY;
			if(therm){
				OurChar.Attributes = FOREGROUND_RED;
			}
			else if(nv){
				OurChar.Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			}
			else{
				OurChar.Attributes = FOREGROUND_INTENSITY;
			}
			Priority = true;
		}
		else if(IsTypeOfObjectWithinVisionRange(RAYCAST_OBJECT_LIVEHUMANS, map, &player->psychskill, player->location, cell, nv, therm, Environ)){
			if(Enemy->friendly){
				if(therm){
					OurChar.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
				}
				else if(nv){
					OurChar.Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				}
				else{
					OurChar.Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
				}
				OurChar.Char.AsciiChar = PLAYER_SYM_PLAYER;
			}
			else if(Enemy->primary){
				if(therm){
					OurChar.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
				}
				else if(nv){
					OurChar.Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				}
				else{
					OurChar.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				}
				OurChar.Char.AsciiChar = ENEMY_SYM_PRIMARYTARGETENEMY;
			}
			
			else{
				switch(Enemy->turndirection){
				case TURN_DIRECTION_NORTH:
					OurChar.Char.AsciiChar = ENEMY_NORTH;
					break;
				case TURN_DIRECTION_SOUTH:
					OurChar.Char.AsciiChar = ENEMY_SOUTH;
					break;
				case TURN_DIRECTION_EAST:
					OurChar.Char.AsciiChar = ENEMY_EAST;
					break;
				case TURN_DIRECTION_WEST:
					OurChar.Char.AsciiChar = ENEMY_WEST;
					break;
				}
			}
			//TO DECIDE COLOR, RUN SWITCH FOR AI
			if(therm){
				OurChar.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
			}
			else if(nv){
				OurChar.Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			}
			else{
				switch(Enemy->AIStruct.CurrentAlertLevel){
				case ENEMY_ALERT_WHITE:
				default:
					OurChar.Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
					break;
				case ENEMY_ALERT_YELLOW:
				case ENEMY_ALERT_ORANGE:
					OurChar.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
					break;
				case ENEMY_ALERT_RED:
				case ENEMY_ALERT_BLACK:
					OurChar.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
					break;
				}
			}
			Priority = true;
		}
		
	}
	
	//PRIMARY TARGET
	if(cell->xcoord == map->pz.x && cell->ycoord == map->pz.y && IsTypeOfObjectWithinVisionRange(RAYCAST_OBJECT_LIVEHUMANS, map, &player->psychskill, player->location, cell, nv, therm, Environ)){
		if(therm){
			OurChar.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
		}
		else if(nv){
			OurChar.Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		}
		else{
			OurChar.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
		}
		OurChar.Char.AsciiChar = (char)MISSION_SYM_PZ;
		Priority = true;
	}
	
	//THE RZ
	if(map->rz.x == cell->xcoord && map->rz.y == cell->ycoord && IsLeftBehindTimeLeft(player) && !IsRecoveryTimeLeft(player) && IsTypeOfObjectWithinVisionRange(RAYCAST_OBJECT_VEHICLES, map, &player->psychskill, player->location, cell, nv, therm, Environ)){
		if(therm){
			OurChar.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
		}
		else if(nv){
			OurChar.Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		}
		else{
			OurChar.Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		}
		OurChar.Char.AsciiChar = 'R';
		Priority = true;
	}
	
	//ENVIRONMENTALS
	int rnd;
	if(IsTypeOfObjectWithinVisionRange(RAYCAST_OBJECT_BUILDINGS, map, &player->psychskill, player->location, cell, nv, therm, Environ)){
	switch(DoesEnvironmentalEffectExistAtPoint(Thing, ENVIRON_TYPE_ALL))
	{
	case ENVIRON_TYPE_SMOKE:
		if(therm){
			OurChar.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
		}
		else if(nv){
			OurChar.Attributes = FOREGROUND_GREEN;
		}
		else{
			OurChar.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		}
		OurChar.Char.AsciiChar = (char)ENVIRON_SYM_SMOKE;
		Priority = true;
		break;
	case ENVIRON_TYPE_FIRE:
		if(therm){
			OurChar.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN;
		}
		else if(nv){
			OurChar.Attributes = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		}
		else{
			rnd = rand()%3;
			switch(rnd)
			{
			case 0:
				OurChar.Attributes = FOREGROUND_RED;
				break;
			case 1:
				OurChar.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
				break;
			case 2:
				OurChar.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				break;
			}
		}
		OurChar.Char.AsciiChar = ENVIRON_SYM_FIRE;
		Priority = true;
		break;
	case ENVIRON_TYPE_TOXIN:
		if(therm){
			OurChar.Attributes = FOREGROUND_RED;
		}
		else if(nv){
			OurChar.Attributes = FOREGROUND_GREEN;
		}
		else{
			OurChar.Attributes = FOREGROUND_GREEN;
		}
		OurChar.Char.AsciiChar = (char)ENVIRON_SYM_POISON;
		Priority = true;
		break;
	}
	}
	
	//FINAL PRIORITY - PLAYER?
	if(cell == player->location)
	{
		if(therm){
			OurChar.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
		}
		else if(nv){
			OurChar.Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		}
		else{
			OurChar.Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		}
		OurChar.Char.AsciiChar = PLAYER_SYM_PLAYER;
		Priority = true;
	}

	//FINAL FINAL PRIORITY - IF YOU CANT SEE SHEEIT
	if(!CalculateVisionTrajectory(map, player->location, cell)){
			if(nv){
			OurChar.Attributes = FOREGROUND_GREEN;
		}
		else{
			OurChar.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		}
		if(therm && Enemy && IsTypeOfObjectWithinVisionRange(RAYCAST_OBJECT_LIVEHUMANS, map, &player->psychskill, player->location, cell, nv, therm, Environ)){ //IF THERE'S AN ENEMY OUT THERE, WE CAN SEE HIM.
			OurChar.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
			OurChar.Char.AsciiChar = PLAYER_SYM_PLAYER;
		}
			else{
		OurChar.Char.AsciiChar = ' ';
			}
		Priority = true;
	}
	
	if(!Priority) //IF NOTHING ELSE.............
	{
		if(IsTypeOfObjectWithinVisionRange(RAYCAST_OBJECT_BUILDINGS, map, &player->psychskill, player->location, cell, nv, therm, Environ)){
		OurChar.Char.AsciiChar = cell->type;
		}
		else{
			OurChar.Char.AsciiChar = ' ';
		}
	}
	return OurChar;
}

PVEHICLE GetMapCellVehicle(PMAPCELL Cell)
{
	PLIST walker = globals.globalmapstruct.vehicles;
	if(!Cell){
		return NULL;
	}
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		if(walker->current->object.vehicle.loc.x == Cell->xcoord && walker->current->object.vehicle.loc.y == Cell->ycoord){
			return &walker->current->object.vehicle;
		}
	}
	return NULL;
}

PGIZMO GetMapCellGizmo(PMAPCELL Cell)
{
	PLIST walker = globals.globalmapstruct.gizmos;
	if(!Cell){
		return NULL;
	}
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		if(walker->current->object.gizmo.WorldPoint.x == Cell->xcoord && walker->current->object.gizmo.WorldPoint.y == Cell->ycoord){
			return &walker->current->object.gizmo;
		}
	}
	return NULL;
}

PMAPARMORY GetMapCellArmory(PMAPCELL Cell)
{
	if(!Cell){
		return NULL;
	}
	for(int i = 0; i < globals.globalmapstruct.numarmories; i++){
		if(globals.globalmapstruct.armories[i].location.x == Cell->xcoord && globals.globalmapstruct.armories[i].location.y == Cell->ycoord && !globals.globalmapstruct.armories[i].destroyed){
			return &(globals.globalmapstruct.armories[i]);
		}
	}
	return NULL;
}

PENEMY GetMapCellEnemy(PMAPCELL Cell)
{
	if(!Cell){
		return NULL;
	}
	for(int i = 0; i < globals.globalmapstruct.numenemies; i++){
		if(globals.globalmapstruct.enemies[i].location == Cell){
			return &(globals.globalmapstruct.enemies[i]);
		}
	}
	return NULL;
}

bool DrawMapViewport(PPLAYER player, PMAP map, POINT draworigin, int viewportwidth, int viewportheight, POINT viewcenterloc, POINT localcursorloc, bool drawcursor)
{
	//WE INITIALLY TRY TO DRAW THE PLAYER IN THE CENTER.
	POINT offset;
	bool nv = (FindActiveGizmoOfType(GIZMO_TYPE_NVG, player->p_gizmos) != NULL);
	bool therm = (FindActiveGizmoOfType(GIZMO_TYPE_TAS, player->p_gizmos) != NULL);
	
	POINT FinalArrayAccessPoint;
	POINT FinalViewPortDrawPoint;
	POINT* mapcells = (POINT*) malloc(sizeof(POINT) * viewportwidth);
	char * flashcells = (char*) malloc(sizeof(char) * (viewportwidth + 1));
	char flashchar = (char)ENVIRON_SYM_FLASH;
	for(int fl = 0; fl < viewportwidth; fl++){
		flashcells[fl] = flashchar;
	}
	flashcells[fl] = '\0';
	POINT drawcoords;
	offset.x = 0;
	offset.y = 0;
	POINT drawplayerpoint;
	POINT arraystart;
	//	PMAPCELL cell;
	bool right = false;
	bool bottom = false;
	int counter = 0;
	
	arraystart.x = viewcenterloc.x - (viewportwidth / 2);
	arraystart.y = viewcenterloc.y - (viewportheight /2);
	drawplayerpoint.x = draworigin.x + (viewportwidth / 2);
	drawplayerpoint.y = draworigin.y + (viewportheight / 2);
	if(arraystart.x < 0){
		offset.x = arraystart.x;
	}
	else if (arraystart.x + viewportwidth >= map->width){
		offset.x = (arraystart.x + viewportwidth) - map->width;
	}
	
	if(arraystart.y < 0){
		offset.y = arraystart.y;
	}
	else if (arraystart.y + viewportheight >= map->height){
		offset.y = (arraystart.y + viewportheight) - map->height;
	}
	
	//NOW WE DRAW THE GROUND ETC MINUS THE OFFSET.
	FinalViewPortDrawPoint.x = draworigin.x;
	FinalViewPortDrawPoint.y = draworigin.y;
	drawcoords.x = FinalViewPortDrawPoint.x;
	for(int h = 0; h < viewportheight; h++)
	{
		for(int w = 0; w < viewportwidth; w++)
		{
			
			FinalArrayAccessPoint.x = arraystart.x + w - offset.x;
			FinalArrayAccessPoint.y = arraystart.y + h - offset.y;
			mapcells[w].x = FinalArrayAccessPoint.x;
			mapcells[w].y = FinalArrayAccessPoint.y;
			counter++;
		}
		drawcoords.y = FinalViewPortDrawPoint.y + h;
		if(player->flashturnsleft > -1){
			WHITE;
			setcursor(drawcoords.x, drawcoords.y);
			writestring(flashcells);
		}
		else{
			DrawMapCellLineAtCoordinates(player, map, mapcells, drawcoords, viewportwidth, localcursorloc, drawcursor, nv, therm);
		}
	}
	//DONE!
	free(mapcells);
	free(flashcells);
	return true;
}

void InitDoubleArray(bool *** arraytoinit, int width, int height)
{
	
	*arraytoinit = (bool**)malloc(sizeof(bool*) * height);
	for(int i =0; i < height; i++)
	{
		(*arraytoinit)[i] = (bool*) malloc(sizeof(bool) * width);
	}
	ClearDoubleArray(arraytoinit, width, height);
}

void FreeDoubleArray(bool *** arraytofree, int height)
{
	for(int i =0; i < height; i++)
	{
		free((*arraytofree)[i]);
	}
	free(*arraytofree);
}

int MainMission(PPLAYER player, PMAP map)
{
	player->currentmissioninfo.primarycomplete = false;
	player->currentmissioninfo.dynamicmode = false;
	player->location = FindMapCellByCoordinate(map, map->rz);
	//	player->health = 100;
	player->turndirection = GetPlayerInitialTurnDirection(player, map);
	player->ZoomCenter = map->rz;
	player->LocalCursor = map->rz;
	player->CurrentActionMode = ACTION_MODE_MOVE;
	memset(&globals.globalmapstruct.totaltime, 0, sizeof(globals.globalmapstruct.totaltime));
	memset(&player->currentmissioninfo.missionstats, 0, sizeof(player->currentmissioninfo.missionstats));
	//BACK UP ORIGINAL PSYCHSKILL SO THAT PLAYER'S CAN BE MODIFIED BY GIZMOS AND SUCH.
	globals.BackupPsychSkill = player->psychskill;
	//player->currentmissioninfo.missiontype = MISSION_TYPE_CAUTERIZATION;
	
	//SET UP SCRIPTED EVENTS.
	
	
	//BEGIN NOW.
	InitializeMainUI(player, map);
	int RetVal = MainUILoop(player, map);
	//RESTORE OLD PSYCHSKILL
	player->psychskill = globals.BackupPsychSkill;
	return RetVal;
}

bool DoDamageToPlayer(PPLAYER player, int damageamount)
{
	player->health -= damageamount;
	player->currentmissioninfo.missionstats.damagetaken += damageamount;
	if(player->health < 1){
		return true;//DEAD
	}
	return false; //NOT DEAD
}

int GetPlayerInitialTurnDirection(PPLAYER player, PMAP map)
{
	if(map->rz.x < 2) //FACING EAST
	{
		return TURN_DIRECTION_EAST;
	}
	if(map->rz.y < 2)
	{
		return TURN_DIRECTION_SOUTH;
	}
	if(map->rz.x > map->width - 3)
	{
		return TURN_DIRECTION_WEST;
	}
	return TURN_DIRECTION_NORTH;
}

void InitializeMainUI(PPLAYER player, PMAP map)
{
	char buf[500];
	
	RECT HistoryBoxRect;
	RECT MapBoxRect;
	RECT TimeBoxRect;
	RECT StatusBoxRect;
	
	POINT SetPoint;
	
	
	
	MapBoxRect.left = MAINUI_MAPBOXXORIGIN;
	MapBoxRect.right = MapBoxRect.left + MAINUI_MAPBOXWIDTH;
	MapBoxRect.top = MAINUI_MAPBOXYORIGIN;
	MapBoxRect.bottom = MapBoxRect.top + MAINUI_MAPBOXHEIGHT;
	
	HistoryBoxRect.left = MAINUI_HISTORYBOXXORIGIN;
	HistoryBoxRect.top = MAINUI_HISTORYBOXYORIGIN;
	HistoryBoxRect.right = HistoryBoxRect.left + MAINUI_HISTORYBOXWIDTH;
	HistoryBoxRect.bottom = HistoryBoxRect.top + MAINUI_HISTORYBOXHEIGHT;
	
	StatusBoxRect.left = MAINUI_STATUSBOXXORIGIN;
	StatusBoxRect.top = MAINUI_STATUSBOXYORIGIN;
	StatusBoxRect.right = StatusBoxRect.left + MAINUI_STATUSBOXWIDTH;
	StatusBoxRect.bottom = StatusBoxRect.top + MAINUI_STATUSBOXHEIGHT;
	
	TimeBoxRect.left = MAINUI_TIMEBOXXORIGIN;
	TimeBoxRect.top = MAINUI_TIMEBOXYORIGIN;
	TimeBoxRect.right = TimeBoxRect.left + MAINUI_TIMEBOXWIDTH;
	TimeBoxRect.bottom = TimeBoxRect.top + MAINUI_TIMEBOXHEIGHT;
	
	
	
	
	
	LoopMainSong(player);
	
	cls(0);
	
	WHITE;
	printgraphic(globals.graphicslist, globals.zeroed, MAINUI_GRAPHIC_ID);
	//ACTIVATE STATUS
	sprintf(buf, "TACTICAL INFORMATION ACTIVE");
	//Sleep(200);
	GREEN;
	SetPoint = ReturnTextCenterInRect(buf, StatusBoxRect);
	setcursor(SetPoint.x, SetPoint.y);
	writestring(buf);
	Sleep(100);
	LGREEN;
	setcursor(SetPoint.x, SetPoint.y);
	writestring(buf);
	Sleep(1500);
	GREEN;
	SetPoint = ReturnTextCenterInRect(buf, StatusBoxRect);
	setcursor(SetPoint.x, SetPoint.y);
	writestring(buf);
	Sleep(100);
	MainUIPrintPlayerStatus(player);
	
	//ACTIVATE MAP
	sprintf(buf, "TRACKER SYSTEM ACTIVE");
	//Sleep(200);
	GREEN;
	SetPoint = ReturnTextCenterInRect(buf, MapBoxRect);
	setcursor(SetPoint.x, SetPoint.y);
	writestring(buf);
	Sleep(100);
	LGREEN;
	setcursor(SetPoint.x, SetPoint.y);
	writestring(buf);
	Sleep(1500);
	GREEN;
	SetPoint = ReturnTextCenterInRect(buf, MapBoxRect);
	setcursor(SetPoint.x, SetPoint.y);
	writestring(buf);
	Sleep(100);
	ClearRect(MapBoxRect);
	DisplayUIMap(player, map, true, false);
	TRACKERARROWS Arrows = EvaluatePlayerMoveDirections(player, map);
	DrawTrackerArrows(&Arrows);
	MainUIPrintCurrentAction(player, true);
	
	
	//ACTIVATE HISTORY.
	sprintf(buf, "DSA HISTORY TELEMETRY ACTIVE");
	//Sleep(200);
	GREEN;
	SetPoint = ReturnTextCenterInRect(buf, HistoryBoxRect);
	setcursor(SetPoint.x, SetPoint.y);
	writestring(buf);
	Sleep(100);
	LGREEN;
	setcursor(SetPoint.x, SetPoint.y);
	writestring(buf);
	Sleep(1500);
	GREEN;
	SetPoint = ReturnTextCenterInRect(buf, HistoryBoxRect);
	setcursor(SetPoint.x, SetPoint.y);
	writestring(buf);
	Sleep(100);
	ClearRect(HistoryBoxRect);
	//PRINT OUR FIRST HISTORY SEGMENT HERE.
	InsertHistoryString(player, "dsa telemetry test - communications grid 5x5\nall systems green", false);
	SetPoint.x = HistoryBoxRect.left;
	SetPoint.y = HistoryBoxRect.top;
	SaveHistorySnapShot(SetPoint, MAINUI_HISTORYBOXWIDTH, MAINUI_HISTORYBOXHEIGHT);
	
	
	//ACTIVATE CHRONOGRAPH.
	sprintf(buf, "CHRONOGRAPH ACTIVE - INGRESS NOW");
	RED;
	SetPoint = ReturnTextCenterInRect(buf, TimeBoxRect);
	setcursor(SetPoint.x, SetPoint.y);
	writestring(buf);
	Sleep(100);
	LRED;
	setcursor(SetPoint.x, SetPoint.y);
	writestring(buf);
	Sleep(1500);
	RED;
	SetPoint = ReturnTextCenterInRect(buf, TimeBoxRect);
	setcursor(SetPoint.x, SetPoint.y);
	writestring(buf);
	Sleep(100);
	InsertHistoryString(player, "agent ingress - mission commences", true);
	MainUIPrintChronograph(player, TimeBoxRect);
	
}

void PlayerSetTimeToRecovery(PPLAYER player, PMISSIONCHRONO timetorecovery)
{
	player->currentmissioninfo.timetorecovery = *timetorecovery;
	
	//THEY'LL WAIT ANYWHERE FROM 8 - 15 MINUTES.
	player->currentmissioninfo.timetoleftbehind.hours = 0;
	//player->currentmissioninfo.timetoleftbehind.minutes = rand()%8 + 8;
	player->currentmissioninfo.timetoleftbehind.minutes = 1;
	player->currentmissioninfo.timetoleftbehind.seconds = 0;
	player->currentmissioninfo.timetoleftbehind.split = 0;
}

//POINT CellPointToDrawPoint(PMAP map, POINT MapViewOrigin, POINT DrawPointOrigin, POINT CellPoint, int ViewportHeight, int ViewportWidth)
//{
//}

//POINT DrawPointToCellPoint(PMAP map, POINT MapViewOrigin, POINT DrawPointOrigin, POINT DrawPoint, int ViewportHeight, int ViewportWidth
//{
//}


int MainUILoop(PPLAYER player, PMAP map)
{
#define MAIN_UI_SLEEP 50
#define GUARDSLEEP		700
#define MODEBLINKSLEEP	30
#define CURSORBLINKSLEEP	20
#define ENVIRONSLEEP	200
	
	RECT TimeBoxRect;
	TRACKERARROWS Arrows;
	int numevents;
	USHORT key;
	int i;
	int GuardTimer = 0;
	int ModeBlinkTimer = 0;
	int CursorBlinkTimer = 0;
	int EnvironmentalTimer = 0;
	TIMER timeone;
	bool AITurn = false;
	bool ModeBlink = true;
	bool CursorBlink = false;
	
	TimeBoxRect.left = MAINUI_TIMEBOXXORIGIN;
	TimeBoxRect.top = MAINUI_TIMEBOXYORIGIN;
	TimeBoxRect.right = TimeBoxRect.left + MAINUI_TIMEBOXWIDTH;
	TimeBoxRect.bottom = TimeBoxRect.top + MAINUI_TIMEBOXHEIGHT;
	
	while(1)
	{
		
		StartTimer(&timeone); ///////////////TIMER HAS BEGUN! WATCH THE CLOCK!!////////////////
		Sleep(MAIN_UI_SLEEP);
		numevents = checkforinput(); ///////////CHECKING FOR INPUT////////////
		EndTimer(&timeone); //////////////TIMER HAS ENDED///////////////
		if(MainUIHandleTimeDecrement(player, map, SplitElapsed(&timeone))){
			DisplayUIMap(player, map, false, false);
			Arrows = EvaluatePlayerMoveDirections(player, map);
			DrawTrackerArrows(&Arrows);
		}
		
		//////////////TIME SIGNAL PROCESSING//////////////
		GuardTimer += SplitElapsed(&timeone);
		if(GuardTimer >= GUARDSLEEP){
			AITurn = true;
			GuardTimer = 0;
			//GUARDS MOVE?
		}

		EnvironmentalTimer += SplitElapsed(&timeone);
		if(EnvironmentalTimer >= ENVIRONSLEEP){
			EnvironmentalTimer = 0;
			UpdateEnvironmentalEffects(map, player);
			DisplayUIMap(player, map, false, false);
		}
		
		ModeBlinkTimer += SplitElapsed(&timeone);
		if(ModeBlinkTimer >= MODEBLINKSLEEP){
			ModeBlinkTimer = 0;
			//BLINK MODE
			if(player->CurrentActionMode != ACTION_MODE_MOVE){
				bToggle(&ModeBlink);
				MainUIPrintCurrentAction(player, ModeBlink);
			}
		}
		
		CursorBlinkTimer += SplitElapsed(&timeone);
		if(CursorBlinkTimer >= CURSORBLINKSLEEP){
			CursorBlinkTimer = 0;
			//BLINK MODE
			if(ActionModeNeedsCursor(player->CurrentActionMode)){
				bToggle(&CursorBlink);
				DisplayUIMap(player, map, false, CursorBlink);
			}
		}
		
		
		//////////////INPUT PROCESSING///////////////////
		if (numevents > 1)
		{
			for (i = 1; i <= numevents; i++)
			{
				key = getinput(i);
				
				switch(key)
				{
				case VK_ESCAPE:
					
					//MAIN GAME OPTIONS HERE.
					FLUSH;
					clearinputrecords();
					return 0;
					
				case VK_RETURN:
					FLUSH;
					clearinputrecords();
					if(player->CurrentActionMode == ACTION_MODE_USE){
						MainUIPlayerUseObject(player, map);
					}
					if(player->CurrentActionMode == ACTION_MODE_THROW){
						MainUIPlayerThrowObject(player, map);
						player->CurrentActionMode = ACTION_MODE_MOVE;
						MainUIPrintPlayerStatus(player);
					}
					break;
				case 'A': ////////////////DEBUG
					FLUSH;
					clearinputrecords();
					PlantAtomicBomb(player, map);
					MainUIPrintPlayerStatus(player);
					break;
				case 'T': //TURN MODE
					player->CurrentActionMode = ACTION_MODE_TURN;
					player->ZoomCenter.x = player->location->xcoord;
					player->ZoomCenter.y = player->location->ycoord;
					player->LocalCursor.x = player->location->xcoord;
					player->LocalCursor.y = player->location->ycoord;
					MainUIPrintCurrentAction(player, ModeBlink);
					memset(&Arrows, 0, sizeof(Arrows));
					Arrows.TrackerArrows[0] = true;
					Arrows.TrackerArrows[2] = true;
					Arrows.TrackerArrows[4] = true;
					Arrows.TrackerArrows[6] = true;
					DisplayUIMap(player, map, false, false);
					DrawTrackerArrows(&Arrows);
					FLUSH;
					clearinputrecords();
					break;
				case 'B': ////////////DEBUG!!!!!!!!!!
					SetBomb(player, map, player->location, &player->p_gizmos->head->object.gizmo);
					break;
				case 'R':
					MainRadio(player, map);
					break;
				case 'M': //MOVE MODE
					player->CurrentActionMode = ACTION_MODE_MOVE;
					player->ZoomCenter.x = player->location->xcoord;
					player->ZoomCenter.y = player->location->ycoord;
					player->LocalCursor.x = player->location->xcoord;
					player->LocalCursor.y = player->location->ycoord;
					MainUIPrintCurrentAction(player, true);
					DisplayUIMap(player, map, false, false);
					Arrows = EvaluatePlayerMoveDirections(player, map);
					DrawTrackerArrows(&Arrows);
					FLUSH;
					clearinputrecords();
					break;
				case 'I': //INVENTORY SYSTEM
					FLUSH;
					clearinputrecords();
					SaveMainSnapShot();
					MainInventorySelection(player, map);
					RestoreMainSnapShot();
					MainUIPrintPlayerStatus(player);
					DisplayUIMap(player, map, false, false);
					break;
				case 'U': //USE MODE
					player->CurrentActionMode = ACTION_MODE_USE;
					player->ZoomCenter.x = player->location->xcoord;
					player->ZoomCenter.y = player->location->ycoord;
					player->LocalCursor.x = player->location->xcoord;
					player->LocalCursor.y = player->location->ycoord;
					DisplayUIMap(player, map, false, false);
					Arrows = EvaluatePlayerMoveDirections(player, map);
					DrawTrackerArrows(&Arrows);
					MainUIPrintCurrentAction(player, ModeBlink);
					FLUSH;
					clearinputrecords();
					break;
				case 'H': //THROW MODE
					if(!HasThrowableWeapon(player)){
						break;
					}
					player->CurrentActionMode = ACTION_MODE_THROW;
					player->LocalCursor.x = player->ZoomCenter.x;
					player->LocalCursor.y = player->ZoomCenter.y;
					DisplayUIMap(player, map, false, false);
					Arrows = EvaluatePlayerMoveDirections(player, map);
					DrawTrackerArrows(&Arrows);
					MainUIPrintCurrentAction(player, ModeBlink);
					FLUSH;
					clearinputrecords();
					break;
				case 'Z':
					player->CurrentActionMode = ACTION_MODE_ZOOM;
					player->ZoomCenter.x = player->location->xcoord;
					player->ZoomCenter.y = player->location->ycoord;
					DisplayUIMap(player, map, false, false);
					Arrows = EvaluatePlayerMoveDirections(player, map);
					DrawTrackerArrows(&Arrows);
					MainUIPrintCurrentAction(player, ModeBlink);
					FLUSH;
					clearinputrecords();
					break;
				case 'P':
					
					if(!player->currentmissioninfo.primarycomplete)
					{
						player->currentmissioninfo.primarycomplete = true;
						if(player->currentmissioninfo.missiontype == MISSION_TYPE_CAUTERIZATION){
							LoopMainSong(player);
						}
						MainUIPrintPlayerStatus(player);
						InsertHistoryString(player, "DEBUG - PRIMARY SET TO COMPLETE", false);
					}
					else{
						player->currentmissioninfo.primarycomplete = false;
						if(player->currentmissioninfo.missiontype == MISSION_TYPE_CAUTERIZATION){
							LoopMainSong(player);
						}
						MainUIPrintPlayerStatus(player);
						InsertHistoryString(player, "DEBUG - PRIMARY SET TO INCOMPLETE", false);
					}
					break;
				case VK_F1:
					MainUIHelp();		
					break;
				case VK_NUMPAD1:
				case VK_NUMPAD2:
				case VK_NUMPAD3:
				case VK_NUMPAD4:
				case VK_NUMPAD5:
				case VK_NUMPAD6:
				case VK_NUMPAD7:
				case VK_NUMPAD8:
				case VK_NUMPAD9:
					if(MainUIHandleArrowKey(key, player, map))
					{
						AITurn = true;
					}
					break;
				}
				FLUSH;
				clearinputrecords();
				
			}
		}
		
		//PRINTING AND STATUS CRAP HERE.
		
		MainUIPrintChronograph(player, TimeBoxRect);
		
		
		if(AITurn)
		{
			CheckProximityBombs(map, player);
			
			DisplayUIMap(player, map, false, false);
			AITurn = false;
		}
	}///////LOOP END
	
	
}

TRACKERARROWS EvaluatePlayerMoveDirections(PPLAYER player, PMAP map)
{
	
	TRACKERARROWS Arrows;
	POINT ArrowPoints[8];
	PMAPCELL Cell;
	ArrowPoints[0].x = player->ZoomCenter.x; //NORTH
	ArrowPoints[0].y = player->ZoomCenter.y - 1;
	ArrowPoints[1].x = player->ZoomCenter.x + 1; //NE
	ArrowPoints[1].y = player->ZoomCenter.y - 1;
	ArrowPoints[2].x = player->ZoomCenter.x + 1; //E
	ArrowPoints[2].y = player->ZoomCenter.y;
	ArrowPoints[3].x = player->ZoomCenter.x + 1; //SE
	ArrowPoints[3].y = player->ZoomCenter.y + 1;
	ArrowPoints[4].x = player->ZoomCenter.x; //S
	ArrowPoints[4].y = player->ZoomCenter.y + 1;
	ArrowPoints[5].x = player->ZoomCenter.x -1; //SW
	ArrowPoints[5].y = player->ZoomCenter.y + 1;
	ArrowPoints[6].x = player->ZoomCenter.x - 1; //W
	ArrowPoints[6].y = player->ZoomCenter.y;
	ArrowPoints[7].x = player->ZoomCenter.x - 1; //NW
	ArrowPoints[7].y = player->ZoomCenter.y - 1;
	
	for(int i = 0; i < 8; i++)
	{
		Cell = FindMapCellByCoordinate(map, ArrowPoints[i]);
		if(!Cell)
		{
			Arrows.TrackerArrows[i] = false;
		}
		else{
			if(DoesCellBlockMovement(Cell))
			{
				Arrows.TrackerArrows[i] = false;
			}
			else{
				Arrows.TrackerArrows[i] = true;
			}
		}
	}
	return Arrows;
}

bool ActionModeNeedsCursor(int ActionMode)
{
	switch(ActionMode)
	{
	case ACTION_MODE_MOVE:
	case ACTION_MODE_TURN:
	case ACTION_MODE_ZOOM:
	default:
		return false;
	case ACTION_MODE_USE:
	case ACTION_MODE_ZOOMSHOOT:
	case ACTION_MODE_THROW:
		return true;
	}
}

void DisplayUIMap(PPLAYER player, PMAP map, bool redrawoutline, bool drawcursor)
{
	//CHECK THE CURRENT TURN DIRECTION.
	int boxWidth;
	int boxHeight;
	
	int boxXCenter;
	int boxYCenter;
	char buf[500];
	//char tempbuf[500];
	POINT viewPoint;
	POINT cursorPoint = player->LocalCursor;
	
	boxHeight = MAINUI_VIEWPORT_SHORTDISTANCE;
	boxWidth = MAINUI_VIEWPORT_LONGDISTANCE;
	
	if(player->turndirection == TURN_DIRECTION_NORTH)
	{
		
		
		viewPoint.x = player->ZoomCenter.x;
		viewPoint.y = player->ZoomCenter.y - ((boxHeight / 2) - 1);
	}
	
	if(player->turndirection == TURN_DIRECTION_SOUTH)
	{
		
		
		viewPoint.x = player->ZoomCenter.x;
		viewPoint.y = player->ZoomCenter.y + ((boxHeight / 2) - 1);
	}
	
	if(player->turndirection == TURN_DIRECTION_WEST)
	{
		
		viewPoint.x = player->ZoomCenter.x - ((boxWidth / 2) - 1);
		viewPoint.y = player->ZoomCenter.y;
	}
	
	if(player->turndirection == TURN_DIRECTION_EAST)
	{
		
		viewPoint.x = player->ZoomCenter.x + ((boxWidth / 2) - 1);
		viewPoint.y = player->ZoomCenter.y;
	}
	
	boxXCenter = (MAINUI_MAPBOXWIDTH / 2) - ((boxWidth + 2) / 2) + (MAINUI_MAPBOXXORIGIN);
	boxYCenter = (MAINUI_MAPBOXHEIGHT / 2) - ((boxHeight + 2) / 2) + (MAINUI_MAPBOXYORIGIN) + 1; //LEAVE SPACE FOR TEXT
	
	//DRAW RING AROUND BOX, DRAW MAP 1x1 CELL IN
	POINT BoxCenter;
	BoxCenter.x = boxXCenter;
	BoxCenter.y = boxYCenter;
	if(redrawoutline)
	{
		RECT MapRect;
		MapRect.left = 0;
		MapRect.right = boxWidth + 2;
		MapRect.top = 0;
		MapRect.bottom = boxHeight + 2;
		
		
		
		DrawRect(MapRect, BoxCenter, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
	
	BoxCenter.x++;
	BoxCenter.y++;
	DrawMapViewport(player, map, BoxCenter, boxWidth, boxHeight, viewPoint, cursorPoint, drawcursor);
	
	//DRAW COORDINATES.
	LBLUE;
	RECT StatRect;
	StatRect.left = MAINUI_MAPBOXXORIGIN;
	StatRect.right = MAINUI_MAPBOXXORIGIN + MAINUI_MAPBOXWIDTH;
	StatRect.top = MAINUI_MAPBOXHEIGHT + MAINUI_MAPBOXYORIGIN;
	StatRect.bottom = StatRect.top;
	ClearRect(StatRect);
	setcursor(MAINUI_MAPBOXXORIGIN, MAINUI_MAPBOXHEIGHT + MAINUI_MAPBOXYORIGIN);
	sprintf(buf, "(%d/%d) - ", player->ZoomCenter.x, player->ZoomCenter.y);
	switch(player->turndirection)
	{
	case TURN_DIRECTION_NORTH:
		strcat(buf, "Facing North");
		break;
	case TURN_DIRECTION_SOUTH:
		strcat(buf, "Facing South");
		break;
	case TURN_DIRECTION_EAST:
		strcat(buf, "Facing East");
		break;
	default:
		strcat(buf, "Facing West");
		break;
	}
	writestring(buf);
}

void MainUIHelp()
{
	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 1, TRUE);
	StaticDialogBox("Information", "Current Keys:\n(M) - Move Mode\n(T) - Turn Mode\n(Z) - Zoom Mode\n(U) - Use Mode\n(ENTER) - Use Object\n(R) - Radio\n(ESC) - DEBUG: Quit\nPress ENTER", "Network", &globals.dboxparams);
}

void MainUIPrintCurrentAction(PPLAYER player, bool On)
{
	char buf[500];
	POINT DrawCoord;
	POINT LocOne;
	POINT LocTwo;
	DrawCoord.y = (MAINUI_MAPBOXHEIGHT / 2)+ (MAINUI_MAPBOXYORIGIN) - (MAINUI_VIEWPORT_SHORTDISTANCE / 2) - 2;
	
	switch(player->CurrentActionMode)
	{
	case ACTION_MODE_MOVE:
		strcpy(buf, "MOVE MODE");
		break;
	case ACTION_MODE_TURN:
		strcpy(buf, "TURN MODE");
		break;
	case ACTION_MODE_ZOOM:
		//////FIGURE OUT ZOOM BY DISTANCE BETWEEN PLAYER AND ZOOM CENTER.
		LocOne.x = player->location->xcoord;
		LocOne.y = player->location->ycoord;
		LocTwo = player->ZoomCenter;
		sprintf(buf, "ZOOM %2.2fX", ReturnZoomFactor(LocOne, LocTwo, MAINUI_VIEWPORT_LONGDISTANCE, MAINUI_VIEWPORT_SHORTDISTANCE));
		break;
	case ACTION_MODE_ZOOMSHOOT:
		LocOne.x = player->location->xcoord;
		LocOne.y = player->location->ycoord;
		LocTwo = player->ZoomCenter;
		if(ReturnZoomFactor(LocOne, LocTwo, MAINUI_VIEWPORT_LONGDISTANCE, MAINUI_VIEWPORT_SHORTDISTANCE) > 4.0){
			sprintf(buf, "SNIPE %2.2fX ZOOM", ReturnZoomFactor(LocOne, LocTwo, MAINUI_VIEWPORT_LONGDISTANCE, MAINUI_VIEWPORT_SHORTDISTANCE));
		}
		else{
			sprintf(buf, "SHOOT %2.2fX ZOOM", ReturnZoomFactor(LocOne, LocTwo, MAINUI_VIEWPORT_LONGDISTANCE, MAINUI_VIEWPORT_SHORTDISTANCE));
		}
	case ACTION_MODE_USE:
		strcpy(buf, "USE MODE");
		break;
	case ACTION_MODE_THROW:
		strcpy(buf, "THROW MODE");
		break;
	default:
		strcpy(buf, "UNKNOWN");
	}
	
	if(On){
		LRED;
	}
	else{
		DGRAY;
	}
	
	MainUIClearActionString();
	RECT MapBoxRect;
	POINT SetPoint;
	MapBoxRect.left = MAINUI_MAPBOXXORIGIN;
	MapBoxRect.right = MapBoxRect.left + MAINUI_MAPBOXWIDTH;
	MapBoxRect.top = MAINUI_MAPBOXYORIGIN;
	MapBoxRect.bottom = MapBoxRect.top + MAINUI_MAPBOXHEIGHT;
	
	
	SetPoint = ReturnTextCenterInRect(buf, MapBoxRect);
	setcursor(SetPoint.x, DrawCoord.y);
	writestring(buf);
}

void MainUIClearActionString()
{
	RECT MapBoxRect;
	MapBoxRect.left = MAINUI_MAPBOXXORIGIN;
	MapBoxRect.right = MapBoxRect.left + MAINUI_MAPBOXWIDTH;
	MapBoxRect.top = MAINUI_MAPBOXYORIGIN;
	MapBoxRect.bottom = MapBoxRect.top;
	ClearRect(MapBoxRect, 0);
}

void DrawTrackerArrows(PTRACKERARROWS Arrows)
{
	char TrackChars[8];
	POINT ArrowPoints[8];
	int LeftSide = (MAINUI_MAPBOXWIDTH / 2) - ((MAINUI_VIEWPORT_LONGDISTANCE) / 2) + (MAINUI_MAPBOXXORIGIN) - 1;
	int RightSide = LeftSide + MAINUI_VIEWPORT_LONGDISTANCE + 1;
	int TopSide = (MAINUI_MAPBOXHEIGHT / 2)+ (MAINUI_MAPBOXYORIGIN) - (MAINUI_VIEWPORT_SHORTDISTANCE / 2);
	int BottomSide = TopSide + MAINUI_VIEWPORT_SHORTDISTANCE + 1;
	int HorizMiddle = (MAINUI_MAPBOXWIDTH / 2) + (MAINUI_MAPBOXXORIGIN);
	int VertMiddle = (MAINUI_MAPBOXHEIGHT / 2) + (MAINUI_MAPBOXYORIGIN) + 1;
	
	TrackChars[0] = '|';
	TrackChars[1] = '/';
	TrackChars[2] = '-';
	TrackChars[3] = '\\';
	TrackChars[4] = '|';
	TrackChars[5] = '/';
	TrackChars[6] = '-';
	TrackChars[7] = '\\';
	
	ArrowPoints[0].x = HorizMiddle;
	ArrowPoints[0].y = TopSide;
	ArrowPoints[1].x = RightSide;
	ArrowPoints[1].y = TopSide;
	ArrowPoints[2].x = RightSide;
	ArrowPoints[2].y = VertMiddle;
	ArrowPoints[3].x = RightSide;
	ArrowPoints[3].y = BottomSide;
	ArrowPoints[4].x = HorizMiddle;
	ArrowPoints[4].y = BottomSide;
	ArrowPoints[5].x = LeftSide;
	ArrowPoints[5].y = BottomSide;
	ArrowPoints[6].x = LeftSide;
	ArrowPoints[6].y = VertMiddle;
	ArrowPoints[7].x = LeftSide;
	ArrowPoints[7].y = TopSide;
	
	for(int i = 0; i < 8; i++)
	{
		setcursor(ArrowPoints[i].x, ArrowPoints[i].y);
		if(Arrows->TrackerArrows[i]){
			LYELLOW;
		}
		else{
			DGRAY;
		}
		writechar(TrackChars[i]);
	}
}



bool MainUIHandleArrowKey(USHORT vkey, PPLAYER player, PMAP map) //RETURNS WHETHER OR NOT IT WAS A VALID TURN.
{
	int vmovement = 0;
	int hmovement = 0;
	//MOVING
	if(player->CurrentActionMode == ACTION_MODE_MOVE || player->CurrentActionMode == ACTION_MODE_ZOOM)
	{
		
		
		switch(vkey)
		{
		case VK_NUMPAD8:
			vmovement = -1;
			break;
		case VK_NUMPAD4:
			hmovement = -1;
			break;
		case VK_NUMPAD2:
			vmovement = 1;
			break;
		case VK_NUMPAD6:
			hmovement = 1;
			break;
		case VK_NUMPAD1:
			hmovement = -1;
			vmovement = 1;
			break;
		case VK_NUMPAD3:
			hmovement = 1;
			vmovement = 1;
			break;
		case VK_NUMPAD9:
			hmovement = 1;
			vmovement = -1;
			break;
		case VK_NUMPAD7:
			hmovement = -1;
			vmovement = -1;
			break;
		default:
			return false;
		}
		POINT NewPoint;
		if(player->CurrentActionMode == ACTION_MODE_MOVE){
			NewPoint.x = player->location->xcoord + hmovement;
			NewPoint.y = player->location->ycoord + vmovement;
		}
		else{
			NewPoint.x = player->ZoomCenter.x + hmovement;
			NewPoint.y = player->ZoomCenter.y + vmovement;
		}
		PMAPCELL Cell = FindMapCellByCoordinate(map, NewPoint);
		if(!Cell){
			return false;
		}
		else if(DoesCellBlockMovement(Cell) && player->CurrentActionMode == ACTION_MODE_MOVE){
			return false;
		}
		else if(DoAnyGizmosInterfereWithMovement(player->p_gizmos) && player->CurrentActionMode == ACTION_MODE_MOVE){
			SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
			StaticDialogBox("Gizmo Active", "An activated Gizmo is interfering with your movement. If you wish to move, deactivate the gizmo.", "Press ENTER", &globals.dboxparams);
			return false;
		}
		
		else if(player->CurrentActionMode == ACTION_MODE_ZOOM){
			POINT Loc;
			Loc.x = player->location->xcoord;
			Loc.y = player->location->ycoord;
			if(ReturnZoomFactor(Loc, NewPoint, MAINUI_VIEWPORT_LONGDISTANCE, MAINUI_VIEWPORT_SHORTDISTANCE) > GetPlayerMaxZoom(player)){
				return false;
			}
			if(!IsZoomValid(player, map, NewPoint)){
				return false;
			}
			PlayAppropriateZoomSound(player);
		}
		
		if(player->CurrentActionMode == ACTION_MODE_MOVE){
			player->lastlocation = player->location;
			player->location = Cell;
		}
		player->ZoomCenter.x = Cell->xcoord;
		player->ZoomCenter.y = Cell->ycoord;
		player->LocalCursor.x = Cell->xcoord;
		player->LocalCursor.y = Cell->ycoord;
		WHITE;
		DisplayUIMap(player, map, false, false);
		TRACKERARROWS Arrows = EvaluatePlayerMoveDirections(player, map);
		DrawTrackerArrows(&Arrows);
		if(player->CurrentActionMode == ACTION_MODE_ZOOM || player->CurrentActionMode == ACTION_MODE_USE || player->CurrentActionMode == ACTION_MODE_SHOOT || player->CurrentActionMode == ACTION_MODE_ZOOMSHOOT){
			return false;
		}
		else{
		return true;
		}
		
	}
	
	///TURNING
	else if (player->CurrentActionMode == ACTION_MODE_TURN)
	{
		int NewDirection;
		switch(vkey)
		{
		case VK_NUMPAD8:
			NewDirection = TURN_DIRECTION_NORTH;
			break;
		case VK_NUMPAD4:
			NewDirection = TURN_DIRECTION_WEST;
			break;
		case VK_NUMPAD2:
			NewDirection = TURN_DIRECTION_SOUTH;
			break;
		case VK_NUMPAD6:
			NewDirection = TURN_DIRECTION_EAST;
			break;
		default:
			return false;
		}
		if(NewDirection != player->turndirection)
		{
			player->turndirection = NewDirection;
			player->CurrentActionMode = ACTION_MODE_MOVE;
			player->LocalCursor = player->ZoomCenter;
			MainUIPrintCurrentAction(player, true);
			WHITE;
			DisplayUIMap(player, map, false, false);
			TRACKERARROWS Arrows = EvaluatePlayerMoveDirections(player, map);
			DrawTrackerArrows(&Arrows);
			return true;
		}
		return false;
	}
	
	else if (ActionModeNeedsCursor(player->CurrentActionMode)){
		switch(vkey)
		{
		case VK_NUMPAD8:
			vmovement = -1;
			break;
		case VK_NUMPAD4:
			hmovement = -1;
			break;
		case VK_NUMPAD2:
			vmovement = 1;
			break;
		case VK_NUMPAD6:
			hmovement = 1;
			break;
		case VK_NUMPAD1:
			hmovement = -1;
			vmovement = 1;
			break;
		case VK_NUMPAD3:
			hmovement = 1;
			vmovement = 1;
			break;
		case VK_NUMPAD9:
			hmovement = 1;
			vmovement = -1;
			break;
		case VK_NUMPAD7:
			hmovement = -1;
			vmovement = -1;
			break;
		default:
			return false;
		}
		
		POINT viewPoint;
		if(player->turndirection == TURN_DIRECTION_NORTH)
		{
			
			
			viewPoint.x = player->ZoomCenter.x;
			viewPoint.y = player->ZoomCenter.y - ((MAINUI_VIEWPORT_SHORTDISTANCE / 2) - 1);
		}
		
		if(player->turndirection == TURN_DIRECTION_SOUTH)
		{
			
			
			viewPoint.x = player->ZoomCenter.x;
			viewPoint.y = player->ZoomCenter.y + ((MAINUI_VIEWPORT_SHORTDISTANCE / 2) - 1);
		}
		
		if(player->turndirection == TURN_DIRECTION_WEST)
		{
			
			viewPoint.x = player->ZoomCenter.x - ((MAINUI_VIEWPORT_LONGDISTANCE / 2) - 1);
			viewPoint.y = player->ZoomCenter.y;
		}
		
		if(player->turndirection == TURN_DIRECTION_EAST)
		{
			
			viewPoint.x = player->ZoomCenter.x + ((MAINUI_VIEWPORT_LONGDISTANCE / 2) - 1);
			viewPoint.y = player->ZoomCenter.y;
		}
		
		if(IsCellInVisibleBounds(map, MAINUI_VIEWPORT_LONGDISTANCE, MAINUI_VIEWPORT_SHORTDISTANCE, viewPoint, player->LocalCursor.x + hmovement, player->LocalCursor.y + vmovement)){
			player->LocalCursor.x += hmovement;
			player->LocalCursor.y += vmovement;
			return false;
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}
}

int MainUIPlayerUseObject(PPLAYER player, PMAP map)
{
	POINT PlayerLoc;
	PlayerLoc.x = player->location->xcoord;
	PlayerLoc.y = player->location->ycoord;
	//WELL, THIS IS A BIG DEAL.
	//FIRST, YOU MAY USE NO OBJECT THAT'S MORE THAN ONE SQUARE AWAY FROM YOU.
	if(AbsoluteDistance(player->LocalCursor, PlayerLoc) > 1.5)
	{
		return -1; //TOO FAR AWAY.
	}
	/////////////////DEBUG: WE'LL JUST EXAMINE DOORS FOR NOW.
	PMAPCELL Cell = FindMapCellByCoordinate(map, player->LocalCursor);
	if(!Cell){
		return -2; //NO CELL!
	}
	
	//CHECK FOR DOORS FIRST!
	PDOOR Door = GetDoorInfoFromCell(Cell);
	if(Door && !Door->isopen){
		PrintDoorInfo(player, Door);
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		playMIXsound(DOOR_KEYCARD_ACTIVE_SOUND);
		StaticDialogBox("The Day", "But because you're l33t, you just open the door and walk in.", "Is YOURS!", &globals.dboxparams);
		OpenDoor(map, Door);
		return 0;
	}
	
	PMAPARMORY Armory = GetMapCellArmory(Cell);
	if(Armory)
	{
		RaidArmory(player, Armory);
	}
	return 0; //OKAY.
}

int MainUIPlayerThrowObject(PPLAYER player, PMAP map)
{
	POINT HitPoint;
	
	POINT PlayerLoc;
	PlayerLoc.x = player->location->xcoord;
	PlayerLoc.y = player->location->ycoord;
	//WE CAN THROW THREE THINGS.
	//1. GRENADE
	//2. SHARP OBJECT
	//3. BLUNT OBJECT
	bool Grenade = false;
	bool SharpObject = false;
	if(player->currentweapon){
		if(player->currentweapon->attacktype & WEAP_ATT_TYPE_CUT)
			SharpObject = true;
		else
		SharpObject = false;
	}
	else if(AreThereActiveGrenadesInInventory(player)){
		Grenade = true;
	}
	else{
		return -1; //CRAP
	}
	int retcode = ThrowObjectGetFinalLandingPoint(player, map, &HitPoint);
	PMAPCELL Target = FindMapCellByCoordinate(map, HitPoint);
	if(!Target)
		return -1; //CRAP
	if(!retcode) //WE'RE OK.
	{
		if(Grenade)
		{
			PGIZMO Gren = GetActivePlayerGrenade(player);
			if(!Gren)
				return -1;
			PlayThrowSequence(player, map, player->location, Target, (int)AbsoluteDistance(PlayerLoc, HitPoint), GRENADE_BOUNCE_SOUND);
			SpawnLiveGrenade(HitPoint, Gren->unit.grenade.grenadetype, Gren->unit.grenade.primertime);
			DeletePlayerActiveGrenade(player);
		}
	}
	return 0;
}

int ThrowObjectGetFinalLandingPoint(PPLAYER player, PMAP map, POINT* Out)
{
		POINT PlayerLoc, Target, EnRoute;
	PlayerLoc.x = player->location->xcoord;
	PlayerLoc.y = player->location->ycoord;
	Target = player->LocalCursor;
	int Distance = (int)AbsoluteDistance(Target, PlayerLoc);
	int MaxDistance = MAX_PLAYER_THROW_DISTANCE;
	int MaxDistanceOff = (int)MAX_PLAYER_THROW_OFF;
	int AggRef = player->psychskill.psych.agression + player->psychskill.psych.reflex;
	double Weight;
	if(!player->currentweapon){
		Weight = GetActiveGrenadeWeight(player);
	}
	else{
		Weight = player->currentweapon->weight;
	}
	//CHECK THE DISTANCE FOR PLAYER THROWING ABILITY.
	double WeightRatio = ( Weight / MAX_PLAYER_THROW_WEIGHT);
	double PsychRatio = (double)AggRef / (double)(MAX_PSYCH * 2);
	MaxDistance -= (int)(MAX_PLAYER_THROW_WEAP_FALLOFF_WEIGHT * WeightRatio);
	MaxDistance -= (int)(MAX_PLAYER_THROW_WEAP_FALLOFF_PSYCH  * PsychRatio);
	MaxDistance = CapInclusive(MaxDistance, 1, MAX_PLAYER_THROW_DISTANCE);
	//AGGRESSION & REFLEX CAN ADD UP TO CREATE A MAX SCALED DOWN BY WEIGHT.
	
	//player->psychskill.specializations.handweapons
	//PUSH THE TARGET AROUND DUE TO STEALTH COMBAT INABILITY AND HAND WEAPON SPEC.
	PsychRatio = (double)player->psychskill.skills.stealthcombat / (double)MAX_SKILL;
	MaxDistanceOff -= (int)((MAX_PLAYER_THROW_OFF / 2.0) * PsychRatio);
	MaxDistanceOff -= (int)(PLAYER_THROW_SPECIAL_ACCURACY_BONUS * (double)player->psychskill.specializations.handweapons);
	MaxDistanceOff = CapInclusive(MaxDistanceOff, 0, (int)MAX_PLAYER_THROW_OFF);
	if(MaxDistance < Distance){
		Distance = MaxDistance;
	}
	int DescentPoint = Distance / 2;
	Target = GetPointAlongLine(player->location, FindMapCellByCoordinate(map, player->LocalCursor), Distance);
	for(int i = 0; i < MaxDistanceOff; i++){
		if(RandomPercentage(50)){
			Target.x++;
		if(DoesCellBlockMovement(FindMapCellByCoordinate(map, Target)))
			Target.x--;
		}
		else{
			Target.x--;
			if(DoesCellBlockMovement(FindMapCellByCoordinate(map, Target)))
			Target.x++;
		}

		if(RandomPercentage(50)){
			Target.y++;
			if(DoesCellBlockMovement(FindMapCellByCoordinate(map, Target)))
			Target.y--;
		}
		else{
			Target.y--;
			if(DoesCellBlockMovement(FindMapCellByCoordinate(map, Target)))
			Target.y++;
		}
	}
	if(Target.x < 0)
		Target.x = 0;
	if(Target.y < 0)
		Target.y = 0;
	if(Target.x >= map->width)
		Target.x = map->width - 1;
	if(Target.y >= map->height)
		Target.y = map->height - 1;

	PMAPCELL SrcCell = player->location;
	PMAPCELL TgtCell = FindMapCellByCoordinate(map, Target);

	//GRENADES HIT AND STOP WHERE THEY ARE.
	if(GetFirstObstructionBetweenPoints(map, SrcCell, TgtCell, &EnRoute) == RAYCAST_BLOCK_NONE){
		//WE HAVE A STRAIGHT SHOT.
		Out->x = TgtCell->xcoord;
		Out->y = TgtCell->ycoord;
		return 0;
	}
	else{
		//START FROM THE START POINT AND WORK FORWARD.
		POINT NextTarget;
		PMAPCELL CopySrc;
		int ObType;
		for(int Step = 0; Step < Distance; Step++){
			NextTarget = GetPointAlongLine(SrcCell, TgtCell, Step);
			CopySrc = FindMapCellByCoordinate(map, NextTarget);
			ObType = GetFirstObstructionBetweenPoints(map, CopySrc, TgtCell, &EnRoute);
			if(EnRoute.x == NextTarget.x && EnRoute.y == NextTarget.y){
				//THE OBSTACLE IS *HERE*.
				NextTarget = GetPointAlongLine(SrcCell, TgtCell, Step - 1);
				Out->x = NextTarget.x;
				Out->y = NextTarget.y;
				return 0;
			}
			//ELSE DO NOTHING.
		}
		Out->x = NextTarget.x;
		Out->y = NextTarget.y;
		return 0;
	}
}

void PlayThrowSequence(PPLAYER player, PMAP map, PMAPCELL Source, PMAPCELL Target, int Distance, char* Sound)
{
	POINT OldPlayerZoomCenter;
	POINT OldPlayerLocalCursor;
	POINT DestCenter;
	DestCenter.x = Source->xcoord;
	DestCenter.y = Source->ycoord;
	OldPlayerZoomCenter = player->ZoomCenter;
	OldPlayerLocalCursor = player->LocalCursor;
	int Step = 0;
	
	for(Step = 0; Step <= Distance; Step++){
	player->ZoomCenter = GetPointAlongLine(Source, Target, Step);
	player->LocalCursor = player->ZoomCenter;
	DisplayUIMap(player, map, false, true);
	Sleep(50);
	}
	if(Sound){
	playMIXsound(Sound);
	}
	for(int i = 0; i < 20; i++){
	DisplayUIMap(player, map, false, false);
	Sleep(20);
	DisplayUIMap(player, map, false, true);
	Sleep(20);
	}
	player->LocalCursor = OldPlayerLocalCursor;
	player->ZoomCenter = OldPlayerZoomCenter;
}

bool IsCellInVisibleBounds(PMAP map, int viewportwidth, int viewportheight, POINT viewcenterloc, int x, int y)
{
	POINT offset;
	
	
	offset.x = 0;
	offset.y = 0;
	
	POINT arraystart;
	RECT checkrect;
	//	PMAPCELL cell;
	bool right = false;
	bool bottom = false;
	int counter = 0;
	
	arraystart.x = viewcenterloc.x - (viewportwidth / 2);
	arraystart.y = viewcenterloc.y - (viewportheight /2);
	
	if(arraystart.x < 0){
		offset.x = arraystart.x;
	}
	else if (arraystart.x + viewportwidth >= map->width){
		offset.x = (arraystart.x + viewportwidth) - map->width;
	}
	
	if(arraystart.y < 0){
		offset.y = arraystart.y;
	}
	else if (arraystart.y + viewportheight >= map->height){
		offset.y = (arraystart.y + viewportheight) - map->height;
	}
	
	checkrect.left = arraystart.x;
	checkrect.top = arraystart.y;
	checkrect.right = checkrect.left + viewportwidth - offset.x;
	checkrect.bottom = checkrect.top + viewportheight - offset.y;
	
	if(x < checkrect.right && x >= checkrect.left && y < checkrect.bottom && y >= checkrect.top)
	{
		return true;
	}
	return false;
}


bool DoesCellBlockMovement(PMAPCELL Cell)
{
	
	if(!Cell)
	{
		return true;
	}
	
	if(GetMapCellVehicle(Cell)){
		return true;
	}
	
	switch(Cell->type)
	{
		//	case VEHICLE_SYM_LIVEVEHICLE:
		//	case VEHICLE_SYM_DEADVEHICLE:
	case BUILDING_SYM_BLCORNER:
	case BUILDING_SYM_TRCORNER:
	case BUILDING_SYM_BRCORNER:
	case BUILDING_SYM_TLCORNER:
	case BUILDING_SYM_HDOOR:
	case BUILDING_SYM_VDOOR:
	case BUILDING_SYM_VWALL:
	case BUILDING_SYM_HWALL:
	case OBJECT_SYM_FENCE:
	case OBJECT_SYM_LOOBSTRUCT:
	case OBJECT_SYM_HIOBSTRUCT:
		return true;
	default:
		return false;
	}
}

void LoopMainSong(PPLAYER player)
{
	if(player->currentmissioninfo.missiontype == MISSION_TYPE_CAUTERIZATION && player->currentmissioninfo.primarycomplete)
	{
		LoopSong(globals.musiclist.songs[ESCAPE_SONG]);
	}
	else if(player->currentmissioninfo.dynamicmode){
		LoopSong(globals.musiclist.songs[DYNAMIC_SONG]);
	}
	else{
		LoopSong(globals.musiclist.songs[STEALTH_SONG]);
	}
}

bool MainUIHandleTimeDecrement(PPLAYER player, PMAP map, int timediff)
{
	
	if(IsRecoveryTimeLeft(player)){
		
		player->currentmissioninfo.timetorecovery.split -= timediff;
		if(player->currentmissioninfo.timetorecovery.hours <= 0 && player->currentmissioninfo.timetorecovery.minutes <= 0 && player->currentmissioninfo.timetorecovery.seconds <= 0 && player->currentmissioninfo.timetorecovery.split <= 0 && player->currentmissioninfo.missiontype != MISSION_TYPE_CAUTERIZATION){
			RECT TimeBoxRect;
			TimeBoxRect.left = MAINUI_TIMEBOXXORIGIN;
			TimeBoxRect.top = MAINUI_TIMEBOXYORIGIN;
			TimeBoxRect.right = TimeBoxRect.left + MAINUI_TIMEBOXWIDTH;
			TimeBoxRect.bottom = TimeBoxRect.top + MAINUI_TIMEBOXHEIGHT;
			MainUIPrintChronograph(player, TimeBoxRect);
			WHITE;
			DisplayUIMap(player, map, false, false);
			InsertHistoryString(player, "recovery team arrival", true);
		}
		else{
			calibratemissionchrono(&player->currentmissioninfo.timetorecovery);
		}
	}
	else if (IsLeftBehindTimeLeft(player)){
		player->currentmissioninfo.timetoleftbehind.split -= timediff;
		if(player->currentmissioninfo.timetoleftbehind.hours <= 0 && player->currentmissioninfo.timetoleftbehind.minutes <= 0 && player->currentmissioninfo.timetoleftbehind.seconds <= 0 && player->currentmissioninfo.timetoleftbehind.split <= 0){
			RECT TimeBoxRect;
			TimeBoxRect.left = MAINUI_TIMEBOXXORIGIN;
			TimeBoxRect.top = MAINUI_TIMEBOXYORIGIN;
			TimeBoxRect.right = TimeBoxRect.left + MAINUI_TIMEBOXWIDTH;
			TimeBoxRect.bottom = TimeBoxRect.top + MAINUI_TIMEBOXHEIGHT;
			MainUIPrintChronograph(player, TimeBoxRect);
			WHITE;
			DisplayUIMap(player, map, false, false);
			InsertHistoryString(player, "recovery team departure. operative abandoned", true);
		}
		else{
			calibratemissionchrono(&player->currentmissioninfo.timetoleftbehind);
		}
	}
	
	globals.globalmapstruct.totaltime.split += timediff;
	calibratemissionchrono(&globals.globalmapstruct.totaltime);
	//CHECK FOR SCRIPTED EVENTS HERE!!!!!!!!!!!!!!!!!!!!!!!!!
	if(IsScriptedEventDue()){
		RunNextScriptedEvent(player, map);
		return true;
	}
	return false;
}

void MainUIPrintChronograph(PPLAYER player, RECT RectToDrawIn)
{
	
	char timebuf[500];
	
	
	
	ClearRect(RectToDrawIn);
	CHAR_INFO* Timechars;
	SMALL_RECT rect;
	rect.Bottom = (short)RectToDrawIn.bottom;
	rect.Top = (short)RectToDrawIn.top;
	rect.Right = (short)RectToDrawIn.right;
	rect.Left = (short)RectToDrawIn.left;
	
	int rectwidth = RectToDrawIn.right - RectToDrawIn.left;
	int rectheight = RectToDrawIn.bottom - RectToDrawIn.top;
	
	int rectsize = rectwidth * rectheight;
	if(!rectheight){
		rectsize = rectwidth;
	}
	
	Timechars = (CHAR_INFO*) malloc(sizeof(CHAR_INFO) * rectsize);
	memset(Timechars, 0, sizeof(Timechars));
	
	//IF THIS IS A CAUTERY MISSION....
	
	if(player->currentmissioninfo.missiontype == MISSION_TYPE_CAUTERIZATION && player->currentmissioninfo.primarycomplete)
	{
		if(player->currentmissioninfo.timetorecovery.hours > 0)
		{
			LGREEN;
			sprintf(timebuf, "DETONATION CLOCK: %2.2d:%2.2d:%2.2d", player->currentmissioninfo.timetorecovery.hours, player->currentmissioninfo.timetorecovery.minutes, player->currentmissioninfo.timetorecovery.seconds);
		}
		
		else if(player->currentmissioninfo.timetorecovery.minutes > 0 && player->currentmissioninfo.timetorecovery.seconds >= 0)
		{
			LYELLOW;
			sprintf(timebuf, "DETONATION CLOCK: %2.2d:%2.2d.%2.2d", player->currentmissioninfo.timetorecovery.minutes, player->currentmissioninfo.timetorecovery.seconds, player->currentmissioninfo.timetorecovery.split);
		}
		
		else if(player->currentmissioninfo.timetorecovery.seconds >= 0 && player->currentmissioninfo.timetorecovery.split >= 0)
		{
			LRED;
			sprintf(timebuf, "DETONATION IMMINENT: %2.2d.%2.2d seconds", player->currentmissioninfo.timetorecovery.seconds, player->currentmissioninfo.timetorecovery.split);
		}
		else{
			RED;
			sprintf(timebuf, "*DETONATION*");
		}
	}
	
	else
	{
		
		if(player->currentmissioninfo.timetorecovery.hours > 0)
		{
			LGREEN;
			sprintf(timebuf, "EXTRACTION CLOCK: %2.2d:%2.2d:%2.2d", player->currentmissioninfo.timetorecovery.hours, player->currentmissioninfo.timetorecovery.minutes, player->currentmissioninfo.timetorecovery.seconds);
		}
		
		else if(player->currentmissioninfo.timetorecovery.minutes > 0)
		{
			LYELLOW;
			sprintf(timebuf, "EXTRACTION CLOCK: %2.2d:%2.2d.%2.2d", player->currentmissioninfo.timetorecovery.minutes, player->currentmissioninfo.timetorecovery.seconds, player->currentmissioninfo.timetorecovery.split);
		}
		
		else if(player->currentmissioninfo.timetorecovery.seconds >= 0 && player->currentmissioninfo.timetorecovery.split >= 0)
		{
			LRED;
			sprintf(timebuf, "E-TEAM INBOUND: %2.2d.%2.2d seconds", player->currentmissioninfo.timetorecovery.seconds, player->currentmissioninfo.timetorecovery.split);
		}
		
		//ELSE RECOVERY TEAM HAS ALREADY ARRIVED...AND PERHAPS LEFT.
		
		else if (player->currentmissioninfo.timetoleftbehind.hours > 0)
		{
			LGREEN;
			sprintf(timebuf, "EGRESS CLOCK: %2.2d:%2.2d:%2.2d", player->currentmissioninfo.timetoleftbehind.hours, player->currentmissioninfo.timetoleftbehind.minutes, player->currentmissioninfo.timetoleftbehind.seconds);
		}
		
		else if(player->currentmissioninfo.timetoleftbehind.minutes > 0)
		{
			LYELLOW;
			sprintf(timebuf, "EGRESS CLOCK: %2.2d:%2.2d.%2.2d", player->currentmissioninfo.timetoleftbehind.minutes, player->currentmissioninfo.timetoleftbehind.seconds, player->currentmissioninfo.timetoleftbehind.split);
		}
		
		else if(player->currentmissioninfo.timetoleftbehind.seconds >= 0 && player->currentmissioninfo.timetoleftbehind.split >= 0)
		{
			LRED;
			sprintf(timebuf, "E-TEAM DEPARTING: %2.2d.%2.2d seconds", player->currentmissioninfo.timetoleftbehind.seconds, player->currentmissioninfo.timetoleftbehind.split);
		}
		
		else{
			RED;
			sprintf(timebuf, "*CHRONOGRAPH LINK SEVERED*");
		}
		
		
		
	}
	
	int len = strlen(timebuf);
	WORD color = getcolor();
	for(int i = 0; i < rectsize; i++)
	{
		Timechars[i].Attributes = color;
		Timechars[i].Char.AsciiChar = timebuf[i];
		if(i >= len)
		{
			Timechars[i].Attributes = 0;
			Timechars[i].Char.AsciiChar = 0;
		}
	}
	COORD bsize;
	bsize.X = rectsize;
	bsize.Y = 1;
	
	WriteConsoleOutput(getconsoleoutputh(), Timechars, bsize, globals.zeroed, &rect);
	
}

void InsertHistoryString(PPLAYER player, char* string, bool priority)
{
	char cursorchar = (char)219;
	int currentx = MAINUI_HISTORYBOXXORIGIN;
	int currenty = MAINUI_HISTORYBOXYORIGIN + MAINUI_HISTORYBOXHEIGHT;
	int counter = 0;
	char currentchar;
	POINT origin;
	origin.x = MAINUI_HISTORYBOXXORIGIN;
	origin.y = MAINUI_HISTORYBOXYORIGIN;
	
	if(!string){
		return;
	}
	char* ourstring = (char*)malloc(sizeof(char) * (strlen(string) + 20));
	
	if(IsRecoveryTimeLeft(player)){
		sprintf(ourstring, "-%2.2d:%2.2d:%2.2d ", player->currentmissioninfo.timetorecovery.hours, player->currentmissioninfo.timetorecovery.minutes, player->currentmissioninfo.timetorecovery.seconds);
	}
	else if(IsLeftBehindTimeLeft(player)){
		sprintf(ourstring, "+%2.2d:%2.2d:%2.2d ", player->currentmissioninfo.timetoleftbehind.hours, player->currentmissioninfo.timetoleftbehind.minutes, player->currentmissioninfo.timetoleftbehind.seconds);
	}
	else{
		sprintf(ourstring, "UNOFFICIAL- ");
	}
	
	strcat(ourstring, string);
	
	while((currentchar = ourstring[counter]) != '\0'){
		Sleep(10);
		if(currentchar == '\n'){
			currentx = MAINUI_HISTORYBOXXORIGIN;
			ScrollHistoryBox();
			currentx+=5;
			setcursor(currentx, currenty);
			if(priority){
				LRED;
			}
			else{
				LGREEN;
			}
			writechar(cursorchar);
		}
		else{
			if(priority){
				RED;
			}
			else{
				GREEN;
			}
			setcursor(currentx, currenty);
			writechar(currentchar);
			if(ourstring[counter+1] != '\n' && ourstring[counter+1] != '\0'){
				if(priority){
					LRED;
				}
				else{
					LGREEN;
				}
				setcursor(currentx+1, currenty);
				writechar(cursorchar);
				currentx++;
			}
		}
		counter++;
		
	}
	
	currentx = MAINUI_HISTORYBOXXORIGIN;
	ScrollHistoryBox();
	setcursor(currentx, currenty);
	LGREEN;
	writechar(cursorchar);
	free(ourstring);
	SaveHistorySnapShot(origin, MAINUI_HISTORYBOXWIDTH, MAINUI_HISTORYBOXHEIGHT);
}

bool IsRecoveryTimeLeft(PPLAYER player)
{
	if(player->currentmissioninfo.timetorecovery.hours > 0 || player->currentmissioninfo.timetorecovery.minutes > 0 || player->currentmissioninfo.timetorecovery.seconds >0 || player->currentmissioninfo.timetorecovery.split >= 0)
	{
		return true;
	}
	return false;
}

bool IsLeftBehindTimeLeft(PPLAYER player)
{
	if(player->currentmissioninfo.timetoleftbehind.hours > 0 || player->currentmissioninfo.timetoleftbehind.minutes > 0 || player->currentmissioninfo.timetoleftbehind.seconds >0 || player->currentmissioninfo.timetoleftbehind.split >= 0)
	{
		return true;
	}
	return false;
}

void ScrollHistoryBox()
{
	SMALL_RECT OriginalRect;
	OriginalRect.Left = MAINUI_HISTORYBOXXORIGIN;
	OriginalRect.Top = MAINUI_HISTORYBOXYORIGIN;
	OriginalRect.Bottom = OriginalRect.Top + MAINUI_HISTORYBOXHEIGHT;
	OriginalRect.Right = OriginalRect.Left + MAINUI_HISTORYBOXWIDTH;
	COORD NewCoords;
	NewCoords.X = OriginalRect.Left;
	NewCoords.Y = OriginalRect.Top - 1;
	CHAR_INFO cinf;
	cinf.Attributes = 0;
	cinf.Char.AsciiChar = (char)219;
	
	ScrollConsoleScreenBuffer(getconsoleoutputh(), &OriginalRect, &OriginalRect, NewCoords, &cinf);
}

void RedrawMainUI(PPLAYER player, PMAP map)
{
	POINT HistoryOrigin;
	cls(0);
	WHITE;
	printgraphic(globals.graphicslist, globals.zeroed, MAINUI_GRAPHIC_ID);
	HistoryOrigin.x = MAINUI_HISTORYBOXXORIGIN;
	HistoryOrigin.y = MAINUI_HISTORYBOXYORIGIN;
	RestoreHistorySnapShot(HistoryOrigin, MAINUI_HISTORYBOXWIDTH, MAINUI_HISTORYBOXHEIGHT);
	MainUIPrintPlayerStatus(player);
}

void MainUIPrintPlayerStatus(PPLAYER player)
{
	//FIRST, OPERATIVE NAME IN DARK BLUE
	char buf[500];
	//	char tempbuf[500];
	RECT StatusBoxRect;
	POINT SetPoint;
	
	StatusBoxRect.left = MAINUI_STATUSBOXXORIGIN;
	StatusBoxRect.top = MAINUI_STATUSBOXYORIGIN;
	StatusBoxRect.right = StatusBoxRect.left + MAINUI_STATUSBOXWIDTH;
	StatusBoxRect.bottom = StatusBoxRect.top + MAINUI_STATUSBOXHEIGHT;
	ClearRect(StatusBoxRect);
	int CurrentLine = StatusBoxRect.top;
	
	sprintf(buf, "Operative %s", player->codename);
	SetPoint = ReturnTextCenterInRect(buf, StatusBoxRect);
	
	BLUE;
	setcursor(SetPoint.x, CurrentLine);
	writestring(buf);
	CurrentLine+=2;
	
	LBLUE;
	SetPoint.x = StatusBoxRect.left + 1;
	setcursor(SetPoint.x, CurrentLine);
	writestring("Vitals: ");
	//CHECK HEALTH.
	if(player->health > 90){
		LGREEN;
		writestring("Exceptional");
	}
	else if(player->health > 75){
		GREEN;
		writestring("Nominal");
	}
	else if(player->health > 60){
		YELLOW;
		writestring("Fair");
	}
	else if(player->health > 40){
		YELLOW;
		writestring("Faltering");
	}
	else if (player->health > 30){
		RED;
		writestring("Failing");
	}
	else {
		LRED;
		writestring("CRITICAL");
	}
	CurrentLine++;
	
	LBLUE;
	setcursor(SetPoint.x, CurrentLine);
	writestring("Armed: ");
	PGIZMO CurrentGizmo;
	if(!player->currentweapon){
		if((CurrentGizmo = FindActiveGizmoOfType(GIZMO_TYPE_SPIKEMIKE, player->p_gizmos))){
			LRED;
			writestring("SpikeMike");
		}
		else if((CurrentGizmo = FindActiveGizmoOfType(GIZMO_TYPE_BINOCULARS, player->p_gizmos))){
			LRED;
			writestring("Binoculars");
		}
		else if(AreThereActiveGrenadesInInventory(player)){
			LRED;
			writestring("Primed Grenade");
		}
		else{
			RED;
			writestring("-NONE-");
		}
	}
	else{
		LRED;
		writestring(player->currentweapon->shortname);
	}
	CurrentLine++;
	
	LBLUE;
	setcursor(SetPoint.x, CurrentLine);
	writestring("COA: ");
	if(player->currentmissioninfo.dynamicmode)
	{
		LRED;
		writestring("Dynamic");
	}
	else{
		LGREEN;
		writestring("Stealth");
	}
	
	CurrentLine++;
	LBLUE;
	setcursor(SetPoint.x, CurrentLine);
	writestring("Objective: ");
	if(player->currentmissioninfo.primarycomplete)
	{
		LGREEN;
		writestring("Complete");
	}
	else
	{
		LRED;
		writestring("Not Complete");
	}
	CurrentLine++;
	LBLUE;
	setcursor(SetPoint.x, CurrentLine);
	writestring("Maximum Zoom: ");
	LYELLOW;
	sprintf(buf, "%2.2fX", GetPlayerMaxZoom(player));
	writestring(buf);
	LBLUE;
	CurrentLine++;
	setcursor(SetPoint.x, CurrentLine);
	writestring("Weight: ");
	if(player->weight > (3.0 * MAX_PLAYER_WEIGHT) / 4.0){
		LRED;
	}
	else if (player->weight > MAX_PLAYER_WEIGHT / 2.0){
		LYELLOW;
	}
	else{
		LGREEN;
	}
	printf("%2.2f/%2.2f lbs", player->weight, MAX_PLAYER_WEIGHT);
	CurrentLine+=2;
	sprintf(buf, "Press F1 For Help");
	SetPoint = ReturnTextCenterInRect(buf, StatusBoxRect);
	setcursor(SetPoint.x, CurrentLine);
	LRED;
	writestring(buf);
}


void RestoreHistorySnapShot(POINT draworigin, int width, int height)
{
	if(!g_HistorySnapshot){
		return;
	}
	COORD dimensions;
	dimensions.X = width;
	dimensions.Y = height;
	COORD origin;
	origin.X = 0;
	origin.Y = 0;
	SMALL_RECT readrect;
	readrect.Left = (short)(draworigin.x);
	readrect.Top = (short)(draworigin.y);
	readrect.Right = (short)(readrect.Left + width);
	readrect.Bottom = (short)(readrect.Top + height);
	
	WriteConsoleOutput(getconsoleoutputh(), g_HistorySnapshot, dimensions, origin, &readrect);
	free(g_HistorySnapshot);
	g_HistorySnapshot = NULL;
}

void SaveHistorySnapShot(POINT saveorigin, int width, int height)
{
	COORD dimensions;
	dimensions.X = width;
	dimensions.Y = height;
	COORD origin;
	origin.X = 0;
	origin.Y = 0;
	SMALL_RECT readrect;
	readrect.Left = (short)(saveorigin.x);
	readrect.Top = (short)(saveorigin.y);
	readrect.Right = readrect.Left + width;
	readrect.Bottom = readrect.Top + height;
	
	if(g_HistorySnapshot) //IF ALREADY EXISTS
	{
		free(g_HistorySnapshot);
	}
	
	g_HistorySnapshot = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * width * height);
	ReadConsoleOutput(getconsoleoutputh(), g_HistorySnapshot, dimensions, origin, &readrect);
}

void SaveMainSnapShot()
{
	COORD dimensions;
	dimensions.X = getconsize().X;
	dimensions.Y = getconsize().Y;
	COORD origin;
	origin.X = 0;
	origin.Y = 0;
	SMALL_RECT readrect;
	readrect.Left = (short)(0);
	readrect.Top = (short)(0);
	readrect.Right = readrect.Left + dimensions.X - 1;
	readrect.Bottom = readrect.Top + dimensions.Y - 1;
	
	if(g_MainSnapshot) //IF ALREADY EXISTS
	{
		free(g_MainSnapshot);
	}
	
	g_MainSnapshot = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * dimensions.X * dimensions.Y);
	ReadConsoleOutput(getconsoleoutputh(), g_MainSnapshot, dimensions, origin, &readrect);
}

void RestoreMainSnapShot()
{
	if(!g_MainSnapshot){
		return;
	}
	COORD dimensions;
	dimensions.X = getconsize().X;
	dimensions.Y = getconsize().Y;
	COORD origin;
	origin.X = 0;
	origin.Y = 0;
	SMALL_RECT readrect;
	readrect.Left = (short)(0);
	readrect.Top = (short)(0);
	readrect.Right = (short)(readrect.Left + dimensions.X - 1);
	readrect.Bottom = (short)(readrect.Top + dimensions.Y - 1);
	
	WriteConsoleOutput(getconsoleoutputh(), g_MainSnapshot, dimensions, origin, &readrect);
	free(g_MainSnapshot);
	g_MainSnapshot = NULL;
}

bool FinalMobilizationChecks(PPLAYER player)
{
	switch(player->sartumission->missiontype)
	{
	case MISSION_TYPE_SURVEILLANCEDEVICEPLACEMENT:
		if(!DoesListHaveGizmoType(GIZMO_TYPE_SPIKEMIKE, player->p_gizmos, false) && !DoesListHaveGizmoType(GIZMO_TYPE_BUG, player->p_gizmos, false))
		{
			SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
			StaticDialogBox("Necessary Technology", "You are currently slated for a Surveillance Device Placement operation. However, your current manifest shows no surveillance devices. Head back to the Technology Requisiton Center on Level IV and pick up one before you set out.\nPress ENTER.", "Not Acquired", &globals.dboxparams);
			return false;
		}
		break;
	case MISSION_TYPE_CAUTERIZATION:
		if(!player->currentmissioninfo.specifics.cautery.hasbomb){
			SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
			StaticDialogBox("Atomic Device", "You are preparing for a Cauterization mission. Your Atomic device is waiting in the Level VI Atomic Munitions Vault. Proceed there immediately to pick it up before mobilizing.\nPress ENTER.", "Not Acquired", &globals.dboxparams);
			return false;
		}
	}
	
	if(player->p_weapons->objcnt < 1){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		if(!YesNoDialogBox("Weapons Not Loaded", "Your manifest has no allowances for weapons of any kind. You may want to head to Level IV to stock up on weaponry before your mission begins. However, if this lack is intentional, you may proceed. Do you wish to continue mobilization WITHOUT any weapons?", "(Y)es/(N)o", &globals.dboxparams))
			return false;
	}
	if(player->p_gizmos->objcnt < 1){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		if(!YesNoDialogBox("Technology Not Loaded", "Your manifest has no allowances for technology of any kind. You may want to visit the Level IV Technology Requisition Center to choose appropriate technologies for your mission. However, if this lack is intentional, you may proceed. Do you wish to continue mobilization WITHOUT any technologies?", "(Y)es/(N)o", &globals.dboxparams))
			return false;
	}
	
	if(player->ammo.cliplist->objcnt < 1 && player->ammo.roundtypes->objcnt < 1)
	{
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		if(!YesNoDialogBox("Ammuntion Not Loaded", "Your manifset has no allowances for ammunition. You may want to head to the Level IV Ammunition Requisition Center to pick up ammunition for your weapons. However, if this lack is intentional, you may proceed. Do you wish to continue mobilization WITHOUT any ammunition?", "(Y)es/(N)o", &globals.dboxparams))
			return false;
	}
	
	bool syringesready = true;
	for(int i = 0; i < MAX_SYRINGES; i++){
		if(player->drugs.drugtype[i] == 0)
			syringesready = false;
	}
	
	if(!syringesready)
	{
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		if(!YesNoDialogBox("Syringes Not Loaded", "Your manifest has no allowances for DSA pharmaceutical syringes. You may want to visit the Level IV Pharmaceutical Requisition Center to choose pharmaceuticals for your mission. However, if this lack is intentional, you may proceed. Do you wish to continue mobilization WITHOUT any pharmaceuticals?", "(Y)es/(N)o", &globals.dboxparams))
			return false;
	}
	
	
	return true;
}

void MobilizationChatter(PPLAYER player, PMAP map)
{
	char* dirs[MAX_GLOBAL_DIRECTIONS] = {"North", "Northeast", "East", "Southeast", "South", "Southwest", "West", "Northwest", "Central"};
	char tempbuf[5000];
	SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_LARGE, 1, FALSE);
	sprintf(tempbuf, "As you enter the van, you're greeted by a swarthy, smiling man.\"Agent %s, it's good to work with you. I'm Agent Reilly, and our driver is Agent McConnahan. We've got a full plate tonight, and I'm guessing you'll want the low-down on our operations. First, we're going to go into night mode to minimize our signature. Give your eyes a minute to adjust.\"", player->lname);
	StaticDialogBox("Level V Operations", tempbuf, "Press ENTER", &globals.dboxparams);
	playMIXsound(VAN_NIGHTMODE_SOUND);
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_LARGE, 1, FALSE);
	sprintf(tempbuf, "\"Much better. Now, here's what we have for you. You know the details of tonight's op, but I'll let you know that we plan to drop you at the %s edge of the compound. That ought to give you a clean shot to the target, which, at last check, was located in the %s part of the compound area. We'll talk about extraction in a while. It's going to be a bit before we hit our target. We'll arrive in the early morning. Get some rest; we'll wake you up if anything changes.\"", dirs[ReturnGlobalPositionOfObject(map->rz, map)], dirs[ReturnGlobalPositionOfObject(map->pz, map)]);
	StaticDialogBox("Level V Operations", tempbuf, "Press ENTER", &globals.dboxparams);
}

int Mobilization(PPLAYER player)
{
	int RetVal;	
	//	MISSIONCHRONO mc;
	CHRONOSTRUCT ch;
	LoopSong(globals.musiclist.songs[INGRESS_SONG]);
	PMAP Map = initMap(player->sartumission->mapwidth, player->sartumission->mapheight);
	char buf[500];
	createFence(Map);
	FillMapWithPrefabs(Map, player->sartumission->mapbuildingdensitypercentage);
	FillMapWithObstructions(Map,(60 - player->sartumission->mapbuildingdensitypercentage) / 2);
	CreateRandomRZ(Map);
	CreateRandomPZ(Map);
	InitializeGlobalMapDataForCombat(Map, player);
	//ACTIVATE THE PLAYER'S RADIO
	player->currentmissioninfo.radioavailable = true;
	player->currentmissioninfo.RadioChannel = 0;
	//GET THE RZ AND TELL THE PLAYER WHERE THEY'LL BE DROPPED OFF.
	FadeInTitle("Operations Van", "Enroute To Target");
	MobilizationChatter(player, Map);
	//ALLOW THE PLAYER TO DO SOME STUFF HERE?
	//THE PLAYER MAY DO THEIR INITIAL SET UP HERE.
	
	
	////END INITIAL PLAYER SET UP.
	ch.hour = rand()%3 + 1;
	ch.minute = rand()%60 +1;
	ch.second = rand()%60 +1;
	sprintf(buf, "%d:%2.2d AM", ch.hour, ch.minute);
	FadeInTitle("Target Compound", buf);
	//GET THE AMOUNT OF TIME, SYNCHRONIZE CHRONOGRAPH.
	//ASK FOR AMOUNT OF TIME TO PICKUP.
	AskPlayerForTimeToRecovery(player);
	SynchronizeChronoSequence(&ch, &player->currentmissioninfo.timetorecovery);
	FinalMobilizationCutscene();
	//NOW, THE MAIN MISSION. KEEP THE PLAYER STUCK HERE IF HE'S NOT CAPTURED.
	RetVal = MainMission(player, Map);
	Surgery(player);
	terminateMap(Map);
	FreePlayerFromCombat(player);
	FreeGlobalMapDataFromCombat();
	if(RetVal == MISSION_RESULT_CAPTUREDPRIMARYSUCCESS || RetVal == MISSION_RESULT_CAPTUREDPRIMARYFAILURE){
		player->status = Captivity(player);
		if(player->status == OPERATIVE_STATUS_ACTIVE){
			return MISSION_RESULT_ESCAPEDTORTURE;
		}
		else{
			return MISSION_RESULT_CHECKPLAYERSTATUS;
		}
	}
	else{
		return RetVal;
	}
}

void AskPlayerForTimeToRecovery(PPLAYER player)
{
	MISSIONCHRONO Mc;
	Mc.countdown = true;
	Mc.seconds = 59;
	Mc.split = 99;
	char tempbuf[5000];
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
	strcpy(tempbuf, "You're approached by Reilly, who taps his watch. \"Time to go, Ace. How much time do you figure you'll need before we come pick you up?\"");
	StaticDialogBox("Recovery Time Needed", tempbuf, "Press ENTER", &globals.dboxparams);
	char *Times[10] = {
		"0 HOURS, 30 MINUTES",
			"0 HOURS, 45 MINUTES",
			"1 HOUR, 0 MINUTES",
			"1 HOUR, 15 MINUTES",
			"1 HOUR, 30 MINUTES",
			"1 HOUR, 45 MINUTES",
			"2 HOURS, 0 MINUTES",
			"2 HOURS, 15 MINUTES",
			"2 HOURS, 30 MINUTES",
			"2 HOURS, 45 MINUTES"
	};
	int Hours[10] ={
		0,0,1,1,1,1,2,2,2,2
	};
	
	int Minutes[10] = {
		30,45,0,15,30,45,0,15,30,45
	};
	
	
	PLIST OurList;
	CreateDBStringList(&OurList, Times, 10);
	int Selection = SimpleListDialogBox("Select Time To Recovery", "Level V", OurList, &globals.dboxparams, CreateStringDBListItem);
	Mc.hours = Hours[Selection];
	Mc.minutes = Minutes[Selection];
	cls(0);
	StaticDialogBox("Authorization Received", "Reilly nods quickly, and grabs a sheet of paper from the driver. \"All right. We're all set. We've just received Final Authorization from DSA, so you're green to go. Just make sure you read over this printout carefully. Good luck, Ace.\" He pats you on the back, and disappears back into the cabin.", "Press ENTER", &globals.dboxparams);
	SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_MEDIUM, 1, FALSE);
	sprintf(tempbuf, "Agent %s:\nFinal Authorization For Operation #%s Has Been Received.\nYou are instructed to infiltrate the compound and complete your objectives forthwith. Your Extraction team will return in %d Hours and %d Minutes to recover you unless otherwise instructed.\nYou are now required to synchronize your chronograph to mission specifications, at which time you will exit the van and proceed. Good luck.", player->lname, player->sartumission->missioncode, Mc.hours, Mc.minutes);
	ComputerPrintOutString(tempbuf, 30, "Press ENTER to Begin Synchronization.");
	cls(0);
	Sleep(1000);
	PlayerSetTimeToRecovery(player, &Mc);
	
}

void InitializeGlobalMapDataForCombat(PMAP map, PPLAYER player)
{
	
	
	globals.globalmapstruct.bug = createList();
	globals.globalmapstruct.clips = createList();
	globals.globalmapstruct.doors = createList();
	globals.globalmapstruct.gizmos = createList();
	globals.globalmapstruct.sbk = createList();
	globals.globalmapstruct.scriptedevents = createList();
	globals.globalmapstruct.strayrounds = createList();
	globals.globalmapstruct.weapons = createList();
	globals.globalmapstruct.vehicles = createList();
	globals.globalmapstruct.environmental = createList();
	
	InitializeGlobalMapArmories(map, player->sartumission);
	InitializeGlobalMapEnemies(map, player->sartumission);
	InitializeGlobalMapDoors(player, map);
	InitializeGlobalMapVehicles(map, player->sartumission);
	
	SCRIPTEDEVENT Event;
	Event.TriggerTime.hours = 0;
	Event.TriggerTime.minutes = 0;
	Event.TriggerTime.seconds = 5;
	Event.TriggerTime.split = 30;
	AddScriptedEvent(&Event);
}

int InitializeGlobalMapArmories(PMAP map, PSARTUMISSION missioninfo)
{
	int NumTotalArmories = missioninfo->numberarmories;
	bool** LockOutArray; //THIS IS THE LOCKOUT ARRAY.
	bool Continue = true;
	bool Placed = false;
	PMAPCELL TempCell;
	int Iterations = 0;
	
	InitDoubleArray(&LockOutArray, map->width, map->height);
	globals.globalmapstruct.armories = (PMAPARMORY)malloc(sizeof(MAPARMORY) * missioninfo->numberarmories);
	
	for(int i = 0; i < NumTotalArmories && Continue; i++, Iterations = 0, Placed = false){
		globals.globalmapstruct.armories[i].destroyed = false; //WE WANT IT ALIVE!
		do
		{
			globals.globalmapstruct.armories[i].location.x = (rand()%(map->width - 3)) + 3;
			globals.globalmapstruct.armories[i].location.y = (rand()%(map->height - 3)) + 3;
			TempCell = FindMapCellByCoordinate(map, globals.globalmapstruct.armories[i].location);
			if(IsMapCellSuitableForArmory(map, TempCell, LockOutArray)){
				LockOutArray[globals.globalmapstruct.armories[i].location.y][globals.globalmapstruct.armories[i].location.x] = true;
				Placed = true;
			}
			Iterations++;
		}while(Iterations < 1000 && !Placed);
		
		if(Iterations >= 1000){
			Continue = false;
			
		}
		else{ //WE'VE PLACED IT. STANDBY TO INSERT OBJECTS.
			CreateMapArmory(&(globals.globalmapstruct.armories[i]));
		}
	}
	FreeDoubleArray(&LockOutArray, map->height);
	globals.globalmapstruct.numarmories = i;
	return 0; //OK
}

void CreateMapArmory(PMAPARMORY Armory)
{
	Armory->ammo.cliplist = createList();
	Armory->ammo.roundtypes = createList();
	Armory->weaponry = createList();
	Armory->type = rand()%MAX_MAP_ARMORY_TYPES;
	int NumUnloadedWeapons;
	int NumWeapons;
	int NumAmmos;
	int AmmoUnitsPerWeapon = MAX_MAP_ARMORY_TYPES;
	int WeaponsUsed = 0;
	int AmmosUsed = 0;
	int UnloadedWeaponsUsed = 0;
	int WeaponID;
	bool AmmunitionTiedToWeapons;
	//	ROUND RoundType;
	//	int SubAmmoType;
	
	switch(Armory->type)
	{
	case MAP_ARMORY_TYPE_DEPLOYMENT:
		NumWeapons = rand()%4;
		NumUnloadedWeapons = 0;
		NumAmmos = MAX_MAP_ARMORY_ITEMS - NumWeapons;
		if(NumWeapons){
			AmmoUnitsPerWeapon = NumAmmos / NumWeapons;
		}
		AmmunitionTiedToWeapons = true;
		
		break;
	case MAP_ARMORY_TYPE_EMERGENCY:
		NumWeapons = 1;
		NumUnloadedWeapons = 0;
		NumAmmos = MAX_MAP_ARMORY_ITEMS - NumWeapons;
		if(NumWeapons){
			AmmoUnitsPerWeapon = NumAmmos / NumWeapons;
		}
		AmmunitionTiedToWeapons = true;
		break;
	case MAP_ARMORY_TYPE_STORAGE:
		NumWeapons = 0;
		NumUnloadedWeapons = MAX_MAP_ARMORY_ITEMS;
		NumAmmos = 0;
		if(NumWeapons){
			AmmoUnitsPerWeapon = NumAmmos / NumWeapons;
		}
		AmmunitionTiedToWeapons = true;
		break;
	case MAP_ARMORY_TYPE_AMMUNITION:
		NumWeapons = 0;
		NumUnloadedWeapons = 0;
		NumAmmos = MAX_MAP_ARMORY_ITEMS;
		if(NumWeapons){
			AmmoUnitsPerWeapon = NumAmmos / NumWeapons;
		}
		AmmunitionTiedToWeapons = false;
		break;
	case MAP_ARMORY_TYPE_NONE:
		NumWeapons = 0;
		NumUnloadedWeapons = 0;
		NumAmmos = 0;
		break;
	}
	
	for(WeaponsUsed = 0; WeaponsUsed < NumWeapons; WeaponsUsed++){
		WeaponID = GetWeightedNONDSARandomWeaponID(globals.globalweaponsammo.weaponlist);
		if(WeaponID > 0){
			GiveLoadedTrainingWeapon(Armory->weaponry, &Armory->ammo, WeaponID, 0);
			GiveRandomNONDSAExtraAmmo(&Armory->ammo, WeaponID);
		}
	}
	for(UnloadedWeaponsUsed = 0; UnloadedWeaponsUsed < NumUnloadedWeapons; UnloadedWeaponsUsed++){
		WeaponID = GetWeightedNONDSARandomWeaponID(globals.globalweaponsammo.weaponlist);
		if(WeaponID > 0){
			GiveTrainingWeapon(Armory->weaponry, WeaponID);
		}
	}
	if(!AmmunitionTiedToWeapons)
	{
		for(AmmosUsed = 0; AmmosUsed < NumAmmos;){
			WeaponID = GetWeightedNONDSARandomWeaponID(globals.globalweaponsammo.weaponlist);
			if(WeaponID > 0){
				if(GiveRandomNONDSAExtraAmmo(&Armory->ammo, WeaponID)){
					AmmosUsed++;
				}
			}
		}
	}
}

bool IsMapCellSuitableForArmory(PMAP Map, PMAPCELL Cell, bool** LockOutArray)
{
	PMAPCELL TempCell;
	POINT TempCoord;
	
	if(!Cell){
		return false;
	}
	if(!LockOutArray){
		return false;
	}
	
	if(Cell->xcoord < 3 || Cell->ycoord < 3 || Cell->xcoord > Map->width - 3 || Cell->ycoord > Map->height - 3){
		return false;
	}
	
	if(Cell->type != OBJECT_SYM_FLOOR){
		return false;
	}
	
	if(Cell->xcoord == Map->pz.x && Cell->ycoord == Map->pz.y){
		return false;
	}
	
	//CHECK LOCKOUT ARRAY.
	if(LockOutArray[Cell->ycoord][Cell->xcoord]){
		return false;
	}
	
	//NOW CHECK IN A RADIUS AROUND THE CELL. IF IT HAS AT LEAST ONE WALL, IT'S OKAY
	//RIGHT FIRST.
	TempCoord.x = Cell->xcoord + 1;
	TempCoord.y = Cell->ycoord; 
	TempCell = FindMapCellByCoordinate(Map, TempCoord);
	if(IsCellWall(TempCell)){
		return true;
	}
	//LEFT NEXT.
	TempCoord.x = Cell->xcoord - 1;
	TempCoord.y = Cell->ycoord; 
	TempCell = FindMapCellByCoordinate(Map, TempCoord);
	if(IsCellWall(TempCell)){
		return true;
	}
	//TOP NEXT.
	TempCoord.x = Cell->xcoord;
	TempCoord.y = Cell->ycoord - 1; 
	TempCell = FindMapCellByCoordinate(Map, TempCoord);
	if(IsCellWall(TempCell)){
		return true;
	}
	//BOTTOM LAST.
	TempCoord.x = Cell->xcoord;
	TempCoord.y = Cell->ycoord + 1; 
	TempCell = FindMapCellByCoordinate(Map, TempCoord);
	if(IsCellWall(TempCell)){
		return true;
	}
	return false;
}

bool IsCellWall(PMAPCELL Cell)
{
	if(!Cell){
		return false;
	}
	switch(Cell->type)
	{
	case BUILDING_SYM_BLCORNER:
	case BUILDING_SYM_TRCORNER:
	case BUILDING_SYM_BRCORNER:
	case BUILDING_SYM_TLCORNER:
	case BUILDING_SYM_VWALL:
	case BUILDING_SYM_HWALL:
		return true;
	default:
		return false;
	}
}


bool IsCellDoor(PMAPCELL Cell)
{
	if(!Cell){
		return false;
	}
	switch(Cell->type)
	{
	case BUILDING_SYM_VDOOR:
	case BUILDING_SYM_HDOOR:
		return true;
	default:
		return false;
	}
}

int InitializeGlobalMapEnemies(PMAP map, PSARTUMISSION missioninfo)
{
	//CHECK IF WE NEED A PRIMARY d00d.
	bool needprimary = false;
	bool primaryfriendly = false;
	int primarystatus = 0;
	int CurrentEnemyCounter = 0;
	switch(missioninfo->missiontype)
	{
	case MISSION_TYPE_NOTOCHORD:
		primarystatus = 4; //HE'S TRAPPED IN A CHAIR!!! - HE MUST BE IN A BUILDING, MAKE SURE OF IT.
		needprimary = true;
		break;
	case MISSION_TYPE_RESCUE:
		primarystatus = 4;
		primaryfriendly = true;
		needprimary = true;
		break;
	case MISSION_TYPE_ESCAPE:
		primaryfriendly = true;
		needprimary = true;
		break;
	case MISSION_TYPE_ISR:
	case MISSION_TYPE_ASSASSINATION:
		needprimary = true;
		break;
	}
	cls(0);
	//ENEMY NUMBER ZERO IS OUR PRIMARY, IF NECSSARY.
	if(needprimary){
		GeneratePrimaryObjectiveEnemy(primaryfriendly, primarystatus, missioninfo, map);
		CurrentEnemyCounter++;
	}
	for(; CurrentEnemyCounter < missioninfo->numberguards; CurrentEnemyCounter++){
		GenerateEnemy(map, missioninfo, CurrentEnemyCounter);
		PrintPercentageComplete("Guard Scan - ", (int)(CurrentEnemyCounter * 100.0 / missioninfo->numberguards));
	}
//	AssignRandomKeyCards(missioninfo); NOT READY
	return 0; //OK
}

int InitializeGlobalMapVehicles(PMAP map, PSARTUMISSION missioninfo)
{
	int NumTotalVehicles = missioninfo->numbervehicles;
	bool** LockOutArray; //THIS IS THE LOCKOUT ARRAY.
	bool Continue = true;
	bool Placed = false;
	PMAPCELL TempCell;
	int Iterations = 0;
	POTYPE Vehicle;
	
	InitDoubleArray(&LockOutArray, map->width, map->height);
	
	
	for(int i = 0; i < NumTotalVehicles && Continue; i++, Iterations = 0, Placed = false){
		do
		{
			Vehicle.vehicle.loc.x = (rand()%(map->width - 3)) + 3;
			Vehicle.vehicle.loc.y = (rand()%(map->height - 3)) + 3;
			TempCell = FindMapCellByCoordinate(map, Vehicle.vehicle.loc);
			if(IsMapCellSuitableForVehicle(map, TempCell, LockOutArray)){
				LockOutArray[Vehicle.vehicle.loc.y][Vehicle.vehicle.loc.x] = true;
				Placed = true;
			}
			Iterations++;
		}while(Iterations < 1000 && !Placed);
		
		if(Iterations >= 1000){
			Continue = false;
			
		}
		else{ //WE'VE PLACED IT. STANDBY TO INSERT OBJECTS.
			Vehicle.vehicle.ID = globals.globalmapstruct.vehicles->objcnt;
			GenerateVehicle(&Vehicle.vehicle);
			addtoList(globals.globalmapstruct.vehicles, Vehicle, CompareVehicleID);
		}
	}
	FreeDoubleArray(&LockOutArray, map->height);
	return 0; //OK
}

bool IsMapCellSuitableForVehicle(PMAP Map, PMAPCELL Cell, bool** LockOutArray)
{
	PMAPCELL TempCell;
	POINT TempCoord;
	
	if(!Cell){
		return false;
	}
	if(!LockOutArray){
		return false;
	}
	
	if(Cell->xcoord < 3 || Cell->ycoord < 3 || Cell->xcoord > Map->width - 3 || Cell->ycoord > Map->height - 3){
		return false;
	}
	
	if(Cell->type != OBJECT_SYM_GROUND){
		return false;
	}
	
	if(Cell->xcoord == Map->pz.x && Cell->ycoord == Map->pz.y){
		return false;
	}
	
	//CHECK LOCKOUT ARRAY.
	if(LockOutArray[Cell->ycoord][Cell->xcoord]){
		return false;
	}
	
	//NOW CHECK IN A RADIUS AROUND THE CELL. IF IT HAS AT MOST ONE DOOR, IT'S BAD.
	//RIGHT FIRST.
	TempCoord.x = Cell->xcoord + 1;
	TempCoord.y = Cell->ycoord; 
	TempCell = FindMapCellByCoordinate(Map, TempCoord);
	if(IsCellDoor(TempCell)){
		return false;
	}
	//LEFT NEXT.
	TempCoord.x = Cell->xcoord - 1;
	TempCoord.y = Cell->ycoord; 
	TempCell = FindMapCellByCoordinate(Map, TempCoord);
	if(IsCellDoor(TempCell)){
		return false;
	}
	//TOP NEXT.
	TempCoord.x = Cell->xcoord;
	TempCoord.y = Cell->ycoord - 1; 
	TempCell = FindMapCellByCoordinate(Map, TempCoord);
	if(IsCellDoor(TempCell)){
		return false;
	}
	//BOTTOM LAST.
	TempCoord.x = Cell->xcoord;
	TempCoord.y = Cell->ycoord + 1; 
	TempCell = FindMapCellByCoordinate(Map, TempCoord);
	if(IsCellDoor(TempCell)){
		return false;
	}
	return true;
}

void FreeGlobalMapDataFromCombat()
{
	FreeMapEnemies(globals.globalmapstruct.enemies, globals.globalmapstruct.numenemies);
	FreeMapArmories(globals.globalmapstruct.armories, globals.globalmapstruct.numarmories);
	freeList(globals.globalmapstruct.vehicles);
	free(globals.globalmapstruct.vehicles);
	freeList(globals.globalmapstruct.weapons);
	free(globals.globalmapstruct.weapons);
	freeList(globals.globalmapstruct.doors);
	free(globals.globalmapstruct.doors);
	freeList(globals.globalmapstruct.clips);
	free(globals.globalmapstruct.clips);
	freeList(globals.globalmapstruct.strayrounds);
	free(globals.globalmapstruct.strayrounds);
	freeList(globals.globalmapstruct.gizmos);
	free(globals.globalmapstruct.gizmos);
	freeList(globals.globalmapstruct.bug);
	free(globals.globalmapstruct.bug);
	freeList(globals.globalmapstruct.sbk);
	free(globals.globalmapstruct.sbk);
	freeList(globals.globalmapstruct.scriptedevents);
	free(globals.globalmapstruct.scriptedevents);
	freeList(globals.globalmapstruct.environmental);
	free(globals.globalmapstruct.environmental);
}

void FreeMapArmories(PMAPARMORY armories, int numarmories)
{
	for(int i = 0; i < numarmories; i++){
		freeList(armories[i].ammo.cliplist);
		freeList(armories[i].ammo.roundtypes);
		RemoveAllWeapons(armories[i].weaponry);
	}
	free(armories);
}

void FreeMapEnemies(PENEMY enemies, int numenemies)
{
	for(int i = 0; i < numenemies; i++){
		FreeEnemyFromCombat(&(enemies[i]));
	}
	free(enemies);
}

double GetPlayerMaxZoom(PPLAYER player)
{
	if(player->currentweapon && player->currentweapon->attacktype & WEAP_ATT_TYPE_SNIPE){
		return WEAPONSCOPE_ZOOM_MAX;
	}
	else if(FindActiveGizmoOfType(GIZMO_TYPE_TAS, player->p_gizmos)){
		return TAS_ZOOM_MAX;
	}
	else if(FindActiveGizmoOfType(GIZMO_TYPE_BINOCULARS, player->p_gizmos)){
		return BINOCULARS_ZOOM_MAX;
	}
	else if(FindActiveGizmoOfType(GIZMO_TYPE_NVG, player->p_gizmos)){
		return NVG_ZOOM_MAX;
	}
	else if(FindActiveGizmoOfType(GIZMO_TYPE_SPIKEMIKE, player->p_gizmos)){
		return SPIKEMIKE_ZOOM_MAX;
	}
	else if(FindActiveGizmoOfType(GIZMO_TYPE_GASMASK, player->p_gizmos)){
		return GASMASK_ZOOM_MAX;
	}
	else{
		return HUMAN_ZOOM_MAX;
	}
}

int DoAnyGizmosInterfereWithMovement(PLIST gizmolist)
{
	if(FindActiveGizmoOfType(GIZMO_TYPE_TAS, gizmolist)){
		return GIZMO_TYPE_TAS;
	}
	return 0;
}

bool InitializeGlobalMapDoors(PPLAYER player, PMAP map)
{
	POINT CheckPoint;
	PMAPCELL Cell;
	POTYPE Door;
	for(int h = 0; h < map->height; h++){
		CheckPoint.y = h;
		for(int w = 0; w < map->width; w++){
			CheckPoint.x = w;
			Cell = FindMapCellByCoordinate(map, CheckPoint);
			if(!Cell){
				return false;
			}
			if(Cell->type == BUILDING_SYM_VDOOR || Cell->type == BUILDING_SYM_HDOOR){
				//IT'S A DOOR, PREPARE IT.
				Door.door.location = CheckPoint;
				Door.door.vdoor = (Cell->type == BUILDING_SYM_VDOOR);
				InitializeDoorBySecurityFactor(player->sartumission->compoundsecuritypercentage, &Door.door);
				Door.door.ID = globals.globalmapstruct.doors->objcnt;
				if(addtoList(globals.globalmapstruct.doors, Door, CompareDoorID)){
					return false;
				}
			}
		}
	}
	return true;
}

void InitializeDoorBySecurityFactor(int SecurityFactor, PDOOR door)
{
	double Chance;
	int MinLockRings = MIN_LIS_RINGS;
	int MaxLockRings = MAX_LIS_RINGS;
	int LockRings;
	int MinEncryptionStrength = MIN_CIS_ENCRYPTIONSTRENGTH;
	int MaxEncryptionStrength = MAX_CIS_ENCRYPTIONSTRENGTH;
	int EncryptionStrength;
	
	double RealFactor = (double)CapInclusive(SecurityFactor, 1, 100);
	RealFactor /= 100.0;
	//DOOR TYPE CHANCE
	Chance = 60.0 *  RealFactor;
	if(RandomPercentage((int)Chance)){
		door->type = DOOR_REMETAL;
	}
	else if (RandomPercentage((int)(Chance + 10.0))){
		door->type = DOOR_METAL;
	}
	else if (RandomPercentage((int)(Chance + 20.0))){
		door->type = DOOR_WOOD;
	}
	else{
		door->type = DOOR_GLASS;
	}
	//DOOR LOCK CHANCE
	Chance = 50.0 * RealFactor;
	if(RandomPercentage((int)Chance)){
		door->lock = DOOR_LOCK_CIS;
		EncryptionStrength = (int)((((double)(MaxEncryptionStrength - MinEncryptionStrength)) * RealFactor) + MinEncryptionStrength);
		EncryptionStrength = random(EncryptionStrength - 1, EncryptionStrength + 1);
		EncryptionStrength = CapInclusive(EncryptionStrength, MinEncryptionStrength, MaxEncryptionStrength);
		door->encryptionstrength = EncryptionStrength;
	}
	else if(RandomPercentage((int)Chance)){
		door->lock = DOOR_LOCK_LIS;
		LockRings = (int)((((double)(MaxLockRings - MinLockRings)) * RealFactor) + MinLockRings);
		LockRings = random(LockRings - 1, LockRings + 1);
		LockRings = CapInclusive(LockRings, MinLockRings, MaxLockRings);
		door->lockrings = LockRings;
	}
	else{
		door->lock = DOOR_LOCK_NONE;
	}
	
	//DOOR ALARM CHANCE
	Chance = 40.0 * RealFactor;
	if(RandomPercentage((int)Chance)){
		door->alarm = DOOR_ALARM;
	}
	else{
		door->alarm = DOOR_NOALARM;
	}
	
	if(door->lock != DOOR_LOCK_NONE){
		door->lockactive = TRUE;
		Chance = 50.0;
		if(RandomPercentage((int)Chance)){
			door->keycard = true;
			door->keycardnumber = 1 << rand()%MAX_DOOR_KEYCARDS;
		}
		else{
			door->keycard = false;
			door->keycardnumber = 0;
		}
	}
	else{
		door->keycard = false;
		door->keycardnumber = 0;
	}
	
	door->isopen = false;
}

int CompareDoorID(PPOTYPE itm1,PPOTYPE itm2) 
{
	if(itm1->door.ID == itm2->door.ID)
		return (0);
	return (1);
}

int CompareDoorCoords(PPOTYPE itm1,PPOTYPE itm2) 
{
	if(itm1->door.location.x == itm2->door.location.x && itm1->door.location.y == itm2->door.location.y)
		return (0);
	return (1);
}

int CompareScriptedEventID(PPOTYPE itm1,PPOTYPE itm2) 
{
	if(itm1->scriptedevent.Id == itm2->scriptedevent.Id)
		return (0);
	return (1);
}

int CompareScriptedEventChrono(PPOTYPE itm1, PPOTYPE itm2)
{
	return CompareMissionChrono(&itm1->scriptedevent.TriggerTime, &itm2->scriptedevent.TriggerTime);
}

PDOOR GetDoorInfoFromCell(PMAPCELL Cell)
{
	POTYPE Finder;
	PPOTYPE Found;
	
	Finder.door.location.x = Cell->xcoord;
	Finder.door.location.y = Cell->ycoord;
	
	Found = findObject(globals.globalmapstruct.doors, &Finder, CompareDoorCoords);
	if(!Found){
		return NULL;
	}
	return &Found->door;
}

bool AddScriptedEvent(PSCRIPTEDEVENT Event)
{
	POTYPE EventObject;
	if(!Event){
		return false;
	}
	EventObject.scriptedevent = *Event;
	EventObject.scriptedevent.Id = globals.globalmapstruct.scriptedevents->objcnt;
	addtoList(globals.globalmapstruct.scriptedevents, EventObject, CompareScriptedEventID);
	SortScriptedEventListByID(globals.globalmapstruct.scriptedevents);
	return true;
}

PSCRIPTEDEVENT GetScriptedEventByID(int id)
{
	POTYPE Finder;
	PPOTYPE Found;
	Finder.scriptedevent.Id = id;
	Found = findObject(globals.globalmapstruct.scriptedevents, &Finder, CompareScriptedEventID);
	if(!Found){
		return false;
	}
	return &Found->scriptedevent;
}

bool IsScriptedEventDue()
{
	if(globals.globalmapstruct.scriptedevents->objcnt < 1){
		return false;
	}
	if(CompareMissionChrono(&globals.globalmapstruct.totaltime, &globals.globalmapstruct.scriptedevents->head->object.scriptedevent.TriggerTime) <= 0){
		return true;
	}
	return false;
}

bool RunNextScriptedEvent(PPLAYER player, PMAP map)
{
	PSCRIPTEDEVENT Event;
	if(!globals.globalmapstruct.scriptedevents->head){
		return false;
	}
	Event = &globals.globalmapstruct.scriptedevents->head->object.scriptedevent;
	//////////////////////////////RUN EVENT HERE///////////////////////////////////////////////
	ScriptedEventResolution(Event, player, map);
	/////////////////////////////EVENT RUN DONE///////////////////////////////////////////////
	//DELETE THE EVENT
	deleteObject(globals.globalmapstruct.scriptedevents,&globals.globalmapstruct.scriptedevents->head->object);
	//REINITIALIZE THE LIST NUMBERS.
	ReindexScriptedEventList(globals.globalmapstruct.scriptedevents);
	return true;
}


void ScriptedEventResolution(PSCRIPTEDEVENT Event, PPLAYER player, PMAP map)
{
	char buf[500];
	switch(Event->Type)
	{
	case SCRIPTEDEVENT_TYPE_DOORCLOSE:
		CloseDoor(map, player, (PDOOR)Event->Context);
		break;
	case SCRIPTEDEVENT_TYPE_OCULARFOCUS:
		RestoreOcularFocus(player);
		break;
	case SCRIPTEDEVENT_TYPE_DRUGWEAROFF:
		ModifyPlayerPsych(player, (PPSYCHSKILLSTRUCT)Event->Context);
		free(Event->Context);
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		StaticDialogBox("Drug Has Worn Off!", "Your injected drug has been broken down and is now useless.", "Press ENTER", &globals.dboxparams);
		break;
	case SCRIPTEDEVENT_TYPE_ANTIDOTEWEAROFF:
		player->currentmissioninfo.antidoteon = false;
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		StaticDialogBox("Antidote Has Worn Off!", "Your Antidote has been broken down and is now useless.", "Press ENTER", &globals.dboxparams);
		break;
	case SCRIPTEDEVENT_TYPE_REMEDK:
		player->health ++;
		player->health = CapInclusive(player->health, 0, MAX_HEALTH);
		MainUIPrintPlayerStatus(player);
		break;
	case SCRIPTEDEVENT_TYPE_PROXBOMBACTIVATE:
		playSYNCsound(C4_PROXIMITY_ARM);
		sprintf(buf, "proximity detonator at (%d/%d) is now scanning", ((PGIZMO)Event->Context)->WorldPoint.x, ((PGIZMO)Event->Context)->WorldPoint.y);
		((PGIZMO)Event->Context)->AdditionalBool = true; //IT'S NOW SCANNING.
		InsertHistoryString(player, buf, true);
		break;
	case SCRIPTEDEVENT_TYPE_AMPLIFIEDTRACKING:
		AmplifiedTrackingSystemResponse(player, map);
		break;
	case SCRIPTEDEVENT_TYPE_TIMEDBOMBEXPLODE:
		C4Detonate(((PGIZMO)Event->Context)->index, map, player);
		RemoveWorldGizmo(((PGIZMO)Event->Context)->index);
		break;
	case SCRIPTEDEVENT_TYPE_REMOTEBOMBDETONATE:
		DetonateAllBombsOnRemoteFrequency(map, player, Event->ExtraDataOne);
		break;
	case SCRIPTEDEVENT_TYPE_GRENADEDETONATE:
		DetonateGrenade(((PGIZMO)Event->Context), map, player);
		break;
	}
}

void DebugExplosion(PPLAYER player, PMAP map, POINT ExplosionPoint)
{
	ENVIRONSTRUCT EvOne;
	ENVIRONSTRUCT EvTwo;
	EvOne.totalturnsleft = 3;
	EvOne.turnsperexpansion = 1;
	EvOne.currentradius = 3;
	EvOne.radiusperexpansion = 2;
	EvOne.origin = ExplosionPoint;
	EvOne.type = ENVIRON_TYPE_FIRE;
	
	EvTwo = EvOne;
	EvTwo.currentradius = 5;
	EvTwo.totalturnsleft = 20;
	EvTwo.turnsperexpansion = 5;
	EvTwo.radiusperexpansion = 1;
	EvTwo.type = ENVIRON_TYPE_SMOKE;
	
	playMIXsound(IMPACTSOUND_DEFAULT_EXPLOSIVE);
	
	DestroyObjectsInRadius(map, player, EvOne.origin, EvOne.currentradius, 100, 10);
	
	InsertEnvironmentalEffect(&EvOne);
	InsertEnvironmentalEffect(&EvTwo);
	Sleep(rand()%300 + 100);
}

void SortScriptedEventListByID(PLIST List)
{
	sortList(List, CompareScriptedEventChrono, NULL, CompareScriptedEventID);
}

void ReindexScriptedEventList(PLIST List)
{
	int EventID = 0;
	PLIST walker;
	walker = List;
	for(walker->current = walker->head ; walker->current != NULL ; walker->current = walker->current->nextnode)
	{
		walker->current->object.scriptedevent.Id = EventID;
		EventID++;
	}
}

bool IsZoomValid(PPLAYER player, PMAP map, POINT NextZoomPoint)
{
	//CHECK FOR WALLS, CHECK FOR GOING BEHIND PLAYER (TURN DIRECTION)
	RECT ValidRect;
	ValidRect.top = 1;
	ValidRect.left = 1;
	ValidRect.right = map->width - 2;
	ValidRect.bottom = map->height - 2;
	
	switch(player->turndirection)
	{
	case TURN_DIRECTION_NORTH:
		ValidRect.bottom = player->location->ycoord;
		break;
	case TURN_DIRECTION_SOUTH:
		ValidRect.top = player->location->ycoord;
		break;
	case TURN_DIRECTION_WEST:
		ValidRect.right = player->location->xcoord;
		break;
	case TURN_DIRECTION_EAST:
		ValidRect.left = player->location->xcoord;
		break;
	}
	return IsPointInRect(ValidRect, NextZoomPoint);
}

void PrintDoorInfo(PPLAYER player, PDOOR Door)
{
	char buf[5000];
	char *CardTypes[MAX_DOOR_KEYCARDS] =
	{
		"Tau",
			"Theta",
			"Omicron",
			"Epsilon",
			"Phi",
			"Rho"
	};
	int CardIDs[MAX_DOOR_KEYCARDS] =
	{
		1, 2, 4, 8, 16, 32
	};
	int difficulty = 0;
	bool CanSeeAlarm = false;
	char tempbuf[500];
	sprintf(buf, "You Examine The Door.\nYou Find That:\n-It Appears To Be Made Of ", Door->location.x, Door->location.y);
	switch(Door->type){
	case DOOR_GLASS:
		strcpy(tempbuf, "Glass");
		break;
	case DOOR_WOOD:
		strcpy(tempbuf, "Wood");
		break;
	case DOOR_METAL:
		strcpy(tempbuf, "Thin Metal");
		break;
	case DOOR_REMETAL:
	default:
		strcpy(tempbuf, "Reinforced Metal");
		break;
	}
	strcat(buf, tempbuf);
	strcat(buf, "\n");
	strcat(buf, "-It Is Secured With ");
	switch(Door->lock){
	case DOOR_LOCK_NONE:
		strcpy(tempbuf, "No Visible Lock");
		break;
	case DOOR_LOCK_CIS:
		if(Door->encryptionstrength > 7){
			strcpy(tempbuf, "A Very Advanced ");
		}
		else if(Door->encryptionstrength > 5){
			strcpy(tempbuf, "A Fairly Advanced ");
		}
		else if(Door->encryptionstrength > 3){
			strcpy(tempbuf, "An Adequate ");
		}
		else{
			strcpy(tempbuf, "A Fairly Rudimentary ");
		}
		strcat(tempbuf, "Cryptographic Lock");
		difficulty = (int)((double)Door->encryptionstrength * 100.0 / (double) MAX_CIS_ENCRYPTIONSTRENGTH);
		break;
	case DOOR_LOCK_LIS:
		strcpy(tempbuf, "A Standard Tumbler Lock ");
		if(Door->lockrings > 4* MAX_LIS_RINGS / 5){
			strcat(tempbuf, "Of An Advanced Type.");
		}
		else if(Door->lockrings > 3 * MAX_LIS_RINGS / 4){
			strcat(tempbuf, "Of A Secure Type.");
		}
		else if(Door->lockrings > MIN_LIS_RINGS / 2){
			strcat(tempbuf, "Of An Adequate Type.");
		}
		else{
			strcat(tempbuf, "Of A Simple Type.");
		}
		difficulty = (int)((double)Door->lockrings * 100.0 / (double) MAX_LIS_RINGS);
		break;
	}
	strcat(tempbuf, "\n");
	strcat(buf, tempbuf);
	
	if(Door->keycard ==  DOOR_KEYCARD){
		strcat(buf, "-The Door Is Wired To A Keycard Authorization Unit, Configured To Accept ");
		for(int i = 0; i < MAX_DOOR_KEYCARDS; i++){
			if(Door->keycardnumber == CardIDs[i]){
				strcat(buf, CardTypes[i]);
				strcat(buf, " Keycards.\n");
			}
		}
	}
	
	switch(Door->alarm)
	{
	case DOOR_NOALARM:
		strcat(buf, "-It Doesn't Seem to Have Any Attached Alarm System.");
		break;
	case DOOR_ALARM:
		if((((player->psychskill.skills.electronics + player->psychskill.specializations.electronics) * 100) / MAX_SKILL) > difficulty){
			CanSeeAlarm = true;
		}
		if(CanSeeAlarm){
			strcat(buf, "-There's Some Sort of Alarm Attached to the Frame.");
		}
		else{
			strcat(buf, "-It Doesn't Seem to Have Any Attached Alarm System.");
		}
		
	}
	
	
	
	//FINAL PRINT OUT.
	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 1, FALSE);
	StaticDialogBox("Door Security Check", buf, "Press Enter", &globals.dboxparams);
}

void OpenDoor(PMAP map, PDOOR Door)
{
	if(Door->isopen){
		return;
	}
	Door->isopen = true;
	PMAPCELL Cell = FindMapCellByCoordinate(map, Door->location);
	if(!Cell){
		return;
	}
	Cell->type = OBJECT_SYM_FLOOR;
	PlayDoorOpenSound(Door);
	//NOW, WE NEED TO CREATE AN AUTO-CLOSE EVENT.
	SCRIPTEDEVENT CloseEvent;
	CloseEvent.TriggerTime = globals.globalmapstruct.totaltime;
	CloseEvent.TriggerTime.seconds += 10;
	CloseEvent.Context = (void*)Door;
	CloseEvent.Type = SCRIPTEDEVENT_TYPE_DOORCLOSE;
	AddScriptedEvent(&CloseEvent);
}

void CloseDoor(PMAP map, PPLAYER player, PDOOR Door)
{
	//CHECK FOR ANY OBSTRUCTIONS IN THE WAY AT ALL!
	if(CheckForANYObstructions(Door->location)){
		//SET UP ANOTHER SCRIPTED EVENT IN 5 MORE SECONDS.
		SCRIPTEDEVENT CloseEvent;
		CloseEvent.TriggerTime = globals.globalmapstruct.totaltime;
		CloseEvent.TriggerTime.seconds += 10;
		CloseEvent.Context = (void*)Door;
		CloseEvent.Type = SCRIPTEDEVENT_TYPE_DOORCLOSE;
		AddScriptedEvent(&CloseEvent);
	}
	else{
		Door->isopen = false;
	}
	PMAPCELL Cell = FindMapCellByCoordinate(map, Door->location);
	if(!Cell){
		return;
	}
	if(Door->vdoor){
		Cell->type = BUILDING_SYM_VDOOR;
	}
	else{
		Cell->type = BUILDING_SYM_HDOOR;
	}
	PlayDoorCloseSound(Door);
}

bool CheckForANYObstructions(POINT MapLocation)
{
	return false;
}

void PlayDoorOpenSound(PDOOR Door)
{
	playMIXsound(DOOR_OPEN_SOUND);
}

void PlayDoorCloseSound(PDOOR Door)
{
	playMIXsound(DOOR_CLOSE_SOUND);
}

void RaidArmory(PPLAYER player, PMAPARMORY armory)
{
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
	if(armory->destroyed){
		return;
	}
	playMIXsound(MAP_ARMORY_OPEN_SOUND);
	if(armory->weaponry->objcnt < 1 && armory->ammo.cliplist->objcnt < 1 && armory->ammo.roundtypes->objcnt < 1){
		StaticDialogBox("Armory Empty", "There's Nothing In This Armory.", "Press ENTER", &globals.dboxparams);
	}
	else{
		StaticDialogBox("Armory Raid", "Yeah Baby! ARMORY RAIIIIIIIID!", "Press ENTER", &globals.dboxparams);
	}
	playMIXsound(MAP_ARMORY_CLOSE_SOUND);
}

void ModifyPlayerPsych(PPLAYER player, PPSYCHSKILLSTRUCT ps)
{
	player->psychskill.psych.agression += ps->psych.agression;
	player->psychskill.psych.calmness += ps->psych.calmness;
	player->psychskill.psych.intelligence += ps->psych.intelligence;
	player->psychskill.psych.reflex += ps->psych.reflex;
	player->psychskill.psych.willpower += ps->psych.willpower;
}

bool DoesScriptedSequenceListHaveType(int Type)
{
	PLIST walker;
	walker = globals.globalmapstruct.scriptedevents;
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		if(walker->current->object.scriptedevent.Type == Type){
			return true;
		}
	}
	return false;
}

void InsertEnvironmentalEffect(PENVIRONSTRUCT Env)
{
	POTYPE Type;
	Env->id = globals.globalmapstruct.environmental->objcnt;
	Env->currentturnstoexpansion = 0;
	Type.environ = *Env;
	addtoList(globals.globalmapstruct.environmental, Type, CompareEnvironmentalID);
}

void UpdateEnvironmentalEffects(PMAP map, PPLAYER player)
{
	if(globals.globalmapstruct.environmental->objcnt < 1){
		return;
	}
	
	int NumToRemove = 0;
	int * Removals = (int*)malloc(sizeof(int) * globals.globalmapstruct.environmental->objcnt);
	PLIST walker = globals.globalmapstruct.environmental;
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		if(walker->current->object.environ.totalturnsleft <= 1){
			Removals[NumToRemove] = walker->current->object.environ.id;
			NumToRemove++;
		}
		else{
			walker->current->object.environ.totalturnsleft--;
		}
		if(walker->current->object.environ.currentturnstoexpansion == walker->current->object.environ.turnsperexpansion && walker->current->object.environ.turnsperexpansion != -1){
			walker->current->object.environ.currentradius+= walker->current->object.environ.radiusperexpansion;
			walker->current->object.environ.currentturnstoexpansion = 0;
		}
		else{
			walker->current->object.environ.currentturnstoexpansion++;
		}
	}
	
	if(NumToRemove)
	{
		POTYPE Finder;
		PPOTYPE Found;
		for(int i = 0; i < NumToRemove; i++){
			Finder.environ.id = Removals[i];
			Found = findObject(globals.globalmapstruct.environmental, &Finder, CompareEnvironmentalID);
			if(Found){
				deleteObject(globals.globalmapstruct.environmental, Found);
			}
		}
		ReorderEnvironmentalEffectsList();
	}
	
	for(int q = 0; q < globals.globalmapstruct.numenemies; q++)
	{
		if(globals.globalmapstruct.enemies[q].flashturnsleft >= 0)
		{
			globals.globalmapstruct.enemies[q].flashturnsleft--;
		}
	}
	
	if(player->flashturnsleft >= 0){
		player->flashturnsleft--;
	}
}

void DestroyObjectsInRadius(PMAP map, PPLAYER player, POINT Origin, int Radius, int PowerPercentage, int Falloff)
{
	PMAPCELL Cell;
	int XStart;
	int XEnd;
	int YStart;
	int YEnd;
	int XCounter;
	int YCounter;
	POINT MapPt;
	XStart = Origin.x - Radius;
	XStart = CapInclusive(XStart, 1, Origin.x);
	XEnd = Origin.x + Radius;
	XEnd = CapInclusive(XEnd, Origin.x, map->width - 2);
	YStart = Origin.y - Radius;
	YStart = CapInclusive(YStart, 1, Origin.y);
	YEnd = Origin.y + Radius;
	YEnd = CapInclusive(YEnd, Origin.y, map->height - 2);
	int PowerY = PowerPercentage - (Falloff * Radius / 2);
	int PowerX = PowerY;
	
	for(YCounter = YStart; YCounter < YEnd; YCounter++)
	{
		if(YCounter > (Radius / 2) + YStart)
			PowerY -= Falloff;
		else
			PowerY += Falloff;
		for(XCounter = XStart; XCounter < XEnd; XCounter++)
		{
			MapPt.x = XCounter;
			MapPt.y = YCounter;
			Cell = FindMapCellByCoordinate(map, MapPt);
			if(Cell)
			{
				DestroyGameObjectsInCell(Cell, (PowerY + PowerX) / 2);
			}
			if(XCounter > (Radius / 2) + XStart)
				PowerX -= Falloff;
			else
				PowerX += Falloff;
		}
	}
}

void DestroyGameObjectsInCell(PMAPCELL Cell, int PowerPercentage)
{
	PDOOR Door;
	PMAPARMORY Armory;
	//ARCHITECTURE SECTION
	switch(Cell->type)
	{
	case BUILDING_SYM_HDOOR:
	case BUILDING_SYM_VDOOR:
		//DOORS
	Door = GetDoorInfoFromCell(Cell);
	if(Door && DoorExplosionTest(Door, PowerPercentage))
	{
		Door->isopen = true;
		Cell->type = OBJECT_SYM_FLOOR;
	}
		break;
	case BUILDING_SYM_BLCORNER:
	case BUILDING_SYM_TRCORNER:
	case BUILDING_SYM_BRCORNER:
	case BUILDING_SYM_TLCORNER:
	case BUILDING_SYM_VWALL:
	case BUILDING_SYM_HWALL:
		if(PowerPercentage > 80 && RandomPercentage(PowerPercentage)){
		Cell->type = OBJECT_SYM_GROUND;
		}
		break;
	case OBJECT_SYM_LOOBSTRUCT:
	case OBJECT_SYM_HIOBSTRUCT:
		if(PowerPercentage > 60 && RandomPercentage(PowerPercentage)){
		Cell->type = OBJECT_SYM_GROUND;
		}
		break;
	}

	//ARMORIES
	Armory = GetMapCellArmory(Cell);
	if(Armory && PowerPercentage > 75)
	{
		Armory->destroyed = true;
	}
}

void ReorderEnvironmentalEffectsList()
{
	int counter = 0;
	if(globals.globalmapstruct.environmental->objcnt < 1){
		return;
	}
	PLIST walker = globals.globalmapstruct.environmental;
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		walker->current->object.environ.id = counter;
		counter++;
	}
}

int DoesEnvironmentalEffectExistAtPoint(POINT Point, int Type) //TYPE CAN BE -1 FOR "ALL"
{
	if(globals.globalmapstruct.environmental->objcnt < 1){
		return 0;
	}
	PLIST walker = globals.globalmapstruct.environmental;
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		PENVIRONSTRUCT ev = &walker->current->object.environ;
		if(Point.x >= ev->origin.x - ev->currentradius && Point.x < ev->origin.x + ev->currentradius && Point.y >= ev->origin.y - ev->currentradius && Point.y < ev->origin.y + ev->currentradius){
			if(Type == -1){
				return ev->type;
			}
			else if(Type == ev->type){
				return ev->type;
			}
		}
	}
	return 0;
}

int CompareEnvironmentalID(PPOTYPE itm1,PPOTYPE itm2) 
{
	if(itm1->environ.id == itm2->environ.id)
		return (0);
	return (1);
}

bool SetBomb(PPLAYER player, PMAP map, PMAPCELL CellToSetIn, PGIZMO BombToSet)
{
	//CHECK IF CELL WITHIN ACCEPTABLE BOUNDS.
	
	
	//CHECK IF GIZMO WITHIN ACCEPTABLE BOUNDS.
	if(!BombToSet || BombToSet->isgrenade){
		return false;
	}
	
	char * RemoteChoices[REMOTE_C4_RADIO_CHOICES] = {"900GHz", "1200GHz", "1500GHz"};
	int RemoteNumbers[REMOTE_C4_RADIO_CHOICES] = {1, 2, 3};
	char * ElectronicChoices[ELECTRONIC_C4_VOLTAGE_CHOICES] = {"15V", "110V"};
	int ElectronicNumbers[ELECTRONIC_C4_VOLTAGE_CHOICES] = {1, 2};
	char * ProximityChoices[PROXIMITY_C4_RANGE_CHOICES] = {"5 ft", "10 ft", "15 ft", "20 ft", "25 ft"};
	int ProximityNumbers[PROXIMITY_C4_RANGE_CHOICES] = {0, 1, 2, 3, 4};
	char * TimeChoices[TIMED_C4_MINUTE_CHOICES] = {"1 Min", "5 Min", "20 Min", "60 Min", "90 Min"};
	int TimeNumbers[TIMED_C4_MINUTE_CHOICES] = {1, 5, 20, 60, 90};
	
	//THESE ARE TO BE SET BY CHECKING THE BombToSet->Type.
	int * OurNumbers;
	char** OurChoices;
	int MaxChoices;
	int CurrentChoice = 0;
	
	USHORT key;
	
	
	
	switch(BombToSet->unit.device.devicetype)
	{
	case GIZMO_TYPE_C4TIMED:
		OurChoices = TimeChoices;
		OurNumbers = TimeNumbers;
		MaxChoices = TIMED_C4_MINUTE_CHOICES;
		break;
	case GIZMO_TYPE_C4SWITCH:
		OurChoices = RemoteChoices;
		OurNumbers = RemoteNumbers;
		MaxChoices = REMOTE_C4_RADIO_CHOICES;
		break;
	case GIZMO_TYPE_C4ELECTRONIC:
		OurChoices = ElectronicChoices;
		OurNumbers = ElectronicNumbers;
		MaxChoices = ELECTRONIC_C4_VOLTAGE_CHOICES;
		break;
	case GIZMO_TYPE_C4PROXIMITY:
		OurChoices = ProximityChoices;
		OurNumbers = ProximityNumbers;
		MaxChoices = PROXIMITY_C4_RANGE_CHOICES;
		break;
	default:
		return false;
	}
	
	//I GUESS IT'S LEGIT.
	SaveMainSnapShot();
	DrawC4Shell(OurChoices[CurrentChoice], false, true);
	
	bool BombActive = false;
	while(!BombActive){
		key = waitforVkey();
		switch(key){
		case VK_ESCAPE:
			RestoreMainSnapShot();
			return false;
		case VK_DOWN:
		case VK_LEFT:
			//PLAY SWITCH SOUND
			playMIXsound(C4_TYPE_SELECT_SOUND);
			CurrentChoice--;
			CurrentChoice = CapInclusive(CurrentChoice, 0, MaxChoices-1);
			DrawC4Shell(OurChoices[CurrentChoice], false, false);
			break;
			
		case VK_UP:
		case VK_RIGHT:
			//PLAY SWITCH SOUND
			playMIXsound(C4_TYPE_SELECT_SOUND);
			CurrentChoice++;
			CurrentChoice = CapInclusive(CurrentChoice, 0, MaxChoices-1);
			DrawC4Shell(OurChoices[CurrentChoice], false, false);
			break;
		case VK_RETURN:
			BombActive = true;
			break;
		}
	}
	
	//PLAY ACTIVATE SOUND.
	playMIXsound(C4_ARM_SOUND);
	DrawC4Shell(OurChoices[CurrentChoice], true, true);
	Sleep(1000);
	
	//THE BOMB IS A "GO". AT THIS POINT, WE MUST HANDLE EACH OF THE SPECIAL CASES OF THE BOMB TYPES.
	SCRIPTEDEVENT Event;
	POTYPE GizmoFinder;
	GizmoFinder.gizmo.index = BombToSet->index;
	PPOTYPE GizmoToRemove;
	
	POTYPE GizmoToAdd;
	PPOTYPE RefoundGizmo;
	GizmoToAdd.gizmo.isgrenade = false;
	GizmoToAdd.gizmo.index = globals.globalmapstruct.gizmos->objcnt;
	GizmoToAdd.gizmo.unit.device.devicetype = BombToSet->unit.device.devicetype;
	GizmoToAdd.gizmo.unit.device.active = true;
	GizmoToAdd.gizmo.WorldPoint.x = CellToSetIn->xcoord;
	GizmoToAdd.gizmo.WorldPoint.y = CellToSetIn->ycoord;
	
	GizmoToRemove = findObject(player->p_gizmos, &GizmoFinder, CompareGizmoIndex);
	if(GizmoToRemove){
		deleteObject(player->p_gizmos, GizmoToRemove);
		ReorderGizmoListIndex(player->p_gizmos);
	}
	
	//NOW, SPECIAL SHIZZAT.
	switch(GizmoToAdd.gizmo.unit.device.devicetype)
	{
	case GIZMO_TYPE_C4TIMED:
		addtoList(globals.globalmapstruct.gizmos, GizmoToAdd, CompareGizmoIndex);
		RefoundGizmo = findObject(globals.globalmapstruct.gizmos, &GizmoToAdd, CompareGizmoIndex);
		Event.Type = SCRIPTEDEVENT_TYPE_TIMEDBOMBEXPLODE;
		Event.TriggerTime = globals.globalmapstruct.totaltime;
		Event.TriggerTime.minutes += OurNumbers[CurrentChoice];
		Event.Context = (void*)RefoundGizmo;
		Event.ExtraDataOne = RefoundGizmo->gizmo.index;
		calibratemissionchrono(&Event.TriggerTime);
		AddScriptedEvent(&Event);
		break;
	case GIZMO_TYPE_C4SWITCH:
		GizmoToAdd.gizmo.AdditionalNumber = OurNumbers[CurrentChoice];
		addtoList(globals.globalmapstruct.gizmos, GizmoToAdd, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_C4ELECTRONIC:
		GizmoToAdd.gizmo.AdditionalNumber = OurNumbers[CurrentChoice];
		GizmoToAdd.gizmo.Context = GetMapCellVehicle(CellToSetIn);
		addtoList(globals.globalmapstruct.gizmos, GizmoToAdd, CompareGizmoIndex);
		break;
	case GIZMO_TYPE_C4PROXIMITY:
		GizmoToAdd.gizmo.AdditionalNumber = OurNumbers[CurrentChoice];
		GizmoToAdd.gizmo.AdditionalBool = false;
		GizmoToAdd.gizmo.AdditionalPoint.x = player->location->xcoord;
		GizmoToAdd.gizmo.AdditionalPoint.y = player->location->ycoord;
		addtoList(globals.globalmapstruct.gizmos, GizmoToAdd, CompareGizmoIndex);
		RefoundGizmo = findObject(globals.globalmapstruct.gizmos, &GizmoToAdd, CompareGizmoIndex);
		Event.Type = SCRIPTEDEVENT_TYPE_PROXBOMBACTIVATE;
		Event.TriggerTime = globals.globalmapstruct.totaltime;
		Event.TriggerTime.seconds += 15;
		Event.Context = (void*)RefoundGizmo;
		calibratemissionchrono(&Event.TriggerTime);
		AddScriptedEvent(&Event);
		break;
	}
	
	RestoreMainSnapShot();
	return true;
}

void DetonateAllBombsOnRemoteFrequency(PMAP map, PPLAYER player, int Freq)
{
	PLIST walker = globals.globalmapstruct.gizmos;
	if(walker->objcnt < 1){
		return;
	}
	PPOTYPE* indexes = (PPOTYPE*)malloc(sizeof(PPOTYPE) * (walker->objcnt + 1));
	int NumIndexes = 0;
	
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		if(walker->current->object.gizmo.unit.device.devicetype == GIZMO_TYPE_C4SWITCH && walker->current->object.gizmo.AdditionalNumber == Freq)
		{
			indexes[NumIndexes] = &walker->current->object;
			NumIndexes++;
		}
	}
	
	for(int i = 0; i < NumIndexes; i++)
	{
		C4Detonate(indexes[i]->gizmo.index, map, player);
		deleteObject(globals.globalmapstruct.gizmos, indexes[i]);
	}
	ReorderGizmoListIndex(globals.globalmapstruct.gizmos);
}

void C4Detonate(int Index, PMAP map, PPLAYER player)
{
	POTYPE Finder;
	PPOTYPE Found;
	Finder.gizmo.index = Index;
	Found = findObject(globals.globalmapstruct.gizmos, &Finder, CompareGizmoIndex);
	if(Found){
		DebugExplosion(player, map, Found->gizmo.WorldPoint);
	}
}

void CheckProximityBombs(PMAP map, PPLAYER player)
{
	PLIST BombWalker;
	BombWalker = globals.globalmapstruct.gizmos;
	
	if(!BombWalker || BombWalker->objcnt < 1){
		return;
	}
	
	RECT Rad;
	POTYPE Finder;
	PPOTYPE Found;
	int EnemyC;
	int RadFromCenter;
	bool BombGoesOff = false;
	int TotalBombsToGoOff = 0;
	int* BombsToGoOff = (int*)malloc(sizeof(int) * BombWalker->objcnt);
	
	
	for(BombWalker->current = BombWalker->head; BombWalker->current != NULL; BombWalker->current = BombWalker->current->nextnode)
	{
		if(BombWalker->current->object.gizmo.unit.device.devicetype == GIZMO_TYPE_C4PROXIMITY && BombWalker->current->object.gizmo.AdditionalBool == true) //IF IT'S ALIVE AND SCANNING...
		{
			RadFromCenter = BombWalker->current->object.gizmo.AdditionalNumber + 1;
			//HERE WE HAVE TO RUN THE VISIBILITY CHECK. FOR NOW WE CHECK ANYWHERE IN THE RECT.
			Rad.top = CapInclusive(BombWalker->current->object.gizmo.AdditionalPoint.y - RadFromCenter, 2,BombWalker->current->object.gizmo.AdditionalPoint.y);
			Rad.left = CapInclusive(BombWalker->current->object.gizmo.AdditionalPoint.x - RadFromCenter, 2,BombWalker->current->object.gizmo.AdditionalPoint.x);
			Rad.right = CapInclusive(BombWalker->current->object.gizmo.AdditionalPoint.x + RadFromCenter, BombWalker->current->object.gizmo.AdditionalPoint.x, map->width - 3);
			Rad.bottom = CapInclusive(BombWalker->current->object.gizmo.AdditionalPoint.y + RadFromCenter, BombWalker->current->object.gizmo.AdditionalPoint.y, map->height - 3);
		}
		
		POINT CP;
		CP.x = player->location->xcoord;
		CP.y = player->location->ycoord;
		
		if(IsPointInRect(Rad, CP)){
			BombsToGoOff[TotalBombsToGoOff] = BombWalker->current->object.gizmo.index;
			TotalBombsToGoOff++;
		}
		
		else{
			for(EnemyC = 0; EnemyC < globals.globalmapstruct.numenemies; EnemyC++){
				CP.x = globals.globalmapstruct.enemies[EnemyC].location->xcoord;
				CP.y = globals.globalmapstruct.enemies[EnemyC].location->ycoord;
				if(IsPointInRect(Rad, CP)){
					BombsToGoOff[TotalBombsToGoOff] = BombWalker->current->object.gizmo.index;
					TotalBombsToGoOff++;
				}
			}
		}
	}
	
	for(int i = 0; i < TotalBombsToGoOff; i++){
		Finder.gizmo.index = BombsToGoOff[i];
		Found = findObject(globals.globalmapstruct.gizmos, &Finder, CompareGizmoIndex);
		if(Found){
			playSYNCsound(C4_PROXIMITY_YOURESCREWED);
			C4Detonate(BombsToGoOff[i], map, player);
			deleteObject(globals.globalmapstruct.gizmos, Found);
		}
		
		
		
		InsertHistoryString(player, "proximity explosive detonated", false);
	}
	if(TotalBombsToGoOff > 0){
		ReorderGizmoListIndex(globals.globalmapstruct.gizmos);
	}
	free(BombsToGoOff);
}

bool DoesMapCellHaveAnyItems(PMAPCELL Cell)
{
	PLIST WWalker = globals.globalmapstruct.weapons;
	PLIST CWalker = globals.globalmapstruct.clips;
	PLIST SRWalker = globals.globalmapstruct.strayrounds;

	for(WWalker->current = WWalker->head; WWalker->current != NULL; WWalker->current = WWalker->current->nextnode){
		if(WWalker->current->object.weapon.WorldPoint.x == Cell->xcoord && WWalker->current->object.weapon.WorldPoint.y == Cell->ycoord){
			return true;
		}
	}

	for(CWalker->current = CWalker->head; CWalker->current != NULL; CWalker->current = CWalker->current->nextnode){
		if(CWalker->current->object.clip.WorldPoint.x == Cell->xcoord && CWalker->current->object.clip.WorldPoint.y == Cell->ycoord){
			return true;
		}
	}

	for(SRWalker->current = SRWalker->head; SRWalker->current != NULL; SRWalker->current = SRWalker->current->nextnode){
		if(SRWalker->current->object.rounds.WorldPoint.x == Cell->xcoord && SRWalker->current->object.rounds.WorldPoint.y == Cell->ycoord){
			return true;
		}
	}
	return false;

}

bool DoorExplosionTest(PDOOR Door, int PowerPercentage)
{
	switch(Door->type)
	{
	default:
	case DOOR_GLASS:
		if(PowerPercentage < 7){
			return false;
		}
		return true;
	case DOOR_WOOD:
		if(PowerPercentage < 25){
			return false;
		}
		else{
			return RandomPercentage(PowerPercentage + 10);
		}
	case DOOR_METAL:
		if(PowerPercentage < 50){
			return false;
		}
		else{
			return RandomPercentage(PowerPercentage);
		}
	case DOOR_REMETAL:
		if(PowerPercentage < 70){
			return false;
		}
		return RandomPercentage(PowerPercentage - 10);
	}
}


bool HasThrowableWeapon(PPLAYER player)
{
	if(player->currentweapon){
		if(player->currentweapon->attacktype & WEAP_ATT_TYPE_THROW)
			return true;
		return false;
	}
	return AreThereActiveGrenadesInInventory(player);
}