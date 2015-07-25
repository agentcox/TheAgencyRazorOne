/***********************************************************
;						 Project Title: The Agency : Razor One - Text Based Prototype 
;									  Team: Charles Cox
;				Module Filename: mapcreator.h
;				Module Synopsis: The map creator library, with functionality to access the 
				map structure.
;
;								  Version: First Draft
;					  Revision Date: 03/02/2000
;						   Created By: Charles Cox (Design, Implementation)
************************************************************/
#include "agencyinclude.h"
#include "maplib.h"


//MAIN REMOVED TO ALLOW FOR INCLUSION OF THIS FILE INTO A LIBRARY FILE FOR USE WITH MAP EDITOR
/*
void main()
{
	
	srand((int)time(NULL));
	PMAP gamemap;
	PMAPCELL temp;
	
	

	
	gamemap = initMap(70, 20);
		CreateShittyPracticeMap(gamemap);
	drawMap(gamemap);
	terminateMap(gamemap);
}
*/


/***********************************************************
;					 Function Name: initMap
;								 Synopsis: Initializes the map data structure, mallocs the map cells
based on width and height.
;
;				Input Parameters: int  		-	width:	width of the new map
int			-	height:	height of the new map

  ;	   		 Output Parameters: None
  ;									Return: Pointer to the MAP structure created.
***********************************************************/
PMAP initMap(int width, int height)
{
	PMAP map;

	
	int maxcells = height * width;
	int i = 0;
	
	int heightcounter;
	int widthcounter;
	
	map = (PMAP) malloc(sizeof(MAP));
	map->height = height;
	map->width = width;
	
	map->mapcells = (PMAPCELL) malloc(sizeof(MAPCELL) * width * height);
	
	for (heightcounter = 0; heightcounter < height; heightcounter++){
		for (widthcounter = 0; widthcounter < width; widthcounter++){
			map->mapcells[i].type = OBJECT_SYM_GROUND;
			map->mapcells[i].xcoord = widthcounter;
			map->mapcells[i].ycoord = heightcounter;
			i++;
		}
	}

	return map;
}


/***********************************************************
;					 Function Name: terminateMap
;								 Synopsis: Frees the map.
;
;				Input Parameters: PMAP	-	the map to free
;	   		 Output Parameters: Frees the map.
;									Return: None
***********************************************************/
void terminateMap(PMAP map)
{
	free(map->mapcells);
	
	free(map);
}



/***********************************************************
;					 Function Name: createVwall
;								 Synopsis: creates a series of vertical wall segments from one map coordinate
;						to a given y-coordinate location.
;				Input Parameters:  POINT	-	startcoord	:	specifies the starting map coordinate to edit from.
int				-	endy			:	specifies the ending y-coordinate (the x-coordinate is fixed to prevent misuse).
;	   		 Output Parameters: Edits the map structure.
;									Return:	None.
***********************************************************/
void createVwall(PMAP map, POINT startcoord, int endy)
{

}

/***********************************************************
;					 Function Name: createVwallsection
;								 Synopsis: creates a vertical wall segment at a given map coordinate.
;						
;				Input Parameters:  POINT	-	startcoord	:	specifies the starting map coordinate to edit to.
;	   		 Output Parameters: Edits the map structure.
;									Return:	None.
***********************************************************/
void createVwallsection(PMAP map, POINT startcoord)
{
PMAPCELL cell;
cell = FindMapCellByCoordinate(map, startcoord);

cell->type = BUILDING_SYM_VWALL;

}

/***********************************************************
;					 Function Name: createHwall
;								 Synopsis: creates a series of horizontal wall segments from one map coordinate
;						to a given x-coordinate location.
;				Input Parameters:  POINT	-	startcoord	:	specifies the starting map coordinate to edit from.
int				-	endx			:	specifies the ending x-coordinate (the y-coordinate is fixed to prevent misuse).
;	   		 Output Parameters: Edits the map structure.
;									Return:	None.
***********************************************************/
void createHwall(PMAP map, POINT startcoord, int endx)
{
}

/***********************************************************
;					 Function Name: createHwallsection
;								 Synopsis: creates a horizontal wall segment at a given map coordinate.
;						
;				Input Parameters:  POINT	-	startcoord	:	specifies the starting map coordinate to edit.
;	   		 Output Parameters: Edits the map structure.
;									Return:	None.
***********************************************************/
void createHwallsection(PMAP map, POINT startcoord)
{
	PMAPCELL cell;
cell = FindMapCellByCoordinate(map, startcoord);

cell->type = BUILDING_SYM_HWALL;

}

/***********************************************************
;					 Function Name: createWallCorner
;								 Synopsis: Creates a wall corner section, the type and coordinate for
drawing is specified.
;
;				Input Parameters: POINT	-	drawcoord:	the coordinate to edit.
int			-	type: WALL_CORNER_UL, WALL_CORNER_LL, WALL_CORNER_UR, WALL_CORNER_LR
;	   		 Output Parameters: Modifies the map structure
;									Return: None
***********************************************************/
/*
void createWallCorner(PMAP map, POINT drawcoord, int type)
{
	PMAPCELL cell;
cell = FindMapCellByCoordinate(map, startcoord);

switch (type)
{
case WALL_CORNER_UL:
	cell->type = BUILDING_SYM_TLCORNER;
	break;
case WALL_CORNER_LL:
	cell->type = BUILDING_SYM_BLCORNER;
	break;
case WALL_CORNER_UR:
	cell->type = BUILDING_SYM_TRCORNER;
	break;
case WALL_CORNER_LR:
	cell->type = BUILDING_SYM_BRCORNER;
	break;
}
}
*/
/***********************************************************
;					 Function Name: createFence
;								 Synopsis: creates a rectangular fence section. to be used ONLY to
ring the perimeter of the map.
;
;				Input Parameters: the MAP to modify, the RECT specifying the coordinates to start and end the fence.
;	   		 Output Parameters: Modifies the map structure.
;									Return: None.
***********************************************************/
void createFence(PMAP map)
{
	POINT drawcoord;
	PMAPCELL temp;
	RECT rect;
	rect.top = 0;
	rect.bottom = map->height;
	rect.left =0;
	rect.right = map->width;

	RECT* fencerect = &rect;
	
	drawcoord.y = fencerect->top;
	
	//DRAW THE TOP SIDE
	for (drawcoord.x = fencerect->left; drawcoord.x < fencerect->right;drawcoord.x++){
		temp = FindMapCellByCoordinate(map, drawcoord);
		if (temp == NULL){
			assert(!"CreateFence FAILED - FindMapCellByCoordinate returned NULL");			}
		temp->type = OBJECT_SYM_FENCE;
	}
	
	//NOW THE RIGHT SIDE
	drawcoord.x--; //Move it back one.
	for (drawcoord.y =  fencerect->top; drawcoord.y < fencerect->bottom;drawcoord.y++){
		temp = FindMapCellByCoordinate(map, drawcoord);
		if (temp == NULL){
			assert(!"CreateFence FAILED - FindMapCellByCoordinate returned NULL");			}
		temp->type = OBJECT_SYM_FENCE;
	}
	drawcoord.y--; //Move it back one.
	//NOW THE BOTTOM SIDE
	for (drawcoord.x = fencerect->right - 1; drawcoord.x > fencerect->left;drawcoord.x--){
		temp = FindMapCellByCoordinate(map, drawcoord);
		if (temp == NULL){
			assert(!"CreateFence FAILED - FindMapCellByCoordinate returned NULL");			}
		temp->type = OBJECT_SYM_FENCE;
	}
	
	//NOW THE LEFT SIDE
	for (drawcoord.y = fencerect->bottom - 1; drawcoord.y > fencerect->top;drawcoord.y--){
		temp = FindMapCellByCoordinate(map, drawcoord);
		if (temp == NULL){
			assert(!"CreateFence FAILED - FindMapCellByCoordinate returned NULL");
		}
		temp->type = OBJECT_SYM_FENCE;
	}
	
	
}


/***********************************************************
;					 Function Name: createHGate
;								 Synopsis: creates a horizontal gate cell to replace a section of fence.
The fence section must exist BEFORE the gate is placed.
;
;				Input Parameters: POINT	drawcoord	-	the coordinate to draw the gate in.
;	   		 Output Parameters: Modifes the map structure.
;									Return: None
***********************************************************/
void createHGate(PMAP map, POINT drawcoord)
{
}

/***********************************************************
;					 Function Name: createVGate
;								 Synopsis: creates a vertical  gate cell to replace a section of fence.
The fence section must exist BEFORE the gate is placed.
;
;				Input Parameters: POINT	drawcoord	-	the coordinate to draw the gate in.
;	   		 Output Parameters: Modifes the map structure.
;									Return: None
***********************************************************/
void createVGate(PMAP map, POINT drawcoord)
{
}

/***********************************************************
;					 Function Name: createObstruction
;								 Synopsis: Creates an obstruction on the map, the type and coordinate are specified
;
;				Input Parameters: POINT	-	drawcoord:	The coordinate to edit.
int			-	type: OBSTRUCTION_HI, OBSTRUCTION_LO

  ;	   		 Output Parameters: edits the map structure
  ;									Return: None.
***********************************************************/
void createObstruction(PMAP map, POINT drawcoord, int type)
{
}


/***********************************************************
;					 Function Name: createRZ
;								 Synopsis: Creates a Recovery Zone at the given coordinate in the map structure.
;
;				Input Parameters: PMAP	-	map: The map to edit.
													POINT	-	drawcoord: The coordinates to create at.
;	   		 Output Parameters: Edits the map structure.
;									Return: 0 if ok, -1 if RZ already exists on map.
***********************************************************/
int createRZ(PMAP map, POINT drawcoord)
{
	PMAPCELL temp;
	//CHECK FOR A DUPLICATE RZ.
	temp = FindRZCell(map);
	if (temp != NULL){
		return -1; //There's already an RZ!
	}

	temp = FindMapCellByCoordinate(map, drawcoord);

	if (temp == NULL){
		assert(!"createRZ failed - FindMapCellByCoordinate = NULL - assume bad coordinates.");
	}

	map->rz.x = drawcoord.x;
	map->rz.y = drawcoord.y;
	return 0;

}


/***********************************************************
;					 Function Name: createPZ
;								 Synopsis: Creates a Primary Zone at the given coordinate in the map structure.
;
;				Input Parameters: PMAP	-	map: The map to edit.
													POINT	-	drawcoord: The coordinates to create at.
;	   		 Output Parameters: Edits the map structure.
;									Return: 0 if ok, -1 if PZ already exists on map.
***********************************************************/
int createPZ(PMAP map, POINT drawcoord)
{
		PMAPCELL temp;
	//CHECK FOR A DUPLICATE PZ.
	temp = FindPZCell(map);
	if (temp != NULL){
		return -1; //There's already a PZ!
	}

	temp = FindMapCellByCoordinate(map, drawcoord);

	if (temp == NULL){
		assert(!"createPZ failed - FindMapCellByCoordinate = NULL - assume bad coordinates.");
	}

	map->pz.x = drawcoord.x;
	map->pz.y = drawcoord.y;
	return 0;
}


/***********************************************************
;					 Function Name: CreateShittyPracticeMap
;								 Synopsis: Creates a fairly shitty practice map.
;				---------------FOR DEBUG / TESTING PURPOSES ONLY-------------------
;				Input Parameters: The map to modify
;	   		 Output Parameters: Modifies the map structure.
;									Return: NUTHIN.
***********************************************************/
void CreateShittyPracticeMap(PMAP map)
{
	/************************************************************
	; Initialize the fence around the perimeter.
	************************************************************/
	RECT fencerect;

	fencerect.top = 0;
	fencerect.left = 0;
	fencerect.bottom = map->height;
	fencerect.right = map->width;
	createFence(map);


	/************************************************************
	; Let's make a random RZ on the edge of the map inside the fence.
	************************************************************/
	CreateRandomRZ(map);




}


/***********************************************************
;					 Function Name: CreateRandomRZ
;								 Synopsis: Uh...generally speaking it creates...a random RZ.
;
;				Input Parameters: PMAP map
;	   		 Output Parameters: Modifies the map structure
;									Return: Void
***********************************************************/
void CreateRandomRZ(PMAP map)
{
	POINT drawcoord;
	int i = rand()%4;
		switch (i){
		case 0: //On the left of the map
			drawcoord.x = 1; //Avoid the fence
			drawcoord.y = random(1, map->height - 1) ;//Avoid the fence.
			createRZ(map, drawcoord);
			break;
		case 1: //On the right of the map.
			drawcoord.x = map->width - 2; //Avoid the fence
			drawcoord.y = random(1, map->height - 1) ;//Avoid the fence.
			createRZ(map, drawcoord);
			break;
		case 2: //On the top of the map.
			drawcoord.x = random(1, map->width - 1); //Avoid the fence
			drawcoord.y =  1;//Avoid the fence.
			createRZ(map, drawcoord);
			break;
		case 3: //On the bottom of the map.
			drawcoord.x = random(1, map->width - 1); //Avoid the fence
			drawcoord.y =  map->height - 2;//Avoid the fence.
			createRZ(map, drawcoord);
			break;
	}
}

/***********************************************************
;					 Function Name: FindMapCellByCoordinate
;								 Synopsis: Returns a map cell in a given map structure that has the 
coordinates that correspond to the given coordinates.
;
;				Input Parameters: PMAP	-	map: the map structure to search.
POINT	-	coordtoget: the coordinates to reference the cell by.
;	   		 Output Parameters: None.
;									Return: The found map cell, or NULL if not found
***********************************************************/
PMAPCELL FindMapCellByCoordinate(PMAP map, POINT coordtoget)
{
//	int i;
	int arrayindex = 0;
	if(coordtoget.x < 0 && coordtoget.x >= map->width - 1 || coordtoget.y < 0 || coordtoget.y >= map->height){
		return NULL;
	}
	arrayindex += coordtoget.x;
	arrayindex += coordtoget.y * map->width;

	return &map->mapcells[arrayindex];
}


/***********************************************************
;					 Function Name: drawMap
;								 Synopsis: Draws the entire compound map to the screen, starting at 0,0.
;					It cycles through the map structure and prints out the character associated with each of
the cell's attributes. Utilizes drawing priority which is defined inside this function.
;				Input Parameters: PMAP	-	map: the map to draw.
;	   		 Output Parameters: Screen output
;									Return: None.
***********************************************************/
void drawMap(PMAP map)
{
	int i;
	int widthcounter;
	int heightcounter;
	
	zerocursor();
	
	for (i = 0; i < (map->height* map->width); i++){
		for (heightcounter = 0 ; heightcounter < map->height; heightcounter++){
			
			for (widthcounter = 0; widthcounter < map->width; widthcounter++){
			priorityDrawCell(&map->mapcells[i]);
				i++;
			}
			printf("\n");
		}
	}
}


/***********************************************************
;					 Function Name: priorityDrawCell
;								 Synopsis: This function contains the priority drawing order for the map
						symbols and MUST BE USED TO DRAW ALL MAP GRAPHICS.
;
;				Input Parameters: The map cell to draw.
;	   		 Output Parameters: Screen Output.
;									Return: None.
***********************************************************/
void priorityDrawCell(PMAPCELL mapcell)
{
		printf("%c", mapcell->type);
}


/***********************************************************
;					 Function Name: FindRZCell
;								 Synopsis: Finds the unique Recovery Zone cell in the given map
								and returns that cell.
;
;				Input Parameters: The map to search.
;	   		 Output Parameters: None
;									Return: Returns the RZ cell, or NULL if none exists.
***********************************************************/
PMAPCELL FindRZCell(PMAP map)
{
	for (int i = 0; i < (map->height * map->width); i++){
		if (map->mapcells[i].xcoord == map->rz.x && map->mapcells[i].ycoord == map->rz.y)
			return &map->mapcells[i];
	}
	return NULL;
}

/***********************************************************
;					 Function Name: FindPZCell
;								 Synopsis: Finds the unique Primary Zone cell in the given map
								and returns that cell.
;
;				Input Parameters: The map to search.
;	   		 Output Parameters: None
;									Return: Returns the PZ cell, or NULL if none exists.
***********************************************************/
PMAPCELL FindPZCell(PMAP map)
{
		for (int i = 0; i < (map->height * map->width); i++){
		if (map->mapcells[i].xcoord == map->pz.x && map->mapcells[i].ycoord == map->pz.y)
			return &map->mapcells[i];
	}
	return NULL;
}

int ReturnGlobalPositionOfObject(POINT Point, PMAP map)
{
	int horiz;
	int vert;

	if(Point.x < map->width / 3){
		horiz = -1;
	}
	else if(Point.x > 2 * map->width / 3){
		horiz = 1;
	}
	else{
		horiz = 0;
	}

	if(Point.y < map->height / 3){
		vert = -1;
	}
	else if (Point.y > 2 * map->height / 3){
		vert = 1;
	}
	else{
		vert = 0;
	}

	if(!vert && !horiz){
			return GLOBAL_DIRECTION_CENTRAL;
		}
	if(!horiz)
	{
		
		if(vert < 0){
			return GLOBAL_DIRECTION_NORTH;
		}
		if(vert > 0){
			return GLOBAL_DIRECTION_SOUTH;
		}
	}

	if(!vert){
		if(horiz < 0){
			return GLOBAL_DIRECTION_WEST;
		}
		if(horiz > 0){
			return GLOBAL_DIRECTION_EAST;
		}
	}

	if(horiz < 0){
		if(vert > 0){
			return GLOBAL_DIRECTION_SOUTHWEST;
		}
		if(vert < 0){
			return GLOBAL_DIRECTION_NORTHWEST;
		}
	}

	if(horiz > 0){
		if(vert > 0){
			return GLOBAL_DIRECTION_SOUTHEAST;
		}
		if(vert < 0){
			return GLOBAL_DIRECTION_NORTHEAST;
		}
	}
	return GLOBAL_DIRECTION_CENTRAL;
}

int ReturnGlobalPositionOpposite(int Position)
{
	switch(Position)
	{
	case GLOBAL_DIRECTION_NORTH:
		return GLOBAL_DIRECTION_SOUTH;
	case GLOBAL_DIRECTION_SOUTH:
		return GLOBAL_DIRECTION_NORTH;
	case GLOBAL_DIRECTION_EAST:
		return GLOBAL_DIRECTION_WEST;
	case GLOBAL_DIRECTION_WEST:
		return GLOBAL_DIRECTION_EAST;
	case GLOBAL_DIRECTION_NORTHEAST:
		return GLOBAL_DIRECTION_SOUTHWEST;
	case GLOBAL_DIRECTION_SOUTHEAST:
		return GLOBAL_DIRECTION_NORTHWEST;
	case GLOBAL_DIRECTION_NORTHWEST:
		return GLOBAL_DIRECTION_SOUTHEAST;
	case GLOBAL_DIRECTION_SOUTHWEST:
		return GLOBAL_DIRECTION_NORTHEAST;
	default:
		return GLOBAL_DIRECTION_CENTRAL;
	}
}

void CreateRandomPZ(PMAP map)
{
	int OurDirection;
	RECT IterateRect;
	bool Suitable = false;
	PMAPCELL Cell;
	POINT RandPoint;
	int NumTries = 0;
	
	
	if(rand()%2){
		OurDirection = ReturnGlobalPositionOpposite(ReturnGlobalPositionOfObject(map->rz, map));
	}
	else{
		OurDirection = GLOBAL_DIRECTION_CENTRAL;
	}
	
	GetIterationRectForGlobalDirection(OurDirection, map, &IterateRect);
	do
	{
		do
		{
			
			RandPoint.x = (rand()%(IterateRect.right-IterateRect.left)) + IterateRect.left;
			RandPoint.y = (rand()%(IterateRect.bottom-IterateRect.top)) + IterateRect.top;
			Cell = FindMapCellByCoordinate(map, RandPoint);
			if(Cell->type == OBJECT_SYM_FLOOR){
				Suitable = true;
			}
			NumTries++;
			
		}while(!Suitable && NumTries < 10000); //MIGHT BE AN ISSUE. WHAT IF THERE ARE NO BUILDINGS?
		if(!Suitable)
		{
			NumTries = 0;
			//TRY A RANDOM DIRECTION.
			OurDirection = rand()%MAX_GLOBAL_DIRECTIONS;
			GetIterationRectForGlobalDirection(OurDirection, map, &IterateRect);
		}
	}while(!Suitable);
	
		map->pz = RandPoint;
}

void GetIterationRectForGlobalDirection(int Direction, PMAP map, RECT* RectToFill)
{
	switch(Direction)
	{
	case GLOBAL_DIRECTION_NORTH:
		RectToFill->left = map->width / 3;
		RectToFill->right = 2 * map->width / 3;
		RectToFill->top = 2;
		RectToFill->bottom = map->height / 3;
		return;
	case GLOBAL_DIRECTION_SOUTH:
		RectToFill->left = map->width / 3;
		RectToFill->right = 2 * map->width / 3;
		RectToFill->top = 2 * map->height / 3;
		RectToFill->bottom = map->height - 3;
		return;
	case GLOBAL_DIRECTION_EAST:
		RectToFill->left = 2 * map->width / 3;
		RectToFill->right = map->width - 3;
		RectToFill->top = map->height / 3;
		RectToFill->bottom = 2 * map->height / 3;
		return;
	case GLOBAL_DIRECTION_WEST:
		RectToFill->right = map->width / 3;
		RectToFill->left = 2;
		RectToFill->top = map->height / 3;
		RectToFill->bottom = 2 * map->height / 3;
		return;
	case GLOBAL_DIRECTION_NORTHEAST:
		RectToFill->left = 2 * map->width / 3;
		RectToFill->right = map->width - 3;
		RectToFill->top = 2;
		RectToFill->bottom = map->height / 3;
		return;
	case GLOBAL_DIRECTION_SOUTHEAST:
		RectToFill->left = 2 * map->width / 3;
		RectToFill->right = map->width - 3;
		RectToFill->top = 2 * map->height / 3;
		RectToFill->bottom = map->height - 3;
		return;
	case GLOBAL_DIRECTION_NORTHWEST:
		RectToFill->top = 2;
		RectToFill->bottom = map->height / 3;
		RectToFill->right = map->width / 3;
		RectToFill->left = 2;
		return;
	case GLOBAL_DIRECTION_SOUTHWEST:
		RectToFill->top = 2 * map->height / 3;
		RectToFill->bottom = map->height - 3;
		RectToFill->right = map->width / 3;
		RectToFill->left = 2;
		return;
	default:
		RectToFill->top = map->height / 3;
		RectToFill->bottom = 2* map->height / 3;
		RectToFill->right = 2 * map->width / 3;
		RectToFill->left = map->width / 3;
		return;
	}
}