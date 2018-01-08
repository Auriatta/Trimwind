#ifndef EVENTS_H
#define EVENTS_H

#include"Include.h"
#include "Scrain.h"
#include "Effects.h"
#include "Settings.h"
#include "SoundSystem.h"

/* SCRAIN ANIM NUMBER MEANING:
0 idle left | 11 idle right | 33 preaper to left idle | 44 preaper to right idle
1 left run | 2 right run
301 left idle jump | 302 right idle jump | 311 left direct jump | 312 right direct jump */

#define RUNCHANNEL 1
#define JUMPCHANNEL 2

class Events
{
	Effects* effects;
	bool* ishplane;
	SDL_Event* Event;
	short e_except;
	int l_count; // loop counter
	short sideKeyStat = 0;
	SettingsControl st;
	SoundMenager sm;
	short runSound;

public:
	Events(SDL_Event* eventt);
	~Events();
	bool jumpDelay = 0;
	int t_jDalay = 0;

	void scrain(Scrain* scrain);


	void PushButton(int key);
	short GetSideKeyStatus();
	void ResetSideEffect();
	void ResetSideKeyStatus();
	SDL_Event* GetEventClass();
	void SetException(short event_code);
	int GetException();
	void loadPointers(Effects* effects, bool* ishplane);
};

#endif