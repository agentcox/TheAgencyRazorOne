#include "agencyinclude.h"

int escape(PPLAYER ps, PMAP map, PVEHICLE vs)
{
	return SUCCESS;
}

void GenerateVehicle(PVEHICLE vs)
{

	vs->type = random(0,3);
	vs->wheelsleft = 4;


	for (int i = 0; i < MAX_VEHICLE_BRAKELINES; i++){
	vs->brakelines[i] = TRUE;
	}

	for (i = 0; i < MAX_VEHICLE_WINDOWS; i++){
	vs->windows[i] = TRUE;
	}

	vs->armorlevel = random(0, 4);
	vs->hitpoints = 100;
	vs->rigged = VEHICLE_NOTRIGGED;
	vs->gasjellied = VEHICLE_GAS_OK;
	vs->leakinggas = VEHICLE_NOTLEAKING;
//	vs->x = loc.X;
//	vs->y = loc.Y;
	return;
}


void VehiclePrintout(PVEHICLE vs)
{
		char windows[6][60] = {"Front Windshield", "Driver Side Front", "Passenger Side Front", "Driver Side Back", "Passenger Side Back", "Rear Windshield"};
		char brakelines[4][30] = {"Front Left", "Front Right", "Rear Left", "Rear Right"};

		cls();
	LRED;
	printf("Vehicle Status Report:\n");
	LGREEN;
	printf("GENERAL STATUS\n");
	LCYAN;
	printf("Vehicle Type - ");
	LYELLOW;
		Sleep(NUMBERREPORT_SLEEP);
	switch (vs->type)
	{
	case VEHICLE_VAN:
		printf("Van\n");
		break;
	case VEHICLE_TRUCK:
		printf("Truck\n");
		break;
	case VEHICLE_CAR:
		printf("Car\n");
		break;
	default:
		printf("UNKNOWN TYPE! (YOU SUCK!)\n");
		break;
	}
	onebeepa();

	Sleep(NUMBERREPORT_SLEEP);
	LCYAN;
	printf("Vehicle Armor Plating - ");
	LYELLOW;
		Sleep(NUMBERREPORT_SLEEP);
	switch(vs->armorlevel)
	{
	case 0:
		printf("None\n");
		break;
	case 1:
		printf("Level I\n");
		break;
	case 2:
		printf("Level II\n");
		break;
	case 3:
		printf("Level III\n");
		break;
	case 4:
		printf("Level IV\n");
		break;
	default:
		printf("UNKNOWN QUALITY (YOU SUCK!)\n");
		break;
	}
	onebeepa();

	
	Sleep(NUMBERREPORT_SLEEP);
	LCYAN;
	printf("Vehicle Critical Fuel Tank Integrity (Total 100) - ");
	Sleep(NUMBERREPORT_SLEEP);
	LYELLOW;
	printf("%d\n", vs->hitpoints);
	onebeepa();
Sleep(NUMBERREPORT_SLEEP);

LCYAN;
printf("Fuel Tank Puncture Test - ");
if (vs->leakinggas){
	LRED;
	printf("FUEL TANK IS LEAKING\n");
}
else{
	LYELLOW;
	printf("FUEL TANK INTEGRITY OK\n");
}
twobeeps();

	Sleep(NUMBERREPORT_SLEEP);
	LCYAN;
	printf("Window Seals Check\n");
for (int i = 0; i < MAX_VEHICLE_WINDOWS; i++)
{
	LBLUE;
	printf("\t%s   \t-", windows[i]);
	Sleep(NUMBERREPORT_SLEEP);
	if (!CheckVehicleWindowStatus(vs, i)){
		LRED;
		printf("SEAL INTEGRITY BROKEN");
	}
	else{
		LYELLOW;
		printf("SEAL INTEGRITY PASSED");
	}
	onebeepa();
	printf("\n");
}

Sleep(NUMBERREPORT_SLEEP);
LCYAN;
printf("Tire Integrity Check - ");
Sleep(NUMBERREPORT_SLEEP);
if (vs->wheelsleft < MAX_VEHICLE_WHEELS){
	LRED;
	printf("Severe Underpressure In %d Wheels", MAX_VEHICLE_WHEELS - vs->wheelsleft);
}
else{
	LYELLOW;
	printf("All Tires Are Correctly Pressured");
}
twobeeps();


printf("\n");


	LGREEN;
	printf("SABOTAGE SCAN\n");
	LCYAN;
	printf("- Checking for Explosives");
	bufferprint(50, ".......");
	Sleep(NUMBERREPORT_SLEEP);
	if (vs->rigged){
		LRED;
		printf("VEHICLE HAS BEEN WIRED WITH C4");
	}
	else{
		LYELLOW;
		printf("EXPLOSIVES NOT DETECTED");
	}
	twobeepsmod();
	LCYAN;
	printf("\n- Checking for Fuel Composition");
		bufferprint(50, ".......");
Sleep(NUMBERREPORT_SLEEP);
	if (vs->gasjellied){
		LRED;
		printf("FUEL DISINTEGRATED");
	}
	else{
		LYELLOW;
		printf("FUEL COMPOSITION OK");
	}
	twobeepsmod();

	LCYAN;
		printf("\n- Checking for Brake Integrity\n");
Sleep(NUMBERREPORT_SLEEP);
for (i = 0; i < MAX_VEHICLE_BRAKELINES; i++)
{
	LBLUE;
	printf("\t%s   \t-", brakelines[i]);
	Sleep(NUMBERREPORT_SLEEP);
	if (!CheckVehicleBrakeStatus(vs, i)){
		LRED;
		printf("ZERO BRAKE PRESSURE READING");
	}
	else{
		LYELLOW;
		printf("BRAKE PRESSURE ACCEPTABLE");
	}
	onebeepb();
	

	printf("\n");
}

printf("\n");
	LRED;
	bufferprints(20, "REPORT COMPLETE --  REPORT COMPLETE  --  REPORT COMPLETE  --  REPORT COMPLETE", TYPEWRITER);


}

BOOL CheckVehicleWindowStatus(PVEHICLE vs, int index)
{
	return vs->windows[index];
}

BOOL CheckVehicleBrakeStatus(PVEHICLE vs, int index)
{
	return vs->brakelines[index];
}

int CompareVehicleID(PPOTYPE itm1, PPOTYPE itm2)
{
		if(itm1->vehicle.ID == itm2->vehicle.ID){
		return 0;
	}
	return 1;
}