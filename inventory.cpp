#include "agencyinclude.h"
#include "guns.h"

void MainInventorySelection(PPLAYER player, PMAP map)
{
	cls(0);
	TIMER Timer;
	char * Selections[4] = {"Weapons", "Gizmos", "Syringes", "DEACTIVATE"};
	char * Title = "DSA INVENTORY CONTROL MANIFEST - ACTIVE";
	POINT SelectionPoints[4];
	POINT TitlePoint;
	WORD CurrentTitleColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
	int MaxTitleRedrawTime = 50;
	int CurrentTitleRedrawTime = 0;
	bool TitleColorIncreasing = true;
	WORD CurrentSelectedColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
	bool SelectedColorIncreasing = false;
	int MaxSelectedRedrawTime = 20;
	int CurrentSelectedRedrawTime = 0;
	bool RedrawSelections = true;
	bool RedrawTitle = true;
	int CurrentSelection = 0;
	USHORT key;
	int numevents = 0;
	int NumSelections = 4;
	TitlePoint.y = 0;
	TitlePoint.x = retrieveTextCenterH(Title);
	FLUSH;
	clearinputrecords();
	SelectionPoints[2].x = retrieveTextCenterH(Selections[2]);
	SelectionPoints[2].y = retrieveTextCenterV();
	SelectionPoints[0].x = 1;
	SelectionPoints[0].y = 2;
	SelectionPoints[3].x = getconsize().X - strlen(Selections[3]) - 2;
	SelectionPoints[3].y = getconsize().Y - 3;
	SelectionPoints[1].y = ((SelectionPoints[2].y + SelectionPoints[0].y) / 2);
	SelectionPoints[1].x = getconsize().X / 4 - strlen(Selections[1]) / 2;
	LoopSong(globals.musiclist.songs[INVENTORY_SONG]);
	setcursor(TitlePoint.x, TitlePoint.y);
	GREEN;
	bufferprints(30, Title, BEEPA);
	RED;
	for(int i = 0; i < NumSelections; i++){
		setcursor(SelectionPoints[i].x, SelectionPoints[i].y);
		writestring(Selections[i]);
		playMIXsound(INVENTORY_MAIN_POPUP_SOUND);
		Sleep(100);
	}
	while(1)
	{
		StartTimer(&Timer);
		if(RedrawTitle){
			LGREEN;
			setcursor(TitlePoint.x, TitlePoint.y);
			writestring(Title);
			RedrawTitle = false;
		}
		if(RedrawSelections)
		{
			for(i = 0; i < NumSelections; i++){
				if(i == CurrentSelection){
					setcolor(CurrentSelectedColor);
					setcursor(SelectionPoints[i].x, SelectionPoints[i].y);
					writestring(Selections[i]);
					
				}
				else{
					RED;
					setcursor(SelectionPoints[i].x, SelectionPoints[i].y);
					writestring(Selections[i]);
				}
				RED;
				setcursor(SelectionPoints[i].x - 1, SelectionPoints[i].y);
				if(i == CurrentSelection){	
					writechar('>');
				}
				else{
					writechar(' ');
				}
				setcursor(SelectionPoints[i].x + strlen(Selections[i]), SelectionPoints[i].y);
				if(i == CurrentSelection){
					writechar('<');
				}
				else{
					writechar(' ');
				}
				
				
			}
			RedrawSelections = false;
		}
		numevents = checkforinput();
		if (numevents > 1){
			for (i = 1; i <= numevents; i++){
				key = getinput(i);
				switch(key)
				{
				case VK_ESCAPE:
					return;
				case VK_DOWN:
				case VK_RIGHT:
				case VK_TAB:
					CurrentSelection++;
					CurrentSelection = CapInclusive(CurrentSelection,0,NumSelections-1);
					RedrawSelections = true;
					break;
				case VK_UP:
				case VK_LEFT:
					CurrentSelection--;
					CurrentSelection = CapInclusive(CurrentSelection,0,NumSelections-1);
					RedrawSelections = true;
					break;
				case VK_RETURN:
					FLUSH;
					clearinputrecords();
					RedrawSelections = true;
					RedrawTitle = true;
					switch(CurrentSelection){ //HERE'S WHERE WE BRANCH OFF.
					case 0:
						WeaponInventory(player, map, false);
						cls(0);
						break;
					case 1:
						GizmoInventory(player);
						cls(0);
						break;
					case 2:
						SyringeInventory(player);
						cls(0);
						break;
					case 3:
						LoopMainSong(player);
						return;
					}
					break;
				}
				FLUSH;
			}
			clearinputrecords();
		}
		
		Sleep(20);
		EndTimer(&Timer);
		CurrentSelectedRedrawTime += SplitElapsed(&Timer);
		CurrentTitleRedrawTime += SplitElapsed(&Timer);
		if(CurrentSelectedRedrawTime > MaxSelectedRedrawTime){
			CycleMainInventoryColor(&CurrentSelectedColor, &SelectedColorIncreasing);
			CurrentSelectedRedrawTime = 0;
			RedrawSelections = true;
		}
		if(CurrentTitleRedrawTime > MaxTitleRedrawTime){
			CycleMainInventoryColor(&CurrentTitleColor, &TitleColorIncreasing);
			CurrentTitleRedrawTime = 0;
			RedrawTitle = true;
		}
	}
}

void CycleMainInventoryColor(WORD* CurrentColor, bool* ColorIncreasing)
{
	if(*CurrentColor == 0){
		*CurrentColor = FOREGROUND_RED;
		*ColorIncreasing = true;
	}
	else if(*CurrentColor == (FOREGROUND_RED | FOREGROUND_INTENSITY)){
		*CurrentColor = FOREGROUND_RED;
		*ColorIncreasing = false;
	}
	else if(*ColorIncreasing){
		*CurrentColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
	}
	else{
		*CurrentColor = 0;
	}
}

void SyringeInventory(PPLAYER player)
{
	cls(0);
	int Temperatures[MAX_SYRINGES] = {random(32,35), random(32,35), random(32,35)};
	int Percentages[MAX_SYRINGES] = {100, 100, 100};
	bool Injection = false;
	char* Exitbuf = "DEACTIVATE";
	char* Titlebuf = "SYRINGE INVENTORY";
	POINT ExitPoint;
	ExitPoint.y = getconsize().Y - 2;
	ExitPoint.x = getconsize().X - strlen(Exitbuf) - 3;
	//	char buf[500];
	TIMER Timer;
	int i = 0;
	int numevents;
	USHORT key;
	int CurrentSelection = 0;
	int CurrentInjection = 0;
	int MaxTempTime = 50;
	int CurrentTempTime = 0;
	int BarWidth = getconsize().X / 2;
	int BarXOrigin = getconsize().X / 2 - BarWidth / 2;
	bool RedrawSyringes = true;
	POINT StatusPts[MAX_SYRINGES];
	StatusPts[0].x = BarXOrigin;
	StatusPts[0].y = getconsize().Y / 4;
	StatusPts[1].x = BarXOrigin;
	StatusPts[1].y = getconsize().Y / 2;
	StatusPts[2].x = BarXOrigin;
	StatusPts[2].y = 3 * getconsize().Y / 4;
	LGREEN;
	setcursor(retrieveTextCenterH(Titlebuf), 0);
	writestring(Titlebuf);
	while(1){
		StartTimer(&Timer);
		if(RedrawSyringes){
			for(i = 0; i < MAX_SYRINGES; i++){
				DrawInventorySyringe(player, i, Temperatures[i], Percentages[i], i == CurrentSelection, StatusPts[i], BarWidth);
			}
			if(CurrentSelection == MAX_SYRINGES){
				BLUE;
				setcursor(ExitPoint.x, ExitPoint.y);
				writechar('>');
				setcursor(ExitPoint.x + strlen(Exitbuf), ExitPoint.y);
				writechar('<');
				LBLUE;
				setcursor(ExitPoint.x, ExitPoint.y);
				writestring(Exitbuf);
			}
			else{
				BLUE;
				setcursor(ExitPoint.x, ExitPoint.y);
				writechar(' ');
				setcursor(ExitPoint.x + strlen(Exitbuf), ExitPoint.y);
				writechar(' ');
				BLUE;
				setcursor(ExitPoint.x, ExitPoint.y);
				writestring(Exitbuf);
			}
			RedrawSyringes = false;
		}
		numevents = checkforinput();
		if (numevents > 1){
			for (i = 1; i <= numevents; i++){
				key = getinput(i);
				switch(key)
				{
				case VK_ESCAPE:
					return;
				case VK_DOWN:
				case VK_TAB:
					CurrentSelection++;
					CurrentSelection = CapInclusive(CurrentSelection,0,MAX_SYRINGES + 1);
					RedrawSyringes = true;
					break;
				case VK_UP:
					CurrentSelection--;
					CurrentSelection = CapInclusive(CurrentSelection,0,MAX_SYRINGES + 1);
					RedrawSyringes = true;
					break;
				case VK_RETURN:
					switch(CurrentSelection){
					case SYRINGE_ALPHA:
					case SYRINGE_BETA:
					case SYRINGE_GAMMA:
						if(player->drugs.drugtype[CurrentSelection]){
							Injection = true;
							CurrentInjection = CurrentSelection;
						}
						break;
					case MAX_SYRINGES:
						return;
					default:
						break;
					}
					break;
				}
			}
			FLUSH;
		}
		clearinputrecords();
		
		
		Sleep(20);
		EndTimer(&Timer);
		
		if(Injection){
			playMIXsound(SYRINGE_USE_SOUND);
			while(Percentages[CurrentInjection] > 0){
				Percentages[CurrentInjection] -= random(5, 10);
				Percentages[CurrentInjection] = CapInclusive(Percentages[CurrentInjection], 0, 100);
				DrawInventorySyringe(player, CurrentInjection, Temperatures[CurrentInjection], Percentages[CurrentInjection], CurrentInjection == CurrentSelection, StatusPts[CurrentInjection], BarWidth);
				Sleep(70);
			}
			//INJECT HERE!
			InjectPlayer(player, CurrentInjection);
			Injection = false;
			CurrentInjection = 0;
			RedrawSyringes = true;
			
		}
		CurrentTempTime += SplitElapsed(&Timer);
		if(CurrentTempTime > MaxTempTime){
			for(i = 0; i < MAX_SYRINGES; i++){
				Temperatures[i] = random(32, 35);
			}
			CurrentTempTime = 0;
			RedrawSyringes = true;
		}
	}
}

void DrawInventorySyringe(PPLAYER player, int syringe, int temperature, int percentage, bool selected, POINT DrawPoint, int BarWidth)
{
	
	char buf[500];
	char tempbuf[500];
	char barbuf[500];
	int CurrentWidth = 0;
	RECT Rect;
	Rect.top = DrawPoint.y;
	Rect.bottom = Rect.top + 1;
	Rect.left = 0;
	Rect.right = getconsize().X - 2;
	
	strcpy(barbuf, "");
	if(percentage > 0){
		CurrentWidth = (BarWidth * percentage) / 100;
	}
	
	//GET THE NAME OF THE SYRINGE.
	ClearRect(Rect);
	switch(syringe)
	{
	case SYRINGE_ALPHA:
		strcpy(buf, "Alpha - ");
		break;
	case SYRINGE_BETA:
		strcpy(buf, "Beta - ");
		break;
	case SYRINGE_GAMMA:
		strcpy(buf, "Gamma - ");
		break;
	default:
		strcpy(buf, "UNKNOWN SYRINGE - ");
		break;
	}
	
	switch(player->drugs.drugtype[syringe])
	{
	case DRUG_STIMULANT:
		strcat(buf, "Oxymethyl D.: ");
		break;
	case DRUG_ADRENALIN:
		strcat(buf, "N.P. C-Adrenaline: ");
		break;
	case DRUG_REMEDK:
		strcat(buf, "Remed - K: ");
		break;
	case DRUG_ANTIDOTE:
		strcat(buf, "Atropine/Amyl Mix: ");
		break;
	default:
		strcat(buf, "EMPTY");
		break;
	}
	
	if(player->drugs.drugtype[syringe]){
		sprintf(tempbuf, "%d%c - %d%%", temperature, 248, percentage);
		strcat(buf, tempbuf);
	}
	
	if(selected){
		
		if(!player->drugs.drugtype[syringe]){
			DGRAY;
			setcursor(DrawPoint.x-1, DrawPoint.y);
			writechar('>');
			setcursor(DrawPoint.x + strlen(buf), DrawPoint.y);
			writechar('<');
			GRAY;
			setcursor(DrawPoint.x, DrawPoint.y);
			writestring(buf);
		}
		else{
			BLUE;
			setcursor(DrawPoint.x-1, DrawPoint.y);
			writechar('>');
			setcursor(DrawPoint.x + strlen(buf), DrawPoint.y);
			writechar('<');
			LBLUE;
			setcursor(DrawPoint.x, DrawPoint.y);
			writestring(buf);
		}	
	}
	
	else{
		if(!player->drugs.drugtype[syringe]){
			DGRAY;
		}
		else{
			BLUE;
		}
		setcursor(DrawPoint.x-1, DrawPoint.y);
		writechar(' ');
		setcursor(DrawPoint.x + strlen(buf), DrawPoint.y);
		writechar(' ');
		setcursor(DrawPoint.x, DrawPoint.y);
		writestring(buf);
	}
	
	if(player->drugs.drugtype[syringe]){
		setcursor(DrawPoint.x, DrawPoint.y + 1);
		if(percentage > 70){
			LGREEN;
		}
		else if (percentage > 50){
			LYELLOW;
		}
		else if (percentage > 30){
			LRED;
		}
		else{
			RED;
		}
		for(int i = 0; i < CurrentWidth; i++){
			barbuf[i] = (char)219;
		}
		barbuf[i] = '\0';
		
		writestring(barbuf);
	}
}

void InjectPlayer(PPLAYER player, int syringeid)
{
	PPSYCHSKILLSTRUCT OldPS = (PPSYCHSKILLSTRUCT)malloc(sizeof(PSYCHSKILLSTRUCT)); //FREE THIS UP WHEN THE SCRIPTED EVENT COMES AROUND.
	SCRIPTEDEVENT Event;
	MISSIONCHRONO MC;
	int i = 0;
	MC = globals.globalmapstruct.totaltime;
	memset(OldPS, 0, sizeof(*OldPS));
	
	int CurrentDrugType;
	if(syringeid < 0 || syringeid > MAX_SYRINGES){
		return;
	}
	CurrentDrugType = player->drugs.drugtype[syringeid];
	player->drugs.drugtype[syringeid] = 0;
	
	switch(CurrentDrugType)
	{
	case DRUG_STIMULANT:
		player->psychskill.psych.reflex += 3;
		OldPS->psych.reflex = -3;
		player->psychskill.psych.calmness -= 1;
		OldPS->psych.calmness = 1;
		Event.Context = (void*)OldPS;
		Event.Type = SCRIPTEDEVENT_TYPE_DRUGWEAROFF;
		MC.seconds += random(45, 90);
		calibratemissionchrono(&MC);
		Event.TriggerTime = MC;
		AddScriptedEvent(&Event);
		break;
	case DRUG_ADRENALIN:
		player->psychskill.psych.willpower += 2;
		OldPS->psych.willpower = -2;
		player->psychskill.psych.agression += 3;
		OldPS->psych.agression = -3;
		player->psychskill.psych.calmness -= 2;
		OldPS->psych.calmness = 2;
		player->psychskill.psych.intelligence -= 1;
		OldPS->psych.intelligence = 1;
		Event.Context = (void*)OldPS;
		Event.Type = SCRIPTEDEVENT_TYPE_DRUGWEAROFF;
		MC.seconds += random(45, 90);
		calibratemissionchrono(&MC);
		Event.TriggerTime = MC;
		AddScriptedEvent(&Event);
		break;
	case DRUG_ANTIDOTE:
		player->currentmissioninfo.antidoteon = true;
		Event.Type = SCRIPTEDEVENT_TYPE_ANTIDOTEWEAROFF;
		MC.seconds += random(45, 90);
		calibratemissionchrono(&MC);
		Event.TriggerTime = MC;
		AddScriptedEvent(&Event);
		break;
	case DRUG_REMEDK:
		Event.Type = SCRIPTEDEVENT_TYPE_REMEDK;
		for(i = 0; i < REMEDK_HEALTH_POINTS; i++){
			MC.seconds += random(1, 3);
			calibratemissionchrono(&MC);
			Event.TriggerTime = MC;
			AddScriptedEvent(&Event);
		}
		break;
	}
}

void GizmoInventory(PPLAYER player)
{
	if(player->p_gizmos->objcnt < 1){
		SetGlobalDBoxParam(DB_SCHEME_MONOCHROMATIC, DB_MEDIUM, 1, FALSE);
		StaticDialogBox("No Technology Items", "The Technology Inventory Is Empty.", "Press ENTER", &globals.dboxparams);
		return;
	}
	char StatusBuf[500];
	char HelpBuf[500];
	char ExtraBuf[500];
	char * TechTitle = "TECHNOLOGY INVENTORY SYSTEM";
	char * NotFound = "BAD GIZMO INDEX!";
	int TitleYOrigin = (getconsize().Y/2) - 3;
	int TitleXOrigin = retrieveTextCenterH(TechTitle);
	int GizmoTopYOrigin = TitleYOrigin + 2;
	int GizmoTopXOrigin = 0;
	int GizmoMiddleYOrigin = GizmoTopYOrigin + 1;
	int GizmoMiddleXOrigin = 0;
	int GizmoBottomYOrigin = GizmoMiddleYOrigin + 1;
	int GizmoBottomXOrigin = 0;
	int GizmoStatusYOrigin = GizmoBottomYOrigin + 2;
	int GizmoStatusXOrigin = 0;
	int GizmoHelpYOrigin = GizmoStatusYOrigin + 1;
	int GizmoHelpXOrigin = 0;
	int CurrentObject;
	//int AboveObject;
	//	int BelowObject;
	POTYPE ArmoryFinder;
	PPOTYPE ArmoryFound;
	POTYPE Finder;
	PPOTYPE Found = NULL;
	USHORT Key;
	char Quantity[50];
	bool ReworkList = true;
	bool DrawAbove = true;
	bool DrawBelow = true;
	bool UseQuantity = false;
	bool UseActive = false;
	bool CanActivate = false;
	bool CanDrop = false;
	bool IsGren = false;
	POINT WDPt;
	
	cls(0);
	
	
	while(1)
	{
		UseQuantity = false;
		UseActive = false;
		CanActivate = false;
		CanDrop = false;
		if(ReworkList)
		{
			CurrentObject = 0;
			ReworkList = false;
			
		}
		if(player->p_gizmos->objcnt < 2){
			DrawAbove = false;
			DrawBelow = false;
		}
		else if(player->p_gizmos->objcnt < 3){
			if(CurrentObject == 0){
				DrawBelow = true; //WE KNOW THAT WE CAN DRAW OBJECT 0 + 1
				DrawAbove = false;
			}
			else{
				DrawAbove = true; //WE KNOW WE CAN DRAW OBJECT 1 - 1.
				DrawBelow = false;
			}
		}
		
		setcursor(TitleXOrigin, TitleYOrigin);
		RED;
		writestring(TechTitle);
		
		//GET DATA ABOUT MIDDLE.
		Finder.gizmo.index = CurrentObject;
		Found = findObject(player->p_gizmos, &Finder, CompareGizmoIndex);
		if(!Found){
			GizmoMiddleXOrigin = retrieveTextCenterH(NotFound);
			setcursor(GizmoMiddleXOrigin, GizmoMiddleYOrigin);
			LRED;
			writestring(NotFound);
		}
		else if(!Found->gizmo.isgrenade){
			IsGren = false;
			switch(Found->gizmo.unit.device.devicetype)
			{
			case GIZMO_TYPE_BUG:
				UseQuantity = true;
				CanDrop = true;
				sprintf(Quantity, "%d Bugs Remain", Found->gizmo.unit.device.numberleft);
				break;
			case GIZMO_TYPE_SPIKEMIKE:
				UseQuantity = true;
				CanDrop = true;
				sprintf(Quantity, "%d Mikes Remain", Found->gizmo.unit.device.numberleft);
				break;
			case GIZMO_TYPE_NVG:
				UseActive = true;
				CanActivate = true;
				CanDrop = true;
				break;
			case GIZMO_TYPE_TAS:
				UseActive = true;
				CanActivate = true;
				CanDrop = true;
				break;
			case GIZMO_TYPE_XRIFLE:
				UseActive = true;
				CanActivate = true;
				CanDrop = true;
				break;
			case GIZMO_TYPE_FOVS:
				UseQuantity = true;
				CanDrop = true;
				sprintf(Quantity, "%d Drill Bits Remain", Found->gizmo.unit.device.numberleft);
				break;
			case GIZMO_TYPE_LIS:
				CanDrop = true;
				break;
			case GIZMO_TYPE_CIS:
				CanDrop = true;
				break;
			case GIZMO_TYPE_SBK:
				CanDrop = true;
				UseQuantity = true;
				sprintf(Quantity, "%d Units Remain", Found->gizmo.unit.device.numberleft);
				break;
			case GIZMO_TYPE_BINOCULARS:
				UseActive = true;
				CanActivate = true;
				CanDrop = true;
				break;
			case GIZMO_TYPE_C4TIMED:
				CanDrop = true;
				break;
			case GIZMO_TYPE_C4SWITCH:
				CanDrop = true;
				break;
			case GIZMO_TYPE_C4PROXIMITY:
				CanDrop = true;
				break;
			case GIZMO_TYPE_C4ELECTRONIC:
				CanDrop = true;
				break;
			case GIZMO_TYPE_OCTYLDICHLORIDE:
				CanDrop = true;
				UseQuantity = true;
				sprintf(Quantity, "%d Injections Remain", Found->gizmo.unit.device.numberleft);
				break;
			case GIZMO_TYPE_GASMASK:
				UseActive = true;
				CanActivate = true;
				CanDrop = true;
				break;
			case GIZMO_TYPE_TRAUMAPLATES:
				CanDrop = true;
				break;
			}
		}
		else{
			IsGren = true;
			CanActivate = true;
			UseActive = true;
			CanDrop = true;
		}
		//PRINT THE MIDDLE.
		if(!IsGren){
			ArmoryFinder.armorygizmo.type = Found->gizmo.unit.device.devicetype;
			ArmoryFinder.armorygizmo.isgrenade = false;
		}
		else{
			ArmoryFinder.armorygizmo.type = Found->gizmo.unit.grenade.grenadetype;
			ArmoryFinder.armorygizmo.isgrenade = true;
		}
		ArmoryFound = findObject(globals.armorygizmolist, &ArmoryFinder, CompareArmoryGizmoBothTypes);
		if(!ArmoryFound){
			GizmoMiddleXOrigin = retrieveTextCenterH(NotFound);
			setcursor(GizmoMiddleXOrigin, GizmoMiddleYOrigin);
			LRED;
			writestring(NotFound);
		}
		else{
			GizmoMiddleXOrigin = retrieveTextCenterH(ArmoryFound->armorygizmo.longname);
			setcursor(GizmoMiddleXOrigin, GizmoMiddleYOrigin);
			LGREEN;
			writestring(ArmoryFound->armorygizmo.longname);
		}
		
		//RETRIEVE OBJECT STATUS.
		strcpy(StatusBuf, "");
		if(!IsGren){
			if(UseActive){
				if(Found->gizmo.unit.device.active){
					strcat(StatusBuf, "Status - ACTIVE  ");
				}
				else{
					strcat(StatusBuf, "Status - inactive  ");
				}
			}
			if(UseQuantity){	
				sprintf(ExtraBuf, "Quantity - %s", Quantity);
				strcat(StatusBuf, ExtraBuf);
			}
			if(!UseQuantity && !UseActive){
				
				strcat(StatusBuf, "NO DATA AVAILABLE");
			}
		}
		else{
			if(Found->gizmo.unit.grenade.primertime == -1){
				strcat(StatusBuf, "Safe");
			}
			else if(Found->gizmo.unit.grenade.primertime == 0){
				strcat(StatusBuf, "Primed For CONTACT IGNITION");
			}
			else{
				sprintf(ExtraBuf, "Primed - Ignition %d Seconds After Contact", Found->gizmo.unit.grenade.primertime);
				strcat(StatusBuf, ExtraBuf);
			}
		}
		LBLUE;
		GizmoStatusXOrigin = retrieveTextCenterH(StatusBuf);
		setcursor(GizmoStatusXOrigin, GizmoStatusYOrigin);
		writestring(StatusBuf);
		
		//RETRIEVE HELP TEXT.
		LYELLOW;
		strcpy(HelpBuf, "");
		if(CanActivate){
			strcat(HelpBuf, "(ENTER) - Activate/Deactivate  ");
		}
		if(CanDrop){
			strcat(HelpBuf, "(D) - Drop  ");
		}
		strcat(HelpBuf, "(ESC) - Exit");
		GizmoHelpXOrigin = retrieveTextCenterH(HelpBuf);
		setcursor(GizmoHelpXOrigin, GizmoHelpYOrigin);
		writestring(HelpBuf);
		
		if(DrawBelow){
			Finder.gizmo.index = WrapNumber(CurrentObject + 1, 0, player->p_gizmos->objcnt);
			Found = findObject(player->p_gizmos, &Finder, CompareGizmoIndex);
			if(!Found){
				RED;
				GizmoBottomXOrigin = retrieveTextCenterH(NotFound);
				setcursor(GizmoBottomXOrigin, GizmoBottomYOrigin);
				writestring(NotFound);
			}
			else{
			if(Found->gizmo.isgrenade){
				ArmoryFinder.armorygizmo.type = Found->gizmo.unit.grenade.grenadetype;
				ArmoryFinder.armorygizmo.isgrenade = true;
			}
			else{
				ArmoryFinder.armorygizmo.type = Found->gizmo.unit.device.devicetype;
				ArmoryFinder.armorygizmo.isgrenade = false;
			}
			ArmoryFound = findObject(globals.armorygizmolist, &ArmoryFinder, CompareArmoryGizmoBothTypes);
			if(!ArmoryFound){
				GizmoBottomXOrigin = retrieveTextCenterH(NotFound);
				setcursor(GizmoBottomXOrigin, GizmoBottomYOrigin);
				RED;
				writestring(NotFound);
			}
			else{
				GizmoBottomXOrigin = retrieveTextCenterH(ArmoryFound->armorygizmo.longname);
				setcursor(GizmoBottomXOrigin, GizmoBottomYOrigin);
				GREEN;
				writestring(ArmoryFound->armorygizmo.longname);
			}
			}
		}
		
		if(DrawAbove){
			Finder.gizmo.index = WrapNumber(CurrentObject - 1, 0, player->p_gizmos->objcnt);
			Found = findObject(player->p_gizmos, &Finder, CompareGizmoIndex);
			if(!Found){
				RED;
				GizmoTopXOrigin = retrieveTextCenterH(NotFound);
				setcursor(GizmoTopXOrigin, GizmoTopYOrigin);
				writestring(NotFound);
			}
			else{
			if(Found->gizmo.isgrenade){
				ArmoryFinder.armorygizmo.type = Found->gizmo.unit.grenade.grenadetype;
				ArmoryFinder.armorygizmo.isgrenade = true;
			}
			else{
				ArmoryFinder.armorygizmo.type = Found->gizmo.unit.device.devicetype;
				ArmoryFinder.armorygizmo.isgrenade = false;
			}
			ArmoryFound = findObject(globals.armorygizmolist, &ArmoryFinder, CompareArmoryGizmoBothTypes);
			if(!ArmoryFound){
				GizmoTopXOrigin = retrieveTextCenterH(NotFound);
				setcursor(GizmoTopXOrigin, GizmoTopYOrigin);
				RED;
				writestring(NotFound);
			}
			else{
				GizmoTopXOrigin = retrieveTextCenterH(ArmoryFound->armorygizmo.longname);
				setcursor(GizmoTopXOrigin, GizmoTopYOrigin);
				GREEN;
				writestring(ArmoryFound->armorygizmo.longname);
			}
			}
		}
		
		
		Key = waitforVkey();
		switch(Key)
		{
		case VK_ESCAPE:
			return;
		case 'A':
		case VK_RETURN:
			if(CanActivate && Found){
				//ACTIVATE GIZMO HERE.
				Finder.gizmo.index = CurrentObject;
				Found = findObject(player->p_gizmos, &Finder, CompareGizmoIndex);
				if(!Found){
					break;
				}
				
				if(IsGren){
					if(Found->gizmo.unit.grenade.primertime != -1){
						DeactivatePlayerGrenade(player, CurrentObject);
					}
					else{
						ActivatePlayerGrenade(player, CurrentObject);
					}
				}
				else{
					
					if(Found->gizmo.unit.device.active){
						DeactivatePlayerGizmo(player, CurrentObject);
					}
					else{
						ActivatePlayerGizmo(player, CurrentObject);
					}
				}
				cls(0);
			}
			break;
		case 'D':
			if(CanDrop && Found){
				//DROP GIZMO HERE.
				Finder.gizmo.index = CurrentObject;
				Found = findObject(player->p_gizmos, &Finder, CompareGizmoIndex);
				if(!Found){
					break;
				}
				if(IsGren && Found->gizmo.unit.grenade.primertime == 0){
					SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
					StaticDialogBox("CONTACT FUSE", "You are NOT letting this grenade leave your hand with a contact fuse!", "Press ENTER", &globals.dboxparams);
					break;
				}
				else if(IsGren && Found->gizmo.unit.grenade.primertime != -1){
					SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
					StaticDialogBox("LIVE GRENADE", "You let the grenade leave your hand and fall to the ground. It's live.", "Press ENTER", &globals.dboxparams);
					//DROP A LIVE GRENADE!!!
					WDPt.x = player->location->xcoord;
					WDPt.y = player->location->ycoord;
					if(!SpawnLiveGrenade(WDPt, Found->gizmo.unit.grenade.grenadetype, Found->gizmo.unit.grenade.primertime)){
						StaticDialogBox("But...", "Somehow it doesn't make it out in the big bad world. ERROR.", "Press ENTER", &globals.dboxparams);
						break;
					}
					else{
						DeletePlayerActiveGrenade(player);
					}
				}
				else if(Found->gizmo.unit.device.active){
					DeactivatePlayerGizmo(player, CurrentObject);
					SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
					StaticDialogBox("Gizmo Deactivated", "You switch off the gizmo before dropping it.", "Press ENTER", &globals.dboxparams);
				}
				if(!IsGren){
					DropPlayerGizmo(player, CurrentObject);
				}
				if(player->p_gizmos->objcnt < 1){
					SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
					StaticDialogBox("No Technology Items", "The Technology Inventory Is Empty.", "Press ENTER", &globals.dboxparams);
					return;
				}
				CurrentObject--;
				CurrentObject = WrapNumber(CurrentObject, 0, player->p_gizmos->objcnt);
				
				cls(0);
			}
			break;
		case VK_UP:
		case VK_LEFT:
			cls(0);
			CurrentObject--;
			CurrentObject = WrapNumber(CurrentObject, 0, player->p_gizmos->objcnt);
			break;
		case VK_DOWN:
		case VK_RIGHT:
			cls(0);
			CurrentObject++;
			CurrentObject = WrapNumber(CurrentObject, 0, player->p_gizmos->objcnt);
			break;
		}
	}
}

void WeaponInventory(PPLAYER player, PMAP map, bool InVan) //IF IN VAN, CANNOT DROP ANYTHING.
{
	if(player->p_weapons->objcnt < 1){
		SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
		StaticDialogBox("Out Of Weaponry", "You have no weapons to select from!", "Press ENTER", &globals.dboxparams);
		return;
	}
	POTYPE Finder;
	PPOTYPE Found;
	PWEAPON FoundWeapon;
	char ActivityBuf[500];
	int ActivityXOrigin;
	int ActivityYOrigin = retrieveTextCenterV() + 3;
	int LoadedStatusXOrigin;
	int LoadedStatusYOrigin = retrieveTextCenterV();
	char LoadedStatusBuf[500];
	int NameXOrigin;
	int NameYOrigin = retrieveTextCenterV() - 2;
	char NameBuf[500];
	char ModifierBuf[500];
	int ModifierBufYOrigin = retrieveTextCenterV() + 2;
	int ModifierBufXOrigin;
	char ModifierActivityBuf[500];
	int ModifierActivityBufYOrigin = retrieveTextCenterV() + 4;
	int ModifierActivityBufXOrigin;
	char ChangeWeaponBuf[500];
	sprintf(ChangeWeaponBuf, "(%c),(%c) Change Weapon - (ESC) Quit", 24, 25);
	int ChangeWeaponXOrigin = retrieveTextCenterH(ChangeWeaponBuf);
	int ChangeWeaponYOrigin = retrieveTextCenterV() + 5;
	int CurrentWeapon = 0;
	bool FSCheck = false;
	bool SCheck = false;
	bool LSCheck = false;
	bool AnyModifiers = false;
	USHORT key;
	bool WeaponUseAmmo = false;
	if(player->currentweapon){
		CurrentWeapon = player->currentweapon->index;
	}
	bool NewWeapon = true;
	
	while(1)
	{
		if(NewWeapon)
		{
			NewWeapon = false;
			cls(0);
			Finder.weapon.index = CurrentWeapon;
			Found = findObject(player->p_weapons, &Finder, CompareWeaponIndex);
			if(!Found){
				RED;
				sprintf(NameBuf, "WEAPON NOT FOUND!");
				NameXOrigin = retrieveTextCenterH(NameBuf);
				setcursor(NameXOrigin, NameYOrigin);
				writestring(NameBuf);
				sprintf(ActivityBuf, "");
			}
			else{
				//NAME
				FoundWeapon = &Found->weapon;
				sprintf(NameBuf, "%s", FoundWeapon->longname);
				LRED;
				if(FoundWeapon == player->currentweapon){
					strcat(NameBuf, " (ACTIVATED)");
					LGREEN;
				}
				NameXOrigin = retrieveTextCenterH(NameBuf);
				setcursor(NameXOrigin, NameYOrigin);
				writestring(NameBuf);
				
				//AMMO LOADING
				int FullEmpty = GetWeaponLoadedState(FoundWeapon);
				if(FullEmpty == WEAPON_NOTAMMO){
					GRAY;
					sprintf(LoadedStatusBuf, "WEAPON DOES NOT USE AMMUNITION");
					WeaponUseAmmo = false;
					sprintf(ActivityBuf, "");
				}
				else
				{
					sprintf(ActivityBuf, "(R)eload/Unload - ");
					
					switch(FoundWeapon->loadtype)
					{
					case WEAP_LOADTYPE_CLIP:
						WeaponUseAmmo = true;
						switch(FullEmpty)
						{
						case WEAPON_UNLOADED:
							RED;
							if(!FoundWeapon->loadprocedure.clip.isclipinserted){
								strcpy(LoadedStatusBuf, "NO CLIP HAS BEEN INSERTED");
							}
							else{
								strcpy(LoadedStatusBuf, "INSERTED CLIP HAS RUN DRY");
							}
							break;
						case WEAPON_PARTIALLY_LOADED:
							LYELLOW;
							sprintf(LoadedStatusBuf, "PARTIALLY FULL CLIP OF %s %s", RetrieveMainAmmoName(&FoundWeapon->loadprocedure.clip.clipinserted.clipround), RetrieveSubAmmoShortName(&FoundWeapon->loadprocedure.clip.clipinserted.clipround));
							break;
						case WEAPON_FULLY_LOADED:
							LGREEN;
							sprintf(LoadedStatusBuf, "FULL CLIP OF %s %s", RetrieveMainAmmoName(&FoundWeapon->loadprocedure.clip.clipinserted.clipround), RetrieveSubAmmoShortName(&FoundWeapon->loadprocedure.clip.clipinserted.clipround));
							break;
						}
						break;
						case WEAP_LOADTYPE_CYLINDER:
							WeaponUseAmmo = true;
							switch(FullEmpty)
							{
							case WEAPON_UNLOADED:
								RED;
								strcpy(LoadedStatusBuf, "ALL CHAMBERS EMPTY");
								break;
							case WEAPON_PARTIALLY_LOADED:
								LYELLOW;
								strcpy(LoadedStatusBuf, "CYLINDER PARTIALLY LOADED");
								break;
							case WEAPON_FULLY_LOADED:
								LGREEN;
								strcpy(LoadedStatusBuf, "CYLINDER FULLY LOADED");
								break;
							}
							break;
							case WEAP_LOADTYPE_MAGAZINE:
								WeaponUseAmmo = true;
								switch(FullEmpty)
								{
								case WEAPON_UNLOADED:
									RED;
									strcpy(LoadedStatusBuf, "MAGAZINE TUBE EMPTY");
									break;
								case WEAPON_PARTIALLY_LOADED:
									LYELLOW;
									strcpy(LoadedStatusBuf, "MAGAZINE TUBE PARTIALLY LOADED");
									break;
								case WEAPON_FULLY_LOADED:
									LGREEN;
									strcpy(LoadedStatusBuf, "MAGAZINE TUBE FULLY LOADED");
									break;
								}
								break;
					}
					//MODIFIERS
					sprintf(ModifierBuf, " ");
					sprintf(ModifierActivityBuf, " ");
					if(FoundWeapon->modifiersattached.flashsuppressor){
						FSCheck = true;
						AnyModifiers = true;
						
						if(FoundWeapon->modifiersactivated.flashsuppressor){
							strcat(ModifierBuf, "Flash Suppressor ON - ");
							strcat(ModifierActivityBuf, "Remove (F)lash Suppressor - ");
						}
						else{
							strcat(ModifierBuf, "Flash Suppressor OFF - ");
							strcat(ModifierActivityBuf, "Attach (F)lash Suppressor - ");
						}
					}
					if(FoundWeapon->modifiersattached.silencer){
						SCheck = true;
						AnyModifiers = true;
						if(FoundWeapon->modifiersactivated.silencer){
							strcat(ModifierBuf, "Silencer ON - ");
							strcat(ModifierActivityBuf, "Remove (S)ilencer - ");
						}
						else{
							strcat(ModifierBuf, "Silencer OFF - ");
							strcat(ModifierActivityBuf, "Attach (S)ilencer - ");
						}
					}
					if(FoundWeapon->modifiersattached.lasersight){
						LSCheck = true;
						AnyModifiers = true;
						if(FoundWeapon->modifiersactivated.lasersight){
							strcat(ModifierBuf, "Laser Sight ON");
							strcat(ModifierActivityBuf, "Remove (L)aser Sight ");
						}
						else{
							strcat(ModifierBuf, "Laser Sight OFF");
							strcat(ModifierActivityBuf, "Attach (L)aser Sight ");
						}
					}
				}
				
				
				LoadedStatusXOrigin = retrieveTextCenterH(LoadedStatusBuf);
				setcursor(LoadedStatusXOrigin, LoadedStatusYOrigin);
				writestring(LoadedStatusBuf);
				if(AnyModifiers){
					ModifierBufXOrigin = retrieveTextCenterH(ModifierBuf);
					setcursor(ModifierBufXOrigin, ModifierBufYOrigin);
					LYELLOW;
					writestring(ModifierBuf);
					ModifierActivityBufXOrigin = retrieveTextCenterH(ModifierActivityBuf);
					setcursor(ModifierActivityBufXOrigin, ModifierActivityBufYOrigin);
					LBLUE;
					writestring(ModifierActivityBuf);
				}
			}	
			//ACTIONS AVAILABLE
			LBLUE;
			strcat(ActivityBuf, "(D)rop - (ENTER)Activate/Deactivate");
			ActivityXOrigin = retrieveTextCenterH(ActivityBuf);
			setcursor(ActivityXOrigin, ActivityYOrigin);
			writestring(ActivityBuf);
			LCYAN;
			setcursor(ChangeWeaponXOrigin, ChangeWeaponYOrigin);
			writestring(ChangeWeaponBuf);
			
		}
		
		key = waitforVkey();
		switch(key)
		{
		case 'A':
		case VK_RETURN:
			if(player->currentweapon != NULL && FoundWeapon == player->currentweapon){
				//DEACTIVATE IT.
				playMIXsound(ACTIVATEDEACTIVATE_WEAPON_SOUND);
				player->currentweapon = NULL;
				NewWeapon = true;
			}
			else{
				PGIZMO Giz;
				Giz = FindActiveGizmoOfType(GIZMO_TYPE_SPIKEMIKE, player->p_gizmos);
				if(!Giz){
					Giz = FindActiveGizmoOfType(GIZMO_TYPE_BINOCULARS, player->p_gizmos);
				}
				if(Giz){
					DeactivatePlayerGizmo(player, Giz->index);
					SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
					StaticDialogBox("Gizmo Stowed", "You stow away your current gizmo, as your weapon would interfere with its use.", "Press ENTER", &globals.dboxparams);
				}
				if(WeaponUseAmmo){
					playWeaponSound(FoundWeapon, WEAP_SOUND_RELOAD, SOUND_MODE_MIX);
				}
				else{
					playMIXsound(ACTIVATEDEACTIVATE_WEAPON_SOUND);
				}
				player->currentweapon = FoundWeapon;
				CurrentWeapon = FoundWeapon->index;
				NewWeapon = true;
			}
			break;
		case 'R':
			//RELOAD
			switch(FoundWeapon->loadtype)
			{
			case WEAP_LOADTYPE_CLIP:
				ClipLoadingInventory(player, FoundWeapon);
				NewWeapon = true;
				break;
			case WEAP_LOADTYPE_MAGAZINE:
				MagazineLoadingInventory(player, FoundWeapon);
				NewWeapon = true;
				break;
			case WEAP_LOADTYPE_CYLINDER:
				CylinderLoadingInventory(player, FoundWeapon);
				NewWeapon = true;
				break;
			default:
				break;
			}
			break;
			case 'D':
				//DROP IT.
				if(InVan){
					SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
					StaticDialogBox("Cannot Drop Here", "You can't drop your weapons in the van!", "Press ENTER", &globals.dboxparams);
					break;
				}
				if(player->currentweapon == FoundWeapon){
					player->currentweapon = NULL;
				}
				DropPlayerWeapon(player, CurrentWeapon);
				if(player->p_weapons->objcnt < 1){
					SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
					StaticDialogBox("Out Of Weaponry", "You have no weapons to select from!", "Press ENTER", &globals.dboxparams);
					return;
				}
				CurrentWeapon--;
				CurrentWeapon = WrapNumber(CurrentWeapon, 0, player->p_weapons->objcnt);
				NewWeapon = true;
				break;
			case 'L':
				if(LSCheck){
					bToggle(&FoundWeapon->modifiersactivated.lasersight);
					playWeaponLasersight(SOUND_MODE_MIX);
					NewWeapon = true;
				}
				break;
			case 'F':
				if(FSCheck){
					if(FoundWeapon->modifiersactivated.silencer && !FoundWeapon->modifiersactivated.flashsuppressor){
						SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
						playWeaponSilencer(SOUND_MODE_MIX);
						bToggle(&FoundWeapon->modifiersactivated.silencer);
						StaticDialogBox("Removing Silencer...", "You remove your silencer to make room for your flash suppressor.", "Press ENTER", &globals.dboxparams);
						
					}
					bToggle(&FoundWeapon->modifiersactivated.flashsuppressor);
					playWeaponFlashsuppressor(SOUND_MODE_MIX);
					NewWeapon = true;
				}
				break;
			case 'S':
				if(SCheck){
					if(FoundWeapon->modifiersactivated.flashsuppressor && !FoundWeapon->modifiersactivated.silencer){
						SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
						playWeaponFlashsuppressor(SOUND_MODE_MIX);
						bToggle(&FoundWeapon->modifiersactivated.flashsuppressor);
						StaticDialogBox("Removing Suppressor...", "You remove your flash suppressor to make room for your silencer.", "Press ENTER", &globals.dboxparams);
					}
					bToggle(&FoundWeapon->modifiersactivated.silencer);
					playWeaponSilencer(SOUND_MODE_MIX);
					NewWeapon = true;
				}
				break;
			case VK_UP:
				CurrentWeapon--;
				CurrentWeapon = WrapNumber(CurrentWeapon, 0, player->p_weapons->objcnt);
				NewWeapon = true;
				break;
			case VK_DOWN:
				CurrentWeapon++;
				CurrentWeapon = WrapNumber(CurrentWeapon, 0, player->p_weapons->objcnt);
				NewWeapon = true;
				break;
			case VK_ESCAPE:
				return;
		}
		
	}
}

void DropPlayerWeapon(PPLAYER player, int index)
{
	POTYPE Finder;
	PPOTYPE Found;
	float weighttoremove;
	
	Finder.weapon.index = index;
	Found = findObject(player->p_weapons, &Finder, CompareWeaponIndex);
	if(!Found){
		return;
	}
	weighttoremove = Found->weapon.weight + (((float)CountTotalRoundsInWeapon(&Found->weapon)) * (float)ROUND_WEIGHT);
	POTYPE NewWeapon = *Found;
	NewWeapon.weapon.index = globals.globalmapstruct.weapons->objcnt;
	NewWeapon.weapon.WorldPoint.x = player->location->xcoord;
	NewWeapon.weapon.WorldPoint.y = player->location->ycoord;
	deleteObject(player->p_weapons, Found);
	addtoList(globals.globalmapstruct.weapons, NewWeapon, CompareWeaponIndex);
	playWeaponSound(&NewWeapon.weapon, WEAP_SOUND_DROP, SOUND_MODE_MIX);
	ReorderWeaponListIndex(player->p_weapons);
	player->weight -= weighttoremove;
	if(player->weight < 0.0){
		player->weight = 0.0;
	}
}

void ClipLoadingInventory(PPLAYER player, PWEAPON weapon)
{
	char* ClipTitle = "CLIP LOADING SYSTEM";
	int ClipTitleXOrigin = retrieveTextCenterH(ClipTitle);
	int ClipTitleYOrigin = retrieveTextCenterV() - 3;
	char* WeaponTitle = weapon->longname;
	int WeaponTitleXOrigin = retrieveTextCenterH(WeaponTitle);
	int WeaponTitleYOrigin = retrieveTextCenterV() - 2;
	char CurrentSelectedBuf[500];
	int CurrentSelectedBufXOrigin;
	int CurrentSelectedBufYOrigin = retrieveTextCenterV();
	char CurrentLoadedBuf[500];
	int CurrentLoadedBufXOrigin;
	int CurrentLoadedBufYOrigin = retrieveTextCenterV() + 2;
	char* Actions = "(ENTER) Load This Clip - (D)rop This Clip";
	int ActionsXOrigin = retrieveTextCenterH(Actions);
	int ActionsYOrigin = retrieveTextCenterV()+3;
	char* EjectClipST = "(E)ject Current Weapon Clip";
	int EjectClipXOrigin = retrieveTextCenterH(EjectClipST);
	int EjectClipYOrigin = retrieveTextCenterV()+4;
	char ChangeClipBuf[500];
	sprintf(ChangeClipBuf, "(%c),(%c) Change Clip - (ESC) Quit", 24, 25);
	int ChangeClipBufXOrigin = retrieveTextCenterH(ChangeClipBuf);
	int ChangeClipBufYOrigin = retrieveTextCenterV() + 5;
	bool NewClip = true;
	USHORT key;
	PCLIP CurrentClip;
	int EjectResult;
	int InsertResult;
	
	CurrentClip = FindNextClipInInventory(player->ammo.cliplist, NULL, weapon, true);
	
	while(1)
	{
		if(NewClip){ //DRAWING SCHEME
			NewClip = false;
			cls(0);
			RED;
			setcursor(ClipTitleXOrigin, ClipTitleYOrigin);
			writestring(ClipTitle);
			//THE WEAPON NAME
			GRAY;
			setcursor(WeaponTitleXOrigin, WeaponTitleYOrigin);
			writestring(WeaponTitle);
			//THE CURRENT CLIP
			if(!CurrentClip){
				RED;
				strcpy(CurrentSelectedBuf, "NO CLIPS IN INVENTORY");
			}
			else{
				LGREEN;
				sprintf(CurrentSelectedBuf, "Clip of %s (%d/%d Rds)", RetrieveSubAmmoLongName(&CurrentClip->clipround), CurrentClip->currentrounds, weapon->maxammo);
			}
			CurrentSelectedBufXOrigin = retrieveTextCenterH(CurrentSelectedBuf);
			setcursor(CurrentSelectedBufXOrigin, CurrentSelectedBufYOrigin);
			writestring(CurrentSelectedBuf);
			//THE LOADED CLIP
			if(!weapon->loadprocedure.clip.isclipinserted){
				RED;
				sprintf(CurrentLoadedBuf, "Loaded : NO CLIP LOADED");
			}
			else{
				LRED;
				sprintf(CurrentLoadedBuf, "Loaded : Clip of %s (%d/%d Rds)", RetrieveSubAmmoShortName(&weapon->loadprocedure.clip.clipinserted.clipround), weapon->loadprocedure.clip.clipinserted.currentrounds, weapon->maxammo);
			}
			CurrentLoadedBufXOrigin = retrieveTextCenterH(CurrentLoadedBuf);
			setcursor(CurrentLoadedBufXOrigin, CurrentLoadedBufYOrigin);
			writestring(CurrentLoadedBuf);
			//THE AXSHUNS!
			LRED;
			setcursor(EjectClipXOrigin, EjectClipYOrigin);
			writestring(EjectClipST);
			LBLUE;
			setcursor(ActionsXOrigin, ActionsYOrigin);
			writestring(Actions);
			LCYAN;
			setcursor(ChangeClipBufXOrigin, ChangeClipBufYOrigin);
			writestring(ChangeClipBuf);
		}
		
		key = waitforVkey();
		switch(key)
		{
		case VK_UP:
			CurrentClip = FindPreviousClipInInventory(player->ammo.cliplist, CurrentClip, weapon, true);
			NewClip = true;
			break;
		case VK_DOWN:
			CurrentClip = FindNextClipInInventory(player->ammo.cliplist, CurrentClip, weapon, true);
			NewClip = true;
			break;
		case 'E':
			EjectResult = EjectClip(weapon, player->ammo.cliplist);
			switch(EjectResult)
			{
			case EJECTCLIP_OK_SAVED:
				//SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_MEDIUM, 1, FALSE);
				//StaticDialogBox("Clip Ejected And Saved", "You eject the clip and shove it back into your vest.", "Press ENTER", &globals.dboxparams);
				playMIXsound(CLIP_OUT_SAVE_SOUND);
				break;
			case EJECTCLIP_OK_DROPPED:
				//SetGlobalDBoxParam(DB_SCHEME_COOLBLUELINES, DB_MEDIUM, 1, FALSE);
				//StaticDialogBox("Clip Ejected And Dropped", "You eject the empty clip onto the ground.", "Press ENTER", &globals.dboxparams);
				playMIXsound(CLIP_OUT_DUMP_SOUND);
				break;
			default:
				SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
				StaticDialogBox("No Clip Loaded", "There is no clip loaded that you can eject.", "Press ENTER", &globals.dboxparams);
			}
			if(!CurrentClip){
				CurrentClip = FindNextClipInInventory(player->ammo.cliplist, NULL, weapon, true);
			}
			NewClip = true;
			break;
			case 'D':
				//DROP CLIP!
				if(CurrentClip)
				{
					playMIXsound(CLIP_DUMP_SOUND);
					PlayerDropClip(player, CurrentClip);
					ReorderPlayerAmmoLists(&player->ammo);
					CurrentClip = FindNextClipInInventory(player->ammo.cliplist, NULL, weapon, true);
					NewClip = true;
				}
				break;
			case VK_RETURN:
				//LOAD CLIP!
				if(CurrentClip)
				{
					if(weapon->loadprocedure.clip.isclipinserted)
					{
						//WE MUST EJECT THE OTHER ONE FIRST.
						EjectResult = EjectClip(weapon, player->ammo.cliplist);
						switch(EjectResult)
						{
						case EJECTCLIP_OK_SAVED:
							playMIXsound(CLIP_OUT_SAVE_SOUND);
							SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_MEDIUM, 1, FALSE);
							StaticDialogBox("Clip Ejected And Saved", "You eject your weapon's clip and shove it back into your vest.", "Press ENTER", &globals.dboxparams);
							
							break;
						case EJECTCLIP_OK_DROPPED:
							playMIXsound(CLIP_OUT_DUMP_SOUND);
							SetGlobalDBoxParam(DB_SCHEME_COOLBLUELINES, DB_MEDIUM, 1, FALSE);
							StaticDialogBox("Clip Ejected And Dropped", "You eject your weapon's empty clip onto the ground.", "Press ENTER", &globals.dboxparams);
							
							break;
						}
						if(!CurrentClip){
							CurrentClip = FindNextClipInInventory(player->ammo.cliplist, NULL, weapon, true);
						}
						NewClip = true;
					}
					
					InsertResult = LoadClip(player->ammo.cliplist, weapon, CurrentClip->id);
					switch(InsertResult)
					{
					case LOADCLIP_OK:
						playWeaponSound(weapon, WEAP_SOUND_RELOAD, SOUND_MODE_MIX);
						ReorderPlayerAmmoLists(&player->ammo);
						CurrentClip = FindNextClipInInventory(player->ammo.cliplist, NULL, weapon, true);
						break;
					default:
						SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
						StaticDialogBox("Cannot Load Clip", "This is a problem. Somehow, you aren't able to load the clip.", "Press ENTER", &globals.dboxparams);
					}
				}
				NewClip = true;
				break;
			case VK_ESCAPE:
				return;
		}
	}
}


void MagazineLoadingInventory(PPLAYER player, PWEAPON weapon)
{
	int retcode;
	char* MagazineTitle = "MAGAZINE LOADING SYSTEM";
	int MagChambers = weapon->maxammo;
	int MagazineTitleXOrigin = retrieveTextCenterH(MagazineTitle);
	int MagazineTitleYOrigin = retrieveTextCenterV() - ((MagChambers / 2) - 1) - 4;
	char* WeaponTitle = weapon->longname;
	int WeaponTitleXOrigin = retrieveTextCenterH(WeaponTitle);
	int WeaponTitleYOrigin = MagazineTitleYOrigin + 1;
	int EndOfMagTubesY = WeaponTitleYOrigin + MagChambers + 1;
	char CurrentSelectedBuf[500];
	int CurrentSelectedBufXOrigin;
	int CurrentSelectedBufYOrigin = EndOfMagTubesY + 1;
	//	char CurrentLoadedBuf[500];
	//	int CurrentLoadedBufXOrigin;
	//	int CurrentLoadedBufYOrigin = WeaponTitleYOrigin + 1;
	char* Actions = "(ENTER) Load This Round Type - (F)ill With This Type";
	int ActionsXOrigin = retrieveTextCenterH(Actions);
	int ActionsYOrigin = CurrentSelectedBufYOrigin + 1;
	char* EjectMagST = "(E)ject Round - (D)ump Ammo";
	int EjectMagXOrigin = retrieveTextCenterH(EjectMagST);
	int EjectMagYOrigin = ActionsYOrigin + 1;
	char ChangeMagRoundBuf[500];
	sprintf(ChangeMagRoundBuf, "(%c),(%c) Change Round Type - (ESC) Quit", 24, 25);
	int ChangeMagRoundBufXOrigin = retrieveTextCenterH(ChangeMagRoundBuf);
	int ChangeMagRoundBufYOrigin = EjectMagYOrigin + 1;
	bool NewRounds = true;
	bool LoadedSomething = false;
	USHORT key;
	PSTRAYROUNDS CurrentRounds;
	ROUND RoundStruct;
	int EjectResult;
	//	int InsertResult;
	
	CurrentRounds = FindNextStrayRoundsInInventory(player->ammo.roundtypes, NULL, weapon, true);
	RoundStruct = GetRoundStructFromStrayRounds(CurrentRounds);
	
	while(1)
	{
		if(NewRounds){ //DRAWING SCHEME
			NewRounds = false;
			cls(0);
			RED;
			setcursor(MagazineTitleXOrigin, MagazineTitleYOrigin);
			writestring(MagazineTitle);
			//THE WEAPON NAME
			GRAY;
			setcursor(WeaponTitleXOrigin, WeaponTitleYOrigin);
			writestring(WeaponTitle);
			//THE CURRENT CLIP
			if(!CurrentRounds){
				RED;
				strcpy(CurrentSelectedBuf, "NO ROUNDS LEFT IN INVENTORY");
			}
			else{
				LGREEN;
				sprintf(CurrentSelectedBuf, "%s (%d Left)", RetrieveSubAmmoLongName(&RoundStruct), CurrentRounds->numberofroundsleft);
			}
			CurrentSelectedBufXOrigin = retrieveTextCenterH(CurrentSelectedBuf);
			setcursor(CurrentSelectedBufXOrigin, CurrentSelectedBufYOrigin);
			writestring(CurrentSelectedBuf);
			//THE MAG TUBE
			DrawMagTubeContents(weapon, WeaponTitleYOrigin + 1);
			//THE AXSHUNS!
			LRED;
			setcursor(EjectMagXOrigin, EjectMagYOrigin);
			writestring(EjectMagST);
			LBLUE;
			setcursor(ActionsXOrigin, ActionsYOrigin);
			writestring(Actions);
			LCYAN;
			setcursor(ChangeMagRoundBufXOrigin, ChangeMagRoundBufYOrigin);
			writestring(ChangeMagRoundBuf);
		}
		
		key = waitforVkey();
		switch(key)
		{
		case VK_UP:
			CurrentRounds = FindPreviousStrayRoundsInInventory(player->ammo.roundtypes, CurrentRounds, weapon, true);
			RoundStruct = GetRoundStructFromStrayRounds(CurrentRounds);
			NewRounds = true;
			break;
		case VK_DOWN:
			CurrentRounds = FindNextStrayRoundsInInventory(player->ammo.roundtypes, CurrentRounds, weapon, true);
			RoundStruct = GetRoundStructFromStrayRounds(CurrentRounds);
			NewRounds = true;
			break;
		case 'E':
			EjectResult = EjectFromMagazine(weapon, player->ammo.roundtypes);
			switch(EjectResult)
			{
			case EJECTFROMMAG_OK:
				//SetGlobalDBoxParam(DB_SCHEME_DIGITALGREENLINES, DB_MEDIUM, 1, FALSE);
				//StaticDialogBox("Clip Ejected And Saved", "You eject the clip and shove it back into your vest.", "Press ENTER", &globals.dboxparams);
				playRoundInsert(&RoundStruct, false, false, SOUND_MODE_ASYNC);
				break;
			default:
				SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, FALSE);
				StaticDialogBox("Weapon Empty", "The weapon's magazine tube is empty.", "Press ENTER", &globals.dboxparams);
			}
			if(!CurrentRounds){
				CurrentRounds = FindNextStrayRoundsInInventory(player->ammo.roundtypes, NULL, weapon, true);
				RoundStruct = GetRoundStructFromStrayRounds(CurrentRounds);
			}
			NewRounds = true;
			break;
			case 'D':
				//DUMP AMMO!
				if(DumpStrayRoundsDialog(player, CurrentRounds))//WATCH OUT, YOU MAY HAVE TO REORDER THE LIST!
				{
					//WE HAVE TO REORDER.
					ReorderPlayerAmmoLists(&player->ammo);
					CurrentRounds = FindNextStrayRoundsInInventory(player->ammo.roundtypes, NULL, weapon, true);
					RoundStruct = GetRoundStructFromStrayRounds(CurrentRounds);
				}
				NewRounds = true;
				break;
			case VK_RETURN:
				LoadedSomething = true;
				//LOAD MAG!
				retcode = LoadMagazine(weapon, player->ammo.roundtypes, CurrentRounds->id, 1);
				if(retcode == LOADMAGAZINE_OK_REMOVED_NOROUNDS){
					playRoundInsert(&RoundStruct, false, false, SOUND_MODE_ASYNC);
					ReorderPlayerAmmoLists(&player->ammo);
					CurrentRounds = FindNextStrayRoundsInInventory(player->ammo.roundtypes, NULL, weapon, true);
					RoundStruct = GetRoundStructFromStrayRounds(CurrentRounds);
				}
				else if(retcode != LOADMAGAZINE_OK){
					SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, true);
					if(retcode == LOADMAGAZINEFAILED_MAGAZINEFULL){
						StaticDialogBox("Full Tube", "The magazine tube is full.", "Press ENTER", &globals.dboxparams);
					}
					else{
						
						StaticDialogBox("Error", "This is a problem. Somehow, you aren't able to load the magazine tube.", "Press ENTER", &globals.dboxparams);
					}
				}
				else{
					playRoundInsert(&RoundStruct, false, false, SOUND_MODE_ASYNC);
				}
				
				NewRounds = true;
				break;
			case 'F':
				LoadedSomething = true;
				//FILL MAG!
				if(weapon->loadprocedure.magazine.totalroundschambered < weapon->maxammo && weapon->maxammo > 1 && CurrentRounds->numberofroundsleft > 1){
					playRoundInsert(&RoundStruct, false, true, SOUND_MODE_ASYNC);
				}
				else if(weapon->loadprocedure.magazine.totalroundschambered < weapon->maxammo){
					playRoundInsert(&RoundStruct, false, false, SOUND_MODE_ASYNC);
				}
				if(FillMagazineWithRounds(weapon, CurrentRounds, player->ammo.roundtypes))
				{
					ReorderPlayerAmmoLists(&player->ammo);
					CurrentRounds = FindNextStrayRoundsInInventory(player->ammo.roundtypes, NULL, weapon, true);
					RoundStruct = GetRoundStructFromStrayRounds(CurrentRounds);
				}
				NewRounds = true;
				break;
			case VK_ESCAPE:
				if(LoadedSomething)
					playWeaponSound(weapon, WEAPONSOUND_RELOAD, SOUND_MODE_MIX);
				return;
		}
	}
}

void CylinderLoadingInventory(PPLAYER player, PWEAPON weapon)
{
	int retcode;
	char* CylinderTitle = "CYLINDER LOADING SYSTEM";
	int Chambers = weapon->maxammo;
	int CylinderTitleXOrigin = retrieveTextCenterH(CylinderTitle);
	int CylinderTitleYOrigin = retrieveTextCenterV() - ((Chambers / 2) - 1) - 4;
	char* WeaponTitle = weapon->longname;
	int WeaponTitleXOrigin = retrieveTextCenterH(WeaponTitle);
	int WeaponTitleYOrigin = CylinderTitleYOrigin + 1;
	int EndOfCylindersY = WeaponTitleYOrigin + Chambers + 1;
	char CurrentSelectedBuf[500];
	int CurrentSelectedBufXOrigin;
	int CurrentSelectedBufYOrigin = EndOfCylindersY + 1;
	//	char CurrentLoadedBuf[500];
	//	int CurrentLoadedBufXOrigin;
	//	int CurrentLoadedBufYOrigin = WeaponTitleYOrigin + 1;
	char* Actions = "(ENTER) Load/Unload Chamber w/ Type - (S)peedload w/ Type - (E)mpty All";
	int ActionsXOrigin = retrieveTextCenterH(Actions);
	int ActionsYOrigin = CurrentSelectedBufYOrigin + 1;
	char ChangeBuf[500];
	char ChangeTemp[500];
	sprintf(ChangeBuf, "(%c,%c) Change Round - ", 24,25);
	sprintf(ChangeTemp, "(%c,%c) Spin - (1-9) Chamber", 16, 17);
	strcat(ChangeBuf, ChangeTemp);
	int ChangeXOrigin = retrieveTextCenterH(ChangeBuf);
	int ChangeYOrigin = ActionsYOrigin + 1;
	char ExitBuf[500] = "(D)ump Ammo - (ESC) Close Chamber - (C) Close Chamber And Spin";
	int ExitXOrigin = retrieveTextCenterH(ExitBuf);
	int ExitYOrigin = ChangeYOrigin + 1;
	bool NewRounds = true;
	USHORT key;
	PSTRAYROUNDS CurrentRounds;
	ROUND RoundStruct;
	//	int EjectResult;
	int CurrentChamber = 0;
	int i;
	//	int InsertResult;
	POTYPE tmpObj;
	PPOTYPE lstObj;
	char buf[10];
	
	playMIXsound(CYLINDER_LOAD_OPENCHAMBER_SOUND);
	
	CurrentRounds = FindNextStrayRoundsInInventory(player->ammo.roundtypes, NULL, weapon, true);
	RoundStruct = GetRoundStructFromStrayRounds(CurrentRounds);
	
	while(1)
	{
		if(NewRounds){ //DRAWING SCHEME
			NewRounds = false;
			cls(0);
			RED;
			setcursor(CylinderTitleXOrigin, CylinderTitleYOrigin);
			writestring(CylinderTitle);
			//THE WEAPON NAME
			GRAY;
			setcursor(WeaponTitleXOrigin, WeaponTitleYOrigin);
			writestring(WeaponTitle);
			//THE CURRENT CLIP
			if(!CurrentRounds){
				RED;
				strcpy(CurrentSelectedBuf, "NO ROUNDS LEFT IN INVENTORY");
			}
			else{
				LGREEN;
				sprintf(CurrentSelectedBuf, "%s (%d Left)", RetrieveSubAmmoLongName(&RoundStruct), CurrentRounds->numberofroundsleft);
			}
			CurrentSelectedBufXOrigin = retrieveTextCenterH(CurrentSelectedBuf);
			setcursor(CurrentSelectedBufXOrigin, CurrentSelectedBufYOrigin);
			writestring(CurrentSelectedBuf);
			//THE CHAMBERS
			for(i = 0; i < Chambers; i++){
				DrawChamberContents(weapon, i, CurrentChamber, WeaponTitleYOrigin + 1 + i);
			}
			//THE AXSHUNS!
			LRED;
			setcursor(ExitXOrigin, ExitYOrigin);
			writestring(ExitBuf);
			LCYAN;
			setcursor(ChangeXOrigin, ChangeYOrigin);
			printf("%s", ChangeBuf);
			LBLUE;
			setcursor(ActionsXOrigin, ActionsYOrigin);
			writestring(Actions);
			
		}
		
		key = waitforVkey();
		switch(key)
		{
		case VK_UP:
			CurrentRounds = FindPreviousStrayRoundsInInventory(player->ammo.roundtypes, CurrentRounds, weapon, true);
			RoundStruct = GetRoundStructFromStrayRounds(CurrentRounds);
			NewRounds = true;
			break;
		case VK_DOWN:
			CurrentRounds = FindNextStrayRoundsInInventory(player->ammo.roundtypes, CurrentRounds, weapon, true);
			RoundStruct = GetRoundStructFromStrayRounds(CurrentRounds);
			NewRounds = true;
			break;
		case VK_RIGHT:
			weapon->loadprocedure.cylinder.currentposition = WrapNumber(weapon->loadprocedure.cylinder.currentposition + 1, 0, weapon->maxammo);
			NewRounds = true;
			break;
		case VK_LEFT:
			weapon->loadprocedure.cylinder.currentposition = WrapNumber(weapon->loadprocedure.cylinder.currentposition - 1, 0, weapon->maxammo);
			NewRounds = true;
			break;
		case 'E':
			//EMPTY CYLINDER!
			for(i = 0; i < weapon->maxammo; i++)
				EmptyCylinderChamber(weapon, player->ammo.roundtypes, i);
			playMIXsound(DUMP_GENERIC_AMMO_SOUND);
			NewRounds = true;
			break;
		case 'D':
			//DUMP AMMO!
			if(DumpStrayRoundsDialog(player, CurrentRounds))//WATCH OUT, YOU MAY HAVE TO REORDER THE LIST!
			{
				//WE HAVE TO REORDER.
				ReorderPlayerAmmoLists(&player->ammo);
				CurrentRounds = FindNextStrayRoundsInInventory(player->ammo.roundtypes, NULL, weapon, true);
				RoundStruct = GetRoundStructFromStrayRounds(CurrentRounds);
			}
			NewRounds = true;
			break;
		case VK_RETURN:
			switch(weapon->loadprocedure.cylinder.statusindicators[CurrentChamber])
			{
				//EMPTY CYLINDER
			case CYLINDER_CHAMBER_FULL:
				playRoundInsert(&weapon->loadprocedure.cylinder.chamberedrounds[CurrentChamber],false, false, SOUND_MODE_ASYNC);
				EmptyCylinderChamber(weapon, player->ammo.roundtypes, CurrentChamber);
				break;
			default:
			case CYLINDER_CHAMBER_SHELLONLY:
				playRoundDump(&weapon->loadprocedure.cylinder.chamberedrounds[CurrentChamber], false, SOUND_MODE_ASYNC);
				weapon->loadprocedure.cylinder.statusindicators[CurrentChamber] = CYLINDER_CHAMBER_EMPTY;
				break;
			case CYLINDER_CHAMBER_EMPTY:
				//LOAD CYLINDER!
				if(CurrentRounds == NULL){
					break;
				}
				retcode = LoadCylinder(weapon, player->ammo.roundtypes, CurrentRounds->id, CurrentChamber);
				if(retcode == LOADCYLINDER_OK_REMOVED_NOROUNDS){
					tmpObj.rounds.id = CurrentRounds->id;
					lstObj = findObject(player->ammo.roundtypes, &tmpObj, CompareStrayRoundsID);
					if(lstObj)
						deleteObject(player->ammo.roundtypes, lstObj);
					playRoundInsert(&RoundStruct, false, false, SOUND_MODE_ASYNC);
					ReorderPlayerAmmoLists(&player->ammo);
					CurrentRounds = FindNextStrayRoundsInInventory(player->ammo.roundtypes, NULL, weapon, true);
					RoundStruct = GetRoundStructFromStrayRounds(CurrentRounds);
				}
				else if(retcode != LOADCYLINDER_OK){
					SetGlobalDBoxParam(DB_SCHEME_EMERGENCYREDLINES, DB_MEDIUM, 1, true);
					StaticDialogBox("Error", "This is a problem. Somehow, you aren't able to load the chamber.", "Press ENTER", &globals.dboxparams);
				}
				else{
					playRoundInsert(&RoundStruct, false, false, SOUND_MODE_ASYNC);
				}
				break;
			}
			
			NewRounds = true;
			break;
			case 'S':
				//FILL CHAMBERS!
				for(i = 0; i < weapon->maxammo; i++)
					EmptyCylinderChamber(weapon, player->ammo.roundtypes, i);
				for(i = 0; i < weapon->maxammo; i++)
				{
					if(LoadCylinder(weapon, player->ammo.roundtypes, CurrentRounds->id, i) == LOADCYLINDERFAILED_NOAMMOLEFT)
						break;
				}
				playRoundInsert(&RoundStruct, true, false, SOUND_MODE_ASYNC);
				if(CurrentRounds->numberofroundsleft < 1)
				{
					tmpObj.rounds.id = CurrentRounds->id;
					lstObj = findObject(player->ammo.roundtypes, &tmpObj, CompareStrayRoundsID);
					if(lstObj)
						deleteObject(player->ammo.roundtypes, lstObj);
					ReorderPlayerAmmoLists(&player->ammo);
					CurrentRounds = FindNextStrayRoundsInInventory(player->ammo.roundtypes, NULL, weapon, true);
					RoundStruct = GetRoundStructFromStrayRounds(CurrentRounds);
				}
				NewRounds = true;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				sprintf(buf, "%c", key);
				if(buf[0] == '0'){
					strcpy(buf, "10");
				}
				CurrentChamber = CapInclusive(atoi(buf) - 1,0, weapon->maxammo - 1);
				NewRounds = true;
				break;
			case 'C':
				weapon->loadprocedure.cylinder.currentposition = rand()%weapon->maxammo;
				playMIXsound(CYLINDER_LOAD_SPINCHAMBER_SOUND);
				return;
			case VK_ESCAPE:
				playMIXsound(CYLINDER_LOAD_CLOSECHAMBER_SOUND);
				return;
		}
	}
}

void DrawChamberContents(PWEAPON Weapon, int ChamberNumber, int SelectedNumber, int ycoord)
{
	char buf[500] = "";
	char tempbuf[500];
	int x;
	switch(Weapon->loadprocedure.cylinder.statusindicators[ChamberNumber])
	{
	default:
		RED;
		sprintf(tempbuf, "ERROR!!!");
		break;
	case CYLINDER_CHAMBER_EMPTY:
		GRAY;
		sprintf(tempbuf, "%d: EMPTY CHAMBER",ChamberNumber+1);
		break;
	case CYLINDER_CHAMBER_SHELLONLY:
		GRAY;
		sprintf(tempbuf, "%d: SHELL CASING",ChamberNumber+1);
		break;
	case CYLINDER_CHAMBER_FULL:
		LBLUE;
		sprintf(tempbuf, "%d: %s", ChamberNumber+1, RetrieveSubAmmoLongName(&Weapon->loadprocedure.cylinder.chamberedrounds[ChamberNumber]));
		break;
		
	}
	if(ChamberNumber == SelectedNumber)
		LRED;
	sprintf(buf, "%s", tempbuf);
	x = retrieveTextCenterH(buf);
	setcursor(x, ycoord);
	writestring(buf);
	if(ChamberNumber == Weapon->loadprocedure.cylinder.currentposition){
		RED;
		setcursor(x-2, ycoord);
		writechar((char)16);
	}
	
}

void DrawMagTubeContents(PWEAPON Weapon, int YOrigin)
{
	char buf[500];
	//THE FIRST TUBE IS ALWAYS TO BE SHOWN AS "First: *contents*"
	//IF THERE IS NO NEXT, IF ALL ARE FULL, THEN DISPLAY AS NORMAL.
	//THE NEXT AVAILABLE TUBE IS TO BE SHOWN AS "Next: EMPTY"
	if(!Weapon)
		return;
	if(Weapon->loadprocedure.magazine.totalroundschambered < 1){
		strcpy(buf, "First: EMPTY");
		GRAY;
	}
	else{
		sprintf(buf, "First: %s", RetrieveSubAmmoLongName(&Weapon->loadprocedure.magazine.chamberedrounds[0]));
		LRED;
	}
	int XOrigin = retrieveTextCenterH(buf);
	setcursor(XOrigin, YOrigin);
	writestring(buf);
	LBLUE;
	for(int i = 1; i < Weapon->maxammo; i++)
	{
		YOrigin++;
		if(i >= Weapon->loadprocedure.magazine.totalroundschambered)
		{
			GRAY;
			sprintf(buf, "Next: EMPTY");
			XOrigin = retrieveTextCenterH(buf);
			setcursor(XOrigin, YOrigin);
			writestring(buf);
			LBLUE;
		}
		else{
			sprintf(buf, "%d: %s", i, RetrieveSubAmmoLongName(&Weapon->loadprocedure.magazine.chamberedrounds[i]));
			XOrigin = retrieveTextCenterH(buf);
			setcursor(XOrigin, YOrigin);
			writestring(buf);
		}
	}
}

bool DumpStrayRoundsDialog(PPLAYER player, PSTRAYROUNDS Rounds)
{
	char buf[2500];
	ROUND RS;
	RS = GetRoundStructFromStrayRounds(Rounds);
	SetGlobalDBoxParam(DB_SCHEME_COOLBLUELINES, DB_MEDIUM, 5, true);
	sprintf(buf, "You have %d %s in your inventory. How many do you want to get rid of?", Rounds->numberofroundsleft, RetrieveSubAmmoLongName(&RS));
	int RoundsToDump = NumberDialogBox("AMMO DUMP", buf, "ENTER QUANTITY", 5, &globals.dboxparams);
	if(!RoundsToDump){
		return false;
	}
	playRoundDump(&RS, RoundsToDump > 1, SOUND_MODE_ASYNC);
	if(PlayerDropStrayRounds(player, Rounds, RoundsToDump)){
		return true;
	}
	return false;
}
