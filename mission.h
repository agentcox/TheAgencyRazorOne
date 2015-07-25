//DEBUG FUNCTIONS
void DebugPrefabs();//DEBUG!!!
void DebugExplosion(PPLAYER player, PMAP map, POINT ExplosionPoint);

//UTILITY FUNCTIONS
void ClearDoubleArray(bool *** array, int width, int height);
void InitDoubleArray(bool *** arraytoinit, int width, int height);
void FreeDoubleArray(bool *** arraytofree, int height);

//MAP GENERATION FUNCTIONS
int FillMapWithPrefabs(PMAP map, int DensityPercentage);
void FillMapWithObstructions(PMAP map, int DensityPercentage);
int TryToPlacePrefab(PPREFAB prefabtoplace, PMAP maptoplaceon, POINT originpoint, bool*** Plockoutarray);

//MAP DRAWING FUNCTIONS
bool DrawMapViewport(PPLAYER player, PMAP map, POINT draworigin, int viewportwidth, int viewportheight, POINT viewcenterloc, POINT localcursorloc, bool drawcursor);
void DebugMapViewer(PPLAYER player, PMAP map); //DEBUG!!!
void DrawMapCellLineAtCoordinates(PPLAYER player, PMAP map, POINT* mapcells, POINT drawcoordinateorigin, int numelements, POINT localcursorloc, bool drawcursor, bool NV, bool therm);
CHAR_INFO GetMapCharacterToDraw(PMAP map, PPLAYER player, PMAPCELL cell, bool NV, bool therm);
void DisplayUIMap(PPLAYER player, PMAP map, bool redrawoutline, bool drawcursor);
void DrawTrackerArrows(PTRACKERARROWS Arrows);
void MainUIPrintCurrentAction(PPLAYER player, bool On);
void MainUIClearActionString();
void PlayThrowSequence(PPLAYER player, PMAP map, PMAPCELL Source, PMAPCELL Target, int Distance, char* Sound = NULL);

//HISTORY WINDOW FUNCTIONS
void RestoreHistorySnapShot(POINT draworigin, int width, int height);
void SaveHistorySnapShot(POINT saveorigin, int width, int height);
void ScrollHistoryBox();
void InsertHistoryString(PPLAYER player, char* string, bool priority);

//STATUS WINDOW FUNCTIONS
void MainUIPrintPlayerStatus(PPLAYER player);

//TIME WINDOW FUNCTIONS
void MainUIPrintChronograph(PPLAYER player, RECT RectToDrawIn);

//MAIN UI FUNCTIONS
void RedrawMainUI(PPLAYER player, PMAP map);
void InitializeMainUI(PPLAYER player, PMAP map);
int MainUILoop(PPLAYER player, PMAP map);
void LoopMainSong(PPLAYER player);
void MainUIHelp();
int MainMission(PPLAYER player, PMAP map); /////////////CALL THIS ONE!!!!!!!!!!!!
bool MainUIHandleArrowKey(USHORT vkey, PPLAYER player, PMAP map);
bool MainUIHandleTimeDecrement(PPLAYER player, PMAP map, int timediff);
void RestoreMainSnapShot();
void SaveMainSnapShot();

//MISSION INITIALIZATION FUNCTIONS
bool FinalMobilizationChecks(PPLAYER player);
int Mobilization(PPLAYER player);
void AskPlayerForTimeToRecovery(PPLAYER player);
void MobilizationChatter(PPLAYER player, PMAP map);
void PlayerSetTimeToRecovery(PPLAYER player, PMISSIONCHRONO timetorecovery);
void InitializeGlobalMapDataForCombat(PMAP map, PPLAYER player);
int InitializeGlobalMapArmories(PMAP map, PSARTUMISSION missioninfo);
int InitializeGlobalMapEnemies(PMAP map, PSARTUMISSION missioninfo);
void InitializeDoorBySecurityFactor(int SecurityFactor, PDOOR door);
bool InitializeGlobalMapDoors(PPLAYER player, PMAP map);
void CreateMapArmory(PMAPARMORY Armory);
int InitializeGlobalMapVehicles(PMAP map, PSARTUMISSION missioninfo);

//DATA CHECKING FUNCTIONS
bool IsRecoveryTimeLeft(PPLAYER player);
bool IsLeftBehindTimeLeft(PPLAYER player);
int GetPlayerInitialTurnDirection(PPLAYER player, PMAP map);
bool DoesCellBlockMovement(PMAPCELL Cell);
TRACKERARROWS EvaluatePlayerMoveDirections(PPLAYER player, PMAP map);
bool ActionModeNeedsCursor(int ActionMode);
bool IsCellInVisibleBounds(PMAP map, int viewportwidth, int viewportheight, POINT viewcenterloc, int x, int y);
bool IsMapCellSuitableForArmory(PMAP Map, PMAPCELL Cell, bool** LockOutArray);
bool IsMapCellSuitableForVehicle(PMAP Map, PMAPCELL Cell, bool** LockOutArray);
bool IsCellWall(PMAPCELL Cell);
bool IsCellDoor(PMAPCELL Cell);
PMAPARMORY GetMapCellArmory(PMAPCELL Cell);
PVEHICLE GetMapCellVehicle(PMAPCELL Cell);
PENEMY GetMapCellEnemy(PMAPCELL Cell);
PGIZMO GetMapCellGizmo(PMAPCELL Cell);
double GetPlayerMaxZoom(PPLAYER player);
int CompareDoorID(PPOTYPE itm1,PPOTYPE itm2);
PDOOR GetDoorInfoFromCell(PMAPCELL Cell);
int CompareDoorCoords(PPOTYPE itm1,PPOTYPE itm2);
int CompareScriptedEventID(PPOTYPE itm1,PPOTYPE itm2);
int CompareEnvironmentalID(PPOTYPE itm1,PPOTYPE itm2);
bool IsZoomValid(PPLAYER player, PMAP map, POINT NextZoomPoint);
void PrintDoorInfo(PPLAYER player, PDOOR Door);
bool CheckForANYObstructions(POINT MapLocation);
int DoAnyGizmosInterfereWithMovement(PLIST gizmolist);
bool DoesScriptedSequenceListHaveType(int Type);
int DoesEnvironmentalEffectExistAtPoint(POINT Point, int Type); //TYPE CAN BE -1 FOR "ALL"
bool DoesMapCellHaveAnyItems(PMAPCELL Cell);
bool HasThrowableWeapon(PPLAYER player);
//PLAYER / ENEMY ACTION FUNCTIONS
bool DoDamageToPlayer(PPLAYER player, int damageamount);
int MainUIPlayerUseObject(PPLAYER player, PMAP map);
int MainUIPlayerThrowObject(PPLAYER player, PMAP map);
int ThrowObjectGetFinalLandingPoint(PPLAYER player, PMAP map, POINT* Out);
void RaidArmory(PPLAYER player, PMAPARMORY armory);
void ModifyPlayerPsych(PPLAYER player, PPSYCHSKILLSTRUCT ps);
bool SetBomb(PPLAYER player, PMAP map, PMAPCELL CellToSetIn, PGIZMO BombToSet);

//MISSION TERMINATION FUNCTIONS
void FreeGlobalMapDataFromCombat();
void FreeMapArmories(PMAPARMORY armories, int numarmories);
void FreeMapEnemies(PENEMY enemies, int numenemies);

//SCRIPTED EVENT FUNCTIONS
bool AddScriptedEvent(PSCRIPTEDEVENT Event);
bool IsScriptedEventDue();
PSCRIPTEDEVENT GetScriptedEventByID(int id);
bool RunNextScriptedEvent(PPLAYER player, PMAP map);
void SortScriptedEventListByID(PLIST list);
int CompareScriptedEventChrono(PPOTYPE itm1, PPOTYPE itm2);
void ReindexScriptedEventList(PLIST List);
void ScriptedEventResolution(PSCRIPTEDEVENT Event, PPLAYER player, PMAP map);

//DOOR FUNCTIONS
void OpenDoor(PMAP map, PDOOR Door);
void CloseDoor(PMAP map, PPLAYER player, PDOOR Door);
void PlayDoorCloseSound(PDOOR Door);
void PlayDoorOpenSound(PDOOR Door);
bool DoorExplosionTest(PDOOR Door, int PowerPercentage);
//ENVIRONMENTAL FUNCTIONS
void InsertEnvironmentalEffect(PENVIRONSTRUCT Env);
void UpdateEnvironmentalEffects(PMAP map, PPLAYER player);
void ReorderEnvironmentalEffectsList();
void DestroyObjectsInRadius(PMAP map, PPLAYER player, POINT Origin, int Radius, int PowerPercentage, int Falloff);
void DestroyGameObjectsInCell(PMAPCELL Cell, int PowerPercentage);
void DetonateAllBombsOnRemoteFrequency(PMAP map, PPLAYER player, int Freq);
void C4Detonate(int Index, PMAP map, PPLAYER player);
void CheckProximityBombs(PMAP map, PPLAYER player);
