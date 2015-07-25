#include "stdafx.h"
#include "..\agencyinclude.h"

//USER INTERFACE MANIPULATION
bool HandleTileTypeAreaClick(HWND hwnd, POINT cursor);
bool HandleTileGridAreaClick(HWND hwnd, POINT cursor);
void HandleButtonClicks(HWND hwnd, POINT cursor);
void UpdateScrollCoords(POINT delta);

//GRAPHICAL MANIPULATION
void LoadProgramBitmaps(HINSTANCE hinst, HWND hwnd);
void TerminateProgramBitmaps();
void UpdateBackground(HWND hwnd);
void UpdateTileTypes(HWND hwnd);
void UpdateTileGrid(HWND hwnd); //THERE IS GEOMETRY HERE, MUST USE DOUBLE BUFFERING!
void RedrawScreen(HWND hwnd);

//PREFAB LIST MANIPULATION
void FreePrefabListEntries(PLIST prefablist);
void WritePrefabListToDisk(PLIST prefablist, char* filename);
bool LoadPrefabListFromDisk(PLIST prefablist, char* filename);
int PrefabIDCompare(PPOTYPE itm1,PPOTYPE itm2) ;
void AddPrefabToList(PPREFAB prefab, PLIST prefablist);
void DeletePrefab(PPREFAB prefab, PLIST prefablist); //CALLED WHEN DELETE BUTTON IS CLICKED.
bool RemovePrefabFromList(PLIST prefablist, int id);
void SavePrefab(PLIST prefablist, PPREFAB prefab); //CALLED WHEN SAVE BUTTON IS CLICKED.
void NewPrefab(PLIST prefablist, PPREFAB prefab);
int GetNextAvailablePrefabID();
void LoadPrefab(int id);
void ReorderPrefabList(PLIST prefablist);


//PREFAB DATA MANIPULATION

void CropPrefab(PPREFAB prefab);
bool CheckPrefabForSave(PPREFAB prefab);
int CALLBACK NewPrefabBoxProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);