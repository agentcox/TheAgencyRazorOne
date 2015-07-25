#ifndef MAPLIB_H
#define MAPLIB_H

/***********************************************************
;						 Project Title: The Agency : Art of The State - Text Based Prototype 
;									  Team: Charles Cox
;				Module Filename: mapcreator.h
;				Module Synopsis: The map creator, with the capability to create, import,
			export, edit, and randomly generate maps for The Agency - Art Of The State.
;
;								  Version: First Draft
;					  Revision Date: 03/02/2000
;						   Created By: Charles Cox (Design, Implementation)
************************************************************/



/************************************************************
;BEGIN SYMBOL TABLE
************************************************************/

//OBJECTS MUST BE DRAWN IN A PRIORITY ORDER, WITH THE MOST IMPORTANT OBJECT GOING LAST.
//PRIORITY EIGHT IS THE GROUND.
//PRIORITY SEVEN IS ITEMS.
//PRIORITY SIX IS PLAYER GIZMOS.
//PRIORITY FIVE IS ACTIVE SURVEILLANCE DEVICES.
//PRIORITY FOUR IS VEHICLES AND WRECKS OF VEHICLES.
//PRIORITY THREE IS ENEMIES AND DEAD ENEMIES.
//PRIORITY TWO IS THE PLAYER. HE MUST BE DRAWN NEXT TO LAST.
//PRIORITY ONE ARE ENVIRONMENTAL EFFECTS - GAS, SMOKE, THAT MIGHT OBSCURE THE PLAYER AND ALL OTHER THINGS.

//THE PLAYER SYMBOL
#define PLAYER_SYM_PLAYER			42

//ENVIRONMENTAL MASK SYMBOLS
#define ENVIRON_SYM_POISON			88
#define ENVIRON_SYM_SMOKE			247

//VARIOUS NON-BUILDING SYMBOLS
#define OBJECT_SYM_GROUND			45
#define OBJECT_SYM_HIOBSTRUCT	240
#define OBJECT_SYM_LOOBSTRUCT	61
#define OBJECT_SYM_PLAYERGIZMO	232
#define OBJECT_SYM_ITEMS				94	
#define OBJECT_SYM_FENCE				146
#define OBJECT_SYM_GATE					120

//MISSION SYMBOLS
#define MISSION_SYM_RZ	82
#define MISSION_SYM_PZ	80

//ENEMY SYMBOLS
#define ENEMY_SYM_LIVEENEMY	40
#define ENEMY_SYM_ALERTENEMY	33
#define ENEMY_SYM_PRIMARYTARGETENEMY	15
#define ENEMY_SYM_DEADENEMY	22

//VEHICLE SYMBOLS
#define VEHICLE_SYM_LIVEVEHICLE		17
#define VEHICLE_SYM_DEADVEHICLE	  15

//BUILDING STRUCTURE SYMBOLS
#define BUILDING_SYM_BLCORNER	192
#define BUILDING_SYM_TRCORNER	191
#define BUILDING_SYM_BRCORNER	217
#define BUILDING_SYM_TLCORNER	218
#define BUILDING_SYM_VWALL			179
#define BUILDING_SYM_HWALL			196
#define BUILDING_SYM_VDOOR			186
#define BUILDING_SYM_HDOOR			205
//BUILDING CAMERA SYMBOLS
#define BUILDING_SYM_CAMERADOWN	30
#define BUILDING_SYM_CAMERAUP		31
#define BUILDING_SYM_CAMERALEFT		10
#define BUILDING_SYM_CAMERARIGHT	11


//IDENTIFIERS


/************************************************************
;END SYMBOL TABLE
************************************************************/


/************************************************************
; BEGIN FUNCTION TABLE
************************************************************/

/***********************************************************
;					 Function Name: createVwallsection
;								 Synopsis: creates a vertical wall segment at a given map coordinate.
;						
;				Input Parameters:  COORD	-	startcoord	:	specifies the starting map coordinate to edit to.
;	   		 Output Parameters: Edits the map structure.
;									Return:	None.
***********************************************************/
void createVwallsection(PMAP map, COORD startcoord);

/***********************************************************
;					 Function Name: createHwallsection
;								 Synopsis: creates a horizontal wall segment at a given map coordinate.
;						
;				Input Parameters:  COORD	-	startcoord	:	specifies the starting map coordinate to edit to.
;	   		 Output Parameters: Edits the map structure.
;									Return:	None.
***********************************************************/
void createHwallsection(PMAP map, COORD startcoord);

/***********************************************************
;					 Function Name: createVwall
;								 Synopsis: creates a series of vertical wall segments from one map coordinate
;						to a given y-coordinate location.
;				Input Parameters:  COORD	-	startcoord	:	specifies the starting map coordinate to edit from.
													int				-	endy			:	specifies the ending y-coordinate (the x-coordinate is fixed to prevent misuse).
;	   		 Output Parameters: Edits the map structure.
;									Return:	None.
***********************************************************/
void createVwall(PMAP map, COORD startcoord, int endy);

/***********************************************************
;					 Function Name: createHwall
;								 Synopsis: creates a series of horizontal wall segments from one map coordinate
;						to a given x-coordinate location.
;				Input Parameters:  COORD	-	startcoord	:	specifies the starting map coordinate to edit from.
													int				-	endx			:	specifies the ending x-coordinate (the y-coordinate is fixed to prevent misuse).
;	   		 Output Parameters: Edits the map structure.
;									Return:	None.
***********************************************************/
void createHwall(PMAP map, COORD startcoord, int endx);

/***********************************************************
;					 Function Name: createHdoor
;								 Synopsis: Creates a horizontal door at the given coordinate.
;
;				Input Parameters: COORD	-	startcoord	:	specifies the map coordinate to edit.
													DOOR	-	ds	:	The door structure to copy lock/alarm/type data from.
;	   		 Output Parameters: Edits the map structure.
;									Return: None.
***********************************************************/
void createHdoor(PMAP map, COORD drawcoord, DOOR ds);

/***********************************************************
;					 Function Name: createVdoor
;								 Synopsis: Creates a vertical door at the given coordinate.
;
;				Input Parameters: COORD	-	startcoord	:	specifies the map coordinate to edit.
													DOOR	-	ds	:	The door structure to copy lock/alarm/type data from.
;	   		 Output Parameters: Edits the map structure.
;									Return: None.
***********************************************************/
void createVdoor(PMAP map, COORD drawcoord, DOOR ds);

/***********************************************************
;					 Function Name: createFence
;								 Synopsis: creates a rectangular fence section. to be used ONLY to
					ring the perimeter of the map.
;
;				Input Parameters: the MAP to modify, the RECT specifying the coordinates to start and end the fence.
;	   		 Output Parameters: Modifies the map structure.
;									Return: None.
***********************************************************/
void createFence(PMAP map, RECT *fencerect);

/***********************************************************
;					 Function Name: createVGate
;								 Synopsis: creates a vertical  gate cell to replace a section of fence.
						The fence section must exist BEFORE the gate is placed.
;
;				Input Parameters: COORD	drawcoord	-	the coordinate to draw the gate in.
;	   		 Output Parameters: Modifes the map structure.
;									Return: None
***********************************************************/
void createVGate(PMAP map, COORD drawcoord);

/***********************************************************
;					 Function Name: createHGate
;								 Synopsis: creates a horizontal gate cell to replace a section of fence.
						The fence section must exist BEFORE the gate is placed.
;
;				Input Parameters: COORD	drawcoord	-	the coordinate to draw the gate in.
;	   		 Output Parameters: Modifes the map structure.
;									Return: None
***********************************************************/
void createHGate(PMAP map, COORD drawcoord);

/***********************************************************
;					 Function Name: createWallCorner
;								 Synopsis: Creates a wall corner section, the type and coordinate for
							drawing is specified.
;
;				Input Parameters: COORD	-	drawcoord:	the coordinate to edit.
													int			-	type: WALL_CORNER_UL, WALL_CORNER_LL, WALL_CORNER_UR, WALL_CORNER_LR
;	   		 Output Parameters: Modifies the map structure
;									Return: None
***********************************************************/
void createWallCorner(PMAP map, COORD drawcoord, int type);


/***********************************************************
;					 Function Name: CreateShittyPracticeMap
;								 Synopsis: Creates a fairly shitty practice map.
;				---------------FOR DEBUG / TESTING PURPOSES ONLY-------------------
;				Input Parameters: The map to modify
;	   		 Output Parameters: Modifies the map structure.
;									Return: NUTHIN.
***********************************************************/
void CreateShittyPracticeMap(PMAP map);


/***********************************************************
;					 Function Name: drawMap
;								 Synopsis: Draws the entire compound map to the screen, starting at 0,0.
;					It cycles through the map structure and prints out the character associated with each of
					the cell's attributes. Utilizes drawing priority which is defined inside this function.
;				Input Parameters: PMAP	-	map: the map to draw.
;	   		 Output Parameters: Screen output
;									Return: None.
***********************************************************/
void drawMap(PMAP map);


/***********************************************************
;					 Function Name: createObstruction
;								 Synopsis: Creates an obstruction on the map, the type and coordinate are specified
;
;				Input Parameters: COORD	-	drawcoord:	The coordinate to edit.
													int			-	type: OBSTRUCTION_HI, OBSTRUCTION_LO

  ;	   		 Output Parameters: edits the map structure
;									Return: None.
***********************************************************/
void createObstruction(PMAP map, COORD drawcoord, int type);

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
PMAP initMap(int width, int height);


/***********************************************************
;					 Function Name: terminateMap
;								 Synopsis: Frees the map.
;
;				Input Parameters: PMAP	-	the map to free
;	   		 Output Parameters: Frees the map.
;									Return: None
***********************************************************/
void terminateMap(PMAP map);

/***********************************************************
;					 Function Name: FindMapCellByCoordinate
;								 Synopsis: Returns a map cell in a given map structure that has the 
							coordinates that correspond to the given coordinates.
;
;				Input Parameters: PMAP	-	map: the map structure to search.
													COORD	-	coordtoget: the coordinates to reference the cell by.
;	   		 Output Parameters: None.
;									Return: The found map cell, or NULL if not found
***********************************************************/
PMAPCELL FindMapCellByCoordinate(PMAP map, COORD coordtoget);

/***********************************************************
;					 Function Name: FindPZCell
;								 Synopsis: Finds the unique Primary Zone cell in the given map
								and returns that cell.
;
;				Input Parameters: The map to search.
;	   		 Output Parameters: None
;									Return: Returns the PZ cell, or NULL if none exists.
***********************************************************/
PMAPCELL FindPZCell(PMAP map);

/***********************************************************
;					 Function Name: FindRZCell
;								 Synopsis: Finds the unique Recovery Zone cell in the given map
								and returns that cell.
;
;				Input Parameters: The map to search.
;	   		 Output Parameters: None
;									Return: Returns the RZ cell, or NULL if none exists.
***********************************************************/
PMAPCELL FindRZCell(PMAP map);

/***********************************************************
;					 Function Name: createRZ
;								 Synopsis: Creates a Recovery Zone at the given coordinate in the map structure.
;
;				Input Parameters: PMAP	-	map: The map to edit.
													COORD	-	drawcoord: The coordinates to create at.
;	   		 Output Parameters: Edits the map structure.
;									Return: 0 if ok, -1 if RZ already exists on map.
***********************************************************/
int createRZ(PMAP map, COORD drawcoord);

/***********************************************************
;					 Function Name: createPZ
;								 Synopsis: Creates a Primary Zone at the given coordinate in the map structure.
;
;				Input Parameters: PMAP	-	map: The map to edit.
													COORD	-	drawcoord: The coordinates to create at.
;	   		 Output Parameters: Edits the map structure.
;									Return: 0 if ok, -1 if PZ already exists on map.
***********************************************************/
int createPZ(PMAP map, COORD drawcoord);

/***********************************************************
;					 Function Name: priorityDrawCell
;								 Synopsis: This function contains the priority drawing order for the map
						symbols and MUST BE USED TO DRAW ALL MAP GRAPHICS.
;
;				Input Parameters: The map cell to draw.
;	   		 Output Parameters: Screen Output.
;									Return: None.
***********************************************************/
void priorityDrawCell(PMAPCELL mapcell);

void CreateRandomRZ(PMAP map);

#endif