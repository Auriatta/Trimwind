#include"ParticleSystem.h"
#include"SDLStuff.h"
#include"SoundSystem.h"

ParticleSpawn::ParticleSpawn()
{

}
ParticleSpawn::~ParticleSpawn()
{
	//for (int i = 0; i < N_SPAWNS; i++)
	//	spawner[i].~ParticleMenager();
}
int *ParticleSpawn::spawn(int x1, int y1, int x2, int y2, int timer, int h, int particleNumber, short type, SDL_Renderer* renderer, SDL_Rect *cam)
{
	for (int i = 0; i <N_SPAWNS; i++)
	{
		if (!spawner[i].isCreated())
		{
			spawner[i].set(x1, y1, x2, y2, timer, h, particleNumber, type, renderer, cam);
			spawner[i].create();
			if (type == 1 && timer == 300)
			{
				SoundMenager sm;
				sm.addSound("DirtParticle", sm.getFreeChannel(), 0);
			}
			return &spawner[i].y1;
		}
		else
			if (spawner[i].x1 == x1 && spawner[i].y1 == y1)
				return 0;
	}
}
void ParticleSpawn::update()
{
	for (int i = 0; i < N_SPAWNS; i++)
	{
		if (spawner[i].isCreated())
		{
			spawner[i].update();
		}
	}
}

ParticleSpawn::ParticleMenager::ParticleMenager()
{

}
ParticleSpawn::ParticleMenager::~ParticleMenager()
{
	renderer = nullptr;
	cam = nullptr;
	for (int i = 0; i < MAXPARTICLES; i++)
		SDL_DestroyTexture(particle[i].texture);
}
void ParticleSpawn::ParticleMenager::set(int x1, int y1, int x2, int y2, int timer, int range, int particleNumber, short type, SDL_Renderer* renderer, SDL_Rect *cam)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	this->timeToEnd = timer;
	this->range = range;
	this->type = type;
	this->renderer = renderer;
	this->cam = cam;
	n_prtc = particleNumber;
	counterTime = SDL_GetTicks();
	
}
void ParticleSpawn::ParticleMenager::create()
{
	switch (type)
	{
	case 1:// dirt
	{
		for (int i = 0; i < n_prtc; i++)
			particle[i].texture = LoadTextureRW(SDL_RWFromFile("graphics/particle2.png", "rb"), renderer);
		break;
	}
	case 2:// ressurection
	{
		for (int i = 0; i < n_prtc; i++)
			particle[i].texture = LoadTextureRW(SDL_RWFromFile("graphics/particle3.png", "rb"), renderer);
		break;
	}
	case 3:// tundra background
	{
		for (int i = 0; i < n_prtc; i++)
			particle[i].texture = LoadTextureRW(SDL_RWFromFile("graphics/particle1.png", "rb"), renderer);
		break;
	}
	case 4: // smoke
	{
		for (int i = 0; i < n_prtc; i++)
			particle[i].texture = LoadTextureRW(SDL_RWFromFile("graphics/particle4.png", "rb"), renderer);
		break;
	}
	case 5: // irlicht
	{
		for (int i = 0; i < n_prtc; i++)
			particle[i].texture = LoadTextureRW(SDL_RWFromFile("graphics/particle5.png", "rb"), renderer);
		break;
	}
	case 6: // barrier
	{
		for (int i = 0; i < n_prtc; i++)
			particle[i].texture = LoadTextureRW(SDL_RWFromFile("graphics/particle6.png", "rb"), renderer);
		break;
	}
	case 7: // water drop
	{
		for (int i = 0; i < n_prtc; i++)
			particle[i].texture = LoadTextureRW(SDL_RWFromFile("graphics/particle7.png", "rb"), renderer);
		break;
	}
	case 8: // glimbugs
	{
		for (int i = 0; i < n_prtc; i++)
			particle[i].texture = LoadTextureRW(SDL_RWFromFile("graphics/particle8.png", "rb"), renderer);
		break;
	}
	default:
		break;
	}
}
void ParticleSpawn::ParticleMenager::update()
{
	if (SDL_GetTicks() - counterTime>= timeToEnd && timeToEnd != 0 && timeToEnd != -1)
	{
		counterTime = 0;
		timeToEnd = 0;
	}

	for (int i = 0; i < n_prtc; i++)
	{
		if (particle[i].elapsed < 1 && particle[i].alpha != 0) // znika gdy czas sie skonczy
		{
			timeOut(i);
		} 
		if (particle[i].elapsed < 1) // jesli martwy
		{
			dead(i);
		}
		else // jesli zywy
		{
			alive(i);
		}
	}
	if (timeToEnd == 0 && counterTime > n_prtc)
		destroy();
}
void ParticleSpawn::ParticleMenager::destroy()
{
	for (int i = 0; i < n_prtc; i++)
	{
		SDL_DestroyTexture(particle[i].texture);
		particle[i].texture = NULL;
		particle[i].alpha = 0;
		particle[i].elapsed = 0;
		particle[i].life = 0;
		particle[i].position = {};
		particle[i].size = 0;
		particle[i].speed = 0;
	}
	n_prtc = 0;
	range = 0;
	type = 0;
	timeToEnd = 0;
	counterTime = 0;
}
bool ParticleSpawn::ParticleMenager::isCreated()
{
	if (n_prtc != NULL)
		return true;
	else
		return false;
}

void ParticleSpawn::ParticleMenager::timeOut(int i)
{
	if (particle[i].position.x < cam->x + cam->w && particle[i].position.x > cam->x &&
		particle[i].position.y < cam->y + cam->h && particle[i].position.y > cam->y)
	{
		if (type == 3 || type == 8)
		{
			if (particle[i].alpha > 0)
			{
				if (type == 3)
					particle[i].alpha -= 3;
				else
					particle[i].alpha -= 1;
			}
			else
			{
				particle[i].alpha = 0;
			}
			if (type == 8)
			{
				randir = (rand() % 7) + 0;
				if (randir < 4)
					particle[i].position.y += particle[i].speed;
				else
					particle[i].position.y -= particle[i].speed;
				randir = (rand() % 7) + 0;
				if (randir < 4)
					particle[i].position.x += particle[i].speed;
				else
					particle[i].position.x -= particle[i].speed;
			}
			SDL_SetTextureAlphaMod(particle[i].texture, particle[i].alpha);
			drawingPos.x = particle[i].position.x - cam->x;
			drawingPos.y = particle[i].position.y - cam->y;
			drawingPos.w = drawingPos.h = particle[i].position.w;
			SDL_RenderCopy(renderer, particle[i].texture, NULL, &drawingPos);
		}
		else
			particle[i].alpha = 0;
			
	}
	else
	{
		particle[i].alpha = 0;
	}
}
void ParticleSpawn::ParticleMenager::dead(int i)
{
	if (particle[i].alpha == 0)
	{
		if (timeToEnd != 0)
		{
			if (type == 1 || type == 2 || type == 4 || type == 5 || type == 6 || type == 7)
			{
				particle[i].position.y = y1;
				particle[i].position.x = (rand() % abs(range)) + x1;

				if (type == 5)
				{
					particle[i].direction = 2 * M_PI * rand() / RAND_MAX;
				}

				if (type == 4)
					particle[i].speed = 1;
				else
					if (type == 6)
						particle[i].speed = (rand() % 4) + 3;
				else
					particle[i].speed = (rand() % 2) + 1;

				particle[i].alpha = 255;
				SDL_SetTextureAlphaMod(particle[i].texture, particle[i].alpha);
				switch (type)
				{
				case 1:
				{
					particle[i].position.w = particle[i].position.h = (rand() % 6) + 3;
					particle[i].life = (rand() % 30) + 10;
					break;
				}
				case 4:
				{
					particle[i].position.w = particle[i].position.h = (rand() % 4) + 2;
					particle[i].life = (rand() % 40) + 20;
					break;
				}
				case 5:
				{
					particle[i].position.w = particle[i].position.h = (rand() % 7) + 4;
					particle[i].life = (rand() % 30) + 10;
					break;
				}
				case 6:
				{
					particle[i].position.w = 20;
					particle[i].position.h = 50;
					particle[i].life = (rand() % 120) + 110;
					break;
				}
				case 7:
				{
					particle[i].position.y = y1;
					particle[i].position.x = x1;
					particle[i].speed = 4;
					particle[i].position.w = particle[i].position.h = (rand() % 6) + 4;
					particle[i].life = 200;
					break;
				}
				default:
				{
					particle[i].position.w = particle[i].position.h = (rand() % 10) + 5;
					particle[i].life = (rand() % 25) + 11;
					break;
				}
					
				}
			}
			else
				if (type == 3)
				{
					particle[i].position.y = (rand() % abs(cam->h)) + cam->y;
					particle[i].position.x = (rand() % abs(cam->w)) + cam->x;
					particle[i].position.w = particle[i].position.h = (rand() % 6) + 3;
					particle[i].life = (rand() % 131) + 40;
				}
				else
					if (type == 8)
					{
						particle[i].position.y = (rand() % abs(cam->h)) + cam->y;
						particle[i].position.x = (rand() % abs(cam->w)) + cam->x;
						particle[i].life = (rand() % 200) + 100;
						particle[i].position.w = particle[i].position.h = (rand() % 7) + 4;
						particle[i].speed = 1;
					}

			particle[i].elapsed = particle[i].life;

		}
		else
			counterTime += 1;
	}
}
void ParticleSpawn::ParticleMenager::alive(int i)
{
	if (timeToEnd == 0)
		counterTime = 0;
	particle[i].elapsed -= 1;

	switch (type)
	{
	case 1:
	{
		particle[i].position.y += particle[i].speed;
		break;
	}
	case 2:
	{
		particle[i].position.y -= particle[i].speed;
		break;
	}
	case 3:
	{
		if (particle[i].position.x > cam->x + cam->w || particle[i].position.x < cam->x ||
			particle[i].position.y > cam->y + cam->h || particle[i].position.y < cam->y)
		{
			particle[i].position.y = (rand() % abs(cam->h)) + cam->y;
			particle[i].position.x = (rand() % abs(cam->w)) + cam->x;
			particle[i].life = (rand() % 131) + 40;
			particle[i].position.w = particle[i].position.h = (rand() % 6) + 3;
			particle[i].elapsed = particle[i].life;
		}

		if (particle[i].alpha != 255)
		{
			particle[i].alpha += 3;
			if (particle[i].alpha > 255)
				particle[i].alpha = 255;
			SDL_SetTextureAlphaMod(particle[i].texture, particle[i].alpha);
		}
		break;
	}
	case 4:
	{
		particle[i].size += 0.5;
		particle[i].position.y -= particle[i].speed;
		if (particle[i].position.h < 20 && particle[i].size >= 1)
		{

			particle[i].position.w = particle[i].position.h += 1;
			particle[i].size = 0;
		}
		break;
	}
	case 5:
	{
		particle[i].position.x += round((double)particle[i].speed *cos(particle[i].direction));
		particle[i].position.y += round((double)particle[i].speed *sin(particle[i].direction));
		break;
	}
	case 6:
	{
		particle[i].position.y -= particle[i].speed;
		break;
	}
	case 7:
	{
		if (particle[i].position.y< y1+range)
			particle[i].position.y += particle[i].speed;
		break;
	}
	case 8:
	{
		randir = (rand() % 7) + 0;
		if (randir < 4)
			particle[i].position.y += (rand() % particle[i].speed + 1) + 0;
		else
			particle[i].position.y -= (rand() % particle[i].speed + 1) + 0;
		randir = (rand() % 7) + 0;
		if (randir < 4)
			particle[i].position.x += (rand() % particle[i].speed + 1) + 0;
		else
			particle[i].position.x -= (rand() % particle[i].speed + 1) + 0;

		if (particle[i].position.x > cam->x + cam->w || particle[i].position.x < cam->x ||
			particle[i].position.y > cam->y + cam->h || particle[i].position.y < cam->y)
		{
			particle[i].position.y = (rand() % abs(cam->h)) + cam->y;
			particle[i].position.x = (rand() % abs(cam->w)) + cam->x;
			particle[i].life = (rand() % 200) + 100;
			particle[i].position.w = particle[i].position.h = (rand() % 7) + 4;
			particle[i].elapsed = particle[i].life;
		}

		if (particle[i].alpha != 255)
		{
			particle[i].alpha += 1;
			if (particle[i].alpha > 255)
				particle[i].alpha = 255;
			SDL_SetTextureAlphaMod(particle[i].texture, particle[i].alpha);
		}

		break;
	}
	default:
		break;
	}

	drawingPos.x = particle[i].position.x - cam->x;
	drawingPos.y = particle[i].position.y - cam->y;
	drawingPos.w = drawingPos.h = particle[i].position.w;
	SDL_RenderCopy(renderer, particle[i].texture, NULL, &drawingPos);
}