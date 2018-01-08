#ifndef PARTICLE_H
#define PARTICLE_H

#include"Include.h"

#define MAXPARTICLES 50

class ParticleSpawn
{
	#define N_SPAWNS 5
	class ParticleMenager
	{
		struct particleData
		{
			double direction = 0;
			int speed = 0;
			int life = 0;
			int elapsed = 0;
			Uint8 alpha = 0;
			SDL_Texture* texture = NULL;
			float size = 0;
			SDL_Rect position = {};
		}particle[MAXPARTICLES];
		SDL_Rect drawingPos;
		int counterTime;
		int n_prtc;
		int range;
		short type;
		int timeToEnd;
		SDL_Renderer* renderer;
		SDL_Rect* cam;
		int y1;
		int x1, x2, y2;
		short randir;
		friend class ParticleSpawn;
	public:
		ParticleMenager();
		~ParticleMenager();

		bool isCreated();
		void set(int x1, int y1, int x2, int y2, int timer, int range, int particleNumber, short type, SDL_Renderer* renderer, SDL_Rect *cam);

		void create();
		void update();
		void destroy();

		void dead(int i);
		void timeOut(int i);
		void alive(int i);
	}spawner[N_SPAWNS];


public:
	ParticleSpawn();
	~ParticleSpawn();
	int *spawn(int x1, int y1, int x2, int y2, int timer, int range, int particleNumber, short type, SDL_Renderer* renderer, SDL_Rect *cam);
	void update();

};
#endif