#include "agencyinclude.h"
#include "guns.h"

void genericPlaySound(char* toplay, int soundmode)
{
	switch(soundmode)
	{
	case SOUND_MODE_SYNC:
		playSYNCsound(toplay);
			break;
	case SOUND_MODE_ASYNC:
		playASYNCsound(toplay);
			break;
	case SOUND_MODE_MIX:
		playMIXsound(toplay);
			break;
	}
}

void playRoundDump(PROUND round, bool multiple, int soundmode)
{
	char* toplay;
	if(multiple)
		toplay = DUMP_GENERIC_AMMO_SOUND;
	else
	{
		switch(RetrieveGlobalSubAmmoType(round))
		{
		default:
		case AMMOTYPE_BULLET:
				toplay = DUMP_BULLET_SOUND;
			break;
		case AMMOTYPE_SHOTGUNSHELL:
			toplay = DUMP_SHELL_SOUND;
			break;
		case AMMOTYPE_EXPLOSIVE:
				toplay = DUMP_EXPLOSIVE_SOUND;
			break;
		}
	}
	genericPlaySound(toplay, soundmode);
}

void playRoundInsert(PROUND round, bool speedload, bool speedfill, int soundmode)
{
	char* toplay;
	if(speedload){
		toplay = LOAD_SPEEDLOAD_SOUND;
		
	}
	else{
	switch(RetrieveGlobalSubAmmoType(round))
	{
	default:
	case AMMOTYPE_BULLET:
		if(speedfill)
			toplay = LOAD_FILLBULLET_SOUND;
		else
			toplay = LOAD_INSERTBULLET_SOUND;
		break;
	case AMMOTYPE_SHOTGUNSHELL:
	case AMMOTYPE_EXPLOSIVE:
		if(speedfill)
			toplay = LOAD_FILLSHELL_SOUND;
		else
			toplay = LOAD_INSERTSHELL_SOUND;
		break;
	}
	}

	genericPlaySound(toplay, soundmode);
	
}

void playWeaponSound(PWEAPON weapon, int field, int soundmode)
{
	char buf[500];
	if (weapon->soundfilename[field]){
		genericPlaySound(weapon->soundfilename[field], soundmode);
	}
	else{
		switch (field){
		case WEAPONSOUND_SHOOT:
			strcpy(buf, WEAPONSOUND_DEFAULT_SHOOT);
			break;
		case WEAPONSOUND_DROP:
			strcpy(buf, WEAPONSOUND_DEFAULT_DROP);
				break;
		case WEAPONSOUND_THROW:
			strcpy(buf, WEAPONSOUND_DEFAULT_THROW);
			break;
		case WEAPONSOUND_RELOAD:
			strcpy(buf, WEAPONSOUND_DEFAULT_RELOAD);
			break;
		case WEAPONSOUND_BREAK:
			strcpy(buf, WEAPONSOUND_DEFAULT_BREAK);
			break;
		case WEAPONSOUND_HIT:
			strcpy(buf, WEAPONSOUND_DEFAULT_HIT);
			break;
		}
		genericPlaySound(buf, soundmode);
	}
}

void playAmmoImpactSound(ROUND round, int soundmode)
{
	char* name;
	PPOTYPE mainammo;
	mainammo = ReturnListItemByNumber(round.mainammotype - 1, globals.globalweaponsammo.ammolist);
	if(!mainammo){
		return;
	}
	switch(RetrieveGlobalSubAmmoType(&round)){
	case AMMOTYPE_BULLET:
		name = mainammo->ammo.specialammo.bulletdata[round.subammotype].impactsound;
		if(name){
			genericPlaySound(name, soundmode);
		}
		else{
			genericPlaySound(IMPACTSOUND_DEFAULT_BULLET, soundmode);
		}
		return;
	case AMMOTYPE_SHOTGUNSHELL:
		name = mainammo->ammo.specialammo.shelldata[round.subammotype].impactsound;
		if(name){
			genericPlaySound(name, soundmode);
		}
		else{
			genericPlaySound(IMPACTSOUND_DEFAULT_SHELL, soundmode);
		}
		return;
	case AMMOTYPE_EXPLOSIVE:
		name = mainammo->ammo.specialammo.explosivedata[round.subammotype].impactsound;
		if(name){
			genericPlaySound(name, soundmode);
		}
		else{
			genericPlaySound(IMPACTSOUND_DEFAULT_EXPLOSIVE, soundmode);
		}
		return;
	}
}

void playRicochet(int soundmode)
{
	char buf[255];
	sprintf(buf, "sounds\\hits\\ric%d.wav", rand()%5+1);
	genericPlaySound(buf, soundmode);
}

void playWeaponSilencer(int soundmode)
{
	genericPlaySound(SILENCER_SOUND, soundmode);
}

void playWeaponFlashsuppressor(int soundmode)
{
	genericPlaySound(FLASHSUPPRESSOR_SOUND, soundmode);
}

void playWeaponLasersight(int soundmode)
{
	genericPlaySound(LASERSIGHT_SOUND, soundmode);
}

void twobeeps()
{
	genericPlaySound(TWOBEEPS_SOUND, SOUND_MODE_MIX);
}

void twobeepsmod()
{
	genericPlaySound(TWOBEEPSMOD_SOUND, SOUND_MODE_MIX);
}

void onebeepa()
{
	genericPlaySound(BEEPA_SOUND, SOUND_MODE_MIX);
}

void onebeepb()
{
	genericPlaySound(BEEPB_SOUND, SOUND_MODE_MIX);
}

void onebeepc()
{
	genericPlaySound(BEEPC_SOUND, SOUND_MODE_MIX);
}

void typeclick()
{
	genericPlaySound(TYPEWRITER_SOUND, SOUND_MODE_MIX);
}