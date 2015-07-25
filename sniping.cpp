#include "agencyinclude.h"

SNIPERESULT snipe(PPLAYER ps, PWEAPON weapon, bool training)
{
	SNIPERESULT sr;
	int hitcode;
	bool shotfired = false;
	bool critical = false;
	
	
	if(!weapon){
			shotfired = false;
	}
	else{
		//PLAY RELOAD SOUND
		SnipeShowOptics(ps, training);
		//NOW DO THE TEST
		hitcode = snipetest(ps, training);
	}
		

		
		//PLAY THE SOUND FOR THE WEAPON AND FLASH
		if(hitcode != SNIPECODE_OVEROCULAR){
		playWeaponSound(weapon, WEAPONSOUND_SHOOT, SOUND_MODE_MIX);
		flashscope();
		Sleep(500);
		if (hitcode != SNIPECODE_MISS && hitcode != SNIPECODE_CHEST_SCRAPE ){
			//CRITICAL POSSIBLITY = SNIPING SKILL FOR A MAX CHANCE OF 40%.
		}
		
		sr.shotfired = true;
		}
		else{
			if(!training){
				//ADD IN A SCRIPTED EVENT THAT RESTORES OCULAR FOCUS.
				ps->currentmissioninfo.cansnipe = false;
				SCRIPTEDEVENT Event;
				Event.Type = SCRIPTEDEVENT_TYPE_OCULARFOCUS;
				Event.TriggerTime = GetTimeToNewOcularFocus(ps, &globals.globalmapstruct.totaltime);
				AddScriptedEvent(&Event);
			}
		}
		reportsnipehit(hitcode, critical);
		sr.hitcode = hitcode;
		sr.critical = critical;
	return sr;
}

void SnipeShowOptics(PPLAYER ps, bool training)
{
	char* Normal = "DSA STANDARD OPTICS ONLINE";
	char* NV = "DSA X-RIFLE INTERFACE - LIGHT AMPLIFICATION";
	char* Therm = "DSA X-RIFLE INTERFACE - THERMAL AMPLIFICATION";
	char* Switch = "X-RIFLE - NO CONNECTION - DEFAULTING TO NORMAL OPTICS";
	POINT Point;
	Point.y = retrieveTextCenterV();
	if(training || !FindActiveGizmoOfType(GIZMO_TYPE_XRIFLE, ps->p_gizmos)){
		WHITE;
		Point.x = retrieveTextCenterH(Normal);
		setcursor(Point.x, Point.y);
		bufferprints(30, Normal, BEEPC);
		Sleep(500);
		DGRAY;
		showscope();
		Sleep(100);
		GRAY;
		showscope();
		Sleep(100);
		WHITE;
		showscope();
		
	}
	else if(FindActiveGizmoOfType(GIZMO_TYPE_TAS, ps->p_gizmos)){
		LRED;
		Point.x = retrieveTextCenterH(Therm);
		setcursor(Point.x, Point.y);
		bufferprints(30, Therm, BEEPC);
		Sleep(500);
		RED;
		showscope();
		Sleep(100);
		LRED;
		showscope();
	}
	else if(FindActiveGizmoOfType(GIZMO_TYPE_NVG, ps->p_gizmos)){
		LGREEN;
		Point.x = retrieveTextCenterH(NV);
		setcursor(Point.x, Point.y);
		bufferprints(30, NV, BEEPC);
		Sleep(500);
		GREEN;
		showscope();
		Sleep(100);
		LGREEN;
		showscope();
	}
	else
	{
			WHITE;
		Point.x = retrieveTextCenterH(Switch);
		bufferprints(30, Switch, BEEPC);
		Sleep(500);
		DGRAY;
		showscope();
		Sleep(100);
		GRAY;
		showscope();
		Sleep(100);
		WHITE;
		showscope();
	}
}

WORD SnipeGetOpticColor(PPLAYER ps, bool training)
{
	if(training)
	{
		return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	}
	if(!FindActiveGizmoOfType(GIZMO_TYPE_XRIFLE, ps->p_gizmos)){
		return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	}
	else if(FindActiveGizmoOfType(GIZMO_TYPE_TAS, ps->p_gizmos)){
		return FOREGROUND_RED | FOREGROUND_INTENSITY;
	}
	else if(FindActiveGizmoOfType(GIZMO_TYPE_NVG, ps->p_gizmos)){
		return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	}
	else{
		return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	}
}

void RestoreOcularFocus(PPLAYER player)
{
	player->currentmissioninfo.cansnipe = TRUE;
	SetGlobalDBoxParam(DB_SCHEME_BLUEANDYELLOW, DB_MEDIUM, 1, FALSE);
	StaticDialogBox("Ocular Focus Restored", "Your eyes calm down from their painful over-correction and return to normal. You feel ready to snipe again should the need arise.", "Press ENTER", &globals.dboxparams);
}

MISSIONCHRONO GetTimeToNewOcularFocus(PPLAYER player, PMISSIONCHRONO totaltime)
{
	MISSIONCHRONO OurChron = *totaltime;
	int minutes = 0;
	int seconds = 0;
	int split = 0;

	//WE'LL JUST GO AHEAD AND GET THE NUMBER OF SPLIT SECONDS TO RECOVERY.

		//WE WANT A NUMBER WITHIN MAX_SNIPE_OCULAR_STATES.
	double MinTime = (double)MAX_OCULAR_RECOVERY_SECONDS;
	double MaxSubtractive = (double)MAX_OCULAR_RECOVERY_SECONDS - MIN_OCULAR_RECOVERY_SECONDS;
	double CurrentPercentage = (double)(player->psychskill.skills.snipingcombat * 100 / MAX_SKILL);
	MinTime -= MaxSubtractive * CurrentPercentage / 100.0;
	MinTime *= 100; //FOR SPLIT SECONDS.

	split = (int)MinTime;


	if(split >= 100){
		seconds = split/100;
		split = split%100;
	}
	if(seconds >= 60){
		minutes = seconds /60;
		seconds = seconds % 60;
	}
	OurChron.minutes += minutes;
	OurChron.seconds += seconds;
	OurChron.split += split;
	calibratemissionchrono(&OurChron);
	return OurChron;
}

void SnipeDrawOcularState(int status)
{
	RECT ClearingRect;
	ClearingRect.top = 0;
	ClearingRect.left = 0;
	ClearingRect.right = getconsize().X/2 - 3;
	ClearingRect.bottom = 0;

	ClearRect(ClearingRect);
	setcursor(0,0);
	WHITE;
	writestring("Ocular Status: ");
	switch(status)
	{
	case SNIPEOCULAR_RELAXED:
		LGREEN;
		writestring("Relaxed");
		break;
	case SNIPEOCULAR_TENSING:
		LBLUE;
		writestring("Tensing");
		break;
	case SNIPEOCULAR_TENSE:
		GRAY;
		writestring("Tensed");
		break;
	case SNIPEOCULAR_TREMBLING:
		YELLOW;
		writestring("Trembling");
		break;
	case SNIPEOCULAR_BLURRING:
		LYELLOW;
		writestring("Blurring");
		break;
	case SNIPEOCULAR_LOSINGFOCUS:
		LRED;
		writestring("LOSING FOCUS");
		break;
	default:
		RED;
		writestring("UNKNOWN");
		break;
	}
}

int GetNewOcularState(PPLAYER player, int totalsplitelapsed)
{
	//WE WANT A NUMBER WITHIN MAX_SNIPE_OCULAR_STATES.
	double MaxTime = (double)MIN_OCULAR_HOLD_SECONDS;
	double MaxAdditive = (double)MAX_OCULAR_HOLD_SECONDS - MIN_OCULAR_HOLD_SECONDS;
	double CurrentPercentage = (double)(player->psychskill.skills.snipingcombat * 100 / MAX_SKILL);
	MaxTime += MaxAdditive * CurrentPercentage / 100.0;
	MaxTime *= 100; //FOR SPLIT SECONDS.

	//WE NOW HAVE THE MAXIMUM AMOUNT OF TIME BEFORE WE GO BLURRY.
	if(totalsplitelapsed > MaxTime){
		return SNIPEOCULAR_LOST;
	}
	double CurrentDState = (double)totalsplitelapsed * (double)MAX_SNIPE_OCULAR_STATES / MaxTime;
	int CurrentIState = (int)CurrentDState;
	return CurrentIState;
}

int snipetest(PPLAYER ps, bool training)
{
//	int choice; 
	int vcenter;
	int hcenter;
	int numevents;
	int i;
	unsigned int splittime = 0;
	int OcularState = SNIPEOCULAR_RELAXED;
	int NextState = SNIPEOCULAR_RELAXED;
	USHORT key;
	WORD opticcolor = SnipeGetOpticColor(ps, training);
	TIMER Timer;
	
	INPUT_RECORD InputRec;
	
	RECT scopebound;
	//ULONG events;
//	ULONG written;
	
	COORD consize;
	COORD targetxarrow;
	COORD targetyarrow;
	COORD yourxarrow;
	COORD scoperedrawpt1;
	COORD scoperedrawpt2;

	BOOL yourxarrowdir = 0;
	COORD youryarrow;
	BOOL youryarrowdir = 0;
	
	consize = getconsize();
	hcenter = consize.X / 2;
	vcenter = consize.Y / 2;
	
	FLUSH;
	clearinputrecords();

	//GET THE SCOPE BOUNDARIES
	scopebound.left = 12;
	scopebound.right = consize.X - 16;
	scopebound.top = 1;
	scopebound.bottom = consize.Y - 2;
	
	scoperedrawpt1.X = hcenter - 2;
	scoperedrawpt1.Y = vcenter - 1;

	scoperedrawpt2.X = hcenter -3;
	scoperedrawpt2.Y = vcenter;

	targetxarrow.X = random(scopebound.left + 2, scopebound.right - 2);
	targetxarrow.Y = vcenter + 1;
	if(targetxarrow.X == hcenter){
		targetxarrow.X ++;
	}
	
	targetyarrow.X =  hcenter - 1;
	targetyarrow.Y = random(scopebound.top + 2, scopebound.bottom - 2);
	if(targetyarrow.Y == vcenter){
		targetyarrow.Y ++;
	}
	
	yourxarrow.X = random(scopebound.left, scopebound.right);
	yourxarrow.Y = vcenter - 1;
	
	youryarrow.X = hcenter - 3;
	youryarrow.Y =	random(scopebound.top, scopebound.bottom);
	
	youryarrowdir = rand()%2;
	yourxarrowdir = rand()%2;
	
	
	setcursor(targetxarrow.X, targetxarrow.Y);
	LRED;
	printf("%c",SNIPECHAR_UPARROW);
	
	setcursor(targetyarrow.X, targetyarrow.Y);
	LRED;
	printf("%c",SNIPECHAR_RTARROW);

	SnipeDrawOcularState(OcularState);
	
	memset(&InputRec, 0, sizeof(INPUT_RECORD));
	FLUSH;
	//	memset(&yourxarrowoverwrite, 0, sizeof(CHAR_INFO));
	//memset(&youryarrowoverwrite, 0, sizeof(CHAR_INFO));
	for(;;)
	{
	
		StartTimer(&Timer);
		//SET THE CURSOR TO REWRITE THE CHARACTER UNDERNEATH THE ARROW
		setcursor(yourxarrow.X, yourxarrow.Y);
		//	printf("%c", yourxarrowoverwrite.Char.AsciiChar);
		printf(" ");
		
		setcursor(youryarrow.X, youryarrow.Y);
		//	printf("%c", youryarrowoverwrite.Char.AsciiChar);
		printf(" ");
		
		
		//SET NEW COORDINATES
		if (yourxarrowdir) // IF GOING RIGHT
		{
			if (yourxarrow.X < scopebound.right)
			{
				//yourxarrow.X = random(scopebound.left, scopebound.right);
				yourxarrow.X += 1;
			}
		}
		
		else if (!yourxarrowdir) //IF GOING LEFT
		{
			if (yourxarrow.X > scopebound.left)
			{
				yourxarrow.X -= 1;
			}
		}
		//	yourxarrow.X += 1;
		if (youryarrowdir) // IF GOING DOWN
		{
			if (youryarrow.Y < scopebound.bottom)
			{
				//yourxarrow.X = random(scopebound.left, scopebound.right);
				youryarrow.Y += 1;
			}
		}
		
		else if (!youryarrowdir) //IF GOING UP
		{
			if (youryarrow.Y > scopebound.top)
			{
				youryarrow.Y -= 1;
			}
		}
		
		
		
		//GET THE NEW CHAR_INFOS AT THOSE COORDS
		//	ReadConsoleOutputCharacter(getconsoleoutputh(), &yourxarrowoverwrite.Char.AsciiChar, 1, yourxarrow, &written);
		//	ReadConsoleOutputAttribute(getconsoleoutputh(), &yourxarrowoverwrite.Attributes, 1, yourxarrow, &written);
		
	
//REDRAW THE SCOPE OVERDRAW POINTS
			setcursor(scoperedrawpt1.X, scoperedrawpt1.Y);
	setcolor(opticcolor);
		printf("%c", SNIPECHAR_CROSS);
		
		setcursor(scoperedrawpt2.X, scoperedrawpt2.Y);
		printf("%c", SNIPECHAR_HLINE);

			//DRAW THE NEW ARROWS

		setcursor(yourxarrow.X, yourxarrow.Y);
		LGREEN;
		printf("%c", SNIPECHAR_DNARROW);
		
		setcursor(youryarrow.X, youryarrow.Y);
		LGREEN;
		printf("%c", SNIPECHAR_LTARROW);

		setcursor(targetxarrow.X, targetxarrow.Y);
		LRED;
		printf("%c", SNIPECHAR_UPARROW);
		
		setcursor(targetyarrow.X, targetyarrow.Y);
		LRED;
		printf("%c", SNIPECHAR_RTARROW);

	


		
		numevents = checkforinput();
		if (numevents > 1)
		{
			for (i = 1; i <= numevents; i++)
			{
				key = getinput(i);
								
					if (key == ' ')
					{
						return CheckSnipeHit(yourxarrow.X - targetxarrow.X, youryarrow.Y - targetyarrow.Y);
					}
					else if (key == VK_LEFT)
					{
						yourxarrowdir = 0;
					}
					else if (key == VK_RIGHT)
					{
						
						yourxarrowdir = 1;
					}
					else if (key == VK_UP)
					{
						
						youryarrowdir = 0;
					}
					else if (key == VK_DOWN)
					{
						youryarrowdir = 1;
					}
					
					
				
				FLUSH;
			
		
			}
		
		clearinputrecords();
			
			
			
		}
			Sleep(SNIPE_MAX_SLEEP - SNIPE_DROP_PER_OCULAR * OcularState + SNIPE_SLEEP_CALMNESS_BONUS * ps->psychskill.psych.calmness);
			EndTimer(&Timer);
			splittime += SplitElapsed(&Timer);
			NextState = GetNewOcularState(ps, splittime);
			if(NextState == SNIPEOCULAR_LOST){
				GRAY;
				showscope();
				Sleep(100);
				DGRAY;
				showscope();
				Sleep(100);
				cls(0);
				return SNIPECODE_OVEROCULAR;
			}
			else if(NextState != OcularState){
				OcularState = NextState;
				SnipeDrawOcularState(OcularState);
			}
		}
}



void showscope()
{
	
	COORD scopecoord;
	COORD consize;
	COORD scopesize;
	
	cls(0);

	consize = getconsize();
	scopesize = getgraphicdimensions(globals.graphicslist, SNIPE_GRAPHIC_ID);
	scopecoord.X = (consize.X / 2) - (scopesize.X / 2) - 2;
	scopecoord.Y = 1;
	printgraphic(globals.graphicslist, scopecoord, SNIPE_GRAPHIC_ID);
    zerocursor();
	
	
}

void flashscope()
{
	fillwithcolor(FOREGROUND_RED  | FOREGROUND_INTENSITY |  BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	Sleep(100);
	fillwithcolor(FOREGROUND_RED  | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
	Sleep(500);
	cls(0);
	
}

int CheckSnipeHit(int xoff, int yoff)
{
if (!xoff)//DEAD ON HORIZ
{
	if(!yoff){
		return SNIPECODE_HEART;
	}
	else if (yoff == -2 || yoff == -1){
		return SNIPECODE_BRAINSTEM;
	}
	else if (yoff == 2 || yoff == 1){
		return SNIPECODE_ABDOMEN;
	}
	else{
		return SNIPECODE_MISS;
	}
}
else if (xoff >= -2 && xoff <= 2)//OFF BY SMALL AMOUNT HORIZ LEFT
{
	if(!yoff){
		return SNIPECODE_CHEST;
	}
	if (yoff == -2 || yoff == -1){
		return SNIPECODE_SHOULDER;
	}
	else if (yoff == 2  || yoff == 1){
		return SNIPECODE_LEG;
	}
	
	else{
		return SNIPECODE_MISS;
	}
}
else if (xoff >= -4 && xoff <= 4) //OFF BY A CONSIDERABLE MARGIN
{
	if(!yoff){
		return SNIPECODE_CHEST_SCRAPE;
	}
	else if (yoff == -2 || yoff == -1){
		return SNIPECODE_ARM;
	}
	else if (yoff == 2  || yoff == 1){
		return SNIPECODE_LEG;
	}
	else{
		return SNIPECODE_MISS;
	}
}
else{
	return SNIPECODE_MISS;
}
}

int reportsnipehit(int hitcode,bool critical)
{
	int vcenter;
	int hcenter;
	int t_hcenter;
	COORD consize;
	char hittext[9][30] = {"MISSED TARGET", "BRAINSTEM SEVERED", "HEART PUNCTURED", "CHEST PIERCED", "ABDOMEN PUNCTURED", "SHOULDER PIERCED", "ARM PIERCED", "LEG PIERCED", "CHEST LACERATED"};
	char* scritical = "C R I T I C A L";
	char* focusloss = "F O C U S   L O S T";
	
	//GET SOME POSITION DATA
	consize = getconsize();
	vcenter = consize.Y / 2;
	hcenter = consize.X / 2;
	
	
	//PRINT THE RESULT
	if (hitcode >= 0 && hitcode <= 9)
	{
		t_hcenter = (hcenter - (strlen(hittext[hitcode]) / 2));
		setcursor(t_hcenter, vcenter);
		RED;
		bufferprints(50, hittext[hitcode], BEEPA);
	}
	else if(hitcode == -1){
		t_hcenter = (hcenter - (strlen(focusloss) / 2));
		setcursor(t_hcenter, vcenter);
		LRED;
		bufferprints(50, focusloss, BEEPC);
	}
	else
	{
		t_hcenter = (hcenter - (strlen("ERROR!!!") / 2));
		setcursor(t_hcenter, vcenter);
		RED;
		bufferprint(50, "ERROR!!!");
	}
	Sleep(1500);
	cls(0);
	Sleep(500);

	if (critical)
	{
		t_hcenter = (hcenter - (strlen(scritical) / 2));
		setcursor(t_hcenter, vcenter);
		RED;
		writestring(scritical);
		Sleep(100);
		LRED;
		setcursor(t_hcenter, vcenter);
		writestring(scritical);
	}
	Sleep(1500);
	if (critical)
	{
		t_hcenter = (hcenter - (strlen(scritical) / 2));
		setcursor(t_hcenter, vcenter);
		RED;
		writestring(scritical);
		Sleep(100);
		cls(0);
		Sleep(500);
	}
	
	return 1;
}