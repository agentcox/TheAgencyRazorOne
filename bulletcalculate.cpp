#include "agencyinclude.h"


#define BULLETVELMODIFY_WALL		0
#define BULLETVELMODIFY_VEHICLE		0
#define BULLETVELMODIFY_OBSTACLE	0
#define BULLETVELMODIFY_PERSON		0

#define BULLET_STOP					1
#define BULLET_CONTINUE				0

#define BULLET_STOPPEDBEFORETARGET	1
#define BULLET_HITTARGET			0

#define X_STEP						0
#define Y_STEP						1


bool CalculateVisionTrajectory(PMAP map, PMAPCELL SourceCell, PMAPCELL DestCell)
{
	long x0 = SourceCell->xcoord;
	long y0 = SourceCell->ycoord;
	long x1 = DestCell->xcoord;
	long y1 = DestCell->ycoord;
	
	
	
	///////////BEGIN BRESENHAM SUBSTITUTIONS FOR HORIZ/VERT LINES
	
	if (!(y1-y0)) //IF THE LINE IS HORIZONTAL
	{
		if (x1 >= x0)
		{
			for (int i = x0; i <= x1; i++) // IF X1 IS GREATER THAN X0
			{
				//DO SOMETHING HERE!
				//POINT IS (i, Y0)
				if(i == x1){
					return true;
				}
			if(CalculateVisionTrajectoryHitAtPoint(map, i, y0)){
				return false;
			}
			}
		}
		else //IF X0 IS GREATER THAN X1
		{
			for (int i = x0; i >= x1; i--)
			{
				//DO SOMETHING HERE!
				//POINT IS (i, Y0)
				if(i == x1){
					return true;
				}
				if(CalculateVisionTrajectoryHitAtPoint(map, i, y0)){
				return false;
			}
			}
		}
		return true;
	}
	
	if (!(x1-x0)) //IF THE LINE IS VERTICAL
	{
		if (y1 > y0) //IF Y1 IS ABOVE Y0
		{
			for (int i = y0; i <= y1; i++)
			{
				//DO SOMETHING HERE!
				//POINT IS (x0, i)
				if(i == y1){
					return true;
				}
				if(CalculateVisionTrajectoryHitAtPoint(map, x0, i)){
				return false;
			}
			}
		}
		else //IF Y0 IS ABOVE Y1
		{
			for (int i = y0; i >= y1; i--)
			{
				//DO SOMETHING HERE!
				//POINT IS (x0, i)
				if(i == y1){
					return true;
				}
				if(CalculateVisionTrajectoryHitAtPoint(map, x0, i)){
				return false;
			}
			}
		}
		return true;
	}
	
	////END BRESENHAM SUBSTITUTIONS
	
	////BEGIN BRESENHAM ALGORITHM
	
	///WE HAVE TO DETERMINE WHICH DIRECTION TO GO WITH THIS ALGORITHM.
	///THERE ARE FOUR POSSIBILITIES FROM HERE:
	///POSITIVE SLOPE > 1, POSITIVE SLOPE < 1, NEGATIVE SLOPE < -1, NEGATIVE SLOPE > -1
	
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	
	int xstep = 1;
	int ystep = 1;
	int x = x0;
	int y = y0;
	
	//WE, OF COURSE, WANT TO START WITH THE SAME PIXEL. X0, Y0.
		
	//IF WE HAVE CASE : NEGATIVE SLOPE > -1 WE CAN CHANGE THIS TO A POSITIVE SLOPE WITH
	//SOME QUICK MATH.
	xstep = (x1 < x0 ? -1 : 1);
	ystep = (y1 < y0 ? -1 : 1);
	
	//////IF 0 < SLOPE < 1//////
	if (dx > dy) 
	{
		int dE = 2 * dy;
		int dNE = dE - (dx * 2);
		int d = dE - dx;
		
		for (;dx >= 0; dx--)
		{
			if (d <= 0)
			{
				d += dE;
				x+=xstep;
			}
			
			else
			{
				d += dNE;
				x+=xstep;
				y+=ystep;
			}
			if(x == x1 && y== y1){
				return true;
			}
			//DO SOMETHING HERE!
				//POINT IS (x, y)
			if(CalculateVisionTrajectoryHitAtPoint(map, x, y)){
				return false;
			}
		}
		return true;
	}

	////IF SLOPE > 1/////
	else
	{
		int dN = dx * 2;
		int dNE = dN - (dy * 2);
		int d = dN - dy;

		for(;dy >= 0; dy--)
		{
			if (d <= 0)
			{
				d += dN;
				y+=ystep;
			}
			
			else
			{
				d += dNE;
				x+=xstep;
				y+=ystep;
			}
						if(x == x1 && y== y1){
				return true;
			}
			//DO SOMETHING HERE!
				//POINT IS (x, y)
			if(CalculateVisionTrajectoryHitAtPoint(map, x, y)){
				return false;
			}
		}
		return true;
	}
	return true;
	
	
}

int GetFirstObstructionBetweenPoints(PMAP map, PMAPCELL SourceCell, PMAPCELL DestCell, POINT* Out) //RETURNS TYPE.
{
	long x0 = SourceCell->xcoord;
	long y0 = SourceCell->ycoord;
	long x1 = DestCell->xcoord;
	long y1 = DestCell->ycoord;
	int Found;
	
	
	
	///////////BEGIN BRESENHAM SUBSTITUTIONS FOR HORIZ/VERT LINES
	
	if (!(y1-y0)) //IF THE LINE IS HORIZONTAL
	{
		if (x1 >= x0)
		{
			for (int i = x0; i <= x1; i++) // IF X1 IS GREATER THAN X0
			{
				//DO SOMETHING HERE!
				//POINT IS (i, Y0)
				if(i == x1){
					return RAYCAST_BLOCK_NONE;
				}
				Found = GetObstructionAtPoint(map, i, y0);
				if(Found != RAYCAST_BLOCK_NONE){
					Out->x = i;
					Out->y = y0;
					return Found;
				}
			}
		}
		else //IF X0 IS GREATER THAN X1
		{
			for (int i = x0; i >= x1; i--)
			{
				//DO SOMETHING HERE!
				//POINT IS (i, Y0)
				if(i == x1){
					return RAYCAST_BLOCK_NONE;
				}
				Found = GetObstructionAtPoint(map, i, y0);
				if(Found != RAYCAST_BLOCK_NONE){
					Out->x = i;
					Out->y = y0;
					return Found;
				}
			}
		}
		return RAYCAST_BLOCK_NONE;
	}
	
	if (!(x1-x0)) //IF THE LINE IS VERTICAL
	{
		if (y1 > y0) //IF Y1 IS ABOVE Y0
		{
			for (int i = y0; i <= y1; i++)
			{
				//DO SOMETHING HERE!
				//POINT IS (x0, i)
				if(i == y1){
					return RAYCAST_BLOCK_NONE;
				}
				Found = GetObstructionAtPoint(map, x0, i);
				if(Found != RAYCAST_BLOCK_NONE){
					Out->x = x0;
					Out->y = i;
					return Found;
				}
			}
		}
		else //IF Y0 IS ABOVE Y1
		{
			for (int i = y0; i >= y1; i--)
			{
				//DO SOMETHING HERE!
				//POINT IS (x0, i)
				if(i == y1){
					return RAYCAST_BLOCK_NONE;
				}
				Found = GetObstructionAtPoint(map, x0, i);
				if(Found != RAYCAST_BLOCK_NONE){
					Out->x = x0;
					Out->y = i;
					return Found;
				}
			}
		}
		return true;
	}
	
	////END BRESENHAM SUBSTITUTIONS
	
	////BEGIN BRESENHAM ALGORITHM
	
	///WE HAVE TO DETERMINE WHICH DIRECTION TO GO WITH THIS ALGORITHM.
	///THERE ARE FOUR POSSIBILITIES FROM HERE:
	///POSITIVE SLOPE > 1, POSITIVE SLOPE < 1, NEGATIVE SLOPE < -1, NEGATIVE SLOPE > -1
	
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	
	int xstep = 1;
	int ystep = 1;
	int x = x0;
	int y = y0;
	
	//WE, OF COURSE, WANT TO START WITH THE SAME PIXEL. X0, Y0.
		
	//IF WE HAVE CASE : NEGATIVE SLOPE > -1 WE CAN CHANGE THIS TO A POSITIVE SLOPE WITH
	//SOME QUICK MATH.
	xstep = (x1 < x0 ? -1 : 1);
	ystep = (y1 < y0 ? -1 : 1);
	
	//////IF 0 < SLOPE < 1//////
	if (dx > dy) 
	{
		int dE = 2 * dy;
		int dNE = dE - (dx * 2);
		int d = dE - dx;
		
		for (;dx >= 0; dx--)
		{
			if (d <= 0)
			{
				d += dE;
				x+=xstep;
			}
			
			else
			{
				d += dNE;
				x+=xstep;
				y+=ystep;
			}
			if(x == x1 && y== y1){
				return RAYCAST_BLOCK_NONE;
			}
			//DO SOMETHING HERE!
				//POINT IS (x, y)
			Found = GetObstructionAtPoint(map, x, y);
				if(Found != RAYCAST_BLOCK_NONE){
					Out->x = x;
					Out->y = y;
					return Found;
				}
		}
		return RAYCAST_BLOCK_NONE;
	}

	////IF SLOPE > 1/////
	else
	{
		int dN = dx * 2;
		int dNE = dN - (dy * 2);
		int d = dN - dy;

		for(;dy >= 0; dy--)
		{
			if (d <= 0)
			{
				d += dN;
				y+=ystep;
			}
			
			else
			{
				d += dNE;
				x+=xstep;
				y+=ystep;
			}
						if(x == x1 && y== y1){
				return RAYCAST_BLOCK_NONE;
			}
			//DO SOMETHING HERE!
				//POINT IS (x, y)
				Found = GetObstructionAtPoint(map, x, y);
				if(Found != RAYCAST_BLOCK_NONE){
					Out->x = x;
					Out->y = y;
					return Found;
				}
		}
		return RAYCAST_BLOCK_NONE;
	}
	return RAYCAST_BLOCK_NONE;
	
}

POINT GetPointAlongLine(PMAPCELL SourceCell, PMAPCELL DestCell, int StepsIntoLine)
{
	long x0 = SourceCell->xcoord;
	long y0 = SourceCell->ycoord;
	long x1 = DestCell->xcoord;
	long y1 = DestCell->ycoord;
	int Steps = 0;
	POINT Out;
	Out.x = x0;
	Out.y = y0;
	
	
	
	///////////BEGIN BRESENHAM SUBSTITUTIONS FOR HORIZ/VERT LINES
	
	if (!(y1-y0)) //IF THE LINE IS HORIZONTAL
	{
		if (x1 >= x0)
		{
			for (int i = x0; i <= x1; i++, Steps++) // IF X1 IS GREATER THAN X0
			{
				//DO SOMETHING HERE!
				//POINT IS (i, Y0)
				Out.x = i;
				Out.y = y0;
				if(i == x1){
					return Out;
				}
				if(Steps == StepsIntoLine)
				return Out;
			}
		}
		else //IF X0 IS GREATER THAN X1
		{
			for (int i = x0; i >= x1; i--, Steps++)
			{
				//DO SOMETHING HERE!
				//POINT IS (i, Y0)
				Out.x = i;
				Out.y = y0;
				if(i == x1){
					return Out;
				}
				if(Steps == StepsIntoLine)
				return Out;
			}
		}
		return Out;
	}
	
	if (!(x1-x0)) //IF THE LINE IS VERTICAL
	{
		if (y1 > y0) //IF Y1 IS ABOVE Y0
		{
			for (int i = y0; i <= y1; i++, Steps++)
			{
				//DO SOMETHING HERE!
				//POINT IS (x0, i)
				Out.x = x0;
				Out.y = i;
				if(i == y1){
					return Out;
				}
				if(Steps == StepsIntoLine)
				return Out;
			}
		}
		else //IF Y0 IS ABOVE Y1
		{
			for (int i = y0; i >= y1; i--, Steps++)
			{
				//DO SOMETHING HERE!
				//POINT IS (x0, i)
				Out.x = x0;
				Out.y = i;
				if(i == y1){
					return Out;
				}
				if(Steps == StepsIntoLine)
				return Out;
			}
		}
		return Out;
	}
	
	////END BRESENHAM SUBSTITUTIONS
	
	////BEGIN BRESENHAM ALGORITHM
	
	///WE HAVE TO DETERMINE WHICH DIRECTION TO GO WITH THIS ALGORITHM.
	///THERE ARE FOUR POSSIBILITIES FROM HERE:
	///POSITIVE SLOPE > 1, POSITIVE SLOPE < 1, NEGATIVE SLOPE < -1, NEGATIVE SLOPE > -1
	
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	
	int xstep = 1;
	int ystep = 1;
	int x = x0;
	int y = y0;
	
	//WE, OF COURSE, WANT TO START WITH THE SAME PIXEL. X0, Y0.
		
	//IF WE HAVE CASE : NEGATIVE SLOPE > -1 WE CAN CHANGE THIS TO A POSITIVE SLOPE WITH
	//SOME QUICK MATH.
	xstep = (x1 < x0 ? -1 : 1);
	ystep = (y1 < y0 ? -1 : 1);
	
	//////IF 0 < SLOPE < 1//////
	if (dx > dy) 
	{
		int dE = 2 * dy;
		int dNE = dE - (dx * 2);
		int d = dE - dx;
		
		for (;dx >= 0; dx--, Steps++)
		{
			if (d <= 0)
			{
				d += dE;
				x+=xstep;
			}
			
			else
			{
				d += dNE;
				x+=xstep;
				y+=ystep;
			}
			Out.x = x;
			Out.y = y;
			if(x == x1 && y== y1 || Steps == StepsIntoLine){
				return Out;
			}
			//DO SOMETHING HERE!
				//POINT IS (x, y)
		}
		return Out;
	}

	////IF SLOPE > 1/////
	else
	{
		int dN = dx * 2;
		int dNE = dN - (dy * 2);
		int d = dN - dy;

		for(;dy >= 0; dy--)
		{
			if (d <= 0)
			{
				d += dN;
				y+=ystep;
			}
			
			else
			{
				d += dNE;
				x+=xstep;
				y+=ystep;
			}
			Out.x = x;
			Out.y = y;
			if(x == x1 && y== y1 || Steps == StepsIntoLine){
				return Out;
			}
			//DO SOMETHING HERE!
				//POINT IS (x, y)
		}
		return Out;
	}
	return Out;
	
}

int GetObstructionAtPoint(PMAP map, int x, int y)
{
	return GetRayCastBlocker(map, x, y);
}

int GetRayCastBlocker(PMAP map, int x, int y)
{
	POINT blah;
	blah.x = x;
	blah.y = y;
	return GetRayCastBlocker(map, blah);
}

int GetRayCastBlocker(PMAP map, POINT celltocheck)
{
	PMAPCELL Cell = FindMapCellByCoordinate(map, celltocheck);
	if(!Cell){
		return RAYCAST_BLOCK_OUTOFBOUNDS;
	}
	switch(Cell->type)
	{
	case BUILDING_SYM_BLCORNER:
	case BUILDING_SYM_TRCORNER:
	case BUILDING_SYM_BRCORNER:
	case BUILDING_SYM_TLCORNER:
		return RAYCAST_BLOCK_CORNER;
	case BUILDING_SYM_HDOOR:
	case BUILDING_SYM_VDOOR:
		return RAYCAST_BLOCK_DOOR;
	case BUILDING_SYM_VWALL:
	case BUILDING_SYM_HWALL:
		return RAYCAST_BLOCK_WALL;
	case OBJECT_SYM_FENCE:
		return RAYCAST_BLOCK_OUTOFBOUNDS;
	case OBJECT_SYM_LOOBSTRUCT:
		return RAYCAST_BLOCK_LOOBS;
	case OBJECT_SYM_HIOBSTRUCT:
		return RAYCAST_BLOCK_HIOBS;
	}
	PVEHICLE Vehicle = GetMapCellVehicle(Cell);
	if(Vehicle){
		return RAYCAST_BLOCK_VEHICLE;
	}
	PENEMY Enemy = GetMapCellEnemy(Cell);
	if(Enemy){
		return RAYCAST_BLOCK_HUMAN;
	}
	PDOOR Door = GetDoorInfoFromCell(Cell);
	if(Door){
		if(!Door->isopen){
		return RAYCAST_BLOCK_DOOR;
		}
	}
	return RAYCAST_BLOCK_NONE;
}

bool DoesRayCastBlockerBlockVision(int RayCastBlocker)
{
	switch(RayCastBlocker)
	{
	case RAYCAST_BLOCK_NONE:
	case RAYCAST_BLOCK_LOOBS:
	case RAYCAST_BLOCK_HUMAN:
		return false;
	default:
		return true;
	}
}

bool CalculateVisionTrajectoryHitAtPoint(PMAP map, int x, int y)
{
	POINT Pt;
	Pt.x = x; Pt.y = y;
	PMAPCELL Cell = FindMapCellByCoordinate(map, Pt);
	if(!Cell){
		return true;
	}
	return DoesRayCastBlockerBlockVision(GetRayCastBlocker(map, Pt));
}

bool IsTypeOfObjectWithinVisionRange(int ObjectType, PMAP map, PPSYCHSKILLSTRUCT viewerpsychskill, PMAPCELL SourceCell, PMAPCELL DestCell, bool UsingNvg, bool UsingTas, bool InEnvironmentalHazard)
{
	bool Nvg = UsingNvg;
	bool Tas = UsingTas;
	
	
	int RealMaxRange = (((viewerpsychskill->psych.intelligence + viewerpsychskill->psych.reflex)/2) * MAX_VIEW_RANGE) / MAX_PSYCH;
	if(Nvg){
		RealMaxRange += 50;
	}
	if(Tas){
		RealMaxRange += 20;
	}

	if(InEnvironmentalHazard){
		RealMaxRange = 3;
	}
	
	POINT ptOne;
	ptOne.x = SourceCell->xcoord;
	ptOne.y = SourceCell->ycoord;
	POINT ptTwo;
	ptTwo.x = DestCell->xcoord;
	ptTwo.y = DestCell->ycoord;
	int DistanceToTarget;
	DistanceToTarget = (int)AbsoluteDistance(ptOne, ptTwo);
	if(DistanceToTarget > RealMaxRange){
		return false;
	}
	double Ratio;
	int OurVisionDistance;
	
	switch(ObjectType)
	{
	case RAYCAST_OBJECT_HIOBS:
		Ratio = RAYCAST_HIOBS_RANGERATIO;
		break;
	case RAYCAST_OBJECT_LOOBS:
		Ratio = RAYCAST_LOOBS_RANGERATIO;
		break;
	case RAYCAST_OBJECT_VEHICLES:
		Ratio = RAYCAST_VEHICLES_RANGERATIO;
		break;
	case RAYCAST_OBJECT_LIVEHUMANS:
		Ratio = RAYCAST_LIVEHUMANS_RANGERATIO;
		if(Tas){
			Ratio += RAYCAST_TAS_BONUS_LIVEHUMANS;
		}
		break;
	case RAYCAST_OBJECT_DEADHUMANS:
		Ratio = RAYCAST_DEADHUMANS_RANGERATIO;
		break;
	case RAYCAST_OBJECT_GIZMOS:
		Ratio = RAYCAST_GIZMOS_RANGERATIO;
		if(Nvg){
			Ratio += RAYCAST_NVG_BONUS_GIZMOS;
		}
		break;
	case RAYCAST_OBJECT_ITEMS:
		Ratio = RAYCAST_ITEMS_RANGERATIO;
		if(Nvg){
			Ratio += RAYCAST_NVG_BONUS_ITEMS;
		}
		break;
	default:
		Ratio = 1.00;
	}
	OurVisionDistance = (int)((double)RealMaxRange * Ratio);
	
	
	if(OurVisionDistance >= DistanceToTarget){
		return true;
	}
	return false;
	
}