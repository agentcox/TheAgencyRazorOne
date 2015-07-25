#ifndef PREFABS_H
#define PREFABS_H


#define PREFABFILENAME "prefabs.dsa"

//STANDARD OPERATIONS
void TerminatePrefab(PPREFAB prefab);
bool InitializePrefab(PPREFAB prefab, int width, int height);
bool FillPrefab(PPREFAB prefab, int** tiles, int width, int height);

//MODIFICATION OPERATIONS
//TILEWISE OPERATIONS
void RotatePrefabTiles(PPREFAB prefab);
void HFlipPrefabTiles(PPREFAB prefab);
void VFlipPrefabTiles(PPREFAB prefab);

//FULL PREFAB OPERATIONS
void RotatePrefab(PPREFAB prefab);
void HFlipPrefab(PPREFAB prefab);
void VFlipPrefab(PPREFAB prefab);

//LOAD/SAVE
int LoadPrefabArrayFromDisk(PPREFAB* output, char* prefabfile);
bool SavePrefabArrayToDisk(PPREFAB array, int numberelements, char* filename);
void TerminatePrefabArray(PPREFAB array, int numberofelements);
int PrefabDataToTileType(int data);
int TileTypeToPrefabData(int type);

//DEBUGGING OPERATIONS
PREFAB CreateDebugPrefab();
void CreatePrefabString(PPREFAB prefab, char* out);
#endif