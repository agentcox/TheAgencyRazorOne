#define LIS_MAX_RINGPOINTS 42
#define LIS_X_OFFSET 5
#define LIS_RING_WIDTH 3

typedef struct _LISPOINT
{
	int point; //THE ACTUAL POINT INDEX.
	COORD relativedrawcoord; //WHERE TO DRAW IT IN RELATIVE RING COORDS.
	BOOL blocked; //IS IT BLOCKED?
}LISPOINT, *PLISPOINT;

typedef struct _LISRING
{
	int ringnumber;
	int pinpoint; //WHERE THE PIN IS CURRENTLY LOCATED
	int xoffset;
	PLISPOINT points;
}LISRING, *PLISRING;

typedef struct _LISSYSTEM
{
	PLISRING rings;
	int currentring;
	int numberofrings;
	int pointsperring;
	MISSIONCHRONO mchron;
}LISSYSTEM, *PLISSYSTEM;