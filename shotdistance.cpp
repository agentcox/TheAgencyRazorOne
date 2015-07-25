#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <float.h>

float ShotDistance(POINT, POINT);

void main()
{
	POINT origincoord;
	POINT targetcoord;
	float distancepersquare;
	float finaldistance;

	printf("How many feet in each square? Decimal OK >");
	scanf("%f", &distancepersquare);
	printf("Hey, enter Origin X Value >");
	scanf("%d", &origincoord.x);
	printf("Hey, enter Origin Y Value >");
	scanf("%d", &origincoord.y);
	printf("Hey, enter Target X Value >");
	scanf("%d", &targetcoord.x);
	printf("Hey, enter Target Y Value >");
	scanf("%d", &targetcoord.y);
	finaldistance = (ShotDistance(origincoord, targetcoord) * distancepersquare);
	printf("\nShot Distance = %2.2f feet", finaldistance);
	printf("\nPress A Key to Quit.");
	getchar();
}
float ShotDistance(POINT origin, POINT target)
{
int xdistance;
int ydistance;
xdistance = target.x - origin.x;
ydistance = target.y - origin.y;

return ((float)sqrt((xdistance*xdistance) + (ydistance*ydistance)));
}