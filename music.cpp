#include "agencyinclude.h"
#include "fmod.h"
#include <process.h>

FSOUND_STREAM* g_CurrentSong;
long g_MixSoundChannels = 32;
long g_MusicSoundChannel = 0;
long g_StartMixSoundChannel = 1;
long g_CurrentMixSoundChannel = g_StartMixSoundChannel;

bool initmusic()
{
//	SECURITY_ATTRIBUTES secatt;
	int SplitToWaitForThread = 5000;
	int SplitElapse = 0;
	TIMER Time;
	
	assert(loadmusiclist());
	globals.musicthread = (HANDLE)_beginthread(&MusicProc, 0, NULL);
	if((int)globals.musicthread == -1){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		StaticDialogBox("Music Interface Failed", "It appears that your current configuration does not support the DSA Music Interface. Please attempt to run this program in the \"No Music\" mode through the start menu or by running this program with the -m switch. Press ENTER to Terminate This Program.", "Cannot Initialize Thread", &globals.dboxparams);
		return false;
	}
	globals.musicthreadid = MUSICTHREADID;
	globals.musicloopstatus = FALSE;
	SendMusicMessage(MUSIC_INIT, NULL);
	while(globals.musicthreadopen == FALSE)
	{
		//DO NOTHING UNTIL THE MUSIC THREAD IS READY!
		StartTimer(&Time);
		Sleep(1);
		EndTimer(&Time);
		SplitElapse += SplitElapsed(&Time);
		if(SplitElapse > SplitToWaitForThread){
			SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
			StaticDialogBox("Sound/Music Interface Failed", "It appears that your current configuration does not support the DSA Music Interface. Please attempt to run this program in the \"No Music\" mode through the start menu or by running this program with the -m switch. Press ENTER to Terminate This Program.", "Timed Out For MusicThread", &globals.dboxparams);
			return false;
		}
	}
	if(globals.musicthreadopen == -1){
		SetGlobalDBoxParam(DB_SCHEME_REDANDYELLOW, DB_MEDIUM, 1, FALSE);
		StaticDialogBox("Sound/Music Interface Failed", "It appears that your current configuration does not support the DSA Music Interface. Please attempt to run this program in the \"No Music\" mode through the start menu or by running this program with the -m switch. Press ENTER to Terminate This Program.", "Cannot Create New Music Player", &globals.dboxparams);
		return false;
	}
	return true;
}

bool loadmusiclist()
{
	char buffer[255];
	FILE* fp = fopen(MUSICLISTFILE, "r");

	if (!fp){
		return false;
	}
	

	for(int i = 0; i < MAXSONGS; i++){
		if (feof(fp)){
			fclose(fp);
			return false;
		}
		fgets(buffer, 255, fp);
		for(int k = 0; k < 255; k++){
			if (buffer[k] == '\n'){
				buffer[k] = '\0';
				break;
			}
	}
		strcpy(globals.musiclist.songs[i], buffer);
	
}
	fclose(fp);
	return true;
}

void SendMusicMessage(int message, char* song)
{
	globals.musicsong = song;
	globals.musicmessagewaiting = TRUE;
	globals.musicmessage = message;
}

void PlaySong(char* song)
{
	ResumeThread(globals.musicthread);
	SendMusicMessage(MUSIC_PLAY, song);
	globals.musicloopstatus = FALSE;
}

void playMIXsound(char* sound)
{
	FSOUND_SAMPLE * snd = NULL;
	snd = FSOUND_Sample_Load(FSOUND_FREE,
		sound,
		FSOUND_2D,
		0);
	if(snd)
	{
		int retval = FSOUND_PlaySound(g_CurrentMixSoundChannel, snd);
		if(retval != -1)
		{
			if((++g_CurrentMixSoundChannel) >= g_MixSoundChannels)
				g_CurrentMixSoundChannel = g_StartMixSoundChannel;
		}
	}
}


void LoopSong(char* song)
{

	ResumeThread(globals.musicthread);
	SendMusicMessage(MUSIC_LOOP, song);
	globals.musicloopstatus = TRUE;
}

void StopSong()
{
	ResumeThread(globals.musicthread);
	SendMusicMessage(MUSIC_STOP, NULL);
	globals.musicloopstatus = FALSE;
	
}

void terminatemusic()
{
	ResumeThread(globals.musicthread);
	SendMusicMessage(MUSIC_TERMINATE, NULL);
	globals.musicloopstatus = FALSE;
}


//--------------------------------------------------------------------------------------------
// Function Name - MusicProc
//
// Description   - This is the main music ThreadProc.
//
// Return Type   - DWORD WINAPI 
//
// Arguments     - void* MM ( Uh...ignore this. THX. )
//
// Author        - Charles Cox
// Date Modified - 08/06/2000
//--------------------------------------------------------------------------------------------
void MusicProc(void* MM)
{
	while(1)
	{
		if (globals.musicmessagewaiting)
		{
			globals.musicmessagewaiting = FALSE;
			
			switch(globals.musicmessage)
			{
				
			case MUSIC_TERMINATE:
				if(g_CurrentSong)
				{
					FSOUND_Stream_Stop(g_CurrentSong);
					FSOUND_Stream_Close(g_CurrentSong);
					g_CurrentSong = NULL;
				}
				FSOUND_Close();
				globals.musicthreadopen = FALSE;
				//ExitThread(1);
				_endthread();
				break;
				
			case MUSIC_LOOP: //STILL NEED TO BE ABLE TO SUSPEND UNTIL THE VERY LAST SECOND
				if(globals.options.musicon)
				{
					g_CurrentSong = FSOUND_Stream_OpenFile(globals.musicsong,
						FSOUND_LOOP_NORMAL | FSOUND_2D,
						0);
					if(g_CurrentSong)
					{
						FSOUND_Stream_Play(g_MusicSoundChannel,g_CurrentSong);
						SuspendThread(globals.musicthread);
					}
				}
				break;
				
			case MUSIC_INIT:
				g_CurrentSong = NULL;
				if(FSOUND_Init(44100, g_MixSoundChannels, 0))
					globals.musicthreadopen = true;
				SuspendThread(globals.musicthread);
				break;
				
			case MUSIC_PLAY:
				if(globals.options.musicon)
				{
					g_CurrentSong = FSOUND_Stream_OpenFile(globals.musicsong,
						FSOUND_LOOP_OFF | FSOUND_2D,
						0);
					if(g_CurrentSong)
					{
						FSOUND_Stream_Play(g_MusicSoundChannel,g_CurrentSong);
						SuspendThread(globals.musicthread);
					}
				}
				break;
			case MUSIC_STOP:
				if(g_CurrentSong)
				{
					FSOUND_Stream_Stop(g_CurrentSong);
					FSOUND_Stream_Close(g_CurrentSong);
					g_CurrentSong = NULL;
				}
				globals.musicloopstatus = FALSE;
				SuspendThread(globals.musicthread);
				break;
			}
			
		}
		
	}
	
	return;	
}
