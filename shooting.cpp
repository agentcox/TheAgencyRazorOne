#include "agencyinclude.h"

#define SHOOTING_SILHOUETTE_WIDTH	77
#define SHOOTING_SILHOUETTE_HEIGHT	22
#define SHOOTING_SILHOUETTE_HITMARKERSNUM	3

/*
//BODY DIAGRAM
#define BODY_HEAD		7
#define BODY_LSHOULDER	8
#define BODY_RSHOULDER	6
#define BODY_TORSO		4
#define BODY_RSIDE		3
#define BODY_LSIDE		5
#define BODY_ABDOMEN	1
#define BODY_LLEG		2
#define BODY_RLEG		0

#define MAX_BODY_PARTS	9
*/

COORD g_ShootingCenters[MAX_BODY_PARTS] = {
	{35, 17}, //0 - RLEG - 0
	{38, 12}, //1 - ABDOMEN - 1
	{42, 17}, //2 - LLEG - 2
	{33, 10}, //3 - RSIDE - 3
	{38,  8}, //4 - TORSO - 4
	{45, 10}, //5 - LSIDE - 5
	{30,  8}, //6 - RSHOULDER - 6
	{39, 2}, //7 - HEAD - 7
	{47, 8}, //8 - LSHOULDER - 8
};


char g_Shooting_HitMarkers[SHOOTING_SILHOUETTE_HITMARKERSNUM] = {'*','@',(char)219};
int g_ShootingHitMarksMatrix[SHOOTING_SILHOUETTE_HEIGHT][SHOOTING_SILHOUETTE_WIDTH]; //the current hits.

GRAPHIC g_CurrentSilhouetteMatrix;


void ShootingClearHitMatrix()
{
	memset(&g_ShootingHitMarksMatrix, 0, sizeof(int) * SHOOTING_SILHOUETTE_HEIGHT * SHOOTING_SILHOUETTE_WIDTH);
}

bool ShootingLoadSilhouetteMatrix(char* datafile)
{
	FILE* fp = fopen(datafile, "r");
	if(!fp)
		return false;
	loadgraphic(fp, &g_CurrentSilhouetteMatrix);
	fclose(fp);
	return true;
}

void ShootingPrintSilhouette(int siltype)
{
	GRAY;
	switch(siltype)
	{
	case SHOOTING_TGTPOS_OPEN:
		printgraphic(globals.graphicslist, globals.zeroed, SHOOTING_SILHOUETTE_CLEAR_ID);
		break;
	case SHOOTING_TGTPOS_FULLCOVER:
		printgraphic(globals.graphicslist, globals.zeroed, SHOOTING_SILHOUETTE_FULLCOVER_ID);
		break;
	case SHOOTING_TGTPOS_PARTIALCOVER:
		printgraphic(globals.graphicslist, globals.zeroed, SHOOTING_SILHOUETTE_PARTIALCOVER_ID);
		break;
	}
	Sleep(500);
}

bool ShootingGetHitFromMatrix(COORD loc, int& outBodyPart, int& outTimesAlreadyHit)
{
		COORD consize = getconsize();

	if (loc.X < 0 || loc.X >= consize.X || loc.Y < 0 || loc.Y > consize.Y)
		return false;
	char foundchar = g_CurrentSilhouetteMatrix.graphic[(loc.Y * SHOOTING_SILHOUETTE_WIDTH) + loc.X];
	char foundcharstr[2] = "\0";
	if(foundchar == ' ' || foundchar == '\0')
	{
		outBodyPart = -1;
	}
	else
	{
		foundcharstr[0] = foundchar;
		outBodyPart = atoi(foundcharstr);
	}
	outTimesAlreadyHit = (g_ShootingHitMarksMatrix[loc.Y][loc.X])++;
	return true;
}

/*
THIS GIVES A GOOD IDEA AS TO HOW THE SHOOTING SHOULD PROGRESS - THE SHOTS SHOULD BE MORE OR LESS BASED OFF
STATS AND DECISIONS, AND PERPETUATE FOR HOWEVER MANY SHOTS THE PLAYER CHOOSES TO USE. THE singleshot BOOL IS
FOR CINEMATIC EFFECT. A SINGLE SHOT WILL FIRE, WAIT, AND THEN REGISTER A HIT OR MISS - JUST SLOWER TO GET THE EFFECT.
FULL AUTO OR BURST WILL TEAR INTO THE TARGET (OR NOT) RIGHT AWAY.
*/

/*
AS A NOTE, SHOTGUN BURSTS WILL SPREAD USING MULTIPLE "BULLETS" AS IT WERE, BUT THE DAMAGE IS ONLY 1/5TH
(OR 1/WHATEVER THE PELLET # IS)
  */

void ShootSequenceDemoRandom(bool singleshot)
{
	COORD shotloc;
	int bodypart;
	int timeshit;
	int shotstofire = singleshot?1:random(31,32);
	

	ShootingClearHitMatrix();
	ShootingLoadSilhouetteMatrix("data\\sil_partialcover.dat");
	ShootingPrintSilhouette(SHOOTING_TGTPOS_PARTIALCOVER);

	for(int i = 0; i < shotstofire; i++)
	{
		shotloc = ShootingGetHitLocFromAccuracy(BODY_HEAD, 80, 5);
		
		playMIXsound("sounds\\weapons\\rifle2.wav");
		Sleep(singleshot?500:60); //100 / num rds turn?

		bool onscreen = ShootingGetHitFromMatrix(shotloc, bodypart, timeshit);
		if(bodypart == -1 || !onscreen)
		{
			playRicochet(SOUND_MODE_MIX);
			GRAY;
		}
		else
		{
			playMIXsound("sounds\\hits\\bullethit.wav");
			LRED;
		}
		if(onscreen)
			ShootingPrintHitGraphic(shotloc, timeshit++);
	}
	waitforkey();
}

COORD ShootingGetHitLocFromAccuracy(int bodyloc, int accuracypercentage, int directhitpercentage)
{
	COORD center = g_ShootingCenters[bodyloc];
	bool directhit = RandomPercentage(directhitpercentage);
	float widthoff = (float)SHOOTING_SILHOUETTE_WIDTH; //adjust these if we seem to be too far off most of the time.
	float heightoff = (float)SHOOTING_SILHOUETTE_HEIGHT;
	float damping = 2.0f; //accuracy loss % damping



//	if(directhit) //nice shot.
//		return center;

	//else we need to calculate how far "off" we can go.
	float wpct = ((float)random(0,100-accuracypercentage))/100.0f;
	float hpct = ((float)random(0,100-accuracypercentage))/100.0f;
	widthoff *= accuracypercentage?((wpct)/damping):1.0f;
	heightoff *= accuracypercentage?((hpct)/damping):1.0f;

	if(RandomPercentage(50))
		widthoff *= -1.0;
	if(RandomPercentage(50))
		heightoff *= -1.0;

	COORD retcoord;
	retcoord.X = center.X + (short)widthoff;
	retcoord.Y = center.Y + (short)heightoff;
	return retcoord;
}

bool ShootingPrintHitGraphic(COORD location, int timeshit)
{
	COORD consize = getconsize();

	if (location.X < 0 || location.X >= consize.X || location.Y < 0 || location.Y > consize.Y)
		return false;

	if(timeshit >= SHOOTING_SILHOUETTE_HITMARKERSNUM)
	{
		timeshit = SHOOTING_SILHOUETTE_HITMARKERSNUM - 1;
	}
	char toprint = g_Shooting_HitMarkers[timeshit];
	setcursor(location.X, location.Y);
	writechar(toprint);
	return true;
}