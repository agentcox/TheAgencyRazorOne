#include "agencyinclude.h"


//--------------------------------------------------------------------------------------------
// Function Name - random
//
// Description   - Returns a random number between two numbers INCLUSIVE.
//
// Return Type   - int 
//
// Arguments     - int min ( The minimum. Inclusive. )
//               - int max ( Maximum. Inclusive. )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
int random(int min, int max)
{
	//int i;
//	int r = rand()%50 + 25;
	int val;
	
//	for (i = 0; i < r; i++)
	//{
		val = (rand()%(max-min)) + min;
		//	printf("%d\n", val);
//	}
	return val;
}


//--------------------------------------------------------------------------------------------
// Function Name - bToggle
//
// Description   - Toggles a boolean.
//
// Return Type   - void 
//
// Arguments     - BOOL* Pboolean ( The pointer to the boolean. )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void bToggle(BOOL* Pboolean)
{
	if (*Pboolean == TRUE){
		*Pboolean = FALSE;
		return;
	}
	*Pboolean = TRUE;
}

void bToggle(bool* Pboolean)
{
	if (*Pboolean == true){
		*Pboolean = false;
		return;
	}
	*Pboolean = true;
}


//--------------------------------------------------------------------------------------------
// Function Name - RandomAlphaNumeric
//
// Description   - Returns a random alphabetical character or number
//
// Return Type   - char 
//
//
// Author        - Charles Cox
// Date Modified - 04/24/2000
//--------------------------------------------------------------------------------------------
char RandomAlphaNumeric()
{
	int alpha; //IS IT AN ALPHA OR A NUMBER

	alpha = rand()%2; //1 or 0
	if (alpha){
		return (random(65,91));
	}
	return (random(48, 58));
}

char RandomAlpha()
{
	return (random(65,91));
}


//--------------------------------------------------------------------------------------------
// Function Name - RandomNumeric
//
// Description   - Returns a random numeric digit as a char
//
// Return Type   - char 
//
//
// Author        - Charles Cox
// Date Modified - 04/26/2000
//--------------------------------------------------------------------------------------------
char RandomNumeric()
{
		return (random(48, 58));
}


//--------------------------------------------------------------------------------------------
// Function Name - calibratechrono
//
// Description   - Necessary function to check for time rollovers and adjust accordingly
//
// Return Type   - void 
//
// Arguments     - PCHRONOSTRUCT cs ( The pointer to the chronological structure )
//
// Author        - Charles Cox
// Date Modified - 04/24/2000
//--------------------------------------------------------------------------------------------
void calibratechrono(PCHRONOSTRUCT cs)
{

	/************************************************************
	; SECONDS FIRST
	************************************************************/

	if (cs->second > 59){
		
		cs->minute += ((cs->second) / 60);
		cs->second = cs->second%60;
	}

	/************************************************************
	; MINUTES NEXT
	************************************************************/

	if (cs->minute > 59){
		
		cs->hour+= ((cs->minute) / 60);
		cs->minute = cs->minute%60;
	}

	/************************************************************
	; DEAL WITH DAY ROLLOVERS
	************************************************************/
	if (cs->hour >= 24){
		cs->day += cs->hour / 24;
		cs->hour = cs->hour%24;
		
	}


	/************************************************************
	; DEAL WITH MONTH/YEAR
	************************************************************/

	switch (cs->month){
	case 0:
	case 13:
		cs->month = 1;
		cs->year++;
		calibratechrono(cs); //SET THE MONTH AND RUN AGAIN
		return;
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		if (cs->day > 31){
			cs->month++;
			if (cs->month > 12){
				cs->month = 1;
				cs->year++;
			}
			cs->day = 1;
		}
		break;
	case 2:
			if (cs->day > 29){
			cs->month++;
			if (cs->month > 12){
				cs->month = 1;
				cs->year++;
			}
			cs->day = 1;
		}
			break;

	default:
		if (cs->day > 30){
			cs->month++;
			if (cs->month > 12){
				cs->month = 1;
				cs->year++;
			}
			cs->day = 1;
		}
		break;
	}
	

}


//--------------------------------------------------------------------------------------------
// Function Name - calibratemissionchrono
//
// Description   - Necessary function checks for rollovers (backwards and forwards)
//
// Return Type   - void 
//
// Arguments     - PMISSIONCHRONO mcs ( The pointer to the mission chronographical structure )
//
// Author        - Charles Cox
// Date Modified - 04/24/2000
//--------------------------------------------------------------------------------------------
void calibratemissionchrono(PMISSIONCHRONO mcs)
{

	/************************************************************
	; THE COUNTDOWN DOES NOT CURRENTLY WORK PROPERLY, IT DOES NOT CORRECTLY MODULUS
		TO  SET ROLLOVER TIMES AS IN THE COUNTUP VERSION.
	************************************************************/

	if (mcs->countdown)
	{
		if (mcs->split < 0){
			mcs->seconds -= abs((mcs->split / 100)) +1;
			mcs->split = 99 - abs((mcs->split % 100));
		}
		if (mcs->seconds < 0)
		{
			mcs->minutes -= abs((mcs->seconds / 60)) + 1;
			mcs->seconds = 60 - abs((mcs->seconds%60));
		}
		if (mcs->minutes < 0){
			mcs->hours -= abs((mcs->minutes / 60)) + 1;
			mcs->minutes = 60 - abs((mcs->minutes%60));
		}
	}
	else
	{
		if (mcs->split > 99){
		
			mcs->seconds += mcs->split / 100;
				mcs->split = mcs->split%100;
		}
		if (mcs->seconds > 59)
		{
			mcs->minutes += mcs->seconds / 60;
			mcs->seconds = mcs->seconds% 60;
			
		}
		if (mcs->minutes > 59)
		{
			mcs->hours += mcs->minutes / 60;
			mcs->minutes = mcs->minutes % 60;
		}
	}
}


//--------------------------------------------------------------------------------------------
// Function Name - SPrintFullDateTime
//
// Description   - Prints a full date-time string.
//
// Return Type   - void 
//
// Arguments     - char* timebuf ( The buffer to print to. )
//               - PCHRONOSTRUCT chronostruct ( The chronostruct to get data from. )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void SPrintFullDateTime(char* timebuf, PCHRONOSTRUCT chronostruct)
{
	sprintf(timebuf, "%2.2d/%2.2d/%4.4d - %2.2d%2.2d HOURS", chronostruct->month, chronostruct->day, chronostruct->year, chronostruct->hour, chronostruct->minute);
}


//--------------------------------------------------------------------------------------------
// Function Name - SPrintFullDateTimeSeconds
//
// Description   - Prints a full date-time string PLUS seconds.
//
// Return Type   - void 
//
// Arguments     - char* timebuf ( The buffer to print to )
//               - PCHRONOSTRUCT chronostruct ( The chronostruct to get data from. )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void SPrintFullDateTimeSeconds(char* timebuf, PCHRONOSTRUCT chronostruct)
{
	sprintf(timebuf, "%2.2d/%2.2d/%4.4d - %2.2d%2.2d.%2.2d HOURS", chronostruct->month, chronostruct->day, chronostruct->year, chronostruct->hour, chronostruct->minute, chronostruct->second);
}


//--------------------------------------------------------------------------------------------
// Function Name - SPrintMissionChrono
//
// Description   - Prints a time string from a MissionChrono structure
//
// Return Type   - void 
//
// Arguments     - char* timebuf ( The buffer to print to )
//               - PMISSIONCHRONO mc ( The missionchronograph structure )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void SPrintMissionChrono(char* timebuf, PMISSIONCHRONO mc)
{
	sprintf(timebuf, "%2.2d:%2.2d:%2.2d.%2.2d",  mc->hours, mc->minutes, mc->seconds, mc->split);
}


//--------------------------------------------------------------------------------------------
// Function Name - SPrintChrono
//
// Description   - Prints a time string.
//
// Return Type   - void 
//
// Arguments     - char* timebuf ( The buffer to print to )
//               - PCHRONOSTRUCT chron ( The pointer to the chronostring to get data from. )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void SPrintChrono(char* timebuf, PCHRONOSTRUCT chron)
{
	sprintf(timebuf, "%2.2d:%2.2d.%2.2d", chron->hour, chron->minute, chron->second);
}


//--------------------------------------------------------------------------------------------
// Function Name - MissionChronoTimeUp
//
// Description   - Returns whether or not the mission chronograph has run out.
//
// Return Type   - BOOL 
//
// Arguments     - PMISSIONCHRONO mc ( The mission chronograph to check )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
BOOL MissionChronoTimeUp(PMISSIONCHRONO mc)
{
	if (mc->countdown){ //ONLY IF IT'S COUNTING DOWN WILL WE POSSIBLY RETURN TRUE
		if (mc->hours < 1 && mc->minutes < 1 && mc->seconds < 1 && mc->split < 1){
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}


//--------------------------------------------------------------------------------------------
// Function Name - printwordwrap
//
// Description   - Will print a word wrap to a coordinate system OUTDATED!!!!
//
// Return Type   - void 
//
// Arguments     - char* string ( The string to print )
//               - COORD wrapcoord ( The coordinates to wrap to )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void printwordwrap(char* string, COORD wrapcoord)
{
	int charcounter = 0;
	int numchars = strlen(string);
	char* printchars;

	printchars = (char*) malloc(sizeof(char) *numchars + 1);
	strcpy(printchars, string);

	/************************************************************
	; CHECK IF IT'S ALREADY BROKEN UP!
	************************************************************/

	for (charcounter = 0; charcounter < numchars - 1; charcounter++){
		if (printchars[charcounter] == '\n'){
			printf(printchars);
			free(printchars);
			return;
		}
	}
	charcounter = 0;

	/************************************************************
	; IF NOT, GO AHEAD AND RUN THE WORDWRAPPER.
	************************************************************/

	if (wrapcoord.X > 1){
	wrapcoord.X --;
	}

	if (numchars < wrapcoord.X){
		printf(printchars);
	free(printchars);
		return;
	}

	while (charcounter < numchars + wrapcoord.X){
		charcounter += wrapcoord.X;
		if (charcounter >= numchars){
			break;
		}
		while(!isspace(printchars[charcounter])){
			charcounter--;
		}
		printchars[charcounter] = '\n'; //INSERT THE NEWLINE CHARACTER
	}
	printf(printchars);
	free(printchars);
}


//--------------------------------------------------------------------------------------------
// Function Name - printwordwrapcoordinate
//
// Description   - Better version of wordwrap. Will print inside a box.
//						Does not modify the string.
//
// Return Type   - void 
//
// Arguments     - char* string ( The string to wrap )
//               - COORD wrapcoord ( The coordinates to wrap to. )
//               - COORD printcoord ( The top left of the box to print to. )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void printwordwrapcoordinate(char* string, COORD wrapcoord, COORD printcoord)
{
	int charcounter = 0;
	int numchars = strlen(string);
	char* printchars;

	printchars = (char*) malloc(sizeof(char) *numchars + 1);
	strcpy(printchars, string);


	/************************************************************
	; CHECK IF IT'S ALREADY BROKEN UP!
	************************************************************/

	for (charcounter = 0; charcounter < numchars - 1; charcounter++){
		if (printchars[charcounter] == '\n'){
			setcursor(printcoord.X, printcoord.Y);
			printinrect(printchars, printcoord);
			free(printchars);
			return;
		}
	}
	charcounter = 0;


	/************************************************************
	; IF NOT, GO AHEAD AND RUN THE WORDWRAPPER.
	************************************************************/


	if (wrapcoord.X > 1){
	wrapcoord.X --;
	}

	if (numchars < wrapcoord.X){
		setcursor(printcoord.X, printcoord.Y);
	printinrect(printchars, printcoord);
	free(printchars);
		return;
	}

	while (charcounter < numchars + wrapcoord.X){
		charcounter += wrapcoord.X;
		if (charcounter >= numchars){
			break;
		}
		while(!isspace(printchars[charcounter])){
			charcounter--;
		}
		printchars[charcounter] = '\n'; //INSERT THE NEWLINE CHARACTER
	}
	setcursor(printcoord.X, printcoord.Y);
	printinrect(printchars, printcoord);
	free(printchars);
}


//--------------------------------------------------------------------------------------------
// Function Name - printinrect
//
// Description   - Prints in a rectangle. Private function.
//
// Return Type   - void 
//
// Arguments     - char* string ( The string to print )
//               - COORD startcoord ( The start coordinate )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void printinrect(char* string, COORD startcoord)
{
	int len;
	int i;
	len = strlen(string);
	COORD printcoords = startcoord;
	setcursor(startcoord.X, startcoord.Y);

	for(i = 0; i < len; i++){
		if (string[i] == '\n'){
			printcoords.Y ++;
			printcoords.X = startcoord.X;
			setcursor(printcoords.X, printcoords.Y);
		}
		else{
			printf("%c", string[i]);
		}
	}
}


//--------------------------------------------------------------------------------------------
// Function Name - PrintRandomNumberString
//
// Description   - Will print a random number string of a given amount of digits
//
// Return Type   - void 
//
// Arguments     - int numberofdigits ( The number of digits )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
void PrintRandomNumberString(int numberofdigits)
{
	int i;
	char* digits;

	digits = (char*) malloc(sizeof(char) * numberofdigits+1);
	for (i = 0; i < numberofdigits; i++){
		digits[i] = RandomNumeric();
	}
	digits[i-1] = '\0';

	printf("%s", digits);
	free(digits);
}


//--------------------------------------------------------------------------------------------
// Function Name - RetrieveGreekString
//
// Description   - Retrieves a greek string. Cannot print directly from this function.
//
// Return Type   - char* 
//
// Arguments     - int index ( The index to retrieve )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
char greekstrings[MAX_GREEK_CHARACTERS + 1][MAX_GREEK_LETTERS] = {"Alpha", "Beta", "Gamma", "Delta", "Epsilon", "Zeta", "Eta", "Theta", "Iota", "Kappa", "Lambda", "Mu", "Nu", 
		"Xi", "Omicron", "Pi", "Rho", "Sigma", "Tau", "Phi", "Chi", "Psi", "Omega", "ERROR, O.O.B.!"};
char* RetrieveGreekString(int index)
{
	

	if (index < 0 || index > MAX_GREEK_CHARACTERS){
		return greekstrings[0];
	}
	return greekstrings[index];
}


//--------------------------------------------------------------------------------------------
// Function Name - RandomPercentage
//
// Description   - Will return True or False given a percentage chance.
//
// Return Type   - BOOL 
//
// Arguments     - int percentage ( The percentage chance. )
//
// Author        - Charles Cox
// Date Modified - 05/12/2000
//--------------------------------------------------------------------------------------------
bool RandomPercentage(int percentage)
{
	int randomnum;

	if (percentage >= 100){
		return TRUE;
	}
	if (percentage <= 0){
		return FALSE;
	}

	randomnum = random(1, 100);

	if (randomnum <= percentage){
		return TRUE;
	}
	return FALSE;

}


//--------------------------------------------------------------------------------------------
// Function Name - CheckForObjectTypeInRadius
//
// Description   - Checks for a particular object type in a radius of a cell
//
// Return Type   - int 
//
// Arguments     - PMAP map ( The map to check in )
//               - int objecttype ( The object type to check for )
//               - MAPCELL center ( The center to radiate out from )
//               - int radius ( The radius (half the diameter of the search area) )
//
// Author        - Charles Cox
// Date Modified - 08/06/2000
//--------------------------------------------------------------------------------------------
int CheckForObjectTypeInRadius(PMAP map, int objecttype, MAPCELL center, int radius)
{
	RECT checkrect;
	int totalnumberofobjects = 0;
	int xiteration = 0;
	int yiteration = 0;
	PMAPCELL cell;
	POINT cellrefcoord;

	//DRAWS TWO DIAGONAL LINES OUT FROM THE CENTER IN OPPOSITE DIRECTIONS TO FIND THE TOP LEFT
	//AND BOTTOM RIGHT OF A SQUARE THAT IT THEN ITERATES THRU.

	if (center.xcoord + radius > (map->width - 1)){
		checkrect.right = map->width - 1;
	}
	else{
		checkrect.right = center.xcoord + radius;
	}

	if (center.ycoord + radius > (map->height - 1)){
		checkrect.bottom = map->height - 1;
	}
	else{
		checkrect.bottom = center.ycoord + radius;
	}

	//WE'VE GOT THE BOTTOM LINE DRAWN.
	

	if (center.xcoord - radius < 1){
		checkrect.left = 1;
	}
	else{
		checkrect.left = center.xcoord - radius;
	}

	if (center.ycoord - radius < 1){
		checkrect.top = 1;
	}
	else{
		checkrect.top = center.ycoord - radius;
	}

	//WE'VE GOT THE TOP LINE.

	//ITERATE THRU THE RECTANGLE YOU NOW HAVE.
	for (yiteration = checkrect.top, xiteration = checkrect.left; yiteration < checkrect.bottom && xiteration < checkrect.right; yiteration += 1)
	{
		for (xiteration = checkrect.left; xiteration < checkrect.right; xiteration += 1){
			cellrefcoord.x = xiteration;
			cellrefcoord.y = yiteration;
			cell = FindMapCellByCoordinate(map, cellrefcoord);
			if (cell == NULL){
				return -1;
			}
			if (cell->type == objecttype){
				totalnumberofobjects++;
			}
		}
	}
	return totalnumberofobjects;
	
}

int CapInclusive(int digit, int low, int high)
{
	if(digit < low){
		digit = low;
	}
	else if (digit > high){
		digit = high;
	}
	return digit;
}

int CapExclusive(int digit, int low, int high)
{
	if (digit <= low){
		digit = low +1;
	}
	else if (digit >= high)
	{
		digit = high - 1;
	}
	return digit;
}

int WrapNumber(int digit, int mintowrapto, int maxexclusive)
{

	while(digit >= maxexclusive){
		digit = mintowrapto + (digit - maxexclusive);
	}
	while(digit < mintowrapto){
		digit = maxexclusive - (mintowrapto - digit);
	}
	return digit;
}

double AbsoluteDistance(POINT pointone, POINT pointtwo)
{
	int XPart = (pointtwo.x - pointone.x);
	XPart*=XPart;
	int YPart = (pointtwo.y - pointone.y);
	YPart*=YPart;
	return(sqrt(XPart + YPart));
}

double ReturnZoomFactor(POINT Location, POINT ZoomLocation, int ViewportWidth, int ViewportHeight)
{
	double Avg = ((double)(ViewportWidth + ViewportHeight)) / 2.0;
	double Distance = AbsoluteDistance(Location, ZoomLocation);

	double ZF = (Distance + Avg) / (Avg);
	return ZF;
}

int CompareMissionChrono(PMISSIONCHRONO C1, PMISSIONCHRONO C2)
{
	if(C1->hours != C2->hours){
		return C2->hours - C1->hours;
	}
	if(C1->minutes != C2->minutes){
		return C2->minutes - C1->minutes;
	}
	if(C1->seconds != C2->seconds){
		return C2->seconds - C1->seconds;
	}
	if(C1->split != C2->split){
		return C2->split - C1->split;
	}
	return 0;
}

bool IsPointInRect(RECT Rect, POINT Point)
{
	if(Point.x >= Rect.left && Point.x < Rect.right && Point.y >= Rect.top && Point.y < Rect.bottom){
		return true;
	}
	return false;
}