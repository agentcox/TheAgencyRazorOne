#include "agencyinclude.h"

CHAR_INFO* g_BackBuffer;
void WriteToGlobalBackBuffer(int xorigin, int yorigin, int width, int height)
{
	COORD dimensions;
	dimensions.X = width;
	dimensions.Y = height;
	COORD origin;
	origin.X = 0;
	origin.Y = 0;
	SMALL_RECT readrect;
	readrect.Left = xorigin;
	readrect.Top = yorigin;
	readrect.Right = readrect.Left + width;
	readrect.Bottom = readrect.Top + height;

	g_BackBuffer = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * width * height);
	ReadConsoleOutput(getconsoleoutputh(), g_BackBuffer, dimensions, origin, &readrect);
}

void WriteFromGlobalBackBuffer(int xorigin, int yorigin, int width, int height)
{
		COORD dimensions;
	dimensions.X = width;
	dimensions.Y = height;
	COORD origin;
	origin.X = 0;
	origin.Y = 0;
	SMALL_RECT readrect;
	readrect.Left = xorigin;
	readrect.Top = yorigin;
	readrect.Right = readrect.Left + width;
	readrect.Bottom = readrect.Top + height;

	WriteConsoleOutput(getconsoleoutputh(), g_BackBuffer, dimensions, origin, &readrect);
	free(g_BackBuffer);
}

void DBTest()
{
	SARTUDATASTRUCT sdata;
	int integer;
	int selection;
	char buf[255];
	char name[255];
	char cname[255];
	assert(LoadSartuInputLists(&sdata));
	//OPERATE ON THESE, MAKE MISSIONS
	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_LARGE, 0, TRUE);
	StaticDialogBox("Welcome To", "You are about to try a demonstration of The Agency - Razor One ' s Dialog Box System.\n To Continue, Press ENTER.", "The Agency", &globals.dboxparams);
	SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_SMALL, 20, TRUE);
	cls(0);
	StringDialogBox("Enter", "Please Enter Your Last Name", "Your Name", name, 20, &globals.dboxparams);
	cls(0);
	integer = NumberDialogBox("Enter", "Please Enter Your Five-Digit ID Number", "Your Code", 5, &globals.dboxparams);
	cls(0);
	SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 0, TRUE);
	selection = SimpleListDialogBox("Codename", "Selection", sdata.agentcodenames, &globals.dboxparams,  CreateStringDBListItem);
	sprintf(cname, "%s", ReturnStringListItemByNumber(selection, sdata.agentcodenames));
	cls(0);
	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 0, TRUE);
	sprintf(buf, "Agent %s - Number %d\nCodename:%s\nIs This Correct?", name, integer, cname);
	YesNoDialogBox("Confirm", buf, "(Y)es/(N)o", &globals.dboxparams);
	FreeSartuInputLists(&sdata);
}

int CompareDbitemID(PPOTYPE itm1,PPOTYPE itm2) 
{
	if(itm1->dblistitem.id == itm2->dblistitem.id)
		return (0);
	return (1);
}

int CompareStringID(PPOTYPE itm1, PPOTYPE itm2)
{
	return 1;
}

PLIST BuildDBList(PLIST inputlist, DBLISTITEM(ItemCreate)(PPOTYPE object))
{
	
	PLIST ourlist;
	POTYPE item;
	
	ourlist = createList();
	inputlist->current = inputlist->head;
	while(inputlist->current->nextnode != NULL){
		item.dblistitem = ItemCreate(&inputlist->current->object);
		addtoList(ourlist, item, CompareDbitemID);
		inputlist->current = inputlist->current->nextnode;
	}
	return ourlist;
}

int GetLongestDBItemLength(PLIST dblist)
{
	unsigned int longest = 0;
	PLIST walker;
	walker = dblist;
	walker->current = walker->head;
	while(walker->current != NULL){
		if (strlen(walker->current->object.dblistitem.string) > longest){
			longest = strlen(walker->current->object.dblistitem.string);
			
		}
		walker->current = walker->current->nextnode;
	}
	return longest;
	
}

void FreeDBList(PLIST dblist)
{
	dblist->current = dblist->head;
	while(dblist->current->nextnode != NULL){
		free(dblist->current->object.dblistitem.string);
		dblist->current = dblist->current->nextnode;
	}
	freeList(dblist);
	free(dblist);
}

DBLISTITEM* ReturnDBListItemByNumber(int number, PLIST list)
{
	PLIST walker;
	walker = list;
	walker->current = walker->head;
	while(walker->current != NULL){
		if (list->current->object.dblistitem.id == number){
			return (&list->current->object.dblistitem);
		}
		walker->current = walker->current->nextnode;
	}
	return NULL;
}

PPOTYPE ReturnListItemByNumber(int index, PLIST list)
{
		PLIST walker;
	walker = list;
	walker->current = walker->head;
	int i = 0;
	while(walker->current != NULL){
		if (i == index){
			return (&list->current->object);
		}
		walker->current = walker->current->nextnode;
		i++; 
	}
	return NULL;
}

char* ReturnStringListItemByNumber(int index, PLIST list)
{
	int counter = 0;
	PLIST walker;
	walker = list;
	walker->current = walker->head;
	if (counter == index){
		return walker->current->object.string;
	}
	while(walker->current != NULL)
	{
		if(counter == index - 1){
			return walker->current->object.string;
		}
		else{
			counter++;
			walker->current = walker->current->nextnode;
		}
	}
	return NULL;
}

DBLISTITEM AlreadyDBListItem(PPOTYPE object)
{
	return object->dblistitem;
}

DBLISTITEM CreateStringDBListItem(PPOTYPE object)
{
	DBLISTITEM item;
	item.string = (char*) malloc(sizeof(char)* strlen(object->string) + 1);
	strcpy(item.string, object->string);
	return item;
}

DBLISTITEM CreateWorldWeaponDBListItem(PPOTYPE object)
{
	DBLISTITEM item;
	
	item.id = object->worldweapon.weapon.id;
	item.string = (char*)malloc(sizeof(char) * strlen(object->worldweapon.weapon.shortname) + 1);
	strcpy(item.string, object->worldweapon.weapon.shortname);
	return item;
}

int GetNumberOfStringLines(char* string)
{
	int len = strlen(string);
	int numlines =1;
	for (int i = 0 ; i < len; i++){
		if (string[i] == '\n'){
			numlines++;
		}
	}
	return numlines;
}

void WrapStringToWidth(char* string, int width)
{
	int len = strlen(string);
	int currentpos = 0;
	int currentstaticpos = 0;
	int currentlinepos = 0;
	if (len < width){
		return;
	}
	//WE GO A LINE AT A TIME, A LINE EQUAL TO THE WIDTH PASSED IN.
	//IF WE FIND A \n IN THE STRING, GO ON TO THE NEXT LINE.
	//IF WE FIND A \0 IN THE STRING, RETURN. WE'RE DONE.
	
	while(currentpos < len){
		//SCAN FORWARD FOR \n's and \0's.
		currentstaticpos = currentpos;
		
		for (currentlinepos = 0; currentlinepos < width; currentlinepos++){
			currentpos++;
			if (string[currentstaticpos+currentlinepos] == '\n'){
				//currentpos++;
				break;
			}
			else if (string[currentstaticpos+currentlinepos] == '\0'){
				return;
			}
		}
		//FORWARD SCAN COMPLETE.
		if (string[currentstaticpos+currentlinepos] != ' ' && string[currentstaticpos+currentlinepos] != '\n'){
			//REVERSE SCAN TO INSERT \n.
			for (;currentlinepos > 0; currentlinepos--){
				if (string[currentstaticpos+currentlinepos] == ' ' && currentlinepos < width){
					//WE'VE FOUND THE SPACE, INSERT THE \n.
					string[currentstaticpos+currentlinepos] = '\n';
					currentpos -= currentpos - currentlinepos;
					break;
				}
			}
			if (currentlinepos == 0){
				return; //WE HAVE A PROBLEM, TOO FAR.
			}
		}
		else{
			string[currentstaticpos+currentlinepos] = '\n';
			currentpos++;
		}
	}
	
}

int getObjectCenterCoordinate(int distance, int holdingdistance)
{
	return ((holdingdistance/2) - (distance / 2));
}

void StaticDialogBox(char* topstring, char* contents, char* bottomstring, PDBOXPARAM dbp)
{
	int key;
	unsigned int topstringxorigin;
	unsigned int bottomstringxorigin;
	unsigned int boxwidth;
	unsigned int boxheight;
	unsigned int xorigin;
	unsigned int yorigin;
	int width;
	bool ok = false;
	
	char* newcontents = new char[strlen(contents) + 1];
	COORD consize;
	
	
	consize  = getconsize();
	//WE DETERMINE THE WIDTH BY THE CALLER'S PARAM.
	switch(dbp->RelativeSize){
	case DB_SMALL:
		//SET INITIAL WIDTH TO BE DB_SMALL DEFAULT.
		for (width = consize.X / 3; width < consize.X / 2; width++)
		{
			//WRAP NEWCONTENTS, CHECK FOR HEIGHT, IF TOO HIGH, RAISE WIDTH AND TRY AGAIN.
			strcpy(newcontents, contents);
			WrapStringToWidth(newcontents, width);
			if (GetNumberOfStringLines(newcontents) < consize.Y - 5){
				ok = true;
				boxheight = GetNumberOfStringLines(newcontents)  +3; //ACCOUNT FOR BORDERS, ETC.
				boxwidth = width;
				break; // NICE WORK! WE'RE ALL SET.
			}
		}
		if (ok){
			break;
		}
		//ELSE FALL THROUGH TO MEDIUM AND TRY AGAIN.
	case DB_MEDIUM:
		for (width = consize.X / 2; width < ((3 * consize.X) / 4); width++)
		{
			//WRAP NEWCONTENTS, CHECK FOR HEIGHT, IF TOO HIGH, RAISE WIDTH AND TRY AGAIN.
			strcpy(newcontents, contents);
			WrapStringToWidth(newcontents, width);
			if (GetNumberOfStringLines(newcontents) < consize.Y - 5){
				ok = true;
				boxheight = GetNumberOfStringLines(newcontents)  + 3; //ACCOUNT FOR BORDERS, ETC.
				boxwidth = width;
				break; // NICE WORK! WE'RE ALL SET.
			}
		}
		if (ok){
			break;
		}
		//ELSE FALL THROUGH TO LARGE AND TRY AGAIN.
	case DB_LARGE:
		for (width = ((3 * consize.X) / 4); width < consize.X - 2; width++)
		{
			//WRAP NEWCONTENTS, CHECK FOR HEIGHT, IF TOO HIGH, RAISE WIDTH AND TRY AGAIN.
			strcpy(newcontents, contents);
			WrapStringToWidth(newcontents, width);
			if (GetNumberOfStringLines(newcontents) < consize.Y - 5){
				ok = true;
				boxheight = GetNumberOfStringLines(newcontents)  + 3; //ACCOUNT FOR BORDERS, ETC.
				boxwidth = width;
				break; // NICE WORK! WE'RE ALL SET.
			}
		}
		if (ok){
			break;
		}
		else{
			return; //YOU SUCK!
		}
	}
	
	//FINALLY, SET THE ORIGINS.
	xorigin = getObjectCenterCoordinate(boxwidth, consize.X);
	yorigin = getObjectCenterCoordinate(boxheight, consize.Y);
	
	//CHECK AND CENTER THE TOP AND BOTTOM TEXTS.
	if (strlen(topstring) < boxwidth){
		topstringxorigin = getObjectCenterCoordinate(strlen(topstring), boxwidth) + xorigin;
	}
	else{
		topstringxorigin = xorigin + 1;
	}
	
	if (strlen(bottomstring) < boxwidth){
		bottomstringxorigin = getObjectCenterCoordinate(strlen(bottomstring), boxwidth) + xorigin;
	}
	else{
		bottomstringxorigin= xorigin + 1;
	}

	WriteToGlobalBackBuffer(xorigin, yorigin, boxwidth, boxheight);
	
	//WE DRAW ONCE AND HOLD FOR ENTER KEY.
	DrawDialogBoxShell(dbp, boxwidth, boxheight, xorigin, yorigin);
	//NOW DRAW ALL TEXT.
	setcursor(topstringxorigin, yorigin);
	setcolor(dbp->TopStringFGColor | dbp->TopStringBGColor);
	writestring(topstring);
	setcursor(bottomstringxorigin, yorigin + boxheight - 1);
	setcolor(dbp->BottomStringFGColor | dbp->BottomStringBGColor);
	writestring(bottomstring);
	setcursor(xorigin + 1, yorigin + 1);
	strcpy(newcontents, contents);
	WrapStringToWidth(newcontents, boxwidth - 3);
	setcolor(dbp->CenterStringFGColor | dbp->DialogBoxBGColor);
	WriteDialogString(newcontents, xorigin +1, yorigin + 1, boxwidth, dbp->CenteredText);
	
	//WAIT HERE.
	while(1)
	{
		key  = waitforkey();
		if (key == VK_RETURN){
			delete [] newcontents;
			WriteFromGlobalBackBuffer(xorigin, yorigin, boxwidth, boxheight);
			return;
		}
	}
}

void StaticHoldDialogBox(char* topstring, char* contents, char* bottomstring, PDBOXPARAM dbp, int splittohold)
{
//	int key;
	unsigned int topstringxorigin;
	unsigned int bottomstringxorigin;
	unsigned int boxwidth;
	unsigned int boxheight;
	unsigned int xorigin;
	unsigned int yorigin;
	int width;
	bool ok = false;
	//TIMER Time;
	
	char* newcontents = new char[strlen(contents) + 1];
	COORD consize;
	
	
	consize  = getconsize();
	//WE DETERMINE THE WIDTH BY THE CALLER'S PARAM.
	switch(dbp->RelativeSize){
	case DB_SMALL:
		//SET INITIAL WIDTH TO BE DB_SMALL DEFAULT.
		for (width = consize.X / 3; width < consize.X / 2; width++)
		{
			//WRAP NEWCONTENTS, CHECK FOR HEIGHT, IF TOO HIGH, RAISE WIDTH AND TRY AGAIN.
			strcpy(newcontents, contents);
			WrapStringToWidth(newcontents, width);
			if (GetNumberOfStringLines(newcontents) < consize.Y - 5){
				ok = true;
				boxheight = GetNumberOfStringLines(newcontents)  +3; //ACCOUNT FOR BORDERS, ETC.
				boxwidth = width;
				break; // NICE WORK! WE'RE ALL SET.
			}
		}
		if (ok){
			break;
		}
		//ELSE FALL THROUGH TO MEDIUM AND TRY AGAIN.
	case DB_MEDIUM:
		for (width = consize.X / 2; width < ((3 * consize.X) / 4); width++)
		{
			//WRAP NEWCONTENTS, CHECK FOR HEIGHT, IF TOO HIGH, RAISE WIDTH AND TRY AGAIN.
			strcpy(newcontents, contents);
			WrapStringToWidth(newcontents, width);
			if (GetNumberOfStringLines(newcontents) < consize.Y - 5){
				ok = true;
				boxheight = GetNumberOfStringLines(newcontents)  + 3; //ACCOUNT FOR BORDERS, ETC.
				boxwidth = width;
				break; // NICE WORK! WE'RE ALL SET.
			}
		}
		if (ok){
			break;
		}
		//ELSE FALL THROUGH TO LARGE AND TRY AGAIN.
	case DB_LARGE:
		for (width = ((3 * consize.X) / 4); width < consize.X - 2; width++)
		{
			//WRAP NEWCONTENTS, CHECK FOR HEIGHT, IF TOO HIGH, RAISE WIDTH AND TRY AGAIN.
			strcpy(newcontents, contents);
			WrapStringToWidth(newcontents, width);
			if (GetNumberOfStringLines(newcontents) < consize.Y - 5){
				ok = true;
				boxheight = GetNumberOfStringLines(newcontents)  + 3; //ACCOUNT FOR BORDERS, ETC.
				boxwidth = width;
				break; // NICE WORK! WE'RE ALL SET.
			}
		}
		if (ok){
			break;
		}
		else{
			return; //YOU SUCK!
		}
	}
	
	//FINALLY, SET THE ORIGINS.
	xorigin = getObjectCenterCoordinate(boxwidth, consize.X);
	yorigin = getObjectCenterCoordinate(boxheight, consize.Y);
	
	//CHECK AND CENTER THE TOP AND BOTTOM TEXTS.
	if (strlen(topstring) < boxwidth){
		topstringxorigin = getObjectCenterCoordinate(strlen(topstring), boxwidth) + xorigin;
	}
	else{
		topstringxorigin = xorigin + 1;
	}
	
	if (strlen(bottomstring) < boxwidth){
		bottomstringxorigin = getObjectCenterCoordinate(strlen(bottomstring), boxwidth) + xorigin;
	}
	else{
		bottomstringxorigin= xorigin + 1;
	}

	WriteToGlobalBackBuffer(xorigin, yorigin, boxwidth, boxheight);
	
	//WE DRAW ONCE AND HOLD FOR ENTER KEY.
	DrawDialogBoxShell(dbp, boxwidth, boxheight, xorigin, yorigin);
	//NOW DRAW ALL TEXT.
	setcursor(topstringxorigin, yorigin);
	setcolor(dbp->TopStringFGColor | dbp->TopStringBGColor);
	writestring(topstring);
	setcursor(bottomstringxorigin, yorigin + boxheight - 1);
	setcolor(dbp->BottomStringFGColor | dbp->BottomStringBGColor);
	writestring(bottomstring);
	setcursor(xorigin + 1, yorigin + 1);
	strcpy(newcontents, contents);
	WrapStringToWidth(newcontents, boxwidth - 3);
	setcolor(dbp->CenterStringFGColor | dbp->DialogBoxBGColor);
	WriteDialogString(newcontents, xorigin +1, yorigin + 1, boxwidth, dbp->CenteredText);
	
	//WAIT HERE.
	int Mil = splittohold * 10;

		Sleep(Mil);
			delete [] newcontents;
			WriteFromGlobalBackBuffer(xorigin, yorigin, boxwidth, boxheight);
			return;
	
}

bool YesNoDialogBox(char* topstring, char* contents, char* bottomstring, PDBOXPARAM dbp)
{
	int key;
	unsigned int topstringxorigin;
	unsigned int bottomstringxorigin;
	unsigned int boxwidth;
	unsigned int boxheight;
	unsigned int xorigin;
	unsigned int yorigin;
	int width;
	bool ok = false;
	
	char* newcontents = new char[strlen(contents) + 1];
	COORD consize;
	
	
	consize  = getconsize();
	//WE DETERMINE THE WIDTH BY THE CALLER'S PARAM.
	switch(dbp->RelativeSize){
	case DB_SMALL:
		//SET INITIAL WIDTH TO BE DB_SMALL DEFAULT.
		for (width = consize.X / 3; width < consize.X / 2; width++)
		{
			//WRAP NEWCONTENTS, CHECK FOR HEIGHT, IF TOO HIGH, RAISE WIDTH AND TRY AGAIN.
			strcpy(newcontents, contents);
			WrapStringToWidth(newcontents, width);
			if (GetNumberOfStringLines(newcontents) < consize.Y - 5){
				ok = true;
				boxheight = GetNumberOfStringLines(newcontents)  +3; //ACCOUNT FOR BORDERS, ETC.
				boxwidth = width;
				break; // NICE WORK! WE'RE ALL SET.
			}
		}
		if (ok){
			break;
		}
		//ELSE FALL THROUGH TO MEDIUM AND TRY AGAIN.
	case DB_MEDIUM:
		for (width = consize.X / 2; width < ((3 * consize.X)) / 4; width++)
		{
			//WRAP NEWCONTENTS, CHECK FOR HEIGHT, IF TOO HIGH, RAISE WIDTH AND TRY AGAIN.
			strcpy(newcontents, contents);
			WrapStringToWidth(newcontents, width);
			if (GetNumberOfStringLines(newcontents) < consize.Y - 5){
				ok = true;
				boxheight = GetNumberOfStringLines(newcontents)  + 3; //ACCOUNT FOR BORDERS, ETC.
				boxwidth = width;
				break; // NICE WORK! WE'RE ALL SET.
			}
		}
		if (ok){
			break;
		}
		//ELSE FALL THROUGH TO LARGE AND TRY AGAIN.
	case DB_LARGE:
		for (width = ((3 * consize.X) / 4); width < consize.X - 2; width++)
		{
			//WRAP NEWCONTENTS, CHECK FOR HEIGHT, IF TOO HIGH, RAISE WIDTH AND TRY AGAIN.
			strcpy(newcontents, contents);
			WrapStringToWidth(newcontents, width);
			if (GetNumberOfStringLines(newcontents) < consize.Y - 5){
				ok = true;
				boxheight = GetNumberOfStringLines(newcontents)  + 3; //ACCOUNT FOR BORDERS, ETC.
				boxwidth = width;
				break; // NICE WORK! WE'RE ALL SET.
			}
		}
		if (ok){
			break;
		}
		else{
			return false; //YOU SUCK!
		}
	}
	
	//FINALLY, SET THE ORIGINS.
	xorigin = getObjectCenterCoordinate(boxwidth, consize.X);
	yorigin = getObjectCenterCoordinate(boxheight, consize.Y);
	
	//CHECK AND CENTER THE TOP AND BOTTOM TEXTS.
	if (strlen(topstring) < boxwidth){
		topstringxorigin = getObjectCenterCoordinate(strlen(topstring), boxwidth) + xorigin;
	}
	else{
		topstringxorigin = xorigin + 1;
	}
	
	if (strlen(bottomstring) < boxwidth){
		bottomstringxorigin = getObjectCenterCoordinate(strlen(bottomstring), boxwidth) + xorigin;
	}
	else{
		bottomstringxorigin= xorigin + 1;
	}
	
	WriteToGlobalBackBuffer(xorigin, yorigin, boxwidth, boxheight);
	//WE DRAW ONCE AND HOLD FOR ENTER KEY.
	DrawDialogBoxShell(dbp, boxwidth, boxheight, xorigin, yorigin);
	//NOW DRAW ALL TEXT.
	setcursor(topstringxorigin, yorigin);
	setcolor(dbp->TopStringFGColor | dbp->TopStringBGColor);
	writestring(topstring);
	setcursor(bottomstringxorigin, yorigin + boxheight - 1);
	setcolor(dbp->BottomStringFGColor | dbp->BottomStringBGColor);
	writestring(bottomstring);
	setcursor(xorigin + 1, yorigin + 1);
	strcpy(newcontents, contents);
	WrapStringToWidth(newcontents, boxwidth - 3);
	setcolor(dbp->CenterStringFGColor | dbp->DialogBoxBGColor);
	WriteDialogString(newcontents, xorigin +1, yorigin + 1, boxwidth, dbp->CenteredText);
	
	//WAIT HERE.
	while(1)
	{
		key  = waitforkey();
		if (key == 'Y' || key == 'y'){
			WriteFromGlobalBackBuffer(xorigin, yorigin, boxwidth, boxheight);
			delete [] newcontents;
			return true;
		}
		else if (key == 'N' || key == 'n'){
			WriteFromGlobalBackBuffer(xorigin, yorigin, boxwidth, boxheight);
			delete [] newcontents;
			return false;
		}
	}
}

void StringDialogBox(char* topstring, char* contents, char* bottomstring, char* outstring, int numchars, PDBOXPARAM dbp)
{
	int key;
	unsigned int topstringxorigin;
	unsigned int bottomstringxorigin;
	unsigned int boxwidth;
	unsigned int boxheight;
	unsigned int xorigin;
	unsigned int yorigin;
	int width;
	bool ok = false;
	int size = dbp->RelativeSize;
	char cursorchar = (char)254;
	
	char* newcontents = new char[strlen(contents) + 1];
	char* ourtempstring = new char[numchars + 1];
	int numberofcharstyped = 0;
	COORD consize;
	
	
	consize  = getconsize();
	
	
	if (numchars >= consize.X /2 && size == DB_SMALL){
		size = DB_MEDIUM;
	}
	if (numchars > ((3 * consize.X) /4) && (size == DB_SMALL || size == DB_MEDIUM)){
		size = DB_LARGE;
	}
	
	//WE DETERMINE THE WIDTH BY THE CALLER'S PARAM.
	switch(size){
	case DB_SMALL:
		//SET INITIAL WIDTH TO BE DB_SMALL DEFAULT.
		for (width = consize.X / 3; width < consize.X / 2; width++)
		{
			//WRAP NEWCONTENTS, CHECK FOR HEIGHT, IF TOO HIGH, RAISE WIDTH AND TRY AGAIN.
			strcpy(newcontents, contents);
			WrapStringToWidth(newcontents, width);
			if (GetNumberOfStringLines(newcontents) < consize.Y - 5){
				ok = true;
				boxheight = GetNumberOfStringLines(newcontents)  +5; //ACCOUNT FOR BORDERS, ETC.
				boxwidth = width;
				break; // NICE WORK! WE'RE ALL SET.
			}
		}
		if (ok){
			break;
		}
		//ELSE FALL THROUGH TO MEDIUM AND TRY AGAIN.
	case DB_MEDIUM:
		for (width = consize.X / 2; width < ((3 * consize.X)) / 4; width++)
		{
			//WRAP NEWCONTENTS, CHECK FOR HEIGHT, IF TOO HIGH, RAISE WIDTH AND TRY AGAIN.
			strcpy(newcontents, contents);
			WrapStringToWidth(newcontents, width);
			if (GetNumberOfStringLines(newcontents) < consize.Y - 5){
				ok = true;
				boxheight = GetNumberOfStringLines(newcontents)  + 5; //ACCOUNT FOR BORDERS, ETC.
				boxwidth = width;
				break; // NICE WORK! WE'RE ALL SET.
			}
		}
		if (ok){
			break;
		}
		//ELSE FALL THROUGH TO LARGE AND TRY AGAIN.
	case DB_LARGE:
		for (width = ((3 * consize.X) / 4); width < consize.X - 2; width++)
		{
			//WRAP NEWCONTENTS, CHECK FOR HEIGHT, IF TOO HIGH, RAISE WIDTH AND TRY AGAIN.
			strcpy(newcontents, contents);
			WrapStringToWidth(newcontents, width);
			if (GetNumberOfStringLines(newcontents) < consize.Y - 5){
				ok = true;
				boxheight = GetNumberOfStringLines(newcontents)  + 5; //ACCOUNT FOR BORDERS, ETC.
				boxwidth = width;
				break; // NICE WORK! WE'RE ALL SET.
			}
		}
		if (ok){
			break;
		}
		else{
			return; //YOU SUCK!
		}
	}
	
	//FINALLY, SET THE ORIGINS.
	xorigin = getObjectCenterCoordinate(boxwidth, consize.X);
	yorigin = getObjectCenterCoordinate(boxheight, consize.Y);
	
	//CHECK AND CENTER THE TOP AND BOTTOM TEXTS.
	if (strlen(topstring) < boxwidth){
		topstringxorigin = getObjectCenterCoordinate(strlen(topstring), boxwidth) + xorigin;
	}
	else{
		topstringxorigin = xorigin + 1;
	}
	
	if (strlen(bottomstring) < boxwidth){
		bottomstringxorigin = getObjectCenterCoordinate(strlen(bottomstring), boxwidth) + xorigin;
	}
	else{
		bottomstringxorigin= xorigin + 1;
	}
	WriteToGlobalBackBuffer(xorigin, yorigin, boxwidth, boxheight);
	//WE DRAW ONCE AND HOLD FOR ENTER KEY.
	DrawStringDialogBoxShell(dbp, boxwidth, boxheight, xorigin, yorigin);
	//NOW DRAW ALL TEXT.
	setcursor(topstringxorigin, yorigin);
	setcolor(dbp->TopStringFGColor | dbp->TopStringBGColor);
	writestring(topstring);
	setcursor(bottomstringxorigin, yorigin + boxheight - 1);
	setcolor(dbp->BottomStringFGColor | dbp->BottomStringBGColor);
	writestring(bottomstring);
	setcursor(xorigin + 1, yorigin + 1);
	strcpy(newcontents, contents);
	WrapStringToWidth(newcontents, boxwidth - 3);
	setcolor(dbp->CenterStringFGColor | dbp->DialogBoxBGColor);
	WriteDialogString(newcontents, xorigin +1, yorigin + 1, boxwidth, dbp->CenteredText);
	
	//DRAW INITIAL CURSOR.
	setcolor(dbp->InputtedTextColor);
	setcursor(xorigin + 1, yorigin + boxheight - 2);
	writechar(cursorchar);
	
	//WAIT HERE.
	while(1)
	{
		key  = waitforkey();
		if (key == VK_BACK && numberofcharstyped > 0){
			setcursor(xorigin + 1 + numberofcharstyped, yorigin + boxheight - 2);
			writechar(' ');
			numberofcharstyped--;
			ourtempstring[numberofcharstyped] = '\0';
			setcursor(xorigin + 1 + numberofcharstyped, yorigin + boxheight - 2);
			setcolor(dbp->TextCursorColor);
			writechar(cursorchar);
		}
		else if (isprint(key) && numberofcharstyped < numchars){
			setcolor(dbp->InputtedTextColor);
			setcursor(xorigin + 1 + numberofcharstyped, yorigin + boxheight - 2);
			writechar(key);
			ourtempstring[numberofcharstyped] = key;
			ourtempstring[numberofcharstyped + 1] = '\0';
			numberofcharstyped++;
			setcolor(dbp->TextCursorColor);
			writechar(cursorchar);
		}
		else if (key == VK_RETURN && numberofcharstyped > 0){
			strcpy(outstring, ourtempstring);
			delete [] newcontents;
			delete [] ourtempstring;
			FLUSH;
			WriteFromGlobalBackBuffer(xorigin, yorigin, boxwidth, boxheight);
			return;
		}
		FLUSH;
	}
}

int NumberDialogBox(char* topstring, char* contents, char* bottomstring, int numdigits, PDBOXPARAM dbp)
{
	int key;
	unsigned int topstringxorigin;
	unsigned int bottomstringxorigin;
	unsigned int boxwidth;
	unsigned int boxheight;
	unsigned int xorigin;
	unsigned int yorigin;
	int width;
	bool ok = false;
	int size = dbp->RelativeSize;
	char cursorchar = (char)254;
	int returnint;
	
	char* newcontents = new char[strlen(contents) + 1];
	char* ourtempstring = new char[numdigits + 1];
	int numberofcharstyped = 0;
	COORD consize;
	
	
	consize  = getconsize();
	
	
	if (numdigits >= consize.X /2 && size == DB_SMALL){
		size = DB_MEDIUM;
	}
	if (numdigits > ((3 * consize.X) /4) && (size == DB_SMALL || size == DB_MEDIUM)){
		size = DB_LARGE;
	}
	
	//WE DETERMINE THE WIDTH BY THE CALLER'S PARAM.
	switch(size){
	case DB_SMALL:
		//SET INITIAL WIDTH TO BE DB_SMALL DEFAULT.
		for (width = consize.X / 3; width < consize.X / 2; width++)
		{
			//WRAP NEWCONTENTS, CHECK FOR HEIGHT, IF TOO HIGH, RAISE WIDTH AND TRY AGAIN.
			strcpy(newcontents, contents);
			WrapStringToWidth(newcontents, width);
			if (GetNumberOfStringLines(newcontents) < consize.Y - 5){
				ok = true;
				boxheight = GetNumberOfStringLines(newcontents)  +5; //ACCOUNT FOR BORDERS, ETC.
				boxwidth = width;
				break; // NICE WORK! WE'RE ALL SET.
			}
		}
		if (ok){
			break;
		}
		//ELSE FALL THROUGH TO MEDIUM AND TRY AGAIN.
	case DB_MEDIUM:
		for (width = consize.X / 2; width < ((3 * consize.X)) / 4; width++)
		{
			//WRAP NEWCONTENTS, CHECK FOR HEIGHT, IF TOO HIGH, RAISE WIDTH AND TRY AGAIN.
			strcpy(newcontents, contents);
			WrapStringToWidth(newcontents, width);
			if (GetNumberOfStringLines(newcontents) < consize.Y - 5){
				ok = true;
				boxheight = GetNumberOfStringLines(newcontents)  + 5; //ACCOUNT FOR BORDERS, ETC.
				boxwidth = width;
				break; // NICE WORK! WE'RE ALL SET.
			}
		}
		if (ok){
			break;
		}
		//ELSE FALL THROUGH TO LARGE AND TRY AGAIN.
	case DB_LARGE:
		for (width = ((3 * consize.X) / 4); width < consize.X - 2; width++)
		{
			//WRAP NEWCONTENTS, CHECK FOR HEIGHT, IF TOO HIGH, RAISE WIDTH AND TRY AGAIN.
			strcpy(newcontents, contents);
			WrapStringToWidth(newcontents, width);
			if (GetNumberOfStringLines(newcontents) < consize.Y - 5){
				ok = true;
				boxheight = GetNumberOfStringLines(newcontents)  + 5; //ACCOUNT FOR BORDERS, ETC.
				boxwidth = width;
				break; // NICE WORK! WE'RE ALL SET.
			}
		}
		if (ok){
			break;
		}
		else{
			return - 1; //YOU SUCK!
		}
	}
	
	//FINALLY, SET THE ORIGINS.
	xorigin = getObjectCenterCoordinate(boxwidth, consize.X);
	yorigin = getObjectCenterCoordinate(boxheight, consize.Y);
	
	//CHECK AND CENTER THE TOP AND BOTTOM TEXTS.
	if (strlen(topstring) < boxwidth){
		topstringxorigin = getObjectCenterCoordinate(strlen(topstring), boxwidth) + xorigin;
	}
	else{
		topstringxorigin = xorigin + 1;
	}
	
	if (strlen(bottomstring) < boxwidth){
		bottomstringxorigin = getObjectCenterCoordinate(strlen(bottomstring), boxwidth) + xorigin;
	}
	else{
		bottomstringxorigin= xorigin + 1;
	}
	WriteToGlobalBackBuffer(xorigin, yorigin, boxwidth, boxheight);
	//WE DRAW ONCE AND HOLD FOR ENTER KEY.
	DrawStringDialogBoxShell(dbp, boxwidth, boxheight, xorigin, yorigin);
	//NOW DRAW ALL TEXT.
	setcursor(topstringxorigin, yorigin);
	setcolor(dbp->TopStringFGColor | dbp->TopStringBGColor);
	writestring(topstring);
	setcursor(bottomstringxorigin, yorigin + boxheight - 1);
	setcolor(dbp->BottomStringFGColor | dbp->BottomStringBGColor);
	writestring(bottomstring);
	setcursor(xorigin + 1, yorigin + 1);
	strcpy(newcontents, contents);
	WrapStringToWidth(newcontents, boxwidth - 3);
	setcolor(dbp->CenterStringFGColor | dbp->DialogBoxBGColor);
	WriteDialogString(newcontents, xorigin +1, yorigin + 1, boxwidth, dbp->CenteredText);
	
	//DRAW INITIAL CURSOR.
	setcolor(dbp->InputtedTextColor);
	setcursor(xorigin + 1, yorigin + boxheight - 2);
	writechar(cursorchar);
	
	//WAIT HERE.
	while(1)
	{
		key  = waitforkey();
		if (key == VK_BACK && numberofcharstyped > 0){
			setcursor(xorigin + 1 + numberofcharstyped, yorigin + boxheight - 2);
			writechar(' ');
			numberofcharstyped--;
			ourtempstring[numberofcharstyped] = '\0';
			setcursor(xorigin + 1 + numberofcharstyped, yorigin + boxheight - 2);
			setcolor(dbp->TextCursorColor);
			writechar(cursorchar);
		}
		else if (isdigit(key) && numberofcharstyped < numdigits){
			setcolor(dbp->InputtedTextColor);
			setcursor(xorigin + 1 + numberofcharstyped, yorigin + boxheight - 2);
			writechar(key);
			ourtempstring[numberofcharstyped] = key;
			ourtempstring[numberofcharstyped + 1] = '\0';
			numberofcharstyped++;
			setcolor(dbp->TextCursorColor);
			writechar(cursorchar);
		}
		else if (key == VK_RETURN && numberofcharstyped > 0){
			returnint = atoi(ourtempstring);
			delete [] newcontents;
			delete [] ourtempstring;
			FLUSH;
			WriteFromGlobalBackBuffer(xorigin, yorigin, boxwidth, boxheight);
			return returnint;
			
		}
		FLUSH;
	}
}




void WriteDialogString(char* string, int xorigin, int yorigin, int boxwidth, bool centered)
{
	int len = strlen(string);
	int counter = 0;
	int countertwo = 0;
	int localcounter = 0;
	int staticcounter = 0;
	int ycoord = yorigin;
	int xcoord = xorigin;
	char* stringpiece = new char[len + 1];
	//THIS IS FOR EACH PIECE OF THE STRING.
	
	
	setcursor(xorigin, yorigin);
	while(counter < len){
		staticcounter = counter;
		//GO THROUGH STRING UNTIL YOU FIND A \n.
		for(countertwo = staticcounter; countertwo < len; countertwo++){
			stringpiece[countertwo - staticcounter] = string[countertwo];
			counter++;
			if (string[countertwo] == '\n'){
				//COPY OVER, MOVE COUNTER PAST THE \N
				break;
			}
		}
		
		//WITH OUR LOCAL COPY, CHANGE \n TO A \0, PRINT OUT, MOVE DOWN A LINE.
		stringpiece[countertwo - staticcounter] = '\0';
		
		
		if (centered){
			if (strlen(stringpiece) < (unsigned int) boxwidth - 2){
				setcursor(getObjectCenterCoordinate(strlen(stringpiece), boxwidth - 2) + xorigin, ycoord);
			}
			else{
				setcursor(xcoord, ycoord);
			}
		}
		else{
			setcursor(xcoord, ycoord);
		}
		writestring(stringpiece);
		xcoord = xorigin;
		ycoord ++;
		if (counter == len){	
			return;
		}
	}
}

void DrawDialogBoxShell(PDBOXPARAM dbp, int boxwidth, int boxheight, int xorigin, int yorigin)
{
	int h;
	int w;
	char TRcornerchar;
	char TLcornerchar;
	char BRcornerchar;
	char BLcornerchar;
	char horizchar;
	char vertchar;
	
	switch(dbp->DialogBoxBorderType)
	{
	case DB_NOBORDER:
		TLcornerchar = TRcornerchar = BRcornerchar = BLcornerchar = ' ';
		horizchar = ' ';
		vertchar = ' ';
		break;
	case DB_SINGLEBORDER:
	default:
		TLcornerchar = (char)218;
		TRcornerchar = (char)191;
		BLcornerchar = (char)192;
		BRcornerchar = (char)217;
		horizchar =	(char)196;
		vertchar = (char)179;
		break;
	case DB_DOUBLEBORDER:
		TLcornerchar = (char)201;
		TRcornerchar = (char)187;
		BLcornerchar = (char)200;
		BRcornerchar = (char)188;
		horizchar = (char)205;
		vertchar = (char)186;
		break;
	case DB_BLOCKBORDER:
		TLcornerchar = TRcornerchar = BRcornerchar = BLcornerchar = horizchar = vertchar = (char)219;
		break;
	}
	
	
	//DRAW THE SHELL.
	if (dbp->DrawBGUnderBorder){
		setcolor(dbp->DialogBoxBGColor | dbp->DialogBoxBorderColor);
	}
	else{
		setcolor(dbp->DialogBoxBorderColor);
	}
	
	//DRAW LINES CONNECTING EACH CORNER.
	for(w = xorigin +1; w < boxwidth + xorigin; w++){
		setcursor(w, yorigin);
		writechar(horizchar);
	}
	
	for(w = xorigin +1; w < boxwidth + xorigin; w++){
		setcursor(w, yorigin + boxheight - 1);
		writechar(horizchar);
	}
	
	for(h = yorigin; h < yorigin + boxheight -1; h++)
	{
		setcursor(xorigin, h);
		writechar(vertchar);
		setcursor(xorigin + boxwidth - 1, h);
		writechar(vertchar);
	}
	
	//DRAW EACH CORNER.
	setcursor(xorigin, yorigin);
	writechar(TLcornerchar);
	setcursor(xorigin + boxwidth - 1, yorigin);
	writechar(TRcornerchar);
	setcursor(xorigin, yorigin + boxheight - 1);
	writechar(BLcornerchar);
	setcursor(xorigin + boxwidth - 1, yorigin + boxheight - 1);
	writechar(BRcornerchar);
	
	
	/*
	//BORDERS DONE, ITERATE BACKGROUND COLOR ON THE INSIDE.
	setcolor(dbp->DialogBoxBGColor | dbp->DialogBoxBorderColor);
	for (h = yorigin + 1; h < yorigin + boxheight - 1; h++)
	{
		for (w = xorigin + 1; w < xorigin + boxwidth - 1; w++){	
			setcursor(w, h);
			writechar(' ');
		}
	}
	*/

	setcolor(dbp->DialogBoxBGColor | dbp->DialogBoxBorderColor);
	RECT clearrect;
	clearrect.left = xorigin + 1;
	clearrect.top = yorigin + 1;
	clearrect.bottom = yorigin + boxheight -2;
	clearrect.right = xorigin + boxwidth - 1;
	ClearRect(clearrect, dbp->DialogBoxBGColor);
}

void DrawStringDialogBoxShell(PDBOXPARAM dbp, int boxwidth, int boxheight, int xorigin, int yorigin)
{
	int h;
	int w;
	char TRcornerchar;
	char TLcornerchar;
	char BRcornerchar;
	char BLcornerchar;
	char CRchar;
	char CLchar;
	char horizchar;
	char vertchar;
	
	switch(dbp->DialogBoxBorderType)
	{
	case DB_NOBORDER:
		TLcornerchar = TRcornerchar = BRcornerchar = BLcornerchar = ' ';
		horizchar = ' ';
		vertchar = ' ';
		break;
	case DB_SINGLEBORDER:
	default:
		CLchar	= (char)195;
		CRchar = (char)180;
		TLcornerchar = (char)218;
		TRcornerchar = (char)191;
		BLcornerchar = (char)192;
		BRcornerchar = (char)217;
		horizchar =	(char)196;
		vertchar = (char)179;
		break;
	case DB_DOUBLEBORDER:
		CLchar	= (char)204;
		CRchar = (char)185;
		TLcornerchar = (char)201;
		TRcornerchar = (char)187;
		BLcornerchar = (char)200;
		BRcornerchar = (char)188;
		horizchar = (char)205;
		vertchar = (char)186;
		break;
	case DB_BLOCKBORDER:
		TLcornerchar = TRcornerchar = BRcornerchar = BLcornerchar = horizchar = vertchar = CLchar = CRchar = (char)219;
		break;
	}
	
	
	//DRAW THE SHELL.
	if (dbp->DrawBGUnderBorder){
		setcolor(dbp->DialogBoxBGColor | dbp->DialogBoxBorderColor);
	}
	else{
		setcolor(dbp->DialogBoxBorderColor);
	}
	
	//DRAW LINES CONNECTING EACH CORNER.
	for(w = xorigin +1; w < boxwidth + xorigin; w++){
		setcursor(w, yorigin);
		writechar(horizchar);
	}
	
	for(w = xorigin +1; w < boxwidth + xorigin; w++){
		setcursor(w, yorigin + boxheight - 1);
		writechar(horizchar);
	}
	
	for(h = yorigin; h < yorigin + boxheight -1; h++)
	{
		setcursor(xorigin, h);
		writechar(vertchar);
		setcursor(xorigin + boxwidth - 1, h);
		writechar(vertchar);
	}
	
	//DRAW EACH CORNER.
	setcursor(xorigin, yorigin);
	writechar(TLcornerchar);
	setcursor(xorigin + boxwidth - 1, yorigin);
	writechar(TRcornerchar);
	setcursor(xorigin, yorigin + boxheight - 1);
	writechar(BLcornerchar);
	setcursor(xorigin + boxwidth - 1, yorigin + boxheight - 1);
	writechar(BRcornerchar);
	
	
	
	//BORDERS DONE, ITERATE BACKGROUND COLOR ON THE INSIDE.
	setcolor(dbp->DialogBoxBGColor | dbp->DialogBoxBorderColor);
	RECT clearrect;
	clearrect.left = xorigin + 1;
	clearrect.top = yorigin  + 1;
	clearrect.bottom = yorigin + boxheight -3;
	clearrect.right = xorigin + boxwidth - 1;
	ClearRect(clearrect, dbp->DialogBoxBGColor);
//	for (h = yorigin + 1; h < yorigin + boxheight - 3; h++)
	//{
		//for (w = xorigin + 1; w < xorigin + boxwidth - 1; w++){	
		//	setcursor(w, h);
		//	writechar(' ');
		//}
//	}
	
	
	//EXTRA BORDERS, SPECIAL CHARACTERS. THREE UP FROM BOXHEIGHT.
	for(w = xorigin + 1; w < xorigin + boxwidth - 1; w++){
		setcursor(w, yorigin + boxheight - 3);
		writechar(horizchar);
	}
	
	
	setcursor(xorigin, yorigin + boxheight -3);
	writechar(CLchar);
	setcursor(xorigin + boxwidth - 1, yorigin + boxheight - 3);
	writechar(CRchar);

	setcolor(0);
	for(w = xorigin + 1; w < xorigin + boxwidth - 1; w++){
		setcursor(w, yorigin + boxheight - 2);
		writechar((char)219);
	}
}

void SetGlobalDBoxParam(int type, int size, int maxresponsechars, bool centered)
{
	globals.dboxparams.RelativeSize = size;
	globals.dboxparams.CenteredText = centered;
	globals.dboxparams.MaxResponseChars = maxresponsechars;
	
	switch(type)
	{
	case DB_SCHEME_EMERGENCYREDLINES:
		globals.dboxparams.DialogBoxBorderColor = FOREGROUND_RED;
		globals.dboxparams.DialogBoxBorderType = DB_SINGLEBORDER;
		globals.dboxparams.DrawBGUnderBorder = FALSE;
		globals.dboxparams.DialogBoxBGColor = 0;
		
		globals.dboxparams.BottomStringFGColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
		globals.dboxparams.BottomStringBGColor = 0;
		
		globals.dboxparams.TopStringFGColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
		globals.dboxparams.TopStringBGColor = 0;
		
		globals.dboxparams.CenterStringFGColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
		
		globals.dboxparams.InputtedTextColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
		globals.dboxparams.SelectionBlockColor = BACKGROUND_RED;
		globals.dboxparams.TextCursorColor = FOREGROUND_RED;
		globals.dboxparams.SelectedObjectTextColor = 0;
		globals.dboxparams.UnselectedObjectTextColor = FOREGROUND_RED;
		
		break;
		
	case DB_SCHEME_BLUEANDYELLOW:
		globals.dboxparams.DialogBoxBGColor = BACKGROUND_BLUE;
		globals.dboxparams.DialogBoxBorderColor = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
		globals.dboxparams.DialogBoxBorderType = DB_SINGLEBORDER;
		globals.dboxparams.DrawBGUnderBorder = TRUE;
		
		globals.dboxparams.BottomStringBGColor = BACKGROUND_BLUE;
		globals.dboxparams.BottomStringFGColor = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
		
		globals.dboxparams.TopStringBGColor = BACKGROUND_BLUE;
		globals.dboxparams.TopStringFGColor = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
		
		globals.dboxparams.CenterStringFGColor = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
		
		globals.dboxparams.InputtedTextColor = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
		globals.dboxparams.SelectionBlockColor = BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
		globals.dboxparams.TextCursorColor =  FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
		globals.dboxparams.SelectedObjectTextColor = 0;
		globals.dboxparams.UnselectedObjectTextColor =  FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;
		
	case DB_SCHEME_DIGITALGREENLINES:
		globals.dboxparams.DialogBoxBorderColor = FOREGROUND_GREEN;
		globals.dboxparams.DialogBoxBorderType = DB_SINGLEBORDER;
		globals.dboxparams.DrawBGUnderBorder = FALSE;
		globals.dboxparams.DialogBoxBGColor = 0;
		
		globals.dboxparams.BottomStringFGColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		globals.dboxparams.BottomStringBGColor = 0;
		
		globals.dboxparams.TopStringFGColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		globals.dboxparams.TopStringBGColor = 0;
		
		globals.dboxparams.CenterStringFGColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		
		globals.dboxparams.InputtedTextColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		globals.dboxparams.SelectionBlockColor = BACKGROUND_GREEN;
		globals.dboxparams.TextCursorColor = FOREGROUND_GREEN;
		globals.dboxparams.SelectedObjectTextColor = 0;
		globals.dboxparams.UnselectedObjectTextColor = FOREGROUND_GREEN;
		break;
	case DB_SCHEME_REDANDYELLOW:
		globals.dboxparams.DialogBoxBGColor = BACKGROUND_RED;
		globals.dboxparams.DialogBoxBorderColor = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
		globals.dboxparams.DialogBoxBorderType = DB_SINGLEBORDER;
		globals.dboxparams.DrawBGUnderBorder = TRUE;
		
		globals.dboxparams.BottomStringBGColor = BACKGROUND_RED;
		globals.dboxparams.BottomStringFGColor = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
		
		globals.dboxparams.TopStringBGColor = BACKGROUND_RED;
		globals.dboxparams.TopStringFGColor = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
		
		globals.dboxparams.CenterStringFGColor = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
		
		globals.dboxparams.InputtedTextColor = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
		globals.dboxparams.SelectionBlockColor = BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
		globals.dboxparams.TextCursorColor =  FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
		globals.dboxparams.SelectedObjectTextColor = 0;
		globals.dboxparams.UnselectedObjectTextColor =  FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;
	case DB_SCHEME_COOLBLUELINES:
		globals.dboxparams.DialogBoxBorderColor = FOREGROUND_BLUE;
		globals.dboxparams.DialogBoxBorderType = DB_SINGLEBORDER;
		globals.dboxparams.DrawBGUnderBorder = FALSE;
		globals.dboxparams.DialogBoxBGColor = 0;
		
		globals.dboxparams.BottomStringFGColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		globals.dboxparams.BottomStringBGColor = 0;
		
		globals.dboxparams.TopStringFGColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		globals.dboxparams.TopStringBGColor = 0;
		
		globals.dboxparams.CenterStringFGColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		
		globals.dboxparams.InputtedTextColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		globals.dboxparams.SelectionBlockColor = BACKGROUND_BLUE;
		globals.dboxparams.TextCursorColor = FOREGROUND_BLUE;
		globals.dboxparams.SelectedObjectTextColor = 0;
		globals.dboxparams.UnselectedObjectTextColor = FOREGROUND_BLUE;
		break;
	case DB_SCHEME_MONOCHROMATIC:
		globals.dboxparams.DialogBoxBorderColor = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN;
		globals.dboxparams.DialogBoxBorderType = DB_SINGLEBORDER;
		globals.dboxparams.DrawBGUnderBorder = FALSE;
		globals.dboxparams.DialogBoxBGColor = 0;
		
		globals.dboxparams.BottomStringFGColor = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		globals.dboxparams.BottomStringBGColor = 0;
		
		globals.dboxparams.TopStringFGColor = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		globals.dboxparams.TopStringBGColor = 0;
		
		globals.dboxparams.CenterStringFGColor = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		
		globals.dboxparams.InputtedTextColor = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		globals.dboxparams.SelectionBlockColor = BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN;
		globals.dboxparams.TextCursorColor = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN;
		globals.dboxparams.SelectedObjectTextColor = 0;
		globals.dboxparams.UnselectedObjectTextColor = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN;
		break;
	default:
		break;
	}
}
int SimpleListDialogBox (char* topstring, char* bottomstring, PLIST objects, PDBOXPARAM dbp, DBLISTITEM(ItemCreate)(PPOTYPE object))
{
	POTYPE holder;
	PLIST listitems = createList();
	unsigned int selected = 0;
	int identifier = 0;
	unsigned int numberitems;
	unsigned int numberitemstodisplayperscreen;
	unsigned int scrolloffset = 0;
	unsigned int localoffset = 0;
	bool scrollneeded = FALSE;
	unsigned int topstringxorigin;
	unsigned int bottomstringxorigin;
	unsigned int boxwidth;
	unsigned int boxheight;
	unsigned int xorigin;
	unsigned int yorigin;
//	char buf[255];
	int key;
//	int width;
	bool ok = false;
	bool drawscrollneeded = false;
	bool draw = false;
	int size = dbp->RelativeSize;
	int minwidth;
	char uparrowchar = (char)24;
	char downarrowchar = (char)25;
	COORD consize;
	
	consize = getconsize();
	for(objects->current = objects->head; objects->current != NULL; objects->current = objects->current->nextnode){
		holder.dblistitem = ItemCreate(&objects->current->object);
		holder.dblistitem.id = identifier;
		identifier++;
		addtoList(listitems, holder, CompareDbitemID);
	}
	//LIST READY TO GO!
	numberitems = listitems->objcnt;
	minwidth = GetLongestDBItemLength(listitems) + 2;
	
	
	switch(size){
	case DB_SMALL:
		//SET INITIAL WIDTH TO BE DB_SMALL DEFAULT.
		if (minwidth < consize.X / 2){
			
			boxwidth = minwidth + 1;
			boxheight = consize.Y / 3;
			if (numberitems > boxheight - 4){
				scrollneeded = TRUE;
			}
			if (scrollneeded){
				numberitemstodisplayperscreen = boxheight - 4;
			}
			else{
			numberitemstodisplayperscreen = numberitems;
		}
			ok = TRUE;
			break;
		}
		if (ok){
			break;
		}
		//ELSE FALL THROUGH TO MEDIUM AND TRY AGAIN.
	case DB_MEDIUM:
		if (minwidth < ((3 * consize.X) / 4)){
			if (minwidth < consize.X / 3){
				boxwidth = consize.X / 3;
			}
			else{
				boxwidth = minwidth + 1;
			}
			boxheight = consize.Y / 2;
			if (numberitems > boxheight - 4){
				scrollneeded = TRUE;
			}
			if (scrollneeded){
				numberitemstodisplayperscreen = boxheight - 4;
			}
			else{
			numberitemstodisplayperscreen = numberitems;
		}
			ok = TRUE;
			break;
		}
		if (ok){
			break;
		}
		//ELSE FALL THROUGH TO LARGE AND TRY AGAIN.
	case DB_LARGE:
		if (minwidth < consize.X / 2){
			boxwidth = consize.X / 2;
		}
		else{
			boxwidth = minwidth + 1;
		}
		boxheight = consize.Y - 2;
		if (numberitems > boxheight - 4){
			scrollneeded = TRUE;
		}
		if (scrollneeded){
			numberitemstodisplayperscreen = boxheight - 4;
		}
		else{
			numberitemstodisplayperscreen = numberitems;
		}
		ok = TRUE;
		break;
		if (ok){
			break;
		}
		else{
			return - 1; //YOU SUCK!
		}
	}
	
	//FINALLY, SET THE ORIGINS.
	xorigin = getObjectCenterCoordinate(boxwidth, consize.X);
	yorigin = getObjectCenterCoordinate(boxheight, consize.Y);
	
	//CHECK AND CENTER THE TOP AND BOTTOM TEXTS.
	if (strlen(topstring) < boxwidth){
		topstringxorigin = getObjectCenterCoordinate(strlen(topstring), boxwidth) + xorigin;
	}
	else{
		topstringxorigin = xorigin + 1;
	}
	
	if (strlen(bottomstring) < boxwidth){
		bottomstringxorigin = getObjectCenterCoordinate(strlen(bottomstring), boxwidth) + xorigin;
	}
	else{
		bottomstringxorigin= xorigin + 1;
	}
	WriteToGlobalBackBuffer(xorigin, yorigin, boxwidth, boxheight);
	DrawDialogBoxShell(dbp, boxwidth, boxheight, xorigin, yorigin);
	//NOW WE MUST FILL IN THE REST.
	//TOP STRING/BOTTOM STRING
	setcursor(topstringxorigin, yorigin);
	setcolor(dbp->TopStringFGColor | dbp->TopStringBGColor);
	writestring(topstring);
	setcursor(bottomstringxorigin, yorigin + boxheight - 1);
	setcolor(dbp->BottomStringFGColor | dbp->BottomStringBGColor);
	writestring(bottomstring);
	//WE MUST FILL IN ARROWS, CONTENTS, AND SELECTED BOXES.
	if (scrollneeded){
		setcolor(dbp->UnselectedObjectTextColor | dbp->DialogBoxBGColor);
		setcursor(xorigin + 1, yorigin + boxheight - 2);
		writechar(downarrowchar);
	}
	//NOW PRINT ALL CONTENTS.
	for(unsigned int i = 0; i < numberitemstodisplayperscreen; i++){
		setcursor(xorigin + 2, yorigin + 2 + i);
		setcolor(dbp->UnselectedObjectTextColor | dbp->DialogBoxBGColor);
		writestring(ReturnDBListItemByNumber(i, listitems)->string);
	}
	setcursor(xorigin + 2, yorigin + 2);
	setcolor(dbp->SelectedObjectTextColor | dbp->SelectionBlockColor);
	writestring(ReturnDBListItemByNumber(0, listitems)->string);
	
	//ALL SET. BEGIN LOOP.
	while(1)
	{
		key  = waitforVkey();
		if (key == VK_UP && scrolloffset + localoffset > 0){
			selected--;
			if(selected < 0){
				selected = 0;
			}
			if (!localoffset && scrolloffset > 0){
				scrolloffset--;
				drawscrollneeded = true;
			}
			else{
				localoffset--;
				if(localoffset < 0){
					localoffset = 0;
				}
			}
			draw = true;
		}
		else if (key == VK_DOWN && scrolloffset + localoffset < numberitems - 1){
			//DOWN
			selected++;
			if (selected > numberitems - 1){
				selected = numberitems - 1;
			}
			if(localoffset == numberitemstodisplayperscreen - 1 && scrolloffset + localoffset < numberitems){
				scrolloffset++;
				drawscrollneeded = true;
			}
			else{
				localoffset++;
				if (localoffset > numberitemstodisplayperscreen){
					localoffset = numberitemstodisplayperscreen;
				}
			}
			draw = true;
		}
		else if (key == 34) //PAGE DOWN
		{
			//ONE PAGE DOWN. WILL ATTEMPT TO MOVE THE SCROLL OFFSET BY THE MAX ITEMS IN THE BOX. IF IT CANNOT, THEN IT WILL MOVE
			//THE SCROLL OFFSET AS FAR AS IT CAN, AND WILL DROP THE LOCALOFFSET TO THE REMAINDER OF THE DISTANCE.
			if(scrolloffset + numberitemstodisplayperscreen >= numberitems - numberitemstodisplayperscreen){
				scrolloffset = selected = numberitems - numberitemstodisplayperscreen;
				localoffset = numberitemstodisplayperscreen - 1;
				selected += numberitemstodisplayperscreen - 1;
			}
			else{
				scrolloffset += numberitemstodisplayperscreen;
				selected += numberitemstodisplayperscreen;
			}
			draw = true;
			drawscrollneeded = true;
		}
		else if (key == 33)
		{
			//ONE PAGE UP. WILL ATTEMPT TO MOVE THE SCROLL OFFSET BY THE MAX ITEMS IN THE BOX. IF IT CANNOT, THEN IT WILL MOVE
			//THE SCROLL OFFSET AS FAR AS IT CAN, AND WILL RAISE THE LOCALOFFSET TO THE REMAINDER OF THE DISTANCE.
			if(scrolloffset - numberitemstodisplayperscreen > 0){
				scrolloffset = 0;
				localoffset = 0;
				selected = 0;
			}
			else{
				scrolloffset -= numberitemstodisplayperscreen;
				selected -= numberitemstodisplayperscreen;
			}
			draw = true;
			drawscrollneeded = true;
		}
		else if (key == VK_RETURN){
			FreeDBList(listitems);
			WriteFromGlobalBackBuffer(xorigin, yorigin, boxwidth, boxheight);
			return selected;
		}
		FLUSH;
		if (draw){
			//REDRAW HERE.
			//CLEAR INNER RECTANGLE.
			if(drawscrollneeded){
			RECT rect;
			rect.left = xorigin + 1;
			rect.right = rect.left + boxwidth - 2;
			rect.top = yorigin +  1;
			rect.bottom = rect.top + boxheight -3;
			ClearRect(rect, dbp->DialogBoxBGColor);
			//REDRAW NECESSARY ARROWS FIRST,
			if (scrollneeded){
				if(scrolloffset  < numberitems - numberitemstodisplayperscreen){
					setcolor(dbp->UnselectedObjectTextColor | dbp->DialogBoxBGColor);
					setcursor(xorigin + 1, yorigin + boxheight - 2);
					writechar(downarrowchar);
				}
				if (scrolloffset > 0){
					setcolor(dbp->UnselectedObjectTextColor | dbp->DialogBoxBGColor);
					setcursor(xorigin + 1, yorigin + 1);
					writechar(uparrowchar);
				}
			}
			}

			if(drawscrollneeded){
			//GRAB STRINGS FROM SCROLLOFFSET TO SCROLLOFFSET + MAXITEMSTODISPLAY
			for(unsigned int i = 0; i < numberitemstodisplayperscreen; i++){
				setcursor(xorigin + 2, yorigin + 2 + i);
				setcolor(dbp->UnselectedObjectTextColor | dbp->DialogBoxBGColor);
				writestring(ReturnDBListItemByNumber(i+scrolloffset, listitems)->string);
				drawscrollneeded = false;
			}
			}
			else{
				if(key == VK_UP){
					//DRAW THE ONE BELOW AS REGULAR.
					setcursor(xorigin + 2, yorigin + 2 + localoffset + 1);
			setcolor(dbp->UnselectedObjectTextColor | dbp->DialogBoxBGColor);
			writestring(ReturnDBListItemByNumber(localoffset + 1 + scrolloffset, listitems)->string);
				}
				else if (key == VK_DOWN)
				{
					//DRAW THE ONE ABOVE AS REGULAR
					setcursor(xorigin + 2, yorigin + 2 + localoffset - 1);
			setcolor(dbp->UnselectedObjectTextColor | dbp->DialogBoxBGColor);
			writestring(ReturnDBListItemByNumber(localoffset - 1 + scrolloffset, listitems)->string);
				}
			}
			//DRAW SELECTED BOX AROUND YORIGIN + 1 + LOCALOFFSET
			setcursor(xorigin + 2, yorigin + 2 + localoffset);
			setcolor(dbp->SelectedObjectTextColor | dbp->SelectionBlockColor);
			writestring(ReturnDBListItemByNumber(localoffset + scrolloffset, listitems)->string);
			
		//		sprintf(buf, "Sel:%2.2d Scr:%2.2d Loc:%2.2d", selected, scrolloffset, localoffset);
	//			setcursor(0,0);
		//		writestring(buf);
			
			draw = false;
		}
	}
	
	
}


LISTCONTROLOUTPUT ListDialogBoxWithControls (char* topstring, char* bottomstring, char* helptext,PLIST objects,  PDBOXPARAM dbp, DBLISTITEM(ItemCreate)(PPOTYPE object), int selectionnum)
{
	POTYPE holder;
	PLIST listitems = createList();
	unsigned int selected = 0;
	int identifier = 0;
	unsigned int numberitems;
	unsigned int numberitemstodisplayperscreen;
	unsigned int scrolloffset = 0;
	unsigned int localoffset = 0;
	bool scrollneeded = FALSE;
	unsigned int topstringxorigin;
	unsigned int bottomstringxorigin;
	unsigned int boxwidth;
	unsigned int boxheight;
	unsigned int xorigin;
	unsigned int yorigin;
//	char buf[255];
	char* centertext = (char*)malloc(sizeof(char) * (strlen(helptext) + 1));
	int key;
//	int width;
	bool ok = false;
	bool draw = false;
	bool drawwholebox = false;
	int size = dbp->RelativeSize;
	int minwidth;
	char uparrowchar = (char)24;
	char downarrowchar = (char)25;
	LISTCONTROLOUTPUT lp;
	COORD consize;
	
	consize = getconsize();
	for(objects->current = objects->head; objects->current != NULL; objects->current = objects->current->nextnode){
		holder.dblistitem = ItemCreate(&objects->current->object);
		holder.dblistitem.id = identifier;
		identifier++;
		addtoList(listitems, holder, CompareDbitemID);
	}
	//LIST READY TO GO!
	strcpy(centertext, helptext);
	numberitems = listitems->objcnt;
	minwidth = GetLongestDBItemLength(listitems) + 2;
	
	
	switch(size){
	case DB_SMALL:
		//SET INITIAL WIDTH TO BE DB_SMALL DEFAULT.
		if (minwidth < consize.X / 2){
			
			boxwidth = minwidth + 1;
			boxheight = consize.Y / 3;
			if (numberitems > boxheight - 8){
				scrollneeded = TRUE;
			}
			if (scrollneeded){
				numberitemstodisplayperscreen = boxheight - 8;
			}
			else{
			numberitemstodisplayperscreen = numberitems;
		}
			ok = TRUE;
			break;
		}
		if (ok){
			break;
		}
		//ELSE FALL THROUGH TO MEDIUM AND TRY AGAIN.
	case DB_MEDIUM:
		if (minwidth < ((3 * consize.X) / 4)){
			if (minwidth < consize.X / 3){
				boxwidth = consize.X / 3;
			}
			else{
				boxwidth = minwidth + 1;
			}
			boxheight = consize.Y / 2;
			if (numberitems > boxheight - 8){
				scrollneeded = TRUE;
			}
			if (scrollneeded){
				numberitemstodisplayperscreen = boxheight - 8;
			}
			else{
			numberitemstodisplayperscreen = numberitems;
		}
			ok = TRUE;
			break;
		}
		if (ok){
			break;
		}
		//ELSE FALL THROUGH TO LARGE AND TRY AGAIN.
	case DB_LARGE:
		if (minwidth < consize.X / 2){
			boxwidth = consize.X / 2;
		}
		else{
			boxwidth = minwidth + 1;
		}
		boxheight = consize.Y - 2;
		if (numberitems > boxheight - 8){
			scrollneeded = TRUE;
		}
		if (scrollneeded){
			numberitemstodisplayperscreen = boxheight - 8;
		}
		else{
			numberitemstodisplayperscreen = numberitems;
		}
		ok = TRUE;
		break;
		if (ok){
			break;
		}
		else{
			free(centertext);
			LISTCONTROLOUTPUT elp;
			elp.id = -1;
			elp.key = 0;
			return elp; //YOU SUCK!
		}
	}
	
	//FINALLY, SET THE ORIGINS.
	xorigin = getObjectCenterCoordinate(boxwidth, consize.X);
	yorigin = getObjectCenterCoordinate(boxheight, consize.Y);
	
	//CHECK AND CENTER THE TOP AND BOTTOM TEXTS.
	if (strlen(topstring) < boxwidth){
		topstringxorigin = getObjectCenterCoordinate(strlen(topstring), boxwidth) + xorigin;
	}
	else{
		topstringxorigin = xorigin + 1;
	}
	
	if (strlen(bottomstring) < boxwidth){
		bottomstringxorigin = getObjectCenterCoordinate(strlen(bottomstring), boxwidth) + xorigin;
	}
	else{
		bottomstringxorigin= xorigin + 1;
	}
	WriteToGlobalBackBuffer(xorigin, yorigin, boxwidth, boxheight);
	DrawListBoxControlDialogBoxShell(dbp, boxwidth, boxheight, xorigin, yorigin);
	//NOW WE MUST FILL IN THE REST.
	//TOP STRING/BOTTOM STRING
	setcursor(topstringxorigin, yorigin);
	setcolor(dbp->TopStringFGColor | dbp->TopStringBGColor);
	writestring(topstring);
	setcursor(bottomstringxorigin, yorigin + boxheight - 1);
	setcolor(dbp->BottomStringFGColor | dbp->BottomStringBGColor);
	writestring(bottomstring);
	//WE MUST FILL IN ARROWS, CONTENTS, AND SELECTED BOXES.
	if (scrollneeded){
		setcolor(dbp->UnselectedObjectTextColor | dbp->DialogBoxBGColor);
		setcursor(xorigin + 1, yorigin + boxheight - 2);
		writechar(downarrowchar);
	}

	//SET SELECTED NUMBER
	int selnum = selectionnum;
	if ((unsigned int)selectionnum > numberitems){
		selnum = numberitems;
	}

	for(int m = 0; m < selnum; m++){
			selected++;
			if (selected > numberitems - 1){
				selected = numberitems - 1;
			}
			if(localoffset == numberitemstodisplayperscreen - 1 && scrolloffset + localoffset < numberitems){
				scrolloffset++;
			}
			else{
				localoffset++;
				if (localoffset > numberitemstodisplayperscreen){
					localoffset = numberitemstodisplayperscreen;
				}
			}
	}

	//NOW PRINT ALL CONTENTS.
	for(unsigned int i = 0; i < numberitemstodisplayperscreen; i++){
		setcursor(xorigin + 2, yorigin + 2 + i);
		setcolor(dbp->UnselectedObjectTextColor | dbp->DialogBoxBGColor);
		writestring(ReturnDBListItemByNumber(i, listitems)->string);
	}
	setcursor(xorigin + 2, yorigin + 2);
	setcolor(dbp->SelectedObjectTextColor | dbp->SelectionBlockColor);
	writestring(ReturnDBListItemByNumber(0, listitems)->string);

	WrapStringToWidth(centertext, boxwidth - 2);
	WriteDialogString(centertext, xorigin + 1, yorigin + boxheight - 4, boxwidth - 2, true);
	
	//ALL SET. BEGIN LOOP.
	while(1)
	{
		key  = waitforVkey();
		lp.id = selected;
		lp.key = key;
		if (key == VK_UP && scrolloffset + localoffset > 0){
			selected--;
			if(selected < 0){
				selected = 0;
			}
			if (!localoffset && scrolloffset > 0){
				scrolloffset--;
			}
			else{
				localoffset--;
				if(localoffset < 0){
					localoffset = 0;
				}
			}
			draw = true;
		}
		else if (key == VK_DOWN && scrolloffset + localoffset < numberitems - 1){
			//DOWN
			selected++;
			if (selected > numberitems - 1){
				selected = numberitems - 1;
			}
			if(localoffset == numberitemstodisplayperscreen - 1 && scrolloffset + localoffset < numberitems){
				scrolloffset++;
			}
			else{
				localoffset++;
				if (localoffset > numberitemstodisplayperscreen){
					localoffset = numberitemstodisplayperscreen;
				}
			}
			draw = true;
		}
		else {
			FreeDBList(listitems);
			WriteFromGlobalBackBuffer(xorigin, yorigin, boxwidth, boxheight);
			free(centertext);
			return lp;
		}
		drawwholebox = true;
		draw = true;
		FLUSH;
		if (drawwholebox)
		{
			//WE HAVE TO RECREATE THE LIST.
			DrawListBoxControlDialogBoxShell(dbp, boxwidth, boxheight, xorigin, yorigin);
	//NOW WE MUST FILL IN THE REST.
	//TOP STRING/BOTTOM STRING
	setcursor(topstringxorigin, yorigin);
	setcolor(dbp->TopStringFGColor | dbp->TopStringBGColor);
	writestring(topstring);
	setcursor(bottomstringxorigin, yorigin + boxheight - 1);
	setcolor(dbp->BottomStringFGColor | dbp->BottomStringBGColor);
	writestring(bottomstring);
	//WE MUST FILL IN ARROWS, CONTENTS, AND SELECTED BOXES.
	if (scrollneeded){
		setcolor(dbp->UnselectedObjectTextColor | dbp->DialogBoxBGColor);
		setcursor(xorigin + 1, yorigin + boxheight - 2);
		writechar(downarrowchar);
	}

	//NOW PRINT ALL CONTENTS.
	for(unsigned int i = 0; i < numberitemstodisplayperscreen; i++){
		setcursor(xorigin + 2, yorigin + 2 + i);
		setcolor(dbp->UnselectedObjectTextColor | dbp->DialogBoxBGColor);
		writestring(ReturnDBListItemByNumber(i, listitems)->string);
	}
	setcursor(xorigin + 2, yorigin + 2);
	setcolor(dbp->SelectedObjectTextColor | dbp->SelectionBlockColor);
	writestring(ReturnDBListItemByNumber(0, listitems)->string);

	WrapStringToWidth(centertext, boxwidth - 2);
	WriteDialogString(centertext, xorigin + 1, yorigin + boxheight - 4, boxwidth - 2, true);
		}
		if (draw){
			//REDRAW HERE.
			//CLEAR INNER RECTANGLE.
			RECT rect;
			rect.left = xorigin + 1;
			rect.right = rect.left + boxwidth - 2;
			rect.top = yorigin +  1;
			rect.bottom = rect.top + boxheight -3;
			ClearRect(rect, dbp->DialogBoxBGColor);
			//REDRAW NECESSARY ARROWS FIRST,
			if (scrollneeded){
				if(scrolloffset  < numberitems - numberitemstodisplayperscreen){
					setcolor(dbp->UnselectedObjectTextColor | dbp->DialogBoxBGColor);
					setcursor(xorigin + 1, yorigin + boxheight - 2);
					writechar(downarrowchar);
				}
				if (scrolloffset > 0){
					setcolor(dbp->UnselectedObjectTextColor | dbp->DialogBoxBGColor);
					setcursor(xorigin + 1, yorigin + 1);
					writechar(uparrowchar);
				}
			}
			//GRAB STRINGS FROM SCROLLOFFSET TO SCROLLOFFSET + MAXITEMSTODISPLAY
			for(unsigned int i = 0; i < numberitemstodisplayperscreen; i++){
				setcursor(xorigin + 2, yorigin + 2 + i);
				setcolor(dbp->UnselectedObjectTextColor | dbp->DialogBoxBGColor);
				writestring(ReturnDBListItemByNumber(i+scrolloffset, listitems)->string);
			}
			//DRAW SELECTED BOX AROUND YORIGIN + 1 + LOCALOFFSET
			setcursor(xorigin + 2, yorigin + 2 + localoffset);
			setcolor(dbp->SelectedObjectTextColor | dbp->SelectionBlockColor);
			writestring(ReturnDBListItemByNumber(localoffset + scrolloffset, listitems)->string);
			
		//		sprintf(buf, "Sel:%2.2d Scr:%2.2d Loc:%2.2d", selected, scrolloffset, localoffset);
	//			setcursor(0,0);
		//		writestring(buf);
			
			draw = false;
		}
	}	
}

void DrawListBoxControlDialogBoxShell(PDBOXPARAM dbp, int boxwidth, int boxheight, int xorigin, int yorigin)
{
	int h;
	int w;
	char TRcornerchar;
	char TLcornerchar;
	char BRcornerchar;
	char BLcornerchar;
	char CRchar;
	char CLchar;
	char horizchar;
	char vertchar;
	
	switch(dbp->DialogBoxBorderType)
	{
	case DB_NOBORDER:
		TLcornerchar = TRcornerchar = BRcornerchar = BLcornerchar = ' ';
		horizchar = ' ';
		vertchar = ' ';
		break;
	case DB_SINGLEBORDER:
	default:
		CLchar	= (char)195;
		CRchar = (char)180;
		TLcornerchar = (char)218;
		TRcornerchar = (char)191;
		BLcornerchar = (char)192;
		BRcornerchar = (char)217;
		horizchar =	(char)196;
		vertchar = (char)179;
		break;
	case DB_DOUBLEBORDER:
		CLchar	= (char)204;
		CRchar = (char)185;
		TLcornerchar = (char)201;
		TRcornerchar = (char)187;
		BLcornerchar = (char)200;
		BRcornerchar = (char)188;
		horizchar = (char)205;
		vertchar = (char)186;
		break;
	case DB_BLOCKBORDER:
		TLcornerchar = TRcornerchar = BRcornerchar = BLcornerchar = horizchar = vertchar = CLchar = CRchar = (char)219;
		break;
	}
	
	
	//DRAW THE SHELL.
	if (dbp->DrawBGUnderBorder){
		setcolor(dbp->DialogBoxBGColor | dbp->DialogBoxBorderColor);
	}
	else{
		setcolor(dbp->DialogBoxBorderColor);
	}
	
	//DRAW LINES CONNECTING EACH CORNER.
	for(w = xorigin +1; w < boxwidth + xorigin; w++){
		setcursor(w, yorigin);
		writechar(horizchar);
	}
	
	for(w = xorigin +1; w < boxwidth + xorigin; w++){
		setcursor(w, yorigin + boxheight - 1);
		writechar(horizchar);
	}
	
	for(h = yorigin; h < yorigin + boxheight -1; h++)
	{
		setcursor(xorigin, h);
		writechar(vertchar);
		setcursor(xorigin + boxwidth - 1, h);
		writechar(vertchar);
	}
	
	//DRAW EACH CORNER.
	setcursor(xorigin, yorigin);
	writechar(TLcornerchar);
	setcursor(xorigin + boxwidth - 1, yorigin);
	writechar(TRcornerchar);
	setcursor(xorigin, yorigin + boxheight - 1);
	writechar(BLcornerchar);
	setcursor(xorigin + boxwidth - 1, yorigin + boxheight - 1);
	writechar(BRcornerchar);
	
	
	
	//BORDERS DONE, ITERATE BACKGROUND COLOR ON THE INSIDE.
	setcolor(dbp->DialogBoxBGColor | dbp->DialogBoxBorderColor);
	for (h = yorigin + 1; h < yorigin + boxheight - 1; h++)
	{
		for (w = xorigin + 1; w < xorigin + boxwidth - 1; w++){	
			setcursor(w, h);
			writechar(' ');
		}
	}
	
	
	//EXTRA BORDERS, SPECIAL CHARACTERS. FIVE UP FROM BOXHEIGHT.
	for(w = xorigin + 1; w < xorigin + boxwidth - 1; w++){
		setcursor(w, yorigin + boxheight - 5);
		writechar(horizchar);
	}
	
	
	setcursor(xorigin, yorigin + boxheight -5);
	writechar(CLchar);
	setcursor(xorigin + boxwidth - 1, yorigin + boxheight - 5);
	writechar(CRchar);

	setcolor(0);
	for(w = xorigin + 1; w < xorigin + boxwidth - 1; w++){
		setcursor(w, yorigin + boxheight - 2);
		writechar((char)219);
	}
}

bool CreateDBStringList(PLIST* list, char** items, int numitems)
{
	*list = createList();
	POTYPE types;

	for(int i = 0; i < numitems; i++){
		types.string = (char*) malloc(sizeof(char) * (strlen(items[i]) + 1));
		strcpy(types.string, items[i]);
		addtoList(*list, types, StringCompare);
	}
	return true;
}

void FreeDBStringList(PLIST list)
{
	PLIST walker = list;
	
	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		free(walker->current->object.string);
	}
	free(list);
}