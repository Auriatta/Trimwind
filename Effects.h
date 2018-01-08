#ifndef EFFECTS_H
#define EFFECTS_H
#include "Include.h"
#include "ParticleSystem.h"
class Effects
{
	SDL_Renderer* renderer;
	SDL_Rect* cam;

	class Effect
	{
		int rendSpeed;
		SDL_Rect* scrainPoz;
		SDL_Rect setting;
		SDL_Rect* trackPoz;
		short target;
		SDL_Texture* texture;
		int frameW, frameH;
		int textureW, textureH;
		SDL_Rect animRect;
		
	public:
		Effect();
		~Effect();
		bool isLoad;
		bool rem_timer;

		void load(int w, int h, SDL_Rect* TrackPoz, SDL_RWops *file, SDL_Renderer* renderer, int num_animX, int num_animY);
		void runAnim();
		void setTarget(short v);
		void loadPozFromTarget();
		void remove(bool timer);

		void render(SDL_Renderer* renderer, SDL_Rect* cam);
		void loadPointers(SDL_Rect* ScrainPoz);
	};
	Effect curse;
	ParticleSpawn particleSpawnMenager;
public:
	Effects(SDL_Renderer* rend, SDL_Rect* scrainPoz, SDL_Rect* camr);
	~Effects();
	
	void loadEffect(int index, short target, SDL_Rect* poz); // index = id 1 = curse | target 1 = scrain | podaj lokalizacje jesli nie podano target
	void renderEffects();
	void removeEffect(int index, bool runTimer); // 1 = curse runTimer 1 = poczekaj az skonczy sie animacja 0 = usun odrazu
	bool isRun(int index);
	bool isTimerRun(int index);
};

#endif