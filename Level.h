#include"Include.h"
#include"Object.h"
#include "ParticleSystem.h"

class level
{
	SDL_Texture* bg_level;
	SDL_Texture* blay_level;
	SDL_Texture* base_level[6];
	int n_baseParts = 0;
	SDL_Rect base_worldPos[6];
	SDL_Texture* lay_level;

	SDL_Rect bg_drawPos[2]; // pozycja animacji
	SDL_Rect bg_worldPos[2]; // pozycja w swiecie
	int bgOriginW[2];
	int bgOriginH;
	SDL_Rect prevcam[3]; // 0 bg 1 blay 3 base

	void passToLeft();
	void passToRight();
	ParticleSpawn particleSpawnMenager;
	long * spawnPos = new long[4]; // {type, x, y}
	int n_enemySpawn;
	int tempColPos[3]; // {x,y}
	long * spawnPosEnemyX = new long[1];
	long * spawnPosEnemyY = new long[1];
	Point spawnPosPlayer;

	friend class Game;
public:
	
	SizeRect baseSize;
	SizeRect bgSize;

	level();
	~level();
	int LoadNewLevel(SDL_Window* window, SDL_Renderer* renderer, Objects* objects);
	void RenderBGEffects();
	void LoadBGEffects(SDL_Renderer* renderer, SDL_Rect *camera, int type);
	void RenderBaseLevel(SDL_Renderer* renderer, SDL_Rect * camera);
	void RenderBGLevel(SDL_Renderer* renderer, SDL_Rect *camera);
};

