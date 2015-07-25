// PrefabEditor.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Resource.h"

#define WINDOWWIDTH	700
#define WINDOWHEIGHT	480
#define NUMBERTILEBUTTONS 10
#define TILETYPEBUTTONFIRSTX	576
#define TILETYPEBUTTONFIRSTY	45
#define TILETYPEBUTTONHSPACER	15
#define TILETYPEBUTTONVSPACER	15
#define TILETYPEBUTTONWIDTH			48
#define TILETYPEBUTTONHEIGHT		48
#define HEIGHTFROMTOPTOSCROLLBAR	46
#define HSCROLLBARHEIGHT			20
#define HSCROLLBARWIDTH				540
#define VSCROLLBARHEIGHT			413
#define VSCROLLBARWIDTH				20
#define SCROLLBARSPACERBLOCKWIDTH	22
#define SCROLLBARSPACERBLOCKHEIGHT	22
#define GRIDAREAWIDTH	546
#define GRIDAREAHEIGHT	414
#define GRIDAREASTARTX 0
#define GRIDAREASTARTY	66
#define EXITBUTTONWIDTH	75
#define EXITBUTTONHEIGHT	32
#define EXITBUTTONXORIGIN	590
#define EXITBUTTONYORIGIN	440
#define DELETEBUTTONWIDTH	75
#define DELETEBUTTONHEIGHT	32
#define DELETEBUTTONXORIGIN	590
#define DELETEBUTTONYORIGIN	400
#define SAVEBUTTONWIDTH 75
#define SAVEBUTTONHEIGHT	32
#define SAVEBUTTONXORIGIN 590
#define SAVEBUTTONYORIGIN 360
#define NEWBUTTONWIDTH 75
#define NEWBUTTONHEIGHT	32
#define NEWBUTTONXORIGIN 590
#define NEWBUTTONYORIGIN 5
#define LISTBOXXORIGIN	294
#define LISTBOXYORIGIN	10
#define LISTBOXWIDTH	273
#define LISTBOXHEIGHT	200
#define ID_LISTBOX 1

//#define GRIDHDCEXTENTX (GRIDAREAWIDTH + TILETYPEBUTTONWIDTH) - (GRIDAREAWIDTH + TILETYPEBUTTONWIDTH) % (TILETYPEBUTTONWIDTH)
//#define GRIDHDCEXTENTY (GRIDAREAHEIGHT + TILETYPEBUTTONHEIGHT) - (GRIDAREAHEIGHT + TILETYPEBUTTONHEIGHT) % (TILETYPEBUTTONHEIGHT)

//GLOBAL DECLARATIONS!!!!!
POINT g_ScrollCoords;
PREFAB g_CurrentPrefab;
PLIST g_PrefabList = createList();
HBITMAP g_TileTypeBitmaps[NUMBERTILEBUTTONS];
HDC g_GridDC;
HBITMAP g_GridBitmap;
HDC g_ExitBtnDC;
HBITMAP g_ExitBtnBitmap;
HDC g_SaveBtnDC;
HBITMAP g_SaveBtnBitmap;
HDC g_DeleteBtnDC;
HBITMAP g_DeleteBtnBitmap;
HDC g_NewBtnDC;
HBITMAP g_NewBtnBitmap;
HDC g_TileTypeDCs[NUMBERTILEBUTTONS];
HDC g_BackgroundDC;
HBITMAP g_BackgroundBitmap;
HWND g_Hwnd;
HWND g_ListHwnd;
HCURSOR g_StandardCursorIcon;
HCURSOR g_ScrollCursorIcon;
POINT g_ScrollCursor;
char g_Prefabfilename[200];
bool g_Scrolling = false;
int g_GlobalTileType = -1;
bool g_Exit = false;
bool g_NeedSave = false;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	// TODO: Place code here.
	static TCHAR AppName[] = TEXT ("The Agency : Prefab Editor");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;
	
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(PREFABICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = AppName;
	
	if(!RegisterClass(&wndclass)){
		return 1;
	}
	sprintf(g_Prefabfilename, "..\\data\\%s", PREFABFILENAME);
	
	hwnd = CreateWindow(AppName, AppName, WS_CAPTION | WS_CLIPCHILDREN | WS_OVERLAPPED, CW_USEDEFAULT, CW_USEDEFAULT, WINDOWWIDTH, WINDOWHEIGHT+30, NULL, NULL, hInstance, NULL);
	g_Hwnd = hwnd;
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	
	LoadProgramBitmaps(hInstance, hwnd);
	
	while(GetMessage(&msg, NULL, 0,0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	
	return 0;
}

void UpdateScrollCoords(POINT delta)
{
	g_ScrollCoords.x += delta.x;
	g_ScrollCoords.y += delta.y;
	int upperxbound;
	int upperybound;

	if(g_ScrollCoords.x < 0){
		g_ScrollCoords.x = 0;
	}
	if (g_ScrollCoords.y < 0){
		g_ScrollCoords.y = 0;
	}
	//MUST CHECK UPPER BOUNDS
	//THEY'RE A MEASURE OF DISPLACEMENT.
	//I BELIEVE WE NEED TO CAP THEM AT (NUMBERTILES * TILEWIDTH - GRIDAREAWIDTH, NUMBERTILES * TILEHEIGHT - GRIDAREAHEIGHT).
	upperxbound = (g_CurrentPrefab.width+1)* TILETYPEBUTTONWIDTH - GRIDAREAWIDTH;
	if(upperxbound < 0){
		upperxbound = 0;
	}

	upperybound = (g_CurrentPrefab.height+1) * TILETYPEBUTTONHEIGHT - GRIDAREAHEIGHT;
	if(upperybound < 0){
		upperybound = 0;
	}

	if(g_ScrollCoords.x > upperxbound){
		g_ScrollCoords.x = upperxbound;
	}
	if(g_ScrollCoords.y > upperybound){
		g_ScrollCoords.y = upperybound - 1;
	}
}

void UpdateBackground(HWND hwnd)
{
	HDC maindc = GetDC(hwnd);
		BitBlt(maindc, 0,0, WINDOWWIDTH, WINDOWHEIGHT, g_BackgroundDC, 0,0, SRCCOPY);
		ReleaseDC(hwnd, maindc);
	//	BringWindowToTop(g_ListHwnd);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
//	HDC maindc;
	POINT cursor;
	POINT delta;
	switch(msg)
	{
	case WM_CREATE:
		g_ListHwnd = CreateWindow("combobox", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_DISABLENOSCROLL, LISTBOXXORIGIN, LISTBOXYORIGIN, LISTBOXWIDTH, LISTBOXHEIGHT, hwnd, (HMENU)ID_LISTBOX,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
		LoadProgramBitmaps((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), hwnd);
		if(LoadPrefabListFromDisk(g_PrefabList, g_Prefabfilename)){
			ReorderPrefabList(g_PrefabList);
			LoadPrefab(0);
		}
		else{
		InitializePrefab(&g_CurrentPrefab, 10,10);
		MessageBox(hwnd, "Unable to load prefabs.dsa - Starting with default 10 x 10 prefab.", "Error...", MB_OK);
		}
		g_ScrollCoords.x = 0;
		g_ScrollCoords.y = 0;
		
		return 0;
	case WM_COMMAND: //THIS IS FROM THE LISTBOX.
		if(LOWORD(wparam) == ID_LISTBOX){
			//THEN GET THE CURRENTSELECTION, CHECK AGAINST ID. IF DIFFERENT, LOAD UP THE NEW ID BUT ASK FOR SAVE IF NEEDED.
			int index= SendMessage(g_ListHwnd, CB_GETCURSEL, 0,0);
			int value = SendMessage(g_ListHwnd, CB_GETITEMDATA, index, 0);
			if(value != g_CurrentPrefab.id){
				if(g_NeedSave){
						if(MessageBox(hwnd, "Do You Wish To Save Changes To This Prefab?", "Load Preparation... ", MB_YESNO) == IDYES){
				SavePrefab(g_PrefabList, &g_CurrentPrefab);
				g_NeedSave = false;
			}
				}
				LoadPrefab(value);
				UpdateBackground(hwnd);
		UpdateTileTypes(hwnd);
		UpdateTileGrid(hwnd);
			}
		}
		return 0;
	case WM_SIZE:
	case WM_SHOWWINDOW:
	case WM_ACTIVATE:
		if(!g_Exit){
		UpdateBackground(hwnd);
		UpdateTileTypes(hwnd);
		UpdateTileGrid(hwnd);
	//	ShowWindow(g_ListHwnd, SW_SHOW);
		}
		return 0;
	case WM_LBUTTONDOWN:
		cursor.x = LOWORD(lparam);
		cursor.y = HIWORD(lparam);
		if(HandleTileTypeAreaClick(hwnd, cursor)){
			UpdateTileTypes(hwnd);
	//		ShowWindow(g_ListHwnd, SW_SHOW);
			return 0;
		}
		else if(HandleTileGridAreaClick(hwnd, cursor)){
			UpdateTileGrid(hwnd);
	//		ShowWindow(g_ListHwnd, SW_SHOW);
			return 0;
		}
		else{ 
			HandleButtonClicks(hwnd, cursor);
			if(!g_Exit){
			UpdateTileGrid(hwnd);
			UpdateTileTypes(hwnd);
			}
			return 0;
		}
	case WM_RBUTTONDOWN:
		SetCursor(g_ScrollCursorIcon);
		if(!g_Scrolling){
		g_Scrolling = true;
		g_ScrollCursor.x = LOWORD(lparam);
		g_ScrollCursor.y = HIWORD(lparam);
		}
		else{
			//PROCESS DELTA HERE.
			delta.x = LOWORD(lparam) - g_ScrollCursor.x;
			delta.y = HIWORD(lparam) - g_ScrollCursor.y;
			UpdateScrollCoords(delta);
			UpdateTileGrid(hwnd);
			g_ScrollCursor.x = LOWORD(lparam);
		g_ScrollCursor.y = HIWORD(lparam);
		}
		return 0;
	case WM_MOUSEMOVE:
		cursor.x = LOWORD(lparam);
		cursor.y = HIWORD(lparam);
		if(wparam & MK_RBUTTON){	
			SetCursor(g_ScrollCursorIcon);
		if(!g_Scrolling){
		g_Scrolling = true;
		g_ScrollCursor.x = LOWORD(lparam);
		g_ScrollCursor.y = HIWORD(lparam);
		}
		else{
			//PROCESS DELTA HERE.
			delta.x = LOWORD(lparam) - g_ScrollCursor.x;
			delta.y = HIWORD(lparam) - g_ScrollCursor.y;
			UpdateScrollCoords(delta);
			UpdateTileGrid(hwnd);
			g_ScrollCursor.x = LOWORD(lparam);
		g_ScrollCursor.y = HIWORD(lparam);
		}
		}
		else if (wparam & MK_LBUTTON){
		if(HandleTileGridAreaClick(hwnd, cursor)){
			UpdateTileGrid(hwnd);
	//		ShowWindow(g_ListHwnd, SW_SHOW);
			return 0;
		}
		}
		return 0;
	case WM_RBUTTONUP:
		g_Scrolling = false;
		SetCursor(g_StandardCursorIcon);
		return 0;
	case WM_DESTROY:
		WritePrefabListToDisk(g_PrefabList, g_Prefabfilename);
		FreePrefabListEntries(g_PrefabList);
		freeList(g_PrefabList);
	//	TerminatePrefab(&g_CurrentPrefab);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void HandleButtonClicks(HWND hwnd, POINT cursor)
{
	PPREFAB gfab = &g_CurrentPrefab;
	if(cursor.x > EXITBUTTONXORIGIN && cursor.x < EXITBUTTONXORIGIN + EXITBUTTONWIDTH && cursor.y > EXITBUTTONYORIGIN && cursor.y < EXITBUTTONYORIGIN + EXITBUTTONHEIGHT){
		//YOU JUST CLICKED THE EXIT BUTTON
		if (g_NeedSave){
			if(MessageBox(hwnd, "Do You Wish To Save Changes To This Prefab?", "Exiting...", MB_YESNO) == IDYES){
				SavePrefab(g_PrefabList, &g_CurrentPrefab);
			}
		}
		g_Exit = true;
		SendMessage(hwnd, WM_DESTROY, 0,0);
		return;
	}
	if(cursor.x > DELETEBUTTONXORIGIN && cursor.x < DELETEBUTTONXORIGIN + DELETEBUTTONWIDTH && cursor.y > DELETEBUTTONYORIGIN && cursor.y < DELETEBUTTONYORIGIN + DELETEBUTTONHEIGHT){
		DeletePrefab(gfab, g_PrefabList);
	}
	if(cursor.x > SAVEBUTTONXORIGIN && cursor.x < SAVEBUTTONXORIGIN + SAVEBUTTONWIDTH && cursor.y > SAVEBUTTONYORIGIN && cursor.y < SAVEBUTTONYORIGIN + SAVEBUTTONHEIGHT){
		SavePrefab(g_PrefabList, &g_CurrentPrefab);
	}
	if(cursor.x > NEWBUTTONXORIGIN && cursor.x < NEWBUTTONXORIGIN + NEWBUTTONWIDTH && cursor.y > NEWBUTTONYORIGIN && cursor.y < NEWBUTTONYORIGIN + NEWBUTTONHEIGHT){
		NewPrefab(g_PrefabList, &g_CurrentPrefab);
	}
}

void NewPrefab(PLIST prefablist, PPREFAB prefab)
{
		if (g_NeedSave){
			if(MessageBox(g_Hwnd, "Do You Wish To Save Changes To This Prefab?", "Exiting...", MB_YESNO) == IDYES){
				SavePrefab(g_PrefabList, &g_CurrentPrefab);
			}
		}
		DialogBox((HINSTANCE)GetWindowLong(g_Hwnd, GWL_HINSTANCE),MAKEINTRESOURCE(NEWPREFABBOX),g_Hwnd, NewPrefabBoxProc);
		g_ScrollCoords.x = 0;
		g_ScrollCoords.y = 0;
		UpdateBackground(g_Hwnd);
		UpdateTileTypes(g_Hwnd);
		UpdateTileGrid(g_Hwnd);

}

int GetNextAvailablePrefabID()
{
	int id = 0;
	if(g_PrefabList == NULL || g_PrefabList->objcnt < 1){
		return id;
	}
	PLIST walker = g_PrefabList;
	walker->current = walker->head;
	while(walker->current != NULL)
	{
		id = walker->current->object.prefab.id;
		walker->current = walker->current->nextnode;
	}
	return id + 1;
}

int CALLBACK NewPrefabBoxProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
//	int retval;
	int width;
	int height;
	char buf[200];

	switch (msg){
	case WM_INITDIALOG:	
		SendDlgItemMessage(hwnd, WIDTHBOX, WM_SETTEXT, 0, (LPARAM)"20");
		SendDlgItemMessage(hwnd, HEIGHTBOX, WM_SETTEXT, 0, (LPARAM)"20");
		return 1;
	
	case WM_COMMAND:
		switch (LOWORD(wparam))
		{
		case IDOK:
			SendDlgItemMessage(hwnd, WIDTHBOX, WM_GETTEXT, 200, (LPARAM)buf);
			width = atoi(buf);
			SendDlgItemMessage(hwnd, HEIGHTBOX, WM_GETTEXT, 200, (LPARAM)buf);
			height = atoi(buf);
			//CHECK OUR VALUES;
			if(height < 1 || width < 1){
				MessageBox(hwnd, "Your inputted values for Width and Height are out of bounds.", "Error.", MB_OK);
			}
			else{
				//TerminatePrefab(&g_CurrentPrefab);
				InitializePrefab(&g_CurrentPrefab, width, height);
				//ASSIGN NEW ID TO PREFAB.
				g_CurrentPrefab.id = GetNextAvailablePrefabID();
			}
			EndDialog(hwnd, 1);
			return 0;
		}
	}
	return 0;
}

void LoadProgramBitmaps(HINSTANCE hinst, HWND hwnd)
{
	char * tilebuttonpaths[NUMBERTILEBUTTONS] = {
		"graphics\\buttons\\tlcorner.bmp",
			"graphics\\buttons\\trcorner.bmp",
			"graphics\\buttons\\blcorner.bmp",
			"graphics\\buttons\\brcorner.bmp",
			"graphics\\buttons\\vwall.bmp",
			"graphics\\buttons\\hwall.bmp",
			"graphics\\buttons\\vdoor.bmp",
			"graphics\\buttons\\hdoor.bmp",
			"graphics\\buttons\\ground.bmp",
			"graphics\\buttons\\floor.bmp"
	};
	
	HDC maindc = GetDC(hwnd);
	g_NewBtnDC = CreateCompatibleDC(maindc);
	g_NewBtnBitmap = (HBITMAP) LoadImage(hinst, "graphics\\buttons\\new.bmp", IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
	SelectObject(g_NewBtnDC, g_NewBtnBitmap);
	g_ExitBtnDC = CreateCompatibleDC(maindc);
	g_ExitBtnBitmap = (HBITMAP) LoadImage(hinst, "graphics\\buttons\\exit.bmp", IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
	SelectObject(g_ExitBtnDC, g_ExitBtnBitmap);
	g_DeleteBtnDC = CreateCompatibleDC(maindc);
	g_DeleteBtnBitmap = (HBITMAP) LoadImage(hinst, "graphics\\buttons\\delete.bmp", IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
	SelectObject(g_DeleteBtnDC, g_DeleteBtnBitmap);
	g_SaveBtnDC = CreateCompatibleDC(maindc);
	g_SaveBtnBitmap = (HBITMAP) LoadImage(hinst, "graphics\\buttons\\save.bmp", IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
	SelectObject(g_SaveBtnDC, g_SaveBtnBitmap);
	g_BackgroundDC = CreateCompatibleDC(maindc);
	g_BackgroundBitmap = (HBITMAP) LoadImage(hinst, "graphics\\background.bmp", IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
	SelectObject(g_BackgroundDC, g_BackgroundBitmap);
	for(int i = 0; i < NUMBERTILEBUTTONS; i++){
		g_TileTypeDCs[i] = CreateCompatibleDC(maindc);
		g_TileTypeBitmaps[i] = (HBITMAP) LoadImage(hinst, tilebuttonpaths[i], IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
		SelectObject(g_TileTypeDCs[i], g_TileTypeBitmaps[i]);
	}
	g_StandardCursorIcon = LoadCursor(NULL, IDC_ARROW);
	g_ScrollCursorIcon = LoadCursor(NULL, IDC_SIZEALL);
	g_GridBitmap = CreateCompatibleBitmap(maindc,  (GRIDAREAWIDTH + TILETYPEBUTTONWIDTH) - (GRIDAREAWIDTH + TILETYPEBUTTONWIDTH) % (TILETYPEBUTTONWIDTH),  (GRIDAREAHEIGHT + TILETYPEBUTTONHEIGHT) - (GRIDAREAHEIGHT + TILETYPEBUTTONHEIGHT) % (TILETYPEBUTTONHEIGHT));
	g_GridDC = CreateCompatibleDC(maindc);
	SelectObject(g_GridDC, g_GridBitmap);
	ReleaseDC(hwnd, maindc);
}

bool HandleTileGridAreaClick(HWND hwnd, POINT cursor)
{
	if(cursor.x > GRIDAREAWIDTH || cursor.y < GRIDAREASTARTY){
		return false;
	}

	//UPDATE NEEDSAVE.
	g_NeedSave = true;

	//STILL HAVE TO ACCOUNT FOR PARTIAL SCROLLING.........

	int tilex;
	int tiley;
	int newcursorx = cursor.x  - ((cursor.x / TILETYPEBUTTONWIDTH)); //TO ACCOUNT FOR 1 UNIT GRID SQUARES.
	tilex = (newcursorx + (g_ScrollCoords.x % TILETYPEBUTTONWIDTH)) / TILETYPEBUTTONWIDTH;
	int newcursory = cursor.y - GRIDAREASTARTY;
	newcursory -= (cursor.y / TILETYPEBUTTONHEIGHT);
	tiley = (newcursory + (g_ScrollCoords.y % TILETYPEBUTTONHEIGHT)) / TILETYPEBUTTONHEIGHT;
//THESE ARE ABSOLUTES. THEY NEED TO BE MOVED BY THE SCROLL BAR.
	int topytile = g_ScrollCoords.y / TILETYPEBUTTONHEIGHT; //NOW, THIS IS OUR TOP ROW.
	int bottomytile = topytile + (GRIDAREAHEIGHT / TILETYPEBUTTONHEIGHT);
	//NOW, BOUNDS CHECK THE BOTTOM Y TILE.
	if (bottomytile >= g_CurrentPrefab.height){
		bottomytile = g_CurrentPrefab.height - 1;
	}
	
	
	
	//NOW, CHECK X
	int leftxtile = g_ScrollCoords.x / TILETYPEBUTTONWIDTH;
	int rightxtile = leftxtile + (GRIDAREAWIDTH / TILETYPEBUTTONWIDTH);
	//NOW, BOUNDS CHECK THE RIGHT X TILE.
	if(rightxtile >= g_CurrentPrefab.width){
		rightxtile = g_CurrentPrefab.width - 1;
	}

	tilex += leftxtile;
	tiley += topytile;

	if(tilex >= g_CurrentPrefab.width){
		return false;
	}
	if(tiley >= g_CurrentPrefab.height){
		return false;
	}

	//NOW ACTUALLY CHANGE THE PREFAB.
	if(g_GlobalTileType > -1){
	g_CurrentPrefab.prefabdata[tiley][tilex] = TileTypeToPrefabData(g_GlobalTileType);
	}
//	UpdateTileGrid(hwnd);
	return true;
}

bool HandleTileTypeAreaClick(HWND hwnd, POINT cursor)
{
	RECT srcoord;
	bool downline = true;
	int columnoffset = TILETYPEBUTTONWIDTH + TILETYPEBUTTONHSPACER;
	int rowoffset = TILETYPEBUTTONHEIGHT + TILETYPEBUTTONVSPACER;
	int numrows = NUMBERTILEBUTTONS / 2;
	int numcols = NUMBERTILEBUTTONS / numrows;
	
	//WE'LL JUST ITERATE FROM TOP TO BOTTOM

	if(cursor.x < TILETYPEBUTTONFIRSTX || cursor.y < TILETYPEBUTTONFIRSTY || cursor.y > TILETYPEBUTTONFIRSTY + (((NUMBERTILEBUTTONS / 2) * TILETYPEBUTTONHEIGHT) + ((NUMBERTILEBUTTONS / 2) * TILETYPEBUTTONVSPACER)) ){
		return false;
	}
	srcoord.top = TILETYPEBUTTONFIRSTY;
	srcoord.bottom = srcoord.top + TILETYPEBUTTONHEIGHT;
	for(int col = 0; col < numcols; col++){
		for(int row = 0; row < numrows; row++)
		{
	srcoord.left = TILETYPEBUTTONFIRSTX + (columnoffset * col);
	srcoord.right = srcoord.left + TILETYPEBUTTONWIDTH;
	srcoord.top =  TILETYPEBUTTONFIRSTY + (rowoffset * row);
	srcoord.bottom = srcoord.top + TILETYPEBUTTONHEIGHT;
	if(cursor.x >= srcoord.left && cursor.x < srcoord.right && cursor.y >= srcoord.top && cursor.y < srcoord.bottom){ //IF WE'rE IN THE BUTTON
		g_GlobalTileType = col + (row * numcols);
		return true;
		}
		}
	}
	return true;
}

void UpdateTileGrid(HWND hwnd)
{
	//START BY DRAWING GRID TO GRID DC.
//	HPEN gridpen = CreatePen(PS_SOLID, 1, RGB(100,100,100));
	
	if(g_CurrentPrefab.prefabdata == NULL){
		return;
	}
	
	HDC maindc = GetDC(hwnd);
	int hlineiterations = (GRIDAREAHEIGHT / TILETYPEBUTTONHEIGHT);
	int vlineiterations = (GRIDAREAWIDTH / TILETYPEBUTTONWIDTH);
		int gridblitwidth = GRIDAREAWIDTH;
	int gridblitheight = GRIDAREAHEIGHT;
	int correcttiletype;
//	POINT point;
	RECT gridrect;
	gridrect.left = 0;
	gridrect.top = 0;
	gridrect.right =(GRIDAREAWIDTH + TILETYPEBUTTONWIDTH) - (GRIDAREAWIDTH + TILETYPEBUTTONWIDTH) % (TILETYPEBUTTONWIDTH);
	gridrect.bottom = (GRIDAREAHEIGHT + TILETYPEBUTTONHEIGHT) - (GRIDAREAHEIGHT + TILETYPEBUTTONHEIGHT) % (TILETYPEBUTTONHEIGHT);
	

	//WE REALLY ONLY WANT TO ITERATE THE NUMBER OF GRID SPACES THAT ARE LEFT.
	//LET'S CHECK OUT HOW MANY TILES WE HAVE, AND OUR CURRENT SCROLL POSITION.
	
	int topytile = g_ScrollCoords.y / TILETYPEBUTTONHEIGHT; //NOW, THIS IS OUR TOP ROW.
	int bottomytile = topytile + (GRIDAREAHEIGHT / TILETYPEBUTTONHEIGHT) + 1;
	//NOW, BOUNDS CHECK THE BOTTOM Y TILE.
	if (bottomytile >= g_CurrentPrefab.height){
		bottomytile = g_CurrentPrefab.height - 1;
	}
	
	
	
	//NOW, CHECK X
	int leftxtile = g_ScrollCoords.x / TILETYPEBUTTONWIDTH;
	int rightxtile = leftxtile + (GRIDAREAWIDTH / TILETYPEBUTTONWIDTH) + 1;
	//NOW, BOUNDS CHECK THE RIGHT X TILE.
	if(rightxtile >= g_CurrentPrefab.width){
		rightxtile = g_CurrentPrefab.width - 1;
	}
	
	
	//RESTRICT THE H AND V ITERATIONS TO ACTUAL COORDINATES.
	if(hlineiterations + topytile > bottomytile){
		hlineiterations = bottomytile - topytile + 1;
		gridblitheight = hlineiterations * TILETYPEBUTTONHEIGHT + hlineiterations;
	}
	if (vlineiterations + leftxtile > rightxtile){
		vlineiterations = rightxtile - leftxtile + 1;
		gridblitwidth = vlineiterations * TILETYPEBUTTONWIDTH + vlineiterations;
	}
	
	
	
	FillRect(g_GridDC, &gridrect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	
//	SelectObject(g_GridDC, gridpen);
	//HORIZONTAL LINES
//	for(int h = 0; h < hlineiterations; h++)
//	{
//		MoveToEx(g_GridDC, 0, h * TILETYPEBUTTONHEIGHT, &point);
	//	LineTo(g_GridDC, GRIDAREAWIDTH, h*TILETYPEBUTTONHEIGHT);
//	}
	//VERTICAL LINES
//	for(int w = 0; w < vlineiterations; w++)
//	{
//		MoveToEx(g_GridDC, w * TILETYPEBUTTONWIDTH, 0, &point);
//		LineTo(g_GridDC,  w*TILETYPEBUTTONWIDTH, GRIDAREAHEIGHT);
//	}
	
	//NOW PUT TILES INTO THE GRID.
	for(int h = 0; h < hlineiterations; h++){
		
		for(int w = 0; w < vlineiterations; w++){
			correcttiletype = PrefabDataToTileType(g_CurrentPrefab.prefabdata[topytile + h][leftxtile + w]);
			BitBlt(g_GridDC, w * TILETYPEBUTTONWIDTH + w, h * TILETYPEBUTTONHEIGHT + h, TILETYPEBUTTONWIDTH, TILETYPEBUTTONHEIGHT, g_TileTypeDCs[correcttiletype], 0,0, SRCCOPY);
		}
	}


	
	//BLT BACKGROUND TO MAIN DC...
	BitBlt(maindc, 0,HEIGHTFROMTOPTOSCROLLBAR + HSCROLLBARHEIGHT,GRIDAREAWIDTH, GRIDAREAHEIGHT, g_BackgroundDC, 0,HEIGHTFROMTOPTOSCROLLBAR + HSCROLLBARHEIGHT, SRCCOPY);
	//BLT OVER WITH GRID DC.
	BitBlt(maindc, 0, HEIGHTFROMTOPTOSCROLLBAR + HSCROLLBARHEIGHT, gridblitwidth, gridblitheight, g_GridDC, g_ScrollCoords.x % TILETYPEBUTTONWIDTH, g_ScrollCoords.y % TILETYPEBUTTONHEIGHT, SRCCOPY);
	//DRAW BORDER
//	HPEN gridborderpen = CreatePen(PS_SOLID, 2, RGB(0,0,0));
	/*
	SelectObject(maindc, gridborderpen);
	MoveToEx(maindc, 0,HEIGHTFROMTOPTOSCROLLBAR + HSCROLLBARHEIGHT, &point);
	LineTo(maindc, GRIDAREAWIDTH, HEIGHTFROMTOPTOSCROLLBAR + HSCROLLBARHEIGHT);
	LineTo(maindc, GRIDAREAWIDTH, HEIGHTFROMTOPTOSCROLLBAR + HSCROLLBARHEIGHT + GRIDAREAHEIGHT);
	LineTo(maindc, 0, HEIGHTFROMTOPTOSCROLLBAR + HSCROLLBARHEIGHT + GRIDAREAHEIGHT);
	LineTo(maindc, 0, HEIGHTFROMTOPTOSCROLLBAR + HSCROLLBARHEIGHT);
	*/
	
	//FREE UP
	
	SelectObject(g_GridDC, NULL);
//	SelectObject(maindc, gridborderpen);
	//DeleteObject(gridpen);
//	DeleteObject(gridborderpen);
	SelectObject(g_GridDC, g_GridBitmap);
	ReleaseDC(hwnd, maindc);
	
}



void UpdateTileTypes(HWND hwnd)
{
/*
#define TILETYPEBUTTONFIRSTX	576
#define TILETYPEBUTTONFIRSTY	45
#define TILETYPEBUTTONHSPACER	15
#define TILETYPEBUTTONVSPACER	15
#define TILETYPEBUTTONWIDTH			48
#define TILETYPEBUTTONHEIGHT		48
	*/
	
	int ycounter;
	int selectedyline;
	int selectedxline;
	POINT point;
	RECT srcoord;
	
	HDC maindc = GetDC(hwnd);
	//NEED TO UPDATE BACKGROUND HERE.
	BitBlt(maindc, TILETYPEBUTTONFIRSTX - 5, TILETYPEBUTTONFIRSTY - 5, WINDOWWIDTH - (TILETYPEBUTTONFIRSTX - 5), WINDOWHEIGHT - (TILETYPEBUTTONFIRSTY), g_BackgroundDC,TILETYPEBUTTONFIRSTX - 5, TILETYPEBUTTONFIRSTY - 5, SRCCOPY); 
	
	//NOW UPDATE TILES.
	for(ycounter = 0; ycounter < NUMBERTILEBUTTONS / 2; ycounter++){
		BitBlt(maindc, TILETYPEBUTTONFIRSTX, TILETYPEBUTTONFIRSTY + ((TILETYPEBUTTONVSPACER + TILETYPEBUTTONHEIGHT) * ycounter), TILETYPEBUTTONWIDTH, TILETYPEBUTTONHEIGHT, g_TileTypeDCs[ycounter*2], 0,0, SRCCOPY);
		BitBlt(maindc, TILETYPEBUTTONFIRSTX + TILETYPEBUTTONHSPACER + TILETYPEBUTTONWIDTH, TILETYPEBUTTONFIRSTY + ((TILETYPEBUTTONVSPACER + TILETYPEBUTTONHEIGHT) * ycounter), TILETYPEBUTTONWIDTH, TILETYPEBUTTONHEIGHT, g_TileTypeDCs[ycounter*2 + 1], 0,0, SRCCOPY);
	}
	
	//NOW CREATE PEN AND DRAW 2 WIDTH BOX AROUND SELECTED TILE TYPE
	HPEN pen = CreatePen(PS_SOLID, 2,  RGB(200,0,0));
	SelectObject(maindc, pen);
	if(g_GlobalTileType > -1 && g_GlobalTileType < NUMBERTILEBUTTONS)
	{
		selectedyline = g_GlobalTileType / 2;
		selectedxline = (g_GlobalTileType % 2); //FANCY THING, EH?
		srcoord.left = TILETYPEBUTTONFIRSTX + ((TILETYPEBUTTONWIDTH + TILETYPEBUTTONHSPACER) * selectedxline);
		srcoord.right = srcoord.left + TILETYPEBUTTONWIDTH;
		srcoord.top = TILETYPEBUTTONFIRSTY + ((TILETYPEBUTTONHEIGHT + TILETYPEBUTTONVSPACER) * selectedyline);
		srcoord.bottom = srcoord.top + TILETYPEBUTTONHEIGHT;
		MoveToEx(maindc, srcoord.left, srcoord.top, &point);
		LineTo(maindc, srcoord.right, srcoord.top);
		LineTo(maindc, srcoord.right, srcoord.bottom);
		LineTo(maindc, srcoord.left, srcoord.bottom);
		LineTo(maindc, srcoord.left, srcoord.top);
	}

	//UPDATE BUTTONS
	BitBlt(maindc, EXITBUTTONXORIGIN, EXITBUTTONYORIGIN, EXITBUTTONWIDTH, EXITBUTTONHEIGHT, g_ExitBtnDC, 0,0, SRCCOPY);
	BitBlt(maindc, SAVEBUTTONXORIGIN, SAVEBUTTONYORIGIN, SAVEBUTTONWIDTH, SAVEBUTTONHEIGHT, g_SaveBtnDC, 0,0, SRCCOPY);
	BitBlt(maindc, DELETEBUTTONXORIGIN, DELETEBUTTONYORIGIN, DELETEBUTTONWIDTH, DELETEBUTTONHEIGHT, g_DeleteBtnDC, 0,0, SRCCOPY);
	BitBlt(maindc, NEWBUTTONXORIGIN, NEWBUTTONYORIGIN, NEWBUTTONWIDTH, NEWBUTTONHEIGHT, g_NewBtnDC, 0,0, SRCCOPY);
	
	//FREE ALL STUFF.
	SelectObject(maindc, NULL);
	DeleteObject(pen);
	ReleaseDC(hwnd, maindc);
}

void TerminateProgramBitmaps()
{
		SelectObject(g_NewBtnDC, NULL);
	DeleteObject(g_NewBtnBitmap);
	DeleteDC(g_NewBtnDC);
	SelectObject(g_ExitBtnDC, NULL);
	DeleteObject(g_ExitBtnBitmap);
	DeleteDC(g_ExitBtnDC);
		SelectObject(g_SaveBtnDC, NULL);
	DeleteObject(g_SaveBtnBitmap);
	DeleteDC(g_SaveBtnDC);
		SelectObject(g_DeleteBtnDC, NULL);
	DeleteObject(g_DeleteBtnBitmap);
	DeleteDC(g_DeleteBtnDC);
	SelectObject(g_BackgroundDC, NULL);
	DeleteObject(g_BackgroundBitmap);
	DeleteDC(g_BackgroundDC);
	SelectObject(g_GridDC, NULL);
	DeleteObject(g_GridBitmap);
	DeleteDC(g_GridDC);
	for(int i = 0; i < NUMBERTILEBUTTONS; i++){
		SelectObject(g_TileTypeDCs[i], NULL);
		DeleteObject(g_TileTypeBitmaps[i]);
		DeleteDC(g_TileTypeDCs[i]);
	}
	
}

void DeletePrefab(PPREFAB prefab, PLIST prefablist)
{
	if (MessageBox(g_Hwnd, "Are You Sure You Wish To Delete This Prefab?", "Confirmation Required", MB_YESNO) != IDYES){
		return;
	}
 	
	
	//GO FIND THE ID IN THE LISTBOX, DELETE IT.	
	int value;
	for(int i = 0; i < prefablist->objcnt; i++){
		value = SendMessage(g_ListHwnd, CB_GETITEMDATA, i, 0);
		if(value == prefab->id){
			SendMessage(g_ListHwnd, CB_DELETESTRING, i, 0);
			break;
		}
	}
	//KILL THE PREFAB. POW!
	RemovePrefabFromList(prefablist, prefab->id);
	TerminatePrefab(prefab);

	//RUN THROUGH LIST, REORDER
	ReorderPrefabList(prefablist);
	
	//LOAD THE FIRST PREFAB IN THE LIST.
		if(prefablist->objcnt > 0){
		LoadPrefab(0);
		}
		else{
			MessageBox(g_Hwnd, "Prefab List Is Empty. Creating 20x20 Default Prefab.", "Empty List", MB_OK);
			InitializePrefab(prefab, 20,20);
		}
	g_ScrollCoords.x = 0;
		g_ScrollCoords.y = 0;
	g_NeedSave = false;
}

void ReorderPrefabList(PLIST prefablist)
{
	int newid = 0;
	char buf[200];
	int index;
	PLIST walker = prefablist;
	walker->current = walker->head;
	while(walker->current != NULL){
		walker->current->object.prefab.id = newid;
		newid++;
		walker->current = walker->current->nextnode;
	}

	//GOOD. CLEAR OUT THE LISTBOX AND RE-INITIALIZE.
	SendMessage(g_ListHwnd, CB_RESETCONTENT, 0,0);

	newid = 0;
	walker->current = walker->head;
	while(walker->current != NULL){
	sprintf(buf, "PREFAB ID#%15.15d", walker->current->object.prefab.id);
	index = SendMessage(g_ListHwnd, CB_ADDSTRING, 0, (LPARAM)buf);
	SendMessage(g_ListHwnd, CB_SETITEMDATA, (WPARAM)index, (LPARAM)walker->current->object.prefab.id);
	SendMessage(g_ListHwnd, CB_SETCURSEL, (WPARAM)index, 0);
	walker->current = walker->current->nextnode;
	}
}

void SavePrefab(PLIST prefablist, PPREFAB prefab)
{
	POTYPE temp;
	PPOTYPE found;
//	PREFAB newprefab;
	//FIRST, RUN A CHECK ON THE PREFAB TO DETERMINE LEGITIMACY.
	if(!CheckPrefabForSave(prefab)){
		return;
	}
	temp.prefab.id = prefab->id;
	found = findObject(prefablist, &temp, PrefabIDCompare);
	if(!found){ //THEN ADD TO LIST.
		//MAKE A NEW COPY.
		AddPrefabToList(prefab, prefablist);
		//BETTER ADD IT TO THE LIST BOX.
		char buf[200];
	sprintf(buf, "PREFAB ID#%15.15d", prefab->id);
	int index = SendMessage(g_ListHwnd, CB_ADDSTRING, 0, (LPARAM)buf);
	SendMessage(g_ListHwnd, CB_SETITEMDATA, (WPARAM)index, (LPARAM)prefab->id);
	SendMessage(g_ListHwnd, CB_SETCURSEL, (WPARAM)index, 0);
	}
	else{ //MODIFY EXISTING.
//		TerminatePrefab(&found->prefab);
	//	found->prefab.id = prefab->id;
		FillPrefab(&found->prefab, prefab->prefabdata, prefab->width, prefab->height);
	}
	
	
	g_NeedSave = false;
}

bool CheckPrefabForSave(PPREFAB prefab)
{
	//RUN AROUND THE PREFAB AND CHECK FOR ONE OF EACH TYPE OF CORNER. IF THERE'S AT LEAST ONE, THERE SHOULD BE AN EXIT, TOO.
	int numbertl = 0;
	int numbertr = 0;
	int numberbr = 0;
	int numberbl = 0;
	int numberexit = 0;
	int others = 0;

	for(int h = 0; h < prefab->height; h++){
		for(int w = 0; w < prefab->width; w++){
			switch(prefab->prefabdata[h][w])
			{
			case BUILDING_SYM_BLCORNER:
				numberbl++;
				break;
			case BUILDING_SYM_BRCORNER:
				numberbr++;
				break;
			case BUILDING_SYM_TRCORNER:
				numbertr++;
				break;
			case BUILDING_SYM_TLCORNER:
				numbertl++;
				break;
			case BUILDING_SYM_VDOOR:
			case BUILDING_SYM_HDOOR:
				numberexit++;
				break;
			case OBJECT_SYM_GROUND:
			case OBJECT_SYM_FLOOR:
				break;
			default:
				others++;
			}
		}
	}

	if(!numbertl && !numbertr && !numberbr && !numberbl && !numberexit && !others){
		MessageBox(g_Hwnd, "This is an empty prefab. Saving Cancelled.", "Saving Cancelled.", MB_OK);
		return false;
	}

	//NUMBER OF BUILDINGS IS THE MINIMUM OF THE NUMBER OF CORNERS.
	int numberbuildings = numbertl;
	if (numbertr < numberbuildings){
		numberbuildings = numbertr;
	}
	if (numberbr < numberbuildings){
		numberbuildings = numberbr;
	}
	if(numberbl < numberbuildings){
		numberbuildings = numberbl;
	}
	if(numberbuildings > numberexit){
		//HMMM.
		if(MessageBox(g_Hwnd, "It appears that you don't have enough exits for the buildings in your prefab. Do you wish to save anyway?", "Concern", MB_YESNO) != IDYES){
			return false;
		}
	}
	CropPrefab(prefab);
	return true;
	
}


void CropPrefab(PPREFAB prefab)
{
	int firstrowwithdata;
	int lastrowwithdata;
	int firstcolwithdata;
	int lastcolwithdata;
	
	bool checkingforfirst = true;
	bool stoploop = false;
	int h;
	int w;
	
	//GET THE ROWS FIRST.
	for(h = 0; h < prefab->height && !stoploop ; h++){
		for(w = 0; w < prefab->width && !stoploop; w++){
			if(prefab->prefabdata[h][w] != OBJECT_SYM_GROUND){
				firstrowwithdata = h;
				stoploop = true;
			}
		}
	}

	stoploop = false;

	for(h = firstrowwithdata; h < prefab->height; h++){
		for(w = 0; w < prefab->width; w++){
			if(prefab->prefabdata[h][w] != OBJECT_SYM_GROUND){
				lastrowwithdata = h;
			}
		}
	}

	//GET THE COLUMNS NEXT.
	
	for(w = 0; w < prefab->width && !stoploop; w++){
			for(h = 0; h < prefab->height && !stoploop ; h++){
			if(prefab->prefabdata[h][w] != OBJECT_SYM_GROUND){
				firstcolwithdata = w;
				stoploop = true;
			}
		}
	}

	for(w = firstcolwithdata; w < prefab->width; w++){
			for(h = 0; h < prefab->height ; h++){
			if(prefab->prefabdata[h][w] != OBJECT_SYM_GROUND){
				lastcolwithdata = w;
			}
		}
	}

	//NOW. LET'S CHECK IF THERE'S ANY CROPPING NEEDED.
	if(firstcolwithdata || firstrowwithdata || lastcolwithdata + 1 < prefab->width || lastrowwithdata + 1 < prefab->height){ //IF THE FIRST ROW OR COL OF DATA IS NOT ROW OR COL ZERO...
		char buf[200];
		sprintf(buf, "It appears that this prefab could be 'cropped' down from %d x %d to a smaller size of %d x %d. Do you wish to do this?\n(If you wish to leave space for further modification, select NO.)", prefab->width, prefab->height, lastcolwithdata - firstcolwithdata + 1, lastrowwithdata - firstrowwithdata + 1);
		if(MessageBox(g_Hwnd, buf, "Cropping Allowed", MB_YESNO ) != IDYES){
			return;
		}
		else{ //CROP!
			g_ScrollCoords.x =0;
			g_ScrollCoords.y = 0;
			PREFAB tempprefab;
			InitializePrefab(&tempprefab, lastcolwithdata - firstcolwithdata + 1, lastrowwithdata - firstrowwithdata + 1);
			for(int srcH = firstrowwithdata, destH = 0; destH < tempprefab.height; destH++, srcH++){
				for(int srcW = firstcolwithdata, destW = 0; destW < tempprefab.width; srcW++, destW++){
					tempprefab.prefabdata[destH][destW] = prefab->prefabdata[srcH][srcW];
				}
			}
			TerminatePrefab(prefab);
			InitializePrefab(prefab, tempprefab.width, tempprefab.height);
			FillPrefab(prefab, tempprefab.prefabdata, tempprefab.width, tempprefab.height);
			TerminatePrefab(&tempprefab);
		}
	}
	else{
		return;
	}
	

}

void AddPrefabToList(PPREFAB prefab, PLIST prefablist)
{
	POTYPE temp;
	temp.prefab.id = prefab->id;
	InitializePrefab(&temp.prefab, prefab->width, prefab->height);
	FillPrefab(&temp.prefab, prefab->prefabdata, prefab->width, prefab->height);
	addtoList(prefablist, temp, PrefabIDCompare);
}

bool RemovePrefabFromList(PLIST prefablist, int id)
{
	POTYPE temp;
	PPOTYPE found;
	temp.prefab.id = id;
	found = findObject(prefablist, &temp, PrefabIDCompare);
	if(!found)
		return false;
	deleteObject(prefablist, found);
	return true;
}

void LoadPrefab(int id)
{
	POTYPE temp;
	PPOTYPE found;
//	PREFAB newprefab;
	//FIRST, RUN A CHECK ON THE PREFAB TO DETERMINE LEGITIMACY.
	temp.prefab.id = id;
	found = findObject(g_PrefabList, &temp, PrefabIDCompare);
	if(!found){ //UH...PROBLEM.
	}
	else{ 
		g_CurrentPrefab.height = found->prefab.height;
		g_CurrentPrefab.width = found->prefab.width;
		g_CurrentPrefab.id = found->prefab.id;
		g_CurrentPrefab.prefabdata = found->prefab.prefabdata;
	}
	//GO FIND THE ID IN THE LISTBOX, SET CURRENT SELECTION TO IT.
	int value;
	for(int i = 0; i < g_PrefabList->objcnt; i++){
		value = SendMessage(g_ListHwnd, CB_GETITEMDATA, i, 0);
		if(value == temp.prefab.id){
			SendMessage(g_ListHwnd, CB_SETCURSEL, i, 0);
			break;
		}
	}
	g_NeedSave = false;
}

bool LoadPrefabListFromDisk(PLIST prefablist, char* filename)
{
	POTYPE temp;
	PPREFAB array;
	int numelements;
	numelements = LoadPrefabArrayFromDisk(&array, filename);
	if(numelements < 1){
		return false;
	}
	for(int i = 0; i < numelements; i++){
		temp.prefab.id = array[i].id;
		temp.prefab.width = array[i].width;
		temp.prefab.height = array[i].height;
		temp.prefab.prefabdata = array[i].prefabdata;
		FillPrefab(&temp.prefab, array[i].prefabdata, array[i].width, array[i].height);
		addtoList(prefablist, temp, PrefabIDCompare);
	}
	
//	TerminatePrefabArray(&array, numelements);
	return true;
}

int PrefabIDCompare(PPOTYPE itm1,PPOTYPE itm2) 
{
	if(itm1->prefab.id == itm2->prefab.id)
		return (0);
	return (1);
}

void WritePrefabListToDisk(PLIST prefablist, char* filename)
{
	PPREFAB array;
	PLIST walker;
	int i =0;
	walker = prefablist;
	walker->current = walker->head;
	int numelements = prefablist->objcnt;
	if(numelements < 1){
		return;
	}
	array = (PPREFAB)malloc(sizeof(PREFAB) * numelements);
	while(walker->current != NULL){
		array[i].height = walker->current->object.prefab.height;
		array[i].width = walker->current->object.prefab.width;
		array[i].prefabdata = walker->current->object.prefab.prefabdata;
		FillPrefab(&array[i], walker->current->object.prefab.prefabdata, walker->current->object.prefab.width, walker->current->object.prefab.height);
		array[i].id = walker->current->object.prefab.id;
		i++;
		walker->current = walker->current->nextnode;
	}
	SavePrefabArrayToDisk(array, numelements, filename);
	free(array);
	//TerminatePrefabArray(&array, numelements);
}

void FreePrefabListEntries(PLIST prefablist)
{
	PLIST walker;
	walker = prefablist;
	walker->current = walker->head;
	while(walker->current != NULL){
		TerminatePrefab(&walker->current->object.prefab);
		walker->current = walker->current->nextnode;
	}
	return;
}
