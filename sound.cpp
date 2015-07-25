#include "agencyinclude.h"



void playASYNCsound(char* sound)
{
	if (sound != NULL)
	PlaySound(sound, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
}

void playSYNCsound(char* sound)
{
	if (sound != NULL)
	PlaySound(sound, NULL, SND_FILENAME | SND_SYNC | SND_NODEFAULT);
}
