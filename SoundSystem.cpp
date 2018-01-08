#include"SoundSystem.h"


#pragma region SoundMenager
SoundMenager::SoundMenager()
{
	
}
SoundMenager::~SoundMenager()
{

}
short SoundMenager::getActualAmbient()
{
	return sound_data.ambient_switch;
}
void SoundMenager::erase()
{
	for (int i = 0; i < MAXWHITELIST; i++)
	{
		for (int j = 0; j < 3; j++)
			sound_data.white_list[i][j] = -1;
	}
	for (int i = 0; i < MAXCHANNELS; i++)
		sound_data.occ_channels[i] = CHANNEL_FREE;
	for (int i = 0; i < MAXSOUNDSFX; i++)
	{
		if (sound_data.soundFX[i] != NULL)
		{
			Mix_FreeChunk(sound_data.soundFX[i]);
			sound_data.soundFX[i] = NULL;
		}
	}
	Mix_FreeMusic(sound_data.ambient[0]);
	sound_data.ambient[0] = NULL;
	Mix_FreeMusic(sound_data.ambient[1]);
	sound_data.ambient[1] = NULL;
}
void SoundMenager::restartSoundSystem()
{
	sound_data.restartSoundSystem = true;
}
int SoundMenager::getIndexFromName(string filename)
{
	#pragma region SoundFX
	if (filename == "LevelIntro")
		return 0;
	if (filename == "Jump")
		return 1;
	if (filename == "Run")
		return 2;
	if (filename == "RuneStone")
		return 3;
	if (filename == "Platform")
		return 4;
	if (filename == "DirtParticle")
		return 5;
	if (filename == "Ressurection")
		return 6;
	if (filename == "StoneHit1")
		return 7;
	if (filename == "StoneHit2")
		return 8;
	if (filename == "CyclonOpen")
		return 9;
	if (filename == "Cyclon")
		return 10;
	if (filename == "CyclonClose")
		return 11;
	if (filename == "Crow1")
		return 12;
	if (filename == "FlameActive")
		return 13;
	if (filename == "FlameSound")
		return 14;
	if (filename == "RuneStoneUnlock")
		return 15;
	if (filename == "IrlichtCatch")
		return 16;
	if (filename == "Bird")
		return 17;
	if (filename == "Bush1")
		return 18;
	if (filename == "Bush2")
		return 19;
	if (filename == "Bush3")
		return 20;
	if (filename == "Irlicht")
		return 21;
	if (filename == "Barrier")
		return 22;
	if (filename == "Outro")
		return 23;
	if (filename == "Hunt")
		return 24;
	if (filename == "HuntEnd")
		return 25;
	if (filename == "UIChoice")
		return 26;
	if (filename == "UIClick")
		return 27;
	if (filename == "IrlichtDeath")
		return 28;
	#pragma endregion

	#pragma region MainAmbients
	if (filename == "Theme" || filename == "Wind" || filename == "Swamp")
		return 0;
	#pragma endregion

	#pragma region SecondAmbients
	if (filename == "Cave")
		return 1;
	#pragma endregion

	return -1;
}
void SoundMenager::loadSound(string filename, bool type)
{
	int index = getIndexFromName(filename);
	
	filename = "sfx/" + filename + ".wav";
	if (type == 0)
	{
		if (sound_data.soundFX[index] == NULL)
		{
			sound_data.soundFX[index] = Mix_LoadWAV(filename.c_str());
		}
	}
	else
	{
		////if (sound_data.ambient[index] != NULL)
		//{
		///	Mix_FreeMusic(sound_data.ambient[index]);
		//	sound_data.ambient[index] = NULL;
		//}
		sound_data.ambient[index] = Mix_LoadMUS(filename.c_str());
	}
	soundVolAdj(index);
}
void SoundMenager::endAmbient()
{
	sound_data.music_fadeout = true;
}
void SoundMenager::changeAmbient(short type)
{
	if (type < 2)
		sound_data.ambient_switch = type;
}
void SoundMenager::changeSoundPos(int channel, int objpoz)
{
	//nope
}
void SoundMenager::addSound(string filename, int channel, int reapets)
{
	if (sound_data.occ_channels[channel] == CHANNEL_FREE && channel > -1)
	{
		for (int i = 0; i < MAXWHITELIST; i++)
		{
			if (sound_data.white_list[i][0] == -1)
			{
				sound_data.white_list[i][0] = getIndexFromName(filename);
				sound_data.white_list[i][1] = channel;
				sound_data.white_list[i][2] = reapets;
				sound_data.occ_channels[channel] = CHANNEL_RESERVED;
				break;
			}
		}
	}
}
void SoundMenager::stopSound(int channel)
{ 
	if (channel < MAXCHANNELS && sound_data.occ_channels[channel] == CHANNEL_BUSY&&
		sound_data.occ_channels[channel] != CHANNEL_FREE && channel > -1)
		sound_data.occ_channels[channel] = CHANNEL_WAITING;
}
void SoundMenager::pauseSound(int channel)
{
	Mix_Pause(channel);
}
void SoundMenager::resumeSound(int channel)
{
	Mix_Resume(channel);
}
int SoundMenager::getFreeChannel()
{
	for (int i = 7; i < MAXCHANNELS; i++)
	{
		if (sound_data.occ_channels[i] == CHANNEL_FREE)
			return i;
	}
	return -1;
}
void SoundMenager::soundVolAdj(int index)
{
	if (sound_data.soundFX[index] != NULL)
	{
		if (index == 2)
			Mix_VolumeChunk(sound_data.soundFX[index], 20);
		if (index == 6)
			Mix_VolumeChunk(sound_data.soundFX[index], 80);
		if (index == 1)
			Mix_VolumeChunk(sound_data.soundFX[index], 30);
		if (index == 3)
			Mix_VolumeChunk(sound_data.soundFX[index], 70);
		if (index == 4)
			Mix_VolumeChunk(sound_data.soundFX[index], 70);
		if (index == 9)
			Mix_VolumeChunk(sound_data.soundFX[index], 50);
		if (index == 10)
			Mix_VolumeChunk(sound_data.soundFX[index], 50);
		if (index == 11)
			Mix_VolumeChunk(sound_data.soundFX[index], 50);
		if (index == 5)
			Mix_VolumeChunk(sound_data.soundFX[index], 5);
		if (index == 17)
			Mix_VolumeChunk(sound_data.soundFX[index], 60);
		if (index == 18)
			Mix_VolumeChunk(sound_data.soundFX[index], 80);
		if (index == 19)
			Mix_VolumeChunk(sound_data.soundFX[index], 80);
		if (index == 20)
			Mix_VolumeChunk(sound_data.soundFX[index], 40);
		if (index == 23)
			Mix_VolumeChunk(sound_data.soundFX[index], 80);
		if (index == 24)
			Mix_VolumeChunk(sound_data.soundFX[index], 80);
		if (index == 25)
			Mix_VolumeChunk(sound_data.soundFX[index], 80);
		if (index == 28)
			Mix_VolumeChunk(sound_data.soundFX[index], 20);
	}
}
#pragma endregion

#pragma region SoundSystem
SoundSystem::SoundSystem()
{
	for (int i = 0; i < MAXCHANNELS; i++)
		sound_data.occ_channels[i] = CHANNEL_FREE;
	Mix_AllocateChannels(MAXCHANNELS);
	Mix_Volume(-1, st.getSoundLevel());
	Mix_VolumeMusic(st.getSoundLevel());
	ambientFade = 0;
	actualAmbient = 1;
}
SoundSystem::~SoundSystem()
{
	Mix_HaltChannel(-1);
	Mix_HaltMusic();
	Mix_CloseAudio();
	for (int i = 0; i < MAXSOUNDSFX; i++)
	{
		Mix_FreeChunk(sound_data.soundFX[i]);
		sound_data.soundFX[i] = NULL;
	}
	Mix_FreeMusic(sound_data.ambient[0]);
	sound_data.ambient[0] = NULL;
	Mix_FreeMusic(sound_data.ambient[1]);
	sound_data.ambient[1] = NULL;
}
void SoundSystem::menage()
{
	while (true)
	{
		Mix_HaltChannel(-1);
		Mix_HaltMusic();
		Mix_Volume(-1, st.getSoundLevel());
		Mix_VolumeMusic(st.getSoundLevel());
		for (int i = 0; i < MAXWHITELIST; i++)
		{
			for (int j = 0; j < 3; j++)
				sound_data.white_list[i][j] = -1;
		}
		sound_data.restartSoundSystem = 0;

		while (true)
		{
			if (sound_data.restartSoundSystem == true)
				break;
			if (sound_data.ambient_switch == 1 && actualAmbient != 1 ||
				sound_data.ambient_switch == 0 && actualAmbient != 0 ||
				sound_data.ambient_switch == -1)
			{
				if (sound_data.ambient_switch == -1)
					sound_data.ambient_switch = 0;
				if (Mix_PlayingMusic() == 1)
				{
					if (st.getSoundLevel() > 1)
						Mix_FadeOutMusic(200);
				}
				actualAmbient = sound_data.ambient_switch;
				if (st.getSoundLevel() > 1)
				{
					Mix_FadeInMusic(sound_data.ambient[actualAmbient], -1, 1000);
				}
			}
			if (sound_data.music_fadeout == true && ambientFade == false)
			{
				if (st.getSoundLevel() > 1)
					Mix_FadeOutMusic(1000);
				ambientFade = true;
			}
			else
				if (sound_data.music_fadeout == true && ambientFade == true)
				{
					if (Mix_PlayingMusic() == 0)
					{
						ambientFade = false;
						sound_data.music_fadeout = false;
					}
				}
			checkWhiteList();
			checkChannels();
		}
	}
}
void SoundSystem::checkChannels()
{
	for (int i = 0; i < MAXCHANNELS; i++)
	{
		if (sound_data.occ_channels[i] == CHANNEL_BUSY && Mix_Playing(i) == 0 ||
			sound_data.occ_channels[i] == CHANNEL_WAITING)
		{
			//if (sound_data.occ_channels[i] == CHANNEL_WAITING)
			Mix_HaltChannel(i);
			//Mix_Volume(i, st.getSoundLevel());
			sound_data.occ_channels[i] = CHANNEL_FREE;
			//cout << "Kanal " << i << " uwolniony"<<endl;
		}
	}
}
void SoundSystem::checkWhiteList()
{
	for (int i = 0; i < MAXWHITELIST; i++)
	{
		if (sound_data.white_list[i][0] != -1)
		{
			if (st.getSoundLevel() > 1)
			{
				sound_data.occ_channels[sound_data.white_list[i][1]] = CHANNEL_BUSY;
				Mix_PlayChannel(sound_data.white_list[i][1], sound_data.soundFX[sound_data.white_list[i][0]], sound_data.white_list[i][2]);
			}
			//cout << "Kanal " << sound_data.white_list[i][1] << " zostal zajety" << endl;
			sound_data.white_list[i][0] = -1;
			sound_data.white_list[i][1] = -1;
			sound_data.white_list[i][2] = -1;
			

		}
	}
}
#pragma endregion