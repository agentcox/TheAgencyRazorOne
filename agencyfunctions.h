#ifndef AGENCYFUNCTIONS_H
#define AGENCYFUNCTIONS_H

//SELECTION SCREEN FUNCTIONS
void playerselectdrugs (PPLAYER);
void playergetname(PPLAYER);
void SetAccessCode(PPLAYER ps);
BOOL CheckAccessCode(PPLAYER ps);
void bufferprints(int speed, char* buffer, int snd);
PROMPTRESPONSE DSAPlayerPrompt(char* question, int answertype);

//SARTU FUNCTIONS
int StringCompare(PPOTYPE itm1,PPOTYPE itm2);
PLIST SartuGenerateMissions(PSARTUDATASTRUCT sdata);
bool LoadSartuInputLists(PSARTUDATASTRUCT sdata);
void FreeSartuInputLists(PSARTUDATASTRUCT sdata);
bool LoadNLDStrings(PLIST *list, FILE* fp); //LOAD NEWLINE DELINEATED STRINGS
void SartuDailySession(PPLAYER player, int numberofmissionstocreate);
void FreeStringList(PLIST list);
PPOTYPE RetrieveRandomObject(PLIST list);
int GetHighestAvailableMissionType(PPLAYER player);
int CreateSartuMissions(PPLAYER player, PSARTUDATASTRUCT sdata, int nummissions);
void ShutDownSartuForTheDay(PPLAYER player, PSARTUDATASTRUCT sdata);
void ReorderSartuList();
void FreeSartuMission(PSARTUMISSION mission);
void TerminateSartu();
int GetNumberOfMissionsPerDay(PPLAYER player);
int CreateSartuMission(PSARTUMISSION mission, PPLAYER player, PSARTUDATASTRUCT sdata, bool priority = false, int prioritymissiontype = 0, int priorityspecificidone = 0, int priorityspecificidtwo = 0);
void SartuGenerateBriefing(PSARTUMISSION mission, PPLAYER player, PSARTUDATASTRUCT sdata, bool priority = false, int prioritymissiontype = 0, int priorityspecificidone = 0, int priorityspecificidtwo = 0);
int CompareSartuMissionID(PPOTYPE itm1,PPOTYPE itm2);
char* ReturnMissionTypeString(int missiontype);
char* ReturnDirection(int direction);
void SartuTacticalSituation(PSARTUMISSION mission, char* buf);
void SartuDrawShell(PPLAYER player);
void SartuTerminal(PPLAYER player); //THE SARTU UPLINK!
void SartuPrintTakenMissionData(PSARTUMISSION mission);
void SartuPrintMissionAcceptance(PPLAYER player);
void SartuPrintCodeWrong(PPLAYER player);
void SartuPrintPleaseEnterCode(PPLAYER player);
void SartuPrintSartuDeactivated();
void SartuPrintSartuActive();

//OFFICE OF RECORDS FUNCTIONS
void OfficeOfRecordsPlayerDossierOne(PPLAYER player);
char* OfficeOfRecordsGetGeneralInformation(char* record, PPLAYER player);
void OfficeOfRecordsPlayerDossierTwo(PPLAYER player);
char* OfficeOfRecordsGetQuantifiedReport(char* record, PPLAYER player);
void OfficeOfRecordsPlayerDossierThree(PPLAYER player);
char* OfficeOfRecordsGetSkillProfile(char* record, PPLAYER player);
char* OfficeOfRecordsGetSpecializationProfile(char* record, PPLAYER player);
void OfficeOfRecordsPlayerDossierFour(PPLAYER player);
char* OfficeOfRecordsGetPsychProfile(PPLAYER player);
void OfficeOfRecordsPlayerDossierChooseDossier(PPLAYER player);
void OfficeOfRecordsAccessPlayerDossiers();
void SendPlayerDossierToPrinterorText(PPLAYER player, bool toprinter);
void HonorBoardMainMenu(PPLAYER player);
void HonorBoard(int sorting);
int HBMissionCompare(const void* elem1, const void* elem2);
int HBKillCompare(const void* elem1, const void* elem2);
int HBDamageCompare(const void* elem1, const void* elem2);


//DIALOG BOX FUNCTIONS
PLIST BuildDBList(PLIST inputlist, DBLISTITEM(ItemCreate)(PPOTYPE object));
DBLISTITEM CreateStringDBListItem(PPOTYPE object);
DBLISTITEM AlreadyDBListItem(PPOTYPE object);
int CompareDbitemID(PPOTYPE itm1,PPOTYPE itm2);
int GetLongestDBItemLength(PLIST dblist);
void WrapStringToWidth(char* string, int width);
int GetNumberOfStringLines(char* string);
int getObjectCenterCoordinate(int distance, int holdingdistance);
void DrawDialogBoxShell(PDBOXPARAM dbp, int boxwidth, int boxheight, int xorigin, int yorigin);
void StaticDialogBox(char* topstring, char* contents, char* bottomstring, PDBOXPARAM dbp);
void StaticHoldDialogBox(char* topstring, char* contents, char* bottomstring, PDBOXPARAM dbp, int splittohold);
bool YesNoDialogBox(char* topstring, char* contents, char* bottomstring, PDBOXPARAM dbp);
void WriteDialogString(char* string, int xorigin, int yorigin, int boxwidth, bool centered);
void SetGlobalDBoxParam(int type, int size, int maxresponsechars,  bool centered);
void StringDialogBox(char* topstring, char* contents, char* bottomstring, char* outstring, int numchars, PDBOXPARAM dbp);
int NumberDialogBox(char* topstring, char* contents, char* bottomstring, int maxdigits, PDBOXPARAM dbp);
void DrawStringDialogBoxShell(PDBOXPARAM dbp, int boxwidth, int boxheight, int xorigin, int yorigin);
DBLISTITEM* ReturnDBListItemByNumber(int number, PLIST list);
int CompareStringID(PPOTYPE itm1, PPOTYPE itm2);
char* ReturnStringListItemByNumber(int index, PLIST list);
void DBTest();
PPOTYPE ReturnListItemByNumber(int index, PLIST list);
int SimpleListDialogBox (char* topstring, char* bottomstring, PLIST objects, PDBOXPARAM dbp, DBLISTITEM(ItemCreate)(PPOTYPE object));
LISTCONTROLOUTPUT ListDialogBoxWithControls (char* topstring, char* bottomstring, char* helptext,PLIST objects,  PDBOXPARAM dbp, DBLISTITEM(ItemCreate)(PPOTYPE object), int selectionnum);
void WriteToGlobalBackBuffer(int xorigin, int yorigin, int width, int height);
void WriteFromGlobalBackBuffer(int xorigin, int yorigin, int width, int height);
void DrawListBoxControlDialogBoxShell(PDBOXPARAM dbp, int boxwidth, int boxheight, int xorigin, int yorigin);
bool CreateDBStringList(PLIST* list, char** items, int numitems);
void FreeDBStringList(PLIST list);

//INVENTORY FUNCTIONS
void MainInventorySelection(PPLAYER player, PMAP map);
void CycleMainInventoryColor(WORD* CurrentColor, bool* ColorIncreasing);
void DrawInventorySyringe(PPLAYER player, int syringe, int temperature, int percentage, bool selected, POINT DrawPoint, int BarWidth);
void SyringeInventory(PPLAYER player);
void InjectPlayer(PPLAYER player, int syringeid);
void GizmoInventory(PPLAYER player);
void WeaponInventory(PPLAYER player, PMAP map, bool InVan);
void DropPlayerWeapon(PPLAYER player, int index);
void ClipLoadingInventory(PPLAYER player, PWEAPON weapon);
void MagazineLoadingInventory(PPLAYER player, PWEAPON weapon);
void CylinderLoadingInventory(PPLAYER player, PWEAPON weapon);
void DrawMagTubeContents(PWEAPON Weapon, int YOrigin);
bool DumpStrayRoundsDialog(PPLAYER player, PSTRAYROUNDS Rounds);
void DrawChamberContents(PWEAPON Weapon, int ChamberNumber, int SelectedNumber, int ycoord);
//SNIPING FUNCTIONS
void showscope();
void flashscope();
SNIPERESULT snipe(PPLAYER ps, PWEAPON weapon, bool training);
int CheckSnipeHit(int xoff, int yoff);
int reportsnipehit(int hitcode,bool critical);
int snipetest(PPLAYER player, bool training);
int GetNewOcularState(PPLAYER player, int totalsplitelapsed);
void SnipeDrawOcularState(int status);
MISSIONCHRONO GetTimeToNewOcularFocus(PPLAYER player, PMISSIONCHRONO totaltime);
void RestoreOcularFocus(PPLAYER player);
void SnipeShowOptics(PPLAYER ps, bool training);
WORD SnipeGetOpticColor(PPLAYER ps, bool training);
//MATH FUNCTIONS
int random(int min, int max);
void bToggle(BOOL* Pboolean);
void bToggle(bool* Pboolean);
void calibratechrono(PCHRONOSTRUCT cs);
void calibratemissionchrono(PMISSIONCHRONO mcs);
char RandomAlpha();
char RandomNumeric();
char RandomAlphaNumeric();
void SPrintFullDateTime(char* timebuf, PCHRONOSTRUCT chronostruct);
void SPrintFullDateTimeSeconds(char* timebuf, PCHRONOSTRUCT chronostruct);
void SPrintMissionChrono(char* timebuf, PMISSIONCHRONO mc);
void SPrintChrono(char* timebuf, PCHRONOSTRUCT chron);
BOOL MissionChronoTimeUp(PMISSIONCHRONO mc);
void printwordwrap(char* string, COORD wrapcoord);
void printwordwrapcoordinate(char* string, COORD wrapcoord, COORD printcoord);
void printinrect(char* string, COORD startcoord);
void PrintRandomNumberString(int numberofdigits);
char* RetrieveGreekString(int index);
bool RandomPercentage(int percentage);
int CapExclusive(int digit, int low, int high);
int CapInclusive(int digit, int low, int high);
double AbsoluteDistance(POINT pointone, POINT pointtwo);
double ReturnZoomFactor(POINT Location, POINT ZoomLocation, int ViewportWidth, int ViewportHeight);
int CompareMissionChrono(PMISSIONCHRONO C1, PMISSIONCHRONO C2);
bool IsPointInRect(RECT Rect, POINT Point);
int WrapNumber(int digit, int mintowrapto, int maxexclusive);

//RADIO FUNCTIONS
int FillSquelchChannel(int Max, bool Lowest);
int GetMaxSNR(PPLAYER player, PMAP map);
void ReinitSquelchChannels(PPLAYER player, PMAP map, int** SquelchChannels, int CurrentChannel);
bool RadioInterface(PPLAYER player, PMAP map, PLIST TransmissionList);
void ScrollRadioWords(RECT RadioScrollRect);
void PrintSNRString(int SNR);
void MainRadio(PPLAYER player, PMAP map);
char ReturnRadioLetter(int channel);
void SwitchRadioChannel(PPLAYER player);
void AmplifiedTrackingSystemRequest(PPLAYER player);
void NROScanRequest(PPLAYER player);
void ETeamCommunicationRequest(PPLAYER player);
bool RadioBombDetonation(PPLAYER player, PMAP map);
void IncomingRadioMessage();
void AmplifiedTrackingSystemResponse(PPLAYER player, PMAP map);

//VEHICLE FUNCTIONS
void GenerateVehicle(PVEHICLE vs);
void VehiclePrintout(PVEHICLE vs);
BOOL CheckVehicleWindowStatus(PVEHICLE vs, int index);
BOOL CheckVehicleBrakeStatus(PVEHICLE vs, int index);
int escape(PPLAYER ps, PMAP map, PVEHICLE vs);
int CompareVehicleID(PPOTYPE itm1, PPOTYPE itm2);
//CUTSCENE FUNCTIONS
void CreditScreen();
void opcontitles(int titlenumber);
void StartMissionChronoSequence(PMISSIONCHRONO mchrono);
void SynchronizeChronoSequence(PCHRONOSTRUCT chrono, PMISSIONCHRONO mchrono);
void atomicdetonation();
void FadeInTitle(char* title, char* subtitle);
void IntroScreen();
void PromptForCorrectScreenSize();
void PrintDayAndMonth(PPLAYER player);
void PrintPercentageComplete(char* text, int percentage);
void ComputerPrintOutString(char* string, int sleeptime, char* finalstring);
void PrintFinalData(char* stringone, char* stringtwo, bool bottomred, int topsleep, int bottomsleep, int intervalsleep, int finalsleep);
void FinalMobilizationCutscene();

//PRE-MISSION FUNCTIONS
void CreateNewPlayer(PPLAYER player);
void GenerateMissionCode(PMISSION ms);
void GenerateMissionCode(char** codebuf);
PWEAPON weaponquickselect(PLIST weaponlist, PLIST ammolist); //DEBUG ONLY, MOST LIKELY
BOOL weaponcheck(PWEAPON weapon, PPLAYER player); //AGAIN, DEBUG ONLY. STUPID STUFF. TRUE IF QUIT, FALSE IF WANT NEW WEAPON
void AllWeaponsAvailable(PLIST weaponlist, PLIST ammolist);
void InitializeRealisticWeaponAvailability(PLIST weaponlist, PLIST ammolist, PLIST armorygizmolist);
void RandomizeWeaponAvailability(PWEAPON weapon);
void RandomizeAmmoAvailability(PAMMO ammo);
void ammoquickdisplay(PWEAPON weapon, PLIST ammolist);
void GiveAtomicDevice(PPLAYER player);
void InitializePlayerHistory(PPLAYER player);
void FreePlayerHistory(PPLAYER player);
void InitializePlayerForCombat(PPLAYER player);
void FreePlayerFromCombat(PPLAYER player);
void PlayerGetHistoryMatrix(PPLAYER player);
void SetDefaultOptions();
void GetPlayerMilitaryHistory(PPLAYER player);
void ClearPlayerStats(PPLAYER player);
bool DoesIDExist(PPLAYER player);
void SavePlayer(PPLAYER player);
bool LoadPlayer(char* filename, PPLAYER player);
DBLISTITEM CreateAgentNameListItem(PPOTYPE object);
bool SelectPlayer(PPLAYER player, bool debug);
bool IsClearedForWeapon(PPLAYER player, PWEAPON weapon);
void CRMC(PPLAYER player);
void FillAgentArray(PPLAYER playerarray);
int GetNumberOfSavedAgents();
void AssignNewAgentID(PPLAYER player);
void LevelIVWeaponLockers(PPLAYER player);
void LevelIVGizmoLockers(PPLAYER player);
void LevelIVPharmacy(PPLAYER player);
void LevelIVSelectWeaponExtras(PWEAPON weapon);
void ArmoryCreateAmmoDesc(char* outstring, PWEAPON weapon);
int ArmoryDrawDescription(char* description, RECT boundingrect, int page);
void ArmoryDrawWeaponList(PPLAYER player, PLIST weaponlist, RECT boundingrect, int localselection, int scrolloffset, bool hasfocus, bool usesIndex);
void ArmoryDrawGizmoList(PPLAYER player, PLIST armorygizmolist, PLIST gizmolist, RECT boundingrect, int localselection, int scrolloffset, bool hasfocus, bool usingArmorylist);
void ArmoryCreateExtendedDescription(PWEAPON weapon, char* OutString);
void ReorderWeaponListIndex(PLIST weaponlist);
void ReorderGizmoListIndex(PLIST gizmolist);
bool DoesWeaponUseAmmunition(PWEAPON weapon);
bool DoesAnyAmmunitionExistForPlayerWeapon(PPLAYER player, int index);
void ArmoryAmmunitionDrawWeaponList(PPLAYER player, RECT boundingrect, int localselection, int scrolloffset, bool hasfocus);
void ArmoryAmmunitionDrawAmmunitionList(PPLAYER player, RECT boundingrect, int weaponindex, int localselection, int scrolloffset, bool hasfocus);
PCLIP ArmoryGetClipByWeaponAndSelection(PPLAYER player, int weaponindex, int localselection);
PSTRAYROUNDS ArmoryGetStrayRoundsByWeaponAndSelection(PPLAYER player, int weaponindex, int localselection);
bool IsAmmunitionUsedForMoreThanOnePlayerWeapon(PPLAYER player, int weaponindex);
void PurgeAllAmmunitionForWeaponIndex(PPLAYER player, int weaponindex);
int GetNumberOfAmmoUnitsForPlayerWeapon(PPLAYER player, int weaponindex);
void LevelIVAmmunitionLockers(PPLAYER player);
void GiveAmmunitionForWeaponIndex(PPLAYER player, int weaponindex);
void RemoveAmmunitionForWeaponIndex(PPLAYER player, int Weaponindex, int localselection);

int GetNumberOfAgentFiles();
bool ProcessPlayerDataDebugUnsignedNumericalManipulationBox(LISTCONTROLOUTPUT ls, void* extradata); //DEBUG!!
void PlayerDataDebugFunctions(PPLAYER player); //DEBUG!!!
void debugchangecharacterrank(PPLAYER player); //DEBUG!!!
void debugchangepsych(PPLAYER player); //DEBUG!!!
void debugchangeskills(PPLAYER player); //DEBUG!!!!
void debugchangespecializations(PPLAYER player);//DEBUG!!!!



//OPCON FUNCTIONS
bool OpConElevator(PPLAYER player);
void LevelOne(PPLAYER player);
void LevelTwo(PPLAYER player);
void LevelThree(PPLAYER player);
void LevelFour(PPLAYER player);
void LevelFive(PPLAYER player);
void LevelSix(PPLAYER player);
bool HeadHome(PPLAYER player);
void LoopOpConMusic(PPLAYER player);
void SilentWall();

//BULLET TRAJECTORY FUNCTIONS
bool ResolveBulletPassThrough(PMAPCELL cell, PAMMO ammunition, PLIST outputlist, int percentchance);
//bool CalculateTrajectory(PMAP map, PMAPCELL SourceCell, PMAPCELL DestCell, PRAYCASTMOD modifiers, bool Vision);
bool DoesRayCastBlockerBlockVision(int RayCastBlocker);
int GetRayCastBlocker(PMAP map, POINT celltocheck);
int GetRayCastBlocker(PMAP map, int x, int y);
bool CalculateVisionTrajectory(PMAP map, PMAPCELL SourceCell, PMAPCELL DestCell);
bool CalculateVisionTrajectoryHitAtPoint(PMAP map, int x, int y);
bool IsTypeOfObjectWithinVisionRange(int ObjectType, PMAP map, PPSYCHSKILLSTRUCT viewerpsychskill, PMAPCELL SourceCell, PMAPCELL DestCell, bool UsingNvg, bool UsingTas, bool InEnvironmentalHazard);
int GetObstructionAtPoint(PMAP map, int x, int y);
int GetFirstObstructionBetweenPoints(PMAP map, PMAPCELL SourceCell, PMAPCELL DestCell, POINT* Out); //RETURNS TYPE.
POINT GetPointAlongLine(PMAPCELL SourceCell, PMAPCELL DestCell, int StepsIntoLine);
//POST-MISSION FUNCTIONS
void debrief(PPLAYER ps, PMISSION ms);
void debugchangecharacterstatus(PPLAYER player);
void WrapUpMonth(PPLAYER player);
void CheckForHomePsychIncrease(PPLAYER player, int daystotakeoff);
void Surgery(PPLAYER player);
void FinalAnalysis(PPLAYER player);
void FinalCable(PPLAYER player);
int Captivity(PPLAYER player);
void AnalyzeMissionEndCode(PPLAYER player, int RetVal);

//HAND TO HAND FUNCTIONS
int MeleeCombatMain(PMAPCELL mapcell, PPLAYER player, PENEMY enemy, BOOL playersurprisedenemy);
void MeleeCombatInitialize(PMAPCELL mapcell, PPLAYER player, PENEMY enemy, BOOL playersurprisedenemy);
int MeleeCombatEnd(PMAPCELL mapcell, PPLAYER player, PENEMY enemy, int deathtype);
int MeleeCombatLoop(PMAPCELL mapcell, PPLAYER player, PENEMY enemy);
int MeleeDebug();

//INITIALIZATION FUNCTIONS
int initweaponsammo(PLIST *weaponlist, PLIST *ammolist);
void wordwrapweaponsdesc(PLIST weaponlist);
void wordwrapammodesc(PLIST ammolist);
bool initmusic();
bool GameStart();
void SetDefaultCmdLineOptions();

//TERMINATION FUNCTIONS
void freeweaponsammo(PLIST *weaponlist, PLIST *ammolist);
void GameEnd();
void terminatemusic();

//SOUND FUNCTIONS
void playWeaponSound(PWEAPON weapon, int field, int soundmode);
void playWeaponSilencer(int soundmode);
void playWeaponFlashsuppressor(int soundmode);
void playWeaponLasersight(int soundmode);
void twobeepsmod();
void twobeeps();
void onebeepa();
void onebeepb();
void onebeepc();
void typeclick();
void playRicochet(int soundmode);
void playAmmoImpactSound(ROUND round, int soundmode);
void playRoundInsert(PROUND round, bool speedload, bool speedfill, int soundmode);
void playRoundDump(PROUND round, bool multiple, int soundmode);
//MUSIC FUNCTIONS
void PlaySong(char* song);
void StopSong();
void FreeXAudioPlayer();
void CreateXAudioPlayer();
void LoopSong(char* song);
bool loadmusiclist();

void MusicProc(void* MM);

void SendMusicMessage(int message, char* song);
void GetXaMessage();

//MIX SOUND FUNCTIONS
void playMIXsound(char* sound);

//GIZMO FUNCTIONS
BOOL CISDebug();
BOOL CISTest(PPLAYER player, PDOOR door);
BOOL LISTest(PPLAYER player, PDOOR door);
BOOL LISDebug();
void DrawLISRing(PLISSYSTEM lock, int ringnumber, COORD extraoffset, BOOL bolded, BOOL showblocks);
char RandomCISDigit();
char cyclechars (char* array, int pos, int max);
BOOL AtomicActivation(PPLAYER player);
void AtomicLight(PATOMICLIGHT light);
void GiveGizmo(PPLAYER player, int gizmotype);
void GiveGrenade(PPLAYER player, int grenadetype);
BOOL DoesListHaveGizmoType(int type, PLIST gizmolist, BOOL isgrenade);
int CompareGizmoDeviceType(PPOTYPE itm1,PPOTYPE itm2);
int CompareGizmoGrenadeType(PPOTYPE itm1,PPOTYPE itm2);
int CompareGizmoIndex(PPOTYPE itm1, PPOTYPE itm2);
int CompareArmoryGizmo(PPOTYPE itm1, PPOTYPE itm2);
int CompareArmoryGizmoBothTypes(PPOTYPE itm1, PPOTYPE itm2);
bool LoadArmoryGizmoList();
void FreeArmoryGizmoList();
PGIZMO FindActiveGizmoOfType(int Type, PLIST GizmoList);
void DrawC4Shell(char* StringInBox, bool GreenLight, bool NeedTotalRedraw);
void ActivatePlayerGizmo(PPLAYER player, int index);
void DeactivatePlayerGizmo(PPLAYER player, int index);
void PlayAppropriateZoomSound(PPLAYER player);
void DropPlayerGizmo(PPLAYER player, int index);
void ActivatePlayerGrenade(PPLAYER player, int index);
void DeactivatePlayerGrenade(PPLAYER player, int index);
bool SpawnLiveGrenade(POINT loc, int GrenadeType, int FuseLength);
bool AreThereActiveGrenadesInInventory(PPLAYER player);
void DetonateGrenade(PGIZMO Gren, PMAP map, PPLAYER player);
double GetActiveGrenadeWeight(PPLAYER player);
PGIZMO GetActivePlayerGrenade(PPLAYER player);
void DeletePlayerActiveGrenade(PPLAYER player);
//GRAPHICS FUNCTIONS
int CompareGraphicID(PPOTYPE itm1,PPOTYPE itm2);
PLIST loadgamegraphics();
void loadgraphic(FILE* fp, PGRAPHIC graphic);
void freegamegraphics(PLIST glist);
void printgraphic(PLIST graphicslist, COORD drawcoord, int graphicid);
COORD getgraphicdimensions(PLIST graphicslist, int graphicid);
void PlantAtomicBomb(PPLAYER player, PMAP map);
void RemoveWorldGizmo(int index);

//TRAINING FUNCTIONS
void TrainingCenter(PPLAYER player);
void ElectronicsTraining(PPLAYER player);
void LISTraining(PPLAYER player);
void CISTraining(PPLAYER player);
void MeleeTraining(PPLAYER player);
void MeleeTrainingSystem(PPLAYER player);
void TargetTraining(PPLAYER player);
void SnipingTraining(PPLAYER player);

//SHOOTING FUNCTIONS
void ShootingPrintSilhouette(int siltype);
bool ShootingGetHitFromMatrix(COORD loc, int& outBodyPart, int& outTimesAlreadyHit);
bool ShootingLoadSilhouetteMatrix(char* datafile);
void ShootingClearHitMatrix();
bool ShootingPrintHitGraphic(COORD location, int timeshit);
void ShootSequenceDemoRandom(bool singleshot);
COORD ShootingGetHitLocFromAccuracy(int bodyloc, int accuracypercentage, int directhitpercentage);

//ENEMY FUNCTIONS
void ClearEnemyStats(PENEMY enemy);
void FreeEnemyFromCombat(PENEMY enemy);
void InitializeEnemyForCombat(PENEMY enemy);
bool EnemyTakePlayerWeapon(PENEMY enemy, PPLAYER player, PWEAPON* weapon);
bool PlayerTakeEnemyWeapon(PENEMY enemy, PPLAYER player, PWEAPON* weapon);
void GenerateRandomPsychSkill(PPSYCHSKILLSTRUCT ps);
bool GenerateEnemy(PMAP map, PSARTUMISSION missioninfo, int CurrentEnemyCounter);
bool GeneratePrimaryObjectiveEnemy(bool primaryfriendly, bool primarystatus, PSARTUMISSION missioninfo, PMAP map);
void AssignRandomKeyCards(PSARTUMISSION missioninfo);
void GiveKeyCard(int* keycardfield, int keycardvalue);
void RemoveKeyCard(int* keycardfield, int keycardvalue);
bool HasKeyCard(int* keycardfield, int keycardtolookfor);
void PrintKeyCardsOnHand(int* keycardfield, char* buf, char delineator);

#endif