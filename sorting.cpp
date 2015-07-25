#include "agencyinclude.h"
#include "guns.h"

int SortCompareWeaponArmory(PPOTYPE WOne, PPOTYPE WTwo)
{

	if(!WOne ||!WTwo){
		return 0;
	}
	if(WOne->weapon.filingtype != WTwo->weapon.filingtype)
	{
		return WTwo->weapon.filingtype - WOne->weapon.filingtype;
	}
	else if(WOne->weapon.classtype != WTwo->weapon.classtype)
	{
		return WTwo->weapon.classtype - WOne->weapon.classtype;
	}
	else return (strcmp(WTwo->weapon.longname, WOne->weapon.longname));
}

void SwapWeaponIndex(PPOTYPE WOne, PPOTYPE WTwo)
{
	int temp;
if(WOne && WTwo){
	temp = WTwo->weapon.index;
	WTwo->weapon.index = WOne->weapon.index;
	WOne->weapon.index = temp;
}
}

void SwapWeaponID(PPOTYPE WOne, PPOTYPE WTwo)
{
	int temp;

	if(WOne && WTwo){
	temp = WTwo->weapon.id;
	WTwo->weapon.id = WOne->weapon.id;
	WOne->weapon.id = temp;
	}
}

void SortArmoryWeapons(PLIST armorylist)
{
sortList(armorylist, SortCompareWeaponArmory, NULL, CompareWeaponID);
ReorderWeaponArmoryIDs(armorylist);
}

void SortPersonalWeapons(PLIST weaponlist)
{
	sortList(weaponlist, SortCompareWeaponArmory, SwapWeaponIndex, CompareWeaponIndex);
}

void ReorderWeaponArmoryIDs(PLIST armorylist)
{

	int counter = 1;
	if(!armorylist){
		return;
	}
	PLIST walker = armorylist;

	for(walker->current = walker->head; walker->current != NULL; walker->current = walker->current->nextnode)
	{
		walker->current->object.weapon.id = counter;
		counter++;
	}
}
