#include "Include.h"
#include "FPSControl.h"
#include "Level.h"
#include"PhysicEngine.h"
#include "Events.h"
#include "Camera.h"
#include "Effects.h"
#include "Resolution.h"
#include "Irlicht.h"
#include "AI.h"
#include "Interface.h"

class Game
{
	short done = 0;
	bool endLevel = 0;
	bool pause = 0;
	bool firstRun = true;
	int deley = 20;
	Uint8 alpha;
	short migniecie;
	int lvlid;
	SDL_Event event;
	level lvl;
	Scrain* scrain;
	Camera* cam;
	int n_fps;
	SDL_Renderer* RenderTarget;
	SDL_Rect layRect;
	Objects* objects;
	Effects* effects;
	Irlicht* irlicht;
	Interface* g_interface;
	SDL_Texture * target_texture;
	SoundMenager sm;

	bool checkLevelState(SettingsControl st);
public:
	Game();
	~Game();

	int startLoop(SDL_Window* window);
	void render();
};