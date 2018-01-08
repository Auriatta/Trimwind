#ifndef SOUND_H
#define SOUND_H

#include "Include.h"
#include"Settings.h"

#define MAXSOUNDSFX 29
#define MAXWHITELIST 6
#define MAXCHANNELS 20

#define CHANNEL_FREE 0
#define CHANNEL_BUSY 1
#define CHANNEL_WAITING 2
#define CHANNEL_RESERVED 3

/*
Ambient uruchamiasz funkcj¹ changeambient
Reszte dzwieków funkcj¹ addSound
*/

struct sound
{
	Mix_Music *ambient[2]; // [0] zawsze jest aktualnym ambientem
	Mix_Chunk *soundFX[MAXSOUNDSFX];
	int white_list[MAXWHITELIST][3]; // kolejka soundFX. [0] Index dŸwiêku [1] kana³ [2] iloœæ powtórzeñ (-1) nieskonczona
	short occ_channels[MAXCHANNELS]; // zajêty kana³ 0, wolny 1, oczekuje 2
	bool music_fadeout = 0; // zamyka ambient w sposób naturalny
	short ambient_switch = 0; // prze³¹cza miêdzy 0 g³ównym a 1 dodatkowym ambientem -1 uruchom
	bool restartSoundSystem = 0;
	bool shotdownThread = 0;
};

static sound sound_data;

class SoundMenager
{
private:
	int getIndexFromName(string filename);
	void soundVolAdj(int index);
	Uint8 leftVol;
	Uint8 rightVol;
	int procent;

public:
	SoundMenager();
	~SoundMenager();

	void restartSoundSystem();
	void erase();
	void loadSound(string filename, bool type); // type to 0 sound 1 ambient filename bez formatu
	void changeAmbient(short type);
	short getActualAmbient();
	void endAmbient();
	void changeSoundPos(int channel, int ObjPoz); // pozycja na ekranie, nie w swiecie gry!
	void stopSound(int channel);
	void pauseSound(int channel);
	void resumeSound(int channel);
	void addSound(string filename, int channel, int reapets);
	int getFreeChannel();
};


class SoundSystem
{
private:
	void checkChannels();
	void checkWhiteList();
	bool actualAmbient;
	SettingsControl st;
	bool ambientFade;

public:
	SoundSystem();
	~SoundSystem();
	void menage();
	
};

#endif